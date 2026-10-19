# MemC F100181 sampled output-latch state

The adapter saves the original model's four output-latch bits, retaining
its sampled transparent-while-enable-low behavior. Arithmetic, carry and
lookahead expressions remain unchanged, including the original model's
documented lack of BCD correction. This does not claim historical BCD accuracy.

Untouched-original and independent per-bit/ripple-oracle tests pass 96,678
host edges, 274 held edges, every one of 16,384 input combinations, all
sixteen saved states, transport padding, ignored writes while running,
idle holds, destructive continuation/restore and 20,000 post-restore cycles.
Five faulty RTL variants and three changed-original sources reject. Fresh
XML accounts for four mutable bits and the complete combinational table;
nine damaged inventories/schemas reject. Earlier simulation evidence is
retained; the final version adds idle holds and running-write checks.

Matched negative-edge launch/positive-edge capture 50 MHz shells pass all
four timing conditions, minimum +0.150 ns, no cuts or unconstrained paths.
Both variants have identical 20 ns clock, 2 ns maximum/0 ns minimum virtual
I/O constraints: the shell gives 10 ns into the cell and 20 ns output paths.
Resources change 49 -> 51 ALMs and 29 -> 42 registers, zero RAM, including
shell and administration. Five damaged-fit controls reject.

The first matched positive-edge shell fails virtual-input hold paths in
both variants (original -0.421 ns, adapter -3.070 ns at slow/-40 C) and is
rejected. All raw evidence and its executed builder are retained. Only the
shell launch edge changed for the accepted pair; cell RTL and SDC stayed
identical. No production timing exception or clock change is introduced.

This is one primitive qualification. Full MemC coverage/restore/timing and
whole-machine barriers/physical snapshots remain separate work. Text is
gzip-compressed without content changes; no build databases are included.
