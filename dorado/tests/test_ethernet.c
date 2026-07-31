#include "ethernet.h"

#include <stdio.h>
#include <string.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

/* Run down the server-turnaround hold (the packet is "in flight"
 * until the wakeup poll has ticked it away). */
static void eth_wait_arrival(dorado_ethernet *eth)
{
    for (int i = 0; i < 70000; i++) (void)dorado_ethernet_wakeup_mask(eth);
}

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

    /* InitialEther.mc sequence: ResetEther = TurnOffRx, TurnOffTx; then
     * TurnOnRx, TurnOnTx (EthC encodings per HM Figure 16). */
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 0170377);
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 0007777);
    EXPECT(dorado_ethernet_wakeup_mask(&eth) == 0,
           "no wakeups with transmitter and receiver off");
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 0173377);
    dorado_io_write(&io, 0, DORADO_ETHERNET_TIOA_CTL, 0047777);
    EXPECT(dorado_ethernet_wakeup_mask(&eth) & (1u << DORADO_ETHERNET_TASK_EOT),
           "EOT wake after TurnOnTx");

    uint16_t req[15] = {
        042, 01000, 026, 0264, 01, 0112, 0, 0, 0, 0, 04, 042, 01, 01, 0177777
    };
    for (int i = 0; i < 15; i++) {
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, req[i]);
    }
    /* SendEOP transmits the buffered packet; the controller's TxGone
     * then clears TxEOP, producing EOT's end-of-packet wakeup. */
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0067777);
    EXPECT(eth.tx_eop == 0, "TxGone clears TxEOP after transmission");
    EXPECT(dorado_ethernet_wakeup_mask(&eth) & (1u << DORADO_ETHERNET_TASK_EOT),
           "EOT end-of-packet wakeup");
    /* EOStop: TurnOffTx; EOT wakeups must cease (this is what lets the
     * loaded emulator world run with EOT blocked). */
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0007777);
    EXPECT(!(dorado_ethernet_wakeup_mask(&eth) &
             (1u << DORADO_ETHERNET_TASK_EOT)),
           "no EOT wakeups after TurnOffTx");

    EXPECT(eth.requests_seen == 1, "requests_seen=%llu",
           (unsigned long long)eth.requests_seen);
    EXPECT(eth.last_boot_offset == 0112, "boot offset 0o%o",
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

    /* Drain through the first reply status word. IOAtten is a level on
     * the word the next read delivers (HM §11), so test before reading. */
    int saw_attention = 0;
    for (int i = 4; i < 12 + 255 + 2; i++) {
        int attn = dorado_io_attention(&io, DORADO_ETHERNET_TASK_EIT,
                                       DORADO_ETHERNET_TIOA_DATA);
        (void)dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                             DORADO_ETHERNET_TIOA_DATA, &bad);
        if (attn) {
            saw_attention = 1;
            break;
        }
    }
    EXPECT(saw_attention, "status word asserted IOAttention");

    dorado_ethernet_free(&eth);
    return 0;
}

/* Stage-2: a Mayday request must produce an EFTP stream of the configured
 * Alto boot file (Data packets seq 0.., then an End), addressed to the
 * EFTP receiver socket, with payload matching the file verbatim. */
static int test_eftp_boot_reply_queue(void)
{
    const char *boot = "../chm/bootfiles/CRTTEST.BOOT!1";

    /* Independently read the boot file: first word + total word count. */
    FILE *fp = fopen(boot, "rb");
    EXPECT(fp != NULL, "open %s", boot);
    long file_words = 0;
    uint16_t first_word = 0;
    for (;;) {
        int hi = fgetc(fp), lo = fgetc(fp);
        if (hi == EOF || lo == EOF) break;
        uint16_t w = (uint16_t)((hi << 8) | lo);
        if (file_words == 0) first_word = w;
        file_words++;
    }
    fclose(fp);
    EXPECT(file_words > 0, "boot file is non-empty");
    long data_pkts = (file_words + DORADO_EFTP_DATA_WORDS - 1) /
                     DORADO_EFTP_DATA_WORDS;

    dorado_io io;
    dorado_ethernet eth;
    dorado_io_init(&io);
    dorado_ethernet_init(&eth);
    dorado_ethernet_set_eftp_boot_file(&eth, boot);
    dorado_ethernet_attach_to_io(&eth, &io);

    /* Broadcast Mayday Pup: etherType=typePup, pupType=Mayday, bfn in
     * pupID+1. Other fields are not inspected by the fake server. */
    uint16_t mayday[15] = {
        042, 01000, 026, DORADO_PUP_TYPE_MAYDAY, 0, 0 /*bfn*/,
        0, 0, 04, 042, 0, 0, 0, 0, 0177777
    };
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0047777);   /* TurnOnTx  */
    for (int i = 0; i < 15; i++) {
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, mayday[i]);
    }
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0067777);   /* SendEOP   */
    eth_wait_arrival(&eth);

    EXPECT(eth.eftp_requests_seen == 1, "eftp_requests_seen=%llu",
           (unsigned long long)eth.eftp_requests_seen);
    EXPECT(eth.eftp_last_bfn == 0, "eftp_last_bfn=0o%o", eth.eftp_last_bfn);
    EXPECT(eth.requests_seen == 0, "Mayday must not be a microcode request");
    /* Lock-step server: only packet 0 is on the wire at Mayday time;
     * later packets are released by Acks. */
    (void)data_pkts;
    EXPECT((long)eth.eftp_replies_queued == 1,
           "queued=%llu want 1 (lock-step packet 0)",
           (unsigned long long)eth.eftp_replies_queued);
    EXPECT(eth.eftp_state == 1, "transfer in flight");

    int bad = 0;
    uint16_t h[12];
    for (int i = 0; i < 12; i++) {
        h[i] = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                              DORADO_ETHERNET_TIOA_DATA, &bad);
    }
    EXPECT(h[0] == ((042u << 8) | 01u), "data ether hosts 0x%04X", h[0]);
    EXPECT(h[1] == 01000, "data ether type 0o%o", h[1]);
    EXPECT(h[3] == DORADO_PUP_TYPE_EFTP_DATA, "first pup type 0o%o", h[3]);
    EXPECT(h[5] == 0, "first EFTP seq=%u", h[5]);
    EXPECT(h[8] == DORADO_EFTP_RECEIVER_SOCKET, "dest socket 0o%o", h[8]);

    uint16_t p0 = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                 DORADO_ETHERNET_TIOA_DATA, &bad);
    EXPECT(p0 == first_word, "payload word0 0x%04X != file 0x%04X",
           p0, first_word);

    dorado_ethernet_free(&eth);
    return 0;
}

