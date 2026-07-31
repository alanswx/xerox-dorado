# Handoff — Cedar 6.1 on an emulated file server: `ls` and `OpenR`

> **Status 2026-07-30 (later the same day): ALL FIVE PHASES ARE DONE, natively
> and in the browser.** `List [Cedar]<Cedar6.1>VersionMap>*` prints every file
> with its true version, size and date — screenshot
> `docs/images/cedar-ls-versionmap-2026-07-30.png`, gate `make verify-cedar-ls`
> — and in the deployed page shape both `ls` and the author's own
> `OpenR TiogaDoc` work, verified in Chrome:
> `docs/images/cedar-browser-ls-openr-2026-07-30.png`. Getting there needed one
> thing this document did not know about: **the LookupFile packet exchange**
> (§7.3). See §7 for what changed and what the numbers really were.

Written 2026-07-30, after a Cedar author tried the emulator, said Cedar was
"really close", found `OpenR` did not work, and suggested two things:

1. host Cedar 6.1 on an emulated **file server**, so you can say
   `[fileservername]<Cedar6.1>` and do an `ls`;
2. get **`OpenR TiogaDoc`** working.

This document is self-contained: what actually happens today (measured, with
the command that reproduces it), how Cedar's remote file access really works
(grounded in the original Xerox sources in `chm/cedar/`), and a phased plan.

**The headline is not what it looks like.** `OpenR` is not broken. It works
today, including the author's exact `OpenR TiogaDoc`, on the native full-tree
path. What is broken is `ls` against a remote directory — our STP server has
no enumeration — and what the author almost certainly hit is that the
**deployed browser build serves 2 of Cedar 6.1's 70 directories**.

---

## 1. What actually happens today

Every result below was measured on `49332c3` with the desktop checkpoint,
serving the full `chm/cedar/stp-root`. The common command is:

```
cp -f build/good-packs/cedar-desktop.pdi /tmp/probe.pdi
DORADO_FTP_TRACE=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_FAKE_TIME=1783285880 \
./build/dorado --boot-reason disk --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --pilot-disk /tmp/probe.pdi --ftp-root ../chm/cedar/stp-root \
  --snapshot-in build/good-packs/cedar-desktop.snap --key-hold 1600000 \
  --type-at 31100000000 --click 700,733 \
  --paste-at 31300000000 --paste '<COMMAND>\n' \
  --cycles 40000000000 --out /tmp/probe.pgm
```

### 1.1 `OpenR` works. So does `OpenR TiogaDoc`.

`OpenR CommandTool` opens a real Tioga viewer titled
`[Cedar]<Cedar6.1>CommandTool>CommandTool.mesa!1`, with the Tioga menu bar
and the formatted source — italic comments, bold headings, the `DIRECTORY`
clause — in real Tioga fonts. 100,623 px. Screenshot:
`docs/images/cedar-openr-commandtool-2026-07-30.png`. The wire trace shows
why it worked:

```
STP_SERVE Cedar6.1/CommandTool/CommandTool.mesa (17048 bytes)
STP_SERVE Fonts/TiogaFonts/Tioga10I.ks (1836 bytes)
STP_SERVE Fonts/TiogaFonts/Math10I.ks (1888 bytes)
STP_SERVE Fonts/TiogaFonts/TimesRoman8.ks (1176 bytes)
STP_SERVE Fonts/TiogaFonts/TimesRoman12B.ks (2040 bytes)
```

— the source, then the fonts the viewer demand-fetched to paint it. Zero
`STP_MISSING`.

`OpenR TiogaDoc` — the author's exact command — likewise opens
`[Cedar]<Cedar6.1>Documentation>TiogaDoc.tioga!1`: "The Tioga Editor", the
byline list, the Cedar 6.0 banner and the Abstract, in real Tioga fonts.
92,271 px. It serves `Cedar6.1/Documentation/TiogaDoc.tioga` (97,028 bytes)
and then `Tioga10I.ks`, `TimesRoman8.ks`, `Helvetica18B.ks`, `Math8.ks`.
Screenshot: `docs/images/cedar-openr-tiogadoc-2026-07-30.png`.

### 1.2 …but in the browser it silently opens the WRONG FILE

`make web` serves a **pruned** tree (`dorado/Makefile`, target `web-stp`):

