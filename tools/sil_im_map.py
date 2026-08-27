#!/usr/bin/env python3
"""Map ContB's IM packages to (microinstruction field bit, bank).

IM is 4096 x 36 bits, built from 144 F10415A packages -- each 1024 x 1 -- as
FOUR banks of 1024 words. A package therefore holds ONE bit of the
microinstruction for ONE bank, and the netlist says which:

  * its DATA OUTPUT names the field bit. The 36 distinct output nets are
    exactly a Dorado microinstruction:
        RSTK.0-3  ALUF.0-3  BSEL.0-2  LC.0-2  ASEL.0-2
        FF.0-7    JCN.0-7   Block'    IMLH  IMRH
    4+4+3+3+3+8+8+1+2 = 36.
  * its CHIP ENABLE names the bank -- CS0'/CS1'/CS2'/CS3', with a suffix
    (ACa..ACd, BDa..BDd) identifying which fan-out group drives it.
  * its ADDRESS pins are RA.01a..RA.10a, ten bits within a bank. The cell reads
    them MSB-FIRST: pin 2 takes RA.01a, which comes from TNIA.05. Getting that
    backwards is what once made a Write-IM addressing 195 deposit at 780.

THE INTERLEAVE, derived rather than assumed. The four chip selects are driven by
TWO MC10101s, wired-OR, and their inputs name the bits:

    d21  bdRA.11a on all four gate inputs, common DoCBr
    d22  bdRA.00a on all four gate inputs

So the bank is chosen by RA.00 and RA.11 -- the TOP and BOTTOM of the twelve
address bits, PARC numbering MSB-first -- while RA.01..RA.10 are the ten-bit
index within a bank:

    bank  = {addr[11], addr[0]}
    index = addr[10:1]

Consecutive addresses therefore alternate banks on the LSB, which is exactly
what tb_exec records as "copies 0,2 are A and 1,3 are B, so IM[0] = A and
IM[1] = B".

WHY THIS EXISTS: walking a whole 4096-word world into IM over the
control-processor bus takes far too long to simulate -- boot0-test does 16
microinstructions and exec-test 4 hunks. A bench that wants to RUN a real world
must preload the arrays directly. That is legitimate: the CP-bus path is already
gated by boot0-test, so preloading skips nothing unproven. But it must be
VERIFIED the same way boot0-test verifies its load -- read the arrays back and
compare against the .MB.
"""
import collections
import glob
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0])
import sil_netlist as sn

DI, DO, CE, WE = 15, 1, 14, 13
ADDR_PINS = (2, 3, 4, 5, 6, 7, 9, 10, 11, 12)   # A0..A9, MSB first


def im_packages():
    """(package, field-bit net, bank 0-3, CE net, address nets) per IM package."""
    board = sn.load_board(glob.glob('chm/sil/ContB*/*.wl')[0])
    out = []
    for pkg, rec in sorted(board.packages.items()):
        if rec.get('type') != 'F10415A':
            continue
        pins = {p['pin']: nm for nm, nt in board.nets.items()
                for p in nt['pins'] if p['pkg'] == pkg}
        ce = pins.get(CE) or ''
        bank = int(ce[2]) if len(ce) > 2 and ce[2].isdigit() else None
        out.append((pkg, pins.get(DO), bank, ce,
                    tuple(pins.get(k) for k in ADDR_PINS)))
    return out



# The four chip selects, evaluated. Knowing WHICH bit picks the bank is not the
# same as knowing which VALUE picks which bank -- CS0'..CS3' are net NAMES, and
# nothing says CS0' is the RA.00=0,RA.11=0 corner. That comes from the gate
# polarities, so evaluate them.
#
# MC10101 is a quad 2-input OR/NOR with a COMMON input on pin 12. Its gates and
# the sense of each output pin (role `OUT` = inverting) are cell_MC10101.v's:
#
#     (4 12) > (2 NOR, 5 OR)      (7 12)  > (3 NOR, 6 OR)
#     (10 12) > (14 NOR, 11 OR)   (12 13) > (15 NOR, 9 OR)
#
# The selects are open-emitter wired-OR, so a bank is selected when EVERY
# driver contributes 0.
_GATE_IN = {2: 4, 5: 4, 3: 7, 6: 7, 14: 10, 11: 10, 15: 13, 9: 13}
_INVERTING = {2, 3, 14, 15}


