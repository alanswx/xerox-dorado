// Simulation-side block medium and the same isolated Pup service as the ARM
// daemon. No Dorado CPU/memory emulator or guest IOCB completion is linked.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <deque>
#include <algorithm>
extern "C" {
#include "ethernet.h"
int dorado_trace_gate=1,dorado_trace_env_present=1;
unsigned long long dorado_trace_cycle=0;
int dorado_trace_flag_lookup(const char *name){return getenv(name)!=nullptr;}
void dorado_trace_init(void){}
}
static dorado_ethernet eth;
static std::vector<uint16_t> pack,tx,rx;
static std::deque<std::vector<uint16_t>> pending;
static uint64_t previous_ns=0,next_broadcast=0;
static unsigned boot_request=0;
static void fail(const char *s){fprintf(stderr,"NATIVE_HOST_ERROR %s\n",s);std::abort();}
extern "C" int native_host_open(const char *medium,const char *aemu,const char *cedar,const char *alto,const char *root){
    FILE *f=fopen(medium,"rb");if(!f)fail("open medium");
    fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);
    if(size<=0 || size%2)fail("medium length");
    std::vector<unsigned char> bytes(size);
    if(fread(bytes.data(),1,size,f)!=(size_t)size)fail("read medium");fclose(f);
    pack.resize(size/2);
    for(size_t i=0;i<pack.size();i++)pack[i]=bytes[2*i]|(uint16_t(bytes[2*i+1])<<8);
    dorado_ethernet_init(&eth);
    dorado_ethernet_set_boot_file(&eth,0110,aemu);
    dorado_ethernet_set_boot_file(&eth,0113,cedar);
    dorado_ethernet_set_eftp_boot_file(&eth,alto);
    dorado_ethernet_set_ftp_root(&eth,root);eth.rx_on=1;
    printf("NATIVE_HOST_READY medium_bytes=%ld original_boot=113B\n",size);fflush(stdout);
    return 1;
}
extern "C" unsigned native_pack_read(unsigned word){if(word>=pack.size())fail("block read bounds");return pack[word];}
extern "C" void native_pack_write(unsigned word,unsigned value){if(word>=pack.size())fail("block write bounds");pack[word]=uint16_t(value);}
extern "C" void native_net_tx_word(unsigned index,unsigned value){if(index>=1024)fail("TX bounds");if(tx.size()<=index)tx.resize(index+1);tx[index]=uint16_t(value);}
extern "C" void native_net_tx(unsigned words){
    if(words<2 || words>1024 || tx.size()<words)fail("TX length");
    tx.resize(words);pending.push_back(tx);
    printf("NATIVE_TX words=%u",words);
    for(unsigned i=0;i<std::min(words,13u);i++)printf(" %06o",tx[i]);puts("");fflush(stdout);
    if(words>=6 && tx[3]==0264){boot_request=tx[5];printf("NATIVE_BOOT_REQUEST %03o\n",boot_request);fflush(stdout);}
}
extern "C" unsigned native_boot_request(){return boot_request;}
extern "C" void native_net_tick(unsigned long long host_ns){
    uint64_t ticks=(host_ns-previous_ns)/32;
    if(!ticks)return;previous_ns+=ticks*32;
    if(ticks>1){
        eth.rx_hold=eth.rx_hold>ticks-1?eth.rx_hold-(ticks-1):0;
        if(eth.eftp_resend_timer && eth.rx_pos>=eth.rx_count)
            eth.eftp_resend_timer=eth.eftp_resend_timer>ticks?eth.eftp_resend_timer-(ticks-1):1;
    }
    dorado_trace_cycle++;(void)dorado_ethernet_wakeup_mask(&eth);
    if(eth.rx_pos>=eth.rx_count && !pending.empty()){
        auto packet=std::move(pending.front());pending.pop_front();
        eth.local_host=packet[0]&255;
        dorado_ethernet_direct_transmit(&eth,packet.data(),int(packet.size()));
    }
    // Same quiet-after-Cedar-113 policy as the deployed standalone daemon.
    if(host_ns>=next_broadcast && eth.last_boot_offset!=0113){
        (void)dorado_ethernet_breath_of_life(&eth);next_broadcast=host_ns+1000000000ull;
    }
}
extern "C" unsigned native_net_rx(){
    if(eth.rx_hold || eth.rx_pos>=eth.rx_count)return 0;
    size_t end=eth.rx_pos;
    while(end<eth.rx_count && !eth.rx_attention[end])end++;
    if(end==eth.rx_count || end<eth.rx_pos+3 || eth.rx_words[end])fail("Pup queue boundary");
    size_t n=end-eth.rx_pos-1;
    rx.assign(eth.rx_words+eth.rx_pos,eth.rx_words+eth.rx_pos+n);
    if(n>=12 && rx[1]==DORADO_PUP_TYPE_ETHERNET && rx[3]==DORADO_PUP_TYPE_MICROCODE_BOOT_REPLY && rx[2]==n*2-2)rx.push_back(0xffff);
    if(rx.size()>1024)fail("RX length");eth.rx_pos=end+1;
    return unsigned(rx.size());
}
extern "C" unsigned native_net_rx_word(unsigned index){if(index>=rx.size())fail("RX bounds");return rx[index];}
extern "C" unsigned long long native_limit(const char *path){
    FILE *f=fopen(path,"r");if(!f)fail("limit file");unsigned long long n=0;
    if(fscanf(f,"%llu",&n)!=1 || !n)fail("limit value");fclose(f);return n;
}
