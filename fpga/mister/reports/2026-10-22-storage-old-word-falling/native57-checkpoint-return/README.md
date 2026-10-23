# 5c65: original Cedar Checkpoint returns to an interactive desktop

One ordinary `Checkpoint` command runs through the original Cedar/germ path on
the previously qualified 5c65 core, Main4355/daemon4354, without importing C
state, host completion, a reset or a repeat. The first bounded observer records
48 samples: sample12 shows counting swapped-out VM pages, sample18 actual
maintenance code812. Its final sample is a progressing faded desktop, not a
terminal failure. The second observer stops at11 samples after the original
Creating checkpoint / Rollback messages, NoteNewUser/NotePerLogin completion and
the final prompt are visible. Their original state/logs/screens are retained.

The final native capture shows the saved full ProcH-BitSlice07 schematic,
CommandTool, Clock, Watch/Edit icons and exact fresh `early57 checkpoint` Echo.
`Type ///early57persist.txt` returns `early57persist`, the file created before
the save, followed by a final prompt. Input was delivered through normal USB
keyboard/mouse and all keys/buttons were released. The readback sender passes
the file content directly as a subprocess argument, preserving its newline.
The pinned fake-time server explains the calendar going backwards between the
checkpoint and rollback messages; those timestamps are not elapsed-time data.

The bounded collection rechecks Main21e6, receiver0845, RBF5c65, source236b and
closed predecessora19a identities/hashes. It retains2,216 read request/ACK pairs,
14,894 write requests/14,895 ACKs,834 pwrite pairs and288 fsync pairs. Two explicit
diagnostic loss markers represent21,700 lost records; the write history is
incomplete and its count mismatch is retained. There are no recorded disk
errors; the last network line has good356, CRC/RX/abort/drop0. This is not an
assertion that unrecorded history was error-free.

This qualifies original Checkpoint return and post-return interaction/file
readback. It does not qualify a separately issued Rollback, post-save file cold
persistence, a current-volume close barrier, all-device rates or final stress.
The measured4.02547345M prelogin StartCycle issues/s and57.142857MHz/SYSPER4/
140ns nominal remain the separate preceding measurement, not a new speed gain.
No bitstream, disk image, compiler database or executable is stored here.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-old-word-falling/native57-checkpoint-return`.
