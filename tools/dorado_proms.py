#!/usr/bin/env python3
"""Generate Dorado PROM contents from PARC's own definitions.

The Dorado's PROMs are not blobs anyone has to dump: `DoradoProms.run` is a
BCPL PROGRAM that computes every PROM's contents, and its source survives at
`<DoradoSource>DoradoProms.dm!14_` (mirrored into `chm/doradoproms/`). From
`DoradoProms.help`:

    "DoradoProms is a program for defining, blowing, and listing any and all
     proms used the the DORADO. ... DoradoProms.run is run to produce a
     microbinary format file (called DoradoProms.mb) which contains the
     actual PROM definitions"

and it emits a `Prom.cm` carrying, per PROM, "the 'name' of the prom, and a
label containg board name and location". So the archive gives us contents AND
placement -- better than raw binaries, because the source says what each PROM
*means*.

This tool ports the content generators to Python. They are small, closed-form
loops, so the port is a transcription rather than a reimplementation, and each
one cites the BCPL procedure it came from.

WHY THIS IS WORTH DOING TWICE. `MakeLMask`/`MakeRMask` compute the shifter's
edge masks -- which the C emulator also implements, independently, from the
Hardware Manual (`shifter_output()`, "ALUF[0:2] mask op: ShiftNoMask,
ShiftLMask, ShiftRMask, ShiftBothMasks"). Generating them from PARC's PROM
source and diffing against the emulator checks one against the other. That is
the same cross-check that has repeatedly been worth more here than either
source alone.

Usage:
    dorado_proms.py --list                 the PROM map (name, board, location)
    dorado_proms.py --emit LMASK [--out DIR]
    dorado_proms.py --check                compare against the C emulator
"""

from __future__ import annotations

import argparse
import os
import re
import sys

SRC = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                   '..', 'chm', 'doradoproms')


# --- ported generators ------------------------------------------------
# Each returns (name, width_bits, list-of-words).

def make_lmask() -> tuple[str, int, list[int]]:
    """ProcProms.bcpl MakeLMask.

        Zero(buff,32); let Lbit = #100000; let Lmask = 0
        for address = 1 to 31 do        //address 0 should be zero
            [ Lmask = Lmask % Lbit; Lbit = Lbit rshift 1; buff!address = Lmask ]

    A LEFT mask: entry N has the top N bits set. `#100000` is octal, i.e.
    bit 15 of a 16-bit word. Note the BCPL runs to 31 while the word is 16
    bits, so entries 16..31 are all-ones -- that is the part's own behaviour
    (a 32-entry PROM holding a 16-bit mask), not a transcription slip.
    """
    buff = [0] * 32
    lbit = 0o100000
    lmask = 0
    for address in range(1, 32):
        lmask |= lbit
        lbit >>= 1
        buff[address] = lmask & 0xFFFF
    return ('LMASK', 16, buff)


def make_rmask() -> tuple[str, int, list[int]]:
    """ProcProms.bcpl MakeRMask -- the mirror: entry N has the low N bits set."""
    buff = [0] * 32
    rbit = 1
    rmask = 0
    for address in range(1, 32):
        rmask |= rbit
        rbit = (rbit << 1) & 0xFFFF
        buff[address] = rmask & 0xFFFF
    return ('RMASK', 16, buff)



def make_keyboard_map() -> tuple[str, int, list[int]]:
    """IFUProms.bcpl MakeKeyboardMap -- 128 x 8, at IFU-k05 (+ l05 right nibble).

    This is the Dorado's OWN keyboard map, and therefore the authority on it.
    The BCPL builds it in three steps:

        Map = table [ 08;00;16;29;... ]        // 64 entries, addr -> value
        for val 0..63:  buff[val] = the addr where Map[addr] == val   // INVERT
        for val 64..127: buff[val] = (val-64) xor #60
        for val 0..127:  buff[val] = buff[val] xor #77

    Note the inversion: the table is written as addr->value and the PROM holds
    value->addr. Both XORs are octal (#60 = 48, #77 = 63)."""
    Map = [
        8, 0, 16, 29, 36, 30, 37, 11,
        52, 46, 60, 53, 63, 49, 5, 61,
        28, 20, 21, 25, 22, 26, 56, 45,
        31, 57, 58, 59, 62, 41, 51, 33,
        24, 12, 13, 2, 14, 3, 38, 19,
        23, 27, 55, 54, 34, 50, 4, 6,
        1, 9, 10, 17, 18, 44, 39, 47,
        15, 43, 40, 32, 35, 48, 7, 42,
    ]
    buff = [0] * 128
    for val in range(64):
        for addr in range(64):
            if Map[addr] == val:
                buff[val] = addr
    for val in range(64, 128):
        buff[val] = (val - 64) ^ 0o60
    for val in range(128):
        buff[val] ^= 0o77
    return ('Keyboard-Map', 8, buff)


def make_mouse_motion() -> tuple[str, int, list[int]]:
    """IFUProms.bcpl MakeMouseMotion -- 256 x 4, at IFU-i03.

    A QUADRATURE DECODER in a PROM. The address is the four mouse phase
    signals plus their delayed copies, so the PROM sees a transition and
    reports which way each axis moved. The source's own encoding:

        Prom=0: Y=0    X=0        Prom=3: Y=Y+1  X=0     Prom=6: Y=Y-1  X=0
        Prom=1: Y=0    X=X+1      Prom=4: Y=Y+1  X=X+1   Prom=7: Y=Y-1  X=X+1
        Prom=2: Y=0    X=X-1      Prom=5: Y=Y+1  X=X-1   Prom=8: Y=Y-1  X=X-1

    i.e. value = 3*Ydir + Xdir with each direction in {0 none, 1 up, 2 down}.

    Bit order comes from the BCPL structure, which is MSB-first over the low
    byte: MX1=0x80 MX2=0x40 MY1=0x20 MY2=0x10, then the delayed copies
    MX1dly=0x08 MX2dly=0x04 MY1dly=0x02 MY2dly=0x01."""
    MX1, MX2, MY1, MY2 = 0x80, 0x40, 0x20, 0x10
    MX1d, MX2d, MY1d, MY2d = 0x08, 0x04, 0x02, 0x01
    buff = [0] * 256
    for addr in range(256):
        b = lambda m: 1 if (addr & m) else 0
        xdir = 0
        if b(MX1) != b(MX1d):
            xdir = (b(MX1) ^ b(MX2)) + 1
        if b(MX2) != b(MX2d):
            xdir = (b(MX1) ^ b(MX2) ^ 1) + 1
        ydir = 0
        if b(MY1) != b(MY1d):
            ydir = (b(MY1) ^ b(MY2)) + 1
        if b(MY2) != b(MY2d):
            ydir = (b(MY1) ^ b(MY2) ^ 1) + 1
        buff[addr] = (3 * ydir + xdir) & 0xF
    return ('Mouse-Motion', 4, buff)


def make_data_select() -> tuple[str, int, list[int]]:
    """IFUProms.bcpl MakeDataSelect -- 32 x 8, at IFU-a06.

    A literal table (octal in the source): the data-select control per
    instruction type."""
    T = [0o377, 0o377, 0o377, 0o377, 0o377, 0o367, 0o263, 0o221,
         0o377, 0o377, 0o377, 0o377, 0o377, 0o377, 0o225, 0o204,
         0o377, 0o377, 0o377, 0o377, 0o377, 0o377, 0o167, 0o063,
         0o377, 0o377, 0o377, 0o377, 0o377, 0o377, 0o073, 0o031]
    return ('Data-Select', 8, list(T))


# --- MemX: the memory system's control PROMs --------------------------
# All seven are literal tables in MemProms.bcpl, and their ROW COMMENTS are
# the documentation: each 8-entry row is one memory operation, so the tables
# read as a state machine indexed by (operation, cycle).

