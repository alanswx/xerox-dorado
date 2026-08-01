#!/bin/sh
# check_cedar_ls_gate.sh LOG
#
# Decide whether a verify-cedar-ls run passed, from its trace log.
#
# The bug this gate exists for: our STP server had no Enumerate. Cedar sent a
# correct New-Directory (mark 0o14), the server treated it as a one-file
# Retrieve of the literal path "Cedar6.1/VersionMap/*", found nothing, and
# answered markNo -- and the client printed NOTHING, not an error, because
# FSRemoteFileImpl.InnerEnumerate swallows noSuchFile ("EXITS NotFound =>
# {}"). A missing directory and an empty one are indistinguishable to the
# user, so "the command ran without an error" proves nothing here.
#
# Two independent signals, one from each end of the wire:
#
#   STP_ENUM        the server matched the pattern and produced entries.
#                   Server-side only: it says what we sent, not what Cedar
#                   made of it.
#   STP_LOOKUP      the guest asked us to look up files BY NAME afterwards.
#                   This is the client-side half: FSRemoteFileImpl.Info runs
#                   the LookupFile packet exchange per file to fill List's
#                   version/size/date columns, and it can only name files it
#                   read out of our listing. A server that answers an
#                   enumeration with nothing can never produce these lines.
#
# Deliberately NOT a pixel count. The CommandTool typescript is already full
# of text, so a listing SCROLLS rather than adding ink: measured 2026-07-30,
# 167,129 px with the 11-file listing on screen against 167,192 px for a
# listing of a directory that does not exist. The count moves less than the
# length of the echoed command.

log="$1"

if [ ! -r "$log" ]; then
    echo "FAIL: no log at $log"
    exit 1
fi

fail=0
want_names="CedarSource.VersionMap CedarSymbols.VersionMap VersionMap.mesa
            MakeVersionMap.cm"

# 1. The server enumerated the pattern.
enum=$(sed -n 's/.*STP_ENUM <Cedar6\.1>VersionMap>\* -> \([0-9][0-9]*\) files.*/\1/p' \
       "$log" | tail -1)
if [ -z "$enum" ]; then
    echo "FAIL: the server never enumerated <Cedar6.1>VersionMap>*."
    echo "      Cedar's New-Directory (mark 0o14) went unanswered or was"
    echo "      handled as a Retrieve -- look for STP_ENUM_MISSING in $log."
    fail=1
elif [ "$enum" -ne 11 ]; then
    echo "FAIL: enumerated $enum files, expected 11 (the VersionMap directory)"
    fail=1
fi

# 2. Every name we expect appears in an entry we actually transmitted.
for n in $want_names; do
    if ! grep -q "STP_ENUM_ENTRY .*Name-Body $n)" "$log"; then
        echo "FAIL: no listing entry for $n"
        fail=1
    fi
done

# 3. The guest read those names back to us. NOT all 11, and the exact count is
#    not stable: a file already in the checkpoint's local FS cache needs no
#    remote check, so it varies with which checkpoint is restored (9 on the
#    2026-07-30 desktop, 7 on the 2026-08-01 one that also installs Sil).
#    What the check is really for is "did the listing reach the guest at all",
#    and any lookup at all proves that -- a server that answers an enumeration
#    with nothing can produce none. Keep a margin above zero, not a majority.
looks=$(grep -c "STP_LOOKUP Cedar6\.1/VersionMap/" "$log" 2>/dev/null || true)
[ -z "$looks" ] && looks=0
if [ "$looks" -lt 5 ]; then
    echo "FAIL: the guest looked up only $looks of the listed names (want >= 5)."
    echo "      It can only ask for names it read out of our listing, so this"
    echo "      is the half of the gate that proves Cedar received one."
    fail=1
fi

# 4. Nothing the served tree should have was missing.
miss=$(grep -c "STP_ENUM_MISSING\|STP_LOOKUP_MISSING" "$log" 2>/dev/null || true)
[ -z "$miss" ] && miss=0
if [ "$miss" -ne 0 ]; then
    echo "FAIL: $miss enumerate/lookup(s) the served tree could not answer:"
    grep "STP_ENUM_MISSING\|STP_LOOKUP_MISSING" "$log" | sort -u | sed 's/^/      /'
    fail=1
fi

# 5. A listing truncated at the match cap is not a complete listing.
if grep -q "STP_ENUM TRUNCATED" "$log"; then
    echo "FAIL: the walk hit its match cap; the listing was incomplete"
    fail=1
fi

# 6. The run has to have got as far as painting a screen at all.
px=$(sed -n 's/.*dorado: \([0-9][0-9]*\) display-list pixels.*/\1/p' "$log" \
     | tail -1)
if [ -z "$px" ] || [ "$px" -lt 100000 ]; then
    echo "FAIL: the desktop is not up (${px:-no} pixels) -- the listing never ran"
    fail=1
fi

if [ "$fail" -eq 0 ]; then
    echo "PASS: listed $enum files; the guest looked up $looks of them by name"
fi
exit "$fail"
