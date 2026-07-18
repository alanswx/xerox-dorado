#include "ethernet.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

enum {
    PUP_SOCKET_MISC = 04,
    PUP_SOCKET_FTP = 03,
    /* PupWKS.mesa: Grapevine registry enquiry/poll sockets. */
    PUP_SOCKET_GV_RS_ENQUIRY = 050,
    PUP_SOCKET_GV_RS_POLL = 052,

    PUP_TYPE_ERROR = 04,
    PUP_TYPE_RTP_RFC = 010,
    PUP_TYPE_RTP_ABORT = 011,
    PUP_TYPE_RTP_END = 012,
    PUP_TYPE_RTP_END_REPLY = 013,

    PUP_TYPE_BSP_DATA = 020,
    PUP_TYPE_BSP_ADATA = 021,
    PUP_TYPE_BSP_ACK = 022,
    PUP_TYPE_BSP_MARK = 023,
    PUP_TYPE_BSP_AMARK = 026,

    FTP_MARK_RETRIEVE = 01,
    FTP_MARK_YES = 03,
    FTP_MARK_NO = 04,
    FTP_MARK_HERE_IS_FILE = 05,
    FTP_MARK_EOC = 06,
    FTP_MARK_VERSION = 010,
    FTP_MARK_HERE_IS_PLIST = 013,
    FTP_MARK_NEW_DIRECTORY = 014,

    FTP_TX_NONE = 0,
    FTP_TX_VERSION = 1,
    FTP_TX_PLIST = 2,
    FTP_TX_FILE = 3,
    FTP_TX_DONE = 4,
    FTP_TX_NOT_FOUND = 5,
    FTP_TX_GV_AUTH_REPLY = 6,

    FTP_PHASE_IDLE = 0,
    FTP_PHASE_WAIT_RETRIEVE_YES = 1,
    FTP_PHASE_STREAMING = 2,
    FTP_PHASE_DONE = 3,
    FTP_PHASE_GV_WAIT = 4,
    FTP_PHASE_GV_DONE = 5
};

#define DORADO_ETH_STP_INTERPACKET_TICKS 60000u

static uint32_t pup_id32(uint16_t hi, uint16_t lo)
{
    return ((uint32_t)hi << 16) | lo;
}

static uint16_t hi16(uint32_t v) { return (uint16_t)(v >> 16); }
static uint16_t lo16(uint32_t v) { return (uint16_t)(v & 0xFFFFu); }

static int ftp_trace(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_FTP_TRACE") ? 1 : 0;
    return cached;
}

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
    /* DORADO_FAKE_TIME=<unix-seconds> pins this clock so the whole boot is
     * DETERMINISTIC. This time(NULL) (the NetExec date/time response) is the
     * ONLY wall-clock/nondeterministic input in the emulator; letting it
     * float makes run-to-run behavior vary (e.g. the FULL.SYSOUT vp-8206
     * fault reproduces on some boots and not others). Pin it for repeatable
     * debugging and regression runs. */
    time_t now;
    {
        const char *ftm = getenv("DORADO_FAKE_TIME");
        now = (ftm && ftm[0]) ? (time_t)strtoll(ftm, NULL, 0) : time(NULL);
    }
    if (getenv("DORADO_ETHTIME_TRACE"))
        fprintf(stderr, "ETHTIME now=%lld (this is the sole nondeterministic "
                "wall-clock input; DORADO_FAKE_TIME pins it)\n", (long long)now);
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

void dorado_ethernet_set_ftp_sysout(dorado_ethernet *eth, const char *path)
{
    if (!eth) return;
    eth->ftp_sysout_path[0] = '\0';
    eth->ftp_enabled = 0;
    if (!path || !path[0]) return;
    snprintf(eth->ftp_sysout_path, sizeof eth->ftp_sysout_path, "%s", path);
    eth->ftp_enabled = 1;
}

