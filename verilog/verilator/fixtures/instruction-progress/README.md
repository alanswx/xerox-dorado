# Instruction-progress microprograms

Plain microcode fields for the existing RTL CP/startup fixture. No CPU or
RAM checkpoint is imported. All test instructions increment T by one using
ALUFM[1]=0x20 (A+carry with carry1), ASEL6 (T), LC1 (T from Pd).
`ordinary.txt` stays at address0x100; a PC-change counter would read zero.

The extended tests alternate the incrementing access instruction with an
ordinary increment/branch at0x101. HM Figure6's access decodes are ReadTPC
0x67, WriteTPC0x6f and ReadIM0x77; the controller should insert four clocks
per access (six clocks instead of two). The WriteTPC test selects task1;
the isolated startup fixture has tasking off. Read results are unused.

`held.txt` first loads T=0x80, then the original Hold&TaskSim register from
T (FF0154B, HM section3.12), then increments at a self-loop at0x102. This
produces one held cycle in eight without forcing a control-board net.

`tb_instruction_progress.sv` observes100 actual T increments between
matching pipeline phases. It checks exactly100 issues, separately counted
held cycles, the cycle partition, clock counts and constant/alternating PC.
It does not compare against an interpreter or an edge-derived instruction
estimate. Run `make -C verilog instruction-progress-test SYSPER=4`, then8
and16. The monitor remains unintegrated with hardware diagnostics until
the generated wiring and coherent host reporting are qualified.
