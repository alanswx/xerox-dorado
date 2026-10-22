# Native disk-window desktop and live input

RBF `9e0d4163`, unchanged Main `21e67a3f`/receiver `08452a86`, same Main5080 and server4619. Ready/Viewers, normal mouse/new initialized CommandTool/focus, exact short text, lowercase and uppercase alphabets, digits, and live file creation/Type readback pass. `interactive-checks-v1.json` identifies exact commands/screenshots.

The 70,741-record prefix contains 22,768 complete write request/ACK pairs, all within their loaded full-track span. Core WRITE traffic is 23,430,656 bytes versus 366,900,224 whole-track bytes for those same requests (93.6139% reduction). The 3,612 reads remain whole-track. Backing-file I/O results match; scheduling poll gaps remain visible. Seven damaged-trace controls reject. Reproduce with the frozen inspector/input. This quantifies bytes, not pinned elapsed performance or instruction speed.

Cold media persistence and checkpoint remount are still open. This inherited run is excluded from clean stress/latency because of earlier host trap/OOM; those are preserved in the first-firmware receipt. Normal 720p60 output was acknowledged but HDMI remained black and1080p30 was restored. Read-only Main globals prove the Linux framebuffer is off; no debugger attachment or injected call was used.

Lossless evidence readback: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/native57-window-desktop`. Qualified503-source/timing/assembly and earlier failures are in the sibling receipts. No bitstream, disk image or ELF is included. Final60ns/three-fresh-boot/two-hour acceptance remains open.