void dorado_ethernet_set_ftp_root(dorado_ethernet *eth, const char *root)
{
    /* The fixed Interlisp sysout and the Cedar release tree share the original
     * Pup FTP/STP endpoint (socket 3).  Keep the state ABI stable for existing
     * snapshots: a directory in ftp_sysout_path means a rooted STP tree; a
     * regular file retains the old single-sysout behaviour. */
    if (!eth || !root || !root[0]) return;
    snprintf(eth->ftp_sysout_path, sizeof eth->ftp_sysout_path, "%s", root);
    eth->ftp_enabled = 1;
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

/* On a physical 3 Mb/s Ethernet, the next Pup cannot appear in the few
 * microinstructions between EIT consuming a packet's status word and its
 * WaitForBOP re-arm.  The in-process STP server used to enqueue the next
 * packet immediately from wakeup_mask(), so EIT saw it as stale input and
 * discarded/re-armed forever.  Keep the packet on the wire for this small
 * turnaround interval; 60,000 wakeup polls is the same conservative delay
 * used by the EFTP receiver handoff above. */

/* Faithful transmit wire model toggle (DORADO_ETH_WIRE). Default OFF so the
 * EFTP boot -- which relies on instant tx-completion -- is unaffected. */
static int eth_wire_model(void)
{
    static int cached = -1;
    if (cached < 0) cached = getenv("DORADO_ETH_WIRE") ? 1 : 0;
    return cached;
}

static int eth_queue_pup_bytes(dorado_ethernet *eth, uint16_t pup_type,
                               uint32_t id, uint16_t dnet_host,
                               uint16_t dsock_hi, uint16_t dsock_lo,
                               uint16_t snet_host, uint16_t ssock_hi,
                               uint16_t ssock_lo, const uint8_t *payload,
                               size_t nbytes)
{
    size_t cap = 0;
    size_t payload_words = (nbytes + 1u) / 2u;
    uint16_t dest_host = (uint16_t)(dnet_host & 0377);
    int ok = 1;

    eth_clear_rx(eth);
    uint16_t hdr[12] = {
        (uint16_t)((dest_host << 8) | eth->remote_host),
        DORADO_PUP_TYPE_ETHERNET,
        (uint16_t)(026 + nbytes),
        pup_type,
        hi16(id),
        lo16(id),
        dnet_host,
        dsock_hi,
        dsock_lo,
        snet_host,
        ssock_hi,
        ssock_lo
    };
    for (int i = 0; i < 12 && ok; i++)
        ok = append_rx_word(eth, &cap, hdr[i], 0);
    for (size_t i = 0; i < payload_words && ok; i++) {
        uint16_t w = (uint16_t)(payload[i * 2] << 8);
        if (i * 2 + 1 < nbytes) w |= payload[i * 2 + 1];
        ok = append_rx_word(eth, &cap, w, 0);
    }
    if (ok) {
        uint16_t cks = pup_checksum(eth->rx_words, 2,
                                    (size_t)(12 + payload_words) - 2);
        ok = append_rx_word(eth, &cap, cks, 0);
    }
    if (ok) ok = append_rx_word(eth, &cap, 0, 0);
    if (ok) ok = append_rx_word(eth, &cap, 0, 1);
    if (!ok) {
        eth_clear_rx(eth);
        return 0;
    }
    eth->rx_pos = 0;
    eth->ftp_packets_queued++;
    if (ftp_trace()) {
        fprintf(stderr,
                "FTP_QUEUE type=0o%o id=%08x bytes=%zu d=%06o/%o/%o "
                "s=%06o/%o/%o rx_words=%zu\n",
                pup_type, id, nbytes, dnet_host, dsock_hi, dsock_lo,
                snet_host, ssock_hi, ssock_lo, eth->rx_count);
    }
    return 1;
}

/* Translate an IFS Server-filename property into a safe path below the
 * configured release root.  Cedar's STP client sends names such as
 * [Cedar]<Cedar6.1>Top>Basic.Loadees.  The original STP server receives
 * that property list at the FTP endpoint (STPServerImpl.mesa DoFiles); our
 * first read-only implementation intentionally supports only this lookup /
 * retrieve path. */
static int eth_ftp_resolve_file(dorado_ethernet *eth, char *out, size_t outsz)
{
    struct stat st;
    if (!eth || !out || !outsz || !eth->ftp_sysout_path[0]) return 0;
    if (stat(eth->ftp_sysout_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        snprintf(out, outsz, "%s", eth->ftp_sysout_path);
        return 1;
    }

    const char *value = strstr((const char *)eth->ftp_cmd_data,
                               "Server-filename");
    char requested[384];
    if (!value) {
        /* Cedar's STP client normally supplies the decomposed IFS form:
         * (Directory Cedar6.1)(Name-Body Top>Basic.Loadees), rather than
         * the optional Server-filename string used by older FTP clients. */
        const char *directory = strstr((const char *)eth->ftp_cmd_data,
                                       "Directory ");
        const char *name_body = strstr((const char *)eth->ftp_cmd_data,
                                       "Name-Body ");
        if (directory && name_body) {
            directory += strlen("Directory ");
            name_body += strlen("Name-Body ");
            size_t dn = strcspn(directory, ") \t");
            size_t nn = strcspn(name_body, ") \t");
            if (dn == 0 || nn == 0 || dn + 1 + nn >= sizeof requested)
                return 0;
            memcpy(requested, directory, dn);
            requested[dn] = '>';
            memcpy(requested + dn + 1, name_body, nn);
            requested[dn + 1 + nn] = '\0';
            value = requested;
        } else {
            value = (const char *)eth->ftp_cmd_data; /* normalized form */
        }
    } else {
        value += strlen("Server-filename");
        while (*value == ' ' || *value == '\t') value++;
    }
    if (*value == '[') {
        const char *close = strchr(value, ']');
        if (!close) return 0;
        value = close + 1;
    }
    while (*value == '<' || *value == '>' || *value == ' ' || *value == '\t')
        value++;

    char relative[384];
    size_t n = 0;
    for (; *value && *value != ')' && n + 1 < sizeof relative; value++) {
        unsigned char c = (unsigned char)*value;
        if (c == '>') c = '/';
        if (!(isalnum(c) || c == '.' || c == '_' || c == '-' || c == '/' ||
              c == '!'))
            return 0;
        relative[n++] = (char)c;
    }
    relative[n] = '\0';
    while (relative[0] == '/') memmove(relative, relative + 1, strlen(relative));
    if (!relative[0] || strstr(relative, "..")) return 0;

    /* An IFS name may carry an explicit "!<version>" (FS demand-fetches an
     * attached file by the exact version its DF pinned).  The real server
     * parses the version into a separate property (STPServerImpl
     * SendPropList sends `Version` alongside the name); our tree stores one
     * version of each file under its bare name, and the DF date index is
     * keyed the same way, so strip the suffix. */
    {
        char *bang = strrchr(relative, '!');
        if (bang && bang != relative && strchr(bang, '/') == NULL) {
            const char *d = bang + 1;
            if (*d == 'H' || *d == 'h' || *d == 'L' || *d == 'l') {
                if (d[1] == '\0') *bang = '\0';
            } else {
                while (isdigit((unsigned char)*d)) d++;
                if (*d == '\0' && d != bang + 1) *bang = '\0';
            }
        }
    }
    char response_relative[sizeof relative];
    snprintf(response_relative, sizeof response_relative, "%s", relative);

    /* LoaderDriver's per-user probe has no standalone User volume.  Serve the
     * shared command-file bytes, but retain the requested pathname in
     * ftp_cmd_data: STP's confirmation compares the HereIsPList name with the
     * name it requested, while a following Yes will resolve this alias again. */
    if (strcmp(relative, "Guest/6.1/Basic.Loadees") == 0)
        snprintf(relative, sizeof relative, "Cedar6.1/Top/Basic.Loadees");
    snprintf((char *)eth->ftp_cmd_data, sizeof eth->ftp_cmd_data, "%s",
             response_relative);
    eth->ftp_cmd_len = strlen((char *)eth->ftp_cmd_data);
    if (snprintf(out, outsz, "%s/%s", eth->ftp_sysout_path, relative) >=
        (int)outsz)
        return 0;
    return 1;
}

/* ---- Creation dates from the release DFs ---------------------------------
 *
 * A DF pins every file it names to an exact creation date, and BringOver asks
 * the server for that specific version: FSRemoteFileImpl compares the date in
 * our HereIsPList against the DF's, and on a mismatch reports
 *   FS.Error: Could not find "<file>" created on <date>
 * and skips the file.  A synthetic constant date therefore silently installs
 * nothing.  The dates are recorded in the DFs themselves, so index them --
 * `Exports`/`Directory [Cedar]<Cedar6.1>Dir>` opens a section, and each entry
 * is `name!version   dd-Mon-yy hh:mm:ss ZONE` (a leading '+' is DF
 * bookkeeping).  Host-side data, so it is a file static rather than emulated
 * state that would have to be snapshotted. */
#define FTP_DATE_MAX 4096
static struct {
    char key[128];      /* "dir/name", lowercased */
    char date[40];      /* verbatim from the DF */
} ftp_dates[FTP_DATE_MAX];
static int ftp_date_count;
static int ftp_dates_loaded;

static void ftp_date_add(const char *dir, const char *name, const char *date)
{
    if (ftp_date_count >= FTP_DATE_MAX) return;
    char key[128];
    if ((size_t)snprintf(key, sizeof key, "%s/%s", dir, name) >= sizeof key)
        return;
    for (char *p = key; *p; p++) *p = (char)tolower((unsigned char)*p);
    for (int i = 0; i < ftp_date_count; i++)
        if (strcmp(ftp_dates[i].key, key) == 0) return;   /* first wins */
    snprintf(ftp_dates[ftp_date_count].key,
             sizeof ftp_dates[0].key, "%s", key);
    snprintf(ftp_dates[ftp_date_count].date,
             sizeof ftp_dates[0].date, "%s", date);
    ftp_date_count++;
}

static void ftp_dates_scan_df(const char *path)
{
    /* These are original Mesa text files: lines end with CR, not LF, so they
     * must be split by hand rather than with fgets(). */
    FILE *fp = fopen(path, "rb");
    if (!fp) return;
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return; }
    long size = ftell(fp);
    if (size <= 0 || size > (1 << 20)) { fclose(fp); return; }
    rewind(fp);
    char *buf = malloc((size_t)size + 1);
    if (!buf) { fclose(fp); return; }
    size_t got = fread(buf, 1, (size_t)size, fp);
    fclose(fp);
    buf[got] = '\0';

    char dir[64] = "";
    char line[512];
    for (size_t pos = 0; pos < got; ) {
        size_t end = pos;
        while (end < got && buf[end] != '\r' && buf[end] != '\n') end++;
        size_t len = end - pos;
        if (len >= sizeof line) len = sizeof line - 1;
        memcpy(line, buf + pos, len);
        line[len] = '\0';
        pos = end;
        while (pos < got && (buf[pos] == '\r' || buf[pos] == '\n')) pos++;

        /* Section header: `Exports|Directory [Volume]<Dir>Sub>` -- the entries
         * that follow live there.  Keep every path component (the header can
         * be multi-level, `<Cedar6.1>Top>`), drop the volume, and use '/' as
         * the separator, so the key matches the relative path
         * eth_ftp_resolve_file derives from a request. */
        const char *lt = strchr(line, '<');
        const char *lb = strchr(line, '[');
        if (lt && lb && lb < lt &&
            (strstr(line, "Exports") || strstr(line, "Directory")) &&
            !strstr(line, "Imports")) {
            const char *last = strrchr(lt, '>');
            size_t n = last && last > lt + 1 ? (size_t)(last - (lt + 1)) : 0;
            if (n > 0 && n < sizeof dir) {
                memcpy(dir, lt + 1, n);
                dir[n] = '\0';
                for (char *p = dir; *p; p++)
                    if (*p == '>') *p = '/';
            }
            continue;
        }
        if (strstr(line, "Imports")) { dir[0] = '\0'; continue; }
        if (!dir[0] || (line[0] != ' ' && line[0] != '\t')) continue;

        const char *s = line;
        while (*s == ' ' || *s == '\t' || *s == '+') s++;
        /* An entry is `name[!version]   date`.  A DF's own self-export
         * carries NO "!version" (`BootEssentials.df   02-Jul-86 ...`),
         * and BringOver validates the DF against exactly that date --
         * requiring the bang here made the server advertise a synthetic
         * date for every DF and tripped "Could not find <df> created on
         * <date> / Shall I continue anyway?" on each install. */
        size_t tok = strcspn(s, " \t");
        if (tok == 0) continue;
        const char *bang = memchr(s, '!', tok);
        size_t nlen = bang ? (size_t)(bang - s) : tok;
        if (nlen == 0 || nlen >= 64 || !memchr(s, '.', nlen)) continue;
        const char *d;
        if (bang) {
            d = bang + 1;
            while (isdigit((unsigned char)*d)) d++;
            if (d == bang + 1) continue;                    /* bare '!' */
        } else {
            d = s + tok;
        }
        while (*d == ' ' || *d == '\t') d++;
        if (!isdigit((unsigned char)*d)) continue;          /* no date */
        char name[64];
        memcpy(name, s, nlen);
        name[nlen] = '\0';
        char date[40];
        snprintf(date, sizeof date, "%s", d);
        for (int i = (int)strlen(date) - 1; i >= 0 && isspace((unsigned char)date[i]); i--)
            date[i] = '\0';
        if (date[0]) ftp_date_add(dir, name, date);
    }
    free(buf);
}

/* Index every DF in one directory of the served tree. */
static void ftp_dates_scan_dir(const char *dir)
{
    DIR *dp = opendir(dir);
    if (!dp) return;
    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        size_t n = strlen(de->d_name);
        if (n < 4 || strcmp(de->d_name + n - 3, ".df") != 0) continue;
        char path[1024];
        if ((size_t)snprintf(path, sizeof path, "%s/%s", dir, de->d_name) <
            sizeof path)
            ftp_dates_scan_df(path);
    }
    closedir(dp);
}

