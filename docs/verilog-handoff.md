# Verilog from Sil: handoff

2026-08-15. Everything below is committed and pushed. This file is written to
be picked up cold: **Task A** is self-contained Ethernet work, **Task B** is
putting the machine together and testing it. Read `docs/verilog-from-sil.md`
for the background and why each decision was made.

---

## Where it stands

```
make -C verilog boards     # 16 boards from PARC's wire lists   -> generated/
make -C verilog cells      # cell skeletons for any new part    -> cells/
make -C verilog lint       # THE GATE: every board must elaborate
make -C verilog/verilator  # the imgui harness
./verilog/verilator/obj_dir/Vemu --headless --cycles 5000
```

| piece | state |
|---|---|
| Netlist reader + Verilog generator | 16/16 boards, 72,277 lines, **all lint clean** |
| Cell library | 44 cells with behaviour, **82.9%** of 3,771 logic packages |
| 6502 | Andrew Holme's netlist-derived core (via jotego), wired into `cell_MCS6502` |
| 6532 RIOT | MiSTer Atari 7800's, patched for Verilator. **CC BY-NC** -- see `verilog/vendor/LICENSES.md` |
| PROMs | **24 of 26** generated from PARC's BCPL, all self-checked |
| Harness | Verilator + Dear ImGui, builds, runs, `--headless` CI mode |
| Backplane | **no schematic needed** -- derivable from net names, see Task B |

Nothing here computes yet as a *machine*: the boards elaborate and most cells
have behaviour, but no board is instantiated in `sim.v` and nothing is wired
to anything else. Task B is that step.

---

## Task A -- the last two PROMs (self-contained, ~half a day)

`EtherRcvr` and `EtherXmtr` in `chm/doradoproms/EtherProms.bcpl` (Ed Taft,
27-Aug-81). Everything else in that file is already ported.

**Why they were left.** They are the only PROMs that are genuine STATE
MACHINES rather than tables or closed-form functions -- 257 and ~200 lines,
with named states (`idle`, `maybe`, `full`, `imip`), and a silent
transcription error would be both easy to make and hard to notice. They want
attention, not the tail of a long session.

### What to do

1. Read `EtherProms.bcpl` from `and EtherRcvr` to the end. Both follow the
   same shape as `EtherPD`, which IS already ported and is the worked
   example: read the `structure Input:` / `structure Output:` declarations
   for bit positions, then transcribe the body.

2. **Bit positions.** BCPL structures are MSB-first over a 16-bit word. For
   an 8-bit PROM address that puts the first field at bit 7. `EtherRcvr`'s
   input is:

   ```
   blank bit 8 / currentState bit 3 / rxCollision bit / pdCarrier bit
   / pdEvent bit 2 / rxSRFull bit      // *** Low True ***
   ```

   so `currentState` = bits 7:5, `rxCollision` = 4, `pdCarrier` = 3,
   `pdEvent` = bits 2:1, `rxSRFull` = 0. **Watch the low-true fields** --
   the source marks them, and `rxSRFull` is read as `... eq 0`.

3. Add the generators to `tools/dorado_proms.py` beside `make_ether_pd`,
   register them in `GENERATORS`, and run `--check`. The size check will
   confirm 256 words and that nothing exceeds 12 bits.

4. **Add a property check**, as every other PROM has. For a state machine the
   natural ones are: every state must be reachable; every state must have a
   defined next state for all inputs (no accidental hole); and the source's
   own comments name what each state means, so assert the transitions it
   describes. See how `EtherFifo` and `EtherPD` are checked -- those found
   nothing, but the size check found two real errors in the display batch,
   including a bug in the tool's own parser.

### Definition of done

```
python3 tools/dorado_proms.py --check      # 26 of 26, 26/26 sized
python3 tools/dorado_proms.py --emit-all --out verilog/proms
```

---

## Task B -- putting it together, and testing

This is the interesting one, and it splits into four steps that can land
separately. **Do them in this order** -- each makes the next testable.

### B1. Generate the backplane (no schematic required)

Established this session, and it is the finding that unblocks everything:
each board's `Term100` packages ARE its backplane connectors, and every one
carries a NAMED net. Measured across all sixteen boards:

- 9,145 nets reach a connector pin (7,971 distinct names)
- **964 names appear on more than one board** -- those are the backplane
  signals, and they are recognisably the real buses (`DMadr.01-11` across
  exactly DskEth/IFU/MemC/MemD/MemX/ProcH/ProcL; `clk1'Aa` clock
  distribution; `prepreFH'` across seven boards)
