# Actual BaseBoard Intel 2125 SRAM state

Simulation, original wire-list grouping, fresh XML classification and matched
50MHz routed fits PASS. This is an isolated primitive qualification; no
production BaseBoard or live MiSTer RTL changed.

The actual 32 packages form four1024x8banks. Each group has the original
RA9..RA0 address pins, RS0'..RS3' select and WE'AC/WE'BD write-enable pairing.
Word bit i is the named MCD_i wire, without assuming 6502 byte significance.
All32768RAMbits and32raw registered-read bits are saved. Local indices0..1023
address RAM,1024 the read pipeline;1025..2047 reject without writes. Padding
reads as zero. Output is disabled during either deselection or a write.
The existing f10415_word_bank.sv transport is reused byte-identically.

Simulation compares four candidates with32untouched original cells, an
independent byte-array/pipeline oracle, and physical candidate state on every
host edge.164821host/127087held/127084accepted/82000writes/127084late changes
pass: every state bit and8192combined indices, allCE/WE combinations, dense
RAM, noncurrent-address pipeline, padding, invalid writes, backend holds,
candidate-only destructive continuation, complete restore/first edge and
20000subsequent steps. Nine broken RTL variants reject.

Grouping checks original SIL pins, package membership, all address/control/
data/output wires and shared transport/cell hashes;11bad controls reject.
Fresh actual BaseBd nominal SYSPER4/CLOCKGEN N4 XML accounts for each RAM,
raw read, derived address/output and constant unused-power field;12bad schemas
reject, including nonaliased plane permutation. Every other board candidate
remains in the full inventory, outside this partial classification. Both
classification iterations are retained;v2adds independent wire-list schema
comparison. Original board/cell bytes match Native59's compiled-input hashes.

Matched negative-edge launch/capture fits have the same50MHz clock,10ns
embedded paths and2ns/0ns virtual-I/O budgets, no timing cuts. Both variants
pass all four conditions/minimum candidate slack+0.211ns, no illegal or
unconstrained clocks/I/O. Both retain32768bits/fourphysical M10Ksites and
old-data mixed-port collision mode. Baseline32one-bit rows share four sites;
candidate four8-bit rows occupy four sites. Physical row counts must not be
mistaken for M10K counts. Shell totals171ALM/112regs ->290ALM/330regs; these
include launch/capture/admin/pruning and are not whole-core enabled costs.
Ten damaged-fit controls reject. All24compiled input hashes and the independent
audit pass again after reconstructing exact bytes from the archive. Raw
sources/reports/logs/XML and JSONabove100KiB are gzip stored unchanged.

No global freeze barrier, full BaseBoard restore or physical machine snapshot
is implemented here. Next: EPROM read registers, remaining clock/reset/FF
state, complete board qualification, then enabled whole-core resource/timing
and coherent backend/input/ARM/server/matching-disk save/restore.