```
cp -R ../chm/cedar/stp-root/Cedar6.1/Top     '$(WEB_STP_DIR)/Cedar6.1/'
cp -R ../chm/cedar/stp-root/Cedar6.1/Viewers '$(WEB_STP_DIR)/Cedar6.1/'
```

Two directories out of seventy. `Cedar6.1/CommandTool/`,
`Cedar6.1/Documentation/`, `Cedar6.1/VersionMap/` are all absent. Measured
against that tree, the same `OpenR CommandTool`:

```
STP_MISSING Cedar6.1/CommandTool/CommandTool.mesa
STP_SERVE   Cedar6.1/Top/CommandTool.df (5309 bytes)
```

and the typescript says — **not** an error —

```
% OpenR CommandTool
Opened: CommandTool
```

with a viewer on `[Cedar]<Cedar6.1>Top>CommandTool.df!1`. 97,930 px.

That is the interesting part. `OpenCommand` asks the version map for
`CommandTool.*` and hands the candidate list to `TryExtensions`, which picks
the "best" suffix it can actually open. The `.mesa` retrieve fails against
the pruned tree, so it falls through to a candidate that *is* served — the
DF. **You get a viewer, a success message, and the wrong file.** No error is
printed anywhere, which is exactly the kind of thing that reads as "OpenR
didn't work".

The comment on that Makefile target explains the pruning honestly — the full
tree "would push `index.data` past GitHub's 100 MB per-file limit" — and it
is right: `index.data` is **97.8 MiB of a 100 MiB ceiling** today. See
Phase 4.

Note also that the version map itself is never retrieved in either run: it is
already resident in the desktop checkpoint. That is why short-name lookup
keeps working even when `Cedar6.1/VersionMap/` is not served at all.

### 1.3 `ls` on a remote directory silently returns nothing

```
% List [Cedar]<Cedar6.1>VersionMap>*
%
```

No files. No error. Just a new prompt. The wire trace shows exactly why:

```
FTP_CMD mark=0o14 len=148
STP_MISSING ((User-Name Guest.pa)(Directory Cedar6.1)(Name-Body VersionMap>*)
             (Desired-property Directory)(Desired-property Name-Body)
             (Desired-property Version))
   → markNo (0o4)
```

Cedar sent a correct **New-Directory** request. Everything upstream of the
server is already right — the name parsed, `[Cedar]` resolved to us, the
directory and pattern arrived intact. Our server treated it as a single-file
Retrieve of the literal path `Cedar6.1/VersionMap/*`, found nothing, and
answered `markNo`/`fileNotFound`.

The silence (rather than an error) is correct Cedar behaviour, not a second
bug: `FSRemoteFileImpl.InnerEnumerate` swallows `noSuchFile` and exits via
`EXITS NotFound => {}`. **A missing directory and an empty directory are
indistinguishable to the user.**

Three concrete defects, all in `dorado/src/ethernet.c`:

- `markDirectory` (**0o12**) is not in the mark enum at all, so the old-style
  Enumerate falls through `default: break;` and gets **no reply whatsoever** —
  worse than a refusal; the client waits.
- `markNewDirectory` (0o14) is aliased onto `FTP_MARK_RETRIEVE` in
  `eth_ftp_handle_command` — one file, no pattern.
- `eth_ftp_resolve_file` rejects `*` outright (its character filter admits
  only alphanumerics, `.`, `_`, `-`, `/`, `!`), so a wildcard can never
  resolve even in principle.

---

## 2. How Cedar's remote file access actually works

Reference section, grounded in the original sources. Everything cited is in
the repository; nothing here is inferred from behaviour alone.

### 2.1 Names

```
[Server]<Directory>SubDir>Name.ext!Version
/Server/Directory/SubDir/Name.ext!Version     -- equivalent slash form
```

