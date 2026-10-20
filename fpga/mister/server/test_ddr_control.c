#define _GNU_SOURCE
#include "dorado_ddr_control.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

static dorado_ddr_control service;
static unsigned char *memory;
static unsigned opens,closes,close_fail,events;
static const uint64_t epoch=UINT64_C(0x9a56ed77003125af);
static void check(int ok,const char *why){if(!ok){fprintf(stderr,"DDR CONTROL FAIL: %s\n",why);exit(23);}}
static void put32(unsigned char *p,uint32_t v){for(unsigned i=0;i<4;i++)p[i]=(unsigned char)(v>>(8*i));}
static void put64(unsigned char *p,uint64_t v){put32(p,(uint32_t)v);put32(p+4,(uint32_t)(v>>32));}
static uint32_t get32(const unsigned char *p){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)p[i]<<(8*i);return v;}
static int open_mapping(void *unused,volatile void **out){
    (void)unused;opens++;check(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"map oracle");*out=memory;return 0;
}
static int close_mapping(void *unused,volatile void *p){
    (void)unused;check(p==memory,"wrong mapping close");closes++;
    check(!service.ring.active && !service.ring.memory && !service.granted,"cleanup before quiesce");
    check(!mprotect(memory,65536,PROT_NONE),"protect stopped oracle");
    return close_fail?EIO:0;
}
static void seed(uint64_t cookie){
    check(!service.granted,"seed while granted");
    check(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"seed protection");memset(memory,0xa5,65536);
    put64(memory,UINT64_C(0x31474e49524f4444));put64(memory+8,cookie);
    put32(memory+0x10,0);put32(memory+0x18,0);put32(memory+0x20,0);put32(memory+0x28,0);
    put32(memory+0x30,1);put64(memory+0x38,UINT64_C(0x0001080000100008));
    check(!mprotect(memory,65536,PROT_NONE),"seal seed");
}
static void pump(unsigned count){while(count--)events|=dorado_ddr_control_poll(&service);}
static int connect_client(const char *path){
    int fd=socket(AF_UNIX,SOCK_STREAM,0);check(fd>=0,"client socket");
    struct sockaddr_un address={0};address.sun_family=AF_UNIX;strcpy(address.sun_path,path);
    check(!connect(fd,(struct sockaddr *)&address,sizeof address),"client connect");pump(2);return fd;
}
static void packet(unsigned char *p,uint32_t command,uint32_t seq,uint64_t cookie){
    put64(p,UINT64_C(0x00314c5254434444));put32(p+8,command);put32(p+12,seq);put64(p+16,cookie);
    put32(p+24,0x1ff00000);put32(p+28,0x10000);
}
static void send_bytes(int fd,const unsigned char *p,unsigned count){
    check(send(fd,p,count,0)==(ssize_t)count,"client send");
}
static void response(int fd,unsigned char *p){
    unsigned used=0;
    for(unsigned i=0;i<1000 && used<32;i++){
        pump(1);ssize_t n=recv(fd,p+used,32-used,MSG_DONTWAIT);
        if(n>0)used+=(unsigned)n;
        else check(n<0&&(errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR),"missing reply/EOF");
    }
    check(used==32,"response timeout");
}
static void rpc(int fd,uint32_t command,uint32_t seq,uint64_t cookie,int error,int active){
    unsigned char p[32],r[32];packet(p,command,seq,cookie);send_bytes(fd,p,32);response(fd,r);
    check(get32(r+8)==(command|0x80000000u)&&get32(r+12)==seq,"response command/sequence");
    check(get32(r+24)==(unsigned)error && get32(r+28)==(unsigned)active,"false success/error state");
}
int main(void){
    signal(SIGPIPE,SIG_IGN);
    memory=mmap(NULL,65536,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);check(memory!=MAP_FAILED,"oracle allocation");
    char directory[]="/tmp/dorado-control-XXXXXX";check(mkdtemp(directory)!=NULL,"private directory");
    char path[108];snprintf(path,sizeof path,"%s/control.sock",directory);
    dorado_ddr_mapper mapper={open_mapping,close_mapping,NULL};
    int error=dorado_ddr_control_open(&service,path,mapper);
    if(error)fprintf(stderr,"control open error: %s (%d)\n",strerror(error),error);
    check(!error,"control open");
    struct stat st;check(!stat(path,&st) && (st.st_mode&0777)==0600,"control permissions");
    dorado_ddr_control other;check(dorado_ddr_control_open(&other,path,mapper)==EADDRINUSE,"stole existing endpoint");
    int fd=connect_client(path);unsigned char p[32],r[32];
    packet(p,2,1,epoch);send_bytes(fd,p,31);pump(8);check(!opens&&!service.granted,"partial grant mapped");
    close(fd);pump(3);check(!opens&&service.client<0,"partial EOF granted");
    fd=connect_client(path);seed(epoch);
    packet(p,2,1,epoch);put32(p+24,0x30000000);send_bytes(fd,p,32);response(fd,r);
    check(get32(r+24)==EINVAL && !opens,"arbitrary mapping address accepted");
    rpc(fd,2,2,epoch+1,ESTALE,0);check(opens==1 && closes==1,"stale header mapping leaked");
    seed(epoch);events=0;packet(p,2,3,epoch);
    send_bytes(fd,p,3);pump(4);send_bytes(fd,p+3,6);pump(4);
    check(opens==1&&!service.granted,"fragment prefix granted");
    send_bytes(fd,p+9,23);response(fd,r);
    check(!get32(r+24) && get32(r+28)==1 && opens==2 && service.granted,"fragmented valid grant");
    check(events&DORADO_CTRL_RESUMED_EVENT,"resume event missing");
    events=0;send_bytes(fd,p,32);response(fd,r);
    check(opens==2&&!events && get32(r+28)==1,"duplicate resume remapped/reset service");
    int second=connect_client(path);unsigned char byte;
    check(recv(second,&byte,1,MSG_DONTWAIT)==0 && service.granted,"second controller took ownership");close(second);
    close_fail=1;rpc(fd,1,4,0,EIO,0);
    check(!service.granted && !service.ring.active && service.mapped,"failed cleanup claimed complete");
    unsigned char data[4]={1,2,3,4};
    check(dorado_ddr_send(&service.ring,data,4)==-ENOTCONN,"MMIO after failed cleanup/stop");
    close_fail=0;rpc(fd,1,5,0,0,0);check(!service.mapped,"retry failed cleanup");
    rpc(fd,2,6,epoch,EINVAL,0);check(opens==2,"stale grant reopened");
    seed(epoch+2);rpc(fd,2,7,epoch+2,0,1);check(service.granted,"fresh resume failed");
    close(fd);pump(4);check(!service.granted && !service.mapped,"disconnect retained access");
    fd=connect_client(path);seed(epoch+3);rpc(fd,2,0xfffffffeu,epoch+3,0,1);
    rpc(fd,3,0xffffffffu,0,0,1);rpc(fd,3,1,0,0,1); /* sequence wrap */
    packet(p,1,0xffffffffu,0);send_bytes(fd,p,32);pump(4);
    check(!service.granted && service.client<0,"old sequence retained owner");close(fd);
    fd=connect_client(path);seed(epoch+4);rpc(fd,2,1,epoch+4,0,1);
    /* A data-path session fault invalidates cached active grant replies. */
    service.ring.active=0;pump(2);check(!service.granted&&!service.mapped,"ring fault not quiesced");
    rpc(fd,2,1,epoch+4,EINVAL,0);
    packet(p,2,2,epoch+5);p[0]^=1;send_bytes(fd,p,32);pump(3);
    check(service.client<0&&!service.granted,"malformed magic retained controller");close(fd);
    /* Snapshot pause is a different operation from lifecycle STOP/RESUME.
     * Pending frames and both private cursors must survive without mapping
     * again or polling any DDR byte while the pause is acknowledged. */
    fd=connect_client(path);seed(epoch+6);rpc(fd,DORADO_CTRL_RESUME,1,epoch+6,0,1);
    unsigned pause_opens=opens,pause_closes=closes;
    check(!dorado_ddr_send(&service.ring,data,4),"seed pending RX");
    put32(memory+DORADO_DDR_TX_DESC,4);put32(memory+DORADO_DDR_TX_DESC+4,0);
    put64(memory+DORADO_DDR_TX_DESC+8,epoch+6);
    memcpy(memory+DORADO_DDR_TX_DATA,data,4);put32(memory+DORADO_DDR_TX_PROD,1);
    unsigned char saved[65536];memcpy(saved,memory,sizeof saved);
    events=0;rpc(fd,DORADO_CTRL_PAUSE,2,epoch+6,0,3);
    check(events==DORADO_CTRL_PAUSED_EVENT && service.ring.paused,"pause event/state differs");
    check(opens==pause_opens && closes==pause_closes && service.mapped==memory,"pause remapped/unmapped");
    check(!mprotect(memory,65536,PROT_NONE),"protect paused DDR");
    events=0;rpc(fd,DORADO_CTRL_STATUS,3,0,0,3);
    rpc(fd,DORADO_CTRL_PAUSE,4,epoch+6,0,3);
    packet(p,DORADO_CTRL_PAUSE,4,epoch+6);send_bytes(fd,p,32);response(fd,r);
    check(!events && get32(r+28)==3,"duplicate pause changed continuation");
    unsigned char receive[4]={9,9,9,9};
    check(dorado_ddr_send(&service.ring,data,4)==-EAGAIN,"paused send touched DDR");
    check(dorado_ddr_receive(&service.ring,receive,4)==-EAGAIN && receive[0]==9,"paused receive touched DDR/destination");
    rpc(fd,DORADO_CTRL_CONTINUE,5,epoch+7,ESTALE,3);
    packet(p,DORADO_CTRL_PAUSE,6,epoch+6);put32(p+24,0x30000000);send_bytes(fd,p,32);response(fd,r);
    check(get32(r+24)==EINVAL && get32(r+28)==3,"pause accepted arbitrary aperture");
    second=connect_client(path);check(recv(second,&byte,1,MSG_DONTWAIT)==0 && service.ring.paused,"paused owner was replaced");close(second);
    check(!mprotect(memory,65536,PROT_READ|PROT_WRITE),"restore paused DDR access");
    check(!memcmp(saved,memory,sizeof saved),"paused control changed queued bytes/cursors");
    events=0;rpc(fd,DORADO_CTRL_CONTINUE,7,epoch+6,0,1);
    check(events==DORADO_CTRL_CONTINUED_EVENT && !service.ring.paused,"continue reset lifecycle");
    check(opens==pause_opens && closes==pause_closes && !memcmp(saved,memory,sizeof saved),"continue remapped or changed ring state");
    check(service.ring.tx_cons==0 && service.ring.rx_prod==1,"continue lost private cursors");
    events=0;packet(p,DORADO_CTRL_CONTINUE,7,epoch+6);send_bytes(fd,p,32);response(fd,r);
    check(!events && !get32(r+24) && get32(r+28)==1,"duplicate continue changed state");
    rpc(fd,DORADO_CTRL_CONTINUE,8,epoch+6,EINVAL,1);
    check(dorado_ddr_receive(&service.ring,receive,4)==4 && !memcmp(receive,data,4),"pending TX did not survive pause");
    rpc(fd,DORADO_CTRL_PAUSE,9,epoch+6,0,3);put64(memory+DORADO_DDR_EPOCH_OFF,epoch+7);
    events=0;rpc(fd,DORADO_CTRL_CONTINUE,10,epoch+6,ESTALE,0);
    check(events==DORADO_CTRL_STOPPED_EVENT && !service.mapped && !service.ring.active,"stale paused ring retained grant");
    seed(epoch+7);rpc(fd,DORADO_CTRL_RESUME,11,epoch+7,0,1);
    rpc(fd,DORADO_CTRL_PAUSE,12,epoch+7,0,3);close(fd);pump(4);
    check(!service.granted && !service.mapped && !service.ring.paused,"paused EOF did not revoke mapping");
    check(!dorado_ddr_control_close(&service),"control close");
    check(access(path,F_OK)<0 && errno==ENOENT,"socket not removed");
    check(!rmdir(directory)&&!munmap(memory,65536),"cleanup");
    printf("DDR CONTROL PASS: fragments/EOF, ownership, epochs/sequences, failed cleanup, protected stopped/paused memory, pending-frame/cursor preservation, duplicate pause/continue, stale continuation and endpoint lifetime; opens=%u closes=%u\n",opens,closes);
}
