# Native57 desktop and actual input PASS

Same live Main25287/server25286/RBF064e3784, unchanged50MHz/SYSPER4.
Full Cedar Viewers/Ready, mouse Cmd click, initialized second CommandTool and
separate focus, exact short and alphabet/digit keyboard echoes pass in captures.
Normal echo-to-file followed by type returns rtl57persist from
///rtl57persist.txt. Input uses the real virtual USB keyboard/mouse; no guest
CPU/RAM/IOCB or OS file injection. This is warm readback, not cold persistence.

Read-only log collection streams fixed-size prefixes to SD and parses one disk
record at a time. All complete disk records have no recorded error/trace loss;
counts and exact prefix hashes are in desktop-collection.json. The first
collector spooled to almost-full tmpfs and failed ENOSPC; its source/log are
preserved. The accepted collector spools on SD, where14GB is free, then removes
its temporary spool. No active pack copy or host barrier occurs here. Main and
server retain the same identities. No guest Boot/clean shutdown/full snapshot,
original60ns cadence, speed gain or final stability qualification is claimed.
