# Atlas record schema

This is a documentation schema for the JSONL records in this directory. It
is intentionally small enough to edit by hand during the seed phase.

## Artifact record

Required conceptual fields:

```json
{
  "id": "stable unique identifier",
  "identity": {
    "server": "CHM IFS server or local",
    "directory": "original directory path",
    "name": "original basename",
    "version": 1,
    "normalized": "case-folded lookup identity"
  },
  "local_path": "path relative to repository root, if present",
  "kind": ["microcode", "source"],
  "roles": ["microcode-world"],
  "systems": ["Cedar"],
  "targets": ["Dorado"],
  "status": "observed-successfully",
  "confidence": "high",
  "evidence": ["docs/..."],
  "notes": "short interpretation"
}
```

Optional fields include `bytes`, `sha256`, `archive_date`, `raw_url`,
`index_url`, `view_url`, `container`, `format`, `geometry`, `authors`, and
`as_of`.

## Remote inventory record

`remote-inventory.jsonl` is occurrence-oriented rather than semantic. It has
one record for each file occurrence parsed from the CHM cross-reference,
including duplicate copies in different servers or releases. In addition to
the archive identity and CHM metadata, it carries:

- `cross_reference_path` and `raw_url`;
- `source_path`, the relative archive path used by the index;
- `mirror_status`;
- `local_matches` and `local_match_count`;
- lightweight filename/extension classification in `kind`.

The mirror status is intentionally conservative:

- `mirrored-exact-name-and-content`: local basename, size, and CRC32 agree;
- `mirrored-same-content-different-name`: local size and CRC32 agree, but the
  local basename/path differs;
- `same-name-and-size-crc-differs`: a likely candidate exists but does not
  match the cross-reference CRC32;
- `not-mirrored-or-unmatched`: no matching local candidate was found.

This is an inventory comparison, not a proof of archival absence. The local
mirror may contain transformed, regenerated, or renamed artifacts whose
metadata does not match. Preserve the original CHM occurrence in all cases.

Controlled vocabulary is preferred but not mandatory during curation.

### Artifact kinds

`source`, `microcode`, `boot-loader`, `germ`, `boot-file`, `os-image`,
`disk-image`, `filesystem-volume`, `package-manifest`, `runtime`, `object`,
`executable`, `font`, `data`, `diagnostic`, `documentation`, `container`,
`local-derivative`.

### Artifact roles

`bootstrap`, `initial-loader`, `microcode-world`, `im-loader`, `os-boot`,
`pilot-germ`, `pilot-volume`, `alto-boot`, `sysout`, `swap-volume`,
`runtime-closure`, `network-service`, `package`, `font-install`,
`validation-fixture`, `format-reference`, `source-of-truth`.

### Status values

- `observed-successfully`: exercised by the emulator or a documented tool.
- `present-unclassified`: local or remote artifact recorded, interpretation
  incomplete.
- `candidate`: plausible input, not yet validated.
- `source-only`: useful source, not itself loadable media.
- `missing-scoped`: absent from a specified search scope.
- `superseded`: retained for history, not the preferred input.
- `incompatible`: known not to combine with a specified generation.

## Relationship record

```json
{
  "from": "artifact-or-release-id",
  "relation": "compatible-with",
  "to": "artifact-or-release-id",
  "confidence": "high",
  "evidence": ["docs/..."],
  "notes": "why this edge exists"
}
```

Useful relations are `contains`, `same-bytes-as`, `later-version-of`,
`source-for`, `generates`, `loads`, `requires`, `compatible-with`,
`incompatible-with`, `installed-by`, `bootstraps`, `runs-on`, and
`validated-by`.

## Recipe record

A recipe is not just a list of files. It records a target environment and an
ordered boot or construction procedure.

```json
{
  "id": "stable-recipe-id",
  "goal": "what should appear or become usable",
  "system": "software generation",
  "target": "machine",
  "inputs": [{"artifact": "id", "role": "pilot-germ"}],
  "boot_path": ["..."],
  "media": {"format": "...", "geometry": "..."},
  "network": {"required": true, "services": ["STP"]},
  "status": "candidate",
  "validation": {"gate": "...", "evidence": ["docs/..."]},
  "known_incompatibilities": ["..."],
  "open_questions": ["..."]
}
```

## Gap record

Every absence or uncertainty must be scoped.

```json
{
  "id": "gap-id",
  "scope": "servers/directories/search covered",
  "claim": "what is missing or uncertain",
  "impact": "why it matters",
  "checked_as_of": "YYYY-MM-DD",
  "evidence": ["docs/..."],
  "next_action": "specific archival or emulator investigation",
  "confidence": "medium"
}
```
