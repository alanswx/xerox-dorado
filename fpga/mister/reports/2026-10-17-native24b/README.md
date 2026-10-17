# Native24b corrected passive receive monitor

SHA256 `2cffe64309801e11fd000757bbfef04ed551b23efe408638ce5e472a1dc97e74`.
Deployed on MiSTer14 with a fresh verified pack. Original firmware boot/BFN113 completed; C released.
No functional change from Native24; CPU EOP tap corrected from parity17 to18.
Machine word bytes: FIFO EOP count, CPU EOP count, FIFO status, CPU status.
Counts wrap256. CPU task/PC and machine status remain separate.

29 timing checks pass; minimum+0.080ns, no illegal/unconstrained clocks.
Reviewed hold exceptions are only the three MemC cells' own write-address
bit1 (1 source/4 sinks each), whose capture/write enables are mutually exclusive.
Full before-review paths and exact reviewed SDC are included. Setup, read,
data, enables and cross-cell paths remain timed. External framework I/O
coverage is separate. Directed routing replies advance both EOP counters with status00, but
Cedar still retries. Early physical-DDR capture lacked packet headers; no
coherent Map/cache was captured. Screenshot has no Cedar login/desktop/input.
