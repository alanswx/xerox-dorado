# Native41d: real cold-remount persistence PASS

Both files created/closed on Native41c survive a new ARM process/cache,
physical-file reload and full FPGA cold boot. Actual Cedar CommandTool:

```
% type ///rtl41cpersist.txt
rtl41cpersist20261018
% type ///rtlflush42.txt
worker42
%
```

`cold-persistence-final.png` was inspected for the complete command text,
exact values and fresh prompt,22:10:14UTC. This closes the earlier distinction
between cached readback and cold disk persistence. It does not qualify
power-loss recovery, complete machine snapshots, three untouched-seed boots
or the final two-hour combined stress requirement.

Boundaries: normal41c guest Boot entered the germ; Main16875 drained/fsynced
and exited, server16872 stopped. The physical63578624-byte source was
preserved and copied, with equal SHA256
`81c4195570989a9d3ebf216f5600348af0c57997c738c4e98bf3ea6dd351ca04`.
The preserved source was rechecked unchanged after the cold readbacks.
Native41 RBF a97af936 was fully reloaded. New Main19438 (40e14b6b) preloaded
that physical file, new server19352 (e237dffc) served original firmware.
No C/RTL saved state, guest RAM/IOCB patch or ARM completion of guest code.

Actual BaseBoard/CoolBoot/BFN113, default Guest.pa login, all34 loadees,
profile No, BootEssentials/fonts and Viewers pass. Existing newer font DFs
required accepting visible No overwrite defaults with Return. A second
CommandTool opened through the real Cmd button and accepted mouse focus and
the two Type commands. The first attempt clicked at the menu border and
typed into the busy startup tool; the corrected center click/new-tool focus
produced the qualified readbacks. The FPGA keyboard queue was allowed to
finish pacing the input before evaluating the result.

Main video/input correction: `video_fb_enable(1,0)` selects Linux,0 selects
the core. The initial toggle left Linux visible; a final0 call restored
Cedar and input focus. No capture-card reset. The fourth known GDB control
was that correction. A fifth, diagnostic post-readback barrier attempt
failed **before calling Main** because GDB's selected language lacked bool.
Its log is retained; the helper now explicitly selects C++, but that change
was not rerun here. The later normal Main core-load/restart durability
barriers passed and are the actual retirement boundary.

The observer prefix has211 read ACKs,41 write ACKs,39 physical writes and29
fsyncs, no error/drop events. Max completed pwrite35.427613ms. Poll-gap
statistics include known diagnostic GDB pauses and are not a clean stress
latency claim. Prefix/statistics are archived; normal loader then flushes
the rest of the observer to `Cedar41d/disk-through-native42-load.jsonl`.

After readback, normal guest Boot again displayed germ812. The normal Main
loader drained before programming42 and restart. The retired41d physical
volume remains at its original path,63578624 bytes, SHA256
`c7420ed1a220f2b0fd3075cf25c5c149bc99c79fe326e326464d07160acca30c`.
`preservation.json` records that boundary. The original41c preserved source
also remains. Neither file is a full FPGA-state capture.
