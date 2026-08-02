#!/usr/bin/env python3
"""Build the CHM remote inventory and the file://-friendly viewer seed.

This is deliberately a small, dependency-free archival utility.  It reads
the checked-in CHM cross-reference, compares each occurrence with files under
the local chm/ mirror by basename/size/CRC, and writes one JSON object per
remote occurrence.  It does not download anything and does not modify the
mirror.
"""

from __future__ import annotations

import argparse
import datetime as dt
import html
import json
import re
import sys
import zlib
from collections import Counter, defaultdict
from pathlib import Path


ENTRY_RE = re.compile(
    r'<li><a href="(?P<href>[^"]+)/\.index\.html">'
    r'(?P<display>.*?)</a>(?P<tail>.*?)</li>'
)


def crc32_file(path: Path) -> str:
    value = 0
    with path.open("rb") as stream:
        while True:
            chunk = stream.read(1024 * 1024)
            if not chunk:
                break
            value = zlib.crc32(chunk, value)
    return f"{value & 0xffffffff:08x}"


def local_index(root: Path):
    by_name = defaultdict(list)
    by_content = defaultdict(list)
    files = 0
    for path in sorted(p for p in root.rglob("*") if p.is_file()):
        try:
            size = path.stat().st_size
            crc = crc32_file(path)
        except OSError as exc:
            print(f"warning: cannot read {path}: {exc}", file=sys.stderr)
            continue
        record = {"path": path.as_posix(), "bytes": size, "crc32": crc}
        by_name[path.name.casefold()].append(record)
        if size and crc != "00000000":
            by_content[(size, crc)].append(record)
        files += 1
    return by_name, by_content, files


def parse_entry(match: re.Match[str]):
    href = match.group("href")
    display = html.unescape(match.group("display"))
    parts = match.group("tail").split()
    if len(parts) < 7:
        return None

    filename, size, crc, date, time, timezone, file_type = parts[:7]
    rest = parts[7:]
    archive_year = None
    archive_tape = None
    archive_record = None
    author_parts = rest
    if len(rest) >= 3 and rest[-3].isdigit() and rest[-2].isdigit():
        archive_year, archive_tape, archive_record = rest[-3:]
        author_parts = rest[:-3]
    elif len(rest) >= 2 and rest[-2].isdigit() and rest[-1].isdigit():
        archive_year, archive_tape = rest[-2:]
        author_parts = rest[:-2]

    server_match = re.match(r"\[(.*?)\]<(.*)>", display)
    if server_match:
        server = server_match.group(1)
        directory = server_match.group(2)
    else:
        server = None
        directory = display

    return {
        "filename": filename,
        "archive_path": f"[{server}]<{directory}>{filename}" if server else f"<{directory}>{filename}",
        "server": server,
        "directory": directory,
        "cross_reference_path": href,
        "raw_url": f"https://xeroxparcarchive.computerhistory.org/{href}/{filename}",
        "bytes": int(size),
        "crc32": crc.lower(),
        "created": f"{date} {time} {timezone}",
        "file_type": file_type,
        "author": " ".join(author_parts) if author_parts else None,
        "archive_year": int(archive_year) if archive_year else None,
        "archive_tape": int(archive_tape) if archive_tape else None,
        "archive_record": archive_record,
    }


def classify(filename: str, file_type: str):
    lower = filename.casefold()
    suffix = lower.rsplit(".", 1)[-1] if "." in lower else ""
    kinds = []
    if suffix in {"dm"}:
        kinds += ["container", "dump-load"]
    elif suffix in {"bfs", "altodisk", "copydisk", "disk"}:
        kinds += ["container", "disk-image"]
    elif suffix in {"eb", "pb", "boot", "bootmesa"}:
        kinds += ["boot-artifact"]
    elif suffix in {"mb", "mc", "mu", "midas"}:
        kinds += ["microcode"]
    elif suffix in {"mesa", "bcpl", "br", "bcd", "cm", "d", "decl", "asm", "masm"}:
        kinds += ["source-or-object"]
    elif suffix in {"df", "load", "config", "profile", "profileslice"}:
        kinds += ["manifest-or-configuration"]
    elif suffix in {"press", "tioga", "bravo", "txt", "memo", "pdf"}:
        kinds += ["documentation"]
    else:
        kinds += [file_type.casefold() if file_type else "unclassified"]
    if "dorado" in lower:
        kinds.append("dorado-candidate")
    if "cedar" in lower:
        kinds.append("cedar-candidate")
    if "smalltalk" in lower or "dsemu" in lower:
        kinds.append("smalltalk-candidate")
    if "lisp" in lower or "lyric" in lower:
        kinds.append("interlisp-candidate")
    return kinds


