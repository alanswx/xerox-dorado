# Matched image handoff and native task/IFU probes

V13 CONTROL exits0: original Initial passes, all3,600 loaded Cedar IM words and
256 IFUM entries match, and entry238 executes. The changed CPU candidate exits1:
3,856 mismatches and entry not reached, with Initial still passing. Original
pack/reply bytes and deployed memory controllers are shared. V13's control CPU
is the earlier generic component; the later native comparison is recorded in
the [closed receipt](../2026-10-21-r-front-native-handoff-closed/README.md).

V14 substitutes the deployed IFU and maps all27 physical IFUM planes to native
packed RAM lanes using original output-pin wiring. It passes the image gate
but does not clear the later wait. A passive phase trace shows task0 receiving
2,153 post-entry host samples, then task12 receiving5,431,273; no other task
runs and no IFU memory request occurs. Task12 enters new Cedar code and ends
in MesaIFUNotReady09c. PilotMesaProcess.mc makes that an IFUNext0 retry entry;
this is not evidence of an IFU defect. No germ/desktop qualification.

The V15 native CPU pair, Q capture probe V1 and RAM port probe V3 were verified
live when collected; their logs here are **running prefixes**. They are now
closed in the later receipt. Exact source, commands and original artifacts
remain bound by frozen provenance. State administration is inactive; no target
state is driven by the new probes. The underlying fixture remains original-ROM
direct entry with generic raw MSA storage, without native BaseBoard/SDRAM/ARM.
It cannot qualify a complete FPGA boot.

The archive maps3,074 logical members to465 gzip blobs,34,713,995 raw bytes and
4,686,935 stored bytes, all read back losslessly. It preserves original EB,
response bytes, native inventory and exact helper versions. Executables, FPGA
images, databases and disk packs are excluded. Verification:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-front-native-handoff-diagnostic
```

Use `manifest.json.gz` to locate each named member's gzip blob. The manifest
records exact process observations, not inferred completion. This receipt
contains no speed gain, Cedar desktop, rollback or FPGA snapshot claim.