/* Simulate the whole EFTP boot transfer end to end: play the role of the
 * booting Alto's EtherBoot ReceiveEFTPPacket loop against the in-process
 * server, reassemble the streamed image, and verify it byte-for-byte
 * against the boot file. This validates the server (chunking, sequence
 * numbers, headers, End handshake, payload integrity) without needing any
 * Dorado microcode or AEmu bring-up. Mirrors EtherBoot.asm: read Data
 * packets in sequence, Ack each, stop at End. */
static int simulate_eftp_boot(const char *boot, long *out_words)
{
    /* Independently slurp the boot file. */
    FILE *fp = fopen(boot, "rb");
    EXPECT(fp != NULL, "open %s", boot);
    static uint16_t file_buf[1u << 18];
    long file_n = 0;
    for (;;) {
        int hi = fgetc(fp), lo = fgetc(fp);
        if (hi == EOF || lo == EOF) break;
        EXPECT(file_n < (long)(sizeof file_buf / sizeof file_buf[0]),
               "boot file too large for test buffer");
        file_buf[file_n++] = (uint16_t)((hi << 8) | lo);
    }
    fclose(fp);
    EXPECT(file_n > 0, "boot file non-empty");

    dorado_io io;
    dorado_ethernet eth;
    dorado_io_init(&io);
    dorado_ethernet_init(&eth);
    dorado_ethernet_set_eftp_boot_file(&eth, boot);
    dorado_ethernet_attach_to_io(&eth, &io);

    /* Broadcast the Mayday boot request. */
    uint16_t mayday[15] = {
        042, 01000, 026, DORADO_PUP_TYPE_MAYDAY, 0, 0,
        0, 0, 04, 042, 0, 0, 0, 0, 0177777
    };
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0047777);   /* TurnOnTx */
    for (int i = 0; i < 15; i++)
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, mayday[i]);
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0067777);   /* SendEOP  */
    eth_wait_arrival(&eth);
    EXPECT(eth.eftp_requests_seen == 1, "server saw the Mayday");

    /* Client receive loop. */
    static uint16_t image[1u << 18];
    long image_n = 0;
    uint16_t expected_seq = 0;
    int saw_end = 0;
    int guard = 0;
    while (!saw_end && guard++ < 100000) {
        /* Read one packet: words until the attention (status) word. */
        uint16_t pkt[600];
        int n = 0, bad = 0;
        for (;;) {
            EXPECT(n < (int)(sizeof pkt / sizeof pkt[0]), "packet overflow");
            /* IOAtten is a level on the word the next read delivers
             * (HM §11): when true, this read returns the status word. */
            int attn = dorado_io_attention(&io, DORADO_ETHERNET_TASK_EIT,
                                           DORADO_ETHERNET_TIOA_DATA);
            pkt[n] = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                    DORADO_ETHERNET_TIOA_DATA, &bad);
            EXPECT(!bad, "rx underrun mid-packet (n=%d)", n);
            n++;
            if (attn) break;        /* status word = end of packet */
        }
        /* Layout: 12 header + N payload + pup checksum + HW CRC +
         * status. */
        EXPECT(n >= 15, "short packet n=%d", n);
        long npay = n - 15;
        EXPECT(pkt[1] == 01000, "ether type 0o%o", pkt[1]);
        EXPECT(pkt[5] == expected_seq, "seq %u != expected %u",
               pkt[5], expected_seq);
        EXPECT(pkt[2] == (uint16_t)(026 + 2 * npay),
               "pupLength 0o%o for %ld payload words", pkt[2], npay);

        if (pkt[3] == DORADO_PUP_TYPE_EFTP_END) {
            EXPECT(npay == 0, "End packet carries %ld payload words", npay);
            saw_end = 1;
        } else {
            EXPECT(pkt[3] == DORADO_PUP_TYPE_EFTP_DATA,
                   "unexpected pup type 0o%o", pkt[3]);
            EXPECT(pkt[8] == DORADO_EFTP_RECEIVER_SOCKET,
                   "dest socket 0o%o", pkt[8]);
            for (long i = 0; i < npay; i++) {
                EXPECT(image_n < (long)(sizeof image / sizeof image[0]),
                       "image overflow");
                image[image_n++] = pkt[12 + i];
            }
        }
        /* Ack the packet (server pre-queues, so this is absorbed). */
        uint16_t ack[15] = {
            042, 01000, 026, DORADO_PUP_TYPE_EFTP_ACK, 0, expected_seq,
            0, 0, 020, 042, 0, 0, 0, 0, 0177777
        };
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_CTL, 0047777); /* TurnOnTx */
        for (int i = 0; i < 15; i++)
            dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                            DORADO_ETHERNET_TIOA_DATA, ack[i]);
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_CTL, 0067777); /* SendEOP  */
        eth_wait_arrival(&eth);
        expected_seq++;
    }
    EXPECT(saw_end, "transfer ended with an EFTP End");

    /* The reassembled image must equal the boot file exactly. */
    EXPECT(image_n == file_n, "received %ld words, file has %ld",
           image_n, file_n);
    for (long i = 0; i < file_n; i++) {
        EXPECT(image[i] == file_buf[i],
               "word %ld: got 0x%04X want 0x%04X",
               i, image[i], file_buf[i]);
    }

    dorado_ethernet_free(&eth);
    if (out_words) *out_words = image_n;
    return 0;
}

static int test_eftp_boot_full_transfer(void)
{
    long w = 0;
    /* Small single-/few-packet file. */
    if (simulate_eftp_boot("../chm/bootfiles/CRTTEST.BOOT!1", &w)) return 1;
    printf("  EFTP sim: CRTTEST.BOOT delivered %ld words byte-exact\n", w);
    /* Large multi-packet file (exercises chunk boundaries + seq rollover). */
    if (simulate_eftp_boot("../chm/bootfiles/NETEXEC.BOOT!8", &w)) return 1;
    printf("  EFTP sim: NETEXEC.BOOT delivered %ld words byte-exact\n", w);
    return 0;
}

