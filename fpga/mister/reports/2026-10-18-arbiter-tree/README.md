# Balanced storage request/payload selection

Motivation: Native29's actual critical path takes22.666ns through readiness,
round-robin selection and the address mux, limiting the fitted core to42.55MHz.

The new tree carries owner/address/data/write together. Each subtree selects
its first requester at or above next_group, otherwise its first requester.
An internal node prefers a left preferred request; otherwise a right
preferred request; otherwise the first nonempty child. Induction gives the
same root choice as the old cyclic scan. Existing FSM/grant/response/reset
cycles and one-outstanding-request contract are unchanged.

Independent cyclic-scan/payload/stall scoreboard:

| Groups | Masks | Completed requests | Request stalls | Response stalls |
| --- | --- | --- | --- | --- |
|1|4|3|6|9|
|3|48|60|150|180|
|7|1792|4032|10080|12096|
|128|256|8371|20926|25113|

All masks and pointers are exhaustive at1/3/7 groups;128 groups primes
every pointer through real transactions and uses deterministic random
masks. Addresses/data/write flags and response ownership are checked under
stalls, including changes to an accepted producer's original inputs.

Existing complete storage regressions pass: three-group array1505 cycles /
4449 DDR reads /463 writes including reset drain; whole MSA12012 cycles,
7872 writes,1111 output transitions and all2,359,296 bits identical; larger
geometry all9,437,184 bits identical. No ECC or read-before-write bypass.

The installed Quartus17 parser required explicit generate/genvar syntax;
the first Native31 map error is preserved remotely, corrected map passes.
Native31's50MHz fit/timing/hardware are still pending. Simulation success
does not establish a higher physical frequency. Native29 on hardware uses
the original arbiter and remains the independent40MHz control.
