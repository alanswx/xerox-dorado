# Running the emulator — software combinations and how to load them

A practical runbook: the software we have, which combinations work, and the
exact commands to load each. For the *survey* of what exists in the CHM
archive see `docs/dorado-software.md`; for the live bring-up state see
`docs/CONTINUE-HERE.md`.

All commands are run from the `dorado/` directory after `make` (headless
binary `build/dorado`) or `make sdl` (interactive SDL binary
`build/dorado-sdl`). Paths beginning `../chm/...` are relative to `dorado/`.

---

## The two boot paths at a glance

The boot chain is the same up to the point where emulator microcode is
running; then it forks:

```
BaseBoard ROM ─► Bootstrap ─► Initial ─► [microcode world] ─► [OS / app]
 (6502)          (IM 7700+)   (loads     ┌─ AEmu world ──► Alto sw boot (EFTP)      ◄── WORKS
                              the world)  └─ Cedar world ─► Pilot germ ─► OS boot   ◄── WORKS
```

| Path | Microcode world | Second stage | Status |
|---|---|---|---|
| **A. Alto-on-Dorado** | `worlds/aemu.eb` (AEmu) | Alto boot file over EFTP | **Working** — Galaxian 121553 px, NetExec menu renders |
| **B. Cedar/Pilot germ** | `CedarDorado.eb` | `Dorado.germ` → Pilot disk | **Boots Cedar 6.1 all the way to the Viewers desktop** (2026-07-15): Guest login → LoaderDriver install over the in-process STP server → live Clock + CommandTool |
| **C. Alto/Mesa** | `AltoMesaDorado.eb!2` | `MesaNetExec.boot` over EFTP | **Boots the Mesa Network Executive to an interactive `>` prompt** — herald + live clock + cursor; typed keys echo (the prompt appears ~155M cycles in) (`./build/dorado-sdl --eb '../chm/dorado/AltoMesaDorado.eb!2' --eftp '../chm/bootfiles/MesaNetExec.boot!1'`) |

Both stages share an in-process fake Pup/EFTP boot server (`src/ethernet.c`)
so no real network is needed: `--eftp PATH` registers the boot file the
server streams when the running software asks for it.

---

## Software inventory (what we have locally)

### Microcode worlds (`--eb`)
| File | What it is | Use |
|---|---|---|
| `dorado/worlds/aemu.eb` | The **Alto emulator** on Dorado (built locally from `chm/dorado/AEmu.mb!2` by `make worlds`) | Path A — runs Alto software |
| `chm/dorado/AltoMesaDorado.eb!2` | Alto/Mesa world (1984) | Path A variant; a regression-gate world |
| `chm/dorado/CedarDorado.eb!6` | **Cedar/Mesa microcode** (17-May-1984; the build shipped with Cedar 5.3/6.0/6.1) | Path B — runs the Pilot germ |
| `chm/dorado-mc-alt/CedarDorado.eb-1986.3` | Cedar microcode, May-1986 | Path B (newer; behaves identically on the germ so far) |
| `chm/dorado-mc-alt/CedarDorado.eb-1987.4` | Cedar microcode, Feb-1987 | Path B (newer) |
| `chm/dorado/Initial*.eb` | Bootstrap (Initial) microcode stages | Used internally by the full-boot chain |

### Pilot germs (`--germ`, Path B only)
The germ is the Pilot bootstrap that the Cedar microcode loads into VM. It
then loads the actual OS boot file. **Germ and microcode versions must
match.**

| File | Version | Status |
|---|---|---|
| `chm/cedar/germ/Dorado.germ!4` | Dec 1983 (Cedar ~5) | **Mismatched** with the 1984 microcode → faults early (`0o27132`) |
| `chm/cedar/germ-alt/Dorado.germ-6.1.6` | **Cedar 6.1, 1986** | **The working germ** — the matched pair with `CedarDorado.eb!6`; boots Cedar to the desktop |
| `chm/cedar/germ-alt/Dorado.germ-6.0.6` | Cedar 6.0, 1985 | Behaves like !4 (same early fault) |
| `chm/cedar/germ-alt/Dorado10.germ-6.1.5` | Cedar 6.1 (Dorado10) | Same early-fault family |
| `chm/cedar/germ-alt/Dorado.germ-5.3.1` | Cedar 5.3, 1984 | Same early-fault family |

### OS / application boot files (`--eftp`)
Streamed by the EFTP server as the "next stage." Two families:

**Alto boot files (Path A — these WORK):** `chm/bootfiles/*.boot` and
`*.BOOT` — `NETEXEC.BOOT!8` (the network executive menu), games
(`Galaxian.boot!1`, `Invaders.boot!1`, `MissileCommand.boot!1`,
`PinBall.boot!1`, `AstroRoids.boot!1`, `Boggs.boot!1`, `Reversi.boot!1`, …),
and tools (`CRTTEST.BOOT!1`, `DMT.BOOT!22`, `KeyTest.boot!1`,
`MadTest.boot!1`, `FTP.boot!1`, `EtherWatch.boot!1`, …).

Three caveats, all reproducing real-Dorado behavior (June 2026 findings):

- **Load time.** Boot + the lock-step EFTP transfer cost ~1M cycles per
  512-byte packet, so a game's own code only starts after ~80M cycles
  (small file) to ~190M (Trek-sized). A headless `--cycles 100000000` run
  of a big boot file snapshots the *loader*, not the program.
- **Blank start screens.** `AstroRoids` and `Invaders` come up waiting for
  a keystroke on an essentially blank screen (their FullBootInit display
  stream is empty); type any key and the playfield paints. `Reversi` is a
  text-prompt UI.
- **Games that need Alto RAM microcode never run:** `Pool.boot!1`
  (`JMPRAM`), `StarWars.boot!1` and `Trek.boot!1` (custom `70000B`-family
  opcodes). They `WRTRAM` a custom Alto microcode image and jump into it.
  The Dorado has no Alto control RAM and AEmu makes no pretense of one
  (`chm/doradosource/AEmuSources-cedar6.0.dm!1_/ATraps.mc`: `WRTRAM`
  no-ops, `RDRAM` returns 0, `JMPRAM` and all of `70000B..77777B` branch
  to the trap microcode), so they trap into Swat and park in TeleSwat's
  receive loop — identical to real hardware. `MissileCommand.boot!1`
  paints its attract screen first, then dies the same way on its first
  `70000B` opcode. Diagnostic: the machine-debug line `TRAPPC=` (Alto
  `M[527B]`) is nonzero and the world idles on Pup socket `60B`
  (TeleSwat), acking the fake server's probes with type-`204B` Pups.

**Cedar/Pilot boot files (Path B net-boot alternative):**
`chm/bootfiles/CedarNetExec.boot!4`, `MesaNetExec.boot!1`, `NEWOS.BOOT!21`,
and `chm/cedar/germ/OthelloDorado.boot!8` (Othello, the Pilot disk/volume
utility). The EFTP/Mayday server serves these byte-exact; they are what the
germ would request via `DoInLoad` if driven to net-boot. The working Cedar
path boots from the PDI disk instead.

### Pilot disk images (`--pilot-disk`, Path B)

Gzipped in `CedarDisk/` and rehydrated by make on demand:

| File | What it is |
|---|---|
| `CedarDisk/CedarDorado-boot.pdi` | The minimal boot volume — reaches the SimpleTerminal login prompt (`make run-cedar`) |
| `CedarDisk/CedarDorado-work.pdi` | The working volume the **desktop install** runs on — log in as Guest and LoaderDriver installs Basic.Loadees/BootEssentials/fonts from `--ftp-root` (`make run-cedar-work`) |
| `CedarDisk/CedarDorado-bestof.pdi` | Corrected kitchen-sink volume with 2,240 recovered files (`make run-cedar-bestof`) |

---

## How to load each combination

### Path C — Smalltalk-76 (2026-07-28)

The world is **DSemu** — the Alto emulator PLUS the Dorado's Smalltalk
microcode, ether-booted as BFN `111B` — and the medium is PARC's "XM
Smalltalk" pack. Smalltalk is started the period way, by the Alto
Executive's `Bootfrom`, exactly as `Smalltalk.midas!17` and
`DSemuRelease.cm` describe.

