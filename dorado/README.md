# Dorado C emulator

A software emulator of the Xerox Dorado (see the top-level and in-directory
`CLAUDE.md` for the project mission, architecture, and build details). Build
and run everything from this directory.

```
make           # build the emulator, tools, and tests
make test      # run the integration tests (11 binaries)
make sdl       # build the windowed SDL frontend (build/dorado-sdl)
make web       # build the WebAssembly frontend (needs emcc on PATH)
make run-cedar # boot Cedar 6.1 to its login prompt (windowed)
```

Two families run today: the **Alto** games / NetExec menu (below), and
**Cedar 6.1**, which boots from a Pilot disk image to its SimpleTerminal
login prompt with working keyboard. Cedar also has a saved colour checkpoint:
SDL presents the second 640x480 DispM screen, while the WebAssembly build
offers the `Cedar 6.1 — ColorDisplay on (DispM)` entry with Both / Color /
Monochrome controls. See the top-level `README.md` and
`docs/running-the-emulator.md` for exact commands.

## Game menu (NetExec)

The Alto **Net Executive** ("NetExec") boots over Ethernet and presents an
interactive `>` prompt from which you can boot any Alto program by name. The
emulator ships a fake in-process boot server that advertises every Alto
game/utility we have (`../chm/bootfiles/*.boot`) as a NetExec boot-file
directory, so the prompt becomes a menu of the whole collection.

Launch it (the standalone headless binary, or `build/dorado-sdl` for a live
window):

```
make sdl
./build/dorado-sdl --eb worlds/aemu.eb --boot-dir-all
```

`--boot-dir-all` auto-registers, as a NetExec directory, every boot file in
the directory that holds the EFTP boot file (`../chm/bootfiles/`) whose first
word is the Alto **B-format** tag `0o405` (runnable on the AEmu Alto
emulator). Mesa-format files (`0o345`, e.g. `CedarNetExec.boot`) are skipped
from this Alto menu — Cedar/Mesa boots via its own disk/germ path
(`make run-cedar`), not by chaining through the Alto NetExec. The flag is **on by
default** when no explicit `--boot-dir` is given, so the demo just works;
pass `--no-boot-dir-all` to turn it off, or `--boot-dir NAME=BFN=PATH` to
register a single file by hand (which disables the auto-scan).

`NETEXEC` itself boots first (served as the Stage-2 EFTP world); the games
are served only on demand. At the NetExec prompt:

- Type a game name, e.g. `Galaxian.boot`, and press Return. NetExec asks the
  server for its boot-file directory (a `257B` BootDirReq), the fake server
  answers with the menu (a `260B` BootDirReply), NetExec finds the name,
  Maydays (`244B`) the matching boot-file number, and our EFTP server streams
  the game; it then boots and paints.
- NetExec's keyword completion lists the directory once it has been fetched.

This is a **read-only, asset-only** demo: no disk image is needed, only the
boot files already in `chm/bootfiles/`. That makes it the natural first
"browser" demo of the running Dorado.

Notes:

- The directory can exceed one Pup packet (~266 words; NetExec posts a
  ~279-word input buffer). The reply is paginated and NetExec's `InstallDir`
  accumulates entries across the BootDirReqs it retransmits per directory
  fetch, so the whole menu is delivered over a couple of requests.
- Boot-file numbers are auto-assigned from `0o200` up, avoiding the microcode
  boot offsets `0o110`-`0o114` and any explicit `--boot-dir` entry.
- Typing the game name and watching it boot is verified interactively in the
  SDL frontend. The individual per-game EFTP serve+boot path (what a typed
  selection ends up doing) is also covered by the `make run-<game>` targets,
  e.g. `make run-galaxian`, which boot a single game directly.
