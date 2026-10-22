# Original V1 combined candidate: full terminal timing rejection

The same original fit and read-only report collection at Dorado_TimingPaths57_1_20261022 completed; no retry/refit or assembly was performed. All1096 collected source/report members were copied and read back. All31 summaries and444 detailed timing categories/four conditions were inspected.98 detailed categories are negative: minimum setup-15.454ns, hold-0.061ns. Source count503. This is rejection evidence, never permission to assemble.

The actual20-level address/cache-tag -> backend response path includes cached_choice/raw_accepted/selection_origin feedback and a5.864ns LOOP. Functional component tests had allowed Verilator UNOPTFLAT, so their passing settled outputs did not establish acyclic hardware. The separately reviewed V2 repair removes this feedback and rejects the actual old source with fatal circular-warning checks. The V1 source and unfavorable results remain preserved.

Bounded local observers V1/V2 expired while the original remote work continued. A distinct V3 observer later collected the SAME terminal job. Timeout was not treated as job failure or grounds to restart. This archive includes all three observations and the final terminal diagnostic. Native59, clock goal and final firmware acceptance are unaffected.

Exact terminal source/reports, diagnostic output and collection history are deduplicated against the V2/staging receipts. No bitstream, executable or disk image is included. Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-timing-paths/native57-rejected-terminal`.