```
make run-smalltalk             # boot from scratch in SDL (~90 s to the desktop)
make run-smalltalk-snapshot-sdl  # restore the saved desktop instantly
make verify-smalltalk          # headless gate: >= 100000 px (measures 124945)
make smalltalk-desktop-snapshot  # (re)make the native checkpoint pair
make smalltalk-web-snapshot      # (re)make the browser checkpoint pair
```

It reaches **Top View**, the **Classes** browser with its four panes, and a
**UserView workspace** — the same screen ContrAlto reaches on the same pack
and command. Screenshot: `docs/images/smalltalk76-desktop-2026-07-28.png`.

By hand, the two flags that are easy to get wrong:

```
./build/dsk2trident --all-heads ../AltoInfo/ContrAlto2-beta/Disks/xmsmall.dsk /tmp/xmsmall.pack
./build/dorado --cycles 1950000000 --eb '../chm/microcode/SmalltalkDorado.eb!1' \
    --disk 0=/tmp/xmsmall.pack --boot-reason disk --no-alto-boot \
    --key-hold 3000000 --type-at 350000000 --type 'Bootfrom xmsmall.boot\n'
```

- **`--no-alto-boot` is required.** Without it the run silently falls back to
  Ethernet and a "renders from the pack" result is not what it looks like.
- **`--key-hold 3000000` is required.** At the 600K default the doubled `o`
  in `Bootfrom` collapses into one press (the Alto path applies keys with no
  pacing) and the Executive sees `Botfrom`.
- Rebuild the pack per run: the guest writes to it.

**View-only for now.** After the restore the interpreter keeps running and
the guest polls MOUSELOC (`0o424`/`0o425`), but a mouse click derails it:
the guest stores 0 to the display word task's bank register `0177751` from
Alto PC `0002270`, the rasteriser follows it back to bank 0, and the Nova
side drops into a 4-instruction spin with no further Smalltalk bytecodes.

**Testing a restore by hand:** `--cycles` is an ABSOLUTE target. A
`--snapshot-in` run needs a target beyond the checkpoint's cycle (1.95 B
here) or the emulator exits having executed nothing — which looks exactly
like a dead machine.

### Path A — Alto software on Dorado (working)

Headless (runs N cycles, writes a `dorado-screen.pgm` snapshot):
```
cd dorado
./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1' --cycles 160000000
```
Interactive (SDL window, keyboard/mouse) — the Makefile has a `run-<name>`
target per boot file:
```
make run-galaxian      # Galaxian
make run-netexec       # the NetExec boot menu
make run-invaders      # Space Invaders (type a key once loaded)
make run-missilecommand / run-pinball / run-astroids / run-reversi
make run-boggs / run-crttest / run-dmt / run-keytest / ...
# run-pool / run-starwars / run-trek load but trap into Swat: they need
# Alto RAM microcode, which no Dorado has (see the caveats above).
```
Any Alto boot file works by hand:
```
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/<NAME>.boot!N'
```
Booting the NetExec menu (`make run-netexec`) lets you pick from the served
directory interactively; pass `--boot-dir` / `--boot-dir-all` to control
what the directory server advertises.

### Path B — Cedar/Pilot germ (boots to the Viewers desktop)

Fastest first:

```sh
make run-cedar-desktop-sdl   # restore the saved Viewers-desktop checkpoint
make run-cedar-work          # full cold boot + install; log in as "Guest"
make run-cedar               # boot volume only, to the login prompt
```

`run-cedar-work` boots `CedarDorado-work.pdi` with
`--ftp-root ../chm/cedar/stp-root` (the in-process STP file server) and
`DORADO_PDI_IGNORE_LABEL_FLAGS=1` (masks the label DA-hint words that
synthetic PDI media store as 0). After the Guest login, LoaderDriver
installs the loadee set and the desktop comes up. `make
cedar-desktop-snapshot` regenerates the checkpoint headlessly (it replays
the whole ~31 G-cycle boot + install and saves the snapshot **together with
the mutated PDI** — a matched pair, preserved gzipped in
`dorado/snapshot-assets/`); `make cedar-desktop-web-snapshot` produces the
separate wasm32-ABI pair for the browser build. The colour variant is baked
with the same command-injection mechanism:

```sh
make cedar-color-web-snapshot \
     EMCC=/Users/alans/emsdk/upstream/emscripten/emcc
```

It installs and enables Cedar's `ColorDisplay` package, then writes the
wasm32 snapshot/PDI pair under `dorado/web-assets/`. After `make web`, choose
`Cedar 6.1 — ColorDisplay on (DispM)` in the browser world menu; the page's
Both / Color / Monochrome controls remain available.

Under the hood, use the **matched** pair (Cedar 6.1 germ + the Cedar
microcode) with the Pilot/Cedar PDI mounted as drive 0:
```
cd dorado
./build/dorado --boot-reason disk \
               --no-alto-boot \
               --eb '../chm/dorado/CedarDorado.eb!6' \
               --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
               --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' \
               --cycles 1200000000
```
This boots the BaseBoard→Bootstrap→Initial chain, Initial netboots the Cedar
world, the germ is planted into VM, the Pilot disk path runs, and Cedar boots
to its **SimpleTerminal login prompt** ("Please login ... / Name:"). The
headless run writes `dorado-screen.pgm` at exit.

**Keyboard works.** Type at the `Name:` prompt in `make run-cedar`; the live
keys are delivered to Cedar's KeyBits at absolute `LONG[177033B]` and a
display vertical-field naked-notify wakes its keyboard watcher (see
`machine_cedar_io` in `src/machine.c`). Headless self-test (echoes `abc` at
the prompt; the login appears around 640M cycles):

```
./build/dorado --boot-reason disk --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' \
  --type "abc" --type-at 655000000 --key-hold 4000000 \
  --cycles 700000000 --out /tmp/cedar-type.pgm
```

The emulator runs ~23.9M cycles/sec (~1.4x the real 16.67 MIPS Dorado) after
the per-step `getenv()` caching fix, so the login prompt appears in ~27s.

SDL command for the same path:
```
make run-cedar
```
The SDL window is blank until roughly 650M emulated cycles; the Makefile uses
`--speed 4000000` so pixels usually appear around frame 160. For a headless
SDL proof image:
```
make run-cedar-screenshot
```
which writes `/tmp/dorado-sdl-cedar-300.pgm`.

For an immediate native launch at the verified, keyboard-responsive login
prompt, use:

```sh
make cedar-login-snapshot       # one-time 650M-cycle producer
make run-cedar-snapshot-sdl     # subsequent launches restore immediately
```

The native checkpoint is also preserved in compressed form under
`dorado/snapshot-assets/`, so `make clean` can rehydrate it without repeating
the Cedar boot. The browser has a separate wasm32-native checkpoint under
`dorado/web-assets/`; selecting Cedar restores the login prompt instead of
replaying the boot.

The older `Dorado.germ!4` is the historical default but is version-mismatched
with the 1984 microcode; prefer `Dorado.germ-6.1.6`.

For the corrected software-filled Cedar volume, use:

```sh
make run-cedar-bestof                 # cold boot
make cedar-bestof-snapshot            # regenerate the 1.2B-cycle login checkpoint
make run-cedar-bestof-snapshot-sdl    # immediate interactive restore
```

`CedarDorado-bestof.pdi` was rebuilt with Cedar-compatible two-page file
headers and a 17-page VAM. It currently carries 2,240 files recovered from the
first historical kitchen-sink image, with named launchers including ChessHack,
Celtics, Football, Draw2d, Clock, ColorTool, Chat, FTP, FontEdit, IconEditor,
Interpress, Animation3D, and Clip3d. The compressed PDI is the reproducible
source for the gitignored raw image; the compressed native snapshot survives
`make clean`.

---

## Ready-to-run packs

Each of these opens from a saved checkpoint, so it appears in seconds
rather than replaying a multi-billion-cycle boot. The same set is in the
browser build's **Boot** dropdown.

