# Strict F10145 MLAB placement: rejected

The requested memory placement was not inferred and area increased. This
variant is not selectable in the full-core builder and was not deployed.

The experiment changes only the shared-write helper's RAM hint from logic to
strict MLAB. Preprocessed functional RTL matches after removing that hint;
the semantic SHA256 is recorded in provenance.json. No no_rw_check attribute,
latency, state exclusion, clock change or extra timing exception is introduced.

Both fits complete and all four timing conditions pass. That result does not
prove memory placement. Quartus reports Info276009 for both mem and mem__dual:
unsupported read-during-write behavior prevents RAM inference. Total MLAB
memory bits are zero. The fallback increases the shell from 96 to 107 ALMs
and 139 to 202 registers. The candidate minimum timing margin is +0.170ns.
Location names containing MLABCELL do not establish that memory was inferred.

rejection.json records REJECTED_MAPPING_AND_AREA and includes the independent
source/timing review. All 114 raw files pass lossless hash readback, including
preprocessed source, map/fit reports and every corner. Raw data is gzip stored
unchanged; generated databases are excluded.

This trial implicates read-during-write requirements. A next experiment can
return zero on private administrative writes, matching the already-zero
public fabric response, while proving guest state, reads and normal outputs
unchanged, then recheck strict memory inference. No such variant is qualified
by this report. Larger reductions in parallel state-access logic may still
be required to fit the complete design.
