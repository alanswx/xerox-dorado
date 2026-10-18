#include "dorado_ddr_ring.h"
#include <errno.h>
#include <string.h>

_Static_assert(DORADO_DDR_TX_DESC + DORADO_DDR_TX_SLOTS*16 <= DORADO_DDR_RX_DESC,
               "descriptor overlap");
_Static_assert(DORADO_DDR_RX_DESC + DORADO_DDR_RX_SLOTS*16 <= DORADO_DDR_TX_DATA,
               "descriptor/data overlap");
_Static_assert(DORADO_DDR_TX_DATA + DORADO_DDR_TX_SLOTS*2048 <= DORADO_DDR_RX_DATA,
               "TX/RX payload overlap");
_Static_assert(DORADO_DDR_RX_DATA + DORADO_DDR_RX_SLOTS*2048 <= DORADO_DDR_SIZE,
               "mailbox overflow");

static void order(void) {
#if defined(__arm__) || defined(__aarch64__)
    __asm__ volatile("dmb sy" ::: "memory");
#else
    __sync_synchronize();
#endif
}
static void complete(void) {
#if defined(__arm__) || defined(__aarch64__)
    __asm__ volatile("dsb sy" ::: "memory");
#else
    __sync_synchronize();
#endif
}
static uint32_t native_le(uint32_t value) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return __builtin_bswap32(value);
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return value;
#else
#error Unsupported host byte order
#endif
}
static uint32_t load(dorado_ddr_ring *r, unsigned offset) {
    return native_le(*(volatile uint32_t *)(r->memory+offset));
}
static void store(dorado_ddr_ring *r, unsigned offset, uint32_t value) {
    *(volatile uint32_t *)(r->memory+offset)=native_le(value);
}
static uint64_t pair(dorado_ddr_ring *r, unsigned offset) {
    uint32_t low=load(r,offset), high=load(r,offset+4);
    return (uint64_t)high<<32 | low;
}
static int fail(dorado_ddr_ring *r, int error) {
    r->active=0;
    return -error;
}
static int session(dorado_ddr_ring *r) {
    if (!r || !r->active) return -ENOTCONN;
    order();
    if (pair(r,DORADO_DDR_MAGIC_OFF)!=DORADO_DDR_MAGIC ||
        pair(r,DORADO_DDR_EPOCH_OFF)!=r->epoch ||
        pair(r,DORADO_DDR_ABI_OFF)!=DORADO_DDR_ABI ||
        load(r,DORADO_DDR_STATUS)!=1)
        return fail(r,ESTALE);
    order();
    /* Another host must not publish into our single-owner indices. */
    if (load(r,DORADO_DDR_TX_CONS)!=r->tx_cons ||
        load(r,DORADO_DDR_RX_PROD)!=r->rx_prod)
        return fail(r,EPROTO);
    return 0;
}
int dorado_ddr_bind(dorado_ddr_ring *r, volatile void *memory, size_t size,
                    uint64_t epoch) {
    if (!r) return -EINVAL;
    memset(r,0,sizeof *r);
    if (!memory || ((uintptr_t)memory&3) || size<DORADO_DDR_SIZE || !epoch)
        return -EINVAL;
    r->memory=memory;r->epoch=epoch;r->active=1;
    order();r->tx_cons=load(r,DORADO_DDR_TX_CONS);r->rx_prod=load(r,DORADO_DDR_RX_PROD);
    int result=session(r);
    if (result) return result;
    uint32_t pending=load(r,DORADO_DDR_TX_PROD)-r->tx_cons;
    uint32_t occupied=r->rx_prod-load(r,DORADO_DDR_RX_CONS);
    if (pending>DORADO_DDR_TX_SLOTS || occupied>DORADO_DDR_RX_SLOTS)
        return fail(r,EPROTO);
    return session(r);
}
void dorado_ddr_quiesce(dorado_ddr_ring *r) {
    if (!r) return;
    /* Caller is the sole owner/service thread: there is no background MMIO
     * after this barrier or while sending the local control STOP ACK. */
    r->active=0;complete();r->memory=NULL;
}
int dorado_ddr_send(dorado_ddr_ring *r, const unsigned char *bytes, size_t size) {
    if (!bytes || size<4 || size>DORADO_DDR_SLOT_BYTES || (size&1)) return -EINVAL;
    int result=session(r);if(result)return result;
    uint32_t occupied=r->rx_prod-load(r,DORADO_DDR_RX_CONS);
    if (occupied>DORADO_DDR_RX_SLOTS) return fail(r,EPROTO);
    if (occupied==DORADO_DDR_RX_SLOTS) return -EAGAIN;
    order();
    unsigned slot=r->rx_prod&(DORADO_DDR_RX_SLOTS-1);
    unsigned data=DORADO_DDR_RX_DATA+slot*DORADO_DDR_SLOT_BYTES;
    for (size_t i=0;i<size;i+=4) {
        uint32_t word=0;
        for(unsigned j=0;j<4 && i+j<size;j++)word|=(uint32_t)bytes[i+j]<<(8*j);
        store(r,data+(unsigned)i,word);
    }
    unsigned desc=DORADO_DDR_RX_DESC+slot*DORADO_DDR_DESC_BYTES;
    store(r,desc,(uint32_t)size);store(r,desc+4,r->rx_prod);
    store(r,desc+8,(uint32_t)r->epoch);store(r,desc+12,(uint32_t)(r->epoch>>32));
    order();
    result=session(r);if(result)return result;
    store(r,DORADO_DDR_RX_PROD,r->rx_prod+1u);order();r->rx_prod++;
    return 0;
}
int dorado_ddr_receive(dorado_ddr_ring *r, unsigned char *bytes, size_t capacity) {
    if (!bytes) return -EINVAL;
    int result=session(r);if(result)return result;
    uint32_t pending=load(r,DORADO_DDR_TX_PROD)-r->tx_cons;
    if (pending>DORADO_DDR_TX_SLOTS) return fail(r,EPROTO);
    if (!pending) return -EAGAIN;
    order();
    unsigned slot=r->tx_cons&(DORADO_DDR_TX_SLOTS-1);
    unsigned desc=DORADO_DDR_TX_DESC+slot*DORADO_DDR_DESC_BYTES;
    uint32_t size=load(r,desc), sequence=load(r,desc+4);
    uint64_t epoch=pair(r,desc+8);
    if (sequence!=r->tx_cons || epoch!=r->epoch ||
        size<4 || size>DORADO_DDR_SLOT_BYTES || (size&1)) return fail(r,EPROTO);
    if (size>capacity) return -EMSGSIZE;
    unsigned data=DORADO_DDR_TX_DATA+slot*DORADO_DDR_SLOT_BYTES;
    for(unsigned i=0;i<size;i+=4) {
        uint32_t word=load(r,data+i);
        for(unsigned j=0;j<4 && i+j<size;j++)bytes[i+j]=(unsigned char)(word>>(8*j));
    }
    order();
    result=session(r);if(result)return result;
    store(r,DORADO_DDR_TX_CONS,r->tx_cons+1u);order();r->tx_cons++;
    return (int)size;
}
