# Native26 experimental memory geometry

Four64K-chip modules provide8MB physical RAM with64K Map entries.
Extra row/column address bits are wired from MSA MemAd.1 and MemX RTMapAd.1
through the explicit density straps; all21 Map planes select64K arrays.
Raw DDR grows to16MB. Native25 receiver setup correction is retained.

Parameter defaults preserve16K cells and byte-identical generated default
MemX/MSA boards. Independent cell, full MemX geometry and full MSA-vs-DDR
comparisons pass, including all9,437,184 stored bits. Default2,359,296-bit
MSA comparison passes too. Full generated wrapper elaborates.

Cold500M prototype finished: original Initial/Cedar image checks pass,
IFU8540094/8543534, all six errors enabled/clean486611635 samples. Source
sampling has8880 words with zero mismatch, below existing8900 minimum;
this gate fails coverage and must not be called a pass. See final reports.
The prototype predates Native25 receive-full correction and uses fixture
Pendulum/Refresh, so it does not qualify full BaseBoard startup.

Fit/review complete:29 checks,min+0.074ns, no illegal/unconstrained clocks;
exact same-cell write-address hold review included. RBF586dc7f8... is now
on MiSTer14 as the separate memory-geometry hardware control with a fresh
corrected-label packf3bce2a8.... Original BaseBoard/Initial firmware emitted
BFN113 at22:46 UTC and C released; actual OS routing requests follow.
Login/loader/disk-fault reproduction/desktop remain under observation.
