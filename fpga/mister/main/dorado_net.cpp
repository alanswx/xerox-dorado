// Local packet transport only. No Pup server, guest CPU or device emulation.
#include "dorado_net.h"
#include "dorado_ddr.h"
#include "dorado_disk.h"
#include "spi.h"
#include "user_io.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

namespace {
int fd=-1;
char socket_path[108];
uint64_t next_poll=0,next_connect=0;
uint64_t next_trace=0;
unsigned char outgoing[2048],incoming[2049];
unsigned outgoing_bytes=0,incoming_bytes=0;
uint64_t milliseconds(){
    timespec t;clock_gettime(CLOCK_MONOTONIC,&t);
    return uint64_t(t.tv_sec)*1000+t.tv_nsec/1000000;
}
void disconnect(){
    if(fd>=0){close(fd);unlink(socket_path);fd=-1;}
    // The FPGA packet stays pending until the server accepted it. A core
    // change invalidates any local copies; the next core is queried anew.
    outgoing_bytes=0;incoming_bytes=0;
}
bool connect_server(){
    int s=socket(AF_UNIX,SOCK_DGRAM,0);
    if(s<0)return false;
    if(fcntl(s,F_SETFL,O_NONBLOCK) || fcntl(s,F_SETFD,FD_CLOEXEC)){close(s);return false;}
    sockaddr_un local={},server={};local.sun_family=server.sun_family=AF_UNIX;
    snprintf(socket_path,sizeof socket_path,"/tmp/dorado-main-%ld.sock",long(getpid()));
    snprintf(local.sun_path,sizeof local.sun_path,"%s",socket_path);
    const char *server_path=getenv("DORADO_NET_SOCKET");
    if(!server_path)server_path="/tmp/dorado-net.sock";
    if(strlen(server_path)>=sizeof server.sun_path){close(s);return false;}
    strcpy(server.sun_path,server_path);
    // Remove only this process's previous pathname, never the service path.
    unlink(socket_path);
    if(bind(s,reinterpret_cast<sockaddr*>(&local),sizeof local) ||
       connect(s,reinterpret_cast<sockaddr*>(&server),sizeof server) ||
       send(s,"",0,MSG_DONTWAIT)<0){
        close(s);unlink(socket_path);return false;
    }
    chmod(socket_path,0600);
    fd=s;printf("Dorado: ARM-local packet server connected\n");fflush(stdout);return true;
}
}

bool dorado_net_barrier(const char *reason){
    if(!dorado_ddr_barrier(reason))return false;
    disconnect();return true;
}
bool dorado_net_reset(){
    if(!dorado_ddr_reset())return false;
    disconnect();next_poll=next_connect=next_trace=0;return true;
}

void dorado_net_poll(){
    // MGL setname overrides CoreName for configuration/display. Capability
    // belongs to the FPGA's original config-string name, not the launcher.
    if(strcmp(user_io_get_core_name(1),"Dorado")){disconnect();next_connect=0;return;}
    uint64_t now=milliseconds();if(now<next_poll)return;next_poll=now+1;
    uint16_t magic=spi_uio_cmd_cont(0x70);
    uint16_t flags=spi_w(0);
    if(magic==0xd074 && now>=next_trace && getenv("DORADO_NET_TRACE")){
        next_trace=now+1000;
        unsigned length=spi_w(0),good=spi_w(0),bad=spi_w(0),dropped=spi_w(0),rx_bad=spi_w(0),aborted=spi_w(0);
        unsigned debug_lo=spi_w(0),debug_hi=spi_w(0),machine_status=spi_w(0);
        unsigned cpu_lo=spi_w(0),cpu_hi=spi_w(0);
        printf("Dorado net: flags=%x tx_words=%u good=%u crc_bad=%u dropped=%u rx_bad=%u aborted=%u machine=%04x%04x state=%04x cpu=%04x%04x\n",
               flags,length,good,bad,dropped,rx_bad,aborted,debug_hi,debug_lo,machine_status,cpu_hi,cpu_lo);fflush(stdout);
    }
    DisableIO();
    if(magic!=0xd074){disconnect();next_connect=now+500;return;}
    if((flags&8) && (getenv("DORADO_NET_DDR") || dorado_ddr_needs_barrier()) && !dorado_disk_prepare_exit()){
        disconnect();return;
    }
    if(dorado_ddr_poll(flags)){disconnect();return;}
    if(fd<0 && now<next_connect)return;
    if(fd<0){next_connect=now+1000;if(!connect_server())return;}

    if(!outgoing_bytes && (flags&2)){
        unsigned words=spi_uio_cmd_cont(0x71);
        if(words<2||words>((flags&4)?1024u:512u)){DisableIO();return;}
        for(unsigned i=0;i<words;i++){
            uint16_t w=spi_w(0);outgoing[2*i]=w>>8;outgoing[2*i+1]=w;
        }
        DisableIO();outgoing_bytes=words*2;
    }
    if(outgoing_bytes){
        ssize_t n=send(fd,outgoing,outgoing_bytes,MSG_DONTWAIT);
        if(n==ssize_t(outgoing_bytes)){
            spi_uio_cmd(0x72);outgoing_bytes=0;
        }else if(n<0 && errno!=EAGAIN && errno!=ENOBUFS && errno!=EINTR){
            disconnect();next_connect=now+1000;return;
        }
    }
    if(!incoming_bytes){
        ssize_t n=recv(fd,incoming,sizeof incoming,MSG_DONTWAIT);
        if(n>=4 && n<=((flags&4)?2048:1024) && !(n&1))incoming_bytes=unsigned(n);
        else if(n<0 && errno!=EAGAIN && errno!=EINTR){
            disconnect();next_connect=now+1000;return;
        }
    }
    if(incoming_bytes && (flags&1)){
        bool ready=spi_uio_cmd_cont(0x73)&1;
        if(ready){
            spi_w(incoming_bytes/2);
            for(unsigned i=0;i<incoming_bytes;i+=2)spi_w((incoming[i]<<8)|incoming[i+1]);
        }
        DisableIO();if(ready)incoming_bytes=0;
    }
}
