/*
 * dorado_machine — runnable Dorado orchestration (see machine.h).
 *
 * This is the Stage-2 Alto-style Ethernet software-boot path extracted
 * from test_cpu.c's probe_full_boot_with_bootstrap, with the trace /
 * probe / assertion scaffolding removed. The boot sequence is:
 *
 *   1. BaseBoard cold-boots from its EPROMs; three boot-button presses
 *      drive CoolBoot -> RebootDorado -> LoadDoradoCode, which jams
 *      Boot0 into IM and releases the Dorado.
 *   2. When the Dorado spins at Boot0Go (0o7740) after the BB has begun
 *      streaming Boot1, we swap in Bootstrap.MB, then (at 0o7717) the
 *      canonical Initial+kernel+memMisc+IfuComplex microcode.
 *   3. Initial falls through DiskHardMicrocodeBoot to EtherMicrocodeBoot;
 *      the in-process fake Pup server serves the boot-0110 world; Initial
 *      checksums it, LoadRam's it, and the Alto/Mesa emulator world runs.
 *   4. With alto_ether_boot set, the machine plays boot server: it
 *      broadcasts breath-of-life / time packets and serves NETEXEC.BOOT
 *      over EFTP, and injects the Ethernet + display task wakeups the
 *      loaded world needs.
 */

#include "machine.h"

#include "baseboard.h"
#include "cpu.h"
#include "disk.h"
#include "ethernet.h"
#include "fastio.h"
#include "io.h"
#include "mb.h"
#include "memory.h"
#include "microcode.h"
#include "pdi.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int machine_alto_dcb_chain_sane(dorado_memory *mem, uint32_t base,
                                       uint16_t dl);
static int machine_alto_display_active(dorado_memory *mem);
static int machine_ddc_display_active(dorado_machine *m);
static int machine_prefer_live_ddc_frame(dorado_machine *m,
                                         int ddc_pixels, int dcb_pixels);
static int machine_display_fifo_used(const dorado_display *d, int subtask);
static int machine_display_fb_pixels(const dorado_display *d);
static void machine_dump_lisp_display_probe(dorado_machine *m);
static void machine_dump_words_at_va(dorado_memory *mem, const char *label,
                                     uint32_t va, int n);
static void machine_dump_lisp_atom_probe(dorado_machine *m);
static void machine_dump_env_storage_words(dorado_machine *m);

/* Default firmware/microcode locations, relative to the dorado/ dir. */
#define DEF_BB_ROM    "../chm/dorado/doradobaserom.mb!13"
#define DEF_BOOTSTRAP "../chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb"
#define DEF_INITIAL   "../chm/dorado/expanded/bootstrap.dm!20_/Initial.mb"
#define DEF_KERNEL    "../chm/dorado/expanded/kernel.dm!38_/kernel.mb"
#define DEF_MEMMISC   "../chm/dorado/expanded/memMisc.dm!11_/memMisc.mb"
#define DEF_IFU       "../chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb"
#define DEF_ETH_110   "../chm/microcode/AltoMesaDorado.eb!1"
#define DEF_EFTP      "../chm/bootfiles/NETEXEC.BOOT!8"

/* Boot-button press schedule (BaseBoard cycles). */
#define T_PRESS1_DOWN 1000000u
#define T_PRESS1_UP   1400000u
#define T_PRESS2_DOWN 2000000u
#define T_PRESS2_UP   2400000u
#define T_PRESS3_DOWN 3000000u
#define T_PRESS3_UP   3400000u

struct dorado_machine {
    /* mb_file sources must outlive the microcode that references their
     * symbol tables. */
    mb_file bs_mb;
    mb_file initial_mb, kernel_mb, memmisc_mb, ifu_mb;

    dorado_microcode mc;       /* the live control store           */
    dorado_microcode bs_mc;    /* Bootstrap.MB (BB-streamed loader) */
    dorado_microcode init_mc;  /* canonical Initial+kernel+...      */

    dorado_baseboard bb;
    dorado_cpu cpu;
    dorado_memory mem;
    dorado_io io;
    dorado_display display;
    dorado_disk_controller disk;
    dorado_ethernet ethernet;
    dorado_disk_pack disk_pack;
    dorado_disk_pack disk_packs[DORADO_DISK_NUM_DRIVES]; /* --disk SLOT=PATH */
    uint8_t          disk_pack_loaded[DORADO_DISK_NUM_DRIVES];
    dorado_pdi pilot_pdi[DORADO_DISK_NUM_DRIVES];
    dorado_fastio_router fastio;

    int disk_attached;
    uint8_t pilot_pdi_loaded[DORADO_DISK_NUM_DRIVES];
    int pilot_pdi_stream_active;
    uint32_t pilot_pdi_next_page;
    int alto_ether_boot;
    int disk_real;             /* drive the real disk controller (plan D4) */
    int alto_ether_quote;
    uint16_t boot_file_number;
    uint8_t  alto_cold_ac_done; /* one-shot: cold-Alto ACs at DiskBoot (experiment) */

    /* Boot-key chord held down through Stage-2 boot selection (see
     * dorado_machine_config.boot_keys). Resolved at create time. */
    dorado_display_key boot_chord[8];
    int      boot_chord_count;

    /* Boot state machine. */
    int      pressed;
    int      swapped;
    int      bb_strobes_pre_swap;
    uint16_t pre_swap_cpreg;
    int      initial_canonical_loaded;
    int      initial_substituted;
    int      checksum_and_load_seen;
    int      identity_map_shimmed;
    uint64_t ether_loaded_world_cycle;
    uint64_t next_bol_cycle;
    uint64_t next_display_scanline_cycle;
    int      keys_live;        /* 1 once boot selection is done and the
                                * frontend's live keys drive the world */
    int      divprot_set;      /* 1 once the divide-vector guard is armed */
    int      mouse_present;    /* 1 once the frontend has set the mouse */
    int      mouse_x, mouse_y; /* absolute mouse position (Alto coords) */
    int      mouse_buttons;    /* DORADO_MOUSE_* bitmask                */

    uint32_t pchist[4096];
    uint32_t pchist_all[4096]; /* every task (DORADO_MACHINE_PCHIST) */
    uint32_t ifu_pcx_hist[65536];
    uint32_t ifu_op_hist[1024];
    struct {
        uint64_t cycle;
        uint16_t pcx, pcf;
        uint8_t  insset, opcode, alpha, beta, len;
        uint16_t T, Q, Cnt, StkP, RBase, MemBase;
        uint16_t stk[8];
        uint16_t rm[8];
        uint32_t br31, br36;
    } ifu_ring[64];
    unsigned ifu_ring_next;
    uint16_t initseq[600];     /* first task-0 PCs after world-load */
    int      initseq_n;

    /* Pilot germ plant (Route B). When germ_path is set, the germ file
     * image is loaded here at create and fed to the real PilotBoot disk
     * read passes the first time the Cedar germ-boot disk-transfer spin
     * is reached. */
    uint16_t germ_words[16384]; /* Dorado.germ!4 = 32 pages; !6/Dorado10 larger */
    int      germ_word_count;
    int      germ_passes;      /* # of DiskBootSoft IOCB passes completed */
    int      germ_descriptor_done; /* pass 1 (descriptor) completed       */
    int      germ_label_done;      /* pass 2 (label) completed            */
    int      germ_data_done;       /* pass 3 (germ file) completed        */
    int      germ_netboot;
    uint16_t germ_netboot_bfn;
    int      germ_netboot_seeded;
    int      germ_netboot_diag_done;
    int      germ_netboot_header_seeded;
    /* A BSP file packet completed through the Cedar EthernetOne CSB bridge.
     * Keep the one-packet fake wire closed until RecvInner has reposted an
     * IOCB, observed via ControllerStatusBlock.lastInput. */
    uint8_t  stp_direct_wait_repost;
    uint16_t stp_direct_last_input;
    int      pilot_timer_started;
    uint64_t next_pilot_timer_cycle;
    uint64_t next_cedar_field_cycle; /* next display vertical-field notify */
};

static uint32_t machine_pchist_task[16][4096];
/* PDI media is normally an ephemeral host attachment.  This one-process path
 * table supports the explicit DORADO_PDI_SAVE diagnostic without changing the
 * machine snapshot ABI. */
static char machine_pdi_path[DORADO_DISK_NUM_DRIVES][512];

static void machine_store_va(dorado_memory *mem, uint32_t va, uint16_t value);

static uint16_t machine_disk_dmux_read(uint16_t addr, int *handled, void *ctx)
{
    return dorado_disk_controller_dmux_read((dorado_disk_controller *)ctx,
                                           addr, handled);
}

/* Pilot germ resident VM base (Dorado.loadmap GERM FILE MAP: file page
 * W -> VM word 0o17401000 + W, contiguous, no leader page). This is the
 * FINAL resident location, produced by PilotBoot.GERMREMAP. The microcode
 * first reads the germ into the low-64K buffer at BootDataPtr=baseGerm
 * (0o1000), validates the PV descriptor, then GERMREMAP relocates it. */
#define GERM_VM_BASE 017401000u

/* PilotBoot / DiskBootSoft / PilotDiskDefs constants (octal).
 * IOCB is built at (R400)+31 = 0o431 with MemBase=IOBR (base 0), so the
 * absolute VA of each field == 0o431 + field offset from PilotDiskDefs.mc. */
#define IOCB_BASE_VA      0431u
#define IOCB_SEAL_VA      0432u   /* IOCB.seal       (offset 1)  */
#define IOCB_PAGECOUNT_VA 0434u   /* IOCB.pageCount  (offset 3)  */
#define IOCB_COMMAND_VA   0435u   /* IOCB.command    (offset 4)  */
#define IOCB_LABELSTAT_VA 0453u   /* IOCB.labelStatus(offset 22) */
#define IOCB_DATAPTR_VA   0454u   /* IOCB.dataPtr lo (offset 23) */

#define IOCB_SEAL_VALUE   0125377u /* IOCBSealValue (PilotDiskDefs.mc)    */

/* DiskBootSoft.mc disk commands distinguishing the three read passes. */
#define DISK_CMD_DESCRIPTOR 0274u   /* [check,read,read], descriptor->page 0 */
#define DISK_CMD_LABEL      0260u   /* [check,read,none], first-page label    */
#define DISK_CMD_GERMDATA   0100254u/* incrementDataPtr|[check,check,read]    */
#define DISK_CMD_ACTION_MASK 077777u
/* DiskHeadDorado.mesa: DiskCommand has header, label, and data Action fields
 * in hardware bit positions 8..9, 10..11, and 12..13 respectively.  Mesa
 * bit 0 is the numeric high bit, hence these masks/shifts in a host word. */
#define DISK_CMD_HEADER_ACTION(cmd) (((cmd) >> 6) & 3u)
#define DISK_CMD_LABEL_ACTION(cmd)  (((cmd) >> 4) & 3u)
#define DISK_CMD_DATA_ACTION(cmd)   (((cmd) >> 2) & 3u)
#define DISK_ACTION_NONE  0u
#define DISK_ACTION_WRITE 1u
#define DISK_ACTION_CHECK 2u
#define DISK_ACTION_READ  3u
/* PilotDiskDefs.mc: Lab.fileFlags occupies the low three bits of word 7.
 * PilotDisk.mc/KSectorDone clears it after every successfully transferred
 * page while retaining the File.type in the rest of that word. */
#define PILOT_LABEL_FILE_FLAGS 0007u

/* PilotBoot.mc: baseGerm = BootSwap.countSkip*wordsPerPage = 0o1000.
 * GermBoot sets BootDataPtr_ baseGerm, so pass 3 reads the germ into the
 * low-64K buffer starting here. */
#define GERM_LOW_BUFFER   01000u

/* PilotBoot.mc / BootSwapGerm.mesa boot request layout. pRequest is a
 * MDS-relative POINTER TO Request at 0o1360; after GERMREMAP the germ MDS
 * is bank 0o76 (VA base 0o17400000). */
#define GERM_MDS_BASE          (GERM_VM_BASE - GERM_LOW_BUFFER)
#define GERM_REQUEST_VA        (GERM_MDS_BASE + 01360u)
#define GERM_REQ_ACTION        0u
#define GERM_REQ_DEVICE_TYPE   1u
#define GERM_REQ_DEVICE_ORD    2u
#define GERM_REQ_ETH_BFN       3u
#define GERM_REQ_ETH_NET       4u
#define GERM_REQ_ETH_HOST      5u

#define GERM_ACT_INLOAD        0u
#define GERM_ACT_BOOT_PV       2u
#define GERM_DTYPE_SA4000      3u
#define GERM_DTYPE_ETHERNET    5u
#define GERM_PAGE_BUFFER_VA    017422400u
#define GERM_IOCB_ARENA_BASE   0177200u
#define GERM_IOCB_ARENA_LIMIT  0177400u

/* EthernetOneHeadDorado.mesa fixes the controller status block at
 * LONG[177600B]. The microcode also exposes an IOBR-side alias around
 * off=0177520B; BootChannelEther's Mesa code uses this LONG address. */
#define GERM_ETH_CSB_VA        0177600u
#define GERM_DISK_CSB_VA       0177520u
#define ETH_CSB_NEXT_INPUT     0000u
#define ETH_CSB_IN_INTERRUPT   0001u
#define ETH_CSB_LAST_INPUT     0004u
#define ETH_CSB_NEXT_OUTPUT    0010u
#define ETH_CSB_OUT_INTERRUPT  0011u
#define ETH_IOCB_NEXT          0000u
#define ETH_IOCB_COMPLETION    0001u
#define ETH_IOCB_USED          0002u
#define ETH_IOCB_LOAD          0003u
#define ETH_IOCB_WORDS         0004u
#define ETH_IOCB_BUFFER        0005u
#define ETH_COMPLETION_DONE    000400u
/* PilotMesaProcess.mc: TimerChanMask = 100000B.  This is intentionally not
 * AltoMesaProcess.mc's 20B timer channel: Cedar's Pilot process scheduler
 * tests the sign bit in MesaInterrupt before entering CheckForTimeouts.
 * Inject the Pilot-specific source-defined channel at display-field cadence
 * once the germ has loaded the world. */
#define PILOT_TIMER_CHAN_MASK       0100000u
#define PILOT_TIMER_INTERVAL_CYCLES 277778ull

/* Cedar/Pilot display vertical-field (retrace) interrupt cadence. The LF
 * monitor TerminalHeadDorado configures runs at ~60 fields/s; at the 60 ns
 * Dorado cycle that is ~277778 cycles per field. SimpleTerminalImpl's
 * keyboard watcher (ProcessKeyboard) blocks on this retrace notify, so the
 * cadence sets how often it samples the keyboard. */
#define CEDAR_FIELD_INTERVAL_CYCLES 277778ull

/* Cedar/Pilot KeyBits (TerminalDefs.KeyBits) live at absolute LONG[177033B]
 * (TerminalHeadDorado.mesa: keyboard _ LOOPHOLE[LONG[177033B]]). It is a
 * PACKED ARRAY KeyName OF DownUp (down=0, up=1 -> all-up = 0177777). The
 * KeyName enum places VAL 16..79 as exactly the four Alto keyboard words, in
 * the same bit order as our Alto matrix, so words 177034..177037 take our
 * active-low keyboard_words[0..3] verbatim. Word 177033 (VAL 0..15) carries
 * Pen/Keyset and the mouse buttons (Red=left=bit2, Blue=right=bit1,
 * Yellow=middle=bit0). CSB.wakeupMask (the vertical-field naked-notify
 * channel) lives one word past the DCB chain head at LONG[421B]. */
#define CEDAR_KEYBITS_VA       0177033u
#define CEDAR_CSB_WAKEMASK_VA  0421u

#define PDA_READY       0000u
#define PDA_COUNT       0001u
#define PDA_STATE       0010u
#define PDA_INTERRUPT   0020u
#define PDA_FAULT       0060u
#define PDA_PSBS        0100u
#define PSB_LINK        0000u
#define PSB_FLAGS       0001u
#define PSB_CONTEXT     0002u
#define PSB_TIMEOUT     0003u
#define SIZE_PSB        0004u

/* DiskHeadDorado IOCB layout (os-src/DiskHeadDorado.mesa). */
#define SA_IOCB_NEXT           0u
#define SA_IOCB_SEAL           1u
#define SA_IOCB_DRIVE          2u
#define SA_IOCB_PAGECOUNT      3u
#define SA_IOCB_COMMAND        4u
#define SA_IOCB_DISKADDR       5u
#define SA_IOCB_DISKHEADER     7u
#define SA_IOCB_HEADERPTR      011u
#define SA_IOCB_HEADERSTATUS   015u
#define SA_IOCB_LABELPTR       016u
#define SA_IOCB_LABELSTATUS    022u
#define SA_IOCB_DATAPTR        023u
#define SA_IOCB_DATASTATUS     027u
#define SA_IOCB_DISKLABEL      030u

/* DiskHeadDorado.mesa: DoradoOperation = [operation(0), iocb(15B)].
 * DiskFace.mesa: Operation.labelPtr is the client's LONG POINTER TO Label at
 * word 2B (words 2B and 3B). */
#define SA_DORADO_OPERATION_IOCB   015u
#define SA_OPERATION_LABELPTR      002u

/* The germ's private, polled boot chain stores a flat PDI VDA in the two
 * DiskAddress words.  Once Pilot starts the interrupt-driven disk head, those
 * words are a real Dorado CHS address on every drive. */
#define PILOT_DISK_CYLINDERS   815u
#define PILOT_DISK_HEADS       5u
#define PILOT_DISK_SECTORS     28u

static uint32_t machine_pilot_disk_address_to_vda(uint16_t drive,
                                                   uint16_t cylinder,
                                                   uint16_t head_sector,
                                                   int flat)
{
    uint32_t head = (head_sector >> 8) & 0377u;
    uint32_t sector = head_sector & 0377u;
    if (flat)
        return (uint32_t)cylinder + (uint32_t)head_sector;
    if (sector >= PILOT_DISK_SECTORS) return UINT32_MAX;
    /* FileImpl.ComputeVMBackingLocation calls the Dorado an
     * Alto-environment-compatible disk: cylinder changes before head.  The
     * boot drive's low address range therefore advances cylinder every
     * 28 sectors.  Pilot reports later drives in conventional head-first
     * order in the IOCBs observed during volume discovery. */
    if (drive == 0) {
        /* PilotDiskDefs.mc specifies the Alto-compatible boot drive as
         * 4075 (= 5 * 815) virtual cylinders with one head.  Its IOCB
         * cylinder field is therefore a virtual cylinder and legitimately
         * exceeds a physical Trident's 815-cylinder limit.  Rejecting it at
         * 815 made File's later cache allocations read no PDI page and turn
         * otherwise valid BCD headers into Loader.Error[invalid BCD]. */
        if (cylinder >= PILOT_DISK_CYLINDERS * PILOT_DISK_HEADS)
            return UINT32_MAX;
        return (uint32_t)cylinder * PILOT_DISK_SECTORS + sector;
    }
    if (head >= PILOT_DISK_HEADS || cylinder >= PILOT_DISK_CYLINDERS)
        return UINT32_MAX;
    return ((uint32_t)cylinder * PILOT_DISK_HEADS + head) *
           PILOT_DISK_SECTORS + sector;
}

static void machine_pilot_disk_address_advance(uint16_t drive,
                                                uint16_t *cylinder,
                                                uint16_t *head_sector,
                                                uint16_t pages,
                                                int flat)
{
    if (flat) {
        *head_sector = (uint16_t)(*head_sector + pages);
        return;
    }
    if (drive == 0) {
        uint32_t sector = (*head_sector & 0377u) + pages;
        *cylinder = (uint16_t)(*cylinder + sector / PILOT_DISK_SECTORS);
        *head_sector = (uint16_t)(sector % PILOT_DISK_SECTORS);
        return;
    }
    uint32_t head = (*head_sector >> 8) & 0377u;
    uint32_t sector = (*head_sector & 0377u) + pages;
    head += sector / PILOT_DISK_SECTORS;
    sector %= PILOT_DISK_SECTORS;
    *cylinder = (uint16_t)(*cylinder + head / PILOT_DISK_HEADS);
    head %= PILOT_DISK_HEADS;
    *head_sector = (uint16_t)((head << 8) | sector);
}

static const uint8_t standard_alufm[ALUFM_SIZE] = {
    025, 000, 014, 054, 062, 022, 035, 027,
    023, 031, 040, 036, 013, 033, 001, 006,
};

static int machine_ethernet_plausible_iocb(const dorado_machine *m,
                                           uint32_t iocb_va)
{
    if (!m || iocb_va == 0 || iocb_va >= GERM_ETH_CSB_VA)
        return 0;
    if (iocb_va + 6u >= m->mem.storage_words)
        return 0;
    return 1;
}

static uint32_t machine_germ_disk_csb_iocb(dorado_memory *mem,
                                           uint16_t *next_out,
                                           uint16_t *interrupt_mask_out)
{
    uint16_t next = dorado_visible_word_at_va(mem, GERM_DISK_CSB_VA);
    uint16_t interrupt_mask =
        dorado_visible_word_at_va(mem, GERM_DISK_CSB_VA + 1u);
    if (next_out) *next_out = next;
    if (interrupt_mask_out) *interrupt_mask_out = interrupt_mask;
    /* PilotDiskDefs.mc: CSB.next is interpreted only if it is odd. Word 1 is
     * CSB.interruptMask, not a pointer high half. */
    return (next & 1u) ? next : 0;
}

static void machine_pilot_timer_channel(dorado_machine *m, dorado_cpu *cpu,
                                        dorado_baseboard *bb, uint16_t pre_pc,
                                        int is_imfetch)
{
    static int stop_init = 0;
    static uint64_t stop_at = 0;
    static int prereq_reported = 0;
    static int status_reported = 0;
    /* Diagnostic only: distinguish a queue/scheduler defect from the
     * host-injected 60 Hz Pilot timer.  The real path remains the default;
     * a Cedar snapshot taken after login can safely continue without this
     * synthetic source while investigating one reschedule window. */
    if (dorado_trace_flag("DORADO_NO_PILOT_TIMER")) return;
    if (!stop_init) {
        const char *stop = getenv("DORADO_PILOT_TIMER_STOP_AT");
        stop_at = (stop && *stop) ? strtoull(stop, NULL, 0) : 0;
        stop_init = 1;
    }
    if (stop_at && bb->cycles >= stop_at) return;
    if (!m->germ_word_count || !m->germ_data_done ||
        !m->ether_loaded_world_cycle) {
        /* Snapshot continuations depend on these three Route-B markers.
         * Log the first failed prerequisite so an apparently idle Cedar
         * scheduler is not mistaken for a guest-side deadlock. */
        if (dorado_trace_flag("DORADO_PILOT_TIMER_TRACE") &&
            !prereq_reported) {
            fprintf(stderr,
                    "[pilot-timer] disabled: germ_words=%d data_done=%d "
                    "world_cycle=%llu bb_cycle=%llu cpu_cycle=%llu\n",
                    m->germ_word_count, m->germ_data_done,
                    (unsigned long long)m->ether_loaded_world_cycle,
                    (unsigned long long)bb->cycles,
                    (unsigned long long)cpu->cycles);
            prereq_reported = 1;
        }
        return;
    }

    if (dorado_trace_flag("DORADO_PILOT_TIMER_TRACE") &&
        !status_reported) {
        fprintf(stderr,
                "[pilot-timer] armed: bb_cycle=%llu next=%llu cpu_cycle=%llu "
                "started=%d nww=0o%o\n",
                (unsigned long long)bb->cycles,
                (unsigned long long)m->next_pilot_timer_cycle,
                (unsigned long long)cpu->cycles, m->pilot_timer_started,
                cpu->RM[0] & 0177777);
        status_reported = 1;
    }

    (void)is_imfetch;

    if (!m->pilot_timer_started) {
        m->pilot_timer_started = 1;
        m->next_pilot_timer_cycle = bb->cycles;
    }

    /* PilotMesaProcess.mc's BusyWait declares an idle processor with a
     * nonzero WDC a RescheduleError.  Keep this deliberately opt-in while
     * isolating the earlier WDC corruption: it lets an otherwise valid
     * saved Cedar state take its pending timer interrupt instead of spinning
     * forever in MTrap. */
    if (dorado_trace_flag("DORADO_PILOT_WDC_RECOVER") &&
        cpu->RM[1] == 0 && cpu->RM[6] != 0 &&
        (cpu->RM[0] & PILOT_TIMER_CHAN_MASK)) {
        if (dorado_trace_flag("DORADO_PILOT_TIMER_TRACE")) {
            fprintf(stderr,
                    "[pilot-timer] recovering idle WDC=0o%o @cyc=%llu\n",
                    cpu->RM[6] & 0177777,
                    (unsigned long long)bb->cycles);
        }
        cpu->RM[6] = 0;
    }

    /* Diagnostic companion to the WDC recovery.  The Pilot scheduler's
     * Ready queue is a circular list rooted by PDA.ready.  A preempted PSB
     * at handle 0100 with a self link, a state-vector context, and waiting
     * clear is runnable by definition; leaving PDA.ready nil in that state
     * violates PilotMesaProcess.mc's IdleReschedule invariant. */
    if (dorado_trace_flag("DORADO_PILOT_READY_RECOVER") &&
        cpu->RM[1] == 0) {
        uint32_t pda = dorado_br_get(&m->mem, 3);
        uint32_t psb = pda + PDA_PSBS;
        uint16_t ready = dorado_visible_word_at_va(&m->mem, pda + PDA_READY);
        uint16_t link = dorado_visible_word_at_va(&m->mem, psb + PSB_LINK);
        uint16_t flags = dorado_visible_word_at_va(&m->mem, psb + PSB_FLAGS);
        uint16_t context = dorado_visible_word_at_va(&m->mem,
                                                      psb + PSB_CONTEXT);
        if (ready == 0 && (link & 07774u) == PDA_PSBS &&
            (link & 1u) != 0 && (flags & 0002u) == 0 && context != 0) {
            machine_store_va(&m->mem, pda + PDA_READY, PDA_PSBS);
            if (dorado_trace_flag("DORADO_PILOT_TIMER_TRACE")) {
                fprintf(stderr,
                        "[pilot-timer] recovering detached ready PSB 0o100 "
                        "@cyc=%llu context=0o%o\n",
                        (unsigned long long)bb->cycles, context);
            }
        }
    }

    if (bb->cycles < m->next_pilot_timer_cycle) return;

    if ((cpu->RM[0] & PILOT_TIMER_CHAN_MASK) == 0) {
        cpu->RM[0] |= PILOT_TIMER_CHAN_MASK;
        if (dorado_trace_flag("DORADO_PILOT_TIMER_TRACE")) {
            fprintf(stderr,
                    "[pilot-timer] NWW|=TimerChan @cyc=%llu pc=0o%o "
                    "next=%llu\n",
                    (unsigned long long)bb->cycles, pre_pc,
                    (unsigned long long)(bb->cycles +
                        PILOT_TIMER_INTERVAL_CYCLES));
        }
    }
    do {
        m->next_pilot_timer_cycle += PILOT_TIMER_INTERVAL_CYCLES;
    } while (m->next_pilot_timer_cycle <= bb->cycles);
}

static void restore_standard_alufm(dorado_microcode *mc)
{
    for (int i = 0; i < ALUFM_SIZE; i++) {
        mc->alufm[i] = standard_alufm[i];
        mc->alufm_present[i] = 1;
    }
}

/* Coherently write one word at a VA: through the map to physical
 * storage and into any matching cache line. Used to seed the Alto
 * keyboard words the loaded world polls. */
static void machine_store_va(dorado_memory *mem, uint32_t va, uint16_t value)
{
    if (!mem || !mem->storage) return;
    static int phys_trace_parsed = 0;
    static unsigned long phys_trace_lo = 0, phys_trace_hi = 0;
    if (!phys_trace_parsed) {
        const char *env = getenv("DORADO_MACHINE_STORE_TRACE_PHYS");
        phys_trace_parsed = 1;
        if (env && env[0]) {
            char *end = NULL;
            phys_trace_lo = strtoul(env, &end, 0);
            if (end && *end == ',') {
                phys_trace_hi = strtoul(end + 1, &end, 0);
                if (!end || *end != '\0') phys_trace_hi = 0;
            }
        }
    }
    if (dorado_trace_flag("DORADO_MACHINE_STORE_TRACE") &&
        (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE")) &&
        va >= 0100u && va <= 0117u) {
        fprintf(stderr,
                "MACHINE_STORE cyc=%llu va=%06o value=%06o br31=%07o br36=%07o\n",
                (unsigned long long)dorado_trace_cycle, va & 017777777u,
                value & 0177777u, dorado_br_get(mem, 031),
                dorado_br_get(mem, 036));
    }
    /* Deliver the word through the Map when the entry holds a real
     * translation.  The old behavior also wrote storage[va] (raw VA as a
     * physical address) unconditionally, a relic of identity-mapped
     * bring-up.  Once Pilot owns the Map that raw write corrupts whatever
     * physical page happens to share the VA's number: FS-cache IOCB
     * deliveries around VA 0o3757200 destroyed the germ's credentials
     * strings (physical page of VM bank 62) and killed the Cedar loadee
     * START phase.  A vacant entry has no translation (its rp field is
     * Pilot's software word, NOT a page number), so only then fall back to
     * the legacy raw write for pre-map planting. */
    uint32_t idx = dorado_map_index(va);
    const dorado_map_entry *e = dorado_map_get(mem, idx);
    int vacant = e->wp && e->dirty;
    if (vacant) {
        if ((size_t)va < mem->storage_words) {
            if (phys_trace_hi &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE")) &&
                va >= phys_trace_lo && va <= phys_trace_hi) {
                fprintf(stderr,
                        "MACHINE_STORE_PHYS direct cyc=%llu va=%07o phys=%07o "
                        "value=%06o br31=%07o br36=%07o\n",
                        (unsigned long long)dorado_trace_cycle,
                        va & 017777777u, va & 017777777u, value & 0177777u,
                        dorado_br_get(mem, 031), dorado_br_get(mem, 036));
            }
            mem->storage[va] = value;
        }
    } else {
        size_t phys = (size_t)e->rp * DM_PAGE_SIZE + (va & (DM_PAGE_SIZE - 1));
        if (phys < mem->storage_words) {
            if (phys_trace_hi &&
                (dorado_trace_gate || !dorado_trace_flag("DORADO_TRACE_GATE")) &&
                phys >= phys_trace_lo && phys <= phys_trace_hi) {
                fprintf(stderr,
                        "MACHINE_STORE_PHYS mapped cyc=%llu va=%07o idx=%04X "
                        "rp=%04X phys=%07o value=%06o br31=%07o br36=%07o\n",
                        (unsigned long long)dorado_trace_cycle,
                        va & 017777777u, idx, e->rp, (unsigned)phys,
                        value & 0177777u, dorado_br_get(mem, 031),
                        dorado_br_get(mem, 036));
            }
            mem->storage[phys] = value;
        }
    }

    uint32_t row = (va >> 4) & DM_CACHE_ROW_MASK;
    uint32_t tag = va >> 10;
    uint32_t off = va & DM_CACHE_LINE_MASK;
    for (int way = 0; way < DM_CACHE_WAYS; way++) {
        dorado_cache_line *line = &mem->cache[row].ways[way];
        if (line->valid && line->tag == tag) line->data[off] = value;
    }
}

/* Host-side input shortcut: update the named virtual/absolute cell and any
 * resident cache copy, but do not follow the guest Map to backing physical
 * storage.  Lisp can temporarily map its Dandelion-style IOPage onto RP 0;
 * using machine_store_va() there corrupts low-core external links such as
 * M[0100] when all-up keyboard words are refreshed. */
static void machine_store_host_input(dorado_memory *mem, uint32_t va,
                                     uint16_t value)
{
    dorado_memory_host_io_write(mem, va, value);
}

static int alto_rda_to_vda(uint16_t rda, int cylinders, int sectors, int *vda)
{
    int cylinder = (rda >> 3) & 0777;
    int head = (rda >> 2) & 1;
    int sector = (rda >> 12) & 017;
    int edrive = (rda >> 1) & 1;

    if ((rda & 1) || cylinder >= cylinders || sector >= sectors)
        return 0;

    *vda = (((edrive * cylinders + cylinder) * 2 + head) * sectors) + sector;
    return 1;
}

static void machine_trace_alto_kcb_completion(dorado_machine *m,
                                              uint16_t status_addr,
                                              uint64_t cycles)
{
    if (!m || !dorado_trace_flag("DORADO_ALTO_KCB_TRACE")) return;

    uint16_t kptr = (uint16_t)(status_addr - 1u);
    uint16_t status = dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 1u));
    uint16_t command = dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 2u));
    uint16_t header_addr =
        dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 3u));
    uint16_t label_addr =
        dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 4u));
    uint16_t data_addr =
        dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 5u));
    uint16_t normal_wakeup =
        dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 6u));
    uint16_t error_wakeup =
        dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 7u));
    uint16_t disk_addr =
        dorado_visible_word_at_va(&m->mem, (uint16_t)(kptr + 011u));
    uint16_t label[8];
    for (uint16_t i = 0; i < 8; i++)
        label[i] = dorado_visible_word_at_va(&m->mem,
                                             (uint16_t)(label_addr + i));

    int vda = -1;
    int valid_vda = alto_rda_to_vda(disk_addr, 406, 14, &vda);
    if (!valid_vda)
        (void)alto_rda_to_vda((uint16_t)(disk_addr & 0177776u), 406, 14, &vda);

    const char *filter = getenv("DORADO_ALTO_KCB_TRACE_FILTER");
    if (filter && filter[0]) {
        unsigned long want = strtoul(filter, NULL, 0);
        int match = ((uint16_t)vda == (uint16_t)want);
        for (uint16_t i = 0; i < 8 && !match; i++)
            match = (label[i] == (uint16_t)want);
        if (!match)
            return;
    }

    fprintf(stderr,
            "[alto-kcb] cyc=%llu kptr=0o%06o status=0o%06o command=0o%06o "
            "headerAddr=0o%06o labelAddr=0o%06o dataAddr=0o%06o "
            "wake={0o%06o,0o%06o} diskAddr=0o%06o vda=%d "
            "label={0o%06o,0o%06o,0o%06o,0o%06o,0o%06o,0o%06o,0o%06o,0o%06o}\n",
            (unsigned long long)cycles, kptr, status, command, header_addr,
            label_addr, data_addr, normal_wakeup, error_wakeup, disk_addr,
            vda, label[0], label[1], label[2], label[3], label[4], label[5],
            label[6], label[7]);
}

