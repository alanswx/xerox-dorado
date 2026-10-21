# Native baseline passes image handoff; changed CPU fails

The preserved MC1668 native CPU CONTROL V15 passes original Initial,3,600 loaded
IM words/256 IFUM entries and Cedar entry238. The new Ready/front CPU candidate
fails3,856 IM/IFUM items with entry not reached. Both execute the same original
reply stream/pack and byte-identical deployed I/O, memory and IFU helpers.
All640 captured CPU receive records match exactly. `matched-analysis-v2.json`
under the candidate's logical members verifies those identities and records
every differing common frozen input. This establishes the CPU integration
regression in the component fixture, not its eventual hardware cause.

The baseline's later wait is task12, after2,153 task0 host samples; task12
accounts for the remaining5,431,273 samples. No instruction-fetch request or
germ entry occurs. These direct-ROM-entry/raw-MSA fixtures omit BaseBoard and
software-boot interaction and cannot qualify Cedar's desktop on FPGA. The
earlier [receipt](../2026-10-21-r-front-native-handoff-diagnostic/README.md)
contains V13's generic-CPU comparison and V14's IFU-only control.

Native RAM shadows V3 pass50M with the deployed memory controllers. V4 adds
settled asynchronous read-output comparisons outside writes; all12 packages
pass at least28,107,459 read comparisons each. Stored-word coverage remains
155,922 RM write windows/39,916,032 words per RM package and3 STK windows.
The shadow outputs drive no target net. These observations do not establish
every transient or physical timing at the target frequency.

Q shadow V1 is **terminal FAILED for insufficient coverage**. No sampled
mismatch occurs, but the three-sample filter observes only71/91 packages;
20 phase/address packages have zero checks. Its simulator exits0 and the
runner correctly rejects coverage. V2's one-sample settling filter observes
all91 packages (minimum31,487 comparisons) and passes20M startup, before the
download. These open-loop models observe candidate pins; they do not replicate
the original CPU's independent feedback or prove full-machine equivalence.
V1's checksum PCT ends at5191 and suggests mismatches in the16-word VM0e10
line. Its nonzero delta groups are0e11..0e13 and0e15..0e1f. CIA-change operands
are unsettled, so this remains a placement hint for the next trace, not verified
consumed MD data. The original checksum/retry behavior is retained.

At collection the paired V16 cache/storage traces and full50M one-sample Q3
probe were verified live as7072/7220/7608. Their logs are **running prefixes**.
V16 records CPU/MD/VA/phase/Hold, all14 cache bank addresses/selects/write
enables, D0/D1 inputs and raw/delayed storage data around Initial's xxxxE10
accesses and matching input-task BootDataPtr, plus32 trailing samples. Some
early setup records precede full error enable; align valid records using the
parity-enable event and instruction/task before comparing sample numbers.
Inspect actual process/state and finish these jobs; do not restart on timeout.
Find the first changed memory/producer capture before modifying RTL.

The archive maps3,982 members to481 deduplicated blobs:48,273,578 raw bytes,
5,507,584 stored bytes, all verified losslessly. It includes exact frozen
sources/helpers, terminal failures and running prefixes, original EB/replies
and native inventory. FPGA images, executables, databases and disk packs are
excluded. Verification:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-front-native-handoff-closed
```

No production RTL, clock, Main or daemon change resulted. MiSTer14 remains
RBF57a552af/50MHz/SYSPER4; ordinary-instruction ceiling6.25M/s against16.667M/s
target. Last qualified idle-login result is1,947,686.01 issues/s. Same-clock
Cedar recovery, speed migration, authentic rollback and final acceptance remain
open. Complete FPGA snapshots remain deferred; goal ACTIVE.
