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

.PHONY: all test cedar-test bsp-retry-test bsp-ack-race-test
all:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) -I$(ROOT)/dorado/include dorado_server.c \
		$(ROOT)/dorado/src/ethernet.c $(ROOT)/dorado/src/io.c $(LDFLAGS) -o $(OUT)
test: all
	python3 test_server.py $(OUT) $(ROOT)
cedar-test: all
	python3 test_cedar_server.py $(OUT) $(ROOT)
bsp-retry-test: all
	python3 test_bsp_retry.py $(OUT) $(ROOT)
bsp-ack-race-test: all
	python3 test_bsp_ack_race.py $(OUT) $(ROOT)
