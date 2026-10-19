# Exact frozen working Native49 native-machine inventory

This replaces the earlier Native45-style illustrative inventory with actual
frozen working Native49 sources and its QSF compile macros. It does not change
RTL or implement machine save/load. Source hashes, macros, command, XML and
raw inventory are archived. The generated sources.f is supplemented with the
frozen device/backend library added to QSF after generation; only instantiated
cells beneath dorado_native_machine enter the inventory.

4158 instances contain8644 sequential candidates/2107636 bits,4405 initial-
only candidates/87421 bits,13 subroutine-review candidates/335bits and23308
combinational-review candidates/41887 bits. These are conservative review
candidates, not snapshot size or FPGA resource estimates.

Compared with the old deferred-read illustration,128 storage groups each
lose only pipe_addr16bits and pipe_known1bit (256 candidates/2176bits). The
selected non-deferred baseline STILL contains old_data and pending write,
cached address/data, read pipeline and response bookkeeping. Those must not
be discarded during pause/restore; writes may have reached external RAM
before their modeled chip commit.

The complete ContB manifest/classifier is additionally applied to its actual
1572 candidates inside dorado_native_machine.m.b_ContB. All147809 modeled
state bits map to the qualified board export, with the remaining continuous-
wire expressions classified as recomputed. That proof is archived; every
other board/device candidate remains UNCLASSIFIED. Per-scope sequential counts
guide the next access/inventory work. Source code and field layout are still
exact-build dependent; no cross-build compatibility is asserted.

Outside this inventory: actual16MiB external raw SDRAM contents, the outer
emu wrapper's clock gates/phases, keyboard/mouse/DDR-network queues and video
state, MiSTer framework and ARM Main/cache/worker/disk/server state. Physical
SDRAM command/refresh/open-row state must be drained/reconstructed while
refresh keeps running. The host-input/transport barriers and pending/error
cases still need implementation and classification. A complete snapshot
requires these scopes, matching disk generation, atomic validated bundles,
whole-core timing and an actual FPGA Cedar restore.

Reproduce with `fpga/mister/snapshot/inventory_native.py --sources FROZEN_SRC
--qsf FROZEN_CORE_QSF --output NEW_DIRECTORY`. The frozen source tree is the
one copied from Dorado_Native49_20261019/src, not the newer worktree generator.
Local run build/rtl-executive/snapshot-native49-inventory-20261019/handle31688
COMPLETED. The ContB proof uses the qualified manifest/classifier against the
native hierarchy prefix, retaining the source XML hash. No C-state import or
host completion of guest instructions/IOCBs is involved.
