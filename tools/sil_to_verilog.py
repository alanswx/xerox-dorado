#!/usr/bin/env python3
"""Generate structural Verilog for a Dorado board from PARC's own Sil output.

Inputs, all of them original Xerox data:
  <board>.wl          the wire list  (nets, package pins, directions)
  <board>.lc          the part list  (part type per package position)
  EclDict.Analyze     the ANALYZE part dictionary (pin roles per part)

Output: one Verilog module per board, instantiating a cell per package and
wiring them with the board's own net names. The cell MODELS live in
`verilog/cells/` and are hand-written per part number; this tool never invents
behaviour, it only places and wires what the netlist says.

Why this shape (see docs/verilog-from-sil.md): the `.wl` gives the direction
of every pin, so the generator can emit real `input`/`output` ports rather
than guessing, and the board's own net names survive into the RTL -- which is
what makes the result diffable against the schematics and against the C
emulator's behaviour.

THREE THINGS THE GENERATOR WILL NOT DO SILENTLY, because each would produce
plausible-looking wrong RTL:

  * **Wired-OR.** MECL 10K open-emitter outputs are legitimately tied
    together (91 such nets on ProcH alone). Verilog `wire` with multiple
    drivers is a multi-driver error, so those nets are emitted as an explicit
    OR of their drivers, and every one is listed in the report.
  * **Missing cells.** A package whose part has no model is emitted as a
    named stub instance with its real ports, and counted. The design does not
    quietly lose logic.
  * **Undriven nets.** A net that does NOT reach a backplane connector and
    has no driver here is left undriven, so simulation reads X rather than a
    plausible zero. It means a cell model is still a skeleton, and the count
    is in the report. It is NOT quietly promoted to a module port -- the port
    list is stated by `<board>.bp`, not inferred from what happens to be
    driven.

Usage:
    sil_to_verilog.py <board.wl> --dict EclDict.Analyze [-o out.v] [--report]
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sil_netlist import load_board, vpart   # noqa: E402
from sil_ecldict import EclDict             # noqa: E402
from dorado_proms import placement, PROM_PARTS   # noqa: E402
import firmware_eproms                          # noqa: E402


# Sil punctuation -> escape. INJECTIVE on purpose: `CTask.0` and `CTask=0`
# are DIFFERENT nets on ContA, and a lossy mapping silently merged them into
# one wire. Verilog caught that as a duplicate declaration, but only by luck --
# had the two names not both been declared, the board would have elaborated
# with two signals shorted together and no diagnostic at all.
#
# `_` is escaped first so no escape sequence can be produced another way; the
# rest stay readable, because being able to grep the RTL for the schematic's
# own signal name is most of the value of generating it this way.
_ESCAPES = [('_', '_u_'), ('.', '_'), ("'", '_p_'), ('=', '_eq_'),
            ('-', '_m_'), ('#', '_h_'), ('/', '_s_'), ('+', '_pl_')]

# Where the per-package PROM images live, repo-relative.
PROM_DIR = 'verilog/proms/packages'
EPROM_DIR = 'verilog/proms/eprom'


def vname(name: str) -> str:
    """A Sil net or package name -> a legal, UNIQUE Verilog identifier."""
    s = name
    for ch, esc in _ESCAPES:
        s = s.replace(ch, esc)
    s = re.sub(r'[^A-Za-z0-9_]', lambda m: '_x%02x_' % ord(m.group(0)), s)
    if not s or s[0].isdigit():
        s = 'n_' + s
    return s


class Generator:
    def __init__(self, board, ecl: EclDict, cells: set[str],
                 cell_dirs: dict | None = None,
                 clocked: set[str] | None = None):
        self.b = board
        self.ecl = ecl
        self.cells = cells
        self.cell_dirs = cell_dirs or {}
        self.clocked = clocked or set()
        self.clocked_placed = 0
        self.missing: Counter = Counter()
        self.wired_or: list[tuple[str, int]] = []
        self.ports: dict[str, str] = {}      # net -> input|output|inout
        self.undriven_internal: list[str] = []
        # Backplane nets this board DRIVES: each becomes a `<net>__drv`
        # output carrying only this board's contribution.
        self.exports: set[str] = set()
        self.unknown_pins: Counter = Counter()
        # (package, pin) -> the level a resistor pack holds that pin at.
        # Computed once, before anything asks a pin which way it faces.
        self._sips = self.sip_drives()
        # (package, pin) -> the net a wire-wrap jumper brings to that pin.
        self._straps = self.jumper_straps()
        # (package, pin) -> the net a series resistor brings to that pin.
        self._series = self.series_pass()
        # package position -> the .mem holding that PROM's contents. PARC's
        # own PromCommand labels say which package each PROM is blown into;
        # tools/dorado_proms.py resolves them and slices the word per part.
        short = board.name.split('-Rev')[0].split('.')[0]
        self.prom_image = {
            r['pos']: f'{PROM_DIR}/{r["board"]}-{r["pos"]}.mem'
            for r in placement() if r['board'] == short
        }
        # The BaseBoard's 2716 sockets, filled from the base ROM the C
        # emulator boots -- see tools/firmware_eproms.py for how each socket's
        # address was established.
        if short == firmware_eproms.BOARD:
            self.prom_image.update(
                {sock: f'{EPROM_DIR}/{mem}'
                 for sock, mem in firmware_eproms.images().items()})
        self.proms_wired = 0

    # A part whose drive OVERRIDES the wired-OR on its nets.
    #
    # MECL outputs are open emitters: they can pull a net UP, and the OR that
    # resolves a wired-OR net is exactly right for that. `MPQ3303` is not a
    # gate -- it is the transistor quad of the BaseBoard's VCO, and its job on
    # `VCOPhase0`/`VCOPhase1` is to pull them DOWN. An OR cannot express a
    # pull-down, so resolving that loop as an OR latches it high and the
    # machine's clock never starts.
    #
    # Since the VCO is already a documented SUBSTITUTION (an analog oscillator
    # has no digital model -- see cell_MPQ3303), the substitute drives those
    # nets outright. One part, one net pair, stated here rather than hidden in
    # a cell.
    OVERRIDE_DRIVERS = ('MPQ3303',)

    # ...and the opposite: a driver that LOSES to any other on its net.
    #
    # A 6532's port pin is high-Z with an internal pull-up when its DDR makes
    # it an input, and the MiSTer core states that convention directly --
    # `PA_out = out_a | ~dir_a`, an input pin reading back 1, with the comment
    # that the output "must be fed back to input ... for the chip to read
    # properly". That is a WIRE-AND convention. These nets are resolved as
    # wired-OR, so the pull-up would win instead of losing, pinning the net
    # high and hiding whatever really drives it.
    #
    # The DDR is runtime state, so the generator cannot ask it. It does not
    # need to: across the whole machine there are 33 nets where a 6532 port pin
    # shares with another driver, and in EVERY one the other driver is a real
    # totem-pole part ('174, '259, '01, '157, '175, '74, MC10125) or a strap,
    # and the 6532 pin is the READER -- `RCPReg.00-15` (the CP register read
    # back from the Dorado), `MCManif.0-3`, `TCPI.0-3`, the temperature senses,
    # `WatchdogIn`. So a shared net means the pin is an input, and its pull-up
    # must lose. Where the 6532 is the SOLE driver the pull-up stands, which
    # matters: `WatchdogOut` is such a net, and dropping its pull-up holds the
    # 6502 in reset.
    WEAK_PORT_DRIVERS = {'MCS6532': frozenset(
        [8, 9, 10, 11, 12, 13, 14, 15,          # PA0..PA7
         16, 17, 18, 19, 21, 22, 23, 24])}      # PB0..PB7

    # Neither a driver nor a consumer. `Term100` is a 100-ohm TERMINATING
    # RESISTOR network -- ECL terminates every line -- and an empty socket is
    # nothing at all. Reading a terminator as the board's connector is what
    # made the old port inference wrong.
    _NOT_LOGIC = ('Term', 'SpareSocket')

    # A resistor pack, which is not a cell and cannot be one: its pins DRIVE
    # on some boards and are the tie point on others (of the eight, seven are
    # used both ways), so no fixed set of port directions fits. It is
    # resolved here instead -- see sip_pull.
    _RESISTOR_PACK = ('SIPpackage',)

    _POWER_RE = re.compile(r'^(VCC|GND|VEE)')

    # The supply rails, which the RTL has to state because a wire nobody
    # assigns reads zero -- and a board is full of gates whose enable, preset
    # or count input is tied straight to VCC. `VCC101` on the ROM decoder is
    # one; there are 1,010 such nets across the sixteen boards.
    #
    # Everything below ground is a logic 0: VEE is -5.2 V, VTT the -2 V ECL
    # terminator, VBB the -1.3 V reference. VDD is the +12 V analog rail and
    # sits with VCC on the high side. Names come from the wire lists.
    _RAIL_HIGH = re.compile(r'^(VCC|VDD)')
    _RAIL_LOW = re.compile(r'^(GND|SGND|VEE|VTT|VBB|VSS)')

    # Wire-wrap headers -- a field of jumper positions, not a part. Like the
    # resistor packs they cannot be a cell: what a header DOES is decided by
    # which wires a technician wrapped onto it, and that is not in the
    # netlist. What IS in the netlist is the geometry: every pin carries an
    # {x,y}, and a jumper position is a COLUMN of pins at one x. See
    # jumper_straps for how much of that can honestly be resolved.
    _JUMPER_FIELD = ('AUGAT',)

    # A resistor PLATFORM: sixteen pins, eight resistors, pin N wired to pin
    # 17-N. That pairing is not inferred -- every pin carries an {x,y} and the
    # two ends of each resistor share an x, unanimously, 122 pairs across the
    # machine with no exception. Like the SIPs and the Augat headers it cannot
    # be a cell, because what it does depends on what a board put across it.
    _SERIES_PACK = ('PLAT',)

    def series_pass(self) -> dict[tuple[str, int], str]:
        """(package, pin) -> the net a series resistor brings to that pin.

        A series resistor PASSES a signal; it does not create one. So this is
        taken only where the netlist forces it: the far pin's net has no other
        source on the board, and neither end is a supply rail.

        MemX is what this is for. Three platforms carry the map DRAM's address
        and strobes through series damping resistors -- `TMapAd.0a` in on pin 1
        and `RTMapAd.0a` out on pin 16, and so on for 24 signals -- and with
        them unmodelled every one of those address lines sat at zero.

        The display boards' platforms are the reason for the conditions. Theirs
        sit in the video DAC's supply filtering, across `GNDBlue`, `RegVCCB`,
        `FilterVEEB`, a DAC output and two references. Those are analog nodes
        and a couple are rails; passing a level through them would be
        inventing a signal, so they are left alone and counted."""
        out: dict[tuple[str, int], str] = {}
        self.series_skipped = 0
        for pos in sorted(self.b.packages):
            if not self._pkg_type(pos).startswith(self._SERIES_PACK):
                continue
            pins = dict(self.pkg_pins(pos))
            for pin, netname in sorted(pins.items()):
                far = pins.get(17 - pin)
                if far is None or far == netname:
                    continue
                if self.rail_value(netname) or self.rail_value(far):
                    self.series_skipped += 1
                    continue
                if (self._strappable(netname) and not self._strappable(far)
                        and self._driven_by_digital(far)):
                    out[(pos, pin)] = far
                else:
                    self.series_skipped += 1
        return out

    def rail_value(self, netname: str) -> str | None:
        if self._RAIL_HIGH.match(netname):
            return "1'b1"
        if self._RAIL_LOW.match(netname):
            return "1'b0"
        return None

    def read_name(self, netname: str) -> str:
        """The identifier to READ a net by, in the emitted module.

        Not always its own name. A backplane net this board only DRIVES has
        no plain wire -- its contribution goes out on `<net>__drv` and the
        resolved bus never comes back, because nothing here senses it. So
        anything on this board that wants to read it (a jumper strapping it
        somewhere, a part taking its own output back on a companion input)
        has to name the contribution."""
        if netname in self.exports and netname not in self.ports:
            return f'{vname(netname)}__drv'
        return vname(netname)

    def read_excluding(self, netname: str, pos: str) -> str:
        """What this net carries, MINUS what `pos` itself puts on it.

        A part reading a bus it also drives cannot simply be handed the
        resolved net. Inside the netlist 6502 `dbo` is combinational on `dbi`
        -- both come out of the same relaxation of 1,725 nodes -- so wiring
        the bus back to `dbi` is a wire that inputs its own output. It settles
        perfectly while the part is off the bus and stops settling the instant
        it drives one: the machine ran thousands of cycles and then failed to
        converge at the 6502's first `STA`, reported against a different board
        entirely.

        Excluding the part's own contribution is also what the hardware means.
        A driver does not read its own drive to know what it is driving; what
        it needs off the bus is what everyone ELSE has put there. The
        wired-OR already gives every driver a private stub, so this is an OR
        of the other stubs -- no delay, and the loop cannot form.

        A net driven only by this package carries nothing else, hence 0.
        """
        drivers = self.drivers_in_rtl(netname)
        others = [p for p in drivers if p['pkg'] != pos]
        if not others:
            return "1'b0"
        if len(drivers) > 1:            # wired-OR: each driver has a stub
            return '(' + ' | '.join(
                f'{vname(netname)}__{vname(p["pkg"])}_{p["pin"]}'
                for p in others) + ')'
        return self.read_name(netname)  # the sole driver is someone else

    def _expr(self, v: str) -> str:
        """A rail literal passes through; anything else is a net to read."""
        return v if v.startswith("1'b") else self.read_name(v)

    def _pkg_type(self, pos: str) -> str:
        return self.b.packages.get(pos, {}).get('type', '')

    def jumper_straps(self) -> dict[tuple[str, int], str]:
        """(package, pin) -> the net a wire-wrap jumper brings to it.

        The wire list gives every pin an {x,y}, and on these headers the pins
        line up in COLUMNS -- one x, two rows. A column is one jumper
        position: wrap a wire across it and the two nets are connected.

        Which jumpers are fitted is a configuration, so only the case where
        the netlist forces the answer is taken: a column of EXACTLY TWO pins
        where one of the nets has no other source on the board at all. That
        net exists to be strapped -- nothing else can ever drive it -- so the
        jumper is fitted, and the netlist has told us so.

        A column of three pins is a CHOICE and is left alone: MemX's b14
        offers `RTMapAd.1a` or `VCC-47` on `RamA1orVCCa`, and msa's e26 picks
        `ChipsAre4k` against `ChipsAre16k`. Guessing there would be inventing
        a machine configuration. They are counted in the report instead.

        This is what makes the BaseBoard's ROM decode work. `RSA.0/1/2` --
        the three select inputs of the '138 that produces `Rom0'`..`Rom7'` --
        have no driver anywhere; they sit across from `MCA.11`, `MCA.12` and
        `MCA.13` in three two-pin columns. tools/firmware_eproms.py had
        derived that same strapping from what tiles the address space with 2K
        parts; the header's geometry states it outright.
        """
        out: dict[tuple[str, int], str] = {}
        pinnet_of: dict[tuple[str, int], str] = {}
        for name, net in self.b.nets.items():
            for p in net['pins']:
                pinnet_of[(p['pkg'], p['pin'])] = name
        self.jumper_choices = 0
        for pos in sorted(self.b.packages):
            if not self._pkg_type(pos).startswith(self._JUMPER_FIELD):
                continue
            cols: dict[int, list] = {}
            for pin, netname in self.pkg_pins(pos):
                for p in self.b.nets[netname]['pins']:
                    if p['pkg'] == pos and p['pin'] == pin and p['xy']:
                        cols.setdefault(p['xy'][0], []).append(
                            (p['xy'][1], pin, netname))
            for x, items in cols.items():
                if len(items) != 2:
                    self.jumper_choices += len(items) > 2
                    continue
                items.sort()
                (_, pin_a, net_a), (_, pin_b, net_b) = items
                if net_a == net_b:
                    continue
                open_a = self._strappable(net_a)
                open_b = self._strappable(net_b)
                if open_a and not open_b:
                    out[(pos, pin_a)] = net_b
                elif open_b and not open_a:
                    out[(pos, pin_b)] = net_a

        # A NET OFFERED MORE THAN ONE SOURCE is a choice too, even though each
        # column is only two pins. MemX's b13 offers `RamA1orVCCa` a ground and
        # b14 offers it VCC-47 and the map address line `RTMapAd.1a`; the names
        # say it outright -- "RamA1 OR VCC" -- and it selects the size of RAM
        # fitted. Exactly one of those jumpers is in place and the netlist does
        # not say which, so taking any of them asserts a machine configuration,
        # and taking several at once asserts a contradictory one: this used to
        # emit that net strapped to GND and to VCC together.
        #
        # The same shape appears wherever several jumper positions meet at one
        # Sil sheet label, which on the BaseBoard and DispM is an analog node
        # rather than a choice. Neither wants a level invented for it.
        target = Counter(pinnet_of[k] for k in out)
        for k in [k for k in out if target[pinnet_of[k]] > 1]:
            out.pop(k)
            self.jumper_choices += 1
        return out

    def _driven_by_digital(self, netname: str) -> bool:
        """Is this net driven by a part the dictionary gives a behaviour?

        A resistor in series with a LOGIC signal passes that signal on; a
        resistor between a D/A converter's output and a supply node is an
        analog filter, and passing a level across it would be inventing one.
        The dictionary separates the two: a digital part carries a `[G ...]`,
        `[FF ...]` or `[M ...]` summary and an analog one carries none. DispM's
        `DACBlue` comes off an MC10318, which has no summary at all."""
        for p in self.b.nets[netname]['pins']:
            if p['dir'] != 'out':
                continue
            t = self._pkg_type(p['pkg'])
            if t.startswith((self._NOT_LOGIC + self._RESISTOR_PACK
                             + self._JUMPER_FIELD + self._SERIES_PACK)):
                continue                      # passive; not a source
            got = self.ecl.by_full_type(t)
            if got and got[0] in self.ecl.behavioural:
                return True
        return False

    def _strappable(self, netname: str) -> bool:
        """Nothing on this board can drive this net, and it stays here.

        A rail is never the strapped side -- it is what you strap TO -- and a
        net that leaves the board may be driven from another slot, so neither
        qualifies."""
        if self.rail_value(netname):
            return False
        if self.b.leaves_board(netname):
            return False
        for p in self.b.nets[netname]['pins']:
            if p['dir'] != 'out':
                continue
            t = self._pkg_type(p['pkg'])
            # A jumper field, a terminator and a resistor platform are all
            # PASSIVE: a pin of theirs on a net is not a source for it. Missing
            # the platform here made every net one drives look driven, so no
            # series resistor resolved at all -- MemX's 24 map-DRAM address
            # lines stayed at zero -- while the display boards' analog nodes,
            # where both ends are open, resolved the wrong way round.
            if (t.startswith(self._JUMPER_FIELD)
                    or t.startswith(self._NOT_LOGIC)
                    or t.startswith(self._SERIES_PACK)):
                continue
            if t.startswith(self._RESISTOR_PACK):
                if (p['pkg'], p['pin']) in self._sips:
                    return False
                continue
            return False
        return True

    # Buses the wire list calls OUTPUTS and the part also READS.
    #
    # A 6502's D0-D7 are one bidirectional bus, but PARC marks them `o` on
    # this board and the RTL has no `inout` anywhere by design, so the cells
    # present the two directions separately: the core drives `dbo` onto the
    # net and takes `dbi` back from it. Nothing in the wire list can say that
    # -- it is a fact about the PART -- so the companion port is named here
    # and the generator connects it to the RESOLVED net at each pin, which is
    # exactly what the chip's pin sees.
    #
    # Pins are listed MOST SIGNIFICANT FIRST, matching the concatenation.
    # Without this the 6502 reads 0x00 forever, which is BRK: it fetches a
    # vector, takes the interrupt, and fetches the vector again.
    READBACK = {
        'MCS6502': {'dbi': [26, 27, 28, 29, 30, 31, 32, 33]},   # D7..D0
        'MCS6532': {'d_in':  [26, 27, 28, 29, 30, 31, 32, 33],  # D7..D0
                    'pa_in': [15, 14, 13, 12, 11, 10, 9, 8],    # PA7..PA0
                    'pb_in': [16, 17, 18, 19, 21, 22, 23, 24]}, # PB7..PB0
    }

    # ...and these read their OWN pin, not just what others drive onto it.
    #
    # A gate does not read its own output, which is why `read_excluding` drops
    # the package's own contribution. A 6532 PORT PIN is not a gate output: the
    # chip reads the PIN, and for a pin its DDR makes an OUTPUT the pin is
    # whatever the chip itself is driving. The MiSTer core states the
    # requirement outright -- "NOTE that port output must be fed back to input
    # ... in order for the chip to read properly".
    #
    # Without it, every read-modify-write on a port reads ZERO for its own
    # output bits. BaseBd i62 is MCPBusL, and `SetMufflerAddress` pulses the
    # CP-bus strobe with `INC $0582` / `DEC $0582`: reading 0x00 instead of
    # 0x10 made those produce 0x01 and 0xFF, so the three `MCPABus` function
    # bits read 0 then 7 instead of a constant 1 (`Clock`) and the BaseBoard's
    # own k22/k17 never decoded a DMux pulse.
    #
    # No combinational loop: the port output comes from the core's `out_b`/
    # `dir_b` REGISTERS, so the path net -> pb_in -> ... -> out_b -> net is
    # broken by a flip-flop. `loop-check` covers it.
    READBACK_OWN_PIN = {('MCS6532', 'pa_in'), ('MCS6532', 'pb_in')}


    # A SIP's LEGS CAN BE CUT, and only the schematic says which.
    #
    # A resistor SIP is a set of resistors from a common pin to the rest, so
    # `sip_pull` holds every other pin at whatever the common sits on. But the
    # boards break individual legs to leave particular pins alone, and the wire
    # list cannot express that -- the pin is still listed, it just has no
    # resistor behind it any more. The SCHEMATIC states it, on a per-board
    # "Configuration Information" sheet.
    #
    # DispY's (DoradoDocs/schematics/DispY.pdf page 31, "Rev Ci, K. Pier,
    # 11/02/79") reads:
    #
    # TAKE THE SHEET THE WIRE LIST NAMES. Each `.wl` header lists every
    # constituent `.sil` file with its own Rev and Date, and the scanned
    # `DoradoDocs/schematics/DispY.pdf` is Rev Ci 11/02/79 while the wire list
    # says `File=DispY31.sil Rev=Cl Date=3/25/82` -- two and a half years
    # apart, and g41's cut list CHANGED between them (3,4,5 -> 4,5). The
    # per-revision scans in `DoradoDocs/doradodrawings/` carry the built one.
    #
    #     3. SIPs are 100 ohm terminator package with legs broken:
    #          location | break legs
    #          g41      | 3,4,5
    #          g42      | none
    #          k51      | 4,5
    #          k52      | 3
    #
    # All four have pin 1 on `True`, so they are pull-UPS, and a pull-up is
    # exactly the case `sip_pull` says matters under the OR that resolves these
    # nets. Without the cuts, `WakeupWait.1/2/3` (g41), `DDCDMD.03/04` (k51) and
    # `DWTTask.1` (k52) were being forced HIGH -- task wakeups, the display's
    # manifold data, and the display word task's number.
    #
    # The same sheet also settles two parts: `PLAT1816` at a01/a03 is NOT a chip
    # but a platform of discrete components (a 3K/120/180 ohm and 0.1uF DAC
    # network at a01, a 78L05 regulator with 12 mH and 22 mF filtering at a03),
    # and `MC10318` at a02 is the D/A converter those feed. Neither needs a
    # digital model.
    #
    # EVERY BOARD HAS SUCH A SHEET. Only DispY's has been read; the others are
    # in DoradoDocs/schematics/ and doradodrawings/ and should be checked the
    # same way before their boards are trusted.
    SIP_BROKEN_LEGS = {
        # BaseBd l49, stated TWICE and consistently: the stuffing sheet
        # (BaseBd17.sil) says "Location l49 must be loaded with a SIP having
        # pins 2,4,5,&7 cut", and the sheet that draws it (Basebd04.sil,
        # "Muffler and Manifold", PDF page 8) labels the same pack
        # "Terminator with all pins cut except 1, 3, 6, & 8". Pins 1 and 8
        # are the VCC commons, so the surviving legs are pin 3 = Midas.01
        # and pin 6 = Midas.04.
        ('BaseBd', 'l49'): frozenset({2, 4, 5, 7}),
        ('DispY', 'g41'): frozenset({4, 5}),
        ('DispY', 'g42'): frozenset(),
        ('DispY', 'k51'): frozenset({4, 5}),
        ('DispY', 'k52'): frozenset({3}),

        # DispM, from DispM30.sil Rev Ch 11/09/82 -- again the revision the
        # wire list names. This sheet states the RESULT of each cut, so the
        # straps check themselves against PARC's MSB-first field numbering:
        #
        #   "SIP in location g41 is 100 ohm terminator with leg 6 cut
        #    (making DDMTIOA = 360B)".  g41 pin 1 is `True`; pins 2..6 are
        #    DDMTIOA.00..04. Cutting leg 6 drops DDMTIOA.04, leaving
        #    11110 = 36B -- and 360B is 36B with a low octal digit for the
        #    register select, so the board answers at 360B..367B. That is
        #    where the C emulator already finds it (presence reads
        #    "360B=3 361B=1").
        #
        #   "SIP in location b52 is 100 ohm terminator with legs 3 and 4 cut
        #    for Task 9D = 11B".  b52 pins 2..5 are AltoWTask.0..3; cutting
        #    legs 3 and 4 leaves 1001 = 9 decimal = 11 octal, exactly as
        #    stated.
        ('DispM', 'g41'): frozenset({6}),
        ('DispM', 'b52'): frozenset({3, 4}),

        # DskEth, from Ether12.sil's reference sheet (DskEth.pdf sheet 7) --
        # not a "Configuration Information" page at all, which is why it has
        # to be searched for by content:
        #
        #     Cut SIP legs at e41 to set the IOA bus addresses for the board.
        #   * Standard addresses are 10-17.
        #
        # followed by a table of all 32 ranges against which of P4..P8 to cut.
        # e41 pins 4..8 are TIOA-Ad.0..4, so the columns ARE the pins, and the
        # table states the encoding outright: a cut leg is 0, an intact one is
        # 1, MSB first. Check it at both ends -- 000-007 cuts all five, and
        # 170-177 cuts P4 alone for 01111 = 17B. The starred standard row
        # 010-017 cuts P4,P5,P6,P7, leaving 00001 = 1, and 010B..017B is that
        # with the low octal digit selecting the register. Uncut the board
        # would sit at 370B..377B.
        ('DskEth', 'e41'): frozenset({4, 5, 6, 7}),
    }

    # Where a pull-UP pack and a pull-DOWN pack hold the SAME net, the
    # default reading is a resistive divider -- a bias network at an analog
    # input -- and the net is left open (see sip_drives). That is right for
    # DskEth's `RcvData`, which is the Ethernet receiver's own input off the
    # wire. It is NOT right for a CONFIGURATION STRAP, where the pull-down
    # pack supplies the default 0 on every line and the pull-up pack has legs
    # only on the lines that are meant to read 1 -- so the pull-up wins.
    #
    # The BaseBoard's Midas straps are that second thing, and the schematic
    # says so rather than leaving it to be inferred: Basebd04.sil draws l50
    # as a full pack to GND and l49 as one to VCC "with all pins cut except
    # 1, 3, 6, & 8". Electrically the pull-up does win -- these feed an
    # SN74LS85's B inputs, which draw 20 uA when high, so the divider sits at
    # about VCC/2 = 2.5 V against a 2.0 V VIH.
    #
    # The result is the BaseBoard's own MUFFLER NUMBER. k20 compares the
    # muffler address DMD.04..01 against {Midas.04,03,02,01} = 1001 = 9 and
    # drives `BaseMuf'`, which is the enable on k22 (the LS151 that sources
    # `CPDMuxData`) and on k19. Left open, `BaseMuf'` never asserts and the
    # BaseBoard can never answer a muffler read at all.
    SIP_STRAP_PULLUP_WINS = {
        ('BaseBd', 'Midas.00'), ('BaseBd', 'Midas.01'), ('BaseBd', 'Midas.02'),
        ('BaseBd', 'Midas.03'), ('BaseBd', 'Midas.04'),
    }

    # A PIN CAN BE BROKEN OFF A LOGIC CHIP TOO, not just a resistor pack, and
    # again only the schematic says so. MemX carries a sheet headed "Dorado
    # Memory Extension Board / Stuffing and Configuration Instructions"
    # (Memx23.sil, dated 10/29/79 -- the date the wire list names) whose
    # instruction 2 is unconditional:
    #
    #     2. Break h20.10 before stuffing. Label this chip as MemX-h20.
    #
    # h20 is an MC10105 and pin 10 carries `MapPerr`, so with the pin present
    # the memory-parity summary is (STPerr | MapPerr | HitPerr); broken, a map
    # parity error does not raise it. MapPerr keeps its other two consumers
    # (g49.2, k08.5), so the signal is not lost -- only this term.
    #
    # A broken MECL input is not floating: 10K inputs have an internal pulldown
    # to VEE, which is why the ECL symbol sheet's rule for a genuinely unused
    # input is "ALL UNUSED INPUTS MUST BE TIED TO VEE". So the pin reads 0.
    #
    # NOT here, deliberately: the same sheet's instruction 1, "If 256 chips are
    # NOT installed in the Memory Storage Arrays (MSA), then break g10.6". That
    # one is CONDITIONAL on how much memory the machine was built with, and
    # `ChipsAre256/16K` and `ChipsAre64K` are backplane INPUTS to MemX rather
    # than straps on it, so the configuration arrives from outside the board
    # and is not ours to pick from the documents. Its blue wire (k4.6/k4.5/k4.4
    # to k4.16) and the b14/c12 jumper sockets are the same choice.
    # Per-position cell parameters, for parts whose VALUE is a property of the
    # component fitted rather than of the part type. So far that is the
    # crystal oscillators: four K1115A positions across three boards, and the
    # configuration sheets give each a different frequency.
    #
    #   DispY  a05  50 MHz        DispY31.sil Rev Cl 3/25/82 note 5 -- and
    #                             note the older Rev Ci scan says 20, which is
    #                             why the sheet has to be the one the wire
    #                             list names.
    #   DispM  c05  10 MHz, VCO   DispM30.sil Rev Ch 11/09/82
    #   DispM  d13  20 MHz Alto / 50 MHz LF, same sheet. Taken as 20: the
    #                             board's two monitor modes are 640x480 and
    #                             1024x768, a 2.5x pixel-rate ratio which is
    #                             exactly 50/20, and the C emulator's head
    #                             reports the 640x480 "standard" one
    #                             (include/dispm.h DORADO_DISPM_STANDARD).
    #   DskEth j20  no value on Ether12.sil, so it keeps the default. The
    #                             23.530 MHz on DskEth sheet 7 belongs to a
    #                             separate xtalosc in the parts list.
    #
    # The cell takes a frequency, not a divisor: an integer divisor of the
    # 266.667 MHz sys_clk cannot express these three (13.33, 6.67 and 2.67),
    # and rounding them gives a 1 : 1.86 : 4.33 ratio where the parts are
    # 1 : 2 : 5. cell_K1115A carries the remainder in a phase accumulator.
    CELL_PARAMS = {
        ('DispY', 'a05'): {'FREQ_KHZ': 50000},
        ('DispM', 'c05'): {'FREQ_KHZ': 10000},
        ('DispM', 'd13'): {'FREQ_KHZ': 20000},
    }

    BROKEN_PACKAGE_PINS = {
        ('MemX', 'h20'): frozenset({10}),
    }

    def sip_pull(self, pos: str) -> tuple[str | None, str]:
        """What a resistor pack ties its pins to, as an expression.

        A SIP is a set of resistors from a COMMON pin to the rest, so every
        other pin is weakly held at whatever the common sits on. The common
        is not at a fixed pin number -- BaseBd g47 uses pin 7, l49 and l50 use
        pins 1 and 8 -- so it is found by what it is CONNECTED to:

          * a power net (`VCC103`, `GND639`, `VEE-60`) -- a pull-up or a
            pull-down, the usual case; or
          * a reference net a board makes for the purpose (`True`,
            `ECLTrueA`, `TTLHigh`), which the pack then passes on.

        A pull-up matters and a pull-down does not, under the OR that resolves
        these nets: contributing 1 forces an otherwise-open net high, which is
        exactly what an open input sees, and contributing 0 changes nothing.

        This is how the BaseBoard gets `TTLTrue.A`..`E` from g47, and those
        are the constant 1 that its TTL counters and flip-flops count, load
        and enable from -- so with the packs missing the 6502 cannot be
        clocked or released from reset at all.

        Returns (expression or None, why)."""
        power, ref = None, None
        for pin, netname in self.pkg_pins(pos):
            if self._POWER_RE.match(netname):
                power = '1\'b1' if netname.startswith('VCC') else '1\'b0'
                why = netname
            elif (re.search(r'True|High', netname) and ref is None
                  and not self._pack_drives(pos, netname)):
                # `and not self._pack_drives(...)`: a pack that MAKES a
                # reference names it something like `TTLTrueA`, and taking
                # that as the pack's own common emits `assign X = X;` -- a
                # wire that inputs its own output. Verilator accepts it and
                # the machine then fails to settle, tens of thousands of
                # cycles later and on a different board.
                ref = refwhy = netname
        if power:
            return power, why
        if ref:
            return ref, refwhy
        return None, 'no rail or reference net at any pin'

    def _pack_drives(self, pos: str, netname: str) -> bool:
        """Is this net an OUTPUT of this package?"""
        return any(p['pkg'] == pos and p['dir'] == 'out'
                   for p in self.b.nets[netname]['pins'])

    def pkg_pins(self, pos: str):
        """(pin, net) for one package, from the wire list."""
        for name, net in self.b.nets.items():
            for p in net['pins']:
                if p['pkg'] == pos:
                    yield p['pin'], name

    def sip_drives(self) -> dict[tuple[str, int], str]:
        """(package, pin) -> expression, for every pack pin that holds a net.

        Two pins are left out deliberately. A pin sitting ON the rail is the
        common, not an output. And a net held by a pull-UP pack and a
        pull-DOWN pack at once is a RESISTIVE DIVIDER -- a bias network at an
        analog input, not a logic level -- so neither contributes and the net
        stays open. That is what BaseBd's Midas straps and DskEth's `RcvData`
        are; `RcvData` is the Ethernet receiver's own input, and forcing it
        high would be inventing a signal off the wire."""
        packs = {pos for pos, pkg in self.b.packages.items()
                 if pkg.get('type', '').startswith(self._RESISTOR_PACK)}
        pulls = {pos: self.sip_pull(pos)[0] for pos in packs}
        held: dict[str, set] = {}
        for pos in packs:
            broken = self.SIP_BROKEN_LEGS.get((self.b.name.split('-Rev')[0], pos), frozenset())
            for pin, netname in self.pkg_pins(pos):
                if pin in broken:
                    continue
                if pulls[pos] and not self._POWER_RE.match(netname):
                    held.setdefault(netname, set()).add(pulls[pos])
        board = self.b.name.split('-Rev')[0]
        divided = {n for n, v in held.items()
                   if len(v) > 1 and (board, n) not in self.SIP_STRAP_PULLUP_WINS}

        out = {}
        for pos in sorted(packs):
            expr = pulls[pos]
            if not expr:
                continue
            broken = self.SIP_BROKEN_LEGS.get((self.b.name.split('-Rev')[0], pos), frozenset())
            for pin, netname in self.pkg_pins(pos):
                if pin in broken:
                    continue            # leg cut: this pin is not pulled
                if self._POWER_RE.match(netname) or netname in divided:
                    continue
                if (expr == "1'b0"
                        and (board, netname) in self.SIP_STRAP_PULLUP_WINS
                        and len(held.get(netname, ())) > 1):
                    continue        # strap: the pull-up on this net wins
                # A leg the wire list calls `out` is a source. So is one it
                # calls `in`, when nothing else on the net drives it -- a
                # resistor has no direction, and Sil's letter on a pack pin
                # records how the sheet drew it, not what the part does.
                #
                # The schematic PROVES this rather than it being a guess.
                # DispM30.sil states "leg 6 cut (making DDMTIOA = 360B)", and
                # 360B needs DDMTIOA.02 high -- but .02 hangs on g41 pin 4,
                # which the wire list marks `in`, so nothing sourced it and
                # the field read 320B. The same pin-4 artifact leaves five
                # more strap bits open on DispY and DispM, the IFU's
                # reference net `TTLHigh` at 0, and six active-low DskEth
                # drive-status lines (`Selected0'`, `TtlReadOnly'`,
                # `TtlEndOfCyl'`, ...) reading ASSERTED, which fabricates a
                # drive that is not there.
                #
                # Source of last resort only: where something else drives the
                # net, the pull is left off, because these are resolved by an
                # OR tree and a pull-up would nail the net high forever.
                is_out = any(q['pkg'] == pos and q['pin'] == pin and q['dir'] == 'out'
                             for q in self.b.nets[netname]['pins'])
                others = any(q['dir'] == 'out' and
                             not (q['pkg'] == pos and q['pin'] == pin)
                             for q in self.b.nets[netname]['pins'])
                if (is_out or (not others and netname != self.sip_pull(pos)[1])):
                    out[(pos, pin)] = expr
        return out

    def _rtl_dir(self, p: dict) -> str | None:
        """What the EMITTED RTL does at this pin, or None if nothing.

        The CELL's port direction is the truth here, because the cell is what
        gets instantiated -- and it is global, aggregated over all sixteen
        boards, so it can differ from this board's wire-list direction. Where
        the cell says nothing about the pin, fall back to the wire list."""
        ptype = self.b.packages.get(p['pkg'], {}).get('type', '')
        if ptype.startswith(self._NOT_LOGIC):
            return None
        if ptype.startswith(self._RESISTOR_PACK):
            return ('output' if (p['pkg'], p['pin']) in self._sips
                    else None)
        if ptype.startswith(self._JUMPER_FIELD):
            return ('output' if (p['pkg'], p['pin']) in self._straps
                    else None)
        if ptype.startswith(self._SERIES_PACK):
            return ('output' if (p['pkg'], p['pin']) in self._series
                    else None)
        d = self.cell_dirs.get(vpart(ptype), {}).get(p['pin'])
        if d:
            return d
        return 'output' if p['dir'] == 'out' else 'input'

    def _weak_port(self, p: dict) -> bool:
        """Is this driver a pull-up that any real driver overrides?"""
        t = self.b.packages.get(p['pkg'], {}).get('type', '')
        return p['pin'] in self.WEAK_PORT_DRIVERS.get(t, ())

    def drivers_in_rtl(self, name: str) -> list[dict]:
        """The pins that actually DRIVE this net in the emitted RTL.

        THE ONE definition, used by classify() for port direction and by
        emit() for wired-OR resolution. It has to be one function: when the
        two decided separately -- emit() from the wire list's `o` pins,
        classify() from the cell's port directions -- they disagreed on five
        nets and the generator assigned to a module input (`%Error-ASSIGNIN`),
        and on eight more it left BaseBd's `MCD_0..7` with two continuous
        drivers, an `assign` racing a cell output."""
        return [p for p in self.b.nets[name]['pins']
                if self._rtl_dir(p) in ('output', 'inout')]

    def classify(self) -> None:
        """The ports are the nets PARC says leave the board, and nothing else.

        `<Board>.bp` lists exactly those, one per line (`ALUCarry: E179`), and
        the same fact is stated twice more -- as bare `E179` tokens in the
        `.wl` and slot-qualified in `-C.nl`/`-E.nl` -- with all three agreeing
        on 2,052 of 2,054 pins. `sil_netlist.load_bp` reconciles them.

        This replaces an INFERENCE that was wrong in both directions: ports
        were taken to be nets with no local driver, plus nets whose only
        consumers were `Term100` packages. Measured against the `.bp` files
        that missed 703 backplane nets across the sixteen boards, emitting
        them as internal wires -- so those signals could never have reached
        another board -- while inventing 833 ports that are not on the
        backplane at all.

        Direction comes from the wire list, since `.bp` does not state it:

          senses -> `input <name>`, the RESOLVED bus arriving.
          drives -> `output <name>__drv`, this board's CONTRIBUTION only.

        A board that does both gets both ports. That is the FPGA-friendly
        shape: no `inout`, no multiply-driven net, and the top level resolves
        a bus as an explicit OR of the contributions -- which is what MECL
        open emitters wired together actually compute, and what a LUT does in
        one level. 115 backplane nets are driven by more than one board, the
        B bus `BMux.00-15` among them (ContA, IFU, MemC, MemD, MemX and
        ProcH/ProcL all drive it).

        The earlier `inout` + `wor` form simulated correctly but is not
        synthesisable: an FPGA has no wired-OR outside its I/O ring, and a
        multiply-driven net is an error to every synthesis tool.
        """
        for name, net in self.b.nets.items():
            drv = self.drivers_in_rtl(name)
            if len(drv) > 1:                   # on-board wired-OR
                self.wired_or.append((name, len(drv)))

            drives = bool(drv)
            senses = any(self._rtl_dir(p) == 'input' for p in net['pins'])

            if not self.b.leaves_board(name):
                # Stays on the board. If nothing drives it, that is a REAL
                # gap -- the part that should drive it is a cell skeleton with
                # no behaviour -- and it is left undriven so simulation reads
                # X there rather than a plausible zero. Counted in the report.
                if not drives:
                    self.undriven_internal.append(name)
                continue

            # A net the board only terminates still has to appear, so that
            # the port list matches what PARC's .bp states.
            if senses or not drives:
                self.ports[name] = 'input'
            if drives:
                self.exports.add(name)

    def emit(self) -> str:
        self.classify()
        mod = vname(self.b.name)
        out: list[str] = []
        A = out.append

        A(f'// Generated by tools/sil_to_verilog.py from PARC\'s own Sil output.')
        A(f'// Board: {self.b.name}')
        A(f'// Source: {self.b.name}.wl (wire list) + .lc (parts)'
          f' + EclDict.Analyze (pin roles)')
        A(f'// DO NOT EDIT -- regenerate. Cell models live in verilog/cells/.')
        A('')
        A('`default_nettype none')
        A('')

        inputs = sorted(self.ports)
        exports = sorted(self.exports)
        A(f'// Ports: the {len(set(inputs) | set(exports))} nets '
          f'{self.b.name}.bp says reach a backplane')
        A(f'// connector -- PARC\'s own statement of this module\'s boundary,')
        A(f'// not an inference.')
        A('//')
        A('// `sys_clk` is the fabric clock. It is NOT a Dorado signal: the')
        A('// machine\'s own clock arrives on CLK.<board>\' and is used as an')
        A('// ENABLE inside the clocked cells, because 1,201 packages clocked')
        A('// from combinational nets is not something an FPGA can route.')
        A('//')
        A('// SYNTHESISABLE SHAPE, not the physical one: a net this board')
        A('// drives is exported as `<net>__drv`, carrying ONLY this board\'s')
        A('// contribution, and the resolved bus arrives back on `<net>`. The')
        A('// machine ORs the contributions. That is what MECL open emitters')
        A('// wired together compute, and unlike an `inout` on a multiply-')
        A('// driven net it maps to one level of LUT on an FPGA.')
        A(f'module {mod} (')
        decl = ['    input  wire sys_clk'] + \
               [f'    input  wire {vname(n)}' for n in inputs] + \
               [f'    output wire {vname(n)}__drv' for n in exports]
        A(',\n'.join(decl) if decl else '    // no backplane ports detected')
        A(');')
        A('')

        internal = [n for n in sorted(self.b.nets)
                    if n not in self.ports and n not in self.exports]
        A(f'  // {len(internal)} internal nets')
        for n in internal:
            A(f'  wire {vname(n)};')
        A('')

        # Which net does each (package,pin) belong to?
        pinnet: dict[tuple[str, int], str] = {}
        for name, net in self.b.nets.items():
            for p in net['pins']:
                pinnet[(p['pkg'], p['pin'])] = name

        # Wired-OR resolution, on-board. Several MECL outputs on one net is
        # the design working as intended, and an explicit OR of the drivers
        # is both what it computes and what synthesises.
        wired = {n for n, _ in self.wired_or}
        if self.wired_or:
            A('  // ---- wired-OR nets (MECL 10K open emitters tied together)')
            A('  // An explicit OR of the drivers: what the open emitters')
            A('  // compute, and one LUT level rather than a multiply-driven')
            A('  // net that no synthesis tool accepts.')
            for name, _n in self.wired_or:
                drivers = self.drivers_in_rtl(name)
                # Every driver still needs its stub -- the package's pin is
                # wired to it either way -- but an overriding driver is the
                # only one that reaches the net.
                for p in drivers:
                    A(f'  wire {vname(name)}__{vname(p["pkg"])}_{p["pin"]};')
                # A weak (pull-up) driver loses to any real one -- see
                # WEAK_PORT_DRIVERS. Its stub is still emitted above; it just
                # does not reach the net.
                strong = [p for p in drivers if not self._weak_port(p)]
                if strong and len(strong) != len(drivers):
                    A(f'  // {name}: a 6532 port pin here is an INPUT (its '
                      f'pull-up loses)')
                    drivers = strong
                over = [p for p in drivers
                        if self.b.packages.get(p['pkg'], {}).get('type', '')
                        in self.OVERRIDE_DRIVERS]
                if over:
                    A(f'  // {name}: the '
                      f'{self.b.packages[over[0]["pkg"]]["type"]} pulls this '
                      f'net and')
                    A('  // overrides the wired-OR -- see OVERRIDE_DRIVERS.')
                    drivers = over
                terms = ' | '.join(f'{vname(name)}__{vname(p["pkg"])}_{p["pin"]}'
                                   for p in drivers)
                tgt = f'{vname(name)}__drv' if name in self.exports else vname(name)
                A(f'  assign {tgt} = {terms};')
            A('')

        # A net driven by exactly ONE pin here but exported still needs its
        # contribution named, so the driver has somewhere to go.
        single = [n for n in sorted(self.exports)
                  if n not in wired]
        if single:
            A(f'  // {len(single)} single-driver contributions to the backplane')
            A('')

        # Resistor packs. Not instantiated as cells -- see sip_pull -- so
        # their contribution is stated here, at the level the pack's common
        # pin sits on.
        if self._sips:
            A('  // ---- resistor packs (SIP): pins held at the pack\'s common')
            for (pos, pin), expr in sorted(self._sips.items()):
                netname = pinnet[(pos, pin)]
                if netname in wired:
                    tgt = f'{vname(netname)}__{vname(pos)}_{pin}'
                elif netname in self.exports:
                    tgt = f'{vname(netname)}__drv'
                else:
                    tgt = vname(netname)
                why = self.sip_pull(pos)[1]
                A(f'  assign {tgt} = {self._expr(expr)};'
                  f'   // {pos}.{pin} {netname}, tied to {why}')
            A('')

        # The supply rails, stated. A wire nobody assigns reads zero, and
        # a great many gates take an enable or a preset straight from VCC.
        rails = [n for n in sorted(self.b.nets)
                 if self.rail_value(n) and n not in self.ports
                 and n not in self.exports]
        if rails:
            A(f'  // ---- {len(rails)} supply rails')
            for n in rails:
                A(f'  assign {vname(n)} = {self.rail_value(n)};')
            A('')

        # Wire-wrap jumpers. See jumper_straps for which are taken and why.
        if self._straps:
            A('  // ---- wire-wrap jumpers (Augat headers): the strap the'
              ' netlist forces')
            for (pos, pin), src in sorted(self._straps.items()):
                netname = pinnet[(pos, pin)]
                if netname in wired:
                    tgt = f'{vname(netname)}__{vname(pos)}_{pin}'
                elif netname in self.exports:
                    tgt = f'{vname(netname)}__drv'
                else:
                    tgt = vname(netname)
                A(f'  assign {tgt} = {self.read_name(src)};'
                  f'   // {pos}.{pin}: {netname} strapped to {src}')
            A('')

        # Resistor platforms: a series resistor passes its signal on.
        if self._series:
            A('  // ---- series resistor platforms (PLAT): pin N to pin 17-N')
            for (pos, pin), src in sorted(self._series.items()):
                netname = pinnet[(pos, pin)]
                if netname in wired:
                    tgt = f'{vname(netname)}__{vname(pos)}_{pin}'
                elif netname in self.exports:
                    tgt = f'{vname(netname)}__drv'
                else:
                    tgt = vname(netname)
                A(f'  assign {tgt} = {self.read_name(src)};'
                  f'   // {pos}.{pin} {netname} <- {src} (pin {17 - pin})')
            A('')

        A('  // ---- packages')
        placed = 0
        for pos in sorted(self.b.packages):
            pkg = self.b.packages[pos]
            ptype = pkg.get('type', '')
            if ptype.startswith(('SpareSocket', 'Term')):
                continue                      # not logic
            if ptype.startswith(self._RESISTOR_PACK):
                continue                      # resolved above, not a cell
            if ptype.startswith(self._JUMPER_FIELD):
                continue                      # a jumper field, not a part
            if ptype.startswith(self._SERIES_PACK):
                continue                      # a resistor platform, not a part
            roles = self.ecl.pin_roles(ptype)
            conns = []
            for (pk, pin), netname in sorted(pinnet.items()):
                if pk != pos:
                    continue
                role = roles.get(pin)
                if role is None:
                    self.unknown_pins[ptype] += 1
                # A driver connects to its private stub if the net has
                # several drivers here, else straight to whatever carries
                # this board's contribution; a receiver always reads the
                # resolved bus.
                isout = any(p['pkg'] == pos and p['pin'] == pin
                            for p in self.drivers_in_rtl(netname))
                if isout and netname in wired:
                    target = f'{vname(netname)}__{vname(pos)}_{pin}'
                elif isout and netname in self.exports:
                    target = f'{vname(netname)}__drv'
                else:
                    target = vname(netname)
                if (not isout and netname.upper().startswith('VBB')
                        and role in ('in', 'common')):
                    # VBB IS THE ECL SWITCHING THRESHOLD, not a logic signal,
                    # and these nets have no driver anywhere (39 of them,
                    # zero drivers). A differential pair with VBB on one side
                    # follows the OTHER side -- so VBB must LOSE to a real
                    # signal and BEAT an open pin, which is exactly what the
                    # roles distinguish: `common` is the complement input
                    # (EclDict's `c`), where VBB must read 0 so the true
                    # input decides; `in` is the true input, where it must
                    # read 1 against a complement that is open (an open MECL
                    # input sits at VEE, i.e. 0).
                    #
                    # A single constant per NET cannot do this -- one VBB net
                    # reaches both sides. MemX e15 is the case that found it:
                    # channel a has VBB on `common` (pin 2) with a real signal
                    # on pin 3, while channel d has VBB on `in` (pin 15) with
                    # pin 14 open, and that channel MANUFACTURES A CONSTANT
                    # TTL HIGH -- `THi`, which strobes pin 6 of five MC10124
                    # translators. Left at 0 it gated off the entire map read
                    # path, holding MapWP'/MapDirty'/MapEven' and so
                    # MapTrouble asserted forever.
                    target = "1'b1" if role == 'in' else "1'b0"
                elif pin in self.BROKEN_PACKAGE_PINS.get(
                        (self.b.name.split('-Rev')[0], pos), frozenset()):
                    # leg broken off before stuffing: an open MECL input
                    # sits at VEE, so it reads 0 and the net is not connected
                    target = "1'b0"
                conns.append(f'    .p{pin}({target})')
            cell = f'cell_{vpart(ptype)}'
            # COMPARE THE SANITISED NAME. `known_cells` reads the names off
            # the FILENAMES, which are already sanitised (cell_MK4096P_6.v),
            # while `ptype` is PARC's raw part name (MK4096P-6). Testing the
            # raw name marked 151 packages "NO MODEL" that have a perfectly
            # good one and are correctly instantiated -- 144 of them the
            # MK4096 DRAMs of the storage array -- which also understated the
            # cell-coverage figure.
            if vpart(ptype) not in self.cells:
                self.missing[ptype] += 1
                A(f'  // NO MODEL for {ptype} -- stub, ports preserved')
            params = ''
            if ptype in (*PROM_PARTS, 'i2716') and pos in self.prom_image:
                # A PROM package with contents PARC's own BCPL computes. The
                # path is repo-relative, so run the simulation from the repo
                # root; $readmemh fails loudly rather than silently if not.
                params = f'#(.INIT_FILE("{self.prom_image[pos]}")) '
                self.proms_wired += 1
            elif ptype in (*PROM_PARTS, 'i2716'):
                A(f'  // PROM with no contents in the archive -- reads X')
            extra = self.CELL_PARAMS.get((self.b.name.split('-Rev')[0], pos))
            if extra:
                # A value that belongs to the fitted component, not the part
                # type -- see CELL_PARAMS.
                params = ('#(' + ', '.join(f'.{k}({v})'
                                           for k, v in sorted(extra.items()))
                          + ') ')
            # A bus the part drives AND reads: hand back what the OTHER
            # drivers put on it. See read_excluding.
            for port, pins in self.READBACK.get(vpart(ptype), {}).items():
                own = (vpart(ptype), port) in self.READBACK_OWN_PIN
                bits = [(self.read_name(pinnet[(pos, pn)]) if own
                         else self.read_excluding(pinnet[(pos, pn)], pos))
                        if (pos, pn) in pinnet else "1'b0" for pn in pins]
                conns.append(f'    .{port}({{{", ".join(bits)}}})')
            A(f'  {cell} {params}u_{vname(pos)} (')
            if vpart(ptype) in self.clocked:
                conns = ['    .sys_clk(sys_clk)'] + conns
                self.clocked_placed += 1
            A(',\n'.join(conns) if conns else '    // no connections')
            A(f'  ); // {ptype}')
            placed += 1
        A('')
        A(f'endmodule')
        A('`default_nettype wire')
        self.placed = placed
        return '\n'.join(out) + '\n'

    def report(self) -> str:
        r = [f'board {self.b.name}',
             f'  packages placed     {self.placed}',
             f'  backplane nets {len(set(self.ports) | self.exports)}'
             f'  ({len(self.ports)} in, {len(self.exports)} contributions out)',
             f'  PROM packages wired {self.proms_wired}',
             f'  clocked packages    {self.clocked_placed} (on sys_clk, '
             f'ECL clock as enable)',
             f'  on-board wired-OR   {len(self.wired_or)}',
             f'  undriven internal   {len(self.undriven_internal)} '
             f'(a cell skeleton owes these a driver)',
             f'  parts without model {len(self.missing)} '
             f'({sum(self.missing.values())} packages)']
        if self.missing:
            r.append('  missing cell models, most used first:')
            for t, n in self.missing.most_common(15):
                known = 'in EclDict' if self.ecl.by_full_type(t) else 'NOT in EclDict'
                r.append(f'    {t:<16} {n:>4}   ({known})')
        if self.unknown_pins:
            r.append('  pins with no role in EclDict (per part):')
            for t, n in self.unknown_pins.most_common(8):
                r.append(f'    {t:<16} {n:>4}')
        return '\n'.join(r)


def known_cells(cells_dir: str) -> set[str]:
    out = set()
    if not os.path.isdir(cells_dir):
        return out
    for f in os.listdir(cells_dir):
        m = re.match(r'cell_(.+)\.v$', f)
        if m:
            out.add(m.group(1))
    return out


CELL_PORT_RE = re.compile(r'^\s*(input|output|inout)\s+wire\s+p(\d+)')
SYS_CLK_RE = re.compile(r'^\s*input\s+wire\s+sys_clk\b')


def cells_wanting_clock(cells_dir: str) -> set[str]:
    """Cells that take the fabric clock.

    A part whose real clock is a distributed ECL net is modelled on `sys_clk`
    with the net as an ENABLE -- see any of them for why. Only those declare
    the port, so the generator asks the file rather than keeping a list that
    could drift from the cell library."""
    out = set()
    if not os.path.isdir(cells_dir):
        return out
    for f in os.listdir(cells_dir):
        m = re.match(r'cell_(.+)\.v$', f)
        if m and any(SYS_CLK_RE.match(l)
                     for l in open(os.path.join(cells_dir, f))):
            out.add(m.group(1))
    return out


def cell_port_dirs(cells_dir: str) -> dict[str, dict[int, str]]:
    """cell module suffix -> {pin: 'input'|'output'|'inout'}.

    The generator needs these because a cell's port direction is GLOBAL --
    aggregated over all sixteen boards -- while a net's classification is
    per-board. A part pin that drives on MemC and is merely sensed on IFU is
    `output` in the shared cell, so a board where nothing drives that net
    would otherwise declare it a module input and then connect an output to
    it: `%Error-ASSIGNIN`. Reading the cells back closes that loop."""
    out: dict[str, dict[int, str]] = {}
    if not os.path.isdir(cells_dir):
        return out
    for f in os.listdir(cells_dir):
        m = re.match(r'cell_(.+)\.v$', f)
        if not m:
            continue
        pins: dict[int, str] = {}
        with open(os.path.join(cells_dir, f)) as fh:
            for line in fh:
                pm = CELL_PORT_RE.match(line)
                if pm:
                    pins[int(pm.group(2))] = pm.group(1)
        out[m.group(1)] = pins
    return out


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('wl')
    ap.add_argument('--dict', required=True, help='EclDict.Analyze')
    ap.add_argument('--lc')
    ap.add_argument('--cells', default='verilog/cells')
    ap.add_argument('-o', '--out')
    ap.add_argument('--report', action='store_true')
    args = ap.parse_args(argv[1:])

    board = load_board(args.wl, args.lc)
    ecl = EclDict()
    ecl.load(args.dict)
    gen = Generator(board, ecl, known_cells(args.cells),
                    cell_port_dirs(args.cells),
                    cells_wanting_clock(args.cells))
    text = gen.emit()

    if args.out:
        os.makedirs(os.path.dirname(args.out) or '.', exist_ok=True)
        with open(args.out, 'w') as fh:
            fh.write(text)
        print(f'wrote {args.out}  ({text.count(chr(10))} lines)')
    else:
        sys.stdout.write(text)

    if args.report:
        print(gen.report(), file=sys.stderr)
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