static void machine_dump_words(dorado_memory *mem, const char *label,
                               uint32_t va, int count)
{
    fprintf(stderr, "[machine] %s @0o%o:", label, va);
    for (int i = 0; i < count; i++) {
        fprintf(stderr, " %06o",
                dorado_visible_word_at_va(mem, va + (uint32_t)i));
    }
    fprintf(stderr, "\n");
}

static void machine_dump_pilot_pda(dorado_machine *m)
{
    dorado_memory *mem = &m->mem;
    uint32_t pda = dorado_br_get(mem, 3);
    uint16_t ready = dorado_visible_word_at_va(mem, pda + PDA_READY);
    uint16_t count = dorado_visible_word_at_va(mem, pda + PDA_COUNT);
    /* Normal final-state diagnostics keep this bounded, but a fault queue
     * stores a PDA-relative handle.  The Cedar pager lives beyond the first
     * 64 PSBs (for example at 02414B), so DORADO_PDA_DUMP_ALL must really
     * traverse the complete declared array rather than silently omitting the
     * process which a fault condition names. */
    uint16_t n = dorado_trace_flag("DORADO_PDA_DUMP_ALL")
               ? count : (count > 64 ? 64 : count);

    fprintf(stderr,
            "[pilot-pda] PDA=0o%o ready=0o%o count=0o%o currentPSB=0o%o "
            "NWW=0o%o RM[0..31]:",
            pda, ready, count, m->cpu.RM[1] & 0177777,
            m->cpu.RM[0] & 0177777);
    for (int i = 0; i < 32; i++)
        fprintf(stderr, " %02o:%06o", i, m->cpu.RM[i] & 0177777);

    fprintf(stderr, "\n[pilot-pda] state:");
    for (uint32_t i = 0; i < 8; i++) {
        fprintf(stderr, " p%u=0o%o", (unsigned)i,
                dorado_visible_word_at_va(mem, pda + PDA_STATE + i));
    }

    fprintf(stderr, "\n[pilot-pda] interrupt CVs:");
    for (uint32_t i = 0; i < 16; i++) {
        uint32_t cv = pda + PDA_INTERRUPT + i * 2u;
        uint16_t cond = dorado_visible_word_at_va(mem, cv);
        uint16_t extra = dorado_visible_word_at_va(mem, cv + 1u);
        if (cond || extra)
            fprintf(stderr, " i%02u=[0o%o,0o%o]", (unsigned)i, cond, extra);
    }

    fprintf(stderr, "\n[pilot-pda] fault queues:");
    for (uint32_t i = 0; i < 8; i++) {
        uint32_t f = pda + PDA_FAULT + i * 2u;
        uint16_t q = dorado_visible_word_at_va(mem, f);
        uint16_t cv = dorado_visible_word_at_va(mem, f + 1u);
        if (q || cv)
            fprintf(stderr, " f%u=[0o%o,0o%o]", (unsigned)i, q, cv);
    }

    fprintf(stderr, "\n[pilot-pda] PSBs:");
    for (uint32_t i = 0; i < n; i++) {
        uint32_t handle = PDA_PSBS + i * SIZE_PSB;
        uint32_t psb = pda + handle;
        uint16_t link = dorado_visible_word_at_va(mem, psb + PSB_LINK);
        uint16_t flags = dorado_visible_word_at_va(mem, psb + PSB_FLAGS);
        uint16_t context = dorado_visible_word_at_va(mem, psb + PSB_CONTEXT);
        uint16_t timeout = dorado_visible_word_at_va(mem, psb + PSB_TIMEOUT);
        int waiting = (flags & 2u) != 0;
        int interesting = waiting || context || timeout ||
            handle == (ready & 07774u) ||
            handle == (m->cpu.RM[1] & 07774u) ||
            dorado_trace_flag("DORADO_PDA_DUMP_ALL");
        if (interesting) {
            fprintf(stderr,
                    " h=0o%03o link=0o%06o(pri=%u next=0o%03o vec=%u) "
                    "flags=0o%06o(wait=%u abort=%u clean=0o%03o) "
                    "ctx=0o%06o timeout=0o%06o;",
                    handle, link, (unsigned)((link & 070000u) >> 12),
                    link & 07774u, link & 1u, flags,
                    (unsigned)waiting,
                    (unsigned)(flags & 1u), flags & 07774u,
                    context, timeout);
        }
    }
    fprintf(stderr, "\n");
}

static void machine_germ_complete_ethernet_rx(dorado_machine *m,
                                              uint16_t iocb_va)
{
    dorado_ethernet *eth = &m->ethernet;
    if (!eth->rx_words || eth->rx_pos != 0 || eth->rx_count == 0) return;
    if (!machine_ethernet_plausible_iocb(m, iocb_va)) return;

    /* EthernetOneDriver.QueueInput waits on the naked condition named by
     * CSB.inInterruptBit.  A non-NIL-looking nextInput with a zero mask is
     * an inactive/stale IOCB while the driver reshapes its input chain; a
     * completion there is invisible to Cedar and drops the Pup.  Hardware
     * would leave the packet at the receiver until a live input chain is
     * armed, so keep our one-packet wire buffer intact as well. */
    uint16_t interrupt_mask = dorado_visible_word_at_va(
        &m->mem, GERM_ETH_CSB_VA + ETH_CSB_IN_INTERRUPT);
    if (interrupt_mask == 0) {
        if (dorado_trace_flag("DORADO_ETH_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] germ EthernetOne direct RX hold: "
                    "iocb=0o%o has no inInterruptBit\n", iocb_va);
        }
        return;
    }

    uint16_t completion =
        dorado_visible_word_at_va(&m->mem, iocb_va + ETH_IOCB_COMPLETION);
    if (completion != 0) return;

    uint16_t length =
        dorado_visible_word_at_va(&m->mem, iocb_va + ETH_IOCB_WORDS);
    uint16_t buffer_lo =
        dorado_visible_word_at_va(&m->mem, iocb_va + ETH_IOCB_BUFFER);
    uint16_t buffer_hi =
        dorado_visible_word_at_va(&m->mem, iocb_va + ETH_IOCB_BUFFER + 1u);
    int is_eftp = eth->rx_count > 3 &&
        (eth->rx_words[3] == DORADO_PUP_TYPE_EFTP_DATA ||
         eth->rx_words[3] == DORADO_PUP_TYPE_EFTP_END);
    /* EthernetOneDriver.RecvInner queues exactly one input IOCB, wakes the
     * network receiver, then waits until the driver has consumed and reposted
     * that buffer before it queues the next one.  Apply that hardware FIFO
     * backpressure to the whole STP exchange, not only a BCD body: a
     * HereIsPList mark/data/EOC triple can otherwise consume three of the
     * five IOCBs before STP has entered its confirmation callback. */
    int is_stp_pup = !is_eftp && eth->ftp_enabled && eth->ftp_open;
    uint16_t used = (uint16_t)eth->rx_count;
    if (!is_eftp && used >= 2)
        used = (uint16_t)(used - 2); /* omit hardware CRC/status trailer */
    if (dorado_trace_flag("DORADO_CSB_TRACE") || dorado_trace_flag("DORADO_ETH_IOCB_TRACE")) {
        fprintf(stderr,
                "[machine] germ direct RX candidate: "
                "type=0o%o seq=0o%o packet_words=0o%zx "
                "eftp_state=%u eftp_seq=%u pos=%zu/%zu "
                "iocb=0o%o completion=0o%o length=0o%o "
                "buffer=0o%o used=0o%o%s\n",
                eth->rx_count > 3 ? eth->rx_words[3] : 0,
                eth->rx_count > 5 ? eth->rx_words[5] : 0,
                eth->rx_count, eth->eftp_state, eth->eftp_seq,
                eth->eftp_pos, eth->eftp_len, iocb_va, completion,
                length, ((uint32_t)buffer_hi << 16) | buffer_lo,
                used, is_eftp ? " eftp" : "");
    }
    if (length == 0 || (buffer_lo == 0 && buffer_hi == 0)) return;
    if (length < used) {
        if (dorado_trace_flag("DORADO_CSB_TRACE")) {
            fprintf(stderr,
                    "[machine] germ direct RX wait: packet_words=0o%zx "
                    "used=0o%o posted_length=0o%o\n",
                    eth->rx_count, used, length);
        }
        return;
    }

    uint16_t status = ETH_COMPLETION_DONE;

    uint32_t buffer_va = ((uint32_t)buffer_hi << 16) | buffer_lo;
    for (uint16_t i = 0; i < used; i++)
        machine_store_va(&m->mem, buffer_va + i, eth->rx_words[i]);

    machine_store_va(&m->mem, iocb_va + ETH_IOCB_COMPLETION, status);
    machine_store_va(&m->mem, iocb_va + ETH_IOCB_USED, used);
    if (dorado_visible_word_at_va(&m->mem,
                                  GERM_ETH_CSB_VA + ETH_CSB_NEXT_INPUT) ==
        iocb_va) {
        uint16_t next_iocb =
            dorado_visible_word_at_va(&m->mem, iocb_va + ETH_IOCB_NEXT);
        machine_store_va(&m->mem, GERM_ETH_CSB_VA + ETH_CSB_NEXT_INPUT,
                         next_iocb);
    }
    m->cpu.RM[0] |= interrupt_mask;
    m->cpu.reschedule_pending = 1;

    if (dorado_trace_flag("DORADO_CSB_TRACE") || dorado_trace_flag("DORADO_ETH_IOCB_TRACE")) {
        fprintf(stderr,
                "[machine] germ EthernetOne direct RX complete: "
                "iocb=0o%o buffer=0o%o used=0o%o status=0o%o "
                "NWW|=0o%o\n",
                iocb_va, buffer_va, used, status, interrupt_mask);
    }

    free(eth->rx_words);
    free(eth->rx_attention);
    eth->rx_words = NULL;
    eth->rx_attention = NULL;
    eth->rx_count = 0;
    eth->rx_pos = 0;
    eth->rx_hold = 0;
    if (is_stp_pup) {
        /* QueueInput writes CSB.lastInput after it has accepted an input
         * buffer.  Require that repost before exposing another STP Pup: the
         * real hardware's DMA/FIFO provides this backpressure but the old
         * host shortcut could otherwise consume all five IOCBs in a few host
         * instructions.  This includes the short Yes/text/EOC tail, which
         * is otherwise able to overwrite itself after the final data Pup. */
        m->stp_direct_last_input = dorado_visible_word_at_va(
            &m->mem, GERM_ETH_CSB_VA + ETH_CSB_LAST_INPUT);
        m->stp_direct_wait_repost = 1;
        eth->ftp_delivery_blocked = 1;
        if (dorado_trace_flag("DORADO_ETH_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] Cedar direct RX waits for repost after "
                    "IOCB=0o%o lastInput=0o%o\n",
                    iocb_va, m->stp_direct_last_input);
        }
    }
}

static void machine_germ_complete_ethernet_tx(dorado_machine *m)
{
    dorado_ethernet *eth = &m->ethernet;
    uint16_t iocb_va = dorado_visible_word_at_va(
        &m->mem, GERM_ETH_CSB_VA + ETH_CSB_NEXT_OUTPUT);
    if (!machine_ethernet_plausible_iocb(m, iocb_va)) return;
    uint16_t completion = dorado_visible_word_at_va(
        &m->mem, (uint32_t)iocb_va + ETH_IOCB_COMPLETION);
    if (completion != 0) return;

    uint16_t words = dorado_visible_word_at_va(
        &m->mem, (uint32_t)iocb_va + ETH_IOCB_WORDS);
    uint16_t buffer_lo = dorado_visible_word_at_va(
        &m->mem, (uint32_t)iocb_va + ETH_IOCB_BUFFER);
    uint16_t buffer_hi = dorado_visible_word_at_va(
        &m->mem, (uint32_t)iocb_va + ETH_IOCB_BUFFER + 1u);
    uint32_t buffer_va = ((uint32_t)buffer_hi << 16) | buffer_lo;
    if (words == 0 || buffer_va == 0) return;
    if (words > 512u) words = 512u;

    uint16_t pkt[512];
    for (uint16_t i = 0; i < words; i++)
        pkt[i] = dorado_visible_word_at_va(&m->mem, buffer_va + i);
    dorado_ethernet_direct_transmit(eth, pkt, words);

    machine_store_va(&m->mem, (uint32_t)iocb_va + ETH_IOCB_COMPLETION,
                     ETH_COMPLETION_DONE);
    machine_store_va(&m->mem, (uint32_t)iocb_va + ETH_IOCB_LOAD, 1);
    if (dorado_visible_word_at_va(&m->mem,
                                  GERM_ETH_CSB_VA + ETH_CSB_NEXT_OUTPUT) ==
        iocb_va) {
        uint16_t next_iocb = dorado_visible_word_at_va(
            &m->mem, (uint32_t)iocb_va + ETH_IOCB_NEXT);
        machine_store_va(&m->mem, GERM_ETH_CSB_VA + ETH_CSB_NEXT_OUTPUT,
                         next_iocb);
    }
    {
        uint16_t interrupt_mask = dorado_visible_word_at_va(
            &m->mem, GERM_ETH_CSB_VA + ETH_CSB_OUT_INTERRUPT);
        if (interrupt_mask) {
            m->cpu.RM[0] |= interrupt_mask;
            m->cpu.reschedule_pending = 1;
        }
        if (dorado_trace_flag("DORADO_ETH_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] EthernetOne direct TX complete: "
                    "iocb=0o%o buffer=0o%o words=0o%o type=0o%o "
                    "NWW|=0o%o\n",
                    iocb_va, buffer_va, words, words > 3 ? pkt[3] : 0,
                    interrupt_mask);
        }
    }
}

static void machine_germ_seed_ethernet_header_page(dorado_machine *m)
{
    dorado_ethernet *eth = &m->ethernet;
    if (m->germ_netboot_header_seeded) return;
    if (!eth->eftp_wait_for_rx_arm || !eth->eftp_words) return;
    if (eth->eftp_pos != DORADO_EFTP_DATA_WORDS) return;
    if (eth->eftp_len < DORADO_EFTP_DATA_WORDS * 2u) return;

    for (uint16_t i = 0; i < DORADO_EFTP_DATA_WORDS; i++) {
        machine_store_va(&m->mem, GERM_PAGE_BUFFER_VA + i,
                         eth->eftp_words[DORADO_EFTP_DATA_WORDS + i]);
    }
    m->germ_netboot_header_seeded = 1;
    if (dorado_trace_flag("DORADO_GERM_NETBOOT_TRACE")) {
        fprintf(stderr,
                "[machine] germ seeded Ethernet header page @0o%o "
                "word0=0o%o word1=0o%o\n",
                GERM_PAGE_BUFFER_VA,
                eth->eftp_words[DORADO_EFTP_DATA_WORDS],
                eth->eftp_words[DORADO_EFTP_DATA_WORDS + 1]);
    }
}

static uint32_t machine_read_long_va(dorado_memory *mem, uint32_t va)
{
    uint16_t lo = dorado_visible_word_at_va(mem, va);
    uint16_t hi = dorado_visible_word_at_va(mem, va + 1u);
    return ((uint32_t)hi << 16) | lo;
}

static void machine_write_long_va(dorado_memory *mem, uint32_t va,
                                  uint32_t value)
{
    machine_store_va(mem, va, (uint16_t)value);
    machine_store_va(mem, va + 1u, (uint16_t)(value >> 16));
}

static void machine_germ_complete_disk_iocb(dorado_machine *m)
{
    if (!m || !m->germ_data_done) return;

    uint16_t csb_next = 0, csb_interrupt_mask = 0;
    uint32_t iocb = machine_germ_disk_csb_iocb(&m->mem,
                                               &csb_next,
                                               &csb_interrupt_mask);
    if (iocb == 0 || iocb >= m->mem.storage_words) return;
    uint16_t seal = dorado_visible_word_at_va(&m->mem, iocb + SA_IOCB_SEAL);
    if (seal != IOCB_SEAL_VALUE) {
        if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
            static uint32_t last_bad_iocb;
            static uint16_t last_bad_seal;
            if (iocb != last_bad_iocb || seal != last_bad_seal) {
                fprintf(stderr,
                        "[machine] PDI disk IOCB wait: csb=[0o%o,0o%o] "
                        "iocb=0o%o seal=0o%o\n",
                        csb_next, csb_interrupt_mask, iocb, seal);
                last_bad_iocb = iocb;
                last_bad_seal = seal;
            }
        }
        return;
    }

    uint16_t count = dorado_visible_word_at_va(&m->mem,
                                               iocb + SA_IOCB_PAGECOUNT);
    /* A sealed zero-page IOCB is a valid no-op completion in the Cedar driver.
     * It must clear its seal, but has not transferred a sector and therefore
     * must not publish its stale private diskLabel back to the client. */
    uint16_t drive = dorado_visible_word_at_va(&m->mem, iocb + SA_IOCB_DRIVE);
    dorado_pdi *pdi =
        drive < DORADO_DISK_NUM_DRIVES && m->pilot_pdi_loaded[drive]
            ? &m->pilot_pdi[drive] : NULL;
    uint16_t command = dorado_visible_word_at_va(
        &m->mem, iocb + SA_IOCB_COMMAND);
    uint16_t disk_addr_low = dorado_visible_word_at_va(
        &m->mem, iocb + SA_IOCB_DISKADDR);
    uint16_t disk_addr_high = dorado_visible_word_at_va(
        &m->mem, iocb + SA_IOCB_DISKADDR + 1u);
    if (drive == 0 && m->pilot_pdi_stream_active && csb_interrupt_mask != 0 &&
        dorado_trace_flag("DORADO_PILOT_BOOT_ONLY")) {
        /* Diagnostic mode for a transient slot-0 bootstrap volume.  Once its
         * germ/boot-file stream has completed, let Pilot discover the data
         * volume in a later slot without rejecting its cloned volume IDs as
         * duplicates of the bootstrap image. */
        pdi = NULL;
    }
    /* Every post-germ IOCB carries PilotDisk's real drive-0 DiskAddress
     * (virtual cylinder plus sector). The germ's three early disk passes are
     * planted before this bridge is enabled, so no flat-PDI convention leaks
     * into the ordinary boot-file and filesystem traffic. */
    int flat_address = 0;
    uint32_t disk_page = machine_pilot_disk_address_to_vda(
        drive, disk_addr_low, disk_addr_high, flat_address);
    uint32_t label_ptr = machine_read_long_va(&m->mem, iocb + SA_IOCB_LABELPTR);
    uint32_t iocb_label = iocb + SA_IOCB_DISKLABEL;
    uint32_t operation = iocb - SA_DORADO_OPERATION_IOCB;
    /* Dorado virtual addresses are 28 bits (the high nibble in a Mesa LONG
     * POINTER is not an address bit). */
    uint32_t client_label_ptr = machine_read_long_va(
        &m->mem, operation + SA_OPERATION_LABELPTR) & 0x0fffffffu;
    uint32_t client_label_file_page = 0;
    if (client_label_ptr && pdi && pdi->label_words >= 7) {
        client_label_file_page =
            (uint32_t)dorado_visible_word_at_va(&m->mem,
                                                client_label_ptr + 5u) |
            ((uint32_t)(dorado_visible_word_at_va(&m->mem,
                                                   client_label_ptr + 6u) &
                        0177u) << 16);
    }
    uint32_t data_ptr = machine_read_long_va(&m->mem, iocb + SA_IOCB_DATAPTR);
    uint16_t next = dorado_visible_word_at_va(&m->mem, iocb + SA_IOCB_NEXT);
    unsigned label_action = DISK_CMD_LABEL_ACTION(command);
    unsigned data_action = DISK_CMD_DATA_ACTION(command);
    /* The germ's boot chain is polled (CSB interrupt mask 0) and follows
     * its own conventions: a rusty-backup image stores a flat PDI VDA in
     * the link words (not the hardware CHS DiskAddress Pilot's
     * interrupt-driven head uses), and the germ's DiskBootTransfer
     * expectations were validated against the original flat bridge -- the
     * raw page label copied back verbatim, sequential streaming for the
     * GERMDATA command, and unconditional success.  Restore exactly those
     * semantics for polled IOCBs; the per-action, CHS, KSectorDone-faithful
     * path below remains for everything Pilot issues once it owns the
     * disk head (2e8018b). */
    if (csb_interrupt_mask == 0 && pdi) {
        uint32_t flat_page = machine_pilot_disk_address_to_vda(
            drive, disk_addr_low, disk_addr_high, 1);
        if (command == DISK_CMD_GERMDATA && disk_addr_low >= 0100u) {
            if (!m->pilot_pdi_stream_active) {
                m->pilot_pdi_stream_active = 1;
                m->pilot_pdi_next_page = flat_page;
            }
            flat_page = m->pilot_pdi_next_page;
        } else {
            /* Any other polled command (e.g. the 0o260 label read the germ
             * issues to follow a boot-file link) starts a new transfer whose
             * first GERMDATA carries a fresh, correct start address.  A
             * cursor left over from an EARLIER germ boot must not override
             * it: the stream latch used to persist for the process lifetime
             * (and through snapshot restore), so a soft reboot -- BootTool's
             * herald volume buttons -> Booting.Boot -> germ re-entry --
             * continued the old stream at the wrong page and the germ
             * halt-spun with zero disk transfers (2026-07-15 wedge). */
            m->pilot_pdi_stream_active = 0;
        }
        uint16_t polled_done = 0;
        for (; polled_done < count; polled_done++) {
            uint32_t page = flat_page + polled_done;
            const uint16_t *label, *data;
            uint16_t pl[DORADO_PILOT_LABEL_WORDS];
            uint16_t pd[DORADO_PILOT_DATA_WORDS];
            if (m->disk_real) {
                /* --disk-real: route the read through the controller. */
                int plw = pdi->label_words < DORADO_PILOT_LABEL_WORDS
                    ? pdi->label_words : DORADO_PILOT_LABEL_WORDS;
                int pdw = pdi->data_words < DORADO_PILOT_DATA_WORDS
                    ? pdi->data_words : DORADO_PILOT_DATA_WORDS;
                m->disk.selected_drive = (uint8_t)drive;
                if (dorado_disk_controller_read_page(&m->disk, page,
                                                     pl, plw, pd, pdw) != 0)
                    break;
                label = pl; data = pd;
            } else {
                label = dorado_pdi_page_label(pdi, page);
                data = dorado_pdi_page_data(pdi, page);
                if (!label || !data) break;
            }
            if (label_ptr) {
                for (uint16_t w = 0; w < pdi->label_words; w++)
                    machine_store_va(&m->mem, label_ptr + w, label[w]);
            }
            if (data_ptr) {
                uint32_t dst = data_ptr +
                    (uint32_t)polled_done * pdi->data_words;
                for (uint16_t w = 0; w < pdi->data_words; w++)
                    machine_store_va(&m->mem, dst + w, data[w]);
            }
        }
        if (m->pilot_pdi_stream_active && command == DISK_CMD_GERMDATA)
            m->pilot_pdi_next_page = flat_page + polled_done;
        if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] PDI germ polled IOCB @cyc=%llu: iocb=0o%o "
                    "cmd=0o%o page=0o%o count=0o%o done=0o%o "
                    "label=0o%o data=0o%o next=0o%o "
                    "req_lo=0o%o req_hi=0o%o stream={act=%d next=0o%o}\n",
                    (unsigned long long)m->bb.cycles, iocb, command,
                    flat_page, count, polled_done, label_ptr, data_ptr,
                    next, disk_addr_low, disk_addr_high,
                    m->pilot_pdi_stream_active,
                    (unsigned)m->pilot_pdi_next_page);
        }
        machine_store_va(&m->mem, iocb + SA_IOCB_DISKADDR, disk_addr_low);
        machine_store_va(&m->mem, iocb + SA_IOCB_DISKADDR + 1u,
                         (uint16_t)(disk_addr_high + polled_done));
        machine_store_va(&m->mem, iocb + SA_IOCB_DISKHEADER, disk_addr_low);
        machine_store_va(&m->mem, iocb + SA_IOCB_DISKHEADER + 1u,
                         disk_addr_high);
        machine_write_long_va(&m->mem, iocb + SA_IOCB_DATAPTR,
                              data_ptr + (uint32_t)polled_done *
                              (uint32_t)pdi->data_words);
        machine_store_va(&m->mem, iocb + SA_IOCB_PAGECOUNT,
                         (uint16_t)(count - polled_done));
        machine_store_va(&m->mem, iocb + SA_IOCB_HEADERSTATUS, 0);
        machine_store_va(&m->mem, iocb + SA_IOCB_LABELSTATUS, 0);
        machine_store_va(&m->mem, iocb + SA_IOCB_DATASTATUS, 0);
        machine_store_va(&m->mem, iocb + SA_IOCB_SEAL, 0);
        machine_store_va(&m->mem, GERM_DISK_CSB_VA, next);
        return;
    }
    /* PilotDisk.mc's KCheckError reports DS.checkErr (0100B).  Keep the
     * three independently reported block statuses here, just as IOCB does. */
    uint16_t header_status = pdi ? 0 : 004000u; /* DS.notOnLine */
    uint16_t label_status = header_status;
    uint16_t data_status = header_status;
    int disk_error = 0;
    /* PilotDisk.mc/KSectorDone updates IOCB.diskLabel after every successful
     * page, irrespective of the label Action.  That is deliberately distinct
     * from labelPtr: a label-read points at the client's label, but the
     * controller still advances its private IOCB copy. */
    uint32_t label_file_page = 0;
    if (pdi && pdi->label_words >= 7) {
        label_file_page =
            (uint32_t)dorado_visible_word_at_va(&m->mem, iocb_label + 5u) |
            ((uint32_t)(dorado_visible_word_at_va(&m->mem, iocb_label + 6u) &
                        0177u) << 16);
    }
    if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
        fprintf(stderr,
                "[machine] PDI disk IOCB start @cyc=%llu: iocb=0o%o cmd=0o%o "
                "page=0o%o count=0o%o diskLabel={id=0o%o/0o%o "
                "fp=0o%o attr=0o%o} clientLabel=0o%o/fp=0o%o\n",
                (unsigned long long)m->bb.cycles, iocb, command, disk_page, count,
                dorado_visible_word_at_va(&m->mem, iocb_label),
                dorado_visible_word_at_va(&m->mem, iocb_label + 1u),
                (unsigned)label_file_page,
                dorado_visible_word_at_va(&m->mem, iocb_label + 7u),
                client_label_ptr, (unsigned)client_label_file_page);
    }

    /* Optional timing probe for the direct PDI bridge.  The ordinary bridge
     * deliberately has no media delay, but a real Trident produces at most
     * one 256-word sector roughly every 9,920 Dorado cycles.  Retaining the
     * source-level one-page IOCB state while spacing sectors lets us check
     * whether a Pilot file-cache race is hiding behind the instantaneous
     * host media.  It is strictly opt-in and has no snapshot ABI state. */
    const char *sector_delay_text = getenv("DORADO_PDI_SECTOR_CYCLES");
    unsigned long sector_delay = sector_delay_text && *sector_delay_text
        ? strtoul(sector_delay_text, NULL, 0) : 0;
    if (sector_delay && pdi && count) {
        static const dorado_machine *paced_machine;
        static uint64_t next_pdi_sector_cycle;
        if (paced_machine != m || m->bb.cycles < next_pdi_sector_cycle) {
            paced_machine = m;
            next_pdi_sector_cycle = 0;
        }
        if (m->bb.cycles < next_pdi_sector_cycle)
            return;
        next_pdi_sector_cycle = m->bb.cycles + sector_delay;
    }

    /* PilotDisk.mc/KSectorDone is deliberately a one-sector state machine:
     * it updates the IOCB after each sector and returns through
     * KContinueCmmd while the seal remains set.  Completing an entire PDI
     * request atomically used to look equivalent, but it is not: File's
     * multi-page allocation/cache operations cross a 32-page boundary and
     * rely on those intermediate IOCB updates.  Mirror the controller one
     * page at a time.  The sealed IOCB remains at CSB.next until the final
     * page below, so the next machine tick naturally resumes this same
     * command without any host-only continuation state. */
    uint16_t done = 0;
    uint16_t budget = count ? 1u : 0u;
    /* --disk-real: temp buffers for the controller-mediated read (the read
     * path delivers header+label+data via the FIFO). */
    uint16_t cl[DORADO_PILOT_LABEL_WORDS];
    uint16_t cd[DORADO_PILOT_DATA_WORDS];
    int lw = pdi ? (int)pdi->label_words : DORADO_PILOT_LABEL_WORDS;
    int dw = pdi ? (int)pdi->data_words : DORADO_PILOT_DATA_WORDS;
    if (lw > DORADO_PILOT_LABEL_WORDS) lw = DORADO_PILOT_LABEL_WORDS;
    if (dw > DORADO_PILOT_DATA_WORDS)  dw = DORADO_PILOT_DATA_WORDS;
    for (; done < budget; done++) {
        uint32_t page = disk_page + done;
        const uint16_t *label, *data;
        if (!pdi) {
            break;
        } else if (m->disk_real && label_action != DISK_ACTION_WRITE &&
                   data_action != DISK_ACTION_WRITE) {
            /* Route this page's read through the real controller (FIFO path). */
            m->disk.selected_drive = (uint8_t)drive;
            if (dorado_disk_controller_read_page(&m->disk, page,
                                                 cl, lw, cd, dw) != 0)
                break;
            label = cl; data = cd;
        } else {
            label = dorado_pdi_page_label(pdi, page);
            data = dorado_pdi_page_data(pdi, page);
            if (!label || !data) break;
        }

        /* DiskHeadDorado.Initiate directs only a label-read at the client's
         * labelPtr; label writes use IOCB.diskLabel at that same pointer.
         * The PDI bridge used to copy in the read direction unconditionally.
         * Cedar's File.Create therefore kept rereading its old VAM/root page
         * for 100244B ([check,check,write]) and could never create the STP
         * cache stream.  Honor each Action field just as the controller does. */
        if (label_action == DISK_ACTION_READ && label_ptr) {
            for (uint16_t w = 0; w < pdi->label_words; w++)
                machine_store_va(&m->mem, label_ptr + w, label[w]);
        }
        if (label_action == DISK_ACTION_CHECK && label_ptr) {
            /* PilotDiskDefs.mc and PilotDisk.mc/DoDiskBlock: checking a
             * label compares its first eight words, then transfers the last
             * two unconditionally.  Those two words are the boot-chain
             * link, so dropping them is observable even when the comparison
             * succeeds. */
            uint16_t compare_words = pdi->label_words < 8u
                ? pdi->label_words : 8u;
            /* PilotDisk.mc/KSectorDone clears Lab.fileFlags (the low three
             * bits of label word 7) after every successful sector.  Cedar's
             * File code subsequently reconstructs HeaderLabel/DataLabel for
             * a later request to that same file page.  This opt-in probe
             * leaves the file type bits and all other identity fields exact,
             * but treats only the microcode-mutated flags as nonpersistent.
             * It exists to reconcile those two original-source behaviours;
             * keep the default hardware-exact comparison for regression
             * work until the compatibility result is established. */
            int ignore_label_flags =
                dorado_trace_flag("DORADO_PDI_IGNORE_LABEL_FLAGS");
            int mismatch = 0;
            for (uint16_t w = 0; w < compare_words; w++) {
                uint16_t want = dorado_visible_word_at_va(&m->mem,
                                                           label_ptr + w);
                uint16_t have = label[w];
                if (w == 7u && ignore_label_flags) {
                    want &= (uint16_t)~PILOT_LABEL_FILE_FLAGS;
                    have &= (uint16_t)~PILOT_LABEL_FILE_FLAGS;
                }
                /* Cedar-nucleus fileID label words 2-3 are File.FP's DA --
                 * a 32-bit disk-address hint (PARC_PILOT_FORMAT.md 2.1).
                 * Pilot builds its expected label from the FP it holds, so
                 * on real media the stored labels carry the hint and the
                 * hardware compare covers it.  Our converted/synthetic PDIs
                 * store DA=0 in every label, which fails FS's FP-to-label
                 * validation ("File.FP from directory/cache doesn't
                 * correspond to a local volume file").  Under the same
                 * media-compat switch, treat the hint words as dontCare. */
                if ((w == 2u || w == 3u) && ignore_label_flags)
                    continue;
                if (want != have) {
                    mismatch = 1;
                }
            }
            if (mismatch) {
                /* PilotDisk.mc/KCheckError exits the compare loop directly,
                 * before its read-the-last-two-words tail.  In particular it
                 * preserves diskLabel.bootChainLink from the last successful
                 * sector; DiskBootTransfer uses that saved link to find the
                 * next allocation run.  PageCount, diskAddress, dataPtr, and
                 * the private label remain unadvanced as well. */
                label_status |= 0100u; /* DS.checkErr */
                disk_error = 1;
                if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
                    fprintf(stderr,
                            "[machine] PDI label check mismatch: page=0o%o "
                            "iocb=0o%o label=0o%o cmd=0o%o count=0o%o "
                            "addr=[0o%o,0o%o] "
                            "want={0o%o,0o%o,0o%o,0o%o;0o%o,0o%o,0o%o,0o%o} "
                            "have={0o%o,0o%o,0o%o,0o%o;0o%o,0o%o,0o%o,0o%o} "
                            "link={want:[0o%o,0o%o] have:[0o%o,0o%o]}\n",
                            page, iocb, label_ptr, command, count,
                            disk_addr_low, disk_addr_high,
                            dorado_visible_word_at_va(&m->mem, label_ptr),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 1u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 2u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 3u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 4u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 5u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 6u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 7u),
                            label[0], label[1], label[2], label[3],
                            label[4], label[5], label[6], label[7],
                            dorado_visible_word_at_va(&m->mem, label_ptr + 8u),
                            dorado_visible_word_at_va(&m->mem, label_ptr + 9u),
                            label[8], label[9]);
                }
                break;
            }
            /* A successful check compares words 0..7 then reads the final
             * two words (normally a boot-chain link) into IOCB.diskLabel --
             * the controller's private label, not the client's.  Writing
             * them to labelPtr instead let the completion copyback replace
             * the client's fresh link with the private label's stale one. */
            if (pdi->label_words > 8u) {
                for (uint16_t w = 8u; w < pdi->label_words; w++)
                    machine_store_va(&m->mem, iocb_label + w, label[w]);
            }
        }
        if (label_action == DISK_ACTION_WRITE && label_ptr) {
            uint16_t *dst = pdi->labels + (size_t)page * pdi->label_words;
            if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
                fprintf(stderr,
                        "[machine] PDI label write: page=0o%o src=0o%o "
                        "words={0o%o,0o%o,0o%o,0o%o; fp=0o%o/0o%o, attr=0o%o}\n",
                        page, label_ptr,
                        dorado_visible_word_at_va(&m->mem, label_ptr),
                        dorado_visible_word_at_va(&m->mem, label_ptr + 1u),
                        dorado_visible_word_at_va(&m->mem, label_ptr + 2u),
                        dorado_visible_word_at_va(&m->mem, label_ptr + 3u),
                        dorado_visible_word_at_va(&m->mem, label_ptr + 5u),
                        dorado_visible_word_at_va(&m->mem, label_ptr + 6u),
                        dorado_visible_word_at_va(&m->mem, label_ptr + 7u));
            }
            for (uint16_t w = 0; w < pdi->label_words; w++)
                dst[w] = dorado_visible_word_at_va(&m->mem, label_ptr + w);
            if (pdi->label_words >= 7) {
                uint32_t file_page = label_file_page + done;
                dst[5] = (uint16_t)file_page;
                dst[6] = (uint16_t)((dst[6] & ~0177u) |
                                    ((file_page >> 16) & 0177u));
                if (done != 0)
                    dst[7] &= (uint16_t)~PILOT_LABEL_FILE_FLAGS;
            }
        }
        if (data_action == DISK_ACTION_READ && data_ptr) {
            uint32_t dst = data_ptr + (uint32_t)done * pdi->data_words;
            if (dorado_trace_flag("DORADO_PDI_BCD_TRACE") &&
                (data[0] == 0x0500u ||
                 (page >= 03500u && page < 03700u))) {
                fprintf(stderr,
                        "[machine] PDI BCD read: page=0o%o dst=0o%o "
                        "words={%04x,%04x,%04x,%04x}\n",
                        page, dst, data[0], data[1], data[2], data[3]);
            }
            for (uint16_t w = 0; w < pdi->data_words; w++)
                machine_store_va(&m->mem, dst + w, data[w]);
        }
        if (data_action == DISK_ACTION_WRITE && data_ptr) {
            uint16_t *dst = pdi->data + (size_t)page * pdi->data_words;
            uint32_t src = data_ptr + (uint32_t)done * pdi->data_words;
            if (dorado_trace_flag("DORADO_PDI_BCD_TRACE") &&
                (dorado_visible_word_at_va(&m->mem, src) == 0x0500u ||
                 (page >= 03500u && page < 03700u))) {
                const uint16_t *lab = dorado_pdi_page_label(pdi, page);
                fprintf(stderr,
                        "[machine] PDI BCD write: page=0o%o src=0o%o "
                        "words={%04x,%04x,%04x,%04x} "
                        "label={id=%04x/%04x fp=%04x/%04x attr=%04x}\n",
                        page, src,
                        dorado_visible_word_at_va(&m->mem, src),
                        dorado_visible_word_at_va(&m->mem, src + 1u),
                        dorado_visible_word_at_va(&m->mem, src + 2u),
                        dorado_visible_word_at_va(&m->mem, src + 3u),
                        lab ? lab[0] : 0, lab ? lab[1] : 0,
                        lab ? lab[5] : 0, lab ? lab[6] : 0,
                        lab ? lab[7] : 0);
            }
            if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
                fprintf(stderr,
                        "[machine] PDI data write: page=0o%o src=0o%o "
                        "words={0o%o,0o%o,0o%o,0o%o}\n",
                        page, src,
                        dorado_visible_word_at_va(&m->mem, src),
                        dorado_visible_word_at_va(&m->mem, src + 1u),
                        dorado_visible_word_at_va(&m->mem, src + 2u),
                        dorado_visible_word_at_va(&m->mem, src + 3u));
            }
            for (uint16_t w = 0; w < pdi->data_words; w++)
                dst[w] = dorado_visible_word_at_va(&m->mem, src + w);
        }
    }

    /* KSectorDone increments the private IOCB label and clears its flags;
     * DiskHeadDorado.Poll later copies that label to the client for every
     * Action except label-read. */
    if (pdi && done != 0 && pdi->label_words >= 7) {
        uint32_t next_file_page = label_file_page + done;
        uint16_t high = dorado_visible_word_at_va(&m->mem, iocb_label + 6u);
        uint16_t flags = dorado_visible_word_at_va(&m->mem, iocb_label + 7u);
        machine_store_va(&m->mem, iocb_label + 5u,
                         (uint16_t)next_file_page);
        machine_store_va(&m->mem, iocb_label + 6u,
                         (uint16_t)((high & ~0177u) |
                                    ((next_file_page >> 16) & 0177u)));
        machine_store_va(&m->mem, iocb_label + 7u,
                         (uint16_t)(flags & ~PILOT_LABEL_FILE_FLAGS));
        if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] PDI disk label advanced @cyc=%llu: "
                    "iocb=0o%o fp=0o%o attr=0o%o\n",
                    (unsigned long long)m->bb.cycles, iocb,
                    (unsigned)((uint32_t)dorado_visible_word_at_va(
                        &m->mem, iocb_label + 5u) |
                        ((uint32_t)(dorado_visible_word_at_va(
                            &m->mem, iocb_label + 6u) & 0177u) << 16)),
                    dorado_visible_word_at_va(&m->mem, iocb_label + 7u));
        }
    }

    /* DiskHeadDorado.mesa: DiskCommand.incrementDataPtr is an IOCB-only
     * control bit.  Poll copies IOCB.dataPtr back to the client, so ordinary
     * multi-page operations must retain their starting buffer address; only
     * explicit streaming transfers (the germ's 0100254B read, for example)
     * advance it.  Advancing unconditionally corrupts the caller's next
     * cache/file operation after a multi-page write. */
    uint32_t new_data_ptr = data_ptr;
    if (command & 0100000u)
        new_data_ptr += (uint32_t)done * (uint32_t)dw;
    uint16_t next_disk_addr_low = disk_addr_low;
    uint16_t next_disk_addr_high = disk_addr_high;
    /* DiskHeadDorado.Poll copies IOCB.diskHeader back to the client after a
     * completed operation.  The controller has overwritten that field with
     * the header from the last physical page it touched; retaining the
     * caller's first-page request makes a multi-page File/Create operation
     * look as though it ended on the wrong sector.  A PDI has no separate
     * hardware-header bytes, so synthesize that final DiskAddress. */
    uint16_t final_header_low = disk_addr_low;
    uint16_t final_header_high = disk_addr_high;
    if (done != 0) {
        machine_pilot_disk_address_advance(drive, &final_header_low,
                                           &final_header_high,
                                           (uint16_t)(done - 1u),
                                           flat_address);
    }
    {
        machine_pilot_disk_address_advance(drive, &next_disk_addr_low,
                                           &next_disk_addr_high, done,
                                           flat_address);
    }
    machine_store_va(&m->mem, iocb + SA_IOCB_DISKADDR,
                     next_disk_addr_low);
    machine_store_va(&m->mem, iocb + SA_IOCB_DISKADDR + 1u,
                     next_disk_addr_high);
    machine_store_va(&m->mem, iocb + SA_IOCB_DISKHEADER, final_header_low);
    machine_store_va(&m->mem, iocb + SA_IOCB_DISKHEADER + 1u,
                     final_header_high);
    machine_write_long_va(&m->mem, iocb + SA_IOCB_DATAPTR, new_data_ptr);
    machine_store_va(&m->mem, iocb + SA_IOCB_PAGECOUNT,
                     (uint16_t)(count - done));
    machine_store_va(&m->mem, iocb + SA_IOCB_HEADERSTATUS, header_status);
    machine_store_va(&m->mem, iocb + SA_IOCB_LABELSTATUS, label_status);
    machine_store_va(&m->mem, iocb + SA_IOCB_DATASTATUS, data_status);

    if (disk_error) {
        /* PilotDisk.mc/KSectorError freezes the chain by replacing CSB.next
         * with the failed IOCB's even pointer, then wakes the client. */
        machine_store_va(&m->mem, iocb + SA_IOCB_SEAL, 0);
        machine_store_va(&m->mem, GERM_DISK_CSB_VA,
                         (uint16_t)(iocb - 1u));
        if (csb_interrupt_mask) {
            m->cpu.RM[0] |= csb_interrupt_mask;
            m->cpu.reschedule_pending = 1;
        }
        return;
    }

    /* KSectorDone branches back to KContinueCmmd until pageCount reaches
     * zero.  In particular, do not publish the next IOCB or its interrupt
     * before the final sector: Pilot's DiskHead polls the sealed IOCB as the
     * completion fence. */
    if (done != 0 && done < count) {
        if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] PDI disk IOCB progress: iocb=0o%o "
                    "cmd=0o%o page=0o%o remaining=0o%o data=0o%o->0o%o\n",
                    iocb, command, disk_page, (uint16_t)(count - done),
                    data_ptr, new_data_ptr);
        }
        return;
    }

    /* DiskHeadDorado.mesa/Poll copies the controller's private diskLabel back
     * to Operation.labelPtr on completion, except after a label-read (which
     * was directed to the client pointer in the first place).  The direct PDI
     * bridge is the controller's completion boundary, so publish the same
     * value before clearing the seal.  Without this, a follow-on IOCB is
     * rebuilt from the client's stale filePage and rechecks the wrong label. */
    if (pdi && done != 0 && label_action != DISK_ACTION_READ &&
        client_label_ptr) {
        for (uint16_t w = 0; w < pdi->label_words; w++)
            machine_store_va(&m->mem, client_label_ptr + w,
                             dorado_visible_word_at_va(&m->mem,
                                                       iocb_label + w));
        if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] PDI disk label copyback: iocb=0o%o "
                    "client=0o%o fp=0o%o\n",
                    iocb, client_label_ptr,
                    (unsigned)((uint32_t)dorado_visible_word_at_va(
                        &m->mem, iocb_label + 5u) |
                        ((uint32_t)(dorado_visible_word_at_va(
                            &m->mem, iocb_label + 6u) & 0177u) << 16)));
        }
    }
    machine_store_va(&m->mem, iocb + SA_IOCB_SEAL, 0);
    machine_store_va(&m->mem, GERM_DISK_CSB_VA, next);
    if (csb_interrupt_mask) {
        m->cpu.RM[0] |= csb_interrupt_mask;
        m->cpu.reschedule_pending = 1;
        if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
            fprintf(stderr,
                    "[machine] PDI disk interrupt: NWW|=0o%o reschedule "
                    "@cyc=%llu\n",
                    csb_interrupt_mask,
                    (unsigned long long)m->bb.cycles);
        }
    }

    if (dorado_trace_flag("DORADO_DISK_IOCB_TRACE")) {
        fprintf(stderr,
                "[machine] PDI disk IOCB complete: iocb=0o%o "
                "csb=[0o%o,0o%o] drive=0o%o cmd=0o%o "
                "page=0o%o raw=[0o%o,0o%o] count=0o%o done=0o%o "
                "data=0o%o->0o%o nextPage=0o%o "
                "label=0o%o next=0o%o\n",
                iocb, csb_next, csb_interrupt_mask, drive, command, disk_page,
                disk_addr_low, disk_addr_high, count, done, data_ptr,
                new_data_ptr, m->pilot_pdi_next_page, label_ptr, next);
    }
}

