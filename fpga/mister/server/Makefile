# Standalone Pup service; deliberately excludes the Dorado CPU/machine.
CC ?= cc
ROOT := ../../..
OUT ?= $(ROOT)/build/dorado-server
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
GC := -Wl,-dead_strip
else
GC := -Wl,--gc-sections
endif
# Use an explicit override when cross compiling from macOS.
LDFLAGS ?= $(GC)
CFLAGS ?= -O2 -Wall -Wextra -ffunction-sections -fdata-sections

.PHONY: all test cedar-test bsp-retry-test bsp-ack-race-test ddr-ring-test ddr-control-test ddr-map-test ddr-wire-test service-clock-test snapshot-pause-test
all:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -I$(ROOT)/dorado/include dorado_server.c \
		dorado_ddr_ring.c dorado_ddr_control.c dorado_ddr_mapping.c \
		$(ROOT)/dorado/src/ethernet.c $(ROOT)/dorado/src/io.c $(LDFLAGS) -o $(OUT)
test: all
	python3 test_server.py $(OUT) $(ROOT)
cedar-test: all
	python3 test_cedar_server.py $(OUT) $(ROOT)
bsp-retry-test: all
	python3 test_bsp_retry.py $(OUT) $(ROOT)
bsp-ack-race-test: all
	python3 test_bsp_ack_race.py $(OUT) $(ROOT)
ddr-ring-test:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -std=c11 -Werror dorado_ddr_ring.c test_ddr_ring.c -o $(OUT)-ddr-ring-test
	$(OUT)-ddr-ring-test
ddr-control-test:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -std=c11 -Werror dorado_ddr_ring.c dorado_ddr_control.c test_ddr_control.c -o $(OUT)-ddr-control-test
	$(OUT)-ddr-control-test
ddr-map-test:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -std=c11 -Werror dorado_ddr_mapping.c test_ddr_mapping.c -o $(OUT)-ddr-map-test
	$(OUT)-ddr-map-test
service-clock-test:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -std=c11 -Werror test_service_clock.c -o $(OUT)-service-clock-test
	$(OUT)-service-clock-test
snapshot-pause-test:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -DDORADO_TEST_PAUSE_MAPPER -I$(ROOT)/dorado/include dorado_server.c \
		dorado_ddr_ring.c dorado_ddr_control.c test_pause_mapper.c \
		$(ROOT)/dorado/src/ethernet.c $(ROOT)/dorado/src/io.c $(LDFLAGS) -o $(OUT)-pause-fixture
	python3 test_snapshot_pause.py $(OUT)-pause-fixture $(ROOT)
ddr-wire-test:
	python3 test_ddr_wire.py --mutations
