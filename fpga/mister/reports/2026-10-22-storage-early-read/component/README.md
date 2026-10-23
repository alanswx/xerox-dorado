# Early requested-word response: component proofs; mixed performance

Default-off EARLY_READ_RESPONSE exposes the selected word while BL8 drains. No new state. Next-request acceptance, cache validity and physical read_return still await full drain; the tail cannot rearm a consumed reply. Native wrapper unchanged.

Both simulators pass off/on arbitrary-DQ temporal miters:281 accepted requests, every selected beat, writes/hits/row/rank changes, refresh, backpressure and every reset position. First8 cold reads save28 ticks (7-offset). On ACK counts273/276 differ because three early replies were consumed BEFORE reset while their slower parent replies were canceled. No orphan/duplicate reply escapes. Four actual broken variants reject in BOTH engines.

Independent XSDS pin/coherence/reset/refresh/backpressure tests pass57/66MHz, with three physical negative controls. Hot64-read streams remain65ticks. Mixed96 operations:57MHz444 control/455 early (slower);66MHz455/444 (faster). Unfavorable results are retained.

Original144-chip MSA passes12012 cycles,750 reads,7872 writes,1111 output transitions/all9437184 bits. Canonical operation/data order unchanged. Lookahead/capture control104113ticks versus early103659:454/0.4361% fewer in THIS fixture. Overlap/no-lookahead control103525 is slightly faster than early103659; default-off113899. Wrong owner rejects. No FPGA speed, native timing or firmware qualification.

All inputs/logs/controls/traces retained losslessly, no simulator binaries. Verify: python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-early-read/component
