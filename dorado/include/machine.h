#ifndef DORADO_MACHINE_H_
#define DORADO_MACHINE_H_

/*
 * dorado_machine — a runnable Dorado: BaseBoard + microengine + memory
 * + IFU + tasking + slow/fast I/O + the in-process Ethernet boot server,
 * wired together and driven by a single cycle clock. This is the
 * orchestration that was previously trapped inside test_cpu.c's
 * probe_full_boot_with_bootstrap, lifted into a reusable library so a
 * standalone frontend (src/dorado.c) can boot and present the machine.
 *
 * The default config reproduces the Stage-2 Alto-style Ethernet software
 * boot: BaseBoard -> Bootstrap -> Initial -> EtherMicrocodeBoot loads the
 * Alto/Mesa emulator world, which then Maydays for NETEXEC.BOOT and runs
 * the BCPL Net Executive.
 */

#include "display.h"
#include "dispm.h"

#include <stdint.h>
#include <stdio.h>   /* FILE, for dorado_machine_print_abi */

typedef struct dorado_machine dorado_machine;

typedef struct dorado_machine_config {
    /* Colour board fitted to this machine. AUTO preserves the historical
     * environment-variable selection for external callers; frontends should
     * set this explicitly so switching worlds cannot leak board state. */
    dorado_dispm_type dispm_type;

    /* Koto/Lisp used the older ColorDisplay.mc task/register path. -1 keeps
     * the historical DORADO_LISP_COLOR environment selection; 0 is normal
     * Cedar/Mesa wiring and 1 enables the Lisp compatibility path. */
    int lisp_color;

    /* Firmware / microcode source paths. NULL selects the built-in
     * default (the canonical chm/ tree, relative to the dorado/ dir). */
    const char *bb_rom;        /* doradobaserom.mb!13                  */
    const char *bootstrap_mb;  /* Bootstrap.mb (BB-streamed loader)    */
    const char *initial_mb;    /* Initial.mb (boot microcode)          */
    const char *kernel_mb;     /* kernel.mb (fault task, dispatch)     */
    const char *memmisc_mb;    /* memMisc.mb (memory primitives)       */
    const char *ifu_mb;        /* IfuComplex.mb (IFU decode)           */
    const char *eth_boot_110;  /* netboot world for boot-file 0110     */
    const char *eftp_boot;     /* Stage-2 Alto boot file (NETEXEC.BOOT)*/
    const char *ftp_sysout;    /* Optional Pup FTP sysout served to
                                * Interlisp-D RemoteVmemInit. */
    const char *ftp_root;      /* Optional read-only FTP/STP file tree.
                                * Cedar's LoaderDriver retrieves release
                                * modules from this tree over Pup socket 3. */

    int alto_ether_boot;       /* 1 = drive the Stage-2 Alto ether boot
                                *     (breath-of-life + EFTP server)   */
    int alto_ether_quote;      /* DDC "quote" key state for boot select */
    int no_disk;               /* 1 = leave the disk task idle         */
    int storage_modules;       /* modeled storage size, 1..4 4MW modules.
                                * 0 -> default 1. AEmu only needs one;
                                * 4 modules wraps RealPages to 0 and
                                * confuses InitMem. */
    const char *boot_dir[8];   /* Stage-2 boot-file directory entries the
                                * fake boot server advertises to NetExec,
                                * each "NAME=BFN=PATH" (BFN octal). NAME
                                * must end in ".boot". Lets NetExec boot
                                * e.g. CedarNetExec by name. */
    int      boot_dir_count;
    int      boot_dir_all;     /* 1 = also auto-register every Alto B-format
                                * *.boot file found alongside eftp_boot (the
                                * chm/bootfiles/ tree) as a directory entry,
                                * so NetExec's menu lists all the games. The
                                * --boot-dir-all flag; the standalone frontends
                                * default it on when no explicit --boot-dir is
                                * given so the demo "just works". */
    /* Boot-key chord held down through the Stage-2 boot-selection phase.
     * Models the Dorado/Alto boot-reason chord (Booting memo): the loaded
     * Alto/Mesa world polls these keys to choose its boot path (BS =
     * Ethernet software boot, BS+Quote = NetExec; no key = disk). The DDC
     * keyboard back-channel that would carry the chord is not modeled, so
     * the machine seeds the polled keyboard words directly (gap E2). Empty
     * (count 0) selects the default chord: BS, plus Quote iff
     * alto_ether_quote -- byte-identical to the historic forced seeding. */
    dorado_display_key boot_keys[8];
    int      boot_keys_count;

    /* Route B (Cedar/Mesa germ net-boot): path to the Pilot germ image
     * (default chm/cedar/germ/Dorado.germ!4). When set, the machine
     * deposits the germ into VM at its resident addresses (0o17401000+)
     * the first time the Cedar microcode reaches the disk germ-boot
     * transfer spin -- the germ-plant that bypasses the (contentless)
     * disk read. NULL leaves the plant disabled, so the Alto worlds in
     * the regression gate are structurally untouched. */
    const char *germ_path;
    /* Route B Pilot/Cedar PDI media. `--pilot-disk PATH` fills slot 0;
     * `--pilot-disk SLOT=PATH` mounts up to four physical volumes. */
    const char *pilot_disk_pdi[4];
    const char *disk_pack[4];   /* --disk SLOT=PATH: a real Trident pack image
                                 * (T-80/T-300, R/W) mounted on drive SLOT. */
    int      disk_real;        /* 1 = drive the real disk controller for the
                                * Pilot/Cedar boot: disable the IOCB-level PDI
                                * completion shim, clock the PDI media, and let
                                * the DSK task run PilotDisk.mc against the
                                * controller. Off by default (the shim path is
                                * the working Cedar boot). Plan D4. */
    int      germ_netboot;     /* 1 = after the planted germ is resident,
                                * seed GermSwap.pRequest as Ethernet inLoad
                                * instead of PilotBoot's physical-volume
                                * request. Experimental Route B shortcut. */
    uint16_t germ_netboot_bfn; /* BootChannelEther boot file number. 0 asks
                                * the fake EFTP server for its default
                                * eftp_boot path. */

    uint16_t boot_file_number; /* Mesa/Dorado boot file number, the first
                                * word of the boot-parameter block Initial
                                * normally derives from the boot button +
                                * keyboard. Drives both the netboot file
                                * Initial requests and the STK[1..3] seed
                                * the loaded world reads. 0 -> default 0110
                                * (the normal Mesa/Alto net boot). */

    const char *boot_switches;  /* Pilot boot switches to present to the
                                 * booting world, as the letters/digits a
                                 * user would type at the herald's
                                 * "Switches:" prompt (e.g. "l" to run Iago).
                                 * GermSwap.Switch is the enumeration
                                 * {zero..nine, a..z}; NULL or "" boots with
                                 * none, which is what every world did
                                 * before. See GermSwap.Mesa. */
} dorado_machine_config;

