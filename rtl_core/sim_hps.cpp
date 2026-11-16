// =============================================================================
// sim_hps.cpp — The MiSTer Cedar configuration, rehearsed under Verilator
// =============================================================================
// dorado_rtl_top as the MiSTer Cedar build has it -- storage in DDR3
// (EXT_STORAGE), the HPS mailbox (HPS_MAILBOX), and powering up in the Cedar
// desktop checkpoint (DORADO_INIT + DORADO_CHECKPOINT, from
// rtl_core/init_cedar: sim_cedar.cpp CEDAR_DUMP_INIT) -- in front of the DDR
// model, with the HPS program (hps/hps_host.cpp) running in this process
// against the model's array. Nothing here reaches into the RTL: the only
// connection is DDR, as on the board.
//
// Usage (from rtl_core/): obj_hps/Vdorado_rtl_top [microinstructions]
//   CEDAR_LS=1   the verify-cedar-ls input (click, type a List over STP)
//   CEDAR_PGM=f  render the screen to f at the end
//   HPS_QUANTUM=n, HPS_GERM_POLL=n  the host's step and poll interval
//   HPS_CLICK=x,y  click there instead of CEDAR_LS's (700,733), and type nothing
//   HPS_PCHIST_FROM=n  from n microinstructions on, histogram task 0's
//                IM address per clock; the top 40 at the end
// =============================================================================

#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <verilated.h>
#include "Vdorado_rtl_top.h"
#include "Vdorado_rtl_top___024root.h"
#include "sim_ddr_model.h"
#include "sim_census.h"
#include "hps/hps_host.h"

extern "C" {
#include "machine.h"
#include "cpu.h"
#include "display.h"
#include "memory.h"
#include "typetext.h"
}

static Vdorado_rtl_top *top;
static ddr_model *ddr;
static uint64_t clocks;
static int mouse_tx, mouse_ty, mouse_tb;   // scripted pointer target and buttons
// The Alto-family worlds (dorado-hps takes their keyboard from the core's
// status block): the script's own key matrix stands for the MiSTer's PS/2
// keyboard and drives kbd_words, so the host's copy of the core's keyboard
// is not fed back into it.
static dorado_display *script_kbd;
struct key_tr { uint64_t at; dorado_display_key key; int down; };
static std::vector<key_tr> script_keys;
static size_t script_key_i = 0;
static bool     hist_on;
static uint64_t hist[32];              // fabric clocks per CPU state
static uint64_t trans[32][32], same[32];  // state changes; clocks a state repeated
static const char *stname[] = {"C_FETCH","C_DEC","C_EXEC","C_NF","C_NEXTRD","C_NEXT","C_SCHED",
    "C_IFU_W","C_IFU_DECW","C_IFU_DEC","C_IFU_AW","C_IFU_BETA","C_IFU_BW","C_IFU_END","C_FINISH","C_APW",
    "C_PIPE","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?"};