static void ftp_dates_load(const dorado_ethernet *eth)
{
    if (ftp_dates_loaded || !eth->ftp_sysout_path[0]) return;
    ftp_dates_loaded = 1;
    /* DFs live in the "Top" directory of each release volume served from the
     * root (Cedar6.1/Top, CedarFonts/Top, ...). */
    DIR *dp = opendir(eth->ftp_sysout_path);
    if (!dp) return;
    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        if (de->d_name[0] == '.') continue;
        char top[768];
        if ((size_t)snprintf(top, sizeof top, "%s/%s/Top",
                             eth->ftp_sysout_path, de->d_name) >= sizeof top)
            continue;
        ftp_dates_scan_dir(top);
    }
    closedir(dp);
    if (ftp_trace())
        fprintf(stderr, "STP_DATES indexed %d file dates under %s\n",
                ftp_date_count, eth->ftp_sysout_path);
}

/* Creation date for a resolved relative path ("Cedar6.1/Viewers/Icons.tip"),
 * or NULL if no DF names it (the Basic.Loadees BCDs, which the Loader takes
 * without a date check). */
static const char *eth_ftp_creation_date(const dorado_ethernet *eth,
                                         const char *relative)
{
    ftp_dates_load(eth);
    char key[128];
    if ((size_t)snprintf(key, sizeof key, "%s", relative) >= sizeof key)
        return NULL;
    for (char *p = key; *p; p++) *p = (char)tolower((unsigned char)*p);
    for (int i = 0; i < ftp_date_count; i++)
        if (strcmp(ftp_dates[i].key, key) == 0) return ftp_dates[i].date;
    return NULL;
}

static uint32_t eth_ftp_file_size(dorado_ethernet *eth)
{
    char path[512];
    if (!eth_ftp_resolve_file(eth, path, sizeof path)) return 0;
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return 0;
    }
    long n = ftell(fp);
    fclose(fp);
    if (n < 0) return 0;
    return (uint32_t)n;
}

