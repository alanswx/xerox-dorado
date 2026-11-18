#!/bin/sh
# A release of the one core for another MiSTer: the set of files that is
# installed and verified on the development board, copied off it and laid
# out as they sit under /media/fat.
#   make_release.sh [board] [name]
# board: the MiSTer to copy from (default mister14, the development board).
# The result is dorado/build/release/<name>/ and <name>.tar beside it
# (default name DoradoOne-<date>). Nothing is written to the board.
# Installing is INSTALL.txt in the release: one tar command on the target.
set -e
B=${1:-mister14}
N=${2:-DoradoOne-$(date +%Y%m%d)}
HERE=$(cd "$(dirname "$0")" && pwd)
OUT=$HERE/../../dorado/build/release/$N
mkdir -p "$OUT/_Development" "$OUT/Scripts" "$OUT/dorado/dorado/build/good-packs" "$OUT/dorado/dorado/worlds"
get() { scp -q "root@$B:/media/fat/$1" "$OUT/$1"; }
# the bitstream, the ARM host, the start script
get _Development/DoradoRtlOne_44.rbf
get dorado/dorado-hps-one
get Scripts/DoradoOne.sh
# the stock desktops and their disks
for f in cedar-desktop.snap cedar-desktop.pdi lisp-lyric-full.snap lisp-lyric-full.pack \
         smalltalk76.snap smalltalk76.pack; do
    get dorado/dorado/build/good-packs/$f
done
get dorado/dorado/worlds/aemu.eb
# the cold starts and their disks; Cedar's file server tree
for f in cedar-cold.snap cedar-cold.snap.pdi lisp-cold.snap lisp-cold.snap.pack \
         smalltalk-cold.snap smalltalk-cold.snap.pack stp-root.tar.gz; do
    get dorado/$f
done
# The host and the script must be the ones this repository builds.
same() { [ "$(md5 -q "$1")" = "$(md5 -q "$2")" ] || { echo "$1 is not $2: rebuild or reinstall first" >&2; exit 1; }; }
same "$OUT/dorado/dorado-hps-one" "$HERE/dorado-hps"
same "$OUT/Scripts/DoradoOne.sh" "$HERE/DoradoOne.sh"
cat > "$OUT/INSTALL.txt" <<EOF
Xerox Dorado for the MiSTer: the one core ($N)
Commit $(cd "$HERE" && git rev-parse --short HEAD), copied from $B.

One bitstream runs Interlisp-D (Lyric), Smalltalk-76 and Cedar 6.1. Each
starts at its saved desktop, or cold (the world's own boot from its disk).

Install, on the MiSTer (about 420 MB on the SD card):
    tar -C /media/fat -xf $N.tar --exclude INSTALL.txt --exclude MD5SUMS
It adds _Development/DoradoRtlOne_44.rbf, Scripts/DoradoOne.sh and
dorado/. It replaces files of those names and touches nothing else; saves
already in /media/fat/dorado (*-user.snap, dorado-save.snap) are kept.

Run, from the MiSTer's Scripts menu or a shell:
    /media/fat/Scripts/DoradoOne.sh lisp|smalltalk|cedar          the saved desktop
    /media/fat/Scripts/DoradoOne.sh lisp|smalltalk|cedar cold     a cold start
    /media/fat/Scripts/DoradoOne.sh WORLD /media/fat/dorado/X.snap   a save of that world
Do not load the .rbf from the menu by itself: the screen stays white until
the script's host program starts the machine.

Cold starts:
    smalltalk: at the Alto Executive, type  Bootfrom xmsmall.boot
    lisp:      at the Alto Executive, type  lisp.run/M {DORADO}LISP.SYSOUT
    cedar:     at the login, type Guest and press Return twice
               (the installation takes 12 to 13 minutes)

Save the running machine:  touch /tmp/dorado-save
It writes /media/fat/dorado/dorado-save.snap (and its disk, about a minute
for Lisp and Smalltalk); the machine carries on meanwhile.

The log is /tmp/hps-one.log.
EOF
(cd "$OUT" && find . -type f ! -name MD5SUMS | sort | while read -r f; do echo "$(md5 -q "$f")  $f"; done > MD5SUMS)
(cd "$OUT" && tar -cf "../$N.tar" .)
ls -la "$OUT/../$N.tar"
echo "release in $OUT"
