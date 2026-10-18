#include "Vdorado_ethernet_ddr.h"
#include <verilated.h>
extern "C" {
#include "dorado_ddr_ring.h"
}
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <optional>
#include <vector>

using Bytes=std::vector<unsigned char>;
#ifndef DDR_INITIAL_COUNT
#define DDR_INITIAL_COUNT 0xfffffff0u
#endif
static constexpr uint32_t initial=DDR_INITIAL_COUNT;
static void require(bool ok,const char *why) {
    if(!ok){fprintf(stderr,"DDR RTL FAIL: %s\n",why);std::exit(23);}
}
static Bytes frame(unsigned n,unsigned token) {
    Bytes b(n);for(unsigned i=0;i<n;i++)b[i]=(unsigned char)(token*37+i*53+(i>>8));return b;
}
struct Model {
    Vdorado_ethernet_ddr top;
    alignas(64) std::array<unsigned char,65536> memory;
    dorado_ddr_ring host={};
    uint64_t cycle=0,epoch=UINT64_C(0x83a7156c142957ba);
    uint32_t random=0x719fe30b;
    unsigned reads=0,writes=0,held=0,zero_latency=0,tx_acks=0,rx_commits=0;
    unsigned busy_for=0;
    bool force_busy=false,was_held=false;
    struct Command {uint32_t addr;uint64_t data;unsigned be,read,write;};
    Command previous={};
    struct Response {uint64_t due,data;};
    std::optional<Response> pending;
    Bytes source;
    uint16_t tx_q=0;
    std::array<uint16_t,1024> rx_words={};
    std::deque<Bytes> expected_tx,expected_rx;
    uint32_t rng(){random^=random<<13;random^=random>>17;return random^=random<<5;}
    uint32_t get32(unsigned off)const {
        uint32_t v=0;for(unsigned i=0;i<4;i++)v|=uint32_t(memory[off+i])<<(i*8);return v;
    }
    uint64_t get64(unsigned off)const{return uint64_t(get32(off+4))<<32|get32(off);}
    void put32(unsigned off,uint32_t v){for(unsigned i=0;i<4;i++)memory[off+i]=(unsigned char)(v>>(8*i));}
    void put64(unsigned off,uint64_t v){put32(off,uint32_t(v));put32(off+4,uint32_t(v>>32));}
    Model(){memory.fill(0xa5);top.enable=0;top.reset=0;top.epoch=epoch;top.tx_valid=0;top.tx_words=0;}
    void offer(Bytes b){require(source.empty(),"overwrote pending wire TX");source=b;expected_tx.push_back(b);}
    void tick() {
        top.clk=0;top.ddram_busy=force_busy || (rng()%5==0);
        top.ddram_rvalid=0;top.ddram_rdata=0;top.tx_data=tx_q;
        top.tx_valid=!source.empty();top.tx_words=(unsigned)source.size()/2;
        top.rx_ready=busy_for==0;top.eval();
        if(pending && pending->due<=cycle) {
            top.ddram_rvalid=1;top.ddram_rdata=pending->data;pending.reset();
        }
        require(!(top.ddram_read&&top.ddram_write),"simultaneous DDR read/write");
        Command now={top.ddram_addr,top.ddram_wdata,top.ddram_be,top.ddram_read,top.ddram_write};
        if(was_held)require(now.addr==previous.addr&&now.data==previous.data&&now.be==previous.be&&
            now.read==previous.read&&now.write==previous.write,"changed held DDR request");
        was_held=(now.read||now.write)&&top.ddram_busy;previous=now;
        if(was_held)held++;
        if((now.read||now.write)&&!top.ddram_busy) {
            require(top.ddram_burstcount==1,"unexpected DDR burst");
            uint64_t byte=uint64_t(now.addr)*8;
            require(byte>=0x1ff00000u && byte+8<=0x1ff10000u,"DDR outside reserved aperture");
            unsigned off=(unsigned)(byte-0x1ff00000u);
            if(now.write) {
                bool tx_payload=off>=0x1000 && off<0x5000;
                bool tx_descriptor=off>=0x100 && off<0x180;
                bool indices=off==0x10 || off==0x28;
                bool init_only=(off==8 || off==0x18 || off==0x20 || off==0x38) && !top.initialized;
                require(tx_payload || tx_descriptor || indices || init_only || off==0 || off==0x30,
                        "FPGA wrote host payload/descriptor/index or unrelated memory");
                if(indices && top.initialized)require(now.be==0x0f,"touched reserved index bytes");
                for(unsigned i=0;i<8;i++)if(now.be&(1u<<i))memory[off+i]=(unsigned char)(now.data>>(8*i));
                writes++;
            } else {
                require(off==0x18 || off==0x20 || (off>=0x200 && off<0x300) ||
                        (off>=0x5000 && off<0xd000),"FPGA read unrelated memory");
                require(!pending && !top.ddram_rvalid,"more than one DDR read outstanding");
                unsigned delay=rng()%18;uint64_t data=get64(off);reads++;
                if(!delay){top.ddram_rvalid=1;top.ddram_rdata=data;zero_latency++;}
                else pending=Response{cycle+delay,data};
            }
        }
        // Independent synchronous wire-buffer model, observed at its edge.
        uint16_t next_q=0;
        if(!source.empty() && unsigned(top.tx_addr)*2+1<source.size())
            next_q=uint16_t(source[2*top.tx_addr])<<8|source[2*top.tx_addr+1];
        if(top.rx_wr) {
            require(top.rx_ready && top.rx_addr<1024,"write into busy wire RX");
            rx_words[top.rx_addr]=top.rx_data;
        }
        if(top.rx_commit) {
            require(top.rx_ready && !expected_rx.empty(),"unsolicited/busy wire commit");
            const auto &expected=expected_rx.front();
            require(top.rx_words*2u==expected.size(),"RX length");
            for(unsigned i=0;i<expected.size();i++)
                require(((rx_words[i/2]>>(i%2?0:8))&255)==expected[i],"RX payload/byte order");
            expected_rx.pop_front();rx_commits++;busy_for=500+rng()%500;
        }
        bool ack=top.tx_ack;
        if(ack){require(!source.empty(),"duplicate/unsolicited wire TX ACK");tx_acks++;}
        top.clk=1;top.eval();tx_q=next_q;
        if(ack)source.clear();
        if(busy_for)busy_for--;
        cycle++;
    }
    void start() {
        tick();top.enable=1;
        for(unsigned i=0;i<10000 && !top.initialized;i++)tick();
        require(top.initialized && !top.stopped && !top.fault,"initialization failed");
        require(get64(0)==UINT64_C(0x31474e49524f4444),"literal magic differs");
        require(get64(0x38)==UINT64_C(0x0001080000100008),"literal ABI differs");
        require(get64(8)==epoch,"epoch publication");
        require(!dorado_ddr_bind(&host,memory.data(),memory.size(),epoch),"actual ARM library bind");
    }
    void finish_stop() {
        for(unsigned i=0;i<20000 && !top.stopped;i++)tick();
        require(top.stopped && !top.initialized && !pending,"stop did not drain accepted read");
        require(get64(0)==0,"stopped before invalidating magic");
        unsigned commands=reads+writes;
        for(unsigned i=0;i<100;i++)tick();
        require(reads+writes==commands,"DDR activity after stop");
    }
};
static void round_trip() {
    Model m;m.start();unsigned offered=0,sent=0,received=0,full=0,tx_full_cycles=0;
    constexpr unsigned frames=128;
    for(unsigned i=0;i<2000000;i++) {
        if(m.source.empty() && offered<frames) {
            unsigned size=offered%5==0?2048:4+2*(m.rng()%1023);
            m.offer(frame(size,offered++));
        }
        if(sent<frames && i%7==0) {
            unsigned size=sent%7==0?2048:sent%9==0?6:4+2*((sent*7919)%1023);
            Bytes bytes=frame(size,sent+1000);
            int result=dorado_ddr_send(&m.host,bytes.data(),bytes.size());
            if(!result){m.expected_rx.push_back(bytes);sent++;}
            else {require(result==-EAGAIN,"host RX send fault");full++;}
        }
        // Initially hold the host consumer until the8-slot TX ring is full.
        if(i>60000 && i%101==0) {
            unsigned char bytes[2048];int n=dorado_ddr_receive(&m.host,bytes,sizeof bytes);
            if(n>0) {
                require(!m.expected_tx.empty(),"unexpected host TX frame");
                const auto &expected=m.expected_tx.front();
                require(size_t(n)==expected.size() && !memcmp(bytes,expected.data(),expected.size()),"TX content/length/order");
                m.expected_tx.pop_front();received++;
            } else require(n==-EAGAIN,"host TX receive fault");
        }
        m.tick();require(!m.top.fault,"RTL protocol fault during traffic");
        if(m.get32(0x10)-m.get32(0x18)==8)tx_full_cycles++;
        if(received==frames && m.rx_commits==frames && m.tx_acks==frames &&
           m.get32(0x28)==initial+frames)break;
    }
    require(received==frames && sent==frames && m.rx_commits==frames && m.tx_acks==frames,"traffic timeout");
    require(full>1000 && tx_full_cycles>1000 && m.held>1000 && m.zero_latency>10,"stall/full/zero-latency coverage");
    require(m.get32(0x10)==initial+frames && m.get32(0x28)==initial+frames,"FPGA count wrap");
    require(m.get32(0x18)==initial+frames && m.get32(0x20)==initial+frames,"ARM count wrap");
    dorado_ddr_quiesce(&m.host);m.top.enable=0;m.tick();m.finish_stop();
    printf("DDR RTL TRAFFIC PASS:128 frames each way, initial=%08x clocks=%llu reads=%u writes=%u held=%u immediate=%u full=%u; C library/oracle payloads and32-bit counts\n",
        initial,(unsigned long long)m.cycle,m.reads,m.writes,m.held,m.zero_latency,full);
}
static void mid_frame_stop() {
    for(unsigned direction=0;direction<2;direction++) {
        Model m;m.start();Bytes bytes=frame(2048,direction+77);
        if(direction==0)m.offer(bytes);
        else {
            require(!dorado_ddr_send(&m.host,bytes.data(),bytes.size()),"stage RX stop frame");
            m.expected_rx.push_back(bytes);
        }
        bool transferring=false;
        for(unsigned i=0;i<20000;i++) {
            m.tick();uint64_t address=uint64_t(m.top.ddram_addr)*8-0x1ff00000u;
            if((direction==0 && m.top.ddram_write && address>=0x1000 && address<0x5000) ||
               (direction==1 && m.top.ddram_read && address>=0x5000 && address<0xd000)) {
                transferring=true;break;
            }
        }
        require(transferring,"no mid-frame stop point");
        dorado_ddr_quiesce(&m.host);m.top.enable=0;m.tick();m.finish_stop();
        require(!m.top.fault,"mid-frame stop fault");
        if(direction==0) {
            unsigned slot=initial&7;
            require(m.tx_acks==1 && m.get32(0x10)==initial+1,"TX stop lost/duplicated publication or ACK");
            require(m.get32(0x100+slot*16)==2048 &&
                    !memcmp(m.memory.data()+0x1000+slot*2048,bytes.data(),2048),"TX stop truncated payload");
        } else require(m.rx_commits==1 && m.get32(0x28)==initial+1,"RX stop lost commit/ownership tail");
    }
    puts("DDR RTL MID-FRAME STOP PASS:2048-byte TX/RX finish payload and publication/ACK/commit tails before stop");
}
static void stop_reset() {
    for(unsigned kind=0;kind<3;kind++) {
        Model m;m.start();
        for(unsigned i=0;i<10000 && !m.pending;i++)m.tick();
        require(bool(m.pending),"no outstanding read for stop fixture");
        dorado_ddr_quiesce(&m.host);
        if(kind==0)m.top.enable=0;
        if(kind==1)m.top.reset=1;
        if(kind==2)m.top.epoch=m.epoch+1;
        m.force_busy=true;
        for(unsigned i=0;i<50;i++)m.tick();
        require(!m.top.stopped,"stop falsely completed stalled DDR write");
        m.force_busy=false;m.top.reset=0;m.finish_stop();
        // A reset or epoch transition cannot automatically reauthorize.
        for(unsigned i=0;i<100;i++)m.tick();
        require(m.top.stopped,"auto restart after reset/generation change");
        m.top.enable=0;m.tick();m.top.epoch=m.epoch;m.top.enable=1;
        for(unsigned i=0;i<100;i++)m.tick();
        require(m.top.stopped,"same epoch reused after stop");
        m.top.enable=0;m.tick();m.epoch+=2;m.top.epoch=m.epoch;m.top.enable=1;
        for(unsigned i=0;i<10000 && !m.top.initialized;i++)m.tick();
        require(m.top.initialized && !m.top.fault,"explicit fresh grant failed");
        require(!dorado_ddr_bind(&m.host,m.memory.data(),m.memory.size(),m.epoch),"new epoch library bind");
        dorado_ddr_quiesce(&m.host);m.top.enable=0;m.tick();m.finish_stop();
    }
    puts("DDR RTL STOP PASS: outstanding read/stalled write drain, reset/epoch change, no stale-grant restart, explicit new epoch");
}
static void malformed() {
    for(unsigned kind=0;kind<6;kind++) {
        Model m;m.start();dorado_ddr_quiesce(&m.host);
        unsigned desc=0x200+(initial&15)*16;
        m.put32(desc,4);m.put32(desc+4,initial);m.put64(desc+8,m.epoch);
        switch(kind) {
        case 0:m.put32(desc,2049);break;
        case 1:m.put32(desc,2050);break;
        case 2:m.put32(desc,2);break;
        case 3:m.put32(desc+4,initial+1);break;
        case 4:m.put64(desc+8,m.epoch+1);break;
        case 5:break;
        }
        m.put32(0x20,initial+(kind==5?17:1));
        for(unsigned i=0;i<10000 && !m.top.fault;i++)m.tick();
        require(m.top.fault && !m.rx_commits,"bad RX descriptor/counter accepted");
        m.finish_stop();
    }
    Model m;m.start();dorado_ddr_quiesce(&m.host);m.offer(frame(2048,53));
    m.put32(0x18,initial+1);
    for(unsigned i=0;i<10000 && !m.top.fault;i++)m.tick();
    require(m.top.fault && !m.tx_acks,"impossible TX ownership accepted");m.finish_stop();
    puts("DDR RTL FAULT PASS: malformed length/sequence/epoch, impossible indices, no wire commit/ACK");
}
int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);round_trip();stop_reset();mid_frame_stop();malformed();
}