def _table(name, width, rows, adjust=None) -> tuple[str, int, list[int]]:
    """Assemble a PROM image the way PARC's own blower does.

    `DoradoProms.bcpl Body()` does not store a table entry verbatim. It
    LEFT-JUSTIFIES it into a 16-bit word and keeps the top `width` bits:

        mask = 0
        for i = 1 to MemWidth do mask = (mask rshift 1) % #100000
        data = Buff!address
        data = data lshift adjust    // left justify it
        data = data & mask           // mask off the unused bits

    so the byte actually blown is `(value << adjust) >> (16 - width)`. Every
    Header() in the sources passes `adjust = 16 - width` -- which makes that
    the identity, and is why transcribing the tables verbatim worked for all
    of them -- EXCEPT ONE:

        Header("Map-Mem", 8, buff, 32, 10)

    Ten where eight is standard, so MemX-i14's bytes are PARC's table values
    shifted LEFT BY TWO. Transcribing them raw put every output on the wrong
    pin. `preStartMem'` is i14 pin 3, and raw it reads asserted at exactly one
    address; shifted it asserts at MapState=3 for Refresh, Read AND Write, and
    never for a Map write -- i.e. a reference starts a memory cycle and a pure
    map write does not, which is the behaviour the board needs.
    """
    if adjust is None:
        adjust = 16 - width                      # the identity case
    shift = adjust - (16 - width)
    mask = (1 << width) - 1
    words = []
    for _label, row in rows:
        for w in row:
            words.append(((w << shift) if shift >= 0 else (w >> -shift)) & mask)
    return (name, width, words)


def make_st() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeST -- 32 x 16, MemX-h11 (left half) + i12 (right).

    The STATE memory: the memory system's sequencer. Its two halves are
    commented "11 cycles IO store" and "12 cycles Victim store" -- i.e. the
    cycle counts for a store that goes to I/O versus one that has to evict a
    dirty victim line first."""
    return _table('ST', 16, [
        ('11 cycles IO store', [
            0o052600, 0o012600, 0o052600, 0o012600, 0o056600, 0o016600,
            0o056200, 0o136000, 0o177000, 0o175000, 0o063600, 0o063600,
            0o177000, 0o177000, 0o177000, 0o177000]),
        ('12 cycles Victim store', [
            0o063406, 0o052406, 0o012406, 0o052406, 0o012406, 0o056406,
            0o016406, 0o056404, 0o136000, 0o177000, 0o175000, 0o173002,
            0o177000, 0o177000, 0o177000, 0o177000]),
    ])


def make_ec1() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeEC1 -- 32 x 8, MemX-l12.

    Row comments name the four cache operations the memory system
    distinguishes, which is the cache state machine in four lines."""
    return _table('EC-1', 8, [
        ('Write',                  [0o003, 0o207, 0o203, 0o203, 0o203, 0o202, 0o201, 0o203]),
        ('Cache Load',             [0o003, 0o207, 0o313, 0o313, 0o313, 0o312, 0o201, 0o203]),
        ('Clean or miss IO fetch', [0o003, 0o207, 0o203, 0o223, 0o223, 0o222, 0o201, 0o203]),
        ('Dirty hit IO fetch',     [0o003, 0o207, 0o303, 0o343, 0o363, 0o362, 0o201, 0o203]),
    ])


def make_ec2() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeEC2 -- 32 x 8, MemX-l11. Same four operations."""
    return _table('EC-2', 8, [
        ('Write',                  [0o005, 0o001, 0o001, 0o001, 0o001, 0o001, 0o011, 0o001]),
        ('Cache Load',             [0o226, 0o222, 0o222, 0o002, 0o000, 0o000, 0o011, 0o222]),
        ('Clean or miss IO fetch', [0o045, 0o041, 0o041, 0o041, 0o001, 0o001, 0o011, 0o041]),
        ('Dirty hit IO fetch',     [0o347, 0o343, 0o343, 0o143, 0o041, 0o001, 0o011, 0o343]),
    ])


def make_map_mem() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMapMem -- 32 x 8, MemX-i14.

    Rows: Refresh / Read / Write / Map write -- the four things the MAP can be
    doing to storage."""
    return _table('Map-Mem', 8, [
        ('Refresh',   [0o12, 0o12, 0o12, 0o46, 0o12, 0o12, 0o10, 0o12]),
        ('Read',      [0o12, 0o12, 0o13, 0o07, 0o12, 0o12, 0o10, 0o12]),
        ('Write',     [0o12, 0o12, 0o13, 0o07, 0o12, 0o12, 0o10, 0o12]),
        ('Map write', [0o12, 0o12, 0o13, 0o13, 0o12, 0o12, 0o10, 0o12]),
    ], adjust=10)   # NON-STANDARD -- see _table(); the bytes are these << 2


def make_map_map() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMapMap -- 32 x 8, MemX-g15. Same four rows."""
    return _table('Map-Map', 8, [
        ('Refresh',   [0o037, 0o037, 0o037, 0o235, 0o275, 0o337, 0o327, 0o007]),
        ('Read',      [0o007, 0o007, 0o007, 0o005, 0o045, 0o107, 0o127, 0o007]),
        ('Write',     [0o007, 0o007, 0o007, 0o004, 0o044, 0o107, 0o127, 0o007]),
        ('Map write', [0o007, 0o007, 0o007, 0o000, 0o040, 0o107, 0o127, 0o007]),
    ])


def make_mem16() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMem16 -- 32 x 8, MX16k-j13.

    DRAM timing for 16K parts: read/write, idle, refresh, idle."""
    return _table('16k-Mem', 8, [
        ('Read or write', [0o353, 0o053, 0o153, 0o373, 0o352, 0o340, 0o306, 0o303]),
        ('Idle state',    [0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306]),
        ('Refresh',       [0o303, 0o303, 0o303, 0o303, 0o302, 0o300, 0o306, 0o303]),
        ('Idle state',    [0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306]),
    ])


