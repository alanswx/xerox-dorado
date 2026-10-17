# Four-module Cedar memory failure (2026-10-17)

Native19 on MiSTer14 still displays a white raster with a small cursor,
then repeats RequeueSearchSQ (239/23a/6c1/6c2). A clean Linux reboot and
proper MGL programming reproduce that result. This is not a Cedar screen.

The same original-ROM fixture now reproduces the failure with four storage
modules, without DDR or Linux. One module succeeds with either square
pulses or the BaseBoard's actual pulses; four modules fail with both.
All original 3600 IM / 256 IFUM entries survive, all 15 tasks initialize,
and all six hardware error sources remain enabled and clean. Those checks
alone do not qualify the boot.

`tools/check_cedar_rtl_germ.py LOG` verifies sampled CPU germ-copy reads,
allows the two request words written by PilotBoot itself, requires the germ
handoff, original image/startup checks and Mesa IFU fetch/ack activity.
It passes the one-module trace and fails the four-module trace with 45
unexpected words. This is sampled coverage, not a full memory checksum.

`source0200-module-boundary.txt` shows why: the first two words of a module0
read are driven by module3 (387f, ffd1). Module0 supplies the correct third
word (0518) and the rest of the line. Wrong words occur at offsets0/1/2 of
16-word lines. The cause of the single-bit offset2 differences is still
unresolved. The serial
module handoff is the next boundary to investigate, not a new guest patch.

Schematics checked directly: MemX09 (MemX-Rev-Ch PDF page11) shows j01,
a parallel-load F10016 enabled by inverted LoadSinO on Clk0'Bc. msa01
(msa-Rev-Bg PDF page3) shows h02's two module-enable stages and the separate
Sin output registers. The generated wiring agrees with these sheets.

The new reproducible startup target is:

    make -C verilog/verilator initial-rom-fourmodule-test SYSPER=4

It uses a separate object directory, all eight MSA slot clocks and presence
tokens, original BaseBoard Initial payload, and actual BaseBoard pulses.
It still starts at the fixture's established direct entry; it is not proof
of the complete 6502 firmware startup. Extra Cedar arguments can be passed
with INITIAL_FOUR_ARGS and the bound with INITIAL_FOUR_CYCLES. Use
+bootio +bootio-germ-copy +bootiofrom=94000000 +ifutrace for copy diagnostics.
The full run also needs original Cedar EB replay and the matched CHS pack,
as recorded in docs/verilog-handoff.md.

The Native20 Ab/f04 candidate passes a short clock/map test but leaves the
long-run mismatches identical; it is rejected. Canonical production RTL
and actual FPGA remain Native19. Two isolated timing experiments follow:
MemX j01 capture phase, and the MSA c5 clock leg before the SO-qualified
Sin capture gates. Neither is qualified for promotion or hardware loading.