/* Fill cfg with the default Alto-on-Dorado NetExec boot configuration. */
void dorado_machine_config_default(dorado_machine_config *cfg);

/* Create and cold-start a machine. Returns NULL if firmware/microcode
 * could not be loaded (a message is printed to stderr). */
dorado_machine *dorado_machine_create(const dorado_machine_config *cfg);
void dorado_machine_destroy(dorado_machine *m);

/* Advance the clock until cycle count reaches until_cycle (or the engine
 * halts). Returns the cycle count actually reached. Call repeatedly to
 * run a frame at a time. */
uint64_t dorado_machine_run_until(dorado_machine *m, uint64_t until_cycle);

uint64_t dorado_machine_cycles(const dorado_machine *m);

/* Executed Dorado microinstructions. NOT the same as
 * dorado_machine_cycles(), which returns the BaseBoard 6502 counter
 * (~3.7x larger). Use this one for speed comparisons against the real
 * 16.67 MIPS machine and for converting cycle budgets to emulated time. */
uint64_t dorado_machine_uinstructions(const dorado_machine *m);

/* 1 once Initial has LoadRam'd the Alto/Mesa world and it is running. */
int dorado_machine_booted(const dorado_machine *m);
/* 1 when the EFTP-served image is a Mesa/Pilot outload (see machine.c). */
int dorado_machine_boot_is_mesa_outload(const dorado_machine *m);