def bank_order(board):
    """Which (RA.00, RA.11) value selects each bank. Returns a list of gripes."""
    bad = []
    drivers = collections.defaultdict(list)     # bank -> [(net, inverting)]
    for cs, nt in board.nets.items():
        if not (cs.startswith('CS') and "'" in cs):
            continue
        bank = int(cs[2])
        for p in nt['pins']:
            if p['dir'] != 'out' or board.packages[p['pkg']].get('type') != 'MC10101':
                continue
            ipin = _GATE_IN.get(p['pin'])
            for nm, n2 in board.nets.items():
                for q in n2['pins']:
                    if q['pkg'] == p['pkg'] and q['pin'] in (ipin, 12) and q['dir'] == 'in':
                        drivers[bank].append((nm, p['pin'] in _INVERTING))

    # RA.01a and RA.01b are FAN-OUT COPIES of one bit, not different bits.
    def val(nm, ra00, ra11):
        s = nm[:-1] if nm and nm[-1].isalpha() and nm[-2].isdigit() else nm
        return {'bdRA.00': ra00, 'bdRA.11': ra11}.get(s, 0)

    sel = {}
    for bank in range(4):
        got = []
        for ra00 in (0, 1):
            for ra11 in (0, 1):
                ok = True
                for nm, inv in drivers[bank]:
                    if nm.startswith('DoCBr'):
                        continue                 # the common pin, ORed in below
                    v = val(nm, ra00, ra11) | 0        # DoCBr = 0
                    contrib = (not v) if inv else v    # NOR pin vs OR pin
                    if contrib:
                        ok = False        # a driver pulling the wired-OR high
                if ok:
                    got.append((ra00, ra11))
        if len(got) != 1:
            bad.append(f"CS{bank}' is selected by {len(got)} of the four "
                       f"(RA.00,RA.11) values, not exactly one: {got}")
        else:
            sel[bank] = got[0]

    if not bad:
        print("  bank select, evaluated from the gate polarities (DoCBr=0):")
        for bank in range(4):
            ra00, ra11 = sel[bank]
            print(f"    CS{bank}' selected by RA.00={ra00} RA.11={ra11}"
                  f"   = addr[11]={ra00} addr[0]={ra11}")
        # PARC numbers MSB first, so RA.00 is addr[11] and RA.11 is addr[0].
        want = {b: (b >> 1, b & 1) for b in range(4)}
        if sel != want:
            bad.append(f"bank ordering is {sel}, not bank={{addr[11],addr[0]}} "
                       f"({want}) -- a preload using that order writes the "
                       f"wrong quarter of every word")
        else:
            print("  => bank = {addr[11], addr[0]},  index = addr[10:1]  (DERIVED)")
    return bad



# ---------------------------------------------------------------------------
# The preload include. 144 hierarchical assignments, generated rather than
# typed, because a transcription error here is a machine executing garbage --
# which is indistinguishable from a boot that fails for its own reasons.
#
# PARC NUMBERS FIELDS MSB-FIRST, so dRSTK.0 is bit 3 of a 4-bit rstk. Getting
# that backwards is the same trap as the ECL address pins.
#
# NOTHING HERE IS ASSUMED ABOUT POLARITY. dBlock' is NAMED active-low, but
# whether the ARRAY stores block or ~block is settled by im-preload-test,
# which compares a preload against a control-processor-bus load of the same
# microcode -- a path boot0-test already gates.
_FIELDS = [('rstk', 4), ('aluf', 4), ('bsel', 3), ('lc', 3), ('asel', 3),
           ('ff', 8), ('jcn', 8)]


