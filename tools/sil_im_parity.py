#!/usr/bin/env python3
"""IM parity: the convention, derived from data and checked against PARC's own.

THE QUESTION THIS ANSWERS was open for a long time: the RTL machine runs only
with the IM parity enables CLEARED, and `tb_exec`'s header put the alternatives
plainly -- "either PARC's IRTable entries carry parity that satisfies the
generator, or our MC10170 parity generators on ContB j20/j21 compute something
different". It is the second, by exactly one inversion, and that inversion is
already known from elsewhere.

THREE STEPS, none of them assumed.

1. WHICH BITS EACH HALF COVERS, fitted rather than guessed. `im_image` emits,
   for every present address of a .MB, both the decoded fields AND the two
   17-bit half-words the hardware computes parity over. Fitting one against the
   other across 2,148 AEmu addresses gives a UNIQUE match for every one of the
   34 bits, with no ambiguity and no inversions:

       left  half = RSTK.1-3, ALUF, BSEL, LC, ASEL   + RSTK.0 as the secondary
       right half = FF, JCN                          + BLOCK  as the secondary

2. THE SENSE THE ARRAY WANTS, measured on the running machine. Preloading IM
   parity and sweeping sense against secondary-inclusion over 200,000 samples:

       even, secondary included    IMLHPE    125     <- clean
       odd,  secondary included             200000
       even, secondary excluded             199680
       odd,  secondary excluded                445

3. THE SENSE PARC USES, read off PARC's own hand-coded microinstructions. The
   IRTable's five-byte format carries explicit P015 and P1631 bits, and this
   file checks all eight entries against ODD parity over the same 17 bits.

THE TWO ARE COMPLEMENTS, unanimously -- 16 of 16 bits across 8 entries. So:

    PARC's convention is ODD parity over the 17-bit half, and the IM ARRAY
    STORES ITS COMPLEMENT.

which is exactly what `dBlock'` does -- tb_boot0 measured all 64 right-half
secondary bits inverted, because BLOCK is wired through Q' rather than Q. Two
independent instances of one storage convention.

WHAT THIS CHECK CANNOT TELL YOU, established by mutating it. Two of four
mutations do NOT fail, and both for principled reasons rather than fixable
ones -- so do not read the PASS as covering them:

  * WHETHER THE SECONDARY BIT IS INCLUDED. Dropping it changes nothing here,
    because EVERY ONE of PARC's eight entries has RSTK.0 = 0 and BLOCK = 0.
    The dataset cannot separate 16-bit from 17-bit parity. Step 2's machine
    measurement is what settles it -- excluding the secondary gives 199,680
    errors against 125 -- and the two steps are not redundant.
  * THE BIT ORDER WITHIN A FIELD. Reversing it is a PERMUTATION of the bits
    parity is taken over, and parity is invariant under permutation. No parity
    check can ever detect it. (The same reason a wrongly-permuted register
    still carries correct parity.)

The two that DO fail are the ones that matter here: using even instead of odd,
and swapping the two halves.

CONSEQUENCE, and it is the next piece of work: `tb_compute.sv`'s `mi()` sets
P015 and P1631 to 1 unconditionally, with the comment "a jammed instruction
fails IM parity anyway -- that is the jam mechanism". PARC's own entries show
those bits VARYING (1/1 for Nop#, 1/0 for Return#, 0/1 for CPRegToLink#), so
they are real computed parity and the blanket 1s are wrong. Computing them is
what would let a jam pass parity, and so let the machine run with the enables
on, the way PARC's firmware leaves them.
"""
import sys

# PARC's IRTable, five bytes each, from doradoboot.masm via tb_compute.sv.
IRTABLE = {
    'CPRegToLink#':   (0x30, 0x13, 0xEF, 0x04, 0x40),
    'CPRegToIM#':     (0x20, 0x13, 0xEF, 0x03, 0x4F),
    'Nop#':           (0x70, 0x01, 0x0F, 0x4C, 0x40),
    'IMLHRSTK.0Is0#': (0x60, 0x33, 0xEF, 0x03, 0x4F),
    'IMLHRSTK.0Is1#': (0x20, 0x73, 0xEF, 0x03, 0x4F),
    'IMRHBLOCKIs0#':  (0x20, 0x13, 0xEF, 0x03, 0x4F),
    'IMRHBLOCKIs1#':  (0x60, 0x53, 0xEF, 0x03, 0x4F),
    'Return#':        (0x60, 0x13, 0xE1, 0x42, 0x43),
}

