# Snapshot RAM access qualification

This is milestone4a groundwork, not a complete FPGA snapshot implementation.
The running Native47 core and all production cells are unchanged.

The isolated1024x34 bank saves every RAM bit and its34-bit registered read
output. The reference consists of34 unchanged `cell_F10415A` instances on
an independently stopped clock, plus a separate array oracle. All34850 bits,
all1023 invalid indices, destructive continuation, first-edge resume and
30000 random running steps pass in37194 host edges/5123 state accesses.
Four deliberately broken RTL variants fail with their expected diagnostic.

Whole-word transfer through one shared write port retains RAM packing.
Matched Quartus17.0.2 fits on5CSEBA6U23I7 use50MHz, virtual data I/O and
2ns I/O budgets:

| Resource | Baseline | Snapshot | Added |
| --- | ---: | ---: | ---: |
| ALMs | 101 | 163 | 62 |
| Registers | 0 | 43 | 43 |
| RAM bits | 34816 | 34816 | 0 |
| M10Ks | 4 | 4 | 0 |

All12 setup/hold/minimum-pulse checks per variant pass across four device
operating conditions; snapshot minimum slack is+0.096ns. Recovery/removal
have no applicable asynchronous paths. All unconstrained-path categories
are zero. All34 snapshot RAM mixed-port modes are OLD_DATA. Full-board/core
timing, physical capture/resume and resource scaling are not qualified here.

`fit-qualification.json` records the matched result. Five damaged report/source
controls reject changed packing, missing corner, negative timing, wrong
collision mode and changed fitted source. `rtl-results.json` and per-case
logs record the independent simulation and four RTL controls.

`quartus-evidence.tar.gz` contains frozen sources/projects/reports for three
experiments. `fits-v4` used separate conditional write addresses and lost all
RAM inference (48913 synthesized logic cells); its fitter was intentionally
terminated after that rejection, not timing-qualified. `fits-v5` unified the
write port but used per-bit restore enables:196 ALMs/56 registers/34M10Ks.
`fits-v6` is the final whole-word variant above. Earlier QSF/parser setup
errors remain in the local/Ubuntu work directories and do not qualify a fit.

Reproduce using `fpga/mister/snapshot/test_f10415.py --mutations`. Extract
the archive into a scratch directory and run `audit_f10415.py fits-v6` from
the matching source checkout. `fit_f10415.py --output NEW_DIRECTORY` builds
new isolated projects on misterubuntu; it never programs the FPGA.

The audit scripts include provenance and reject changed source, RAM packing
or missing timing evidence. Virtual-pin overhead is included in each matched
fit; do not extrapolate the ALM delta to the full machine. The prototype
requests OLD_DATA collision semantics whereas the original placement cell
uses `no_rw_check`. Actual-board grouping/strobes/history and the global
guest/device/SDRAM/ARM pause barrier still need implementation. Complete
bundle validation, paired disk generation, local Pup/BSP/STP state and an
actual Cedar desktop restore/time benchmark remain open.
