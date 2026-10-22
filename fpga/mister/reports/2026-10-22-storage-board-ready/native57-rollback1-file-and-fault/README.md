# Explicit native Rollback: file survives; AIS replay faults

Same Main23119/server23118/RBF7c4d39e4 and original checkpoint, no reset or new Checkpoint. Original germ812 runs and the Viewers desktop returns. A fresh fourth CommandTool reads ///rtl57afterck0528.txt, created/closed AFTER the original checkpoint, and prints exact aftercheckpoint0528 plus a prompt. This qualifies post-checkpoint disk file preservation through the explicit native Rollback.

The saved Schematic command replays but AISViewer.DisplayAIS reports UnknownError[sig:1B,msg:177777B]. Opening the absent AIS viewer then raises VMOpsImpl.AddressFault at27B. Normal WalkStack shows ViewerOpsImplA.inner (lf77750B,pc5427B). Native/HDMI proof retained. Normal debugger Abort terminates only that failing command and its CommandTool reaches a prompt. The original checkpoint overlapped the user's Schematic action. Cause is unproven; repeated application survival and a quiet checkpoint remain unqualified. Later isolated viewer-creation screenshots are diagnostic prefixes only.

111266 complete disk-prefix records contain no error or dropped event; monotonic network diagnostics show CRC/RX/abort0, TX dropped2. This is a live text prefix, not a drained disk barrier or full machine snapshot. A collector completed the gzip prefixes but failed on unsupported BusyBox ps -p; a separate streaming finalizer validates those same files and /proc binary identities without recopying the live logs or pack.

The changed-sector cache candidate is NOT deployed and cannot explain this fault. Continue a quiet baseline separately; do not erase or reinterpret this failure as a full repeated-Rollback pass.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-board-ready/native57-rollback1-file-and-fault`
