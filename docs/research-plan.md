# Research-driven plan to close the gaps

Companion to `docs/handoff.md` ("Known gaps — full punch list"). Where
that doc enumerates *what* is missing or stubbed, this one specifies
*how we will recover ground truth* and turn each gap into a closed
implementation.

The gap IDs (A1, A2, B1, …) are stable references — cite them in
research notes, code comments, and commits.

## Methodology (per gap)

For each item:

1. **Source sweep.** Collect everything that mentions it across five
   classes: PDF docs, BCPL/Mesa text sources, schematics, microcode
   binaries/sources, other artifacts. Output: a one-line citation list
   (file:section / sheet+signal / source line range).
2. **Read & extract.** Pull the operative passages into a short "spec
   note" at `docs/research/<id>-<slug>.md` (one file per gap).
3. **Cross-check.** Flag deltas between HM, schematic, and microcode.
   HM stays canonical; record where the actual build diverges.
4. **Implementation sketch.** File:line targets in `dorado/src/*.c` +
   `include/*.h`.
5. **Test plan.** One synthetic microtest (locks in the rule) + one
   probe against real microcode (proves it).
6. **Land.** Implement, verify, mark the gap closed in `handoff.md`.

The research note for each gap is **frozen ground truth**. Once
written, future drift gets fixed by editing it rather than re-deriving.
The note is also the spec the Phase 2 (Verilog) port will read.

### Per-gap research note template

```
docs/research/<id>-<slug>.md
---
gap: <id>            # e.g. B1
status: research | implementing | landed
---

## Source citations
- HM §X.Y page Z: <quoted passage>
- Schematic <NAME>.pdf sheet <N> signal <SIGNAL>: <observation>
- chm/.../<file>.mc lines <L1>-<L2>: <relevant code>
- chm/.../*.masm lines <L>: <BB-side perspective>

## Spec
<distilled rule, in our terminology>

## Cross-check / deltas
<HM-vs-schematic-vs-microcode disagreements; what we resolve to>

## Implementation
- file <path>:<line>: <change>
- new tests: <names>

## Verification
- synthetic: <microtest description, expected pass>
- probe: <which real microcode advances, by how much>
```

---

## Phase 0 — Expand the source base (one session)

Before touching any gap, fill in what we haven't surveyed.

