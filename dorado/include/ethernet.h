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

/* Returns a wakeup bitmask for EOT/EIT based on current controller state. */
uint16_t dorado_ethernet_wakeup_mask(const dorado_ethernet *eth);

#endif
