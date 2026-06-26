# Running the Dorado hardware diagnostics

**Current ground truth: 2026-06-26.** PARC's original Dorado diagnostics are now
useful regression gates for the C emulator. The table below lists commands that
were run successfully against the current tree.

These diagnostics are hardware microprograms, not normal software tests. Some
need diagnostic-specific harness settings because the original Midas recipes
selected a task, a disk controller, or a smaller hardware configuration before
starting the microcode.

## Runner

Build:

```sh
make -C dorado build/rundiag
```

Usage:

```sh
./dorado/build/rundiag <diag.mb> [entry=BEGIN] [done=DONE] [err=ERR] [maxsteps]
```

Useful runner/environment knobs:

| knob | purpose |
| --- | --- |
| `RUNDIAG_TRAIL=1` | dump the last 48 PCs on a non-pass result |
| `RUNDIAG_TASK=<octal>` | start on a nonzero task; TriconD uses task `14` |
| `RUNDIAG_DISK=1` | attach the Trident disk controller to the slow-I/O table |
| `RUNDIAG_DISK_MEDIA=t80\|t300\|diablo` | attach pack media of the selected geometry |
| `RUNDIAG_DISK_PATH=path` | load the selected `RUNDIAG_DISK_MEDIA` geometry from a pack image instead of creating blank media |
| `RUNDIAG_IMRH_SYMBOL=SYM=value` | patch an IM right-half constant by symbol; used for memA `MEMFLAGS` slices |
| `RUNDIAG_RM_SYMBOL=SYM=value` | patch an RM symbol before running |
| `RUNDIAG_DUMP_RM_SYMBOLS=a,b,012` | dump named or numeric RM cells on non-pass |
| `DORADO_STORAGE_MODULES=1..4` | report/allocate a smaller storage population |
| `DORADO_STORAGE_CHIP_TYPE=0..3` | report smaller/larger storage chips to memory diagnostics |
| `DORADO_MAP_IC_K=16\|64\|256` | report the selected map IC size through DMux |

Default world/emulator behavior is unchanged: storage defaults to four 4MW
modules, chip type 3, and a 64K map.

## Verified Commands

General diagnostics:

```sh
./dorado/build/rundiag 'chm/dorado/expanded/kernel.dm!38_/kernel.mb' BEGIN DONE ERR 10000000
./dorado/build/rundiag 'chm/dorado/expanded/eventCounters.dm!5_/eventCounters.mb' BEGIN DONE ERR 12000000
DORADO_STORAGE_MODULES=1 ./dorado/build/rundiag 'chm/dorado/expanded/memMisc.dm!11_/memMisc.mb' BEGIN DONE ERR 120000000
./dorado/build/rundiag 'chm/dorado/expanded/Ifu.dm!51_/IfuSimple.mb' BEGIN DONE ERR 25000000
./dorado/build/rundiag 'chm/dorado/expanded/Ifu.dm!51_/IfuComplex.mb' BEGIN DONE ERR 120000000
RUNDIAG_DISK=1 RUNDIAG_TASK=14 ./dorado/build/rundiag 'chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN TESTOK-WITHOUT-DISK ERR 50000000
RUNDIAG_DISK=1 RUNDIAG_DISK_MEDIA=diablo RUNDIAG_DISK_PATH=dorado/build/run-disks/alto-games-trident.pack RUNDIAG_TASK=14 ./dorado/build/rundiag 'chm/dorado/expanded/Tricond.dm!5_/TriconD.mb' BEGIN DONE ERR 120000000
```

The file-backed TriconD command is normally run through Make so the pack starts
clean each time:

```sh
make -C dorado run-tricond-pack
```

Observed pass points:

| diagnostic | result |
| --- | --- |
| kernel | PASS, `DONE` after 4,063,197 steps |
| eventCounters | PASS, `DONE` after 5,355,594 steps |
| memMisc | PASS, `DONE` after 90,950,270 steps with one storage module |
| IfuSimple | PASS, `DONE` after 19,800,637 steps |
| IfuComplex | PASS, `DONE` after 97,850,744 steps |
| TriconD no-pack | PASS, `TESTOK-WITHOUT-DISK` after 23,482,557 steps |
| TriconD pack-present | PASS, `DONE` after 25,412,162 steps using a copied Diablo-on-Trident pack |

