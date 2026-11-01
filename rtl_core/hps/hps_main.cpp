// =============================================================================
// hps_main.cpp — dorado-hps: the Cedar host services on the MiSTer's ARM
// =============================================================================
// Load the Cedar checkpoint core (fpga/mister_rtl, CORE=cedar) first; it waits
// for this program to fill storage and say go. Then:
//
//   dorado-hps [-d DIR] [-f FTP_ROOT] [-q QUANTUM] [-p GERM_POLL] [-s SECONDS]
//              [-c X,Y] [-t TEXT]
//
// DIR (default /media/fat/dorado) holds the same files the C emulator uses,
// at the same relative paths: dorado/build/good-packs/cedar-desktop.{snap,pdi}
// (the snapshot the core was built from) and chm/cedar/stp-root,
// chm/dorado/CedarDorado.eb!6, chm/cedar/germ-alt/Dorado.germ-6.1.6, and the
// default microcode the machine opens at creation. FTP_ROOT is the Pup/STP
// server's tree (default DIR/chm/cedar/stp-root); on the MiSTer it lives in
// /tmp, unpacked from a tarball, because 5,000 small files cost the SD
// card's 128 KB FAT clusters 600 MB.
// -c clicks the left button at X,Y five seconds in, and -t types TEXT ten
// seconds later through the paced typing queue (\n is Return); with either,
// the keyboard and mouse are scripted instead of taken from the core.
// Every SECONDS (default 10) a status line is printed and the screen is
// rendered to /tmp/dorado.pgm (also on SIGUSR1).
// =============================================================================
#include "hps_host.h"

extern "C" {
#include "machine.h"
#include "memory.h"
#include "typetext.h"
}

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

static const off_t  DDR_PHYS   = 0x30000000;     // dorado_rtl_storage_ddr DDR_BASE
static const size_t DDR_LEN    = 0x00810000;     // storage 8 MB + the mailbox
static const size_t MB_OFFSET  = 0x00800000;     // dorado_rtl_mailbox MB_BASE

static volatile sig_atomic_t want_pgm = 0;
static void on_usr1(int) { want_pgm = 1; }
static void wait_fn(void *) { /* spin: a mailbox round trip is microseconds */ }