static void machine_germ_netboot_diag(dorado_machine *m)
{
    dorado_cpu *cpu = &m->cpu;
    dorado_memory *mem = &m->mem;
    fprintf(stderr,
            "[machine] germ-netboot diag @cyc=%llu pc=0o%o "
            "dispatch=%llu MDS=0o%o MemBase=%02o RBase=%02o "
            "StkP=%03o T=%06o Q=%06o Cnt=%06o Link=0o%o\n",
            (unsigned long long)m->bb.cycles,
            cpu->real_PC,
            (unsigned long long)cpu->ifu_dispatch_count,
            (unsigned)dorado_br_get(mem, 031),
            cpu->MemBase & 037,
            cpu->RBase & 017,
            cpu->StkP & 0377,
            cpu->T, cpu->Q, cpu->Cnt, cpu->Link);
    machine_dump_words(mem, "pRequest", GERM_REQUEST_VA, 8);
    machine_dump_words(mem, "EthernetOne CSB", GERM_ETH_CSB_VA, 16);
    fprintf(stderr, "[machine] STK around StkP:");
    for (int d = -8; d <= 8; d++) {
        uint8_t sp = (uint8_t)(cpu->StkP + d);
        fprintf(stderr, " [%03o]=%06o", sp, cpu->STK[sp]);
    }
    fprintf(stderr, "\n");
}

/* Seed the four Alto keyboard words (base+0177034..7) at each plausible
 * base (absolute, IOBR, MDS) so the loaded world's boot-key poll sees
 * them regardless of which space it reads. */
static void machine_seed_keyboard(dorado_memory *mem, const uint16_t w[4])
{
    if (!mem || !mem->storage) return;
    uint32_t bases[] = { 0, dorado_br_get(mem, 031), dorado_br_get(mem, 036) };
    for (size_t b = 0; b < sizeof bases / sizeof bases[0]; b++) {
        for (uint32_t i = 0; i < 4; i++) {
            uint32_t va = (bases[b] + 0177034u + i) & 0x0FFFFFFFu;
            machine_store_va(mem, va, w[i]);
        }
    }
}

/* Drive the boot-key chord onto the DDC keyboard: clear all keys, then
 * hold each chord key down. Returns the four resulting active-low Alto
 * keyboard words via w[] so the caller can also seed them into the cells
 * the loaded world polls (the DDC keyboard back-channel is not modeled;
 * gap E2). With the default chord {BS} (+ Quote) this reproduces the
 * historic forced seeding byte-for-byte. */
static void machine_apply_boot_chord(dorado_display *disp,
                                     const dorado_display_key *keys, int n,
                                     uint16_t w[4])
{
    dorado_display_keyboard_all_up(disp);
    for (int i = 0; i < n; i++)
        dorado_display_keyboard_set_key(disp, keys[i], 1);
    for (int i = 0; i < 4; i++)
        w[i] = dorado_display_keyboard_word(disp, i);
}

/* Seed the Alto mouse cells: MOUSEX 0o424 / MOUSEY 0o425 (absolute
 * position) and the UTILIN button word 0o177030..0o177033 (active-low
 * ~buttons, matching ContrAlto's ~(buttons|keyset)). Written at each
 * plausible base like the keyboard words. */
static void machine_seed_mouse(dorado_memory *mem, int x, int y, int buttons)
{
    if (!mem || !mem->storage) return;
    uint16_t bw = (uint16_t)~((unsigned)buttons & 07u);
    uint32_t bases[] = { 0, dorado_br_get(mem, 031), dorado_br_get(mem, 036) };
    for (size_t b = 0; b < sizeof bases / sizeof bases[0]; b++) {
        machine_store_va(mem, (bases[b] + 0424u) & 0x0FFFFFFFu, (uint16_t)x);
        machine_store_va(mem, (bases[b] + 0425u) & 0x0FFFFFFFu, (uint16_t)y);
        for (uint32_t a = 0177030u; a <= 0177033u; a++)
            machine_store_va(mem, (bases[b] + a) & 0x0FFFFFFFu, bw);
    }
}

static void machine_seed_lisp_iopage_keyboard(dorado_memory *mem,
                                              const uint16_t w[4],
                                              int mouse_present,
                                              int mouse_buttons)
{
    if (!mem || !mem->storage) return;

    /* The Fugue StartLisp bootstrap clears 0x1403A..0x14040 once
     * (IOPage.keyBitsm1=0x39), but the later Dandelion Lisp definitions used
     * by DoradoLispMc place IOPage.key at 0x41 and keyBitsm1 at 0x3C.  After
     * the Lyric sysout loads, 0x1403A..0x14040 hold live IOPage state, so the
     * host-side terminal shortcut must refresh only the later key window. */
    const uint32_t base = 0x14041u;
    for (uint32_t i = 0; i < 4; i++)
        machine_store_host_input(mem, base + i, w[i]);

    uint16_t buttons = 0177777u;
    if (mouse_present)
        buttons = (uint16_t)~((unsigned)mouse_buttons & 07u);
    machine_store_host_input(mem, base + 4u, buttons);
    machine_store_host_input(mem, base + 5u, 0177777u);
    machine_store_host_input(mem, base + 6u, 0177777u);
}

static int machine_boot_chord_is_disk(const dorado_machine *m)
{
    return m && m->boot_chord_count == 1 &&
           m->boot_chord[0] == DORADO_KEY_NONE;
}

static void machine_seed_alto_live_io(dorado_machine *m, dorado_display *disp)
{
    if (!m || !disp) return;
    if (!m->keys_live) {
        dorado_display_keyboard_all_up(disp);
        m->keys_live = 1;
    }

    uint16_t w[4];
    for (int i = 0; i < 4; i++)
        w[i] = dorado_display_keyboard_word(disp, i);
    machine_seed_keyboard(&m->mem, w);
    machine_seed_lisp_iopage_keyboard(&m->mem, w, m->mouse_present,
                                      m->mouse_buttons);
    if (m->mouse_present)
        machine_seed_mouse(&m->mem, m->mouse_x, m->mouse_y, m->mouse_buttons);
}

static void machine_seed_lisp_live_io(dorado_machine *m, dorado_display *disp)
{
    if (!m || !disp) return;
    if (!m->keys_live) {
        dorado_display_keyboard_all_up(disp);
        m->keys_live = 1;
    }

    uint16_t w[4];
    for (int i = 0; i < 4; i++)
        w[i] = dorado_display_keyboard_word(disp, i);
    machine_seed_keyboard(&m->mem, w);

    if (dorado_trace_flag("DORADO_LISP_FORCE_KEY_MASK")) {
        /* Diagnostic: LLKEY!\KEYBOARDON sets DISPINTERRUPT.EM[020000].
         * If the loaded sysout never gets that far, the display field
         * handler posts only BcplKeyMask and the Lisp key/timer process
         * never runs. */
        uint16_t force_mask = dorado_visible_word_at_va(&m->mem, 0421u);
        machine_store_va(&m->mem, 0421u,
                         (uint16_t)(force_mask | 020000u));
    }
    if (m->mouse_present)
        machine_seed_mouse(&m->mem, m->mouse_x, m->mouse_y,
                           m->mouse_buttons);
    if (dorado_trace_flag("DORADO_LISP_KEY_TRACE") &&
        (w[0] != 0177777u || w[1] != 0177777u ||
         w[2] != 0177777u || w[3] != 0177777u)) {
        fprintf(stderr,
                "[lisp-key] cyc=%llu words=%06o %06o %06o %06o "
                "NWW=%06o dispint=%06o IOBR=%07o MDS=%07o\n",
                (unsigned long long)m->bb.cycles, w[0], w[1], w[2], w[3],
                m->cpu.RM[0],
                dorado_visible_word_at_va(&m->mem, 0421u),
                dorado_br_get(&m->mem, 031),
                dorado_br_get(&m->mem, 036));
    }
}

/* Deliver the live keyboard (and mouse-button) state to the native Cedar
 * world's KeyBits at absolute 177033 (see CEDAR_KEYBITS_VA above). The
 * 7-wire keyboard back-channel -> Cedar I/O microcode -> KeyBits path is not
 * modeled for the native world, so -- exactly as machine_seed_keyboard does
 * for the Alto-on-Dorado world at 177034 -- we write the polled cells
 * directly. w[] are the active-low Alto keyboard words; they map verbatim
 * onto KeyName words 177034..177037. */
static void machine_seed_cedar_keyboard(dorado_memory *mem, const uint16_t w[4],
                                        int mouse_present, int mouse_buttons)
{
    if (!mem || !mem->storage) return;
    uint16_t w0 = 0177777u;   /* 177033: x0..x6, Pen, Keyset1..5, mouse */
    if (mouse_present) {
        if (mouse_buttons & DORADO_MOUSE_LEFT)   w0 &= (uint16_t)~0004u; /* Red    */
        if (mouse_buttons & DORADO_MOUSE_RIGHT)  w0 &= (uint16_t)~0002u; /* Blue   */
        if (mouse_buttons & DORADO_MOUSE_MIDDLE) w0 &= (uint16_t)~0001u; /* Yellow */
    }
    machine_store_va(mem, CEDAR_KEYBITS_VA, w0);
    for (uint32_t i = 0; i < 4; i++)
        machine_store_va(mem, CEDAR_KEYBITS_VA + 1u + i, w[i]);
    /* KeyName VAL 80.. (Margins/Same/Find/... and unused) -- keep all-up. */
    machine_store_va(mem, CEDAR_KEYBITS_VA + 5u, 0177777u);
    machine_store_va(mem, CEDAR_KEYBITS_VA + 6u, 0177777u);
}

/* Once per display field for the native Cedar/Pilot world: sample the live
 * keyboard/mouse into KeyBits and post the display vertical-field (retrace)
 * naked-notify. On real hardware the keyboard arrives over the 7-wire back
 * channel and the field interrupt ORs CSB.wakeupMask (LONG[421B]) into NWW;
 * the Mesa emulator then NOTIFYs the InterruptVector condition the display
 * head registered, which is what SimpleTerminalImpl.ProcessKeyboard waits on
 * before it samples GetKeys. Because our display is rasterized in C rather
 * than by the display microcode tasks, drive both directly at field rate:
 * the keyboard sample mirrors machine_seed_keyboard's direct-write shortcut,
 * and the notify uses the RM[0] |= mask idiom the modeled Ethernet/disk
 * completions use (NWW lives in RM[0]; see machine_pilot_timer_channel). */
static void machine_cedar_io(dorado_machine *m, dorado_baseboard *bb,
                             dorado_display *disp)
{
    if (m->alto_ether_boot || !m->germ_word_count || !m->germ_data_done ||
        !m->ether_loaded_world_cycle)
        return;

    /* Cheap field-cadence gate first: the body (VM reads/writes) runs only
     * ~60x/emulated-second, so the per-microinstruction cost is just a few
     * integer comparisons. */
    if (m->next_cedar_field_cycle == 0)
        m->next_cedar_field_cycle = bb->cycles;
    if (bb->cycles < m->next_cedar_field_cycle) return;
    do {
        m->next_cedar_field_cycle += CEDAR_FIELD_INTERVAL_CYCLES;
    } while (m->next_cedar_field_cycle <= bb->cycles);

    /* CSB.dcbChainHead (LONG[420B]) is non-trivial only once Cedar's
     * TerminalHeadDorado has connected the display and the world is live. */
    uint16_t head = dorado_visible_word_at_va(&m->mem, CEDAR_CSB_WAKEMASK_VA - 1u);
    if (head == 0 || head == 0177777u) return;

    if (!m->keys_live) {            /* first live field: drop the boot chord */
        dorado_display_keyboard_all_up(disp);
        m->keys_live = 1;
    }

    uint16_t w[4];
    for (int i = 0; i < 4; i++)
        w[i] = dorado_display_keyboard_word(disp, i);
    machine_seed_cedar_keyboard(&m->mem, w, m->mouse_present, m->mouse_buttons);
    if (m->mouse_present) {
        machine_store_va(&m->mem, 0424u, (uint16_t)m->mouse_x); /* mouse.x */
        machine_store_va(&m->mem, 0425u, (uint16_t)m->mouse_y); /* mouse.y */
    }

    uint16_t mask = dorado_visible_word_at_va(&m->mem, CEDAR_CSB_WAKEMASK_VA);
    if (mask) {
        m->cpu.RM[0] |= mask;
        m->cpu.reschedule_pending = 1;
    }
    if (dorado_trace_flag("DORADO_CEDAR_FIELD_TRACE")) {
        fprintf(stderr,
                "[cedar-field] NWW|=0o%o keys=%06o,%06o,%06o,%06o @cyc=%llu\n",
                mask, w[0], w[1], w[2], w[3],
                (unsigned long long)bb->cycles);
    }
}

/* Identity-map the installed storage pages. Bring-up shim: Initial's
 * PresetMap should leave installed storage identity-mapped before
 * BootEmulator clears the first 64K and builds the Ethernet request at
 * VM 177400, but the Config'/PresetMap path still reaches the vacant-map
 * case after the NoStorage bypass. Install the map shape directly until
 * that gap is closed. */
static void machine_identity_map_storage(dorado_memory *mem)
{
    uint32_t pages = (uint32_t)(mem->storage_words / DM_PAGE_SIZE);
    if (pages > DM_MAP_ENTRIES) pages = DM_MAP_ENTRIES;
    for (uint32_t pg = 0; pg < pages; pg++) {
        dorado_map_set(mem, pg, (uint16_t)pg, /*wp=*/0, /*dirty=*/0);
    }
}

/* Find a real (physical) page not referenced by any resident Map entry.
 * Models a Pilot fault handler allocating a free real page to back a
 * faulting VM page. Returns the real-page number, or 0xFFFF if none. */
static uint16_t machine_find_free_rp(const dorado_memory *mem)
{
    uint32_t total = (uint32_t)(mem->storage_words / DM_PAGE_SIZE);
    if (total == 0) return 0xFFFF;
    if (total > 0x10000u) total = 0x10000u; /* rp is 16-bit */
    /* Mark every real page that a resident map entry uses. */
    static uint8_t used[0x10000];
    memset(used, 0, total);
    for (uint32_t i = 0; i < DM_MAP_ENTRIES; i++) {
        const dorado_map_entry *e = dorado_map_get(mem, i);
        if (e->wp && e->dirty) continue;          /* vacant -- ignore */
        if (e->rp < total) used[e->rp] = 1;
    }
    /* Allocate from the top so we avoid the low identity-mapped pages. */
    for (uint32_t rp = total; rp-- > 0; )
        if (!used[rp]) return (uint16_t)rp;
    return 0xFFFF;
}

void dorado_machine_config_default(dorado_machine_config *cfg)
{
    if (!cfg) return;
    memset(cfg, 0, sizeof *cfg);
    cfg->bb_rom       = DEF_BB_ROM;
    cfg->bootstrap_mb = DEF_BOOTSTRAP;
    cfg->initial_mb   = DEF_INITIAL;
    cfg->kernel_mb    = DEF_KERNEL;
    cfg->memmisc_mb   = DEF_MEMMISC;
    cfg->ifu_mb       = DEF_IFU;
    cfg->eth_boot_110 = DEF_ETH_110;
    cfg->eftp_boot    = DEF_EFTP;
    cfg->alto_ether_boot = 1;
    cfg->alto_ether_quote = 0;
    cfg->no_disk = 1;
    cfg->storage_modules = 1;
    cfg->boot_file_number = 0110;
}

static const char *pick(const char *v, const char *def)
{
    return (v && *v) ? v : def;
}

