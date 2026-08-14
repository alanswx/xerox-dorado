# Cedar checkpoint / rollback (B1): how it works, and the two bugs that broke it

2026-08-14. The checkpoint crash a PARC veteran reported is **fixed**: the
machine no longer dies at a blank screen. Reproducible in one command:

```
make cedar-checkpoint-repro
```

**This is Cedar's OWN checkpoint, not our `--snapshot-out`.** They share a
word and nothing else.

## 1. What the operation actually is

Reading Xerox's sources, it is four cooperating pieces, not one:

| piece | file | role |
|---|---|---|
| `Checkpoint` command | `InitialCommandsImpl.mesa` | CommandTool entry; prints "Creating checkpoint at ..." then, on the way back, "Rollback at ..." |
| `Booting.Checkpoint` | `BootingImpl.mesa` | sets the internal `v` switch, calls `VerifyFile`, reboots self with `outload: [logical[root: checkpoint]]` |
| `RollbackImpl` | `Rollback/RollbackImpl.mesa` | the VM copying, triggered by `switches[v]` / `switches[r]` in its module-init body |
| `MyCheckpoint` / `MyRollback` | `FileInit.mesa` | registered via `Booting.RegisterProcs`; commit volumes, lock files, re-read root pages |

So the "Checkpoint ... Rollback" pair a user sees is ONE command
round-tripping through a self-reboot. That is normal output, not two
failures.

`Booting.Checkpoint` in full:

```
Checkpoint: PUBLIC PROC RETURNS [rejection: ROPE _ NIL] = {
  mySwitches: Booting.Switches _ ALL[FALSE];
  mySwitches[v] _ TRUE;                          -- => copy VM for checkpoint
  VerifyFile[File.SystemVolume[], checkpoint];
  RETURN[Boot[boot: [self[]], switches: mySwitches,
              outload: [logical[root: checkpoint]]]]
};
```

`VerifyFile` **creates the checkpoint file if the volume has none**
(`File.Create` sized from allocated VM pages, then `FileBackdoor.SetRoot`).

## 2. The instrument that found it: MP codes

The Dorado has no maintenance-panel lamps, so Cedar renders the MP code as
three decimal digits into the **cursor bitmap** at `LONG[431B]`
(`ProcessorHeadDorado.mesa`, Taft 11-Dec-1980, "Maint panel in cursor").
`DORADO_MP_TRACE=1` reads those six rows back through Xerox's own
`digitFont` and prints the code on every change, so a blank screen becomes a
named boot stage. A healthy Cedar cold boot reads:

```
810 germStarting  812 germInLoad  814 germFinished  845 mesaRuntimeInitialized
850 vmInitialized 855 storageInitialized 860 fileInitialized  (cleared = emptyMP)
```

This is the single highest-value instrument for any boot/outload work; it
should be the FIRST thing turned on, not the last. The failing checkpoint
read:

```
830 cleaningUp -> 810 germStarting -> 811 germOutLoad -> (nothing, forever)
```

## 3. Bug 1: the polled germ IOCB path was read-only

`machine_germ_complete_disk_iocb`'s polled arm (`csb_interrupt_mask == 0`)
ignored the IOCB command's Action fields entirely and unconditionally copied
disk -> guest memory, then reported success. That is correct for the germ's
boot chain, which only ever reads -- and it had only ever been exercised by
boot chains.

But **the germ writes through exactly the same interface**. `DoOutLoad`
(`bootswapgerm-6.1.mesa`) posts `cmd=0o100244` = `incrementDataPtr |
[check,check,WRITE]` for every page of the outload. Serviced as a read, each
one silently discarded a page of the checkpoint *and* overwrote the guest's
VM with stale disk contents.

The fix honors `DISK_CMD_DATA_ACTION` / `DISK_CMD_LABEL_ACTION` in the
polled arm. Reads keep their previous unconditional form -- no boot-path
command has a WRITE action, so the branch cannot perturb them.

### Why this looked like "no disk I/O at all"

`DoOutLoad` does ~39 M cycles of **pure CPU work** between `ShowMP[germ
OutLoad]` and its first `Transfer`: it walks the whole page map counting
non-vacant pages (`countData`), one `RMap` per page at ~600 cycles each over
`countVM` = 64K pages. An IOCB trace windowed 34 M cycles past the hang
therefore showed *zero* disk operations and looked like a CPU spin. It was
simply too short. `DORADO_RMAP_TRACE` (now windowable with
`DORADO_TRACE_GATE`) shows that walk directly: pages 0,1,2,... sequential,
one read each.

