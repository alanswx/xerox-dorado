#ifndef DORADO_DISASM_H
#define DORADO_DISASM_H

#include <stddef.h>
#include <stdint.h>

/*
 * Microinstruction decoder for Dorado model 1 (DMachine == 2).
 *
 * The .MB loader hands us 4 × 16-bit "stored" words per IM entry; this
 * module unshuffles them into logical iw0/iw1/iw2 (per mddump.bcpl's
 * inverse), extracts the eight microinstruction fields, and formats
 * them symbolically.
 *
 * Field layout (from chm/microd/mdfields.d, BCPL bit ordering converted
 * to C bit ordering):
 *
 *   iw0   bits 15..13 = RSTK[2..0]    (low 3 bits of the 4-bit field)
 *         bits 12..9  = ALUF[3..0]
 *         bits 8..6   = BSEL[2..0]
 *         bits 5..3   = LC[2..0]
 *         bits 2..0   = ASEL[2..0]
 *
 *   iw1   bit  15     = BLOCK
 *         bits 14..7  = FF[7..0]
 *         bits 6..0   = JCN[6..0]     (high 7 bits of the 8-bit field)
 *
 *   iw2   bit  15     = RSTK1top      (high bit of RSTK)
 *         bit  14     = JN1bit7       (low bit of JCN)
 *         bits 13..12 = brkP1         (breakpoint / bad-parity)
 *
 * Total useful bits: 34 = 4(RSTK) + 4(ALUF) + 3(BSEL) + 3(LC) + 3(ASEL)
 *                       + 1(BLOCK) + 8(FF) + 8(JCN), per HM Table 1.
 */

typedef struct {
    /* Reconstructed logical words (after model-1 unshuffle). */
    uint16_t iw0, iw1, iw2;
    /* awd: storage word 3 — control bits (breakpoint, emulator). */
    uint16_t awd;

    /* Decoded microinstruction fields. */
    uint8_t  rstk;   /* 4 bits */
    uint8_t  aluf;   /* 4 bits */
    uint8_t  bsel;   /* 3 bits */
    uint8_t  lc;     /* 3 bits */
    uint8_t  asel;   /* 3 bits */
    uint8_t  block;  /* 1 bit  */
    uint8_t  ff;     /* 8 bits */
    uint8_t  jcn;    /* 8 bits */
    uint8_t  brk_p;  /* 2 bits — breakpoint / bad-parity flags */
} dorado_uinstr;

/*
 * Unshuffle 4 stored words into logical iw0/iw1/iw2/awd and decode
 * fields. `stored` must point at 4 contiguous uint16_t.
 */
void dorado_decode_model1(const uint16_t stored[4], dorado_uinstr *out);

/*
 * Re-derive the decoded fields from iw0/iw1/iw2 after they've been
 * mutated in place (e.g., by a Write IM operation). The awd /
 * brk_p / iw0..2 values are not touched.
 */
void dorado_redecode_fields(dorado_uinstr *u);

/*
 * Decode the 5-byte microinstruction format the BaseBoard uses to
 * inject MIR via DoDoradoMicroInst (doradocpint.masm). Bytes are:
 *   mir[0] = ExtraInstBits: bit 7 = RSTK[0], bit 5 = JCN[7], bits 6/4 = parity
 *   mir[1] = MIR0: RSTK[1..3], ALUF[0], BLOCK, FF[0..2]
 *   mir[2] = MIR1: ALUF[1..3], BSEL[0], FF[3..6]
 *   mir[3] = MIR2: BSEL[1..2], LC[0..1], FF[7], JCN[0..2]
 *   mir[4] = MIR3: LC[2], ASEL[0..2], JCN[3..6]
 *
 * The output's iw0/iw1/iw2 are constructed from the decoded fields
 * so subsequent Write IM bookkeeping stays consistent.
 */
void dorado_decode_mir(const uint8_t mir[5], dorado_uinstr *out);

/*
 * Format a decoded microinstruction into a buffer in a single line, no
 * trailing newline. Returns the number of bytes written (excluding NUL).
 *
 * Output style: "RSTK=0o12 ALUF=A+1 BSEL=Md LC=T ASEL=A+1 FF=040 JCN=L177 BLOCK"
 * Field values use the symbolic names from HM Tables 7–13 where known;
 * fall back to "0oNNN" for unrecognized codes.
 */
size_t dorado_format(const dorado_uinstr *u, char *buf, size_t buflen);

/* Field-by-field symbolic name lookup. Returns NULL if no symbolic
 * form is defined; caller can fall back to printing the raw value. */
const char *dorado_bsel_name(uint8_t bsel);
const char *dorado_asel_name(uint8_t asel);
const char *dorado_lc_name(uint8_t lc);
const char *dorado_jcn_class(uint8_t jcn);   /* "local", "long", etc. */

#endif
