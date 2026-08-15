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

## State

- **16/16 boards elaborate** under Verilator (72,277 lines).
- **No cell has behaviour yet** -- all 125 are correct ports with a `TODO`
  body, so the boards build but do not compute. That is the next work, and
  usage gives the order: **48 logic types cover 90% of logic packages.**
- The harness needs `sim.v` (the MiSTer `emu` wrapper) and `sim_main.cpp`
  before `make` links; `make lint` is the working gate today.

Full account, including the two bugs elaboration caught and why pin NAMES
come from the dictionary while pin DIRECTIONS come from the wire lists:
`docs/verilog-from-sil.md`.
