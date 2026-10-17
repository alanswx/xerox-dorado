# BaseBoard timer cadence mismatch (open)

BaseBd01.sil in the actual Rev Am wire-list source says Xtl=10Mhz.
The MC12061 model instead divides the fabric clock by8, independent of
SYSPER. An isolated nominal BaseBoard probe (SYSPER4; timer-enable latches
held in their enabled configuration) measures MCPreClk80 samples and
Pendulum/Refresh1280 samples. Primary wiring is /5 Max, /2 MCPreClk,
/2 Reference and /8 Pendulum. A10MHz crystal should produce1MHz MCPreClk
and16us Pendulum/Refresh, approximately133.333/2133.333 samples at SYSPER4.
The current native timer therefore runs about1.667x fast relative to the
modeled Dorado. The component fixture already uses16us Pendulum/Refresh.
This is an independent fidelity bug, not yet the cause of the queue loop:
the correctly paced original-storage diagnostic also reaches that loop.
No oscillator production change or new FPGA qualification is claimed.

A separate scratch phase-accumulator candidate now passes the nominal BaseBoard
cadence probe atSYSPER4,8,16: 1MHz MCPreClk and16us Pendulum/Refresh relative to
guest time. Candidate oscillator adds3 against5*SYSPER per fabric sample,
i.e. one output edge per50ns. Probe checks individual and average periods;
Pendulum/Refresh are quantized to the board's clock phase (4/8/16 samples),
so the first overly-tight one-sample jitter assertion was corrected to that
measured/modelled quantization. Average period remains independently checked.
Scratch timer10mhz-candidate includes candidate cell, board and probe.
This is NOT in Native23 or production. Full firmware startup remains to be
qualified before adopting the crystal fix; use a bounded accumulator width
and generator-provided SYSPER when promoting it. Native23 isolates d13 only.
