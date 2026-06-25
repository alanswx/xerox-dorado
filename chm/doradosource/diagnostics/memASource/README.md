# memASource — the real memA diagnostic sources

Pulled from CHM `[_CD8_]<doradosource>MEMASOURCE.DM!15_` (the dump-format
archive that builds `memA.mb`). URL form (note the MixedCase archive name):

```
https://xeroxparcarchive.computerhistory.org/_cd8_/doradosource/MEMASOURCE.DM!15_/<file>
```

`memA.cm` is the build manifest — memA.mb is assembled from `memAsave.cm`
(definitions) + the shared `memSubrs*` + `memPostamble` + these `memRW*`
tests + `memAonly.cm`/`memAplace.cm`. The matching subroutine/definition
sources live one level up in `../diagnosticSubrs/` (memSubrsC/S/D/A,
memDefs, preamble, postamble).

| file | what it tests |
|---|---|
| `memRWc.mc` | **Cache** read/write + the **Cache Addressing Test** (`cacheAddrTest`, labels `CAT…`). Contains `catFindUpF`/`catUpAddrErr`, `readCurrentCAmem`/`readOldCAmem` (the `mcr.dPipeVa` destructive cache-address read), and `beginCtest`'s `disableConditionalTask` call. This is the source of the `CATUPADDRERR` failure. |
| `memRWs.mc` | Storage read/write (the long pattern walk). |
| `memRWd.mc` | Data-path read/write. |
| `memRWx.mc` | Map/translation read/write. |
| `memChaosS.mc` | Pseudo-random "chaos" storage stress. |

## Why these were pulled (CATUPADDRERR root cause)

`cacheAddrTest` zeroes the whole cache, then walks every (row, column)
expecting each freshly-visited cache-address-memory entry to read 0 (it
sets visited ones to -1 as it goes). It calls `disableConditionalTask` at
`beginCtest` precisely so the §3.12 memory-simulator task (the jumpered
task 0o12) does not preempt task 0 and make a `noRef`+`useMcrV` STORE that
writes the cache-address memory (which would inherit task 0's sweep MCR and
clobber the zeroed cache).

`disableConditionalTask` (postamble.mc) clears `flags.conditionOK`, then
`makeHoldValue`+`resetHold` jam Hold&TaskSim such that taskFreq=0 (the
`taskSim[0]` ENABLE bit clears). The C emulator was counting/firing TASKSIM
on any non-zero counter (ignoring the enable bit), so a stale re-arm value
(e.g. 0o104) kept waking task 12 and corrupted the cache — `CATUPADDRERR`.
Fixed in `dorado/src/cpu.c` by gating the TASKSIM tick on the `0o10` enable
bit (postamble.mc: "taskSim[0] enables the task simulator and taskSim[1:3]
form a counter … when hardware counts to 17[octal] it awakens").