static int eth_ftp_file_exists(dorado_ethernet *eth)
{
    char path[512];
    struct stat st;
    return eth_ftp_resolve_file(eth, path, sizeof path) &&
           stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static void eth_ftp_start_tx(dorado_ethernet *eth, uint8_t mode)
{
    eth->ftp_tx_mode = mode;
    eth->ftp_tx_step = 0;
}

/* PupStreamImpl's Handle owns one ring of output fingers for the lifetime of
 * the BSP connection. WaitOutputReady marks every half-window packet as
 * AData/AMark, so the count spans STP's version reply, directory plist,
 * retrieve plist, file, and trailing completion -- it does not restart at
 * HereIsFile. Cedar advertises 32 input fingers, hence an acknowledgement is
 * requested every 16 outgoing fingers while as many as 32 may be in flight. */
static unsigned eth_ftp_file_ack_window(const dorado_ethernet *eth)
{
    unsigned n = eth->ftp_client_pup_alloc;
    if (n < 2) n = 32;       /* RFC allocation arrives before file traffic. */
    return (n + 1u) / 2u;
}

static int eth_ftp_file_packet_needs_ack(const dorado_ethernet *eth)
{
    if (!eth->ftp_open) return 0;
    /* Byte-window liveness: once the bytes outstanding since the last
     * acknowledged position pass half the client's advertised byte
     * allocation, every further packet asks for an ack.  The old
     * pup-count marks (next == half/full) drifted out of alignment with
     * the client's own finger accounting on multi-window transfers, so a
     * window could pass with no AData mark at all; the client then never
     * acked, the byte window overran, and (with no retransmit ring) the
     * transfer wedged -- the >100 KB demand-fetch stall of 2026-07-18. */
    if (eth->ftp_client_byte_alloc != 0) {
        uint32_t outstanding = eth->ftp_tx_next - eth->ftp_last_ack;
        return outstanding * 2u >= eth->ftp_client_byte_alloc;
    }
    {
        unsigned next = eth->ftp_tx_in_flight + 1u;
        unsigned half = eth_ftp_file_ack_window(eth);
        unsigned full = eth->ftp_client_pup_alloc;
        return next == half || (full != 0 && next == full);
    }
}

static int eth_ftp_queue_ack(dorado_ethernet *eth)
{
    uint8_t body[6];
    uint16_t bytes_per_pup = 512;
    uint16_t pup_alloc = 6;
    uint16_t byte_alloc = (uint16_t)(bytes_per_pup * pup_alloc);
    body[0] = (uint8_t)(bytes_per_pup >> 8);
    body[1] = (uint8_t)(bytes_per_pup & 0xFF);
    body[2] = (uint8_t)(pup_alloc >> 8);
    body[3] = (uint8_t)(pup_alloc & 0xFF);
    body[4] = (uint8_t)(byte_alloc >> 8);
    body[5] = (uint8_t)(byte_alloc & 0xFF);
    eth->ftp_pending_ack = 0;
    return eth_queue_pup_bytes(eth, PUP_TYPE_BSP_ACK, eth->ftp_rx_next,
                               eth->ftp_client_net_host,
                               eth->ftp_client_sock_hi,
                               eth->ftp_client_sock_lo,
                               eth->ftp_server_net_host,
                               eth->ftp_server_sock_hi,
                               eth->ftp_server_sock_lo,
                               body, sizeof body);
}

static int eth_ftp_queue_mark(dorado_ethernet *eth, uint8_t mark,
                              int request_ack)
{
    uint8_t body[1] = { mark };
    uint32_t id = eth->ftp_tx_next;
    if (!eth_queue_pup_bytes(eth,
                             request_ack ? PUP_TYPE_BSP_AMARK
                                         : PUP_TYPE_BSP_MARK,
                             id,
                             eth->ftp_client_net_host,
                             eth->ftp_client_sock_hi,
                             eth->ftp_client_sock_lo,
                             eth->ftp_server_net_host,
                             eth->ftp_server_sock_hi,
                             eth->ftp_server_sock_lo,
                             body, sizeof body))
        return 0;
    eth->ftp_tx_next += 1;
    eth->ftp_tx_last_end = eth->ftp_tx_next;
    eth->ftp_waiting_for_ack = request_ack ? 1 : 0;
    if (eth->ftp_open)
        eth->ftp_tx_in_flight++;
    if (ftp_trace() && eth->ftp_open) {
        fprintf(stderr, "FTP_WINDOW mark=0o%o ack=%d mode=%u in_flight=%u/%u\n",
                mark, request_ack, eth->ftp_tx_mode, eth->ftp_tx_in_flight,
                eth_ftp_file_ack_window(eth));
    }
    return 1;
}

static int eth_ftp_queue_data(dorado_ethernet *eth, const uint8_t *data,
                              size_t nbytes, int request_ack)
{
    uint32_t id = eth->ftp_tx_next;
    if (!eth_queue_pup_bytes(eth,
                             request_ack ? PUP_TYPE_BSP_ADATA
                                         : PUP_TYPE_BSP_DATA,
                             id,
                             eth->ftp_client_net_host,
                             eth->ftp_client_sock_hi,
                             eth->ftp_client_sock_lo,
                             eth->ftp_server_net_host,
                             eth->ftp_server_sock_hi,
                             eth->ftp_server_sock_lo,
                             data, nbytes))
        return 0;
    eth->ftp_tx_next += (uint32_t)nbytes;
    eth->ftp_tx_last_end = eth->ftp_tx_next;
    eth->ftp_waiting_for_ack = request_ack ? 1 : 0;
    if (eth->ftp_open)
        eth->ftp_tx_in_flight++;
    if (ftp_trace() && eth->ftp_open) {
        fprintf(stderr, "FTP_WINDOW data=%zu ack=%d mode=%u in_flight=%u/%u\n",
                nbytes, request_ack, eth->ftp_tx_mode, eth->ftp_tx_in_flight,
                eth_ftp_file_ack_window(eth));
    }
    return 1;
}

static int eth_ftp_queue_text_with_code(dorado_ethernet *eth, uint8_t code,
                                        const char *text)
{
    uint8_t buf[256];
    size_t n = 0;
    buf[n++] = code;
    if (text)
        while (*text && n < sizeof buf) buf[n++] = (uint8_t)*text++;
    return eth_ftp_queue_data(eth, buf, n, 0);
}

static int eth_ftp_queue_plist(dorado_ethernet *eth)
{
    char plist[512];
    char path[512];
    uint32_t advertised_size = eth->ftp_file_size;
    /* Diagnostic only: lets the Cedar cache callback be tested with a
     * smaller allocation while the transmitted host file remains unchanged.
     * Normal runs always preserve the source file's exact byte count. */
    const char *override_size = getenv("DORADO_FTP_PLIST_SIZE_OVERRIDE");
    if (override_size && *override_size) {
        char *end = NULL;
        unsigned long value = strtoul(override_size, &end, 0);
        if (end && *end == '\0' && value <= UINT32_MAX)
            advertised_size = (uint32_t)value;
    }
    if (!eth_ftp_resolve_file(eth, path, sizeof path)) return 0;
    const char *relative = (const char *)eth->ftp_cmd_data;
    const char *name = strrchr(relative, '/');
    name = name ? name + 1 : relative;
    char directory[384];
    size_t dn = (size_t)(name - relative);
    if (dn && relative[dn - 1] == '/') dn--;
    if (dn >= sizeof directory) return 0;
    memcpy(directory, relative, dn);
    directory[dn] = '\0';
    for (size_t i = 0; i < dn; i++)
        if (directory[i] == '/') directory[i] = '>';

    /* Preserve LoaderDriver's requested property decomposition for its user
     * override probe.  Its Name-Body deliberately includes "6.1>"; folding
     * that component into Directory changes the plist it confirms. */
    if (strcmp(relative, "Guest/6.1/Basic.Loadees") == 0) {
        snprintf(directory, sizeof directory, "Guest");
        name = "6.1>Basic.Loadees";
    }

    /* STPServerMainImpl.SendPropList: honor Cedar's requested names and
     * spelling.  In particular, `Creation-Date` is case-sensitive in its
     * Lisp property parser; our old FTP-style Server-filename plist made
     * STP.Open unwind before it could accept the transfer. */
    /* STPServerImpl.SendPropList forces Server-Filename, Directory,
     * Name-Body, Version, and Byte-Size into every reply.  LoaderDriver
     * needs Byte-Size to open a received BCD as binary even when it did not
     * explicitly list that property in its request. */
    /* Serve the file's real creation date when a release DF records one:
     * BringOver asks for a file "created on <date>" and skips anything whose
     * advertised date differs (see eth_ftp_creation_date).  The synthetic
     * fallback stands in for files no DF names, such as the Basic.Loadees
     * BCDs, which the Loader accepts without a date check. */
    const char *created = eth_ftp_creation_date(eth, relative);
    snprintf(plist, sizeof plist,
             "((Server-Filename <%s>%s!1)(Directory %s)(Name-Body %s)"
             "(Version 1)(Byte-Size 8)"
             "(Creation-Date %s)(Size %u))",
             directory, name, directory, name,
             created ? created : "01-Jan-84 00:00:00 GST", advertised_size);
    if (ftp_trace()) fprintf(stderr, "STP_PLIST %s\n", plist);
    return eth_ftp_queue_data(eth, (const uint8_t *)plist, strlen(plist),
                              eth_ftp_file_packet_needs_ack(eth));
}

static int eth_ftp_queue_file_chunk(dorado_ethernet *eth)
{
    /* PupStreamImpl's get-hop buffer is maxDataBytes (1,478 bytes on this
     * directly connected Cedar host).  Preserve its finger granularity: the
     * 18,432-byte RPCRuntime BCD then needs 13 data Pups, not 36, and fits
     * inside the receiver's 16-finger half-window before an acknowledgement
     * is required. */
    uint8_t buf[1478];
    char path[512];
    if (!eth_ftp_resolve_file(eth, path, sizeof path)) return 0;
    FILE *fp = fopen(path, "rb");
    if (!fp) return 0;
    if (fseek(fp, (long)eth->ftp_file_pos, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }
    size_t want = eth->ftp_file_size - eth->ftp_file_pos;
    if (want > sizeof buf) want = sizeof buf;
    if (eth->ftp_client_bytes_per_pup != 0 &&
        want > eth->ftp_client_bytes_per_pup)
        want = eth->ftp_client_bytes_per_pup;
    size_t got = fread(buf, 1, want, fp);
    fclose(fp);
    if (got == 0) return 0;
    /* Most file fingers are ordinary Data.  Every half-window is AData so
     * PupStream can replenish Cedar's finite input-finger pool. */
    if (!eth_ftp_queue_data(eth, buf, got,
                            eth_ftp_file_packet_needs_ack(eth))) return 0;
    eth->ftp_file_pos += (uint32_t)got;
    return 1;
}

static void eth_ftp_maybe_deliver(dorado_ethernet *eth);
static void eth_ftp_ctx_store(dorado_ethernet *eth);
static void eth_ftp_ctx_activate(dorado_ethernet *eth, int slot);

static void eth_ftp_handle_command(dorado_ethernet *eth)
{
    if (ftp_trace()) {
        fprintf(stderr, "FTP_CMD mark=0o%o len=%zu phase=%u\n",
                eth->ftp_cmd_mark, eth->ftp_cmd_len, eth->ftp_phase);
        if (eth->ftp_cmd_mark == FTP_MARK_RETRIEVE)
            fprintf(stderr, "FTP_RETRIEVE \"%.*s\"\n",
                    (int)eth->ftp_cmd_len, eth->ftp_cmd_data);
    }
    switch (eth->ftp_cmd_mark) {
    case FTP_MARK_VERSION:
        eth_ftp_start_tx(eth, FTP_TX_VERSION);
        break;
    case FTP_MARK_RETRIEVE:
    case FTP_MARK_NEW_DIRECTORY:
        eth->ftp_file_pos = 0;
        if (!eth_ftp_file_exists(eth)) {
            if (ftp_trace())
                fprintf(stderr, "STP_MISSING %s\n", eth->ftp_cmd_data);
            eth_ftp_start_tx(eth, FTP_TX_NOT_FOUND);
            eth->ftp_phase = FTP_PHASE_DONE;
            break;
        }
        eth->ftp_file_size = eth_ftp_file_size(eth);
        if (ftp_trace())
            fprintf(stderr, "STP_SERVE %s (%u bytes)\n",
                    eth->ftp_cmd_data, eth->ftp_file_size);
        if (eth->ftp_file_size == 0) {
            eth_ftp_start_tx(eth, FTP_TX_DONE);
            eth->ftp_phase = FTP_PHASE_DONE;
        } else {
            eth_ftp_start_tx(eth, FTP_TX_PLIST);
            eth->ftp_phase = FTP_PHASE_WAIT_RETRIEVE_YES;
        }
        break;
    case FTP_MARK_YES:
        if (eth->ftp_phase == FTP_PHASE_WAIT_RETRIEVE_YES) {
            if (ftp_trace())
                fprintf(stderr, "STP_TRANSFER %s (%u bytes)\n",
                        eth->ftp_cmd_data, eth->ftp_file_size);
            eth_ftp_start_tx(eth, FTP_TX_FILE);
            eth->ftp_phase = FTP_PHASE_STREAMING;
        }
        break;
    case FTP_MARK_NO:
        eth->ftp_phase = FTP_PHASE_DONE;
        break;
    default:
        break;
    }
    eth->ftp_cmd_mark = 0;
    /* A Retrieve request's normalized name remains in ftp_cmd_data until the
     * transfer ends; a following Yes carries no text and must not erase it. */
    if (eth->ftp_tx_mode != FTP_TX_PLIST && eth->ftp_tx_mode != FTP_TX_FILE &&
        eth->ftp_tx_mode != FTP_TX_DONE)
        eth->ftp_cmd_len = 0;
}

static void eth_ftp_ingest_payload(dorado_ethernet *eth)
{
    uint16_t type = eth->tx_words[3];
    uint32_t id = pup_id32(eth->tx_words[4], eth->tx_words[5]);
    size_t nbytes = eth->tx_words[2] > 026 ? (size_t)(eth->tx_words[2] - 026) : 0;
    int duplicate = id + nbytes <= eth->ftp_rx_next;

    if (id + nbytes > eth->ftp_rx_next)
        eth->ftp_rx_next = id + (uint32_t)nbytes;
    /* PupBSPProt.BSPPupProc schedules an immediate acknowledgement only for
     * AData/AMark.  Ordinary Data/Mark packets advance rcvByteID but are
     * acknowledged by the normal BSP timer/allocation path.  In particular,
     * do not inject an ACK between Cedar's Retrieve EOC and our HereIsPList. */
    eth->ftp_pending_ack = type == PUP_TYPE_BSP_ADATA ||
                           type == PUP_TYPE_BSP_AMARK;

    if (type == PUP_TYPE_BSP_MARK || type == PUP_TYPE_BSP_AMARK) {
        if (nbytes > 0) {
            uint8_t mark = (uint8_t)(eth->tx_words[12] >> 8);
            if (ftp_trace()) {
                fprintf(stderr,
                        "FTP_MARK id=%08x mark=0o%o phase=%u rx_next=%08x\n",
                        id, mark, eth->ftp_phase, eth->ftp_rx_next);
            }
            if (mark == FTP_MARK_EOC) {
                /* The client retransmits an acknowledged Retrieve EOC while
                 * it waits for our first plist segment.  It belongs to the
                 * preceding command stream (and can have a lower byte ID
                 * than the current receive cursor), so acknowledge it but
                 * do not restart the pending plist transfer. */
                if (eth->ftp_phase == FTP_PHASE_WAIT_RETRIEVE_YES &&
                    eth->ftp_cmd_mark != FTP_MARK_YES && duplicate) {
                    eth->ftp_pending_ack = 0;
                    return;
                }
                eth->ftp_cmd_data[eth->ftp_cmd_len] = '\0';
                eth_ftp_handle_command(eth);
            } else {
                /* The Loader checkpoint can retransmit an older Retrieve
                 * after the current one has advanced the receive cursor.
                 * Ignore that duplicate command. */
                if (mark == FTP_MARK_RETRIEVE &&
                    eth->ftp_phase == FTP_PHASE_WAIT_RETRIEVE_YES &&
                    duplicate) {
                    eth->ftp_pending_ack = 0;
                    return;
                }
                /* BSP's DataPacket ignores a finger at or below pullId.
                 * Cedar can retransmit a prior STP Yes while it begins the
                 * next Retrieve; its old byte ID must not replace the new
                 * Retrieve mark in any server phase.  In particular, after
                 * a completed file the older Yes otherwise changes the
                 * command mark just before the new Retrieve's EOC arrives. */
                if (mark == FTP_MARK_YES && duplicate) {
                    if (eth->ftp_phase != FTP_PHASE_WAIT_RETRIEVE_YES)
                        eth->ftp_pending_ack = 0;
                    return;
                }
                eth->ftp_cmd_mark = mark;
                /* STPImpl.PutCommand sends a complete command, not a bare
                 * mark: Yes, its zero reply-code plus text, then EOC.  The
                 * original STPServerImpl.RetrieveFile calls GetCommandString
                 * before it emits HereIsFile.  Preserve the normalized
                 * Retrieve path while that Yes command arrives; its code and
                 * text are not a pathname. */
                if (mark == FTP_MARK_YES &&
                    eth->ftp_phase == FTP_PHASE_WAIT_RETRIEVE_YES) {
                    eth->ftp_cmd_mark = mark;
                    return;
                }
                eth->ftp_cmd_len = 0;
                for (size_t i = 1; i < nbytes; i++) {
                    if (eth->ftp_cmd_len >= sizeof eth->ftp_cmd_data) break;
                    uint16_t w = eth->tx_words[12 + i / 2];
                    eth->ftp_cmd_data[eth->ftp_cmd_len++] =
                        (uint8_t)((i & 1) ? (w & 0xFF) : (w >> 8));
                }
            }
        }
    } else {
        if (!(eth->ftp_phase == FTP_PHASE_WAIT_RETRIEVE_YES &&
              eth->ftp_cmd_mark == FTP_MARK_YES)) {
            for (size_t i = 0; i < nbytes; i++) {
                if (eth->ftp_cmd_len >= sizeof eth->ftp_cmd_data) break;
                uint16_t w = eth->tx_words[12 + i / 2];
                eth->ftp_cmd_data[eth->ftp_cmd_len++] =
                    (uint8_t)((i & 1) ? (w & 0xFF) : (w >> 8));
            }
        }
        /* GVProtocol.SendNow flushes an unmarked BSP byte stream.  This is
         * deliberately not a fake credential authority: report AllDown(9).
         * UserCredentialsImpl treats that source-defined result as
         * "Grapevine down, proceeding anyway", which permits a standalone
         * historical release tree to continue into LoaderDriver/STP.
         * GVProtocol.ReturnCode is one Mesa word: the machine-dependent
         * Code and RNameType occupy its high and low byte respectively.
         * The complete Authenticate request is at least operation + an empty
         * RName + Password, so wait for a full record before replying. */
        if (eth->ftp_server_sock_lo == PUP_SOCKET_GV_RS_ENQUIRY &&
            eth->ftp_phase == FTP_PHASE_GV_WAIT && eth->ftp_cmd_len >= 14) {
            eth_ftp_start_tx(eth, FTP_TX_GV_AUTH_REPLY);
            eth->ftp_phase = FTP_PHASE_GV_DONE;
            if (ftp_trace()) {
                fprintf(stderr, "GV_AUTH request bytes=%zu op=%02x%02x\n",
                        eth->ftp_cmd_len, eth->ftp_cmd_data[0],
                        eth->ftp_cmd_data[1]);
            }
        }
    }
}

static int eth_ftp_tx_next_segment(dorado_ethernet *eth)
{
    switch (eth->ftp_tx_mode) {
    case FTP_TX_VERSION:
        if (eth->ftp_tx_step == 0) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_mark(eth, FTP_MARK_VERSION,
                                      eth_ftp_file_packet_needs_ack(eth));
        }
        if (eth->ftp_tx_step == 1) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_data(eth,
                                      (const uint8_t *)"\1Dorado FTP Server",
                                      sizeof "\1Dorado FTP Server" - 1,
                                      eth_ftp_file_packet_needs_ack(eth));
        }
        eth->ftp_tx_mode = FTP_TX_NONE;
        return eth_ftp_queue_mark(eth, FTP_MARK_EOC,
                                  eth_ftp_file_packet_needs_ack(eth));
    case FTP_TX_PLIST:
        if (eth->ftp_tx_step == 0) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_mark(eth, FTP_MARK_HERE_IS_PLIST,
                                      eth_ftp_file_packet_needs_ack(eth));
        }
        if (eth->ftp_tx_step == 1) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_plist(eth);
        }
        eth->ftp_tx_mode = FTP_TX_NONE;
        return eth_ftp_queue_mark(eth, FTP_MARK_EOC,
                                  eth_ftp_file_packet_needs_ack(eth));
    case FTP_TX_FILE:
        if (eth->ftp_tx_step == 0) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_mark(eth, FTP_MARK_HERE_IS_FILE,
                                      eth_ftp_file_packet_needs_ack(eth));
        }
        if (eth->ftp_file_pos < eth->ftp_file_size)
            return eth_ftp_queue_file_chunk(eth);
        eth_ftp_start_tx(eth, FTP_TX_DONE);
        eth->ftp_phase = FTP_PHASE_DONE;
        return eth_ftp_tx_next_segment(eth);
    case FTP_TX_DONE:
        if (eth->ftp_tx_step == 0) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_mark(eth, FTP_MARK_YES,
                                      eth_ftp_file_packet_needs_ack(eth));
        }
        if (eth->ftp_tx_step == 1) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_data(eth,
                                      (const uint8_t *)"\0Transfer complete",
                                      sizeof "\0Transfer complete" - 1,
                                      eth_ftp_file_packet_needs_ack(eth));
        }
        eth->ftp_tx_mode = FTP_TX_NONE;
        return eth_ftp_queue_mark(eth, FTP_MARK_EOC,
                                  eth_ftp_file_packet_needs_ack(eth));
    case FTP_TX_NOT_FOUND:
        if (eth->ftp_tx_step == 0) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_mark(eth, FTP_MARK_NO, 0);
        }
        if (eth->ftp_tx_step == 1) {
            eth->ftp_tx_step++;
            return eth_ftp_queue_text_with_code(eth, 0100, "File not found");
        }
        eth->ftp_tx_mode = FTP_TX_NONE;
        return eth_ftp_queue_mark(eth, FTP_MARK_EOC, 0);
    case FTP_TX_GV_AUTH_REPLY: {
        static const uint8_t reply[] = { 9, 0 }; /* AllDown, group */
        eth->ftp_tx_mode = FTP_TX_NONE;
        return eth_ftp_queue_data(eth, reply, sizeof reply, 1);
    }
    default:
        return 0;
    }
}

