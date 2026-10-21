# Native Ready/front: full build and assembly PASS

Same50MHz/SYSPER4; state administration INACTIVE. Source is the752-Q/9216-bit
native replacement already qualified by actual-wrapper Initial V5. This is
full-device source/memory/timing/assembly qualification, not a Cedar hardware
boot or speed qualification. Main/server/active MiSTer core are unchanged.

- 31 timing summaries and196 existing plus136 central-Q targeted reports,
  four operating conditions: minimum slack+0.118ns. Summary setup+1.333ns,
  hold+0.249ns. No new clock/timing exceptions; unchanged I/O coverage.
- 512 compiled source/configuration identities match the tested candidate.
- Actual native Initial:960 words read back,15 task initializations, all six
  error sources enabled for2611683 samples; aliases/unsettled/repeated checked.
- Resources:30933ALMs,52481registers,331M10Ks,35DSPs;38912MLAB bits total.
  726 matched central-Q register nodes are coverage, not source-bit savings.
- 158 unaffected inferred RAM rows/153 parameter tables remain byte-identical.
  Four256x9 MLAB banks preserve9216bits, async read and original source epochs.
  All other fitted device memory contracts pass. Eight1024x18 physical
  instruction banks use16 disjoint M10Ks and retain OLD_DATA.
- Only the already reviewed cache3/5 pair shares three M10Ks;8/10 stays
  independent,39 cache M10Ks total. The exact profile checks every geometry,
  OLD_DATA value, disjoint physical group and capacity; no arbitrary sharing.
- All disk/Ethernet retained child kinds/register counts match the qualified
  MC1668 build after removing exactly zero-state MC10102 l24. All133 mutable
  helpers/eight previously absent helpers stay exact. Previous cache and
  DskEth reviews reproduce byte-identically with the profile additions.

Six corrupted synthesis RAM reports and three warning controls reject. Two
new warning diagnostics are pinned to exact simulation-qualified HDL: dangling
comma on unused Ready outputs; parity index404..464 needs nine bits although
addressing every element of the540-bit vector would need ten. Existing warning
categories cannot increase except unused-object10036.

Eleven final full-review controls reject missing/negative/violated timing,
physical processor read/type, cache and instruction physical OLD_DATA, changed
disk state count, missing central coverage and rehashed constraints. The first
development control accidentally selected ContB bank3 instead of MemD bank3,
exposing that the reader checked instruction OLD_DATA only in the inference
parameters. The final reader ALSO checks all eight actual physical instruction
banks. The actual RTL/reports already retain OLD_DATA; no RTL change resulted.
Failed reviewer/control sources and the bad report are preserved in review/.

Builder138712 and all eight stages exited0, then serial reviewer165785/STA182575
exited0. Every recorded PID is absent before collection. Main build sources
remain unchanged. Assembly driver201198/assembler201199 then exited0:

```
RBF SHA256 57a552aff6fd0131f307a25ee75316c56b4c4da9118a437d2c9c17b0172dfd21
RBF bytes  3937608
```

Image excluded from git/raw source receipts. Local:
`build/rtl-executive/r-front-ready-native-terminal-20261021/Dorado.rbf`.
On misterubuntu: `/home/alans/mister/Dorado_RFrontReady_v1_20261021`.
At02:58:26UTC the image/qualification/assembly/controls hash-verify on MiSTer14
in `/media/fat/games/DoradoRTL/RFrontReadyBuild_20261021/`. NOT LOADED.
See `mister14-staging.json`; actual running core remains MC1668/Main3486/
server3485. The assembly record binds the qualification SHA to the image SHA.

Existing hardware check: the blanker/profile interaction returns to the same
two CommandTools and history. Actual typing `type ///rtlmc1668persist.txt`
prints `rtlmc1668persist` and returns to a prompt. No guest reset, active pack
copy, cold-persistence proof or new boot claim. Main/server/RBF fingerprints
match; good3395/dropped33/CRC0/RX0/abort0 at02:25:04UTC. Screenshots are in
`review/synthesis-and-live-baseline/`. All scripted keys are released.

Raw main archive:930 members,141034729 raw bytes ->6554409 stored bytes.
Nested review archive:58 members,119241925 raw bytes ->4187518 stored bytes.
Every raw member reads back losslessly. Re-running the complete reviewer
against compressed archived inputs and the prior candidate/Initial/component/
MC1668 receipts yields a byte-identical qualification. No full simulation rerun
from the archive is claimed. Databases, binaries and FPGA images are excluded;
small JSON remains raw, large JSON/reports/source are losslessly compressed.

Next use Main's normal loader barriers, preserve the CLOSED mutated old pack
and trace, then boot a fresh original seed through firmware to Cedar on this
image. Clock acceleration, measured progress/holds, authentic Checkpoint/
Rollback, cold file persistence, three fresh boots and2h combined stress remain
pending. Last hardware measurement is1947686.01 instruction issues/second.
Complete FPGA snapshots remain deferred; goal ACTIVE.
