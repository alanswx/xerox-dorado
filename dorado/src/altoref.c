/*
 * altoref - spec-derived Nova/Alto opcode reference (see altoref.h).
 * Semantics from the Alto Hardware Manual (May 1979) Section 3:
 *
 *   ALC (bit 0 = 1): SrcAC(1:2) DstAC(3:4) AFunc(5:7) SH(8:9) CY(10:11)
 *     NL(12) SK(13:15).
 *   - Carry generator: cin = [C, 0, 1, ~C] for CY = [0,1,2,3].
 *   - Function generator -> Result16 + carryResult; logical ops (COM/MOV/
 *     AND) pass carry unaffected (carryResult = cin); arithmetic ops
 *     complement cin on overflow (NEG: SrcAC==0; INC: SrcAC==0xFFFF;
 *     ADC/SUB/ADD: adder carry-out).
 *   - Shifter (SH): none / L rotate-17-left / R rotate-17-right / S swap
 *     bytes (carry unaffected). Carry-in to the rotate is carryResult;
 *     carry-out becomes the post-shift carry.
 *   - Skip sensor: skip = ((carry==0 & SK1) | (result==0 & SK0)) ^ SK2,
 *     SK0/1/2 = the three SK bits (verified against the SKP..SBN table).
 *   - NL=1: neither DstAC nor carry is loaded.
 *   MRI (bit 0 = 0): MFunc(1:2) 0=Jump 1=LDA 2=STA; within Jump JFunc(3:4)
 *     0=JMP 1=JSR 2=ISZ 3=DSZ. I(5) X(6:7) DISP(8:15). EffAddr: X=0 page
 *     zero (abs 0..255), X=1 PC-rel, X=2 AC2-rel, X=3 AC3-rel, with
 *     SignExtend(disp) = disp>0o200 ? disp+0o177400 : disp; I -> one
 *     indirect level. ISZ/DSZ do not alter carry.
 */
#include "altoref.h"

static uint16_t sign_extend(uint16_t disp)
{
    return (disp > 0200u) ? (uint16_t)(disp + 0177400u) : disp;
}

