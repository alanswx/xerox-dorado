# Cache16 original Cedar desktop, input, files and Clock

The same reviewed 93ad core, Main15534/21e6 and server15531/0845 complete the
original installer and reach interactive Viewers on MiSTer14. The bounded
observer ends normally at 60 samples. A normal Cmd click creates a second
CommandTool; clicking below its text selects EOF. Exact `Echo cache16 exact`
and alphabet/digits (`abcdefghijklmnopqrstuvwxyz 0123456789`) return correctly.
Native screenshots and the actual MiSTerKVMS HDMI capture show the desktop.
A normal middle click opens the original Clock viewer, which redraws.

`Type ///window57persist.txt` returns `window57persist`: this is cold readback
across the prior cache4-to-cache16 FPGA/Main/daemon/cache transition from the
preserved 236b disk seed. The new guest command
`Echo cache16persist > ///cache16persist.txt`, followed by Type, returns
`cache16persist`. That new file has only warm readback; cold remount is pending.

Two agent errors are retained. The first redirection omitted the space after
`>` and echoed literal text instead of creating the file. Also, the digit 5
is present in BOTH alphabet runs: the initial visual report of a missing key
was mistaken. These runs demonstrate no keyboard loss. The startup tool also
retains unbound ImagerPress/Interpress diagnostics; this is not an all-apps pass.

The frozen live prefix verifies original source236b and RBF93ad hashes,
Main/receiver executable hashes and live identities. Latest CRC/RX/abort are
zero, TX dropped remains 3, and the pack-fault bit is clear. The disk trace is
buffered, not complete close/remount evidence. No new checkpoint or rollback
is claimed on this image; the earlier cache4 checkpoint proof remains separate.

This desktop result adds no speed claim: matched idle issue rates were already
effectively unchanged at 2.969M/s. The fabric remains 57.142857MHz/SYSPER4,
140ns nominal before waits. Original60ns, peripheral timing, three fresh seeds
and two-hour sustained qualification remain open. No C-state import, host
completion of guest instructions/IOCBs, binary replacement or new FPGA load.

31 frozen members pass lossless readback. Verify with
`python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-cache-capacity/native57-cache16-desktop`.
