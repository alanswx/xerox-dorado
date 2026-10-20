# Readiness proof and separate disk-cache phase prototype

Serial65Ready contains only the two combinational readiness connections in
`core/Dorado.sv`. It preserves initialization, global hold, all periods and
timing constraints. Its full-core Quartus run is pending; this receipt does not
claim area, timing, full machine pause, a new FPGA image or a speed improvement.

The disk phase prototype is separate and is **not in Serial65**. It adds eighteen
implementation-state bits that require snapshot coverage before adoption. No
timing exception has been added. Functional proof and the serial payload gate
pass, but component source/inventory and matched fit/timing remain required.

`archive-manifest.json.gz` describes 1,403 raw members; every member is stored
losslessly and checked by `archive-readback.json`. Everything except small JSON
is gzip compressed. Generated binaries, object directories and Quartus
databases are omitted. The disk input is included and its hash matches the
executed payload test.

The raw namespaces are:

- `project`: all 659 source inputs and both integration records.
- `classification`: actual-QSF native XML, inventory and fourteen rejecting cases.
- `msa`, `owner`, `fourstate`: normal/restore traffic, actual eight-board SPI
  ownership, and exhaustive local four-state readiness checks, including failing
  variants that demonstrate the guards are tested.
- `disk-capture`, `disk-phase`, `disk-payload`: clock-condition check, paired
  original/prototype registers and buffer, and independent serial oracle plus
  whole-image writeback.
- `tools`: executed tools, fixture dependencies and frozen required cell models.
- `replay`: results from extracted tools. `archive-replay.json` summarizes them.

Replay requires the prior immutable receipt directories already committed in
this repository. Extract members at their `raw_name`; point the extracted
`tools/fpga/mister/reports` at those receipts. No working HDL is required. Use the
extracted tools to restage at the recorded destination, run source controls,
regenerate the MSA/owner/four-state/disk tests and compare their results. The
archived payload input replaces the original disk path for that test. Existing
receipt files are verified by their own manifests before use.

The clock-condition proof models the core's falling-edge ALTCLKCTRL enable
contract and PLL-reset startup, including a gate initially enabled. It does
not substitute for the encrypted vendor timing model. The transport fixture
stops guest clocks and retains eleven endpoint/five service stubs; normal guest
traffic is exercised by the separate actual-MSA test. Frontend readiness is
not proof of complete arbiter/SDRAM drainage.

Pending build: `/home/alans/mister/Dorado_Serial65Ready_20261019`, builder2458594,
fitter2461232. Observe the same live handles; do not restart on observation
timeout. Assembly is disabled. Source packet SHA256:
`9a4e93d8fadb7b64e3fa97e91c0ae8b37809ece5636f965914354a504e22160d`.