dorado_machine *dorado_machine_create(const dorado_machine_config *user_cfg)
{
    dorado_machine_config cfg;
    dorado_machine_config_default(&cfg);
    if (user_cfg) {
        cfg.bb_rom       = pick(user_cfg->bb_rom,       cfg.bb_rom);
        cfg.bootstrap_mb = pick(user_cfg->bootstrap_mb, cfg.bootstrap_mb);
        cfg.initial_mb   = pick(user_cfg->initial_mb,   cfg.initial_mb);
        cfg.kernel_mb    = pick(user_cfg->kernel_mb,    cfg.kernel_mb);
        cfg.memmisc_mb   = pick(user_cfg->memmisc_mb,   cfg.memmisc_mb);
        cfg.ifu_mb       = pick(user_cfg->ifu_mb,       cfg.ifu_mb);
        cfg.eth_boot_110 = pick(user_cfg->eth_boot_110, cfg.eth_boot_110);
        cfg.eftp_boot    = pick(user_cfg->eftp_boot,    cfg.eftp_boot);
        cfg.ftp_sysout   = pick(user_cfg->ftp_sysout,   cfg.ftp_sysout);
        cfg.ftp_root     = pick(user_cfg->ftp_root,     cfg.ftp_root);
        cfg.germ_path    = pick(user_cfg->germ_path,    cfg.germ_path);
        cfg.disk_real    = user_cfg->disk_real;
        for (int s = 0; s < 4; s++) {
            cfg.pilot_disk_pdi[s] = pick(user_cfg->pilot_disk_pdi[s],
                                         cfg.pilot_disk_pdi[s]);
            cfg.disk_pack[s] = pick(user_cfg->disk_pack[s], cfg.disk_pack[s]);
        }
        cfg.germ_netboot = user_cfg->germ_netboot;
        cfg.germ_netboot_bfn = user_cfg->germ_netboot_bfn;
        cfg.alto_ether_boot  = user_cfg->alto_ether_boot;
        cfg.alto_ether_quote = user_cfg->alto_ether_quote;
        cfg.no_disk          = user_cfg->no_disk;
        if (user_cfg->storage_modules)
            cfg.storage_modules = user_cfg->storage_modules;
        if (user_cfg->boot_file_number)
            cfg.boot_file_number = user_cfg->boot_file_number;
        cfg.boot_dir_count = user_cfg->boot_dir_count;
        for (int i = 0; i < user_cfg->boot_dir_count &&
                        i < (int)(sizeof cfg.boot_dir / sizeof cfg.boot_dir[0]);
             i++)
            cfg.boot_dir[i] = user_cfg->boot_dir[i];
        cfg.boot_dir_all = user_cfg->boot_dir_all;
        cfg.boot_keys_count = user_cfg->boot_keys_count;
        for (int i = 0; i < user_cfg->boot_keys_count &&
                        i < (int)(sizeof cfg.boot_keys / sizeof cfg.boot_keys[0]);
             i++)
            cfg.boot_keys[i] = user_cfg->boot_keys[i];
    }
    if (cfg.storage_modules < 1 || cfg.storage_modules > 4)
        cfg.storage_modules = 1;

    dorado_machine *m = calloc(1, sizeof *m);
    if (!m) return NULL;
    m->alto_ether_boot  = cfg.alto_ether_boot;
    m->disk_real        = cfg.disk_real;
    m->alto_ether_quote = cfg.alto_ether_quote;
    m->boot_file_number = cfg.boot_file_number;
    m->germ_netboot     = cfg.germ_netboot;
    m->germ_netboot_bfn = cfg.germ_netboot_bfn;
    m->pre_swap_cpreg   = 0;

    /* Resolve the boot-key chord. An explicit chord (from the frontend's
     * --boot-keys / --boot-reason) is used verbatim; otherwise default to
     * BS, and fold in Quote iff alto_ether_quote -- byte-identical to the
     * historic forced BS-down (+Quote) seeding the regression gate locks
     * in. (Booting memo: BS selects the Ethernet software boot, Quote
     * NetExec.) */
    m->boot_chord_count = 0;
    if (cfg.boot_keys_count > 0) {
        for (int i = 0; i < cfg.boot_keys_count &&
                        m->boot_chord_count <
                            (int)(sizeof m->boot_chord / sizeof m->boot_chord[0]);
             i++)
            m->boot_chord[m->boot_chord_count++] = cfg.boot_keys[i];
    } else {
        m->boot_chord[m->boot_chord_count++] = DORADO_KEY_BS;
        if (cfg.alto_ether_quote)
            m->boot_chord[m->boot_chord_count++] = DORADO_KEY_QUOTE;
    }

    /* Bootstrap.MB -> bs_mc (the loader the BB streams in). */
    mb_init(&m->bs_mb);
    if (mb_load(&m->bs_mb, cfg.bootstrap_mb) != MB_OK ||
        dorado_microcode_load(&m->bs_mb, &m->bs_mc) != DM_OK) {
        fprintf(stderr, "dorado: cannot load Bootstrap.MB (%s)\n",
                cfg.bootstrap_mb);
        free(m);
        return NULL;
    }

    /* Live control store starts with the canonical ALUFM convention so
     * Boot0 has working ALU semantics from the first instruction. */
    memset(&m->mc, 0, sizeof m->mc);
    restore_standard_alufm(&m->mc);

    /* Canonical Initial = kernel + memMisc + IfuComplex + Initial,
     * layered (kernel first so Initial overrides conflicts). */
    mb_init(&m->kernel_mb);
    mb_init(&m->memmisc_mb);
    mb_init(&m->ifu_mb);
    mb_init(&m->initial_mb);
    if (mb_load(&m->kernel_mb, cfg.kernel_mb) == MB_OK)
        dorado_microcode_load(&m->kernel_mb, &m->init_mc);
    if (mb_load(&m->memmisc_mb, cfg.memmisc_mb) == MB_OK)
        dorado_microcode_layer_load(&m->memmisc_mb, &m->init_mc);
    if (mb_load(&m->ifu_mb, cfg.ifu_mb) == MB_OK)
        dorado_microcode_layer_load(&m->ifu_mb, &m->init_mc);
    if (mb_load(&m->initial_mb, cfg.initial_mb) == MB_OK &&
        dorado_microcode_layer_load(&m->initial_mb, &m->init_mc) == DM_OK) {
        m->initial_canonical_loaded = 1;
    }
    if (!m->initial_canonical_loaded) {
        fprintf(stderr, "dorado: cannot load canonical Initial microcode\n");
        free(m);
        return NULL;
    }

    /* BaseBoard. */
    baseboard_init(&m->bb);
    if (baseboard_load_rom(&m->bb, cfg.bb_rom) != 0) {
        fprintf(stderr, "dorado: cannot load BaseBoard ROM (%s)\n", cfg.bb_rom);
        free(m);
        return NULL;
    }
    baseboard_reset(&m->bb);

    /* Microengine. */
    dorado_cpu_init(&m->cpu, &m->mc, 0);
    /* The CPU core currently collapses the three-cycle processor pipeline.
     * Its same-instruction Md bypass compatibility is required by
     * DoradoLispMc, but is not the real previous-instruction bypass described
     * by HM pp.6 and 77. Applying it to Cedar's planted-germ Mesa world
     * corrupts Pilot before TerminalHeadDorado installs its DCB. */
    if (cfg.germ_path)
        m->cpu.compat_same_instr_md_bypass = 0;
    m->cpu.baseboard = &m->bb;
    m->cpu.baseboard_cycles_per_uop = 1;

    /* Memory + I/O + devices. Resize storage to the requested module
     * count, independent of the DORADO_STORAGE_MODULES env var that
     * dorado_memory_init() consults — a library shouldn't depend on
     * process environment for correctness. */
    if (dorado_memory_init(&m->mem) != 0) {
        fprintf(stderr, "dorado: memory init failed\n");
        free(m);
        return NULL;
    }
    {
        m->mem.storage_chip_type = DM_STORAGE_CHIP_TYPE_DEFAULT;
        size_t want = (size_t)DM_STORAGE_MODULE_WORDS *
                      (size_t)cfg.storage_modules;
        if (m->mem.storage_words != want) {
            uint16_t *ns = calloc(want, sizeof(uint16_t));
            if (!ns) {
                fprintf(stderr, "dorado: storage alloc failed\n");
                free(m->mem.storage);
                free(m);
                return NULL;
            }
            free(m->mem.storage);
            m->mem.storage = ns;
            m->mem.storage_words = want;
        }
    }
    dorado_io_init(&m->io);
    dorado_display_init(&m->display);
    dorado_disk_controller_init(&m->disk);
    dorado_ethernet_init(&m->ethernet);
    if (cfg.germ_path) {
        /* Route B's Cedar/Pilot germ uses EthernetOneHeadDorado's CSB
         * (LONG[177600B]) instead of Alto EICLOC. Model the in-process
         * EFTP server like a retransmitting boot server: make each packet
         * visible only once the germ has posted an input IOCB. */
        m->ethernet.eftp_wait_for_rx_arm = 1;
        m->ethernet.eftp_turnaround_ticks = 0;
    }
    dorado_ethernet_set_boot_file(&m->ethernet, cfg.boot_file_number,
                                  cfg.eth_boot_110);
    dorado_ethernet_set_eftp_boot_file(&m->ethernet, cfg.eftp_boot);
    dorado_ethernet_set_ftp_sysout(&m->ethernet, cfg.ftp_sysout);
    dorado_ethernet_set_ftp_root(&m->ethernet, cfg.ftp_root);
    for (int s = 0; s < DORADO_DISK_NUM_DRIVES; s++) {
        if (!cfg.pilot_disk_pdi[s]) continue;
        char err[128];
        if (dorado_pdi_load(cfg.pilot_disk_pdi[s], &m->pilot_pdi[s],
                            err, sizeof err) != 0) {
            fprintf(stderr, "dorado: cannot load Pilot disk %d '%s': %s\n",
                    s, cfg.pilot_disk_pdi[s], err);
        } else {
            m->pilot_pdi_loaded[s] = 1;
            snprintf(machine_pdi_path[s], sizeof machine_pdi_path[s], "%s",
                     cfg.pilot_disk_pdi[s]);
            dorado_disk_controller_attach_pdi(&m->disk, s, &m->pilot_pdi[s]);
            m->disk.allow_pdi_timing = (uint8_t)m->disk_real;  /* D4 */
            m->disk_attached = 1;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] Pilot PDI mounted in slot %d: %s "
                        "(%u pages)\n", s, cfg.pilot_disk_pdi[s],
                        (unsigned)m->pilot_pdi[s].page_count);
        }
    }

    /* --disk SLOT=PATH: mount real Trident pack image(s), R/W. Geometry is
     * auto-detected from the file size (T-80 then T-300). A missing file is
     * created as a blank T-80 (so a writable target exists for Othello /
     * volume install -- plan D6); writes are flushed back on machine_destroy. */
    for (int s = 0; s < DORADO_DISK_NUM_DRIVES; s++) {
        if (!cfg.disk_pack[s]) continue;
        int ok = 0;
        FILE *probe = fopen(cfg.disk_pack[s], "rb");
        if (probe) {
            fclose(probe);
            ok = (dorado_disk_pack_load(&m->disk_packs[s],
                                        &DORADO_DISK_T80, cfg.disk_pack[s]) == 0);
            if (!ok)
                ok = (dorado_disk_pack_load(&m->disk_packs[s],
                                        &DORADO_DISK_T300, cfg.disk_pack[s]) == 0);
            if (!ok)
                ok = (dorado_disk_pack_load(&m->disk_packs[s],
                                        &DORADO_DISK_DIABLO, cfg.disk_pack[s]) == 0);
            if (!ok)
                fprintf(stderr, "dorado: '%s' is not a T-80/T-300/Diablo-Trident "
                        "pack (slot %d)\n", cfg.disk_pack[s], s);
        } else if (dorado_disk_pack_create(&m->disk_packs[s],
                                           &DORADO_DISK_T80) == 0) {
            snprintf(m->disk_packs[s].path, sizeof m->disk_packs[s].path,
                     "%s", cfg.disk_pack[s]);
            dorado_disk_pack_save(&m->disk_packs[s]);   /* persist the blank */
            ok = 1;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] created blank T-80 pack: %s\n",
                        cfg.disk_pack[s]);
        }
        if (!ok) continue;
        m->disk_pack_loaded[s] = 1;
        dorado_disk_controller_attach_drive(&m->disk, s, &m->disk_packs[s]);
        m->disk_attached = 1;
        if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
            fprintf(stderr, "[machine] Trident pack mounted on slot %d: %s "
                    "(%dx%dx%d)\n", s, cfg.disk_pack[s],
                    m->disk_packs[s].geometry.cylinders,
                    m->disk_packs[s].geometry.heads,
                    m->disk_packs[s].geometry.sectors);
    }

    /* Register the boot-file directory entries the fake server advertises
     * to NetExec. Each is "NAME=BFN=PATH" (BFN octal). NAME must end in
     * ".boot"; PATH may itself contain no '=' (file paths here do not). */
    for (int i = 0; i < cfg.boot_dir_count; i++) {
        const char *spec = cfg.boot_dir[i];
        if (!spec) continue;
        const char *eq1 = strchr(spec, '=');
        if (!eq1) { fprintf(stderr, "dorado: bad --boot-dir '%s'\n", spec);
                    continue; }
        const char *eq2 = strchr(eq1 + 1, '=');
        if (!eq2) { fprintf(stderr, "dorado: bad --boot-dir '%s'\n", spec);
                    continue; }
        char name[48];
        size_t nlen = (size_t)(eq1 - spec);
        if (nlen >= sizeof name) nlen = sizeof name - 1;
        memcpy(name, spec, nlen);
        name[nlen] = '\0';
        uint16_t bfn = (uint16_t)strtoul(eq1 + 1, NULL, 8);
        const char *path = eq2 + 1;
        dorado_ethernet_add_boot_dir(&m->ethernet, bfn, name, path);
    }

    /* --boot-dir-all: auto-register every Alto B-format *.boot file in the
     * directory holding the EFTP boot file (chm/bootfiles/), so NetExec's
     * menu lists all the games. Explicit --boot-dir entries above take
     * precedence (auto-register skips names already present). */
    if (cfg.boot_dir_all && cfg.eftp_boot) {
        char dir[256];
        snprintf(dir, sizeof dir, "%s", cfg.eftp_boot);
        char *slash = strrchr(dir, '/');
        if (slash) *slash = '\0';
        else snprintf(dir, sizeof dir, ".");
        int n = dorado_ethernet_add_boot_dir_all(&m->ethernet, dir);
        if (dorado_trace_flag("DORADO_BOOTDIR_DEBUG"))
            fprintf(stderr, "[bootdir] --boot-dir-all: registered %d game(s) "
                    "from %s (%d entries total)\n", n, dir,
                    m->ethernet.bootdir_count);
    }

    /* Route B germ plant: load the Pilot germ image for later deposit into
     * VM. The germ file holds big-endian 16-bit words: the Dorado memory
     * word is (first byte << 8) | second byte. Proof: byte-swapping the
     * germ's GFT entries (MDS 0o1400+) yields EXACTLY the global-frame
     * addresses in Dorado.loadmap (gfi1->0o3400 ProcessorHead,
     * gfi4->0o4634 BootSwapGerm, ...), and BootSwapGerm's code-base-high
     * word byte-swaps to 0o76 = pilotMDSHi. Reading little-endian made
     * GFT[gfi] point past the loaded germ, so the boot Xfer through
     * SD[sBoot] took an sUnbound trap (microcode-grounded: DMesaXfer.mc
     * LoadGC/XferProc). Failure is non-fatal -- it just disables the plant. */
    if (cfg.germ_path) {
        FILE *gf = fopen(cfg.germ_path, "rb");
        if (!gf) {
            fprintf(stderr, "dorado: cannot open germ '%s'; plant disabled\n",
                    cfg.germ_path);
        } else {
            int n = 0;
            int b0, b1;
            while (n < (int)(sizeof m->germ_words / sizeof m->germ_words[0]) &&
                   (b0 = fgetc(gf)) != EOF && (b1 = fgetc(gf)) != EOF) {
                m->germ_words[n++] = (uint16_t)((b0 << 8) | b1);
            }
            fclose(gf);
            m->germ_word_count = n;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] germ loaded: %d words from %s "
                        "(word0=0o%o)\n", n, cfg.germ_path,
                        n ? m->germ_words[0] : 0);
        }
    }

    dorado_display_attach_to_io(&m->display, &m->io);
    dorado_disk_controller_attach_to_io(&m->disk, &m->io);
    dorado_ethernet_attach_to_io(&m->ethernet, &m->io);
    dorado_fastio_router_init(&m->fastio, &m->display, &m->disk);
    m->mem.fast_io_cb = dorado_fastio_dispatch;
    m->mem.fast_io_ctx = &m->fastio;
    m->mem.dmux_cb = machine_disk_dmux_read;
    m->mem.dmux_ctx = &m->disk;
    m->cpu.mem = &m->mem;
    m->cpu.io = &m->io;

    return m;
}

void dorado_machine_destroy(dorado_machine *m)
{
    if (!m) return;
    dorado_ethernet_free(&m->ethernet);
    for (int s = 0; s < DORADO_DISK_NUM_DRIVES; s++) {
        if (!m->pilot_pdi_loaded[s]) continue;
        if (getenv("DORADO_PDI_SAVE") && machine_pdi_path[s][0]) {
            char err[128];
            if (dorado_pdi_save(machine_pdi_path[s], &m->pilot_pdi[s], err,
                                sizeof err) != 0)
                fprintf(stderr, "dorado: cannot save Pilot disk %d '%s': %s\n",
                        s, machine_pdi_path[s], err);
        }
        dorado_pdi_free(&m->pilot_pdi[s]);
    }
    if (m->disk_attached && m->disk_pack.sectors)
        dorado_disk_pack_free(&m->disk_pack);
    for (int s = 0; s < DORADO_DISK_NUM_DRIVES; s++) {
        if (!m->disk_pack_loaded[s]) continue;
        /* Flush writes back to the pack image if anything was modified. */
        dorado_disk_pack *p = &m->disk_packs[s];
        int dirty = 0;
        for (int i = 0; i < p->num_sectors && !dirty; i++)
            if (p->sectors[i].modified) dirty = 1;
        if (dirty && !p->read_only && p->path[0])
            dorado_disk_pack_save(p);
        dorado_disk_pack_free(p);
    }
    mb_free(&m->bs_mb);
    mb_free(&m->kernel_mb);
    mb_free(&m->memmisc_mb);
    mb_free(&m->ifu_mb);
    mb_free(&m->initial_mb);
    free(m);
}

