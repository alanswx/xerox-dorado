# Combined RIOT/register-file core: smaller, still over capacity

Synthesis passes; the combined eleven-board design fails fitting at
**48,353 ALMs versus 41,910**, an excess of 6,443. Error170012 requires
4,899 LABs versus 4,191 available. The builder is terminal at fitter exit3.
No STA, assembler, RBF load or physical snapshot follows.

This is 2,061 ALMs below the RIOT-only revision's 50,414 and 6,050 below
the first all-enabled experiment's 54,403. All 1,944,744 modeled board bits
remain accessible. The source audit verifies every original board connection,
all 413 baseline receipt files, unchanged clocks/memory/ECC constraints and
seventeen damaged integrations, including incorrect or duplicate replacement
source assignments. Quartus confirms that the new register-file helper was
compiled for all 85 instances. Its total synthesis ALUT attribution falls
from 12,179 to 7,411; mapped RAM/register state remains intact.

Nine raw files pass lossless SHA256 readback, including the exact capacity
auditor whose bytes match capacity-review.json. Failed-fit attributions are
diagnostics, not a legal placed design or proof that separate savings add
linearly. Baseline timing/source evidence remains in the adjacent Native59
report; component/full-board qualification is in snapshot-f10145-shared-write.

The remaining gap motivates a private zero-write-reply experiment. Public
fabric writes already return zero; no guest state or read result needs to
change. Its original/model/restore unit gate passes, including explicit zero
write replies and a broken-reply control. Strict MLAB inference and timing
are a separate active experiment, not established by this report. Larger
reductions in the parallel state-access path may still be needed. Live
Native59, original clocks and last qualified speed remain unchanged.
