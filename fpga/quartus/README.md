# Quartus fit for the eleven-board Dorado

The capacity case for this design has been ARITHMETIC so far: 2,824 cell
instances against a DE10-Nano's 41,910 ALMs, and 246 M10K worst case against
557 available (`tools/sil_ram_groups.py`, measured). Quartus itself has never
been run, so ALM / M10K / Fmax are unknown -- and Fmax is the one that decides
whether the machine can run at its real 60 ns microinstruction cycle.

    python3 tools/quartus_project.py     # regenerate dorado.qsf
    cd fpga/quartus
    quartus_sh --flow compile dorado

The project is GENERATED because the top module has 386 ports -- every cable
leaving the backplane is one -- and no DE10-Nano can pin that. Every port but
`sys_clk` is declared a VIRTUAL PIN, which is the standard way to place and
route a module that is not the whole chip, and gives real numbers rather than
an estimate. Listing 385 of them by hand is not a thing to do twice.

`dorado.sdc` constrains `sys_clk` to 20 ns. That is deliberately loose: the
first run exists to LEARN Fmax, not to meet a target.
