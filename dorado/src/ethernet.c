#include "ethernet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DORADO_ETH_TRACE=1 traces the first 256 controller operations;
 * larger values raise the cap. */
static long eth_trace_cap(void)
{
    static long cached = -1;
    if (cached < 0) {
        const char *v = getenv("DORADO_ETH_TRACE");
        cached = v ? strtol(v, NULL, 0) : 0;
        if (cached == 1) cached = 256;
    }
    return cached;
}

static void eth_trace(const dorado_ethernet *eth, const char *op,
                      int task, uint8_t tioa, uint16_t data)
{
    long cap = eth_trace_cap();
    if (!cap) return;
    uint64_t n = eth->control_writes[task & 0xF] + eth->data_writes +
                 eth->data_reads;
    if (n > (uint64_t)cap) return;
    fprintf(stderr,
            "ETH %-5s task=%o tioa=%03o data=%06o rx_on=%u tx_on=%u "
            "eop=%u complete=%u tx_count=%zu rx=%zu/%zu\n",
            op, task, tioa, data, eth->rx_on, eth->tx_on,
            eth->tx_eop, eth->tx_complete, eth->tx_count,
            eth->rx_pos, eth->rx_count);
}

static void eth_clear_rx(dorado_ethernet *eth)
{
    free(eth->rx_words);
    free(eth->rx_attention);
    eth->rx_words = NULL;
    eth->rx_attention = NULL;
    eth->rx_count = 0;
    eth->rx_pos = 0;
}

void dorado_ethernet_init(dorado_ethernet *eth)
{
    memset(eth, 0, sizeof *eth);
    eth->local_host = 042;
    eth->remote_host = 01;
    dorado_ethernet_set_boot_file(
        eth, 0110, "../chm/microcode/AltoMesaDorado.eb!1");
    dorado_ethernet_set_boot_file(
        eth, 0111, "../chm/microcode/SmalltalkDorado.eb!1");
    dorado_ethernet_set_boot_file(
        eth, 0113, "../chm/dorado/CedarDorado.eb!6");
    dorado_ethernet_set_boot_file(
        eth, 0114, "../chm/microcode/TestDorado.eb!1");
    dorado_ethernet_set_eftp_boot_file(
        eth, "../chm/bootfiles/NETEXEC.BOOT!8");
}

void dorado_ethernet_free(dorado_ethernet *eth)
{
    if (!eth) return;
    eth_clear_rx(eth);
    free(eth->eftp_words);
    eth->eftp_words = NULL;
}

void dorado_ethernet_set_boot_file(dorado_ethernet *eth,
                                   uint16_t offset,
                                   const char *path)
{
    if (!eth || !path) return;
    char *dst = NULL;
    switch (offset) {
    case 0110: dst = eth->boot_110_path; break;
    case 0111: dst = eth->boot_111_path; break;
    case 0113: dst = eth->boot_113_path; break;
    case 0114: dst = eth->boot_114_path; break;
    default: return;
    }
    snprintf(dst, 256, "%s", path);
}

void dorado_ethernet_set_eftp_boot_file(dorado_ethernet *eth, const char *path)
{
    if (!eth || !path) return;
    snprintf(eth->eftp_boot_path, sizeof eth->eftp_boot_path, "%s", path);
}

static const char *eth_boot_path(const dorado_ethernet *eth, uint16_t offset)
{
    switch (offset) {
    case 0110: return eth->boot_110_path;
    case 0111: return eth->boot_111_path;
    case 0113: return eth->boot_113_path;
    case 0114: return eth->boot_114_path;
    default: return NULL;
    }
}

static int read_be_word(FILE *fp, uint16_t *out)
{
    int hi = fgetc(fp);
    int lo = fgetc(fp);
    if (hi == EOF && lo == EOF) return 0;
    if (hi == EOF || lo == EOF) return -1;
    *out = (uint16_t)(((uint16_t)hi << 8) | (uint16_t)lo);
    return 1;
}

static int append_rx_word(dorado_ethernet *eth, size_t *cap,
                          uint16_t word, int attention)
{
    if (eth->rx_count >= *cap) {
        size_t ncap = *cap ? *cap * 2 : 1024;
        uint16_t *nw = realloc(eth->rx_words, ncap * sizeof eth->rx_words[0]);
        if (!nw) return 0;
        eth->rx_words = nw;
        uint8_t *na = realloc(eth->rx_attention,
                              ncap * sizeof eth->rx_attention[0]);
        if (!na) return 0;
        eth->rx_attention = na;
        *cap = ncap;
    }
    eth->rx_words[eth->rx_count] = word;
    eth->rx_attention[eth->rx_count] = attention ? 1 : 0;
    eth->rx_count++;
    return 1;
}

