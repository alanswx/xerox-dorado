# Dorado Archive Atlas

Status: seed corpus, started 2026-08-01

This directory is the beginning of a structured map of the Xerox PARC / CHM
archives, with the Dorado emulator and reconstructable disk packs as the
first use case.

The Atlas is deliberately separate from the emulator. It is documentation
and archival data only. Its job is to answer questions such as:

- Which files belong to the same software generation?
- Which microcode, germ, boot file, volume, and network services form a
  compatible boot chain?
- Is an item source, an executable, a container, a disk image, or merely a
  document describing one of those things?
- Which claims are observed, which are inferred, and which are only
  unresolved archive leads?
- What exact ingredients and validation evidence are needed to make a disk
  pack or emulator fixture?

## Why this exists

The CHM archive is an IFS preservation of several servers and snapshots, not
a modern software distribution. A complete identity includes the archive
server, directory path, filename, and version. The same basename can occur in
several releases, servers, or generations. The archive also expands `.dm`
Dump/Load files and `.bfs` disk images into browsable directories. Those
relationships must be retained rather than flattened away.

The existing project documents already contain much of the interpretation,
but in different forms:

- [`chm-archive.md`](../chm-archive.md) is the local CHM catalog.
- [`parc-archive-survey.md`](../parc-archive-survey.md) covers archive
  branches and disk formats.
- [`xerox-systems-archive-survey.md`](../xerox-systems-archive-survey.md)
  organizes Cedar, Mesa, Smalltalk, and Alto by release.
- [`dorado-software.md`](../dorado-software.md) records runnable Dorado
  combinations.
- [`cedar-media-provenance.md`](../cedar-media-provenance.md) records how
  Cedar volumes were actually built.
- [`cedar-archive-gaps.md`](../cedar-archive-gaps.md) and
  [`cedarchest-packages.md`](../cedarchest-packages.md) record package-level
  completeness and recovery leads.

The Atlas joins these into a small graph: artifacts, releases, relationships,
recipes, and gaps.

## Directory contents

