# Interlisp-D release gap mirror

The CHM cross-reference contains many duplicate `eris`/`phylum` views of the
same Lisp releases. The reusable fetcher
[`tools/fetch_lisp_gap.py`](../tools/fetch_lisp_gap.py) selects one canonical
set of release roots, preserves the archive paths, checks sizes/CRC32 where
the CHM metadata is consistent, and resumes without touching an active Lisp
FTP root.

The fetched data lives under `chm/lisp-archive/`, which is deliberately
ignored by Git. It is an archive cache, not a package set to insert blindly
into Lyric or Koto. The manifest is `chm/lisp-archive/manifest.jsonl`.

## Canonical expansion

| release | files | indexed bytes | selected roots |
|---|---:|---:|---|
| Lyric | 2,701 | 84.9 MB | `phylum/lisp/lyric`, `phylum/lispusers/lyric`, `phylum/lisplibrary/lyric`, Lyric fonts |
| Koto | 5,083 | 99.4 MB | `phylum/lisp/koto`, `phylum/lispusers/koto`, `phylum/lisplibrary/koto` |
| Harmony | 1,698 | 78.4 MB | `eris/lisp/harmony` |
| Intermezzo | 1,158 | 52.3 MB | `eris/lisp/intermezzo` |
| Carol | 820 | 35.9 MB | `eris/lisp/carol` |
| Fugue.6 | 226 | 17.9 MB | `eris/lisp/fugue.6` |
| Medley | 22 | 11.2 MB | `phylum/medley` |
| **Total** | **11,708** | **380.1 MB** | duplicate mirrors excluded |

The counts differ from the broad CHM report because that report counts
additional mirrors and approximate basename matches. This mirror is the
useful set for source archaeology and release-specific image work; it avoids
making the same file appear several times under different archive hosts.

## Commands

```sh
cd /Users/alans/Documents/development/Dorado

# Rebuild the plan without downloading anything.
python3 tools/fetch_lisp_gap.py --plan

# Resume the complete canonical expansion.
python3 tools/fetch_lisp_gap.py --fetch --jobs 8 --delay 0

# Fetch or refresh one release only.
python3 tools/fetch_lisp_gap.py --fetch --release medley
```

The CHM cross-reference lowercases expanded `.dm!version_` paths, while the
live archive is case-sensitive. The fetcher resolves both the container and
member spelling from the parent `.index.html`; this is why the first naïve
download showed hundreds of false 404s. A handful of 1985 Intermezzo rows
also have stale cross-reference sizes, so valid non-HTML raw responses are
marked `fetch*` and retained after direct retrieval.

The next integration step is selective: choose release-matched compiled
files (`.LCOM` for Lyric, `.DCOM` for Koto/Harmony-era systems) and their
document/font companions for separate image experiments. Do not point the
working Lyric or Koto FTP roots at this whole archive.