**0a. Walk the CHM tree.** Spider
`https://xeroxparcarchive.computerhistory.org/_cd8_/.index.html` and
produce a flat directory listing. Diff against `docs/chm-archive.md`
and `chm/`. Likely unsurveyed branches (anchored by what we've seen):

- `_cd8_/doradodebug/` — Midas + DoradoDebug (ground truth for Read IM,
  breakpoints, parity).
- `_cd8_/doradomicrocode/` — rest of the tree we partially pulled into
  `chm/doradomicrocode/`.
- `_cd8_/doradosource/` — broader source archive sibling of
  `BootstrapSources.dm/`.
- `_cd8_/diag/` — diagnostics microcode.
- `_cd8_/io/` siblings of `io-doradologic`.
- `_cd8_/doradoboot/` if present.

**0b. Grep `chm/cross-reference.html`** (568K-line index, already
mirrored) for the keyword stems we'll need: `ECC`, `Hold`, `Pipe`,
`MCR`, `MCPBus`, `DDC`, `Ether`, `Trident`, `Format`, `subsector`,
`Read.PROM`, `Sequence.PROM`, `IFUM`, `Boot1`, `Boot0`, `Midas`,
`DoradoDebug`, `LoadMB`. Each hit is a candidate fetch.

**0c. 6502 disassembler.** Stand up a tiny disassembler (or borrow one
— `da65` from cc65 works) and produce annotated dumps of:

- `chm/dorado/doradobaserom.mb!13` C000–D7FF (BB Boot1Data region)
- F000–FFFF (Boot0 jam path)

Cross-reference labels from `doradobaserom.dm!12_/*.masm`. **Without
this, A1, A2, D2 cannot be resolved.**

**0d. Update `docs/chm-archive.md`** and
`docs/parc-archive-survey.md` with the new inventory.

## Phase 1 — Foundation gaps (research-light, blocking many others)

Ground truth lives in HM + an existing local source — no archive
expansion needed.

1. **B6** — A[12:15]←FF[4:7] (HM Table 11 footnote, ProcH A-bus mux). Half-day.
2. **B7** — `ff_full_function_ok` gating (HM §3.9). Half-day.
3. **C4 / C5** — silent WP swallow + ReadMap-on-Vacant (HM §5,
   Table 14, kernel.mc fault handlers). Day.
4. **G1** — fast-IO unrouted discard logging (HM §8). Half-day.
5. **D1** — BB unmapped-offset / EPROM-write logging (BB schematic +
   doradobaserom.masm). Half-day.
6. **B11** — breakpoints / parity / perf counters (HM §4.10; Midas
   source from 0a). Day, mostly stubs that record state.

## Phase 2 — BaseBoard & boot stream (unblocks A1/A2)

7. **D2** — disassemble BB ROM (depends on 0c). Output: annotated
   assembly with labels matched to `.masm`. 1 session.
8. **A2** — Boot0 stalls. Walk the actual Boot0 path and identify each
   TIOA Boot0 polls. Cross-reference HM §7 Table 21. Each unmodeled
   register becomes a B2/H2 sub-task.
9. **B2 / H2** — slow-IO devices (Hold/TaskSim register, MCR-as-IO,
   Reset register). Source: HM §7 + ProcH/ProcL schematics +
   doradobaserom.masm. 1–2 sessions. Test: synthetic per device + Boot0
   advance probe.
10. **B8** — `B←RWCPReg` polarity asymmetry (HM page 31 + BB schematic
    RIOT#3 latch + doradobaserom.masm sequencing). 1 session.
11. **A1** — Bootstrap streaming corruption. Pre-reqs: D2, B8. Trace one
    full BootByteL iteration (BootstrapMain.mc, HM §3.11 shifter, HM
    §4.8 Write IM). Likely outcomes: shifter bug, CPReg byte-order
    mismatch, or BB layout differs from `Initial.mb`. 1–2 sessions.

After Phase 2, A4's probe workarounds for Bootstrap can be removed.

## Phase 3 — Memory subsystem (unblocks AEmu and probe_aemu)

12. **B1 / C1** — Hold semantics + deferred refs (HM §4 Hold, §5 timing,
    Figure 9; MEMC/MEMD/MEMX schematics; kernel.mc + memMisc.mc for the
    cycle assumptions microcode makes). Biggest item in the plan.
    2–3 sessions.
13. **C2** — per-slot Pipe4 errors (HM Table 16/17, MEMD schematic).
    1 session.
14. **C3** — ECC (HM §5.12 polynomial, MEMD ECC schematic, memMisc fault
    handlers). 1–2 sessions. Implement only generation + single-bit
    detect first; double-bit can wait.
15. **C6** — MCR decode (HM LoadMcr §5, MEMC MCR latch schematic,
    kernel.mc actual usage). 1 session. Removes the `0xFEE7` workaround
    in A4.

## Phase 4 — Disk to a working hard-disk boot

16. **F1–F4** — sequence PROM, Fire ECC, write side, sector-pulse
    timing. Sources: HM §9 pages 96–104, DSK schematic, read/sequence
    PROM contents (find in 0a survey), InitialDisk.mc, Mesa disk driver.
    2–3 sessions.
17. **F5 / F6** — status readout + tag decode unification (HM §9, DSK
    schematic). 1 session.
18. **A3** — find a Trident pack containing Dorado private microcode at
    "page 4." Pure archival. Look under `_cd8_/disks/`, `_cd8_/dorado/`,
    `_cd8_/doradoboot/` in the 0a survey. If none exists, defer A3
    indefinitely and rely on Path B (Ethernet) instead.

## Phase 5 — Display & terminal

19. **E1 / E4** — per-(task,tioa) dispatch + tighten registration
    (HM §11, DisplayMain.mc, DDC schematic). 1 session.
20. **E3 / E5** — pixel clock + waveform + scanline timing (HM §11, DDC
    schematic). 1–2 sessions.
21. **E2** — 7-wire keyboard back-channel (HM page 120 Table 25, DDC
    schematic 7-wire spec, DisplayMain.mc terminal task). 1–2 sessions.
    After this, the boot-key emulator selection works.

## Phase 6 — Ethernet (alternative to A3)

22. **H1** — Ethernet controller (HM §10, Ether schematic,
    EthernetBoot.mc + EtherEmulator.mc once located in 0a, Booting Memo
    for the Ether boot protocol). 2–3 sessions. With this, Path A (the
    canonical boot) works end-to-end.

## Phase 7 — Microengine residual

23. **B3** — FF table holes (HM Tables 11a–e + per-entry schematic
    refs + mdfields.d usage). Item-by-item. ~1 session per cluster
    (`XorCarry`/`ModStkPBeforeW`, RMaddr-replace family,
    `MidasStrobe`/`UseDMD`, Multiply, Hold&TaskSim/WF/RF).
24. **B4** — Multiply / Divide / CDivide (HM §3.7–3.8, Mesa divide
    subroutines in microd source, Processor schematic multiply-iterator
    FF). 1 session.
25. **B5** — Read IM (HM §4.8, Midas/DoradoDebug source from 0a).
    1 session.
26. **B9** — IFU pipeline staging (HM §6 full chapter, IFU schematic,
    Ifu.mc Simple + Complex). 2 sessions. Most of HM §6 is already
    modeled; this brings the F/G→J→H→M staging up to spec.
27. **B10** — IOAttention wiring (HM §4 + §7). Half session, after B2
    lands.

## Phase 8 — Cleanup

28. **A4** — once A1–A3, B2, C6 land, all probe-only workarounds in
    `tests/test_cpu.c` should be deletable. Delete them and mark A4
    closed.
29. **I1** — pre-plant Mesa state from Pilot Programmer's Manual / Mesa
    Principles of Operation (find in 0a). 1 session.
30. **I2** — refactor env-var-driven probe paths into named test
    fixtures. Half session.
31. **J1** — model-0 / D0 .MB paths (mddump.bcpl source; D0 samples in
    chm/microcode/). 1 session.
32. **J2** — disassembler polish. Whenever convenient.
33. **K1** — cycle-accuracy review. Emergent. After B1/C1/F4/E5 land,
    audit remaining places where a cycle is implicitly free.