def make_mem4() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMem4 -- 32 x 8, MX4k-j14. RETIRED.

    DRAM timing for 4K parts. The generator survives but its call site is
    COMMENTED OUT, and the file header says why:

        "change change tomemx-16k-j13 from -j14. comment-out the memx-4k
         option.  September 26, 1979"

    So by late 1979 the memory boards were built with 16K DRAMs and the 4K
    timing PROM was no longer blown. Kept here because it is real data about
    an earlier configuration, and because it explains why the board has two
    PROM sockets (j13 and j14) for one function."""
    return _table('4k-Mem', 8, [
        ('Read or write', [0o303, 0o313, 0o113, 0o153, 0o153, 0o053, 0o153, 0o363]),
        ('(second half)', [0o302, 0o302, 0o302, 0o306, 0o303, 0o306, 0o306, 0o306]),
        ('Refresh',       [0o303, 0o303, 0o303, 0o343, 0o343, 0o343, 0o343, 0o343]),
        ('(second half)', [0o302, 0o302, 0o302, 0o306, 0o303, 0o306, 0o306, 0o306]),
    ])


# --- DskEth: the Ethernet PROMs (EtherProms.bcpl, Taft, 27-Aug-81) -----
# Unlike the MemX PROMs these are COMPUTED, so these are real ports of the
# BCPL rather than transcribed tables. Bit positions come from the source's
# own `structure` declarations, which are MSB-first over a 16-bit word; for
# an 8-bit address that puts the first field at bit 7.

def make_ether_fifo() -> tuple[str, int, list[int]]:
    """EtherProms.bcpl EtherFifo -- 256 x 4, at Eth-l10 AND Eth-l15.

    A circular-FIFO full/empty comparator, and the same PROM is blown twice:
    once for the transmit FIFO and once for the receive FIFO ("the second one
    is identical" in the source).

        Input:  blank bit 8, write bit 4, read bit 4
        Output: full, notFull, empty, notEmpty

        full  = ((read-1) & 17b) eq write
        empty = read eq write

    i.e. full when the read pointer is one ahead of the write pointer modulo
    16, empty when they coincide -- the classic one-slot-sacrificed circular
    buffer. Both senses are brought out because MECL gives you the complement
    for free."""
    buff = [0] * 256
    for adr in range(256):
        write = (adr >> 4) & 0xF
        read = adr & 0xF
        full = 1 if ((read - 1) & 0o17) == write else 0
        empty = 1 if read == write else 0
        buff[adr] = (full << 3) | ((1 - full) << 2) | (empty << 1) | (1 - empty)
    return ('EtherFifo', 4, buff)


def make_ether_pd() -> tuple[str, int, list[int]]:
    """EtherProms.bcpl EtherPD -- 256 x 4, at Eth-h22.

    The MANCHESTER PHASE DECODER. Its address is the carrier, the new and old
    data samples, a 4-bit inter-transition TIMER and a reportCollisions
    strap; it classifies each transition by how long since the last one:

        timer  0..1   too many transitions  -> collision
        timer  2..4   setup transition      -> ignore
        timer  5..9   data transition       -> dataZero / dataOne, reload
        timer 10..15  too few transitions   -> collision (or data), reload

    and with no transition, timer >= 12 means jam or end of packet. That is
    Ethernet bit recovery done in one 256x4 PROM."""
    noEvent, collision, dataZero, dataOne = 0, 1, 2, 3
    count, load = 1, 0
    buff = [0] * 256
    for adr in range(256):
        pdCarrier = (adr >> 7) & 1
        newData = (adr >> 6) & 1
        oldData = (adr >> 5) & 1
        timer = (adr >> 1) & 0xF
        reportCollisions = adr & 1

        carrier = pdCarrier
        event = noEvent
        cntCtrl = count

        if pdCarrier:
            if oldData != newData:
                if timer <= 1:
                    if reportCollisions:
                        event = collision
                elif timer <= 4:
                    pass
                elif timer <= 9:
                    cntCtrl = load
                    event = dataOne if newData else dataZero
                else:
                    cntCtrl = load
                    event = collision if reportCollisions else (
                        dataOne if newData else dataZero)
            else:
                if timer >= 12:
                    carrier = 1 if newData else 0
                    cntCtrl = load
        else:
            if oldData != newData:
                cntCtrl = load
                carrier = 1
                event = dataOne if newData else collision

        buff[adr] = (carrier << 3) | ((event & 3) << 1) | cntCtrl
    return ('EtherPD', 4, buff)


# The receiver and transmitter are the only PROMs in the machine that are
# genuine STATE MACHINES rather than tables or closed-form functions, so they
# are transcribed with the source's own state names and each has a property
# check below asserting the behaviour its comments describe.
#
# Two traps, both marked in the source and both easy to lose:
#   - LOW TRUE INPUTS. `rxSRFull`, `txAbort` and `txSREmpty` are read as
#     `... eq 0`, so the PROM sees the signal asserted when its address bit is
#     ZERO.
#   - LOW TRUE OUTPUT. `rxSync` is stored inverted (`rxSync? low, high`).
# And BCPL `switchon` FALLS THROUGH without `endcase`, so `case collision:`
# followed immediately by `case dataZero:` means collision runs dataZero's
# body -- which is what the comments say it should ("Getting collision means
# the phase decoder missed the first bit of the packet").

def make_ether_rcvr() -> tuple[str, int, list[int]]:
    """EtherProms.bcpl EtherRcvr -- 256 x 12, three parts: Eth-h09 (left
    nibble), Eth-h10 (middle), Eth-h11 (right).

    THE RECEIVER. Four states, and the source names what each one means:

        idle   waiting for a packet; CRC held in reset
        maybe  in the first word -- nothing in the Fifo yet, so a runt can be
               abandoned without telling anyone downstream
        full   at a word boundary, shift register full
        imip   in the middle of a word

    The interesting design point is in the comments rather than the code: a
    carrier drop out of `full` is the NORMAL end of a packet (rxIncTrans
    false), while the same drop out of `imip` or `maybe` means the packet was
    cut mid-word, so the incomplete-transmission bit is set. And a packet is
    recognised by a degenerate one-bit preamble -- in `idle`, dataOne is the
    mark bit: it is clocked into the CRC but NOT into the receiver shift
    register, while dataZero or a collision there means the phase decoder is
    out of step and the fragment is flagged.

        Input:  currentState 7:5, rxCollision 4, pdCarrier 3,
                pdEvent 2:1, rxSRFull 0 (*** low true ***)
        Output: nextState 11:9, rxCollision 8, rxEOP 7,
                rxSync 6 (*** low true ***), rxIncTrans 5, rxCRCReset 4,
                rxCRCClk 3, rxData 2, rxSRCtrl 1:0
    """
    high, low = 1, 0
    srLoad, srShift, srHold = 0, 2, 3          # low,low / high,low / high,high
    idle, maybe, full, imip = 0, 1, 2, 3
    noEvent, collision, dataZero, dataOne = 0, 1, 2, 3

    buff = [0] * 256
    for adr in range(256):
        currentState = (adr >> 5) & 7
        rxCollision = ((adr >> 4) & 1) != 0
        pdCarrier = ((adr >> 3) & 1) != 0
        pdEvent = (adr >> 1) & 3
        rxSRFull = (adr & 1) == 0              # *** low true ***

        nextState = currentState
        preCollision = rxCollision
        rxEOP = False
        rxSync = False
        rxIncTrans = False
        rxCRCReset = low
        rxCRCClk = low
        rxData = low
        rxSRCtrl = srHold

        if currentState == idle:
            # All paths into idle have done srLoad, which reset the bit
            # counter, so the assertion the source makes holds by
            # construction.
            rxCRCReset = high
            preCollision = False
            if pdCarrier:
                nextState = maybe
                if pdEvent in (collision, dataZero):
                    preCollision = True
                elif pdEvent == dataOne:
                    rxCRCReset = low           # the mark bit
                    rxCRCClk = high
                    rxData = high

        elif currentState == maybe:
            if not pdCarrier:
                # A runt, almost certainly a collision. The data is discarded;
                # status is reported only if the PD already saw a collision,
                # which implies the microcode asked to see them.
                if rxCollision:
                    rxIncTrans = True
                    rxEOP = True
                    rxSync = True
                rxSRCtrl = srLoad
                nextState = idle
            elif pdEvent == collision:
                preCollision = True
            elif pdEvent in (dataZero, dataOne):
                rxSRCtrl = srShift
                rxCRCClk = high
                rxData = high if pdEvent == dataOne else low
                if rxSRFull:
                    nextState = full           # dump the word into the Fifo
                    rxSync = True

        elif currentState == full:
            if not pdCarrier:
                nextState = idle               # the normal end of a packet
                rxSRCtrl = srLoad
                rxIncTrans = False
                rxEOP = True
                rxSync = True
            elif pdEvent == collision:
                preCollision = True
            elif pdEvent in (dataZero, dataOne):
                rxSRCtrl = srShift
                rxCRCClk = high
                rxData = high if pdEvent == dataOne else low
                nextState = imip

        elif currentState == imip:
            if not pdCarrier:
                nextState = idle               # cut off mid-word
                rxSRCtrl = srLoad
                rxIncTrans = True
                rxEOP = True
                rxSync = True
            elif pdEvent == collision:
                preCollision = True
            elif pdEvent in (dataZero, dataOne):
                rxSRCtrl = srShift
                rxCRCClk = high
                rxData = high if pdEvent == dataOne else low
                if rxSRFull:
                    nextState = full
                    rxSync = True

        else:                                  # unused states go to idle
            nextState = idle
            rxSRCtrl = srLoad

        buff[adr] = (((nextState & 7) << 9) |
                     ((high if preCollision else low) << 8) |
                     ((high if rxEOP else low) << 7) |
                     ((low if rxSync else high) << 6) |     # *** low true ***
                     ((high if rxIncTrans else low) << 5) |
                     (rxCRCReset << 4) |
                     (rxCRCClk << 3) |
                     (rxData << 2) |
                     (rxSRCtrl & 3))
    return ('EtherRcvr', 12, buff)


def make_ether_xmtr() -> tuple[str, int, list[int]]:
    """EtherProms.bcpl EtherXmtr -- 256 x 12, three parts: Eth-h14 (left
    nibble), Eth-h15 (middle), Eth-h16 (right).

    THE TRANSMITTER. Six states in one line, and unlike the receiver it is a
    straight progression rather than a loop:

        idle -> mark -> data -> preCRC -> crc -> postCRC -> idle

    `mark` wire-ORs a single one bit ahead of the data so the far receiver can
    acquire bit phase -- the other half of the degenerate preamble EtherRcvr
    looks for in `idle`. `preCRC` exists for one Dorado cycle purely to
    predecrement the bit counter, so the end of the packet is announced one
    bit time early; the source notes the shift register keeps being shifted
    there but produces zeros, which matters because the CRC output is wire-ORed
    onto it.

    txAbort is checked BEFORE the state dispatch and forces idle from
    anywhere: it is TxCollision % TxDataLate % TxFifoPE % TxOff, i.e. every
    reason to stop mid-packet.

        Input:  currentState 7:5, gotTxBit 4, txStop 3,
                txAbort 2 (*** low true ***), txStart 1,
                txSREmpty 0 (*** low true ***)
        Output: nextState 11:9, txCRCEnbl 8, txCRCClk 7, txGone 6, txGo 5,
                txData 4, txSRCtrl 3:2, spare 1:0
    """
    high, low = 1, 0
    srLoad, srShift, srHold = 0, 2, 3
    idle, mark, data, preCRC, crc, postCRC = 0, 1, 2, 3, 4, 5

    buff = [0] * 256
    for adr in range(256):
        currentState = (adr >> 5) & 7
        gotTxBit = ((adr >> 4) & 1) != 0
        txStop = ((adr >> 3) & 1) != 0         # TxEOP & TxFifoEmpty
        txAbort = ((adr >> 2) & 1) == 0        # *** low true ***
        txStart = ((adr >> 1) & 1) != 0        # (TxFifoFull % TxEOP) & PDCarrier'
        txSREmpty = (adr & 1) == 0             # *** low true ***

        nextState = currentState
        txCRCEnbl = low
        txCRCClk = low
        txGone = txAbort
        txGo = False
        txData = low
        txSRCtrl = srHold

        if txAbort:
            nextState = idle

        elif currentState == idle:
            if txStart:
                nextState = mark
                txGo = True                    # enable the phase encoder

        elif currentState == mark:
            txGo = True
            txData = high
            if gotTxBit:                       # the encoder acked the mark bit
                nextState = data
                txCRCClk = high
                txSRCtrl = srLoad

        elif currentState == data:
            txGo = True
            if gotTxBit:
                txCRCClk = high
                txSRCtrl = srShift
                if txSREmpty:
                    txSRCtrl = srLoad          # last bit of the word
                    if txStop:
                        # Last bit of the last word: shift instead, so the SR
                        # emits zeros under the wire-ORed CRC output.
                        txSRCtrl = srShift
                        nextState = preCRC

        elif currentState == preCRC:
            txGo = True
            txSRCtrl = srShift
            txCRCEnbl = high
            nextState = crc

        elif currentState == crc:
            txGo = True
            txCRCEnbl = high
            if gotTxBit:
                txCRCClk = high
                txSRCtrl = srShift
                if txSREmpty:                  # ack for the next-to-last bit
                    txGo = False
                    nextState = postCRC

        elif currentState == postCRC:
            txCRCEnbl = high
            if gotTxBit:
                # One cycle of txGone on the way to idle clears txEOP and
                # wakes the microcode for the last time.
                txGone = True
                nextState = idle

        else:                                  # unused states go to idle
            nextState = idle

        buff[adr] = (((nextState & 7) << 9) |
                     (txCRCEnbl << 8) |
                     (txCRCClk << 7) |
                     ((high if txGone else low) << 6) |
                     ((high if txGo else low) << 5) |
                     (txData << 4) |
                     ((txSRCtrl & 3) << 2))    # spare bits 1:0 stay zero
    return ('EtherXmtr', 12, buff)


# --- DispY / DispM: the horizontal timing PROMs ------------------------
# These are WAVEFORM GENERATORS: the PROM is addressed by a counter that
# ticks once per four pixels, and its four output bits are the sync, blank
# and half-line signals for that point in the scan line. So the PROM contents
# ARE the raster timing, stated by PARC in its own comments.

def _hrom(total, sync, left, visible, addr_count):
    """Shared body of MakeHRomLF / MakeHRomAlto -- they differ only in the
    line geometry. Bit assignment from the source: notSync #10, HSync 4,
    HBlank 2, HalfLine 1."""
    notSync, HSync, HBlank, HalfLine = 0o10, 4, 2, 1
    buff = [0] * addr_count
    for addr in range(addr_count):
        # "The HRom clock ticks once for every four pixels. Additionally,
        #  HSync starts during the LAST HRom clock of a scan line instead of
        #  the first one; this accounts for the +1 below"
        px = 4 * ((addr + 1) % addr_count)
        c = HSync if (px < sync or px >= total) else notSync
        if px < sync + left or px >= sync + left + visible:
            c |= HBlank
        if total // 2 <= px < total // 2 + sync:
            c |= HalfLine
        buff[addr] = c
    return buff


def make_hrom_lf() -> tuple[str, int, list[int]]:
    """DispProms.bcpl MakeHRomLF -- 512 words, split across LFProm-b20 (low
    256) and LFProm-a16 (high 256).

    The LARGE-FORMAT display's horizontal waveform, and the source states the
    line outright:

        336 bits of sync / 20 blanked left border / 1072 visible /
        12 blanked right border  = 1440 bits total

    plus 360 bits of HalfLine starting 720 bits after the start of sync.

    Worth noting against the emulator: this says 1072 VISIBLE pixels, while
    the C emulator rasterises Cedar at 1024 wide. Not necessarily a
    contradiction -- the extra may be overscan the monitor never showed -- but
    it is the sort of thing worth reconciling if display geometry is ever in
    question."""
    return ('LFProm-Low', 4, _hrom(1440, 336, 20, 1072, 512)[:256])


def make_hrom_lf_high() -> tuple[str, int, list[int]]:
    """The HIGH half of the same 512-word LF waveform, at LFProm-a16.

    The BCPL builds one 512-word buffer and then declares TWO 256-word PROMs
    from it -- `Header("LFProm-Low", ..., buff, 256, 12)` and
    `Header("LFProm-High", ..., buff+256, 256, 12)`. Splitting it here rather
    than emitting a single 512-word blob keeps each generated file matching a
    real part. (The size check found this: a 512-word `LFProm` matched no
    Header in the source.)"""
    return ('LFProm-High', 4, _hrom(1440, 336, 20, 1072, 512)[256:])


def make_hrom_alto() -> tuple[str, int, list[int]]:
    """DispProms.bcpl MakeHRomAlto -- 256 words, AltoProm-b20.

    The Alto-compatible display's waveform: 68 sync / 20 left / 656 visible /
    16 right = 760 total, HalfLine 68 bits from 380.

    The source carries its own warning above this one: "*** Note: this has
    been changed since last tested ***" (Taft, 9-Jun-83). Kept, and repeated
    here, because it is exactly the sort of caveat that should not be lost in
    transcription."""
    return ('AltoProm', 4, _hrom(760, 68, 20, 656, 256))


def make_disprom() -> tuple[str, int, list[int]]:
    """DispProms.bcpl MakeDisProm -- 256 x 4, blown twice as DisPromA-i15 and
    DisPromB-l15.

    A pointer comparator for the display FIFO, the same shape as EtherFifo:
    RP = addr>>4, SP = addr & 17b, Delta = (RP-SP) & 17b, then

        Delta = 1  ->  Pin15 and Pin14
        Delta = 0  ->  Pin12

    Bit positions from DoradoProms.defs:
        structure MCM149[ Pin15 bit; Pin14 bit; Pin12 bit; Pin11 bit; ... ]
    i.e. MSB-first, so Pin15 is bit 3 of the nibble down to Pin11 at bit 0.

    TRANSCRIBED AS WRITTEN, not as named. The manifest in the procedure calls
    the four bits Full/WriteEn/ReadEn/Empty, which would put Empty at Pin11 --
    yet nothing in this version ever sets Pin11, even though the file's own
    change log says "Pier November 10, 1983: Added Empty to Disp PROM". So
    either the names do not line up with the pins in the way the ordering
    suggests, or this copy predates that edit. The code is reproduced
    faithfully rather than reconciled by guesswork."""
    buff = [0] * 256
    for adr in range(256):
        rp = adr >> 4
        sp = adr & 0o17
        delta = (rp - sp) & 0o17
        c = 0
        if delta == 1:
            c |= (1 << 3) | (1 << 2)      # Pin15, Pin14
        if delta == 0:
            c |= (1 << 1)                 # Pin12
        buff[adr] = c
    return ('DisPromA', 4, buff)


# --- DskEth: the disk controller PROMs ---------------------------------

# Bit values from DiskProms.bcpl (octal in the source).
_P1, _P2, _P3, _P4 = 0o200, 0o100, 0o40, 0o20


def _disk_cont(read_flag: bool) -> list[int]:
    """DiskProms.bcpl MakeCont -- the disk controller's SEQUENCER PROGRAM.

    This is the most program-like PROM in the machine: 32 steps, each a
    command to the disk controller, and PARC commented every one. The low
    bits select a parameter out of the controller's RAM register (block word
    counts, tag commands, the various delays); the high bits are the control
    strobes.

        LoadSR/ReadSR = Pin1  shift register load / read
        CompECC       = Pin2  configure the ECC generator
        NewBlock      = Pin3  shift the command register at start of step
        Tag           = Pin4  load the RAM value into the Tag register
                              (otherwise into the count register)

    RAM parameter slots, from the source's own table:
        0-3 Count0..Count3   words per block, four blocks per sector
        4-7 Read/Write/Init/Reset commands (each carries Tag)
        8-9 WrDlyFirst / WrDlyNext    zero-words written before block data
       10-11 RdDlyFirst / RdDlyNext   words to wait before reading
       12-14 Delay3 / Delay2 / Delay1

    The read and write programs have the same shape: initialise, then four
    near-identical per-block sequences, then reset. A sector is four blocks --
    which is the header/label/data structure the emulator's disk code models
    at a higher level."""
    LoadSR = ReadSR = _P1
    CompECC = _P2
    NewBlock = _P3
    Tag = _P4
    Count = [0, 1, 2, 3]
    ReadComm, WriteComm, InitComm, ResetComm = 4 + Tag, 5 + Tag, 6 + Tag, 7 + Tag
    WrDlyFirst, WrDlyNext, RdDlyFirst, RdDlyNext = 8, 9, 10, 11
    Delay3, Delay2, Delay1 = 12, 13, 14

    b = [0] * 32
    if read_flag:
        b[0] = InitComm                       # enable the heads
        b[1] = Delay2                         # wait for disk to settle
        for blk in range(4):
            o = 2 + 7 * blk
            b[o + 0] = RdDlyFirst if blk == 0 else RdDlyNext
            b[o + 1] = ReadComm                       # wait for sync pattern
            b[o + 2] = Count[blk] + CompECC + ReadSR  # the block words
            b[o + 3] = Delay2 + CompECC + ReadSR      # the ECC words
            b[o + 4] = InitComm + ReadSR              # one ECC word, read off
            b[o + 5] = Delay1 + ReadSR                # last ECC word
            b[o + 6] = Delay1 + NewBlock              # next block
        b[30] = ResetComm + NewBlock
        b[31] = ResetComm + NewBlock
    else:
        b[0] = InitComm
        b[1] = Delay2
        for blk in range(4):
            o = 2 + 7 * blk
            b[o + 0] = WriteComm                          # tag with write bit
            b[o + 1] = WrDlyFirst if blk == 0 else WrDlyNext
            b[o + 2] = Delay1 + CompECC + LoadSR          # the sync word
            b[o + 3] = Count[blk] + CompECC + LoadSR      # the block words
            b[o + 4] = Delay1 + CompECC                   # ECC on last word
            b[o + 5] = Delay3                             # shift out ECC
            b[o + 6] = Delay1 + NewBlock
        b[30] = ResetComm + NewBlock
        b[31] = ResetComm + NewBlock
    return b


def make_disk_read() -> tuple[str, int, list[int]]:
    """32 x 8 at Disk-a21 -- the READ sequencer."""
    return ('DiskRead', 8, _disk_cont(True))


def make_disk_write() -> tuple[str, int, list[int]]:
    """32 x 8 at Disk-a20 -- the WRITE sequencer."""
    return ('DiskWrite', 8, _disk_cont(False))


def make_disk_tag() -> tuple[str, int, list[int]]:
    """DiskProms.bcpl MakeTagTiming -- 32 x 4, Disk-d21.

    Tag-line timing: the strobe is asserted for counts 3..13, the counter is
    turned off at count 12 when active, and done is signalled at 15."""
    TagStrobe, TagDone, CntOff = 4, 2, 1
    b = [0] * 32
    for adr in range(32):
        active = adr & 0o20
        cnt = adr & 0o17
        v = 0
        if 3 <= cnt <= 13:
            v = TagStrobe
        if cnt == 12 and active:
            v |= CntOff
        if cnt == 15:
            v = TagDone
        b[adr] = v
    return ('DiskTag', 4, b)


def make_disk_fifo() -> tuple[str, int, list[int]]:
    """DiskProms.bcpl MakeFifoPointers -- 256 x 4, Disk-b14.

    The disk FIFO's pointer comparator. Same idea as EtherFifo but with more
    hysteresis: ReadEn once at least 2 words are buffered, WriteEn while
    fewer than 12 are, and the two enables are INVERTED on the way out.

    The source carries its own repair note on the Full case: "14B. change to
    accommodate lost fix"."""
    Full, WriteEn, ReadEn, Empty = 0o10, 4, 2, 1
    invert = WriteEn | ReadEn
    b = [0] * 256
    for adr in range(256):
        w = adr >> 4
        r = adr & 0o17
        delta = (w - r) & 0o17
        v = 0
        if delta == 0:
            v = Empty
        if delta == 15:
            v = Full
        if delta >= 2:
            v |= ReadEn
        if delta < 12:
            v |= WriteEn
        b[adr] = v ^ invert
    return ('DiskFifo', 4, b)