static double now_s() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    const char *dir = "/media/fat/dorado";
    const char *ftp_root = "../chm/cedar/stp-root";
    unsigned quantum = 64;
    unsigned long long germ_poll = 37000;      // ~10,000 microinstructions
    double every = 10;
    int click_x = -1, click_y = -1;
    std::string text;
    int c;
    while ((c = getopt(argc, argv, "d:f:q:p:s:c:t:")) != -1) {
        switch (c) {
        case 'd': dir = optarg; break;
        case 'f': ftp_root = optarg; break;
        case 'q': quantum = (unsigned)atoi(optarg); break;
        case 'p': germ_poll = strtoull(optarg, nullptr, 0); break;
        case 's': every = atof(optarg); break;
        case 'c': sscanf(optarg, "%d,%d", &click_x, &click_y); break;
        case 't':
            for (const char *q = optarg; *q; q++) {
                if (q[0] == '\\' && q[1] == 'n') { text += '\n'; q++; }
                else text += *q;
            }
            break;
        default:
            fprintf(stderr, "usage: %s [-d DIR] [-f FTP_ROOT] [-q QUANTUM] [-p GERM_POLL] [-s SECONDS]\n", argv[0]);
            return 2;
        }
    }
    setvbuf(stdout, nullptr, _IOLBF, 0);
    setenv("DORADO_PDI_IGNORE_LABEL_FLAGS", "1", 1);
    std::string ddir = std::string(dir) + "/dorado";
    if (chdir(ddir.c_str()) != 0) { perror(ddir.c_str()); return 2; }

    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) { perror("/dev/mem"); return 1; }
    void *p = mmap(nullptr, DDR_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, DDR_PHYS);
    if (p == MAP_FAILED) { perror("mmap"); return 1; }

    hps_transport t;
    t.mb = (volatile uint64_t *)((char *)p + MB_OFFSET);
    t.storage = (volatile uint16_t *)p;
    t.storage_words = 4u << 20;
    t.wait = wait_fn;
    t.ctx = nullptr;
    hps_config cfg;
    cfg.snapshot = "build/good-packs/cedar-desktop.snap";
    cfg.pdi = "build/good-packs/cedar-desktop.pdi";
    cfg.ftp_root = ftp_root;
    cfg.eb = "../chm/dorado/CedarDorado.eb!6";
    cfg.germ = "../chm/cedar/germ-alt/Dorado.germ-6.1.6";
    cfg.quantum = quantum;
    cfg.germ_poll = germ_poll;
    const bool scripted = click_x >= 0 || !text.empty();
    cfg.keyboard = scripted ? 0 : 1;
    if (hps_start(&cfg, &t) != 0) return 1;
    signal(SIGUSR1, on_usr1);

    const hps_stats *S = hps_get_stats();
    double t0 = now_s(), tl = t0;
    uint64_t c0 = S->rtl_cycles, cl = c0;
    uint64_t kl = S->rtl_clocks;
    dorado_machine *M = (dorado_machine *)hps_machine();
    static dorado_typequeue tq;
    int click = 0;
    bool typed = false;
    double tclick = 0;
    uint64_t next_frame = 0;
    for (;;) {
        hps_poll();
        double tn = now_s();
        uint64_t cyc = dorado_machine_cycles(M);
        if (click_x >= 0) {
            if (click == 0 && tn - t0 >= 5) {
                dorado_machine_set_mouse(M, click_x, click_y, 0);
                click = 1; tclick = tn;
            } else if (click == 1 && tn - tclick >= 0.3) {
                dorado_machine_set_mouse(M, click_x, click_y, DORADO_MOUSE_LEFT);
                click = 2; tclick = tn;
            } else if (click == 2 && tn - tclick >= 0.3) {
                dorado_machine_set_mouse(M, click_x, click_y, 0);
                click = 3;
                printf("[hps] clicked (%d,%d)\n", click_x, click_y);
            }
        }
        if (!text.empty() && !typed && tn - t0 >= 15) {
            typed = true;
            dorado_typequeue_start(&tq, text.c_str(), 800000ull, cyc);
            printf("[hps] typing %zu characters\n", text.size());
        }
        // once per 2M-cycle frame, as dorado.c pumps it (more often loses keys)
        if (typed && cyc >= next_frame) {
            next_frame = cyc + 2000000ull;
            dorado_typequeue_pump(&tq, M);
        }
        if (tn - tl >= every || want_pgm) {
            double mips = (S->rtl_cycles - cl) / (tn - tl) / 1e6;
            double cpi  = (double)(S->rtl_clocks - kl) / (double)(S->rtl_cycles > cl ? S->rtl_cycles - cl : 1);
            kl = S->rtl_clocks;
            printf("[hps] %.0f s: %llu microinstructions (%.2f M/s = %.2fx a Dorado, %.2f clocks each); "
                   "%llu ops, %llu reads, %llu packets in, %llu out\n",
                   tn - t0, (unsigned long long)(S->rtl_cycles - c0), mips, mips / 16.67, cpi,
                   (unsigned long long)S->ops, (unsigned long long)S->reads,
                   (unsigned long long)S->rx_packets, (unsigned long long)S->tx_packets);
            printf("[hps] host loop: %.1f ms of guest time behind the FPGA\n",
                   S->host_target > S->host_bb_cycles
                       ? (S->host_target - S->host_bb_cycles) / 3.7 * 60e-6 : 0.0);
            {
                dorado_memory *mem = dorado_machine_memory(M);
                printf("[hps] KeyBits 177033..37:");
                for (unsigned a = 0177033; a <= 0177037; a++)
                    printf(" %06o", dorado_visible_word_at_va(mem, a));
                printf("  mouse 424/425: %d %d  cursor 426/427: %d %d\n",
                       (short)dorado_visible_word_at_va(mem, 0424), (short)dorado_visible_word_at_va(mem, 0425),
                       (short)dorado_visible_word_at_va(mem, 0426), (short)dorado_visible_word_at_va(mem, 0427));
            }
            int px = hps_snapshot_pgm("/tmp/dorado.pgm");
            printf("[hps] screen: %d pixels -> /tmp/dorado.pgm\n", px);
            tl = now_s();
            cl = S->rtl_cycles;
            want_pgm = 0;
        } else {
            usleep(200);
        }
    }
}