/* A BootDirReq (257B) from NetExec must draw a 260B BootDirReply listing
 * each registered boot file as {bfn, date(2), BCPL-string name}, so the
 * user can boot one by name. Validates the on-wire format and that a later
 * Mayday for that bfn serves the right file. */
static int test_bootdir_reply_format(void)
{
    dorado_io io;
    dorado_ethernet eth;
    dorado_io_init(&io);
    dorado_ethernet_init(&eth);
    /* A default Stage-2 file (served for the breath-of-life bfn 0) plus a
     * directory entry for CedarNetExec at a distinct boot file number. */
    dorado_ethernet_set_eftp_boot_file(&eth, "../chm/bootfiles/NETEXEC.BOOT!8");
    const char *cedar = "../chm/bootfiles/CedarNetExec.boot!4";
    dorado_ethernet_add_boot_dir(&eth, 0111, "CedarNetExec.boot", cedar);
    dorado_ethernet_attach_to_io(&eth, &io);

    /* BootDirReq Pup: etherType=typePup, pupType=257B, dPort socket = misc
     * services (4), sPort host=042 socket-lo=020 (where the reply goes). */
    uint16_t req[13] = {
        042, 01000, 026, DORADO_PUP_TYPE_BOOTDIR_REQ,
        0, 0,           /* id          */
        0, 0, 04,       /* dPort: misc services socket */
        042, 0, 020,    /* sPort: host 042, socket-lo 020 */
        0177777
    };
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0047777);   /* TurnOnTx */
    for (int i = 0; i < 13; i++)
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, req[i]);
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0067777);   /* SendEOP */
    eth_wait_arrival(&eth);

    EXPECT(eth.bootdir_replies == 1, "bootdir_replies=%llu want 1",
           (unsigned long long)eth.bootdir_replies);

    int bad = 0;
    /* Capture the whole reply: 12 Ethernet+Pup-header words, 12 content
     * words (one BFD block), and the Pup checksum word. */
    uint16_t pkt[26];
    for (int i = 0; i < 25; i++)
        pkt[i] = dorado_io_read(&io, DORADO_ETHERNET_TASK_EIT,
                                DORADO_ETHERNET_TIOA_DATA, &bad);
    EXPECT(pkt[1] == 01000, "ether type 0o%o", pkt[1]);
    EXPECT(pkt[3] == DORADO_PUP_TYPE_BOOTDIR_REPLY, "pup type 0o%o", pkt[3]);
    /* One entry: bfn(1) + date(2) + name "CedarNetExec.boot" (17 chars ->
     * 18 bytes incl length -> 9 words) = 12 data words. length = 026+2*12. */
    EXPECT(pkt[2] == (uint16_t)(026 + 2 * 12), "pup length 0o%o", pkt[2]);
    EXPECT(pkt[6] == 042, "reply dPort host 0o%o (= requestor)", pkt[6]);
    EXPECT(pkt[12] == 0111, "bfn 0o%o want 0o111", pkt[12]);

    /* BCPL string starts at content word 3 (pkt[15]): length byte, chars. */
    int slen = (pkt[15] >> 8) & 0377;
    EXPECT(slen == 17, "name length %d want 17", slen);
    char name[64];
    int ni = 0;
    name[ni++] = (char)(pkt[15] & 0377);
    int name_words = (slen + 1 + 1) / 2;
    for (int wi = 1; wi < name_words; wi++) {
        uint16_t w = pkt[15 + wi];
        name[ni++] = (char)((w >> 8) & 0377);
        name[ni++] = (char)(w & 0377);
    }
    name[slen] = '\0';
    EXPECT(strcmp(name, "CedarNetExec.boot") == 0, "name '%s'", name);

    /* The Pup checksum must be a real value NetExec's Pup stack accepts,
     * not the 0xFFFF "skip" sentinel (which it dropped). Recompute it
     * receiver-side over the Pup proper, words [2,24), and compare. */
    EXPECT(pkt[24] != 0177777, "checksum must be real, not skip sentinel");
    {
        uint32_t sum = 0;
        for (int i = 2; i < 24; i++) {
            sum += pkt[i];
            sum = (uint16_t)(sum + ((sum & 0x10000u) >> 16));
            sum = sum << 1;
            sum = (uint16_t)(sum + ((sum & 0x10000u) >> 16));
        }
        if (sum == 0xFFFFu) sum = 0;
        EXPECT(pkt[24] == (uint16_t)sum,
               "Pup checksum 0o%o want 0o%o", pkt[24], (uint16_t)sum);
    }

    /* Now a Mayday for that bfn must serve CedarNetExec.boot, not the
     * default NETEXEC. Independently size CedarNetExec to compare. */
    FILE *fp = fopen(cedar, "rb");
    EXPECT(fp != NULL, "open %s", cedar);
    long cedar_words = 0;
    for (;;) { int hi = fgetc(fp), lo = fgetc(fp);
               if (hi == EOF || lo == EOF) break; cedar_words++; }
    fclose(fp);

    uint16_t mayday[15] = {
        042, 01000, 026, DORADO_PUP_TYPE_MAYDAY, 0, 0111 /*bfn*/,
        0, 0, 04, 042, 0, 0, 0, 0, 0177777
    };
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0047777);
    for (int i = 0; i < 15; i++)
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, mayday[i]);
    dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0067777);
    eth_wait_arrival(&eth);
    EXPECT(eth.eftp_last_bfn == 0111, "Mayday bfn 0o%o", eth.eftp_last_bfn);
    /* CedarNetExec is a "000345 Mesa-format" boot file (image word 0 =
     * 0o345 = "JMP 0o345", the Mesa relocation entry). It is the raw Mesa
     * outload and is served VERBATIM: no synthetic B-format leader page is
     * prepended (the Mesa world's own loader consumes the outload directly).
     * The served stream is therefore exactly the file, word for word, and
     * still begins with the Mesa relocation entry 0o345. */
    EXPECT((long)eth.eftp_len == cedar_words,
           "served %ld words verbatim, CedarNetExec %ld",
           (long)eth.eftp_len, cedar_words);
    EXPECT(eth.eftp_words[0] == 0000345,
           "served image word0=0o%o (want 0o345, verbatim)", eth.eftp_words[0]);

    printf("  BootDir: 260B reply advertises CedarNetExec.boot (bfn 0o111),"
           " Mayday serves %ld words verbatim (Mesa outload, no leader)\n",
           (long)eth.eftp_len);
    dorado_ethernet_free(&eth);
    return 0;
}

