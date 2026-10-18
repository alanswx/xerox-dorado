#include "dorado_control_client.h"
// Copy these two protocol headers alongside Main's Dorado sources when
// assembling its pinned build. They contain the daemon's exact wire format.
#include "dorado_ddr_control.h"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif
namespace {
uint64_t milliseconds(){timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return uint64_t(t.tv_sec)*1000+t.tv_nsec/1000000;}
uint32_t get32(const unsigned char *p){uint32_t n=0;for(unsigned i=0;i<4;i++)n|=uint32_t(p[i])<<(8*i);return n;}
uint64_t get64(const unsigned char *p){return uint64_t(get32(p+4))<<32|get32(p);}
void put32(unsigned char *p,uint32_t n){for(unsigned i=0;i<4;i++)p[i]=(unsigned char)(n>>(8*i));}
void put64(unsigned char *p,uint64_t n){put32(p,uint32_t(n));put32(p+4,uint32_t(n>>32));}
bool peer_is_owner(int fd){
    uid_t uid;
#ifdef __linux__
    ucred credentials;socklen_t size=sizeof credentials;
    if(getsockopt(fd,SOL_SOCKET,SO_PEERCRED,&credentials,&size))return false;
    uid=credentials.uid;
#else
    gid_t gid;if(getpeereid(fd,&uid,&gid))return false;
#endif
    return uid==geteuid();
}
}
void DoradoControlClient::close(){
    if(fd_>=0)::close(fd_);
    fd_=-1;connecting_=pending_=false;sent_=received_=0;sequence_=0;reply_={};
}
int DoradoControlClient::fail(int error){close();return -error;}
int DoradoControlClient::open(const char *path){
    close();sockaddr_un address={};address.sun_family=AF_UNIX;
    if(!path || strlen(path)>=sizeof address.sun_path)return -EINVAL;
    strcpy(address.sun_path,path);
    fd_=socket(AF_UNIX,SOCK_STREAM,0);if(fd_<0)return -errno;
    int flags=fcntl(fd_,F_GETFL);
    if(flags<0 || fcntl(fd_,F_SETFL,flags|O_NONBLOCK) || fcntl(fd_,F_SETFD,FD_CLOEXEC))return fail(errno);
#ifdef SO_NOSIGPIPE
    int one=1;if(setsockopt(fd_,SOL_SOCKET,SO_NOSIGPIPE,&one,sizeof one))return fail(errno);
#endif
    if(connect(fd_,reinterpret_cast<sockaddr *>(&address),sizeof address)) {
        if(errno!=EINPROGRESS && errno!=EAGAIN)return fail(errno);
        connecting_=true;
    } else if(!peer_is_owner(fd_))return fail(EPERM);
    return 0;
}
int DoradoControlClient::begin(uint32_t command,uint64_t epoch){
    if(fd_<0)return -ENOTCONN;
    if(pending_)return -EBUSY;
    if(command!=DORADO_CTRL_STOP && command!=DORADO_CTRL_STATUS && command!=DORADO_CTRL_RESUME)return -EINVAL;
    if(command==DORADO_CTRL_RESUME && !epoch)return -EINVAL;
    if(++sequence_==0)++sequence_;
    command_=command;epoch_=epoch;sent_=received_=0;pending_=true;reply_={};
    put64(output_,DORADO_CTRL_MAGIC);put32(output_+8,command);put32(output_+12,sequence_);
    put64(output_+16,epoch);put32(output_+24,DORADO_DDR_BASE);put32(output_+28,DORADO_DDR_SIZE);
    return 0;
}
int DoradoControlClient::pump(){
    if(fd_<0)return -ENOTCONN;
    if(connecting_) {
        pollfd p={fd_,POLLOUT,0};int n=poll(&p,1,0);
        if(n<0){if(errno==EINTR)return 0;return fail(errno);}
        if(!n)return 0;
        int error=0;socklen_t size=sizeof error;
        if(getsockopt(fd_,SOL_SOCKET,SO_ERROR,&error,&size))return fail(errno);
        if(error)return fail(error);
        if(!peer_is_owner(fd_))return fail(EPERM);
        connecting_=false;
    }
    if(!pending_)return 1;
    if(sent_<32) {
        ssize_t n=io_.send?io_.send(fd_,output_+sent_,32-sent_,MSG_DONTWAIT|MSG_NOSIGNAL):
            ::send(fd_,output_+sent_,32-sent_,MSG_DONTWAIT|MSG_NOSIGNAL);
        if(n<0){if(errno==EINTR || errno==EAGAIN || errno==EWOULDBLOCK)return 0;return fail(errno);}
        if(!n)return fail(EPIPE);
        sent_+=unsigned(n);if(sent_<32)return 0;
    }
    ssize_t n=io_.receive?io_.receive(fd_,input_+received_,32-received_,MSG_DONTWAIT):
        recv(fd_,input_+received_,32-received_,MSG_DONTWAIT);
    if(n<0){if(errno==EINTR || errno==EAGAIN || errno==EWOULDBLOCK)return 0;return fail(errno);}
    if(!n)return fail(ECONNRESET);
    received_+=unsigned(n);if(received_<32)return 0;
    if(get64(input_)!=DORADO_CTRL_MAGIC || get32(input_+8)!=(command_|DORADO_CTRL_ACK) ||
       get32(input_+12)!=sequence_ || get32(input_+28)>1 || get32(input_+24)>4095)return fail(EPROTO);
    reply_.error=get32(input_+24);reply_.active=get32(input_+28)!=0;reply_.epoch=get64(input_+16);
    if(!reply_.error && command_==DORADO_CTRL_RESUME && (!reply_.active || reply_.epoch!=epoch_))return fail(EPROTO);
    if(!reply_.error && command_==DORADO_CTRL_STOP && reply_.active)return fail(EPROTO);
    pending_=false;return 1;
}
int DoradoControlClient::wait(uint64_t deadline){
    for(;;){
        int result=pump();if(result)return result;
        uint64_t now=milliseconds();if(now>=deadline)return -ETIMEDOUT;
        pollfd p={fd_,short(connecting_ || sent_<32 ? POLLOUT : POLLIN),0};
        int timeout=int(deadline-now);if(timeout>5)timeout=5;
        if(poll(&p,1,timeout)<0 && errno!=EINTR)return fail(errno);
    }
}
int DoradoControlClient::stop(unsigned timeout_ms){
    uint64_t deadline=milliseconds()+timeout_ms;
    if(pending_) {int result=wait(deadline);if(result<0)return result;}
    int result=begin(DORADO_CTRL_STOP);if(result)return result;
    result=wait(deadline);if(result<0)return result;
    return reply_.error?-int(reply_.error):0;
}
