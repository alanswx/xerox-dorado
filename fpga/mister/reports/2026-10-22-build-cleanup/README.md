# User-requested old build-cache cleanup

Mac: removed2610 generated Verilator fast/slow precompiled headers,57002902288 bytes, under build/rtl-executive. No tracked PCHs, active Dorado compiler or simulator. Source, generated HDL/C++, compiled executables, reports, all disk images and protected Executive files remain. The build tree shrank114G to61G. macOS still reports14GiB free; Time Machine local snapshots exist and may retain deleted blocks. No backup snapshots were purged, and no physical free-space gain is claimed for the Mac.

misterubuntu: removed133 db/incremental_db directories from67 superseded Dorado Native/Serial experiments dated Oct17..20,16817173243 bytes.950 protected project/source/report/bitstream files hash-identical before/after. Native26/34/49/59 and all Oct21/22 projects retained, including current57MHz and rejected60ns timing references. No active Quartus process. Other projects, card backups and repositories are untouched. Reported free space666G to682G.

All removed paths, byte counts, plans and completion checks are retained. Deleted PCHs/databases regenerate if old builds are compiled again. Keep frozen sources and text reports for review; these caches are not required to run the preserved executables or bitstreams. Current MiSTer14 Main/core/server were not restarted or changed by cleanup.

Verify this receipt with tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-build-cleanup.
