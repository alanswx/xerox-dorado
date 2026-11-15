#!/bin/sh
# One Dorado RTL core for every world (the scan core), on the MiSTer:
#   DoradoOne.sh [-r] lisp|smalltalk|cedar [SAVED.snap]
# The core has no world compiled in; dorado-hps-one loads the checkpoint
# (storage, <snapshot>.rams, <snapshot>.regs) and says how it is presented.
# SAVED.snap: a save of that world (touch /tmp/dorado-save while it runs
# writes /media/fat/dorado/dorado-save.snap) in the checkpoint's place.
# If the core is already loaded it is not loaded again: dorado-hps-one asks
# it to stop and start over (a few seconds). It exits 3 when no core answers,
# and then the core is loaded and it runs again. -r: load the core anyway.
# Installed as /media/fat/Scripts/DoradoOne.sh.
R=0
[ "$1" = -r ] && { R=1; shift; }
W=${1:-lisp}
S=${2:+-S $2}
# (a file of its own: a start still running reads its script as it goes)
T=/tmp/dorado-one-start.$$.sh
cat > $T <<INNER
killall dorado-hps dorado-hps-st dorado-hps-scan dorado-hps-one 2>/dev/null
sleep 1
[ $W != cedar ] || [ -d /tmp/dorado/stp-root ] || { mkdir -p /tmp/dorado && tar -C /tmp/dorado -xzf /media/fat/dorado/stp-root.tar.gz; }
cd /media/fat/dorado
[ -f /tmp/hps-one.log ] && mv /tmp/hps-one.log /tmp/hps-one.log.1
host() {
case $W in
cedar) DORADO_FTP_TRACE=1 DORADO_HPS_KEYTRACE=1 ./dorado-hps-one $S -f /tmp/dorado/stp-root -s 10 >> /tmp/hps-one.log 2>&1 ;;
*)     DORADO_HPS_KEYTRACE=1 ./dorado-hps-one -w $W $S -s 10 >> /tmp/hps-one.log 2>&1 ;;
esac
}
if [ $R = 0 ]; then
    host
    [ \$? = 3 ] || { rm -f $T; exit; }
fi
echo load_core /media/fat/_Development/DoradoRtlOne_44.rbf > /dev/MiSTer_cmd
sleep 8
host
rm -f $T
INNER
setsid nohup sh $T > /dev/null 2>&1 < /dev/null &
echo "Dorado $W starting on the one core; log in /tmp/hps-one.log"
