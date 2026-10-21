# Native BL8 fit and guarded assembly

The full MC1668-parent design with the new raw backend passes preassembly
review. All31 device summary checks,196 inherited timing categories and56
new buffer-path categories pass over all four operating conditions. Minimum
detailed slack is+0.107ns. Actual fit:30,882 ALMs,53,193 registers,332 RAM
blocks and35 DSP blocks at unchanged50MHz/SYSPER4.

All170 logical/physical RAM rows retain behavior and geometry, and all165
inferred-memory parameter tables match the qualified parent. The unchanged
BaseBoard ROMs occupy a repacked shared seven-block pool. The guest's cache
still occupies40 independent M10Ks. The new raw buffer infers no RAM blocks:
384 data flip-flops,80 tag bits,four valid bits and12-bit input/response groups.
The original MSA groups consume at most12 bits per raw halfword, so synthesis
correctly removes four unused high data bits from the generic16-bit backend.

The reader rejects seven altered-evidence cases, including a rehashed relaxed
clock, missing cache register, unused compiled helper, changed RAM geometry,
negative new capture setup, negative original SDRAM hold and a missing corner.
A first reader-fixture selector also matched two unrelated bank-zero rows;
that fixture error and its three preceding rejections are preserved. The
separate corrected attempt passes all seven cases.

The first report script collided with an SDC scalar named `group`. Its terminal
failure and exact script are preserved. A new attempt uses a distinct array
name and completes against the same fit/database. No RTL/SDC/refit change.
The only new synthesis warning is classified exactly: FIFO wrap's unsized
zero causes32->2 truncation; the other branch increments only below three.

The guarded assembler checks all499 compiled inputs, every reviewed report
hash, absent build/review processes and all seven reader controls. Assembly
succeeds with RBF SHA256
`f46eb4f5bc68c1060ad14e96cb127d5bb05d5af85ffa48a5b1f15fc4123062c6`.
The image is3,908,208 bytes. This receipt excludes the binary and databases.

882 members preserve raw reports, reviewed sources, process return codes,
qualification, assembly receipt and altered-evidence inputs. Unchanged blobs
are shared with the preceding component archive. Verify lossless readback:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-sdram-burst/native-fit
```

This is timing/memory/assembly qualification. Original-firmware Cedar boot,
interactive desktop, pinned hardware progress/holds and speedup remain open.
