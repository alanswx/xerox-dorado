# MC1668 native-copy and Initial component regression

**Native cell and Initial component PASS. Full native fit is running; no new
RBF has been deployed or hardware speedup measured.** Native59 remains on 14.

Native59 actually instantiates `cell_MC1668_snapshot`, not the generic cell,
at all twelve processor/MemC positions. Correcting only the generic source
would therefore leave the hardware's edge-only behavior intact. The new
stager reconstructs and checks 413 compiled / 498 archived Native59 inputs,
then changes exactly the generic MC1668 and its instantiated copy. Its normal
processes are taken directly from the qualified manufacturer-table correction.
The existing inactive state interface and historical clock-history slots stay
compatible; those slots have no influence on normal capture. No Main/server,
clock, storage, networking, pin or timing-constraint change is made.

Both simulators pass 1,425 manufacturer-table observations through the actual
native copy, plus 4,272 checks of unchanged state while machine advance pauses.
Legacy edge-only behavior, ignoring pause, and a shared half clock each reject.
No state-admin operation is exercised or qualified.

The isolated Initial fixture uses Native59's original generated boards/cells,
the corrected MC1668 and the existing CPU-manifold/ALUFM setup. Its static ROM
payload has 960 instructions loaded and read back without mismatch. All 15 tasks
initialize, and all six error sources remain enabled for 2,611,683 samples with
no error over the 16-million-cycle run. It enters the original ROM's F43 debug
entry; **this is not a BaseBoard firmware boot or Cedar qualification**. No C
machine checkpoint/state is imported. Converter hashes, original MB files,
generated fixture bytes, testbench and HDL are frozen in the receipt.

The first compile could not bind Native59's renamed DskEth receive-observer
type. The accepted fixture top binds the existing rxmonitor type without
changing board/cell logic or hierarchy under b_DskEth. Its receive observer
is output-only. The failed compile and sources are diagnostic history.

The full candidate is `/home/alans/mister/Dorado_Mc1668_20261020` on
misterubuntu. Builder 3979425 / fitter 3983817 were confirmed live during routing;
SSH session 20239 observes the same job. Do not restart because a tool observation
expires. Source synthesis shows twelve MC1668 instances with 22 retained latch
registers versus 38 before (duplicate physical inputs share registers), removing
the sixteen obsolete sampled edge-history registers. This is synthesis evidence,
not a fitted result. Frequency remains 50MHz/SYSPER4.

The builder runs map/fit/STA/clock coverage/four corners/board crossings
sequentially, then allows assembly only after timing checks pass. Historical
`native59-*` report prefixes identify unchanged reporting scripts, not an old
artifact. The new source/memory/timing auditor and terminal collector are staged
workflow tools; their full-candidate positive review is still pending.

Correction after this component archive was frozen: the per-part synthesis
totals are 38 before / 22 after, a reduction of sixteen. The archived pending
auditor had a 36/20 arithmetic error; current `tools/audit_mc1668_native.py`
corrects it and reads Quartus's Latin-1 fitter report. These checker corrections
do not change the frozen HDL, native-cell tests or Initial component result.

Current hardware is unchanged Native59, Main 29511/server 29510. HDMI capture was
reachable using the installed MiSTerKVMS CLI. Escape dismissed its blanker and
showed Cedar waiting at the personal-profile question. No core load, guest Boot,
host disk barrier or volume copy was performed during this milestone.

Observe next:

```
ssh misterubuntu 'cat /home/alans/mister/Dorado_Mc1668_20261020/run-state.json'
```

Confirm current process liveness as well as that state file. After terminal
completion, collect and independently audit the exact sources, all memory
geometry/OLD_DATA, all 31 timing summaries / 196 detailed categories/four conditions
and unchanged I/O coverage. Only then run the normal Main-loader procedure,
preserve the old closed volume and cold-boot a fresh original seed through real
firmware. Cedar desktop/input, persistent files and pinned issue/hold comparisons
remain pending. Full FPGA snapshots remain deferred.

Raw sources/reports/logs are losslessly compressed with raw byte counts and
SHA256 in the manifest. Objects, executables and Quartus databases are omitted.
The archive readback is checked. Full replays have not been repeated after
extraction; the accepted runs already execute frozen source inputs.
