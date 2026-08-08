#!/bin/sh
# check_cedar_gate.sh LOG MIN_PIXELS
#
# Decide whether a verify-cedar-desktop run passed, from its trace log.
# Two independent failure signals, both learned the hard way on
# 2026-07-18/19:
#
#   STP_MISSING lines  a file the served tree does not have. The gate
#                      serves the BROWSER's pruned tree, and Cedar's
#                      interpreter demand-fetches while evaluating, so
#                      anything missing there breaks the deployed site
#                      only -- never SDL.
#   pixel count        the desktop must paint the picture -- and must NOT
#                      paint too much. Both bounds are needed: an all-black
#                      region is MORE ink, so a floor alone passes it (that
#                      is how inverted AIS images hid for weeks).
#                      Reference counts: mid-install checkpoint ~87K,
#                      bare desktop ~167K, failed Eval ~169K (error text
#                      only), moon painted ~246K.
#
# Kept separate from the Makefile so it can be exercised against known
# good and known bad logs (see the self-test at the bottom of the
# verify-cedar-desktop section in dorado/Makefile).

log="$1"
min="${2:-200000}"
# Upper bound. 1024x808 = 827,392 px total; a healthy desktop with a picture
# is ~250K (30%). Half the screen filled is not a desktop. See below.
max="${3:-420000}"

if [ ! -r "$log" ]; then
    echo "FAIL: no log at $log"
    exit 1
fi

fail=0

miss=$(grep -c "STP_MISSING" "$log" 2>/dev/null || true)
[ -z "$miss" ] && miss=0
if [ "$miss" -ne 0 ]; then
    echo "FAIL: $miss request(s) for files the served tree lacks:"
    grep "STP_MISSING" "$log" | sed 's/.*STP_MISSING /      /' | sort -u
    echo "      add them to the web-stp target in dorado/Makefile"
    fail=1
fi

px=$(sed -n 's/.*dorado: \([0-9][0-9]*\) display-list pixels.*/\1/p' "$log" \
     | tail -1)
if [ -z "$px" ]; then
    echo "FAIL: the run produced no screen (look at $log)"
    fail=1
elif [ "$px" -lt "$min" ]; then
    echo "FAIL: $px display-list pixels < $min -- the picture did not paint."
    echo "      Either the Eval errored, or the checkpoint is not at the"
    echo "      desktop (mid-install ~87K, bare desktop ~167K, failed Eval"
    echo "      ~169K, painted moon ~246K). Look at build/verify-cedar.pgm."
    fail=1
elif [ "$px" -gt "$max" ]; then
    # TOO MUCH INK IS ALSO A FAILURE, and the minimum alone cannot see it.
    #
    # Learned 2026-08-08. AIS images written from PBM came out with inverted
    # polarity, so a schematic with 14% ink stored as 86% black rendered as a
    # black square. A minimum-pixel gate PASSES that -- an all-black AIS
    # viewer is roughly 600x670 = 400K pixels MORE ink, sailing past any
    # floor. The bug lived from the day the files were made until a human
    # noticed the picture, with the gate green throughout.
    #
    # The screen is 1024x808 = 827,392 pixels. A working Cedar desktop with a
    # picture on it runs ~250K, i.e. 30% ink. Anything past half the screen
    # is not a desktop, it is a fill.
    echo "FAIL: $px display-list pixels > $max -- TOO MUCH ink."
    echo "      A minimum-only gate cannot catch this: solid black is MORE"
    echo "      pixels, not fewer. Suspect inverted polarity (AIS is"
    echo "      luminance, high = white; PBM is 1 = black), or a viewer"
    echo "      painting a filled rectangle. Look at build/verify-cedar.pgm."
    fail=1
fi

if [ "$fail" -eq 0 ]; then
    echo "PASS: picture painted ($px px), nothing missing from the served tree"
fi
exit "$fail"
