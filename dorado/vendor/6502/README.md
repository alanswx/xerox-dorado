# fake6502 (vendored)

A 6502 CPU emulator used by `dorado/src/baseboard.c` to run the
Dorado BaseBoard's 6502 firmware (~12 KB of code + microcode payload
in the F-prefix and C-prefix EPROMs).

## Upstream

- **Project:** C-Chads/MyLittle6502
- **URL:** <https://github.com/C-Chads/MyLittle6502>
- **File vendored:** `fake6502.h` (single-header implementation)
- **License:** CC0 / public domain — see `LICENSE` in this directory.

This is a maintained fork of Mike Chambers's original
[fake6502.c](http://rubbermallet.org/fake6502.c) (2011), with bug
fixes for decimal mode (BCD) and the ADC/SBC/BIT/IRQ-mask handling.
Verified against Klaus Dormann's 6502 functional-test exerciser.

## Why this fork

- **Correct decimal mode** — the original had bugs in BCD that the
  Dorado firmware doesn't appear to exercise but that we shouldn't
  inherit either way.
- **Single header, no platform deps** — drops in cleanly, no make
  target needed beyond an `-I` to find it.
- **Callback-based memory bus** — exactly the
  `read6502(addr)` / `write6502(addr, value)` interface the BaseBoard
  module needs to dispatch RAM / EPROM / I/O at 0x400-0x67F.
- **Cycle-counted stepping** — `step6502()` returns ticks for one
  instruction; `exec6502(n)` runs until at least n cycles have elapsed.
- **Reset / IRQ / NMI hooks** — `reset6502()`, `irq6502()`,
  `nmi6502()` all available; we use IRQ for the 6532 timer underflow.

## Integration notes

1. **Single instance only.** fake6502 keeps CPU registers (`pc`,
   `a`, `x`, `y`, `sp`, `status`, `clockticks6502`, …) at file scope.
   Our `baseboard.c` includes `fake6502.h` once and exposes a
   `baseboard_active` global pointer so the read/write callbacks can
   find the right BaseBoard. We can only have one BaseBoard per
   process — fine for the Dorado, since the real machine had exactly
   one.
2. **No `NES_CPU` define.** The header defaults to NMOS BCD-on
   behavior, which is what the Dorado's stock 6502 expects.
3. **Don't define `FAKE6502_USE_STDINT`** in the current
   build — line 141 of the upstream is missing a trailing semicolon
   and that path won't compile. We use the default `unsigned short`
   typedef.

## Local changes

None. The file is pristine from upstream
([github.com/C-Chads/MyLittle6502](https://github.com/C-Chads/MyLittle6502)).
If we ever modify it, document the diff here.

To re-fetch:

```sh
curl -L -o dorado/vendor/6502/fake6502.h \
  https://raw.githubusercontent.com/C-Chads/MyLittle6502/main/fake6502.h
curl -L -o dorado/vendor/6502/LICENSE \
  https://raw.githubusercontent.com/C-Chads/MyLittle6502/main/LICENSE
```
