# Selectable raw SDRAM backend preparation

The guest still has8MiB (4M16-bit words); its existing padded16MiB raw
data/ECC backing moves to the128MB external module. The original MSA
boards, cache, map and ECC logic stay upstream of the same request/reply
interface. DDR remains the default selectable control.

The controller uses the XSDS two-rank module geometry, CAS2,
single-word bursts, open rows,200us startup, and periodic refresh of both
ranks. Refresh continues through reset and response backpressure. Accepted
operations drain during reset and their obsolete completions are discarded.
It requires an inverted physical clock and a20..50MHz host. Timing covers
Alliance Rev1.2's slower -7 grade, including CAS2 tAC6ns. Fitted constraints
declare a1ns board flight/skew budget; that budget is not a measured trace.

`results.json` and compressed simulator output record:

- 309 raw-port checks,156 reads/153 writes, both ranks and all banks, row
 boundaries, deterministic randomized data, response backpressure, refresh,
 and accepted/completed-operation reset cases.
- Passing chip-model runs at20,33.333333 and50MHz with zero model errors or
 warnings. The model is independent, vendored unchanged under MIT; see
 `provenance.json` and the vendor README/license.
- Compiled mutations fail at runtime: tRCD shortened below the -7 limit,
 and refresh disabled while a response is pending. Compile errors cannot
 satisfy these controls. The former rising-edge input capture also fails
 with a6ns delayed physical clock; the corrected capture passes.
- Full 64Kbit-chip MSA board comparison:12012 cycles,3008 reads,7872 writes,
  1111 output transitions; all9,437,184 stored bits identical. Only this
 fixture seeds unwritten model cells to zero, matching its reference.
 Hardware retains the explicit raw zero-fill.

Reproduce with `make -C verilog storage-sdram-test storage-sdram-board-test`
and `python3 tools/check_storage_sdram.py`. Generate and stage with:

```sh
python3 tools/sil_fpga_native.py -o OUTPUT --storage-backend sdram \
  --storage-modules 4 --storage-chip-kbits 64 --map-kwords 64 --pilot-disk \
  --receive-monitor
DORADO_SYS_MHZ=50 fpga/mister/build.sh TEMPLATE OUTPUT_CORE OUTPUT
```

 The builder adds the backend macro, physical
pins and SDRAM timing file; it rejects unsupported clocks before staging.

Native37 used a backend-only transformation of frozen Native36 rather than
regenerating unrelated machine inputs. Its build recipe lives in the sibling
native37 report directory. It FAILED timing: -4.436ns SDRAM input setup and
-0.040ns internal MemC MLAB hold. It was not deployed. Native38 adds an
unconditional falling-edge input register and transfers into the rising-edge
response one host clock later, preserving the same external constraints.
The revised simulations pass; a new fit/all-device audit is still required.
Hardware remains unqualified. No measured speedup is claimed. Native36's DDR line
buffer has128 entries; the SDRAM candidate has none, requiring a matched
cache-off DDR comparison for isolated attribution.