/* The pump below runs on the working set, so a connection parked in a slot
 * would never make progress on its own.  If the loaded connection has
 * nothing to send, switch to one that does. */
static void eth_ftp_pick_busy_conn(dorado_ethernet *eth)
{
    if (eth->ftp_open && (eth->ftp_pending_ack ||
                          eth->ftp_tx_mode != FTP_TX_NONE))
        return;
    for (int i = 0; i < DORADO_FTP_MAX_CONN; i++) {
        const struct dorado_ftp_ctx *c = &eth->ftp_ctx[i];
        if (eth->ftp_ctx_valid && eth->ftp_ctx_cur == (uint8_t)i) continue;
        if (!c->used || !c->open) continue;
        if (c->pending_ack || c->tx_mode != FTP_TX_NONE) {
            eth_ftp_ctx_activate(eth, i);
            return;
        }
    }
}

static void eth_ftp_maybe_deliver(dorado_ethernet *eth)
{
    if (!eth->ftp_enabled) return;
    if (eth->rx_pos < eth->rx_count || eth->rx_hold ||
        eth->ftp_delivery_blocked) return;
    eth_ftp_pick_busy_conn(eth);
    if (!eth->ftp_open) return;
    if (eth->ftp_pending_ack) {
        (void)eth_ftp_queue_ack(eth);
        return;
    }
    /* An AData/AMark asks for an acknowledgement but does not itself stall
     * PupStream's pusher.  The source continues until all advertised output
     * fingers are in flight; only then must the next segment wait. */
    /* Honor the client's BYTE allocation: never run more than its
     * advertised window past the last acknowledged position (the client
     * buffers exactly that much; excess is dropped and, with no
     * retransmit ring here, lost forever -> wedge). */
    if (eth->ftp_client_byte_alloc != 0 &&
        (uint32_t)(eth->ftp_tx_next - eth->ftp_last_ack) >=
            eth->ftp_client_byte_alloc)
        return;
    if ((eth->ftp_waiting_for_ack && eth->ftp_client_pup_alloc != 0 &&
         eth->ftp_tx_in_flight >= eth->ftp_client_pup_alloc) ||
        eth->ftp_tx_mode == FTP_TX_NONE)
        return;
    (void)eth_ftp_tx_next_segment(eth);
}

