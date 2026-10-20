# Disk cache phase component qualification

The prototype stages cylinder/head at the host falling edge and uses those
values for cache-miss capture on the following rising edge. The prior readiness
receipt contains its actual-clock exclusion check, original/prototype state
comparison, serial oracle and byte-identical whole-image loopback. This receipt
adds independent source, state/clock inventory and matched physical timing.

Inverse review permits only the two fields and cache-source substitution; every
original port/process/clock/parameter stays byte-identical. Fresh Cedar-label10,
50 MHz XML preserves all 55 original fields / 131,816 bits and includes exactly 18
extra REQUIRED wrapper snapshot bits. Six source/seven geometry/three edge
controls reject. No excluded state or timing exception.

`matched-terminal` uses identical registered launch/capture shells and the real
falling-edge ALTCLKCTRL, 50 MHz, 2 ns-max/0 ns-min virtual IO and identical policy.
Capacity is 575 -> 579 ALMs, unchanged 16 RAM blocks / 131,072 buffer bits / two DSPs. Both
variants pass every operating condition with zero unconstrained clocks/IO;
candidate minimum +0.166 ns. Seven rehashed source/clock/pin/report/exception cases
reject against the passing candidate. The shell's registers are fixture logic;
the prototype adds eighteen logical implementation-state bits to the medium.

`rejected-syntax` retains the initial reserved-instance-name map failure.
`rejected-virtual-io` retains the unregistered fixture's real virtual-pin hold
failures (-0.035 ns original/-0.095 ns candidate); these are not timing passes.
The corrected matched fixture changes both shells together and retains the
same clock periods, delays and all setup/hold checks. No timing cut was added.
All three remote builds are terminal and generated no FPGA image.

All 263 manifest members pass readback; extracted fit/source controls and fresh
state/edge inventories replay against the prior immutable receipts. No working
HDL is required. This prototype is not in Serial65 or Serial66. Its new state
still requires wrapper snapshot access; native fit/timing and hardware adoption
remain separate.
