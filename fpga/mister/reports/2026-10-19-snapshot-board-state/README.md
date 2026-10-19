# Eleven-board state transport and source integration

Board-only transport simulation and independent source-integration audit PASS.
The all-enabled full-core synthesis passes; fitting is still running. No RBF
was assembled or deployed, and no whole-machine snapshot is claimed.

The diagnostic UIO uses commands76..79 alongside unchanged Native59 Ethernet
commands70..75. Exact key/length/padding checks precede command commitment on
deselection. The fabric requires all eleven board frozen acknowledgments,
checks full index widths, routes one outstanding operation by registered
owner, and quarantines timeout/lost-boundary requests until explicit thaw.
Status is captured coherently on its command word; bit8 identifies board-only
scope. See ../../snapshot/README.md for the word format and error codes.

Accepted simulation v6:9,974host ticks,433packets,201checks. Eleven stub boards
exercise read/write routing, each full index-width boundary, all invalid board
IDs, high-bit aliases, short/long/bad-key/padding packets, running requests,
partial frozen masks, child errors/delay, busy release, timeout recovery, lost
boundary, a real completion during status, private sequence rollover and reset.
The actual Native59 Ethernet UIO participates unchanged on the same EXT_BUS
mux used by the new integration. All13broken RTL controls reject. This fixture
qualifies transport and routing, not the internal eleven-board implementations,
which have separate complete-board evidence, or actual FPGA save/restore.

Earlier v1..v3 expose/diagnose an initial index0/board0 unknown in the decoder.
Changing the combinational decode to always_comb fixes its initial evaluation.
V4passes the positive fixture; v5expands coverage but its running-port mutation
fails earlier than the driver's expected diagnostic. V6accepts that earlier
specific failure and passes all13controls. No guest CPU/RAM state is imported;
the rollover preload changes only the private test transport sequence register.

The builder enables all eleven boards/1,944,744modeled state bits in a frozen
Native59-derived experiment. Four shared helpers remain byte-identical;18new
project sources include the complete BaseBoard and the transport. The source
audit independently checks413frozen receipt files,2,137original board pins,
the complete ordinary backplane equations,85other core sources, every state
binding, guest boundary, Ethernet coexistence wiring, constraints and project
policies. All15damaged source integrations reject. Both local staging and the
actual remote inputs pass. Initial auditor attempts rejected derived reports
and Quartus's automatically appended relative source aliases. The accepted
auditor excludes derived outputs and permits exactly one complete relative
alias set resolving to the same18files, without admitting changed sources.

Raw inputs/logs and earlier attempts are losslessly archived; simulator .vvp
objects are excluded. Each manifest row contains the original byte count and
SHA256; all398raw files pass archive readback. Sources/logs/reports and JSON
over100KiB use unchanged gzip bytes. Native59's other source files remain in
the adjacent 2026-10-19-native59-dsketh evidence; its receipt is included here.
The local integration captures both original and candidate source-only core
trees, both changed backplane/wrapper files and prepared adapter assets.
Actual-build review includes the terminal successful map log, source audit,
post-map QSF aliases and staged integration manifest. Fitter/STA outputs are
not included because the build is still active.

Build: `/home/alans/mister/Dorado_Enabled60_20261019`, builder1827796,
fitter1835666 on misterubuntu. The run omits assembler/deployment. Original
50MHz/SYSPER4 clocks, MemD input delay2, SDRAM/DDR policies and SDC remain
unchanged. Live MiSTer14 Native59 and its last qualified1,947,654.12issues/s
are unchanged. Remaining work: actual capacity/timing audit, coherent raw RAM/
wrapper/input/ARM/server/matching-disk barriers and atomic bundle, actual Cedar
restore, original60ns timing migration and final combined release gates.