int dorado_ethernet_ftp_file_delivery_active(const dorado_ethernet *eth)
{
    if (!eth || !eth->ftp_enabled || !eth->ftp_open) return 0;
    return eth->ftp_tx_mode == FTP_TX_FILE ||
           eth->ftp_tx_mode == FTP_TX_DONE ||
           eth->ftp_phase == FTP_PHASE_DONE;
}

static int eth_ftp_queue_rfc_reply(dorado_ethernet *eth)
{
    uint8_t body[6];
    body[0] = (uint8_t)(eth->ftp_server_net_host >> 8);
    body[1] = (uint8_t)(eth->ftp_server_net_host & 0xFF);
    body[2] = (uint8_t)(eth->ftp_server_sock_hi >> 8);
    body[3] = (uint8_t)(eth->ftp_server_sock_hi & 0xFF);
    body[4] = (uint8_t)(eth->ftp_server_sock_lo >> 8);
    body[5] = (uint8_t)(eth->ftp_server_sock_lo & 0xFF);
    return eth_queue_pup_bytes(eth, PUP_TYPE_RTP_RFC,
                               pup_id32(eth->ftp_conn_hi, eth->ftp_conn_lo),
                               eth->ftp_client_net_host,
                               eth->ftp_client_sock_hi,
                               eth->ftp_client_sock_lo,
                               eth->ftp_server_net_host,
                               eth->ftp_server_sock_hi,
                               eth->ftp_server_sock_lo,
                               body, sizeof body);
}

/* ---- Per-connection context switching ------------------------------------
 *
 * The ftp_* fields are the working set for exactly one BSP connection. Cedar
 * runs several to socket 3 concurrently (LoaderDriver's, plus FS/DFOperations
 * ones for the Installer) and interleaves them, and each has its own byte-ID
 * space, so the working set is saved to / loaded from a per-connection slot
 * keyed by the client socket named in that connection's RFC. */

static void eth_ftp_ctx_store(dorado_ethernet *eth)
{
    struct dorado_ftp_ctx *c;
    if (!eth->ftp_ctx_valid) return;
    c = &eth->ftp_ctx[eth->ftp_ctx_cur];
    c->used = 1;
    c->open = eth->ftp_open;
    c->pending_ack = eth->ftp_pending_ack;
    c->waiting_for_ack = eth->ftp_waiting_for_ack;
    c->tx_mode = eth->ftp_tx_mode;
    c->tx_step = eth->ftp_tx_step;
    c->phase = eth->ftp_phase;
    c->cmd_mark = eth->ftp_cmd_mark;
    c->tx_in_flight = eth->ftp_tx_in_flight;
    c->conn_hi = eth->ftp_conn_hi;
    c->conn_lo = eth->ftp_conn_lo;
    c->client_net_host = eth->ftp_client_net_host;
    c->client_sock_hi = eth->ftp_client_sock_hi;
    c->client_sock_lo = eth->ftp_client_sock_lo;
    c->server_net_host = eth->ftp_server_net_host;
    c->server_sock_hi = eth->ftp_server_sock_hi;
    c->server_sock_lo = eth->ftp_server_sock_lo;
    c->client_bytes_per_pup = eth->ftp_client_bytes_per_pup;
    c->client_pup_alloc = eth->ftp_client_pup_alloc;
    c->client_byte_alloc = eth->ftp_client_byte_alloc;
    c->rx_next = eth->ftp_rx_next;
    c->tx_next = eth->ftp_tx_next;
    c->tx_last_end = eth->ftp_tx_last_end;
    c->last_ack = eth->ftp_last_ack;
    c->file_pos = eth->ftp_file_pos;
    c->file_size = eth->ftp_file_size;
    c->cmd_len = eth->ftp_cmd_len;
    memcpy(c->cmd_data, eth->ftp_cmd_data, sizeof c->cmd_data);
}

static void eth_ftp_ctx_activate(dorado_ethernet *eth, int slot)
{
    const struct dorado_ftp_ctx *c;
    if (slot < 0 || slot >= DORADO_FTP_MAX_CONN) return;
    if (eth->ftp_ctx_valid && eth->ftp_ctx_cur == (uint8_t)slot) return;
    eth_ftp_ctx_store(eth);
    c = &eth->ftp_ctx[slot];
    eth->ftp_ctx_cur = (uint8_t)slot;
    eth->ftp_ctx_valid = 1;
    if (!c->used) return;      /* fresh slot: the RFC fills the working set */
    eth->ftp_open = c->open;
    eth->ftp_pending_ack = c->pending_ack;
    eth->ftp_waiting_for_ack = c->waiting_for_ack;
    eth->ftp_tx_mode = c->tx_mode;
    eth->ftp_tx_step = c->tx_step;
    eth->ftp_phase = c->phase;
    eth->ftp_cmd_mark = c->cmd_mark;
    eth->ftp_tx_in_flight = c->tx_in_flight;
    eth->ftp_conn_hi = c->conn_hi;
    eth->ftp_conn_lo = c->conn_lo;
    eth->ftp_client_net_host = c->client_net_host;
    eth->ftp_client_sock_hi = c->client_sock_hi;
    eth->ftp_client_sock_lo = c->client_sock_lo;
    eth->ftp_server_net_host = c->server_net_host;
    eth->ftp_server_sock_hi = c->server_sock_hi;
    eth->ftp_server_sock_lo = c->server_sock_lo;
    eth->ftp_client_bytes_per_pup = c->client_bytes_per_pup;
    eth->ftp_client_pup_alloc = c->client_pup_alloc;
    eth->ftp_client_byte_alloc = c->client_byte_alloc;
    eth->ftp_rx_next = c->rx_next;
    eth->ftp_tx_next = c->tx_next;
    eth->ftp_tx_last_end = c->tx_last_end;
    eth->ftp_last_ack = c->last_ack;
    eth->ftp_file_pos = c->file_pos;
    eth->ftp_file_size = c->file_size;
    eth->ftp_cmd_len = c->cmd_len;
    memcpy(eth->ftp_cmd_data, c->cmd_data, sizeof eth->ftp_cmd_data);
}

/* Slot whose client socket matches (net_host, sock_hi, sock_lo), or -1. */
static int eth_ftp_ctx_find(const dorado_ethernet *eth, uint16_t net_host,
                            uint16_t sock_hi, uint16_t sock_lo)
{
    for (int i = 0; i < DORADO_FTP_MAX_CONN; i++) {
        const struct dorado_ftp_ctx *c = &eth->ftp_ctx[i];
        /* The slot currently in the working set is authoritative there. */
        if (eth->ftp_ctx_valid && eth->ftp_ctx_cur == (uint8_t)i) {
            if (eth->ftp_client_net_host == net_host &&
                eth->ftp_client_sock_hi == sock_hi &&
                eth->ftp_client_sock_lo == sock_lo)
                return i;
            continue;
        }
        if (c->used && c->client_net_host == net_host &&
            c->client_sock_hi == sock_hi && c->client_sock_lo == sock_lo)
            return i;
    }
    return -1;
}

/* Pick a slot for a new RFC: reuse the same client socket, else a free
 * slot, else a slot whose connection has ended (RTP End/Abort cleared its
 * open flag), else the oldest connection that is not the working set.
 * Evicting a live connection strands its client -- the guest retransmits
 * into FTP_UNSERVED forever and the install hangs -- so closed slots must
 * be recycled first. */
static int eth_ftp_ctx_alloc(dorado_ethernet *eth, uint16_t net_host,
                             uint16_t sock_hi, uint16_t sock_lo)
{
    int slot = eth_ftp_ctx_find(eth, net_host, sock_hi, sock_lo);
    if (slot >= 0) return slot;
    for (int i = 0; i < DORADO_FTP_MAX_CONN; i++) {
        if (eth->ftp_ctx[i].used) continue;
        if (eth->ftp_ctx_valid && eth->ftp_ctx_cur == (uint8_t)i) continue;
        return i;
    }
    for (int i = 0; i < DORADO_FTP_MAX_CONN; i++) {
        if (eth->ftp_ctx[i].open) continue;
        if (eth->ftp_ctx_valid && eth->ftp_ctx_cur == (uint8_t)i) continue;
        return i;
    }
    for (int i = 0; i < DORADO_FTP_MAX_CONN; i++)
        if (!eth->ftp_ctx_valid || eth->ftp_ctx_cur != (uint8_t)i) return i;
    return 0;
}

