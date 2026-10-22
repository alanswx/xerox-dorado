# Faster native Cedar desktop: measured 11.0% issue gain

The reviewed2278 image boots original firmware to Guest.pa and then the full
Cedar Viewers desktop on MiSTer14. Main19164/21e6, receiver19163/0845 and loader
19134 are hash-bound/live. The same immutable236b seed and57.142857MHz/SYSPER4
are used. The complete8M-cell address/zero selftest gates guest release; normal
C is released at the actual113 request. No early OSD or extra keys before the
visible Guest login. Measurements precede accepting Guest or display controls.

| Separate quiet-login metric | matched cache4/9e | overlap+parallel lookup/2278 |
|---|---:|---:|
| Mean StartCycle issues/s | 2,968,816.02 | 3,295,738.51 |
| Held instruction cycles | 0.180632% | 0.185804% |
| Enabled host edges | 41.637317% | 46.231986% |
| Storage-only host-edge pause | 58.361471% | 53.768315% |
| Disk-only / both pause | 0% /0% | 0% /0% |
| Raw-buffer hit/read delta | counter unwired | 75.0000% |

Three adjacent10s new issue windows are3296053.24/3296186.37/3294975.90 per
second: an11.0119% gain over the matched earlier boot, about19.7744% of the
original16.67M/s nominal. These are issues qualified by Run/Jam and excluding
RepeatCur, not pipeline-drained retirements. One new boot versus one matched
control is not three independent fresh-seed qualification boots. Cause views
are separate intervals, not an atomic decomposition. All selectors return Off.
Nominal ordinary cadence remains140ns before waits; original60ns is pending.

Two normal Returns accept Guest.pa/empty password. All34 ordered base-file
lookups and sizes match original Basic.Loadees (manifest+33BCDs, including
End.bcd); the raster shows every Loading...ok.236b caches those files, so this
is not34 full network payload transfers or a pinned whole-loader time. Profile
Yes/missing-full-name/default No and the older-font No prompts advance without
profile text input. Original Viewers, both tools and the icons initialize.

Normal mouse Cmd/EOF clicks create and focus a clean second CommandTool.
Exact `Echo response57 exact` and all26letters/10digits return correctly,
checked at higher magnification. Type window57persist.txt returns its exact
old content after the actual cold FPGA/Main/daemon/cache transition. A normal
guest Echo redirection creates response57persist.txt and Type returns its
exact content; this new file has warm readback only. Normal middle click opens
Clock, and a later native frame shows its hands redrawn. Startup ImagerPress/
Interpress import diagnostics remain as before; this is not an all-apps pass.

Before loading2278, the prior93ad desktop volume closes with10288 complete
trace records,447 read/ACK pairs,65 write/ACK pairs,two barriers,no recorded
disk errors or trace-loss markers,no open pack/trace FDs and old mapping
revoked. The changed imagef9c92416 is copied/hash-verified on the Mac. This is
normal Main/host closure, not a clean guest shutdown. Its cache16persist file
still needs cold guest remount/readback; current236b is a separate volume.

Native captures are clear. HDMI stays black before/after CLI and web shortcut
sequences, after Viewers initializes, and in the immediate OSD capture. A12s
capture stream receives121 frames; its saved LAST frame remains black. This
does not establish why, whether host shortcuts were processed, or a permanent
Main fix. No resolution change, shared capture-service restart, debugger,
ptrace or second SPI owner. Existing exact Main source/config inspection is
read-only. The final live prefix retains TXdrop3,CRC/RX/abort0 and pack fault
clear; quiet pre-Guest measurement samples had no drops.

Both bounded native observers finish normally (9/52 samples); all key/mouse
senders release controls. Current2278 Checkpoint/Rollback,new-file cold remount,
HDMI,original60ns/all peripheral rates,Main337 isolation,final three fresh seeds
and two-hour sustained qualification remain open. A separate synthetic-MSA
replay verifies its final oracle and finds no eliminated reads with1/2/4/8-word
group-local LRU caches; it is not native evidence or a speed forecast, and
causes no additional RTL/cache change. Qualified Native59 is preserved and
complete FPGA snapshots remain deferred. No C-state import or host completion
of guest instructions/IOCBs.

116 frozen members pass lossless readback. Verify:
`python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-response-overlap/native57-desktop-speed`.
