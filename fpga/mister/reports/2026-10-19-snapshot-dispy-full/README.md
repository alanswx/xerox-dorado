# Actual native field-display complete modeled state qualification

Complete access covers 13,993 mutable bits: 140 FF packages (1,439 bits),
nine 256x4 MB7071H files, three 16x4 files plus write windows, three 1024x1
F10415 arrays plus private raw read pipelines, the signed 33-bit oscillator
state, and eight board phase bits. Two immutable PROMs (2,048 bits) are pinned.
The 19-bit state index has eight package bits and eleven row bits; invalid
rows/packages reject without mutation. Original omitted F10415 CE inputs
remain omitted. No C-state import or guest instruction completion is used.

Accepted actual-default simulation passes 1,201,722 host edges, 1,126,659 held
edges, 1,101,701 administrative requests, all 13,993 modeled bits and all
524,288 indices. It checks untouched original outputs/state and independent
RAM arrays while exercising every normal RAM writer. Physical probes cover
all state, transport padding, destructive writes, whole-state restoration,
quiet release/recapture, first resumed raw pipelines and 20,000 later cycles.
Eight broken RTL variants reject. Fresh original XML classifies every state
bit and both PROMs; 14 damaged schemas/inventories reject. 318 simulation
only diagnostic bits are explicitly excluded under checked source guards.

The actual backplane omits the board parameter: both board and oscillator
use default SYSPER16, with fabric configuration SYSPER4. The board parameter
is currently unused, verified under the pinned whole source. Accepted
simulation, XML and fits use the actual defaults and native memory macros.
Earlier review-fixture4 tests/fits and a classification without the memory
placement macro are retained separately. The generated board RTL is unchanged
by that fixture correction. The first generation guard exposed the omitted
CE pins; the adapter preserves that omission. Earlier test harness fixes
handle scalar Verilator output buses and clock the untouched reference with
the same actual enable; no production source changed.

Matched isolated 50 MHz fits pass all four operating conditions, minimum
snapshot slack +0.121 ns, no illegal/unconstrained clocks or I/O, no cuts.
Physical audit preserves all three 1024x1 arrays in M10K, and retains all nine
256x4 files in MLAB for enabled snapshot access. The baseline prunes three
unused bits of u_f04 (256x1) and packs the three F10415 planes into one M10K.
The administrative ports retain u_f04's full width and require three
independent M10Ks. Counts include this shell/admin/pruning difference:
1,159 -> 2,966 ALMs, 1,358 -> 2,434 registers, one -> three M10Ks.
These are not enabled whole-core costs.

The unchanged original F10415 source permits DONT_CARE read-during-write via
no_rw_check; its physical baseline reports that mode. The snapshot banks
require OLD_DATA, matching the modeled nonblocking assignment behavior and
independent collision tests. Both actual parameter metadata and physical rows
are checked. Eight corrupt source/macro/constraint/memory/placement/PROM/timing
controls reject. Full-core Cedar qualification is required after integration.

Raw reports/logs/sources are gzip-compressed unchanged. Native58 builds this
adapter into frozen Native57 with all administrative inputs inactive; no FPGA
programming follows from these isolated results. Remaining devices, wrappers,
ARM state, coherent machine bundle/restore, original60ns/timers and final
release qualification remain open. No complete snapshot or speed gain claimed.
