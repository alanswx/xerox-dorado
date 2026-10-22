# Native AIS isolation: independent command works; AIS Eval pending

After the preserved explicit-Rollback file pass and AIS/OpenIcon failure, normal debugger Abort returns the parent tool to its prompt. A fresh fourth CommandTool accepts Eval AISViewer.CreateAISViewer[] but has no observed result or new prompt in successive native captures. Normal STOP and a slower extra Return do not change that observation. This bare Eval includes result formatting; the exact blocked routine is unproven. Do not treat sent USB input alone as proof of execution.

A different original CommandTool, focused below its final prompt, executes echo alive57 with0.8-second USB pacing and prints exact alive57 plus a new prompt. Thus Cedar is not globally stalled. All injected keys/buttons are released. No new Checkpoint, reset, Main change or daemon restart occurs. A quiet checkpoint must wait for diagnosis/recovery of the pending application operation.

BackStopImpl.mesa shows that UnknownError[sig,msg] is the generic ANY-signal fallback calling RuntimeError.SendMsgSignal; it does not identify an AIS file or filesystem error. Cause remains unproven. The original120-frame read-only observer completes normally; its terminal state and observation index are preserved, while prior receipts retain the qualified return/file/fault frames.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-board-ready/native57-rollback1-ais-isolation`