def emit_verilog(rows, out):
    bits = {}
    for pkg, field, bank, _, _ in rows:
        bits[(field, bank)] = pkg

    w = out.write
    w("// im_preload.vh -- GENERATED by tools/sil_im_map.py. Do not edit.\n//\n")
    w("// Writes a microinstruction straight into ContB's IM arrays. The\n")
    w("// control-processor bus is the REAL loader (gated by boot0-test); this\n")
    w("// exists because 4,096 words of it will not simulate in reasonable time.\n")
    w("//\n//   bank  = {addr[11], addr[0]}   derived from the CS gate polarities\n")
    w("//   index = addr[10:1]\n\n")
    w("task automatic im_preload_word(input int unsigned addr,\n")
    for nm, wd in _FIELDS:
        w(f"                               input [{wd-1}:0] {nm},\n")
    w("                               input blk,\n")
    w("                               input imlh, input imrh);\n")
    w("  int idx; int bank;\n")
    w("  begin\n")
    w("    idx  = (addr >> 1) & 32'h3FF;\n")
    w("    bank = ((addr >> 11) & 1) * 2 + (addr & 1);\n")
    w("    case (bank)\n")
    for bank in range(4):
        w(f"    2'd{bank}: begin\n")
        for nm, wd in _FIELDS:
            for i in range(wd):
                # PARC numbers MSB first: <FIELD>.0 is the field's top bit.
                pkg = bits[(f"d{nm.upper()}.{i}", bank)]
                w(f"      m.b_ContB.u_{pkg}.mem[idx] = {nm}[{wd-1-i}];"
                  f"   // d{nm.upper()}.{i}\n")
        w(f"      m.b_ContB.u_{bits[(chr(100)+chr(66)+'lock' + chr(39), bank)]}"
          f".mem[idx] = blk;   // dBlock'\n")
        w(f"      m.b_ContB.u_{bits[('dIMLH', bank)]}.mem[idx] = imlh;   // dIMLH\n")
        w(f"      m.b_ContB.u_{bits[('dIMRH', bank)]}.mem[idx] = imrh;   // dIMRH\n")
        w("    end\n")
    w("    endcase\n  end\nendtask\n")

    # And the same map read the other way. This is what makes the map GATEABLE:
    # load microcode the PROVEN way (the control-processor bus, boot0-test),
    # read it back through here, and compare against the .MB.
    w("\n\ntask automatic im_readback_word(input int unsigned addr,\n")
    for nm, wd in _FIELDS:
        w(f"                                output [{wd-1}:0] {nm},\n")
    w("                                output blk,\n")
    w("                                output imlh, output imrh);\n")
    w("  int idx; int bank;\n")
    w("  begin\n")
    w("    idx  = (addr >> 1) & 32'h3FF;\n")
    w("    bank = ((addr >> 11) & 1) * 2 + (addr & 1);\n")
    w("    case (bank)\n")
    for bank in range(4):
        w(f"    2'd{bank}: begin\n")
        for nm, wd in _FIELDS:
            for i in range(wd):
                pkg = bits[(f"d{nm.upper()}.{i}", bank)]
                w(f"      {nm}[{wd-1-i}] = m.b_ContB.u_{pkg}.mem[idx];"
                  f"   // d{nm.upper()}.{i}\n")
        w(f"      blk = m.b_ContB.u_{bits[(chr(100)+chr(66)+'lock'+chr(39), bank)]}"
          f".mem[idx];   // dBlock'\n")
        w(f"      imlh = m.b_ContB.u_{bits[('dIMLH', bank)]}.mem[idx];   // dIMLH\n")
        w(f"      imrh = m.b_ContB.u_{bits[('dIMRH', bank)]}.mem[idx];   // dIMRH\n")
        w("    end\n")
    w("    endcase\n  end\nendtask\n")



