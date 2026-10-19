Historical preparation: COMPLETED after normal guest Boot/germ812 on19Oct.
Passing STOP/unmap and subsequent fresh normal reload/new epoch evidence:
../2026-10-19-native47-active-reload/. Main1509/server1506 are retired.
The original preparation below describes the preconditions, not current state.

Only after Native47 Viewers/input/disk checks, normal guest Boot
visibly reaches germ812, and its current Main/daemon hashes/PIDs are checked.
This calls the real Main network barrier at input_poll, verifies active epoch
before and stopped/unowned/disabled after, and preserves the exact response.
Afterwards require daemon1506 has no /dev/mem mapping and Main1509 stays
quiesced. Use normal load_core to a fresh-seed launcher for actual reload;
never turn a failed or timed-out barrier into permission to program.
No automatic RESUME: it resets server sessions, so do not run mid-guest boot.