memA is best run as focused slices. The full S-board path includes very long
storage sweeps and task-simulator/chaos paths that are not a quick regression.

```sh
DORADO_STORAGE_MODULES=1 ./dorado/build/rundiag 'chm/dorado/expanded/MEMA.DM!18_/memA.mb' BEGINXTEST MAPRWDONE ERR 400000000
DORADO_STORAGE_MODULES=1 ./dorado/build/rundiag 'chm/dorado/expanded/MEMA.DM!18_/memA.mb' BEGINXTEST MAP2RWDONE ERR 300000000
DORADO_STORAGE_MODULES=1 ./dorado/build/rundiag 'chm/dorado/expanded/MEMA.DM!18_/memA.mb' BEGINDTEST DTESTDONE ERR 700000000
DORADO_MAP_IC_K=16 DORADO_STORAGE_MODULES=1 DORADO_STORAGE_CHIP_TYPE=0 RUNDIAG_IMRH_SYMBOL=MEMFLAGS=004 ./dorado/build/rundiag 'chm/dorado/expanded/MEMA.DM!18_/memA.mb' BEGINSTEST SADDRTESTDONE ERR 1500000000
DORADO_MAP_IC_K=16 DORADO_STORAGE_MODULES=1 DORADO_STORAGE_CHIP_TYPE=0 RUNDIAG_IMRH_SYMBOL=MEMFLAGS=020 ./dorado/build/rundiag 'chm/dorado/expanded/MEMA.DM!18_/memA.mb' BEGINSTEST SDTESTDONE ERR 2500000000
DORADO_MAP_IC_K=16 DORADO_STORAGE_MODULES=1 DORADO_STORAGE_CHIP_TYPE=0 ./dorado/build/rundiag 'chm/dorado/expanded/MEMA.DM!18_/memA.mb' SFLUSHTEST AFTERSFLUSH ERR 500000000
```

Observed memA pass points:

| slice | result |
| --- | --- |
| X board `MAPRWDONE` | PASS after 216,663,496 steps |
| X board `MAP2RWDONE` | PASS after 237,148,591 steps |
| D board `DTESTDONE` | PASS after 520,709,182 steps |
| S address `SADDRTESTDONE` | PASS after 10,564,382 steps |
| S data `SDTESTDONE` | PASS after 299,432,359 steps |
| S flush `AFTERSFLUSH` | PASS after 307,123 steps |

## Notes

- TriconD's ordinary `DONE` label is not the no-pack success point. The source
  says the first part of the test runs without a disk present and reaches
  `TESTOK-WITHOUT-DISK` when the controller is good but no drive is spinning.
  With pack media attached, the diagnostic continues through tag wakeups,
  sector/index wakeups, and sector-counter checks and then reaches `DONE`.
- TriconD must run on DSK task `14` with `RUNDIAG_DISK=1`; task 0 reads a
  floating/non-disk slow-I/O bus and fails the first state-muffler check.
- `make -C dorado trident-readonly-images` creates read-only golden packs in
  `dorado/testdata/trident-readonly/`. Runtime targets copy those into
  `dorado/build/run-disks/` and use the writable copy.
- `make -C dorado run-alto-disk-screenshot` uses that same copy-on-run pack
  path and writes `/tmp/dorado-alto-disk-300.pgm`; the expected current result
  is the Alto Executive prompt after a disk boot.
- memA's S-board subtests use source-supported `MEMFLAGS` bits and reduced
  diagnostic hardware sizing so the test covers the intended behavior without
  turning into a large storage burn-in.
- `SCHAOSTEST` is not a standalone quick gate unless the diagnostic task
  simulator is enabled the way the original Midas harness expected.

## Unit Gates

The low-level C tests that back these diagnostics are:

```sh
./dorado/build/test_cpu
./dorado/build/test_memory
./dorado/build/test_disk
```

All three passed on 2026-06-26.
