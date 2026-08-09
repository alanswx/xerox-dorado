#!/bin/sh
# Make an isolated Cedar PDI and replace User.Profile with a minimal,
# CR-terminated profile for cold-boot package/version experiments.

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)
DORADO="$ROOT/dorado"

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
    cat <<EOF
Usage: $0 [SOURCE_PDI] [TARGET_PDI]

Defaults:
  source: $ROOT/CedarDisk/CedarDorado-work.pdi
  target: $DORADO/build/good-packs/cedar-clean-gargoyle-cold.pdi

CEDAR_PROFILE may override the User.Profile path.
EOF
    exit 0
fi

SOURCE_PDI=${1:-"$ROOT/CedarDisk/CedarDorado-work.pdi"}
TARGET_PDI=${2:-"$DORADO/build/good-packs/cedar-clean-gargoyle-cold.pdi"}
PROFILE=${CEDAR_PROFILE:-"$ROOT/chm/cedar/stp-root/Cedar6.1/Top/User.Profile"}

if [ ! -f "$SOURCE_PDI" ]; then
    echo "source PDI not found: $SOURCE_PDI" >&2
    exit 1
fi
if [ -e "$TARGET_PDI" ]; then
    echo "refusing to overwrite existing target PDI: $TARGET_PDI" >&2
    exit 1
fi
if [ ! -f "$PROFILE" ]; then
    echo "profile not found: $PROFILE" >&2
    exit 1
fi

SOURCE_PDI="$SOURCE_PDI" python3 - <<'PY'
import os
from pathlib import Path

p = Path(os.environ["SOURCE_PDI"])
if b"CedarChest6.0" in p.read_bytes():
    raise SystemExit(
        f"refusing polluted source PDI containing CedarChest6.0: {p}"
    )
PY

mkdir -p "$(dirname -- "$TARGET_PDI")"
cp -p "$SOURCE_PDI" "$TARGET_PDI"

STAMP=$(date +%Y%m%d-%H%M%S)
BACKUP="$PROFILE.before-clean.$STAMP"
cp -p "$PROFILE" "$BACKUP"

PROFILE="$PROFILE" python3 - <<'PY'
import os
from pathlib import Path

profile = Path(os.environ["PROFILE"])
text = (
    "VersionMap.SourceMaps: /Cedar/Cedar6.1/VersionMap/CedarSource.VersionMap /Cedar/CedarChest6.1/VersionMap/CedarChestSource.VersionMap\r"
    "VersionMap.SymbolsMaps: /Cedar/Cedar6.1/VersionMap/CedarSymbols.VersionMap /Cedar/CedarChest6.1/VersionMap/CedarChestSymbols.VersionMap\r"
    'CommandTool.BootCommands: "\r"\r'
    'CommandTool.PerLogin: "\r"\r'
    'CommandTool.NewUser: "\r"\r'
    'CommandTool.PerCommandTool: "\r"\r'
)
profile.write_bytes(text.encode("ascii"))
PY

PROFILE="$PROFILE" python3 - <<'PY'
import os
from pathlib import Path

b = Path(os.environ["PROFILE"]).read_bytes()
if b"\n" in b or b"CedarChest6.0" in b:
    raise SystemExit("profile validation failed")
print(f"profile validated: {len(b)} bytes, {b.count(bytes([13]))} CR terminators")
PY

echo "created PDI:       $TARGET_PDI"
echo "profile backup:    $BACKUP"
echo "active profile:    $PROFILE"
echo
echo "Restore the profile with:"
echo "  cp -p '$BACKUP' '$PROFILE'"
