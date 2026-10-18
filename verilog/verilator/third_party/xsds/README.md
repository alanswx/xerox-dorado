# XSDS SDRAM verification model

Vendored unchanged from `birdybro/SDRAMsim_MiSTer`, commit
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
