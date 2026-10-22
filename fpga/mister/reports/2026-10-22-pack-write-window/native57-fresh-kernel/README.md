# Original Cedar fresh-kernel boot: Viewers, input and files

The original BootTool parses switches before calling Booting.Boot at the volume argument. Correct command is `Boot -f CedarWork`. The preceding `Boot CedarWork -f` returned the old checkpoint because trailing switches were ignored; that attempt and source audit remain preserved as a non-pass.

One correctly ordered normal-keyboard invocation, same Main7626/server7625/RBF9e on a copied checkpoint disk, enters the Basic boot file with explicit Boot switches:F (frame05). It loads/starts Basic.Loadees, runs the installer, accepts N at the profile choice and No for older font DFs, then reaches Viewers/Clock/Watch/Edit, Sil Library Files loading A-OK and the final DF/tool prompt. This is a fresh kernel on the copied disk, not a final fresh-seed/cold-firmware test.

Ordinary mouse Cmd click creates and initializes a second CommandTool (frame43). Earlier keyboard attempts edited old typescript at misplaced selections; all are retained and not counted as command execution. Clicking blank space below the lower typescript moves the caret to EOF. Echo fresh57 exact then prints exact fresh57 exact and a new prompt/caret (echo-v3.png).

Type of both existing ///rtl57link1persist.txt and ///rtl57quietafter.txt prints exact contents. Normal Echo redirection creates ///fresh57persist.txt and Type prints fresh57persist (files-v1.png). This proves live guest filesystem I/O, not cold remount persistence of the new file. The prior source1670 cold checkpoint recovery is a separate completed receipt. Any fresh application result is explicitly recorded in fresh-desktop-observed-v1.json and its images.

No extra FPGA load, repeat Boot, Main/daemon restart, guest-state import or debugger operation. Live process identity and complete-line log prefixes are hash-bound; prefixes are bounded observations, not a claim that the ongoing session ended. Native clock/speed unchanged. Final three fresh seeds, two-hour stress, full timing, isolated Main cache comparison and original60ns target remain open.

Verify: python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/native57-fresh-kernel. No disk, ELF or bitstream included.
