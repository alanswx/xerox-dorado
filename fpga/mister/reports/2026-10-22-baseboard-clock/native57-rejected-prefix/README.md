# Clock candidate rejected by memory-response timing

The same BaseBoard-only candidate finishes map/fit but fails setup: ordinary summary -0.399ns; the already produced slow1100mV/-40C detailed path reaches -0.423ns. Actual launch is `b_msaOdd/u_dram_addr_1/col[3]`, capture is the raw backing buffer's `rsp_data[8]` and its duplicate. Hold summary is only+0.003ns; final detailed minima are not yet established.

This negative path rejects assembly/load. The builder and queued read-only reporters continue so their remaining evidence can be collected. This receipt preserves the exact preliminary source reports and hashes; it is not a terminal report or firmware result. The original qualified2278 core remains on MiSTer14.

Next: collect the same job to completion, inspect the new clock-chain reports, and shorten the real arbitration/address-to-tag/word-response path before another native candidate. Registered request lookahead is an investigation, requiring per-group data/ordering, fairness, held response, reset, refresh, pin-level MSA and original-firmware evidence. No new implementation, clock speed or hardware gain is claimed.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-baseboard-clock/native57-rejected-prefix`.
