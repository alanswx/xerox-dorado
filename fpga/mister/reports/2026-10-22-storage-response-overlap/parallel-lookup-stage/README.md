# Parallel buffer lookup: same component behavior, new native fit

The first overlap fit completed all nine builder stages but failed physical
timing: setup −0.363ns and hold −0.041ns. The setup path runs from the arbiter's
round-robin next_group state (and original group address inputs) to the buffer's
rsp_data register. The hold failure is a separate old_data-to-read_pipe path
inside a storage group. This fit is rejected for assembly/deployment. Its full
timing summary and all four conditions' global setup/hold paths are retained
here, with original503-source verification and actual terminal process checks.
Supplementary reporting and its complete collection are separate evidence.

The revised backend selects a word within each constant-index line alongside
tag comparison, then retains the original last-matching-line priority. It
removes the explicit binary line-encoder/data-mux dependency without adding a
register or response cycle. No duplicate-tag uniqueness assumption is required.
Fill, write invalidation, response stability, refresh and reset are unchanged.
The arbiter bytes are identical to the earlier dual-simulator128-group proof.

The complete independent pin/coherence/reset/refresh suite passes again with
both parameters off/on. Hot64 reads remain128/65host ticks; mixed96 requests
remain566/444. The original144-chip MSA again preserves all12,012 board cycles,
9,437,184 bits,750reads/7872writes/1111transitions and canonical operation order:
113899/103525host cycles. Default-off reproduces the frozen trace byte for byte.
All prior mutation controls reject. Six new staging cases reject, including a
parallel recipe paired with the serial transform; the original serial staging
mode also still passes its five controls. These are component results only.

One distinct project runs on misterubuntu:
`/home/alans/mister/Dorado_StorageResponseParallel57_1_20261022`.
Builder4184174 and fitter2946 were verified live; map passed. Reporter4186252
and collector4186253 follow the same job. The changed PID numbering reflects
normal host PID wrap. The503-source recipe still changes only the arbiter,
backend and wrapper; no CPU/clock/SDC/Map/ECC/peripheral/Main/daemon change.

Actual new setup AND hold closure are pending. No constraint waiver, new RBF,
hardware load or native speed claim. MiSTer14 stays on interactive93ad/Main15534/
server15531. Original60ns/peripheral timing and full delivery qualification
remain open. This734-member receipt includes a time-bounded process observation;
its source/result state does not imply the new native fit has passed.

Verify:
`python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-response-overlap/parallel-lookup-stage`.