static int eth_ftp_handle_rfc(dorado_ethernet *eth, uint16_t server_socket)
{
    uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
    uint16_t rfc_net_host = eth->tx_count >= 15
        ? eth->tx_words[12] : eth->tx_words[9];
    uint16_t rfc_sock_hi = eth->tx_count >= 15
        ? eth->tx_words[13] : eth->tx_words[10];
    uint16_t rfc_sock_lo = eth->tx_count >= 15
        ? eth->tx_words[14] : eth->tx_words[11];
    if ((rfc_net_host & 0377) == 0)
        rfc_net_host = (uint16_t)((DORADO_PUP_LOCAL_NET << 8) | req_src_host);
    /* Give this connection its own slot before initializing the working set,
     * so an existing connection's state is preserved rather than clobbered. */
    eth_ftp_ctx_activate(eth,
                         eth_ftp_ctx_alloc(eth, rfc_net_host, rfc_sock_hi,
                                           rfc_sock_lo));
    eth->ftp_conn_hi = eth->tx_words[4];
    eth->ftp_conn_lo = eth->tx_words[5];
    eth->ftp_client_net_host = eth->tx_count >= 15
        ? eth->tx_words[12]
        : eth->tx_words[9];
    eth->ftp_client_sock_hi = eth->tx_count >= 15
        ? eth->tx_words[13]
        : eth->tx_words[10];
    eth->ftp_client_sock_lo = eth->tx_count >= 15
        ? eth->tx_words[14]
        : eth->tx_words[11];
    if ((eth->ftp_client_net_host & 0377) == 0)
        eth->ftp_client_net_host =
            (uint16_t)((DORADO_PUP_LOCAL_NET << 8) | req_src_host);
    eth->ftp_server_net_host =
        (uint16_t)((DORADO_PUP_LOCAL_NET << 8) | eth->remote_host);
    eth->ftp_server_sock_hi = 0;
    eth->ftp_server_sock_lo = server_socket;
    eth->ftp_rx_next = pup_id32(eth->ftp_conn_hi, eth->ftp_conn_lo);
    eth->ftp_tx_next = eth->ftp_rx_next;
    eth->ftp_tx_last_end = eth->ftp_tx_next;
    eth->ftp_last_ack = eth->ftp_rx_next;
    eth->ftp_tx_in_flight = 0;
    eth->ftp_pending_ack = 1;       /* Initial BSP allocation for client. */
    eth->ftp_waiting_for_ack = 0;
    eth->ftp_tx_mode = FTP_TX_NONE;
    eth->ftp_tx_step = 0;
    eth->ftp_phase = server_socket == PUP_SOCKET_GV_RS_ENQUIRY
        ? FTP_PHASE_GV_WAIT : FTP_PHASE_IDLE;
    eth->ftp_open = 1;
    eth->ftp_cmd_mark = 0;
    eth->ftp_cmd_len = 0;
    eth_ftp_ctx_store(eth);         /* the slot now owns this connection */
    if (ftp_trace()) {
        fprintf(stderr,
                "FTP_RFC conn=%08x client=%06o/%o/%o server=%06o/%o/%o slot=%u\n",
                eth->ftp_rx_next, eth->ftp_client_net_host,
                eth->ftp_client_sock_hi, eth->ftp_client_sock_lo,
                eth->ftp_server_net_host, eth->ftp_server_sock_hi,
                eth->ftp_server_sock_lo, eth->ftp_ctx_cur);
    }
    return eth_ftp_queue_rfc_reply(eth);
}

static int eth_queue_netdir_reply(dorado_ethernet *eth, uint16_t server_socket)
{
    uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
    uint8_t body[6];
    /* The in-process server is the explicit host on the synthetic local
     * segment.  PupName validates a NameReply with PupHop, so callers must
     * receive a GatewayInfo announcement (above, and again after Loader's
     * rollback) before this net-1 address can be used. */
    uint16_t port0 =
        (uint16_t)((DORADO_PUP_LOCAL_NET << 8) | eth->remote_host);
    body[0] = (uint8_t)(port0 >> 8);
    body[1] = (uint8_t)(port0 & 0xFF);
    body[2] = 0;
    body[3] = 0;
    body[4] = 0;
    body[5] = (uint8_t)server_socket;
    return eth_queue_pup_bytes(eth, DORADO_PUP_TYPE_NETDIR_REPLY,
                               pup_id32(eth->tx_words[4], eth->tx_words[5]),
                               (uint16_t)((DORADO_PUP_LOCAL_NET << 8) |
                                          req_src_host),
                               eth->tx_words[10], eth->tx_words[11],
                               port0, eth->tx_words[7], eth->tx_words[8],
                               body, sizeof body);
}

/* PupRouterImpl learns routes from GatewayInfo (201B) at socket 2.  A
 * standalone Cedar image has no physical gateway to announce its directly
 * attached release server, so this one-entry announcement says net 1 is one
 * hop through the in-process server. */
static int eth_queue_gateway_info(dorado_ethernet *eth)
{
    uint8_t body[] = { DORADO_PUP_LOCAL_NET, DORADO_PUP_LOCAL_NET,
                       eth->remote_host, 0 };
    uint16_t req_src_host = (uint16_t)(eth->tx_words[0] & 0377);
    return eth_queue_pup_bytes(eth, DORADO_PUP_TYPE_GATEWAY_REPLY,
                               pup_id32(eth->tx_words[4], eth->tx_words[5]),
                               (uint16_t)((DORADO_PUP_LOCAL_NET << 8) |
                                          req_src_host),
                               0, 02,
                               (uint16_t)((DORADO_PUP_LOCAL_NET << 8) |
                                          eth->remote_host),
                               0, 02, body, sizeof body);
}

/* A NetDir lookup carries the requested service/host name as raw Pup bytes. */
static int eth_netdir_requests_name(const dorado_ethernet *eth,
                                    const char *wanted)
{
    size_t nbytes = eth->tx_words[2] > 026 ?
        (size_t)(eth->tx_words[2] - 026) : 0;
    size_t wn = strlen(wanted);
    if (wn == 0 || nbytes < wn) return 0;
    for (size_t i = 0; i + wn <= nbytes; i++) {
        size_t j = 0;
        for (; j < wn; j++) {
            uint16_t w = eth->tx_words[12 + (i + j) / 2];
            unsigned char c = (unsigned char)(((i + j) & 1) ?
                                               (w & 0xFF) : (w >> 8));
            if (tolower(c) != tolower((unsigned char)wanted[j])) break;
        }
        if (j == wn) return 1;
    }
    return 0;
}

/* Keep the exact lookup token in the trace.  LoaderDriver's post-Loadees
 * request is the boundary between a successful command-file transfer and a
 * failed BCD lookup, and the raw token tells us whether it is a host-name
 * query or a differently encoded NetDir operation. */
static void eth_trace_netdir_request(const dorado_ethernet *eth)
{
    if (!ftp_trace()) return;
    size_t nbytes = eth->tx_words[2] > 026 ?
        (size_t)(eth->tx_words[2] - 026) : 0;
    if (nbytes > 96) nbytes = 96;
    fprintf(stderr, "FTP_NETDIR request id=%04o%04o src=%06o/%o/%o text=\"",
            eth->tx_words[4], eth->tx_words[5], eth->tx_words[9],
            eth->tx_words[10], eth->tx_words[11]);
    for (size_t i = 0; i < nbytes; i++) {
        uint16_t w = eth->tx_words[12 + i / 2];
        unsigned char c = (unsigned char)((i & 1) ? (w & 0xFF) : (w >> 8));
        fputc(isprint(c) ? c : '.', stderr);
    }
    fprintf(stderr, "\"\n");
}

/* Select the BSP connection this packet belongs to (by its source socket)
 * and make it the working set.  Returns 0 if the packet is not for the STP
 * server or names no connection we have open.
 *
 * The source socket identifies the connection, and each connection has its
 * own byte-ID space (PupBSPProt seeds it from the RFC connection ID), so a
 * packet must never be ingested against another connection's cursor: doing
 * that let LoaderDriver's abandoned-but-still-retransmitting connection
 * advance ftp_rx_next past the Installer's live one, whose Retrieve and Yes
 * then looked like duplicates and were dropped. */
static int eth_ftp_select_conn(dorado_ethernet *eth)
{
    int slot;
    if (!eth->ftp_enabled) return 0;
    slot = eth_ftp_ctx_find(eth, eth->tx_words[9], eth->tx_words[10],
                            eth->tx_words[11]);
    if (slot < 0) return 0;
    eth_ftp_ctx_activate(eth, slot);
    if (!eth->ftp_open) return 0;
    /* The slot's own server socket, now loaded, is what this packet must be
     * addressed to (STP lives on socket 3, the Grapevine shim on 050). */
    return eth->tx_words[6] == eth->ftp_server_net_host &&
           eth->tx_words[7] == eth->ftp_server_sock_hi &&
           eth->tx_words[8] == eth->ftp_server_sock_lo;
}

