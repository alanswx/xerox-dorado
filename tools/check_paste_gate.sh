#!/bin/sh
# check_paste_gate.sh LOG
#
# Did a pasted command reach the guest INTACT?
#
# Built 2026-08-08 after three ~45-minute runs died on the same typo and I
# only found out at the end, by converting a framebuffer to PNG and reading
# it. The bug was real -- the immediate key path did not reserve its display
# field, so the FIRST character of every paste was doubled and
# "Bringover ..." arrived as "BBringover ..." -- but the cost was entirely in
# how long it took to see.
#
# READ THE WIRE, NOT THE SCREEN. A correct `Bringover [Cedar]<CedarChest6.1>
# Top>DoradoWelcome` makes the guest ask our in-process STP server for that
# DF, which logs STP_SERVE. A mistyped one is rejected by the CommandTool as
# [[BBringover not found]] and produces NO network traffic at all. So the
# presence of one trace line is an exact test of whether the keystrokes
# arrived, and it needs no font table -- pgm_text.py has been taught the
# Interlisp font, not Cedar's Tioga, so OCR was not available here anyway.
#
# This is the same reasoning as check_cedar_ls_gate.sh: a pixel count cannot
# read text, and the wire already carries the answer.

log="$1"
[ -r "$log" ] || { echo "FAIL: no log at $log"; exit 1; }

if grep -q "STP_SERVE .*DoradoWelcome.df" "$log"; then
    echo "PASS: the pasted Bringover reached the guest intact (STP_SERVE seen)"
    exit 0
fi

echo "FAIL: no STP_SERVE of DoradoWelcome.df -- the pasted command did not run."
echo "      The CommandTool rejects a mistyped command silently as far as the"
echo "      network is concerned, so this is what a corrupted paste looks like."
echo "      Convert the .pgm and read the typescript to see what it received;"
echo "      a doubled FIRST character means the immediate key path is not"
echo "      reserving its display field (machine.c, dorado_machine_set_key)."
grep -c "STP_SERVE" "$log" 2>/dev/null | sed 's/^/      total STP_SERVE lines: /'
exit 1