static int append_reply(dorado_ethernet *eth, size_t *cap,
                        uint16_t seq, const uint16_t *payload,
                        size_t nwords)
{
    uint16_t lhost = eth->local_host;
    uint16_t rhost = eth->remote_host;
    uint16_t length = (uint16_t)(026 + 2 * nwords);
    uint16_t header[12] = {
        (uint16_t)((lhost << 8) | rhost),
        DORADO_PUP_TYPE_ETHERNET,
        length,
        DORADO_PUP_TYPE_MICROCODE_BOOT_REPLY,
        01,
        seq,
        lhost,
        01,
        01,
        rhost,
        00,
        04
    };
    for (size_t i = 0; i < sizeof header / sizeof header[0]; i++) {
        if (!append_rx_word(eth, cap, header[i], 0)) return 0;
    }
    for (size_t i = 0; i < nwords; i++) {
        if (!append_rx_word(eth, cap, payload[i], 0)) return 0;
    }
    if (!append_rx_word(eth, cap, 0, 0)) return 0; /* dummy CRC */
    if (!append_rx_word(eth, cap, 0, 1)) return 0; /* good status */
    eth->replies_queued++;
    return 1;
}

static int eth_queue_boot_replies(dorado_ethernet *eth, uint16_t offset)
{
    const char *path = eth_boot_path(eth, offset);
    if (!path || !*path) return 0;

    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;

    uint16_t version = 0;
    if (read_be_word(fp, &version) != 1 || version != 1) {
        fclose(fp);
        return 0;
    }
    if (fseek(fp, 512, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    eth_clear_rx(eth);

    size_t payload_cap = 16384;
    uint16_t *payload = malloc(payload_cap * sizeof payload[0]);
    if (!payload) {
        fclose(fp);
        return 0;
    }
    size_t payload_n = 0;
    for (;;) {
        uint16_t word = 0;
        int rc = read_be_word(fp, &word);
        if (rc == 0) break;
        if (rc < 0) {
            free(payload);
            fclose(fp);
            return 0;
        }
        if (payload_n >= payload_cap) {
            size_t ncap = payload_cap * 2;
            uint16_t *nw = realloc(payload, ncap * sizeof payload[0]);
            if (!nw) {
                free(payload);
                fclose(fp);
                return 0;
            }
            payload = nw;
            payload_cap = ncap;
        }
        payload[payload_n++] = word;
    }
    fclose(fp);

    size_t cap = 0;
    size_t pos = 0;
    uint16_t seq = 0;
    while (pos < payload_n) {
        size_t max_words = 255;
        size_t n = payload_n - pos;
        if (n > max_words) n = max_words;
        if (!append_reply(eth, &cap, seq, &payload[pos], n)) {
            free(payload);
            eth_clear_rx(eth);
            return 0;
        }
        pos += n;
        seq++;
    }
    free(payload);

    if (!append_reply(eth, &cap, seq, NULL, 0)) {
        eth_clear_rx(eth);
        return 0;
    }
    eth->rx_pos = 0;
    return 1;
}

/* Build one EFTP packet (Data or End) addressed to the booting Alto on
 * the EFTP receiver socket. Mirrors append_reply's on-wire delivery shape
 * (12-word Ethernet/Pup header, payload, dummy trailer, attention status)
 * but with EFTP Pup fields: type, sequence number in pupID+1, dest socket
 * 20. EFTPSPEC: Pup overhead is 026 bytes, so length = 026 + 2*nwords. */
static int append_eftp_packet(dorado_ethernet *eth, size_t *cap,
                              uint16_t seq, uint16_t pup_type,
                              const uint16_t *payload, size_t nwords)
{
    uint16_t lhost = eth->local_host;   /* booting Alto (destination) */
    uint16_t rhost = eth->remote_host;  /* boot server (source)       */
    uint16_t length = (uint16_t)(026 + 2 * nwords);
    uint16_t header[12] = {
        (uint16_t)((lhost << 8) | rhost),  /* etherAdr: dest||source   */
        DORADO_PUP_TYPE_ETHERNET,          /* etherType = typePup      */
        length,                            /* pupLength                */
        pup_type,                          /* EFTP Data / End          */
        00,                                /* pupID high               */
        seq,                               /* pupID low = EFTP seq #   */
        lhost,                             /* pupDNetHost = Alto, net 0 */
        00,                                /* pupDSocket high          */
        DORADO_EFTP_RECEIVER_SOCKET,       /* pupDSocket low = 20      */
        rhost,                             /* pupSNetHost = server     */
        00,                                /* pupSSocket high          */
        04                                 /* pupSSocket low = misc    */
    };
    for (size_t i = 0; i < sizeof header / sizeof header[0]; i++) {
        if (!append_rx_word(eth, cap, header[i], 0)) return 0;
    }
    for (size_t i = 0; i < nwords; i++) {
        if (!append_rx_word(eth, cap, payload[i], 0)) return 0;
    }
    if (!append_rx_word(eth, cap, 0177777, 0)) return 0; /* nil Pup cksum */
    /* The hardware CRC word follows the Pup on the real wire (HM 11:
     * "the receiver delivers first the CRC word and then a status
     * word"). AltoEther.mc's receive depends on it: the loader's
     * eICLoc=269 covers hdr+data+pup-cksum exactly; the loop then
     * reads the CRC as its buffer-full extra word and the IOAtten test
     * at that instruction sees the status next ("the word in T is the
     * CRC and the next input is the status"). Without the CRC word the
     * status arrives one slot early and every packet posts
     * InBufOverflow instead of InDone. */
    if (!append_rx_word(eth, cap, 0, 0)) return 0;       /* dummy HW CRC  */
    if (!append_rx_word(eth, cap, 0, 1)) return 0;       /* good status   */
    eth->eftp_replies_queued++;
    return 1;
}

/* Queue the whole Alto boot file as an EFTP stream (Data packets seq 0..,
 * then an End packet). The booting Alto Acks each packet; the in-process
 * fake pre-queues the whole stream and absorbs the Acks. Unlike a
 * microcode boot, the entire file is sent verbatim (no 256-word header
 * skip): an Alto boot file is itself the loadable image. */
static int eth_queue_eftp_boot(dorado_ethernet *eth, uint16_t bfn)
{
    (void)bfn;
    if (!eth->eftp_boot_path[0]) return 0;
    FILE *fp = fopen(eth->eftp_boot_path, "rb");
    if (!fp) return 0;

    size_t payload_cap = 16384;
    uint16_t *payload = malloc(payload_cap * sizeof payload[0]);
    if (!payload) { fclose(fp); return 0; }
    size_t payload_n = 0;
    for (;;) {
        uint16_t word = 0;
        int rc = read_be_word(fp, &word);
        if (rc == 0) break;
        if (rc < 0) { free(payload); fclose(fp); return 0; }
        if (payload_n >= payload_cap) {
            size_t ncap = payload_cap * 2;
            uint16_t *nw = realloc(payload, ncap * sizeof payload[0]);
            if (!nw) { free(payload); fclose(fp); return 0; }
            payload = nw;
            payload_cap = ncap;
        }
        payload[payload_n++] = word;
    }
    fclose(fp);

    free(eth->eftp_words);
    eth->eftp_words = payload;
    eth->eftp_len = payload_n;
    eth->eftp_pos = 0;
    eth->eftp_seq = 0;
    eth->eftp_state = 1;
    return 1;
}

/* Put the current lock-step packet (per eftp_state/seq/pos) on the
 * wire as the only rx content, with a short server-turnaround hold so
 * the loader's poll loop is armed before the packet "arrives". */
static int eth_eftp_deliver_current(dorado_ethernet *eth)
{
    eth_clear_rx(eth);
    size_t cap = 0;
    int ok;
    if (eth->eftp_state == 1) {
        size_t n = eth->eftp_len - eth->eftp_pos;
        if (n > DORADO_EFTP_DATA_WORDS) n = DORADO_EFTP_DATA_WORDS;
        ok = append_eftp_packet(eth, &cap, eth->eftp_seq,
                                DORADO_PUP_TYPE_EFTP_DATA,
                                &eth->eftp_words[eth->eftp_pos], n);
    } else {
        /* End (state 2) and the dally End (state 3): no data. */
        ok = append_eftp_packet(eth, &cap, eth->eftp_seq,
                                DORADO_PUP_TYPE_EFTP_END, NULL, 0);
    }
    if (!ok) { eth_clear_rx(eth); return 0; }
    eth->rx_pos = 0;
    /* Server turnaround: hold EIT wakeups (in wakeup-poll ticks,
     * ~1/BB-cycle) so a stale receiver-enable window cannot steal
     * the packet before the loader re-arms its control block. The
     * post-Mayday window (EOT wrap-up + DoEtherOutput return + boot3
     * arming) spans tens of thousands of cycles; a real boot server's
     * turnaround is milliseconds. 60000 ticks ~ 2 ms. */
    eth->rx_hold = 60000;
    /* EFTPSPEC: the sender retransmits an unacknowledged packet about
     * once a second until the Ack arrives. The receiver depends on
     * this — EtherBoot.asm's poll loop re-arms ePLoc on its own
     * timeout, and that store can race the EIT's InDone post and wipe
     * it; the lost packet is then recovered only by the sender's
     * resend (a duplicate Data packet just gets re-Acked). Ticks are
     * ~2 per microcycle; the loader's process-and-Ack window after
     * InDone is ~650 K cycles (~1.3 M ticks), so 3 M ticks resends
     * only when the receiver has really lost the packet. */
    eth->eftp_resend_timer = 3000000;
    if (eth->eftp_seq > eth->eftp_max_seq) eth->eftp_max_seq = eth->eftp_seq;
    return 1;
}

/* Lock-step advance on the receiver's EFTP Ack (EFTPSPEC): the Ack
 * for the packet on the wire releases the next Data packet, then the
 * End, then one more End to close the receiver's dally. A duplicate
 * Ack (older seq) retransmits the current packet. */
static void eth_eftp_ack(dorado_ethernet *eth, uint16_t acked_seq)
{
    if (eth->eftp_state == 0 || !eth->eftp_words) return;
    if (acked_seq != eth->eftp_seq) {
        (void)eth_eftp_deliver_current(eth);   /* dup/old Ack: resend */
        return;
    }
    switch (eth->eftp_state) {
    case 1: {
        size_t n = eth->eftp_len - eth->eftp_pos;
        if (n > DORADO_EFTP_DATA_WORDS) n = DORADO_EFTP_DATA_WORDS;
        eth->eftp_pos += n;
        eth->eftp_seq++;
        if (eth->eftp_pos >= eth->eftp_len) eth->eftp_state = 2;
        break;
    }
    case 2:
        /* The End was Acked; send one more End (dally close) with the
         * next seq, then the transfer is over. */
        eth->eftp_seq++;
        eth->eftp_state = 3;
        break;
    case 3:
    default:
        eth->eftp_state = 0;
        return;
    }
    (void)eth_eftp_deliver_current(eth);
    if (eth->eftp_state == 3) eth->eftp_state = 0;
}

/* The Alto Ethernet boot loader, served as the payload of a
 * "breath of life" packet (ether type 602B). Words [31B..426B] of
 * `bootLoaderProgram` in EtherBoot.mesa (CHM, "Snitched from Taft's
 * EtherBoot.asm"): word 0 is the ether dest/source header (filled in
 * per packet), word 1 is typeBreathOfLife. The booting Alto stores the
 * packet at VM 1..376B and jumps to location 3; the loader then reads
 * the boot file number from keyboard word 177035, broadcasts a Mayday
 * Pup, and receives the boot file by EFTP. */
static const uint16_t eth_bol_loader[254] = {
    0, 0602, 022574, 0100000, 040437, 0102000, 034431, 0164000,
    061005, 0102460, 024567, 034572, 061006, 024565, 034570, 061006,
    024564, 034566, 061006, 020565, 034565, 061005, 0125220, 046573,
    020576, 061004, 0123400, 030551, 041211, 04416, 0, 01000,
    026, 0244, 0, 0, 0, 0, 04, 0,
    0, 020, 0177777, 055210, 025400, 0107000, 045400, 041411,
    020547, 041207, 020544, 061004, 06531, 034517, 030544, 051606,
    020510, 041605, 042526, 0102460, 041601, 020530, 061004, 021601,
    0101014, 0414, 061020, 014737, 0773, 014517, 0754, 020517,
    061004, 030402, 02402, 0, 0732, 034514, 0162414, 0746,
    021001, 024511, 0106414, 0742, 021003, 0163400, 035005, 024501,
    0106415, 0175014, 0733, 021000, 042465, 034457, 056445, 055775,
    055776, 0101300, 041400, 020467, 041401, 020432, 041402, 0121400,
    041403, 021006, 041411, 021007, 041412, 021010, 041413, 021011,
    041406, 021012, 041407, 021013, 041410, 015414, 06427, 012434,
    06426, 020421, 024437, 0134000, 030417, 02422, 0177035, 026,
    0415, 0427, 0567, 0607, 0777, 0177751, 0177641, 0177600,
    0225, 0177624, 01013, 0764, 0431, 0712, 0634, 0735,
    0611, 0567, 0564, 0566, 036, 02, 03, 015,
    030, 0377, 01000, 0177764, 0436, 054731, 050750, 020753,
    040745, 0102460, 040737, 020762, 061004, 020734, 0105304, 0406,
    020743, 0101014, 014741, 0772, 02712, 034754, 0167700, 0116415,
    024752, 021001, 0106414, 0754, 021000, 024703, 0106414, 0750,
    021003, 0163400, 024736, 0106405, 0404, 0121400, 0101404, 0740,
    044714, 021005, 042732, 024664, 0122405, 0404, 0101405, 04404,
    0727, 010656, 034654, 024403, 0120500, 0101404, 0777, 040662,
    040664, 040664, 0102520, 061004, 020655, 0101015, 0776, 0106415,
    01400, 014634, 0761, 020673, 061004, 0400, 061005, 0102000,
    0143000, 034672, 024667, 0166400, 061005, 04670, 020663, 034664,
    0164000, 0147000, 061005, 024762, 0132414, 0133000, 020636, 034416,
    0101015, 0156415, 0131001, 0754, 024643, 044625, 0101015, 0750,
    014623, 04644, 020634, 061004, 02000, 0176764
};

/* "Transmit" the buffered packet. Called when EOT sets TxEOP with words
 * in the Fifo (HM §11: "the transmitter ends normally when the Fifo is
 * empty and TxEOP is true"). Dispatches the packet to the in-process
 * boot server, then models TxGone: "TxGone clears TxEOP to cause a
 * wakeup at the end of each packet" — EOT's post-SendEOP Block wakes and
 * it reads status / turns the transmitter off (InitialEther.mc EOStop). */
static void eth_tx_packet_done(dorado_ethernet *eth)
{
    if (eth->tx_count < 4) return;     /* runt: no Pup header to dispatch */
    eth->tx_complete = 1;
    eth->last_tx_pup_type = eth->tx_words[3];
    if (getenv("DORADO_ETH_TX_TRACE")) {
        fprintf(stderr, "TX n=%zu:", eth->tx_count);
        for (size_t i = 0; i < eth->tx_count && i < 14; i++)
            fprintf(stderr, " %06o", eth->tx_words[i]);
        fprintf(stderr, "\n");
    }

    /* Stage-2: a Mayday Pup is the Alto software-boot request. Serve the
     * configured Alto boot file as a LOCK-STEP EFTP stream: packet 0
     * now, each next packet on the previous one's Ack. */
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        eth->tx_words[3] == DORADO_PUP_TYPE_MAYDAY) {
        eth->eftp_requests_seen++;
        eth->eftp_last_bfn = eth->tx_words[5];
        if (eth_queue_eftp_boot(eth, eth->eftp_last_bfn)) {
            (void)eth_eftp_deliver_current(eth);
        }
        return;
    }
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        eth->tx_words[3] == DORADO_PUP_TYPE_EFTP_ACK) {
        if (getenv("DORADO_ETH_TX_TRACE")) {
            fprintf(stderr, "TX_ACK n=%zu:", eth->tx_count);
            for (size_t i = 0; i < eth->tx_count && i < 16; i++)
                fprintf(stderr, " %06o", eth->tx_words[i]);
            fprintf(stderr, " (server seq=%u)\n", eth->eftp_seq);
        }
        eth_eftp_ack(eth, eth->tx_words[5]);
        return;
    }
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        (eth->tx_words[3] & 0377) == 0204) {
        /* Type 204B request. NetExec uses 204B on two sockets:
         * psRouteInfo (2) = routing-info request -> answer with a
         * GatewayInfoReply (201B, routing tuples); psMiscServ (4) =
         * Alto time request -> answer 205B with the NTime body.
         * Reply directed to the requestor's source port. */
        uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
        uint16_t req_dsock_lo = eth->tx_words[8];
        uint16_t req_ssock_hi = eth->tx_words[10];
        uint16_t req_ssock_lo = eth->tx_words[11];
        eth_clear_rx(eth);
        size_t cap = 0;
        static const uint16_t routing[2] = {
            (uint16_t)((1 << 8) | 1),       /* target net 1, gw net 1 */
            (uint16_t)((01 << 8) | 0)       /* gw host 1, hops 0      */
        };
        static const uint16_t ntime[5] = {
            0x9C8Eu, 0x0000u, 0x0800u, 121u, 305u
        };
        const uint16_t *body = (req_dsock_lo == 02) ? routing : ntime;
        int blen = (req_dsock_lo == 02) ? 2 : 5;
        uint16_t rtype = (req_dsock_lo == 02) ? 0201 : 0205;
        uint16_t hdr[12];
        hdr[0]  = (uint16_t)((req_src_host << 8) | eth->remote_host);
        hdr[1]  = DORADO_PUP_TYPE_ETHERNET;
        hdr[2]  = (uint16_t)(026 + 2 * blen);
        hdr[3]  = rtype;
        hdr[4]  = eth->tx_words[4];
        hdr[5]  = eth->tx_words[5];
        hdr[6]  = req_src_host;     /* dnet 0 = "this net": must
                                     * deliver locally even before the
                                     * requestor learns its net number
                                     * (otherwise the routing reply
                                     * that would teach it localNet
                                     * gets forwarded, a bootstrap
                                     * deadlock). */
        hdr[7]  = req_ssock_hi;
        hdr[8]  = req_ssock_lo;
        hdr[9]  = (uint16_t)((1 << 8) | eth->remote_host);
        hdr[10] = eth->tx_words[7];
        hdr[11] = eth->tx_words[8];
        int ok = 1;
        for (int i = 0; i < 12 && ok; i++)
            ok = append_rx_word(eth, &cap, hdr[i], 0);
        for (int i = 0; i < blen && ok; i++)
            ok = append_rx_word(eth, &cap, body[i], 0);
        if (ok) ok = append_rx_word(eth, &cap, 0177777, 0);
        /* No dummy CRC word here: the Pup driver computes the packet
         * length from the ending count, and EtherPupFilter requires
         * (pup.length+5) rshift 1 == packetLength exactly; an extra
         * word makes every reply silently fail the filter. */
        if (ok) ok = append_rx_word(eth, &cap, 0, 1);
        if (ok) { eth->rx_pos = 0; eth->time_bcasts++; }
        return;
    }

    if (eth->tx_words[1] != DORADO_PUP_TYPE_ETHERNET ||
        eth->tx_words[3] != DORADO_PUP_TYPE_MICROCODE_BOOT_REQUEST) {
        int all_zero = 1;
        for (size_t i = 0; i < eth->tx_count; i++) {
            if (eth->tx_words[i] != 0) {
                all_zero = 0;
                break;
            }
        }
        if (!all_zero) return;
        eth->requests_seen++;
        eth->last_boot_offset = 0110;
        (void)eth_queue_boot_replies(eth, eth->last_boot_offset);
        return;
    }
    eth->requests_seen++;
    eth->last_boot_offset = eth->tx_words[5];
    (void)eth_queue_boot_replies(eth, eth->last_boot_offset);
}