`List` accepts both (`CommandToolCommands.tioga`: "List works with both /
and [ style filenames"). `!H` means highest version, `!L` lowest, `!*` all.
`///` is the local volume root.

### 2.2 The stack

```
CommandTool List / OpenR
  → FS.EnumerateForNames / FS.EnumerateForInfo / FS.Open
    → FSRemoteFileImpl.InnerEnumerate            (FS/FSRemoteFileImpl.mesa)
      → STP.Enumerate / STP.Retrieve             (stp/client-6.1/STPImpl.mesa!5)
        → PupStream (BSP) on PupWKS.ftp = socket 3
```

The server side of that same protocol is `stp/server-6.0/STPServerImpl.mesa!9`,
which listens on `PupTypes.ftpSoc`. Our implementation is
`dorado/src/ethernet.c`, reusing the existing Pup FTP socket-3/BSP framing —
correct for STP, not a shortcut.

### 2.3 Mark bytes (`stp/client-6.0/STPOps.mesa!2`, octal)

| mark | name | ours |
|---|---|---|
| 1 | `markRetrieve` | yes |
| 2 | `markStore` | no |
| 3 | `markYes` | yes |
| 4 | `markNo` | yes |
| 5 | `markHereIsFile` | yes |
| 6 | `markEOC` | yes |
| 7 | `markComment` | no |
| 10 | `markIAmVersion` | yes |
| 11 | `markNewStore` | no |
| **12** | **`markDirectory`** (Enumerate) | **no — not decoded** |
| 13 | `markHereIsPList` | yes (send only) |
| 14 | `markNewDirectory` / `markYouAreUser` | aliased to Retrieve |
| 15 | `markAbort` | no |
| 16 | `markDelete` | no |
| 17 | `markRename` | no |

### 2.4 The property list

A command string is a Lisp-ish list parsed by `STPServerImpl.ParseLisp`:

```
((User-Name Guest.pa)(Directory Cedar6.1)(Name-Body VersionMap>*)
 (Desired-property Directory)(Desired-property Name-Body)(Desired-property Version))
```

Keys the server reads (`GetUserProperties`): `User-Name`, `User-Password`,
`Directory`, `Name-Body`, `Version`, `Server-Name`, `Size`, `Create-Date`,
and repeated `Desired-property` entries. Note the client does **not** send
`Server-Name` in practice — the server identity is implicit in which server
you connected to. That matters for §4.3.

The reply plist (`SendPropList`) always forces `Server-Filename`,
`Directory`, `Name-Body`, `Version` and `Byte-Size` on, whatever the client
asked for, and adds `Creation-Date` and `Size` when desired.

### 2.5 The enumeration wire shape — exact

This is the part to get right. From `STPServerImpl.DoFiles`:

```
IF (first AND cs.mark = markNewDirectory) OR cs.mark = markDirectory
THEN SendMark[markHereIsPList];
action[...];              -- SendPropList writes "(...)": no mark of its own
first _ FALSE;
```

and at the end, `IF matches THEN Finish[stream, ""]` (which is
`SendMark[markEOC]`), else `No[stream, fileNotFound, "File not found"]`.

So:

- **New-Directory (0o14):** `markHereIsPList` **once**, then one bare plist
  per matching file, then `markEOC`.
- **Old Directory (0o12):** `markHereIsPList` before **every** entry, then
  its plist, then `markEOC`.
- **No matches:** `markNo`, the reply code character, the explanation string,
  `markEOC`.

The client half confirms it (`STPImpl.TryNewDirectory`): on `hereIsPList` it
loops calling `GetPList` — each successive plist is another entry — until it
sees `eoc`. On `no` it inspects the code and, for `protocolError`,
`requestRefused` or `badCommand`, **falls back to the old Directory form**.
That fallback is why implementing 0o12 as well as 0o14 is worth the ten extra
lines: an old-style client, or a new one whose New-Directory was refused,
will retry with it.

### 2.6 Versions and dates

`SendPropList` sends the real version from the expanded file name, and
`Creation-Date` from the file. Ours hardcodes `(Version 1)` and takes the
date from a DF index (`ftp_dates_scan_df`), which currently **parses
`name!version` and then discards the version** — `nlen` stops at the `!`.
The DFs therefore already hold the real version numbers we need; we are
throwing them away. 2,667 of 2,701 served files (98.7%) have a DF date.

Dates matter beyond cosmetics: `OpenR` calls
`FS.Open[name: name, wantedCreatedTime: date]` with the date **from the
version map**, and `BringOver` skips files whose advertised date differs.

### 2.7 Pseudo-servers (`PSAdd`)

`FSPseudoServers` turns one server name into a list of real ones —
"a simple, but effective, form of file replication for read-only files …
especially intended to be used for the Cedar release". `TranslateForRead`
returns a list, `TranslateForWrite` a single name, `AvoidRemoteCheck` says
the files are frozen so don't re-check. `InnerEnumerate` walks the whole
read list. Exposed as `PseudoServerAdd` / `PSAdd` in the CommandTool.

This is the sanctioned mechanism for pointing a running Cedar at a different
release host, and it is the cheapest way to let a visitor say
`[Anything]<Cedar6.1>` and reach our tree.

### 2.8 VersionMap — what `OpenR` and `FindR` actually use

`OpenR` does **not** enumerate. `OtherCommandsImpl.OpenCommand` →
`FindSource[shortName]` → `VersionMapDefaults.GetMapList[$Source]` →
`VersionMap.ShortNameToRanges`. Only then does it `FS.Open` the winner and
`TiogaMenuOps.Open` a viewer.

The map is fetched by name, from `VersionMapDefaultsImpl`:

```
defaultSourceName ← DefaultRemoteNames.Get[].current + "VersionMap>CedarSource.VersionMap"
```

and `DefaultRemoteNamesImpl` hardcodes `current: "[Cedar]<Cedar6.1>"` in the
6.1 build. So the file is
`[Cedar]<Cedar6.1>VersionMap>CedarSource.VersionMap` — which we have, at
66,296 bytes. The profile keys `VersionMap.SourceMaps` and
`VersionMap.SymbolsMaps` add further maps.

If the map list is empty, `OpenR` prints
`Sorry, 'X' is not in the current Cedar release.` — a *different* message
from `Not found: X`, which is what you get when the map resolves the name but
the server cannot supply the file. **The two messages tell you which half
failed.** Worth telling the author.

Both maps we serve contain what is wanted:
`Documentation>TiogaDoc.tioga!1` and `CommandTool>CommandTool.mesa!1` are
both in `CedarSource.VersionMap`.

---

## 3. Plan

Ordered so that each phase is independently useful and independently gated.

### Phase 1 — Enumerate: make `ls` work (the main event)

**Goal:** `List [Cedar]<Cedar6.1>*`, `List [Cedar]<Cedar6.1>Documentation>*`
and `List [Cedar]<Cedar6.1>CommandTool>*.mesa` print real listings.

Changes, all in `dorado/src/ethernet.c`:

1. Add `FTP_MARK_DIRECTORY = 012` to the mark enum and give it and
   `FTP_MARK_NEW_DIRECTORY` their own case, split from `FTP_MARK_RETRIEVE`.
2. Factor the plist parse out of `eth_ftp_resolve_file` so `Directory`,
   `Name-Body`, `Version` and the `Desired-property` set are available
   without also demanding that the result name a single existing file.
3. Add a pattern matcher over the served tree. `Name-Body` may contain `*`
   at any level (`VersionMap>*`, `*.mesa`, `Top>*.df`). Cedar's `*` matches
   `>` unless the `X` switch is given, so the default is a **recursive**
   walk; keep a depth cap and a match cap.
4. Emit the framing in §2.5 exactly. Reuse the existing plist builder for
   each entry; it already emits the five forced properties.
5. Answer `markNo` + `fileNotFound` only when there are genuinely no matches.

**Gate:** `make verify-cedar-ls` — paste `List [Cedar]<Cedar6.1>VersionMap>*`,
require the four known names in the typescript. Validate against its own bug:
it must fail on `49332c3`, where the same command prints nothing.

**Risk:** low and well-contained. Enumeration is a new code path; Retrieve is
untouched, so `verify-cedar-desktop` and every BringOver keep their meaning.

### Phase 2 — Real versions and dates

**Goal:** listings show truthful `!version`, size and date, and
`List -f`/`-d`/`!H` behave.

Keep the version alongside the date in `ftp_date_add` (the parser already
walks past the `!`), and use it for `(Version N)` and for `!H` / `!L` / `!*`
resolution. Where no DF names the file, keep serving `!1` and the synthetic
date — that is already the documented fallback.

**Gate:** extend `verify-cedar-ls` to require `CommandToolImpl.mesa!2`
(a file whose DF version is deliberately not 1).

### Phase 3 — More than one server name

**Goal:** `[Cedar]<Cedar6.1>` and, say, `[Ivy]<Guest>` are different places.

Today every Pup name lookup is answered with the same STP address
(`ethernet.c`, the `NETDIR_LOOKUP` handler) and the server ignores the host
entirely, keying only on `Directory`. That is why `[fileservername]<Cedar6.1>`
already reaches us — and also why every server name is the same server.

Two options, in increasing cost:

- **(a) Do nothing structural, document it.** Any name works; the directory
  selects the tree. Combined with `PSAdd` this is probably enough for a
  visitor. Zero code.
- **(b) Per-server roots.** Give the connection its resolved host name (we
  already see the name in the NetDir lookup) and map `<host, directory>` →
  a subtree of `--ftp-root`. This is what a second, writable `[Ivy]`-style
  user volume would need.

Recommend (a) now, (b) only if someone wants a writable user host.

### Phase 4 — Ship the release to the browser

This is what actually blocks the author's `OpenR` in the deployed page, and
it is a packaging problem, not an emulator one.

Numbers: full `stp-root` is 37 MB / 2,701 files; `Cedar6.1` alone is 25 MB
across 70 directories, of which `.bcd` is 12.3 MB and `.mesa` 10.5 MB.
`index.data` is already 97.8 MiB against a 100 MiB ceiling — about 2.2 MiB of
headroom. Preloading the release as-is is not possible.

The set `OpenR`/`ls`/`FindR` actually need is the **sources**, not the
binaries: `.mesa` + `.tioga` + `.df` + `.cm` + `.config` ≈ 12.2 MB raw, and
**2.9 MiB gzipped**. Options:

- **(a) Fetch on demand over HTTP.** Keep it out of `index.data`; have the
  in-process STP server fall back to an `XMLHttpRequest`/`fetch` against a
  static path when a file is not in MEMFS. Highest fidelity per byte, needs
  an async path in a synchronous server — the hard part.
- **(b) Ship a second, lazily-loaded gzipped bundle.** ~3 MiB, fetched when
  the Cedar world is chosen. Simple, no async plumbing inside the server.
- **(c) Ship sources only, drop unreferenced `.bcd`.** Needs care: the
  interpreter demand-fetches `Cedar6.1/Viewers/*.bcd`, which is exactly why
  that directory is already in the pruned set.

Recommend **(b)**, with `VersionMap` and `Documentation` in the first cut —
those two alone make `OpenR TiogaDoc` work in the browser.

### Phase 5 — Polish worth having

- `markComment` (0o7) — accept and ignore; trivial, and the client may send it.
- Reply-code fidelity: distinguish `fileNotFound` from `accessDenied` /
  `badCommand`, since the client's fallback logic branches on it (§2.5).
- `PSAdd` documentation in `docs/running-the-emulator.md` so a visitor can
  repoint the release host from inside the running Cedar.

---

## 4. What to tell the Cedar author

- `OpenR` works, including `OpenR TiogaDoc` — but only on the native build
  serving the full tree (`make run-cedar-work`). In the browser the release
  directory holding the source is not served, and `OpenR` then quietly opens
  a *different* file that is (for `CommandTool`, the `.df`) and still reports
  `Opened:`. Ask which they were running; if it was the web page, that alone
  explains it.
- The failure messages are diagnostic when you do get one:
  `Sorry, 'X' is not in the current Cedar release.` = the **version map**
  didn't resolve it; `Not found: X` = the map resolved it and no candidate
  could be opened. Silence plus the wrong content = a partial tree.
- `ls` against a remote directory returns nothing because our STP server has
  no Enumerate. That is Phase 1 and it is a contained change.
- Worth asking them: which host names they would expect to work besides
  `[Cedar]`; whether `[Ivy]`-style writable user directories matter for what
  they want to demo; and whether `FindR` (which needs no viewer) would be a
  useful early check.

---

## 5. Traps

- **`markDirectory` (0o12) currently gets no reply at all**, not a refusal.
  When adding it, make sure every path answers something — a hung client is
  harder to diagnose than a refused one.
- **Cedar's `*` crosses `>` by default.** `List [Cedar]<Cedar6.1>*` is a
  recursive walk of 2,701 files unless `-X` is given. Cap it.
- **Silence is ambiguous.** Because `InnerEnumerate` swallows `noSuchFile`,
  a gate that only checks "no error" will pass on a server that returns
  nothing. Gate on the **names printed**, with a large margin.
- **`Opened:` does not mean the right file was opened.** `TryExtensions`
  falls through to whatever candidate it can fetch, so a partial tree turns a
  missing source into a silently substituted DF. Any `OpenR` gate must assert
  the **viewer's title**, not that the command succeeded. This one nearly
  went into this document as "the browser prints Not found" — it does not;
  measuring it is what caught the difference.
- **Text files served to Cedar must be CR-terminated**, and `Bringover -p`
  fetches public files only. Both failure modes look identical and silent
  ("1 files acted upon"). Already documented in
  `docs/running-the-emulator.md`; it bites again whenever the tree is edited.
- **Don't read a criterion off a log the run is still writing.** These probes
  take ~8 minutes of wall clock each; the FTP trace goes quiet long before
  the run ends.

---

## 6. Files worth reading first

| what | where |
|---|---|
| Historical STP server — the specification | `chm/cedar/stp/server-6.0/STPServerImpl.mesa!9` |
| Mark byte values | `chm/cedar/stp/client-6.0/STPOps.mesa!2` |
| Client enumerate + fallback | `chm/cedar/stp/client-6.1/STPImpl.mesa!5` |
| FS → STP bridge | `chm/cedar/stp-root/Cedar6.1/FS/FSRemoteFileImpl.mesa` |
| Pseudo-servers (`PSAdd`) | `chm/cedar/stp-root/Cedar6.1/FS/FSPseudoServers.mesa` |
| `OpenR` / `FindR` | `chm/cedar/stp-root/Cedar6.1/CommandTool/OtherCommandsImpl.mesa` |
| Version map lookup | `chm/cedar/stp-root/Cedar6.1/VersionMap/VersionMapDefaultsImpl.mesa` |
| Release root name | `chm/cedar/stp-root/Cedar6.1/UserCredentials/DefaultRemoteNamesImpl.mesa` |
| Command reference | `chm/cedar/cedar6.1-docs/CommandToolCommands.tioga!1` |
| Our server | `dorado/src/ethernet.c` (`eth_ftp_*`) |
| Why STP over socket 3 | `chm/cedar/stp/README.md` |
| LookupFile protocol | `chm/cedar/stp-root/Cedar6.1/FS/FSFileLookupImpl.mesa` |

---

## 7. What was built, and the one thing this plan missed

Implemented 2026-07-30, all in `dorado/src/ethernet.c`.

### 7.1 Phase 1 — Enumerate

`FTP_MARK_DIRECTORY` (0o12) and `FTP_MARK_NEW_DIRECTORY` (0o14) each have
their own case now, split from Retrieve, and answer with the §2.5 framing
exactly: HereIsPList once for New-Directory and before every entry for the old
Directory, one bare plist per match, then a bare EOC; `markNo` +
`fileNotFound` only when nothing matched. The pattern matcher walks the served
tree with `*` crossing `>` (capped at 512 matches and 8 levels, and it *says
so* in the trace when it truncates). Enumeration state is file-scope static —
a new `dorado_ethernet` member would change the snapshot ABI and every baked
checkpoint would fail to restore — with the per-connection cursor kept in
`ftp_file_pos`, which the context switch already saves.

A listing has no file behind it to act as a retransmit ring, so each entry
records its byte offset as it goes out and a duplicate ack rewinds to the
entry that starts there (the same job the file itself does during a retrieve).

### 7.2 Phase 2 — Real versions and dates

`ftp_date_add` keeps the DF's `!n` beside its date, and both the listing and
the retrieve plist report it. Nothing had to be invented for `!H`/`!L`/`!*`:
the tree holds one version of each file, so all of them select it.

### 7.3 The missing piece: LookupFile (`PupType.fileLookup`, 0o200)

**The first end-to-end listing printed all eleven names and then junk**:
`MakeVersionMap.cm!257  32 ??` — a wrong version, a wrong size, and `??` for
every date. Our plists carried the truth; `List` was not reading it from
there.

`FSRemoteFileImpl.Info` tries a **single-Pup exchange first** and only falls
back to an STP enumerate if nothing answers — "This procedure uses the
LookupFile packet exchange protocol to obtain the version number, create time,
and byte length of a file on a remote file server" (`FSRemoteFile.mesa`).
Unanswered, `FSFileLookupImpl` retries four times and then caches
`noResponse` against the whole server for 30 seconds, so every remote name in
a listing prints `??` and a stale number.

The exchange is small: request body is the file name in bracket syntax laid
straight into the Pup (`PupSocket.CopyRope` — no count prefix); the reply is
type 0o201 carrying `PupBuffer.FileLookupReply` = version (1 word), createTime
(2 words, seconds since the 1901 Pup epoch), length (2 words); 0o202 is
`fileLookupError` = noSuchFile.

Note **which socket**: `FSFileLookupImpl` asks the name server for the address
of the server at `PupWKS.fileLookup` (0o61), and our NetDir handler answers
every lookup with the STP address — socket 3 — so that is where the client
sends it. It is therefore answered on whatever socket it arrives at, keyed on
the Pup type. That is §3's "every server name is the same server" showing up
one layer down.

With it served, the guest asks us about each name it just received, and the
listing is completely correct: `CedarSource.VersionMap!34  66296 04-Dec-86
13:05:32 EST` (our `10:05:32 PST`, converted by the guest), and the summary
line totals the real bytes.

### 7.4 Gates

- `make test` — `test_ethernet` now plays the client half of
  `STPImpl.TryNewDirectory`/`DoFiles` against the in-process server over real
  BSP: New-Directory and old-Directory framing, level-crossing `*`, exact
  names, `fileNotFound` on no match, DF versions, and the LookupFile reply
  word-for-word (its createTime is asserted against a constant computed
  independently). Milliseconds, and it **fails on `49332c3`** — where the same
  request draws `markNo`/"File not found".
- `make verify-cedar-ls` — the real desktop, the real client, one paste.
  Reads **both ends of the wire** out of the trace: `STP_ENUM` (we produced
  entries) and `STP_LOOKUP` (the guest named those files back to us, which it
  can only do having received them). `make verify-cedar-ls-selftest` checks
  the gate against a broken-server log and against a server-half-only log.
- Deliberately **not** a pixel count. The trap in §5 is real but understated:
  the CommandTool typescript is already full, so a listing *scrolls* rather
  than adding ink — 167,129 px with the eleven-file listing on screen against
  167,192 px for a listing of a directory that does not exist. The count moves
  less than the length of the echoed command.
- `make verify-cedar-desktop` — 245,711 px (was 246,086). The small change is
  expected: `Moon.cm`'s own transcript prints file names, and their version
  numbers are now the DFs' rather than a flat `!1`.

### 7.5 Phase 4 — the release in the browser (option (b), as recommended)

**The ceiling in §4 was not real.** `dorado/web/` is gitignored: `make web`
runs in CI and the result is uploaded with `actions/upload-pages-artifact`, so
`index.data` never enters git and git's 100 MB per-file limit — the one that
comment cites — cannot be what constrains it. Measured with the toolchain that
turned out to be installed all along (`~/emsdk`, emcc 6.0.0, the version the
workflow pins): `index.data` is **102,565,493 bytes = 97.81 MiB**, exactly §4's
figure. The real constraint is simply that Emscripten preloads it in full
before the page boots, so anything added lands on every visitor — including
one who came to play Galaxian.

So: option **(b)**. `make web-cedar-src` builds `web/cedar-src.tar.gz` beside
`index.data` — never inside it — and `web_shell.html` fetches it in the
background when a Cedar world is chosen, unpacking it into MEMFS at `/stp`
where the in-process STP server looks. A tar is the one archive a browser can
walk with ~30 lines of JS and no library; gzip comes free from
`DecompressionStream`, which the shell already used for the snapshots. A
failed fetch is not fatal — the world boots without it and the next boot
retries.

Contents are a **deny-list**, not an allow-list of source extensions:
everything except `.bcd` (12.3 MB of compiled code the desktop checkpoint
already carries installed) and the two directories already preloaded. The
first cut listed `.mesa/.tioga/.df/.cm/.config` and silently dropped **both
version maps** — `CedarSource.VersionMap` is the file `OpenR` resolves short
names through — plus the Tioga `.style` files and every capitalised variant
(`.Mesa`, `.Tioga`, `.Profile`), because `find -name` is case-sensitive. Only
the browser probe caught it. Final: **963 files, 62 of the release's 70
directories, 3,072,063 bytes (2.93 MiB)**; the 6 uncovered directories hold a
single `.bcd` and nothing else.

Verified in Chrome against the built page: `/stp/Cedar6.1` goes from **2
directories to 64**, `OpenR TiogaDoc` opens a viewer titled
`[Cedar]<Cedar6.1>Documentation>TiogaDoc.tioga!1` (the title, per §5's trap —
not just the `Opened:` line), and `List [Cedar]<Cedar6.1>VersionMap>*` prints
7 files with real versions and dates. Seven, not the native eleven: the four
`.bcd` files in that directory are deliberately not shipped.

### 7.6 Not done

- **Phase 5's `markComment`** — the existing `default:` already ignores an
  unknown mark without replying, which is the correct handling; an explicit
  case would add a line and change nothing.
- **Store, Delete, Rename** (marks 0o2, 0o11, 0o16, 0o17) — still refused.
  The server is read-only, which is what a release host is.