def make_disk_units() -> tuple[str, int, list[int]]:
    """DiskProms.bcpl MakeDriveSelect -- 32 x 8, Disk-d05.

    Drive select: a 4-bit drive number plus a select bit in, a unary select
    line per drive plus the binary drive number out. Note the clamp the
    source spells out -- "drive numbers greater than 3 address drive 3"."""
    b = [0] * 32
    for adr in range(32):
        select = (adr >> 4) & 1
        drive = adr & 0xF
        out = min(drive, 3) & 3          # driveNum, binary, bits 3:2
        v = out << 2
        if select:
            v |= 1 << (7 - min(drive, 3))   # select0..3, unary, bits 7:4
        b[adr] = v
    return ('DiskUnits', 8, b)


GENERATORS = {
    'LMASK': make_lmask,
    'RMASK': make_rmask,
    'Keyboard-Map': make_keyboard_map,
    'Mouse-Motion': make_mouse_motion,
    'Data-Select': make_data_select,
    'ST': make_st,
    'EC-1': make_ec1,
    'EC-2': make_ec2,
    'Map-Mem': make_map_mem,
    'Map-Map': make_map_map,
    '16k-Mem': make_mem16,
    '4k-Mem': make_mem4,
    'EtherFifo': make_ether_fifo,
    'EtherPD': make_ether_pd,
    'EtherRcvr': make_ether_rcvr,
    'EtherXmtr': make_ether_xmtr,
    'LFProm-Low': make_hrom_lf,
    'LFProm-High': make_hrom_lf_high,
    'AltoProm': make_hrom_alto,
    'DisPromA': make_disprom,
    'DisPromB': lambda: ('DisPromB',) + make_disprom()[1:],
    'DiskRead': make_disk_read,
    'DiskWrite': make_disk_write,
    'DiskTag': make_disk_tag,
    'DiskFifo': make_disk_fifo,
    'DiskUnits': make_disk_units,
}


