# Functional RM/STK ports coupled to the clock evaluator

PASS: the existing 596-bit control/task/T/R/sign/parity component now reads and
writes real functional RM/STK ports. The 9,216 RAM bits remain inside four
256×9 banks; the processor evaluator's explicit Q input/output remains596 bits. The independent
reference extracts all twelve original SRAM packages and both byte-parity
generators directly from the PARC wire lists. Together the references cover
9,812 original bits.

The separate port gate passes 12,928 checks per profile in both simulators,
across three illustrative arrival profiles. It covers every address in all
four banks, all sixteen chip-select masks, the original wired-OR outputs,
independent half writes, the native advance gate and 1,024 individual stored
parity-bit injections. Eight deliberately broken implementations reject.

The coupled gate passes 20,816 events per profile in both simulators across
six arrival profiles. All33 observed clock-edge channels agree with the original
graph. It checks5,822 R write-window observations and10,402 read-window
observations, all addresses/select masks/LC values,128 Hold cases and eight
directed Md/Pd source checks (both sources in every bank). It independently
observes the actual j05 write outputs, rejects read/write-window overlap, and
checks that committing RAM does not change the already sampled processor state.
Five broken coupled implementations reject.

The g16 data-arrival monitor records41,608 setup and1,726 hold observations per
profile. PARC min/max parity-arc controls enforce1.7ns setup/1.1ns hold; their
observed minima are317ns/3ns and309.1ns/10.9ns respectively. These generous
fixture setup intervals are not a native60ns proof or independent-skew analysis.

The isolated15ns fit passes all four operating conditions: minimum setup/evaluator
slack+4.190ns, hold+0.105ns and pulse width+6.113ns. Resources:1,819ALMs,
2,303registers,9,216MLAB bits, zero M10Ks/DSPs/PLLs. All four256×9 logical RAM
banks and their inferred parameter tables are checked, with no initialization.
The `no_rw_check` collision mode remains DONT_CARE; architectural read/write
nonoverlap and sampled-source preservation are required. No same-edge raw RAM
output guarantee is claimed.

All five recorded fit processes were absent before collection; every prepared
input matches its hash (only Quartus's version annotation is allowed). Fifteen
damaged/rehashed evidence controls reject, including missing/changed banks,
geometry, type, initialization, bit count, output registration and collision mode.
The preceding sign/odd timing profile still passes the extended reader.

1,442 selected raw source/report members are losslessly archived and read back.
Extracting them and repeating the independent positive timing review gives a
byte-identical result. A full simulator replay after extraction is not claimed.
History retains the earlier, narrower coupled run and the strengthened run;
the accepted run additionally uses complete-line timing readout. The first
prepared timing shell's missing output connections were corrected before any
Quartus build; the actual executed shell includes every output.

This remains a component experiment. Physical address/select producers are
external and change only in a closed high-root window. Their actual register/
mux handoff, bypass/IFU/Ready/MIR/Hold producers and native clock integration
remain open. No FPGA/Main/server change or speed gain resulted from this gate.

```
python3 tools/test_r_file_ports.py --output PORTS --mutations
python3 tools/test_r_storage.py --output COUPLED --mutations
python3 tools/fit_r_storage.py --prepare FIT
# On misterubuntu only: python3 fit_r_storage.py --run .
python3 tools/audit_control_clock_fit.py FIT --qualified COUPLED --output review.json
python3 tools/test_control_clock_fit_reader.py FIT --qualified COUPLED --output CONTROLS
```

`ports-qualified/` and `coupled-qualified/` freeze all executed inputs. `timing/`
contains the actual compiled fixture, terminal process/readback proof and raw
reports. `review-tools/` freezes the collector, independent reader, controls and
archive utility. MiSTer14 remains on the separately qualified MC1668 core15730f44.
