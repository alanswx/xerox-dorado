# Original final R address/select stages

PASS for an isolated636-bit processor component:193 original packages,
1,991 connected pins. The added40 bits cover ProcH l05 and ProcL g10/h10,
i15, k11/g08/j11, l11 and h15. Both address mux banks, stack/RM selects and
the shared R bypass are derived from the original circuit. Consumed mixed-net
drivers are checked exhaustively during extraction, including h12/h13/i11 and
i19's fourth low-address-bit contribution.

DblClock'B is ppClk without the StartA qualification of Clock0'D. The candidate
retains that distinction, LOW-transparent h15 and the original task/phase source
epoch. Newly internal h17 also supplies the Md parity input to g18.

The accepted simulation passes16,448 events/profile in Icarus and Verilator,
across five arrival profiles, with all636 state bits and all45 clock channels
compared. Each physical port sees all256 addresses; both legal bank selections,
all LC values and all four mixed-net drivers are exercised. Observed changes:
5,997 addresses,1,120 selects,565 bypasses and1,672 transparent updates. Seven
broken variants reject, including omitted mux/OR drivers and wrong clock,
address phase, latch transparency and bank select.

g16 monitoring records8,240 setup and11,851 hold observations per profile. PARC
correlated parity min/max controls enforce1.7ns/1.1ns limits; observed minima
are604ns setup and3ns/10.9ns hold. These generous fixture intervals and correlated
profiles are not an independent-skew or60ns board qualification.

The isolated15ns shell passes all four operating conditions. Minimum slacks:
setup/evaluator+3.982ns, hold+0.155ns, pulse width+6.547ns. Resources:
1,485ALMs/2,487registers, zero block RAM/DSP/PLL;1,042 launch/791 result bits.
Prepared input hashes and all five absent process IDs are checked. Eight damaged/
rehashed reader controls reject. The previous functional-R-storage profile still
passes the extended reader.

881 raw source/report members are losslessly archived and read back. Extraction
and the archived positive timing reader reproduce a byte-identical review. A
second full simulator replay after extraction is not claimed. Compiler objects,
executables/databases and FPGA images are excluded.

Functional RM/STK storage is NOT attached to this new address graph. RAM data/
parity and front producers retain explicit boundaries. Next: couple real RAM
ports while preserving the old g16 sample epoch and original write-enable/
address/select ordering. See `contract.md.gz` for the full map and limits.
No native FPGA/Main/server change or hardware speed gain resulted from this gate.

```
python3 tools/test_r_address.py --output QUALIFIED --mutations
python3 tools/fit_r_address.py --prepare FIT
# On misterubuntu only: python3 fit_r_address.py --run .
python3 tools/audit_control_clock_fit.py FIT --qualified QUALIFIED --output review.json
python3 tools/test_control_clock_fit_reader.py FIT --qualified QUALIFIED --output CONTROLS
```

Remote `/home/alans/mister/r-address-fit-20261020` is terminal. Builder4107985,
map4107986,fit4108290,sta4109537,corners4109696 are absent. Do not restart it.
MiSTer14 remains on the separately qualified MC1668 core15730f44; last measured
login issue rate1,947,686.01/s. Goal remains active, full FPGA snapshots deferred.