uint64_t dorado_machine_run_until(dorado_machine *m, uint64_t until_cycle)
{
    if (!m) return 0;
    dorado_baseboard *bb = &m->bb;
    dorado_cpu *cpu = &m->cpu;
    dorado_display *disp = &m->display;
    dorado_ethernet *eth = &m->ethernet;

    /* The vendored 6502 core reaches its memory callbacks through the
     * global `baseboard_active` (set at create/reset). With snapshot/
     * restore there can be several machines alive at once (e.g. a booted
     * original + a restored experiment); point the 6502 at the machine
     * we are about to step so running one never drives another's BB. */
    baseboard_active = bb;

    while (bb->cycles < until_cycle && !cpu->halted) {
        /* Trace-gate cycle window (env DORADO_TRACE_GATE="lo,hi"), so the
         * standalone binary can drive the same gated IFUDISP/BR/store
         * traces as the test harness. Trace-only; no behavioral effect. */
        {
            static long tg_lo = -1, tg_hi = -1;
            extern int dorado_trace_gate;
            extern unsigned long long dorado_trace_cycle;
            dorado_trace_cycle = bb->cycles;
            if (tg_lo == -1) {
                const char *w = getenv("DORADO_TRACE_GATE");
                tg_lo = 0; tg_hi = 0;
                if (w) sscanf(w, "%ld,%ld", &tg_lo, &tg_hi);
            }
            dorado_trace_gate =
                (tg_hi && bb->cycles >= (uint64_t)tg_lo &&
                 bb->cycles <= (uint64_t)tg_hi);

            /* Opt-in raw microinstruction trace for a bounded standalone
             * diagnosis.  Couple it to the existing cycle gate so a Cedar
             * continuation can expose the exact IM loop without producing
             * billions of lines.  Do not touch a caller-installed trace
             * unless this explicit diagnostic is requested. */
            if (dorado_trace_flag("DORADO_UCODE_TRACE")) {
                int trace_now = dorado_trace_gate ||
                    !dorado_trace_flag("DORADO_TRACE_GATE");
                dorado_cpu_trace(cpu, trace_now ? stderr : NULL);
            }
        }
        /* One-shot VM memory dump (env DORADO_VMDUMP="lo,hi,cycle"),
         * trace-only. Dumps visible words [lo,hi) once at/after cycle. */
        {
            static long vd_lo = -2, vd_hi = 0, vd_cyc = 0;
            if (vd_lo == -2) {
                const char *w = getenv("DORADO_VMDUMP");
                vd_lo = -1;
                if (w) sscanf(w, "%li,%li,%li", &vd_lo, &vd_hi, &vd_cyc);
            }
            if (vd_lo >= 0 && bb->cycles >= (uint64_t)vd_cyc) {
                fprintf(stderr, "[VMDUMP] cyc=%llu lo=0o%lo hi=0o%lo\n",
                        (unsigned long long)bb->cycles,
                        (unsigned long)vd_lo, (unsigned long)vd_hi);
                for (long a = vd_lo; a < vd_hi; a++)
                    fprintf(stderr, "  M[0o%lo] = 0o%06o\n", (unsigned long)a,
                            dorado_visible_word_at_va(&m->mem, (uint32_t)a));
                vd_lo = -1; /* one-shot */
            }
        }
        /* Periodic map census (env DORADO_MAPCOUNT="lo,hi,interval"),
         * trace-only. Every `interval` cycles, classifies the map entries
         * covering VA range [lo,hi) the way Pilot's VM allocator does
         * (VMInternal.IsFree / VMMapEntry): a mapped (non-vacant) page is
         * allocated; a vacant page is free iff its software word has
         * dataState=none (bits 14:15) and checkedOut=0 (bit 12).  Used to
         * distinguish genuine MDS/VM exhaustion from allocator-state
         * corruption in the Cedar loadee bring-up. */
        {
            static long mc_lo = -2, mc_hi = 0, mc_int = 0;
            static uint64_t mc_next = 0;
            if (mc_lo == -2) {
                const char *w = getenv("DORADO_MAPCOUNT");
                mc_lo = -1;
                if (w) sscanf(w, "%li,%li,%li", &mc_lo, &mc_hi, &mc_int);
                if (mc_lo >= 0 && (mc_int <= 0 || mc_hi <= mc_lo)) mc_lo = -1;
            }
            if (mc_lo >= 0 && bb->cycles >= mc_next) {
                uint32_t lo_pg = (uint32_t)mc_lo / DM_PAGE_SIZE;
                uint32_t hi_pg = ((uint32_t)mc_hi + DM_PAGE_SIZE - 1) /
                                 DM_PAGE_SIZE;
                unsigned in_n = 0, free_n = 0, alloc_out_n = 0;
                unsigned ref_vacant_n = 0;
                for (uint32_t pg = lo_pg; pg < hi_pg && pg < DM_MAP_ENTRIES;
                     pg++) {
                    const dorado_map_entry *e = dorado_map_get(&m->mem, pg);
                    if (!(e->wp && e->dirty)) {
                        in_n++;                    /* mapped: allocated */
                    } else if (e->ref) {
                        /* Vacant but Ref set: VMInternal.InOut compares
                         * against flagsVacant exactly, so Pilot misreads
                         * this page as mapped/allocated — a free-page
                         * leak if the emulator ever produces it. */
                        ref_vacant_n++;
                    } else if ((e->rp & 0x0003u) == 0 &&
                               (e->rp & 0x0008u) == 0) {
                        free_n++;                  /* vacant, IsFree */
                    } else {
                        alloc_out_n++;             /* vacant, allocated */
                    }
                }
                fprintf(stderr, "[MAPCOUNT] cyc=%llu va=[0o%lo,0o%lo) "
                        "in=%u out_free=%u out_alloc=%u ref_vacant=%u\n",
                        (unsigned long long)bb->cycles, (unsigned long)mc_lo,
                        (unsigned long)mc_hi, in_n, free_n, alloc_out_n,
                        ref_vacant_n);
                if (dorado_trace_flag("DORADO_MAPCOUNT_LAYOUT")) {
                    /* One char per page: I=in, F=out+free, A=out+alloc,
                     * R=vacant+ref.  Lets a log reader compute hole sizes
                     * (runs of F) the way FindHole would. */
                    fprintf(stderr, "[MAPLAYOUT] cyc=%llu ",
                            (unsigned long long)bb->cycles);
                    for (uint32_t pg = lo_pg;
                         pg < hi_pg && pg < DM_MAP_ENTRIES; pg++) {
                        const dorado_map_entry *e =
                            dorado_map_get(&m->mem, pg);
                        char c;
                        if (!(e->wp && e->dirty))          c = 'I';
                        else if (e->ref)                   c = 'R';
                        else if ((e->rp & 0x0003u) == 0 &&
                                 (e->rp & 0x0008u) == 0)   c = 'F';
                        else                               c = 'A';
                        fputc(c, stderr);
                    }
                    fputc('\n', stderr);
                }
                mc_next = bb->cycles + (uint64_t)mc_int;
            }
        }
        /* Boot-button schedule (three presses). */
        if (!m->pressed && bb->cycles >= T_PRESS1_DOWN &&
            bb->cycles < T_PRESS1_UP) {
            baseboard_boot_button(bb, 1); m->pressed = 1;
        } else if (m->pressed && bb->cycles >= T_PRESS1_UP &&
                   bb->cycles < T_PRESS2_DOWN) {
            baseboard_boot_button(bb, 0); m->pressed = 0;
        } else if (!m->pressed && bb->cycles >= T_PRESS2_DOWN &&
                   bb->cycles < T_PRESS2_UP) {
            baseboard_boot_button(bb, 1); m->pressed = 1;
        } else if (m->pressed && bb->cycles >= T_PRESS2_UP &&
                   bb->cycles < T_PRESS3_DOWN) {
            baseboard_boot_button(bb, 0); m->pressed = 0;
        } else if (!m->pressed && bb->cycles >= T_PRESS3_DOWN &&
                   bb->cycles < T_PRESS3_UP) {
            baseboard_boot_button(bb, 1); m->pressed = 1;
        } else if (m->pressed && bb->cycles >= T_PRESS3_UP) {
            baseboard_boot_button(bb, 0); m->pressed = 0;
        }

        int will_inject = bb->dorado_ss_pending && bb->dorado_mir_loaded;
        int will_hold   = !will_inject && !bb->dorado_running;
        int is_imfetch  = !will_inject && !will_hold;

        /* Count BB CPReg strobes before the Bootstrap swap so we wait
         * until the Boot1 stream is in flight. */
        if (!m->swapped && bb->cpreg_to_dorado != m->pre_swap_cpreg) {
            m->bb_strobes_pre_swap++;
            m->pre_swap_cpreg = bb->cpreg_to_dorado;
        }

        /* Swap Bootstrap.MB in once the Dorado is spinning at Boot0Go
         * (0o7740) and the BB has begun streaming Boot1. */
        if (!m->swapped && is_imfetch && cpu->real_PC == 07740 &&
            m->bb_strobes_pre_swap >= 16) {
            for (int a = 0; a < 4096; a++) {
                if (m->bs_mc.im_present[a]) {
                    m->mc.im[a]         = m->bs_mc.im[a];
                    m->mc.im_present[a] = 1;
                }
            }
            for (int a = 0; a < 16; a++) {
                if (m->bs_mc.alufm_present[a]) {
                    m->mc.alufm[a]         = m->bs_mc.alufm[a];
                    m->mc.alufm_present[a] = 1;
                }
            }
            m->swapped = 1;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] Bootstrap swap @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
        }

        /* Replace the (different-build / stream-corrupted) IM with the
         * canonical Initial microcode once Bootstrap reaches 0o7717,
         * keeping Bootstrap's own 0o7700-0o7777 page. */
        if (m->initial_canonical_loaded && !m->initial_substituted &&
            m->swapped && is_imfetch && cpu->real_PC == 07717) {
            for (int a = 0; a < 4096; a++) {
                if (a >= 07700 && a < 010000) continue;
                if (m->init_mc.im_present[a]) {
                    m->mc.im[a]         = m->init_mc.im[a];
                    m->mc.im_present[a] = 1;
                }
            }
            for (int a = 0; a < 1024; a++) {
                if (m->init_mc.ifum_present[a]) {
                    m->mc.ifum_lo[a]      = m->init_mc.ifum_lo[a];
                    m->mc.ifum_hi[a]      = m->init_mc.ifum_hi[a];
                    m->mc.ifum_present[a] = 1;
                }
            }
            for (int a = 0; a < 16; a++) {
                if (m->init_mc.alufm_present[a]) {
                    m->mc.alufm[a]         = m->init_mc.alufm[a];
                    m->mc.alufm_present[a] = 1;
                }
            }
            m->initial_substituted = 1;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] Initial substituted @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
            /* Drive the Stage-2 boot decision via the DDC keyboard by
             * holding the configured boot-key chord (default BS, which
             * selects the Ethernet software boot). */
            if (m->alto_ether_boot) {
                uint16_t w[4];
                machine_apply_boot_chord(disp, m->boot_chord,
                                         m->boot_chord_count, w);
            }
        }

        uint16_t pre_pc = cpu->real_PC;

        /* Present the disk-booted Alto image with cold ACs (Stack[1..4]=0),
         * the way a real Alto power-on does. The AEmu's DiskBoot/Start never
         * clear the emulator stack (Start.mc: "Accumulators AC0-AC3 are kept in
         * Stack[1] through Stack[4]"), so the booted image would otherwise
         * inherit the AEmu's leftover ACs. A real Alto disk-boots cold (AC=0);
         * verified against the salto reference Alto, whose loader runs with
         * AC=0 -- clearing them here makes the first 11453 booted opcodes match
         * salto exactly (vs diverging at opcode 5 without it). One-shot.
         *
         * Fires at BOTH AEmu boot vectors (AEmu.mb!2 real addrs): DiskBoot
         * (0o2005) for a disk boot, and EBoot (0o2006) for the Stage-2 ether
         * games. The ether path needs the identical cold-Alto state: a
         * tracepcdiff vs ContrAlto (tools/nova-trace-diff) shows both Invaders
         * and MissileCommand otherwise inherit the AEmu's leftover Stack ACs
         * (AC1=056623, AC2=121045) at the loaded program's first opcode where
         * ContrAlto cold-boots clean 0. EBoot is gated to alto_ether_boot so
         * Cedar's germ path (different microcode at 0o2006) is untouched. */
        if (is_imfetch && cpu->ctask == 0 && !m->alto_cold_ac_done &&
            (pre_pc == 02005 || (pre_pc == 02006 && m->alto_ether_boot))) {
            cpu->STK[1] = cpu->STK[2] = cpu->STK[3] = cpu->STK[4] = 0;
            /* Initialize the Alto I/O page (177000-177777) to the hardware
             * floating-bus default 177777. On a real Alto these addresses are
             * memory-mapped I/O: undecoded reads return 177777 (IOB pulls high)
             * and the active-low input words (UTILIN buttons/keyset 177030-3,
             * keyboard 177034-7) read all-ones = "nothing pressed". Our AEmu
             * maps the Alto address space 1:1 onto Dorado VM, so without this
             * the I/O page reads as zeroed RAM = "every button/key pressed",
             * which diverges the booted OS from a real Alto (verified against
             * salto: its I/O page reads 177777 here, ours read 0). The keyboard
             * seeding overwrites 177034-7 with live key state on top of this. */
            for (uint32_t va = 0177000u; va <= 0177777u; va++)
                machine_store_va(&m->mem, va, 0177777u);
            /* The 16 per-task bank registers (177740-177757) are NOT floating:
             * a real Alto reads them as bank_reg|177760, so bank 0 (no extended
             * memory) reads 177760, not 177777. Leaving 177777 there selects
             * bank 15 and sends the booted code's memory references to the wrong
             * bank. Set the default (bank 0). */
            for (uint32_t va = 0177740u; va <= 0177757u; va++)
                machine_store_va(&m->mem, va, 0177760u);
            m->alto_cold_ac_done = 1;
        }

        /* Alto disk-boot bring-up trace: log when any task reaches the AEmu
         * boot-decision vectors (ABoot/DiskBoot/EBoot, AEmu.mb!2 real addrs). */
        if (is_imfetch && dorado_trace_flag("DORADO_ALTOBOOT_TRACE") &&
            (pre_pc == 0724 || pre_pc == 02005 || pre_pc == 02006 ||
             pre_pc == 02022 || pre_pc == 02000 ||
             pre_pc == 03020 || pre_pc == 01565 || pre_pc == 02355 ||
             pre_pc == 02367 || pre_pc == 02615 || pre_pc == 03371 ||
             pre_pc == 03246 || pre_pc == 03206 || pre_pc == 03446)) {
            const char *nm = pre_pc == 0724  ? "ABoot" :
                             pre_pc == 02005 ? "DiskBoot" :
                             pre_pc == 02006 ? "EBoot" :
                             pre_pc == 02022 ? "DiskBootRetry" :
                             pre_pc == 03000 ? "DSKInitPC" :
                             pre_pc == 02612 ? "AltoLoop" :
                             pre_pc == 03446 ? "DoACmmd" :
                             pre_pc == 01565 ? "KBootTimeout" :
                             pre_pc == 02355 ? "ACmmdEnd" :
                             pre_pc == 02367 ? "ACmmdEnd2" :
                             pre_pc == 02615 ? "AForgetCmmd" :
                             pre_pc == 03371 ? "ACmmdAbort" :
                             pre_pc == 03246 ? "ABadSector" :
                             pre_pc == 03206 ? "ACmmdBadSeal" :
                             pre_pc == 03446 ? "DoACmmd2" :
                             pre_pc == 03020 ? "InitRamDiablo" : "SelectPartitionIfKey";
            uint16_t kblk = dorado_visible_word_at_va(&m->mem, 0521u);
            uint16_t dyn0 = dorado_visible_word_at_va(&m->mem, kblk);
            uint16_t dyn1 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 1u));
            uint16_t dyn2 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 2u));
            uint16_t dyn3 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 3u));
            uint16_t dyn4 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 4u));
            uint16_t dyn5 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 5u));
            uint16_t dyn6 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 6u));
            uint16_t dyn7 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 7u));
            uint16_t dyn11 = dorado_visible_word_at_va(&m->mem, (uint16_t)(kblk + 011u));
            fprintf(stderr, "[altoboot] task=%o pc=0o%o %s @cyc=%llu kbd[0]=0o%o "
                    "KBLK[521]=0o%o KCB431=0o%o stat432=0o%o KCB433=0o%o "
                    "dynKCB[%06o]={next=%06o stat=%06o cmd=%06o hdr=%06o lbl=%06o data=%06o ok=%06o err=%06o dsk=%06o}\n",
                    cpu->ctask, pre_pc, nm,
                    (unsigned long long)m->bb.cycles,
                    dorado_visible_word_at_va(&m->mem, 0177034u),
                    kblk,
                    dorado_visible_word_at_va(&m->mem, 0431u),
                    dorado_visible_word_at_va(&m->mem, 0432u),
                    dorado_visible_word_at_va(&m->mem, 0433u),
                    kblk, dyn0, dyn1, dyn2, dyn3, dyn4, dyn5, dyn6, dyn7, dyn11);
        }

        if (m->germ_netboot_seeded && !m->germ_netboot_diag_done &&
            dorado_trace_flag("DORADO_GERM_NETBOOT_TRACE") &&
            is_imfetch && cpu->ctask == 0 && pre_pc == 0150) {
            machine_germ_netboot_diag(m);
            m->germ_netboot_diag_done = 1;
        }

        /* Seed Initial's boot parameter (STK[1]=boot file number,
         * STK[2]=BootParameterSeal, STK[1]+STK[2]+STK[3]=0) so the
         * loaded world selects the normal Mesa boot instead of falling
         * into the cold/no-storage path. The 7-wire terminal back-channel
         * that would carry this is not modeled, so we plant it directly.
         * The boot file number is configurable (see boot_file_number);
         * STK[2] is the fixed BootParameterSeal and STK[3] is the
         * checksum word that makes the three sum to 0 mod 2^16. */
        if (m->initial_substituted && is_imfetch &&
            !m->checksum_and_load_seen &&
            ((pre_pc >= 06170 && pre_pc <= 06217) ||
             (pre_pc >= 06406 && pre_pc <= 06443))) {
            uint16_t seal = 056623u;
            cpu->STK[1] = m->boot_file_number;
            cpu->STK[2] = seal;
            cpu->STK[3] = (uint16_t)(0u - cpu->STK[1] - seal);
        }

        /* Keep Initial's boot keys "up" until CheckChecksumAndLoad so it
         * falls through DiskHardMicrocodeBoot to EtherMicrocodeBoot.
         * (The 7-wire terminal back-channel is not modeled.) */
        if (m->initial_substituted && is_imfetch &&
            !m->checksum_and_load_seen &&
            (pre_pc == 06417 || (pre_pc >= 06407 && pre_pc <= 06431))) {
            cpu->RM[0x1A] = 0xFFFFu;
            cpu->RM[0x1B] = 0xFFFFu;
            cpu->RM[0x1C] = 0xFFFFu;
            cpu->RM[0x1D] = 0xFFFFu;
        }

        /* Identity-map installed storage before BootEmulator builds the
         * Ethernet request (bring-up shim for the PresetMap gap). */
        if (m->initial_substituted && is_imfetch &&
            !m->identity_map_shimmed && pre_pc == 06222) {
            machine_identity_map_storage(&m->mem);
            m->identity_map_shimmed = 1;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] identity-map @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
        }

        /* CheckChecksumAndLoad has started. */
        if (m->initial_substituted && is_imfetch &&
            !m->checksum_and_load_seen && pre_pc == 06460) {
            m->checksum_and_load_seen = 1;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] checksum&load @cyc=%llu\n",
                        (unsigned long long)bb->cycles);
        }

        /* Detect the LoadRam handoff: Initial has replaced the control
         * store and branched into the loaded Alto/Mesa world (running
         * code outside Initial's own region that differs from Initial). */
        if (m->checksum_and_load_seen && m->ether_loaded_world_cycle == 0 &&
            is_imfetch && pre_pc < IM_SIZE && m->mc.im_present[pre_pc] &&
            !(pre_pc >= 06000 && pre_pc < 07700)) {
            if (!dorado_trace_flag("DORADO_NO_ALUFM_RESTORE"))
                restore_standard_alufm(&m->mc);
            m->ether_loaded_world_cycle = bb->cycles;
            /* Quiet the junk timer and drop any stale pending wakeups at the
             * LoadRam handoff so high-priority I/O tasks do not run their
             * pre-reload microcode and starve the emulator task. Do NOT
             * invalidate task TPCs: the fault task (0o17) must remain
             * runnable to service the map faults InitMem raises while it
             * enumerates storage. Route B's Pilot timer channel is injected
             * later, only after the planted germ reaches BusyWait. */
            cpu->wakeup_pending = 0;
            cpu->junk_tw_enabled = 0;
            cpu->junk_tw_countdown = 0;
            if (dorado_trace_flag("DORADO_MACHINE_TRACE"))
                fprintf(stderr, "[machine] world loaded @cyc=%llu pc=0o%o\n",
                        (unsigned long long)bb->cycles, pre_pc);
        }

        /* Route B germ disk-read interception. The Cedar microcode's disk
         * germ-boot is PilotBoot.GermBoot -> DiskBootSoft -> BootTransfer
         * (DiskBootTransfer.mc). DiskBootSoft issues THREE disk-read passes
         * against an IOCB at VM 0o431:
         *   1. command 0o274  ([check,read,read])  -- read the PV root-page
         *      Descriptor into memory page 0 (IOCB.dataPtr=0).
         *   2. command 0o260  ([check,read,none])  -- read the first page's
         *      label (no data) to learn the file type.
         *   3. command 0o100254 (incrementDataPtr|[check,check,read]) -- read
         *      the whole germ boot file into the low-64K buffer at
         *      IOCB.dataPtr = BootDataPtr = baseGerm (0o1000), advancing
         *      dataPtr by 0o400 per page.
         * Each pass posts the IOCB (seal=IOCBSealValue) then spins in
         * BootTransferLp at real PC 0o7012 (the seal-fetch) until the disk
         * microcode clears iocb.seal. We have no boot pack and the disk data
         * path is incomplete, so we FAKE each pass at the spin: deposit the
         * data the real microcode expects, then KCmmdDone-complete the IOCB
         * (seal=0, pageCount=0, labelStatus=0 -> BootTransfer returns +2,
         * proven mechanic). The REAL microcode then validates the Descriptor
         * (seal=0o121212, version=6 -- DiskBootSoft.mc), reads the germ,
         * runs PilotBoot.GERMREMAP to relocate the germ into MDS 76 and
         * XFERs into the germ wart -- we hand-roll none of that.
         *
         * PC 0o7012 is placed only in the Cedar/Mesa world (the Alto worlds
         * never reach it), so this stays gated off the regression gate. */
        if (m->germ_word_count && !m->germ_data_done &&
            m->ether_loaded_world_cycle && is_imfetch && cpu->ctask == 0 &&
            pre_pc == 07012) {
            uint16_t seal = dorado_visible_word_at_va(&m->mem, IOCB_SEAL_VA);
            if (seal == (uint16_t)IOCB_SEAL_VALUE) {
                uint16_t cmd = dorado_visible_word_at_va(&m->mem,
                                                         IOCB_COMMAND_VA);
                if (cmd == (uint16_t)DISK_CMD_DESCRIPTOR &&
                    !m->germ_descriptor_done) {
                    /* Pass 1: fabricate the PV root-page Descriptor in page 0.
                     * PhysicalVolumeFormat.mesa / PilotBootDefs.mc:
                     *   word 0  Desc.seal           = 121212B
                     *   word 1  Desc.currentVersion = 6
                     *   Desc.bi.germ (offset 0o32) = germ's DiskFileID
                     *     {fID[5]@0, firstPage@5, da[2]@7} -- the microcode
                     *     copies these into the IOCB but our fake completion
                     *     bypasses the real disk read, so plain zeroes (germ
                     *     file page 0, fID 0, da 0) suffice. */
                    /* The Mesa world leaves VM page 0 vacant (its null-trap
                     * page: wp=1,dirty=1). The real descriptor DMA write to
                     * page 0 would map-fault and XMFaultTask would make the
                     * page resident before the write lands; our fake bypasses
                     * the disk DMA, so emulate that side effect by making
                     * page 0 resident (identity rp 0, writable) before the
                     * deposit. The microcode comment confirms DiskBootSoft
                     * "clobbers memory page 0 by reading the root page into
                     * it." */
                    const dorado_map_entry *e0 = dorado_map_get(&m->mem, 0);
                    if (e0->wp && e0->dirty) {
                        /* Allocate a free real page (the existing e0->rp is
                         * aliased to another live VM page in the Mesa map, so
                         * reusing it lets that page's dirty writeback clobber
                         * our descriptor). */
                        uint16_t frp = machine_find_free_rp(&m->mem);
                        if (frp == 0xFFFF) frp = e0->rp;
                        dorado_map_set(&m->mem, 0, frp, /*wp=*/0, /*dirty=*/0);
                    }
                    for (uint32_t w = 0; w <= 0043u; w++)
                        dorado_storage_store_at_va(&m->mem, w, 0);
                    dorado_storage_store_at_va(&m->mem, 0, 0121212u);
                    dorado_storage_store_at_va(&m->mem, 1, 06u);
                    m->germ_descriptor_done = 1;
                    fprintf(stderr,
                        "[machine] germ pass1 (descriptor) @cyc=%llu: "
                        "seal=0o121212 version=6 at page 0 "
                        "(readback[0]=0o%o [1]=0o%o)\n",
                        (unsigned long long)bb->cycles,
                        dorado_visible_word_at_va(&m->mem, 0),
                        dorado_visible_word_at_va(&m->mem, 1));
                } else if (cmd == (uint16_t)DISK_CMD_LABEL &&
                           !m->germ_label_done) {
                    /* Pass 2: first-page label read (no data). DiskBootSoft
                     * does not inspect the label contents, so just complete. */
                    m->germ_label_done = 1;
                    fprintf(stderr,
                        "[machine] germ pass2 (label) @cyc=%llu: completed\n",
                        (unsigned long long)bb->cycles);
                } else if ((cmd == (uint16_t)DISK_CMD_GERMDATA) &&
                           !m->germ_data_done) {
                    /* Pass 3: read the germ file into the low buffer at
                     * IOCB.dataPtr (= BootDataPtr = baseGerm 0o1000). Deposit
                     * germ file word W -> VM dataPtr+W, then advance dataPtr
                     * by 0o400 per page (as the disk microcode would) so
                     * GermBoot reads BootDataPtr = dataPtr + 0o400*pages and
                     * GERMREMAP relocates exactly the loaded extent. */
                    uint16_t dptr = dorado_visible_word_at_va(&m->mem,
                                                              IOCB_DATAPTR_VA);
                    int landed = 0;
                    for (int w = 0; w < m->germ_word_count; w++) {
                        if (dorado_storage_store_at_va(&m->mem,
                                (uint32_t)dptr + (uint32_t)w,
                                m->germ_words[w]) == 0)
                            landed++;
                    }
                    uint32_t pages = (uint32_t)(m->germ_word_count + 0377) / 0400u;
                    uint16_t new_dptr = (uint16_t)(dptr + pages * 0400u);
                    dorado_storage_store_at_va(&m->mem, IOCB_DATAPTR_VA,
                                               new_dptr);
                    m->germ_data_done = 1;
                    fprintf(stderr,
                        "[machine] germ pass3 (data) @cyc=%llu: %d/%d words "
                        "at VM 0o%o+ (word0=0o%o), dataPtr 0o%o->0o%o\n",
                        (unsigned long long)bb->cycles, landed,
                        m->germ_word_count, dptr,
                        dorado_visible_word_at_va(&m->mem, dptr),
                        dptr, new_dptr);
                }
                /* KCmmdDone completion (proven): clear seal, zero pageCount
                 * and labelStatus -> BootTransfer's post-spin check sees
                 * pageCount==0 and returns +2 (success). */
                dorado_storage_store_at_va(&m->mem, IOCB_SEAL_VA, 0);
                dorado_storage_store_at_va(&m->mem, IOCB_PAGECOUNT_VA, 0);
                dorado_storage_store_at_va(&m->mem, IOCB_LABELSTAT_VA, 0);
                m->germ_passes++;
            }
        }

        /* Experimental Route B shortcut: PilotBoot.mc seeds the germ with
         * pRequest=[bootPhysicalVolume, sa4000, ...], which is faithful for
         * disk boot but stalls us before Stage-2 Ethernet because no Dorado
         * Pilot disk volume survives. BootSwapGerm's own documented initial
         * contract accepts pRequest=[inLoad, locationOfBootFile]. When the
         * option is enabled, wait until GERMREMAP has copied that request
         * into resident MDS 76, then rewrite it to the Ethernet overlay:
         *   Request.action = inLoad
         *   Location.deviceType = ethernet
         *   Location.deviceOrdinal = 0
         *   Location.bootFileNumber = germ_netboot_bfn
         *   Location.net/host = 0 (broadcast/any server)
         * Inert unless --germ-netboot-bfn was supplied. */
        if (m->germ_netboot && m->germ_data_done &&
            !m->germ_netboot_seeded && m->ether_loaded_world_cycle &&
            is_imfetch && cpu->ctask == 0) {
            uint16_t action = dorado_visible_word_at_va(
                &m->mem, GERM_REQUEST_VA + GERM_REQ_ACTION);
            uint16_t dtype = dorado_visible_word_at_va(
                &m->mem, GERM_REQUEST_VA + GERM_REQ_DEVICE_TYPE);
            if (action == GERM_ACT_BOOT_PV && dtype == GERM_DTYPE_SA4000) {
                machine_store_va(&m->mem, GERM_REQUEST_VA + GERM_REQ_ACTION,
                                 GERM_ACT_INLOAD);
                machine_store_va(&m->mem, GERM_REQUEST_VA + GERM_REQ_DEVICE_TYPE,
                                 GERM_DTYPE_ETHERNET);
                machine_store_va(&m->mem, GERM_REQUEST_VA + GERM_REQ_DEVICE_ORD,
                                 0);
                machine_store_va(&m->mem, GERM_REQUEST_VA + GERM_REQ_ETH_BFN,
                                 m->germ_netboot_bfn);
                machine_store_va(&m->mem, GERM_REQUEST_VA + GERM_REQ_ETH_NET,
                                 0);
                machine_store_va(&m->mem, GERM_REQUEST_VA + GERM_REQ_ETH_HOST,
                                 0);
                m->germ_netboot_seeded = 1;
                fprintf(stderr,
                    "[machine] germ netboot request seeded @cyc=%llu: "
                    "pRequest=0o%o action=inLoad device=ethernet bfn=0o%o\n",
                    (unsigned long long)bb->cycles, GERM_REQUEST_VA,
                    m->germ_netboot_bfn);
            }
        }

        /* Diagnostic (DORADO_FORCE_BCLINK): BootChannelDisk's codeLink #9 (the
         * RemainingChannels.Create link to BootChannelEther) resolves to
         * 0o3424 -> TrapsImpl in our emulator instead of BootChannelEther.
         * Force that slot (VA 0o17410532) to the known-valid BootChannelEther
         * link 0o1221 (taken from index #2 of the same module's codeLink
         * table) to test whether the germ then reaches BootChannelEther.Create.
         * Inert unless the env is set; fires once when the slot reads 0o3424. */
        if (dorado_trace_flag("DORADO_FORCE_BCLINK") && m->germ_data_done &&
            is_imfetch && cpu->ctask == 0) {
            /* codeLink #9 (0o3424) is an INDIRECT pointer; the germ derefs it
             * to *0o3424 = M[0o17403424] (ProcessorHead global 0o24) = 0o6200,
             * which resolves to TrapsImpl. Patch the indirect TARGET to a
             * valid BootChannelEther procedure link 0o1221 (gfi 10 -> GFT[10]
             * = 0o6234) to test whether the germ then reaches BootChannelEther. */
            uint16_t tgt = dorado_visible_word_at_va(&m->mem, 017403424u);
            if (tgt == 06200u) {
                machine_store_va(&m->mem, 017403424u, 01221u);
                fprintf(stderr,
                    "[machine] forced indirect target M[0o17403424]: "
                    "0o6200 -> 0o1221 @cyc=%llu\n",
                    (unsigned long long)bb->cycles);
            }
        }

        /* Track the Alto world's currently-posted Ethernet input-buffer size
         * (EICLOC at Alto VM 604B, Alto HW Manual Sec 7) so the receiver can
         * refuse to overflow it. This is an AEmu/Alto-world convention, not a
         * Dorado Ethernet controller register: the Cedar/Pilot germ posts
         * input IOCBs through EthernetOne's CSB at VM 177600B and VM 604B is
         * unrelated state there. Keep the gate disabled for planted-germ
         * Route B runs (world_rx_words = 0xFFFF from init). */
        if (m->ether_loaded_world_cycle && !m->germ_word_count)
            eth->world_rx_words = dorado_visible_word_at_va(&m->mem, 0604u);
        if (m->germ_word_count) {
            /* Complete the germ's disk IOCBs. Default: direct PDI copy. Under
             * --disk-real: the same IOCB bridge, but each page is read through
             * the real controller's read path (FIFO + framing, D0-D3) instead
             * of a direct copy -- so Cedar boots *through the controller*. The
             * fully microcode-driven boot (a DSK-task disk processor) is the
             * deeper architecture; the germ boot has none (it posts IOCBs and
             * waits for a processor -- the bridge plays that role). Plan D4. */
            machine_germ_complete_disk_iocb(m);
            machine_germ_seed_ethernet_header_page(m);
            machine_germ_complete_ethernet_tx(m);
            uint16_t next_input =
                dorado_visible_word_at_va(&m->mem,
                                          GERM_ETH_CSB_VA +
                                          ETH_CSB_NEXT_INPUT);
            if (m->stp_direct_wait_repost) {
                uint16_t last_input = dorado_visible_word_at_va(
                    &m->mem, GERM_ETH_CSB_VA + ETH_CSB_LAST_INPUT);
                if (last_input != m->stp_direct_last_input) {
                    m->stp_direct_wait_repost = 0;
                    eth->ftp_delivery_blocked = 0;
                    if (dorado_trace_flag("DORADO_ETH_IOCB_TRACE")) {
                        fprintf(stderr,
                                "[machine] Cedar direct RX reposted "
                                "lastInput=0o%o; release next file Pup\n",
                                last_input);
                    }
                }
            }
            int plausible_input = machine_ethernet_plausible_iocb(m, next_input);
            uint16_t next_completion = next_input
                ? dorado_visible_word_at_va(&m->mem, (uint32_t)next_input + 1u)
                : 0;
            dorado_ethernet_set_eftp_rx_armed(
                eth, eth->rx_on && plausible_input && next_completion == 0);
            /* The CSB-level completion shim is only for the germ's EFTP
             * boot reader.  Once EFTP has finished, the running Cedar
             * EthernetOne driver must receive ordinary BSP/STP Pups through
             * EIT; completing those directly exhausts its five IOCBs before
             * the driver has a chance to repost them. */
            if (eth->eftp_wait_for_rx_arm &&
                (eth->eftp_state != 0 ||
                 dorado_trace_flag("DORADO_ETH_STP_DIRECT_RX")) &&
                eth->eftp_rx_armed && plausible_input)
                machine_germ_complete_ethernet_rx(m, next_input);
            if (dorado_trace_flag("DORADO_CSB_TRACE") && plausible_input) {
                static uint16_t last_seen_iocb;
                uint32_t iocb_va = (uint32_t)next_input;
                uint16_t completion =
                    dorado_visible_word_at_va(&m->mem, iocb_va + 1u);
                uint16_t used =
                    dorado_visible_word_at_va(&m->mem, iocb_va + 2u);
                if (completion && used && last_seen_iocb != next_input) {
                    uint16_t buffer_lo =
                        dorado_visible_word_at_va(&m->mem, iocb_va + 5u);
                    uint16_t buffer_hi =
                        dorado_visible_word_at_va(&m->mem, iocb_va + 6u);
                    uint32_t buffer_va =
                        ((uint32_t)buffer_hi << 16) | buffer_lo;
                    last_seen_iocb = next_input;
                    machine_dump_words(&m->mem, "EthernetOne CSB",
                                       GERM_ETH_CSB_VA, 16);
                    machine_dump_words(&m->mem, "EthernetOne IOCB",
                                       iocb_va, 10);
                    machine_dump_words(&m->mem, "EthernetOne RX buffer",
                                       buffer_va, 24);
                }
            }
        }

        /* Seed Alto keyboard/mouse cells for AEmu worlds. During Ethernet
         * software boot we hold the boot-selection chord until EFTP starts.
         * In disk mode, or once the boot file is downloading, the frontend's
         * live state drives the Alto input words directly (the DDC 7-wire
         * back-channel is not modeled; gap E2). */
        if (m->ether_loaded_world_cycle && !m->germ_word_count &&
            !cpu->ifu_active) {
            int disk_boot_reason = machine_boot_chord_is_disk(m);
            if (m->alto_ether_boot && !disk_boot_reason &&
                eth->eftp_max_seq == 0) {
                /* Boot-selection phase: hold the boot-key chord down so the
                 * world picks its boot path (default BS = Ethernet software
                 * boot). The chord is applied to the DDC keyboard and its
                 * Alto words are seeded into the polled cells (gap E2). */
                uint16_t w[4];
                machine_apply_boot_chord(disp, m->boot_chord,
                                         m->boot_chord_count, w);
                machine_seed_keyboard(&m->mem, w);
            } else if (m->alto_cold_ac_done) {
                machine_seed_alto_live_io(m, disp);
            }

            /* (The divide-vector guard was retired 2026-06-13: the
             * page-zero BitBlt spray that corrupted M[0o344] is fixed at
             * the source -- see the RM-write RBase-timing fix in cpu.c
             * and docs/CONTINUE-HERE.md. The world is now stable to 200M
             * with no guard.) */
        }

        /* Lisp runs under the IFU after the sysout transfer, so the Alto
         * live-I/O path above no longer refreshes its input cells.  LLKEY's
         * Dorado machine case reads the absolute low-core keyboard/mouse
         * words; keep those current until the DDC back-channel is modeled. */
        if (m->ether_loaded_world_cycle && !m->germ_word_count &&
            cpu->ifu_active && m->ethernet.ftp_sysout_path[0] &&
            ((bb->cycles & 037777u) == 0))
            machine_seed_lisp_live_io(m, disp);

        /* Optional task-0 PC histogram of the loaded world (env-gated):
         * DORADO_MACHINE_PCHIST dumps the hottest emulator-task PCs at
         * the end of the run, to localize a post-LoadRam stall. */
        if (m->ether_loaded_world_cycle && is_imfetch && cpu->ctask == 0 &&
            pre_pc < 4096 && dorado_trace_flag("DORADO_MACHINE_PCHIST")) {
            m->pchist[pre_pc]++;
            if (m->initseq_n < (int)(sizeof m->initseq / sizeof m->initseq[0]))
                m->initseq[m->initseq_n++] = (uint16_t)pre_pc;
        }
        /* All-task variant: lets us check whether display/field handlers
         * (ENDOFFIELD/EVENFIELD/RTCCARRY, which run in the display task,
         * not task 0) actually execute for a stalled game. */
        if (m->ether_loaded_world_cycle && is_imfetch && pre_pc < 4096 &&
            dorado_trace_flag("DORADO_MACHINE_PCHIST")) {
            m->pchist_all[pre_pc]++;
            machine_pchist_task[cpu->ctask & 017][pre_pc]++;
        }

        /* InitMem GotMapConfig/NoStorage register trace (env-gated):
         * dump the registers feeding the storage-detect branch so the
         * NoStorage divergence can be compared against the harness. */
        if (m->ether_loaded_world_cycle && is_imfetch && cpu->ctask == 0 &&
            pre_pc >= 01005 && pre_pc <= 01025 &&
            dorado_trace_flag("DORADO_MACHINE_INITMEM")) {
            fprintf(stderr,
                    "INITMEM pc=0o%o T=%06o Q=%06o ShC=%06o Cnt=%06o "
                    "aluZ=%u aluLT=%u link=0o%o\n",
                    pre_pc, cpu->T, cpu->Q, cpu->ShC, cpu->Cnt,
                    cpu->alu_zero, cpu->alu_lt0, cpu->Link);
        }

        /* DORADO_POKE="va,value,cycle" (octal): one-shot write of value to
         * VA once bb->cycles >= cycle. Used to test which word unsticks a
         * deadlocked Alto game loop (an exit-flag the game waits on). */
        {
            static int poke_init = -1, poke_hold = 0, poke_fired = 0;
            static uint32_t poke_va;
            static uint16_t poke_val;
            static uint64_t poke_cyc;
            if (poke_init < 0) {
                const char *p = getenv("DORADO_POKE");
                poke_init = 0;
                if (p) {
                    unsigned long a = 0, v = 0, h = 0;
                    unsigned long long c = 0;
                    int n = sscanf(p, "%lo,%lo,%llu,%lo", &a, &v, &c, &h);
                    if (n >= 3) {
                        poke_va = (uint32_t)a;
                        poke_val = (uint16_t)v;
                        poke_cyc = c;
                        poke_hold = (n >= 4 && h);
                    } else {
                        poke_init = 1;
                    }
                } else {
                    poke_init = 1;
                }
            }
            if (poke_init == 0 && bb->cycles >= poke_cyc) {
                if (!poke_fired || poke_hold) {
                    dorado_storage_store_at_va(&m->mem, poke_va, poke_val);
                    if (!poke_fired)
                        fprintf(stderr,
                                "[poke] M[%07o]=%06o at cyc=%llu hold=%d\n",
                                poke_va, poke_val,
                                (unsigned long long)bb->cycles, poke_hold);
                    poke_fired = 1;
                    if (!poke_hold) poke_init = 1;
                }
            }
        }

        /* DORADO_RM_POKE="idx,value,cycle[,hold]" (octal idx/value):
         * one-shot diagnostic write to RM. */
        {
            static int poke_init = -1, poke_hold = 0, poke_fired = 0;
            static unsigned poke_idx;
            static uint16_t poke_val;
            static uint64_t poke_cyc;
            if (poke_init < 0) {
                const char *p = getenv("DORADO_RM_POKE");
                poke_init = 0;
                if (p) {
                    unsigned long i = 0, v = 0, h = 0;
                    unsigned long long c = 0;
                    int n = sscanf(p, "%lo,%lo,%llu,%lo", &i, &v, &c, &h);
                    if (n >= 3 && i < 256) {
                        poke_idx = (unsigned)i;
                        poke_val = (uint16_t)v;
                        poke_cyc = c;
                        poke_hold = (n >= 4 && h);
                    } else {
                        poke_init = 1;
                    }
                } else {
                    poke_init = 1;
                }
            }
            if (poke_init == 0 && bb->cycles >= poke_cyc) {
                if (!poke_fired || poke_hold) {
                    cpu->RM[poke_idx & 0xFFu] = poke_val;
                    if (!poke_fired)
                        fprintf(stderr,
                                "[rm-poke] RM[%03o]=%06o at cyc=%llu hold=%d\n",
                                poke_idx & 0377u, poke_val & 0177777,
                                (unsigned long long)bb->cycles, poke_hold);
                    poke_fired = 1;
                    if (!poke_hold) poke_init = 1;
                }
            }
        }

        /* DORADO_FORCE_RESCHED_AT=<cycle>: one-shot diagnostic
         * RescheduleNow edge. */
        {
            static int init = -1, fired = 0;
            static uint64_t cyc;
            if (init < 0) {
                const char *p = getenv("DORADO_FORCE_RESCHED_AT");
                init = 0;
                if (p && p[0]) {
                    cyc = strtoull(p, NULL, 0);
                } else {
                    init = 1;
                }
            }
            if (init == 0 && !fired && bb->cycles >= cyc) {
                cpu->reschedule_pending = 1;
                fprintf(stderr, "[force-resched] at cyc=%llu\n",
                        (unsigned long long)bb->cycles);
                fired = 1;
                init = 1;
            }
        }

        static uint16_t alto_disk_prev_pc = 0177777u;
        static uint16_t alto_disk_prev2_pc = 0177777u;
        static uint64_t alto_disk_prev_cycle = 0;
        static uint64_t alto_disk_prev2_cycle = 0;

        int alto_check_enabled =
            dorado_trace_flag("DORADO_ALTOCHECK_TRACE") &&
            (!dorado_trace_flag("DORADO_TRACE_GATE") || dorado_trace_gate);
        int alto_check_trace = 0;
        uint16_t alto_check_md_before = 0;
        uint16_t alto_check_t_before = 0;
        uint16_t alto_check_kaddr = 0;
        uint16_t alto_check_sector = 0;
        uint16_t alto_check_kptr = 0;
        uint16_t alto_check_kcmmd = 0;
        uint16_t alto_check_dskmaddr = 0;
        uint16_t alto_check_kstatus = 0;
        uint16_t alto_check_ktemp0 = 0;
        uint16_t alto_check_ktemp1 = 0;
        uint16_t alto_check_ktemp2 = 0;
        uint16_t alto_check_ktemp3 = 0;
        uint16_t alto_check_prev_pc = alto_disk_prev_pc;
        uint16_t alto_check_prev2_pc = alto_disk_prev2_pc;
        uint64_t alto_check_prev_cycle = alto_disk_prev_cycle;
        uint64_t alto_check_prev2_cycle = alto_disk_prev2_cycle;
        uint16_t alto_check_task_md_before = 0;
        uint16_t alto_check_kcb0 = 0;
        uint16_t alto_check_kcb1 = 0;
        uint16_t alto_check_vm521 = 0;
        uint16_t alto_check_diskbr_word = 0;
        if (alto_check_enabled && is_imfetch &&
            cpu->ctask == DORADO_DISK_TASK &&
            (pre_pc == 03045 || pre_pc == 03067 || pre_pc == 03225 ||
             pre_pc == 03244 || pre_pc == 03246 || pre_pc == 03301 ||
             pre_pc == 03306 || pre_pc == 03324 || pre_pc == 03326 ||
             pre_pc == 03330 || pre_pc == 03333 || pre_pc == 03337 ||
             pre_pc == 03347 || pre_pc == 03370 || pre_pc == 03371 ||
             pre_pc == 02355 || pre_pc == 02356 || pre_pc == 02357 ||
             pre_pc == 02367 || pre_pc == 02370 || pre_pc == 02371 ||
             pre_pc == 02372 || pre_pc == 02375 || pre_pc == 02376)) {
            alto_check_trace = 1;
            alto_check_md_before = m->mem.md;
            alto_check_task_md_before =
                cpu->task_md_valid[DORADO_DISK_TASK]
                    ? cpu->task_md[DORADO_DISK_TASK]
                    : m->mem.md;
            alto_check_t_before = cpu->T;
            alto_check_kaddr = cpu->RM[(5u << 4) | 004u];
            alto_check_sector = cpu->RM[(5u << 4) | 005u];
            alto_check_kptr = cpu->RM[(5u << 4) | 007u];
            alto_check_kcmmd = cpu->RM[(5u << 4) | 010u];
            alto_check_dskmaddr = cpu->RM[(5u << 4) | 011u];
            alto_check_kstatus = cpu->RM[(5u << 4) | 012u];
            alto_check_ktemp0 = cpu->RM[(5u << 4) | 014u];
            alto_check_ktemp1 = cpu->RM[(5u << 4) | 015u];
            alto_check_ktemp2 = cpu->RM[(5u << 4) | 016u];
            alto_check_ktemp3 = cpu->RM[(5u << 4) | 017u];
            alto_check_kcb0 = dorado_visible_word_at_va(&m->mem,
                                                        alto_check_kptr);
            alto_check_kcb1 = dorado_visible_word_at_va(&m->mem,
                                                        alto_check_kptr + 1u);
            alto_check_vm521 = dorado_visible_word_at_va(&m->mem, 0521u);
            alto_check_diskbr_word = dorado_visible_word_at_va(
                &m->mem,
                (uint16_t)(alto_check_md_before + alto_check_dskmaddr));
        }

        machine_pilot_timer_channel(m, cpu, bb, pre_pc, is_imfetch);
        machine_cedar_io(m, bb, disp);

        uint64_t ifu_dispatch_before = cpu->ifu_dispatch_count;
        if (dorado_cpu_step(cpu)) break;
        if (m->ether_loaded_world_cycle &&
            cpu->ifu_dispatch_count != ifu_dispatch_before &&
            dorado_trace_flag("DORADO_MACHINE_PCHIST")) {
            uint16_t pcx = cpu->ifu_pcx;
            unsigned op = ((unsigned)(cpu->ifu_insset & 3u) << 8) |
                          (unsigned)cpu->ifu_opcode;
            m->ifu_pcx_hist[pcx]++;
            m->ifu_op_hist[op & 01777]++;
            unsigned r = m->ifu_ring_next++ &
                         ((unsigned)(sizeof m->ifu_ring /
                                     sizeof m->ifu_ring[0]) - 1u);
            m->ifu_ring[r].cycle = bb->cycles;
            m->ifu_ring[r].pcx = pcx;
            m->ifu_ring[r].pcf = cpu->ifu_pcf;
            m->ifu_ring[r].insset = cpu->ifu_insset & 3u;
            m->ifu_ring[r].opcode = cpu->ifu_opcode;
            m->ifu_ring[r].alpha = cpu->ifu_alpha;
            m->ifu_ring[r].beta = cpu->ifu_beta;
            m->ifu_ring[r].len = cpu->ifu_length;
            m->ifu_ring[r].T = cpu->T;
            m->ifu_ring[r].Q = cpu->Q;
            m->ifu_ring[r].Cnt = cpu->Cnt;
            m->ifu_ring[r].StkP = cpu->StkP;
            m->ifu_ring[r].RBase = cpu->RBase;
            m->ifu_ring[r].MemBase = cpu->MemBase;
            for (int si = 0; si < 8; si++)
                m->ifu_ring[r].stk[si] =
                    cpu->STK[(cpu->StkP + si) & 0377];
            for (int ri = 0; ri < 8; ri++)
                m->ifu_ring[r].rm[ri] = cpu->RM[ri];
            m->ifu_ring[r].br31 = dorado_br_get(&m->mem, 31);
            m->ifu_ring[r].br36 = dorado_br_get(&m->mem, 036);
        }

        if (alto_check_trace) {
            const char *nm = pre_pc == 03333 ? "ACheckLoop" :
                             pre_pc == 03337 ? "ANoCheckWord" :
                             pre_pc == 03347 ? "ACheckLast" :
                             pre_pc == 03045 ? "AWaitSector" :
                             pre_pc == 03067 ? "ACmmdInTime" :
                             pre_pc == 03225 ? "ABadCylinder" :
                             pre_pc == 03244 ? "ACmmdSeekOnly" :
                             pre_pc == 03246 ? "ABadSector" :
                             pre_pc == 03301 ? "ACmmdRead" :
                             pre_pc == 03306 ? "AReadBadTW" :
                             pre_pc == 03324 ? "ACheckBadTW" :
                             pre_pc == 03326 ? "AChecksumError" :
                             pre_pc == 03330 ? "ACmmdCheck" :
                             pre_pc == 02355 ? "ACmmdEnd" :
                             pre_pc == 02356 ? "ACmmdEnd+1" :
                             pre_pc == 02357 ? "ACmmdEnd+2" :
                             pre_pc == 02367 ? "ACmmdEnd2" :
                             pre_pc == 02370 ? "ACmmdEnd2+1" :
                             pre_pc == 02371 ? "ACmmdEnd2+2" :
                             pre_pc == 02372 ? "ACmmdEnd2+3" :
                             pre_pc == 02375 ? "ACmmdEnd2Store" :
                             pre_pc == 02376 ? "ACmmdEndStatus" :
                             pre_pc == 03370 ? "ReadCheckEnd" :
                             pre_pc == 03371 ? "ACmmdAbort" : "ACheck";
            fprintf(stderr,
                    "[altocheck] pc=0o%o %-12s @cyc=%llu "
                    "prev=0o%o@%llu prev2=0o%o@%llu "
                    "MD(before)=0o%06o taskMD(before)=0o%06o "
                    "T(before)=0o%06o "
                    "disk_in=0o%06o KAddr=0o%06o Sector=0o%06o "
                    "KPtr=0o%06o KCmmd=0o%06o DskMAddr=0o%06o "
                    "KStatus=0o%06o KTemp0=0o%06o KTemp1=0o%06o "
                    "KTemp2=0o%06o KTemp3=0o%06o "
                    "KCB+0=0o%06o KCB+1=0o%06o VM521=0o%06o "
                    "tmpl[0o%06o]=0o%06o "
                    "next=0o%o\n",
                    pre_pc, nm, (unsigned long long)bb->cycles,
                    alto_check_prev_pc,
                    (unsigned long long)alto_check_prev_cycle,
                    alto_check_prev2_pc,
                    (unsigned long long)alto_check_prev2_cycle,
                    alto_check_md_before, alto_check_task_md_before,
                    alto_check_t_before,
                    m->disk.last_input_data, alto_check_kaddr,
                    alto_check_sector, alto_check_kptr, alto_check_kcmmd,
                    alto_check_dskmaddr, alto_check_kstatus,
                    alto_check_ktemp0, alto_check_ktemp1,
                    alto_check_ktemp2, alto_check_ktemp3,
                    alto_check_kcb0, alto_check_kcb1, alto_check_vm521,
                    (uint16_t)(alto_check_md_before + alto_check_dskmaddr),
                    alto_check_diskbr_word,
                    cpu->real_PC);
            if (pre_pc == 03330) {
                uint16_t base = alto_check_md_before;
                fprintf(stderr, "[altocheck-block] base=0o%06o", base);
                for (uint16_t i = 0; i < 8; i++) {
                    fprintf(stderr, " [%u]=0o%06o", i,
                            dorado_visible_word_at_va(
                                &m->mem, (uint16_t)(base + i)));
                }
                fprintf(stderr, "\n");
            }
        }
        if (is_imfetch && cpu->ctask == DORADO_DISK_TASK &&
            pre_pc == 02376 && dorado_trace_flag("DORADO_ALTO_KCB_TRACE")) {
            uint16_t kptr = cpu->RM[(5u << 4) | 007u];
            machine_trace_alto_kcb_completion(m, kptr, bb->cycles);
        }

        if (alto_check_enabled && is_imfetch &&
            cpu->ctask == DORADO_DISK_TASK) {
            alto_disk_prev2_pc = alto_disk_prev_pc;
            alto_disk_prev2_cycle = alto_disk_prev_cycle;
            alto_disk_prev_pc = pre_pc;
            alto_disk_prev_cycle = bb->cycles;
        }

        if (m->initial_substituted) {
            /* Bring-up guard: clear the EOT load-overflow retry mask
             * Initial's `ELoad_ A0` should have zeroed, until the
             * remaining RM/RBase init issue is fixed. */
            if (eth->tx_on && eth->requests_seen == 0 &&
                eth->tx_count == 0 && eth->data_writes == 0 &&
                cpu->RM[0x19] != 0) {
                cpu->RM[0x19] = 0;
            }

            /* Stage-2 boot server: broadcast breath-of-life / time
             * packets so AEmu's EBoot receives one and the loaded Pup
             * package's unbounded raw-receive completes. */
            if (m->alto_ether_boot && m->ether_loaded_world_cycle &&
                bb->cycles >= m->next_bol_cycle) {
                int sent = (eth->eftp_max_seq > 0)
                    ? dorado_ethernet_time_broadcast(eth)
                    : dorado_ethernet_breath_of_life(eth);
                uint64_t interval = sent ? 2000000u : 100000u;
                if (sent && eth->time_bcasts > 3) interval = 50000000u;
                /* Once the world is loaded (post-EFTP), drop to the real
                 * gateway-info cadence -- ~one every 30s per the Pup spec
                 * (IFS GatewayInformationProtocol), not a flood. A flood
                 * lands an input completion in EPLOC during the loaded
                 * world's Tx-completion poll and derails CedarNetExec's
                 * Ethernet init; one early packet (delivered when its Rx is
                 * armed) is enough, and the 30s gap keeps subsequent
                 * broadcasts clear of its Tx poll. 500M cyc ~ 30s @60ns. */
                if (sent && eth->eftp_max_seq > 0) interval = 500000000u;
                m->next_bol_cycle = bb->cycles + interval;
            }

            uint16_t eth_mask = dorado_ethernet_wakeup_mask(eth);
            for (int task = 0; task < 16; task++) {
                if (eth_mask & (uint16_t)(1u << task)) {
                    dorado_cpu_wakeup(cpu, task);
                }
            }

            /* Display scan-line wakeups (DHT/AHT) and DWT word-task
             * wakeups, every ~1000 cycles -- but only once the loaded world
             * has installed a sane Alto DCB chain. Most Alto software uses
             * MDS+0420; Interlisp-D's handoff can leave the visible display
             * state in the IOBR bank and one word later. Waking the display
             * tasks before a real chain exists can run stale pre-LoadRam
             * task TPCs at high priority. */
            /* Pilot boot data can transiently resemble an Alto DCB before
             * the germ has been planted; waking the freshly loaded task 3
             * there runs an uninitialised TPC and starves GermBoot.  Once
             * the germ is live, Cedar installs a real DCB chain and uses the
             * same scanline wake source.  The DDC predicate remains limited
             * to non-germ (Alto/Interlisp) worlds below. */
            int display_active = machine_alto_display_active(&m->mem) &&
                                 (!m->germ_word_count || m->germ_data_done);
            if (!m->germ_word_count)
                display_active |= machine_ddc_display_active(m);
            if (!display_active && dorado_trace_flag("DORADO_FORCE_DISPLAY_WAKE")) {
                display_active = 1;
            }
            if (display_active && bb->cycles >= m->next_display_scanline_cycle) {
                uint16_t mask = dorado_display_scanline_wakeup_mask(disp);
                for (int task = 0; task < 16; task++) {
                    if (mask & (uint16_t)(1u << task)) {
                        if (cpu->task_tpc[task] != 0177777)
                            dorado_cpu_wakeup(cpu, task);
                    }
                }
                /* Scanline cadence experiment knob (DORADO_SCANLINE_CYCLES,
                 * default 1000). Gated to display_active so it touches only
                 * the running world, not the boot. Measured against the
                 * M[3016] tracediff, not pixels. */
                static long scanline_cycles = -1;
                if (scanline_cycles < 0) {
                    const char *w = getenv("DORADO_SCANLINE_CYCLES");
                    scanline_cycles = (w && atol(w) > 0) ? atol(w) : 1000;
                }
                m->next_display_scanline_cycle =
                    bb->cycles + (uint64_t)scanline_cycles;
            }
            int dwt_subtask = 0;
            if (display_active && dorado_display_dwt_wakeup(disp, &dwt_subtask)) {
                int word_task = disp->terminal_task == DORADO_DISPLAY_TASK_AHT
                                    ? DORADO_DISPLAY_TASK_AWT
                                    : DORADO_DISPLAY_TASK_DWT;
                if (cpu->task_tpc[word_task] != 0177777) {
                    dorado_cpu_set_subtask(cpu, word_task,
                                           (uint8_t)dwt_subtask);
                    dorado_cpu_wakeup(cpu, word_task);
                }
            }

            /* Disk: advance the clock-driven sector/index timing. Wake the DSK
             * task ONLY for a real Trident pack. The PDI/Cedar boot is
             * EMU-task-driven (DiskBootSoft/BootTransferLp run on task 0); the
             * DSK-task disk driver isn't installed during the germ boot, so
             * waking it runs its uninitialized TPC in a non-yielding busy-spin
             * that starves the emulator task and freezes the boot. (That was a
             * regression of the D1 wakeup on the experimental --disk-real PDI
             * path; the default shim path never enabled it.) The PDI is still
             * clock-ticked so the sector/index state the EMU boot transfer
             * polls advances. */
            (void)dorado_disk_controller_tick(&m->disk, bb->cycles);
            if (m->disk.drive[m->disk.selected_drive].pack &&
                dorado_disk_controller_wakeup_pending(&m->disk) &&
                cpu->task_tpc[DORADO_DISK_TASK] != 0177777) {
                dorado_cpu_wakeup(cpu, DORADO_DISK_TASK);
            }
        }
    }
    return bb->cycles;
}