static int eth_ftp_handle_packet(dorado_ethernet *eth)
{
    uint16_t type = eth->tx_words[3];
    eth->ftp_packets_seen++;
    if (ftp_trace()) {
        fprintf(stderr,
                "FTP_IN type=0o%o id=%08x len=%u rx_next=%08x "
                "tx_next=%08x wait=%u mode=%u\n",
                type, pup_id32(eth->tx_words[4], eth->tx_words[5]),
                eth->tx_words[2], eth->ftp_rx_next, eth->ftp_tx_next,
                eth->ftp_waiting_for_ack, eth->ftp_tx_mode);
    }
    switch (type) {
    case PUP_TYPE_BSP_ACK: {
        uint32_t ack = pup_id32(eth->tx_words[4], eth->tx_words[5]);
        uint32_t previous_ack = eth->ftp_last_ack;
        if (eth->tx_count >= 15) {
            eth->ftp_client_bytes_per_pup = eth->tx_words[12];
            eth->ftp_client_pup_alloc = eth->tx_words[13];
            eth->ftp_client_byte_alloc = eth->tx_words[14];
            if (ftp_trace()) {
                fprintf(stderr, "FTP_ACK ack=%08x alloc=%u*%u bytes\n",
                        ack, eth->ftp_client_pup_alloc,
                        eth->ftp_client_bytes_per_pup);
            }
        }
        if (ack != previous_ack) {
            /* Acknowledge all fingers up to this byte position.  Keeping
             * the one later packet (if any) would be more exact, but this
             * server has no retransmit ring; resetting the accounting is
             * the safe approximation and matches PupStream's unblocking
             * behavior at the acknowledged half-window. */
            eth->ftp_waiting_for_ack = 0;
            eth->ftp_tx_in_flight = 0;
        }
        eth->ftp_last_ack = ack;
        eth_ftp_maybe_deliver(eth);
        return 1;
    }
    case PUP_TYPE_BSP_DATA:
    case PUP_TYPE_BSP_ADATA:
    case PUP_TYPE_BSP_MARK:
    case PUP_TYPE_BSP_AMARK:
        eth_ftp_ingest_payload(eth);
        eth_ftp_maybe_deliver(eth);
        return 1;
    case PUP_TYPE_RTP_END:
        (void)eth_queue_pup_bytes(eth, PUP_TYPE_RTP_END_REPLY,
                                  pup_id32(eth->tx_words[4], eth->tx_words[5]),
                                  eth->tx_words[9], eth->tx_words[10],
                                  eth->tx_words[11],
                                  eth->ftp_server_net_host,
                                  eth->ftp_server_sock_hi,
                                  eth->ftp_server_sock_lo,
                                  NULL, 0);
        eth->ftp_open = 0;
        return 1;
    case PUP_TYPE_RTP_ABORT:
        if (ftp_trace()) {
            size_t nbytes = eth->tx_words[2] > 026
                ? (size_t)(eth->tx_words[2] - 026) : 0;
            fprintf(stderr, "FTP_ABORT code=%06o text=\"",
                    nbytes >= 2 ? eth->tx_words[12] : 0);
            for (size_t i = 2; i < nbytes; i++) {
                uint16_t w = eth->tx_words[12 + i / 2];
                uint8_t c = (uint8_t)((i & 1) ? (w & 0xFF) : (w >> 8));
                fputc(c >= 040 && c < 0177 ? c : '.', stderr);
            }
            fprintf(stderr, "\"\n");
        }
        eth->ftp_open = 0;
        return 1;
    default:
        return 0;
    }
}

/* GVLocate probes the registry with an EchoMe Pup before it opens the BSP
 * stream.  Echo the packet body back from the requested poll socket, as the
 * Pup specification's iAmEcho service requires. */
static int eth_queue_gv_echo_reply(dorado_ethernet *eth)
{
    uint8_t body[512];
    size_t nbytes = eth->tx_words[2] > 026 ?
        (size_t)(eth->tx_words[2] - 026) : 0;
    if (nbytes > sizeof body) return 0;
    for (size_t i = 0; i < nbytes; i++) {
        uint16_t w = eth->tx_words[12 + i / 2];
        body[i] = (uint8_t)((i & 1) ? (w & 0xFF) : (w >> 8));
    }
    return eth_queue_pup_bytes(eth, 02 /* iAmEcho */,
                               pup_id32(eth->tx_words[4], eth->tx_words[5]),
                               eth->tx_words[9], eth->tx_words[10],
                               eth->tx_words[11],
                               (uint16_t)((DORADO_PUP_LOCAL_NET << 8) |
                                          eth->remote_host),
                               eth->tx_words[7], eth->tx_words[8],
                               body, nbytes);
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

    if (eth->ftp_enabled && eth->tx_words[1] == DORADO_PUP_TYPE_ETHERNET) {
        if (eth->tx_words[3] == DORADO_PUP_TYPE_NETDIR_LOOKUP) {
            static int route_primed;
            static int cyan_route_refreshed;
            int cedar = eth_netdir_requests_name(eth, "Cedar");
            int gv = eth_netdir_requests_name(eth, "GrapevineRServer");
            int cyan = eth_netdir_requests_name(eth, "Cyan");
            eth_trace_netdir_request(eth);
            if (!route_primed) {
                route_primed = 1;
                if (ftp_trace()) fprintf(stderr, "FTP_NETDIR prime route\n");
                (void)eth_queue_gateway_info(eth);
                return;
            }
            /* LoaderDriver's Basic boot rolls Cedar's network environment
             * back before it resolves the release host Cyan.  PupRouterImpl
             * Rollback explicitly discards non-direct routes, so a NameReply
             * alone is unusable: PupName rejects it with noRoute before it
             * can issue the STP RFC.  Re-announce the directly attached
             * release segment first; PupName retries its query and receives
             * the ordinary address response below. */
            if (cyan && !cyan_route_refreshed) {
                cyan_route_refreshed = 1;
                if (ftp_trace())
                    fprintf(stderr, "FTP_NETDIR refresh route for Cyan\n");
                (void)eth_queue_gateway_info(eth);
                return;
            }
            if (ftp_trace()) fprintf(stderr, "FTP_NETDIR lookup %s\n",
                                     cedar ? "Cedar -> STP" :
                                     gv ? "GrapevineRServer -> GV" : "release host -> STP");
            if (gv)
                (void)eth_queue_netdir_reply(eth, PUP_SOCKET_GV_RS_ENQUIRY);
            else
                /* PupName can fragment its short name query across several
                 * lookup Pups.  Once the registry name above is handled,
                 * the standalone release server is intentionally the answer
                 * for every remaining host lookup (first User/Guest, then
                 * Cedar), letting STP return fileNotFound where appropriate. */
                (void)eth_queue_netdir_reply(eth, PUP_SOCKET_FTP);
            return;
        }
        if (eth->tx_words[3] == 01 /* echoMe */ &&
            eth->tx_words[8] == PUP_SOCKET_GV_RS_POLL) {
            (void)eth_queue_gv_echo_reply(eth);
            return;
        }
        if (eth->tx_words[3] == PUP_TYPE_RTP_RFC &&
            (eth->tx_words[8] == PUP_SOCKET_FTP ||
             eth->tx_words[8] == PUP_SOCKET_GV_RS_ENQUIRY)) {
            (void)eth_ftp_handle_rfc(eth, eth->tx_words[8]);
            return;
        }
        if (eth_ftp_select_conn(eth) && eth_ftp_handle_packet(eth))
            return;
        /* Anything else the guest sends that we never answer: a Pup aimed at
         * a host/socket this shim does not implement (Cedar reaching for a
         * fonts or file server), or a live connection's packet rejected by
         * the client-socket filter.  Cedar blocks on those forever, so make
         * them visible instead of silently dropping them. */
        if (ftp_trace() && eth->ftp_enabled) {
            fprintf(stderr,
                    "FTP_UNSERVED type=0o%o id=%08x d=%06o/%o/%o s=%06o/%o/%o\n",
                    eth->tx_words[3],
                    pup_id32(eth->tx_words[4], eth->tx_words[5]),
                    eth->tx_words[6], eth->tx_words[7], eth->tx_words[8],
                    eth->tx_words[9], eth->tx_words[10], eth->tx_words[11]);
        }
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
    if (eth->rx_pos == eth->rx_count && eth->ftp_enabled && eth->ftp_open &&
        !eth_wire_model()) {
        if (ftp_trace() && eth->rx_count >= 6) {
            fprintf(stderr, "FTP_RX_CONSUMED type=0o%o id=%08x words=%zu\n",
                    eth->rx_words[3], pup_id32(eth->rx_words[4],
                                                eth->rx_words[5]),
                    eth->rx_count);
        }
        /* Let the EIT tail execute its source-defined WaitForBOP before the
         * fake server makes the next BSP segment visible. */
        eth->rx_hold = DORADO_ETH_STP_INTERPACKET_TICKS;
    }
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
            if (!on && eth->eftp_wait_for_rx_arm && !eth->ftp_enabled) {
                /* HM §11: clearing RxOn resets the receiver; no more
                 * wakeups are generated and queued/current words are
                 * discarded until the receiver is re-armed at a packet
                 * boundary. This packet-level model has no wire FIFO, so
                 * discard the queued packet stream immediately.
                 *
                 * Only for the Cedar/Pilot germ before its STP server is
                 * active (eftp_wait_for_rx_arm && !ftp_enabled), whose
                 * IOCB-gated delivery re-arms per input buffer. Once Cedar
                 * switches to Pup STP, its receive task can toggle RxOn
                 * between packets; clearing the one-packet bridge then
                 * loses a queued HereIsPList/HereIsFile reply. The Alto
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
    if (!eth->rx_hold && eth->rx_pos >= eth->rx_count)
        eth_ftp_maybe_deliver(eth);
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
