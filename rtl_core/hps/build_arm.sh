#!/bin/sh
# Build dorado-hps (the Cedar host services for the MiSTer's ARM) with the
# arm-linux-gnueabihf cross compiler, statically so the MiSTer's own C
# library does not matter.
#
#   rtl_core/hps/build_arm.sh [BUILD_HOST]     (default: misterubuntu)
#
# The sources are copied to BUILD_HOST:~/dorado-hps-build and the binary
# comes back as rtl_core/hps/dorado-hps.
set -e
HOST=${1:-misterubuntu}
SSH_OPTS=${SSH_OPTS:--o HostName=10.0.2.128}
HERE=$(cd "$(dirname "$0")" && pwd)
DORADO=$(cd "$HERE/../../dorado" && pwd)
C_SRCS="mb disasm microcode cpu memory io fastio baseboard display dispm disk ethernet altoref pdi machine typetext"

STAGE=$(mktemp -d)
mkdir -p "$STAGE/src" "$STAGE/include" "$STAGE/vendor" "$STAGE/hps"
for f in $C_SRCS; do cp "$DORADO/src/$f.c" "$STAGE/src/"; done
cp "$DORADO"/include/*.h "$STAGE/include/"
cp -r "$DORADO/vendor/6502" "$STAGE/vendor/"
cp "$HERE/hps_host.h" "$HERE/scan_map.h" "$HERE/hps_host.cpp" "$HERE/hps_main.cpp" "$STAGE/hps/"
cat > "$STAGE/Makefile" <<'MK'
CC  = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++
CFLAGS   = -std=c99 -D_GNU_SOURCE -O2 -g -Iinclude -Ivendor/6502 -mcpu=cortex-a9 -mfpu=neon
CXXFLAGS = -std=c++17 -O2 -g -Iinclude -mcpu=cortex-a9 -mfpu=neon
OBJS = $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c)) obj/hps_host.o obj/hps_main.o
dorado-hps: $(OBJS)
	$(CXX) -static -o $@ $(OBJS) -lm
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<
obj/%.o: hps/%.cpp hps/hps_host.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $<
MK
# shellcheck disable=SC2086
ssh $SSH_OPTS "$HOST" 'rm -rf ~/dorado-hps-build && mkdir -p ~/dorado-hps-build'
# shellcheck disable=SC2086
COPYFILE_DISABLE=1 tar --no-xattrs -C "$STAGE" -cf - . | ssh $SSH_OPTS "$HOST" 'tar -C ~/dorado-hps-build -xf - && make -C ~/dorado-hps-build -j4 dorado-hps 2>&1 | grep -E "error|warning: implicit" ; ls -la ~/dorado-hps-build/dorado-hps'
# shellcheck disable=SC2086
scp $SSH_OPTS "$HOST:dorado-hps-build/dorado-hps" "$HERE/dorado-hps"
rm -rf "$STAGE"
echo "built $HERE/dorado-hps"
