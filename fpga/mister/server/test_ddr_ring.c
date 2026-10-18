#define _GNU_SOURCE
#include "dorado_ddr_ring.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static unsigned char *memory;
static unsigned char before[65536],expected[65536],payload[2048],output[2112];
static const uint64_t epoch=UINT64_C(0x5dba1234a2e6f9c8);
static uint32_t random_state=0x31415926;
static uint32_t rng(void) {
    random_state^=random_state<<13;random_state^=random_state>>17;
    return random_state^=random_state<<5;
}
static void require(int ok,const char *why) {
    if(!ok){fprintf(stderr,"DDR RING FAIL: %s\n",why);exit(23);}
}
/* Independent byte oracle: no implementation load/store/pair helpers. */
static void put32(unsigned char *p,unsigned off,uint32_t v) {
    for(unsigned i=0;i<4;i++)p[off+i]=(unsigned char)(v>>(8*i));
}
static uint32_t get32(const unsigned char *p,unsigned off) {
    uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)p[off+i]<<(8*i);return v;
}
static void put64(unsigned char *p,unsigned off,uint64_t v) {
    put32(p,off,(uint32_t)v);put32(p,off+4,(uint32_t)(v>>32));
}
static void seed(uint32_t tx,uint32_t rx) {
    memset(memory,0xa5,65536);
    /* Deliberately literal contract values, independent of library offsets. */
    put64(memory,0,UINT64_C(0x31474e49524f4444));put64(memory,8,epoch);
    put32(memory,0x10,tx);put32(memory,0x18,tx);
    put32(memory,0x20,rx);put32(memory,0x28,rx);
    put32(memory,0x30,1);put64(memory,0x38,UINT64_C(0x0001080000100008));
}
static unsigned data_for(uint32_t sequence) {
    unsigned n=sequence%37==0?2048:sequence%29==0?6:4+2*(rng()%1023);
    for(unsigned i=0;i<n;i++)payload[i]=(unsigned char)(i*53+(sequence>>8)+sequence);
    return n;
}
static void descriptor(unsigned char *p,unsigned off,unsigned n,uint32_t seq) {
    put32(p,off,n);put32(p,off+4,seq);put64(p,off+8,epoch);
}
static void host_send(void) {
    dorado_ddr_ring ring;uint32_t producer=UINT32_MAX-23,consumer=producer;
    seed(0,producer);memcpy(before,memory,65536);
    require(!dorado_ddr_bind(&ring,memory,65536,epoch),"bind RX");
    require(!memcmp(before,memory,65536),"bind wrote mailbox");
    unsigned full=0;
    for(unsigned i=0;i<8192;i++) {
        unsigned n=data_for(producer),slot=producer&15;
        memcpy(before,memory,65536);memcpy(expected,memory,65536);
        int result=dorado_ddr_send(&ring,payload,n);
        if(producer-consumer==16) {
            require(result==-EAGAIN,"RX full not bounded");
            require(!memcmp(before,memory,65536),"full RX wrote mailbox");full++;
        } else {
            require(!result,"RX enqueue failed");
            unsigned off=0x5000+slot*2048;
            memcpy(expected+off,payload,n);
            if(n&3)memset(expected+off+n,0,4-(n&3));
            descriptor(expected,0x200+slot*16,n,producer);
            put32(expected,0x20,producer+1);
            require(!memcmp(expected,memory,65536),"RX byte oracle/adjacent slot/descriptor/ownership mismatch");
            producer++;
        }
        /* Independently model FPGA consumption, including irregular stalls. */
        if(producer!=consumer && ((rng()&7)<3 || i%31==0)) {
            unsigned off=0x200+(consumer&15)*16;
            require(get32(memory,off+4)==consumer,"RX sequence");
            require(get32(memory,off+8)==(uint32_t)epoch,"RX descriptor epoch");
            consumer++;put32(memory,0x28,consumer);
        }
    }
    require(full>1000,"RX full coverage");
    require(producer<8192,"RX32-bit wrap coverage");
    dorado_ddr_quiesce(&ring);
}
static void host_receive(void) {
    dorado_ddr_ring ring;uint32_t producer=UINT32_MAX-15,consumer=producer;
    seed(producer,0);require(!dorado_ddr_bind(&ring,memory,65536,epoch),"bind TX");
    unsigned empty=0;
    for(unsigned i=0;i<8192;i++) {
        /* Independent FPGA producer writes bytes/descriptor then ownership. */
        if(producer-consumer<8 && (rng()&3)) {
            unsigned n=data_for(producer),slot=producer&7;
            memcpy(memory+0x1000+slot*2048,payload,n);
            descriptor(memory,0x100+slot*16,n,producer);
            producer++;put32(memory,0x10,producer);
        }
        memcpy(before,memory,65536);memset(output,0x7d,sizeof output);
        if(producer==consumer) {
            require(dorado_ddr_receive(&ring,output+32,2048)==-EAGAIN,"TX empty");empty++;
            require(!memcmp(before,memory,65536),"empty TX changed ownership");
            continue;
        }
        unsigned slot=consumer&7,n=get32(memory,0x100+slot*16);
        if(i%19==0) {
            require(dorado_ddr_receive(&ring,output+32,n-1)==-EMSGSIZE,"small destination consumed frame");
            require(!memcmp(before,memory,65536),"small destination changed ownership");
            for(unsigned j=0;j<sizeof output;j++)require(output[j]==0x7d,"small destination changed bytes");
        }
        require(dorado_ddr_receive(&ring,output+32,2048)==(int)n,"TX dequeue");
        require(!memcmp(output+32,memory+0x1000+slot*2048,n),"TX byte order/content");
        for(unsigned j=0;j<32;j++)require(output[j]==0x7d,"TX prefix guard");
        for(unsigned j=32+n;j<sizeof output;j++)require(output[j]==0x7d,"TX suffix guard");
        put32(before,0x18,consumer+1);
        require(!memcmp(before,memory,65536),"TX unexpected MMIO write");consumer++;
    }
    require(empty>1000 && consumer<8192,"TX empty/wrap coverage");
    dorado_ddr_quiesce(&ring);
}
static void fault_tests(void) {
    dorado_ddr_ring ring;
    require(dorado_ddr_bind(NULL,memory,65536,epoch)==-EINVAL,"null context");
    require(dorado_ddr_bind(&ring,memory+1,65536,epoch)==-EINVAL,"unaligned mapping");
    require(dorado_ddr_bind(&ring,memory,65535,epoch)==-EINVAL,"short mapping");
    require(dorado_ddr_bind(&ring,memory,65536,0)==-EINVAL,"zero epoch");
    unsigned fields[]={0,4,8,12,0x30,0x38,0x3c};
    for(unsigned i=0;i<sizeof fields/sizeof fields[0];i++) {
        seed(0,0);memory[fields[i]]^=0x80;memcpy(before,memory,65536);
        require(dorado_ddr_bind(&ring,memory,65536,epoch)==-ESTALE,"bad/stale header accepted");
        require(!memcmp(before,memory,65536),"failed bind mutated mailbox");
        seed(0,0);require(!dorado_ddr_bind(&ring,memory,65536,epoch),"fault bind");
        memory[fields[i]]^=0x80;memcpy(before,memory,65536);
        require(dorado_ddr_send(&ring,payload,2048)==-ESTALE,"live stale header accepted");
        require(dorado_ddr_receive(&ring,output,2048)==-ENOTCONN,"fault auto resumed");
        require(!memcmp(before,memory,65536),"stale grant published data");
    }
    for(unsigned test=0;test<9;test++) {
        seed(0,0);require(!dorado_ddr_bind(&ring,memory,65536,epoch),"malformed bind");
        descriptor(memory,0x100,4,0);put32(memory,0x10,1);
        switch(test) {
        case 0:put32(memory,0x100,2049);break;
        case 1:put32(memory,0x100,2050);break;
        case 2:put32(memory,0x100,2);break;
        case 3:put32(memory,0x104,1);break;
        case 4:memory[0x108]^=1;break;
        case 5:memory[0x10c]^=1;break;
        case 6:put32(memory,0x10,9);break;
        case 7:put32(memory,0x18,1);break; /* another daemon */
        case 8:put32(memory,0x20,1);break;
        }
        memcpy(before,memory,65536);
        require(dorado_ddr_receive(&ring,output,2048)==-EPROTO,"malformed TX accepted");
        require(!memcmp(before,memory,65536),"malformed TX changed ownership");
    }
    seed(0,0);put32(memory,0x28,UINT32_MAX);
    require(dorado_ddr_bind(&ring,memory,65536,epoch)==0,"valid wrapped RX occupancy rejected");
    put32(memory,0x28,UINT32_MAX-16);memcpy(before,memory,65536);
    require(dorado_ddr_send(&ring,payload,4)==-EPROTO,"impossible RX occupancy accepted");
    require(!memcmp(before,memory,65536),"bad RX occupancy wrote");
    seed(0,0);require(!dorado_ddr_bind(&ring,memory,65536,epoch),"stop bind");
    require(dorado_ddr_send(&ring,payload,3)==-EINVAL,"odd length");
    require(dorado_ddr_send(&ring,payload,2050)==-EINVAL,"oversize");
    dorado_ddr_quiesce(&ring);
    require(!mprotect(memory,65536,PROT_NONE),"protect stopped mailbox");
    require(dorado_ddr_send(&ring,payload,4)==-ENOTCONN,"send after stop");
    require(dorado_ddr_receive(&ring,output,2048)==-ENOTCONN,"read after stop");
    require(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"unprotect mailbox");
}
int main(void) {
    long page=sysconf(_SC_PAGESIZE);
    require(page>0 && !(65536%page),"page size");
    void *allocation=mmap(NULL,65536+2*(size_t)page,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    require(allocation!=MAP_FAILED,"guard allocation");memory=(unsigned char *)allocation+page;
    require(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"map mailbox");
    host_send();host_receive();fault_tests();
    require(!munmap(allocation,65536+2*(size_t)page),"unmap");
    puts("DDR RING HOST PASS:16384 oracle attempts;2048-byte payloads, full/empty,32-bit wrap, descriptor epoch/sequence, guards, malformed/stale/ownership faults and stopped-memory access");
}
