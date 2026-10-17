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