uint64_t dorado_machine_cycles(const dorado_machine *m)
{
    return m ? m->bb.cycles : 0;
}

int dorado_machine_booted(const dorado_machine *m)
{
    return m && m->ether_loaded_world_cycle != 0;
}

void dorado_machine_set_key(dorado_machine *m, dorado_display_key key,
                            int down)
{
    if (!m) return;
    dorado_display_keyboard_set_key(&m->display, key, down ? 1 : 0);
}

void dorado_machine_set_ftp_source(dorado_machine *m, const char *sysout,
                                   const char *root)
{
    if (!m) return;
    if (sysout && *sysout)
        dorado_ethernet_set_ftp_sysout(&m->ethernet, sysout);
    if (root && *root)
        dorado_ethernet_set_ftp_root(&m->ethernet, root);
}

int dorado_machine_set_pilot_disk(dorado_machine *m, int slot,
                                  const char *path)
{
    char err[128];
    if (!m || !path || !*path || slot < 0 || slot >= DORADO_DISK_NUM_DRIVES)
        return -1;
    if (m->pilot_pdi_loaded[slot]) {
        dorado_pdi_free(&m->pilot_pdi[slot]);
        m->pilot_pdi_loaded[slot] = 0;
    }
    if (dorado_pdi_load(path, &m->pilot_pdi[slot], err, sizeof err) != 0) {
        fprintf(stderr, "dorado: cannot load Pilot disk %d '%s': %s\n",
                slot, path, err);
        return -1;
    }
    m->pilot_pdi_loaded[slot] = 1;
    snprintf(machine_pdi_path[slot], sizeof machine_pdi_path[slot], "%s",
             path);
    dorado_disk_controller_attach_pdi(&m->disk, slot, &m->pilot_pdi[slot]);
    m->disk.allow_pdi_timing = (uint8_t)m->disk_real;
    m->disk_attached = 1;
    if (slot == 0) {
        m->pilot_pdi_stream_active = 0;
        m->pilot_pdi_next_page = 0;
    }
    return 0;
}

int dorado_machine_interactive(const dorado_machine *m)
{
    return m && m->keys_live;
}

void dorado_machine_set_mouse(dorado_machine *m, int x, int y, int buttons)
{
    if (!m) return;
    if (x < 0) x = 0; else if (x > DORADO_DISPLAY_W - 1) x = DORADO_DISPLAY_W - 1;
    if (y < 0) y = 0; else if (y > DORADO_DISPLAY_H - 1) y = DORADO_DISPLAY_H - 1;
    m->mouse_present = 1;
    m->mouse_x = x;
    m->mouse_y = y;
    m->mouse_buttons = buttons;
}

dorado_display *dorado_machine_display(dorado_machine *m)
{
    return m ? &m->display : NULL;
}

static const char *machine_ref_kind_name(dorado_ref_kind kind)
{
    switch (kind) {
    case DM_REF_NONE:      return "NONE";
    case DM_REF_PREFETCH:  return "PREFETCH";
    case DM_REF_MAP:       return "MAP";
    case DM_REF_IOFETCH:   return "IOFETCH";
    case DM_REF_LONGFETCH: return "LONGFETCH";
    case DM_REF_STORE:     return "STORE";
    case DM_REF_DUMMYREF:  return "DUMMYREF";
    case DM_REF_FLUSH:     return "FLUSH";
    case DM_REF_IOSTORE:   return "IOSTORE";
    case DM_REF_IFETCH:    return "IFETCH";
    case DM_REF_FETCH:     return "FETCH";
    case DM_REF_RMAP:      return "RMAP";
    }
    return "?";
}

static const char *machine_fault_name(dorado_fault_kind fault)
{
    switch (fault) {
    case DM_FAULT_NONE:          return "NONE";
    case DM_FAULT_PAGE:          return "PAGE";
    case DM_FAULT_WRITE_PROTECT: return "WRITE_PROTECT";
    case DM_FAULT_MAP_TROUBLE:   return "MAP_TROUBLE";
    case DM_FAULT_STORAGE_ERROR: return "STORAGE_ERROR";
    }
    return "?";
}

static uint32_t parse_octal_word(const char **pp)
{
    const char *p = *pp;
    while (*p == ' ' || *p == '\t' || *p == ',') p++;
    if (p[0] == '0' && (p[1] == 'o' || p[1] == 'O')) p += 2;
    char *end = NULL;
    unsigned long v = strtoul(p, &end, 8);
    *pp = end ? end : p;
    return (uint32_t)v;
}

static int printable_ascii(uint16_t ch)
{
    return ch >= 040 && ch <= 0176;
}

static void machine_dump_env_vm_words(dorado_machine *m)
{
    const char *p = getenv("DORADO_VM_DUMP");
    if (!p || !*p) return;

    while (*p) {
        uint32_t va = parse_octal_word(&p);
        int count = 16;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == ':' || *p == '+') {
            p++;
            count = (int)parse_octal_word(&p);
        }
        if (count < 1) count = 1;
        if (count > 256) count = 256;

        char label[64];
        snprintf(label, sizeof label, "VM_DUMP %07o", va & 01777777777u);
        machine_dump_words_at_va(&m->mem, label, va, count);

        while (*p == ' ' || *p == '\t') p++;
        if (*p == ',') p++;
        else if (*p) break;
    }
}

static void machine_find_env_vm_pair(dorado_machine *m)
{
    const char *p = getenv("DORADO_VM_FIND_PAIR");
    if (!m || !m->mem.storage || !p || !*p)
        return;

    uint32_t a = parse_octal_word(&p) & 0177777u;
    if (*p == ',' || *p == ':' || *p == '+') p++;
    uint32_t b = parse_octal_word(&p) & 0177777u;
    unsigned limit = 32;
    if (*p == ',' || *p == ':' || *p == '+') {
        p++;
        limit = parse_octal_word(&p);
        if (limit == 0) limit = 32;
        if (limit > 256) limit = 256;
    }

    fprintf(stderr, "[machine] VM_FIND_PAIR %06o %06o:", a, b);
    unsigned n = 0;
    for (size_t i = 0; i + 1 < m->mem.storage_words && n < limit; i++) {
        if (m->mem.storage[i] != (uint16_t)a ||
            m->mem.storage[i + 1] != (uint16_t)b)
            continue;
        fprintf(stderr, " %07o", (unsigned)i & 017777777u);
        n++;
    }
    if (!n)
        fprintf(stderr, " none");
    fprintf(stderr, "\n");
}

static void machine_dump_raw_storage_range(dorado_memory *mem,
                                           uint32_t start, int count)
{
    if (!mem || !mem->storage || count <= 0)
        return;
    if ((size_t)start >= mem->storage_words) {
        fprintf(stderr, "[machine] STORAGE_DUMP %07o: out-of-range\n",
                start & 017777777u);
        return;
    }
    if ((size_t)count > mem->storage_words - start)
        count = (int)(mem->storage_words - start);

    fprintf(stderr, "[machine] STORAGE_DUMP %07o +%o", start, count);
    for (int i = 0; i < count; i += 8) {
        int n = count - i;
        if (n > 8) n = 8;
        fprintf(stderr, "\n  %07o:", (start + (uint32_t)i) & 017777777u);
        for (int j = 0; j < n; j++) {
            uint32_t off = start + (uint32_t)i + (uint32_t)j;
            fprintf(stderr, " %06o", mem->storage[off]);
        }
        fprintf(stderr, "  |");
        for (int j = 0; j < n; j++) {
            uint32_t off = start + (uint32_t)i + (uint32_t)j;
            uint16_t w = mem->storage[off];
            uint16_t hi = (w >> 8) & 0377u;
            uint16_t lo = w & 0377u;
            fputc(printable_ascii(hi) ? (char)hi : '.', stderr);
            fputc(printable_ascii(lo) ? (char)lo : '.', stderr);
        }
        fputc('|', stderr);
    }
    fputc('\n', stderr);
}

static void machine_dump_env_storage_words(dorado_machine *m)
{
    const char *p = getenv("DORADO_STORAGE_DUMP");
    if (!m || !m->mem.storage || !p || !*p)
        return;

    while (*p) {
        uint32_t start = parse_octal_word(&p);
        int count = 16;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == ':' || *p == '+') {
            p++;
            count = (int)parse_octal_word(&p);
        }
        if (count < 1) count = 1;
        if (count > 512) count = 512;

        machine_dump_raw_storage_range(&m->mem, start, count);

        while (*p == ' ' || *p == '\t') p++;
        if (*p == ',') p++;
        else if (*p) break;
    }
}

static void machine_dump_inverse_rp(dorado_machine *m)
{
    const char *env = getenv("DORADO_FIND_RP");
    if (!m || !env || !*env) return;
    char *end = NULL;
    unsigned long rp = strtoul(env, &end, 0);
    if (!end || *end) return;
    fprintf(stderr, "[find-rp] rp=%04lo virtual pages:", rp);
    int found = 0;
    for (uint32_t vp = 0; vp < DM_MAP_ENTRIES; vp++) {
        const dorado_map_entry *e = dorado_map_get(&m->mem, vp);
        if (e->rp != (uint16_t)rp || (e->wp && e->dirty)) continue;
        fprintf(stderr, " %06o(%c%c%c)", vp,
                e->wp ? 'w' : '-', e->dirty ? 'd' : '-', e->ref ? 'r' : '-');
        found++;
    }
    fprintf(stderr, "%s\n", found ? "" : " none");
}

static void print_lisp_pname_candidate(dorado_memory *mem, const char *tag,
                                       uint32_t va)
{
    uint16_t w0 = dorado_visible_word_at_va(mem, va);
    unsigned len = (w0 >> 8) & 0377u;
    unsigned pad = w0 & 0377u;
    if (len == 0 || len > 80)
        return;

    char s[81];
    for (unsigned i = 0; i < len && i < sizeof s - 1; i++) {
        uint16_t w = dorado_visible_word_at_va(mem, va + (i + 1u) / 2u);
        uint16_t ch = (i & 1u) ? ((w >> 8) & 0377u) : (w & 0377u);
        if (!printable_ascii(ch))
            return;
        s[i] = (char)ch;
    }
    s[len] = '\0';
    fprintf(stderr, " pname[%s]@%07o len=%u pad=%03o \"%s\"",
            tag, va & 01777777777u, len, pad, s);
}

static int lisp_pname_equals(dorado_memory *mem, uint32_t va,
                             const char *wanted)
{
    if (!mem || !wanted) return 0;
    uint16_t w0 = dorado_visible_word_at_va(mem, va);
    unsigned len = (w0 >> 8) & 0377u;
    if (len != strlen(wanted)) return 0;
    for (unsigned i = 0; i < len; i++) {
        uint16_t w = dorado_visible_word_at_va(mem, va + (i + 1u) / 2u);
        uint16_t ch = (i & 1u) ? ((w >> 8) & 0377u) : (w & 0377u);
        if (ch != (unsigned char)wanted[i]) return 0;
    }
    return 1;
}

static void dump_lisp_atom_space(dorado_memory *mem, const char *name,
                                 int br, uint32_t idx, int try_pname);

/* DORADO_LISP_FIND_ATOM=NAME[,NAME...] is a read-only live-sysout probe.
 * Lyric LLPARAMS assigns PNAME and PLIST to fixed virtual spaces {010,0}
 * and {002,0}.  The running Lisp microcode keeps DEF and TOPVAL bases in
 * DefBR (4) and ValSpaceBR (035).  Each table has one two-word entry per
 * atom (LISPDEFS.mc). */
static void machine_find_lisp_atoms(dorado_machine *m)
{
    const char *env = getenv("DORADO_LISP_FIND_ATOM");
    if (!m || !env || !*env) return;

    char names[512];
    snprintf(names, sizeof names, "%s", env);
    for (char *wanted = strtok(names, ","); wanted;
         wanted = strtok(NULL, ",")) {
        int found = 0;
        const uint32_t pname_base = 010u << 16;
        for (uint32_t atom = 0; atom <= 0177777u; atom++) {
            uint32_t entry = (pname_base + atom * 2u) & 01777777777u;
            uint16_t hi = dorado_visible_word_at_va(&m->mem, entry);
            uint16_t lo = dorado_visible_word_at_va(&m->mem, entry + 1u);
            uint32_t pname = (((uint32_t)(hi & 0377u)) << 16) | lo;
            if (!lisp_pname_equals(&m->mem, pname, wanted)) continue;

            uint32_t def = (dorado_br_get(&m->mem, 004) + atom * 2u) &
                           01777777777u;
            uint32_t val = (dorado_br_get(&m->mem, 035) + atom * 2u) &
                           01777777777u;
            uint32_t plist = ((002u << 16) + atom * 2u) &
                             01777777777u;
            fprintf(stderr,
                    "[lisp-find-atom] %s atom=%06o pname=%07o "
                    "def@%07o={%06o,%06o} "
                    "value@%07o={%06o,%06o} "
                    "plist@%07o={%06o,%06o}\n",
                    wanted, atom, pname,
                    def, dorado_visible_word_at_va(&m->mem, def),
                    dorado_visible_word_at_va(&m->mem, def + 1u),
                    val, dorado_visible_word_at_va(&m->mem, val),
                    dorado_visible_word_at_va(&m->mem, val + 1u),
                    plist, dorado_visible_word_at_va(&m->mem, plist),
                    dorado_visible_word_at_va(&m->mem, plist + 1u));
            found = 1;
            if (found) break;
        }
        if (!found)
            fprintf(stderr, "[lisp-find-atom] %s not found\n", wanted);
    }
}

/* DORADO_LISP_FIND_DEF=VA (octal): find the atom whose DEF entry points at
 * the active compiled-code object.  This is a read-only aid for identifying
 * a spinning Interlisp frame from IFU BR37. */
static void machine_find_lisp_def(dorado_machine *m)
{
    const char *env = getenv("DORADO_LISP_FIND_DEF");
    if (!m || !env || !*env) return;

    char *end = NULL;
    uint32_t wanted = (uint32_t)strtoul(env, &end, 8) & 01777777777u;
    if (end == env) return;

    const uint32_t pname_base = 010u << 16;
    const uint32_t def_base = dorado_br_get(&m->mem, 004);
    int found = 0;
    for (uint32_t atom = 0; atom <= 0177777u; atom++) {
        uint32_t def_entry = (def_base + atom * 2u) & 01777777777u;
        uint16_t def_hi = dorado_visible_word_at_va(&m->mem, def_entry);
        uint16_t def_lo = dorado_visible_word_at_va(&m->mem, def_entry + 1u);
        uint32_t def = (((uint32_t)(def_hi & 0377u)) << 16) | def_lo;
        if (def > wanted || wanted - def >= 0400u) continue;

        uint32_t pname_entry = (pname_base + atom * 2u) & 01777777777u;
        uint16_t pname_hi = dorado_visible_word_at_va(&m->mem, pname_entry);
        uint16_t pname_lo = dorado_visible_word_at_va(&m->mem,
                                                       pname_entry + 1u);
        uint32_t pname = (((uint32_t)(pname_hi & 0377u)) << 16) | pname_lo;
        fprintf(stderr,
                "[lisp-find-def] active=%07o def=%07o delta=%04o atom=%06o",
                wanted, def, wanted - def, atom);
        print_lisp_pname_candidate(&m->mem, "PNAME", pname);
        fputc('\n', stderr);
        found = 1;
    }
    if (!found)
        fprintf(stderr, "[lisp-find-def] def=%07o not found\n", wanted);
}

static void dump_lisp_atom_space(dorado_memory *mem, const char *name,
                                 int br, uint32_t idx, int try_pname)
{
    uint32_t base = dorado_br_get(mem, br);
    uint32_t va1 = (base + idx) & 01777777777u;
    uint32_t va2 = (base + idx * 2u) & 01777777777u;
    uint16_t a0 = dorado_visible_word_at_va(mem, va1);
    uint16_t a1 = dorado_visible_word_at_va(mem, va1 + 1u);
    uint16_t b0 = dorado_visible_word_at_va(mem, va2);
    uint16_t b1 = dorado_visible_word_at_va(mem, va2 + 1u);

    fprintf(stderr,
            "\n  %-5s BR%02o=%07o +idx=%07o:{%06o,%06o} "
            "+2idx=%07o:{%06o,%06o}",
            name, br, base, va1, a0, a1, va2, b0, b1);

    if (try_pname) {
        uint32_t xp1 = (((uint32_t)(a0 & 0377u)) << 16) | a1;
        uint32_t xp2 = (((uint32_t)(b0 & 0377u)) << 16) | b1;
        uint32_t hi_lo1 = ((((uint32_t)a0) << 16) | a1) & 01777777777u;
        uint32_t hi_lo2 = ((((uint32_t)b0) << 16) | b1) & 01777777777u;
        fprintf(stderr,
                " xptrs={packed:%07o,%07o full:%07o,%07o}",
                xp1, xp2, hi_lo1, hi_lo2);
        print_lisp_pname_candidate(mem, "packed+idx", xp1);
        print_lisp_pname_candidate(mem, "packed+2idx", xp2);
        print_lisp_pname_candidate(mem, "full+idx", hi_lo1);
        print_lisp_pname_candidate(mem, "full+2idx", hi_lo2);
    }
}

static void machine_dump_lisp_atom_probe(dorado_machine *m)
{
    const char *w = getenv("DORADO_LISP_ATOM_PROBE");
    if (!m || !w || !*w)
        return;

    const char *p = w;
    fprintf(stderr, "[lisp-atom-probe] indices:");
    int n = 0;
    while (*p && n++ < 32) {
        const char *before = p;
        uint32_t idx = parse_octal_word(&p) & 0177777u;
        if (p == before)
            break;
        fprintf(stderr, " %06o", idx);
        fprintf(stderr,
                "\n[lisp-atom-probe] atom=%06o atomObj={%06o,%06o}",
                idx,
                dorado_visible_word_at_va(&m->mem,
                                          dorado_br_get(&m->mem, 000) + idx),
                dorado_visible_word_at_va(&m->mem,
                                          dorado_br_get(&m->mem, 000) + idx + 1u));
        dump_lisp_atom_space(&m->mem, "ATOM",  000, idx, 0);
        dump_lisp_atom_space(&m->mem, "PLIST", 002, idx, 0);
        dump_lisp_atom_space(&m->mem, "PNAME", 010, idx, 1);
        dump_lisp_atom_space(&m->mem, "DEF",   012, idx, 0);
        dump_lisp_atom_space(&m->mem, "VAL",   014, idx, 0);
        fputc('\n', stderr);
        while (*p == ' ' || *p == '\t' || *p == ',') p++;
        if (!*p)
            break;
    }
}