| Pack | SDL | What you get |
|---|---|---|
| **Cedar desktop** | `make run-cedar-desktop-sdl` | The Viewers desktop with the welcome menu and the `Schematic`/`Moon`/`Memo`/`Ifu`/`Help` buttons. The one to start with. |
| **Cedar corpus** | `make run-cedar-corpus-sdl` | The same desktop, but its disk is the **recovered PARC file corpus** — `List ///*` shows real 1985 files. |
| **Cedar archive** | `make run-cedar-archive-sdl` | The desktop plus PressReader/Sil and the original Dorado press drawings. |
| **Cedar login** | `make run-cedar` | The bare SimpleTerminal login prompt, for watching a boot from the start. |
| **Interlisp-D Lyric** | `make run-lisp-lispusers-sdl` | The Lyric Exec (XCL) desktop with 204 library packages on disk and a file server behind `{DORADO}` — type `(IL:FILESLOAD HELPSYS)` then `(IL:LOAD '{DORADO}<IRM>IRMDEMO)` (login `Guest`/`Guest`) for the online Reference Manual. |
| **Alto games / NetExec** | `make run-galaxian`, `make run-netexec`, … | Alto software through the Dorado's Alto-emulator microcode. |

Cold boots, if you want to watch the whole thing: `make run-cedar-work`
(the desktop install, ~31 G cycles) and `make run-cedar-corpus` (the
corpus volume, ~33 G).

### Rebuilding the corpus volume

`tools/make_corpus_volume.sh` builds it from the recovered corpus in one
command, and documents why each step is needed. In short: repack from
`CedarDorado-bestof.pdi` (the kitchensink images use one-page file
headers, which Cedar 6.1 cannot read), cap the file count so the volume
keeps free space (Cedar cannot even complete a lookup on a full volume),
and rewrite the PV-root boot links to the flat convention the emulator's
germ path expects.

**Known limitation:** only ~25 of 1,200 files keep their names, and a
long `List` eventually stops with `FSReportImpl $badFP`. `cedar_repack`
copies file content correctly but does not rewrite the name-directory's
`File.FP`s to the new placement — that fix is the next job, tracked in
`docs/handoff.md`.

---

## The friendly desktop: type a word, or click a button

The desktop checkpoint boots with a small set of **command files** already
fetched and **menu buttons** already created, so a first-time visitor never
has to type a line of Mesa. At the CommandTool's `%` prompt (click the
prompt line first for type-in focus) type any of:

| Type this | What happens |
|---|---|
| `Welcome.cm` | prints the menu of everything below |
| `Schematic.cm` | the Dorado displays **its own** ProcH bit-slice drawing |
| `Moon.cm` | a 1978 Lick Observatory moon photograph, halftoned |
| `Ifu.cm` | the IFU board drawing |
| `Memo.cm` | Ed Taft's 1980 "Dorado Booting" memo, formatted in Tioga |
| `Images.cm` | fetches the picture collection and lists it |
| `Color.cm` | turns on the colour display and loads Gargoyle's packages |
| `Gargoyle.cm` | turns on colour, loads Gargoyle, and opens it |
| `Browse.cm` | lists the files on this disk (click a name, then Open) |
| `Source Show.cm <name>.ais` | displays any picture by name |

The same actions are **buttons in the CommandTool menu line** (next to
`STOP!` / `Find` / `Split`) — click one with the red (left) mouse button.
Note that clicking a *button* does not move the type-in focus, so if you
want to type afterwards, click the `%` prompt line again first. (This
also bites scripted runs: a `--click` on a button followed by
`--paste-at` sends the keystrokes nowhere.)

Pictures take real time to appear: the Imager is doing genuine 1985
halftone work, and the schematics are 500x644. The moon is the quick one.

### How this works — Cedar's own mechanisms (not emulator features)

Everything above is period-authentic, lifted from Xerox's own sample
configurations (`chm/cedar/cedar6.1-docs/StandardUser.Profile` and
`StandardUser-LoginWork.cm`, both by Mike Spreitzer, 1987):

- **`.cm` command files** are batch scripts of CommandTool commands. Typing
  the file name runs it; `Command <file>` and `Source <file>` do the same,
  and `Source` additionally passes arguments as `$1`, `$2`, ... and lets a
  `cd` inside persist. Ours live in
  `chm/cedar/stp-root/CedarChest6.1/DoradoWelcome/` and are published by
  `DoradoWelcome.df`.
- **`CreateButton <label> <command> [args]`** adds a clickable button to the
  CommandTool menu; `ClearMenu` empties it first. (`CreateButton Moon
  Moon.cm` verified live, 2026-07-18.)
- **`Alias <name> [(args)] <expansion>`** defines a one-word command, e.g.
  Spreitzer's `Alias BRCM (pkg) pushr Commands; Bringover -mp pkg.DF; pop`.
- **`Install <Package> ...`** loads one or more packages in a single
  command — the idiomatic launcher, and shorter than a `Bringover` plus a
  `Run` per implementation module.
- **`.load` files** are a package's own launch manifest; the CedarChest
  catalog is built by enumerating them (`CedarChestDoc.tioga`).
- **Profile hook points**: `CommandTool.BootCommands` (first CommandTool
  after a full boot — where our setup lives), plus `.NewUser`, `.PerLogin`,
  `.PerCommandTool`, and `Debugger.CommandTool.BootCommands`. Each has a
  matching `///Commands/Note*` command that re-runs it.

