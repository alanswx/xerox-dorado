# BaseBoard clock/reset/TTL state: nine primitive families and all279bits

Accepted simulation v4, fresh primitive/actual-board classification and
matched50MHz routed fits PASS. No production BaseBoard or live FPGA changed;
this preserves the current clock/timer models and does not calibrate them.

Nine new families cover70representative/87actual bits: SN74LS259 q8;
SN74LS163 q4+clock1; SN74LS175 q2+clock1; SN74LS74 two q/two clock bits;
MC1690 q/clock; MC12061 osc_div3; MC14521B cnt24+clock1; MPQ3303 vco2+por16;
and CLOCKGEN phase2 at the actual nominal N4 binding. Local18bit words cover
all fields; the25bit counter spans words0/1 with7valid bits in word1. Other
rows return0 and cannot write. Caller supplies hold/advance and bounded write
selection; there is no primitive board/global handshake. CLOCKGEN rejects
unqualified N values instead of silently serializing the wrong width.

The original processes and continuous pin logic are retained. Only clocked
nonblocking processes move under the host advance/hold/write selection, with
source hashes pinned. The actual Native59 clock-generator bytes have older
comments than the workspace copy; Verilog tokens are identical and tests/fits
use actual archived bytes. Every original family matches Native59 compiled
input hashes. MC10176/MC10136/SN174 adapters for the other192actual bits are
reused byte-identically; their hashes match Native59 compiled inputs too.

Accepted v4 compares physical state and every public output with untouched
original cells and an independent model.170049host/477held/184writes pass:
all70state bits/all local indices/all65536input pin patterns, random backend
holds, partial-word writes/padding, high clock history with inactive resets,
invalid writes using complementary data, dense capture, candidate-only
destructive continuation/restore/first real edge and20000more cycles.
Directed counter checks hit24bit overflow, POR65535->0 and saturation at4096.
Eleven damaged RTL variants and nine changed-source guards reject.

Earlier fixtures are retained rejected. V1/v2 assigned C++ values beyond an
18bit Verilator input; the oracle correctly masked them but the unphysical
high port bits contaminated simulator state. V4's access helper masks the
port at its source. V3's candidate passed but its invalid-alias mutation also
passed because all invalid writes matched the current value. V4 writes its
complement at every invalid row and rejects the mutation. These corrections
changed the fixture only. The original candidate generator/RTL is unchanged.

Fresh standalone original-cell XML maps every state/constant/derived field;
78damaged schemas reject. Primitive classification v1 is rejected for missing
the source-declared MPQ3303 osc combinational alias; v2 accounts for it.
Fresh actual nominal BaseBoard SYSPER4/CLOCKGEN N4 XML maps all279bits across
30packages, including SN174 PRECAP0/1 and partial counter words. Every other
board candidate remains in the inventory outside this partial classification.
13damaged actual-board schemas reject, including the wrong module parameter
specialization. Actual classification v1 passed;v2adds that specialization
check. No global or complete-board restore is inferred from these maps.

Matched50MHz staged-input/positive-edge launch/capture fits preserve20ns
embedded paths and2nsmax/0nsmin virtual-I/O budgets, no cuts. Both variants
pass all four conditions/minimum candidate slack+0.145ns, no illegal or
unconstrained clocks/I/O. Synthesis retains exactly70cell state registers
across all nine candidate families. Actual CLOCKGEN N4 is checked in both
map reports. Fitter shell totals277ALM/150regs ->314ALM/302regs, zeroRAM;
these include admin/shell/pruning and are not whole-core enabled costs.
Eight damaged-fit controls reject. All61compiled input hashes, accepted
immutable simulator inputs and an independent audit pass again after archive
extraction. Raw sources/reports/logs/XML and largeJSON are gzip stored unchanged.

Next: integrate all qualified CPU/RIOT/SRAM/EPROM/clock-reset families into
the actual BaseBoard, qualify complete classification/save/restore/timing,
then full enabled core budget and coherent backend/input/ARM/server/matching-
disk barriers, atomic bundle and physical Cedar save/restore. Original60ns
clock/timer migration and the final combined release gates remain open.
