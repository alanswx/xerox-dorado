# CHM Basic Disks

This directory holds selected Alto basic disk streams from the CHM PARC
archive.

Important: `*.BFS!N` files here are Palo-readable BFS transfer streams, not
ContrAlto/Palo AAR sector images. Use `AltoInfo/palo/src/par -ibfs -1 FILE ...`
to load them. A modifying Palo run without `-obfs` saves the writable copy as
an AAR image, which `dsk2trident` can then map into a Trident pack.

`BcplProg.BFS!13` is used by `make -C dorado lisp-bcplprog-loader-image` as an
installed Alto OS layout whose Swat/Swatee labels and serials must be preserved.

## `Smalltalk14.bfs!1` — the December 1980 Smalltalk-80 disk (added 2026-08-11)

Weyer's installed Alto Smalltalk disk, from
`https://xeroxalto.computerhistory.org/Indigo/BasicDisks/`. Both forms are
here: the packed stream `Smalltalk14.bfs!1` (4,057,490 B, crc32 `85f42d46`)
and CHM's expanded members under `Smalltalk14.bfs!1_/` (44 files, 3.4 MB).
Every member was verified byte-for-byte against the size + crc32 in
`xeroxalto.computerhistory.org/cross_reference.html`; 44/44 match.

This is the **only Smalltalk-80 in either CHM archive**. The ST-80 pieces:

| file | size | date | what |
|---|---:|---|---|
| `st80dec16.im` | 498,604 | 20-Dec-1980 | the ST-80 **V1** image (header `00 03 3c 00`) |
| `st80sources.v00` | 821,336 | 16-Dec-1980 | sources (`"St80Sources.V00 (16 December 1980 4:31:56 pm )"`) |
| `st80changes.v00` / `st80dec16.changes` | 51,317 | 20-Dec-1980 | the change set (identical files) |
| `d80load.run` + `.syms` | 54,884 / 3,916 | 19-Dec-1980 | **the Alto-side ST-80 loader** |

The rest is a normal installed Alto pack around them — `Sys.Boot`,
`Executive.Run`, `Swat`/`Swatee`, `Ftp.Run`, `BRAVO.RUN`, 14 `.AL` fonts —
plus `small.boot` (978,432 B), a **Smalltalk-76** world on the same disk.

Why it matters: `d80load.run` is an **Alto** program, so this ST-80 is
reachable through the Alto-on-Dorado path we already boot (`AEmu`/`DSemu` +
a Trident pack, the way `xmsmall.boot` runs today) — unlike the two standard
interchange images we hold (`chm/archiveorg/smalltalk-80/VirtualImage`,
ST-80 v2; the bitsavers Dolphin `st80-82-11-15-customer.im`, 1982), which
have no Dorado-side loader at all. See `docs/xerox-systems-archive-survey.md`
section 3.

Nothing here is Dorado-native: no Smalltalk-80 microcode for the Dorado
exists in either archive. `SmalltalkDorado.eb!1` / `DSemu` is the
**Smalltalk-76** interpreter (`DSmallDefs.mc`: "Dorado Model 1, XM version",
Deutsch, July 1979).
