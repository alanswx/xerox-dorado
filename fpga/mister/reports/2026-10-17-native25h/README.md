# Native25 RTL, corrected synthetic file-label hints

Same qualified RBF3a8eee40...; no RTL change. Fresh offline pack
f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51.
Forty-five labels carry logical header-address hints; every data byte and
other label field is unchanged. See native25/media-hints-review.json.

Original firmware/BFN113 completed22:27 UTC; real Cedar login and Guest
input at22:28. The earlier File.FP error is cleared. The real CPU fetches
Basic.Loadees and begins system loading. At the later disk fault, its
cached Basic.Loadees (2361B) and RPCRuntime.bcd (18432B) match the served
sources byte-for-byte. UserProfile allocation exists but its transfer is
incomplete: only its first1024 bytes match. Server STP_TRANSFER starts a
transfer; it does not alone prove all bytes arrived.

At22:31–22:33 the medium asserts pack_fault (status47fc), pack_ready=0 and
machine_enable=0, freezing CPU233. Serial versus host fault is unresolved.
The stopped writable pack has91 changed sectors; see disk-at-fault-review.json.
Full changed sectors/raw DDR/logs remain in build/rtl-executive/cedar25h-launch
and on MiSTer14 in Cedar25h_20261017. No guest state was repaired.

One short TX was dropped by the single-packet cable buffer and BSP recovered.
Native26 is a separate8MB/64K control; Native27 adds passive fault context.
Viewers/CommandTool/Echo/mouse remain open.
