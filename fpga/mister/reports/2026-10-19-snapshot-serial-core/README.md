# Nineteen-board serial integration: source/XML PASS, capacity rejected

The full fit is terminal and rejected: **49,250 / 41,910 ALMs**, 7,340 over;
4,954 LABs required versus 4,191 available. Fitter exit3 stopped the driver.
No STA, assembly or hardware programming followed. All nineteen board scopes
and all three transport entities remain in the failed-fit resource table.
The eight MSA adapters account for 13,804.5 estimated ALMs. Failed-fit
attribution is diagnostic, not legal placement or a prediction of another fit;
the summary's zero RAM-block count is not a physical RAM result. The earlier
45,228-ALM experiment lacked eight enabled MSA adapters, so coverage differs.

This is a capacity experiment, not a deployable snapshot core. Eleven qualified
main boards and all eight qualified MSA adapters are connected through the
twelve-slot transport. Five foreign-service acknowledgments and transport reset
are constrained virtual inputs; memory-drained is a virtual output. None of
the service acknowledgments is tied high. RBF generation is OFF, and the driver
does not invoke the assembler or deploy anything.

The independent source audit preserves all 2,761 original board pin connections
and parameter bindings. Removing only allowlisted administrative wiring recovers
the original backplane, wrapper, outer core and framework equations. It checks
the complete QSF policy, qualified source hashes, original real-clock/IO
constraints, scan-mode extension of the existing MSA hold proof, virtual-port
constraints, four-condition reporting and all nineteen crossing scopes.
Twenty-three damaged-source/constraint controls reject. The actual remote project
passes, including Quartus's complete relative source-alias set.

Fresh Mac XML uses the actual QSF's 319 unique HDL inputs and fifteen macros:
3,945 instances, 1,966,264 physical board bits, 4,594 private board bits, 69
private owner/router bits, and 132,484 unchanged wrapper bits with access pending.
Fourteen geometry/missing-state/parameter/exact-alias controls reject. No sequential
state is excluded. Other candidate categories remain review items. emu/UIO,
top-level MiSTer state, external raw SDRAM and ARM services are outside this XML.

All 413 frozen Native59 input receipts verified before remote staging; all 659
staged files match the local project after path rebasing. Quartus synthesis is
successful: 58,736 registers, 2,497,792 block-memory bits, 35 DSP blocks and seven
virtual pins. These are synthesis totals, not fitted area or timing qualification.

The terminal capacity driver was PID **2322395** on misterubuntu, project
`/home/alans/mister/Dorado_Serial61Capacity_20261019`; isolated inputs are
`/home/alans/mister/Dorado_Serial61_inputs_20261019`. At the archived synthesis
checkpoint, child **2325036** was actively fitting. Both process handles are now
absent; the driver records fitter exit3. Do not restart this experiment.
`terminal-fit/` retains the final reports and terminal-state record separately
from the earlier synthesis checkpoint. `capacity-review.json` independently
checks the error, device budget, nineteen board scopes and transport retention;
four deliberately damaged capacity reports reject.

The 1,546 raw archive members pass lossless readback. Extracted source/XML audits,
all 37 source/XML damaged controls, four capacity controls, actual Quartus aliases and the executed inventory
tool hash pass. Restaging from extracted inputs reproduces all 659 files after
path rebasing. The original executed inventory tool is retained in the input
packet; its later edit only adds expected-fields output, recorded separately.

Next investigate shared enable/scan-mux factoring in actual MSA frontends and FF
adapters without dropping any physical fields. Test original/reference behavior,
full state access/restore/abort/first-resume and matched component timing before
another combined fit. Isolated savings cannot establish a legal whole-core fit.

Remaining work includes a legal combined fit/timing audit, wrapper/device/ARM/server
state, coordinated drain/pause and disk generations, and FPGA save/continue/
restore. In particular, PLL-unlock is currently a reset proxy in this capacity
experiment: production must prevent it from clearing quarantine without a genuine
machine reseed. Accepted raw transactions must drain before frontend freeze,
physical SDRAM refresh must continue, and daemon pause/resume must preserve
connections and rebase timers. This image must not be deployed as it stands.

MiSTer14/Native59 is unchanged. Last qualified rate remains 1,947,654.12 issues/s;
the complete nineteen-board combined fit is rejected at 49,250 / 41,910 ALMs.
Original 60 ns clock/timer work and final boot,
persistence, pinned-performance and sustained-operation gates remain required.