## 4. Bug 2: a runtime-created file's links are CHS on a flat-link volume

With writes working, the outload ran and the inload then failed with
`823 germBadBootFile` -- `DoInLoad` reads the first page, checks
`header.version` against `BootFile.currentVersion`/`cedarVersion`, and errors
if it matches neither. It was reading the wrong page.

Our shipped PDIs are written by `rusty-backup`, which stores **flat** page
numbers in the boot links; the mount-time `machine_pdi_links_kind_of`
classifies the volume from them. But the checkpoint file is created **at
runtime** by Pilot, whose computed DiskAddresses are always **CHS**. So the
volume genuinely carries mixed conventions, and a volume-wide classification
cannot answer for it. (The interrupt-driven path already assumed CHS
unconditionally for the same reason.)

Measured on the outload's own addresses -- the two readings are not close:

| decode | distinct pages | range |
|---|---|---|
| flat (`lo+hi`) | **111** of 2,365 | 310..420, overlapping |
| CHS (`lo*28+sector`) | **2,365** of 2,365 | 8665..11038, consecutive |

And the medium settles which is right:

| page | label fileID | filePage | data[0] |
|---|---|---|---|
| 322 (flat) | `[3,0,0,0,0]` | 183 | noise |
| **8665 (CHS)** | `[0o254,0,0o20603,0,0]` | **0** | `0o226` = 150 = `cedarVersion` |

`[254 0 20603 ...]` is exactly the checkpoint file registered in root slot 0.

Two fixes, both in the polled arm:

- **writes** decode as CHS. A polled write only ever targets a file the
  running system created.
- **stream-start reads** are adjudicated from the medium, the same principle
  `machine_pdi_links_kind_of` uses at mount: the germ always begins a boot
  file at its first page ("Read first page, containing header"), so where the
  two readings disagree, the candidate whose label says `filePage 0` wins.
  The test is **asymmetric on purpose** -- when the volume's own convention
  already lands on a file start it always wins -- so every shipped volume
  decodes exactly as before. Verified: a cold Cedar boot fires the
  adjudication **zero** times. Only stream starts are adjudicated; mid-stream
  addresses come from the sequential cursor and are never second-guessed.

## 5. Where it gets to now

```
830 cleaningUp  810 germStarting  811 germOutLoad  813 germMapIO  814 germFinished
810 germStarting 812 germInLoad  845 850 855 860  (cleared)      <- Cedar restarts
830 cleaningUp  810 germStarting 812 germInLoad ... 813 germMapIO (cleared)
```

