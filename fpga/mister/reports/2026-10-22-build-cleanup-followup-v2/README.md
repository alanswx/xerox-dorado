# Follow-up cleanup of old Dorado build files

User requested cleanup on the Mac and misterubuntu. Both cleanup scripts completed with PASS. No compiler was using the affected files. The live MiSTer session was left running.

- Mac: removed 21,784 generated object/archive/header cache files (4.45 GiB), and compressed 61 old top-level diagnostic logs from 13.61 GiB to 0.63 GiB. Every compressed log was decompressed and checked against its original SHA-256 and length before the raw copy was removed. Net logical reduction: 17.43 GiB; `build/rtl-executive` now occupies about 49 GiB. Read the `.log.gz` files with `gzip -cd`, or decompress a needed log before replaying a tool that expects its old path.
- misterubuntu: removed 29 inactive `core/db` and `core/incremental_db` directories (3.79 GiB) from older Dorado experiments. All 25,035 other files in those project trees remained hash-identical. Free space subsequently measured 678 GiB.

All October 22 Ubuntu builds and qualified Native26/34/49/59 fallbacks were retained. The current V1/V2 timing candidates and read-word-capture component were excluded from local cache removal. Source, disk images, snapshots, executables, bitstreams and test results remain. The protected Executive executable and 106 files in its object directory were checked unchanged, and the tracked working-tree diff was identical before and after cleanup. Removed compiler caches must regenerate for a future rebuild.

The Mac still reports about 14 GiB available: deleting files has not produced an immediate physical free-space gain. Time Machine local snapshots are present and may retain their blocks. Backup snapshots were preserved.

`manifest.json` identifies the gzip-compressed plans, exact removed-file lists, original-log hashes, preservation hashes, scripts and completion records. Each stored member was decompressed and compared with its source when this receipt was created. Full working copies also remain under `build/rtl-executive/cleanup-followup-20261022` and `/home/alans/mister/dorado-cleanup-followup-20261022`.
