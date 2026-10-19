# Shared-RIOT full core: measured reduction, still over capacity

The revised all-eleven-board build is terminal. Synthesis passed; fitting
failed with 5,097 LABs needed versus 4,191 available. The failed-fit summary
reports **50,414 ALMs versus 41,910**, an excess of 8,504. Relative to the
first Enabled60 experiment's 54,403 ALMs, the shared RIOT RAM ports save
**3,989 ALMs in the complete design**. No state bit was removed.

Failed-fit attribution places the five RIOTs at 3,390.6 ALMs, versus 7,341.5
previously. These estimates diagnose a rejected design; they are not a legal
placement or a prediction that other isolated savings will add linearly.
The source audit still passes all 413 baseline receipt files, original board
wiring, unchanged clocks/memory/ECC constraints and fifteen broken controls.
The builder stopped at fitter exit3. No STA, assembler, RBF deployment or
physical snapshot followed. Live Native59 and its measured speed are unchanged.

Nine raw evidence files pass lossless hash readback. Raw reports/logs are gzip
stored unchanged. The exact executed capacity auditor is recovered from commit
9051e99a and checked against the auditor hash in capacity-review.json. The
baseline full fit remains in ../2026-10-19-native59-dsketh. Next reductions
are qualified separately in ../2026-10-19-snapshot-f10145-shared-write.
