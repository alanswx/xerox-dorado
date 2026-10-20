#define _GNU_SOURCE
#include "dorado_ddr_control.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

static uint32_t get32(const unsigned char *p) {
    return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
static uint64_t get64(const unsigned char *p){return (uint64_t)get32(p+4)<<32|get32(p);}
static void put32(unsigned char *p,uint32_t value){for(unsigned i=0;i<4;i++)p[i]=(unsigned char)(value>>(8*i));}
static void put64(unsigned char *p,uint64_t value){put32(p,(uint32_t)value);put32(p+4,(uint32_t)(value>>32));}
static int nonblocking(int fd) {
    int flags=fcntl(fd,F_GETFL);
    if(flags<0 || fcntl(fd,F_SETFL,flags|O_NONBLOCK) || fcntl(fd,F_SETFD,FD_CLOEXEC))return errno;
#ifdef SO_NOSIGPIPE
    int one=1;if(setsockopt(fd,SOL_SOCKET,SO_NOSIGPIPE,&one,sizeof one))return errno;
#endif
    return 0;
}
static int owner(int fd) {
    uid_t uid;
#ifdef __linux__
    struct ucred credentials;socklen_t size=sizeof credentials;
    if(getsockopt(fd,SOL_SOCKET,SO_PEERCRED,&credentials,&size))return 0;
    uid=credentials.uid;
#else
    gid_t gid;if(getpeereid(fd,&uid,&gid))return 0;
#endif
    return uid==geteuid();
}
static int stop_access(dorado_ddr_control *c,unsigned *events) {
    if(c->granted)*events|=DORADO_CTRL_STOPPED_EVENT;
    c->granted=0;dorado_ddr_quiesce(&c->ring);
    if(c->mapped) {
        int error=c->mapper.close(c->mapper.opaque,c->mapped);
        if(error)return error;
        c->mapped=NULL;
    }
    return 0;
}
static void disconnect(dorado_ddr_control *c,unsigned *events) {
    (void)stop_access(c,events);
    if(c->client>=0)close(c->client);
    c->client=-1;c->used=c->sent=c->pending=c->last_valid=0;
}
int dorado_ddr_control_open(dorado_ddr_control *c,const char *path,dorado_ddr_mapper mapper) {
    if(!c)return EINVAL;
    memset(c,0,sizeof *c);c->listener=c->client=-1;c->mapper=mapper;
    if(!path || !mapper.open || !mapper.close || strlen(path)>=sizeof c->path ||
       strlen(path)>=sizeof(((struct sockaddr_un *)0)->sun_path))return EINVAL;
    int fd=socket(AF_UNIX,SOCK_STREAM,0);
    if(fd<0)return errno;
    int error=nonblocking(fd);
    if(error){close(fd);return error;}
    struct sockaddr_un address={0};address.sun_family=AF_UNIX;strcpy(address.sun_path,path);
    if(bind(fd,(struct sockaddr *)&address,sizeof address)) {error=errno;close(fd);return error;}
    /* Never unlink/replace another listener. This path now belongs to us. */
    strcpy(c->path,path);
    if(chmod(path,0600) || listen(fd,2)) {error=errno;close(fd);unlink(path);c->path[0]=0;return error;}
    c->listener=fd;return 0;
}
static unsigned dispatch(dorado_ddr_control *c) {
    unsigned events=0;uint32_t command=get32(c->request+8),seq=get32(c->request+12);
    uint64_t epoch=get64(c->request+16);int error=0;
    if(get64(c->request)!=DORADO_CTRL_MAGIC || !seq) {disconnect(c,&events);return events;}
    if(c->last_valid && seq==get32(c->last_request+12) &&
       !memcmp(c->request,c->last_request,32)) {
        memcpy(c->reply,c->last_reply,32);c->pending=1;c->sent=c->used=0;return 0;
    }
    if(c->last_valid && (int32_t)(seq-get32(c->last_request+12))<=0)error=EPROTO;
    else if(command==DORADO_CTRL_STOP)error=stop_access(c,&events);
    else if(command==DORADO_CTRL_STATUS) {
        if(c->granted && !c->ring.active)error=stop_access(c,&events);
    } else if(command==DORADO_CTRL_PAUSE || command==DORADO_CTRL_CONTINUE) {
        if(!c->granted || !c->ring.active)error=ENOTCONN;
        else if(epoch!=c->last_epoch)error=ESTALE;
        else if(get32(c->request+24)!=DORADO_DDR_BASE || get32(c->request+28)!=DORADO_DDR_SIZE)error=EINVAL;
        else {
            int was_paused=c->ring.paused;
            int result=command==DORADO_CTRL_PAUSE ? dorado_ddr_pause(&c->ring) : dorado_ddr_continue(&c->ring);
            if(result)error=-result;
            else if(command==DORADO_CTRL_PAUSE && !was_paused)events|=DORADO_CTRL_PAUSED_EVENT;
            else if(command==DORADO_CTRL_CONTINUE)events|=DORADO_CTRL_CONTINUED_EVENT;
            if(!c->ring.active) {
                int cleanup=stop_access(c,&events);if(cleanup)error=cleanup;
            }
        }
    } else if(command==DORADO_CTRL_RESUME) {
        if(c->granted || c->mapped)error=EBUSY;
        else if(!epoch || epoch==c->last_epoch || get32(c->request+24)!=DORADO_DDR_BASE ||
                get32(c->request+28)!=DORADO_DDR_SIZE)error=EINVAL;
        else {
            error=c->mapper.open(c->mapper.opaque,&c->mapped);
            if(!error && !c->mapped)error=EFAULT;
            if(!error) {
                int result=dorado_ddr_bind(&c->ring,c->mapped,DORADO_DDR_SIZE,epoch);
                if(result)error=-result;
            }
            if(error && c->mapped) {
                int cleanup=stop_access(c,&events);if(cleanup)error=cleanup;
            }
            if(!error){c->granted=1;c->last_epoch=epoch;events|=DORADO_CTRL_RESUMED_EVENT;}
        }
    } else error=EPROTO;
    if(error==EPROTO) {
        disconnect(c,&events);return events;
    }
    put64(c->reply,DORADO_CTRL_MAGIC);put32(c->reply+8,command|DORADO_CTRL_ACK);
    put32(c->reply+12,seq);put64(c->reply+16,c->last_epoch);
    uint32_t flags=c->granted && c->ring.active ? DORADO_CTRL_ACTIVE_FLAG : 0;
    if(flags && c->ring.paused)flags|=DORADO_CTRL_PAUSED_FLAG;
    put32(c->reply+24,(uint32_t)error);put32(c->reply+28,flags);
    memcpy(c->last_request,c->request,32);memcpy(c->last_reply,c->reply,32);
    c->last_valid=1;c->used=c->sent=0;c->pending=1;return events;
}
unsigned dorado_ddr_control_poll(dorado_ddr_control *c) {
    unsigned events=0;
    if(c->listener<0)return 0;
    if(c->granted && !c->ring.active) {
        (void)stop_access(c,&events);c->last_valid=0;
    }
    /* Retire a closed owner BEFORE accepting its replacement. Otherwise a
     * prompt Main reconnect can be rejected as a second controller merely
     * because the old EOF has not reached the recv below yet. Peeking never
     * consumes a partial control record or steals a live owner's session. */
    if(c->client>=0) {
        unsigned char byte;
        ssize_t n=recv(c->client,&byte,1,MSG_PEEK|MSG_DONTWAIT);
        if(!n || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR))
            disconnect(c,&events);
    }
    int incoming=accept(c->listener,NULL,NULL);
    if(incoming>=0) {
        if(c->client>=0 || !owner(incoming) || nonblocking(incoming))close(incoming);
        else {c->client=incoming;c->used=c->sent=c->pending=c->last_valid=0;}
    }
    if(c->client<0)return events;
    if(!c->pending) {
        ssize_t n=recv(c->client,c->request+c->used,32-c->used,MSG_DONTWAIT);
        if(!n || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)) {
            disconnect(c,&events);return events;
        }
        if(n>0)c->used+=(unsigned)n;
        if(c->used==32)events|=dispatch(c);
    }
    if(c->client>=0 && c->pending) {
        ssize_t n=send(c->client,c->reply+c->sent,32-c->sent,MSG_DONTWAIT|MSG_NOSIGNAL);
        if(n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)disconnect(c,&events);
        else if(n>0) {
            c->sent+=(unsigned)n;
            if(c->sent==32)c->sent=c->pending=0;
        }
    }
    return events;
}
int dorado_ddr_control_close(dorado_ddr_control *c) {
    unsigned events=0;int result=stop_access(c,&events);
    if(c->client>=0)close(c->client);
    if(c->listener>=0)close(c->listener);
    if(c->path[0])unlink(c->path);
    c->client=c->listener=-1;c->path[0]=0;return result;
}
