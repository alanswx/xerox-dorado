/* ARM-local Pup service. Main will carry raw Ethernet packets over this
 * local datagram socket; the FPGA owns Manchester framing, CRC and device
 * status. There is no CPU emulator, guest memory or SPI access here. */
#include "ethernet.h"
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

/* Logging symbols shared by the existing server/controller source. In this
 * process the trace counter counts server pump calls, never CPU cycles. */
int dorado_trace_gate = 1, dorado_trace_env_present = 1;
unsigned long long dorado_trace_cycle;
int dorado_trace_flag_lookup(const char *name) { return getenv(name) != NULL; }
void dorado_trace_init(void) {}
static volatile sig_atomic_t stopping;
static void stop(int sig) { (void)sig; stopping = 1; }
static int peer_is_gone(const struct sockaddr_un *peer,socklen_t length) {
    // A core-load re-exec leaves the old pathname behind. Probe without
    // sending data to a live client or changing the server's own socket.
    int probe=socket(AF_UNIX,SOCK_DGRAM,0);
    if(probe<0)return 0;
    int rc=connect(probe,(const struct sockaddr *)peer,length),error=errno;
    close(probe);
    return rc<0 && (error==ENOENT || error==ECONNREFUSED);
}
static uint64_t monotonic_ns(void) {
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t)) { perror("clock_gettime"); exit(1); }
    return (uint64_t)t.tv_sec * 1000000000ull + t.tv_nsec;
}
static uint32_t elapse(uint32_t timer, uint64_t ticks) {
    return timer > ticks ? timer - ticks : 0;
}
static void pump(dorado_ethernet *eth, uint64_t ticks) {
    /* The emulator's server timers are in ~32 ns poll units. Advance idle
     * time arithmetically; do not burn millions of ARM calls per second.
     * Leave the last tick to wakeup_mask so EFTP expiry queues a resend. */
    if (ticks > 1) {
        eth->rx_hold = elapse(eth->rx_hold, ticks - 1);
        if (eth->eftp_resend_timer && eth->rx_pos >= eth->rx_count)
            eth->eftp_resend_timer = eth->eftp_resend_timer > ticks
                ? eth->eftp_resend_timer - (ticks - 1) : 1;
    }
    dorado_trace_cycle++;
    (void)dorado_ethernet_wakeup_mask(eth);
}
static void configure(dorado_ethernet *eth,int argc,char **argv) {
    dorado_ethernet_init(eth);
    dorado_ethernet_set_boot_file(eth,0110,argv[2]);
    dorado_ethernet_set_eftp_boot_file(eth,argv[3]);
    if(argc==5)dorado_ethernet_set_ftp_root(eth,argv[4]);
    eth->rx_on=1;
}
int main(int argc, char **argv) {
    if (argc < 4 || argc > 5) {
        fprintf(stderr, "usage: %s SOCKET AEMU.eb ALTO.boot [STP_ROOT]\n", argv[0]);
        return 2;
    }
    struct sockaddr_un local = {0}, peer = {0};
    local.sun_family = AF_UNIX;
    if (strlen(argv[1]) >= sizeof local.sun_path) {
        fprintf(stderr, "socket path too long\n"); return 2;
    }
    strcpy(local.sun_path, argv[1]);
    for (int i = 2; i <= 3; i++) if (access(argv[i], R_OK)) {
        perror(argv[i]); return 2;
    }
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }
    umask(0077);
    /* Refuse to replace another running service or silently steal its path. */
    if (bind(fd, (struct sockaddr *)&local, sizeof local)) {
        perror("bind"); close(fd); return 1;
    }
    signal(SIGINT, stop); signal(SIGTERM, stop);
    dorado_ethernet eth;
    configure(&eth,argc,argv);
    socklen_t peer_len = 0;
    uint64_t previous = monotonic_ns(), broadcast_at = previous + 1000000000ull;
    uint64_t probe_at = previous + 1000000000ull;
    unsigned probe_cursor = 0;
    unsigned broadcast_kind = 0;
    int result = 0;
    fprintf(stderr, "Dorado Pup server listening on %s (raw frames, no CRC/status)\n", argv[1]);
    while (!stopping) {
        uint64_t now = monotonic_ns();
        pump(&eth, (now - previous) / 32);
        previous = now;
        if (peer_len && now >= probe_at) {
            /* One idle STP probe per second, rotating through connections.
             * This is the BSP sender's timer, independent of guest CPU time.
             * Main continues to own SPI and the FPGA the real wire/CRC. */
            for (unsigned n = 0; n < DORADO_FTP_MAX_CONN; n++) {
                unsigned slot = probe_cursor++ % DORADO_FTP_MAX_CONN;
                if (dorado_ethernet_stp_probe(&eth, slot)) break;
            }
            probe_at = now + 1000000000ull;
        }
        if (peer_len && now >= broadcast_at) {
            if (broadcast_kind++ & 1) (void)dorado_ethernet_time_broadcast(&eth);
            else (void)dorado_ethernet_breath_of_life(&eth);
            broadcast_at = now + 1000000000ull;
        }
        /* Queue entries include the C controller's placeholder CRC and
         * attention/status. Strip BOTH before crossing the packet boundary.
         * The real FPGA receiver must generate/check its own wire status. */
        if (peer_len && !eth.rx_hold && eth.rx_pos < eth.rx_count) {
            size_t end = eth.rx_pos;
            while (end < eth.rx_count && !eth.rx_attention[end]) end++;
            if (end == eth.rx_count || end < eth.rx_pos + 3 || eth.rx_words[end]) {
                fprintf(stderr, "invalid internal server packet boundary\n"); result = 1; break;
            }
            size_t count = end - eth.rx_pos - 1;
            unsigned char bytes[4096];
            if (count >= sizeof bytes / 2) { result = 1; break; }
            for (size_t i = 0; i < count; i++) {
                uint16_t w = eth.rx_words[eth.rx_pos + i];
                bytes[2*i] = w >> 8; bytes[2*i+1] = w;
            }
            /* The C microcode-boot queue predates its general Pup server
             * and omits the Pup checksum, despite including it in length.
             * Supply the legal nil checksum at the real packet boundary. */
            if (count >= 12 && eth.rx_words[eth.rx_pos+1] == DORADO_PUP_TYPE_ETHERNET &&
                eth.rx_words[eth.rx_pos+3] == DORADO_PUP_TYPE_MICROCODE_BOOT_REPLY &&
                eth.rx_words[eth.rx_pos+2] == count*2-2) {
                bytes[2*count] = 255; bytes[2*count+1] = 255; count++;
            }
            ssize_t n = sendto(fd, bytes, count*2, MSG_DONTWAIT,
                              (struct sockaddr *)&peer, peer_len);
            if (n == (ssize_t)(count*2)) eth.rx_pos = end + 1;
            else if(n<0 && (errno==ENOENT || errno==ECONNREFUSED)) {
                // Main changed/reloaded a core. Drop the disconnected
                // session and wait for its next explicit registration.
                dorado_ethernet_free(&eth);configure(&eth,argc,argv);peer_len=0;
            } else if (n < 0 && errno != EAGAIN && errno != ENOBUFS && errno != EINTR) {
                perror("sendto"); result = 1; break;
            }
        }
        /* Drain a queued microcode stream before accepting another request;
         * a new request is allowed to replace that stream in the C model. */
        struct pollfd pfd = {fd, eth.rx_pos >= eth.rx_count ? POLLIN : 0, 0};
        if (poll(&pfd, 1, 1) < 0) {
            if (errno == EINTR) continue;
            perror("poll"); result = 1; break;
        }
        if (pfd.revents & POLLIN) {
            unsigned char bytes[2049];
            struct sockaddr_un from = {0}; socklen_t len = sizeof from;
            ssize_t n = recvfrom(fd, bytes, sizeof bytes, 0, (struct sockaddr *)&from, &len);
            if (n < 0) { if (errno == EINTR) continue; perror("recvfrom"); result = 1; break; }
            if ((n != 0 && n < 4) || n > 2048 || (n & 1) || !from.sun_path[0]) continue;
            if (peer_len && (len != peer_len || memcmp(&from, &peer, len))) {
                // Main can register its new PID before the next periodic
                // broadcast discovers the old socket is dead. Retire that
                // dead session now, otherwise this sole registration is
                // lost and a guest waiting for BOL never sends a request.
                if(n || !peer_is_gone(&peer,peer_len))continue;
                dorado_ethernet_free(&eth);configure(&eth,argc,argv);peer_len=0;
                fprintf(stderr,"Dorado Pup server: retired disconnected Main session\n");
            }
            peer = from; peer_len = len;
            if (!n) continue; /* Main registers its return socket at startup. */
            uint16_t words[1024];
            for (ssize_t i = 0; i < n/2; i++) words[i] = (bytes[2*i] << 8) | bytes[2*i+1];
            eth.local_host = words[0] & 255;
            dorado_ethernet_direct_transmit(&eth, words, n/2);
        }
    }
    dorado_ethernet_free(&eth);
    close(fd); unlink(argv[1]);
    return result;
}