int dorado_ethernet_breath_of_life(dorado_ethernet *eth)
{
    /* Boot servers broadcast breath-of-life packets periodically; only
     * deliver one when the receiver is listening and has consumed
     * everything already queued (a real Alto discards them otherwise,
     * and the prequeued reply streams must not be polluted). */
    if (!eth->rx_on || eth->rx_pos < eth->rx_count) return 0;
    if (eth->eftp_state != 0) return 0;   /* EFTP transfer in flight */

    eth_clear_rx(eth);
    size_t cap = 0;
    size_t n = sizeof eth_bol_loader / sizeof eth_bol_loader[0];
    /* Word 0: ether dest,,source — broadcast from the boot server. */
    uint16_t hdr = (uint16_t)((0 << 8) | eth->remote_host);
    if (!append_rx_word(eth, &cap, hdr, 0)) return 0;
    for (size_t i = 1; i < n; i++) {
        if (!append_rx_word(eth, &cap, eth_bol_loader[i], 0)) return 0;
    }
    if (!append_rx_word(eth, &cap, 0, 0)) return 0;  /* dummy CRC   */
    if (!append_rx_word(eth, &cap, 0, 1)) return 0;  /* good status */
    eth->rx_pos = 0;
    eth->bol_queued++;
    return 1;
}

