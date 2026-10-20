#pragma once
#include <cstdint>

// Bounded transactions on Main's existing SPI owner thread. This client never
// maps DDR, polls workers, grants an ARM lease or authorizes state access.
// Command Done means transmitted; read_status must confirm the exact phase,
// nonce, grants and unchanged error counter before the coordinator advances.
class DoradoSnapshotClient {
public:
    enum class Result { Done, Unavailable, Incompatible, Malformed, Invalid };
    enum class Phase : uint16_t { Idle, Held, Releasing, Fault };
    struct Status {
        Phase phase=Phase::Idle;
        uint64_t build=0,schema=0,epoch=0;
        uint16_t services=0,local_available=0,local_held=0,boundary=0;
        uint32_t errors=0;
        bool host_hold()const{return boundary&1;}
        bool command_hold()const{return boundary&2;}
        bool guest_held()const{return boundary&4;}
        bool global_hold()const{return boundary&8;}
        bool board_busy()const{return boundary&16;}
        bool board_fault()const{return boundary&32;}
    };
    struct IO {
        void *context=nullptr;
        bool (*is_dorado)(void *)=nullptr;
        uint16_t (*select)(void *,uint16_t)=nullptr;
        uint16_t (*word)(void *,uint16_t)=nullptr;
        void (*deselect)(void *)=nullptr;
    };
    // Production adapter uses original config-string identity, ignoring MGL
    // display-name overrides. All methods are called only by Main's poll owner.
    static IO main_io();
    DoradoSnapshotClient(uint64_t build,uint64_t schema,IO io):build_(build),schema_(schema),io_(io){}
    Result read_status(Status &status);
    // Establish the existing board owner hold before Begin. The coordinator
    // must observe guest_held/global_hold before requesting the host lease.
    Result request_hold();
    Result begin(uint64_t nonce){return action(0x7b,nonce,0);}
    Result grant(uint64_t nonce,uint16_t services){return action(0x7c,nonce,services);}
    Result release(uint64_t nonce){return action(0x7d,nonce,0);}
    // Only after native release and the real disk/daemon Continue ACKs.
    Result finish(uint64_t nonce){return action(0x7e,nonce,0);}
    Result quarantine(uint64_t nonce){return action(0x7f,nonce,0);}
private:
    uint64_t build_,schema_;IO io_;
    bool valid()const;
    Result action(uint16_t command,uint64_t nonce,uint16_t services);
};