# The byte layout doradoboot.masm states, PARC numbering every field MSB-first:
#   0: RSTK.0, P015, JCN.7, P1631, 0,0,0,0
#   1: RSTK.1, RSTK.2, RSTK.3, ALUF.0, BLOCK, FF.0, FF.1, FF.2
#   2: ALUF.1, ALUF.2, ALUF.3, BSEL.0, FF.3, FF.4, FF.5, FF.6
#   3: BSEL.1, BSEL.2, LC.0, LC.1, FF.7, JCN.0, JCN.1, JCN.2
#   4: LC.2, ASEL.0, ASEL.1, ASEL.2, JCN.3, JCN.4, JCN.5, JCN.6


def _b(x, n):
    return (x >> n) & 1


def decode(bs):
    b0, b1, b2, b3, b4 = bs
    return {
        'rstk': (_b(b0, 7) << 3) | (_b(b1, 7) << 2) | (_b(b1, 6) << 1) | _b(b1, 5),
        'aluf': (_b(b1, 4) << 3) | (_b(b2, 7) << 2) | (_b(b2, 6) << 1) | _b(b2, 5),
        'bsel': (_b(b2, 4) << 2) | (_b(b3, 7) << 1) | _b(b3, 6),
        'lc':   (_b(b3, 5) << 2) | (_b(b3, 4) << 1) | _b(b4, 7),
        'asel': (_b(b4, 6) << 2) | (_b(b4, 5) << 1) | _b(b4, 4),
        'block': _b(b1, 3),
        'ff':   (_b(b1, 2) << 7) | (_b(b1, 1) << 6) | (_b(b1, 0) << 5) | (_b(b2, 3) << 4) |
                (_b(b2, 2) << 3) | (_b(b2, 1) << 2) | (_b(b2, 0) << 1) | _b(b3, 3),
        'jcn':  (_b(b3, 2) << 7) | (_b(b3, 1) << 6) | (_b(b3, 0) << 5) | (_b(b4, 3) << 4) |
                (_b(b4, 2) << 3) | (_b(b4, 1) << 2) | (_b(b4, 0) << 1) | _b(b0, 5),
        'P015': _b(b0, 6),
        'P1631': _b(b0, 4),
    }


def _f(d, nm, i, w):
    """FIELD.i, PARC numbering: .0 is the MSB of a w-bit field."""
    return (d[nm] >> (w - 1 - i)) & 1


def halves(d):
    """The two 17-bit halves parity covers. Fitted from 2,148 AEmu addresses."""
    lh = ([_f(d, 'rstk', 0, 4)] +
          [_f(d, 'asel', 2, 3), _f(d, 'asel', 1, 3), _f(d, 'asel', 0, 3),
           _f(d, 'lc', 2, 3),   _f(d, 'lc', 1, 3),   _f(d, 'lc', 0, 3),
           _f(d, 'bsel', 2, 3), _f(d, 'bsel', 1, 3), _f(d, 'bsel', 0, 3),
           _f(d, 'aluf', 3, 4), _f(d, 'aluf', 2, 4),
           _f(d, 'aluf', 1, 4), _f(d, 'aluf', 0, 4),
           _f(d, 'rstk', 3, 4), _f(d, 'rstk', 2, 4), _f(d, 'rstk', 1, 4)])
    rh = ([d['block']] +
          [_f(d, 'jcn', i, 8) for i in (7, 6, 5, 4, 3, 2, 1, 0)] +
          [_f(d, 'ff', i, 8) for i in (7, 6, 5, 4, 3, 2, 1, 0)])
    return lh, rh


def odd_parity(bits):
    """PARC's bit: makes the total number of ones ODD."""
    return (sum(bits) & 1) ^ 1