The Gargoyle button is created by the same profile hooks as the other
welcome buttons. After changing `User.Profile`, use a cold boot so Cedar
rebuilds the first CommandTool, then click `Gargoyle`; a restored checkpoint
does not rerun `BootCommands`. The button runs `Gargoyle.cm`, which sources
`Color.cm` and then invokes the loaded `Gargoyle` command.
- **`&cmd["Name"]`** invokes a command from the interpreter, and a line
  beginning `_` (ASCII 0x5F = Cedar's `←`) *is* an interpreter line — the
  file-side spelling of the `Eval` we type.

**Correction to an earlier note in this file:** quoted strings inside a
profile value are fine — Xerox's own profiles are full of `\"` escapes
inside `CommandTool.BootCommands`. Our earlier profile corruption was a
formatting error of ours, not a Cedar limitation.

**The browser serves a PRUNED tree — test the web build against it.**
`make web` preloads `dorado/build/web-stp` at `/stp`, not the whole
`chm/cedar/stp-root` (the full `Cedar6.1` is 24 MB and index.data has to
stay under GitHub's 100 MB per-file limit). Cedar's interpreter
demand-fetches while evaluating, so anything missing there fails ONLY in
the browser: on 2026-07-19 `Moon.cm` worked in SDL and returned
`Error: UnknownError[sig: 10724B, msg: 17777B]` on the web, because
`Cedar6.1/Viewers/{ViewerOps,ButtonsImpl}.bcd` were absent. Reproduce
browser file behaviour natively — no browser needed — with

```sh
./build/dorado ... --ftp-root build/web-stp --snapshot-in ...
```

and watch for `STP_MISSING` lines under `DORADO_FTP_TRACE=1`; each one is
a file the deployed site will fail on. Add it to the `web-stp` target.

**Serving your own files** — two rules, each of which cost us a debugging
round on 2026-07-18, and which fail in the *same* silent way ("1 files acted
upon", meaning the `.df` itself and nothing else):

1. **Cedar text files use CR (`\r`) line endings, not LF.** A `.df` or `.cm`
   written with Unix newlines is one enormous line to Cedar's parser, so no
   entry is ever seen. Convert anything you author for the guest:
   `python3 -c "b=open(p,'rb').read().replace(b'\r\n',b'\n').replace(b'\n',b'\r'); open(p,'wb').write(b)"`
   (`od -c` on any period file in the tree shows the `\r`.)
2. **Fetch data files with a plain `Bringover`, not `Bringover -p`.**
   The `-p` flag means public files only. For a CODE package that is
   usually what you want (the exported `.bcd`s and its `.load`); for a
   package of DATA files (images, `.cm`, documents) nothing is public, so
   `-p` brings over just the `.df` and the payload never arrives.
   Rule of thumb: `-p` for code, plain for data, plain when unsure (it
   fetches the import closure too, which is slower but complete).

Otherwise: drop files in `chm/cedar/stp-root/CedarChest6.1/<Package>/` and
list them in a `<Package>.df` under `Top/`, following the layout of
`AISImages.df` or `DoradoWelcome.df`.

---

## Browsing the release: `ls`, `OpenR`, and other server names

The desktop can list and open files on the emulated release host.

```
% List [Cedar]<Cedar6.1>VersionMap>*
[Cedar]<Cedar6.1>VersionMap>
  CedarSource.VersionMap!34   66296  04-Dec-86 13:05:32 EST
  ...
-- 11 files, 199078 total bytes
% OpenR TiogaDoc
```

Screenshot: `docs/images/cedar-ls-versionmap-2026-07-30.png`. Both the
bracket form and the slash form work (`List /Cedar/Cedar6.1/VersionMap/*`),
and `*` crosses `>` by default — `List [Cedar]<Cedar6.1>*` walks the whole
release, so give it `-X` if you want one level. Our walk stops at 512 matches
and 8 levels and says so (`STP_ENUM TRUNCATED`) when it does.

**Any server name reaches us.** Every Pup name lookup is answered with the
same address, so `[Cedar]`, `[Ivy]` and `[fileservername]` are all this
server; what selects the tree is the DIRECTORY (`<Cedar6.1>`,
`<CedarChest6.1>`, `<Fonts>` — the top-level directories of `--ftp-root`).
To point a running Cedar's release host somewhere else, use Cedar's own
mechanism rather than an emulator flag:

```
% PSAdd Cedar Ivy          -- read [Cedar]<...> from [Ivy] as well
% PSAdd                    -- list the pseudo-server table
```

`FSPseudoServers` calls this "a simple, but effective, form of file
replication for read-only files ... especially intended to be used for the
Cedar release", and `InnerEnumerate` walks the whole read list, so a listing
spans every server in it.

**Two failure messages worth knowing**, because they say which half broke:
`Sorry, 'X' is not in the current Cedar release.` means the **version map**
did not resolve the name; `Not found: X` means it resolved and no candidate
could be fetched. And silence plus the *wrong* file means a partial tree —
`OpenR`'s `TryExtensions` falls through to whatever candidate it can open, so
against a pruned tree `OpenR CommandTool` opens `Top>CommandTool.df` and
still prints `Opened:`, with no error anywhere. Any check of `OpenR` must
therefore assert the **viewer's title**, not that the command succeeded.

**In the browser** the release arrives as a separate ~2.9 MiB bundle
(`web/cedar-src.tar.gz`, built by `make web-cedar-src`) which the page fetches
in the background when a Cedar world is chosen and unpacks into the emulated
file system — so it costs nothing for someone booting Galaxian. It carries
963 files across 62 of the release's 70 directories: everything except the
`.bcd` binaries the desktop checkpoint already has installed, which is why a
browser listing of a directory shows fewer files than the native one.
Screenshot: `docs/images/cedar-browser-ls-openr-2026-07-30.png`.

## Installing CedarChest applications onto the desktop

The desktop world installs software exactly the way PARC did: `Bringover`
reads a package's `.df` from the in-process STP server and fetches its
files, then `Run` starts it. The served tree is `chm/cedar/stp-root/`;
mirror any CedarChest6.1 package into it with

```sh
python3 tools/fetch_cedarchest_app.py 'ChessHack.df!3'   # name!version
```

(the version number comes from
`https://xeroxparcarchive.computerhistory.org/cyan/cedarchest6.1/top/.index.html`;
if a payload directory is missing on cyan, try the CedarChest6.0 directory —
CardTable's files survive only there, with identical creation dates).

Already mirrored: **AIS**, **AISViewer**, **ChessHack**, **CardTable**,
plus Clock and BootTool. In the running desktop (click the CommandTool
first for type-in focus):

```
Bringover [Cedar]<CedarChest6.1>Top>ChessHack
Run ChessHackImpl
ChessHack
```

**A package's run name is not always its DF name.** ChessHack ships
`ChessDefs.bcd`, `ChessHackImpl.bcd` and a `ChessHack.load` manifest —
there is no `ChessHack.bcd`, so `Run ChessHack` fails with "Could not
find ...ChessHack.bcd" no matter how it was fetched (an older note in
this file claiming that recipe worked was only ever verified as far as
the download). Look at the `.df` or the `.load` to see what a package
actually exports, and `Run` an implementation module. Same pattern as
AIS, where the working commands are `Run AISImpl` / `Run AISViewerImpl`.
`Run ChessHackImpl` then `ChessHack` is verified to load and start.

**ChessHack now WORKS — the board paints (2026-07-21).** It once raised
`ImagerImpl.Error[$fontNotFound, "Xerox/TiogaFonts/Chess40"]` because Cedar
has NO font catalog: `ImagerFont.Find` does
`FS.EnumerateForNames["///Fonts/Xerox/TiogaFonts/Chess40.*!h"]` over the
volume's FS name tree, and `ImagerTypefaceImpl` ERRORs unless the stored
name carries the `Xerox` component — a name fonts get ONLY from the
installer's cold-boot attach of the font DFs (not from a served file, not
from a runtime `Bringover`, not from rusty-backup injection — the latter two
crash the world). **Fix:** one CR-terminated line was added to the served
`chm/cedar/stp-root/CedarFonts/Top/TiogaFonts.df` in its
`Exports [Fonts]<Fonts>TiogaFonts>` section — `Chess40.ks!1  18-Mar-86
06:18:12 PST` (size 2624) — then the desktop checkpoint was rebaked (`make
cedar-desktop-snapshot`) so the installer attaches it like the other 187
fonts. Now `Run ChessHackImpl` / `ChessHack` / `Eval
ViewerOps.OpenIcon[ViewerOps.FindViewer["ChessHack"], FALSE, TRUE, TRUE]`
paints the full board and `STP_SERVE Fonts/TiogaFonts/Chess40.ks` fires on
first use (screenshot `docs/images/cedar-chesshack-board-2026-07-21.png`).
See memory `cedar-font-install-attach` for the full mechanism.

### The offline apps demo (Chess + Clock, no network) — `make run-cedar-demo-sdl`

The **apps-demo checkpoint** (`make cedar-demo-snapshot` bakes it;
`run-cedar-demo-sdl` runs it; web dropdown "Cedar 6.1 — apps demo (Chess,
Clock, offline)") restores a clean Viewers desktop with **ChessHack and
Clock parked as icons**, both modules already loaded and the Chess40 font
pre-cached in memory. Clicking the ChessHack icon (then "Open") paints the
chess board with **zero network** (verified: 0 `STP_SERVE`/`STP_MISSING`
against an empty ftp-root). This is **Path B** — apps live in the
checkpoint's *memory*, not on disk, because rusty-backup file injection into
the volume crashes Cedar's live FS. A faithful files-on-disk install needs
Othello/Iago (see `docs/handoff.md` "Building a clean system volume" and
memory `othello-dead-end-iago-is-the-path`).

### The schematics viewer (AISViewer) — the PROVEN recipe (2026-07-18)

**Most of the time you want `Schematic.cm` or `Moon.cm`** (see the section
above) — the command files wrap exactly what follows. This is the long
form, for displaying something the command files don't cover or for
understanding what they do.

AISViewer is a library (a viewer class, no Commander command); images are
displayed through the CommandTool's `Eval` interpreter escape. The current
desktop checkpoint's profile already runs `AISImpl`/`AISViewerImpl` at
boot, so in a fresh desktop (`make run-cedar-desktop-sdl`, click the
CommandTool prompt line first for type-in focus) the whole recipe is:

```
Bringover [Cedar]<CedarChest6.1>Top>AISImages
Delete AISViewer.bcd AIS.bcd ViewerOps.bcd ViewerClasses.bcd TIPUser.bcd TIPTables.bcd Real.bcd AISFormat.bcd
Eval AISViewer.DisplayAIS[AISViewer.CreateAISViewer[], "ProcH-BitSlice07.ais", NIL, FALSE, NIL, NIL, NIL, TRUE]
Eval ViewerOps.OpenIcon[ViewerOps.FindViewer["AIS Viewer"], FALSE, TRUE, TRUE]
```

Line by line, because every deviation here cost us a debugging session:

- `Bringover` (no `-p`) attaches the image DF's files; content is
  demand-fetched over STP when the viewer first reads a file.
- `Delete` removes ATTACHED interface bcds. Attached-but-not-loaded
  interface bcds from the mixed-vintage mirror shadow the loadstate and
  poison every `Eval` through those interfaces ("NoSymbols"/version
  faults); after `Delete`, symbols resolve from the RUNNING modules. Do
  NOT delete `AISImpl.bcd`/`AISViewerImpl.bcd` — runtime-loaded modules
  are their own symbols and must stay attached. (On the CURRENT desktop
  checkpoint every name reports `-- not found!`: the profile's selective
  `-o` Bringovers never create the poisoning attachments, so the line is
  a harmless no-op — kept for worlds that did full Bringovers.)
- The `Eval` passes ALL EIGHT DisplayAIS arguments positionally: the
  interpreter cannot synthesize defaulted arguments ("can't hack default
  for argument 5") or build partial record constructors.
- The last `TRUE` displays into the (iconic) viewer; `OpenIcon` opens it
  on screen. Expect the halftone paint to take a while — the 500x644
  ProcH sheet is ~20 emulated seconds of honest 1985 Imager work.

Both frontends also accept **Cmd/Ctrl+V paste** (typed as paced
keystrokes), so these lines can be pasted instead of typed; headless
scripts can use `--paste-at CYCLES --paste TEXT` for the same path.

Other images to try in place of `ProcH-BitSlice07.ais`: `uscmoon.ais`
(the 1978 Lick Observatory moon photograph), `IFU-Sheet02.ais`,
`ProcH-Title.ais`, `reducedparc.ais`, `AlbumMusician.ais`. Images must be
8-bit AIS: `AISViewerImpl` hardcodes `GrayLinearColorModel[sWhite~255]`,
so 1-bit files render black (convert with `pdftoppm -gray` +
`tools/pbm2ais.py`). Big fetches are safe since the BSP
sender-retransmission fix (`FTP_REWIND` in the `DORADO_FTP_TRACE` log is
the server rescuing a dropped packet, not an error).

`stp-root/CedarChest6.1/AISImages/` serves period images from `[Cyan]<AIS>`
(uscmoon, reducedparc, AlbumMusician) and **the Dorado's own schematics**,
converted from `DoradoDocs/schematics/*.pdf` with

```sh
pdftoppm -mono -f 10 -l 10 -scale-to-x 1000 -scale-to-y -1 \
    DoradoDocs/schematics/ProcH.pdf /tmp/page
python3 tools/pbm2ais.py /tmp/page-10.pbm \
    chm/cedar/stp-root/CedarChest6.1/AISImages/ProcH-BitSlice07.ais
```

(`pbm2ais.py` implements AISFormat.mesa's uca raster layout; header fields
validated byte-for-byte against the period files.)

To make installs permanent, run with `DORADO_PDI_SAVE=1` so the volume
keeps them, or bake them into the checkpoint recipe
(`make cedar-desktop-snapshot` types its install commands itself).

### The "archive" checkpoint (2026-07-16)

`make run-cedar-archive-sdl` boots the desktop with everything above
already attached PLUS PressReader/ShowPress, Cedar SIL (+its
BiScrollers/ViewRec/Abutters/SirPress/ImagerPress/Interpress closure)
and the ORIGINAL Dorado artifacts on the volume:
`ProcH-Rev-Ce.press`, `IFU-Rev-Ch.press`, `DoradoLibraries.press`
(the 1979-81 drawings) and Ed Taft's `DoradoBooting.tioga` memo.

Verified working: `Open DoradoBooting.tioga` in the CommandTool
displays the memo fully formatted in Tioga. Notes from verification:

- Scripted focus clicks on this checkpoint must hit the PROMPT line at
  the bottom of the CommandTool (`--click 700,733`), not mid-transcript
  — clicking into the scrolled log turns keystrokes into text edits.
- `Run ShowPressPackage` reports unbound `[PressReader,*]` imports:
  run `Run PressReaderImpl` FIRST (the config does not include the
  reader impl). Displaying a page then needs the interpreter
  (`ShowPress.CreateShowViewer` / `Open`+`DrawPressPage`), which is
  gated on the interpreter open problem (handoff #1).
- `Run Sil` currently traps at startup into a debugger event viewer
  (`NoSymbols[TrapsImpl.bcd]` — the debugger wants symbols for
  BOOT-FILE modules to even decode the trap; BringOver MesaRuntime
  first to see the real event). Sil bring-up is open.

---

## Iago — Cedar's own disk utility (`--boot-switches l`)

Iago formats disks, creates physical and logical volumes, and installs
boot files, germs and microcode. It is a BOOT-TIME program: it offers
itself only when `Booting.switches[l]` is set, which on a real machine
comes from the herald's "Switches:" prompt. `--boot-switches` supplies
them (letters and digits, as typed there; `DORADO_BOOT_SWITCHES` does the
same from a recipe).

**You must log in first.** `IagoMainImpl.DoIt` calls `IagoCommands.Login`
itself, before the switch test — so the Cedar login prompt you have always
seen IS Iago running. A run that stops at login has not reached the test.

```sh
# Cold boot to Iago's "> " prompt: log in, then answer y.
DORADO_PDI_IGNORE_LABEL_FLAGS=1 DORADO_FAKE_TIME=1783285880 ./build/dorado \
  --boot-reason disk --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
  --pilot-disk work.pdi --ftp-root ../chm/cedar/stp-root \
  --boot-switches l \
  --type-at 760000000  --type 'Guest\n\n' \
  --type-at 2400000000 --type 'y' \
  --cycles 3200000000 --out iago.pgm
```

The prompt appears at ~2.4 B cycles; the `>` interpreter is live by 3.2 B.
`dorado/build/good-packs/cedar-iago.{snap,pdi}` is a checkpoint of exactly
that state (31,348 px), so restore it instead of re-booting.

**Typing commands.** `IagoOps.GetCommand` auto-completes on a SPACE, so
typing a command's FULL name leaves the tail as input for the NEXT prompt
— it silently lands in whatever question follows. Type only through the
token that makes the name unique:

| want | type |
|---|---|
| Describe Machine | `Describe M\n` |
| Describe Drives | `Describe D\n` |
| Describe Physical Volumes | `Describe P\n` (then `0` for "How many Alto partitions") |
| Create Physical Volume | `Create P\n` |
| Create Logical Volume | `Create L\n` |
| Erase Logical Volume | `Erase L\n` |

`?` lists matches, DEL returns to top level, `Quit` resumes the
interrupted boot.

**Building a volume on a second drive.** Iago sees RD0..RD3 at 114,100
pages each. Make a blank target and mount it in another slot:

```sh
python3 ../tools/pdi_create_blank.py target.pdi     # 815 x 5 x 28, sparse
./build/dorado ... --pilot-disk work.pdi --pilot-disk 1=target.pdi \
  --boot-switches l ...
```

`Create Physical Volume` prompts: drive number, new volume name, "destroy
all files ... are you sure?", then "password-protect this disk?".
`Create Logical Volume` prompts: new volume name, physical volume, size in
pages (default = all free), "are you sure?", then "debugger volume?".
Set `DORADO_PDI_SAVE=1` or the writes are discarded at teardown.

## Building a Cedar volume from a blank disk

**If you just want to use the result**, it ships: `make
run-cedar-diskvolume-sdl` opens the finished volume -- a Cedar 6.1 Viewers
desktop on a disk built from a blank file by Iago, carrying ten CedarChest
packages. The checkpoints unpack from `snapshot-assets/` automatically
(`cedar-iago-native.*`, 1.0 MB; `cedar-vol-software-native.*`, 3.8 MB).

The Iago pair is shipped because it is a **dependency, not a convenience**:
`cedar-diskvolume` takes the Iago checkpoint as its INPUT, so without it
none of the volume targets can run. `make cedar-iago-snapshot` rebakes it
(a 3.2 B-cycle cold boot).

To build the whole thing yourself -- three make targets, in order,
~2.5 hours of wall clock:

| target | what it does |
|---|---|
| `make cedar-diskvolume` | Iago builds the volume on RD1 and rewrites the root checksums |
| `make cedar-diskvolume-desktop` | boots it; Cedar installs itself, ending at the Viewers desktop |
| `make cedar-diskvolume-software` | one `Bringover` per CedarChest6.1 package (16 packages, ~279 files) |

**What the period procedure actually says.** `ReleaseMessage.tioga!9`,
"Installing Cedar 6.1 from scratch", has you **net boot** Cedar first --
"get to the CedarNetExec ... type `Cedar` ... log in ... When asked if you
want to use Iago, type `Y`" -- so the local disk RD0 is free and Iago's
`Create User World` formats and fills it. That is why `Create User World`
only ever targets RD0. Our emulator boots Cedar from RD0, so we build on
RD1 and run the steps individually; the same document calls that the hand
path: *"clients who wish to have non-standard volumes will have to do the
various steps by hand. See (or be) a wizard before attempting to perform a
non-standard installation."*

Other guidance from the same section, worth honoring:
- *"How many Alto partitions? For personal Dorados you want 0 or 1
  partitions (roughly 20K pages) for Alto emulation."* We answer 0.
- *"Is your disk already formatted? ... You don't want to reformat if you
  don't have to -- formatting takes a long time."* We never `Format Disk`;
  a blank PDI needs no media formatting.
- *"the installation will take roughly half an hour for a Dorado"* -- ours
  is ~34 B cycles.
- *"When the installation finishes, boot your machine with the boot
  button."*

**The one deliberate omission: `Install Initial Microcode`.** It writes
`InitialDiskDorado.eb` (5192 bytes = exactly 11 pages) into the disk's
hard-microcode region at low physical pages, and `IagoOps.ReservePages`
reserves that region only on the boot drive:

```
IF Disk.DriveAttributes[...].ordinal # 0 OR p = reserved THEN RETURN
```

On RD1 those pages belong to the logical volume, so the microcode lands on
the VAM's header (logical page 1 = physical page 4) and the volume boots to
`FS.Error: Local volume's permanent data structures are inconsistent`. The
emulator supplies microcode with `--eb`, so skipping it costs nothing.

**Root-page checksums.** Cedar zeroes a volume root's checksum while the
volume is mounted and rewrites it on a clean dismount. The emulator never
dismounts, so `tools/pdi_fix_root_checksums.py` does what the dismount
would have done. (Necessary for a tidy volume; it was NOT the cause of the
inconsistency above.)

**What ends up on the disk.** Measured: the Iago build leaves 9 files /
17.1 MB (mostly the 32,000-page VM backing file); the Cedar install adds
131 more, reaching 140 files / 19.8 MB. But `List ///*.bcd` still shows
only 18 bcds -- Cedar 6.1's `Basic` boot file loads its 34 loadees from the
file server on every boot, because `Basic.Loadees` names each one by
UNVERSIONED remote path and unversioned forces a server round trip. A
volume boots from disk; the base system still streams. Software you
`Bringover` does land locally.

## Speed: build with `make pgo`

A plain `make` gives about 0.6x a real Dorado. **`make pgo`** — a
two-stage profile-guided build — gives **1.33x on the Alto path and
1.43x on Cedar**, i.e. faster than the hardware, and it builds the
SDL frontend too. Every run prints the honest figure at the end:

```
270475713 microinstructions = 16.23 s of Dorado time in 12.46 s CPU = 1.30x real hardware
```

**Quote that line, never a cycles/s number.** `--cycles` counts BaseBoard
6502 cycles, 3.70 per Dorado microinstruction, and reading it as
microinstructions is how the emulator was documented as 1.75x real speed
while actually running at 0.46x.

The frontends default to `--speed 1028000` cycles per frame, which is real
Dorado speed; `--speed 400000` is the slower historical pace. A core that
cannot keep up costs frame RATE, never emulated speed.

Background: `docs/performance-plan.md`, and
`docs/performance-methodology.md` before changing anything for speed.

## Verification gates

| Command | Runtime | What it protects |
|---|---|---|
| `make test` | seconds | the C unit/integration suite (11 binaries) |
| `make verify-snapshot-abi` | ~1 s | that every baked checkpoint still restores against this build |
| `make verify-cedar-gate-selftest` | instant | that the Cedar gate's pass/fail logic still discriminates |
| `make verify-cedar-ls-selftest` | instant | that the listing gate still discriminates |
| `make verify-cedar-sil-selftest` | instant | that the Sil gate still discriminates |
| `make verify-cedar-desktop` | ~12 min | the shipped desktop checkpoint AND the browser build |
| `make verify-cedar-ls` | ~8 min | `ls` on a remote directory: STP Enumerate + LookupFile |
| `make verify-cedar-sil` | ~25 min | Sil opens ProcH01.sil: the CedarChest 6.0 chain end to end |
| `./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1' --cycles 2500000000` | ~2 min | the Alto path (expect 121,515 px) |

`verify-snapshot-abi` is the cheapest gate here and covers the widest
blast radius. `dorado_machine_restore` refuses any checkpoint whose
header struct sizes disagree with the running build, so a struct that
grows kills every checkpoint baked before it — and only the ones no gate
exercises stay broken. On 2026-08-04 the Leaf ethernet growth was found
to have left BOTH saved-login checkpoints dead for a week, on both
platforms, because every Cedar gate restores the *desktop*. This reads
the sizes out of each header (`dorado --print-abi` reports what this
build stamps) and boots nothing. **Run it after touching any struct a
snapshot serializes.** Native and wasm32 sizes legitimately differ, so
each set is checked against its own build; `web-assets/` is skipped with
a warning when emsdk is not on PATH.

`verify-cedar-sil` covers the CedarChest 6.0 Bringover/Run chain in the
served `User.Profile`, the 32 ProcH sheets on the volume, and the
`CreateButton` that puts Sil in the menu line — a path nothing else
touched, which is why the browser shipped for three days without Sil at
all. The click coordinate is measured, not guessed (the button spans
x 728–740, y 29–36); guessing cost most of a session. A pixel band works
here, unlike `verify-cedar-ls`, because the drawing replaces the left
half of the screen: 89,614 px open, 167,653 px if the click missed,
~28,500 px if the world cold-booted instead of restoring.

`verify-cedar-desktop` exists because three bugs shipped on 2026-07-18/19
that every test at the time passed. It restores the desktop checkpoint,
clicks the prompt and types `Moon.cm` exactly as a visitor would, and
**serves the pruned `build/web-stp` tree** rather than the full one, so
browser-only gaps fail here instead of in production. Two independent
signals, both real failures we hit:

- **`STP_MISSING` lines** — a file the served tree lacks. Cedar's
  interpreter demand-fetches while evaluating, so a missing `Cedar6.1`
  bcd breaks the deployed site and nothing else.
- **display-list pixel count** — the picture must actually paint.
  Reference values: mid-install checkpoint ~87 K, bare desktop ~167 K,
  failed `Eval` ~169 K (error text only), moon painted ~246 K. The
  threshold is 200 K.

The decision logic lives in `tools/check_cedar_gate.sh` (not inline in
the Makefile) so it can be exercised against known-good and known-bad
logs; `verify-cedar-gate-selftest` does exactly that in a second.

`verify-cedar-ls` guards remote listings, and **cannot** use a pixel count:
the CommandTool typescript is already full of text, so a listing scrolls
rather than adding ink (measured: 167,129 px with an eleven-file listing on
screen, 167,192 px for a listing of a directory that does not exist — less
difference than the length of the echoed command). It reads both ends of the
wire out of the trace instead: `STP_ENUM` for what the server produced, and
`STP_LOOKUP` for the guest naming those files back to us afterwards, which it
can only do having received the listing. `tools/check_cedar_ls_gate.sh`.

Checkpoint bakes are now deterministic: both snapshot recipes pin
`DORADO_FAKE_TIME` (`CEDAR_DESKTOP_FAKE_TIME`), because `time(NULL)` was
the emulator's one unpinned input and moved how far a boot got — once by
6 B cycles, which shipped a mid-install checkpoint.

---

## Useful flags and trace env vars

CLI flags (`./build/dorado --help`): `--cycles N`, `--eb PATH`, `--germ PATH`,
`--eftp PATH`, `--pilot-disk PATH` + `--boot-reason disk` (Cedar/Pilot disk
boot), `--ftp-root DIR` (in-process STP file server), `--out PATH` (final
snapshot PGM), `--shot-prefix PATH`, `--shot-every N` (periodic headless
snapshots), `--snapshot-in PATH`, `--snapshot-out PATH`, `--quote` /
`--boot-keys` (DDC boot keys), `--no-alto-boot`, `--progress`,
`--boot-file-number`, `--boot-dir[-all]`, `--type`, `--type-at CYC`,
`--key-hold`, `--mouse X,Y`, and `--mouse-delta DX,DY` (the latter injects
the terminal motion event used by Cedar's two-display edge-push path). SDL adds
`--scale N`, `--speed CYCLES/frame`,
`--screenshot F1,F2,…`. A running instance (headless or SDL) writes a
`<shot-prefix>-<cycle>.pgm` screenshot on **SIGUSR1**; with the colour board
installed, headless event snapshots also write the matching
`<shot-prefix>-<cycle>.color.ppm`.

For a live SDL run, choose the prefix when launching and signal the process
from another terminal:

```sh
./build/dorado-sdl ... --shot-prefix /tmp/dorado-progress
kill -USR1 <dorado-sdl-pid>
```

The screenshot is written at the next safe frame boundary, so the signal does
not perform SDL or filesystem work asynchronously. Repeat the signal at any
time to collect another cycle-named image.

DispM selection is explicit when launching a native frontend:
`--dispm none|standard|highres`. Use `--dispm standard` for the normal
640×480 colour board and `--dispm highres` for the 1024×768 board. The
WebAssembly frontend selects this in its world boot configuration: Lyric uses
`standard`; Alto, Mesa, Cedar, and Smalltalk use `none`. Older scripts may
still set `DORADO_DISPM_COLOR=1` or `DORADO_DISPM_PRESENT=1`; those remain
compatibility fallbacks, but new launchers should use the explicit setting.

Scripted input: `--type-at CYCLES --type TEXT` types a string synchronously,
and `--paste-at CYCLES --paste TEXT` runs it through the **same paced queue
the frontends' clipboard paste uses** (repeatable — give as many pairs as
you like, up to 16). Prefer `--paste-at` when you want to exercise the
interactive path exactly as a user would.

For colour pointer diagnostics, the shipped colour checkpoint reports
`ColorDisplay left`, so SDL places the colour window to the left of
monochrome. Set `DORADO_COLOR_SIDE=right` only when the guest is also set to
`ColorDisplay right`; otherwise host geometry and Cedar's edge-push direction
disagree.

The colour checkpoint is a restore image, so it also preserves the Cedar
modules loaded when it was baked. After replacing a CedarChest package, use
`make run-cedar-color-cold` to test it from a fresh boot; `Color.cm` cannot
replace an interface that is already loaded with a different version.

The 2026-08-09 interactive colour session is also preserved as three native
checkpoint/PDI pairs:

```sh
make run-cedar-color-before    # before ColorDisplay was loaded
make run-cedar-color-controls  # controls loaded, display still off
make run-cedar-color-on        # after ColorDisplay on
```

These are native snapshots. The browser needs a separate wasm32 bake because
snapshot files contain serialized C structures whose ABI differs between the
native and WebAssembly builds.

Controls in the SDL window: **Cmd/Ctrl+V pastes the host clipboard** as
paced keystrokes (the browser build uses its native paste event), F1
pauses/resumes, Cmd/Ctrl+Q quits, and the
three-button Dorado mouse is mapped left/middle/right = Red/Yellow/Blue with
laptop modifiers Option/Alt+click = Yellow (middle) and Cmd- or Ctrl+click =
Blue (right) — details in the top-level `README.md` "Controls" section (the
browser build behaves identically).

Disk (`docs/disk-subsystem-plan.md`): `--disk-real` boots Cedar through the
**real disk controller** read path (FIFO + framing) instead of the IOCB shim
(`make run-cedar-real` for the interactive SDL version); `--disk SLOT=PATH`
mounts a real Trident pack (T-80/T-300, R/W) on drive SLOT (0..3), creating a
blank T-80 if the file is missing and flushing writes back on exit. Disk traces:
`DORADO_DISK_SEQ` (controller events), `DORADO_DSK_PC_TRACE`,
`DORADO_DISK_MUFF_TRACE`. Map a runtime IM address to a source label with
`./build/mbdis -r 0oADDR file.MB`.

Tracing (env vars, mostly gated by `DORADO_TRACE_GATE="lo,hi"` cycle window):
- `DORADO_IFUDISP_TRACE=1` — one line per dispatched bytecode (pc/pcf/op/br31/stkp/stack).
- `DORADO_XFER_TRACE=1` — per-microinstruction pc/T/Q/md/last-VA (the workhorse).
- `DORADO_STORE_TRACE_VA="lo,hi"` — stores whose VA is in the octal range.
- `DORADO_VMDUMP="lo,hi,cyc"` — one-shot VM word dump at a cycle.
- `DORADO_BR_TRACE`, `DORADO_MAP_TRACE`+`DORADO_MAP_TRACE_INDEX`, `DORADO_FAULT_TRACE=all`,
  `DORADO_ETH_TX_TRACE`, `DORADO_PIPEVA_TRACE` — base registers / map / faults / ether / pipe.
- `DORADO_STORAGE_MODULES=1..4` — model 4MW..16MW of real memory (affects the map config).
- `DORADO_ETH_WIRE=1` — faithful transmit wire model (default OFF): defer EOT's
  OutDone post by carrier-sense (no tx while receiving) + per-word wire time,
  instead of completing transmits instantly. A/B scaffold for the faithful
  receiver (`docs/ethernet-faithful-receiver.md`); boot-safe but partial.

Snapshots serialize the running control store, CPU, memory, display,
BaseBoard, Ethernet, disk controller, fast-I/O router, and scalar machine
state. Pack media bytes are not embedded; restore with the same `--disk`
arguments used to create the snapshot. Writable dirty packs are flushed before
snapshot so the restored controller state and media image agree.

For Interlisp-D Lyric, create the library-world checkpoint pair once, then
resume it:

```sh
cd dorado
make lisp-lispusers-snapshot LISP_INIT_OVERRIDE=../chm/lisp/ftp-root/INIT.DORADO
make run-lisp-lispusers-sdl        # subsequent launches are immediate
```

The SDL frontend accepts `--snapshot-in` and `--snapshot-out`. The generated
`lisp-lyric-lispusers.snap` and `lisp-lyric-lispusers-xcl.pack` are an
inseparable pair because pack media is not embedded in the snapshot. The web
build has a separate wasm32-ABI checkpoint (`make lisp-lispusers-web-snapshot`,
assets under `web-assets/`, fetched lazily by the page); select
**Interlisp-D / Lyric — library desktop + online reference manual**, or link
directly with `?boot=lisp`. Native raw snapshots cannot be used by wasm32
because the current format deliberately serializes C structs and validates
their ABI sizes.

The restored desktop is a live Lisp listener: click inside the Exec (XCL)
window, then type. `(IL:FILESLOAD HELPSYS)` followed by
`(IL:LOAD '{DORADO}<IRM>IRMDEMO)` — answering the `{DORADO} Login:` prompt
with `Guest` twice — opens the 1987 Interlisp-D Reference Manual to the CAR
page over Leaf; `(IL:IRM.LOOKUP 'NAME)` then looks up anything else.
Loading library packages works both from the pack (`(LOAD '{DSK}GREP.LCOM)`)
and over the network (`(IL:FILESLOAD AISBLT)` — a file deliberately NOT on
the pack).

`make clean` removes the native files under `build/good-packs`. The producer
automatically rehydrates its prerequisite Lyric pack from the preserved
compressed web checkpoint media before performing the full native boot.
Historical experimental Current/Harmony/Medley packs in `build/good-packs`
are not automatically regenerated and are not prerequisites of supported run
targets; keep anything valuable outside `build/` before cleaning.

For browser snapshots, prefer checkpoints that remove a long wait without
substantially increasing the initial download. Lyric is already checkpointed.
Cedar's login is now checkpointed after fixing its blank-display regression.
Mesa Network Executive is the next high-value target once its exact capture
frame is revalidated. The directly booted Alto games are fast enough
that a separate snapshot for every menu item is not currently worthwhile.

### Putting Interlisp library packages on the Lyric pack

Lyric fetches its sysout from us over Pup FTP, but Lisp's own file access
cannot reach us -- it wants Leaf, which we do not serve -- so library
packages have to arrive on the **local disk**, where Lisp names them
`{DSK}FOO.DCOM`.

```sh
cd dorado
make lisp-lispusers-fetch      # mirror .DCOMs into chm/lisp/ftp-root/
make lisp-lispusers-pack       # insert them into the Alto FS in the pack
make lisp-lispusers-snapshot   # cold-boot that pack and save the world
make verify-lisp-lispusers     # type a LOAD and read Lisp's reply
```

Pick packages with `LISP_LISPUSERS_PACKAGES='BROWSER EMACS'`;
`python3 tools/fetch_lispusers.py --list` shows all 131.

Three things about this that cost time to learn:

- **The checkpoint has to be re-baked after a pack edit.** A restored world
  answers out of the directory it read at *its* boot, so a file inserted
  afterwards is genuinely invisible to it -- `File not found:
  {DSK}BANNER.DCOM`, while a file that was already on the pack loads fine.
  That is why `lisp-lispusers-snapshot` exists rather than reusing
  `lisp-lyric-xcl.snap`.
- **`--ftp-root` replaces `--ftp-sysout` for the cold boot.** A single
  `--ftp-sysout FILE` short-circuits every request to that one file; a
  served root resolves `{DORADO}LISP.SYSOUT` by name (the STP server strips
  the `{device}` bracket) and serves the packages from the same directory.
  Verified: the fresh boot reaches the desktop at 208,985 px either way.
- **Match the release's extension.** Interlisp compiles to `.DCOM` through
  Koto and **`.LCOM`** from Lyric. A Harmony `.DCOM` in the Lyric world gets
  `Bad compiled function`; the Lyric `.LCOM`s load. The default package list
  (`dorado/lisp-lispusers-packages.txt`, 194 files) is all Lyric-vintage.
- **The Exec is XCL, not Interlisp.** `LOAD` works unprefixed; Interlisp
  functions need `IL:` (`(FILESLOAD X)` answers `Undefined car of form`).
- **The pack cannot be made bigger.** `AltoDiabloDisk.mc` hard-codes 406
  Diablo cylinders, the disk address has one drive bit and one head bit, and
  `406*2+3 = 815` is exactly the T-80's cylinder count -- so 2 drives x 2
  heads x 406 x 14 = **22,736 pages** is architectural, and
  `LISP.VIRTUALMEM.` takes 15,003 of them. Room is bought only by deleting
  Alto utilities the Lisp pack never uses (`LISP_PACK_DROP` in the Makefile,
  via `altofs --delete`): dropping 33 files frees 1,590 pages and takes the
  pack from 119 to **206 packages + 14 fonts**. All 285 `.LCOM`s (~11,300
  pages) do not fit, and neither does the IRM (~7,400) plus its hash
  (~3,760) -- so HELPSYS cannot read the IRM here the way Medley does.
- **Dependencies must be on the pack too.** A package's `COMS` does its own
  `FILESLOAD` against `{DSK}`, so a missing dependency opens a
  `SIMPLE: ERROR` window naming it (`File MATMULT not found on {DSK}`). Add
  it to the list and rebuild. Not everything fits: 285 `.LCOM`s exist,
  ~4800 free pages hold about 194.

Verified loading: `GREP`, `HRULE`, `PACMAN`, and `SKETCH` -- the Interlisp
illustrator -- which resolves SKETCHELEMENTS/SKETCHEDIT/SKETCHOBJ/
SKETCHBMELT/SCALEBITMAP off `{DSK}` unattended.

#### Interactively, in SDL

```sh
cd dorado
make run-lisp-sketch-sdl      # restores the desktop and types the LOAD for you
make run-lisp-lispusers-sdl   # same desktop, load whatever you like by hand
```

Both restore in a few seconds (no 8.8 B-cycle boot). `run-lisp-sketch-sdl`
then clicks and types for you: the first `Loading` line shows up about ten
seconds later, and the full chain is ~4 B cycles, so give it ~3 minutes to
finish. By hand:

1. **Click the Prompt Window** (upper left, around x=250 y=50). Input goes to
   the window under the cursor; without the click the keystrokes land
   nowhere. This is the single most common way to conclude "the desktop is
   not interactive" when it is.
2. Type `(LOAD '{DSK}SKETCH.LCOM)` and Return. It prints `Loading ...` for
   SKETCH and each dependency it pulls in, and takes a little while --
   roughly 4 B cycles for the whole chain.
3. Anything else on the pack loads the same way; `ls
   chm/lisp/ftp-root/*.LCOM` is the menu, and
   `dorado/lisp-lispusers-packages.txt` is what actually fits.

Caveats worth knowing before you demo it: **loading is proved, driving is
not** -- `(IL:SKETCH)` echoed without opening a window, and the entry points
live in the Lyric manuals that are missing from the archive. Packages whose
dependencies did not fit in the 194 open a `SIMPLE: ERROR` window naming the
file; add it to the list and re-run `make lisp-lispusers-pack
lisp-lispusers-snapshot`. And `make run-*` SDL targets never exit on their
own -- close the window when done. Full survey of every
Interlisp release in the archive: `docs/interlisp-archive-survey.md`.

`make test` runs the unit suites (11 binaries); the regression "gate" the
bring-up keeps green is: `make test` + AEmu NETEXEC ≈ 1476–1505 px +
Galaxian ≈ 121.5k px + AltoMesaDorado in band + `make sdl` compiles +
PARC's real hardware diagnostics via `build/rundiag`
(`docs/running-diagnostics.md`).

---

## Current status (August 2026)

- **Path A (Alto-on-Dorado): working.** AEmu boots Alto software over EFTP and
  renders — games and the NetExec menu come up. This is the validated path and
  the regression gate.
- **Path B (Cedar/Pilot germ): boots to the Viewers desktop (2026-07-15).**
  The full BaseBoard→Bootstrap→Initial→Cedar-microcode→germ→Pilot chain
  boots Cedar 6.1 from the PDI disk to the SimpleTerminal login; logging in
  as Guest on the work volume drives LoaderDriver's remote install over the
  in-process STP server, Tioga fonts are demand-fetched, and the Viewers
  desktop comes up live (Clock, CommandTool, menus). Screenshot:
  `docs/images/cedar-desktop-first-boot-2026-07-15.png`. Saved checkpoints
  restore it instantly (`make run-cedar-desktop-sdl`; browser dropdown
  entry). See `docs/CONTINUE-HERE.md` for the live detail and next steps.
- **Cedar colour: working.** `make run-cedar-color` restores the saved
  ColorDisplay-on checkpoint and opens the 640x480 DispM screen beside the
  1024x808 monochrome display. The WebAssembly entry
  `Cedar 6.1 — ColorDisplay on (DispM)` restores the matching wasm32 pair and
  offers Both / Color / Monochrome controls. Gargoyle launches from this
  colour-enabled Cedar 6.1 environment; the remaining colour work is a
  repeatable guest-driven regression gate and Koto Lisp validation.
- **Interlisp-D: boots to the Lyric desktop.** `make run-lisp-snapshot-sdl`
  restores the saved Exec (XCL) desktop; the full boot is
  `make run-lisp-good-sdl`.
- **Stage-2 net-boot server is ready:** the in-process EFTP/Mayday boot
  server serves Cedar boot files byte-exact (proven by `make test`); driving
  the germ to `DoInLoad` over the net is an unexercised alternative to the
  working disk path.
- **Microengine:** thoroughly schematic-audited (`docs/schematic-audit.md`);
  the bring-up fixed six real microengine bugs in the Mesa/field/shifter/
  branch/Link paths (the sixth — `Return` clobbering a same-instruction
  explicit `Link←` load — was the final blocker in front of the Cedar
  desktop). Remaining hardware gaps and their specs:
  `docs/hardware-specs.md`.
