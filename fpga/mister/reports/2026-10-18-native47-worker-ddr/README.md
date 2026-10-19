# Native47: physical DDR progress during actual Main worker delay

Main1509/21e67a3f, daemon1506/c5996249, Native47/3df4a799 on MiSTer14.
Original firmware reached Cedar login; real Guest input started the loader.
The exact previously-qualified ARM probe55bc9a8c is loaded at input_poll.
Main's worker source/ABI is byte-identical to its prior qualified build;
its audited entry moved to0x992c9. The job owns a duplicate of the mounted
image descriptor, sleeps4200ms in the actual worker, then real fsync succeeds.
It never writes image bytes, accesses SPI or changes guest instructions.
This is delayed flush WORK, not a claimed4.2s kernel syscall.

PASS: interval1915296794925..1919496928940ns, duration4.200137685s:
10 disk read ACKs,100 write ACKs,4 normal poll records and no overlapping
poll gap above20ms. No trace errors/drops. Main samples wholly inside the
window advance3,904,318 issues over150,001,118 host edges (about3s); their
CRC/RX/abort/pack flags remain clear and transmit drops stay15.

Six complete guest packets are conservatively bounded inside the worker
interval. A real Watch.bcd retrieve queues168 metadata bytes and EOC; the
guest ACK names that exact response endpoint00262dab inside the same window.
This proves both directions, rather than merely host enqueue progress.
It does not claim a1478-byte file-data transfer or typed command completed
during the window. No keyboard/mouse events were injected in this case.

The observer timestamps precede regular-file reads; the qualifier uses the
NEXT recorded iteration as a conservative completion bound, and reconstructs
complete lines before selecting those strictly inside the worker interval.
The capture finished before end, independently checked on ARM. The disk
prefix includes over3s of later coverage and no error/dropped records.
Setup GDB pause precedes the interval; two fresh Main traces are discarded
before arming. Three known Main GDB controls total on this run now.

Keep the DSO resident until Main exits; no callback can be unloaded while
owned by the worker. Full desktop/input, active STOP/reload, cold persistence
and sustained stress remain separate gates. Reproduction is deliberately
one-shot with exact PID/binary/FD and initially absent gate/event paths.
