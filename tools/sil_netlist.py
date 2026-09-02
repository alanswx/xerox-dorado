#!/usr/bin/env python3
"""Read PARC's Sil design-automation output for the Dorado boards.

`chm/sil/` holds the output of Xerox's own design-automation tools for all
sixteen boards -- the input the machine was actually built from, not a
reconstruction. Two files per board carry the whole structural design:

  <board>.wl   the WIRE LIST: every net by name, and every package pin it
               touches, WITH DIRECTION -- `f17.15o` drives, `g17.12i`
               receives. This is a gate-level netlist in plain text.
  <board>.lc   the LOCATION/part list: which part type sits in each package
               position (`MC10173/16/E1G:` then `a12 a13 b05 ...`).

Together they give a complete structural netlist: parts, pins, directions and
connectivity. That is the input to Phase 2 (Verilog); see
docs/verilog-from-sil.md.

FILE FORMAT, as observed (there is no surviving format document):

  * The files are **CR-terminated** (Xerox), not LF. Split on \\r.
  * `;`-prefixed lines are comments -- a per-page provenance header
    (`;File=ProcH01.sil Rev=Ce Date=11/15/78 Page=01 MARKED BUILT`).
  * A lone `L` line appears at the top; it is not data.
  * The PACKAGE section lists one package per line:
        a01: (MC10102/16/E) ; 5,7,9,10,11,12,13,15
    i.e. position, (partType/pinCount/variant), then a pin list. The trailing
    list is NOT connectivity -- it repeats for identical spare sockets and is
    recorded here as `note_pins` rather than guessed at.
  * The NET section is a record per net:
        Ain.00: <406> (93)
            c41.6i {89,34}   c02.11i {89,52}   b03.4i {61,58}   b05.1o {73,100}
    header = `NAME: <wireLength> (netId)`, then indented pin references
    `pkg.pin{i|o} {x,y}` where x,y is the physical position on the board.
  * Some sections are not nets at all but manufacturing instructions --
    `DISCONNECT:`, `CALIBRATE:` -- whose bodies are bare point names with no
    `.pin` and no direction. They are skipped by requiring the pin syntax.

Usage:
    sil_netlist.py <board.wl> [--lc <board.lc>] [--json OUT] [--summary]
    sil_netlist.py --all chm/sil            # summarise every board
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from collections import Counter

# `c41.6i {89,34}` -- package, pin number, direction, and board position.
PIN_RE = re.compile(r'\b([a-z]+\d+)\.(\d+)([io])\b(?:\s*\{(-?\d+),(-?\d+)\})?')
# A backplane connector pin on a net line: `E179`, `C96`. Package positions are
# lowercase (PKG_RE) and coordinates carry no letters, so this cannot collide.
CONN_RE = re.compile(r'\b[CE]\d+\b')
# `Ain.00: <406> (93)` -- net name, total wire length, net id.
NET_HEAD_RE = re.compile(r'^(\S+):\s*<(\d+)>\s*(?:\((\d+)\))?\s*$')
# `a01: (MC10102/16/E) ; 5,7,9,10`
PKG_RE = re.compile(r'^([a-z]+\d+):\s*\(([^)]*)\)\s*;\s*(.*)$')
# `MC10173/16/E1G:` heading in the .lc file
LC_HEAD_RE = re.compile(r'^(\S+/\S+/\S+):\s*$')


def vpart(part_type: str) -> str:
    """Part type -> a legal Verilog module-name suffix.

    Real part numbers are not identifiers: `LM3911+20K`, `MK4096P-6`,
    `SIP package`. Sanitise once, here, so the generator and the cell emitter
    cannot disagree about what a cell is called -- a mismatch shows up as a
    missing module at elaboration, long after the point of the mistake."""
    s = re.sub(r'[^A-Za-z0-9_]', '_', part_type)
    if not s or s[0].isdigit():
        s = 'p_' + s
    return s


def read_xerox_text(path: str) -> list[str]:
    """Xerox files are CR-terminated; normalise to a list of lines."""
    with open(path, 'rb') as fh:
        raw = fh.read().decode('latin-1')
    return raw.replace('\r\n', '\n').replace('\r', '\n').split('\n')


# SIX BACKPLANE LINES ARE SPELLED TWO WAYS, and one of them is the memory
# section's hold on the processor.
#
# PARC's draughtsmen were not consistent about capitalisation, and this
# backplane is wired BY NAME -- it is not straight-through, 182 pin positions
# carry different nets on different boards -- so a spelling difference silently
# leaves a line unconnected. `PrHold` on MemC and `PRhold` on ProcH/ProcL are
# ONE WIRE: `#07-E.42`, `#s05-E.42`, `#s04-E.42`, same connector and pin in
# three slots. Until this, MemC drove `PrHold` into nothing and the processor
# read `PRhold` from nothing, so the memory section could not hold the
# processor at all.
#
# CASE-INSENSITIVE MATCHING WOULD BE WRONG, which is why this is a table.
# Among backplane nets there are TEN case-variant groups and THREE do not
# agree on a pin: `CLKEnable'a` (C16) vs `ClkEnable'a` (C8), and the two IO
# pairs. Those are separate lines that merely look alike.
#
# THE TWO IO GROUPS ARE SUBTLER THAN THIS COMMENT USED TO CLAIM, and
# `--case-variants` reading the .bp files directly is what corrected it. It
# said "`IOIn'` (E70) vs `IOin'` (E71), `IOOut'` (E71) vs `IOout'` (E74)",
# which has the first pair backwards and hides the real shape. Measured:
#
#     IOIn'   DispM, DispY            E71
#     IOin'   DskEth, Music           E71   <- SAME pin as IOIn'
#     IOin'   ProcL                   E70   <- the odd one
#     IOOut'  DispM, DispY            E74
#     IOout'  DskEth, IOTest, Music   E74   <- SAME pin as IOOut'
#     IOout'  ProcL                   E71   <- the odd one
#
# So the spellings mostly AGREE; it is PROCL that sits on different pins from
# everyone else. The group still must not be merged -- three pins are in play
# -- but not for the reason the comment gave. Outside the backplane it would be far worse: 63 net
# names differ only by case, mostly per-board LOCAL clock fan-out such as
# `Clk0'Aa` on MemX against `clk0'Aa` on IFU, and merging those would tie every
# board's clock distribution together.
#
# So the rule is narrow: merge a case variant ONLY where every board that uses
# it agrees on the pin. That yields exactly these SEVEN -- six, plus
# `ChipsAre64k` which only came into view when the MSA was first wired into a
# machine. No board carries both spellings, so the rename cannot collide with
# a local net.
# `tools/sil_backplane.py --case-variants` re-derives the list from the .bp
# files and fails if it no longer matches.
BACKPLANE_CASE_ALIASES = {
    'PrHold':        'PRhold',        # E42  MemC -> ProcH, ProcL
    'MxHold':        'MXHold',        #      MemX <-> MemC
    'HoldMapbuf':    'HoldMapBuf',    #      MemX <-> MemC
    'Subtask.0':     'SubTask.0',     #      MemX <-> ProcL, DispY
    'Subtask.1':     'SubTask.1',     #      MemX <-> ProcL
    'FoutSubtask.0': 'FoutSubTask.0', #      MemX <-> DispY
    # Found when the MSA was first wired into a machine (2026-08-23). MemX
    # spells it `ChipsAre64K` and the storage board `ChipsAre64k`; both sit on
    # backplane pin E55, so they are ONE WIRE and the case rule applies
    # exactly as it does above.
    'ChipsAre64k':   'ChipsAre64K',   # E55  msa <-> MemX
}


# NOT a case variant, and so NOT merged here: MemX's `ChipsAre256/16K` and the
# MSA's `ChipsAre16k` are both on pin E54 and are plainly the same signal --
# "the storage is built from 16K chips" -- but they differ by more than
# capitalisation, and this table's whole discipline is that a NAME is the
# connection. PcMsa puts a third name on the same pin (`ChipsAreA`, with
# `ChipsAreB` on E55), which is what a pin-based merge would have to
# reconcile, and this backplane has already shown that one pin position
# carries different signals on different boards. Recorded as evidence, not
# acted on: see docs/verilog-handoff.md.


# THE TASK WAKEUPS ARE ROUTED BY A BACKPLANE JUMPER, not by matching names or
# pins, and that is why none of them was connected. Every I/O board puts its
# wakeup request on the SAME two connector pins -- C120 and C121 -- under its
# own local name, and ContA receives fifteen lines `TWReq.01`..`TWReq.15` on
# quite different pins of its own (C44..C140). `Backplane.pdf` (BPRight04.sil,
# 7/24/80) shows pins 120/121 carrying `TWReq.xx*` across the generic I/O
# slots, with the note "for desired Task wake up" -- the `xx` is chosen when
# the machine is wired. The display slots get dedicated names on those pins,
# `DispMwtTW`/`DispMhtTW` and `DispYwtTW`/`DispYhtTW`, word task and head task.
#
# So a board's task number is a property of its SLOT, which is exactly why each
# board also carries a task-number STRAP (see SIP_BROKEN_LEGS in
# sil_to_verilog.py) -- the strap tells the board what the jumper tells the
# backplane, and the two have to agree. That makes the strap the authority for
# which line to wire, and the mapping self-checking:
#
#   DispM  WakeAWT  -> TWReq.09   AltoWTask strap = 1001 = 9; DispM30.sil says
#                                 outright "for Task 9D = 11B"
#   DispY  WakeDWT  -> TWReq.11   DWTTask strap = 1011 = 11
#   MemX   TWReq15  -> TWReq.15   the fault task is task 15 -- HM section 4.1
#                                 ("task 0 = emulator, task 15 = fault task")
#                                 and the C emulator's include/memory.h, and
#                                 MemX's own net name says 15
#
#   DispY  WakeDHT  -> TWReq.03   the display HEAD task
#   DispM  WakeAHT  -> TWReq.04   DispM's terminal-interface head task
#
# THE TWO HEAD TASKS ARE NOW WIRED, and the source cross-checks itself. The C
# emulator's include/display.h names all four display tasks:
#
#     DHT  task 3     Display Horizontal Task (low priority)
#     DWT  task 13o   Display Word Task (high priority)
#     AHT  task 4     DispM terminal-interface horizontal task
#     AWT  task 11o   DispM terminal-interface word task
#
# and 13o = 11, 11o = 9 -- EXACTLY the two word-task numbers the boards' own
# straps give (DispY DWTTask = 1011, DispM AltoWTask = 1001). Two independent
# derivations agreeing on the pair we already knew is what makes the pair we
# did not -- the head tasks, 3 and 4 -- safe to take from the same table.
#
#   DskEth WakeEthTx -> TWReq.06   the Ethernet OUTPUT task
#   DskEth WakeEthRx -> TWReq.07   the Ethernet INPUT task
#
# THE ETHERNET PAIR IS WIRED NOW TOO, from include/ethernet.h:
#
#     DORADO_ETHERNET_TASK_EOT  06     Ethernet Output Task -- Tx
#     DORADO_ETHERNET_TASK_EIT  07     Ethernet Input  Task -- Rx
#
# DskEth's own strap cannot settle these: unlike the display boards' it is an
# I/O ADDRESS (TIOA-Ad = 1 -> 010-017), not a task number, so the numbers have
# to come from elsewhere -- and ethernet.h is a genuinely independent source,
# registering its slow-IO handlers on exactly those two tasks.
#
#   DskEth DiskTW    -> TWReq.12   the DSK task
#
# AND THE DISK'S WAKEUP IS WIRED TOO -- it was there all along under a
# DIFFERENT NAMING CONVENTION. An earlier version of this note said DskEth
# drove no disk wakeup net; that was a search for `Wake*`, and this board uses
# `*TW` (Task Wakeup) instead. It carries nine of them:
#
#     ClearIndexTW  ClearSectorTW  DiskTW  IndexTW  RdFifoTW
#     SectorTW      SeekTagTW      SetTagTW  WrFifoTW
#
# and exactly ONE leaves the board: DiskTW, driven by e04 (an MC10231). The
# other eight are internal, which is what makes DiskTW identifiable as the
# backplane wakeup rather than one of the board's own strobes.
#
# Its number comes from include/disk.h's DORADO_DISK_TASK = 014 octal = 12
# decimal, the same file whose DISK_TIOA_DISKCONTROL 010 independently
# confirms the TIOA-Ad strap.
#
# TWO NAMING CONVENTIONS FOR THE SAME THING is worth remembering: the display
# boards say `WakeDWT`, DskEth says `DiskTW`. Search for both.
#
#   IFU    JunkTW    -> TWReq.02   the JUNK task. cpu.c's DORADO_JUNK_TASK
#                                  = 2, and the IFU board holds the junk
#                                  TIMER (a18 drives JunkTW; the Wakeup[JNK]
#                                  and AckJunkTW FFs control it). Unwired,
#                                  Initial's TASKINITLOOP spins forever at
#                                  c44/c60/c46/c47 -- TaskingOn, get the
#                                  init PC, test, repeat -- waiting for the
#                                  junk task's first run, measured as
#                                  "TASKINITLOOP entries 1" over 600M
#                                  cycles with CTask never changing.
BACKPLANE_WAKEUP_JUMPERS = {
    'JunkTW':  'TWReq.02',
    'WakeAWT': 'TWReq.09',
    'WakeDWT': 'TWReq.11',
    'WakeDHT': 'TWReq.03',
    'WakeAHT': 'TWReq.04',
    'WakeEthTx': 'TWReq.06',
    'WakeEthRx': 'TWReq.07',
    'DiskTW':    'TWReq.12',
    'TWReq15': 'TWReq.15',
}


# ONE WIRE, TWO NAMES -- and not a case variant. ContA drives its task-switch
# select onto backplane pin E166 as `SWb` (j19, a buffered copy of Switch'a);
# ContB receives that pin as `SW` (k21, the fan-out to the e20/f20/g20/g21/h20
# MC1662 muxes that pick BNPC over TNIA for the IM address). Both boards state
# E166 in their .bp files, so by the backplane's own rule they are one wire --
# but this backplane is wired by NAME, and unjoined, ContB's SW sat at 0: a
# task switch loaded CIA and CTask with the new task while the IM went on
# reading the OLD task's next address, so the first instruction of every
# switched-in task was the old task's (measured 2026-09-02: CIA=c61 with
# IM[c00]'s fields in the MIR, right after Initial's task init). The rename is
# BOARD-SCOPED because ContB also has a LOCAL net spelled `SWb` (k21's fan-out
# copy), which a global alias in either direction would collide with.
# (E167 is the same shape -- ContA `SWm`, ContB `TNIA.03` -- but on ContB that
# pin reaches only the muffler mux e23, so it is left alone.)
BACKPLANE_BOARD_ALIASES = {
    ('ContA', 'SWb'): 'SW',      # E166  ContA j19 -> ContB k21
}


def canon_net(name: str, board: str | None = None) -> str:
    """The canonical spelling of a backplane net.

    Three corrections: a handful of lines PARC capitalised inconsistently
    (BACKPLANE_CASE_ALIASES), one line a board names differently from the
    board at the other end of the pin (BACKPLANE_BOARD_ALIASES, applied only
    on the board named), and the task wakeups, which the backplane routes by
    jumper rather than by name (BACKPLANE_WAKEUP_JUMPERS)."""
    if board is not None:
        name = BACKPLANE_BOARD_ALIASES.get((board.split('-Rev')[0], name), name)
    name = BACKPLANE_CASE_ALIASES.get(name, name)
    return BACKPLANE_WAKEUP_JUMPERS.get(name, name)


class Board:
    def __init__(self, name: str):
        self.name = name
        self.packages: dict[str, dict] = {}   # pos -> {type,pins,variant,...}
        self.nets: dict[str, dict] = {}       # net name -> {id,length,pins[]}
        self.provenance: list[str] = []
        self.skipped_sections: list[str] = []
        # net -> {'C96','E179'}: the backplane connector pins it reaches, i.e.
        # exactly the nets that leave the board. See load_bp.
        self.backplane: dict[str, set[str]] = {}
        self.bp_source = 'wl'
        self.bp_mismatch: list[tuple[str, set, set]] = []

    # ---- .wl -----------------------------------------------------------

    def load_wl(self, path: str) -> None:
        lines = read_xerox_text(path)
        net = None
        net_name = None
        for line in lines:
            if not line.strip():
                continue
            if line.startswith(';'):
                self.provenance.append(line[1:].strip())
                continue
            if line.strip() == 'L':
                continue

            indented = line[0].isspace()

            if not indented:
                m = PKG_RE.match(line.strip())
                if m:
                    pos, desc, note = m.group(1), m.group(2), m.group(3)
                    bits = desc.split('/')
                    self.packages[pos] = {
                        'type': bits[0] if bits else desc,
                        'pin_count': int(bits[1]) if len(bits) > 1 and
                                     bits[1].isdigit() else None,
                        'variant': bits[2] if len(bits) > 2 else None,
                        'note_pins': [int(p) for p in note.replace(' ', '').split(',')
                                      if p.strip().isdigit()],
                    }
                    net = None
                    continue
                m = NET_HEAD_RE.match(line.strip())
                if m:
                    net_name = canon_net(m.group(1), self.name)
                    net = {'id': int(m.group(3)) if m.group(3) else None,
                           'length': int(m.group(2)), 'pins': []}
                    self.nets[net_name] = net
                    continue
                # A section that is not a net: DISCONNECT, CALIBRATE, ...
                if line.rstrip().endswith(':') or ': <' in line or ':' in line:
                    label = line.split(':', 1)[0].strip()
                    if label and label not in self.skipped_sections:
                        self.skipped_sections.append(label)
                net = None
                continue

            # Indented: pin references for the current net.
            if net is None:
                continue
            for m in PIN_RE.finditer(line):
                pkg, pin, dirn, x, y = m.groups()
                net['pins'].append({
                    'pkg': pkg,
                    'pin': int(pin),
                    'dir': 'out' if dirn == 'o' else 'in',
                    'xy': [int(x), int(y)] if x is not None else None,
                })
            # A bare `E179 {457,525}` among the pins is a BACKPLANE CONNECTOR
            # pin, not a package: the net leaves the board there. Package
            # names are lowercase (PKG_RE), so an uppercase C/E followed by
            # digits is unambiguous. These used to be dropped, and the
            # generator inferred the same fact from Term100 packages instead
            # -- which is wrong twice over, because Term100 is a 100-ohm
            # TERMINATOR, not a connector.
            for m in CONN_RE.finditer(line):
                net.setdefault('backplane', set()).add(m.group(0))
                self.backplane.setdefault(net_name, set()).add(m.group(0))

    # ---- .lc -----------------------------------------------------------
    def load_lc(self, path: str) -> None:
        """Part type -> positions. Confirms/extends what .wl already said."""
        lines = read_xerox_text(path)
        cur = None
        for line in lines:
            if not line.strip():
                continue
            m = LC_HEAD_RE.match(line.strip())
            if m:
                cur = m.group(1)
                continue
            if cur and line[0].isspace():
                for pos in line.split():
                    bits = cur.split('/')
                    entry = self.packages.setdefault(pos, {})
                    entry.setdefault('type', bits[0])
                    entry.setdefault('pin_count',
                                     int(bits[1]) if len(bits) > 1 and
                                     bits[1].isdigit() else None)
                    entry.setdefault('variant', bits[2] if len(bits) > 2 else None)
                    entry['lc_type'] = cur

    # ---- .bp -----------------------------------------------------------
    def load_bp(self, path: str) -> None:
        """The BACKPLANE file: `ALUCarry: E179`, one line per net that leaves
        the board. This is the module's port list, stated by the tool that
        built the machine rather than inferred from the wiring.

        The same fact is stated THREE times in the archive and all three
        agree: here, as the bare `E179` tokens in the `.wl` (parsed above),
        and slot-qualified in `-C.nl`/`-E.nl` (`#s05-C.5`). Measured across
        all sixteen boards, 2,052 of 2,054 pins are identical; the five
        exceptions are ground nets that the `.wl` numbers individually
        (`GND-26`) and the `.bp` collapses (`GND`).

        `.bp` wins where they differ because it is the file whose only job is
        this, but any disagreement is recorded in `bp_mismatch` rather than
        hidden -- if these two ever diverge on a signal, something is wrong
        with an assumption, not with the machine."""
        from_wl = {n: set(v) for n, v in self.backplane.items()}
        from_bp: dict[str, set[str]] = {}
        for line in read_xerox_text(path):
            if ':' not in line:
                continue
            name, pins = line.split(':', 1)
            name = canon_net(name.strip(), self.name)
            name = name.strip()
            if not name:
                continue
            from_bp[name] = {p.strip() for p in pins.split(',') if p.strip()}
        for name in sorted(set(from_wl) | set(from_bp)):
            if from_wl.get(name, set()) != from_bp.get(name, set()):
                self.bp_mismatch.append((name, from_wl.get(name, set()),
                                         from_bp.get(name, set())))
        self.backplane = from_bp
        self.bp_source = 'bp'

    # ---- derived -------------------------------------------------------
    def drivers_of(self, net: str) -> list[dict]:
        return [p for p in self.nets[net]['pins'] if p['dir'] == 'out']

    def leaves_board(self, net: str) -> bool:
        """Does this net reach a backplane connector?"""
        return net in self.backplane

    def check(self) -> dict:
        """Structural sanity, reported rather than asserted -- a 1979 board
        legitimately has open-emitter wired-OR nets with several drivers."""
        multi, undriven, dangling = [], [], []
        for name, net in self.nets.items():
            outs = sum(1 for p in net['pins'] if p['dir'] == 'out')
            if outs == 0:
                undriven.append(name)
            elif outs > 1:
                multi.append((name, outs))
            if len(net['pins']) < 2:
                dangling.append(name)
        used = {p['pkg'] for n in self.nets.values() for p in n['pins']}
        return {
            'nets': len(self.nets),
            'packages': len(self.packages),
            'pin_refs': sum(len(n['pins']) for n in self.nets.values()),
            'multi_driver_nets': sorted(multi, key=lambda t: -t[1])[:20],
            'multi_driver_count': len(multi),
            'undriven_nets': len(undriven),
            'single_pin_nets': len(dangling),
            'packages_with_no_net': sorted(set(self.packages) - used)[:20],
            'part_types': Counter(p.get('type', '?')
                                  for p in self.packages.values()),
        }

    def to_dict(self) -> dict:
        return {'board': self.name, 'packages': self.packages,
                'nets': self.nets, 'skipped_sections': self.skipped_sections}


def load_board(wl_path: str, lc_path: str | None = None,
               bp_path: str | None = None) -> Board:
    name = os.path.basename(wl_path).split('.')[0]
    b = Board(name)
    b.load_wl(wl_path)
    if lc_path is None:
        guess = wl_path[:-3] + '.lc'
        if os.path.exists(guess):
            lc_path = guess
    if lc_path and os.path.exists(lc_path):
        b.load_lc(lc_path)
    # The .bp sits beside the .wl, except on msa where the board directory is
    # `msa-Rev-Bg` but the files are plain `msa.*`; glob rather than assume.
    if bp_path is None:
        guess = wl_path[:-3] + '.bp'
        if not os.path.exists(guess):
            d = os.path.dirname(wl_path)
            cand = [f for f in sorted(os.listdir(d)) if f.endswith('.bp')]
            guess = os.path.join(d, cand[0]) if cand else None
        bp_path = guess
    if bp_path and os.path.exists(bp_path):
        b.load_bp(bp_path)
    return b


def find_boards(root: str) -> list[tuple[str, str | None]]:
    out = []
    for dirpath, _dirs, files in os.walk(root):
        for f in sorted(files):
            if f.endswith('.wl'):
                wl = os.path.join(dirpath, f)
                lc = wl[:-3] + '.lc'
                out.append((wl, lc if os.path.exists(lc) else None))
    return sorted(out)


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('wl', nargs='?', help='board .wl wire list')
    ap.add_argument('--lc', help='board .lc part list (default: alongside)')
    ap.add_argument('--json', help='write the parsed netlist as JSON')
    ap.add_argument('--summary', action='store_true')
    ap.add_argument('--all', metavar='SILDIR', help='summarise every board')
    ap.add_argument('--net', help='print one net in full')
    args = ap.parse_args(argv[1:])

    if args.all:
        boards = find_boards(args.all)
        if not boards:
            print(f'no .wl files under {args.all}', file=sys.stderr)
            return 1
        tot_n = tot_p = tot_r = 0
        types: Counter = Counter()
        print(f'{"board":<24} {"nets":>6} {"pkgs":>6} {"pinrefs":>8} '
              f'{"multi-drv":>10} {"undriven":>9}')
        for wl, lc in boards:
            b = load_board(wl, lc)
            c = b.check()
            tot_n += c['nets']; tot_p += c['packages']; tot_r += c['pin_refs']
            types.update(c['part_types'])
            print(f'{b.name:<24} {c["nets"]:>6} {c["packages"]:>6} '
                  f'{c["pin_refs"]:>8} {c["multi_driver_count"]:>10} '
                  f'{c["undriven_nets"]:>9}')
        print(f'{"TOTAL":<24} {tot_n:>6} {tot_p:>6} {tot_r:>8}')
        print(f'\ndistinct part types across all boards: {len(types)}')
        real = {t: n for t, n in types.items()
                if not t.startswith(('SpareSocket', 'Term'))}
        print(f'excluding spares/terminators:          {len(real)} types, '
              f'{sum(real.values())} packages')
        print('\nmost common:')
        for t, n in types.most_common(15):
            print(f'  {t:<20} {n:>5}')
        return 0

    if not args.wl:
        ap.print_help()
        return 1

    b = load_board(args.wl, args.lc)

    if args.net:
        net = b.nets.get(args.net)
        if not net:
            print(f'no net {args.net!r}', file=sys.stderr)
            return 1
        print(f'{args.net}: id={net["id"]} length={net["length"]} '
              f'pins={len(net["pins"])}')
        for p in net['pins']:
            pkg = b.packages.get(p['pkg'], {})
            print(f'  {p["pkg"]}.{p["pin"]:<3} {p["dir"]:<3} '
                  f'{pkg.get("type","?"):<16} at {p["xy"]}')
        return 0

    if args.json:
        with open(args.json, 'w') as fh:
            json.dump(b.to_dict(), fh, indent=1)
        print(f'wrote {args.json}')

    if args.summary or not args.json:
        c = b.check()
        print(f'board {b.name}')
        print(f'  nets                {c["nets"]}')
        print(f'  packages            {c["packages"]}')
        print(f'  pin references      {c["pin_refs"]}')
        print(f'  nets with >1 driver {c["multi_driver_count"]}  '
              f'(wired-OR is legal in MECL 10K)')
        print(f'  nets with 0 drivers {c["undriven_nets"]}  '
              f'(board inputs arrive on backplane pins)')
        print(f'  single-pin nets     {c["single_pin_nets"]}')
        if b.skipped_sections:
            print(f'  non-net sections    {", ".join(b.skipped_sections)}')
        print(f'  distinct part types {len(c["part_types"])}')
        for t, n in c['part_types'].most_common(10):
            print(f'    {t:<20} {n:>4}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
