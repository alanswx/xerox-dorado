#include "ethernet.h"

#include <stdio.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

static int test_microcode_boot_reply_queue(void)
{
    dorado_io io;
    dorado_ethernet eth;
    dorado_io_init(&io);
    dorado_ethernet_init(&eth);
    dorado_ethernet_attach_to_io(&eth, &io);

    int bad = 0;
    uint16_t status = dorado_io_read(&io, 0, DORADO_ETHERNET_TIOA_CTL, &bad);
    EXPECT(!bad, "EControl status parity");
    EXPECT((status >> 8) == 042, "local host status = 0x%04X", status);

    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 1);
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 2);
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 3);
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 4);
    EXPECT(dorado_ethernet_wakeup_mask(&eth) & (1u << DORADO_ETHERNET_TASK_EOT),
           "EOT wake after TurnOnTx-shaped control sequence");

    uint16_t req[15] = {
        042, 01000, 026, 0264, 01, 0110, 0, 0, 0, 0, 04, 042, 01, 01, 0177777
    };
    for (int i = 0; i < 15; i++) {
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, req[i]);
    }
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 012345);

    EXPECT(eth.requests_seen == 1, "requests_seen=%llu",
           (unsigned long long)eth.requests_seen);
    EXPECT(eth.last_boot_offset == 0110, "boot offset 0o%o",
           eth.last_boot_offset);
    EXPECT(eth.rx_count > 0, "rx queue populated");
    EXPECT(eth.replies_queued > 1, "replies queued=%llu",
           (unsigned long long)eth.replies_queued);
    EXPECT(dorado_ethernet_wakeup_mask(&eth) & (1u << DORADO_ETHERNET_TASK_EIT),
           "EIT wake with queued replies");

    uint16_t w0 = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                 DORADO_ETHERNET_TIOA_DATA, &bad);
    uint16_t w1 = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                 DORADO_ETHERNET_TIOA_DATA, &bad);
    uint16_t w2 = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                 DORADO_ETHERNET_TIOA_DATA, &bad);
    uint16_t w3 = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                 DORADO_ETHERNET_TIOA_DATA, &bad);
    EXPECT(w0 == ((042u << 8) | 01u), "reply ether hosts 0x%04X", w0);
    EXPECT(w1 == 01000, "reply ether type 0o%o", w1);
    EXPECT(w2 == (uint16_t)(026 + 2 * 255), "first reply length 0o%o", w2);
    EXPECT(w3 == 0265, "reply type 0o%o", w3);

    /* Drain through the first reply status word. */
    int saw_attention = 0;
    for (int i = 4; i < 12 + 255 + 2; i++) {
        (void)dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                             DORADO_ETHERNET_TIOA_DATA, &bad);
        if (dorado_io_attention(&io, DORADO_ETHERNET_TASK_EIT,
                                DORADO_ETHERNET_TIOA_DATA)) {
            saw_attention = 1;
            break;
        }
    }
    EXPECT(saw_attention, "status word asserted IOAttention");

    dorado_ethernet_free(&eth);
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_microcode_boot_reply_queue();
    if (rc == 0) printf("All ethernet tests passed.\n");
    return rc;
}
