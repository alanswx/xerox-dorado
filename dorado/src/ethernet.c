#include "ethernet.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Fill the 5-word Alto NTime body (32-bit time, zone word, begin/end DST)
 * from the host clock so NetExec's banner shows the real wall-clock time.
 * NetExec's WriteDate only accepts years 1983..2000 ("Date and time
 * unknown" otherwise); the Gregorian calendar repeats every 28 years, so
 * shifting the year back by 28 keeps the (undisplayed) year in range while
 * the weekday/month/day/time stay correct. timegm() re-encodes the local
 * broken-down time as seconds, and with zone=0 NetExec displays it
 * verbatim. 2177452800 = seconds from the Alto epoch (1901-01-01) to the
 * Unix epoch (1970-01-01). DORADO_ALTO_TIME (octal hi,lo) forces a fixed
 * value for deterministic runs. */
static void eth_fill_alto_time(uint16_t body[5])
{
    const char *fixed = getenv("DORADO_ALTO_TIME");
    uint32_t at;
    if (fixed) {
        unsigned hi = 0, lo = 0;
        sscanf(fixed, "%o,%o", &hi, &lo);
        at = ((uint32_t)hi << 16) | (uint32_t)(lo & 0xFFFF);
        body[0] = (uint16_t)(at >> 16);
        body[1] = (uint16_t)(at & 0xFFFF);
        body[2] = 0;
        body[3] = 0;
        body[4] = 0;
        return;
    }
    /* The Alto time protocol carries GMT plus a timezone word (hours/
     * minutes west of GMT) and a DST day-of-year window; NetExec displays
     * GMT - zone, adding one hour when the date is inside the window. So:
     * send true UTC, the fixed DST window (121..305), and a zone chosen so
     * the banner shows THIS host's local wall-clock time:
     *   display = UTC - zone + dstNow  ==>  zone = dstNow - offsetHours
     * where offsetHours is the host's current GMT offset (negative west)
     * and dstNow is 1 when today is in the window (what NetExec will add).
     * This compensates correctly whether or not the host observes DST.
     * Year shifted back 28 (a Gregorian repeat) to stay in NetExec's
     * 1983..2000 window; the year is not displayed. 2177452800 = seconds
     * from the Alto epoch (1901) to the Unix epoch (1970). */
    time_t now = time(NULL);
    struct tm lt = *localtime(&now);
    int yday1 = lt.tm_yday + 1;
    int64_t off_hours = ((int64_t)timegm(&lt) - (int64_t)now) / 3600;
    int dst_now = (yday1 >= 121 && yday1 <= 305) ? 1 : 0;
    int zone_h = (int)(dst_now - off_hours);
    int sign = zone_h < 0 ? 1 : 0;
    int zh = zone_h < 0 ? -zone_h : zone_h;
    struct tm ut = *gmtime(&now);
    ut.tm_year -= 28;
    at = (uint32_t)((int64_t)timegm(&ut) + 2177452800LL);
    body[0] = (uint16_t)(at >> 16);
    body[1] = (uint16_t)(at & 0xFFFF);
    body[2] = (uint16_t)((sign << 15) | ((zh & 0x7F) << 8));
    body[3] = 121;
    body[4] = 305;
}

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
    /* Debug: a reply the Alto has not finished reading (rx_pos < rx_count)
     * being dropped here means the single shared rx buffer clobbered a
     * pending socket reply before NetExec consumed it. */
    if (getenv("DORADO_BOOTDIR_DEBUG") && eth->rx_words &&
        eth->rx_pos < eth->rx_count) {
        uint16_t ptype = eth->rx_count > 3 ? eth->rx_words[3] : 0;
        fprintf(stderr,
                "[bootdir] CLOBBER: dropping unread rx (pup type 0o%o, "
                "read %zu/%zu words) before the Alto consumed it\n",
                ptype, eth->rx_pos, eth->rx_count);
    }
    free(eth->rx_words);
    free(eth->rx_attention);
    eth->rx_words = NULL;
    eth->rx_attention = NULL;
    eth->rx_count = 0;
    eth->rx_pos = 0;
    eth->rx_hold = 0;
    eth->rx_wire_timer = 0;
}