/* Send one BootDirReq and merge the entries from the (possibly paginated)
 * BootDirReply into the name/bfn arrays, skipping ones already collected.
 * Returns 0 on success. */
static int collect_bootdir(dorado_io *io, dorado_ethernet *eth,
                           char names[][48], uint16_t *bfns, int *count,
                           int cap)
{
    uint16_t req[13] = {
        042, 01000, 026, DORADO_PUP_TYPE_BOOTDIR_REQ,
        0, 0, 0, 0, 04, 042, 0, 020, 0177777
    };
    dorado_io_write(io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0047777);
    for (int i = 0; i < 13; i++)
        dorado_io_write(io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_DATA, req[i]);
    dorado_io_write(io, DORADO_ETHERNET_TASK_EOT,
                    DORADO_ETHERNET_TIOA_CTL, 0067777);
    eth_wait_arrival(eth);

    int bad = 0;
    uint16_t hdr[12];
    for (int i = 0; i < 12; i++)
        hdr[i] = dorado_io_read(io, DORADO_ETHERNET_TASK_EIT,
                                DORADO_ETHERNET_TIOA_DATA, &bad);
    if (hdr[3] != DORADO_PUP_TYPE_BOOTDIR_REPLY) return 1;
    int nd = ((int)hdr[2] - 026) / 2;
    if (nd < 0 || nd > 256) return 1;
    uint16_t data[256];
    for (int i = 0; i < nd; i++)
        data[i] = dorado_io_read(io, DORADO_ETHERNET_TASK_EIT,
                                 DORADO_ETHERNET_TIOA_DATA, &bad);

    int ptr = 0;
    while (ptr + 4 <= nd) {
        uint16_t bfn = data[ptr];
        int len = (data[ptr + 3] >> 8) & 0377;
        int name_words = (len + 2) / 2;
        char nm[48];
        int ni = 0;
        nm[ni++] = (char)(data[ptr + 3] & 0377);
        for (int wi = 1; wi < name_words && ni < 47; wi++) {
            uint16_t w = data[ptr + 3 + wi];
            if (ni < 47) nm[ni++] = (char)((w >> 8) & 0377);
            if (ni < 47) nm[ni++] = (char)(w & 0377);
        }
        if (len < 47) nm[len] = '\0'; else nm[47] = '\0';
        /* Merge unless already present. */
        int seen = 0;
        for (int j = 0; j < *count; j++)
            if (strcmp(names[j], nm) == 0) { seen = 1; break; }
        if (!seen && *count < cap) {
            snprintf(names[*count], 48, "%s", nm);
            bfns[*count] = bfn;
            (*count)++;
        }
        ptr += 3 + name_words;
    }
    return 0;
}

/* Auto-register every Alto B-format game in chm/bootfiles/ and verify the
 * BootDirReply advertises them all (across the paginated replies NetExec's
 * InstallDir accumulates), and that a Mayday for each bfn serves the right
 * file verbatim. */
