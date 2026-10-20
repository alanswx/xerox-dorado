# Eight complete MSAs share one real memory-pause boundary

One coordinator/router drives all eight original-compatible MSA adapters,
128-client interleaved/prefetch arbitration and actual 50MHz SDRAM. Independent
XSDS pin models provide memory, with original frozen MSA state/pin/traffic
oracles. No host code completes raw memory, guest instructions or IOCBs.

PASS: 15,403,567 checks /4,692,793 host edges /4,405 guest advances; 41,760 reads /
7,472 writes; four pauses with two accepted reads/two accepted writes; all688
held state words; 25,854 physical refresh commands; zero pin-model warnings/errors.
A parser/router reset during a partial member3 write leaves ALL eight consumers
held, despite caller hold release, without raw traffic or refresh starvation.
Early freeze, wrong-member routing and nonsticky hold variants compile/reject.

All125 source/report members read back losslessly. The extracted real circuit
rebuilds/runs, including the partial-import quarantine, and repeats the exact
counts without working HDL. Earlier long generated-line compilation failure
and pre-quarantine positive attempt remain preserved. Objects are excluded.

This is the eight-board memory circuit, not full native CPU/device/ARM state,
physical fit/timing, external RAM snapshot bytes or a complete FPGA restore.
