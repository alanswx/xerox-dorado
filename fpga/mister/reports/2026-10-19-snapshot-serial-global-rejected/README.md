# Serial67Global: rejected timing experiment

The exact Serial65 RTL fits with network_step globally routed: 41,280 / 41,910
ALMs (630 free), 52,555 registers, 329 RAM blocks, 2,498,471 memory bits. Actual
Quartus fitting creates network_step~CLKENA0 on GCLK2. This is a data signal
routing policy; the actual clock definitions, periods and constraints remain
unchanged. All 659 prepared inputs and 2,761 original pins/parameters check.
Builder 2513527 and every project process are absent. No assembly or hardware.

Timing REJECTS it: setup -6.078 ns at slow 100C and -5.519 ns at slow -40C.
The worst path runs from network_step through the global routing and serial
state-accept/abort logic into BaseBoard. All reported hold checks pass (minimum
+0.118 ns), as does SDRAM IO. All 304 board crossing reports are reviewed;
63 groups fail setup. Actual clocks and inherited three input/fifty output
exceptions match Native59; zero illegal/unconstrained clocks. This is not
complete physical framework IO coverage. No new timing waiver or relaxed period.

All 1,668 raw members read back losslessly. Extracted source/capacity/timing/
crossing readers and all six/four/six/three controls replay with prior immutable
readiness tool dependencies and the passing Native59 timing baseline.
Serial65 remains the better candidate; no real service barrier, complete
snapshot restore, deployment or speed gain is claimed.