| File | Purpose |
|---|---|
| `README.md` | Scope, principles, workflow, and maintenance rules. |
| `schema.md` | Record shapes and controlled vocabulary. |
| `artifacts.jsonl` | One JSON object per archive or local artifact. Seeded, not complete. |
| `releases.jsonl` | Software-generation and target-machine records. |
| `relationships.jsonl` | Edges between artifacts and releases. |
| `recipes.jsonl` | Candidate or validated boot/disk-pack recipes. |
| `gaps.jsonl` | Explicit missing, uncertain, stale, or unreviewed areas. |
| `overview.md` | Human-facing first view of the current seed corpus. |
| `remote-inventory.jsonl` | Exhaustive CHM cross-reference occurrences, with local mirror matching. |
| | NOT COMMITTED (138 MB, over GitHub's blob limit) -- regenerate it and `viewer/remote-inventory.js` locally with `tools/build_chm_atlas_inventory.py`. |
| `inventory-summary.json` | Snapshot counts and matching methodology. |
| `viewer/` | Static browser viewer; works from `file://`, with the full inventory bundled. |

The first exhaustive snapshot, regenerated from the checked-in
`chm/cross-reference.html` on 2026-08-02, contains 205,797 CHM occurrences and
checks 4,976 local `chm/` files. Matching is by basename, byte size, and
cross-reference CRC32. It currently classifies 2,399 occurrences as exact
name/content matches, 6,824 as same-content matches under a different local
name/path, 410 as same-name/size but CRC-different, and 196,164 as unmatched.
These are occurrence-level counts, not deduplicated file counts.

JSONL is the durable format: one self-contained record per line, easy to
search, diff, retrieve, and feed selectively to a future LLM. Markdown is the
human-facing layer and should eventually be generated from the records rather
than becoming a second competing database.

To browse it locally, open
[`viewer/index.html`](viewer/index.html). It loads the full inventory
automatically from `viewer/remote-inventory.js`, so no file picker or local HTTP
server is required. The JSONL file-picker button remains available for loading
an alternate snapshot.

The viewer opens on the semantic overview rather than the raw file table. Use
**Environments** for release-level software, **Browse archive** for CHM server and
directory branches, **Disk recipes** for bootable combinations, **Recovery
queue** for unresolved high-value leads, and **Raw inventory** only when doing
file-level audit work.

To regenerate the inventory after changing the CHM cross-reference or local
mirror, run:

```text
python3 tools/build_chm_atlas_inventory.py
```

The generator does not download or modify CHM files; it only reads the local
cross-reference and mirror, then writes the Atlas inventory, viewer seed, and
browser bundle.

## Core principles

### Preserve archive identity

Never identify an item only by basename. Preserve:

```text
[server]<directory>name!version
```

Store original case and a normalized comparison key separately. Store the
local path, raw archive URL, index/view URL when known, byte size, checksum,
and archive date when available.

### Treat containers as containers

An expanded `.dm` or `.bfs` tree is related to its parent object. It is not a
second unrelated copy. A record should say whether it is:

- an original archive object;
- an expanded archive/disk view;
- a file inside that expansion;
- a later extracted copy; or
- an emulator-generated derivative.

### Separate facts from interpretation

Every meaningful claim should have an evidence path. Use `status` and
`confidence` rather than presenting an inference as fact. “Not found” must
include the servers, directories, search method, and `as_of` date that were
checked.

### Organize around bootable environments

OS names and extensions are useful indexes, but the primary unit for this
project is an environment:

```text
target machine
  -> microcode / bootstrap
  -> germ or loader
  -> boot file or disk volume
  -> runtime and package closure
  -> optional network services
  -> observed validation
```

This is what distinguishes a real disk-pack recipe from a pile of historically
related files.

### Do not overclaim completeness

The initial Atlas is a curated seed. It is not yet a complete mirror of the
CHM cross-reference or a complete inventory of `chm/`. Counts and coverage
claims must carry a snapshot date and scope.

## Proposed workflow

1. Freeze an inventory snapshot: source servers, local roots, dates, and
   counts.
2. Record raw archive objects and local artifacts without interpretation.
3. Normalize paths, versions, cases, checksums, dates, and container edges.
4. Collapse exact duplicate bytes only in a derived view; never delete the
   original occurrences.
5. Extract release/package evidence from `.df`, load maps, boot files,
   `.cm`, microcode build files, and existing project runbooks.
6. Hand-classify the relevant systems first: Alto/Mesa, Dorado, Cedar,
   Smalltalk-76, and Interlisp-D Lyric.
7. Add compatibility and dependency relationships.
8. Write recipes with exact inputs, boot path, media format, network needs,
   known incompatibilities, and validation gates.
9. Track gaps as scoped research claims, not as unqualified absences.
10. Generate human Markdown views from the structured records when the
    corpus becomes large enough to justify that step.

## First population tranche

The seed records cover the chains that are already meaningful to the
emulator:

- Dorado Alto/Mesa and Alto game booting;
- Cedar 6.1 with a matched 6.1 germ and Pilot volume;
- Smalltalk-76 with DSemu and the XM pack;
- Interlisp-D Lyric's Dorado Lisp microcode and sysout path;
- the Dorado bootstrap/Initial/LoadMB support artifacts;
- Cedar runtime, `Basic.Loadees`, VersionMap, fonts, and CedarChest package
  anchors needed to explain why a volume is or is not self-contained.

The records intentionally point back to existing runbooks instead of
duplicating every command and every historical discovery.

## What a future LLM should receive for a disk-pack task

Give it a bounded packet containing:

1. the target machine and software generation record;
2. the exact artifact records for microcode, germ, boot file, volume, and
   package closure;
3. all compatibility and incompatibility edges;
4. the disk/media format record and geometry;
5. the recipe and its known omissions;
6. validation evidence and the desired success gate;
7. unresolved choices and negative-search evidence.

This is safer and more useful than passing the entire CHM tree or a flat list
of filenames.