/* The display device (for framebuffer access / snapshot). */
dorado_display *dorado_machine_display(dorado_machine *m);

/* Print a one-line Ethernet/EFTP/display state summary to stderr. */
void dorado_machine_debug(dorado_machine *m);

/* Set a host-translated key up/down (down != 0). The machine forces
 * the boot keys until the Ethernet boot selection is complete; once
 * NetExec is loading, these live key states are what the running world
 * polls. */
void dorado_machine_set_key(dorado_machine *m, dorado_display_key key,
                            int down);

/* 1 once the boot-key selection phase is over and live keyboard input
 * is delivered to the running world. */
int dorado_machine_interactive(const dorado_machine *m);

/* Reapply explicitly selected host-side FTP/STP sources after restoring a
 * snapshot.  These source paths are not part of the emulated machine state. */
/* Read one guest virtual word. Signature chosen for dorado_dispm_render's
 * callback so the colour board model needs no memory.h. */
uint16_t dorado_machine_read_visible_word(void *ctx, uint32_t va);

/* Reattach the host-side DispM device after a checkpoint restore.  DispM is
 * deliberately outside the snapshot ABI, so a machine created for a color
 * checkpoint must explicitly restore the board registration as well as the
 * guest memory image. */
void dorado_machine_ensure_dispm(dorado_machine *m, dorado_dispm_type type);

/* Mouse MOTION on the terminal back channel -- the shape the hardware sends,
 * and the only one in which the guest can cross to a second screen. */
void dorado_machine_mouse_delta(dorado_machine *m, int dx, int dy);

/* True after the colour-display frontend has switched input to terminal
 * deltas. Frontends use this to keep sending deltas while the host pointer
 * returns across the monochrome window; an absolute resync at that boundary
 * would jump the guest pointer to the host window's entry coordinate. */
int dorado_machine_mouse_delta_active(const dorado_machine *m);

/* Return Cedar Interminal's current colour-display side: 1 for right, 0 for
 * left, or -1 when the guest has not exposed a recognizable display record
 * yet.  This is a diagnostic/read-only query; the OS remains the authority. */
int dorado_machine_color_display_right(const dorado_machine *m);

void dorado_machine_set_ftp_source(dorado_machine *m, const char *sysout,
                                   const char *root);

/* Replace one Pilot PDI after snapshot restore.  PDI bytes are host media,
 * not guest state; this permits a checkpoint to be resumed against an
 * explicitly selected writable/free-space volume.  Returns zero on success. */
int dorado_machine_set_pilot_disk(dorado_machine *m, int slot,
                                  const char *path);

/* Mouse button flags (Alto convention, matches UTILIN bit values). */
#define DORADO_MOUSE_MIDDLE 0x1
#define DORADO_MOUSE_RIGHT  0x2
#define DORADO_MOUSE_LEFT   0x4

/* Set the absolute mouse position (x in 0..807, y in 0..605) and button
 * bitmask (DORADO_MOUSE_*). Written to the Alto low-core mouse cells
 * (MOUSEX 0o424 / MOUSEY 0o425) and the UTILIN button word
 * (0o177030..0o177033) once the world is interactive. */
void dorado_machine_set_mouse(dorado_machine *m, int x, int y, int buttons);

/* Update only the terminal mouse-button state. Used when a button event
 * arrives on the colour monitor: that monitor has no combined coordinate
 * space, so changing the absolute monochrome coordinate would move the
 * guest pointer to the wrong display. */