No germ error code anywhere, and the machine ends **alive** at a live Cedar
(herald, `CedarWork on Dorado`, profile prompt) instead of a dead blank
screen. The second boot is `RollbackImpl`'s final act -- `Booting.Boot[boot:
[file[outLdFile, outLdFirstPage]]]`, booting the checkpoint file it just
wrote -- and it now completes, with **no** `845/850/855/860` re-init, which
is what a genuine restore looks like (VM restored wholesale, Cedar resumes).

The volume carries a real checkpoint afterwards:
`tools/cedar_root_files.py` shows slot 0 going from all-zero to
`[254 0 20603 0 0]`. Saved as
`build/good-packs/cedar-checkpoint-written.{pdi,pgm,log}`. Note this also
re-enables rollback: `FileInit.CheckpointThings` clears `switches[r]` only
when the volume has no valid checkpoint, so that volume can now be rolled
back.

## 6. Still open, and what has been ELIMINATED

**The restored screen is a freshly-booted Cedar, not the checkpointed
desktop.** The round trip completes without error, but afterwards the machine
goes on printing the FRESH boot's "install a personal profile?" dialogue
(3 lines at 40.6 B, 5 by 42 B) instead of resuming the desktop.

### Eliminated by measurement -- do not re-investigate

- **"RollbackImpl never runs."** It runs. At 39.0 B the screen reads, in its
  own words: `Checkpoint.` / `Counting swapped-out VM pages ...` / `The VM has
  65280 pages, of which 9760 are swapped-in and 0 are swapped-out (in 0 runs).`
  / `Ensuring checkpoint file length is at least 10386 ... ok` / `Copying VM
  into checkpoint file`. **10386 is exactly the number of distinct pages the
  germ outload wrote** -- Cedar and the germ agree.
- **"`switches[v]` is lost across the reboot."** It survives.
  `DORADO_MP_TRACE` now prints the four switch words at `GERM_SWITCHES_VA`:
  `000000 000001 000000 000000` (v = ordinal 31 = word 1, value 1) holds from
  germStarting through 845/850/855/860 and on into the running system, then
  goes to all-zeros exactly at the second boot -- the signature of
  RollbackImpl's `Booting.Boot[..., switches: ALL[FALSE]]`.
- **"The outload corrupts the volume."** It does not. All 10,385 written
  pages carry fileID `[254 0 20603 0 0]` -- the checkpoint file, which has
  10,973 pages on the volume. (Look labels up in the POST-run image: the
  checkpoint file is created by `File.Create` DURING the run, so the pristine
  image still shows the previous owner and makes this look catastrophic.)
- **"The fresh Cedar overwrites the checkpoint's data."** 510 interrupt-driven
  writes do land on checkpoint pages between outload and restore, but at file
  offsets ~18540+, which is `RollbackImpl`'s own `PutMapPage` loop inside
  `EnumerateRuns` ("Mark it dirty because we won't restore the backing disk.
  And mark it referenced so it won't look vacant!"). Intended behaviour.
- **"The restored pages never reach VM."** They do. The restore's `data_ptr`
  values mirror the outload's exactly -- 276 distinct, same
  `0o400..0o17533400` range -- so pages are written back across the same VM
  addresses they came from. Totals agree too: 10,386 distinct pages written,
  10,385 read back (1 read that was not written, 2 written not read -- header
  and trailer bookkeeping).

### The two live leads

1. **An unexplained `811 germOutLoad` inside the RESTORE boot**, at 38.690 B,
   between `812 germInLoad` (38.683) and `812` (38.696) -- yet phase-3 IOCB
   analysis shows **zero write IOCBs** in that whole window, and 6.4 M cycles
   is far too short for a real `DoOutLoad` (whose map walk alone is ~39 M).
   So the germ enters its outload arm and leaves it without doing I/O. Trace
   `GermSwap.pRequest.action` / `pMon.responseKind` across that boot: a plain
   `Boot[boot: file[...]]` should never reach `DoOutLoad` at all.
2. **What executes after the inload.** The germ ends `DoInLoad` by jumping to
   `header.continuation` ("inLoad exits through JumpCall2"). If VM is
   correctly replaced but the continuation is not honoured, the machine would
   keep running the pre-restore context -- which is precisely what the screen
   shows. Instrument the control transfer at the end of the inload, and check
   `header.continuation` / `pStartListHeader` as read back from the file.

## 7. Things that look like the bug and are not

Each was checked; recorded so nobody re-runs them.

- **The empty `checkpoint` root slot.** `VerifyFile` creates it on demand.
  (Iago cannot install one -- its `Install` is typed
  `File.VolumeFile[microcode..bootFile]` -- so empty is the expected state of
  a fresh volume.)
- **`RollbackImpl` not being loaded.** It is absent from `Basic.Loadees` but
  IS in the boot file: `BasicCedarDorado.boot!22` contains its message
  strings. Its `.bootmesa` explains why it must be.
- **Boot switches not reaching Cedar.** They do. `--boot-switches 3`
  (`FileInit`: `CallDebugger["Key stop 3"]`) takes the machine to 0 px. `v`
  is ordinal 31, so word 1 bit 15, and the log shows
  `SD[sBootSwitches] = 0 1 0 0`.
- **"The volume was not written."** An artifact of the harness: PDI writes
  only reach the file with `DORADO_PDI_SAVE=1`.

## 8. Harness bugs found on the way

- **`--shot-every` starves the paste queue.** With screenshots enabled the
  paste event fires (`dorado: pasting 11 chars`) but no characters reach the
  guest, and the test silently "passes" having never run the command. **Do
  not combine `--shot-every` with `--paste`** until that is fixed.

## 9. Regression gates run for this change

12/12 tests; `verify-cedar-desktop` 245,635 px; `verify-alto-disk` 2092 px
(exact expected value); `verify-cedar-ls` PASS; cold Cedar boot reaches the
login screen with **0** CHS adjudications.
