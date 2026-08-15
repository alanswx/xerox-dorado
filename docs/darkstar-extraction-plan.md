# Plan: getting files off the Dandelion hard disks

2026-08-11. The floppies are done (`docs/xerox-media-catalog.md` -- 13,921
files decoded offline with `tools/pilot_floppy.py`). This is about the six
65 MB hard-disk images, where that approach does not apply yet.

## 1. What the images are

`Harmony.img`, `Koto.img`, `Lyric.img`, `Medley.img`,
`ViewPoint-11-9-1990-18-38.img`, `XDE.img` in `Darkstar/D/Disks/`, plus
`Interlisp-D-Harmony.img` / `ViewPoint-2.0` / `XDE-5.0` in
`chm/bitsavers/bits/Xerox/8010/8010_hd_images.zip`. **Koto, Lyric and
Medley exist only in the Darkstar repo** -- bitsavers does not have them.

All are 65,433,601 bytes: **1 byte of drive type, then raw track data**
(`Darkstar/D/IO/SA1000.cs`), 5,325 words per track at 3 bytes per word.
65,433,600 / 3 / 5,325 = exactly 4,096 tracks, and type `0x02` = `Q2040`
= 512 cylinders x 8 heads. Not a linear sector image: sectors live inside
the track word stream behind address marks, decoded by
`Darkstar/D/IO/ShugartController.cs` (`_headerAddressMark`,
`_labelDataAddressMark`, header/label/data fields per sector).

`Medley.img` is the one that matters most: an installed Medley on a 1108.

## 2. Two routes, and why to try the offline one first

### Plan A -- decode offline, no emulator (preferred)

The work is smaller than it looks, because two of the three layers are
already solved:

1. **Track -> sectors.** Port the address-mark walk from
   `ShugartController.cs`: each sector is header / label / data fields
   introduced by their marks. ~100 lines of Python. Output: a linear
   image of (label, data) per sector.
2. **Pilot volume -> files.** Try `pilot_floppy.py`'s existing
   `PLPAGE` seal-walk **unchanged** on that linear image. Pilot uses the
   same leader-page structure on hard volumes as on floppies, so file
   names, lengths and contiguous data may fall straight out. A first
   scan of the raw `.img` finds only 2 `PLPAGE` seals -- but that is
   expected, because the seals are split across the track word stream
   and cannot be seen until step 1 is done. **This step is a free test:
   if the seal-walk finds files, we are finished.**
3. Only if step 2 comes up empty do we need real Pilot hard-disk volume
   structures (PhysicalRoot / LogicalRoot / VAM), which we already model
   for Cedar in `dorado/src/pdi.c`.

Advantages: scriptable, diffable, no GUI, and it catalogs all six images
in one run -- which is what was actually asked for.

### Plan B -- boot it in Darkstar and use the guest

Fallback, and the only route if the volume turns out to be Interlisp's
own `{DSK}` file system rather than Pilot's.

- Build: C#/.NET. The readme targets Windows or **Mono on Unix with
  SDL 2.0**; it says native macOS support was "a future release", so
  expect Mono plus `brew install sdl2`.
- Boot `Lyric.img` or `Medley.img`, then use Interlisp's own tools --
  `(FILESLOAD ...)`, `DIRECTORY`, `COPYFILE` -- to enumerate, and move
  files out over the emulated 10 Mbit Ethernet (Darkstar speaks XNS via
  [devhawala/dodo](https://github.com/devhawala/dodo)) or onto a
  scratch floppy image, which `pilot_floppy.py` then reads.
- Our Darkstar copy is gitignored, so it can be modified freely. The
  cheapest useful modification is a **headless sector-dump mode**: a
  command-line switch that loads an `.img`, runs the existing
  `ShugartController` decode over every track, and writes a linear
  `.raw`. That turns Plan B's infrastructure into Plan A's input and
  removes the GUI from the loop entirely.

## 3. Order of work

1. Port the track decode (Plan A step 1) and run the existing seal-walk
   over `Medley.img`. Cheap, and decides everything downstream.
2. If it works: catalog all six images, diff against
   `docs/xerox-media-catalog.md`, and report what only exists on the
   hard disks.
3. If it does not: add the headless dump to our Darkstar copy, or model
   the Pilot hard-disk volume using the Cedar PDI code as the reference.
4. Either way, do **not** boot Darkstar interactively just to browse --
   catalog first, boot only to fetch something the catalog says is
   worth having.

## 4. What we already know is NOT on them

Worth stating so nobody boots an image hunting for these: the Medley 1.0
library and LispUsers, Medley 1.1, Lyric/Koto/Harmony/Fugue/Carol/
Intermezzo libraries, LOOPS, ROOMS, NoteCards and XDE 5.0 are all
already extracted from floppies -- see `docs/xerox-media-catalog.md`.
The hard disks are interesting for what an *installed, running* system
has that the distribution media do not: user files, site configuration,
and whatever was left in `{DSK}`.
