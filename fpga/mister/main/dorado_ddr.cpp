#include "dorado_ddr.h"
#include "dorado_control_client.h"
#include "dorado_ddr_control.h"
#include "spi.h"
#include "user_io.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

namespace {
enum class State { Idle, Connect, StopAck, Drain, Init, ResumeAck, StatusAck, Active, Disabled, Fault, Quiesced };
State state=State::Idle;
DoradoControlClient client;
bool selected=false,desired=false,reported=false;
uint64_t deadline=0,next_connect=0,next_status=0,epoch=0;
struct Status {
    uint16_t flags=0;
    uint64_t epoch=0;
    uint32_t fault=0,errors=0;
    bool stopped()const{return flags&1;}
    bool initialized()const{return flags&2;}
    bool enabled()const{return flags&4;}
    bool owned()const{return flags&16;}
};
uint64_t milliseconds(){timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return uint64_t(t.tv_sec)*1000+t.tv_nsec/1000000;}
bool core(){return !strcmp(user_io_get_core_name(1),"Dorado");}
bool option(){const char *p=getenv("DORADO_NET_DDR");return p && !strcmp(p,"1");}
bool status(Status &s){
    uint16_t w[18];w[0]=spi_uio_cmd_cont(0x74);
    for(unsigned i=1;i<18;i++)w[i]=spi_w(0);
    DisableIO();
    if(w[0]!=0xd0d4 || w[1]!=1 || w[2]!=0 || w[3]!=0x1ff0 ||
       w[4]!=0 || w[5]!=1 || w[13]!=2048 || w[14]!=8 || w[15]!=16 || (w[6]&~31))return false;
    s.flags=w[6];s.fault=uint32_t(w[7])|(uint32_t(w[8])<<16);
    s.epoch=uint64_t(w[9])|(uint64_t(w[10])<<16)|(uint64_t(w[11])<<32)|(uint64_t(w[12])<<48);
    s.errors=uint32_t(w[16])|(uint32_t(w[17])<<16);return true;
}
bool control(bool start,uint64_t cookie){
    const uint16_t magic=spi_uio_cmd_cont(0x75);
    spi_w(start?1:0);
    for(unsigned i=0;i<4;i++)spi_w(uint16_t(cookie>>(16*i)));
    DisableIO();return magic==0xd0d4;
}
int connect(){
    char path[108];const char *p=getenv("DORADO_NET_CONTROL");
    if(!p){const char *base=getenv("DORADO_NET_SOCKET");if(!base)base="/tmp/dorado-net.sock";
        int n=snprintf(path,sizeof path,"%s.control",base);if(n<0 || size_t(n)>=sizeof path)return -ENAMETOOLONG;p=path;}
    return client.open(p);
}
bool fresh_epoch(uint64_t old){
    int fd=open("/dev/urandom",O_RDONLY|O_CLOEXEC);if(fd<0)return false;
    unsigned used=0;uint64_t value=0;
    while(used<sizeof value){ssize_t n=read(fd,reinterpret_cast<unsigned char *>(&value)+used,sizeof value-used);
        if(n<0 && errno==EINTR)continue;
        if(n<=0){close(fd);return false;}used+=unsigned(n);}
    close(fd);if(!value || value==old)return false;epoch=value;return true;
}
bool fault(const char *why){
    if(!reported){fprintf(stderr,"Dorado DDR: %s; packet ownership held, lifecycle barrier required\n",why);reported=true;}
    state=State::Fault;selected=true;return true;
}
}

bool dorado_ddr_needs_barrier(){return selected;}

