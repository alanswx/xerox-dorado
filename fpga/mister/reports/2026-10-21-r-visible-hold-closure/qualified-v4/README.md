# Canonical visible-R native timing passes; first firmware boot under diagnosis

V4 passes the independent full-device review at unchanged 50 MHz/SYSPER4:
31 global timing checks, 196 legacy paths and 160 central/R paths across all
four operating conditions. Minimum reviewed slack is +0.062 ns. Global core
setup/hold are +1.418/+0.317 ns. All twenty sampled central hold paths at each
corner explicitly contain 0.100 ns uncertainty. Seventeen damaged-evidence
controls reject, including missing video pipeline state and an unapplied hold
margin. Source readback verifies all 513 compiled inputs.

The sixteen original visible-R bits have thirteen named fitter copies, all
covered by the corrected reporter. There are 705 retained central physical
registers from the 736 logical evaluator bits. Original Initial/map/RM and the
complete original Cedar image/entry component gates remain the exact-source
V26/V27 passes in the parent receipts; these are not hardware desktop gates.

Physical processor files, instruction RAM, IFU, memory controller, cache, map,
display memory and disk/Ethernet contracts all pass. One synthesis warning is
explicitly reviewed: Quartus skipped timing-driven synthesis. Full fitted STA
does run, reads the added SDC, and demonstrably applies the stricter margin.
The five formerly automatic host-video shift RAMs instead remain in registers.
All four source files match the qualified baseline; the read-only fitted
inventory verifies 272 named pipeline bits. The remaining 153 RAM rows and
all 153 unchanged explicit memory parameter tables match. The five inferred
RAMs account for 582 logical bits/seven M10Ks; no Dorado memory is exempted.
This classification is source/physical retention evidence, not a comparison
of the compiler's internal shift encoding or a hardware video qualification.

Resources: 31,047 ALMs, 52,267 registers, 324 M10Ks and 35 DSPs. Assembly succeeds
and the image is read back/hash-checked:

```
7a90adb9ae15e96a6c6121751460e343a1aa7683126bb641b6c9dc97af353de4  Dorado.rbf
```

The raw receipt preserves 962 members/914 blobs, sharing 477 parent blobs:
148,274,063 raw bytes/6,066,846 stored bytes. Its collection flags deliberately
do not qualify timing; the attached `qualification.json`, control results and
assembler journal carry the separate review. FPGA images/databases are excluded.
The archived review tools are the versions used for these added checks. Verify
the lossless raw readback with:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-visible-hold-closure/qualified-v4
```

Extracting all962 raw members and rerunning the strict reader reproduces the
assembled qualification byte-for-byte (`archive-replay.json`). This replay
uses the existing hash-verified V26/V21/component inputs; it is a fit-receipt
replay, not a newly executed firmware simulation or fresh-checkout boot recipe.

The normal Main loader deployed it on MiSTer14 with unchanged Main `21e67a3f`
PID32012/server `c5996249` PID32011 and a fresh original f3bce2a8 seed in
`CedarRVisible4_20261021`. Main remains sole SPI owner. The actual Main capability
has flags22, epoch words62088/3285/59039/6210 and zero fault/control errors.
Three normal boot-button chords completed; a new original BFN113 was observed
and C released. Actual HDMI reaches Pilot status812. Sixteen passive PC snapshots
then find task0 mostly at CIA239/23a and6c1/6c2; five real disk read requests
have matching ACKs in3.157..3.213ms, no writes and zero ring drops at that bounded
observation. The disk file contains preload records only; guest requests were
read passively from Main's buffered trace without flushing or modifying it.
No login/desktop or new speed measurement is qualified. Do not reset merely
because the screen is static; continue diagnosing this running original boot.

`first-boot-prefix/` preserves forty losslessly read-back members including the
actual HDMI captures, executed keyboard/debugger scripts, terminal input-job
states, bounded Main/server/disk prefixes and passive ring/PC observations.
It is a startup prefix, not closed current logs or a machine snapshot.
The retired Boot2 pack is closed and still byte-identical to the original seed;
its complete trace has10,139 records and one explicitly retained dropped-buffer
event at transition. No gap-free Boot2 or guest clean-shutdown claim is made.

Two older barrier-closed tmpfs traces (49b/55) were independently verified closed,
copied/fsynced/hash-checked to `ClosedTmpLogs_20261021_RVisible4` on SD, then only
their RAM duplicates removed, freeing22,733,709bytes. Their complete original
bytes and preservation proof remain available. Protected artifacts are untouched.

Last qualified Cedar login speed remains1,947,686.01 issues/s. No speed gain,
native Viewers/input recovery, authentic rollback, persistence/repeated boots
or sustained-operation acceptance is claimed. The original60ns goal remains
active; complete FPGA snapshots remain deferred.
