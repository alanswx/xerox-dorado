#define main legacy_wire_main
#include "legacy_wire_test.cpp"
#undef main

static std::array<uint32_t,NETWORK_STATE_WORDS> network_state(Model &m) {
    std::array<uint32_t,NETWORK_STATE_WORDS> result;
    for(unsigned i=0;i<result.size();i++)result[i]=m.t.observed_state[i];
    return result;
}
static void pause_network(Model &m) {
    m.t.machine_enable=0;m.ticks(2);
    check(!dorado_ddr_pause(&m.host),"pause actual C peer");
    m.t.snapshot_hold=1;
    m.until([&]{return m.t.snapshot_idle;},300000,"engine did not drain to snapshot pause");
    // Allow the wire's synchronous read outputs to settle after its last
    // possible host ACK/commit. Main must enforce this boundary in integration.
    m.ticks(2);
    check(!m.pending && !m.t.DDRAM_RD && !m.t.DDRAM_WE,"snapshot has pending DDR command");
    const auto state=network_state(m);const auto memory=m.memory;
    const auto commands=m.reads+m.writes;const auto wave=m.wave.size();
    for(unsigned i=0;i<2000;i++) {
        m.tick();check(network_state(m)==state,"held network state changed");
        check(m.memory==memory && commands==m.reads+m.writes,"held network DDR changed");
        check(m.wave.size()==wave && m.t.snapshot_idle,"held network wire advanced");
    }
    // Other EXT_BUS clients keep using SPI while the snapshot is exported.
    // Embedded network opcodes must remain data for that other transaction.
    m.begin(0x76);
    for(unsigned i=0;i<40;i++) {
        m.word(uint16_t(0x70+i%6));
        check(!m.t.io_selected && network_state(m)==state,"snapshot SPI changed held network state");
    }
    m.end();check(network_state(m)==state,"snapshot SPI deselection changed held network state");
}
static void continue_network(Model &m) {
    check(!dorado_ddr_continue(&m.host),"continue actual C peer");
    m.t.snapshot_hold=0;m.ticks(2);m.t.machine_enable=1;
}
static void spi_entry_pause() {
    Model m;m.start(UINT64_C(0x30abc09812765432));m.begin(0x74);
    m.t.machine_enable=0;m.ticks(2);check(!dorado_ddr_pause(&m.host),"pause C peer during SPI");m.t.snapshot_hold=1;
    for(unsigned i=0;i<18;i++){m.word(0);check(!m.t.snapshot_idle,"pause truncated active SPI transaction");}
    m.end();pause_network(m);
    m.begin(0x76);m.t.snapshot_hold=0;
    for(unsigned i=0;i<6;i++){m.word(0x75);check(!m.t.io_selected,"parser released inside shared SPI packet");}
    m.end();continue_network(m);m.stop();
    puts("NETWORK WIRE SPI PASS: active transaction finishes, held parser ignores shared snapshot traffic, framed release");
}
static void receive_pause() {
    Model m;m.start(UINT64_C(0x31abc09812765432));auto bytes=frame(2048,72);
    check(!dorado_ddr_send(&m.host,bytes.data(),bytes.size()),"stage in-flight snapshot RX");
    m.until([&]{return m.t.DDRAM_RD && uint64_t(m.t.DDRAM_ADDR)*8>=0x1ff05000u;},200000,"no in-flight snapshot RX");
    pause_network(m);check(!m.t.rx_ready,"paused RX did not retain wire buffer");
    m.wave.clear();m.record=true;continue_network(m);
    m.until([&]{return m.t.rx_ready;},1000000,"paused RX completion timeout");m.record=false;m.verify_wave(bytes);m.stop();
    puts("NETWORK WIRE RX PASS: DDR tail drained, complete wire RAM retained, independent resumed payload/CRC");
}
static void serial_pause() {
    Model m;m.start(UINT64_C(0x32abc09812765432));auto bytes=frame(2048,73);
    check(!dorado_ddr_send(&m.host,bytes.data(),bytes.size()),"stage serial snapshot RX");
    m.until([&]{return !m.t.rx_ready;},200000,"serial snapshot did not start");
    m.wave.clear();m.record=true;m.ticks(1000);pause_network(m);continue_network(m);
    m.until([&]{return m.t.rx_ready;},1000000,"resumed serial snapshot timeout");m.record=false;m.verify_wave(bytes);m.stop();
    puts("NETWORK WIRE PHASE PASS: partial serial frame and CRC retain exact guest phase");
}
static void transmit_pause() {
    Model m;m.start(UINT64_C(0x33abc09812765432));auto bytes=frame(2048,74);m.cable(bytes);
    m.until([&]{return m.t.DDRAM_WE && uint64_t(m.t.DDRAM_ADDR)*8>=0x1ff01000u && uint64_t(m.t.DDRAM_ADDR)*8<0x1ff05000u;},200000,"no in-flight snapshot TX");
    pause_network(m);check(m.get32(0x10)==1 && !m.t.tx_valid,"snapshot TX tail/ACK did not complete");
    continue_network(m);check(m.receive()==bytes,"resumed captured TX content");m.stop();
    puts("NETWORK WIRE TX PASS: captured2048-byte buffer, publication and ACK tail retained");
}
static void capture_pause() {
    Model m;m.start(UINT64_C(0x34abc09812765432));auto bytes=frame(34,75);uint16_t crc=0x8005;
    m.bit(true);
    unsigned bit=0;
    for(unsigned char byte:bytes)for(int i=7;i>=0;i--) {
        const bool value=(byte>>i)&1;m.bit(value);crc=crc_bit(crc,value);
        if(++bit==16){pause_network(m);check(!m.t.tx_valid,"partial capture fabricated a frame");continue_network(m);}
    }
    for(int i=15;i>=0;i--)m.bit((crc>>i)&1);m.t.ethernet_tx_n=1;m.ticks(150);
    m.until([&]{return m.get32(0x10)==1;},200000,"resumed partial capture timeout");
    check(m.receive()==bytes && m.t.good==1 && !m.t.bad && !m.t.dropped,"resumed partial capture CRC/content");m.stop();
    puts("NETWORK WIRE CAPTURE PASS: partial guest TX shift/count/CRC retained without host completion");
}
int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    spi_entry_pause();receive_pause();serial_pause();transmit_pause();capture_pause();
    legacy_wire_main();
    printf("NETWORK WIRE SNAPSHOT PASS observed_bits=%u all_packet_RAM=1 restored=0\n",NETWORK_STATE_BITS);
    return 0;
}
