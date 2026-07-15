# Cedar Pilot media provenance and rebuild recipe

This records how the project's Cedar `.pdi` volumes were made.  They are
**reconstructed Pilot/Cedar volumes**, written locally with the Rusty Backup
Pilot writer; they are not raw period disk images.

The implementation is in `tools/rusty-backup/src/fs/alto/pilot.rs`.  The two
entry points are `examples/pilot_probe.rs` (the small boot fixture) and
`examples/cedar_kitchen_sink.rs` (the large package volumes).

## The first volume: `CedarDorado-boot.pdi`

The original small boot fixture was created as a 1,400-page Cedar physical
volume and then populated with just two boot-chain files:

```sh
cd tools/rusty-backup
cargo run --example pilot_probe -- new 1400 cedar CedarDorado \
  ../../CedarDisk/CedarDorado-boot.pdi
cargo run --example pilot_probe -- install-boot \
  ../../CedarDisk/CedarDorado-boot.pdi germ \
  ../../chm/cedar/germ/Dorado.germ!4
cargo run --example pilot_probe -- install-boot \
  ../../CedarDisk/CedarDorado-boot.pdi bootfile \
  ../../chm/cedar/cedar6.1/BasicCedarDorado.boot!22
```

`pilot_probe new` calls `pilot::create_blank`; each `install-boot` calls
`pilot::install_boot_file`, which allocates the file and updates the physical
root's `bootingInfo` plus the page-label boot chain.  The resulting layout is:

| VDA | Contents |
|---:|---|
| 0–86 | Pilot physical/logical roots, reserve, and VAM |
| 87–118 | historical `Dorado.germ!4` (32 pages) |
| 119–1178 | `BasicCedarDorado.boot!22` (1,060 pages) |
| 1179–1398 | 220 free pages |

That is why this is a *boot fixture*, not an installed Cedar workstation
volume: it has a germ and a BasicCedar boot world, but no package corpus or
ordinary client-directory entries.

The fixture itself still contains the historical `Dorado.germ!4`.  Do not use
that fact to select the emulator germ: current Cedar runs must pass the
version-matched `chm/cedar/germ-alt/Dorado.germ-6.1.6` alongside
`CedarDorado.eb!6`, as documented in `running-the-emulator.md`.

Inspect the boot slots and chains with:

```sh
cd tools/rusty-backup
cargo run --example pilot_probe -- boot-info ../../CedarDisk/CedarDorado-boot.pdi
```

The byte comparison is also useful:

```sh
cargo run --example pilot_probe -- verify ../../CedarDisk/CedarDorado-boot.pdi \
  germ=../../chm/cedar/germ/Dorado.germ!4 \
  bootfile=../../chm/cedar/cedar6.1/BasicCedarDorado.boot!22
```

As of 2026-07-11, that second command confirms both payload hashes and boot
chains and confirms a byte-for-byte PDI writer round-trip, but exits failure:
the current reader interprets the fixture VAM as 1,315 free pages while the
page labels correctly report 220.  Record this as a known VAM
reader/writer-compatibility discrepancy; do not describe the current fixture
as `pilot_probe verify` clean until that count is reconciled.

## Larger reconstructed volumes

`CedarDorado-kitchensink*.pdi` were made by the same writer using
`examples/cedar_kitchen_sink.rs`.  That program creates a 65,535-page Cedar
volume, installs the germ and boot file, adds a pre-ordered host-file manifest
until the volume is full, then writes a Cedar `client` name-directory B-tree:

```sh
cd tools/rusty-backup
cargo run --release --example cedar_kitchen_sink -- \
  <germ> <boot-file> <manifest.tsv> ../../CedarDisk/CedarDorado-kitchensink.pdi
```

The exact package manifest used for the historical kitchen-sink builds was not
checked in, so they are structurally reproducible from their contents but not
presently byte-for-byte reproducible from a single command.  The builder source
does preserve the method and ordering policy (games first, then the remaining
packages).

`CedarDorado-bestof.pdi` is a later, smaller reconstruction from the
kitchen-sink corpus.  It uses the same Pilot writer and a corrected
Cedar-compatible two-page file-header/VAM layout; see
`examples/cedar_repack.rs` and `running-the-emulator.md`.  It is likewise not
a recovered original pack.

## Safe modification rules

- Preserve FileIDs 2 (germ) and 3 (boot file), their `bootingInfo` slots, and
  their page-label chains.  Those are the pre-filesystem boot path.
- Do not delete a range of apparently optional FileIDs merely to create space.
  Cedar boot and package dependencies are not described by the file ordering.
- A saved emulator snapshot includes guest runtime state.  Mounting a roomier
  PDI after restoring it does not necessarily repair a guest cache or local
  name/page exhaustion recorded in that snapshot.
- Check each derived PDI with `pilot_probe boot-info` and the emulator's
  `pdidump` before using it in a boot experiment.  Also run `verify`, but
  distinguish a boot-chain failure from the known small-fixture VAM-count
  discrepancy above.

For the detailed page-level contract of the first fixture, see
`tools/rusty-backup/docs/cedar_boot_fixture.md` and
`CedarDisk/PARC_PILOT_FORMAT.md`.

For the reproducible 65K-page work-volume build used by current bring-up, see
`tools/rusty-backup/docs/dorado_cedar_work_volume.md` and
`tools/rusty-backup/scripts/build-cedar-work-volume.sh`.

On 2026-07-11, a fresh scripted `CedarDorado-work.pdi` passed Rusty Backup's
verification and Dorado's `pdidump`, then cold-booted to the graphical Cedar
6.1 SimpleTerminal login prompt.  Guest local-file creation remains the next
runtime acceptance gate.

**Logical-volume boot records (2026-07-15).** A properly installed Cedar
disk records the germ + boot file twice: the PHYSICAL volume root's
`bootingInfo` (10B) — all the microcode/germ cold boot reads — and the
LOGICAL volume root's `bootingInfo` (37B) + `rootFile` (125B) arrays, which
Pilot's soft boot (BootTool's herald volume buttons, `Booting.Boot`,
RollBack) resolves instead; a real Othello install writes both via
`File.SetRoot` (FileImpl.mesa `RecordRootFile`).  Volumes built before
2026-07-15 have only the PV records, so clicking a herald boot button
raised an uncaught `File.Error` from FileImpl.  Fixed twice over:
`pilot::install_boot_file` now writes the LV records on fresh builds, and
`python3 tools/pdi_install_lv_bootfiles.py image.pdi` retrofits an existing
image in place (idempotent; copies the PV DiskFileID words verbatim so the
committed images' flat-VDA `firstLink` convention is preserved — do NOT
re-encode to CHS, that recreates the 2e8018b cold-boot regression).  The
committed `CedarDorado-work.pdi.gz` carries the LV records.

`Basic.Loadees` is only the command file: its 33 BCD dependencies must also be
available below the STP root.  Recreate that rooted release tree from the local
CHM index with `python3 tools/fetch_cedar_loadees.py`; the script downloads the
archive revisions named by the index into `chm/cedar/stp-root/Cedar6.1/`.
