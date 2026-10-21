# Passive first-burst trace: component and live fit prefix

A separate capture block retains the first physical read burst's eight 12-bit words and a valid flag (97 logical bits). It samples the same input/event as the existing cache fill and never drives controller state, delays or data. Ports and unused diagnostic pages 24..27 expose marker B178 and the retained words. The exact 501-source stage differs from the reviewed memory test only in this passive backend block, wrapper wires and actual core top readout. Synthesis uses those source files.

The bounded independent chip-pin test passes with all earlier fault cases. A reversed trace lane order is rejected at runtime. The full 23-bit test passes 50,331,648 requests/completions and verifies the initial trace remains intact after the complete sweep. The native build is a verified live prefix, not timing/assembly/hardware qualification. MiSTer14 still has StorageSelftest1 loaded and fails its first raw read: expected A59, actual A58.

The control simulation stopped naturally at its old logger assertion at 1,510,000,000 host ticks. The prepared read-only logger correction was reloaded at that existing stop and the same PID 83509 resumed; no interrupt or CPU/IM/RAM/clock/cycle/IOCB write was sent. The archived control observation at 1,470,000,000 ticks already shows CPU c0fc7000; an earlier handoff mistakenly described both simulations as d0fff000. The immutable earlier capture is the authoritative value.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-storage-burst-observer/component`
