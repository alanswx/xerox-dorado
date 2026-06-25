# IfuSource — the real Ifu diagnostic sources

Pulled from CHM `[_CD8_]<doradosource>IfuSources.dm!53_`. URL form:

```
https://xeroxparcarchive.computerhistory.org/_cd8_/doradosource/IfuSources.dm!53_/<file>
```

`ifu.cm` is the build manifest. Two worlds are built:
- **IfuSimple.mb** (`IfuSimple.mc` → runs `beginIfu1`)
- **IfuComplex.mb** (`IfuComplex.mc` → `beginIfu1` + the ifu3* exception tests)

| file | role |
|---|---|
| `ifu1.mc` | `beginIfu1`: disableConditionalTask → `ifuMemRW` → `iMemRandAddrs` → `iMemAddrs` → `iSingleStepTest`. `iSingleStepTest`/`itTestCase1` is the **first exception test** and where IfuSimple/IfuComplex currently FAIL. |
| `IfuSubrs.mc` | core IFU subroutines: `checkException`, `afterDispatch`, `readMuffler`/`readMufBit` (MidasStrobe), the exception entry-point vectors. |
| `ifu3a/b/c.mc` | the heavier exception tests: reschedule (`iReschedTest`), RAM-parity (`iRamPEtest`), `ifuChaos`, `ifuBrkInsTest`. |
| `IfuDefs.mc` | `exceptions.*` bit layout, `ifuTest` register, muffler addresses. |
| `ifuTestSubrs.mc` | `initIfuCache`/`iMem` (memory + IFUM setup; clears `exceptionsMask`). |
| `ifuStepSubrs.mc`, `ifuRamSubrs.mc` | single-step + RAM helpers. |

## Why IfuSimple/IfuComplex fail (IFUEXCEPTIONERR) — the mechanism

Both stop at `IFUEXCEPTIONERR` (real 0o3013, task 2) early (~1631/1652 steps), in
`iSingleStepTest`'s `itTestCase1` — the first test that uses the **ifuTest
register** (`ifuTest←T`, ifu1.mc).

The IFU exception model (HM §6, Tables 14/20):
- An **IFUJump** that dispatches an opcode can take an **exception** instead of
  the normal entry vector. Four conditions × four FGP bytes (0..3):
  `kfault` (kernel/map fault on the prefetched byte), `FGPe` (FG parity error),
  `RamPe` (IFUM RAM parity error), `resched` (reschedule). Bit layout
  (`IfuDefs.mc`): kfault0=b12/kfault3=b0, FGPe0=b13/FGPe3=b1, RamPe0=b15/RamPe3=b3,
  resched0=b14/resched3=b2. "InstrSet bits, complemented, are OR'd into the
  exception dispatch addresses."
- On an exception the IFU dispatches to a dedicated **trap vector** (the
  kfault/resched/rampe entry points placed by the diagnostic). That entry-point
  handler sets `exceptionsMask` (an RM register) with the fired bit and calls
  `checkException`.
- `checkException` (IfuSubrs.mc): `t ← exceptionsMask; t AND exceptionBit;
  skpif[ALU#0]; error`. So if the expected exception bit is NOT present in
  `exceptionsMask`, it errors → `IFUEXCEPTIONERR`.

`itTestCase1` loads `ifuTest` to **force** a specific exception, does an
IFUJump, and expects the matching exception dispatch. Our engine models a
simplified IFUJump (no exception conditions, no trap vectors), so no exception
fires, `exceptionsMask` stays 0 (cleared by `initIfuCache`), and the first
`checkException` errors. T=0o100000 at the error = the kfault3 bit (b0).

## What an implementation needs (roadmap)

1. **`ifuTest←B`** FF function — store the IFU test/diagnostic control register
   (the analog of MOS/genIO for the IFU). Decode which exception it forces.
2. **IFUJump exception detection + dispatch** — when an exception condition is
   set (forced via ifuTest, or naturally: map fault on the prefetched byte for
   kfault, IFUM parity for RamPe, reschedule countdown for resched), dispatch to
   the exception trap vector (HM Table 14: base + complemented-InsSet) instead
   of the normal IFUM entry vector, and latch the exception (JMPEXC).
3. **`exceptionsMask` feed** — the readMuffler/JMPEXC path so the entry-point
   handlers and `checkException` see the latched exception bits. (`readMufBit`
   uses `MidasStrobe←T` to shift muffler bits out; the diagnostic-muffler model
   is the same shape as the disk-state muffler in Tricond.)

This is a sizable HM §6 feature (comparable to the event-counter work). The
`reschedule` exception is the one we already partially model (`reschedule_pending`),
so the `resched` dispatch may be the cheapest first exception to wire end-to-end.
