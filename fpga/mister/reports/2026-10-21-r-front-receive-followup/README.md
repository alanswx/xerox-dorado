# Ready/front receive and RAM write-epoch follow-up

Component diagnostics only. No RTL, Main, daemon, hardware clock or disk-policy
change, Cedar startup qualification or new speed result. MiSTer14 remains on
the unqualified RBF57a552af session with Main16808/server16807; their exact
executables were revalidated through /proc at04:55:36 UTC. Last qualified
idle-login rate remains1,947,686.01 instruction issues/s. Ordinary-instruction
ceiling remains6.25M/s, target16.667M/s before waits.

The original-firmware hardware startup failures are preserved in the preceding
[field receipt](../2026-10-21-r-front-ready-field-diagnostic/README.md).
Its V3/V5 logs remain historical running prefixes. Here both V3 arms and V5 are
terminal failures. V3 was ended through the supported runtime-limit boundary;
its stop-reason text preserves a subsequently corrected inference.

V5's late SeqNo0/RHost-unset dump did **not** prove packet0 was rejected. V6
shows packet0 passing every filter, RHost established, and correct payload
copied to0200. SeqNo2/BootDataPtr03fe at29M establishes progress. V7 reaches
EOF/EIDone, then CheckChecksumAndLoad, executes15,688 additions and returns to
EtherMicrocodeBoot. InitialMain.mc allows that return only on bad checksum.
The one-shot replay answers only the first request, so the retry goes unanswered.
No firmware checksum or retry behavior is bypassed.

Independent wire readback decodes64 frames including63 consecutive microcode
replies with valid CRCs, exact Pup framing and empty EOF. All15,688 payload
words equal CedarDorado.eb!6; the original image sums to zero. This proves
transmitted bytes, not CPU storage or hardware receive correctness.

PCT samples at CIA changes before operand muxes settle. V8's passive phase trace
instead checks240 complete initial checksum iterations: MD, A and ALU all match
the original image/running sum. PCT suggests four16-word ranges at VM0e10,
1e10,2e10,3e10. Their sum differences1841/b67e/b624/fd4c account for the final
observed822f checksum;38 other differing groups sum to zero. V9 confirms all16
MD mismatches in the FIRST range among234 complete iterations; surrounding MD
reads match. The other three ranges remain PCT indications. Locate words using
BootDataPtr-1, as the original loop increments it during Fetch. The shared VA
bus need not retain that address at completion. V8's historical ETemp0 label
reads RM00; the current helper corrects the label. The sum check uses ALU A.

The new register-file integration also passes independent physical RAM shadows.
All12 preserved native packages observe actual address/data/write pins and
drive no target net. After both write windows close for two samples, all9,216
bits match the new files. V1 passes16M Initial; a shadow-only h06 data-pin fault
is rejected at13,392,081. V2 passes50M through receive/checksum: each RM package
covers155,922 closed windows/39,916,032 word comparisons; each STK package
covers3 windows/768 comparisons. All six original error sources remain enabled.
This is simulation write-epoch evidence, not physical timing or a Cedar boot.

Native memory control V12 was verified LIVE as PID97656 when archived. Its log
is a **running prefix**, not a terminal result. It binds deployed MemC/MemD/MemX
and helpers, with state administration inactive, retaining generic raw MSA
storage.36 existing cache seed assignments are translated via original output
pins to actual packed bank lanes. V10 failed compilation on removed package
names; V11 compiled but failed at time0 for missing STORAGE_INPUT_DELAY=2.
V12 verifies/copies the candidate QSF and uses that required build macro.
Inspect its current process/state before taking a next action; do not restart
on a polling timeout. These direct-ROM-entry fixtures omit native BaseBoard,
SDRAM and ARM services and cannot establish the cause of the FPGA's later Mesa
trap loop by themselves. Next establish a passing memory/receive control,
compare the changed native CPU coupling, and qualify same-clock Cedar/input
before accelerating. Authentic rollback and final acceptance remain open.

The manifest binds5,185 logical members to507 deduplicated gzip blobs:
52,656,840 raw bytes/5,987,641 stored bytes. Every blob was read back losslessly.
Frozen inputs, terminal logs, exact helpers, original EB/response bytes, native
inventory and candidate manifest are included. Executables, RBFs, databases
and disk packs are excluded. The manifest records the exact live V12 command.
This is evidence preservation, not a claimed portable full-machine replay.

The later [terminal V12 receipt](terminal-memory-v12/receipt.json) closes that
historical prefix: exit0 at55M, original Cedar BootOrStart238 at49,570,338,
then MesaIFUNotReady09c through the limit. Initial passes15 task initializations
and all6 error sources for41,611,683 samples. The seven-member extension was
read back losslessly and verifies frozen inputs against this parent manifest.
Full loaded IM/IFUM readback was disabled in the probe; it remains unverified.
No germ/desktop result. Next require original image readback in matched arms
and close native IFU/CPU timing coupling. All listed simulations are terminal.

Verify with:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-front-receive-followup
```

Use `manifest.json.gz` to map each logical member to its gzip blob. Current
wire/checksum analysis helpers and their results are included; earlier analysis
JSON remains historical, while `*-final.json` binds the current analyzer.
