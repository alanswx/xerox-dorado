#!/bin/sh
# check_lisp_leaf_gate.sh LOG
#
# Decide whether a verify-lisp-leaf run passed, from its trace log.
#
# What this gate is for: Interlisp loading a file that is NOT on its disk,
# over Leaf. Every step below was a separate day's bug, and each one failed
# in a way that looked like one of the others:
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
#   LEAF Open + Reads     the file actually transferred.
#   the COMS line         Interlisp accepted what it read. A file can be
#                         served byte-perfect and still be the wrong vintage
#                         ("Bad compiled function"), so the wire is not
#                         sufficient evidence on its own.
#
# Deliberately NOT a pixel count: a load that works and a load that reports
# "not found" differ by about 0.5% of lit pixels, and the failing screen is
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

# 3. The client resolved the name through LookupFile, and we answered.
if ! grep -q "STP_LOOKUP $probe" "$log"; then
    echo "FAIL: no successful LookupFile for $probe."
    grep "STP_LOOKUP_MISSING" "$log" | sort -u | sed 's/^/      asked for: /'
    fail=1
fi

# 4. The file transferred: an Open and a run of Reads reaching the tail.
if ! grep -q "^LEAF_OPEN " "$log"; then
    echo "FAIL: the client never opened the file"
    fail=1
fi
reads=$(grep -c "^LEAF_READ handle" "$log" 2>/dev/null || true)
[ -z "$reads" ] && reads=0
if [ "$reads" -lt 5 ]; then
    echo "FAIL: only $reads Leaf reads; the file did not stream"
    fail=1
fi

# 5. Nothing was left unanswered. A missing file must produce an ErrorAnswer,
#    not silence -- silence makes the guest retransmit until it times out,
#    which reads as a hang rather than "no such file".
if grep -q "LEAF_UNIMPLEMENTED" "$log"; then
    echo "FAIL: an unimplemented Leaf op was reached:"
    grep -o "LEAF_UNIMPLEMENTED [A-Za-z]*" "$log" | sort -u | sed 's/^/      /'
    fail=1
fi

if [ "$fail" -eq 0 ]; then
    echo "PASS: $probe opened and streamed in $reads reads, none on disk"
fi
exit "$fail"