int dorado_ethernet_time_broadcast(dorado_ethernet *eth)
{
    /* A real PARC Ethernet always carries a time server. NetExec's
     * startup raw-listens for an Alto time-protocol packet (Pup types
     * 200B..203B family) before its contexts ever cycle; on a silent
     * wire it waits forever. Broadcast an AltoTimeReply (201B) Pup
     * the way a time server would, with the NTime body:
     * time(2 words, seconds since 1-Jan-1901), zone(sign/hour/min),
     * beginDST, endDST. */
    if (!eth->rx_on || eth->rx_pos < eth->rx_count) return 0;
    if (eth->eftp_state != 0) return 0;

    eth_clear_rx(eth);
    size_t cap = 0;
    /* GatewayInfoReply body: 4-byte tuples of
     * <targetNet, gatewayNet, gatewayHost, hopCount>. One tuple:
     * net 1 is directly connected (hops 0) via the server. NetExec's
     * routing probe (LocateNet 0) raw-listens for this to learn its
     * local net number before its contexts run. */
    static const uint16_t body[2] = {
        (uint16_t)((1 << 8) | 1),               /* target 1, gw net 1 */
        (uint16_t)((01 << 8) | 0)               /* gw host 1, hops 0  */
    };
    uint16_t hdr[12];
    /* NetExec's raw listener (filter at VM 3227B..3303B of
     * NETEXEC.BOOT) demands: InDone post, nonzero SOURCE host,
     * nonzero (directed) DEST host, etherType Pup, a received-word
     * count matching (pupLength+5) rshift 1, dSocket hi = 0 and
     * lo = 60B, and Pup type in 200B..203B (the GatewayInfo family). */
    hdr[0]  = (uint16_t)((eth->local_host << 8) | eth->remote_host);
    hdr[1]  = DORADO_PUP_TYPE_ETHERNET;
    hdr[2]  = (uint16_t)(026 + 2 * 2);                 /* pup length  */
    hdr[3]  = 0201;
    hdr[4]  = 0; hdr[5] = 0;                           /* pup id      */
    hdr[6]  = eth->local_host;                         /* dnet0,,dhost */
    /* Alternate destination sockets: NetExec's pre-context raw
     * routing probe listens on socket 60B (its fixed sequence-derived
     * port), while the context-level GatewayListener owns psRouteInfo
     * (socket 2). Serve both. */
    hdr[7]  = 0;
    hdr[8]  = (uint16_t)((eth->time_bcasts & 1) ? 02 : 060);
    hdr[9]  = (uint16_t)((1 << 8) | eth->remote_host); /* snet1,,shost */
    hdr[10] = 0; hdr[11] = 02;                         /* psRouteInfo */
    for (int i = 0; i < 12; i++)
        if (!append_rx_word(eth, &cap, hdr[i], 0)) return 0;
    for (int i = 0; i < 2; i++)
        if (!append_rx_word(eth, &cap, body[i], 0)) return 0;
    if (!append_rx_word(eth, &cap, 0177777, 0)) return 0; /* no cksum */
    if (!append_rx_word(eth, &cap, 0, 0)) return 0;       /* dummy CRC */
    if (!append_rx_word(eth, &cap, 0, 1)) return 0;       /* status    */
    eth->rx_pos = 0;
    eth->time_bcasts++;
    return 1;
}

