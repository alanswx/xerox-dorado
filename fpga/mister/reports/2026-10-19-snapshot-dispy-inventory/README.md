# Actual native display-board review inventory

Fresh XML elaborates the exact compiled Native56b `DispY_field.v` source,
SHA256 48ae129bd7523b5efb9db87d8f43cc314186f10fc0a4fe52f681b5c345836b4d,
with a review-fixture override of the parent board to SYSPER4. All frozen source hashes and original
source bytes are retained. This is a review inventory, not a completeness
proof or a display snapshot implementation.

The hierarchy has 548 sequential entries / 14,308 bits before excluding
simulator diagnostics. It contains 140 FF packages, nine MB7071H 256x4 RAMs,
three F10145A 16x4 files, three independent F10415A 1024x1 RAMs with private
read pipelines, one K1115A oscillator, and eight board phase bits.

The actual oscillator binds FREQ_KHZ=50000 and the cell default SYSPER=16
(SYS_KHZ=266667, STEP=100000). The review fixture's SYSPER4 override does not propagate to that
instance. Correction after inspecting the actual backplane: it omits the
board parameter too, so the actual board AND oscillator use default SYSPER16.
The board parameter is currently unused under the pinned source. The complete
board qualification in snapshot-dispy-full uses the actual defaults. Preserve this current binding during snapshot integration; change
and calibrate timing only in the clock milestone with all devices checked.
The K1115A state primitive is separately qualified in snapshot-oscillator.

Next: complete display-board access/classification/restore/matched timing,
then full-core qualification. No full-machine snapshot, timer correction or
speed gain is claimed. Raw XML/log/source bytes are gzip-compressed unchanged.
