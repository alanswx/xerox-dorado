# Quartus fit for the eleven-board Dorado

The capacity case for this design has been ARITHMETIC so far: 2,824 cell
instances against a DE10-Nano's 41,910 ALMs, and 246 M10K worst case against
557 available (`tools/sil_ram_groups.py`, measured). Quartus itself has never
been run, so ALM / M10K / Fmax are unknown -- and Fmax is the one that decides
whether the machine can run at its real 60 ns microinstruction cycle.

    python3 tools/quartus_project.py     # regenerate dorado.qsf
    cd fpga/quartus
    quartus_sh --flow compile dorado

## RESULT (2026-08-27, Quartus Prime Lite 17.0.2, 5CSEBA6U23I7)

    Logic utilization (in ALMs) : 25,644 / 41,910  ( 61 % )
    Total registers             : 40,894
    Total RAM Blocks            : 87 / 553        ( 16 % )
    Total block memory bits     : 523,696 / 5,662,720 ( 9 % )
    Total DSP Blocks            : 0
    Fmax (Slow 1100mV 100C)     : 48.99 MHz

IT FITS, with room. And the MEMORY estimate was pessimistic by 3x: 246 M10K
predicted, 87 used -- so the bit-sliced-RAM merging held in reserve is not
needed.

FMAX IS THE CONSTRAINT. The machine oversamples each distributed ECL clock
SYSPER times per microinstruction, so real time needs SYSPER x 16.67 MHz:

    SYSPER=16 -> 266.7 MHz  no   0.18x       SYSPER=4 -> 66.7 MHz  no   0.73x
    SYSPER=8  -> 133.3 MHz  no   0.37x       SYSPER=2 -> 33.3 MHz  YES  1.47x

So SYSPER=2 is the target. Lowering it stresses cell fidelity -- a bug once
"survived at 16x only because the pulse happened to sit inside one stable
address" -- which the existing Verilator gates can hunt without an FPGA.

Summaries kept in reports/. Everything else Quartus emits is 192 MB of
reproducible detritus and is gitignored.

The project is GENERATED because the top module has 386 ports -- every cable
leaving the backplane is one -- and no DE10-Nano can pin that. Every port but
`sys_clk` is declared a VIRTUAL PIN, which is the standard way to place and
route a module that is not the whole chip, and gives real numbers rather than
an estimate. Listing 385 of them by hand is not a thing to do twice.

`dorado.sdc` constrains `sys_clk` to 20 ns. That is deliberately loose: the
first run exists to LEARN Fmax, not to meet a target.