static uint16_t eth_read(void *ctx, int task, int subtask,
                         uint8_t tioa, int *bad)
{
    dorado_ethernet *eth = ctx;
    (void)subtask;
    if (bad) *bad = 0;
    if (tioa == DORADO_ETHERNET_TIOA_CTL) {
        eth_trace(eth, "read", task, tioa, 0);
        return (uint16_t)((uint16_t)eth->local_host << 8);
    }
    if (tioa != DORADO_ETHERNET_TIOA_DATA ||
        task != DORADO_ETHERNET_TASK_EIT) {
        if (bad) *bad = 1;
        return 0xFFFF;
    }

    eth->data_reads++;
    if (eth->rx_pos >= eth->rx_count || eth->rx_hold) {
        /* rx_hold: the packet is still "on the wire" (network /
         * server turnaround) — nothing has reached the bus register. */
        if (bad) *bad = 1;
        return 0xFFFF;
    }
    uint16_t word = eth->rx_words[eth->rx_pos];
    eth->rx_pos++;
    eth_trace(eth, "read", task, tioa, word);
    return word;
}

static void eth_write(void *ctx, int task, int subtask,
                      uint8_t tioa, uint16_t data)
{
    dorado_ethernet *eth = ctx;
    (void)subtask;
    if (tioa == DORADO_ETHERNET_TIOA_CTL) {
        if (eth->control_writes[task & 0xF] == 0) {
            eth->first_control[task & 0xF] = data;
        }
        eth_trace(eth, "write", task, tioa, data);
        {
            extern int dorado_trace_gate;
            extern unsigned long long dorado_trace_cycle;
            if (dorado_trace_gate && getenv("DORADO_ETHC_TRACE")) {
                fprintf(stderr,
                        "ETHC cyc=%llu task=%o ctl=%06o "
                        "(txon=%d txeop=%d cnt=%zu) rxon=%d\n",
                        dorado_trace_cycle, task & 017, data & 0177777,
                        eth->tx_on, eth->tx_eop, eth->tx_count,
                        eth->rx_on);
            }
        }
        eth->control_last[task & 0xF] = data;
        eth->control_writes[task & 0xF]++;

        /* EthC control register, HM §11 Figure 16. The register is
         * shared controller state; any task may write it (Initial
         * drives it from the emulator task, EOT/EIT from their own). */
        if (!(data & DORADO_ETHC_TXCMDENBLN)) {
            uint8_t on = (data & DORADO_ETHC_TXON) != 0;
            if (on && !eth->tx_on) {
                eth->tx_starts++;
                eth->tx_count = 0;
                eth->tx_complete = 0;
            }
            if (!on && eth->tx_on) eth->tx_stops++;
            eth->tx_on = on;
            if (!on) {
                /* "TxEOP/TxCntDwn cleared by TxOn = 0." Resetting the
                 * transmitter also discards the Fifo. */
                eth->tx_eop = 0;
                eth->tx_cntdwn = 0;
                eth->tx_count = 0;
            } else {
                eth->tx_eop = (data & DORADO_ETHC_TXEOP) != 0;
                /* TxCntDwn delays wakeups until the next Pendulum tick
                 * (16 us). This packet-level fake has no Pendulum;
                 * treat the tick as already due so EWait loops simply
                 * count down one iteration per wakeup. */
                eth->tx_cntdwn = 0;
                if (eth->tx_eop && eth->tx_count > 0) {
                    eth_tx_packet_done(eth);
                    /* TxGone: end of packet clears TxEOP, waking EOT. */
                    eth->tx_eop = 0;
                    eth->tx_eops++;
                    eth->tx_count = 0;
                }
            }
        }
        if (!(data & DORADO_ETHC_RXCMDENBLN)) {
            uint8_t on = (data & DORADO_ETHC_RXON) != 0;
            if (on && !(data & DORADO_ETHC_RXBOPN) && eth->rx_on) {
                /* RxBOP' written 0 (WaitForBOP): discard the rest of
                 * the current packet; the controller sets RxBOP again
                 * when the first word of the next packet is available.
                 * The prequeued stream discards synchronously, but the
                 * REAL drain takes ~5.4 us per word (HM 11: words
                 * emerge from the Fifo at wire speed) during which the
                 * EIT sleeps and the emulated program catches up (the
                 * EtherBoot loader re-arms eICLoc in that window; an
                 * instant re-wakeup runs EIIdle with count=0, posts
                 * CountZero, and eats the next packet's first word).
                 * Model the drain as a wakeup-poll hold. */
                if (!eth->rx_hold) {
                    uint32_t skipped = 0;
                    while (eth->rx_pos < eth->rx_count &&
                           !eth->rx_attention[eth->rx_pos]) {
                        eth->rx_pos++;
                        skipped++;
                    }
                    if (eth->rx_pos < eth->rx_count) {
                        eth->rx_pos++;
                        skipped++;
                    }
                    eth->rx_hold = skipped * 170u;
                }
                /* During a hold nothing has arrived yet, so there is
                 * nothing to discard: WaitForBOP just waits. */
            }
            eth->rx_on = on;
        }
        if (!(data & DORADO_ETHC_TESTCMDENBLN)) {
            eth->no_wakeups = (data & DORADO_ETHC_NOWAKEUPS) != 0;
        }
        return;
    }

    if (tioa == DORADO_ETHERNET_TIOA_DATA &&
        task == DORADO_ETHERNET_TASK_EOT) {
        eth->data_writes++;
        eth_trace(eth, "write", task, tioa, data);
        if (eth->tx_count < sizeof eth->tx_words / sizeof eth->tx_words[0]) {
            eth->tx_words[eth->tx_count++] = data;
        }
    }
}