static int test_bootdir_all_games(void)
{
    dorado_io io;
    dorado_ethernet eth;
    dorado_io_init(&io);
    dorado_ethernet_init(&eth);
    dorado_ethernet_attach_to_io(&eth, &io);

    int added = dorado_ethernet_add_boot_dir_all(&eth, "../chm/bootfiles");
    EXPECT(added >= 20, "auto-registered %d games (want >= 20)", added);
    EXPECT(added == eth.bootdir_count, "count %d vs added %d",
           eth.bootdir_count, added);

    /* Every entry: name ends in lower-case ".boot", bfn unique and clear of
     * the microcode boot offsets 0o110-0o114, and the file is B-format. */
    for (int i = 0; i < eth.bootdir_count; i++) {
        const char *nm = eth.bootdir[i].name;
        size_t nl = strlen(nm);
        EXPECT(nl > 5 && strcmp(nm + nl - 5, ".boot") == 0,
               "entry %d name '%s' must end in .boot", i, nm);
        uint16_t b = eth.bootdir[i].bfn;
        EXPECT(!(b >= 0110 && b <= 0114),
               "bfn 0o%o collides with microcode boot offsets", b);
        for (int j = i + 1; j < eth.bootdir_count; j++)
            EXPECT(eth.bootdir[j].bfn != b, "duplicate bfn 0o%o", b);
    }

    /* Known games present; known Mesa-format files (0o345) absent. */
    int have_galaxian = 0, have_invaders = 0, have_missile = 0;
    int have_cedar = 0, have_mazewar = 0;
    for (int i = 0; i < eth.bootdir_count; i++) {
        const char *nm = eth.bootdir[i].name;
        if (!strcmp(nm, "Galaxian.boot"))       have_galaxian = 1;
        if (!strcmp(nm, "Invaders.boot"))        have_invaders = 1;
        if (!strcmp(nm, "MissileCommand.boot"))  have_missile = 1;
        if (!strcmp(nm, "CedarNetExec.boot"))    have_cedar = 1;
        if (!strcmp(nm, "MazeWar.boot"))         have_mazewar = 1;
    }
    EXPECT(have_galaxian, "Galaxian.boot must be advertised");
    EXPECT(have_invaders, "Invaders.boot must be advertised");
    EXPECT(have_missile, "MissileCommand.boot must be advertised");
    EXPECT(!have_cedar, "CedarNetExec (Mesa 0o345) must be skipped");
    EXPECT(!have_mazewar, "MazeWar (Mesa 0o345) must be skipped");

    /* Drive several BootDirReqs and union the (paginated) replies; the
     * advertised directory must cover every registered entry exactly. */
    char rnames[64][48];
    uint16_t rbfns[64];
    int rcount = 0;
    for (int r = 0; r < 8 && rcount < eth.bootdir_count; r++)
        if (collect_bootdir(&io, &eth, rnames, rbfns, &rcount, 64))
            FAIL("BootDirReq %d produced no reply", r);
    EXPECT(rcount == eth.bootdir_count,
           "advertised %d entries, registered %d", rcount, eth.bootdir_count);
    for (int i = 0; i < eth.bootdir_count; i++) {
        int found = 0;
        for (int j = 0; j < rcount; j++)
            if (strcmp(rnames[j], eth.bootdir[i].name) == 0 &&
                rbfns[j] == eth.bootdir[i].bfn) { found = 1; break; }
        EXPECT(found, "entry %s=0o%o missing from BootDirReply",
               eth.bootdir[i].name, eth.bootdir[i].bfn);
    }

    /* A Mayday for each of a few games must serve that game verbatim. */
    const char *probe[] = { "Galaxian.boot", "Invaders.boot",
                            "MissileCommand.boot" };
    for (int p = 0; p < 3; p++) {
        uint16_t bfn = 0, want_words = 0;
        char path[256] = "";
        for (int i = 0; i < eth.bootdir_count; i++)
            if (!strcmp(eth.bootdir[i].name, probe[p])) {
                bfn = eth.bootdir[i].bfn;
                snprintf(path, sizeof path, "%s", eth.bootdir[i].path);
            }
        EXPECT(bfn != 0, "no bfn for %s", probe[p]);
        FILE *fp = fopen(path, "rb");
        EXPECT(fp != NULL, "open %s", path);
        long words = 0;
        for (;;) { int hi = fgetc(fp), lo = fgetc(fp);
                   if (hi == EOF || lo == EOF) break; words++; }
        fclose(fp);
        want_words = (uint16_t)words;

        uint16_t mayday[15] = {
            042, 01000, 026, DORADO_PUP_TYPE_MAYDAY, 0, bfn,
            0, 0, 04, 042, 0, 0, 0, 0, 0177777
        };
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_CTL, 0047777);
        for (int i = 0; i < 15; i++)
            dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                            DORADO_ETHERNET_TIOA_DATA, mayday[i]);
        dorado_io_write(&io, DORADO_ETHERNET_TASK_EOT,
                        DORADO_ETHERNET_TIOA_CTL, 0067777);
        eth_wait_arrival(&eth);
        EXPECT(eth.eftp_last_bfn == bfn, "%s Mayday bfn 0o%o want 0o%o",
               probe[p], eth.eftp_last_bfn, bfn);
        EXPECT((long)eth.eftp_len == words,
               "%s served %ld words, file %ld", probe[p],
               (long)eth.eftp_len, words);
        EXPECT(eth.eftp_words[0] == 0000405,
               "%s served word0=0o%o (want 0o405 B-format)",
               probe[p], eth.eftp_words[0]);
        (void)want_words;
    }

    printf("  BootDirAll: advertised %d Alto B-format games across paginated"
           " 260B replies; Mayday serves each verbatim\n", eth.bootdir_count);
    dorado_ethernet_free(&eth);
    return 0;
}

static int test_ftp_netdir_rtp_and_bsp_alloc(void)
{
    dorado_ethernet eth;
    dorado_ethernet_init(&eth);
    dorado_ethernet_set_ftp_sysout(&eth, "../chm/lisp/harmony/basics/LISP.SYSOUT!15");

    uint16_t netdir[15] = {
        (uint16_t)((01 << 8) | 042), 01000, 026 + 6,
        DORADO_PUP_TYPE_NETDIR_LOOKUP, 0, 077,
        (uint16_t)((01 << 8) | 01), 0, 04,
        (uint16_t)((01 << 8) | 042), 0, 020,
        ('D' << 8) | 'o', ('r' << 8) | 'a', ('d' << 8) | 'o'
    };
    dorado_ethernet_direct_transmit(&eth, netdir,
                                    sizeof netdir / sizeof netdir[0]);
    EXPECT(eth.rx_count >= 17, "NetDir reply queued");
    /* The server's first lookup response is a GatewayInfo route
     * announcement (PupName rejects a NameReply for a net it has no
     * route to); the client retries the lookup and then receives the
     * ordinary address reply. */
    if (eth.rx_words[3] == DORADO_PUP_TYPE_GATEWAY_REPLY) {
        eth.rx_pos = eth.rx_count;  /* consume the route announcement */
        dorado_ethernet_direct_transmit(&eth, netdir,
                                        sizeof netdir / sizeof netdir[0]);
        EXPECT(eth.rx_count >= 17, "NetDir reply queued after route prime");
    }
    EXPECT(eth.rx_words[3] == DORADO_PUP_TYPE_NETDIR_REPLY,
           "NetDir reply type 0o%o", eth.rx_words[3]);
    EXPECT(eth.rx_words[12] == (uint16_t)((01 << 8) | 01),
           "FTP port net/host 0o%o", eth.rx_words[12]);
    EXPECT(eth.rx_words[13] == 0 && eth.rx_words[14] == 03,
           "FTP socket %o/%o", eth.rx_words[13], eth.rx_words[14]);

    eth.rx_pos = eth.rx_count;  /* consume the synthetic reply */
    uint16_t rfc[15] = {
        (uint16_t)((01 << 8) | 042), 01000, 026 + 6,
        010, 012345, 067123,
        (uint16_t)((01 << 8) | 01), 0, 03,
        (uint16_t)((01 << 8) | 042), 0, 020,
        (uint16_t)((01 << 8) | 042), 0, 020
    };
    dorado_ethernet_direct_transmit(&eth, rfc, sizeof rfc / sizeof rfc[0]);
    EXPECT(eth.ftp_open, "FTP RTP connection opened");
    EXPECT(eth.rx_words[3] == 010, "RTP RFC reply type 0o%o", eth.rx_words[3]);
    EXPECT(eth.rx_words[4] == 012345 && eth.rx_words[5] == 067123,
           "RTP RFC reply connection id");
    EXPECT(eth.rx_words[12] == (uint16_t)((01 << 8) | 01),
           "RTP server port net/host 0o%o", eth.rx_words[12]);
    EXPECT(eth.rx_words[14] == 03, "RTP server socket 0o%o", eth.rx_words[14]);

    eth.rx_pos = eth.rx_count;  /* consume RFC reply; pending allocation follows */
    (void)dorado_ethernet_wakeup_mask(&eth);
    EXPECT(eth.rx_words[3] == 022, "initial BSP Ack type 0o%o", eth.rx_words[3]);
    EXPECT(eth.rx_words[4] == 012345 && eth.rx_words[5] == 067123,
           "initial BSP Ack id");
    EXPECT(eth.rx_words[12] == 512, "BSP bytes-per-pup %u", eth.rx_words[12]);
    EXPECT(eth.rx_words[13] > 0, "BSP pup allocation %u", eth.rx_words[13]);
    EXPECT(eth.rx_words[14] >= eth.rx_words[12],
           "BSP byte allocation %u", eth.rx_words[14]);

    printf("  FTP: NetDir advertises socket 3; RTP open gets initial BSP allocation\n");
    dorado_ethernet_free(&eth);
    return 0;
}

