# Mapbuf clock input and storage strobes

Native18 retained map data but delayed MemX Clk1'Aa independently of
Clk1'Cb. Those clocks capture the same MemWE/MakeMemCAS inputs in c02/e23
for the two storage strobe banks (MemX sheets 7/20). The resulting germ
readback lost mostly the A-bank nibbles. The map-only regression did not
exercise that requirement.

The correction restores Aa to the ordinary one-stage distribution shared
with Cb. Three stages now delay preFH'A **before** c07 pin 5, while c07's
HoldMapBuf input and MapbufLd outputs have no added pipeline. The latch
keeps its clock setup interval, and a new hold request closes it without
another three-tick qualifier delay. Ab retains the MCR's three-stage setup.
Explicit whole-board depth experiments retain their previous model.

The expanded map replay compares Aa/Cb throughout both page/Link/hold/release
transactions. The corrected board passes; Native18 fails the clock
comparison. Original-ROM startup passes at SYSPER 4 and 16, with all fifteen
task initializations and all six error sources enabled without error.
The full Cedar run passes the first sixteen actual map RAM write-data values
against pages 0..15.

The germ comparison reads BOOTIO samples at Cedar CIA 5f1, task 0, retaining
the last non-held read for each source VA before the first CIA 7e0 handoff.
It compares those words with the untouched big-endian
`chm/cedar/germ-alt/Dorado.germ-6.1.6`. There are 8,930 distinct sampled words
of 8,960; this is not a whole-image checksum. Native18 has 265 differing
sampled words. The correction has two (02f0/02f1, still unexplained), and
the former broken roots 0200/0304/0305 now read 06ec/09d0/09e0. Its first
IFU acknowledgement is at fabric cycle 76,410,144, CIA 41e, VA 03e06ee.
Mesa execution continues past that point. No guest image, PC, parity enable,
or memory state was patched to obtain this result.

At 100 million cycles, the original 3,600 IM and 256 IFUM entries still
match; the IFU has 600,500 reference and 600,870 acknowledgement transitions.
All six error sources were enabled for 86,611,683 samples without error.
The slower-ratio startup gate has 10,422,953 such clean samples.

The isolated Native19 Quartus build uses the frozen prototype generator.
The canonical cleanup emits identical RTL tokens; their different hashes
and the source-overlay hash are recorded in `candidate.json`. Hardware
timing and the actual FPGA OS screen require separate qualification.
