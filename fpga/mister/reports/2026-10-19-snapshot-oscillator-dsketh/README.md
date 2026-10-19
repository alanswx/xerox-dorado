# Oscillator simulation includes actual native DskEth binding

The existing nine oscillator parameter pairs now include a tenth: FREQ23530,
SYSPER8, the actual inherited DskEth binding. Original/independent signed-wrap
oracle, all33bits/indices, destructive state restoration, exact comparison and
signed boundaries, quiet release/recapture/first edge/20,000more samples pass.
All eight broken RTL and two changed-original-source guards still reject.
Only the test fixture/oracle changed; original/generator/qualified helper are
unchanged. Exact old nine-mode qualification remains in snapshot-oscillator.

This extends simulation coverage. It does not calibrate the current oscillator
to real time, refit the extra parameter pair or qualify complete DskEth/global
snapshots. The full board fit remains required; clock correction remains in
the subsequent clock milestone. Raw sources/logs gzip-compressed unchanged.