- only **67 of 964** sit at the same connector PIN on every board

That last number is the point: the backplane is **not** straight-through, it
routes pin X on one slot to pin Y on another. So the physical pin mapping is
a PCB detail, and **for RTL we want the names**. Connecting boards by signal
name gives the correct logical netlist.

Write `tools/sil_backplane.py` emitting a top module that instantiates all
sixteen boards and wires the shared signals. Two cautions:

- **Filter power and ground** before trusting a match (`GND*`, `VCC*`,
  `VEE*`, `TTLHigh`, terminator nets).
- **Eyeball the low-fanout matches.** Name-matching assumes PARC was
  consistent across boards; `DMadr.NN` strongly suggests they were, but a
  name shared by exactly two boards deserves a look before it becomes a wire.

The starting query is in this session's history:

```python
# nets that reach a Term100 pin, per board; names on >1 board are backplane
```

### B2. Instantiate boards in `sim.v`

`verilog/verilator/sim.v` is MiSTer's `emu` and deliberately instantiates
NOTHING yet -- a comment there says why. Start with **ProcH + ProcL**
(they are already verilated in, and the processor is the part the C emulator
knows best), then add MemC/MemD/MemX, then Control A/B and the IFU.

### B3. Wire the PROMs in

24 of 26 PROM contents are generated into `verilog/proms/*.mem` as one hex
word per line -- what `$readmemh` wants. `cell_MB7071H` is still a skeleton;
give it a `$readmemh`-initialised array and pass the filename as a parameter,
so each of the 70 PROM packages gets the right contents by position. The
map is `python3 tools/dorado_proms.py --list`.

### B4. Test against the C emulator

This is the reason the whole approach is worth it: **we have a working
software Dorado to diff against**, and the two were derived from different
sources 45 years apart.

Cheapest first:

1. **PROM contents vs emulator behaviour.** Already demonstrated: LMASK/RMASK
   generated from PARC's BCPL match the shifter masks the C emulator computes
   from the Hardware Manual. The same trick is available for the memory
   system (`EC-1`/`EC-2`'s four cache operations against `memory.c`), the
   disk (a sector is four blocks), and the keyboard (`Keyboard-Map` against
   `display.c`'s 61-key matrix, which has been checked three other ways).
   **Note the keyboard one is a layer question**: the PROM is what the IFU
   applies, `display.c` is what the terminal delivers, so a mismatch would be
   interesting rather than automatically a bug.
2. **Clock a board and compare a signal.** Once ProcH is instantiated, drive
   a known microinstruction and compare an ALU output against the C
   emulator's at the same point.
3. **Boot something.** A long way off, and the honest ordering is 1, 2, then
   worry about 3.

---

## Things that will bite, learned the hard way this session

- **`make -C verilog lint` after every change.** It is seconds, and it caught
  every structural error here -- including two of mine in one batch.
- **Verilator compiles inside `obj_dir`**, so relative `-CFLAGS` include
  paths need `../`.
- **A comment line beginning with the tool's own name is parsed as a pragma**
  (`BADVLTPRAGMA`). Do not start a comment with it.
- **Hand-written cells must declare every pin the BOARDS wire**, not just the
  ones the dictionary names -- p1/p8/p16 are power on a 16-pin MECL package,
  and omitting them is `PINNOTFOUND`.
- **Net names are not unique after naive sanitising.** `CTask.0` and
  `CTask=0` are different nets on ContA and both mapped to `CTask_0`; the
  mapping is injective now and verified collision-free, but the same trap
  waits in any new name mangling.
- **Cell port directions are global; net classification is per-board.** A pin
  that drives on MemC and is only sensed on IFU is `output` in the shared
  cell.
- **Check a transcription structurally, not by eye.** Every PROM generator
  has a property test for this reason, and they have already paid for
  themselves twice.

---

## The one open licence question

`verilog/vendor/riot/M6532.sv` is **CC BY-NC 4.0** -- more restrictive than
anything else here. DECISION: keep it, the project is not shipping
commercially, and the note exists so the constraint is known rather than
discovered. If it ever binds, the preferred fix is porting our own:
`dorado/src/baseboard.c` already models the 6532 well enough to run the real
BaseBoard EPROMs, so it is a validated spec written in this repository with
no third-party licence at all.
