# Passive clock-count and coherent-debug candidate

Frozen build: `misterubuntu:/home/alans/mister/Dorado_Native30_20261018`.
Based on Native29's40MHz/SYSPER4, adding only passive clock counts and
coherent command70 debug/status snapshots. Map passes; fit/timing and
hardware qualification are pending. Native29's inputs/RBF are unchanged.

Counter regression compares against actual gated rising edges, including
long pauses, reset and modulo wrap. UIO512/1024 tests change both debug words
and status after command70 starts; every returned half must retain the
command-time observation, and the next transaction must see the new values.
The old UIO RTL from5d3dd384 fails `wrong debug low word`, proving this test
detects the old live-word implementation. Framing/CRC/backpressure and
packet commit/abort tests still pass.

In native `Debug monitor=Clock counts`, existing Main `machine=` reports
enabled fabric edges, `cpu=` reports total fabric edges. Both are captured
together, modulo2^32. Main's normal SPI ownership/protocol is unchanged;
medium-fault diagnostics override count mode. Switch back to `CPU` for PCs.

Use unsigned modular deltas over intervals shorter than one counter wrap:
107.37s at40MHz. `delta(machine)/delta(cpu)` measures the enabled fraction.
Counts are **not instructions**. Dividing enabled edges by2*SYSPER gives
only an instruction ceiling: original Hold/extended IM/TPC cycles remain.
No guest registers/memory, disk completion or CPU control are changed.