def make_record(parsed, by_name, by_content, ordinal):
    name_matches = by_name.get(parsed["filename"].casefold(), [])
    exact_name = [
        item for item in name_matches
        if item["bytes"] == parsed["bytes"] and parsed["crc32"] != "0"
        and item["crc32"] == parsed["crc32"]
    ]
    content_matches = by_content.get((parsed["bytes"], parsed["crc32"]), []) if parsed["crc32"] != "0" else []
    if exact_name:
        status = "mirrored-exact-name-and-content"
        matches = exact_name
    elif content_matches:
        status = "mirrored-same-content-different-name"
        matches = content_matches
    elif any(item["bytes"] == parsed["bytes"] for item in name_matches):
        status = "same-name-and-size-crc-differs"
        matches = name_matches
    else:
        status = "not-mirrored-or-unmatched"
        matches = []

    source_path = f"{parsed['cross_reference_path']}/{parsed['filename']}"
    record = {
        "id": f"chm.remote.{ordinal:07d}",
        **parsed,
        "source_path": source_path,
        "kind": classify(parsed["filename"], parsed["file_type"]),
        "mirror_status": status,
        "local_matches": [item["path"] for item in matches[:12]],
        "local_match_count": len(matches),
        "inventory_as_of": dt.date.today().isoformat(),
    }
    return record


def iter_entries(path: Path):
    with path.open("r", encoding="utf-8", errors="replace") as stream:
        for line in stream:
            match = ENTRY_RE.search(line)
            if match:
                parsed = parse_entry(match)
                if parsed:
                    yield parsed


def load_seed(atlas_dir: Path):
    result = {}
    for name in ("artifacts", "releases", "relationships", "recipes", "gaps"):
        records = []
        with (atlas_dir / f"{name}.jsonl").open("r", encoding="utf-8") as stream:
            for line in stream:
                if line.strip():
                    records.append(json.loads(line))
        result[name] = records
    return result


def write_seed_js(path: Path, seed):
    path.parent.mkdir(parents=True, exist_ok=True)
    payload = json.dumps(seed, ensure_ascii=False, separators=(",", ":"))
    path.write_text(f"window.ATLAS_SEED = {payload};\n", encoding="utf-8")


def open_remote_js(path: Path):
    path.parent.mkdir(parents=True, exist_ok=True)
    stream = path.open("w", encoding="utf-8")
    stream.write("window.ATLAS_REMOTE = [\n")
    return stream


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--cross-reference", type=Path, default=Path("chm/cross-reference.html"))
    parser.add_argument("--local-root", type=Path, default=Path("chm"))
    parser.add_argument("--atlas-dir", type=Path, default=Path("docs/chm-atlas"))
    parser.add_argument("--output", type=Path, default=Path("docs/chm-atlas/remote-inventory.jsonl"))
    parser.add_argument("--summary-output", type=Path, default=Path("docs/chm-atlas/inventory-summary.json"))
    parser.add_argument("--seed-output", type=Path, default=Path("docs/chm-atlas/viewer/seed.js"))
    parser.add_argument("--remote-js-output", type=Path, default=Path("docs/chm-atlas/viewer/remote-inventory.js"))
    args = parser.parse_args()

    print(f"indexing local files under {args.local_root}...", file=sys.stderr)
    by_name, by_content, local_count = local_index(args.local_root)
    print(f"indexed {local_count} local files; parsing {args.cross_reference}...", file=sys.stderr)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    remote_js = open_remote_js(args.remote_js_output)
    counts = Counter()
    total = 0
    try:
        with args.output.open("w", encoding="utf-8") as output:
            for total, parsed in enumerate(iter_entries(args.cross_reference), 1):
                record = make_record(parsed, by_name, by_content, total)
                counts[record["mirror_status"]] += 1
                encoded = json.dumps(record, ensure_ascii=False, separators=(",", ":"))
                output.write(encoded + "\n")
                remote_js.write(encoded + ",\n")
    finally:
        remote_js.write("]\n")
        remote_js.close()

    summary = {
        "cross_reference": str(args.cross_reference),
        "local_root": str(args.local_root),
        "inventory_as_of": dt.date.today().isoformat(),
        "remote_occurrences": total,
        "local_files_checked": local_count,
        "mirror_status_counts": dict(sorted(counts.items())),
        "notes": [
            "One record represents one CHM archive occurrence, not one deduplicated byte string.",
            "Mirror matching is metadata-based: basename, size, and cross-reference CRC32.",
            "A same-content match may be a copied release or a renamed local artifact; preserve the archive path.",
        ],
    }
    args.summary_output.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    write_seed_js(args.seed_output, load_seed(args.atlas_dir))
    print(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