# ---------------------------------------------------------------------------
# ALUFM -- 16 entries of 6 bits, and it is TWO PACKAGES on ProcL.
#
# ALUF is not an opcode; it is a 4-bit pointer into a 16-entry memory the
# microcoder loads at startup with the sixteen ALU operations that world
# actually uses. That memory is ProcL e13 and e14, a pair of F10145A (16x4)
# whose DATA pins come straight off the B bus -- which is how you recognise
# them, and it matches HM Table 11d exactly: "ALUFMEM <- B.8, B[11:15]". The
# entry's most significant bit is the ALU's CARRY IN and comes from B.08, not
# from a contiguous field.
#
# The outputs are ALUFdec.0..5, and the F10145A cell fixes the correspondence:
#   mem[a] <= {p12, p11, p4, p5}   and   {p14, p15, p1, p2} = mem[a]
# so pin 12 is the entry's top bit and pin 2 its bottom one.
#
# THE C EMULATOR AGREES, INDEPENDENTLY. cpu.c keeps the entry as a byte in LSB
# order and spells out the same correspondence: alufm[5]=B[8], alufm[4]=B[11],
# alufm[3]=B[12], alufm[2]=B[13], alufm[1]=B[14], alufm[0]=B[15]. So
# ALUFdec.n is bit 5-n of the C emulator's byte, and neither model was derived
# from the other.
_ALUFM_WR = {12: 3, 11: 2, 4: 1, 5: 0}     # data pin -> bit within the package
_ALUFM_RD = {14: 3, 15: 2, 1: 1, 2: 0}     # output pin -> bit within the package


def alufm_packages():
    """[(pkg, {bit-in-package: ALUFdec index}, {bit: B index})] for ProcL."""
    board = sn.load_board(glob.glob('chm/sil/ProcL*/*.wl')[0])
    out = []
    for pkg, rec in sorted(board.packages.items()):
        if rec.get('type') != 'F10145A':
            continue
        pins = {p['pin']: nm for nm, nt in board.nets.items()
                for p in nt['pins'] if p['pkg'] == pkg}
        # The ALUFM packages are the ones fed from the B bus.
        if not any((pins.get(k) or '').startswith('alub.') for k in _ALUFM_WR):
            continue
        rd, wr = {}, {}
        for pin, bit in _ALUFM_RD.items():
            nm = pins.get(pin) or ''
            if nm.startswith('ALUFdec.'):
                rd[bit] = int(nm.split('.')[1])
        for pin, bit in _ALUFM_WR.items():
            nm = pins.get(pin) or ''
            if nm.startswith('alub.'):
                wr[bit] = int(nm[5:7])
        out.append((pkg, rd, wr))
    return out


def emit_alufm(out):
    rows = alufm_packages()
    bad = []
    seen = {}
    for pkg, rd, wr in rows:
        for bit, dec in rd.items():
            if bit in wr:
                seen[dec] = wr[bit]
    # HM Table 11d: the six bits are B.08 then B.11..B.15, in ALUFdec order.
    want = {0: 8, 1: 11, 2: 12, 3: 13, 4: 14, 5: 15}
    if seen != want:
        bad.append(f"ALUFdec -> B mapping is {seen}, not HM Table 11d's {want}")

    w = out.write
    w("\n\n// ALUFM -- 16 x 6, ProcL e13/e14. See tools/sil_im_map.py.\n")
    w("// The entry is the C emulator's byte: bit 5 is B.08 (the ALU CARRY IN),\n")
    w("// bits 4..0 are B.11..B.15. ALUFdec.n is bit 5-n of it.\n")
    for name, val, assign in (("alufm_preload_word", "input  [5:0] e", True),
                              ("alufm_readback_word", "output [5:0] e", False)):
        w(f"task automatic {name}(input int unsigned aluf, {val});\n  begin\n")
        for pkg, rd, _ in rows:
            for bit in sorted(rd, reverse=True):
                dec = rd[bit]
                if assign:
                    w(f"    m.b_ProcL.u_{pkg}.mem[aluf[3:0]][{bit}] = e[{5-dec}];"
                      f"   // ALUFdec.{dec}\n")
                else:
                    w(f"    e[{5-dec}] = m.b_ProcL.u_{pkg}.mem[aluf[3:0]][{bit}];"
                      f"   // ALUFdec.{dec}\n")
        w("  end\nendtask\n")
    return bad



