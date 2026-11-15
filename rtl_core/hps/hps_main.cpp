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
static const size_t DDR_LEN    = 0x01000000;     // storage 8 MB + the mailbox (with its scan and load regions), up to the pack
static const size_t MB_OFFSET  = 0x00800000;     // dorado_rtl_mailbox MB_BASE
static const off_t  PACK_PHYS  = 0x31000000;     // dorado_rtl_top DISK_PACK_BASE (beat 0x0620_0000)
static const size_t PACK_LEN   = 0x04000000;     // 64 MB: a Trident pack's records

static volatile sig_atomic_t want_pgm = 0;
static void on_usr1(int) { want_pgm = 1; }
static volatile sig_atomic_t want_pda = 0;
static void on_usr2(int) { want_pda = 1; }
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
    const char *world = "cedar";
    while ((c = getopt(argc, argv, "d:f:q:p:s:c:t:w:")) != -1) {
        switch (c) {
        case 'd': dir = optarg; break;
        case 'f': ftp_root = optarg; break;
        case 'q': quantum = (unsigned)atoi(optarg); break;
        case 'p': germ_poll = strtoull(optarg, nullptr, 0); break;
        case 'w': world = optarg; break;
        case 's': every = atof(optarg); break;
        case 'c': sscanf(optarg, "%d,%d", &click_x, &click_y); break;
        case 't':
            for (const char *q = optarg; *q; q++) {
                if (q[0] == '\\' && q[1] == 'n') { text += '\n'; q++; }
                else text += *q;
            }
            break;
        default:
            fprintf(stderr, "usage: %s [-w cedar|smalltalk|lisp] [-d DIR] [-f FTP_ROOT] [-q QUANTUM] [-p GERM_POLL] [-s SECONDS]\n", argv[0]);
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
    t.pack = nullptr;
    t.pack_beats = 0;
    t.wait = wait_fn;
    t.ctx = nullptr;
    hps_config cfg;
    cfg.pack = nullptr;
    cfg.storage_image = nullptr;
    const bool lisp_w = !strcmp(world, "lisp");
    // The world's presentation on a scan core (dorado_rtl_mailbox, control
    // beat 1; Dorado.sv): Cedar and Lisp on the full 1024 x 808 screen from
    // line 22, Cedar's mouse through the terminal; Smalltalk-76's 640 x 800
    // bitmap in the LF full-width mode, four lines later, cursor 286 over.
    cfg.world_cfg = !strcmp(world, "smalltalk") ? (1u | 26u << 8 | 286u << 16)
                  : lisp_w                      ? (22u << 8)
                  :                               (2u | 22u << 8);
    if (!strcmp(world, "smalltalk") || lisp_w) {
        // Smalltalk-76 (the CORE=smalltalk build): the XM Smalltalk pack on
        // drive 0, served by the core's own Trident controller from DDR
        void *pp = mmap(nullptr, PACK_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PACK_PHYS);
        if (pp == MAP_FAILED) { perror("mmap pack"); return 1; }
        t.pack = (volatile uint64_t *)pp;
        t.pack_beats = PACK_LEN / 8;
        cfg.snapshot = "build/good-packs/smalltalk76.snap";
        // Smalltalk-76's bitmap is 40 words wide, and the Alto display
        // microcode clips each line to MaxWidthWordsAlto = 38 on an Alto
        // monitor (DisplayMain.mc; the right 32 columns stayed white).
        // Present an LF monitor in full-screen mode instead, as Alto opcode
        // 61032B (SetDisplayConfigA, AC0 = -1) does: DisplayConfig (RM 166B)
        // OR 3, and MaxWidthWords (RM 156B) OR MaxWidthWordsLF with 377 in
        // the left half, which tells DHT to reinitialise at the next field.
        if (!lisp_w) setenv("DORADO_HPS_RM_OR", "166:3,156:177500", 0);
        cfg.pdi = nullptr;
        if (access("build/good-packs/smalltalk76.storage", R_OK) == 0)
            cfg.storage_image = "build/good-packs/smalltalk76.storage";
        cfg.pack = "build/good-packs/smalltalk76.pack";
        cfg.ftp_root = nullptr;
        cfg.eb = "../chm/microcode/SmalltalkDorado.eb!1";
        cfg.germ = nullptr;
        if (lisp_w) {
            // Interlisp-D Lyric, full library (the CORE=lisp build): the Alto
            // emulator on its Trident pack, Leaf/STP from FTP_ROOT (-f)
            cfg.snapshot = "build/good-packs/lisp-lyric-full.snap";
            if (access("build/good-packs/lisp-lyric-full.storage", R_OK) == 0)
                cfg.storage_image = "build/good-packs/lisp-lyric-full.storage";
            cfg.pack = "build/good-packs/lisp-lyric-full.pack";
            cfg.eb = "worlds/aemu.eb";
            // the Cedar tree is the default -f; Lisp's own is chm/lisp/ftp-root
            cfg.ftp_root = strcmp(ftp_root, "../chm/cedar/stp-root") ? ftp_root : "../chm/lisp/ftp-root";
            setenv("DORADO_DISPM_PRESENT", "1", 1);
        }
    } else {
    cfg.snapshot = "build/good-packs/cedar-desktop.snap";
    cfg.pdi = "build/good-packs/cedar-desktop.pdi";
    cfg.ftp_root = ftp_root;
    cfg.eb = "../chm/dorado/CedarDorado.eb!6";
    cfg.germ = "../chm/cedar/germ-alt/Dorado.germ-6.1.6";
    }
    cfg.quantum = quantum;
    cfg.germ_poll = germ_poll;
    const bool scripted = click_x >= 0 || !text.empty();
    cfg.keyboard = scripted ? 0 : 1;
    if (hps_start(&cfg, &t) != 0) return 1;
    signal(SIGUSR1, on_usr1);
    signal(SIGUSR2, on_usr2);     // the Pilot PDA (process table) to stderr
    setvbuf(stdout, nullptr, _IOLBF, 0);

    const hps_stats *S = hps_get_stats();
    double t0 = now_s(), tl = t0;
    uint64_t c0 = S->rtl_cycles, cl = c0;
    uint64_t kl = S->rtl_clocks64;
    uint64_t fl = S->ref_clocks;
    double   ts_l = S->t_status;
    uint64_t hl[16]; for (int i = 0; i < 16; i++) hl[i] = S->st_cnt[i];
    static const char *stname[16] = {"C_FETCH","C_DEC","C_EXEC","C_NF","C_NEXTRD","C_NEXT","C_SCHED",
        "C_IFU_W","C_IFU_DECW","C_IFU_DEC","C_IFU_AW","C_IFU_BETA","C_IFU_BW","C_IFU_END","C_FINISH","C_APW"};
    dorado_machine *M = (dorado_machine *)hps_machine();
    static dorado_typequeue tq;
    int click = 0;
    bool typed = false;
    double tclick = 0;
    uint64_t next_frame = 0;
    for (;;) {
        hps_poll();
        if (want_pda) {
            want_pda = 0;
            hps_sync_rm(32);
            dorado_machine_dump_pda(M);
            // "va value" lines in /tmp/hps-poke (octal) are stored first, once
            if (FILE *f = fopen("/tmp/hps-poke", "r")) {
                unsigned va, v;
                while (fscanf(f, "%o %o", &va, &v) == 2) {
                    dorado_machine_store_va(M, va, (uint16_t)v);
                    fprintf(stderr, "[poke] %09o <- %06o\n", va, v);
                }
                fclose(f);
                remove("/tmp/hps-poke");
            }
            // and any "va count" lines in /tmp/hps-dump-va (octal), word by word
            if (FILE *f = fopen("/tmp/hps-dump-va", "r")) {
                unsigned va, n;
                while (fscanf(f, "%o %o", &va, &n) == 2) {
                    fprintf(stderr, "[dump] %09o:", va);
                    for (unsigned i = 0; i < n; i++)
                        fprintf(stderr, " %06o", dorado_visible_word_at_va(dorado_machine_memory(M), va + i));
                    fprintf(stderr, "\n");
                }
                fclose(f);
            }
        }
        double tn = now_s();
        uint64_t cyc = dorado_machine_cycles(M);
        if (click_x >= 0) {
            if (click == 0 && tn - t0 >= (getenv("HPS_CLICK_AT") ? atof(getenv("HPS_CLICK_AT")) : 5)) {
                dorado_machine_set_mouse(M, click_x, click_y, 0);
                click = 1; tclick = tn;
            } else if (click == 1 && tn - tclick >= 0.3) {
                dorado_machine_set_mouse(M, click_x, click_y, DORADO_MOUSE_LEFT);
                click = 2; tclick = tn;
            } else if (click == 2 && tn - tclick >= (getenv("HPS_CLICK_HOLD") ? atof(getenv("HPS_CLICK_HOLD")) : 0.3)) {
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
            // Rates are over the interval between the status reads the counts
            // came from (a poll can take seconds on the ARM, so the wall clock
            // at the report is not when the counts were taken), and the
            // FPGA's own rate needs no wall clock at all: microinstructions per
            // fabric clock, times the fabric clock against the board's 50 MHz.
            double dt    = S->t_status - ts_l;
            double dn    = (double)(S->rtl_cycles > cl ? S->rtl_cycles - cl : 1);
            double dclk  = (double)(S->rtl_clocks64 - kl);
            double dref  = (double)(S->ref_clocks - fl);
            double cpi   = dclk / dn;
            double fclk  = dref > 0 ? 50.0 * dclk / dref : 0.0;            // MHz
            double mips  = cpi > 0 ? fclk / cpi : 0.0;                     // M/s, the FPGA's own figure
            printf("[hps] %.0f s: %llu microinstructions; FPGA %.2f M/s = %.2fx a Dorado (%.2f clocks each at %.3f MHz, "
                   "%.4f x the 50 MHz reference, PLL locked %d); by the wall clock %.2f M/s over %.1f s; "
                   "%llu ops, %llu reads, %llu packets in, %llu out\n",
                   tn - t0, (unsigned long long)(S->rtl_cycles - c0), mips, mips / 16.67, cpi, fclk,
                   dref > 0 ? dclk / dref : 0.0, S->pll_locked,
                   dt > 0 ? dn / dt / 1e6 : 0.0, dt,
                   (unsigned long long)S->ops, (unsigned long long)S->reads,
                   (unsigned long long)S->rx_packets, (unsigned long long)S->tx_packets);
            // drop accounting: all but the NWW edges should stay 0
            printf("[hps] accounting: %llu NWW edges, %llu reschedules; rx fills %u sent, %u applied; "
                   "%llu transmits lost, %llu result tears, %llu rx lost, %llu unported\n",
                   (unsigned long long)S->nww_posts, (unsigned long long)S->resched,
                   (unsigned)(S->rx_packets & 0x7FFF), (unsigned)S->rx_applied,
                   (unsigned long long)S->tx_unsupported, (unsigned long long)S->result_tears,
                   (unsigned long long)S->rx_lost, (unsigned long long)S->unported);
            printf("[hps] by the wall clock: fabric %.2f MHz, reference %.2f MHz (%llu status reads so far)\n",
                   dt > 0 ? dclk / dt / 1e6 : 0.0, dt > 0 ? dref / dt / 1e6 : 0.0,
                   (unsigned long long)S->status_reads);
            kl = S->rtl_clocks64;
            fl = S->ref_clocks;
            ts_l = S->t_status;
            {
                printf("[hps] clocks by sequencer state, per microinstruction:");
                for (int i = 0; i < 16; i++) {
                    double d = (double)(S->st_cnt[i] - hl[i]) / dn;
                    hl[i] = S->st_cnt[i];
                    if (d >= 0.01) printf(" %s %.2f", stname[i], d);
                }
                printf("\n");
            }
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
            // `touch /tmp/dorado-save`: the registers to /tmp/dorado-save.regs
            // (a scan core; seen at the next status line)
            if (access("/tmp/dorado-save", F_OK) == 0) {
                unlink("/tmp/dorado-save");
                hps_save_regs("/tmp/dorado-save");
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