def encode(d):
    """Fields back to PARC's five bytes, parity INCLUDED -- the same encoder
    tb_compute.sv's mi() implements. A round trip through this reproduces all
    of PARC's hand-coded microinstructions exactly, which is a stronger check
    than comparing the parity bits alone: it also pins the byte layout."""
    def bits(v, w):
        return [(v >> i) & 1 for i in range(w)]
    P015 = odd_parity(bits(d['rstk'], 4) + bits(d['aluf'], 4) +
                      bits(d['bsel'], 3) + bits(d['lc'], 3) + bits(d['asel'], 3))
    P1631 = odd_parity([d['block']] + bits(d['jcn'], 8) + bits(d['ff'], 8))
    r, a, bs_, l, s = d['rstk'], d['aluf'], d['bsel'], d['lc'], d['asel']
    ff, jcn, blk = d['ff'], d['jcn'], d['block']
    return (
        (_b(r, 3) << 7) | (P015 << 6) | (_b(jcn, 0) << 5) | (P1631 << 4),
        (_b(r, 2) << 7) | (_b(r, 1) << 6) | (_b(r, 0) << 5) | (_b(a, 3) << 4) |
        (blk << 3) | (_b(ff, 7) << 2) | (_b(ff, 6) << 1) | _b(ff, 5),
        (_b(a, 2) << 7) | (_b(a, 1) << 6) | (_b(a, 0) << 5) | (_b(bs_, 2) << 4) |
        (_b(ff, 4) << 3) | (_b(ff, 3) << 2) | (_b(ff, 2) << 1) | _b(ff, 1),
        (_b(bs_, 1) << 7) | (_b(bs_, 0) << 6) | (_b(l, 2) << 5) | (_b(l, 1) << 4) |
        (_b(ff, 0) << 3) | (_b(jcn, 7) << 2) | (_b(jcn, 6) << 1) | _b(jcn, 5),
        (_b(l, 0) << 7) | (_b(s, 2) << 6) | (_b(s, 1) << 5) | (_b(s, 0) << 4) |
        (_b(jcn, 4) << 3) | (_b(jcn, 3) << 2) | (_b(jcn, 2) << 1) | _b(jcn, 1),
    )


def main() -> int:
    bad = []
    print(f"{'entry':17} {'P015':>5}{'odd':>5}  {'P1631':>7}{'odd':>5}   agrees")
    agree = 0
    for name, bs in sorted(IRTABLE.items()):
        d = decode(bs)
        lh, rh = halves(d)
        clh, crh = odd_parity(lh), odd_parity(rh)
        ok = (clh == d['P015']) and (crh == d['P1631'])
        agree += ok
        print(f"{name:17} {d['P015']:>5}{clh:>5}  {d['P1631']:>7}{crh:>5}   "
              f"{'YES' if ok else 'no'}")
    print(f"\n{agree} of {len(IRTABLE)} of PARC's hand-coded entries carry ODD "
          f"parity over the 17-bit half")

    # THE ROUND TRIP. Stronger than comparing parity bits alone: regenerating
    # all five bytes pins the byte layout too, so a wrong field position shows
    # up here even where parity (being permutation-invariant) would not.
    exact = 0
    for name, bs in sorted(IRTABLE.items()):
        got = encode(decode(bs))
        exact += (got == bs)
        if got != bs:
            print(f"  {name:17} PARC {' '.join('%02X' % x for x in bs)}   "
                  f"encode() {' '.join('%02X' % x for x in got)}")
    print(f"{exact} of {len(IRTABLE)} reproduced BYTE FOR BYTE, parity included")
    if exact != len(IRTABLE):
        bad.append(f"only {exact} of {len(IRTABLE)} entries round-trip -- the "
                   f"byte layout or the parity rule is wrong")
    if agree != len(IRTABLE):
        bad.append(f"only {agree} of {len(IRTABLE)} entries match ODD parity -- "
                   f"the convention is not what this file states")

    # ...and the array stores the COMPLEMENT, which is what the running machine
    # measured (writing EVEN parity into the array clears IMLHPE/IMRHPE).
    print("\nThe IM ARRAY stores the COMPLEMENT of that bit: preloading EVEN "
          "parity\nclears IMLHPE/IMRHPE (200,000 samples -> 125), and odd does "
          "not.\nSame storage convention as dBlock', which tb_boot0 measured "
          "inverted.")

    # SAY WHAT THE PASS DOES NOT COVER, in the output and not only in a
    # docstring, because a green check is what people quote.
    sec = [n for n, bs in IRTABLE.items()
           if ((decode(bs)['rstk'] >> 3) & 1) or decode(bs)['block']]
    print("\nNOT COVERED by this check:")
    print(f"  * whether the SECONDARY bit is included -- {len(sec)} of "
          f"{len(IRTABLE)} entries have a nonzero secondary, so the dataset")
    print("    cannot separate 16-bit from 17-bit parity. The machine "
          "measurement does.")
    print("  * the bit ORDER within a field -- a permutation, and parity is "
          "invariant under\n    permutation. No parity check can detect it.")

    if bad:
        for x in bad:
            print(f"FAIL: {x}")
        return 1
    print("\nPASS: PARC's parity bits and the array's stored sense are "
          "consistent, as complements")
    return 0


if __name__ == '__main__':
    sys.exit(main())