void dorado_machine_debug(dorado_machine *m)
{
    if (!m) return;
    /* DORADO_FIND_BCD_HEADER is a final-state transport probe for the Cedar
     * STP path.  RPCRuntime.bcd begins 0500,F9E0,7CC6,DC08 (big-endian
     * Mesa words).  Looking for that sequence in physical storage tells us
     * whether PupStream/File has retained the received BCD at all; it does
     * not alter guest state. */
    if (dorado_trace_flag("DORADO_FIND_BCD_HEADER") && m->mem.storage) {
        static const uint16_t bcd_prefix[] = {
            02400u, 0174740u, 076306u, 0156010u
        };
        size_t matches = 0;
        for (size_t i = 0; i + sizeof bcd_prefix / sizeof bcd_prefix[0] <=
                           m->mem.storage_words; i++) {
            if (m->mem.storage[i] != bcd_prefix[0] ||
                m->mem.storage[i + 1] != bcd_prefix[1] ||
                m->mem.storage[i + 2] != bcd_prefix[2] ||
                m->mem.storage[i + 3] != bcd_prefix[3])
                continue;
            fprintf(stderr,
                    "[machine] BCD RPCRuntime prefix at physical 0o%07o "
                    "words={0o%o,0o%o,0o%o,0o%o}\n",
                    (unsigned)i, m->mem.storage[i], m->mem.storage[i + 1],
                    m->mem.storage[i + 2], m->mem.storage[i + 3]);
            matches++;
            if (matches == 16) break;
        }
        if (!matches)
            fprintf(stderr, "[machine] BCD RPCRuntime prefix not in storage\n");
    }
    machine_dump_inverse_rp(m);
    machine_find_lisp_atoms(m);
    machine_find_lisp_def(m);
    /* DORADO_IM_DUMP="lo,hi" (octal): disassemble the LOADED control store
     * (m->mc.im, which Write IM/LoadRam has populated with the running
     * world's microcode) over real IM addresses lo..hi. Lets us read what
     * a given real pc does in the loaded DoradoLispMc without a symbol map. */
    {
        const char *env = getenv("DORADO_IM_DUMP");
        if (env && env[0]) {
            unsigned lo = 0, hi = 0;
            if (sscanf(env, "%o,%o", &lo, &hi) == 2 && hi < 4096 && lo <= hi) {
                for (unsigned a = lo; a <= hi; a++) {
                    char buf[256];
                    const dorado_uinstr *u = &m->mc.im[a];
                    dorado_format(u, buf, sizeof buf);
                    const char *sym =
                        dorado_microcode_symbol_at_real(&m->mc, (uint16_t)a);
                    fprintf(stderr, "IMDIS 0o%04o %-8s : %s\n",
                            a, sym ? sym : "", buf);
                }
            }
        }
    }
    if (dorado_trace_flag("DORADO_MACHINE_PCHIST")) {
        /* Top 12 hottest task-0 PCs in the loaded world. */
        int top[12]; for (int i = 0; i < 12; i++) top[i] = -1;
        for (int a = 0; a < 4096; a++) {
            if (!m->pchist[a]) continue;
            for (int s = 0; s < 12; s++) {
                if (top[s] < 0 || m->pchist[a] > m->pchist[top[s]]) {
                    for (int t = 11; t > s; t--) top[t] = top[t-1];
                    top[s] = a; break;
                }
            }
        }
        fprintf(stderr, "[machine] task-0 hot PCs:");
        for (int i = 0; i < 12 && top[i] >= 0; i++)
            fprintf(stderr, " 0o%o=%u", top[i], m->pchist[top[i]]);
        for (int i = 0; i < 12; i++) top[i] = -1;
        for (int a = 0; a < 4096; a++) {
            if (!m->pchist_all[a]) continue;
            for (int s = 0; s < 12; s++) {
                if (top[s] < 0 || m->pchist_all[a] > m->pchist_all[top[s]]) {
                    for (int t = 11; t > s; t--) top[t] = top[t-1];
                    top[s] = a; break;
                }
            }
        }
        fprintf(stderr, "\n[machine] all-task hot PCs:");
        for (int i = 0; i < 12 && top[i] >= 0; i++)
            fprintf(stderr, " 0o%o=%u", top[i], m->pchist_all[top[i]]);
        for (int task = 0; task < 16; task++) {
            uint64_t total = 0;
            for (int a = 0; a < 4096; a++)
                total += machine_pchist_task[task][a];
            if (!total) continue;
            for (int i = 0; i < 6; i++) top[i] = -1;
            for (int a = 0; a < 4096; a++) {
                if (!machine_pchist_task[task][a]) continue;
                for (int s = 0; s < 6; s++) {
                    if (top[s] < 0 ||
                        machine_pchist_task[task][a] >
                            machine_pchist_task[task][top[s]]) {
                        for (int t = 5; t > s; t--) top[t] = top[t-1];
                        top[s] = a; break;
                    }
                }
            }
            fprintf(stderr, "\n[machine] task-%o hot PCs (%llu):",
                    task, (unsigned long long)total);
            for (int i = 0; i < 6 && top[i] >= 0; i++)
                fprintf(stderr, " 0o%o=%u", top[i],
                        machine_pchist_task[task][top[i]]);
        }
        int ifutop[12]; for (int i = 0; i < 12; i++) ifutop[i] = -1;
        for (int a = 0; a < 65536; a++) {
            if (!m->ifu_pcx_hist[a]) continue;
            for (int s = 0; s < 12; s++) {
                if (ifutop[s] < 0 ||
                    m->ifu_pcx_hist[a] > m->ifu_pcx_hist[ifutop[s]]) {
                    for (int t = 11; t > s; t--) ifutop[t] = ifutop[t-1];
                    ifutop[s] = a; break;
                }
            }
        }
        fprintf(stderr, "\n[machine] IFU hot PCX:");
        for (int i = 0; i < 12 && ifutop[i] >= 0; i++)
            fprintf(stderr, " %06o=%u", ifutop[i],
                    m->ifu_pcx_hist[ifutop[i]]);
        for (int i = 0; i < 12; i++) ifutop[i] = -1;
        for (int a = 0; a < 1024; a++) {
            if (!m->ifu_op_hist[a]) continue;
            for (int s = 0; s < 12; s++) {
                if (ifutop[s] < 0 ||
                    m->ifu_op_hist[a] > m->ifu_op_hist[ifutop[s]]) {
                    for (int t = 11; t > s; t--) ifutop[t] = ifutop[t-1];
                    ifutop[s] = a; break;
                }
            }
        }
        fprintf(stderr, "\n[machine] IFU hot ops:");
        for (int i = 0; i < 12 && ifutop[i] >= 0; i++)
            fprintf(stderr, " ins%u/op%03o=%u",
                    (unsigned)(ifutop[i] >> 8), ifutop[i] & 0377,
                    m->ifu_op_hist[ifutop[i]]);
        fprintf(stderr, "\n[machine] IFU last dispatches:");
        unsigned ring_len = (unsigned)(sizeof m->ifu_ring /
                                       sizeof m->ifu_ring[0]);
        unsigned have = m->ifu_ring_next < ring_len ? m->ifu_ring_next
                                                    : ring_len;
        for (unsigned i = 0; i < have; i++) {
            unsigned idx = (m->ifu_ring_next - have + i) & (ring_len - 1u);
            fprintf(stderr,
                    " [%llu pcx=%06o pcf=%06o i%u op=%03o a=%03o b=%03o "
                    "l=%u T=%06o Q=%06o C=%06o sp=%03o rb=%02o mb=%02o "
                    "br31=%07o br36=%07o stk=%06o,%06o,%06o,%06o "
                    "rm0=%06o,%06o,%06o,%06o]",
                    (unsigned long long)m->ifu_ring[idx].cycle,
                    m->ifu_ring[idx].pcx, m->ifu_ring[idx].pcf,
                    m->ifu_ring[idx].insset, m->ifu_ring[idx].opcode,
                    m->ifu_ring[idx].alpha, m->ifu_ring[idx].beta,
                    m->ifu_ring[idx].len, m->ifu_ring[idx].T,
                    m->ifu_ring[idx].Q, m->ifu_ring[idx].Cnt,
                    m->ifu_ring[idx].StkP & 0377,
                    m->ifu_ring[idx].RBase & 017,
                    m->ifu_ring[idx].MemBase & 037,
                    m->ifu_ring[idx].br31, m->ifu_ring[idx].br36,
                    m->ifu_ring[idx].stk[0], m->ifu_ring[idx].stk[1],
                    m->ifu_ring[idx].stk[2], m->ifu_ring[idx].stk[3],
                    m->ifu_ring[idx].rm[0], m->ifu_ring[idx].rm[1],
                    m->ifu_ring[idx].rm[2], m->ifu_ring[idx].rm[3]);
        }
        fprintf(stderr,
                "\n[machine] tasks: ctask=%o ready=%06o wake=%06o "
                "pipe={%06o,%06o} tasking=%u",
                m->cpu.ctask, m->cpu.ready, m->cpu.wakeup_pending,
                m->cpu.wakeup_pipe[0], m->cpu.wakeup_pipe[1],
                m->cpu.tasking_on);
        for (int t = 0; t < 16; t++) {
            uint16_t tpc = dorado_cpu_get_task_tpc(&m->cpu, t);
            if (tpc || (m->cpu.ready & (1u << t)) ||
                (m->cpu.wakeup_pending & (1u << t)) || t == m->cpu.ctask) {
                fprintf(stderr,
                        " t%o:tpc=0o%o T=%06o L=%06o MB=%02o RB=%02o",
                        t, tpc, m->cpu.task_t[t], m->cpu.task_link[t],
                        m->cpu.task_membase[t], m->cpu.task_rbase[t]);
            }
        }
        static const struct { unsigned pc; const char *name; } fieldpc[] = {
            {01745, "ENDOFFIELD"}, {03546, "EVENFIELD"},
            {03756, "RTCCARRY"},   {01276, "STARTCOUNTERS"},
            {01766, "THTNEWFIELD"}, {01376, "SETDISPLAYFIELDRATE"},
            {02326, "INITIATEINT"}, {02325, "RESCHEDPENDING"},
        };
        fprintf(stderr, "\n[machine] AEmu field/RTC handler hits (all tasks):");
        for (size_t i = 0; i < sizeof fieldpc / sizeof fieldpc[0]; i++)
            fprintf(stderr, " %s(0o%o)=%u", fieldpc[i].name, fieldpc[i].pc,
                    fieldpc[i].pc < 4096 ? m->pchist_all[fieldpc[i].pc] : 0u);
        fprintf(stderr, "\n[machine] task-0 init sequence (first %d):\n",
                m->initseq_n);
        for (int i = 0; i < m->initseq_n; i++) {
            fprintf(stderr, " %o%s", m->initseq[i],
                    (i % 16 == 15) ? "\n" : "");
        }
        fprintf(stderr, "\n");
    }
    dorado_ethernet *e = &m->ethernet;
    uint32_t mds = dorado_br_get(&m->mem, 036);
    uint16_t dastart = dorado_visible_word_at_va(&m->mem, mds + 0420u);
    fprintf(stderr,
            "[machine] booted=%d cyc=%llu pc=0o%o tk=%u | eth: rx=%u tx=%u "
            "req=%llu repl=%llu eftp_r=%llu eftp_q=%llu seq=%u bol=%llu "
            "time=%llu | DASTART=%06o stwords=%zu\n",
            dorado_machine_booted(m),
            (unsigned long long)m->bb.cycles,
            m->cpu.real_PC, m->cpu.ctask,
            e->rx_on, e->tx_on,
            (unsigned long long)e->requests_seen,
            (unsigned long long)e->replies_queued,
            (unsigned long long)e->eftp_requests_seen,
            (unsigned long long)e->eftp_replies_queued,
            e->eftp_max_seq,
            (unsigned long long)e->bol_queued,
            (unsigned long long)e->time_bcasts,
            dastart, m->mem.storage_words);
    if (e->ftp_enabled) {
        fprintf(stderr,
                "[machine] ftp: open=%u phase=%u tx_mode=%u tx_step=%u "
                "pend_ack=%u wait_ack=%u rx_next=%08x tx_next=%08x "
                "tx_last=%08x last_ack=%08x file=%u/%u cmd=%03o/%zu "
                "seen=%llu queued=%llu rxq=%zu/%zu hold=%u rxon=%u "
                "nowake=%u world=%u eitctl=0o%o eitwrites=%llu reads=%llu "
                "client=%06o/%o/%o server=%06o/%o/%o alloc=%u/%u/%u\n",
                e->ftp_open, e->ftp_phase, e->ftp_tx_mode, e->ftp_tx_step,
                e->ftp_pending_ack, e->ftp_waiting_for_ack,
                e->ftp_rx_next, e->ftp_tx_next, e->ftp_tx_last_end,
                e->ftp_last_ack, e->ftp_file_pos, e->ftp_file_size,
                e->ftp_cmd_mark, e->ftp_cmd_len,
                (unsigned long long)e->ftp_packets_seen,
                (unsigned long long)e->ftp_packets_queued,
                e->rx_pos, e->rx_count, e->rx_hold, e->rx_on,
                e->no_wakeups, e->world_rx_words,
                e->control_last[DORADO_ETHERNET_TASK_EIT],
                (unsigned long long)e->control_writes[DORADO_ETHERNET_TASK_EIT],
                (unsigned long long)e->data_reads,
                e->ftp_client_net_host, e->ftp_client_sock_hi,
                e->ftp_client_sock_lo, e->ftp_server_net_host,
                e->ftp_server_sock_hi, e->ftp_server_sock_lo,
                e->ftp_client_bytes_per_pup, e->ftp_client_pup_alloc,
                e->ftp_client_byte_alloc);
    }
    fprintf(stderr,
            "[machine] RM raw: rm000=%06o rm006=%06o rm025=%06o rm026=%06o "
            "clock260..263={%06o,%06o,%06o,%06o} "
            "old165..167={%06o,%06o,%06o} WakeupTime274=%06o\n",
            m->cpu.RM[0000] & 0177777, m->cpu.RM[0006] & 0177777,
            m->cpu.RM[0025] & 0177777, m->cpu.RM[0026] & 0177777,
            m->cpu.RM[0260] & 0177777, m->cpu.RM[0261] & 0177777,
            m->cpu.RM[0262] & 0177777, m->cpu.RM[0263] & 0177777,
            m->cpu.RM[0165] & 0177777, m->cpu.RM[0166] & 0177777,
            m->cpu.RM[0167] & 0177777, m->cpu.RM[0274] & 0177777);
    fprintf(stderr,
            "[machine] cpu: T=%06o Q=%06o Cnt=%06o StkP=%03o ShC=%06o "
            "Link=%06o MemBase=%02o RBase=%02o TIOA=%03o Md=%06o "
            "MdValid=%u mdReady=%llu ifu={active=%u warm=%u ins=%u "
            "op=%03o alpha=%03o beta=%03o len=%u n=%u pcx=%06o "
            "pcf=%06o idcnt=%u dispatch=%llu} stk0..7:",
            m->cpu.T, m->cpu.Q, m->cpu.Cnt, m->cpu.StkP & 0377,
            m->cpu.ShC, m->cpu.Link, m->cpu.MemBase & 037,
            m->cpu.RBase & 017, m->cpu.TIOA & 0377,
            m->mem.md & 0177777, m->cpu.task_md_valid[m->cpu.ctask],
            (unsigned long long)m->cpu.task_md_ready[m->cpu.ctask],
            m->cpu.ifu_active, m->cpu.ifu_warmup,
            m->cpu.ifu_insset & 3, m->cpu.ifu_opcode,
            m->cpu.ifu_alpha, m->cpu.ifu_beta, m->cpu.ifu_length,
            m->cpu.ifu_n, m->cpu.ifu_pcx, m->cpu.ifu_pcf,
            m->cpu.ifu_idcnt,
            (unsigned long long)m->cpu.ifu_dispatch_count);
    fprintf(stderr,
            "[machine] tasking: ready=%04x wake=%04x pipe={%04x,%04x} "
            "tpc[eot]=0o%o tpc[eit]=0o%o\n",
            m->cpu.ready, m->cpu.wakeup_pending,
            m->cpu.wakeup_pipe[0], m->cpu.wakeup_pipe[1],
            dorado_cpu_get_task_tpc(&m->cpu, DORADO_ETHERNET_TASK_EOT),
            dorado_cpu_get_task_tpc(&m->cpu, DORADO_ETHERNET_TASK_EIT));
    for (int i = 0; i < 8; i++)
        fprintf(stderr, " %06o", m->cpu.STK[i] & 0177777);
    fprintf(stderr, "\n");
    fprintf(stderr,
            "[machine] memref: last=%s va=%07o b=%06o task=%o.%o "
            "miss=%u latency=%d mar=%07o fault=%s count=%u first_srn=%o "
            "fault_va=%07o fault_pc=%06o fault_real=0o%o fault_task=%o.%o "
            "fault_mb=%02o fault_tioa=%03o\n",
            machine_ref_kind_name(m->mem.last_ref_kind),
            m->mem.last_ref_va & 01777777777u, m->mem.last_ref_b,
            m->mem.last_ref_task & 017, m->mem.last_ref_subtask & 3,
            m->mem.last_ref_miss, m->mem.last_ref_latency,
            m->mem.mar & 01777777777u,
            machine_fault_name(m->mem.last_fault), m->mem.fault_count,
            m->mem.fault_first_srn & 017,
            m->mem.last_fault_va & 01777777777u,
            m->mem.last_fault_pc & 0177777,
            m->mem.last_fault_real_pc & 07777,
            m->mem.last_fault_task & 017, m->mem.last_fault_subtask & 3,
            m->mem.last_fault_membase & 037,
            m->mem.last_fault_tioa & 0377);
    fprintf(stderr, "[machine] BR:");
    for (int i = 0; i < DM_BR_COUNT; i++)
        if (dorado_br_get(&m->mem, i) != 0)
            fprintf(stderr, " %02o=%07o", i, dorado_br_get(&m->mem, i));
    fprintf(stderr, "\n");
    machine_dump_lisp_atom_probe(m);
    machine_dump_env_vm_words(m);
    machine_find_env_vm_pair(m);
    machine_dump_env_storage_words(m);
    fprintf(stderr, "[machine] RM:");
    for (int i = 0; i < 040; i++)
        fprintf(stderr, " R%02o=%06o", i, m->cpu.RM[i] & 0177777);
    fprintf(stderr, "\n");
    fprintf(stderr,
            "[machine] display RM: DISPLAYCONFIG R166=%06o "
            "TVCW R160=%06o TFIELD R161=%06o TREG400C R162=%06o "
            "TDCB R152=%06o TSLC R153=%06o TERMHI R171=%06o "
            "TERMLO R172=%06o COLORCB R230=%06o\n",
            m->cpu.RM[0166] & 0177777, m->cpu.RM[0160] & 0177777,
            m->cpu.RM[0161] & 0177777, m->cpu.RM[0162] & 0177777,
            m->cpu.RM[0152] & 0177777, m->cpu.RM[0153] & 0177777,
            m->cpu.RM[0171] & 0177777, m->cpu.RM[0172] & 0177777,
            m->cpu.RM[0230] & 0177777);
    fprintf(stderr, "[machine] config_word=0o%o (B<-Config'=0o%o)\n",
            dorado_memory_config_word(&m->mem),
            (uint16_t)~dorado_memory_config_word(&m->mem));
    {
        dorado_disk_controller *dc = &m->disk;
        dorado_disk_drive *dd = &dc->drive[dc->selected_drive];
        fprintf(stderr,
                "[machine] disk: sel=%d chs=%d/%d/%d media_sec=%d "
                "ctrl=%06o en=%u act=%u pend=%u blk=%u pos=%u "
                "fifo=%d h=%d t=%d tw={idx=%u sec=%u tag=%u rd=%u wr=%u} "
                "rstream=%u/%d wstream=%u/%d under=%u over=%u "
                "counts={ctrl=%llu xfer=%llu rs=%llu rf=%llu ws=%llu "
                "fw=%llu fr=%llu secset=%llu secclear=%llu tagset=%llu "
                "tagclear=%llu}\n",
                dc->selected_drive, dd->cur_cyl, dd->cur_head,
                dd->cur_sector, dd->pack ? dd->cur_sector : 0,
                dc->control, dc->enable_run, dc->active, dc->xfer_pending,
                dc->current_block, dc->current_block_pos,
                dc->fifo_count, dc->fifo_head, dc->fifo_tail,
                dc->index_tw, dc->sector_tw, dc->tag_tw, dc->rd_fifo_tw,
                dc->wr_fifo_tw, dc->read_stream_active,
                dc->read_stream_index, dc->write_stream_active,
                dc->write_stream_index, dc->fifo_underflow,
                dc->fifo_overflow,
                (unsigned long long)dc->control_loads,
                (unsigned long long)dc->control_transfer_loads,
                (unsigned long long)dc->read_stream_starts,
                (unsigned long long)dc->read_stream_start_failures,
                (unsigned long long)dc->write_sectors_committed,
                (unsigned long long)dc->fifo_writes,
                (unsigned long long)dc->fifo_reads,
                (unsigned long long)dc->sector_tw_sets,
                (unsigned long long)dc->sector_tw_clears,
                (unsigned long long)dc->tag_tw_sets,
                (unsigned long long)dc->tag_tw_clears);
    }
    fprintf(stderr, "[machine] M[344]=0o%o (guard=%d) Swat-OutLdRet="
            "0o%o AC700=0o%o TRAPPC=0o%o\n",
            dorado_visible_word_at_va(&m->mem, mds + 0344u),
            m->mem.protect_active,
            dorado_visible_word_at_va(&m->mem, mds + 03323u),
            dorado_visible_word_at_va(&m->mem, mds + 0700u),
            /* Alto trap convention: PC+1 of the last trapping opcode
             * (Alto HW Manual: TRAPPC=527B; vectors at 530B..567B).
             * Nonzero after boot = the world took an S-group trap; a
             * FullBootBase .boot then lands in Swat/TeleSwat and idles
             * on socket 60B (see docs/running-the-emulator.md, games
             * that need Alto RAM microcode). */
            dorado_visible_word_at_va(&m->mem, mds + 0527u));
    {
        uint32_t v344 = (mds + 0344u) & 0x0FFFFFFFu;
        uint32_t idx = dorado_map_index(v344);
        const dorado_map_entry *e344 = dorado_map_get(&m->mem, idx);
        fprintf(stderr, "[machine] MDS=0o%o v344=0o%o mapidx=0o%o "
                "rp=0o%o phys=0o%o\n",
                mds, v344, idx, e344->rp,
                (unsigned)((uint32_t)e344->rp * DM_PAGE_SIZE
                           + (v344 & (DM_PAGE_SIZE - 1))));
    }
    machine_dump_lisp_display_probe(m);
    {
        uint32_t ifu_base = dorado_br_get(&m->mem, 31);
        uint32_t ifu_va =
            (ifu_base + (uint16_t)(m->cpu.ifu_pcf >> 1)) & 0x0FFFFFFFu;
        uint32_t ifu_ctx = ifu_va >= 8 ? ifu_va - 8 : 0;
        fprintf(stderr,
                "[machine] IFU stream: base=%07o pcx=%06o pcf=%06o "
                "op_va=%07o ctx=%07o\n",
                ifu_base, m->cpu.ifu_pcx, m->cpu.ifu_pcf, ifu_va, ifu_ctx);
        machine_dump_words_at_va(&m->mem, "IFU stream context", ifu_ctx, 24);
    }
    if (m->germ_word_count)
        machine_dump_pilot_pda(m);
    /* Full Alto 64K dump, one "MD %06o %06o" line per word (same format
     * as cpu.c's DORADO_MEMDUMP_AT, which only arms on the disk-boot
     * loader entry). Runs whenever this debug dump does, e.g. at end of
     * run via DORADO_FINAL_DEBUG. */
    if (getenv("DORADO_FINAL_MEMDUMP")) {
        for (uint32_t va = 0; va < 0200000u; va++)
            fprintf(stderr, "MD %06o %06o\n", va,
                    dorado_visible_word_at_va(&m->mem, va) & 0177777);
    }
}

/* Flip one framebuffer pixel (XOR), for the mouse pointer. */
static void fb_xor(dorado_display *d, int x, int y)
{
    if (x < 0 || x >= DORADO_DISPLAY_W || y < 0 || y >= DORADO_DISPLAY_H)
        return;
    d->fb[y * DORADO_DISPLAY_ROW_BYTES + (x >> 3)] ^=
        (uint8_t)(1u << (7 - (x & 7)));
}

static void machine_overlay_mouse(dorado_machine *m)
{
    if (!m->mouse_present)
        return;

    /* Mouse pointer: a fixed NW-arrow XOR'd in at the host mouse
     * position, so it is visible on any background and never smears
     * (the frame is fully redrawn each time). */
    static const uint16_t arrow[16] = {
        0x8000, 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00,
        0xFF80, 0xF800, 0xD800, 0x8C00, 0x0C00, 0x0600, 0x0600, 0x0300,
    };
    for (int r = 0; r < 16; r++)
        for (int b = 0; b < 16; b++)
            if ((arrow[r] >> (15 - b)) & 1)
                fb_xor(&m->display, m->mouse_x + b, m->mouse_y + r);
}

static int machine_alto_dcb_chain_sane(dorado_memory *mem, uint32_t base,
                                       uint16_t dl)
{
    if (!mem || dl <= 1u || dl == 0xFFFFu)
        return 0;

    int y = 0;
    int saw = 0;
    for (int g = 0; g < 64 && dl > 1u && y < DORADO_DISPLAY_H; g++) {
        uint16_t c   = dorado_visible_word_at_va(mem, base + dl + 1u);
        uint16_t sa  = dorado_visible_word_at_va(mem, base + dl + 2u);
        uint16_t slc = dorado_visible_word_at_va(mem, base + dl + 3u);
        int htab  = (c >> 8) & 077;
        int nwrds = c & 0377;
        int lines = (int)slc * 2;

        if (slc == 0 || lines <= 0 || lines > DORADO_DISPLAY_H ||
            nwrds > 64 || htab > 077 || (nwrds > 0 && sa <= 1u))
            return 0;
        saw = 1;
        y += lines;
        dl = dorado_visible_word_at_va(mem, base + dl);
    }
    return saw;
}

static int machine_alto_display_active(dorado_memory *mem)
{
    if (!mem) return 0;
    const struct {
        uint32_t base;
        uint32_t head;
    } candidates[] = {
        { dorado_br_get(mem, 036), 0420u }, /* MDS: normal Alto worlds. */
        { dorado_br_get(mem, 031), 0420u }, /* IOBR: Lisp/uCode handoff. */
        { dorado_br_get(mem, 031), 0421u }, /* Seen in Fugue sysout state. */
        { 0,                      0420u },
    };

    for (size_t i = 0; i < sizeof candidates / sizeof candidates[0]; i++) {
        uint16_t dl = dorado_visible_word_at_va(
            mem, candidates[i].base + candidates[i].head);
        if (machine_alto_dcb_chain_sane(mem, candidates[i].base, dl))
            return 1;
    }
    return 0;
}

static int machine_ddc_display_active(dorado_machine *m)
{
    if (!m || !m->ether_loaded_world_cycle) return 0;

    /* The native Cedar/Pilot path does not use the Alto/Interlisp DDC
     * scanline driver.  Initial leaves real terminal-task output history in
     * the display model when LoadRam installs CedarDorado; treating that
     * stale history as a live DDC stream wakes task 3 at its newly loaded,
     * uninitialised TPC.  It can then enter the shared BootTransfer wait loop
     * and starve task 0 before the germ IOCB shim sees PC 07012.  Cedar's
     * native display is driven by its DCB chain plus machine_cedar_io(), so
     * keep the DDC wake predicate specific to the non-germ worlds that use
     * it (notably Interlisp-D). */
    if (m->germ_word_count) return 0;

    dorado_display *d = &m->display;

    if (d->statics & DORADO_DISPLAY_STATICS_DHT_SHUTUP)
        return 0;
    if (d->terminal_task == DORADO_DISPLAY_TASK_DHT ||
        d->terminal_task == DORADO_DISPLAY_TASK_AHT) {
        if (d->output_task_count[d->terminal_task] != 0 ||
            d->next_wcb_flag[0] || d->next_wcb_flag[1] ||
            d->current_wcb_flag[0] || d->current_wcb_flag[1])
            return 1;
    }
    if (d->raster_lt_enabled)
        return 1;
    return 0;
}

static int machine_prefer_live_ddc_frame(dorado_machine *m,
                                         int ddc_pixels, int dcb_pixels)
{
    if (!m || ddc_pixels <= 0)
        return 0;

    dorado_display *d = &m->display;

    /* Interlisp-D's DoradoLisp world selects the terminal-interface
     * AHT/AWT path when a DispM board is reported present.  That path
     * produces the live picture through fast-I/O/DDC, while the old Alto
     * DASTART chain can remain as a stale boot banner.  For screenshots,
     * prefer the live DDC framebuffer once AWT/DWT has delivered a real
     * pixel stream instead of repainting the stale DCB over it. */
    int awt_stream = d->terminal_task == DORADO_DISPLAY_TASK_AHT &&
                     d->output_task_count[DORADO_DISPLAY_TASK_AWT] != 0 &&
                     d->iofetch_count != 0;
    int dwt_stream = d->terminal_task == DORADO_DISPLAY_TASK_DHT &&
                     d->output_task_count[DORADO_DISPLAY_TASK_DWT] != 0 &&
                     d->iofetch_count != 0;
    if (!awt_stream && !dwt_stream)
        return 0;

    return dcb_pixels == 0 || ddc_pixels > dcb_pixels * 4;
}

static int machine_display_fifo_used(const dorado_display *d, int subtask)
{
    int head, tail, cap;

    if (!d) return 0;
    cap = (int)(sizeof d->fifo_a / sizeof d->fifo_a[0]);
    if (subtask == 0) {
        head = d->fifo_a_head;
        tail = d->fifo_a_tail;
    } else {
        head = d->fifo_b_head;
        tail = d->fifo_b_tail;
    }
    return (head - tail + cap) % cap;
}

static int machine_display_fb_pixels(const dorado_display *d)
{
    int pixels = 0;

    if (!d) return 0;
    for (size_t i = 0; i < sizeof d->fb; i++) {
        uint8_t v = d->fb[i];
        while (v) {
            pixels += v & 1u;
            v >>= 1;
        }
    }
    return pixels;
}

static void machine_dump_words_at_va(dorado_memory *mem, const char *label,
                                     uint32_t va, int n)
{
    fprintf(stderr, "[machine] %s VA=0x%05x/0o%o:", label, va, va);
    for (int i = 0; i < n; i++) {
        fprintf(stderr, " %06o",
                dorado_visible_word_at_va(mem, va + (uint32_t)i));
    }
    fprintf(stderr, "\n");
}

static void machine_dump_alto_dcb_candidate(dorado_memory *mem,
                                            const char *name,
                                            uint32_t base,
                                            uint16_t dcb)
{
    if (dcb == 0) {
        fprintf(stderr, "[machine] Alto DCB %s head=nil\n", name);
        return;
    }

    uint32_t va = base + dcb;
    uint16_t next = dorado_visible_word_at_va(mem, va + 0u);
    uint16_t ctrl = dorado_visible_word_at_va(mem, va + 1u);
    uint16_t bitmap = dorado_visible_word_at_va(mem, va + 2u);
    uint16_t slc = dorado_visible_word_at_va(mem, va + 3u);
    int htab = (ctrl >> 8) & 0377;
    int nwords = ctrl & 0377;
    int inv = (ctrl >> 15) & 1;

    fprintf(stderr,
            "[machine] Alto DCB %s dcb=%06o va=%07o next=%06o "
            "ctrl=%06o htab=%d nwords=%d inv=%d bitmap=%06o "
            "slc=%06o words:",
            name, dcb, va, next, ctrl, htab, nwords, inv, bitmap, slc);
    for (int i = 0; i < 8; i++)
        fprintf(stderr, " %06o", dorado_visible_word_at_va(mem, va + (uint32_t)i));
    if (bitmap != 0) {
        fprintf(stderr, " sample:");
        for (int i = 0; i < 8; i++)
            fprintf(stderr, " %06o",
                    dorado_visible_word_at_va(mem, base + bitmap + (uint32_t)i));
    }
    fprintf(stderr, "\n");
}

static void machine_dump_color_channel_block(dorado_memory *mem,
                                             const char *prefix,
                                             uint32_t base,
                                             uint16_t ptr)
{
    if (ptr <= 1u) {
        fprintf(stderr, "[machine] %s channel=nil\n", prefix);
        return;
    }

    uint32_t va = base + ptr;
    uint16_t next = dorado_visible_word_at_va(mem, va + 0u);
    uint16_t nwords = dorado_visible_word_at_va(mem, va + 1u);
    uint16_t bitmap_lo = dorado_visible_word_at_va(mem, va + 2u);
    uint16_t bitmap_hi = dorado_visible_word_at_va(mem, va + 3u);
    uint16_t scanlines = dorado_visible_word_at_va(mem, va + 4u);
    uint16_t pixels = dorado_visible_word_at_va(mem, va + 5u);
    uint16_t lmarg = dorado_visible_word_at_va(mem, va + 6u);
    uint16_t scan = dorado_visible_word_at_va(mem, va + 7u);
    uint32_t bitmap = ((uint32_t)bitmap_hi << 16) | bitmap_lo;

    fprintf(stderr,
            "[machine] %s channel ptr=%06o va=%07o next=%06o nwords=%u "
            "bitmap=%07o scanlines=%u pixels=%u lmarg=%u scan=%06o "
            "sample:",
            prefix, ptr, va, next, nwords, bitmap, scanlines, pixels,
            lmarg, scan);
    if (bitmap != 0) {
        for (int i = 0; i < 8; i++)
            fprintf(stderr, " %06o",
                    dorado_visible_word_at_va(mem, bitmap + (uint32_t)i));
    }
    fprintf(stderr, "\n");
}

static void machine_dump_color_mcb_candidate(dorado_memory *mem,
                                             const char *name,
                                             uint32_t base,
                                             uint32_t ptr_word_off)
{
    uint32_t ptr_word_va = base + ptr_word_off;
    uint16_t mcb = dorado_visible_word_at_va(mem, ptr_word_va);

    fprintf(stderr, "[machine] ColorDisplay %s ptr_word=%07o -> %06o",
            name, ptr_word_va, mcb);
    if (mcb <= 1u) {
        fprintf(stderr, "\n");
        return;
    }

    uint32_t mcb_va = base + mcb;
    uint16_t seal = dorado_visible_word_at_va(mem, mcb_va + 0u);
    uint16_t flags = dorado_visible_word_at_va(mem, mcb_va + 1u);
    uint16_t a_chan = dorado_visible_word_at_va(mem, mcb_va + 2u);
    uint16_t b_chan = dorado_visible_word_at_va(mem, mcb_va + 3u);
    uint16_t color = dorado_visible_word_at_va(mem, mcb_va + 4u);

    fprintf(stderr,
            " mcb_va=%07o seal=%06o flags=%06o a=%06o b=%06o color=%06o\n",
            mcb_va, seal, flags, a_chan, b_chan, color);
    if (seal == 0177456u) {
        machine_dump_color_channel_block(mem, name, base, a_chan);
        machine_dump_color_channel_block(mem, name, base, b_chan);
        if (color > 1u)
            machine_dump_words_at_va(mem, "ColorDisplay color control",
                                     base + color, 20);
    }
}

