# Native25 receiver handshake candidate

DskEth k04 B now captures Input setup before clk1; A/RxBOP is unchanged.
Original canonical RTL fails the retiring-Input regression; the candidate
passes retiring/newly-enabled Input cases. A complete cable/FIFO/bus-register
sequence retains every word, CRC3b50 and good EOP status. Existing good/damaged
CRC gates pass all8 phases. See component-review.json and tracked benches.

The FPGA DDR observation in Native24 shows a likely duplicated first receive
word. This correction is a plausible cause, not a confirmed Cedar boot fix.
Fresh120M cold DDR simulation and separate Quartus build are active. Timing
review and hardware verification remain pending; no Native25 image deployed.
Native25 retains2MB/16K geometry and the corrected passive monitor from24b.