# ---------------------------------------------------------------------------
# IFUM -- 1024 x 27, and it is the IFU's twenty-seven F10415A.
#
# The IFU decodes an opcode by looking it up here, so a world loaded without
# IFUM cannot dispatch at all: it takes the IFU fault handlers and never
# reaches an opcode implementation.
#
# EVERY PIECE OF THE LAYOUT IS IN THE NETLIST. Each package's OUTPUT names the
# field bit (InstrAddrK.0'-9', NK.0-3, MemBK.0/1, MemBK34, LengthK.0'/1',
# SignK, TypeJumpK', TypePauseK', RBaseSelK', TwoAlphaK, RamParity.0-2 = 27),
# its ADDRESS pins are {InstrSet.0, InstrSet.1, J.0..J.7} -- two bits of
# instruction set and the eight-bit opcode, MSB first, giving 256 entries x 4
# instruction sets -- and its DATA pin names the bit of the 16-bit write bus:
# RcvdBMux.00 is the MSB, PARC numbering.
#
# THE TWO HALVES ARE TWO WRITE ENABLES, DecHi' and DecLo', which is why a .MB
# stores two words per entry. Which half is which is settled by comparing the
# bit positions against cpu.c, and the agreement is exact and complete:
#
#   DecLo' group, all sixteen bits, against cpu.c's ifu_decode_lh (HM Table 20)
#       00 Sign | 01-03 IPar | 04-05 Length' | 06 RBaseB' | 07-09 MemB
#       10 TPause' | 11 TJump' | 12-15 N
#   DecHi' group, eleven bits 05-15, against cpu.c's `ifum_lo[addr] & 0x07FF`
#       05 TwoAlpha | 06-15 InstrAddr (the microcode entry point)
#
# So DecLo' holds what the C emulator calls ifum_hi (.MB word 1) and DecHi'
# holds ifum_lo (.MB word 0). The names cross over; the bits do not.
_IFUM_WE = {"DecHi_'": 0, "DecLo_'": 1}     # write enable -> .MB word index


def ifum_packages():
    """[(pkg, word-index, MSB-first bit, output net)] for the IFU board."""
    board = sn.load_board(glob.glob('chm/sil/IFU*/*.wl')[0])
    out = []
    for pkg, rec in sorted(board.packages.items()):
        if rec.get('type') != 'F10415A':
            continue
        pins = {p['pin']: nm for nm, nt in board.nets.items()
                for p in nt['pins'] if p['pkg'] == pkg}
        we = pins.get(13)
        di = pins.get(15) or ''
        if we not in _IFUM_WE or not di.startswith('RcvdBMux.'):
            continue
        out.append((pkg, _IFUM_WE[we], int(di.split('.')[1]), pins.get(1)))
    return out


def emit_ifum(out):
    rows = ifum_packages()
    bad = []
    if len(rows) != 27:
        bad.append(f"expected 27 IFUM packages, found {len(rows)}")
    lo = sorted(b for _, w, b, _ in rows if w == 0)
    hi = sorted(b for _, w, b, _ in rows if w == 1)
    if lo != list(range(5, 16)):
        bad.append(f"DecHi' bits are {lo}, not 05-15 (cpu.c masks ifum_lo & 0x07FF)")
    if hi != list(range(0, 16)):
        bad.append(f"DecLo' bits are {hi}, not 00-15 (cpu.c's Table 20 layout)")

    w = out.write
    # GUARDED, because these reach into m.b_IFU and the four-board machine has
    # no IFU board: tb_boot0 and the default tb_exec include this same file.
    w("\n\n`ifdef WORLD\n")
    w("// IFUM -- 1024 x 27, the IFU's F10415A. See tools/sil_im_map.py.\n")
    w("// Address is {InstrSet[1:0], opcode[7:0]}. `lo` is .MB word 0 (written\n")
    w("// by DecHi'), `hi` is .MB word 1 (DecLo'). RcvdBMux.00 is the MSB, so\n")
    w("// MSB-first bit n is bit 15-n of the C-side word.\n")
    for name, sig, assign in (("ifum_preload_word", "input  [15:0]", True),
                              ("ifum_readback_word", "output [15:0]", False)):
        w(f"task automatic {name}(input int unsigned addr, {sig} lo, {sig} hi);\n")
        w("  int idx;\n  begin\n    idx = addr[9:0];\n")
        for pkg, word, bit, netname in rows:
            var = ('lo', 'hi')[word]
            if assign:
                w(f"    m.b_IFU.u_{pkg}.mem[idx] = {var}[{15-bit}];"
                  f"   // {netname}\n")
            else:
                w(f"    {var}[{15-bit}] = m.b_IFU.u_{pkg}.mem[idx];"
                  f"   // {netname}\n")
        if not assign:
            # the five bits DecHi' does not hold read back as zero
            for k in range(5):
                w(f"    lo[{15-k}] = 1'b0;   // DecHi' holds only bits 05-15\n")
        w("  end\nendtask\n")
    w("`endif\n")
    return bad


