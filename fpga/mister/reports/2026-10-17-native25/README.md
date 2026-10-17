# Native25 receiver handshake candidate

DskEth k04 B now captures Input setup before clk1; A/RxBOP is unchanged.
Original canonical RTL fails the retiring-Input regression; the candidate
passes retiring/newly-enabled Input cases. A complete cable/FIFO/bus-register
sequence retains every word, CRC3b50 and good EOP status. Existing good/damaged
CRC gates pass all8 phases. See component-review.json and tracked benches.

The FPGA DDR observation in Native24 shows a likely duplicated first receive
word. Native25 clears the hardware service wait and displays the real Cedar login.
Guest plus two Returns reaches the actual OS authentication path. Exact
receive-buffer bytes have not been coherently recaptured.
Fresh120M cold DDR gate PASSES:8931 germ words, no mismatch, original
Initial/Cedar images and IFU activity. Timing passes29 checks,min+0.071ns,
zero illegal/unconstrained clocks. Four exact same-cell c21 write-address
hold paths are reviewed; setup/read/data/enables remain timed.
Native25 is deployed on MiSTer14 with a fresh pack; original firmware/BFN113
completed. Actual login screenshots are included. Post-login status messages
need diagnosis; Viewers/CommandTool/Echo/mouse remain pending.
Native25 retains2MB/16K geometry and the corrected passive monitor from24b.