bool dorado_ddr_poll(uint16_t flags){
    if(state==State::Quiesced)return true; // Stay stopped until explicit init/reset.
    if(!(flags&8))return selected?fault("capability disappeared"):false;
    const uint64_t now=milliseconds();
    if(state==State::Idle){
        Status s;if(!status(s))return fault("invalid capability record");
        desired=option();
        if(!desired && s.stopped() && !s.owned())return false;
        selected=true;state=State::Connect;next_connect=0;
    }
    if(state==State::Disabled)return false;
    if(state==State::Fault)return true;
    if(state==State::Connect){
        if(now<next_connect)return true;
        next_connect=now+1000;
        if(connect())return true; // No FPGA grant while the controller is absent.
        if(client.begin(DORADO_CTRL_STOP))return fault("cannot request daemon STOP");
        state=State::StopAck;deadline=now+5000;
    }
    if(state==State::StopAck || state==State::ResumeAck || state==State::StatusAck){
        int r=client.pump();if(r<0)return fault("daemon control disconnected");
        if(!r){if(now>=deadline)return fault("daemon acknowledgement timed out");return true;}
        if(client.reply().error)return fault("daemon rejected control request");
        if(state==State::StopAck){
            if(!control(false,0))return fault("FPGA STOP command rejected");
            state=State::Drain;deadline=now+5000;
        }else{
            if(!client.reply().active || client.reply().epoch!=epoch)return fault("daemon lost active epoch");
            if(state==State::ResumeAck)
                printf("Dorado DDR: direct frame queues active, epoch %016llx\n",(unsigned long long)epoch);
            state=State::Active;next_status=now+1000;
        }
    }
    if(state==State::Drain || state==State::Init){
        Status s;if(!status(s))return fault("invalid FPGA state");
        if(s.fault || s.errors)return fault("FPGA transport/control fault");
        if(state==State::Drain && s.stopped() && !s.owned() && !s.enabled()){
            if(!desired){client.close();selected=false;state=State::Disabled;return false;}
            if(!fresh_epoch(s.epoch) || !control(true,epoch))return fault("cannot grant fresh FPGA epoch");
            state=State::Init;deadline=now+5000;return true;
        }
        if(state==State::Init && s.initialized() && s.enabled() && s.owned() && s.epoch==epoch){
            if(client.begin(DORADO_CTRL_RESUME,epoch))return fault("cannot grant daemon epoch");
            state=State::ResumeAck;deadline=now+5000;
        }else if(now>=deadline)return fault("FPGA drain/initialization timed out");
    }
    if(state==State::Active && now>=next_status){
        next_status=now+1000;Status s;
        if(!status(s) || s.fault || s.errors || !s.initialized() || !s.enabled() || !s.owned() || s.epoch!=epoch)
            return fault("active FPGA transport lost");
        if(client.begin(DORADO_CTRL_STATUS))return fault("cannot request daemon status");
        state=State::StatusAck;deadline=now+5000;
    }
    return true;
}

bool dorado_ddr_barrier(const char *reason,unsigned timeout_ms){
    if(!core()){
        if(selected){fprintf(stderr,"Dorado DDR: refusing %s: core changed before STOP\n",reason);return false;}
        return true;
    }
    // Probe independently of the environment/manager state. A new Main may
    // inherit an already-running FPGA engine from its previous process.
    uint16_t magic=spi_uio_cmd_cont(0x70),flags=spi_w(0);DisableIO();
    if(magic!=0xd074 || !(flags&8))return !selected;
    Status s;if(!status(s))return false;
    if(!selected && s.stopped() && !s.owned() && !s.enabled())return true;
    selected=true;
    const uint64_t limit=milliseconds()+timeout_ms;
    if(!client.connected() && connect())return false;
    int result=client.stop(timeout_ms);
    if(result){fprintf(stderr,"Dorado DDR: refusing %s: daemon STOP error %d\n",reason,-result);return false;}
    if(!control(false,0))return false;
    for(;;){
        if(!status(s))return false;
        if(s.stopped() && !s.owned() && !s.enabled()){
            client.close();selected=false;state=State::Quiesced;return true;
        }
        if(milliseconds()>=limit){fprintf(stderr,"Dorado DDR: refusing %s: FPGA has not drained\n",reason);return false;}
        usleep(1000);
    }
}

bool dorado_ddr_reset(){
    if(selected)return false;
    client.close();state=State::Idle;reported=false;desired=false;
    next_connect=next_status=deadline=epoch=0;return true;
}
