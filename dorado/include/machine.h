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

#include <stdint.h>

typedef struct dorado_machine dorado_machine;

typedef struct dorado_machine_config {
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

    uint16_t boot_file_number; /* Mesa/Dorado boot file number, the first
                                * word of the boot-parameter block Initial
                                * normally derives from the boot button +
                                * keyboard. Drives both the netboot file
                                * Initial requests and the STK[1..3] seed
                                * the loaded world reads. 0 -> default 0110
                                * (the normal Mesa/Alto net boot). */
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

/* 1 once Initial has LoadRam'd the Alto/Mesa world and it is running. */
int dorado_machine_booted(const dorado_machine *m);

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

/* Mouse button flags (Alto convention, matches UTILIN bit values). */
#define DORADO_MOUSE_MIDDLE 0x1
#define DORADO_MOUSE_RIGHT  0x2
#define DORADO_MOUSE_LEFT   0x4

/* Set the absolute mouse position (x in 0..807, y in 0..605) and button
 * bitmask (DORADO_MOUSE_*). Written to the Alto low-core mouse cells
 * (MOUSEX 0o424 / MOUSEY 0o425) and the UTILIN button word
 * (0o177030..0o177033) once the world is interactive. */
void dorado_machine_set_mouse(dorado_machine *m, int x, int y, int buttons);

/* Rasterize the Alto display list (DASTART -> DCB chain) from memory
 * straight into the display framebuffer, independent of the DWT word
 * task. Returns the number of lit pixels painted. */
int dorado_machine_render_display_list(dorado_machine *m);

#endif /* DORADO_MACHINE_H_ */
