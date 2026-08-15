#!/usr/bin/env python3
"""Plan and fetch the release-separated Lisp gaps from the CHM archive.

The CHM cross-reference contains several copies of the same Interlisp trees.
This tool deliberately chooses the release roots rather than merging every
basename that happens to contain ``koto`` or ``lyric``.  Files are preserved
under ``chm/lisp-archive/<release>/<root>/`` and are never added to an active
Lyric/Koto FTP root automatically.

Examples::

    python3 tools/fetch_lisp_gap.py --plan
    python3 tools/fetch_lisp_gap.py --fetch --release medley
    python3 tools/fetch_lisp_gap.py --fetch --release lyric,koto

The fetch is resumable: completed files are size/CRC checked and skipped.
The archive data and manifest are intentionally scratch artifacts; use the
small summary it prints for a checked-in inventory, not the 700+ MB payload.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shutil
import sys
import urllib.parse
import urllib.request
import zlib
from collections import OrderedDict
from pathlib import Path

from build_chm_atlas_inventory import iter_entries


BASE = "https://xeroxparcarchive.computerhistory.org/"
DEFAULT_CROSS_REFERENCE = Path("chm/cross-reference.html")
DEFAULT_DEST = Path("chm/lisp-archive")
DEFAULT_MANIFEST = DEFAULT_DEST / "manifest.jsonl"
UA = {"User-Agent": "dorado-lisp-gap-fetch/1.0"}

# These are the release roots used for the gap report.  Do not add the eris
# mirrors when a phylum release root exists: they are often duplicate copies
# with different version suffixes.  Lyric's font roots are included because
# they are part of the usable Lyric release surface and account for the
# otherwise surprising size/count difference in the survey.
ROOTS = OrderedDict(
    [
        (
            "lyric",
            [
                ("lisp", "phylum/lisp/lyric"),
                ("lispusers", "phylum/lispusers/lyric"),
                ("lisplibrary", "phylum/lisplibrary/lyric"),
                ("lyricfonts", "eris/lispcore/lyricfonts"),
            ],
        ),
        (
            "koto",
            [
                ("lisp", "phylum/lisp/koto"),
                ("lispusers", "phylum/lispusers/koto"),
                ("lisplibrary", "phylum/lisplibrary/koto"),
            ],
        ),
        ("harmony", [("lisp", "eris/lisp/harmony")]),
        ("intermezzo", [("lisp", "eris/lisp/intermezzo")]),
        ("carol", [("lisp", "eris/lisp/carol")]),
        ("fugue.6", [("lisp", "eris/lisp/fugue.6")]),
        ("medley", [("medley", "phylum/medley")]),
    ]
)


def crc32_file(path: Path) -> str:
    value = 0
    with path.open("rb") as stream:
        while True:
            chunk = stream.read(1024 * 1024)
            if not chunk:
                break
            value = zlib.crc32(chunk, value)
    return f"{value & 0xFFFFFFFF:08x}"


def index_local(root: Path):
    by_key = {}
    for path in sorted(p for p in root.rglob("*") if p.is_file()):
        try:
            size = path.stat().st_size
            if not size:
                continue
            by_key.setdefault((size, crc32_file(path)), path)
        except OSError as exc:
            print(f"warning: cannot index {path}: {exc}", file=sys.stderr)
    return by_key


def selected_records(cross_reference: Path, releases: set[str]):
    records = []
    seen = set()
    for entry in iter_entries(cross_reference):
        source = f"{entry['cross_reference_path']}/{entry['filename']}"
        lower = entry["cross_reference_path"].casefold()
        for release in releases:
            for root_name, prefix in ROOTS[release]:
                pfx = prefix.casefold()
                if lower != pfx and not lower.startswith(pfx + "/"):
                    continue
                key = (release, source)
                if key in seen:
                    break
                seen.add(key)
                relative = source[len(prefix) :].lstrip("/")
                records.append(
                    {
                        "release": release,
                        "root": root_name,
                        "prefix": prefix,
                        "source": source,
                        "relative": relative,
                        "url": BASE + urllib.parse.quote(source, safe="/!._-"),
                        "bytes": entry["bytes"],
                        "crc32": entry["crc32"].lower().zfill(8),
                    }
                )
                break
            else:
                continue
            break
    return sorted(records, key=lambda r: (r["release"], r["source"]))


def destination(root: Path, record: dict) -> Path:
    return root / record["release"] / record["root"] / record["relative"]


def valid(path: Path, record: dict) -> bool:
    try:
        size = path.stat().st_size
        if size == record["bytes"]:
            return crc32_file(path) == record["crc32"]
        # A few old Intermezzo cross-reference rows have stale sizes and
        # CRCs, while the live raw file is still a valid non-HTML response.
        # Keep those accepted files resumable; the fetch log records them as
        # fetch* when they are first encountered.
        with path.open("rb") as stream:
            prefix = stream.read(64).lstrip().lower()
        return bool(size) and not prefix.startswith((b"<!doctype html", b"<html"))
    except OSError:
        return False


def fetch(url: str) -> bytes:
    request = urllib.request.Request(url, headers=UA)
    with urllib.request.urlopen(request, timeout=120) as response:
        return response.read()


def resolve_case(source: str, cache_dir: Path) -> str:
    """Restore the original case for expanded ``.dm!version_`` components.

    The CHM cross-reference lowercases those components, but the live IFS
    server does not.  The parent directory index has the authoritative href
    (for example ``LISP0MC.DM!1_/.index.html``), so use it as a small cached
    case map before requesting a member.
    """
    parts = source.split("/")
    expanded = False
    for index, component in enumerate(parts):
        is_expanded_container = "!" in component and component.endswith("_")
        if not expanded and not is_expanded_container:
            continue
        parent = "/".join(parts[:index])
        cache = cache_dir / (hashlib.sha1(parent.encode()).hexdigest() + ".html")
        if cache.exists():
            listing = cache.read_text(encoding="utf-8", errors="replace")
        else:
            url = BASE + urllib.parse.quote(parent, safe="/!._-") + "/.index.html"
            listing = fetch(url).decode("utf-8", "replace")
            cache_dir.mkdir(parents=True, exist_ok=True)
            cache.write_text(listing, encoding="utf-8")
        hrefs = []
        for href in re.findall(r'href="([^"]+)"', listing):
            candidate = href.removesuffix("/.index.html")
            if candidate.casefold() == component.casefold():
                hrefs.append(candidate)
        if len(hrefs) != 1:
            raise ValueError(
                f"could not resolve CHM case for {component!r} under {parent!r}"
            )
        parts[index] = hrefs[0]
        expanded = expanded or is_expanded_container
    return "/".join(parts)


def listing_for(parent: str, cache_dir: Path) -> str:
    cache = cache_dir / (hashlib.sha1(parent.encode()).hexdigest() + ".html")
    if cache.exists():
        return cache.read_text(encoding="utf-8", errors="replace")
    url = BASE + urllib.parse.quote(parent, safe="/!._-") + "/.index.html"
    listing = fetch(url).decode("utf-8", "replace")
    cache_dir.mkdir(parents=True, exist_ok=True)
    cache.write_text(listing, encoding="utf-8")
    return listing


def live_size(source: str, cache_dir: Path) -> int | None:
    """Read a member size from the archive's live directory listing."""
    parent, member = source.rsplit("/", 1)
    listing = listing_for(parent, cache_dir)
    escaped = re.escape(member)
    pattern = re.compile(
        rf'href="\.{escaped}\.html".*?href="{escaped}"\s*>'
        rf'\(Original\)</a>\s+(\d+)',
        re.S | re.I,
    )
    match = pattern.search(listing)
    return int(match.group(1)) if match else None


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--cross-reference", type=Path, default=DEFAULT_CROSS_REFERENCE)
    parser.add_argument("--dest", type=Path, default=DEFAULT_DEST)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--release", default="all", help="comma-separated release names")
    parser.add_argument("--plan", action="store_true", help="print the plan without downloading")
    parser.add_argument("--fetch", action="store_true", help="download missing files")
    parser.add_argument("--no-reuse", action="store_true", help="do not copy matching local bytes")
    parser.add_argument("--delay", type=float, default=0.03)
    parser.add_argument("--jobs", type=int, default=8, help="parallel archive requests")
    args = parser.parse_args()
    if args.plan == args.fetch:
        parser.error("choose exactly one of --plan or --fetch")

    if args.release == "all":
        releases = set(ROOTS)
    else:
        releases = {item.strip() for item in args.release.split(",") if item.strip()}
        unknown = releases - set(ROOTS)
        if unknown:
            parser.error("unknown release(s): " + ", ".join(sorted(unknown)))

    records = selected_records(args.cross_reference, releases)
    manifest_records = records
    if args.release != "all" and args.manifest.exists():
        existing = [
            json.loads(line)
            for line in args.manifest.read_text(encoding="utf-8").splitlines()
            if line.strip()
        ]
        merged = {(r["release"], r["source"]): r for r in existing}
        merged.update({(r["release"], r["source"]): r for r in records})
        manifest_records = sorted(merged.values(), key=lambda r: (r["release"], r["source"]))
    args.manifest.parent.mkdir(parents=True, exist_ok=True)
    with args.manifest.open("w", encoding="utf-8") as stream:
        for record in manifest_records:
            stream.write(json.dumps(record, separators=(",", ":")) + "\n")

    print(f"manifest: {args.manifest}")
    total_bytes = sum(record["bytes"] for record in records)
    print(f"selected: {len(records):,} files, {total_bytes / 1_000_000:.1f} MB")
    for release in ROOTS:
        subset = [r for r in records if r["release"] == release]
        if subset:
            print(
                f"  {release:10s} {len(subset):5,} files "
                f"{sum(r['bytes'] for r in subset) / 1_000_000:7.1f} MB"
            )
    if args.plan:
        return 0

    local_index = {} if args.no_reuse else index_local(Path("chm"))
    case_cache = args.dest / ".index-cache"

    def process(item):
        number, record = item
        out = destination(args.dest, record)
        if valid(out, record):
            return number, "skip", record["source"], None
        out.parent.mkdir(parents=True, exist_ok=True)
        source = local_index.get((record["bytes"], record["crc32"]))
        try:
            if source is not None:
                shutil.copyfile(source, out)
                action = "reuse"
            else:
                resolved = resolve_case(record["source"], case_cache)
                data = fetch(BASE + urllib.parse.quote(resolved, safe="/!._-"))
                metadata_mismatch = False
                if len(data) != record["bytes"]:
                    authoritative = live_size(resolved, case_cache)
                    raw_is_html = data.lstrip().lower().startswith(
                        (b"<!doctype html", b"<html")
                    )
                    if authoritative != len(data) and raw_is_html:
                        raise ValueError(
                            f"size {len(data)} != cross-reference {record['bytes']} "
                            f"and live index {authoritative}"
                        )
                    metadata_mismatch = True
                if not metadata_mismatch and f"{zlib.crc32(data) & 0xFFFFFFFF:08x}" != record["crc32"]:
                    raise ValueError("CRC32 mismatch")
                temp = out.with_name(out.name + ".part")
                temp.write_bytes(data)
                os.replace(temp, out)
                action = "fetch*" if metadata_mismatch else "fetch"
            return number, action, record["source"], None
        except Exception as exc:  # noqa: BLE001
            return number, "error", record["source"], exc

    fetched = reused = skipped = failed = 0
    jobs = max(1, min(args.jobs, 32))
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        futures = [pool.submit(process, item) for item in enumerate(records, 1)]
        for future in concurrent.futures.as_completed(futures):
            number, action, source, error = future.result()
            if action in ("fetch", "fetch*"):
                fetched += 1
            elif action == "reuse":
                reused += 1
            elif action == "skip":
                skipped += 1
            else:
                failed += 1
            if error is None:
                print(f"[{number}/{len(records)}] {action:5s} {source}", flush=True)
            else:
                print(f"[{number}/{len(records)}] ERROR {source}: {error}", file=sys.stderr)

    print(f"done: fetched={fetched} reused={reused} skipped={skipped} failed={failed}")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
