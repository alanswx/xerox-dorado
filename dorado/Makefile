CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -Wshadow -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -O2 -g -Iinclude -Ivendor/6502
LDFLAGS ?=
DEPFLAGS := -MMD -MP

OBJDIR  := build
BINDIR  := build

# SDL2 frontend (optional target `make sdl` / build/dorado-sdl).
SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_LIBS   := $(shell sdl2-config --libs 2>/dev/null)

MB_OBJS   := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o $(OBJDIR)/mbdis.o
LIB_OBJS  := $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/microcode.o $(OBJDIR)/cpu.o $(OBJDIR)/baseboard.o $(OBJDIR)/memory.o $(OBJDIR)/io.o $(OBJDIR)/display.o $(OBJDIR)/disk.o $(OBJDIR)/ethernet.o $(OBJDIR)/fastio.o
EXTRA_OBJS := $(OBJDIR)/mctrace.o $(OBJDIR)/bbtrace.o $(OBJDIR)/bbdis.o $(OBJDIR)/machine.o $(OBJDIR)/dorado.o $(OBJDIR)/test_mb.o $(OBJDIR)/test_disasm.o $(OBJDIR)/test_microcode.o $(OBJDIR)/test_cpu.o $(OBJDIR)/test_baseboard.o $(OBJDIR)/test_memory.o $(OBJDIR)/test_display.o $(OBJDIR)/test_disk.o $(OBJDIR)/test_ethernet.o $(OBJDIR)/test_fastio.o $(OBJDIR)/audit_alufm.o $(OBJDIR)/altodiff_dorado.o
DEPS := $(sort $(MB_OBJS) $(LIB_OBJS) $(EXTRA_OBJS))
DEPS := $(DEPS:.o=.d)

all: $(BINDIR)/dorado $(BINDIR)/mbdis $(BINDIR)/mb2eb $(BINDIR)/mctrace $(BINDIR)/bbtrace $(BINDIR)/bbdis $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio

$(BINDIR)/dorado: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/dorado.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# The SDL frontend links SDL2; build with `make sdl`.
.PHONY: sdl
sdl: $(BINDIR)/dorado-sdl

$(OBJDIR)/dorado_sdl.o: src/dorado_sdl.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BINDIR)/dorado-sdl: $(LIB_OBJS) $(OBJDIR)/machine.o $(OBJDIR)/dorado_sdl.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDL_LIBS)

$(BINDIR)/mbdis: $(MB_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(MB_OBJS) $(LDFLAGS)

$(BINDIR)/mb2eb: $(LIB_OBJS) $(OBJDIR)/mb2eb.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/altodiff-dorado: $(LIB_OBJS) $(OBJDIR)/altodiff_dorado.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/mctrace: $(LIB_OBJS) $(OBJDIR)/mctrace.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/bbtrace: $(LIB_OBJS) $(OBJDIR)/bbtrace.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/bbdis: $(OBJDIR)/mb.o $(OBJDIR)/disasm.o $(OBJDIR)/bbdis.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_mb: $(LIB_OBJS) $(OBJDIR)/test_mb.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_disasm: $(LIB_OBJS) $(OBJDIR)/test_disasm.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_microcode: $(LIB_OBJS) $(OBJDIR)/test_microcode.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_cpu: $(LIB_OBJS) $(OBJDIR)/test_cpu.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_baseboard: $(LIB_OBJS) $(OBJDIR)/test_baseboard.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_memory: $(LIB_OBJS) $(OBJDIR)/test_memory.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_display: $(LIB_OBJS) $(OBJDIR)/test_display.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_disk: $(LIB_OBJS) $(OBJDIR)/test_disk.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_ethernet: $(LIB_OBJS) $(OBJDIR)/test_ethernet.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/test_fastio: $(LIB_OBJS) $(OBJDIR)/test_fastio.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/audit_alufm: $(LIB_OBJS) $(OBJDIR)/audit_alufm.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: tests/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $@

test: $(BINDIR)/test_mb $(BINDIR)/test_disasm $(BINDIR)/test_microcode $(BINDIR)/test_cpu $(BINDIR)/test_baseboard $(BINDIR)/test_memory $(BINDIR)/test_display $(BINDIR)/test_disk $(BINDIR)/test_ethernet $(BINDIR)/test_fastio
	$(BINDIR)/test_mb
	@echo
	$(BINDIR)/test_disasm
	@echo
	$(BINDIR)/test_microcode
	@echo
	$(BINDIR)/test_memory
	@echo
	$(BINDIR)/test_display
	@echo
	$(BINDIR)/test_disk
	@echo
	$(BINDIR)/test_ethernet
	@echo
	$(BINDIR)/test_fastio
	@echo
	$(BINDIR)/test_cpu
	@echo
	$(BINDIR)/test_baseboard

clean:
	rm -rf $(OBJDIR)

.PHONY: all clean test

-include $(DEPS)
