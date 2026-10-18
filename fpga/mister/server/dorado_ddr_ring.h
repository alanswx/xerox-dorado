#ifndef DORADO_DDR_RING_H
#define DORADO_DDR_RING_H
#include <stddef.h>
#include <stdint.h>

/* Frame-only mailbox. All control words are little-endian, aligned32-bit
 * accesses. Frame bytes are unchanged network-order bytes. No CRC/status. */
#define DORADO_DDR_BASE UINT32_C(0x1ff00000)
#define DORADO_DDR_SIZE UINT32_C(0x10000)
#define DORADO_DDR_MAGIC UINT64_C(0x31474e49524f4444) /* bytes DDORING1 */
#define DORADO_DDR_ABI UINT64_C(0x0001080000100008) /* v1,2048B,16RX,8TX */
#define DORADO_DDR_MAGIC_OFF 0x00u
#define DORADO_DDR_EPOCH_OFF 0x08u
#define DORADO_DDR_TX_PROD 0x10u /* FPGA owns */
#define DORADO_DDR_TX_CONS 0x18u /* daemon owns */
#define DORADO_DDR_RX_PROD 0x20u /* daemon owns */
#define DORADO_DDR_RX_CONS 0x28u /* FPGA owns */
#define DORADO_DDR_STATUS 0x30u /* FPGA owns:1=running,0=stopped */
#define DORADO_DDR_ABI_OFF 0x38u
#define DORADO_DDR_TX_DESC 0x100u
#define DORADO_DDR_RX_DESC 0x200u
#define DORADO_DDR_DESC_BYTES 16u /* length,sequence,epochLo,epochHi */
#define DORADO_DDR_TX_DATA 0x1000u
#define DORADO_DDR_RX_DATA 0x5000u
#define DORADO_DDR_SLOT_BYTES 2048u
#define DORADO_DDR_TX_SLOTS 8u
#define DORADO_DDR_RX_SLOTS 16u

typedef struct {
    volatile unsigned char *memory;
    uint64_t epoch;
    uint32_t tx_cons, rx_prod;
    int active;
} dorado_ddr_ring;

/* The caller must hold an EXCLUSIVE Main-issued grant for this exact epoch.
 * DDR magic alone never grants access. Bind only reads; this module neither
 * maps /dev/mem nor enables hardware. No fallback or automatic reattachment.
 * Main must revoke the grant, get daemon STOP acknowledgement, and drain the
 * FPGA engine before programming/reloading another core. Polling a header
 * cannot make unsynchronised FPGA replacement safe. */
int dorado_ddr_bind(dorado_ddr_ring *, volatile void *, size_t, uint64_t epoch);
/* Stop in the daemon's sole service thread; complete CPU MMIO before ACK. */
void dorado_ddr_quiesce(dorado_ddr_ring *);
/* Nonblocking. Send returns0 or negative errno. Receive returns byte count
 * or negative errno. EAGAIN leaves ownership unchanged. EMSGSIZE leaves a
 * pending TX frame and the destination unchanged. Protocol/session faults
 * deactivate access until an explicit new grant/bind. */
int dorado_ddr_send(dorado_ddr_ring *, const unsigned char *, size_t);
int dorado_ddr_receive(dorado_ddr_ring *, unsigned char *, size_t);
#endif