alto_ref_result alto_ref_exec(uint16_t op, const uint16_t ac_in[4],
                              int cry_in, uint16_t *mem, uint16_t pc)
{
    alto_ref_result r;
    uint16_t ac[4] = { ac_in[0], ac_in[1], ac_in[2], ac_in[3] };
    int C = cry_in & 1;

    r.ac[0] = ac[0]; r.ac[1] = ac[1]; r.ac[2] = ac[2]; r.ac[3] = ac[3];
    r.cry = C; r.skip = 0; r.wrote = 0; r.wr_addr = 0; r.wr_val = 0;
    r.is_jump = 0; r.jump_target = 0; r.ok = 1;

    if (op & 0100000u) {
        /* ----- ALC ----- */
        int acs  = (op >> 13) & 3;
        int acd  = (op >> 11) & 3;
        int func = (op >> 8) & 7;
        int sh   = (op >> 6) & 3;
        int cyf  = (op >> 4) & 3;
        int nl   = (op >> 3) & 1;
        int sk0  = (op >> 2) & 1;   /* SK[0] (result test)  */
        int sk1  = (op >> 1) & 1;   /* SK[1] (carry test)   */
        int sk2  =  op       & 1;   /* SK[2] (invert)       */

        uint16_t S = ac[acs], D = ac[acd];
        int cin = (cyf == 0) ? C : (cyf == 1) ? 0 : (cyf == 2) ? 1 : (C ^ 1);

        uint16_t result;
        int ovf;
        switch (func) {
        case 0: result = (uint16_t)~S;            ovf = 0;              break; /* COM */
        case 1: result = (uint16_t)(0u - S);      ovf = (S == 0);       break; /* NEG */
        case 2: result = S;                        ovf = 0;              break; /* MOV */
        case 3: result = (uint16_t)(S + 1);        ovf = (S == 0xFFFF);  break; /* INC */
        case 4: { uint32_t t = (uint32_t)D + (uint16_t)~S;
                  result = (uint16_t)t; ovf = (t >> 16) & 1; }          break; /* ADC */
        case 5: { uint32_t t = (uint32_t)D + (uint16_t)~S + 1u;
                  result = (uint16_t)t; ovf = (t >> 16) & 1; }          break; /* SUB */
        case 6: { uint32_t t = (uint32_t)D + S;
                  result = (uint16_t)t; ovf = (t >> 16) & 1; }          break; /* ADD */
        default: result = (uint16_t)(D & S);       ovf = 0;             break; /* AND */
        }
        int carryRes = cin ^ ovf;

        uint16_t shifted;
        int shc;
        switch (sh) {
        case 1: /* L: rotate 17 left  */
            shifted = (uint16_t)((result << 1) | (carryRes & 1));
            shc = (result >> 15) & 1;
            break;
        case 2: /* R: rotate 17 right */
            shifted = (uint16_t)((result >> 1) | ((carryRes & 1) << 15));
            shc = result & 1;
            break;
        case 3: /* S: swap bytes, carry unaffected */
            shifted = (uint16_t)((result >> 8) | (result << 8));
            shc = carryRes;
            break;
        default: /* none */
            shifted = result;
            shc = carryRes;
            break;
        }

        r.skip = (((shc == 0) & sk1) | ((shifted == 0) & sk0)) ^ sk2;

        if (!nl) {
            ac[acd] = shifted;
            r.cry = shc & 1;
        }
        r.ac[0] = ac[0]; r.ac[1] = ac[1]; r.ac[2] = ac[2]; r.ac[3] = ac[3];
        return r;
    }

    /* ----- MRI (M / J group) ----- */
    {
        int mfunc = (op >> 13) & 3;
        int dst   = (op >> 11) & 3;
        int ind   = (op >> 10) & 1;
        int x     = (op >> 8) & 3;
        uint16_t disp = op & 0xFF;
        uint16_t E;

        switch (x) {
        case 0:  E = disp; break;                                  /* page zero */
        case 1:  E = (uint16_t)(pc + sign_extend(disp)); break;    /* PC-rel    */
        case 2:  E = (uint16_t)(ac[2] + sign_extend(disp)); break; /* AC2-rel   */
        default: E = (uint16_t)(ac[3] + sign_extend(disp)); break; /* AC3-rel   */
        }
        if (ind) E = mem[E & 0377];   /* one indirect level (page-zero model) */

        uint16_t e = (uint16_t)(E & 0377);

        if (mfunc == 1) {             /* LDA */
            ac[dst] = mem[e];
        } else if (mfunc == 2) {      /* STA */
            mem[e] = ac[dst];
            r.wrote = 1; r.wr_addr = e; r.wr_val = ac[dst];
        } else if (mfunc == 0) {      /* Jump group */
            int jf = (op >> 11) & 3;
            if (jf == 0) {                       /* JMP */
                r.is_jump = 1; r.jump_target = E;
            } else if (jf == 1) {                /* JSR */
                ac[3] = (uint16_t)(pc + 1);
                r.is_jump = 1; r.jump_target = E;
            } else if (jf == 2) {                /* ISZ */
                uint16_t v = (uint16_t)(mem[e] + 1);
                mem[e] = v;
                r.wrote = 1; r.wr_addr = e; r.wr_val = v;
                r.skip = (v == 0);
            } else {                             /* DSZ */
                uint16_t v = (uint16_t)(mem[e] - 1);
                mem[e] = v;
                r.wrote = 1; r.wr_addr = e; r.wr_val = v;
                r.skip = (v == 0);
            }
        } else {
            r.ok = 0;   /* MFunc 3 = S-group/aug, not modeled here */
        }
        r.ac[0] = ac[0]; r.ac[1] = ac[1]; r.ac[2] = ac[2]; r.ac[3] = ac[3];
        return r;
    }
}
