#ifndef DORADO_ETHERNET_H
#define DORADO_ETHERNET_H

#include "io.h"

#include <stdint.h>
#include <stddef.h>

#define DORADO_ETHERNET_TASK_EOT  06
#define DORADO_ETHERNET_TASK_EIT  07
#define DORADO_ETHERNET_TIOA_DATA 015
#define DORADO_ETHERNET_TIOA_CTL  016

#define DORADO_PUP_TYPE_ETHERNET 01000
#define DORADO_PUP_TYPE_MICROCODE_BOOT_REQUEST 0264
#define DORADO_PUP_TYPE_MICROCODE_BOOT_REPLY   0265

/* Stage-2 Alto software boot over Ethernet. The booting Alto (emulated by
 * AEmu) broadcasts a Mayday Pup to request a boot file, then receives the
 * file as an EFTP stream on socket 20. Constants from Taft's EtherBoot.asm
 * (`docs/research/ethernet-netboot/downloads/EtherBoot.mesa-1.html`) and
 * EFTPSPEC (`chm/pup/EFTPSPEC.BRAVO!1`). */
#define DORADO_PUP_TYPE_MAYDAY     0244   /* boot-file request */
#define DORADO_PUP_TYPE_EFTP_DATA  030
#define DORADO_PUP_TYPE_EFTP_ACK   031
#define DORADO_PUP_TYPE_EFTP_END   032
#define DORADO_PUP_TYPE_EFTP_ABORT 033
#define DORADO_EFTP_RECEIVER_SOCKET 020
#define DORADO_EFTP_DATA_WORDS 256        /* 512-byte EFTP data section */

typedef struct dorado_ethernet {
    uint8_t local_host;
    uint8_t remote_host;

    uint8_t rx_on;
    uint8_t tx_on;
    uint8_t tx_active;
    uint8_t tx_complete;
    uint8_t rx_attention_latched;

    uint16_t control_last[16];
    uint64_t control_writes[16];
    uint64_t data_writes;
    uint64_t data_reads;
    uint64_t tx_starts;
    uint64_t tx_eops;
    uint64_t tx_stops;

    uint16_t tx_words[512];
    size_t tx_count;
    uint64_t requests_seen;
    uint64_t replies_queued;
    uint16_t last_boot_offset;
    uint16_t first_control[16];

    /* Stage-2 EFTP boot server state. */
    uint16_t last_tx_pup_type;
    uint64_t eftp_requests_seen;
    uint64_t eftp_replies_queued;
    uint16_t eftp_last_bfn;
    char eftp_boot_path[256];

    uint16_t *rx_words;
    uint8_t  *rx_attention;
    size_t rx_count;
    size_t rx_pos;

    char boot_110_path[256];
    char boot_111_path[256];
    char boot_113_path[256];
    char boot_114_path[256];
} dorado_ethernet;

void dorado_ethernet_init(dorado_ethernet *eth);
void dorado_ethernet_free(dorado_ethernet *eth);
void dorado_ethernet_attach_to_io(dorado_ethernet *eth, dorado_io *io);

void dorado_ethernet_set_boot_file(dorado_ethernet *eth,
                                   uint16_t offset,
                                   const char *path);

/* Stage-2: the Alto software boot file served by the fake EFTP server in
 * response to a Mayday request (any boot-file number, for now). */
void dorado_ethernet_set_eftp_boot_file(dorado_ethernet *eth,
                                        const char *path);

/* Returns a wakeup bitmask for EOT/EIT based on current controller state. */
uint16_t dorado_ethernet_wakeup_mask(const dorado_ethernet *eth);

#endif
