# Atlas overview

This is the human-facing view of the initial Atlas seed. The structured
records are in the neighboring JSONL files.

## Current environment map

| Environment | Core inputs | Media or transport | Current status |
|---|---|---|---|
| Alto/Mesa on Dorado | `AltoMesaDorado.eb!2`, Initial ether wrapper | Pup/EFTP and Alto/Trident media paths | Working; games and Mesa worlds validated |
| Cedar 6.1 desktop | `CedarDorado.eb!6`, `Dorado.germ-6.1.6`, Cedar 6.1 boot file, Pilot volume | PDI/Pilot volume plus STP/EFTP | Working; Viewers desktop reached |
| Smalltalk-76 | `SmalltalkDorado.eb!1` / `DSemu.mb!1` | XM Smalltalk pack | Working; desktop and interpreter reached |
| Interlisp-D Lyric | `DORADOLISPMC.EB!1`, `LISP.SYSOUT!1` | Retrieve plus multi-partition disk/swap path | Exec/XCL reached; recipe incomplete |

## Most important compatibility fact

The Cedar 6.1 path requires the matched `Dorado.germ-6.1.6`. The older
`Dorado.germ!4` is not an interchangeable substitute for
`CedarDorado.eb!6`.

## What is already recipe-grade

- Cedar 6.1 work-volume desktop: [`recipe.cedar-6.1-work-desktop`](recipes.jsonl).
- Cedar 6.1 login volume: [`recipe.cedar-6.1-login-volume`](recipes.jsonl).
- Alto games: [`recipe.alto-games-trident`](recipes.jsonl).
- Smalltalk-76 desktop: [`recipe.smalltalk-76-desktop`](recipes.jsonl), now
  linked to both the `xmsmall.dsk` source and generated pack.

Smalltalk and Interlisp have working or partially working software paths,
but their exact media provenance is still a first-class Atlas gap.

## Immediate next population pass

1. Add exact checksums and byte sizes for the seeded artifacts.
2. Add checksums and exact conversion provenance for the Smalltalk and Lyric
   pack fixtures.
3. Reconcile superseded Cedar gap claims into one release ledger.
4. Add the raw inventory snapshot and then expand the artifact seed from
   boot-critical items to package and source closure.