def main() -> int:
    rows = im_packages()
    bits = collections.defaultdict(dict)
    for pkg, field, bank, ce, _ in rows:
        bits[field][bank] = (pkg, ce)

    print(f"{len(rows)} IM packages, {len(bits)} field bits, "
          f"{len({b for _, _, b, _, _ in rows})} banks\n")
    print(f"{'field bit':<12} " + "".join(f"bank{b:<8}" for b in range(4)))
    bad = []
    for field in sorted(bits):
        line = f"  {field:<10} "
        for b in range(4):
            ent = bits[field].get(b)
            line += f"{(ent[0] if ent else '--'):<12}"
            if ent is None:
                bad.append(f"{field} has no package for bank {b}")
        print(line)

    # The address nets carry a FAN-OUT SUFFIX -- RA.01a, RA.01b, ... are copies
    # of one bit driven to different parts of the array, not different bits.
    # Strip it before comparing, or 144 packages look like twelve address buses.
    def stem(nets):
        return tuple(n[:-1] if n and n[-1].isalpha() and n[-2].isdigit() else n
                     for n in nets)
    addr = {stem(r[4]) for r in rows}
    if len(addr) != 1:
        bad.append(f"IM packages do not share one address bus: {len(addr)} variants")
    else:
        print(f"\naddress (A0..A9, MSB first): {', '.join(addr.pop())}")
        print(f"  ({len({r[4] for r in rows})} fan-out copies of it across the array)")

    # THE INTERLEAVE. The chip selects must be driven by exactly two address
    # bits, and they must be RA.00 and RA.11 -- the top and bottom of the twelve.
    board = sn.load_board(glob.glob('chm/sil/ContB*/*.wl')[0])
    csdrv = set()
    for cs in ("CS0'ACa", "CS1'ACa", "CS2'ACa", "CS3'ACa"):
        net = board.nets.get(cs)
        if not net:
            bad.append(f"{cs} not found")
            continue
        for p in net['pins']:
            if p['dir'] != 'out':
                continue
            for nm, nt in board.nets.items():
                if any(q['pkg'] == p['pkg'] and q['dir'] == 'in' and
                       nm.startswith('bdRA.') for q in nt['pins']):
                    csdrv.add(nm)
    want = {'bdRA.00a', 'bdRA.11a'}
    print(f"\nbank select driven by: {sorted(csdrv)}")
    if csdrv != want:
        bad.append(f"bank select is driven by {sorted(csdrv)}, not {sorted(want)} "
                   f"-- the interleave is not bank={{addr[11],addr[0]}}")
    else:
        bad += bank_order(board)

    if len(rows) != 144 or len(bits) != 36:
        bad.append(f"expected 144 packages and 36 field bits, got "
                   f"{len(rows)} and {len(bits)}")

    if bad:
        for x in bad:
            print(f"FAIL: {x}")
        return 1

    if '--verilog' in sys.argv:
        path = 'verilog/generated/im_preload.vh'
        with open(path, 'w') as f:
            emit_verilog(rows, f)
            abad = emit_alufm(f) + emit_ifum(f)
        if abad:
            for x in abad:
                print(f"FAIL: {x}")
            return 1
        print(f"\nwrote {path}")
    print("\nPASS: 36 field bits x 4 banks = 144 packages, one shared address bus")
    return 0


if __name__ == '__main__':
    sys.exit(main())