/* ---- STP enumerate ("ls" on a remote directory) --------------------------
 *
 * Play the client half of STPImpl.mesa!5 TryNewDirectory / DoFiles against the
 * in-process server: open the BSP connection, send an enumeration mark plus
 * its property list, and reassemble the reply into (mark, text) segments.
 * The framing under test is STPServerImpl.mesa!9 DoFiles -- HereIsPList once
 * for New-Directory, before every entry for the old Directory, then EOC. */

#define STP_ROOT "../chm/cedar/stp-root"

typedef struct {
    dorado_ethernet eth;
    uint32_t rx_cursor;         /* next byte ID we expect from the server */
    uint32_t tx_cursor;         /* next byte ID we will send */
} stp_client;

/* Drain one queued Pup into `pkt`; returns its word count, or 0 if the server
 * has nothing more to say. */
static int stp_take(stp_client *c, uint16_t *pkt, int cap)
{
    dorado_ethernet *eth = &c->eth;
    int n;
    for (int i = 0; i < 200000 && eth->rx_pos >= eth->rx_count; i++)
        (void)dorado_ethernet_wakeup_mask(eth);
    if (eth->rx_pos >= eth->rx_count) return 0;
    n = (int)eth->rx_count;
    if (n > cap) n = cap;
    for (int i = 0; i < n; i++) pkt[i] = eth->rx_words[i];
    eth->rx_pos = eth->rx_count;         /* consume it off the wire */
    return n;
}

/* Acknowledge everything received so far, advertising an ordinary Alto-side
 * BSP allocation (this is what unblocks the server's window). */
static void stp_ack(stp_client *c)
{
    uint16_t ack[15] = {
        (uint16_t)((01 << 8) | 042), 01000, 026 + 6,
        022, (uint16_t)(c->rx_cursor >> 16), (uint16_t)c->rx_cursor,
        (uint16_t)((01 << 8) | 01), 0, 03,
        (uint16_t)((01 << 8) | 042), 0, 020,
        512, 6, 512 * 6
    };
    dorado_ethernet_direct_transmit(&c->eth, ack, 15);
}

/* One BSP Mark packet: the mark byte, then any command text in the same Pup
 * (PupStream.SendMark flushes the stream, which is how Cedar sends a command). */
static void stp_send_mark(stp_client *c, uint8_t mark, const char *text)
{
    uint16_t pkt[600];
    size_t nbytes = 1 + (text ? strlen(text) : 0);
    size_t nwords = (nbytes + 1) / 2;
    uint8_t bytes[1024];
    bytes[0] = mark;
    if (text) memcpy(bytes + 1, text, nbytes - 1);
    if (nbytes & 1) bytes[nbytes] = 0;
    pkt[0] = (uint16_t)((01 << 8) | 042);
    pkt[1] = 01000;
    pkt[2] = (uint16_t)(026 + 2 * nwords);
    pkt[3] = 023;                                   /* BSP Mark */
    pkt[4] = (uint16_t)(c->tx_cursor >> 16);
    pkt[5] = (uint16_t)c->tx_cursor;
    pkt[6] = (uint16_t)((01 << 8) | 01);
    pkt[7] = 0;
    pkt[8] = 03;
    pkt[9] = (uint16_t)((01 << 8) | 042);
    pkt[10] = 0;
    pkt[11] = 020;
    for (size_t i = 0; i < nwords; i++)
        pkt[12 + i] = (uint16_t)((bytes[2 * i] << 8) | bytes[2 * i + 1]);
    pkt[12 + nwords] = 0177777;
    c->tx_cursor += (uint32_t)nbytes;
    dorado_ethernet_direct_transmit(&c->eth, pkt, (int)(13 + nwords));
}

static int stp_open(stp_client *c, const char *root)
{
    uint16_t rfc[15] = {
        (uint16_t)((01 << 8) | 042), 01000, 026 + 6,
        010, 012345, 067123,
        (uint16_t)((01 << 8) | 01), 0, 03,
        (uint16_t)((01 << 8) | 042), 0, 020,
        (uint16_t)((01 << 8) | 042), 0, 020
    };
    uint16_t pkt[600];
    dorado_ethernet_init(&c->eth);
    dorado_ethernet_set_ftp_root(&c->eth, root);
    /* Both byte-ID spaces are seeded from the RFC connection ID (PupBSPProt). */
    c->rx_cursor = c->tx_cursor = ((uint32_t)012345 << 16) | 067123;
    dorado_ethernet_direct_transmit(&c->eth, rfc, 15);
    EXPECT(c->eth.ftp_open, "STP connection opened");
    EXPECT(stp_take(c, pkt, 600) > 0 && pkt[3] == 010, "RFC reply");
    EXPECT(stp_take(c, pkt, 600) > 0 && pkt[3] == 022, "initial BSP allocation");
    return 0;
}

/* Collect the server's whole answer as a transcript: one line per BSP
 * segment, "M<octal mark>" for a mark and the literal text for data. */