static void machine_dump_lisp_display_probe(dorado_machine *m)
{
    if (!m) return;

    dorado_display *d = &m->display;
    dorado_fastio_router *f = &m->fastio;
    fprintf(stderr,
            "[machine] display: terminal_task=%o statics=%06o outputs=%llu "
            "iofetch=%llu fifoA=%d fifoB=%d next={%u,%u} cur={%u,%u} "
            "rast_next={%u,%u,%u,%u} rast_cur={%u,%u,%u,%u} "
            "scan=%llu twake=%llu "
            "dwake=%llu nlcb=%llu cursor_rows=%llu "
            "term_bits=%llu term_msgs=%llu term_next=%u.%u\n",
            d->terminal_task, d->statics,
            (unsigned long long)d->output_count,
            (unsigned long long)d->iofetch_count,
            machine_display_fifo_used(d, 0),
            machine_display_fifo_used(d, 2),
            d->next_wcb_flag[0], d->next_wcb_flag[1],
            d->current_wcb_flag[0], d->current_wcb_flag[1],
            d->raster_next_wt_flag[0], d->raster_next_wt_flag[1],
            d->raster_next_wt_flag[2], d->raster_next_wt_flag[3],
            d->raster_current_wt_flag[0], d->raster_current_wt_flag[1],
            d->raster_current_wt_flag[2], d->raster_current_wt_flag[3],
            (unsigned long long)d->scanline_ticks,
            (unsigned long long)d->terminal_wakeups,
            (unsigned long long)d->dwt_wakeups,
            (unsigned long long)d->nlcb_writes,
            (unsigned long long)d->cursor_rows_drawn,
            (unsigned long long)d->terminal_bits,
            (unsigned long long)d->terminal_messages,
            d->terminal_msg_word, d->terminal_msg_bit);
    fprintf(stderr,
            "[machine] display outputs by task: t0=%llu t1=%llu t3=%llu "
            "t4=%llu t11=%llu t13=%llu | fastio drops: disp_full=%u "
            "unr_fetch=%u unr_store=%u disk_full=%u disk_empty=%u\n",
            (unsigned long long)d->output_task_count[0],
            (unsigned long long)d->output_task_count[1],
            (unsigned long long)d->output_task_count[3],
            (unsigned long long)d->output_task_count[4],
            (unsigned long long)d->output_task_count[011],
            (unsigned long long)d->output_task_count[013],
            f->drops_display_fifo_full,
            f->drops_unrouted_iofetch,
            f->drops_unrouted_iostore,
            f->drops_disk_fifo_full,
            f->drops_disk_fifo_empty);
    fprintf(stderr, "[machine] display top TIOA:");
    int printed_tioa[12];
    int printed_n = 0;
    for (int rank = 0; rank < 12; rank++) {
        int best = -1;
        for (int tioa = 0; tioa < 256; tioa++) {
            if (d->output_tioa_count[tioa] == 0) continue;
            int already = 0;
            for (int prev = 0; prev < printed_n; prev++)
                if (printed_tioa[prev] == tioa) already = 1;
            if (already) continue;
            if (best < 0 ||
                d->output_tioa_count[tioa] > d->output_tioa_count[best]) {
                best = tioa;
            }
        }
        if (best < 0) break;
        printed_tioa[printed_n++] = best;
        fprintf(stderr, " %03o=%llu(first=%06o,last=%06o)", best,
                (unsigned long long)d->output_tioa_count[best],
                d->output_tioa_first[best], d->output_tioa_last[best]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "[machine] display NLCB A:");
    for (int i = 0; i < DORADO_DISPLAY_NLCB_WORDS; i++)
        fprintf(stderr, " %02o=%04o", i, d->nlcb[0][i]);
    fprintf(stderr, "\n[machine] display NLCB B:");
    for (int i = 0; i < DORADO_DISPLAY_NLCB_WORDS; i++)
        fprintf(stderr, " %02o=%04o", i, d->nlcb[1][i]);
    fprintf(stderr,
            "\n[machine] display line state: nlcb_line=%u odd=%u "
            "cursor_x=%04o cursor_lo=%03o seq_y=%u va_words=%llu "
            "ddc_lines=%llu ddc_pixels=%llu zero_lines=%llu short_lines=%llu "
            "last={line=%u width=%04o ptr=%04o words=%u nz=%u ovf=%u}\n",
            d->nlcb_line, d->nlcb_field_odd, d->nlcb_cursor_x,
            d->nlcb_cursor_lo, d->ddc_seq_y,
            (unsigned long long)d->ddc_va_words_drawn,
            (unsigned long long)d->ddc_lines_rendered,
            (unsigned long long)d->ddc_pixels_rendered,
            (unsigned long long)d->ddc_zero_word_lines,
            (unsigned long long)d->ddc_short_lines,
            d->ddc_last_line, d->ddc_last_width, d->ddc_last_ptr,
            d->ddc_last_word_count, d->ddc_last_nonzero_words,
            d->ddc_last_overflow);
    if (d->iofetch_count) {
        fprintf(stderr,
                "[machine] display first IOFetch VA=0x%05x/0o%o words:",
                d->first_iofetch_va, d->first_iofetch_va);
        for (int i = 0; i < 16; i++)
            fprintf(stderr, " %06o", d->first_iofetch_words[i]);
        fprintf(stderr,
                "\n[machine] display last IOFetch VA=0x%05x/0o%o words:",
                d->last_iofetch_va, d->last_iofetch_va);
        for (int i = 0; i < 16; i++)
            fprintf(stderr, " %06o", d->last_iofetch_words[i]);
        fprintf(stderr, "\n");
        {
            uint32_t first_idx = dorado_map_index(d->first_iofetch_va);
            uint32_t last_idx = dorado_map_index(d->last_iofetch_va);
            const dorado_map_entry *first = dorado_map_get(&m->mem, first_idx);
            const dorado_map_entry *last = dorado_map_get(&m->mem, last_idx);
            uint32_t first_phys =
                (uint32_t)first->rp * DM_PAGE_SIZE +
                (d->first_iofetch_va & (DM_PAGE_SIZE - 1));
            uint32_t last_phys =
                (uint32_t)last->rp * DM_PAGE_SIZE +
                (d->last_iofetch_va & (DM_PAGE_SIZE - 1));
            fprintf(stderr,
                    "[machine] display IOFetch map: first idx=%04o "
                    "rp=%04o phys=%07o flags{wp=%u dirty=%u ref=%u} "
                    "last idx=%04o rp=%04o phys=%07o "
                    "flags{wp=%u dirty=%u ref=%u}\n",
                    first_idx, first->rp, first_phys, first->wp,
                    first->dirty, first->ref, last_idx, last->rp,
                    last_phys, last->wp, last->dirty, last->ref);
        }
    }

    /* Fugue/Dandelion display sources name IOPageHigh=1, IOPage=0x40,
     * DisplayCSBOffset=0xE8 and DCSB=0xE9.  Probe both the long virtual
     * address and the low-half address while we determine how the loaded
     * Lisp microcode maps the Dandelion CSB on Dorado. */
    machine_dump_words_at_va(&m->mem, "Fugue DCSB long", 0x140E8u, 12);
    machine_dump_words_at_va(&m->mem, "Fugue DCSB low",  0x040E8u, 12);
    machine_dump_words_at_va(&m->mem, "Fugue DCSB IOBR",
                             dorado_br_get(&m->mem, 031) + 0x40E8u, 12);
    machine_dump_words_at_va(&m->mem, "Fugue keybits", 0x1403Au, 7);
    {
        uint32_t mds = dorado_br_get(&m->mem, 036);
        uint16_t dastart = dorado_visible_word_at_va(&m->mem, mds + 0420u);
        machine_dump_alto_dcb_candidate(&m->mem, "MDS+DAStart", mds, dastart);
        machine_dump_alto_dcb_candidate(&m->mem, "abs+DAStart", 0, dastart);
    }

    /* ColorDisplay.mc defines pMonitorCtrlBlk through an IfE that resolves
     * differently across builds.  In the Alto-mode DoradoLisp build the DHT
     * code fetches Reg400C|low(pMonitorCtrlBlk), so probe 0400 as well as the
     * PrincOps-style 0414/177414 candidates. */
    machine_dump_color_mcb_candidate(&m->mem, "abs+0400", 0, 0400u);
    machine_dump_color_mcb_candidate(&m->mem, "IOBR+0400",
                                     dorado_br_get(&m->mem, 031), 0400u);
    machine_dump_color_mcb_candidate(&m->mem, "MDS+0400",
                                     dorado_br_get(&m->mem, 036), 0400u);
    machine_dump_color_mcb_candidate(&m->mem, "abs+0414", 0, 0414u);
    machine_dump_color_mcb_candidate(&m->mem, "IOBR+0414",
                                     dorado_br_get(&m->mem, 031), 0414u);
    machine_dump_color_mcb_candidate(&m->mem, "MDS+0414",
                                     dorado_br_get(&m->mem, 036), 0414u);
    machine_dump_color_mcb_candidate(&m->mem, "abs+177414", 0, 0177414u);
}

int dorado_machine_render_display_list(dorado_machine *m)
{
    if (!m) return 0;
    dorado_memory *mem = &m->mem;
    dorado_display *disp = &m->display;
    /* Static, not stack: a framebuffer-sized (101 KB) local overflows the
     * 64 KB Emscripten wasm stack and silently corrupts static data (the
     * symptom was getenv() going NULL mid-run in the node/web builds). */
    static uint8_t ddc_fb[sizeof disp->fb];
    int ddc_pixels_before = machine_display_fb_pixels(disp);
    memcpy(ddc_fb, disp->fb, sizeof ddc_fb);

    /* The rasterizer owns the whole frame: clear to white (0 = white,
     * 1 = black), then paint everything. This avoids the smearing that
     * came from never erasing the previous frame. */
    memset(disp->fb, 0, sizeof disp->fb);

    /* Cedar/Pilot TerminalHeadDorado drives the black-and-white display
     * through an absolute low-core CSB at LONG[420B].  CSB.word0 is the
     * first DCB pointer; a DCB is:
     *   w0 next, w1 resolution/background/indent/width-words,
     *   w2 shortBitmap seal 177423B for long bitmap,
     *   w3 tag+height-in-double-scan-lines, w4/w5 long bitmap.
     * See chm/cedar/os-src/TerminalHeadDorado.mesa. */
    {
        uint16_t dcb = dorado_visible_word_at_va(mem, 0420u);
        int dcb_trace = dorado_trace_flag("DORADO_DCB_TRACE");
        int pixels = 0, y = 0, ndcb = 0;
        uint32_t bmhash = 2166136261u;
        int rendered_cedar = 0;

        for (int g = 0; g < 64 && dcb > 1u && y < DORADO_DISPLAY_H; g++) {
            uint16_t packed =
                dorado_visible_word_at_va(mem, (uint32_t)dcb + 1u);
            uint16_t seal =
                dorado_visible_word_at_va(mem, (uint32_t)dcb + 2u);
            uint16_t tag_height =
                dorado_visible_word_at_va(mem, (uint32_t)dcb + 3u);
            uint16_t bitmap_lo =
                dorado_visible_word_at_va(mem, (uint32_t)dcb + 4u);
            uint16_t bitmap_hi =
                dorado_visible_word_at_va(mem, (uint32_t)dcb + 5u);
            int width_words = packed & 0377;
            int indent = (packed >> 8) & 077;
            int long_bitmap = (tag_height & 0100000u) != 0;
            int rows = (tag_height & 077777u) * 2;
            uint32_t bitmap = ((uint32_t)bitmap_hi << 16) | bitmap_lo;

            if (seal != 0177423u || !long_bitmap || width_words <= 0 ||
                rows <= 0 || bitmap == 0)
                break;
            rendered_cedar = 1;
            ndcb++;
            if (dcb_trace && g < 4)
                fprintf(stderr,
                        "[cedar-dcb] call dl=%06o packed=%06o "
                        "indent=%d width_words=%d rows=%d bitmap=%07o\n",
                        dcb, packed, indent, width_words, rows, bitmap);
            for (int row = 0; row < rows && y < DORADO_DISPLAY_H; row++, y++) {
                for (int wi = 0; wi < width_words; wi++) {
                    uint16_t bits = dorado_visible_word_at_va(
                        mem, bitmap + (uint32_t)(row * width_words + wi));
                    if (dcb_trace) bmhash = (bmhash ^ bits) * 16777619u;
                    for (int b = 0; b < 16; b++) {
                        int pix = (bits >> (15 - b)) & 1;
                        int x = (indent + wi) * 16 + b;
                        if (x < DORADO_DISPLAY_W) {
                            dorado_display_set_pixel(disp, x, y, pix);
                            if (pix) pixels++;
                        }
                    }
                }
            }
            dcb = dorado_visible_word_at_va(mem, dcb);
        }
        if (rendered_cedar) {
            if (dcb_trace)
                fprintf(stderr,
                        "[cedar-dcb] head=%06o wake=%06o ndcb=%d "
                        "pixels=%d bmhash=%08x\n",
                        dorado_visible_word_at_va(mem, 0420u),
                        dorado_visible_word_at_va(mem, 0421u),
                        ndcb, pixels, bmhash);
            /* Cedar drives the full-page "lf" monitor (1024x808). */
            disp->active_w = DORADO_DISPLAY_W;
            disp->active_h = DORADO_DISPLAY_H;
            machine_overlay_mouse(m);
            return pixels;
        }
    }

    int pixels = 0, y = 0;
    int dcb_trace = dorado_trace_flag("DORADO_DCB_TRACE");
    uint32_t bmhash = 2166136261u; /* FNV-1a */
    int ndcb = 0;
    int content_below_alto = 0; /* any pixel drawn past the Alto raster? */
    static unsigned long render_calls = 0;
    render_calls++;
    uint32_t dmds = 0, head_va = 0;
    uint16_t dl = 0;
    const struct {
        const char *name;
        uint32_t base;
        uint32_t head;
    } candidates[] = {
        { "MDS+0420",  dorado_br_get(mem, 036), 0420u },
        { "IOBR+0420", dorado_br_get(mem, 031), 0420u },
        { "IOBR+0421", dorado_br_get(mem, 031), 0421u },
        { "abs+0420",  0,                      0420u },
    };
    const char *candidate_name = "none";
    for (size_t i = 0; i < sizeof candidates / sizeof candidates[0]; i++) {
        uint16_t head = dorado_visible_word_at_va(
            mem, candidates[i].base + candidates[i].head);
        if (machine_alto_dcb_chain_sane(mem, candidates[i].base, head)) {
            dmds = candidates[i].base;
            dl = head;
            head_va = candidates[i].base + candidates[i].head;
            candidate_name = candidates[i].name;
            break;
        }
    }
    if (dl == 0) {
        if (dcb_trace) {
            fprintf(stderr, "[dcb] call=%lu no sane Alto DCB chain\n",
                    render_calls);
            for (size_t i = 0; i < sizeof candidates / sizeof candidates[0]; i++) {
                uint32_t hva = candidates[i].base + candidates[i].head;
                uint16_t head = dorado_visible_word_at_va(mem, hva);
                uint16_t next = head > 1u
                    ? dorado_visible_word_at_va(mem, candidates[i].base + head)
                    : 0;
                uint16_t c = head > 1u
                    ? dorado_visible_word_at_va(mem, candidates[i].base + head + 1u)
                    : 0;
                uint16_t sa = head > 1u
                    ? dorado_visible_word_at_va(mem, candidates[i].base + head + 2u)
                    : 0;
                uint16_t slc = head > 1u
                    ? dorado_visible_word_at_va(mem, candidates[i].base + head + 3u)
                    : 0;
                fprintf(stderr,
                        "[dcb] candidate=%s head_va=%07o head=%06o "
                        "next=%06o c=%06o sa=%06o slc=%06o\n",
                        candidates[i].name, hva, head, next, c, sa, slc);
            }
        }
        memcpy(disp->fb, ddc_fb, sizeof ddc_fb);
        pixels = machine_display_fb_pixels(disp);
        disp->active_w = DORADO_DISPLAY_W;
        disp->active_h = DORADO_DISPLAY_H;
        machine_overlay_mouse(m);
        return pixels;
    }
    /* Alto DCB (Hardware Manual; salto helloworld.asm):
     *   w0 = next DCB (0 ends)
     *   w1 = (res<<15) | (inverse<<14) | (HTAB<<8) | NWRDS
     *        HTAB indents by 16*HTAB bits; NWRDS words/scanline (even)
     *   w2 = SA  bitmap start word address (even)
     *   w3 = SLC the block defines 2*SLC scanlines (SLC per field),
     *            i.e. 2*SLC consecutive bitmap rows of NWRDS words. */
    for (int g = 0; g < 64 && dl > 1u && y < DORADO_DISPLAY_H; g++) {
        uint16_t c   = dorado_visible_word_at_va(mem, dmds + dl + 1u);
        uint16_t sa  = dorado_visible_word_at_va(mem, dmds + dl + 2u);
        uint16_t slc = dorado_visible_word_at_va(mem, dmds + dl + 3u);
        int htab  = (c >> 8) & 077;
        int nwrds = c & 0377;
        int inv   = (c >> 14) & 1;
        int lines = (int)slc * 2;
        if (slc == 0 || lines <= 0 || lines > DORADO_DISPLAY_H ||
            nwrds > 64 || htab > 077 || (nwrds > 0 && sa <= 1u))
            break;
        if (dcb_trace && g < 4)
            fprintf(stderr,
                    "[dcb] call=%lu dl=%06o c=%06o(htab=%d nwrds=%d "
                    "inv=%d res=%d) sa=%06o slc=%d lines=%d\n",
                    render_calls, dl, c, htab, nwrds, inv, (c >> 15) & 1,
                    sa, slc, lines);
        ndcb++;
        for (int row = 0; row < lines && y < DORADO_DISPLAY_H; row++, y++) {
            for (int wi = 0; wi < nwrds; wi++) {
                uint16_t bits = dorado_visible_word_at_va(
                    mem, dmds + sa + (uint32_t)(row * nwrds + wi));
                if (dcb_trace) bmhash = (bmhash ^ bits) * 16777619u;
                for (int b = 0; b < 16; b++) {
                    int pix = (bits >> (15 - b)) & 1;
                    if (inv) pix ^= 1;
                    int x = (htab + wi) * 16 + b;
                    if (x < DORADO_DISPLAY_W) {
                        dorado_display_set_pixel(disp, x, y, pix);
                        if (pix) {
                            pixels++;
                            if (y >= DORADO_DISPLAY_ALTO_H)
                                content_below_alto = 1;
                        }
                    }
                }
            }
        }
        dl = dorado_visible_word_at_va(mem, dmds + dl);
    }

    if (dcb_trace)
        fprintf(stderr,
                "[dcb] call=%lu source=%s head_va=%07o DASTART=%06o "
                "dmds=%06o ndcb=%d "
                "pixels=%d bmhash=%08x\n",
                render_calls, candidate_name, head_va,
                dorado_visible_word_at_va(mem, head_va),
                dmds, ndcb, pixels, bmhash);

    if (machine_prefer_live_ddc_frame(m, ddc_pixels_before, pixels)) {
        memcpy(disp->fb, ddc_fb, sizeof ddc_fb);
        disp->active_w = DORADO_DISPLAY_W;
        disp->active_h = DORADO_DISPLAY_H;
        machine_overlay_mouse(m);
        return ddc_pixels_before;
    }

    /* Alto-on-Dorado uses the smaller Alto raster, but some Mesa-world
     * programs (e.g. PPong) paint a title/score band below the Alto's 606
     * scanlines; present the full-height raster when content extends past it
     * so it isn't clipped. Alto games stay at 808x606 (content never below). */
    disp->active_w = DORADO_DISPLAY_ALTO_W;
    disp->active_h = content_below_alto ? DORADO_DISPLAY_H
                                        : DORADO_DISPLAY_ALTO_H;
    machine_overlay_mouse(m);
    return pixels;
}

/* ====================================================================
 * Machine snapshot / restore (cycle-accurate-timing Phase 0).
 *
 * The whole machine is one by-value struct, so the snapshot is largely
 * fwrite of each mutable sub-struct plus the heap buffers they own. The
 * only real work is on restore: a machine freshly created with the same
 * config has already allocated storage, loaded the static firmware, and
 * wired every internal pointer, so restore must overlay the snapshot's
 * POD state while *preserving* those live pointers (re-wiring would
 * otherwise point at stale snapshot-time addresses) and reconstruct the
 * ethernet heap buffers from their serialized contents.
 * ==================================================================== */

#define DORADO_SNAP_MAGIC   "DORADOSNAPSHOT\x01"  /* 15 chars + NUL = 16 */
#define DORADO_SNAP_VERSION 2u

typedef struct {
    char     magic[16];
    uint32_t version;
    uint32_t pad;
    uint64_t sz_mc, sz_cpu, sz_mem, sz_disp, sz_bb, sz_eth, sz_disk;
    uint64_t sz_fastio, sz_machine;
    uint64_t storage_words;
    int32_t  disk_read_stream_drive;
    int32_t  disk_read_stream_sector;
    int32_t  disk_write_stream_drive;
    int32_t  disk_write_stream_sector;
} dorado_snap_header;

static int snap_wr(FILE *f, const void *p, size_t n)
{
    return (n == 0 || fwrite(p, 1, n, f) == n) ? 0 : -1;
}
static int snap_rd(FILE *f, void *p, size_t n)
{
    return (n == 0 || fread(p, 1, n, f) == n) ? 0 : -1;
}

static void machine_disk_stream_ref(const dorado_machine *m,
                                    const dorado_disk_sector *ptr,
                                    int32_t *out_drive,
                                    int32_t *out_sector)
{
    *out_drive = -1;
    *out_sector = -1;
    if (!m || !ptr) return;
    for (int d = 0; d < DORADO_DISK_NUM_DRIVES; d++) {
        const dorado_disk_pack *p = m->disk.drive[d].pack;
        if (!p || !p->sectors || p->num_sectors <= 0) continue;
        if (ptr >= p->sectors && ptr < p->sectors + p->num_sectors) {
            *out_drive = d;
            *out_sector = (int32_t)(ptr - p->sectors);
            return;
        }
    }
}

static dorado_disk_sector *machine_disk_stream_ptr(dorado_machine *m,
                                                   int32_t drive,
                                                   int32_t sector)
{
    if (!m || drive < 0 || drive >= DORADO_DISK_NUM_DRIVES) return NULL;
    dorado_disk_pack *p = m->disk.drive[drive].pack;
    if (!p || !p->sectors || sector < 0 || sector >= p->num_sectors)
        return NULL;
    return &p->sectors[sector];
}

static void machine_flush_dirty_disk_packs(dorado_machine *m)
{
    if (!m) return;
    /* The wasm32 snapshot generator runs under Emscripten's NODERAWFS. Its
     * stdio layer can read the 60 MB pack but corrupts/truncates it when the
     * snapshot's pre-flush rewrites the file. The web checkpoint is paired
     * with the deterministic native pack captured at the identical cycle, so
     * the generator may explicitly suppress this host-only flush. Clear the
     * dirty flags as well so machine_destroy() does not retry it. This escape
     * hatch is never used by ordinary native snapshots. */
    if (getenv("DORADO_SNAPSHOT_NO_PACK_FLUSH")) {
        for (int s = 0; s < DORADO_DISK_NUM_DRIVES; s++) {
            dorado_disk_pack *p = &m->disk_packs[s];
            if (!m->disk_pack_loaded[s] || !p->sectors) continue;
            for (int i = 0; i < p->num_sectors; i++)
                p->sectors[i].modified = 0;
        }
        return;
    }
    for (int s = 0; s < DORADO_DISK_NUM_DRIVES; s++) {
        dorado_disk_pack *p = &m->disk_packs[s];
        if (!m->disk_pack_loaded[s] || !p->sectors || p->read_only || !p->path[0])
            continue;
        int dirty = 0;
        for (int i = 0; i < p->num_sectors; i++) {
            if (p->sectors[i].modified) { dirty = 1; break; }
        }
        if (dirty) dorado_disk_pack_save(p);
    }
}

int dorado_machine_snapshot(dorado_machine *m, const char *path)
{
    if (!m || !path) return -1;
    machine_flush_dirty_disk_packs(m);
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, "dorado: snapshot: cannot open '%s'\n", path);
        return -1;
    }

    dorado_snap_header hdr;
    memset(&hdr, 0, sizeof hdr);
    memcpy(hdr.magic, DORADO_SNAP_MAGIC, 16);
    hdr.version       = DORADO_SNAP_VERSION;
    hdr.sz_mc         = sizeof m->mc;
    hdr.sz_cpu        = sizeof m->cpu;
    hdr.sz_mem        = sizeof m->mem;
    hdr.sz_disp       = sizeof m->display;
    hdr.sz_bb         = sizeof m->bb;
    hdr.sz_eth        = sizeof m->ethernet;
    hdr.sz_disk       = sizeof m->disk;
    hdr.sz_fastio     = sizeof m->fastio;
    hdr.sz_machine    = sizeof *m;
    hdr.storage_words = m->mem.storage_words;
    machine_disk_stream_ref(m, m->disk.read_stream_sector,
                            &hdr.disk_read_stream_drive,
                            &hdr.disk_read_stream_sector);
    machine_disk_stream_ref(m, m->disk.write_stream_sector,
                            &hdr.disk_write_stream_drive,
                            &hdr.disk_write_stream_sector);

    int rc = 0;
    rc |= snap_wr(f, &hdr, sizeof hdr);

    /* Control store (incl. the LoadRam'd world), cpu, mem. */
    rc |= snap_wr(f, &m->mc, sizeof m->mc);
    rc |= snap_wr(f, &m->cpu, sizeof m->cpu);
    rc |= snap_wr(f, &m->mem, sizeof m->mem);
    rc |= snap_wr(f, m->mem.storage,
                  m->mem.storage_words * sizeof m->mem.storage[0]);

    /* Devices. Flush the live fake6502 globals into bb->cpu6502 first so
     * the BaseBoard's 6502 register state is serialized with it. */
    rc |= snap_wr(f, &m->display, sizeof m->display);
    baseboard_cpu_flush(&m->bb);
    rc |= snap_wr(f, &m->bb, sizeof m->bb);
    rc |= snap_wr(f, &m->ethernet, sizeof m->ethernet);
    rc |= snap_wr(f, &m->disk, sizeof m->disk);
    rc |= snap_wr(f, &m->fastio, sizeof m->fastio);

    /* Ethernet heap buffers (contents only; pointers are reconstructed). */
    uint8_t have_eftp = (m->ethernet.eftp_words != NULL) ? 1 : 0;
    rc |= snap_wr(f, &have_eftp, 1);
    if (have_eftp)
        rc |= snap_wr(f, m->ethernet.eftp_words,
                      m->ethernet.eftp_len * sizeof m->ethernet.eftp_words[0]);
    uint8_t have_rx = (m->ethernet.rx_words && m->ethernet.rx_attention &&
                       m->ethernet.rx_count > 0) ? 1 : 0;
    rc |= snap_wr(f, &have_rx, 1);
    if (have_rx) {
        rc |= snap_wr(f, m->ethernet.rx_words,
                      m->ethernet.rx_count * sizeof m->ethernet.rx_words[0]);
        rc |= snap_wr(f, m->ethernet.rx_attention,
                      m->ethernet.rx_count * sizeof m->ethernet.rx_attention[0]);
    }

    /* The machine's contiguous POD scalar tail (boot state machine +
     * cadence counters + pchist/germ buffers). Everything from
     * disk_attached to the end of the struct is pointer-free. */
    size_t tail_off = offsetof(dorado_machine, disk_attached);
    rc |= snap_wr(f, (char *)m + tail_off, sizeof *m - tail_off);

    if (fclose(f) != 0) rc = -1;
    if (rc) fprintf(stderr, "dorado: snapshot: write error\n");
    return rc;
}

int dorado_machine_restore(dorado_machine *m, const char *path)
{
    if (!m || !path) return -1;
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "dorado: restore: cannot open '%s'\n", path);
        return -1;
    }

    dorado_snap_header hdr;
    if (snap_rd(f, &hdr, sizeof hdr)) { fclose(f); return -1; }
    if (memcmp(hdr.magic, DORADO_SNAP_MAGIC, 16) != 0 ||
        hdr.version    != DORADO_SNAP_VERSION ||
        hdr.sz_mc      != sizeof m->mc ||
        hdr.sz_cpu     != sizeof m->cpu ||
        hdr.sz_mem     != sizeof m->mem ||
        hdr.sz_disp    != sizeof m->display ||
        hdr.sz_bb      != sizeof m->bb ||
        hdr.sz_eth     != sizeof m->ethernet ||
        hdr.sz_disk    != sizeof m->disk ||
        hdr.sz_fastio  != sizeof m->fastio ||
        hdr.sz_machine != sizeof *m ||
        hdr.storage_words != m->mem.storage_words) {
        fprintf(stderr, "dorado: restore: incompatible snapshot "
                "(magic/version/struct-size/storage mismatch)\n");
        fclose(f);
        return -1;
    }

    int rc = 0;

    /* Control store — preserve the back-pointer to the mb_file. */
    {
        const mb_file *mb = m->mc.mb;
        rc |= snap_rd(f, &m->mc, sizeof m->mc);
        m->mc.mb = mb;
    }

    /* CPU — preserve the sibling/static pointers create wired. */
    {
        struct dorado_memory   *mem = m->cpu.mem;
        struct dorado_baseboard *bb = m->cpu.baseboard;
        dorado_io              *io  = m->cpu.io;
        const dorado_microcode *mc  = m->cpu.mc;
        void                  *tfp  = m->cpu.trace_fp;
        rc |= snap_rd(f, &m->cpu, sizeof m->cpu);
        m->cpu.mem       = mem;
        m->cpu.baseboard = bb;
        m->cpu.io        = io;
        m->cpu.mc        = mc;
        m->cpu.trace_fp  = tfp;
    }

    /* Memory — preserve the storage buffer + I/O callbacks, then load the
     * storage contents into the existing (same-size) allocation. */
    {
        uint16_t *storage = m->mem.storage;
        size_t    swords  = m->mem.storage_words;
        void (*fast_io_cb)(struct dorado_memory *, dorado_ref_kind, int, int,
                           uint32_t, uint16_t[16], void *) = m->mem.fast_io_cb;
        void  *fast_io_ctx = m->mem.fast_io_ctx;
        uint16_t (*dmux_cb)(uint16_t, int *, void *) = m->mem.dmux_cb;
        void  *dmux_ctx = m->mem.dmux_ctx;

        rc |= snap_rd(f, &m->mem, sizeof m->mem);
        m->mem.storage       = storage;
        m->mem.storage_words = swords;
        m->mem.fast_io_cb    = fast_io_cb;
        m->mem.fast_io_ctx   = fast_io_ctx;
        m->mem.dmux_cb       = dmux_cb;
        m->mem.dmux_ctx      = dmux_ctx;
        rc |= snap_rd(f, m->mem.storage, swords * sizeof m->mem.storage[0]);
    }

    /* Display + BaseBoard — pure POD. Push the restored 6502 state into
     * the live fake6502 globals and make this BaseBoard the owner. */
    rc |= snap_rd(f, &m->display, sizeof m->display);
    rc |= snap_rd(f, &m->bb, sizeof m->bb);
    baseboard_cpu_reload(&m->bb);

    /* Ethernet — reconstruct the heap buffers from serialized contents. */
    free(m->ethernet.eftp_words);
    free(m->ethernet.rx_words);
    free(m->ethernet.rx_attention);
    rc |= snap_rd(f, &m->ethernet, sizeof m->ethernet);
    m->ethernet.eftp_words   = NULL;
    m->ethernet.rx_words     = NULL;
    m->ethernet.rx_attention = NULL;

    /* Disk + fast I/O — preserve live media/device pointers from create(),
     * but restore the controller state around them. */
    {
        dorado_disk_pack *pack[DORADO_DISK_NUM_DRIVES];
        const dorado_pdi *pdi[DORADO_DISK_NUM_DRIVES];
        for (int d = 0; d < DORADO_DISK_NUM_DRIVES; d++) {
            pack[d] = m->disk.drive[d].pack;
            pdi[d] = m->disk.drive[d].pdi;
        }
        rc |= snap_rd(f, &m->disk, sizeof m->disk);
        for (int d = 0; d < DORADO_DISK_NUM_DRIVES; d++) {
            m->disk.drive[d].pack = pack[d];
            m->disk.drive[d].pdi = pdi[d];
        }
        m->disk.read_stream_sector =
            machine_disk_stream_ptr(m, hdr.disk_read_stream_drive,
                                    hdr.disk_read_stream_sector);
        m->disk.write_stream_sector =
            machine_disk_stream_ptr(m, hdr.disk_write_stream_drive,
                                    hdr.disk_write_stream_sector);
    }
    {
        dorado_display *display = m->fastio.display;
        dorado_disk_controller *disk_ctl = m->fastio.disk_ctl;
        rc |= snap_rd(f, &m->fastio, sizeof m->fastio);
        m->fastio.display = display;
        m->fastio.disk_ctl = disk_ctl;
    }

    uint8_t have_eftp = 0;
    rc |= snap_rd(f, &have_eftp, 1);
    if (have_eftp) {
        size_t n = m->ethernet.eftp_len;
        m->ethernet.eftp_words = malloc(n * sizeof(uint16_t) + 1);
        if (m->ethernet.eftp_words)
            rc |= snap_rd(f, m->ethernet.eftp_words, n * sizeof(uint16_t));
        else
            rc = -1;
    }
    uint8_t have_rx = 0;
    rc |= snap_rd(f, &have_rx, 1);
    if (have_rx) {
        size_t n = m->ethernet.rx_count;
        m->ethernet.rx_words     = malloc(n * sizeof(uint16_t) + 1);
        m->ethernet.rx_attention = malloc(n + 1);
        if (m->ethernet.rx_words && m->ethernet.rx_attention) {
            rc |= snap_rd(f, m->ethernet.rx_words, n * sizeof(uint16_t));
            rc |= snap_rd(f, m->ethernet.rx_attention, n);
        } else {
            rc = -1;
        }
    }

    /* The machine's POD scalar tail, straight into the live struct. */
    size_t tail_off = offsetof(dorado_machine, disk_attached);
    rc |= snap_rd(f, (char *)m + tail_off, sizeof *m - tail_off);

    fclose(f);
    if (rc) fprintf(stderr, "dorado: restore: read error\n");
    return rc;
}

static uint64_t snap_fnv1a(uint64_t h, const void *p, size_t n)
{
    const uint8_t *b = (const uint8_t *)p;
    for (size_t i = 0; i < n; i++) {
        h ^= b[i];
        h *= 0x100000001b3ULL;
    }
    return h;
}

uint64_t dorado_machine_state_digest(const dorado_machine *m)
{
    if (!m) return 0;
    uint64_t h = 0xcbf29ce484222325ULL;  /* FNV-1a offset basis */
    h = snap_fnv1a(h, m->mem.storage,
                   m->mem.storage_words * sizeof m->mem.storage[0]);
    h = snap_fnv1a(h, m->display.fb, sizeof m->display.fb);
    h = snap_fnv1a(h, m->cpu.RM, sizeof m->cpu.RM);
    h = snap_fnv1a(h, m->cpu.STK, sizeof m->cpu.STK);
    h = snap_fnv1a(h, m->cpu.task_t, sizeof m->cpu.task_t);
    h = snap_fnv1a(h, m->cpu.task_tpc, sizeof m->cpu.task_tpc);
    h = snap_fnv1a(h, m->cpu.task_link, sizeof m->cpu.task_link);

    uint16_t regs[] = {
        m->cpu.T, m->cpu.Q, m->cpu.Cnt, m->cpu.ShC, m->cpu.MemBase,
        m->cpu.real_PC, m->cpu.prev_PC, m->cpu.ctask,
        m->cpu.wakeup_pending, m->cpu.ready, m->cpu.StkP,
    };
    h = snap_fnv1a(h, regs, sizeof regs);

    uint64_t counters[] = {
        m->cpu.cycles,
        m->cpu.ifu_dispatch_count,
        m->bb.cycles,
    };
    h = snap_fnv1a(h, counters, sizeof counters);
    return h;
}
