#!/bin/sh
# check_lisp_leaf_gate.sh LOG [PROBE]
#
# Decide whether a verify-lisp-leaf run passed, from its trace log.
#
# WHAT THIS GATE IS FOR, and why it changed on 2026-08-07. Leaf is the IFS
# RANDOM-ACCESS file protocol: an Interlisp stream that seeks. The gate used
# to prove that by loading a package that was not on the disk -- but a
# FILESLOAD is sequential, and it only ever reached Leaf because the STP
# retrieve failed first (an empty Directory in the command plist, fixed the
# same day). With the retrieve working, FILESLOAD legitimately goes over STP
# and the old gate saw zero Leaf traffic and failed.
#
# So the gate now drives what only Leaf can do: OPENSTREAM the file, seek to
# a byte offset, and read. The decisive evidence is a LEAF_READ at a NON-ZERO
# address -- neither an STP retrieve nor a sequential stream can produce one.
#
# Every check below was a separate day's bug, and each failed in a way that
# looked like one of the others:
#
#   LEAF Reset / Params   the Sequin connection opened at all. Sequin keeps
#                         its state in the Pup ID field, and echoing that id
#                         replied with another \SEQUIN.OPEN, so the client
#                         retransmitted forever with no error anywhere.
#   STP_LOOKUP <name>     the client resolved the name through LookupFile.
#                         Spelling the directory {DORADO} instead of
#                         {DORADO}<> made it ask for <GUEST>NAME and give up
#                         BEFORE any Leaf traffic -- the failure was three
#                         packets earlier than it appeared.
#   LEAF Open             the file opened.
#   LEAF_READ addr>0      it SEEKED. This is the whole point.
#   LEAF_READ_LEADER      the IFS leader page, requested at every open. Left
#                         unanswered the client parsed uninitialised packet
#                         buffer -- deterministically, with a clean wire.
#
# The bytes the guest actually got back are checked by the Makefile against
# the host file, because the wire being right is not the same as the guest
# receiving what is on disk.
#
# Deliberately NOT a pixel count: a load that works and one that reports "not
# found" differ by about 0.5% of lit pixels, and the failing screen is
# sometimes the larger of the two.

log="$1"
probe="${2:-AISBLT}"

if [ ! -r "$log" ]; then
    echo "FAIL: no log at $log"
    exit 1
fi

fail=0

# 1. The Sequin connection opened and was answered.
for op in Reset Params; do
    if ! grep -q "^LEAF $op " "$log"; then
        echo "FAIL: no Leaf $op request -- the client never reached us."
        echo "      Check that INIT puts {DORADO}<> in DIRECTORIES, and that"
        echo "      the login prompt was answered (it blocks silently)."
        fail=1
    elif ! grep -q "^LEAF_REPLY $op " "$log"; then
        echo "FAIL: Leaf $op arrived but we never answered it"
        fail=1
    fi
done

# 2. Retransmits mean an answer was sent but not accepted -- the shape the
#    Sequin bug had. One request per op is the healthy count.
for op in Reset Params Open; do
    n=$(grep -c "^LEAF $op " "$log" 2>/dev/null || true)
    [ -z "$n" ] && n=0
    if [ "$n" -gt 3 ]; then
        echo "FAIL: $n Leaf $op requests -- the guest is retransmitting, so"
        echo "      our answer is going out but being rejected."
        fail=1
    fi
done

# 3. NAME RESOLUTION is deliberately NOT checked here any more.
#
#    The old gate required a successful STP_LOOKUP, because a FILESLOAD has
#    to resolve `AISBLT` into `AISBLT.LCOM!1` first, and getting that wrong
#    ({DORADO} instead of {DORADO}<>, so the client asked for <GUEST>NAME)
#    killed the transfer three packets before any Leaf traffic. This gate now
#    opens an explicit full path, which needs no resolution at all -- so
#    asserting a lookup here would fail a perfectly good run.
#
#    That coverage moved rather than vanished: `make verify-lisp-serve` does
#    a FILESLOAD and asserts the whole resolution chain
#    (STP_LOOKUP_MISSING .DFASL / .LCOM, then STP_LOOKUP, then STP_SERVE).
#    If this comment and that gate ever disagree, the coverage is gone.

# 4. The file opened, and its leader page was asked for and answered.
if ! grep -q "^LEAF_OPEN " "$log"; then
    echo "FAIL: the client never opened the file"
    fail=1
fi
if ! grep -q "^LEAF_READ_LEADER " "$log"; then
    echo "FAIL: no leader-page read. Interlisp asks for it at EVERY open;"
    echo "      its absence means the open did not really happen."
    fail=1
fi

# 5. THE POINT OF THE GATE: a read at a non-zero address, i.e. a seek.
#    An STP retrieve cannot produce one, and neither can a sequential
#    stream -- this is the one signature only random access leaves.
seeks=$(grep "^LEAF_READ handle" "$log" 2>/dev/null |
        sed -n 's/.*addr=\([0-9][0-9]*\).*/\1/p' |
        awk '$1 > 0' | wc -l | tr -d ' ')
[ -z "$seeks" ] && seeks=0
if [ "$seeks" -lt 1 ]; then
    echo "FAIL: no Leaf read at a non-zero address -- nothing SEEKED, so this"
    echo "      run does not exercise random access at all. If the reads are"
    echo "      missing entirely, the client probably satisfied the open over"
    echo "      STP instead; check for STP_SERVE of the same name."
    grep -E "^(LEAF_READ|STP_SERVE)" "$log" | head -5 | sed 's/^/      /'
    fail=1
fi

# 6. Nothing was left unanswered. A missing file must produce an ErrorAnswer,
#    not silence -- silence makes the guest retransmit until it times out,
#    which reads as a hang rather than "no such file".
if grep -q "LEAF_UNIMPLEMENTED" "$log"; then
    echo "FAIL: an unimplemented Leaf op was reached:"
    grep -o "LEAF_UNIMPLEMENTED [A-Za-z]*" "$log" | sort -u | sed 's/^/      /'
    fail=1
fi

if [ "$fail" -eq 0 ]; then
    echo "PASS: $probe opened over Leaf and seeked ($seeks read(s) past 0)"
fi
exit "$fail"