static uint64_t pchist[4096];
static std::map<uint32_t, uint64_t> vahist;     // (kind << 28 | va) -> references
static bool     pchist_on, watch_on;
static void clock_n(int n) {
    for (int i = 0; i < n; i++) {
        ddr->drive(top);
        top->clk = 0; top->eval();
        ddr->edge(top);
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();
        clocks++;
        // HPS_OPTRACE=lo,hi[,psb]: task 0's Mesa opcodes in a fabric-clock window
        // (code base BR31, byte PC, opcode, current PSB), one line per change
        {
            static uint64_t olo = 0, ohi = 0;
            static unsigned opsb = 0;
            static bool oparsed = false;
            if (!oparsed) {
                oparsed = true;
                if (const char *o = getenv("HPS_OPTRACE")) sscanf(o, "%llu,%llu,%o", (unsigned long long *)&olo, (unsigned long long *)&ohi, &opsb);
            }
            auto *r = top->rootp;
            if (clocks >= olo && clocks < ohi && r->dorado_rtl_top__DOT__u_cpu__DOT__ctask == 0 &&
                (!opsb || r->dorado_rtl_top__DOT__u_cpu__DOT__RM[1] == opsb)) {
                static uint32_t pb = ~0u; static unsigned pp = ~0u, po = ~0u;
                uint32_t b = r->dorado_rtl_top__DOT__u_cpu__DOT__br[31] & 0x0FFFFFFFu;
                unsigned pc = r->dorado_rtl_top__DOT__u_cpu__DOT__ifu_pcx, op = r->dorado_rtl_top__DOT__u_cpu__DOT__ifu_opcode;
                if (b != pb || pc != pp || op != po)
                    printf("[op] clk %llu cb %09o pc %06o op %03o psb %06o sp %03o\n", (unsigned long long)clocks, b, pc, op,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__RM[1], (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__StkP);
                pb = b; pp = pc; po = op;
            }
        }
        // HPS_FLT=lo,hi: each clock in the window where the task, real PC, StkP,
        // the stack flags or the fault count change
        if (const char *fw = getenv("HPS_FLT")) {
            static uint64_t flo, fhi; static bool fp;
            if (!fp) { fp = true; sscanf(fw, "%llu,%llu", (unsigned long long *)&flo, (unsigned long long *)&fhi); }
            auto *r = top->rootp;
            if (clocks >= flo && clocks < fhi) {
                static unsigned pk = ~0u;
                unsigned ct = r->dorado_rtl_top__DOT__u_cpu__DOT__ctask, rp = r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777;
                unsigned sp = r->dorado_rtl_top__DOT__u_cpu__DOT__StkP, fl = (r->dorado_rtl_top__DOT__u_cpu__DOT__stk_ovf << 1) | r->dorado_rtl_top__DOT__u_cpu__DOT__stk_und;
                unsigned fc = (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__fault_count;
                unsigned k = (ct << 28) ^ (rp << 12) ^ (sp << 4) ^ fl ^ (fc << 20);
                if (getenv("HPS_FLT_ALL")) {
                    auto bits = [](const VlWide<3> &w, int lo, int n) -> unsigned {
                        uint64_t v = 0; for (int i = 0; i < n; i++) { int b = lo + i; v |= (uint64_t)((w[b / 32] >> (b % 32)) & 1) << i; } return (unsigned)v; };
                    auto *cu = &r->dorado_rtl_top__DOT__u_cpu__DOT__cur_u, *xu = &r->dorado_rtl_top__DOT__u_cpu__DOT__x_u, *eu = &r->dorado_rtl_top__DOT__u_cpu__DOT__ex_u;
                    printf("      cur ff %03o jcn %03o | x ff %03o jcn %03o | ex ff %03o jcn %03o | im_q_addr %04o pred_np %04o ok %u x_pred_np %04o x_pred_np_x %04o fresh %u pent %u exec_now %u fexec %u p_pc_x %04o pc_p %04o f_np %04o p_d %u\n",
                           bits(*cu,58,8), bits(*cu,50,8), bits(*xu,58,8), bits(*xu,50,8), bits(*eu,58,8), bits(*eu,50,8),
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__im_q_addr, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pred_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pred_ok, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_pred_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_pred_np_x & 07777, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_pred_fresh,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pent, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__exec_now,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__fexec, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_pc_x & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pc_p & 07777, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__f_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_d);
                }
                if (getenv("HPS_FLT_ALL"))
                    printf("      rms_raddr %d p_raddr %d rm_q %06o byp %u RBase %o p_w %u p_x %u ovx %u p_bub %u we %u waddr %d wd %06o x_rb_iss %o\n",
                           (int)r->dorado_rtl_top__DOT__u_cpu__DOT__rms_raddr, (int)r->dorado_rtl_top__DOT__u_cpu__DOT__p_raddr,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__rm_q, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__rms_byp,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__RBase, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_w,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_x, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ovx,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__p_bub, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__rms_we,
                           (int)r->dorado_rtl_top__DOT__u_cpu__DOT__rms_waddr, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__rms_wd,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_rbase_at_issue);
                if (getenv("HPS_FLT_ALL"))
                    printf("[clk] %llu task %u pc %04o st %u np %04o ap_req %u ap_hold %u ap_done_q %u mb_req %u op %u va %07o pcf %06o pfpcf %06o fast %u StkP %03o\n",
                           (unsigned long long)clocks, ct, rp, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__st,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__x_np & 07777,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ap_req, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ap_hold,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ap_done_q, (unsigned)r->dorado_rtl_top__DOT__mb_req,
                           (unsigned)r->dorado_rtl_top__DOT__mb_op, (unsigned)r->dorado_rtl_top__DOT__mb_va & 0x0FFFFFFF,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ifu_pcf, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__pf_pcf,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__nx_fast, sp);
                if (k != pk)
                    printf("[flt] clk %llu task %u pc %04o StkP %03o ovf/und %u faults %u wake %04x\n", (unsigned long long)clocks, ct, rp, sp, fl, fc,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__wakeup_pending);
                pk = k;
            }
        }
        // HPS_RETIRE=file[,from,to]: the processor as each finish left it (snap_*
        // when C_PIPE executed the next instruction beside it), one binary
        // record per retired microinstruction: diff an overlap run against an
        // RTL_OV_HOLD one (tools/retire_diff.py)
        {
            static FILE *rf = nullptr; static bool rinit = false;
            static uint64_t rfrom = 0, rto = ~0ull, rn = 0;
            if (!rinit) {
                rinit = true;
                if (const char *e = getenv("HPS_RETIRE")) {
                    char fn[512]; unsigned long long a = 0, b = ~0ull;
                    int k = sscanf(e, "%511[^,],%llu,%llu", fn, &a, &b);
                    if (k >= 1) { rf = fopen(fn, "wb"); rfrom = a; if (k == 3) rto = b; }
                }
            }
#define RC(x) top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##x
            if (rf && RC(step_done)) {
                if (rn >= rfrom && rn < rto) {
                    const bool sv = RC(snap_valid) != 0;
                    uint16_t rec[12];
                    rec[0]  = (uint16_t)(sv ? RC(snap_real_PC) : RC(real_PC));
                    rec[1]  = (uint16_t)(sv ? RC(snap_ctask) : RC(ctask));
                    rec[2]  = (uint16_t)(sv ? RC(snap_T) : RC(T));
                    rec[3]  = (uint16_t)(sv ? RC(snap_Link) : RC(Link));
                    rec[4]  = (uint16_t)(sv ? RC(snap_StkP) : RC(StkP));
                    rec[5]  = (uint16_t)(sv ? RC(snap_RBase) : RC(RBase));
                    rec[6]  = (uint16_t)(sv ? RC(snap_MemBase) : RC(MemBase));
                    rec[7]  = (uint16_t)(sv ? RC(snap_Q) : RC(Q));
                    rec[8]  = (uint16_t)(sv ? RC(snap_Cnt) : RC(Cnt));
                    rec[9]  = (uint16_t)(sv ? RC(snap_ShC) : RC(ShC));
                    rec[10] = (uint16_t)(sv ? (unsigned)RC(snap_flags)
                                            : (unsigned)(RC(alu_zero) << 3 | RC(alu_lt0) << 2 | RC(alu_carry) << 1 | RC(alu_overflow)));
                    rec[11] = (uint16_t)sv;
                    fwrite(rec, sizeof rec, 1, rf);
                }
                rn++;
            }
#undef RC
        }
        if (pchist_on && top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__ctask == 0)
            pchist[top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777]++;
        if (watch_on) {
            auto *r = top->rootp;
            auto watched = [](uint32_t va) {
                return va == 0200000u || (va >= 0202154u && va <= 0202157u) || (va >= 0202044u && va <= 0202047u);
            };
            if (r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req && r->dorado_rtl_top__DOT__u_cpu__DOT__mem_accepted) {
                uint32_t va = r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_va & 0x0FFFFFFFu;
                unsigned k = r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_kind;
                static uint64_t wlo = getenv("HPS_WATCH_CLK") ? strtoull(getenv("HPS_WATCH_CLK"), nullptr, 0) : ~0ull;
                bool inwin = clocks >= wlo && clocks < wlo + 400;
                static uint32_t wva = getenv("HPS_WATCH_VA") ? (uint32_t)strtoul(getenv("HPS_WATCH_VA"), nullptr, 8) : 0xFFFFFFFFu;
                if (va == wva && (k == 5 || k == 10 || k == 4))
                    printf("[wva] clk %llu %s b %06o task %u pc %04o psb %06o\n", (unsigned long long)clocks,
                           k == 5 ? "STORE" : "fetch", (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_b,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_task,
                           (unsigned)(r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777),
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__RM[1]);
                if ((watched(va) || inwin) && (k == 5 || k == 10 || k == 4))
                    printf("[watch] clk %llu %s va %07o b %06o task %u pc %04o\n", (unsigned long long)clocks,
                           k == 5 ? "STORE" : "fetch", va, (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_b,
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_task,
                           (unsigned)(r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777));
            }
            static bool ap_prev;
            bool apq = r->dorado_rtl_top__DOT__mb_req;
            if (apq && !ap_prev) {
                uint32_t va = r->dorado_rtl_top__DOT__mb_va & 0x0FFFFFFFu;
                unsigned op = r->dorado_rtl_top__DOT__mb_op;
                if (op == 6 || op == 4 || (op <= 2 && watched(va)))
                    printf("[watch] clk %llu AP op %u va %07o wd %06o pc %04o task %u\n", (unsigned long long)clocks, op, va,
                           (unsigned)r->dorado_rtl_top__DOT__mb_wd,
                           (unsigned)(r->dorado_rtl_top__DOT__u_cpu__DOT__real_PC & 07777),
                           (unsigned)r->dorado_rtl_top__DOT__u_cpu__DOT__ctask);
            }
            ap_prev = apq;
        }
        if (pchist_on && top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req &&
            top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_accepted &&
            top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_task == 0)
            vahist[((uint32_t)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_kind << 28) |
                   (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__mem_req_va & 0x0FFFFFFFu)]++;
        if (hist_on) {
            unsigned stt = (top->dbg_vec[1] >> 24) & 31;
            hist[stt]++;
            static unsigned prev = 0;
            if (stt != prev) trans[prev][stt]++;
            else             same[stt]++;
            prev = stt;
        }
    }
}
static void wait_fn(void *) {
    clock_n(4);
    // HPS_PROGRESS: a host wait that lasts (a mailbox operation the core does
    // not answer) shows the core's state every 2^20 clocks of it
    static const bool progress = getenv("HPS_PROGRESS") != nullptr;
    static uint64_t waits = 0;
    if (progress && (++waits & 0x3FFFF) == 0) {
#define RIO(x) top->rootp->dorado_rtl_top__DOT__u_io__DOT__##x
#define RCP(x) top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##x
        fprintf(stderr, "  [wait] %llu: pc=0%o task=%u cpu st=%d io_busy %d ap_req %d ap_hold %d; disk eng busy %d st %d jobs %d%d%d mem_req %d ack %d aux_owner %d\n",
                (unsigned long long)waits, (unsigned)top->dbg_real_pc, (unsigned)top->dbg_ctask, (int)RCP(st),
                (int)RIO(io_busy), (int)RCP(ap_req), (int)RCP(ap_hold),
                (int)RIO(disk_eng_busy), (int)RIO(disk_eng), (int)RIO(disk_job_commit), (int)RIO(disk_job_load),
                (int)RIO(disk_job_refill), (int)RIO(disk_mem_req), (int)RIO(disk_mem_ack),
                (int)top->rootp->dorado_rtl_top__DOT__aux_owner);
#undef RIO
#undef RCP
    }
}
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    setvbuf(stdout, nullptr, _IOLBF, 0);
    uint64_t want = argc > 1 ? strtoull(argv[1], nullptr, 0) : 2000000;
    hist_on = getenv("HPS_HIST") != nullptr;

    // The image's $readmemh files are init/...: build from obj_hps, where
    // init -> ../init_cedar.
    if (chdir(getenv("HPS_INIT_PARENT") ? getenv("HPS_INIT_PARENT") : "obj_hps") != 0) {
        perror("obj_hps");
        return 2;
    }
    top = new Vdorado_rtl_top;
    ddr = new ddr_model;
    top->clk = 0; top->rst = 1; top->clk_en = 1;
    top->bb_present = 0; top->bb_running = 1; top->bb_ss_inject = 0;
    top->io_bridge_en = 1; top->bootsrv_en = 0; top->assist_en = 0;
    // Cedar's own display microcode draws through the display controller
    top->display_wake_gate = 1; top->display_top_lines = 22; top->eth_local_host = 042;
    top->io_attached = 0; top->fb_rd_addr = 0;
    top->kbd_words = ~0ULL;
    top->mouse_buttons = 0; top->mouse_present = 0; top->mouse_x = 0; top->mouse_y = 0;
    clock_n(4);
    top->rst = 0;
    top->ov_hold = getenv("RTL_OV_HOLD") ? 1 : 0;   // (a PIPELINE build: no C_PIPE)
    clock_n(256);       // the mailbox clears the control beat first, as on the board

    const char *ddir = getenv("DORADO_DIR") ? getenv("DORADO_DIR") : "../../dorado";
    if (chdir(ddir) != 0) { perror(ddir); return 2; }

    hps_transport t;
    t.mb = &ddr->mem[0x100000];                    // byte 0x3080_0000
    t.storage = (volatile uint16_t *)&ddr->mem[0];  // byte 0x3000_0000
    t.storage_words = 4u << 20;
    t.pack = &ddr->mem[0x200000];                   // byte 0x3100_0000
    t.pack_beats = (1u << 24) - 0x200000;
    t.wait = wait_fn;
    t.ctx = nullptr;
    hps_config cfg;
    // SIM_WORLD=smalltalk|lisp (else Cedar): the world's files
    auto world = [&](const char *w, const char *snap) {
        const bool lisp_w = w && !strcmp(w, "lisp");
        const bool smalltalk = lisp_w || (w && !strcmp(w, "smalltalk"));
        cfg.pack = nullptr;
        cfg.storage_image = nullptr;
        cfg.world_cfg = 0;
        top->dispm_present = lisp_w;      // (Dorado.sv: world_cfg bit 2)
        cfg.need_core = 0;
        if (lisp_w) {
            cfg.snapshot = "build/good-packs/lisp-lyric-full.snap";
            cfg.pdi = nullptr;
            cfg.pack = getenv("SIM_PACK") ? getenv("SIM_PACK") : "build/run-disks/lisp-lockstep.pack";
            cfg.eb = "worlds/aemu.eb";
            cfg.ftp_root = "../chm/lisp/ftp-root";
            cfg.germ = nullptr;
        } else if (smalltalk) {
            cfg.snapshot = "build/good-packs/smalltalk76.snap";
            cfg.pdi = nullptr;
            cfg.pack = getenv("SIM_PACK") ? getenv("SIM_PACK") : "build/good-packs/smalltalk76.pack";
            cfg.ftp_root = nullptr;
            cfg.eb = "../chm/microcode/SmalltalkDorado.eb!1";
            cfg.germ = nullptr;
        } else {
            cfg.snapshot = "build/good-packs/cedar-desktop.snap";
            cfg.pdi = "build/good-packs/cedar-desktop.pdi";
            cfg.ftp_root = "../chm/cedar/stp-root";
            cfg.eb = "../chm/dorado/CedarDorado.eb!6";
            cfg.germ = "../chm/cedar/germ-alt/Dorado.germ-6.1.6";
        }
        // a save (hps_save) in the world's place
        if (snap && *snap) cfg.snapshot = snap;
        cfg.quantum = getenv("HPS_QUANTUM") ? (unsigned)atoi(getenv("HPS_QUANTUM")) : 1;
        cfg.germ_poll = getenv("HPS_GERM_POLL") ? strtoull(getenv("HPS_GERM_POLL"), nullptr, 0) : 0;
        // Smalltalk, like the board: dorado-hps takes the keyboard and the
        // absolute mouse from the core's status block (dorado-hps -w smalltalk)
        cfg.keyboard = smalltalk ? 1 : 0;
    };
    // HPS_SNAPSHOT=<path>: restore this snapshot in the world's place
    world(getenv("SIM_WORLD"), getenv("HPS_SNAPSHOT"));
    const bool smalltalk = getenv("SIM_WORLD") && (!strcmp(getenv("SIM_WORLD"), "lisp") || !strcmp(getenv("SIM_WORLD"), "smalltalk"));
    if (hps_start(&cfg, &t) != 0) return 1;
    // HPS_INITCHECK=1: the core as it powered up (init/, DORADO_CHECKPOINT)
    // against the C machine dorado-hps restored: map, BRs, cache
    if (getenv("HPS_INITCHECK")) {
#define RM_(f) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__u_mem__DOT__##f)
#define RC_(f) (top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__##f)
        dorado_machine *Mi = (dorado_machine *)hps_machine();
        const dorado_memory *cm = dorado_machine_memory(Mi);
        int bad = 0;
        for (int i = 0; i < DM_MAP_ENTRIES; i++) {
            if (RM_(map_rp)[i] != cm->map[i].rp || RM_(map_wp)[i] != (cm->map[i].wp ? 1 : 0)) {
                if (bad++ < 10) printf("[init] map[%06o] RTL rp %06o wp %d, C rp %06o wp %d\n", i,
                                       (unsigned)RM_(map_rp)[i], (int)RM_(map_wp)[i], (unsigned)cm->map[i].rp, cm->map[i].wp ? 1 : 0);
            }
        }
        printf("[init] map: %d entries differ\n", bad);
        bad = 0;
        for (int i = 0; i < DM_BR_COUNT; i++)
            if (RC_(br)[i] != (cm->br[i] & 0x0FFFFFFFu) && bad++ < 10)
                printf("[init] br[%02o] RTL %09o C %09o\n", i, (unsigned)RC_(br)[i], (unsigned)cm->br[i]);
        printf("[init] BRs: %d differ\n", bad);
        bad = 0;
        for (int r = 0; r < DM_CACHE_ROWS; r++)
            for (int w = 0; w < DM_CACHE_WAYS; w++) {
                const auto &cw = cm->cache[r].ways[w];
                bool d = RM_(cache_tag)[r][w] != (cw.tag & 0x3FFFFu) || RM_(cache_valid)[r][w] != (cw.valid ? 1 : 0) ||
                         RM_(cache_dirty)[r][w] != (cw.dirty ? 1 : 0);
                for (int k = 0; k < DM_CACHE_LINE_W; k++) {
                    const int ix = r * 16 + k;
                    const uint16_t v = (w == 0) ? RM_(cache_w0)[ix] : (w == 1) ? RM_(cache_w1)[ix] : (w == 2) ? RM_(cache_w2)[ix] : RM_(cache_w3)[ix];
                    if (v != cw.data[k]) d = true;
                }
                if (d && bad++ < 6)
                    printf("[init] cache row %d way %d: RTL tag %06o v%d d%d, C tag %06o v%d d%d\n", r, w,
                           (unsigned)RM_(cache_tag)[r][w], (int)RM_(cache_valid)[r][w], (int)RM_(cache_dirty)[r][w],
                           (unsigned)cw.tag, cw.valid ? 1 : 0, cw.dirty ? 1 : 0);
            }
        printf("[init] cache: %d lines differ (the core has run a little since go)\n", bad);
        {
            const dorado_cpu *cc = (const dorado_cpu *)dorado_machine_cpu(Mi);
            int b2 = 0;
            for (int i = 0; i < 256; i++)
                if (RC_(RM)[i] != cc->RM[i] && b2++ < 12)
                    printf("[init] RM[%03o] RTL %06o C %06o\n", i, (unsigned)RC_(RM)[i], (unsigned)cc->RM[i]);
            printf("[init] RM: %d differ\n", b2);
            b2 = 0;
            for (int i = 0; i < 256; i++)
                if (RC_(STK)[i] != cc->STK[i] && b2++ < 6)
                    printf("[init] STK[%03o] RTL %06o C %06o\n", i, (unsigned)RC_(STK)[i], (unsigned)cc->STK[i]);
            printf("[init] STK: %d differ\n", b2);
            for (int t = 0; t < 16; t++)
                if (RC_(task_t)[t] != cc->task_t[t] || RC_(task_tpc)[t] != cc->task_tpc[t] ||
                    RC_(task_link)[t] != cc->task_link[t] || RC_(task_membase)[t] != cc->task_membase[t])
                    printf("[init] task %2d: RTL T %06o TPC %05o Link %06o MB %02o | C T %06o TPC %05o Link %06o MB %02o\n", t,
                           (unsigned)RC_(task_t)[t], (unsigned)RC_(task_tpc)[t], (unsigned)RC_(task_link)[t], (unsigned)RC_(task_membase)[t],
                           (unsigned)cc->task_t[t], (unsigned)cc->task_tpc[t], (unsigned)cc->task_link[t], (unsigned)cc->task_membase[t]);
        }
#undef RM_
#undef RC_
    }
    dorado_machine *M = (dorado_machine *)hps_machine();
    const hps_stats *S = hps_get_stats();

    const bool ls = getenv("CEDAR_LS") != nullptr;
    const uint64_t c0 = dorado_machine_cycles(M);
    uint64_t start = S->rtl_cycles;
    int click = 0;
    uint64_t click_at = 0;
    bool typed = false;
    uint64_t next_frame = c0 + 2000000ull, next_report = 0;
    static dorado_typequeue tq;
    auto t0 = std::chrono::steady_clock::now();
    // HPS_PROGRESS=n: every n host loops, where the core is (and the disk engine)
    const long progress = getenv("HPS_PROGRESS") ? atol(getenv("HPS_PROGRESS")) : 0;
    long loops = 0;
    // SIM_RESTART=<microinstructions>,<world>[,<snapshot>]: there, start that
    // world (cedar|lisp|smalltalk; or a save of it) over the running one,
    // without a reset of the model -- the program started again on the board
    // with the core left loaded. The run then counts from the new start.
    static char rs_world[32], rs_snap[400];
    unsigned long long rs_at = ~0ull;
    if (const char *rs = getenv("SIM_RESTART")) {
        char *e;
        rs_at = strtoull(rs, &e, 0);
        if (*e == ',') sscanf(e + 1, "%31[^,],%399s", rs_world, rs_snap);
    }
    while (S->rtl_cycles - start < want) {
        clock_n(2048);
        hps_poll();
        if (S->rtl_cycles - start >= rs_at) {
            rs_at = ~0ull;
            world(rs_world, rs_snap);
            if (hps_restart(&cfg) != 0) return 1;
            M = (dorado_machine *)hps_machine();
            start = S->rtl_cycles;
            fprintf(stderr, "[sim] restarted as %s at microinstruction %llu\n", rs_world, (unsigned long long)start);
            continue;
        }
        if (progress && ++loops % progress == 0) {
#define RIO(x) top->rootp->dorado_rtl_top__DOT__u_io__DOT__##x
            fprintf(stderr, "  [prog] loop %ld: %llu microinstructions; pc=0%o task=%u; disk eng busy %d st %d jobs %d%d%d mem_req %d aux_owner %d\n",
                    loops, (unsigned long long)(S->rtl_cycles - start), (unsigned)top->dbg_real_pc, (unsigned)top->dbg_ctask,
                    (int)RIO(disk_eng_busy), (int)RIO(disk_eng), (int)RIO(disk_job_commit), (int)RIO(disk_job_load),
                    (int)RIO(disk_job_refill), (int)RIO(disk_mem_req), (int)top->rootp->dorado_rtl_top__DOT__aux_owner);
#undef RIO
        }
        uint64_t now = dorado_machine_cycles(M);
        // HPS_KEYTEST: A held at the core's keyboard (terminal back channel)
        // from 2M to 4M microinstructions; KeyBits and the mouse read back
        if (getenv("HPS_KEYTEST")) {
            uint64_t u = S->rtl_cycles - start;
            static int phase = 0;
            auto show = [&](const char *w) {
                dorado_memory *mem = dorado_machine_memory(M);
                printf("[hps] %s at %llu: 177033..37", w, (unsigned long long)u);
                for (unsigned a = 0177033; a <= 0177037; a++) printf(" %06o", dorado_visible_word_at_va(mem, a));
                printf("  mouse %d %d\n", (short)dorado_visible_word_at_va(mem, 0424),
                       (short)dorado_visible_word_at_va(mem, 0425));
#define RIO(x) top->rootp->dorado_rtl_top__DOT__u_io__DOT__##x
                printf("        io: kbd words %04x %04x %04x %04x pending %x  term bits %llu messages %llu  msg type %u bit %u\n",
                       (unsigned)RIO(ddc_kbd_words)[0], (unsigned)RIO(ddc_kbd_words)[1], (unsigned)RIO(ddc_kbd_words)[2],
                       (unsigned)RIO(ddc_kbd_words)[3], (unsigned)RIO(ddc_kbd_pending),
                       (unsigned long long)RIO(ddc_term_bits), (unsigned long long)RIO(ddc_term_messages),
                       (unsigned)RIO(ddc_term_msg_type), (unsigned)RIO(ddc_term_msg_bit));
#undef RIO
            };
            if (phase == 0 && u >= 1000000) { show("before"); phase = 1; }
            if (phase == 1 && u >= 2000000) {
                top->kbd_words = ~0ULL & ~(1ULL << (32 + 10));      // A: word 2, 175777
                top->mouse_dx = 30; top->mouse_dy = 20; top->mouse_delta_stb = 1;
                clock_n(1); top->mouse_delta_stb = 0;
                phase = 2;
            }
            if (phase == 2 && (u == 2500000 || u >= 3000000) && u < 3010000) show("A held");
            if (phase == 2 && u >= 4000000) { show("A down"); top->kbd_words = ~0ULL; phase = 3; }
            if (phase == 3 && u >= 6000000) { show("A up"); phase = 4; }
        }
        if (S->rtl_cycles - start >= next_report) {
            {
                dorado_memory *mem = dorado_machine_memory(M);
                printf("[hps] cursor at %llu: CursorX raw %u, rows %llu, last cursor row x %d y %u (lowest y %u); guest mouse %d %d cursor %d %d\n",
                       (unsigned long long)(S->rtl_cycles - start),
                       (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_nlcb_cursor_x,
                       (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_rows,
                       (int)(int16_t)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_dbg_x,
                       (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_dbg_y,
                       (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_dbg_ymin,
                       (short)dorado_visible_word_at_va(mem, 0424), (short)dorado_visible_word_at_va(mem, 0425),
                       (short)dorado_visible_word_at_va(mem, 0426), (short)dorado_visible_word_at_va(mem, 0427));
            }
            next_report += 10000000;
            fprintf(stderr, "  [hps] %llu microinstructions, host cycle %llu; %llu ops (%llu reads, %llu stores), "
                    "%llu packets in\n",
                    (unsigned long long)(S->rtl_cycles - start), (unsigned long long)now,
                    (unsigned long long)S->ops, (unsigned long long)S->reads,
                    (unsigned long long)S->stores, (unsigned long long)S->rx_packets);
        }
        if (!ls && !getenv("HPS_CLICK") && !getenv("HPS_TYPE")) continue;
        // Input reaches the core as the MiSTer's does: keyboard words and
        // mouse motion into dorado_rtl_top, where Cedar's display microcode
        // takes them from the terminal back channel. The script below drives
        // the C machine's keyboard words (the typing queue) and a target
        // pointer position; they are forwarded here.
        {
            dorado_display *d = smalltalk ? script_kbd : dorado_machine_display(M);
            uint64_t kw = 0;
            if (d) for (int i = 0; i < 4; i++) kw |= (uint64_t)dorado_display_keyboard_word(d, i) << (16 * i);
            else kw = ~0ull;
            top->kbd_words = kw;
            static int sent_x = 0, sent_y = 0;          // Cedar's pointer starts at 0,0
            if (smalltalk) {
                // the Alto world's way (Dorado.sv DORADO_SMALLTALK): the
                // absolute position and buttons, through the status block
                top->mouse_present = 1;
                top->mouse_x = (uint16_t)mouse_tx;
                top->mouse_y = (uint16_t)mouse_ty;
            } else if (mouse_tx != sent_x || mouse_ty != sent_y) {
                int dx = mouse_tx - sent_x, dy = mouse_ty - sent_y;
                if (dx > 127) dx = 127; else if (dx < -127) dx = -127;
                if (dy > 127) dy = 127; else if (dy < -127) dy = -127;
                top->mouse_dx = dx; top->mouse_dy = dy; top->mouse_delta_stb = 1;
                clock_n(1);
                top->mouse_delta_stb = 0;
                sent_x += dx; sent_y += dy;
            }
            top->mouse_buttons = mouse_tb;
        }
        // verify-cedar-ls, as sim_cedar.cpp drives it (2M-cycle frames)
        if (now >= next_frame) {
            next_frame += 2000000ull;
            if (typed) dorado_typequeue_pump(&tq, M);
        }
        if (const char *wf = getenv("HPS_WATCH_FROM"))
            watch_on = S->rtl_cycles - start >= strtoull(wf, nullptr, 0);
        if (const char *pf = getenv("HPS_PCHIST_FROM"))
            pchist_on = S->rtl_cycles - start >= strtoull(pf, nullptr, 0);
        if (click == 0 && (ls || getenv("HPS_CLICK")) && now >= c0 + (getenv("HPS_CLICK_AT") ? strtoull(getenv("HPS_CLICK_AT"), nullptr, 0) : 100000000ull)) {
            int cx = 700, cy = 733;
            if (const char *hc = getenv("HPS_CLICK")) sscanf(hc, "%d,%d", &cx, &cy);
            printf("[hps] click (%d,%d) at cycle %llu\n", cx, cy, (unsigned long long)now);
            mouse_tx = cx; mouse_ty = cy;
            click = 1; click_at = now;
        } else if (click == 1 && now >= click_at + (getenv("HPS_CLICK_AT") ? 2000000ull : 20000000ull)) {   // after the pointer arrives
            mouse_tb = DORADO_MOUSE_LEFT;
            click = 2; click_at = now;
        } else if (click == 2 && now >= click_at + (getenv("HPS_CLICK_HOLD") ? strtoull(getenv("HPS_CLICK_HOLD"), nullptr, 0) : 1600000ull)) {
            mouse_tb = 0;
            click = 3;
        }
        // HPS_TYPE=<text> (\n = Return) at HPS_TYPE_AT BaseBoard cycles after
        // the start, one key transition per 3M cycles, through the core's
        // keyboard inputs as the MiSTer's are
        if (const char *ht = getenv("HPS_TYPE")) {
            static std::string txt;
            const uint64_t at = getenv("HPS_TYPE_AT") ? strtoull(getenv("HPS_TYPE_AT"), nullptr, 0) : 20000000ull;
            if (!typed && now >= c0 + at) {
                typed = true;
                for (const char *q = ht; *q; q++) {
                    if (q[0] == '\\' && q[1] == 'n') { txt += '\n'; q++; } else txt += *q;
                }
                printf("[hps] typing at cycle %llu\n", (unsigned long long)now);
                if (smalltalk) {
                    // a typist: key down 3M cycles, up, 3M before the next
                    script_kbd = (dorado_display *)calloc(1, sizeof(dorado_display));
                    dorado_display_keyboard_all_up(script_kbd);
                    uint64_t t = now;
                    for (char c : txt) {
                        int shift = 0;
                        dorado_display_key k = dorado_char_to_key(c, &shift);
                        if (k == DORADO_KEY_NONE) continue;
                        // shift goes down well before the key and up after it, as
                        // a typist's does: a shift that changes in the same
                        // terminal message as the key can reach the guest second
                        if (shift) { script_keys.push_back({t, DORADO_KEY_LSHIFT, 1}); t += 3000000ull; }
                        script_keys.push_back({t, k, 1});
                        t += 3000000ull;
                        script_keys.push_back({t, k, 0});
                        t += 3000000ull;
                        if (shift) { script_keys.push_back({t, DORADO_KEY_LSHIFT, 0}); t += 3000000ull; }
                    }
                } else {
                    dorado_typequeue_start(&tq, txt.c_str(), 3000000ull, now);
                }
            }
            while (script_kbd && script_key_i < script_keys.size() && now >= script_keys[script_key_i].at) {
                dorado_display_keyboard_set_key(script_kbd, script_keys[script_key_i].key, script_keys[script_key_i].down);
                script_key_i++;
            }
        } else if (!typed && !getenv("HPS_CLICK") && now >= c0 + 300000000ull) {
            typed = true;
            printf("[hps] typing at cycle %llu\n", (unsigned long long)now);
            dorado_typequeue_start(&tq, "List [Cedar]<Cedar6.1>VersionMap>*\n", 800000ull, now);
        }
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    printf("[hps] %llu microinstructions on the RTL in %.1f s, %llu clocks (%.2f each; the status beat's count says %.2f); host at cycle %llu\n",
           (unsigned long long)(S->rtl_cycles - start), secs, (unsigned long long)clocks,
           (double)clocks / (double)(S->rtl_cycles - start),
           (double)S->rtl_clocks64 / (double)(S->rtl_cycles - start),
           (unsigned long long)dorado_machine_cycles(M));
    printf("[hps] cursor: %llu CursorHi writes, %llu rows drawn; CursorX %u\n",
           (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_wr,
           (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_cursor_rows,
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_nlcb_cursor_x);
    printf("[hps] TPC[3] %o TPC[4] %o TPC[11] %o TPC[13] %o\n",
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__task_tpc[3], (unsigned)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__task_tpc[4],
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__task_tpc[9], (unsigned)top->rootp->dorado_rtl_top__DOT__u_cpu__DOT__task_tpc[11]);
    printf("[hps] display: wake held %u, terminal task %o, statics %o, %llu frames\n",
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_wake_held,
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_terminal_task,
           (unsigned)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_statics,
           (unsigned long long)top->rootp->dorado_rtl_top__DOT__u_io__DOT__ddc_frame_count);
    printf("[hps] mailbox: %llu operations -- %llu reads, %llu stores, %llu NWW ors, %llu reschedules; "
           "%llu packets in, %llu out, %llu transmits lost, %llu inexpressible\n",
           (unsigned long long)S->ops, (unsigned long long)S->reads, (unsigned long long)S->stores,
           (unsigned long long)S->rm_or, (unsigned long long)S->resched,
           (unsigned long long)S->rx_packets, (unsigned long long)S->tx_packets,
           (unsigned long long)S->tx_unsupported,
           (unsigned long long)S->unported);
    printf("[hps] accounting: %llu NWW edges; rx fills %u sent, %u applied; %llu result tears, %llu rx lost\n",
           (unsigned long long)S->nww_posts, (unsigned)(S->rx_packets & 0x7FFF), (unsigned)S->rx_applied,
           (unsigned long long)S->result_tears, (unsigned long long)S->rx_lost);
    if (hist_on) {
        uint64_t tot = 0;
        for (int i = 0; i < 32; i++) tot += hist[i];
        printf("[hps] clocks by CPU state (%llu, %.2f per microinstruction):\n",
               (unsigned long long)tot, (double)tot / (S->rtl_cycles - start));
        for (int i = 0; i < 32; i++)
            if (hist[i] * 200 > tot)
                printf("         %-10s %5.1f%%  %.2f/uinstr  (repeats %.2f/uinstr)\n", stname[i], 100.0 * hist[i] / tot,
                       (double)hist[i] / (S->rtl_cycles - start), (double)same[i] / (S->rtl_cycles - start));
        printf("[hps] state changes, per microinstruction:\n");
        for (int a = 0; a < 32; a++) for (int b = 0; b < 32; b++)
            if (trans[a][b] * 50 > (S->rtl_cycles - start))
                printf("         %-10s -> %-10s %.2f\n", stname[a], stname[b],
                       (double)trans[a][b] / (S->rtl_cycles - start));
    }
    if (pchist_on) {
        std::vector<std::pair<uint64_t, unsigned>> v;
        uint64_t tot = 0;
        for (unsigned a = 0; a < 4096; a++) if (pchist[a]) { v.push_back({pchist[a], a}); tot += pchist[a]; }
        std::sort(v.rbegin(), v.rend());
        printf("[hps] task 0 IM addresses, %llu clocks:\n", (unsigned long long)tot);
        for (size_t i = 0; i < v.size() && i < 40; i++)
            printf("  %04o %6.2f%%\n", v[i].second, 100.0 * (double)v[i].first / (double)tot);
        std::vector<std::pair<uint64_t, uint32_t>> w;
        uint64_t nref = 0;
        for (auto &e : vahist) { w.push_back({e.second, e.first}); nref += e.second; }
        std::sort(w.rbegin(), w.rend());
        printf("[hps] task 0 references, %llu, %zu distinct (kind va):\n", (unsigned long long)nref, w.size());
        for (size_t i = 0; i < w.size() && i < 40; i++)
            printf("  kind %u va %09o  %llu\n", w[i].second >> 28, w[i].second & 0x0FFFFFFFu,
                   (unsigned long long)w[i].first);
    }
    if (getenv("HPS_CENSUS")) census_report(top);
    top->final();
    printf("[hps] DDR: %llu beats read, %llu written\n",
           (unsigned long long)ddr->beats_rd, (unsigned long long)ddr->beats_wr);
    if (const char *rtl = getenv("HPS_PGM_RTL")) {      // what the core's video shows
        FILE *f = fopen(rtl, "wb");
        int px = 0;
        if (f) {
            fprintf(f, "P5\n1024 808\n255\n");
            for (int i = 0; i < 1024 * 808; i++) {
                int b = top->rootp->dorado_rtl_top__DOT__u_io__DOT__fb[i] & 1;
                {   // the cursor, mixed in as the scan-out does
                    unsigned row = top->rootp->dorado_rtl_top__DOT__u_io__DOT__cur_line[i >> 10];
                    unsigned d = (unsigned)((i & 1023) - top->rootp->dorado_rtl_top__DOT__u_io__DOT__cur_x) & 0xFFFF;
                    if (d < 16 && ((row >> (15 - d)) & 1)) b = 1;
                }
                px += b;
                fputc(b ? 0 : 255, f);
            }
            fclose(f);
        }
        printf("[hps] RTL display controller: %d pixels -> %s\n", px, rtl);
    }
    // RTL_FB_OUT=f.pbm: the RTL DDC framebuffer (what the HDMI shows)
    if (const char *fbp = getenv("RTL_FB_OUT")) {
        int lit = 0;
        if (FILE *pf = fopen(fbp, "wb")) {
            fprintf(pf, "P4\n1024 808\n");
            for (int i = 0; i < (1024 * 808) / 8; i++) {
                int byte = 0;
                for (int k = 0; k < 8; k++) {
                    int b = top->rootp->dorado_rtl_top__DOT__u_io__DOT__fb[i * 8 + k] & 1;
                    byte = (byte << 1) | b; lit += b;
                }
                fputc(byte, pf);
            }
            fclose(pf);
        }
        printf("[hps] RTL DDC framebuffer: %d lit -> %s\n", lit, fbp);
    }
    if (const char *pgm = getenv("CEDAR_PGM")) {
        int px = hps_snapshot_pgm(pgm);
        printf("[hps] %d display-list pixels -> %s\n", px, pgm);
    }
    if (top->dbg_halted) {
        printf("  [FAIL] the RTL halted at pc=0%o\n", (unsigned)top->dbg_real_pc);
        return 1;
    }
    printf("  [PASS] MiSTer Cedar configuration ran\n");
    return 0;
}
