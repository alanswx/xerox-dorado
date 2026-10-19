#include "Vmosram_fixture.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

static Vmosram_fixture d;
static constexpr unsigned transport_mask=(1u<<18)-1;
static constexpr std::array<unsigned,2>address_bits={14,16},depth={16384,65536};
struct State{unsigned row=0,col=0;bool ras=0,cas=0,dout=0,pipe=0,we=0,pending=0,data=0;};
static std::array<State,2>state{},saved_state{};
static std::array<std::array<uint8_t,65536>,2>memory{},saved_memory{};
static std::array<unsigned,2>writes{},reads{};
static uint32_t rng=0x6416d2a7;
static unsigned edges,holds,requests,walked_bits,collisions;
static unsigned random_word(){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void check(bool ok,const char*why){if(!ok){std::fprintf(stderr,"FAIL edge=%u: %s\n",edges,why);std::exit(1);}}
static unsigned pack(const State&s,unsigned k){
    unsigned a=address_bits[k],half=a/2;
    return s.row|(s.col<<half)|(unsigned(s.ras)<<a)|(unsigned(s.cas)<<(a+1))|
      (unsigned(s.dout)<<(a+2))|(unsigned(s.pipe)<<(a+3))|(unsigned(s.we)<<(a+4))|
      (unsigned(s.pending)<<(a+5))|(unsigned(s.data)<<(a+6));
}
static void equal(bool reference=true){
    for(unsigned k=0;k<2;++k){
        check(d.candidate_history[k]==pack(state[k],k),"ordinary/hold history differs");
        check(((d.q>>k)&1)==unsigned(state[k].dout),"ordinary held output differs");
        unsigned a=d.probe_address&(depth[k]-1);
        check(((d.candidate_probe>>k)&1)==memory[k][a],"physical RAM differs");
        if(reference){
            check(d.original_history[k]==pack(state[k],k),"untouched original history differs");
            check(((d.original_q>>k)&1)==unsigned(state[k].dout),"untouched original output differs");
            check(((d.original_probe>>k)&1)==memory[k][a],"untouched original RAM differs");
        }
    }
}
static void edge(bool run,bool reference){
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();d.advance=run;d.eval();
    d.host_clk=1;d.ref_clk=reference;d.eval();++edges;
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.eval();
}
static void ordinary(bool run=true,bool reference=true){
    if(run)for(unsigned k=0;k<2;++k){
        State old=state[k],next=old;unsigned half=address_bits[k]/2,mask=(1u<<half)-1;
        unsigned a=(old.row<<half)|old.col;next.pipe=memory[k][a];
        if(old.pending){memory[k][a]=old.data;++writes[k];++collisions;}
        if(!d.ras_n&&old.ras)next.row=d.address&mask;
        if(!d.cas_n&&old.cas)next.col=d.address&mask;
        next.ras=d.ras_n;next.cas=d.cas_n;next.we=d.we_n;
        next.pending=!d.ras_n&&!d.cas_n&&!d.we_n&&(old.cas||old.we);
        if(next.pending)next.data=d.data;
        if(!d.ras_n&&!d.cas_n&&d.we_n){next.dout=old.pipe;++reads[k];}
        state[k]=next;
    }else ++holds;
    edge(run,reference);check(!d.done,"running state request accepted");equal(reference);
}
static void pins(){d.address=random_word()&255;d.ras_n=random_word()&1;d.cas_n=random_word()&1;d.we_n=random_word()&1;d.data=random_word()&1;d.probe_address=random_word()&65535;}
static void program(unsigned a,bool data){
    d.address=a>>8;d.ras_n=1;d.cas_n=1;d.we_n=1;ordinary();
    d.ras_n=0;ordinary();
    d.address=a&255;d.cas_n=0;d.we_n=0;d.data=data;ordinary();
    d.data=!data;ordinary(); // pending data was sampled, not live data
    d.we_n=1;ordinary();ordinary();
}
static unsigned access(unsigned k,unsigned index,bool write,unsigned data=0,bool valid=true){
    d.request=1u<<k;d.write=write?(1u<<k):0;d.state_index[k]=index;d.state_data[k]=data;
    edge(true,false);++requests;
    check(d.frozen==3&&d.done==(1u<<k),"missing held state response");
    check(d.error==(valid?0:(1u<<k)),"invalid state index accepted");return d.result[k];
}
static unsigned history(unsigned k,unsigned word){return(pack(saved_state[k],k)>>(18*word))&transport_mask;}
static void physical(unsigned k,unsigned a,unsigned value){
    d.probe_address=a;d.eval();
    for(unsigned j=0;j<2;++j){
        unsigned actual=(d.candidate_probe>>j)&1,expected=j==k?value:saved_memory[j][a&(depth[j]-1)];
        check(actual==expected,"physical RAM differs");
        check(d.candidate_history[j]==pack(saved_state[j],j),"RAM write changed history");
    }
    d.probe_address=a^1;d.eval();
    for(unsigned j=0;j<2;++j)check(((d.candidate_probe>>j)&1)==saved_memory[j][(a^1)&(depth[j]-1)],"RAM write changed neighboring row");
}
int main(int argc,char**argv){
    Verilated::commandArgs(argc,argv);
    for(unsigned k=0;k<2;++k)for(unsigned a=0;a<depth[k];++a)
        memory[k][a]=((a*1103515245u+12345u+7919u*k)^(a>>3)^(a>>7)^(a>>11)^(a>>15))&1;
    d.host_clk=0;d.ref_clk=0;d.advance=0;d.freeze=0;d.held=0;d.request=3;d.write=3;d.eval();
    ordinary();d.freeze=1;ordinary();check(!d.frozen&&!d.done,"unheld freeze accepted");d.freeze=0;
    for(unsigned a=0;a<65536;++a){d.probe_address=a;program(a,random_word()&1);}
    for(unsigned i=0;i<20000;++i){pins();bool run=(random_word()&3)!=0;ordinary(run,run);}
    // A pending write must survive restore and commit on the FIRST real edge.
    // Keep held output0 distinct from raw pipeline1, and pending data0 distinct
    // from RAM[1]=1. Address histories and every pipeline bit matter here.
    program(1,1);program(0,0);
    d.address=0;d.ras_n=1;d.cas_n=1;d.we_n=1;ordinary();d.ras_n=0;ordinary();
    d.address=1;d.cas_n=0;ordinary();d.we_n=0;d.data=0;ordinary();
    for(unsigned k=0;k<2;++k)check(state[k].pending&&!state[k].dout&&state[k].pipe&&memory[k][1]==1,"pending/read-pipeline boundary setup failed");
    saved_state=state;saved_memory=memory;d.freeze=1;d.held=1;d.request=3;d.write=3;
    edge(true,false);check(d.frozen==3&&!d.done,"entry request accepted");equal();
    for(unsigned k=0;k<2;++k){
        for(unsigned a=0;a<depth[k];++a)check(access(k,a,false)==saved_memory[k][a],"RAM export differs");
        for(unsigned word=0;word<2;++word)check(access(k,depth[k]+word,false)==history(k,word),"history export differs");
    }
    for(unsigned k=0;k<2;++k)for(unsigned a=0;a<depth[k];++a)for(unsigned bit=0;bit<18;++bit){
        unsigned value=saved_memory[k][a]^(1u<<bit);access(k,a,true,value);
        check(access(k,a,false)==(value&1),"RAM bit/padding write readback differs");physical(k,a,value&1);
        access(k,a,true,saved_memory[k][a]);if(bit==0)++walked_bits;
    }
    for(unsigned k=0;k<2;++k)for(unsigned word=0;word<2;++word)for(unsigned bit=0;bit<18;++bit){
        unsigned width=word?address_bits[k]+7-18:18,mask=(1u<<width)-1;
        unsigned value=history(k,word)^(1u<<bit);access(k,depth[k]+word,true,value);
        check(access(k,depth[k]+word,false)==(value&mask),"history bit write/readback differs");
        unsigned expected=pack(saved_state[k],k)^((bit<width)?(1u<<(word*18+bit)):0);
        check(d.candidate_history[k]==expected,"physical history write differs");
        check(d.candidate_history[1-k]==pack(saved_state[1-k],1-k),"history write changed another socket");
        access(k,depth[k]+word,true,history(k,word));if(bit<width)++walked_bits;
    }
    for(unsigned k=0;k<2;++k){
        for(unsigned a=depth[k]+2;a<2*depth[k];++a){access(k,a,true,transport_mask,false);access(k,a,false,0,false);}
        for(unsigned a=0;a<depth[k];++a)check(access(k,a,false)==saved_memory[k][a],"invalid write mutated RAM");
        for(unsigned word=0;word<2;++word)check(access(k,depth[k]+word,false)==history(k,word),"invalid write mutated history");
    }
    d.request=0;d.freeze=0;edge(false,false);d.held=0;
    for(unsigned i=0;i<10000;++i){pins();ordinary(true,false);}
    for(unsigned k=0;k<2;++k)check(d.original_history[k]==pack(saved_state[k],k),"reference advanced during candidate continuation");
    d.freeze=1;d.held=1;edge(false,false);
    // Write/read an independent destructive whole-image pattern before restore.
    for(unsigned k=0;k<2;++k){
        for(unsigned a=0;a<depth[k];++a)access(k,a,true,!saved_memory[k][a]);
        for(unsigned word=0;word<2;++word)access(k,depth[k]+word,true,history(k,word)^transport_mask);
        for(unsigned a=0;a<depth[k];++a){check(access(k,a,false)==!saved_memory[k][a],"destructive RAM readback differs");d.probe_address=a;d.eval();check(((d.candidate_probe>>k)&1)==!saved_memory[k][a],"destructive physical RAM differs");}
        for(unsigned word=0;word<2;++word){unsigned width=word?address_bits[k]+7-18:18;check(access(k,depth[k]+word,false)==((history(k,word)^transport_mask)&((1u<<width)-1)),"destructive history readback differs");}
    }
    for(unsigned k=0;k<2;++k){
        for(unsigned a=0;a<depth[k];++a)access(k,a,true,saved_memory[k][a]);
        for(unsigned word=0;word<2;++word)access(k,depth[k]+word,true,history(k,word));
    }
    memory=saved_memory;state=saved_state;
    for(unsigned a=0;a<65536;++a){d.probe_address=a;d.eval();equal();}
    for(unsigned k=0;k<2;++k)access(k,0,false); // physical raw0 differs from saved logical raw1
    d.freeze=0;d.request=3;d.write=3;edge(false,false);check(!d.done&&!d.frozen,"release request accepted");d.held=0;d.request=0;
    for(unsigned i=0;i<17;++i){pins();ordinary(false,false);}
    d.freeze=1;d.held=1;edge(false,false);
    for(unsigned k=0;k<2;++k)for(unsigned word=0;word<2;++word)check(access(k,depth[k]+word,false)==history(k,word),"recapture before guest advance lost history");
    d.freeze=0;d.request=0;edge(false,false);d.held=0;
    d.ras_n=0;d.cas_n=0;d.we_n=1;ordinary();
    for(unsigned k=0;k<2;++k)check(memory[k][1]==0&&state[k].dout,"first edge did not commit pending write/use saved read pipeline");
    for(unsigned i=0;i<20000;++i){pins();bool run=(random_word()&3)!=0;ordinary(run,run);}
    check(walked_bits==81964&&writes[0]>65536&&writes[1]>65536&&reads[0]>10000&&holds>5000,"insufficient state/writer coverage");
    std::printf("MOSRAM RESTORE PASS host=%u held=%u requests=%u bits=%u collisions=%u writes=%u,%u reads=%u,%u address_bits=14,16 post_restore=20000\n",edges,holds,requests,walked_bits,collisions,writes[0],writes[1],reads[0],reads[1]);
}
