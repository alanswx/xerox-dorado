#include "dorado_snapshot_client.h"
#include "spi.h"
#include "user_io.h"
#include <cstring>

namespace {
bool is_dorado(void *){return !std::strcmp(user_io_get_core_name(1),"Dorado");}
uint16_t select(void *,uint16_t command){return spi_uio_cmd_cont(command);}
uint16_t word(void *,uint16_t value){return spi_w(value);}
void deselect(void *){DisableIO();}
uint64_t get64(const uint16_t *p){uint64_t n=0;for(unsigned i=0;i<4;++i)n|=uint64_t(p[i])<<(16*i);return n;}
}
DoradoSnapshotClient::IO DoradoSnapshotClient::main_io(){return {nullptr,is_dorado,select,word,deselect};}
bool DoradoSnapshotClient::valid()const{
    return build_ && schema_ && io_.is_dorado && io_.select && io_.word && io_.deselect;
}
DoradoSnapshotClient::Result DoradoSnapshotClient::read_status(Status &status){
    if(!valid())return Result::Invalid;
    if(!io_.is_dorado(io_.context))return Result::Unavailable;
    uint16_t words[22]={};words[0]=io_.select(io_.context,0x7a);
    if(words[0]!=0xd0c0){io_.deselect(io_.context);return Result::Unavailable;}
    for(unsigned i=1;i<22;++i)words[i]=io_.word(io_.context,0);
    io_.deselect(io_.context);
    if(words[1]!=1 || words[3]!=5 || get64(words+4)!=build_ || get64(words+8)!=schema_)return Result::Incompatible;
    if(words[2]>3 || words[16]>31 || words[17]>31 || words[18]>31 ||
       (words[18]&~words[17]) || words[19]>63)return Result::Malformed;
    Status value;value.phase=Phase(words[2]);value.build=get64(words+4);value.schema=get64(words+8);
    value.epoch=get64(words+12);value.services=words[16];value.local_available=words[17];
    value.local_held=words[18];value.boundary=words[19];value.errors=uint32_t(words[20])|(uint32_t(words[21])<<16);
    if((value.phase!=Phase::Idle && !value.epoch) ||
       value.host_hold()!=(value.phase==Phase::Held || value.phase==Phase::Fault) ||
       (value.phase!=Phase::Held && value.services))return Result::Malformed;
    status=value;return Result::Done;
}
DoradoSnapshotClient::Result DoradoSnapshotClient::action(uint16_t command,uint64_t nonce,uint16_t services){
    if(!valid() || !nonce || services>31)return Result::Invalid;
    if(!io_.is_dorado(io_.context))return Result::Unavailable;
    if(io_.select(io_.context,command)!=0xd0c0){io_.deselect(io_.context);return Result::Unavailable;}
    io_.word(io_.context,0xc04d);io_.word(io_.context,1);
    for(unsigned i=0;i<4;++i)io_.word(io_.context,uint16_t(build_>>(16*i)));
    for(unsigned i=0;i<4;++i)io_.word(io_.context,uint16_t(schema_>>(16*i)));
    for(unsigned i=0;i<4;++i)io_.word(io_.context,uint16_t(nonce>>(16*i)));
    io_.word(io_.context,services);io_.deselect(io_.context);
    return Result::Done;
}
DoradoSnapshotClient::Result DoradoSnapshotClient::request_hold(){
    Status status;auto result=read_status(status);
    if(result!=Result::Done)return result;
    if(status.phase!=Phase::Idle || status.board_fault() || status.board_busy())return Result::Invalid;
    if(io_.select(io_.context,0x77)!=0xd0b0){io_.deselect(io_.context);return Result::Unavailable;}
    io_.word(io_.context,0xb04d);io_.word(io_.context,2);
    for(unsigned i=0;i<4;++i)io_.word(io_.context,uint16_t(schema_>>(16*i)));
    io_.word(io_.context,1);io_.deselect(io_.context);return Result::Done;
}
