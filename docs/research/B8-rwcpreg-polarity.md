---
gap: B8
status: research (deferred — must land with A1 fix)
---

# B8 — `B←RWCPReg` polarity

## Source citations

- **HM page 31**: `B←RWCPReg = Link←B, B←CPReg'`. The prime denotes
  inversion: B carries the **complement** of the CPReg latch's
  current value.
- **`BootstrapMain.mc` lines 213–229** (`ReadBB:` subroutine): every
  read of CPReg goes through `RWCPReg`, and the protocol is built
  around the inverted value:
  ```
  T_ RWCPReg;                       * T <- ~CPReg
  PD_ (Tag) XOR T;                  * set ALU flags
  T_ RWCPReg, Branch[.-2, ALU<0];   * loop until Tag[15] == T[15]
  Tag_ NOT (Tag), Link_ Q;          * flip Tag for next call
  ChkSum_ (ChkSum) XOR T, Return;
  ```
  Tag is initialized to 0; after the loop, Tag is flipped, so the
  next call waits for the opposite high bit. This implements a
  classic ping-pong sync where the BB toggles CPReg's high bit
  between successive bytes.
- **`doradoboot.masm SendAHalfMicroInstruction`**: per call, the BB
  performs four MCPBus events:
  1. `ABMux1 ← byte_n`  (CPRegL ← byte_n; CPRegH unchanged)
  2. `ABMux0 ← 0`       (CPRegH ← 0; high bit goes 1→0)
  3. `ABMux1 ← byte_n+1`  (CPRegL ← byte_n+1)
  4. `ABMux0 ← (MicroHalf << 1) | extra_bit`  (CPRegH high bit goes
     0→1, since `MicroHalf = 0x40` rotated through carry sets bit 7)

  So between calls, CPRegH's bit 7 alternates 1 → 0 → 1 → 0 → …,
  which is exactly the sync transition Bootstrap waits for.
- **`doradoboot.masm LoadDoradoCode`** preset: before launching
  Boot0, the BB writes `STA ToCPRegH` with `LDAI 80`, i.e.,
  `CPRegH ← 0x80`. With the manual's `~` inversion, the Dorado
  reads `T = ~0x8000 = 0x7FFF`, T[15] = 0. Bootstrap's first ReadBB
  enters the wait loop with Tag=0; (Tag) XOR T = 0x7FFF, ALU<0
  evaluates `T[15] = 0`, the loop exits on the first iteration.
  This is the synchronization the source describes (line 96:
  *"The Baseboard starts Bootstrap with CPReg[0] = 1"* — `[0]` is
  manual-MSB-first numbering = bit 15 in C-LSB).

## Spec

The polarity is unambiguous: **`B←RWCPReg` always returns
`~CPReg`**. There is no spec-supported "non-inverted" mode.

## Current implementation (cpu.c lines ~542–571)

```c
case 6: /* B ← RWCPReg */
    if (cpu->baseboard) {
        uint16_t v = baseboard_dorado_read_cpreg(cpu->baseboard);
        if (cpu->baseboard->dorado_running &&
            !cpu->baseboard->dorado_ss_pending) {
            *b = v;                 /* ← NON-spec: no inversion */
        } else {
            *b = (uint16_t)~v;      /* spec-correct */
        }
    } else {
        *b = (uint16_t)~cpu->cpreg; /* legacy stub: spec-correct */
    }
    cpu->Link = *b;
    break;
```

The BB-running branch returns the **un-inverted** value. The
explanatory comment notes that this asymmetry was put in
empirically because reverting to always-invert "triggers Bootstrap
to compose Loc with bit 15 set, sending writes into Boot0 region
… corrupting Bootstrap itself."

## Diagnosis

The empirical observation was real but the diagnosis was wrong. The
asymmetry is a band-aid for a separate bug — almost certainly in
how our microengine assembles bytes via `LSH[T, 10]` /
`LDF[T, 10, 0]` / `XOR` during `BootByteL` (gap A1). With the
correct inversion AND the correct shift-mask semantics, the BB
stream and the Dorado decode line up bit-exactly. Today, the
asymmetry is hiding a downstream LSH/LDF/XOR error.

Concretely: with always-invert, the first ReadBB returns
`T = ~0x8000 = 0x7FFF`. `Loc_ LSH[T, 10]` (= LSH-by-octal-10 = 8
bits) leaves Loc's low byte = ~CPRegL high bit set, etc. — but the
*data* the BB intends is in `CPRegL`, not `T[15:8]`. The
reconstruction needs `T_ LDF[T, 10, 0]` to extract just the data
bits from the inverted form. Our LDF implementation may be
extracting the wrong field width or position.

## Plan

B8 cannot be cleanly resolved in isolation. Instead:

1. **Land A1's actual fix first** using `BootstrapMain.mc` and the
   `bbdis --hunks` output as paired oracles.
2. While doing A1, add a per-cycle trace of `T` after every
   `T_ RWCPReg`, every `LSH`, every `LDF`, every `XOR`.
3. With always-invert in place, verify the trace matches the bytes
   `SendAHunk` is actually putting on CPRegL.
4. Once A1 lands and `probe_full_boot_with_bootstrap` produces a
   bit-exact match against canonical `Initial.MB`, the BB-running
   branch in `B←RWCPReg` should be replaced with the spec-correct
   inversion. The BOOTSTAGE2 substitution shim in
   `tests/test_cpu.c` becomes deletable at the same time.

## Non-action

No code change for B8 in this phase. Marking deferred until A1
lands. Comment in `cpu.c` already explains the band-aid; that
explanation now points at this research note.
