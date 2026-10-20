# Corrected MC1668 native Viewers and input regression

PASS: the SAME original-firmware fresh-seed boot from the sibling
`2026-10-20-mc1668-native-field` receipt reaches Viewers with Ready, two
CommandTools, Clock, WatchTool and EditTool. Main3486/server3485/RBF15730f44,
50MHz/SYSPER4/adminOFF and unchanged Main21e6/serverc599 remain in use.

Actual virtual USB mouse movement reaches native912,8 Cmd. Clicking opens a
second CommandTool; after its visible initialization, separate focus737,608
accepts exact paced keyboard input:

```
echo rtlmc1668 keyboard ok
rtlmc1668 keyboard ok
echo abcdefghijklmnopqrstuvwxyz0123456789
abcdefghijklmnopqrstuvwxyz0123456789
echo rtlmc1668persist > ///rtlmc1668persist.txt
type ///rtlmc1668persist.txt
rtlmc1668persist
```

The initial marker capture was mid-render. The settled capture completes without
retyping. Background startup continues in the original pane. The bounded
collector spools fixed-size log prefixes on SD and checks105,666 complete disk
records with no error or trace loss. Each collected prefix matches its hash
after extraction.23 selected evidence members are losslessly archived/read back.
No live volume copy, guest CPU/memory/IOCB patch, state import or host instruction
completion is used. Source, actual input logs, captures and metadata are retained.

This qualifies ordinary original-firmware desktop/input and warm file readback.
It does not qualify clean OS shutdown, remount/cold persistence, Checkpoint/
Rollback, full FPGA snapshots, original60ns clocks or sustained stress. The idle
login rate remains1,947,686.01 issues/s; no speed gain is claimed.

Readable screenshot: `docs/images/cedar-mc1668-native-input-2026-10-20.png`.
Next: finish native clock/memory-port integration and the remaining acceptance
gates in `docs/verilog-clock-plan.md`. Preserve this baseline through normal
Main disk/network loader barriers when reloading.