# --- the map, read from the BCPL rather than retyped -------------------

def prom_map() -> list[dict]:
    """Every PROM the sources define, with where it is blown.

    `Header("NAME",width,buff,depth,...)` declares a PROM; the following

    The buffer argument is an EXPRESSION, not always a bare name --
    `Header("LFProm-High",4,buff+256,256,12)` takes the high half of a
    512-word buffer. An earlier version of this pattern required an
    identifier there and silently missed that PROM.
    `PromCommand("Board-loc")` calls say which package(s) receive it -- more
    than one when a 16-bit PROM is split across a high and a low byte."""
    out = []
    for f in sorted(os.listdir(SRC)):
        if not f.endswith('.bcpl'):
            continue
        text = open(os.path.join(SRC, f), 'rb').read().decode('latin-1')
        text = text.replace('\r\n', '\n').replace('\r', '\n')
        # walk in order so each Header picks up the PromCommands after it
        events = []
        for m in re.finditer(r'Header\("([^"]+)"\s*,\s*(\d+)\s*,\s*[^,]+,\s*(\d+)', text):
            events.append((m.start(), 'header', m.group(1), int(m.group(2)),
                           int(m.group(3))))
        for m in re.finditer(r'PromCommand\("([^"]+)"', text):
            events.append((m.start(), 'cmd', m.group(1), None, None))
        events.sort()
        cur = None
        for _pos, kind, a, b, c in events:
            if kind == 'header':
                cur = {'file': f, 'name': a, 'width': b, 'depth': c,
                       'locations': []}
                out.append(cur)
            elif cur is not None:
                cur['locations'].append(a)
    return out


# --- where each PROM is blown ------------------------------------------
#
# `PromCommand("MemX-h11")` labels the blown part with a board and a package
# position. The board halves of those labels are PARC's own shorthand, not the
# Sil directory names, so they need translating exactly once -- here.
_PLACEMENT_PREFIX = {
    'Disk': 'DskEth', 'Eth': 'DskEth',      # one board carries both controllers
    'IFU': 'IFU',
    'MemX': 'MemX', 'MX4k': 'MemX', 'MX16k': 'MemX',
    'PrH': 'ProcH', 'PrL': 'ProcL',
    'DisPromA': 'DispY', 'DisPromB': 'DispY',
    # The display TIMING proms are not on any of the sixteen boards: LFProm
    # and AltoProm are the large-format and Alto-compatible raster generators,
    # and they sit on a small board of their own that the archive's Sil tree
    # does not include.
    'LFProm': None, 'AltoProm': None,
}


