# Native25 RTL, corrected synthetic file-label hints

Same qualified RBF3a8eee40...; no RTL change. Fresh offline pack
f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51.
Forty-five labels carry their real logical header-address hints; all file
data and other label fields are unchanged. See native25/media-hints-review.json.
Original firmware emitted BFN113 at22:27 UTC. Cedar login is visible at22:28;
Guest plus two Returns was accepted through the FPGA keyboard path.
At22:29 actual STP lookup/enumerate for Guest/6.1/Basic.Loadees reaches the
server (expected missing), then the client looks up Cyan for the release
path. The old immediate FP error is cleared. Successful Basic.Loadees
retrieval, Viewers, CommandTool and mouse remain under observation.

At22:31 real STP_TRANSFER confirms Basic.Loadees, RPCRuntime.bcd and
UserProfileImpl.bcd. Offline label-hint correction clears the real hardware
FP failure. One short TX packet was dropped by the single-packet cable
adapter and BSP recovered. All34 loadees/desktop/input remain open.
