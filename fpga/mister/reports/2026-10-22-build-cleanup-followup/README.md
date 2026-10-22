# Additional user-requested build-cache cleanup

This pass removed 9,826,424,194 bytes of regenerable build caches.

Mac: removed 238 ignored Verilator precompiled headers, 6,571,291,144 bytes, from old `verilog/verilator/obj_*` directories. Only files older than October 21 were selected. The directory shrank from 8.5G to 2.3G. All 111 protected files, including compiled simulation executables, the Executive build, and unrelated working notes, are hash-identical. Newer builds were retained. Reported filesystem availability remained 18GiB; Time Machine snapshots may retain deleted blocks. Backup snapshots were not removed.

misterubuntu: removed 34 Quartus `db`/`incremental_db` directories from 13 inactive Dorado experiments dated October 16–20, 3,255,133,050 bytes. All 294 protected project, constraint, report and output files are hash-identical. All Native/Serial references, all October 21/22 projects, all source and disk images, other repositories and card backups remain. The active timing-path fit (builder 818951, fitter 830410) and reporters 830574/830575 remained running; its database was explicitly checked present after cleanup. Reported availability increased from 675G to 678G.

The exact paths, sizes, age guards, preserved hashes and completion checks are stored in the lossless receipt. Cleanup did not stop or reload the MiSTer. The earlier cleanup is recorded in `../2026-10-22-build-cleanup/README.md`; its totals are separate.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-build-cleanup-followup`.