def placement() -> list[dict]:
    """Every PROM package: which board, which position, which bits.

    A PROM wider than its part is split across several packages, and the
    source states the order: `PromCommand("Eth-h09")` is "the left nibble",
    then h10 with an argument of `4` and h11 with `8` -- the shift applied to
    the left-justified word. So the FIRST location holds the most significant
    slice, and each takes width/len(locations) bits. Same for the 16-bit
    PROMs: `ST` is "MemX-h11 (left half) + i12 (right)", and LMASK's halves
    land on ProcH and ProcL, which is the machine's own high-byte/low-byte
    split.

    Two things this deliberately does NOT do. It does not guess at the three
    display-timing PROMs, whose board is not in the archive. And it does not
    force `Mouse-Motion` or `Keyboard-Map` onto the IFU: the source blows them
    at IFU-i03/k05/l05, but IFU Rev Ch has ordinary logic there. By the 1981
    manual the keyboard and mouse are decoded by the terminal microcomputer
    (Table 24), which serialises them to 177034B, so those PROMs belong to an
    earlier IFU. Reported as unplaced rather than placed wrongly.
    """
    rows = []
    for r in prom_map():
        locs = r['locations']
        if not locs:
            continue
        slice_w = r['width'] // len(locs)
        for i, loc in enumerate(locs):
            pre, _, pos = loc.rpartition('-')
            board = _PLACEMENT_PREFIX.get(pre, '?')
            hi = r['width'] - i * slice_w - 1
            rows.append({'prom': r['name'], 'label': loc, 'board': board,
                         'pos': pos, 'width': r['width'], 'depth': r['depth'],
                         'slice_hi': hi, 'slice_lo': hi - slice_w + 1,
                         'slice_w': slice_w})
    return rows


# The three parts PARC blows: a 32x8 ECL PROM, a 256x4 ECL PROM, and one
# 32x8 TTL PROM for the disk drive-select.
PROM_PARTS = ('SG10139', 'MCM10149', 'SN74S288')


def _package_types() -> dict:
    """(board, position) -> part type, from the Sil part lists."""
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from sil_netlist import load_board, find_boards
    out = {}
    for wl, lc in find_boards(os.path.join(SRC, '..', 'sil')):
        short = os.path.basename(os.path.dirname(wl)).split('-Rev')[0] \
            .split('.dm')[0]
        b = load_board(wl, lc)
        for pos, d in b.packages.items():
            out[(short, pos)] = d.get('type', '')
    return out


def emit_packages(outdir: str) -> int:
    """One .mem per PROM PACKAGE, holding just that package's bits.

    The alternative -- one file per PROM plus slicing in the Verilog -- would
    put the bit order in two places. The part only ever holds its own slice,
    so the slice is done here, once, where the ordering is already stated.

    Bit order within a package: the word is MSB-first over Q0..Qn. Both
    sources agree -- `DiskProms.bcpl` has `Pin1 = #200`, i.e. bit 7 of a byte,
    and Pin1 is Q0; `DoradoProms.defs` declares `MCM149[Pin15; Pin14; Pin12;
    Pin11]` and Pin15 is Q0. So Q0 is the most significant output.
    """
    os.makedirs(outdir, exist_ok=True)
    parts = _package_types()
    n, skipped = 0, []
    for row in placement():
        if row['board'] in (None, '?') or row['prom'] not in GENERATORS:
            skipped.append(row)
            continue
        # Never write contents for a package that is not a PROM. The label is
        # PARC's, the part list is the board's, and where they disagree the
        # board wins -- IFU Rev Ch has ordinary logic where the keyboard and
        # mouse PROMs used to be.
        if parts.get((row['board'], row['pos'])) not in PROM_PARTS:
            skipped.append(row)
            continue
        _name, _w, words = GENERATORS[row['prom']]()
        lo, w = row['slice_lo'], row['slice_w']
        digits = (w + 3) // 4
        path = os.path.join(outdir, f'{row["board"]}-{row["pos"]}.mem')
        with open(path, 'w') as fh:
            fh.write(f'// {row["board"]} {row["pos"]}: {row["prom"]} '
                     f'bits [{row["slice_hi"]}:{lo}] of {row["width"]}, '
                     f'{row["depth"]} words\n')
            fh.write('// Generated by tools/dorado_proms.py --emit-packages '
                     'from PARC\'s\n// DoradoProms BCPL source -- do not edit, '
                     'regenerate.\n')
            for word in words:
                fh.write(f'{(word >> lo) & ((1 << w) - 1):0{digits}X}\n')
        n += 1
    print(f'wrote {n} package images into {outdir}')
    for row in skipped:
        where = (f'{row["board"]} {row["pos"]}'
                 if row['board'] not in (None, '?') else 'board not in chm/sil')
        print(f'  skipped {row["prom"]:<13} {row["label"]:<14} ({where})')
    return 0


def emit_mem(name: str, width: int, words: list[int], outdir: str) -> str:
    """One hex word per line -- what $readmemh wants, and what the MiSTer
    flow uses for ROM initialisation."""
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, f'{name}.mem')
    digits = (width + 3) // 4
    with open(path, 'w') as fh:
        fh.write(f'// {name}: {len(words)} x {width} bits\n')
        fh.write(f'// Generated by tools/dorado_proms.py from PARC\'s\n')
        fh.write(f'// DoradoProms BCPL source -- do not edit, regenerate.\n')
        for w in words:
            fh.write(f'{w:0{digits}X}\n')
    return path


