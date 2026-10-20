# Final R address and selection component

`verilog/rtl/dorado_r_address_step.sv` extends the qualified 596-bit processor
component to 636 bits. It is an isolated migration component, without native
clock integration or functional RM/STK storage. Its independent reference is
extracted from 193 original packages and 1,991 connected pins. The generator
rejects a consumed mixed net when any of its original drivers is missing.

The added registers are ProcH l05 and ProcL g10, h10, i15, k11, g08, j11, l11
and h15. The original k08/k09 address muxes, g09 bank selects, l10 bypass and
their consumed control/mux cones are included. In particular, h12, h13 and i11
all drive the low address data; i19 supplies a fourth contribution to bit 3.
An inactive MC10159 contributes zero to that wired OR.

The physical clock distinction matters: DblClock'B is buffered ppClk, whereas
Clock0'D also carries StartA qualification. g10/h10 and the stack-address
registers can capture when the ordinary Clock0'D write-address stage cannot.
h15 is LOW-transparent. The source task/phase epoch is retained for coincident
captures, rather than sampling the final task selector after the clock event.

The prior free RbBypass inputs are replaced by the actual ProcL l10 output,
forwarded to both processor halves. Adding h17 also internalizes the Md parity
fault input of ProcL g18. Front RBase/stack data producers, complete address
comparison results, RSTK, PrBlock, Q14, IFU and inherited Ready/MIR/Hold producers
remain explicit boundaries. RAM data and parity remain external in this component.

## State and output map

| State bits | Original register |
| --- | --- |
| 595:0 | Existing coupled processor/task/T/R/parity component |
| 601:596 | ProcH l05 |
| 603:602 / 605:604 | ProcL g10 / h10 |
| 611:606 | ProcL i15 |
| 617:612 | ProcL k11 |
| 623:618 / 629:624 | ProcL g08 / j11 |
| 631:630 | ProcL l11 |
| 635:632 | ProcL h15 |

`address[7:0]`, `[15:8]`, `[23:16]`, `[31:24]` are respectively the high RM,
high STK, low RM and low STK physical address bytes, with physical bit 0 at
the most significant end of each byte. `select_n[0..3]` follows the same bank
order. Both read and write phase address polarities are checked against original
package outputs; these bytes are not logical Dorado register numbers.

## Additional input boundaries

| Pins | Original signals, first named signal at lowest pin |
| --- | --- |
| 152..155 | ProcH22.sil+3..6, resolved l05 D inputs |
| 156..159 | ProcL23.sil+3,+6,+7,+9, h12 front mux inputs |
| 160..163 | ProcL23.sil+11..14, resolved i15 D inputs |
| 164..171 | ProcL25.sil+5,+1,+2,+4,+3,+14,+12,+13, stack-address D inputs |
| 172..175 | RSTK.0..3 |
| 176..179 | PrBlock', RmBypass', StkBypass', Q.14 |
| 180..185 | Other physical inputs of selected packages: PropCnt', ProcH17.sil+3, CkMdParity', Md.Perr', ProcL24.sil+3, RBaseWriteEn' |
| 186..187 | StkPSaveEn', IfuRBaseSel' |
| 188..191 | RBase.0'..3' |
| 192..195 | CntMux.12'..15' |
| 196..201 | SelRBaseWadr', RBaseBypass', SubTask.0, SubTask.1, ProcL22.sil+4, BumpRBase |

RBaseBypass' remains a complete resolved external net: its original two-driver
cone must be included together in a later extension. Selecting just i10 would
silently lose the other contribution. FFeqMul' and FF4'..7' are decoded from the
existing FF field; FF4' corresponds to the highest numbered source pin bit.

## Functional memory handoff

`dorado_r_memory_step.sv` connects these stages to the four functional RM/STK
banks. During a g16 capture, each half's physical port retains its pre-event
address and select. Other events use the resolved address. Writes are closed
during that capture, so one address port per bank suffices; no RAM copies or
extra architectural state are needed. The CPU's address outputs still report
the resolved circuit address, separately from this sampled port address.

`dorado_r_port_preview.sv` evaluates the needed clock/address cone without
serially unrolling task arbitration into a RAM read. It reuses the qualified
64-bit processor/controller evaluator and captures SwitchUp' from the pre-event
task source once at task Clock0, with the inherited masked-high producer setup
contract. The complete task/address evaluator remains the CPU-state source.
Runtime agreement checks cover the resolved addresses, selects and both parity
capture events. The independent test compares the full original circuit.

The test observes all 9,216 physical SRAM bits after each resolved event, not
only the currently selected read port. It checks the original level-sensitive
write windows, actual j05 write gates, pre-event parity sampling, suppression of
a RAM commit when advance is clear, and unchanged sampled CPU state after RAM
commit. Initial storage contents are test-only fixtures; production adds no
RAM initialization or scan interface.

The coupled component requires four256×9 MLAB banks with unregistered read
addresses and outputs. This differs from the earlier external-address timing
fixture, where Quartus absorbed its launch flip-flop into the read-address port.
The timing reader checks these two profiles separately, including initialization,
collision mode and register settings. The original RAM read/write nonoverlap and
sampled-source checks remain prerequisites for `no_rw_check`.

## Remaining integration work

The coupled implementation now passes six primary arrival profiles, six directed
reset/stop/maintenance profiles in both simulators, and the isolated15ns budget
(setup+3.241ns/hold+0.136ns). Evidence and limits are in
[the coupled receipt](../fpga/mister/reports/2026-10-20-r-memory-path/README.md).
Include remaining front RBase/stack data, complete comparison, IFU/Ready/MIR/Hold
and board producers before native integration.
The address-only fixture deliberately probes transparent front inputs; its
evidence alone does not establish coupled RAM write correctness. Keep the old
g16 source epoch and write-address stability as explicit gates as the cone grows.

The simulation's generous settled intervals and illustrative arrival profiles
do not prove a complete 60 ns board cycle. The 15 ns registered-shell fit is an
implementation-budget measurement. Neither establishes native speed or Cedar
rollback. Those remain whole-core hardware acceptance gates.
