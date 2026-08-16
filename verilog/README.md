# Dorado RTL, generated from PARC's own design data

The Verilog here is **generated**, not written: `chm/sil/` holds the output of
Xerox's own Sil design-automation tools for all sixteen Dorado boards -- the
input the machine was actually built from -- and `tools/sil_*.py` turn it into
structural RTL.

```
make -C verilog boards     # regenerate generated/*.v from chm/sil
make -C verilog cells      # emit skeletons for any part with no model yet
make -C verilog lint       # every board must still elaborate (the gate)
```

## Layout

```
verilog/
  cells/          one model per part type (125). Ports GENERATED from PARC's
                  EclDict/TtlDict + observed pin directions; behaviour is
                  hand-written per part. Regenerating never overwrites a
                  model that has a body -- use `make cells-force` on purpose.
  generated/      one module per board (16). DO NOT EDIT -- regenerate.
  verilator/      Verilator + Dear ImGui harness, from the Apple-IIgs MiSTer
                  `vsim` framework, with its sim/ support library intact.
```

## State (2026-08-15)

- **16/16 boards elaborate** under Verilator (67,960 lines), and
  `generated/dorado_backplane.v` wires eleven of them into a machine: 501
  internal nets, 83 of them `wor` (ECL open-emitter buses), 407 ports out to
  cables and to boards this configuration lacks. `make backplane MACHINE=--boards=ProcH,ProcL` for any subset.
- **47 cells have behaviour: 83.8%** of 3,771 logic packages (the three
  PROM parts joined the list, 31 packages). The rest are
  skeletons with correct ports.
- **The 6502 and the RIOT are real cores** -- Andrew Holme's netlist-derived
  6502 (via jotego) and MiSTer's Atari 7800 6532. See `vendor/LICENSES.md`:
  the RIOT is CC BY-NC.
- **All 26 PROMs** are generated from PARC's own BCPL into `proms/*.mem`,
  each with a property check (`tools/dorado_proms.py --check`). The 29
  packages that hold them (`SG10139`, `MCM10149`, `SN74S288`) are wired into
  the boards with per-package images from `proms/packages/`, and
  `make prom-test` checks the readback against the closed forms the Hardware
  Manual and the C emulator use.
- The harness builds and runs: `make -C verilator`, then
  `./obj_dir/Vemu --headless --cycles 5000`.
- **Board ports come from PARC's own `.bp` file**, not from inference:
  `<Board>.bp`, the `.wl`'s own `E179` tokens and `-C.nl`/`-E.nl` state the
  same port list three times and agree on 2,052 of 2,054 pins.
  `python3 tools/sil_backplane.py` reports the backplane;
  `--ports` is the gate that the generated modules match it.

**The machine is assembled but does not compute yet**: no board is instantiated in `sim.v`
and nothing is wired to anything else. That is the next step, and it does
NOT need the backplane schematic -- see the handoff.

- **`docs/verilog-handoff.md`** -- pick up from cold: the two remaining
  Ethernet PROMs, then wiring the machine together and testing it.
- `docs/verilog-from-sil.md` -- the full account and why each decision was
  made (including why pin NAMES come from PARC's dictionary while pin
  DIRECTIONS come from the wire lists).