static int stp_collect(stp_client *c, char *out, size_t outsz, int *n_plists,
                       int *n_marks)
{
    uint16_t pkt[600];
    size_t used = 0;
    int n;
    *n_plists = *n_marks = 0;
    out[0] = '\0';
    while ((n = stp_take(c, pkt, 600)) > 0) {
        uint16_t type = pkt[3];
        uint32_t id = ((uint32_t)pkt[4] << 16) | pkt[5];
        size_t nbytes = pkt[2] > 026 ? (size_t)(pkt[2] - 026) : 0;
        char seg[1024];
        size_t si = 0;
        if (type != 020 && type != 021 && type != 023 && type != 026) continue;
        EXPECT(id == c->rx_cursor, "byte ID %08x, expected %08x (a segment was "
               "lost or duplicated)", id, c->rx_cursor);
        c->rx_cursor = id + (uint32_t)nbytes;
        for (size_t i = 0; i < nbytes && si < sizeof seg - 1; i++) {
            uint16_t w = pkt[12 + i / 2];
            seg[si++] = (char)((i & 1) ? (w & 0xFF) : (w >> 8));
        }
        seg[si] = '\0';
        if (type == 023 || type == 026) {
            (*n_marks)++;
            used += (size_t)snprintf(out + used, outsz - used, "M%o\n",
                                     (unsigned)(uint8_t)seg[0]);
            if (si > 1)         /* text riding along with the mark */
                used += (size_t)snprintf(out + used, outsz - used, "%s\n",
                                         seg + 1);
        } else {
            if (seg[0] == '(') (*n_plists)++;
            used += (size_t)snprintf(out + used, outsz - used, "%s\n", seg);
        }
        if (used + 1024 > outsz) break;
        stp_ack(c);
    }
    return 0;
}

/* markNewDirectory(0o14) / markDirectory(0o12) with `pattern` as the
 * Name-Body, exactly as FSRemoteFileImpl.InnerEnumerate sends it. */
static int stp_enumerate(stp_client *c, uint8_t mark, const char *directory,
                         const char *pattern, char *out, size_t outsz,
                         int *n_plists, int *n_marks)
{
    char cmd[512];
    snprintf(cmd, sizeof cmd,
             "((User-Name Guest.pa)(Directory %s)(Name-Body %s)"
             "(Desired-property Directory)(Desired-property Name-Body)"
             "(Desired-property Version))", directory, pattern);
    stp_send_mark(c, mark, cmd);
    stp_send_mark(c, 06, NULL);              /* markEOC ends the command */
    return stp_collect(c, out, outsz, n_plists, n_marks);
}

static int test_stp_enumerate(void)
{
    stp_client c;
    char reply[65536];
    int plists, marks;

    /* 1. New-Directory: one HereIsPList, a bare plist per file, one EOC. */
    if (stp_open(&c, STP_ROOT)) return 1;
    if (stp_enumerate(&c, 014, "Cedar6.1", "VersionMap>*", reply, sizeof reply,
                      &plists, &marks))
        return 1;
    EXPECT(strstr(reply, "M13\n") == reply,
           "listing opens with markHereIsPList; got:\n%s", reply);
    EXPECT(marks == 2, "%d marks (want HereIsPList + EOC):\n%s", marks, reply);
    EXPECT(strstr(reply, "M6\n") != NULL, "listing ends with markEOC:\n%s",
           reply);
    EXPECT(plists == 11, "%d property lists (VersionMap holds 11 files):\n%s",
           plists, reply);
    /* The version and date are the release DF's, not a synthetic !1: the DF
     * pins CedarSource.VersionMap at !34, 04-Dec-86. */
    EXPECT(strstr(reply, "(Server-Filename <Cedar6.1>VersionMap>"
                         "CedarSource.VersionMap!34)") != NULL,
           "the version map itself is listed, at its DF version:\n%s", reply);
    EXPECT(strstr(reply, "(Creation-Date 04-Dec-86 10:05:32 PST)") != NULL,
           "and at its DF creation date:\n%s", reply);
    EXPECT(strstr(reply, "(Directory Cedar6.1>VersionMap)") != NULL,
           "Directory property carries the subdirectory:\n%s", reply);
    /* Lexical order, upper-cased, as an IFS enumerates (FS.mesa). */
    EXPECT(strstr(reply, "CedarSource.VersionMap") <
           strstr(reply, "CedarSymbols.VersionMap"), "sorted:\n%s", reply);
    dorado_ethernet_free(&c.eth);

    /* 2. A pattern that matches nothing is a refusal, not silence -- and
     *    fileNotFound(100B), the code that does NOT make the client fall back
     *    to the old Directory form. */
    if (stp_open(&c, STP_ROOT)) return 1;
    if (stp_enumerate(&c, 014, "Cedar6.1", "VersionMap>*.nonesuch", reply,
                      sizeof reply, &plists, &marks))
        return 1;
    EXPECT(plists == 0, "no plists for an empty match:\n%s", reply);
    EXPECT(strstr(reply, "M4\n") == reply, "markNo first:\n%s", reply);
    EXPECT(strstr(reply, "@File not found") != NULL,
           "fileNotFound(100B = '@') and its explanation:\n%s", reply);
    dorado_ethernet_free(&c.eth);

    /* 3. The old Directory form: a HereIsPList before EVERY entry. */
    if (stp_open(&c, STP_ROOT)) return 1;
    if (stp_enumerate(&c, 012, "Cedar6.1", "VersionMap>*.VersionMap", reply,
                      sizeof reply, &plists, &marks))
        return 1;
    EXPECT(plists == 2, "%d plists (two .VersionMap files):\n%s", plists, reply);
    EXPECT(marks == 3, "%d marks (a HereIsPList each, then EOC):\n%s", marks,
           reply);
    dorado_ethernet_free(&c.eth);

    /* 4. "*" crosses ">": CommandToolCommands.tioga's List has an X switch,
     *    "causes * to not match >", precisely because it does by default. */
    if (stp_open(&c, STP_ROOT)) return 1;
    if (stp_enumerate(&c, 014, "Cedar6.1", "*.VersionMap", reply, sizeof reply,
                      &plists, &marks))
        return 1;
    EXPECT(plists == 2, "%d plists for a level-crossing *:\n%s", plists, reply);
    EXPECT(strstr(reply, "<Cedar6.1>VersionMap>CedarSource.VersionMap") != NULL,
           "found a file one level down:\n%s", reply);
    dorado_ethernet_free(&c.eth);

    /* 5. An exact name (no wildcard) still enumerates. */
    if (stp_open(&c, STP_ROOT)) return 1;
    if (stp_enumerate(&c, 014, "Cedar6.1", "CommandTool>CommandTool.mesa",
                      reply, sizeof reply, &plists, &marks))
        return 1;
    EXPECT(plists == 1, "%d plists for an exact name:\n%s", plists, reply);
    EXPECT(strstr(reply, "(Name-Body CommandTool.mesa)") != NULL,
           "Name-Body is the leaf name:\n%s", reply);
    EXPECT(strstr(reply, "(Size 17048)") != NULL,
           "Size is the real byte count:\n%s", reply);
    dorado_ethernet_free(&c.eth);

    /* 6. Versions come from the DF, per file -- CommandTool.df pins
     *    CommandTool.mesa at !1 and CommandToolImpl.mesa at !2, and a flat
     *    "!1" for both would be a lie the old server told. */
    if (stp_open(&c, STP_ROOT)) return 1;
    if (stp_enumerate(&c, 014, "Cedar6.1", "CommandTool>CommandTool*.mesa",
                      reply, sizeof reply, &plists, &marks))
        return 1;
    EXPECT(strstr(reply, "CommandToolImpl.mesa!2)") != NULL,
           "CommandToolImpl.mesa is version 2:\n%s", reply);
    EXPECT(strstr(reply, "CommandTool.mesa!1)") != NULL,
           "CommandTool.mesa is version 1:\n%s", reply);
    dorado_ethernet_free(&c.eth);

    printf("  STP enumerate: New-Directory and Directory framing, level-"
           "crossing patterns, fileNotFound on no match\n");
    return 0;
}

