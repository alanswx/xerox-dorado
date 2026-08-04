#!/bin/sh
# check_cedar_sil_gate.sh LOG LO HI
#
# Decide whether verify-cedar-sil passed: did clicking the CommandTool's
# Sil button open ProcH01.sil -- the Dorado's own 1981 processor-board
# drawing, in Sil, PARC's vector editor -- from the shipped checkpoint?
#
# A pixel BAND is the right assertion here, unlike verify-cedar-ls where a
# count is useless (see check_cedar_ls_gate.sh: a listing scrolls a full
# typescript rather than adding ink, so success and failure land 0.5%
# apart). Here the three outcomes are nowhere near each other, because the
# drawing REPLACES the left half of the screen with sparse vectors:
#
#   ~89,600 px   Sil open on ProcH01.sil          <- pass
#   ~167,600 px  the untouched desktop            <- the click did nothing
#   ~28,500 px   the Cedar login screen           <- it cold-booted instead
#                                                    (or --snapshot-in was
#                                                     omitted, which cost a
#                                                     whole session once)
#
# Measured 2026-08-04: 89,614 px native AND under the node wasm build, with
# byte-identical framebuffers, against a 167,653 px no-input control over
# the same 4B cycles.
#
# Kept out of the Makefile so it can be run against known good and known
# bad logs.

log="$1"
lo="${2:-85000}"
hi="${3:-95000}"

if [ ! -r "$log" ]; then
    echo "FAIL: no log at $log"
    exit 1
fi

px=$(sed -n 's/.*dorado: \([0-9][0-9]*\) display-list pixels.*/\1/p' "$log" \
     | tail -1)

if [ -z "$px" ]; then
    echo "FAIL: the run produced no screen (look at $log)"
    exit 1
fi

if [ "$px" -lt "$lo" ] || [ "$px" -gt "$hi" ]; then
    echo "FAIL: $px display-list pixels, outside ${lo}..${hi}."
    case "$px" in
        1[5-9][0-9][0-9][0-9][0-9])
            echo "      That is the untouched desktop: the Sil button click"
            echo "      did not land. The button spans x 728-740, y 29-36 --"
            echo "      measure it off the pixels, do not guess." ;;
        2[0-9][0-9][0-9][0-9]|3[0-9][0-9][0-9][0-9])
            echo "      That is a login screen: the world cold-booted rather"
            echo "      than restoring. Check --snapshot-in is present and"
            echo "      that the checkpoint matches this build's snapshot ABI"
            echo "      (make verify-snapshot-abi)." ;;
        *)
            echo "      Look at build/verify-cedar-sil.pgm." ;;
    esac
    exit 1
fi

echo "PASS: Sil opened ProcH01.sil ($px px)"
exit 0