static int eth_attention(void *ctx, int task, uint8_t tioa)
{
    /* HM §11: "IOAtten branches when a status word is present in the
     * receiver bus register" — a LEVEL on the word the next Pd←Input
     * would deliver, not a latch. The CPU samples it at instruction
     * issue (cpu.c io_atten_at_issue). */
    dorado_ethernet *eth = ctx;
    if (getenv("DORADO_ATTEN_TRACE")) {
        static long n = 0;
        n++;
        if (eth->rx_pos < eth->rx_count) {
            fprintf(stderr, "ETH_ATTEN task=%o tioa=%03o pos=%zu/%zu "
                    "mark=%d\n", task & 017, tioa & 0377,
                    eth->rx_pos, eth->rx_count,
                    (eth->rx_pos < eth->rx_count && eth->rx_attention)
                        ? eth->rx_attention[eth->rx_pos] : -1);
        }
    }
    if (task == DORADO_ETHERNET_TASK_EIT &&
        tioa == DORADO_ETHERNET_TIOA_DATA) {
        return !eth->rx_hold &&
               eth->rx_pos < eth->rx_count &&
               eth->rx_attention[eth->rx_pos];
    }
    return 0;
}

void dorado_ethernet_attach_to_io(dorado_ethernet *eth, dorado_io *io)
{
    static dorado_io_device dev;
    dev.read = eth_read;
    dev.write = eth_write;
    dev.attention = eth_attention;
    dev.ctx = eth;
    dev.name = "ethernet";

    dorado_io_register(io, DORADO_ETHERNET_TASK_EOT,
                       DORADO_ETHERNET_TIOA_DATA, &dev);
    dorado_io_register(io, DORADO_ETHERNET_TASK_EOT,
                       DORADO_ETHERNET_TIOA_CTL, &dev);
    dorado_io_register(io, DORADO_ETHERNET_TASK_EIT,
                       DORADO_ETHERNET_TIOA_DATA, &dev);
    dorado_io_register(io, DORADO_ETHERNET_TASK_EIT,
                       DORADO_ETHERNET_TIOA_CTL, &dev);
    dorado_io_register(io, 0, DORADO_ETHERNET_TIOA_CTL, &dev);
}

