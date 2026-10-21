# XSDS SDRAM verification model

Originally vendored from `birdybro/SDRAMsim_MiSTer`, commit
`7e1ccf88a8b178212d189ecff5bb0ad7623cbf58`, under the accompanying MIT
license (Kevin Coleman, 2026). This is simulation-only; it is not FPGA RTL.
File hashes and primary schematic/datasheet provenance are recorded in
`fpga/mister/reports/2026-10-18-sdram-preparation/provenance.json`.

The Dorado fixtures instantiate the two independent chip models directly
and wire them according to the XSDS schematic: inverted rank selection and
DQM on A11/A12. This avoids hierarchical `defparam`, unsupported by our
Verilator version. They override timing to cover the slower -7 part and
CAS2's 6 ns maximum access time. The full-board equivalence fixture alone
sets unwritten cells to zero to match its reference; hardware uses the
existing explicit raw-memory zero-fill.

Run `make -C verilog storage-sdram-test storage-sdram-board-test` for the
raw-port and complete-board comparisons. Run
`python3 tools/check_storage_sdram.py` for the frequency matrix and runtime
negative controls. Passing these does not qualify fitted pin timing or a
physical SDRAM module.

Local correction (2026-10-21): natural read-burst completion now retires the
burst sequencer without immediately clearing output-enable. The old
`stop_burst()` call glitched the preceding beat before the final beat's delayed
data launch. The new pin-only burst test catches this at0.5ns after the clock;
restoring the old model fails that check. The chip's minimum data hold is2.5ns
([Alliance AS4C32M16SB Rev1.4, Table16](https://www.alliancememory.com/wp-content/uploads/AllianceMemory_512M_SDRAM_Bdie_AS4C32M16SB-7TXN-6TIN-7BIN__Rev1.4_June2024NK.pdf)).
The next inactive clock still releases DQ through the existing tHZ delay.
Command/timing/refresh checks are retained. Existing BL1 frequency/delay tests
and all three timing-error controls pass with this correction. Previously
frozen native simulations keep their original model bytes and are unaffected.
