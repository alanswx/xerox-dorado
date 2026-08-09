#ifndef DORADO_DISPM_H_
#define DORADO_DISPM_H_

#include <stdint.h>

#include "io.h"

/*
 * DispM — the Dorado's COLOUR display board (HM §11; the "DDM", display
 * mixer). DispY is the monochrome board: one AltoTTLVideo line, the cursor,
 * the MiniMixer, the OIS terminal link. DispM is sheets 12-31 of its own
 * drawing set: ABuf/BBuf/CBuf, the BMap and CMap, and three video DACs.
 *
 * Everything here is transcribed from Xerox's own driver, which survives in
 * the Cedar 6.1 release we serve to the guest:
 *
 *   chm/cedar/stp-root/Cedar6.1/HeadsDorado/ColorDisplayDorado.mesa
 *       (Doug Wyatt, 1-May-1985)  — register numbers, control-block layout
 *   chm/cedar/stp-root/Cedar6.1/HeadsDorado/ColorDisplayHeadDorado.mesa
 *       (Atkinson/Wyatt, 1985-86) — the presence test and the load sequence
 *
 * and cross-checked against the board's own backplane netlist
 * (chm/sil/DispM-Rev-Ch.dm!1_, see docs/sil-netlist-crosscheck.md): DACRed /
 * DACGreen / DACBlue with GNDRed / GNDGreen / GNDBlue and RefIn, MType'[0-3],
 * A8B2, 24BitMode. Three independent sources, one design.
 *
 * WHY THIS IS ITS OWN FILE. It is a separate board, addressed by a separate
 * task: display.c claims TIOA 0360..0377 for the DISPLAY TASKS (3, 4, 011,
 * 013), while ColorDisplayHeadDorado is Mesa code issuing Output/Input from
 * the EMULATOR task. Same addresses, different task, different board — which
 * is exactly how the hardware disambiguates them, and why registering this
 * device on task 0 does not collide with DispY.
 *
 * NO dorado_machine MEMBER. State is file-scope in dispm.c and reached
 * through these functions: adding a struct member changes the snapshot ABI
 * and every baked checkpoint fails to restore.
 */

/* ColorDisplayHeadDorado.mesa SetDisplayType. The type code is what Input at
 * TIOA 361B must report in its TOP FOUR BITS for the head to accept it. */
typedef enum {
    DORADO_DISPM_AUTO = -1,        /* machine-config selection sentinel */
    DORADO_DISPM_NONE = 0,
    DORADO_DISPM_STANDARD,        /*  640 x 480,  type code 17B */
    DORADO_DISPM_HIGHRES          /* 1024 x 768,  type code 16B */
} dorado_dispm_type;

#define DORADO_DISPM_MAX_W        1024
#define DORADO_DISPM_MAX_H         768

/* ColorDisplayDorado.mesa: AIndex ~ [0..1024), BCIndex ~ [0..256). */
#define DORADO_DISPM_MIXER_WORDS  1024
#define DORADO_DISPM_BC_ENTRIES    256

/* Slow-I/O addresses. mixerRegister/bMapRegister/cMapRegister are named
 * constants in ColorDisplayDorado.mesa; colorBoardAddr is from the head's
 * presence test. Note 361B is BOTH the mixer register (on Output) and the
 * display-type report (on Input) — the hardware reads and writes different
 * things at one address, which is why the head uses InputNoPE there. */
#define DORADO_DISPM_TIOA_BOARD   0360    /* Input: non-zero = board present */
#define DORADO_DISPM_TIOA_MIXER   0361    /* Output: mixer; Input: type */
#define DORADO_DISPM_TIOA_CMAP    0362
#define DORADO_DISPM_TIOA_BMAP    0365

/* The muffler address the head reads for the board revision.
 * "TRUE if color board has an old revision level (less than Cj)" —
 * oldRev is set iff dMuxData = 1. */
#define DORADO_DISPM_DMUX_REV     03107

/* ColorDisplayDorado.mesa: csb ~ LOOPHOLE[177414B], relative to base 0. */
#define DORADO_DISPM_CSB_VA       0177414u
#define DORADO_DISPM_MCB_SEAL     0177456u
#define DORADO_DISPM_PPL_OFFSET   0400u    /* pixelsPerLineOffset */

/* Reset all state. Called from the machine's create path. */
void dorado_dispm_reset(void);

/* Install the board, or remove it. With no board installed, Input at 0360
 * reads back zero and the head leaves displayType at `none`, which is a
 * Dorado with only a 7-wire terminal — the common configuration, and what
 * every checkpoint we ship was baked as. */
void dorado_dispm_install(dorado_dispm_type type);
dorado_dispm_type dorado_dispm_installed(void);

/* Register the board on the EMULATOR task at its four addresses. */
void dorado_dispm_attach_to_io(dorado_io *io);

/* Walk the ColorCSB chain in guest memory and paint the A channel into an
 * internal 24-bit RGB buffer. `read_word` supplies a guest virtual word (the
 * caller passes its own memory accessor so this file needs no memory.h).
 * Returns the number of pixels painted, or 0 if the chain is not set up. */
int dorado_dispm_render(uint16_t (*read_word)(void *ctx, uint32_t va),
                        void *ctx);

/* The rendered image. NULL until dorado_dispm_render has painted something. */
const uint8_t *dorado_dispm_rgb(int *out_w, int *out_h);

/* Write the rendered image as a binary PPM (P6). 0 on success. */
int dorado_dispm_snapshot_ppm(const char *path);

/* One-line census of what the guest has done to the board: reads of the
 * presence registers, writes to each RAM, and whether the CSB is armed. */
void dorado_dispm_dump(void);

#endif /* DORADO_DISPM_H_ */