uint16_t dorado_ethernet_wakeup_mask(dorado_ethernet *eth)
{
    if (eth->rx_hold) eth->rx_hold--;
    /* EFTP sender retransmission (EFTPSPEC): once the receiver has
     * consumed (or lost) the packet on the wire without Acking it,
     * count down and put the same packet back. The timer only runs
     * while the rx queue is drained — an undelivered copy is still
     * "on the wire". */
    if (eth->eftp_state != 0 && eth->eftp_words &&
        !eth->rx_hold && eth->rx_pos >= eth->rx_count) {
        if (eth->eftp_resend_timer > 0 && --eth->eftp_resend_timer == 0) {
            (void)eth_eftp_deliver_current(eth);
        }
    }
    /* HM §11: "EOT wakeups occur when the bus register is empty, TxOn
     * is true, and TxEOP, TxCntDwn, and NoWakeups are false." The fake
     * consumes Output<-B immediately, so the bus register is always
     * empty. "EIT wakeup requests occur when the bus register contains
     * an interesting word" — RxOn and RxBOP true; RxBOP is modeled as
     * implied by an undelivered word in the prequeued stream. */
    uint16_t mask = 0;
    if (eth->no_wakeups) return 0;
    if (eth->tx_on && !eth->tx_eop && !eth->tx_cntdwn) {
        mask |= (uint16_t)(1u << DORADO_ETHERNET_TASK_EOT);
    }
    if (eth->rx_on && !eth->rx_hold && eth->rx_pos < eth->rx_count) {
        mask |= (uint16_t)(1u << DORADO_ETHERNET_TASK_EIT);
    }
    return mask;
}
