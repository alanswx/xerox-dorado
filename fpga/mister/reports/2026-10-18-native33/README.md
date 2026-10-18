# Native33 experiment: preserve cold-read latency

The live Native32 retains DDR lanes but adds one lookup cycle on a miss.
Its firmware-enabled fraction25.0156% versus uncached26.4617% showed no gain.
Cedar performance is still being measured. Avoid paying that extra cycle:
synchronous tags settle during LOOKUP, so a miss can assert its DDR read
in that same cycle. A busy bus holds it into ISSUE; a zero-latency response
completes and fills immediately. A hit never issues a DDR command. Writes,
byte enables, reset draining and response backpressure are unchanged.

Existing uncached regression and458-transaction cache scoreboard pass.
New requirement checks that a cold read reaches DDR immediately after raw
request acceptance, just like the uncached adapter. Native32's exact old
adapter fails that check (bubble-control.txt). Full storage equivalence:
1505 three-group cycles/509reads/463writes;12012 full-board cycles,2944/2992
reads,7872writes,1111 output transitions; every2,359,296/9,437,184 bit matches.
The16-bit fixture now takes213us of simulated host time versus235us with
Native32; this is not a measured FPGA speedup. Strict module lint passes
with only the FPGA PROCASSINIT power-up convention suppressed.

Reproduce with the storage-ddram and storage-linecache test targets.
The failure control is the adapter from git c1935a2f with the current test.
Frozen build on misterubuntu: Dorado_Native33_20261018, cloned Native32;
only the adapter changes. PLL50MHz, SYSPER4 and128-line cache retained.
Build is running; timing/clock/hold and original firmware/Cedar/input
qualification are pending. It is not loaded. Native32 is running Cedar
login/loader on14; native32's frozen sources/evidence remain unchanged.
