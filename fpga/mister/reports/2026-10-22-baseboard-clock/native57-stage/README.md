# BaseBoard crystal ratio: isolated native candidate

The legacy fixed fabric/8 oscillator makes MCPreClk run 5/3 too fast relative to ordinary instructions at board SYSPER4. The original Rev Am Basebd01 crystal is 10MHz, followed by the original e07 divide-five and g08 divide-two chain. The candidate activates the already tested scaled oscillator with `2*SYSPER` samples per original60ns instruction. This restores the ratio, not physical real-time execution on the current slower core.

All503 qualified response-overlap sources were checked. Only the oscillator cell and the actually compiled native16 BaseBoard d08 binding change; configuration changes relocate the project. CPU, memory, other peripherals, host PLL/SYSPER, Main and daemon remain the same. Independent source comparison and exact inverse of the binding pass; five changed-source/qualification/divider/component-proof controls reject before Quartus. The archived component evidence covers2/4/8/16 ratios in both simulators, the unchanged default waveform, four faulty controls and the original generated-board6502 reset bytes. The native16 binding still requires its own firmware qualification.

One full nine-stage fit was started on misterubuntu at `/home/alans/mister/Dorado_BaseBoardCrystal57_1_20261022`, builder530332/map530341. The recorded observation independently verifies both live processes and all uploaded503 source hashes. It is a running prefix, not a fit or timing result. No assembly or hardware load is permitted by this receipt. MiSTer14 still runs RBF22781f30.

Next: observe this same job, review all original and response-path timing categories, memories, warnings and BaseBoard clock paths before considering assembly. Original60ns CPU timing, remaining peripheral rates and final firmware/rollback/stress gates remain open.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-baseboard-clock/native57-stage`.
