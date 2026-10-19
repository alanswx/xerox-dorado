# Width-correct F10415 snapshot RAM primitive

Widths1/2/4/5/18 preserve old-data and raw read-pipeline state, with18-bit
transport padding cleared and invalid indices1025..2047 rejected.2-state
original/oracle tests cover connected CE plus unchanged omitted CE in both
interfaces:93575host edges/7341holds/61498requests/61500state-bit checks,
backend pauses, destructive continuation/restore/recapture and first guest
edge with20000subsequent cycles. Eight broken RTL variants fail. Fresh XML
classifies1025modeled bits per original one-bit cell;11bad schemas reject.
No explicit CE tie-off or four-state/X equivalence is claimed.

Matched independent five-bank50MHz opposite-edge shell fits pass all four
conditions,min+0.171ns, no cuts/unconstrained paths.30720RAMbits/sixM10Ks
retained; all five real macro tables show OLD_DATA.267->388ALMs and120->342
registers include shell/admin/pruning. Seven corrupted-fit controls reject.
The first audit searched user-entity tables instead of inferred-entity
tables and failed closed. Corrected parser audited the same completed
RTL/fit/constraints; both executed scripts and raw logs are preserved.

Primitive qualification only: no complete IFU/global barrier, machine
snapshot, enabled full-core resource budget or speed claim.
