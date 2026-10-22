# Matched known-good cache4 original-firmware login baseline

One normal9e/cache4 control boot uses the SAME immutable236b disk seed, Main21e6 and receiver0845 as the cache16 experiment. Main15052/server15051/loader15029 are source/hash bound. Only normal C hold/release occurs before actual Guest.pa login (native03/04). No Guest is accepted. After login, normal OSD selects passive counters; no debugger/ptrace/injected calls or second SPI owner.

Three adjacent ten-second windows measure2,968,747.44 /2,968,835.83 /2,968,864.79 StartCycle issues/s: mean2,968,816.02/s,17.8129% of the original16.67M/s nominal. Run/Jam-qualified, excluding RepeatCur, not pipeline-drained retirements. Clock remains57.142857MHz/SYSPER4: ordinary nominal140ns,7.143M/s before waits, not original60ns.

A separate quiet-login interval measures0.180632% held instruction cycles. Separate cause windows measure41.637317% enabled host edges,58.361471% storage-only pause,0% disk-only and0% both. These are separate non-atomic intervals: do not sum them as an atomic decomposition. The issue/hold/enabled/storage samples precede repeat preparation; copy preparation overlaps the later disk/both windows. The old2.968685M7c rate used a different seed/server, so matching its number does not establish a controlled new-core speed comparison.

All selectors return Off. Native observer stops normally13 samples. Actual HDMI capture remains black despite a clear native login; no resolution or capture service change is made. Source volume closes through two normal barriers at unchanged236b:2818 complete records,46 read/ACK pairs, one write/ACK pair, no recorded errors/trace loss, no open pack/trace FDs, old DDR mapping revoked. The write leaves identical image bytes. Preserved original815 cache16 run is separate; this control does not qualify a new desktop, keyboard, files, final three fresh seeds or two-hour stress.

Next, one distinct93ad cache16 repeat is loaded from the SAME236b with the SAME Main/receiver/clocks, excluding all early OSD/extra input until an actual login. Its current journal identities are Main15534/server15531/loader15512. C watcher and a bounded native observer follow that same boot. This receipt claims no repeat firmware or speed result. Original60ns and full delivery acceptance remain open.

Verify: tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-cache-capacity/native57-cache4-matched-login.
