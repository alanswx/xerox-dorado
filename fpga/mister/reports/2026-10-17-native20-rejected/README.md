# Native20 candidate — rejected, not deployed

Frozen Native16 base plus the committed memory-DMux and Native19 map-clock
corrections, with an isolated Ab/f04 experiment. Ab returns to one stage;
f04 alone receives two local additional stages so its total waveform stays
at the previous three-stage baseline. f03 shares the early phase with a03.

Overlay SHA256:
8aa84ae27b783e1256cf15789e05e31b59ff7e774772f79c90192f1394a33aff

Quartus17.0 Lite synthesis and fitting succeeded; fit elapsed19m01.
STA has one negative hold check, -0.037ns, which was not waived or reviewed.
The full four-module functional gate also fails: the identical45 unexpected
germ words, source0200=387f instead06ec, IFU reference/ack0 and the queue
loop. See ../2026-10-17-fourmodule-reproducer/.

The experiment is not canonical RTL, not a qualified build and was never
loaded onto MiSTer14. Its MGL and fresh medium were staged, but no RBF was
put on the SD card. Actual hardware remains qualified Native19.