def check_against_emulator() -> int:
    """The masks the C emulator uses are built the same way; compare.

    This is a value check, not a code check: it asserts that the PROM the
    hardware actually holds agrees with what the emulator computes. A
    disagreement would mean one of them has the edge case wrong -- the
    interesting one being entry 0, which the BCPL comment calls out ("address
    0 should be zero")."""
    _n, _w, lmask = make_lmask()
    _n, _w, rmask = make_rmask()
    bad = 0
    # LMASK[n] must be the top n bits; RMASK[n] the low n.
    for n in range(0, 17):
        want_l = ((0xFFFF << (16 - n)) & 0xFFFF) if n else 0
        want_r = ((1 << n) - 1) & 0xFFFF if n else 0
        if lmask[n] != want_l:
            print(f'  LMASK[{n}] = {lmask[n]:04X}, expected {want_l:04X}')
            bad += 1
        if rmask[n] != want_r:
            print(f'  RMASK[{n}] = {rmask[n]:04X}, expected {want_r:04X}')
            bad += 1
    print(f'mask cross-check: {"OK" if not bad else str(bad)+" MISMATCHES"} '
          f'(entries 0..16 against the closed form)')
    print(f'  LMASK[0..4] = {" ".join(f"{lmask[i]:04X}" for i in range(5))}')
    print(f'  RMASK[0..4] = {" ".join(f"{rmask[i]:04X}" for i in range(5))}')
    print(f'  entries 16..31 saturate at FFFF, which is the 32-entry part '
          f'holding a 16-bit mask')

    # Keyboard-Map: the BCPL INVERTS a 64-entry table, so the low half must
    # come out a permutation of 0..63. If the inversion or either XOR were
    # mis-transcribed, that property would break -- it is a bijection check,
    # which is much stronger than eyeballing a few entries.
    _n, _w, kb = make_keyboard_map()
    if sorted(kb[:64]) != list(range(64)):
        print('  Keyboard-Map low half is NOT a permutation of 0..63'); bad += 1
    if kb[64:] != [((v - 64) ^ 0o60) ^ 0o77 for v in range(64, 128)]:
        print('  Keyboard-Map high half does not match its formula'); bad += 1
    print(f'keyboard map: low half is a permutation of 0..63, high half '
          f'matches (val-64)^60^77')

    # Mouse-Motion: value = 3*Ydir + Xdir over {0,1,2} each, so 0..8 and no
    # more. And exactly 2**4 = 16 addresses can mean "no motion" -- the ones
    # where all four phase signals equal their delayed copies.
    _n, _w, mm = make_mouse_motion()
    if set(mm) - set(range(9)):
        print(f'  Mouse-Motion has values outside 0..8: '
              f'{sorted(set(mm) - set(range(9)))}'); bad += 1
    zeros = sum(1 for v in mm if v == 0)
    if zeros != 16:
        print(f'  Mouse-Motion has {zeros} no-motion entries, expected 16'); bad += 1
    print(f'mouse motion: values 0..8 only, and exactly 16 no-motion entries '
          f'(= the 2^4 states where nothing changed)')

    # EtherFifo: full and empty are mutually exclusive, each complement pair
    # must actually be complementary, and over 256 pointer combinations there
    # must be exactly 16 full states and 16 empty ones -- one per write
    # pointer value. If a bit position were transposed, these break.
    _n, _w, ff = make_ether_fifo()
    fulls = sum(1 for v in ff if (v >> 3) & 1)
    empties = sum(1 for v in ff if (v >> 1) & 1)
    badpair = sum(1 for v in ff
                  if ((v >> 3) & 1) == ((v >> 2) & 1) or
                     ((v >> 1) & 1) == (v & 1))
    both = sum(1 for v in ff if ((v >> 3) & 1) and ((v >> 1) & 1))
    if fulls != 16 or empties != 16 or badpair or both:
        print(f'  EtherFifo: full={fulls} empty={empties} '
              f'non-complementary={badpair} both-at-once={both}')
        bad += 1
    print(f'ether fifo: 16 full and 16 empty states of 256, complements '
          f'consistent, never both at once')

    # EtherPD: the phase decoder must reach every one of its four events, and
    # must report noEvent whenever there is no carrier and no transition --
    # the quiescent line.
    _n, _w, pd = make_ether_pd()
    events = {(v >> 1) & 3 for v in pd}
    quiet_bad = 0
    for adr, v in enumerate(pd):
        pdC = (adr >> 7) & 1
        nd, od = (adr >> 6) & 1, (adr >> 5) & 1
        if not pdC and nd == od and ((v >> 1) & 3) != 0:
            quiet_bad += 1
    if events != {0, 1, 2, 3} or quiet_bad:
        print(f'  EtherPD: events seen {sorted(events)}, '
              f'{quiet_bad} quiescent addresses reporting an event')
        bad += 1
    print(f'ether phase decoder: all four events reachable '
          f'(noEvent/collision/dataZero/dataOne), quiet line reports noEvent')

    # EtherRcvr and EtherXmtr are STATE MACHINES, so the checks are the ones a
    # state machine deserves: every state reachable, no input leaves the
    # machine in an undefined state, no illegal shift-register encoding, and
    # the specific transitions the source's comments describe. These are much
    # stronger than eyeballing a transcription -- a misplaced field or a
    # dropped inversion breaks at least one of them.
    _n, _w, rcv = make_ether_rcvr()
    idle, maybe, full, imip = 0, 1, 2, 3
    srLoad, srShift, srHold = 0, 2, 3
    R_STATES = {idle: 'idle', maybe: 'maybe', full: 'full', imip: 'imip'}
    r_next = lambda v: (v >> 9) & 7
    r_srctrl = lambda v: v & 3
    r_reached, r_bad = set(), []
    for adr, v in enumerate(rcv):
        st, coll_in, carrier = (adr >> 5) & 7, (adr >> 4) & 1, (adr >> 3) & 1
        ev = (adr >> 1) & 3
        srfull = (adr & 1) == 0                # *** low true ***
        databit = carrier and ev in (2, 3)     # dataZero or dataOne
        nxt = r_next(v)
        if nxt not in R_STATES:
            r_bad.append(f'state {st} input {adr:02X} -> undefined state {nxt}')
        if r_srctrl(v) == 1:
            r_bad.append(f'{adr:02X}: rxSRCtrl = 01, not a legal srCtrl value')
        if st in R_STATES:
            r_reached.add(nxt)
        else:                                  # unused states go to idle
            if nxt != idle or r_srctrl(v) != srLoad:
                r_bad.append(f'unused state {st} does not reset to idle+srLoad')
        # rxCRCClk (bit 3) and rxData (bit 2) may only move on a real data bit
        if (v >> 3) & 1 and not databit:
            r_bad.append(f'{adr:02X}: rxCRCClk without a data event')
        if (v >> 2) & 1 and ev != 3:
            r_bad.append(f'{adr:02X}: rxData high without dataOne')
        # rxCRCReset (bit 4) is held exactly in idle, released for the mark bit
        want_reset = 1 if (st == idle and not (carrier and ev == 3)) else 0
        if ((v >> 4) & 1) != want_reset:
            r_bad.append(f'{adr:02X}: rxCRCReset {(v>>4)&1}, expected {want_reset}')
        # rxSync is LOW TRUE: the idle line must read it DEASSERTED (= 1).
        # Dropping the inversion asserts sync on every quiescent address.
        if st == idle and not carrier and ((v >> 6) & 1) != 1:
            r_bad.append(f'{adr:02X}: rxSync asserted on an idle line')
        # The three srCtrl ENCODINGS, each pinned by the situation that uses
        # it: shift while taking a bit, load on the way back to idle (below),
        # hold on a quiescent line. Two of these are the same value with the
        # bits transposed, so checking only for the illegal 01 misses a swap.
        if st in (maybe, full, imip) and databit and r_srctrl(v) != srShift:
            r_bad.append(f'{adr:02X}: taking a data bit without srShift')
        if st == idle and not carrier and r_srctrl(v) != srHold:
            r_bad.append(f'{adr:02X}: idle line is not holding the SR')
        # The collision output. In idle the source CLEARS it and re-derives it
        # from this event: a collision or a dataZero where the mark bit should
        # be means the phase decoder missed the start of the packet. (BCPL
        # switchon falls through, so `case collision:` runs dataZero's body --
        # lose that and a real collision goes unreported.) In the other three
        # states it is the input latched, set by a collision event. The unused
        # states run the default arm, which touches neither.
        if st == idle:
            want_coll = 1 if (carrier and ev in (1, 2)) else 0
        elif st in R_STATES:
            want_coll = 1 if (coll_in or (carrier and ev == 1)) else 0
        else:
            want_coll = coll_in
        if ((v >> 8) & 1) != want_coll:
            r_bad.append(f'{adr:02X}: rxCollision {(v>>8)&1} in '
                         f'{R_STATES.get(st, st)}, expected {want_coll}')
        # Carrier drop: normal end of packet from full, damaged from imip,
        # and from maybe only reported at all if a collision was already seen.
        if st in (full, imip) and not carrier:
            inc = (v >> 5) & 1
            if nxt != idle or r_srctrl(v) != srLoad or not ((v >> 7) & 1):
                r_bad.append(f'{adr:02X}: carrier drop did not end the packet')
            if inc != (1 if st == imip else 0):
                r_bad.append(f'{adr:02X}: rxIncTrans {inc} on drop from '
                             f'{R_STATES[st]}')
        if st == maybe and not carrier:
            if nxt != idle or r_srctrl(v) != srLoad:
                r_bad.append(f'{adr:02X}: runt did not return to idle')
            if ((v >> 7) & 1) != coll_in:
                r_bad.append(f'{adr:02X}: runt reported EOP without a collision')
        # The transitions themselves, stated from the comments. These are what
        # pin rxSRFull's polarity: the word only goes to the Fifo when the
        # shift register says it is full, and that input is LOW TRUE.
        want = None
        if st == idle:
            want = maybe if carrier else idle
        elif st == maybe:
            want = idle if not carrier else (full if (databit and srfull)
                                             else maybe)
        elif st == full:
            want = idle if not carrier else (imip if databit else full)
        elif st == imip:
            want = idle if not carrier else (full if (databit and srfull)
                                             else imip)
        if want is not None and nxt != want:
            r_bad.append(f'{adr:02X}: {R_STATES[st]} -> {nxt}, expected {want} '
                         f'(carrier {carrier}, event {ev}, SRfull {srfull:d})')
    if r_reached != set(R_STATES):
        r_bad.append(f'unreachable states: '
                     f'{sorted(set(R_STATES) - r_reached)}')
    if r_bad:
        for line in r_bad[:8]:
            print(f'  EtherRcvr: {line}')
        if len(r_bad) > 8:
            print(f'  EtherRcvr: ... and {len(r_bad)-8} more')
        bad += 1
    print(f'ether receiver: all four states reachable, every input has a '
          f'defined next state, carrier drop ends the packet '
          f'(rxIncTrans only from imip/maybe), rxSync deasserted when idle')

    _n, _w, xmt = make_ether_xmtr()
    xidle, xmark, xdata, xpre, xcrc, xpost = 0, 1, 2, 3, 4, 5
    X_STATES = {xidle: 'idle', xmark: 'mark', xdata: 'data', xpre: 'preCRC',
                xcrc: 'crc', xpost: 'postCRC'}
    X_SUCC = {xidle: xmark, xmark: xdata, xdata: xpre, xpre: xcrc,
              xcrc: xpost, xpost: xidle}       # the one-way progression
    x_next = lambda v: (v >> 9) & 7
    x_srctrl = lambda v: (v >> 2) & 3
    x_reached, x_bad = set(), []
    for adr, v in enumerate(xmt):
        st = (adr >> 5) & 7
        got, stop = (adr >> 4) & 1, (adr >> 3) & 1
        abort = ((adr >> 2) & 1) == 0          # *** low true ***
        start = (adr >> 1) & 1
        srempty = (adr & 1) == 0               # *** low true ***
        nxt = x_next(v)
        if nxt not in X_STATES:
            x_bad.append(f'state {st} input {adr:02X} -> undefined state {nxt}')
        if x_srctrl(v) == 1:
            x_bad.append(f'{adr:02X}: txSRCtrl = 01, not a legal srCtrl value')
        if v & 3:
            x_bad.append(f'{adr:02X}: spare bits are not zero')
        if abort:
            # Abort is tested before the state dispatch, so it forces idle
            # from anywhere and reports txGone (bit 6) in the same cycle.
            if nxt != xidle or not ((v >> 6) & 1):
                x_bad.append(f'{adr:02X}: abort did not force idle+txGone')
            continue
        if st in X_STATES:
            x_reached.add(nxt)
            if nxt not in (st, X_SUCC[st]):
                x_bad.append(f'{X_STATES[st]} -> {nxt}, not itself or '
                             f'{X_STATES[X_SUCC[st]]}')
        elif nxt != xidle:
            x_bad.append(f'unused state {st} does not reset to idle')
        # Each advance, with the condition the source gives for it. These pin
        # txSREmpty's polarity (low true): the packet only moves on to the CRC
        # when the shift register has run out, and only if txStop says that
        # was the last word.
        want = None
        if st == xidle:
            want = xmark if start else xidle
        elif st == xmark:
            want = xdata if got else xmark
        elif st == xdata:
            want = xpre if (got and srempty and stop) else xdata
        elif st == xpre:
            want = xcrc                        # exactly one cycle
        elif st == xcrc:
            want = xpost if (got and srempty) else xcrc
        elif st == xpost:
            want = xidle if got else xpost
        if want is not None and nxt != want:
            x_bad.append(f'{adr:02X}: {X_STATES[st]} -> {nxt}, expected {want} '
                         f'(ack {got}, stop {stop}, SRempty {srempty:d})')
        # The three srCtrl encodings again, each pinned where it is used: load
        # a fresh word once the mark bit is acked, shift through preCRC so the
        # SR emits zeros under the wire-ORed CRC, hold while idle.
        if st == xmark and got and x_srctrl(v) != srLoad:
            x_bad.append(f'{adr:02X}: mark bit acked without srLoad')
        if st == xpre and x_srctrl(v) != srShift:
            x_bad.append(f'{adr:02X}: preCRC is not shifting the SR')
        if st == xidle and not start and x_srctrl(v) != srHold:
            x_bad.append(f'{adr:02X}: idle transmitter is not holding the SR')
        # txGo (bit 5) drives the phase encoder: on through mark/data/preCRC,
        # and in crc until the next-to-last bit is acked.
        if st in (xmark, xdata, xpre) and not ((v >> 5) & 1):
            x_bad.append(f'{adr:02X}: txGo down in {X_STATES[st]}')
        if st == xidle and not start and ((v >> 5) & 1):
            x_bad.append(f'{adr:02X}: txGo up in idle without txStart')
    if x_reached != set(X_STATES):
        x_bad.append(f'unreachable states: '
                     f'{sorted(set(X_STATES) - x_reached)}')
    if x_bad:
        for line in x_bad[:8]:
            print(f'  EtherXmtr: {line}')
        if len(x_bad) > 8:
            print(f'  EtherXmtr: ... and {len(x_bad)-8} more')
        bad += 1
    print(f'ether transmitter: all six states reachable, the only transitions '
          f'are self or successor, txAbort forces idle+txGone from any state')

    # Disk sequencers: both programs must have the same SHAPE -- four
    # per-block sequences of seven steps between a two-step init and a
    # two-step reset -- because a sector is four blocks. If a step were
    # dropped or duplicated in transcription the block stride would break.
    _n, _w, dr = make_disk_read()
    _n, _w, dw = make_disk_write()
    shape_ok = True
    for prog in (dr, dw):
        if len(prog) != 32:
            shape_ok = False
        # the last step of each block issues NewBlock (Pin3)
        for blk in range(4):
            if not (prog[2 + 7 * blk + 6] & _P3):
                shape_ok = False
        if not (prog[30] & _P3) or not (prog[31] & _P3):
            shape_ok = False
    if not shape_ok:
        print('  disk sequencers: block stride or NewBlock placement wrong')
        bad += 1
    print(f'disk sequencers: read and write both 32 steps, NewBlock at the end '
          f'of each of four blocks -- a sector is four blocks')

    # Every generator must match the depth and width the BCPL's own Header()
    # declares. This catches a truncated or over-long table transcription,
    # which is the likeliest way a literal-table port goes wrong.
    m = {r['name']: r for r in prom_map()}
    sized = 0
    for name, fn in GENERATORS.items():
        _n, _w, words = fn()
        r = m.get(name)
        if not r:
            print(f'  {name}: no Header() in the BCPL'); bad += 1; continue
        if len(words) != r['depth']:
            print(f'  {name}: {len(words)} words, Header declares {r["depth"]}')
            bad += 1
        elif max(words) >= (1 << r['width']):
            print(f'  {name}: value {max(words):o}o exceeds {r["width"]} bits')
            bad += 1
        else:
            sized += 1
    print(f'size check: {sized}/{len(GENERATORS)} generators match the depth '
          f'and width their own Header() declares')
    print(f'ported: {len(GENERATORS)} of {len(m)} PROMs')
    return bad


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--emit', metavar='NAME')
    ap.add_argument('--emit-all', action='store_true')
    ap.add_argument('--out', default='verilog/proms')
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--placement', action='store_true',
                    help='which board and package holds each PROM')
    ap.add_argument('--emit-packages', metavar='DIR',
                    help='one .mem per PROM package, sliced for that part')
    args = ap.parse_args(argv[1:])

    if args.list:
        rows = prom_map()
        print(f'{"PROM":<14} {"file":<18} {"size":>10}   locations')
        for r in rows:
            print(f'{r["name"]:<14} {r["file"]:<18} '
                  f'{r["depth"]:>4} x {r["width"]:<3} '
                  f'{", ".join(r["locations"])}')
        print(f'\n{len(rows)} PROMs defined across {len(set(r["file"] for r in rows))} files.')
        have = sum(1 for r in rows if r['name'] in GENERATORS)
        print(f'{have} have a ported generator; the rest are transcribed as needed.')
        return 0

    if args.placement:
        rows = placement()
        parts = _package_types()
        print(f'{"PROM":<14}{"label":<15}{"board":<8}{"pos":<5}{"bits":>9}'
              f'{"depth":>7}   part')
        for r in rows:
            b = r['board'] if r['board'] not in (None, '?') else '--'
            bits = f'[{r["slice_hi"]}:{r["slice_lo"]}]'
            part = parts.get((r['board'], r['pos']), '')
            note = '' if part in PROM_PARTS else '   <- not a PROM package'
            print(f'{r["prom"]:<14}{r["label"]:<15}{b:<8}{r["pos"]:<5}'
                  f'{bits:>9}{r["depth"]:>7}   {part or "?"}{note}')
        good = sum(1 for r in rows
                   if parts.get((r['board'], r['pos'])) in PROM_PARTS)
        print(f'\n{good} of {len(rows)} PROM packages land on a real PROM part '
              f'in chm/sil.')
        print('The rest are the three display-timing PROMs, whose board is not '
              'in the archive, and')
        print('the IFU keyboard/mouse PROMs, which IFU Rev Ch no longer has -- '
              'see placement().')
        return 0

    if args.emit_packages:
        return emit_packages(args.emit_packages)

    if args.check:
        return 1 if check_against_emulator() else 0

    names = list(GENERATORS) if args.emit_all else ([args.emit] if args.emit else [])
    if not names:
        ap.print_help()
        return 1
    for n in names:
        if n not in GENERATORS:
            print(f'no ported generator for {n!r}; '
                  f'available: {", ".join(GENERATORS)}', file=sys.stderr)
            return 1
        name, width, words = GENERATORS[n]()
        print('wrote', emit_mem(name, width, words, args.out))
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