void dorado_machine_set_mouse_buttons(dorado_machine *m, int buttons);

/* Rasterize the installed display chain from memory straight into the
 * display framebuffer, independent of the DWT word task. Supports Cedar's
 * TerminalHeadDorado low-core CSB/DCB and the Alto DASTART DCB chain.
 * Returns the number of lit pixels painted. */
int dorado_machine_render_display_list(dorado_machine *m);

/*
 * Machine snapshot / restore (cycle-accurate-timing Phase 0 — the key
 * enabler in docs/cycle-accurate-timing-plan.md). Serialize the full
 * mutable runtime state of a running machine to `path`, then restore it
 * into a machine *freshly created with the same config*. This lets a
 * timing experiment resume a booted game without re-running the fragile
 * boot, so a cadence change can be measured against ContrAlto on the
 * running game in isolation.
 *
 * Captured: the control store (mc, incl. the LoadRam'd world), cpu, mem +
 * storage, display, baseboard, ethernet + heap buffers, disk controller,
 * fast-I/O router, and the machine's scalar boot/timing fields. Disk pack
 * media buffers themselves are not embedded in the snapshot; create the
 * restore machine with the same --disk paths. Dirty writable packs are flushed
 * before snapshot so a restored disk-backed world sees matching media bytes.
 *
 * Both return 0 on success, -1 on I/O or incompatibility (restore checks
 * a magic + version + struct-size + storage-size header and refuses a
 * mismatched image rather than corrupting memory).
 */
int dorado_machine_snapshot(dorado_machine *m, const char *path);
int dorado_machine_restore(dorado_machine *m, const char *path);

/*
 * Print the struct sizes this build stamps into a snapshot header, one
 * "name value" per line, in header order. Every baked checkpoint must
 * agree with these or restore refuses it, so `make verify-snapshot-abi`
 * diffs the shipped checkpoints against this instead of booting each one.
 * Per-ABI: native and wasm32 differ, and each must be checked against its
 * own build. Exposed via `dorado --print-abi`.
 */
void dorado_machine_print_abi(FILE *out);

/*
 * A 64-bit FNV-1a digest over the machine's core runtime state (main
 * storage, the display framebuffer, RM/STK, the per-task registers, and
 * the cycle/dispatch counters). Two machines with the same digest are
 * behaviorally identical to the resolution that matters for the timing
 * work; used to prove snapshot/restore is faithful and, later, to spot
 * the cycle at which a restored run diverges.
 */
uint64_t dorado_machine_state_digest(const dorado_machine *m);

/* ---- Front panel -------------------------------------------------------
 *
 * What a person standing at the machine could see, gathered in one call so a
 * frontend does not need the internals. `dorado_machine` is opaque on
 * purpose, and the alternative -- a dozen little accessors -- would grow one
 * per light.
 *
 * The activity fields are free-running COUNTERS, not booleans: a frontend
 * lights its lamp when the count moved since the last frame, which is what
 * the real indicators did. Reading them costs nothing and they never reset,
 * so a caller may sample at any rate.
 *
 * `lamp_on` is the real thing: the BaseBoard's green status LED, driven from
 * MiscByte by the 6502 exactly as on the hardware (baseboard.h). */
typedef struct dorado_machine_panel {
    int      lamp_on;          /* BaseBoard green status LED               */
    int      booted;           /* a world has been loaded and started      */
    int      mouse_buttons;    /* DORADO_MOUSE_* bits currently held       */
    uint64_t cycles;           /* BaseBoard 6502 cycles (see --cycles)     */
    uint64_t uinstructions;    /* Dorado microinstructions                 */
    uint64_t disk_activity;    /* sector reads + writes                    */
    uint64_t net_activity;     /* ethernet packets seen + queued           */
    uint64_t display_frames;   /* completed display frames                 */
} dorado_machine_panel;

void dorado_machine_get_panel(const dorado_machine *m,
                              dorado_machine_panel *out);

#endif /* DORADO_MACHINE_H_ */
