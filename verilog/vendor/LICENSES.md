# Vendored cores and their licences

Read this before shipping anything built from `verilog/`.

## `6502/chip_6502.v` -- permissive, with attribution

Andrew Holme's direct conversion of the MOS6502 to a Verilog netlist
(<http://www.aholme.co.uk/6502/Main.htm>), taken from jotego's `jtcores`
(`modules/jtframe/hdl/cpu/chip_6502.v`), which consolidated it into one file
and made it Verilator-friendly. The header records "Used with the author
permission". Keep the attribution header intact.

1,725 nodes -- it is the chip, not a behavioural model, which is the right
shape for this project: the Dorado RTL is generated from PARC's own wire
lists rather than reimplemented, so its CPU should be a netlist too.

## `riot/M6532.sv` -- **CC BY-NC 4.0, NONCOMMERCIAL**

MiSTer's Atari 7800 RIOT (`rtl/RIOT/M6532.sv`, k7800 by Jamie Blanks). The
7800 core's RIOT is SystemVerilog; the 2600 core's is VHDL, which Verilator
cannot compile -- hence this one.

**This licence is more restrictive than the rest of the repository.**
CC BY-NC forbids commercial use, which is not a condition the other sources
here impose. Three ways out, if that matters:

1. Ask Jamie Blanks for a relicence for this use.
2. Find or write a permissively-licensed 6532. The part is small -- 128 bytes
   of RAM, two 8-bit ports with data-direction registers, an interval timer
   with a /1 /8 /64 /1024 prescaler, and edge-detect on PA7.
3. **Port our own.** `dorado/src/baseboard.c` already models the 6532 well
   enough to run the real BaseBoard EPROMs and drive the Dorado boot
   handshake, so it is a validated behavioural spec written in this
   repository -- arguably the best starting point of the three, and it
   carries no third-party licence at all.

Patched for Verilator: ports assigned inside always blocks are declared
`output logic` rather than `output wire`. Quartus infers that; Verilator
follows IEEE 1800 strictly (PROCASSWIRE). No behavioural change.
