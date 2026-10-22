# MC10181 direct carry conditions: all16384 binary inputs/seven pins pass

The measured native setup limit includes processor arithmetic/carry/diagnostic selection. This separate component starts from the exact qualified690f MC10181 cell (SHA6a1041f76a2262a64b3f7eb803a14c80ae356bb79997928cb8c1b86df714fefa), preserving its original function table, FPGA group-sum adder, parallel increment/carry selection and all four result pins. Only the three carry pins are re-expressed.

Algebra of the original X/Y table gives bit-generate X&Y from S1/S0:0,A&~B,A&B,A; bit-propagate X|Y from S3/S2:A,A|B,A|~B,15. These feed ordinary bit carry-lookahead. Pin4 GG remains carry-out if Cn=1; pin8 PG remains carry-out if Cn=0, preserving the original MECL polarity rather than TTL naming. COUT remains forced low in logic mode. This avoids routing carry flags through the result sum while keeping the currently qualified fast result path; it differs from the earlier fully Boolean result-adder rewrite that failed native timing.

Both Icarus and Verilator exhaust all16384 binary inputs and all seven pins against the exact old cell, with independent arithmetic/polarity checks. Wrong select-group, lost-Cn and wrong-GG variants each fail in BOTH engines. Fatal UNOPTFLAT checks are enabled and no circular warnings occur. All frozen inputs remain unchanged.

This is complete binary equivalence for ONE combinational cell, not four-state equivalence, temporal glitch behavior, native timing/firmware or a measured speedup. Full source staging, board/native timing/capture/RAM review and original firmware remain required. No canonical cell, register, clock, source on MiSTer or running690f is changed. The existing140ns/original60ns goal and all final gates remain open. Exact old/new source, finite exhaustive bench, both-engine logs and six actual negative variants are stored without compiled binaries/caches.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-mc10181-direct-carry/component`.
