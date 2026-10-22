# Original Cedar guest diagnostics: preserved failure, setting restored

Same Main23119/server23118/RBF7c4d39e4, no Main/FPGA/daemon reload. Run AISViewerImpl reports previously loaded AND run. Cedar's Interpreter works: module-global readbackFALSE, corrected original-arrow assignmentTRUE and separate TRUE readback, then depth1. Shift-minus entered a dash here; Linux KEY_GRAVE41/PS2 0E is the original arrow key. Initial v1 helper source is reconstructed and labeled, not claimed frozen before execution; the later current helper and actual typed/native proofs are retained.

Typed-frame GetProcesses fails TypeFault. A separately resolved non-NIL most-recent AISViewerImpl context yields NIL. InterpreterCommandsImpl context yields evaluator-only list&12. Normal AMProcess.CallDebugger on its first process nests an event for PROCESS370B at EvalCommand lf77554B/pc546B; reading line confirms the Schematic DisplayAIS command. This filter selects a caller frame, not a proven actual top frame.

The agent did NOT check AMProcess.GetState.topFrame before Proceed on that filtered event. Proceed raises a separate NIL AddressFault in caller PROCESS367B; normal Thaw[&12] executes and returns a prompt; two Abort attempts each raise nested30B faults. These generated failures may reflect unsupported probe use. They are NOT reliable independent evidence of a normal Cedar debugger or RTL regression, and do not identify the original AIS failure. No further event controls are attempted.

BackStopImpl.alwaysRejectAny is restoredFALSE and separately readsFALSE, with actual native proof. The additional diagnostics are ended by ordinary Boot from the independent first CommandTool, whose germ812 start has its own receipt. No new checkpoint contains these diagnostic settings/state. Repeated/quiet/cold checkpoint application recovery remains open.

Streaming regular-log prefix validates115587 complete disk records with no error or dropped event; CRC/RX/abort0, TX dropped3. It is not a drained durability barrier, live-pack copy, machine snapshot, clean speed/stress run or root cause. Previous file-survival and original replay-fault receipts remain authoritative; this incremental receipt preserves new/changed diagnostics and exact log/source bytes only.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-board-ready/native57-rollback1-guest-debug`
