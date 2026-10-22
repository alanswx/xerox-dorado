# Native V42 control: terminal at original limit, no firmware pass

The unchanged57.142857MHz/SYSPER4 parent7c control ended normally at its existing1.5B host-tick limit:592,791,510 enabled source edges, CPUc0fc7000, no BFN113. Original physical-memory startup passes. Normal BaseBoard input was three presses, selected from period DoradoContinuous.masm; no CPU/IM/manifold state plants or host guest-instruction/IOCB completion. This is not a firmware or CPU-front qualification.

All528 frozen input hashes and160 compiled-used HDL files verify. Original4 stages return0, with the test's explicit HOST_LIMIT outcome. The executable was not restarted or extended on an observation timeout. Simulation reports11,981.950s wall time for26s simulated host time; this is simulation throughput, not FPGA instruction speed.

The real FPGA subsequently reaches original113/Cedar login on the separate9e cold checkpoint attempt. Compare source/clock/input and actual progress before choosing a new bounded control. Do not convert this incomplete control into a pass.

Verify exact frozen evidence with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/native57-cool-control-v42-terminal`.
