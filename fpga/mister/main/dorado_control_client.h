#pragma once
#include <cstddef>
#include <cstdint>
#include <sys/types.h>

// Main-side local controller only: no DDR mapping, packet data or SPI here.
// The integration must still stop/drain the FPGA after the daemon STOP ACK.
class DoradoControlClient {
public:
    struct IO {
        ssize_t (*send)(int,const void *,size_t,int)=nullptr;
        ssize_t (*receive)(int,void *,size_t,int)=nullptr;
    };
    struct Reply { uint32_t error=0;bool active=false;uint64_t epoch=0; };
    DoradoControlClient()=default;
    explicit DoradoControlClient(IO io):io_(io){}
    ~DoradoControlClient(){close();}
    DoradoControlClient(const DoradoControlClient &)=delete;
    DoradoControlClient &operator=(const DoradoControlClient &)=delete;
    int open(const char *path);
    int begin(uint32_t command,uint64_t epoch=0);
    //0 pending,1 reply complete, negative errno on transport failure.
    int pump();
    // Finish any partial/in-flight record, then require the exact STOP ACK.
    // Timeout preserves pending stream state; never permission to program.
    int stop(unsigned timeout_ms);
    void close();
    bool pending()const{return pending_;}
    bool connected()const{return fd_>=0;}
    const Reply &reply()const{return reply_;}
private:
    int fd_=-1;bool connecting_=false,pending_=false;
    uint32_t sequence_=0,command_=0;
    uint64_t epoch_=0;
    unsigned sent_=0,received_=0;
    unsigned char output_[32]={},input_[32]={};
    IO io_;Reply reply_;
    int fail(int error);
    int wait(uint64_t deadline);
};