/* The LookupFile packet exchange (PupType.fileLookup, 0o200): the single-Pup
 * protocol FSRemoteFileImpl.Info tries first, and the one that fills List's
 * version/size/date columns.  Reply body is PupBuffer.FileLookupReply --
 * version (1 word), createTime (2 words, seconds since the 1901 Pup epoch),
 * length (2 words). */
static int test_file_lookup(void)
{
    dorado_ethernet eth;
    uint16_t pkt[600];
    int n;
    const char *name = "[Cedar]<Cedar6.1>VersionMap>CedarSource.VersionMap";
    size_t len = strlen(name), words = (len + 1) / 2;

    dorado_ethernet_init(&eth);
    dorado_ethernet_set_ftp_root(&eth, STP_ROOT);

    pkt[0] = (uint16_t)((01 << 8) | 042);
    pkt[1] = 01000;
    pkt[2] = (uint16_t)(026 + 2 * words);
    pkt[3] = 0200;                                  /* fileLookup */
    pkt[4] = 0123; pkt[5] = 04567;                  /* request id */
    pkt[6] = (uint16_t)((01 << 8) | 01); pkt[7] = 0; pkt[8] = 061;
    pkt[9] = (uint16_t)((01 << 8) | 042); pkt[10] = 0; pkt[11] = 020;
    for (size_t i = 0; i < words; i++) {
        uint8_t hi = (uint8_t)name[2 * i];
        uint8_t lo = 2 * i + 1 < len ? (uint8_t)name[2 * i + 1] : 0;
        pkt[12 + i] = (uint16_t)((hi << 8) | lo);
    }
    pkt[12 + words] = 0177777;
    dorado_ethernet_direct_transmit(&eth, pkt, (int)(13 + words));

    n = (int)eth.rx_count;
    EXPECT(n >= 17, "fileLookup answered (%d words)", n);
    EXPECT(eth.rx_words[3] == 0201, "fileLookupReply type 0o%o",
           eth.rx_words[3]);
    EXPECT(eth.rx_words[4] == 0123 && eth.rx_words[5] == 04567,
           "reply carries the request id");
    EXPECT(eth.rx_words[2] == 026 + 10, "reply body is 10 bytes, got 0o%o",
           eth.rx_words[2]);
    EXPECT(eth.rx_words[12] == 34, "version %u (the DF pins !34)",
           eth.rx_words[12]);
    /* 04-Dec-86 10:05:32 PST = 1986-12-04 18:05:32 GMT = 534103532 Unix,
     * + 2177452800 (1901 -> 1970) = 2711556332 = 0o24136323654. */
    EXPECT((((uint32_t)eth.rx_words[13] << 16) | eth.rx_words[14]) ==
           2711556332u, "createTime 0o%o%o", eth.rx_words[13],
           eth.rx_words[14]);
    EXPECT((((uint32_t)eth.rx_words[15] << 16) | eth.rx_words[16]) == 66296u,
           "length %u", (((uint32_t)eth.rx_words[15] << 16) | eth.rx_words[16]));

    /* A name we do not have is fileLookupError (0o202) = noSuchFile, not
     * silence: silence costs the client four retries and 30 seconds of
     * negative caching against the whole server. */
    eth.rx_pos = eth.rx_count;
    pkt[2] = (uint16_t)(026 + 2 * words);
    for (size_t i = 0; i < words; i++) pkt[12 + i] = ('x' << 8) | 'x';
    dorado_ethernet_direct_transmit(&eth, pkt, (int)(13 + words));
    EXPECT(eth.rx_words[3] == 0202, "fileLookupError type 0o%o",
           eth.rx_words[3]);

    printf("  LookupFile: version, 1901-epoch create time and byte length for"
           " a release file; fileLookupError for a name we lack\n");
    dorado_ethernet_free(&eth);
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_microcode_boot_reply_queue();
    rc |= test_eftp_boot_reply_queue();
    rc |= test_eftp_boot_full_transfer();
    rc |= test_bootdir_reply_format();
    rc |= test_bootdir_all_games();
    rc |= test_ftp_netdir_rtp_and_bsp_alloc();
    rc |= test_stp_enumerate();
    rc |= test_file_lookup();
    if (rc == 0) printf("All ethernet tests passed.\n");
    return rc;
}
