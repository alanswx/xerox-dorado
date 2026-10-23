# Cold 5c65 external-video follow-up

MiSTer14 remains on the same5c65 core/Main13244/daemon13243 and open cold volume.
The initial external capture is black. Normal Ctrl-Alt-F9, a capture, F12 and
Escape also produce black capture; the latter returns Main to its core
framebuffer. No direct framebuffer-state query proves the intermediate selection,
so the filename host-framebuffer.png denotes the probe time, not a proved state.
All keys were released by the normal CLI. No debugger or second SPI owner.

Read-only ADV7513 bus1/address0x39 returns41=10,42=f8,af=04,a1=00.
Main's source tests42&60 for the attached display; this check passes. It does
not prove a valid decoded picture. No transmitter register was written.

One temporary standard720p60 runtime modeline was sent via /dev/MiSTer_cmd,
followed by restoration of the original1080p30 in finally. Main acknowledges
both exact modelines and their74.25MHz PLL settings in video-log-readback.json.
Both external captures remain black. Persistent MiSTer.ini was not changed;
no guest reset, core load or capture-service change. The active pack was not copied.

The native screenshot AFTER these probes is clear: original saved processor
schematic, CommandTool, Clock and the exact cold file/input results. All latest
recorded packet CRC/RX/drop/abort counters remain0 and pack fault is clear.
This rules out a blank native guest framebuffer at that observation; it does
not identify the external-video failure. HDMI remains OPEN. Preserve original
interlace (HM printedp115/PDF122). This is not a clock/speed qualification.

manifest.json binds the retained bytes. probe-mode.py and mode-actions.json
retain the executed temporary command sequence and return codes; native capture
metadata names the actual Main screenshot. The first framebuffer key commands
are recorded in the session transcript rather than a separate raw command log.
