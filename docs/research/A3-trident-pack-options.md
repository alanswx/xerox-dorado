---
gap: A3
status: research (no public Dorado Trident pack exists; build path identified)
---

# A3 — Finding or building a Dorado-bootable Trident pack

## What's needed

`InitialDisk.mc` lines 7–22 spell out the on-disk requirements for
`DiskHardMicrocodeBoot` to succeed:

```
* Hard disk microcode file format.
* Note: this is a private Dorado convention, shared only with FormatTridentImpl
* in Othello.

* The microcode file always starts at page:
    MC[firstHardUCodePage, 4];   * i.e., cylinder 0, head 0, sector 4

* Its FileID is:
*   [processor (0): [0, 0, 0], serial (3): [hardUCodeSerial, hardUCodeSerial]]
    MC[hardUCodeSerial, 064732];
* Its type is zero and it is not temporary and not immutable.

* Though it is not actually contained in any Pilot logical volume,
* this file looks more-or-less like a standard Pilot boot file, containing
* the microcode image in .eb format (including initial overhead page).
```

Concretely:

- A Trident T-80 or T-300 pack image (815 cyl × 5 or 19 heads × 9
  sectors, 1024 words/sector, ContrAlto2/Bitsavers byte layout
  = 2074 bytes/sector with 2-byte dummy + 4-byte header + 20-byte
  label + 2048-byte data).
- Cylinder 0, head 0, sector 4 begins a Pilot-style file whose
  label.fileID.serial (high half) and label.fileID.serial (low half)
  both = `0o64732` (the magic constant `hardUCodeSerial`).
- The file is `.eb`-format — first page is the .eb overhead (will
  be skipped by Initial), subsequent pages are the loaded microcode
  image.
- Pages of the file are linked via `Label.bootChainLink`. End of
  file is `bootChainLink = [-1, -1]`.

## Search results — no public Dorado Trident pack exists

Searched both CHM mirrors (`xeroxalto`, `xeroxparcarchive`) and
the local `chm/cross-reference.html` (45 MB index, 568K lines):

- **No `.dsk300`, `.dsk29`, `.tridentpack`, or similar files** exist
  on either CHM mirror under any Dorado-related path.
- The names with `.disk!N_/` extensions on CHM are *expanded
  archives* (server-side directory listings of the contents of an
  Alto/D0 development disk), not raw disk images. They cover D0
  diagnostic packs (`D0cardtester.disk`,
  `EDmicroDiagnosticsRev1x1.disk`, `IOcardtests.disk`,
  `testerdrawings.disk`) — none are Dorado Trident packs.
- The `eris/lispcore/dlioninitial/trident/{klamath,sierra}/`
  directories sound promising but contain DLion (Dandelion)
  initial-build *configurations* — `CoreInitial.mc`,
  `Dandelion.dfn`, `GetLispInitial.cm`, `InitDLion.mc`,
  `InitialFiles.cm` — meant to be assembled into an initial that
  runs on a Dandelion booting from a Trident, *not* the resulting
  disk image.
- The only Dorado-format Trident pack we (or anyone we know of)
  has is `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300`
  — a Spruce print-server pack, no `hardUCodeSerial` file at page 4.

This matches the local `docs/parc-archive-survey.md` §3.4 finding:

> Searched both mirrors and `chm/cross-reference.html` (45 MB index):
> no Trident pack images are hosted on `xeroxalto` or
> `xeroxparcarchive`. … This was a real surprise — Mesa-on-Dorado
> and Cedar-on-Dorado disk packs apparently never made it onto a
> public mirror.

## Three viable paths

### 1. Synthesise a pack from the microcode binaries we already have ★

This is the most actionable option. We already have all the inputs.

**Inputs:**

- `chm/dorado/Mesa.mb!3` (71 KB) — Mesa emulator microcode, .mb form.
- `chm/dorado/Cedar.mb!6` (66 KB) — Cedar emulator.
- `chm/dorado/AEmu.mb!2` (40 KB) — Alto emulator on Dorado.
- `chm/microcode/AltoMesaDorado.eb!1` (35 KB) — Alto-Mesa, already
  in `.eb` form (= `.mb` wrapped with overhead page for booting).
- `chm/dorado/expanded/BootstrapSources.dm/InitialDisk.mc` — the
  on-disk format spec.
- `chm/dorado/expanded/BootstrapSources.dm/PilotDiskDefs.mc` —
  the Pilot label layout (`Lab.filePageLo`, `Lab.bootChainLink`,
  `Lab.fileFlags`, etc.).
- `indigo/cedar/othello/FormatTridentImpl.mesa!2` (12.7 KB) —
  the canonical Mesa source for the format-and-install routine.
  *Worth pulling and reading as the construction reference.*

**Recipe (as a future task):**

1. Allocate an empty T-80 image (815 × 5 × 9 sectors = 36675
   sectors × 2074 bytes = 76 MB). Or T-300 (815 × 19 × 9 = 139365
   sectors).
2. For each sector, fill the 4-byte header with `(cyl, head|sector)`
   words and zero the data.
