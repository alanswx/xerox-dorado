#include "Vdorado_ddr_wire_fixture.h"
#include <verilated.h>
extern "C" {
#include "dorado_ddr_ring.h"
}
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <vector>
using Bytes=std::vector<unsigned char>;
static void check(bool yes,const char *why){if(!yes){fprintf(stderr,"DDR WIRE FAIL: %s\n",why);std::exit(23);}}
static uint16_t crc_bit(uint16_t c,bool b){return uint16_t((c<<1)^(((c>>15)^b)?0x8005:0));}
static Bytes frame(unsigned n,unsigned token){Bytes b(n);for(unsigned i=0;i<n;i++)b[i]=uint8_t(i*53+token*37+(i>>8));return b;}
struct Model {
    Vdorado_ddr_wire_fixture t;
    alignas(64) std::array<unsigned char,65536> memory;
    dorado_ddr_ring host={};
    uint64_t cycles=0;
    uint32_t rng_state=0x617593ab;
    unsigned reads=0,writes=0,holds=0,immediate=0;
    bool force_busy=false,held=false,loopback=false,record=false;
    struct Command{uint32_t address;uint64_t data;unsigned be,rd,wr;};
    Command previous={};
    struct Response{uint64_t due,data;};std::optional<Response> pending;
    std::vector<unsigned char> wave;
    uint32_t rng(){rng_state^=rng_state<<13;rng_state^=rng_state>>17;return rng_state^=rng_state<<5;}
    uint32_t get32(unsigned o){uint32_t n=0;for(unsigned i=0;i<4;i++)n|=uint32_t(memory[o+i])<<(8*i);return n;}
    uint64_t get64(unsigned o){return get32(o)|(uint64_t(get32(o+4))<<32);}
    Model(){memory.fill(0xa5);t.clk_sys=0;t.pll_locked=0;t.machine_enable=1;t.ethernet_tx_n=1;
        t.io_enable=t.io_strobe=0;t.io_din=0;for(unsigned i=0;i<4;i++)tick();t.pll_locked=1;tick();}
    void tick(){
        t.clk_sys=0;if(loopback)t.ethernet_tx_n=!t.ethernet_rx;
        t.DDRAM_BUSY=force_busy || rng()%5==0;t.DDRAM_DOUT_READY=0;t.DDRAM_DOUT=0;t.eval();
        if(pending && pending->due<=cycles){t.DDRAM_DOUT_READY=1;t.DDRAM_DOUT=pending->data;pending.reset();}
        Command now={t.DDRAM_ADDR,t.DDRAM_DIN,t.DDRAM_BE,t.DDRAM_RD,t.DDRAM_WE};
        check(!(now.rd&&now.wr),"simultaneous DDR directions");
        if(held)check(now.address==previous.address&&now.data==previous.data&&now.be==previous.be&&now.rd==previous.rd&&now.wr==previous.wr,"held DDR request changed");
        held=(now.rd||now.wr)&&t.DDRAM_BUSY;previous=now;if(held)holds++;
        if((now.rd||now.wr)&&!t.DDRAM_BUSY){
            check(t.DDRAM_BURSTCNT==1,"burst contract");uint64_t byte=uint64_t(now.address)*8;
            check(byte>=0x1ff00000u&&byte+8<=0x1ff10000u,"DDR outside aperture");unsigned off=unsigned(byte-0x1ff00000u);
            if(now.wr){for(unsigned i=0;i<8;i++)if(now.be&(1u<<i))memory[off+i]=uint8_t(now.data>>(8*i));writes++;}
            else{check(!pending&&!t.DDRAM_DOUT_READY,"multiple outstanding reads");unsigned delay=rng()%19;reads++;
                if(!delay){t.DDRAM_DOUT_READY=1;t.DDRAM_DOUT=get64(off);immediate++;}
                else pending=Response{cycles+delay,get64(off)};}
        }
        t.clk_sys=1;t.eval();if(record && t.wire_step)wave.push_back(t.ethernet_rx);cycles++;
    }
    void ticks(unsigned n){while(n--)tick();}
    template<class P> void until(P done,unsigned limit,const char *why){while(!done()&&limit--)tick();check(done(),why);}
    uint16_t word(uint16_t value){t.io_din=value;t.io_strobe=1;tick();t.io_strobe=0;tick();return t.io_dout;}
    uint16_t begin(unsigned command){t.io_enable=1;return word(uint16_t(command));}
    void end(){t.io_enable=0;ticks(3);}
    void control(bool start,uint64_t epoch){check(begin(0x75)==0xd0d4,"control capability");word(start?1:0);for(unsigned i=0;i<4;i++)word(uint16_t(epoch>>(16*i)));end();}
    void start(uint64_t epoch){control(true,epoch);until([&]{return t.initialized;},10000,"initialize");check(t.owned&&!t.stopped&&!t.fault&&!t.errors,"initial ownership");
        check(!dorado_ddr_bind(&host,memory.data(),memory.size(),epoch),"bind actual C endpoint");}
    void bit(bool b){t.ethernet_tx_n=b;ticks(23);t.ethernet_tx_n=!b;ticks(23);}
    void cable(const Bytes &b,bool corrupt=false){uint16_t crc=0x8005;bit(true);
        for(unsigned char byte:b)for(int i=7;i>=0;i--){bool v=(byte>>i)&1;bit(v);crc=crc_bit(crc,v);}
        for(int i=15;i>=0;i--)bit(((crc>>i)&1)^(corrupt&&i==8));t.ethernet_tx_n=1;ticks(150);}
    Bytes receive(){Bytes b(2048);int n=dorado_ddr_receive(&host,b.data(),b.size());check(n>=4,"missing C frame");b.resize(unsigned(n));return b;}
    void verify_wave(const Bytes &expected){
        // Sample around expected midpoint transitions, independently of the
        // adapter's decoder/state. At SYSPER4 the full bit is1360/30 ticks.
        size_t first=1;while(first<wave.size() && !(wave[first]&&!wave[first-1]))first++;
        check(first<wave.size(),"missing serial sync");uint16_t crc=0x8005;Bytes actual(expected.size(),0);
        unsigned count=unsigned(expected.size()*8+16);
        for(unsigned j=1;j<=count;j++){
            size_t center=first+(uint64_t(j)*1360+15)/30,found=wave.size();
            for(size_t k=center>2?center-2:1;k<=center+2 && k<wave.size();k++)if(wave[k]!=wave[k-1]){check(found==wave.size(),"ambiguous serial midpoint");found=k;}
            check(found<wave.size(),"missing serial midpoint");bool b=wave[found];crc=crc_bit(crc,b);
            if(j<=expected.size()*8)actual[(j-1)/8]|=uint8_t(b<<(7-(j-1)%8));
        }
        check(actual==expected && crc==0,"serialized payload/CRC differs");
    }
    void stop(){dorado_ddr_quiesce(&host);control(false,0);
        until([&]{return t.stopped;},200000,"stop drain");check(!t.owned&&!t.initialized&&!pending&&get64(0)==0,"stop ownership/publication");
        unsigned n=reads+writes;ticks(1000);check(n==reads+writes,"DDR after stopped");}
};
int main(){
    Model m;
    check(m.begin(0x70)==0xd074,"packet capability");check(m.word(0)==13,"power-up SPI fallback");m.end();
    // Actual DskEth captured FileSystem lookup: controller-generated CRC7c50.
    const unsigned char raw[]={0,0xff,0x80,2,0,0,0,1,0,3,0,0x80,0xa,0x46,0x69,0x6c,0x65,0x53,0x79,0x73,0x74,0x65,0x6d,0};
    Bytes golden(raw,raw+sizeof raw);m.bit(true);
    for(unsigned char b:golden)for(int i=7;i>=0;i--)m.bit((b>>i)&1);
    for(int i=15;i>=0;i--)m.bit((0x7c50>>i)&1);m.t.ethernet_tx_n=1;m.ticks(150);
    check(m.t.tx_valid&&m.t.good==1,"actual DskEth capture rejected");
    // A packet pending before ownership changes must cross DDR exactly once.
    m.start(UINT64_C(0x13abce9876543210));m.until([&]{return m.get32(0x10)==1;},200000,"pending SPI-to-DDR frame");
    check(m.receive()==golden,"captured frame bytes changed");
    auto bad=frame(34,12);m.cable(bad,true);check(m.t.bad==1,"corrupt CRC accepted");
    check(m.begin(0x70)==0xd074 && m.word(0)==12,"legacy readiness leaked during DDR");m.end();
    check(m.begin(0x71)==0 && m.word(0)==0,"legacy TX leaked");m.end();
    m.begin(0x72);m.end(); // Cannot steal the next packet's ACK.
    for(unsigned index=0;index<12;index++){
        auto b=frame(index%3==0?2048:4+2*(index*73%1023),index+31);
        m.cable(b);unsigned target=2+index;m.until([&]{return m.get32(0x10)==target;},200000,"wire-to-DDR timeout");
        check(m.receive()==b,"wire-to-DDR content/order");
    }
    // Host staging progresses while guest wire time is paused. Capture the
    // emitted serial bytes and CRC independently once the guest resumes.
    for(unsigned index=0;index<5;index++){
        auto b=frame(index==0?2048:6+2*index*79,index+100);
        m.t.machine_enable=0;m.tick();bool line=m.t.ethernet_rx;
        check(!dorado_ddr_send(&m.host,b.data(),b.size()),"C send into empty RX ring");unsigned target=index+1;
        m.until([&]{return m.get32(0x28)==target;},300000,"DDR did not stage paused wire");
        check(!m.t.rx_ready && m.t.ethernet_rx==line,"wire advanced while paused");
        m.ticks(300);check(m.t.ethernet_rx==line,"wire advanced while paused");
        m.wave.clear();m.record=true;m.t.machine_enable=1;
        m.until([&]{return m.t.rx_ready;},1000000,"serial completion timeout");m.record=false;m.verify_wave(b);
    }
    // Mid-frame pause retains the exact wire phase; the independent decoder
    // sees the same packet with paused host ticks omitted.
    auto b=frame(2048,208);check(!dorado_ddr_send(&m.host,b.data(),b.size()),"stage mid-frame test");
    m.until([&]{return !m.t.rx_ready;},200000,"wire did not accept mid-frame");m.wave.clear();m.record=true;m.ticks(1000);
    m.t.machine_enable=0;m.tick();bool line=m.t.ethernet_rx;m.ticks(1111);check(m.t.ethernet_rx==line,"wire advanced while paused");
    m.t.machine_enable=1;m.until([&]{return m.t.rx_ready;},1000000,"paused frame completion");m.record=false;m.verify_wave(b);
    // STOP drains transport while leaving an already-committed serial frame
    // intact. Switching to SPI must not mutate that owned wire buffer.
    m.t.machine_enable=0;m.tick();b=frame(2048,255);check(!dorado_ddr_send(&m.host,b.data(),b.size()),"stage stop frame");
    m.until([&]{return m.get32(0x28)==7;},300000,"stop frame not staged");m.stop();
    check(m.begin(0x73)==0,"SPI offered busy serial buffer after STOP");m.end();
    m.wave.clear();m.record=true;m.t.machine_enable=1;m.until([&]{return m.t.rx_ready;},1000000,"stopped transport lost serial frame");m.record=false;m.verify_wave(b);
    check(m.begin(0x70)==0xd074 && (m.word(0)&1),"SPI readiness not restored");m.end();
    // New-generation start through production UIO and both independent ends.
    m.start(UINT64_C(0x147bef124356789a));m.cable(frame(1480,999));m.until([&]{return m.get32(0x10)==1;},200000,"restart frame");
    check(m.receive()==frame(1480,999),"restart payload");m.stop();
    check(!m.t.fault&&!m.t.errors&&!m.t.rx_bad&&!m.t.dropped,"unexpected transport/wire fault");
    check(m.holds>100 && m.immediate>10,"random DDR coverage");
    printf("DDR WIRE PASS: production wrapper mux/UIO/engine/wire+C endpoint; captured DskEth golden, CRC rejection,2048-byte frames, pause/serial phase, independent payload+CRC, STOP handoff/restart; clocks=%llu reads=%u writes=%u held=%u immediate=%u\n",(unsigned long long)m.cycles,m.reads,m.writes,m.holds,m.immediate);
}