void dorado_ethernet_init(dorado_ethernet *eth)
{
    memset(eth, 0, sizeof *eth);
    eth->local_host = 042;
    eth->remote_host = 01;
    eth->world_rx_words = 0xFFFFu;  /* no rx-size gate until the world posts EICLOC */
    eth->eftp_turnaround_ticks = 60000;
    eth->eftp_dest_socket_lo = DORADO_EFTP_RECEIVER_SOCKET;
    dorado_ethernet_set_boot_file(
        eth, 0110, "../chm/microcode/AltoMesaDorado.eb!1");
    dorado_ethernet_set_boot_file(
        eth, 0111, "../chm/microcode/SmalltalkDorado.eb!1");
    dorado_ethernet_set_boot_file(
        eth, 0112, "../chm/lisp/DORADOLISPMC.EB!1");
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
    case 0112: dst = eth->boot_112_path; break;
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

void dorado_ethernet_add_boot_dir(dorado_ethernet *eth, uint16_t bfn,
                                  const char *name, const char *path)
{
    if (!eth || !name || !path) return;
    if (eth->bootdir_count >= (int)(sizeof eth->bootdir / sizeof eth->bootdir[0]))
        return;
    int i = eth->bootdir_count++;
    eth->bootdir[i].bfn = bfn;
    snprintf(eth->bootdir[i].name, sizeof eth->bootdir[i].name, "%s", name);
    snprintf(eth->bootdir[i].path, sizeof eth->bootdir[i].path, "%s", path);
    /* A fixed, nonzero version date (Alto epoch seconds, ~mid-1984). Must
     * be nonzero: NetExec's LoadKT marks an entry "local" (a built-in
     * command, not a bootable file) iff port==0 AND date==0. Matches the
     * date our time-reply hands NetExec, so neither looks newer. */
    eth->bootdir[i].date_hi = 0x9C8Eu;
    eth->bootdir[i].date_lo = 0x0000u;
}

/* True iff `bfn` is already used by a directory entry or is one of the
 * microcode boot offsets (0o110-0o114) Initial requests over Ethernet. */
static int eth_bfn_in_use(const dorado_ethernet *eth, uint16_t bfn)
{
    if (bfn >= 0110 && bfn <= 0114) return 1;
    for (int i = 0; i < eth->bootdir_count; i++)
        if (eth->bootdir[i].bfn == bfn) return 1;
    return 0;
}

/* True iff a directory entry already advertises this name (case-sensitive,
 * matching NetExec's keyword table). Lets explicit --boot-dir entries win. */
static int eth_bootdir_has_name(const dorado_ethernet *eth, const char *name)
{
    for (int i = 0; i < eth->bootdir_count; i++)
        if (strcmp(eth->bootdir[i].name, name) == 0) return 1;
    return 0;
}

/* Read the first 16-bit big-endian word of `path` (the boot-file format
 * tag: 0o405 = Alto B-format, 0o345 = Mesa outload). Returns -1 on error. */
static long eth_boot_first_word(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;
    int hi = fgetc(fp), lo = fgetc(fp);
    fclose(fp);
    if (hi == EOF || lo == EOF) return -1;
    return (long)(((unsigned)hi << 8) | (unsigned)lo);
}

/* Derive the advertised boot-file name from a filename: drop any trailing
 * IFS "!<version>" suffix and normalize the extension to lower-case ".boot"
 * (NetExec's InstallDir filters on a case-sensitive ".boot" suffix, and
 * NetBoot appends ".boot" to what the user types). Returns 0 on success. */
static int eth_derive_boot_name(const char *fname, char *out, size_t outcap)
{
    char base[64];
    snprintf(base, sizeof base, "%s", fname);
    char *bang = strchr(base, '!');           /* strip "!<version>" */
    if (bang) *bang = '\0';
    size_t n = strlen(base);
    /* Require a ".boot" extension (any case); re-emit it lower-case. */
    if (n < 5) return -1;
    const char *ext = base + (n - 5);
    if (strcasecmp(ext, ".boot") != 0) return -1;
    base[n - 5] = '\0';                        /* trim extension */
    if (base[0] == '\0') return -1;
    snprintf(out, outcap, "%s.boot", base);
    return 0;
}

/* qsort comparator: alphabetical by filename, so the advertised menu and
 * auto-assigned bfns are deterministic regardless of readdir() order. */
static int eth_name_cmp(const void *a, const void *b)
{
    /* a, b point at char[64] elements -- compare them directly. */
    return strcmp((const char *)a, (const char *)b);
}

int dorado_ethernet_add_boot_dir_all(dorado_ethernet *eth, const char *dir)
{
    if (!eth || !dir) return 0;
    DIR *d = opendir(dir);
    if (!d) {
        if (getenv("DORADO_BOOTDIR_DEBUG"))
            fprintf(stderr, "[bootdir] add_boot_dir_all: cannot open %s\n", dir);
        return 0;
    }

    /* Collect the *.boot filenames first, then sort, for a stable menu. */
    char (*names)[64] = NULL;
    int n = 0, cap = 0;
    struct dirent *de;
    while ((de = readdir(d)) != NULL) {
        char tmp[64];
        if (eth_derive_boot_name(de->d_name, tmp, sizeof tmp) != 0)
            continue;
        if (n >= cap) {
            int nc = cap ? cap * 2 : 16;
            char (*nn)[64] = realloc(names, (size_t)nc * sizeof names[0]);
            if (!nn) break;
            names = nn; cap = nc;
        }
        snprintf(names[n++], sizeof names[0], "%s", de->d_name);
    }
    closedir(d);
    if (n > 1) qsort(names, (size_t)n, sizeof names[0], eth_name_cmp);

    uint16_t next_bfn = 0200;   /* clear of 0o110-0o114 and the 0o111 the */
                                /* CedarNetExec example uses              */
    int added = 0;
    for (int i = 0; i < n; i++) {
        char advname[48];
        if (eth_derive_boot_name(names[i], advname, sizeof advname) != 0)
            continue;
        if (eth_bootdir_has_name(eth, advname))
            continue;                       /* explicit --boot-dir wins */

        char path[256];
        snprintf(path, sizeof path, "%s/%s", dir, names[i]);
        long w0 = eth_boot_first_word(path);
        if (w0 != 0405) {                   /* skip Mesa (0o345) and junk */
            if (getenv("DORADO_BOOTDIR_DEBUG") && w0 >= 0)
                fprintf(stderr, "[bootdir] skip %s (word0=0o%lo, not "
                        "Alto B-format)\n", names[i], (unsigned long)w0);
            continue;
        }
        while (eth_bfn_in_use(eth, next_bfn)) next_bfn++;
        int before = eth->bootdir_count;
        dorado_ethernet_add_boot_dir(eth, next_bfn, advname, path);
        if (eth->bootdir_count == before) break;   /* table full */
        if (getenv("DORADO_BOOTDIR_DEBUG"))
            fprintf(stderr, "[bootdir] register %s as %s bfn=0o%o\n",
                    names[i], advname, next_bfn);
        next_bfn++;
        added++;
    }
    free(names);
    return added;
}

/* Map a Mayday boot file number to a directory file, if registered. */
static const char *eth_bootdir_path(const dorado_ethernet *eth, uint16_t bfn)
{
    for (int i = 0; i < eth->bootdir_count; i++)
        if (eth->bootdir[i].bfn == bfn)
            return eth->bootdir[i].path;
    return NULL;
}

static const char *eth_boot_path(const dorado_ethernet *eth, uint16_t offset)
{
    switch (offset) {
    case 0110: return eth->boot_110_path;
    case 0111: return eth->boot_111_path;
    case 0112: return eth->boot_112_path;
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

/* Pup ones-complement checksum (HM/PupSpec; algorithm cross-checked
 * against the Living Computer Museum IFS PUP.CalculateChecksum). Sum the
 * Pup words [start, start+count) -- i.e. from the length word through the
 * last content word, excluding the trailing checksum word -- with
 * end-around carry and a left rotate after each add. 0xFFFF (negative
 * zero) is normalized to 0. NetExec's BCPL Pup receive validates this;
 * the 0xFFFF "no checksum" sentinel we used before was dropped. */
static uint16_t pup_checksum(const uint16_t *w, size_t start, size_t count)
{
    uint32_t sum = 0;
    for (size_t i = start; i < start + count; i++) {
        sum += w[i];
        sum = (uint16_t)(sum + ((sum & 0x10000u) >> 16));   /* end-around */
        sum = sum << 1;
        sum = (uint16_t)(sum + ((sum & 0x10000u) >> 16));   /* rotate left */
    }
    if (sum == 0xFFFFu) sum = 0;
    return (uint16_t)sum;
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
        (uint16_t)(eth->eftp_wait_for_rx_arm ?
                   lhost : ((lhost << 8) | rhost)),
                                            /* etherAdr / EthernetOne dest */
        DORADO_PUP_TYPE_ETHERNET,          /* etherType = typePup      */
        length,                            /* pupLength                */
        pup_type,                          /* EFTP Data / End          */
        00,                                /* pupID high               */
        seq,                               /* pupID low = EFTP seq #   */
        lhost,                             /* pupDNetHost = Alto, net 0 */
        eth->eftp_dest_socket_hi,          /* pupDSocket from Mayday   */
        eth->eftp_dest_socket_lo,
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
    /* A Mayday whose boot file number matches a directory entry (the user
     * picked it by name in NetExec) serves that file; otherwise serve the
     * default Stage-2 boot file (the breath-of-life Mayday carries bfn 0). */
    const char *path = eth_bootdir_path(eth, bfn);
    if (!path) path = eth->eftp_boot_path;
    if (!path || !path[0]) return 0;
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;

    size_t payload_cap = 16384;
    uint16_t *payload = malloc(payload_cap * sizeof payload[0]);
    if (!payload) { fclose(fp); return 0; }
    size_t payload_n = 0;
    if (eth->eftp_wait_for_rx_arm) {
        /* BootChannelEther.StartRecving always discards the first EFTP
         * packet as the Alto boot-loader page, then returns EFTPGetClump
         * to BootSwapGerm.DoInLoad. Route B therefore needs a disposable
         * 512-byte leader so seq 1 is the real boot-file header. */
        for (size_t i = 0; i < DORADO_EFTP_DATA_WORDS; i++)
            payload[payload_n++] = 0;
    }
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

    /* Format-driven encapsulation, keyed on the served image's first word
     * (ETHERBOOT.BRAVO, "Pointers to other Documentation"):
     *
     *  - Alto B-format (word 0 = 0o405 = "JMP .+5") is already a complete
     *    disk-boot image: word 0 jumps over a 256-word leader page (the
     *    creation-date words) to the loadable body. It is served VERBATIM
     *    -- its own leader is already present. (NETEXEC, Galaxian, the Alto
     *    games and tools.)
     *
     *  - Mesa/Pilot outload (word 0 = 0o345 = "JMP 0o345", the Mesa
     *    relocation entry) is served VERBATIM too. It is the raw outload
     *    the Mesa boot loader expects; the Mesa world (AltoMesaDorado)
     *    loads it directly. Earlier bring-up prepended a synthetic 0o405
     *    leader page to make the *Alto* B-format loader swallow a Mesa
     *    image, but that mis-layers the two loaders: a Mesa outload must
     *    reach the Mesa loader as-is, not be wrapped as a B-format file.
     *
     * So no synthetic leader is prepended for either format; the file is
     * streamed exactly as stored. Both 0o405 (Alto) and 0o345 (Mesa) are
     * served byte-for-byte. */

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
    if (eth->eftp_wait_for_rx_arm && !eth->eftp_rx_armed) {
        eth->eftp_delivery_deferred = 1;
        eth->eftp_resend_timer = 0;
        if (getenv("DORADO_EFTP_TRACE")) {
            fprintf(stderr,
                    "EFTP_DEFER state=%u seq=%u pos=%zu/%zu armed=%u\n",
                    eth->eftp_state, eth->eftp_seq,
                    eth->eftp_pos, eth->eftp_len, eth->eftp_rx_armed);
        }
        return 1;
    }
    size_t cap = 0;
    int ok;
    size_t n = 0;
    uint16_t pup_type = DORADO_PUP_TYPE_EFTP_END;
    if (eth->eftp_state == 1) {
        n = eth->eftp_len - eth->eftp_pos;
        if (n > DORADO_EFTP_DATA_WORDS) n = DORADO_EFTP_DATA_WORDS;
        pup_type = DORADO_PUP_TYPE_EFTP_DATA;
        ok = append_eftp_packet(eth, &cap, eth->eftp_seq,
                                pup_type,
                                &eth->eftp_words[eth->eftp_pos], n);
    } else {
        /* End (state 2) and the dally End (state 3): no data. */
        ok = append_eftp_packet(eth, &cap, eth->eftp_seq,
                                pup_type, NULL, 0);
    }
    if (!ok) { eth_clear_rx(eth); return 0; }
    if (getenv("DORADO_EFTP_TRACE")) {
        fprintf(stderr,
                "EFTP_DELIVER type=0o%o state=%u seq=%u data_words=%zu "
                "packet_words=%zu pos=%zu/%zu armed=%u\n",
                pup_type, eth->eftp_state, eth->eftp_seq, n,
                eth->rx_count, eth->eftp_pos, eth->eftp_len,
                eth->eftp_rx_armed);
    }
    eth->rx_pos = 0;
    /* Server turnaround: hold EIT wakeups (in wakeup-poll ticks,
     * ~1/BB-cycle) so a stale receiver-enable window cannot steal
     * the packet before the loader re-arms its control block. The
     * post-Mayday window (EOT wrap-up + DoEtherOutput return + boot3
     * arming) spans tens of thousands of cycles; a real boot server's
     * turnaround is milliseconds. 60000 ticks ~ 2 ms. */
    eth->rx_hold = eth->eftp_turnaround_ticks;
    eth->eftp_delivery_deferred = 0;
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

void dorado_ethernet_set_eftp_rx_armed(dorado_ethernet *eth, int armed)
{
    int was_armed = eth->eftp_rx_armed != 0;
    eth->eftp_rx_armed = armed ? 1 : 0;
    if (eth->eftp_wait_for_rx_arm && eth->eftp_rx_armed && !was_armed &&
        eth->eftp_delivery_deferred && eth->eftp_state != 0 && eth->eftp_words) {
        (void)eth_eftp_deliver_current(eth);
    }
}

/* Lock-step advance on the receiver's EFTP Ack (EFTPSPEC): the Ack
 * for the packet on the wire releases the next Data packet, then the
 * End, then one more End to close the receiver's dally. A duplicate
 * Ack (older seq) retransmits the current packet. */
static void eth_eftp_ack(dorado_ethernet *eth, uint16_t acked_seq)
{
    if (eth->eftp_state == 0 || !eth->eftp_words) return;
    if (getenv("DORADO_EFTP_TRACE")) {
        fprintf(stderr,
                "EFTP_ACK_IN ack=%u state=%u seq=%u pos=%zu/%zu\n",
                acked_seq, eth->eftp_state, eth->eftp_seq,
                eth->eftp_pos, eth->eftp_len);
    }
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
    if (getenv("DORADO_EFTP_TRACE")) {
        fprintf(stderr,
                "EFTP_ACK_ADV state=%u seq=%u pos=%zu/%zu\n",
                eth->eftp_state, eth->eftp_seq,
                eth->eftp_pos, eth->eftp_len);
    }
    if (eth->eftp_wait_for_rx_arm) eth->eftp_rx_armed = 0;
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

/* Per-word wire time (transmit and receive), in wakeup-poll ticks. The
 * 3 Mb/s Alto wire moves ~one 16-bit word per 5.4 us; ContrAlto schedules its
 * receiver one word per 5400 nsec and its transmitter ~87 us / 16 words. The
 * rx WaitForBOP drain already uses 170 ticks/word for the same "~5.4 us per
 * word" rate (see rx_hold), so use the same constant for both the faithful tx
 * completion and the faithful rx pacing. Active only with DORADO_ETH_WIRE. */
#define DORADO_ETH_WIRE_TICKS_PER_WORD 170u

/* Faithful transmit wire model toggle (DORADO_ETH_WIRE). Default OFF so the
 * EFTP boot -- which relies on instant tx-completion -- is unaffected. */
static int eth_wire_model(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_ETH_WIRE") ? 1 : 0;
    return cached;
}

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
        for (size_t i = 0; i < eth->tx_count && i < 32; i++)
            fprintf(stderr, " %06o", eth->tx_words[i]);
        fprintf(stderr, "\n");
    }
    /* One line per Pup the Alto transmits: which socket protocol it is and
     * (for a Mayday) the boot file number requested. Lets the log show the
     * full request sequence NetExec issues. */
    if (getenv("DORADO_BOOTDIR_DEBUG") && eth->tx_count >= 6 &&
        eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET) {
        uint16_t t = eth->tx_words[3];
        const char *nm =
            t == DORADO_PUP_TYPE_BOOTDIR_REQ   ? "BootDirReq(257)"   :
            t == DORADO_PUP_TYPE_BOOTDIR_REPLY ? "BootDirReply(260)" :
            t == DORADO_PUP_TYPE_MAYDAY        ? "Mayday(244)"       :
            t == DORADO_PUP_TYPE_EFTP_ACK      ? "EFTP-Ack(031)"     :
            t == DORADO_PUP_TYPE_MICROCODE_BOOT_REQUEST ? "uCodeBoot(264)" :
            "other";
        fprintf(stderr, "[bootdir] TX %s type=0o%o", nm, t);
        if (t == DORADO_PUP_TYPE_MAYDAY)
            fprintf(stderr, " bfn=0o%o -> serve %s", eth->tx_words[5],
                    eth_bootdir_path(eth, eth->tx_words[5]) ? "directory file"
                                                            : "default --eftp");
        fprintf(stderr, "\n");
    }

    /* Stage-2: a Mayday Pup is the Alto software-boot request. Serve the
     * configured Alto boot file as a LOCK-STEP EFTP stream: packet 0
     * now, each next packet on the previous one's Ack. */
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        eth->tx_words[3] == DORADO_PUP_TYPE_MAYDAY) {
        eth->eftp_requests_seen++;
        eth->eftp_last_bfn = eth->tx_words[5];
        eth->eftp_dest_socket_hi = eth->tx_words[10];
        eth->eftp_dest_socket_lo = eth->tx_words[11];
        if (eth->eftp_dest_socket_hi == 0 && eth->eftp_dest_socket_lo == 0)
            eth->eftp_dest_socket_lo = DORADO_EFTP_RECEIVER_SOCKET;
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
    /* GatewayInformationRequest (200B) and AltoTimeRequest (206B): the
     * two socket requests NetExec broadcasts after boot. Answer each with
     * the documented reply. The reply's DESTINATION NET is our network
     * number -- this is how NetExec's PupLevel1/ProcessRouteInfoReply
     * learn the directly-connected net (PupRoute.bcpl). Both replies share
     * the same framing; only the type and body differ. Cross-checked
     * against IFS GatewayInformationProtocol / MiscServicesProtocol. */
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        (eth->tx_words[3] == DORADO_PUP_TYPE_GATEWAY_REQ ||
         eth->tx_words[3] == DORADO_PUP_TYPE_ALTOTIME_REQ)) {
        int is_gw = (eth->tx_words[3] == DORADO_PUP_TYPE_GATEWAY_REQ);
        uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
        uint16_t net = DORADO_PUP_LOCAL_NET;
        eth_clear_rx(eth);
        size_t cap = 0;

        /* Gateway reply: one 4-byte routing block {targetNet, gatewayNet,
         * gatewayHost, hopCount} -- net `net` is directly connected via
         * the server (host remote_host), 0 hops. Time reply: the Alto
         * NTime body (32-bit time, zone word, begin/end DST). */
        uint16_t gw_body[2] = {
            (uint16_t)((net << 8) | net),
            (uint16_t)((eth->remote_host << 8) | 0)
        };
        uint16_t time_body[5];
        eth_fill_alto_time(time_body);
        const uint16_t *body = is_gw ? gw_body : time_body;
        int blen = is_gw ? 2 : 5;
        uint16_t rtype = is_gw ? DORADO_PUP_TYPE_GATEWAY_REPLY
                               : DORADO_PUP_TYPE_ALTOTIME_REPLY;

        uint16_t hdr[12];
        hdr[0]  = (uint16_t)((req_src_host << 8) | eth->remote_host);
        hdr[1]  = DORADO_PUP_TYPE_ETHERNET;
        hdr[2]  = (uint16_t)(026 + 2 * blen);
        hdr[3]  = rtype;
        hdr[4]  = eth->tx_words[4];                       /* id matches req */
        hdr[5]  = eth->tx_words[5];
        hdr[6]  = (uint16_t)((net << 8) | req_src_host);  /* dPort net.host:
                                                           * net teaches
                                                           * NetExec its net */
        hdr[7]  = eth->tx_words[10];                       /* dPort socket = */
        hdr[8]  = eth->tx_words[11];                       /*   requestor sPort */
        hdr[9]  = (uint16_t)((net << 8) | eth->remote_host); /* sPort = server */
        hdr[10] = eth->tx_words[7];                        /* sPort socket = */
        hdr[11] = eth->tx_words[8];                        /*   queried socket */
        int ok = 1;
        for (int i = 0; i < 12 && ok; i++)
            ok = append_rx_word(eth, &cap, hdr[i], 0);
        for (int i = 0; i < blen && ok; i++)
            ok = append_rx_word(eth, &cap, body[i], 0);
        if (ok) {
            uint16_t cks = pup_checksum(eth->rx_words, 2,
                                        (size_t)(12 + blen) - 2);
            ok = append_rx_word(eth, &cap, cks, 0);
        }
        /* Hardware CRC trailer word (HM 11): the EIT receive loop
         * (AltoEther.mc EIEnd) unconditionally drops ONE trailing word
         * as the CRC before computing the ending count, so every packet
         * must carry it. Without it EELoc is one too large, packetLength
         * (=lenEtherPacket-EELoc) is one too small, and EtherPupFilter's
         * length-equality check rejects the packet (sent to pbiFreeQ,
         * never demuxed to the socket). Matches append_reply/eftp. */
        if (ok) ok = append_rx_word(eth, &cap, 0, 0);
        if (ok) ok = append_rx_word(eth, &cap, 0, 1);
        if (ok) { eth->rx_pos = 0; eth->time_bcasts++; }
        if (getenv("DORADO_BOOTDIR_DEBUG"))
            fprintf(stderr, "[bootdir] %s req -> %s reply, dPort net=0o%o "
                    "host=0o%o (teaches NetExec net 0o%o)\n",
                    is_gw ? "200b GatewayInfo" : "206b AltoTime",
                    is_gw ? "201b" : "207b", net, req_src_host, net);
        return;
    }

    /* AddressLookup (223B), socket 4 Misc Services. NetExec's GetName
     * sends the local net/host in the body and InstallName expects an
     * AddressIs (224B) whose entire data section is the host name bytes
     * (no leading string length). Cedar/Pilot reaches the same service
     * during post-boot Pup naming. */
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        eth->tx_words[3] == DORADO_PUP_TYPE_ADDRESS_LOOKUP) {
        static const uint8_t name[] = { 'D', 'o', 'r', 'a', 'd', 'o' };
        uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
        uint16_t net = DORADO_PUP_LOCAL_NET;
        eth_clear_rx(eth);
        size_t cap = 0;

        uint16_t hdr[12];
        hdr[0]  = (uint16_t)((req_src_host << 8) | eth->remote_host);
        hdr[1]  = DORADO_PUP_TYPE_ETHERNET;
        hdr[2]  = (uint16_t)(026 + sizeof name);
        hdr[3]  = DORADO_PUP_TYPE_ADDRESS_REPLY;
        hdr[4]  = eth->tx_words[4];
        hdr[5]  = eth->tx_words[5];
        hdr[6]  = (uint16_t)((net << 8) | req_src_host);
        hdr[7]  = eth->tx_words[10];
        hdr[8]  = eth->tx_words[11];
        hdr[9]  = (uint16_t)((net << 8) | eth->remote_host);
        hdr[10] = eth->tx_words[7];
        hdr[11] = eth->tx_words[8];
        int ok = 1;
        for (int i = 0; i < 12 && ok; i++)
            ok = append_rx_word(eth, &cap, hdr[i], 0);
        for (size_t i = 0; i < sizeof name && ok; i += 2) {
            uint16_t w = (uint16_t)(name[i] << 8);
            if (i + 1 < sizeof name) w |= name[i + 1];
            ok = append_rx_word(eth, &cap, w, 0);
        }
        if (ok) {
            size_t body_words = (sizeof name + 1u) / 2u;
            uint16_t cks = pup_checksum(eth->rx_words, 2,
                                        (size_t)(12 + body_words) - 2);
            ok = append_rx_word(eth, &cap, cks, 0);
        }
        if (ok) ok = append_rx_word(eth, &cap, 0, 0);
        if (ok) ok = append_rx_word(eth, &cap, 0, 1);
        if (ok) { eth->rx_pos = 0; eth->time_bcasts++; }
        if (getenv("DORADO_ETH_TX_TRACE"))
            fprintf(stderr, "ADDRESSLOOKUP reply: host=0o%o name=Dorado\n",
                    req_src_host);
        return;
    }

    /* BootDirReq (257B): NetExec asking which boot files we offer. Reply
     * 260B with one {bfn, date(2), name} block per directory entry so the
     * user can boot any of them by name. Pup data starts at the first
     * data word (`^` is 1-indexed in NetExec1.bcpl's InstallDir, so
     * words^1 = data word 0), exactly like the time reply above. */
    if (eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET &&
        eth->tx_words[3] == DORADO_PUP_TYPE_BOOTDIR_REQ &&
        eth->bootdir_count > 0) {
        uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
        eth_clear_rx(eth);
        size_t cap = 0;

        /* Serialize the BFD blocks first, to size pup.length. The whole
         * directory may not fit in one Pup: NetExec posts a ~279-word input
         * buffer (lenPBI; Pup1Init.bcpl), and a packet larger than the
         * posted buffer is refused (Input Buffer Overrun). So emit at most
         * BATCH_DATA_WORDS data words per reply, starting at bootdir_cursor,
         * and advance the cursor. NetExec retransmits the BootDirReq several
         * times per GetDir and its InstallDir accumulates entries across the
         * replies (NetExec1.bcpl), so every entry is delivered over the run
         * of requests even though no single reply lists them all. */
        enum { BATCH_DATA_WORDS = 230 };
        uint16_t data[256];
        int nd = 0;
        int emitted = 0;
        if (eth->bootdir_cursor >= (uint16_t)eth->bootdir_count)
            eth->bootdir_cursor = 0;
        int start = (int)eth->bootdir_cursor;
        for (int s = 0; s < eth->bootdir_count; s++) {
            int e = (start + s) % eth->bootdir_count;
            const char *nm = eth->bootdir[e].name;
            int len = (int)strlen(nm);
            /* bfn + date(2) + ceil((len+1)/2) name words. */
            int name_words = (len + 1 + 1) / 2;
            int blk = 3 + name_words;
            if (blk > (int)(sizeof data / sizeof data[0]))
                break;                          /* pathological single entry */
            /* Stop once the batch is full, but always emit at least one
             * entry so the cursor makes forward progress. */
            if (emitted > 0 && nd + blk > BATCH_DATA_WORDS)
                break;
            data[nd++] = eth->bootdir[e].bfn;
            data[nd++] = eth->bootdir[e].date_hi;
            data[nd++] = eth->bootdir[e].date_lo;
            /* BCPL string: byte0 = length, then chars, packed two per
             * word big-endian, zero-padded to an even byte count. */
            uint8_t sb[64];
            int sn = 0;
            sb[sn++] = (uint8_t)len;
            for (int k = 0; k < len; k++) sb[sn++] = (uint8_t)nm[k];
            if (sn & 1) sb[sn++] = 0;
            for (int k = 0; k < sn; k += 2)
                data[nd++] = (uint16_t)((sb[k] << 8) | sb[k + 1]);
            emitted++;
        }
        eth->bootdir_cursor =
            (uint16_t)((start + emitted) % eth->bootdir_count);

        uint16_t hdr[12];
        hdr[0]  = (uint16_t)((req_src_host << 8) | eth->remote_host);
        hdr[1]  = DORADO_PUP_TYPE_ETHERNET;
        hdr[2]  = (uint16_t)(026 + 2 * nd);   /* pup.length, bytes */
        hdr[3]  = DORADO_PUP_TYPE_BOOTDIR_REPLY;
        hdr[4]  = eth->tx_words[4];           /* id matches the request */
        hdr[5]  = eth->tx_words[5];
        hdr[6]  = req_src_host;               /* dPort net.host (dnet 0) */
        hdr[7]  = eth->tx_words[10];          /* dPort socket = requestor */
        hdr[8]  = eth->tx_words[11];          /*   sPort socket           */
        hdr[9]  = (uint16_t)((1 << 8) | eth->remote_host); /* sPort = us */
        hdr[10] = eth->tx_words[7];           /* sPort socket = the misc- */
        hdr[11] = eth->tx_words[8];           /*   services socket queried */
        int ok = 1;
        for (int i = 0; i < 12 && ok; i++)
            ok = append_rx_word(eth, &cap, hdr[i], 0);
        for (int i = 0; i < nd && ok; i++)
            ok = append_rx_word(eth, &cap, data[i], 0);
        /* Pup checksum over the Pup proper (word[2]=length through the
         * last content word), excluding the 2 Ethernet-framing words and
         * the checksum word itself. rx_count is now 12+nd, so the Pup
         * spans indices [2, 12+nd). */
        if (ok) {
            uint16_t cks = pup_checksum(eth->rx_words, 2,
                                        (size_t)(12 + nd) - 2);
            ok = append_rx_word(eth, &cap, cks, 0);
        }
        /* Hardware CRC trailer word (see the gateway/time reply above):
         * the EIT receive loop drops one trailing CRC word, so without it
         * packetLength is one short and EtherPupFilter rejects the reply. */
        if (ok) ok = append_rx_word(eth, &cap, 0, 0);
        if (ok) ok = append_rx_word(eth, &cap, 0, 1);
        if (ok) { eth->rx_pos = 0; eth->bootdir_replies++; }
        if (getenv("DORADO_ETH_TX_TRACE"))
            fprintf(stderr, "BOOTDIR reply: %d of %d file(s), %d data words\n",
                    emitted, eth->bootdir_count, nd);
        if (getenv("DORADO_BOOTDIR_DEBUG")) {
            fprintf(stderr,
                    "[bootdir] 257b BootDirReq #%llu from host=0o%o "
                    "sPort=0o%o/0o%o/0o%o; rx_on=%d ok=%d. Reply 260b: "
                    "id=0o%o/0o%o dPort=0o%o/0o%o/0o%o len=0o%o(%d data w) "
                    "%d of %d entries:",
                    (unsigned long long)eth->bootdir_replies,
                    (unsigned)(eth->tx_words[0] & 0377),
                    eth->tx_words[9], eth->tx_words[10], eth->tx_words[11],
                    eth->rx_on, ok,
                    hdr[4], hdr[5], hdr[6], hdr[7], hdr[8], hdr[2], nd,
                    emitted, eth->bootdir_count);
            for (int s = 0; s < emitted; s++) {
                int e = (start + s) % eth->bootdir_count;
                fprintf(stderr, " %s=0o%o", eth->bootdir[e].name,
                        eth->bootdir[e].bfn);
            }
            fprintf(stderr, "\n");
        }
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

void dorado_ethernet_direct_transmit(dorado_ethernet *eth,
                                     const uint16_t *words, size_t nwords)
{
    if (!eth || !words) return;
    if (nwords > sizeof eth->tx_words / sizeof eth->tx_words[0])
        nwords = sizeof eth->tx_words / sizeof eth->tx_words[0];
    for (size_t i = 0; i < nwords; i++)
        eth->tx_words[i] = words[i];
    eth->tx_count = nwords;
    eth->tx_starts++;
    eth_tx_packet_done(eth);
    eth->tx_eops++;
    eth->tx_count = 0;
    eth->tx_on = 0;
    eth->tx_eop = 0;
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
    if (eth->rx_pos >= eth->rx_count || eth->rx_hold ||
        (eth->rx_pos == 0 && eth->rx_count > eth->world_rx_words + 2u)) {
        /* rx_hold: the packet is still "on the wire" (network / server
         * turnaround) — nothing has reached the bus register.
         * rx_count > world_rx_words + 2: the world's currently-posted input
         * buffer (EICLOC) is too small for this packet's payload, so handing
         * it over would trip Input Buffer Overrun (Alto HW Manual Sec 7).
         * The +2 excludes the two trailer words every rx packet here appends
         * (a dummy hardware CRC, then the attention/status word): the
         * receiver reads those past the end of its posted buffer as the
         * end-of-packet handshake, so they do not count against EICLOC. An
         * EFTP Data packet is exactly EICLOC+2 (hdr+data+pup-cksum = EICLOC,
         * plus CRC+status) and must pass. Hold it until the world re-posts a
         * big-enough buffer; only gate at the head of a packet (rx_pos==0)
         * so an in-progress read finishes. */
        if (bad) *bad = 1;
        return 0xFFFF;
    }
    uint16_t word = eth->rx_words[eth->rx_pos];
    eth->rx_pos++;
    /* Faithful rx wire pacing: hold the next EIT wakeup off for one word's
     * wire time so the world cannot drain the queue faster than 3 Mb/s. The
     * EIT microcode reads one word then Blocks, so this paces delivery at one
     * word per ~5.4 us (see dorado_ethernet_wakeup_mask). Default off. */
    if (eth_wire_model())
        eth->rx_wire_timer = DORADO_ETH_WIRE_TICKS_PER_WORD;
    /* Debug: note when the Alto finishes reading a BootDirReply, i.e. the
     * 260B reply actually reached NetExec's GetDir (vs being clobbered or
     * never delivered). word[3] is the Pup type of the queued packet. */
    if (getenv("DORADO_BOOTDIR_DEBUG") && eth->rx_pos == eth->rx_count &&
        eth->rx_count > 3 && eth->rx_words[3] == DORADO_PUP_TYPE_BOOTDIR_REPLY)
        fprintf(stderr, "[bootdir] 260b reply CONSUMED by the Alto "
                "(%zu words read)\n", eth->rx_count);
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
                /* Drop any in-flight wire-model packet too. */
                eth->tx_pending = 0;
                eth->tx_wire_timer = 0;
            } else {
                eth->tx_eop = (data & DORADO_ETHC_TXEOP) != 0;
                /* TxCntDwn delays wakeups until the next Pendulum tick
                 * (16 us). This packet-level fake has no Pendulum;
                 * treat the tick as already due so EWait loops simply
                 * count down one iteration per wakeup. */
                eth->tx_cntdwn = 0;
                if (eth->tx_eop && eth->tx_count > 0) {
                    if (eth_wire_model()) {
                        /* Faithful path: don't complete now. Hold tx_eop set
                         * (suppresses the EOT "sent" wakeup) and let
                         * dorado_ethernet_wakeup_mask() finish the packet
                         * after carrier-sense deferral + wire time. */
                        eth->tx_pending = 1;
                        eth->tx_wire_timer =
                            (uint32_t)eth->tx_count * DORADO_ETH_WIRE_TICKS_PER_WORD;
                    } else {
                        eth_tx_packet_done(eth);
                        /* TxGone: end of packet clears TxEOP, waking EOT. */
                        eth->tx_eop = 0;
                        eth->tx_eops++;
                        eth->tx_count = 0;
                    }
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
            if (!on && eth->eftp_wait_for_rx_arm) {
                /* HM §11: clearing RxOn resets the receiver; no more
                 * wakeups are generated and queued/current words are
                 * discarded until the receiver is re-armed at a packet
                 * boundary. This packet-level model has no wire FIFO, so
                 * discard the queued packet stream immediately.
                 *
                 * Only for the Cedar/Pilot germ (eftp_wait_for_rx_arm), whose
                 * IOCB-gated delivery re-arms per input buffer. The Alto
                 * EtherBoot loader toggles RxOn off/on between EFTP packets
                 * while the fake server holds the next (already-delivered)
                 * lock-step packet on the wire; discarding it there drops the
                 * packet the Alto is about to read, stalling the boot
                 * mid-stream. The Alto path therefore keeps the held packet
                 * (matching the pre-regression behavior). */
                eth_clear_rx(eth);
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
    static uint64_t wake_trace_count;
    if (eth->rx_hold) eth->rx_hold--;
    if (eth->rx_wire_timer) eth->rx_wire_timer--;
    /* Faithful tx wire model (matches ContrAlto): finish an in-flight TxEOP
     * packet after a FIXED wire time -- ContrAlto schedules its transmit
     * completion ~87 us after EndTransmission regardless of receiver state
     * (it models no carrier sense / collisions). We use tx_count * 170 ticks
     * for the same ~5.4 us/word. Hold tx_eop set meanwhile (suppresses the
     * EOT "sent" wakeup); on expiry complete and clear tx_eop so EOT wakes to
     * post OutDone. (Default off; see eth_wire_model.) */
    if (eth->tx_pending) {
        if (eth->tx_wire_timer > 0) {
            eth->tx_wire_timer--;
        } else {
            eth_tx_packet_done(eth);
            eth->tx_eop = 0;
            eth->tx_eops++;
            eth->tx_count = 0;
            eth->tx_pending = 0;
        }
    }
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
    if (eth->rx_on && !eth->rx_hold && !eth->rx_wire_timer &&
        eth->rx_pos < eth->rx_count &&
        !(eth->rx_pos == 0 && eth->rx_count > eth->world_rx_words + 2u)) {
        /* Don't wake the input task for a packet whose payload is larger
         * than the world's currently-posted input buffer (EICLOC) -- it
         * would overflow and derail the receive. The +2 excludes the two
         * trailer words (CRC + status) the receiver reads past its buffer as
         * the end-of-packet handshake, so a correctly-sized EFTP Data packet
         * (EICLOC+2 words) still passes. Hold until the world re-posts a
         * buffer that fits. See eth_read() and world_rx_words (HM Sec 7). */
        mask |= (uint16_t)(1u << DORADO_ETHERNET_TASK_EIT);
    }
    if (getenv("DORADO_ETH_WAKE_TRACE") &&
        (eth->rx_count || eth->tx_on || eth->no_wakeups) &&
        (wake_trace_count++ % 1024u) == 0) {
        extern int dorado_trace_gate;
        extern unsigned long long dorado_trace_cycle;
        if (!dorado_trace_gate) {
            fprintf(stderr,
                    "ETH_WAKE mask=%04x rx_on=%u hold=%u no_wake=%u "
                    "rx=%zu/%zu world=%u tx_on=%u eop=%u cntdwn=%u\n",
                    mask, eth->rx_on, eth->rx_hold, eth->no_wakeups,
                    eth->rx_pos, eth->rx_count, eth->world_rx_words,
                    eth->tx_on, eth->tx_eop, eth->tx_cntdwn);
        } else {
            fprintf(stderr,
                    "ETH_WAKE cyc=%llu mask=%04x rx_on=%u hold=%u "
                    "no_wake=%u rx=%zu/%zu world=%u tx_on=%u eop=%u "
                    "cntdwn=%u\n",
                    dorado_trace_cycle, mask, eth->rx_on, eth->rx_hold,
                    eth->no_wakeups, eth->rx_pos, eth->rx_count,
                    eth->world_rx_words, eth->tx_on, eth->tx_eop,
                    eth->tx_cntdwn);
        }
    }
    return mask;
}