3. Starting at sector 4 (cyl 0, head 0, sector 4):
   a. Compute how many disk pages fit the chosen `.eb` file
      (= ceil(filesize / 2048) + 1 for the overhead page).
   b. For page 0 (the .eb overhead page), put the .eb's first 2048
      bytes (or pad with zeros if smaller) into the data area.
   c. For pages 1..N, put the corresponding 2048-byte slice of the
      `.eb`/`.mb` content.
   d. Write each page's label:
      - `Label.fileID.processor = 0`
      - `Label.fileID.serial.high = 0o64732`
      - `Label.fileID.serial.low  = 0o64732`
      - `Label.type = 0`
      - `Label.fileFlags = 0` (not temporary, not immutable)
      - `Label.filePageLo = current page index (0..N)`
      - `Label.filePageHi = 0`
      - `Label.bootChainLink = (cyl, head, sector)` of the next
        page if a run boundary is crossed; `(0, 0)` for sequential
        next; `(-1, -1)` on the last page.
4. Save as `mesa-dorado-trident.dsk300` (or similar name).

The total amount of code is small: a Mesa-format-pack helper of
maybe 200–300 lines of C. Should be a one-shot script, not part
of the live emulator build. Reference it from
`docs/parc-archive-survey.md` once it exists.

### 2. Pull `FormatTridentImpl.mesa!2` and reverse-engineer it

`indigo/cedar/othello/FormatTridentImpl.mesa!2` (12.7 KB,
22-May-1983, by `atkinson.pa`) is the actual Mesa source that
formats a Trident pack and installs the hard-microcode file. It is
the *canonical* answer to "how does the pack get built." Reading it
gives us:

- Exact label byte layout (down to bit positions).
- The cylinder-stride / head-stride convention for chaining pages
  across run boundaries.
- The placement of any auxiliary metadata (volume label, free-page
  table) that we might need to satisfy.

`FormatTrident.mesa!2` (2.7 KB) is the small wrapper Mesa command
that calls into `FormatTridentImpl`. Both should be pulled before
attempting Path 1.

### 3. Find an archived pack from a third party

Channels worth checking, in order of likelihood:

- **`bitsavers.org/bits/Xerox/`** — Al Kossow's collection.
  Periodically receives Xerox tape/disk donations. May have
  Dorado Trident images that never made it into the CHM mirror.
- **Living Computer Museum / LCM+L archive** (now defunct as a
  museum but the digital collection has been redistributed) —
  some Xerox PARC dev disks went there.
- **Mailing list / colleague queries**: `simh-talk`, the
  `simulator` mailing list, the `classiccmp` list. Direct e-mail
  to Eric Schmidt (the original `FormatTridentImpl` author per
  the file timestamp) or other PARC alumni who may still have
  a personal pack.
- **CHM directly**: per `chm-archive.md` the public mirror is a
  subset of CHM's full holdings; CHM staff can sometimes
  retrieve specific items if a credible request is filed.

Path 3 has the highest reward (a real, verified-correct pack)
but unbounded latency. It's worth firing off in parallel with
Path 1's construction work.

## Not-actually-needed-yet escape hatch

Path B in the boot-bringup plan (Ethernet boot) bypasses the disk
problem entirely. The Dorado boots from disk OR Ethernet; if we
implement gap H1 (Ethernet controller) we can fetch Mesa.eb /
Cedar.eb / etc. directly via the Pup `EtherBoot` protocol, and
the Trident pack issue becomes irrelevant for first-light.

This is documented in `boot-bringup-plan.md` and in
`docs/research-plan.md` Phase 6. Once H1 lands, A3 (this gap)
becomes a "nice to have" rather than a blocker, since most of the
useful microcode worlds (Mesa, Cedar, AEmu, AltoMesa) are
available as `.eb` and bootable over Ether.

## Recommended next steps (in priority order)

1. **Pull `FormatTridentImpl.mesa!2`** and `FormatTrident.mesa!2`
   from `indigo/cedar/othello/`. Read both. Add a section to this
   note transcribing the exact label/sector layout it produces,
   including any non-obvious fields.
2. **Pull `Lab.*` definitions** from `PilotDiskDefs.mc` (already
   have) plus `Pilot.PressFile` / `PilotDisk.Label` from
   `indigo/cedar/pilot/` — which is the Cedar/Mesa side definition
   of the Pilot label record.
3. **Decide whether to build (Path 1) or try sourcing (Path 3) first.**
   Path 1 is bounded; Path 3 may produce a verified pack with no
   construction-bug risk. Doing both in parallel is fine.
4. **In parallel: implement gap H1 (Ethernet)**. Lands on
   `docs/research-plan.md` Phase 6. Even if A3 ends up being
   unsolvable, H1 provides an alternate boot path that's
   demonstrably functional (the Pup `EtherBoot` protocol is
   well-documented and the `.eb` files are already on hand).

## Caveat on Path 1

`InitialDisk.mc` reads page 0 of the file (the .eb overhead) into
memory page 0 first, then transfers the rest into a buffer. If our
synthetic pack's overhead page contains *any specific magic* that
Initial validates beyond just "successfully transferred," we'd
need to match that. Per the source comment:

> Perhaps see if the overhead page contains something reasonable —
> if the overhead page of an .eb file ever contains anything worth
> checking!

— so apparently nothing currently checks the overhead page beyond
"transfer succeeded." Good news: any `.eb` file we already have
should work as the source.

## Summary

- **Public Trident packs for Dorado: none** (verified).
- **Most pragmatic path**: build a synthetic pack from any
  existing `.eb` + the `InitialDisk.mc` / `FormatTridentImpl.mesa`
  spec. Bounded work, all inputs in hand.
- **Backup path**: pivot to Ethernet boot (gap H1).
- **Long-shot**: ask third parties for an archived pack.
