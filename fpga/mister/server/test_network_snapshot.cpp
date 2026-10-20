// Reuse the independent DDR/wire-buffer oracle and actual C ring endpoint.
#define main legacy_ddr_main
#include "legacy_test.cpp"
#undef main

static std::array<uint32_t,DDR_PAUSE_STATE_WORDS> state_of(Model &m) {
    std::array<uint32_t,DDR_PAUSE_STATE_WORDS> state;
    for(unsigned i=0;i<state.size();i++)state[i]=m.top.observed_state[i];
    return state;
}
static void request_pause(Model &m) {
    require(!dorado_ddr_pause(&m.host),"actual C peer rejected pause");
    m.top.snapshot_hold=1;m.top.eval();
}
static void hold_paused(Model &m) {
    for(unsigned i=0;i<20000 && !m.top.snapshot_idle;i++)m.tick();
    require(m.top.snapshot_idle && !m.pending && !m.top.ddram_read && !m.top.ddram_write,
            "pause did not drain accepted DDR/frame work");
    require(m.top.initialized && !m.top.stopped && !m.top.fault,"pause revoked active ownership");
    require(m.get64(0)==DORADO_DDR_MAGIC && m.get64(8)==m.epoch && m.get32(0x30)==1,
            "pause invalidated ring header");
    const auto state=state_of(m);const auto memory=m.memory;
    const auto commands=m.reads+m.writes,acks=m.tx_acks,commits=m.rx_commits;
    unsigned char bytes[2048];
    for(unsigned i=0;i<2000;i++) {
        require(dorado_ddr_receive(&m.host,bytes,sizeof bytes)==-EAGAIN,"paused C receive accessed queue");
        require(dorado_ddr_send(&m.host,bytes,4)==-EAGAIN,"paused C send accessed queue");
        m.tick();
        require(state_of(m)==state,"paused engine state changed");
        require(m.top.snapshot_idle && commands==m.reads+m.writes && m.memory==memory,
                "paused engine touched DDR");
        require(acks==m.tx_acks && commits==m.rx_commits,"paused engine repeated wire ACK/commit");
    }
}
static void continue_traffic(Model &m) {
    m.top.snapshot_hold=0;m.top.eval();
    require(!dorado_ddr_continue(&m.host),"same-session C continue failed");
    m.offer(frame(6,939));Bytes rx=frame(2048,940);
    require(!dorado_ddr_send(&m.host,rx.data(),rx.size()),"continued C send failed");
    m.expected_rx.push_back(rx);
    for(unsigned i=0;i<100000;i++) {
        unsigned char bytes[2048];int n=dorado_ddr_receive(&m.host,bytes,sizeof bytes);
        if(n>0) {
            require(!m.expected_tx.empty(),"continued duplicate TX");const auto &expected=m.expected_tx.front();
            require(size_t(n)==expected.size() && !memcmp(bytes,expected.data(),expected.size()),"continued TX content");
            m.expected_tx.pop_front();
        }else require(n==-EAGAIN,"continued C receive fault");
        m.tick();require(!m.top.fault,"continued RTL fault");
        if(m.expected_tx.empty() && m.expected_rx.empty() && m.source.empty() &&
           m.get32(0x28)==m.host.rx_prod && m.get32(0x10)==m.host.tx_cons)break;
    }
    require(m.expected_tx.empty() && m.expected_rx.empty() && m.source.empty(),"continued traffic timeout");
    require(m.get64(8)==m.epoch,"continue changed epoch");
    dorado_ddr_quiesce(&m.host);m.top.enable=0;m.tick();m.finish_stop();
}
static void idle_pause() {
    Model m;m.start();
    // Start a polling interval without issuing a read; it must retain phase.
    m.busy_for=5000;m.tick();request_pause(m);
    require(m.top.snapshot_idle,"idle pause not immediately safe");
    hold_paused(m);continue_traffic(m);
    puts("NETWORK SNAPSHOT IDLE PASS: all original registers/task destinations and poll phase held, same-epoch continuation");
}
static void pending_read_pause() {
    Model m;m.start();
    for(unsigned i=0;i<10000 && !m.pending;i++)m.tick();
    require(bool(m.pending),"no accepted read for snapshot fixture");
    request_pause(m);
    require(!m.top.snapshot_idle,"pause acknowledged an accepted DDR read");
    hold_paused(m);continue_traffic(m);
    puts("NETWORK SNAPSHOT READ PASS: accepted response drained before acknowledgment");
}
static void mid_frame_pause() {
    for(unsigned direction=0;direction<2;direction++) {
        Model m;m.start();Bytes bytes=frame(2048,direction+870);
        if(direction==0)m.offer(bytes);
        else {require(!dorado_ddr_send(&m.host,bytes.data(),bytes.size()),"stage paused RX");m.expected_rx.push_back(bytes);}
        bool transferring=false;
        for(unsigned i=0;i<20000;i++) {
            m.tick();const uint64_t address=uint64_t(m.top.ddram_addr)*8-0x1ff00000u;
            if((direction==0 && m.top.ddram_write && address>=0x1000 && address<0x5000) ||
               (direction==1 && m.top.ddram_read && address>=0x5000 && address<0xd000)) {
                transferring=true;break;
            }
        }
        require(transferring,"no in-frame snapshot point");
        request_pause(m);m.force_busy=true;
        for(unsigned i=0;i<40;i++) {
            m.tick();require(!m.top.snapshot_idle,"pause acknowledged stalled frame");
        }
        m.force_busy=false;hold_paused(m);
        require(direction==0 ? m.tx_acks==1 : m.rx_commits==1,"pause lost publication/ACK/commit tail");
        continue_traffic(m);
    }
    puts("NETWORK SNAPSHOT FRAMES PASS: 2048-byte TX/RX tails, backpressure, immutable queues and exact continued payloads");
}
static void lifecycle_pause() {
    for(unsigned kind=0;kind<3;kind++) {
        Model m;m.start();m.busy_for=5000;m.tick();request_pause(m);hold_paused(m);
        if(kind==0)m.top.reset=1;
        if(kind==1)m.top.enable=0;
        if(kind==2)m.top.epoch=m.epoch+1;
        m.top.eval();require(!m.top.snapshot_idle,"pause hid lifecycle change");
        m.tick();m.top.reset=0;m.finish_stop();
        require(dorado_ddr_continue(&m.host)==-ESTALE && !m.host.active,"continued revoked ring");
        dorado_ddr_quiesce(&m.host);
        for(unsigned i=0;i<100;i++)m.tick();
        require(m.top.stopped && !m.top.initialized,"pause reauthorized stale epoch");
    }
    puts("NETWORK SNAPSHOT LIFECYCLE PASS: reset/disable/epoch change revoke ready, drain STOP and reject stale continuation");
}
static void full_queues_pause() {
    Model m;m.start();m.busy_for=100000;
    for(unsigned i=0;i<8;i++) {
        m.offer(frame(6,i+1010));
        for(unsigned age=0;age<20000 && !m.source.empty();age++)m.tick();
        require(m.source.empty(),"TX ring fill timeout");
    }
    for(unsigned i=0;i<16;i++) {
        Bytes bytes=frame(6,i+1110);
        require(!dorado_ddr_send(&m.host,bytes.data(),bytes.size()),"RX ring fill failed");
        m.expected_rx.push_back(bytes);
    }
    m.offer(frame(6,1210));for(unsigned i=0;i<200;i++)m.tick();
    require(m.get32(0x10)-m.host.tx_cons==8 && m.host.rx_prod-m.get32(0x28)==16,
            "queue-full snapshot precondition");
    request_pause(m);hold_paused(m);
    require(!m.source.empty() && m.tx_acks==8 && m.rx_commits==0,"pause discarded full-queue work");
    m.top.snapshot_hold=0;m.top.eval();require(!dorado_ddr_continue(&m.host),"full-queue continue failed");
    m.busy_for=0;
    for(unsigned age=0;age<100000;age++) {
        unsigned char bytes[2048];int n=dorado_ddr_receive(&m.host,bytes,sizeof bytes);
        if(n>0) {
            require(!m.expected_tx.empty(),"full-queue duplicate TX");const auto &expected=m.expected_tx.front();
            require(size_t(n)==expected.size() && !memcmp(bytes,expected.data(),expected.size()),"full-queue TX content");
            m.expected_tx.pop_front();
        }else require(n==-EAGAIN,"full-queue C receive fault");
        m.tick();require(!m.top.fault,"full-queue RTL fault");
        if(m.expected_tx.empty() && m.expected_rx.empty() && m.source.empty() &&
           m.get32(0x28)==m.host.rx_prod && m.get32(0x10)==m.host.tx_cons)break;
    }
    require(m.expected_tx.empty() && m.expected_rx.empty() && m.source.empty() && m.tx_acks==9 && m.rx_commits==16,
            "full-queue continuation lost work");
    dorado_ddr_quiesce(&m.host);m.top.enable=0;m.tick();m.finish_stop();
    puts("NETWORK SNAPSHOT FULL PASS: 8TX/16RX queued, pending ninth TX, exact payload/count continuation");
}
static void disabled_pause() {
    Model m;m.top.snapshot_hold=1;m.tick();
    require(m.top.snapshot_idle && m.top.stopped,"inactive engine pause not safe");
    const auto state=state_of(m);const auto memory=m.memory;
    for(unsigned i=0;i<300;i++) {
        m.tick();require(state_of(m)==state && m.memory==memory,"paused inactive engine changed");
    }
    m.top.snapshot_hold=0;m.tick();m.start();
    request_pause(m);hold_paused(m);continue_traffic(m);
    puts("NETWORK SNAPSHOT INACTIVE PASS: stopped state held and explicit grant works after release");
}
int main(int argc,char **argv) {
    Verilated::commandArgs(argc,argv);
    idle_pause();pending_read_pause();mid_frame_pause();full_queues_pause();lifecycle_pause();disabled_pause();
    legacy_ddr_main(argc,argv);
    printf("NETWORK SNAPSHOT PASS state_bits=%u initial=%08x actual_C_peer=1 complete_machine=0\n",DDR_PAUSE_STATE_BITS,initial);
    return 0;
}
