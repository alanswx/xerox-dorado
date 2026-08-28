# Generate the Dorado RTL from PARC's own Sil design-automation output.
#
#   make boards   regenerate verilog/generated/*.v from chm/sil, including
#                 the backplane top module that wires them together
#   make cells    (re)emit missing cell skeletons -- never overwrites a
#                 hand-written model; use `make cells-force` deliberately
#   make proms    regenerate proms/*.mem and the per-package images
#   make lint       every board must still elaborate under Verilator
#   make cell-check every cell wires the inputs PARC says its gates have,
#                 and a both-sense gate's two outputs are complements
#   make loop-check no combinational feedback beyond the one that is known
#   make prom-test    the PROMs read back what the machine expects
#   make alu-test     the ALU matches its datasheet's function table
#   make alu-diff     the RTL ALU matches the C emulator's, vector for vector
#   make cpreg-diff   the RTL boot interface matches the C emulator's
#   make mir-diff     the RTL microinstruction load matches, all 36 bits
#   make mirreg-diff  a microinstruction jammed into the MIR reads back right
#   make run-test     THE MACHINE RUNS: the Control section executes cycles
#   make datapath-test four boards, with the microinstruction on the datapath
#   make writeim-test a jammed Write-IM deposits into IM, half-select and all
#   make operand-test ...and its DATA comes from CPReg (the MIR clock held)
#   make step-test    the machine SINGLE-STEPS microinstructions
#   make startseq     PARC's own boot sequence replayed (diagnostic, not a gate)
#   make sendmir      PARC's IM LOADER: words go into IM at their addresses
#   make boot0-test   PARC's BLOCK LOADER: a 17-byte hunk walked into IM
#   make exec-test    THE MACHINE EXECUTES MICROCODE OUT OF IM
#   make compute-test THE MACHINE COMPUTES: Q takes 25o and ALUFM[0] stores it

#   make converge-test the assembled machine settles, and its clock runs
#   make machine-test the assembled machine under the imgui harness
#   make baseboard-test the BaseBoard's 6502 boots from its own EPROMs
#   make mem-test     MemC/MemD/MemX in a machine, with the clock running
#
# The inputs are all original Xerox data; see docs/verilog-from-sil.md.
SIL     = ../chm/sil
ECLDICT = $(SIL)/msa-Rev-Bg.dm!1_/ecldict.analyze
TTLDICT = $(SIL)/msa-Rev-Bg.dm!1_/ttldict.analyze
TOOLS   = ../tools

.PHONY: boards backplane proms cells cells-force lint cell-check loop-check readback-test display-test \
        prom-test alu-test alu-diff cpreg-diff mir-diff mirreg-diff run-test \
        writeim-test operand-test step-test sendmir boot0-test exec-test \
        parity-probe compute-test \
        datapath-test \
        startseq \
        converge-test machine-test \
        baseboard-test firmware-probe mem-test \
        clean-generated

# PROM contents, sliced per package position from PARC's own BCPL.
proms:
	@python3 $(TOOLS)/dorado_proms.py --emit-all --out proms
	@python3 $(TOOLS)/dorado_proms.py --emit-packages proms/packages
	@$(MAKE) -s -C ../dorado build/baseboard_rom_dump
	@cd ../dorado && ./build/baseboard_rom_dump > build/baseboard_rom_dump.txt 2>/dev/null
	@python3 $(TOOLS)/firmware_eproms.py --emit proms/eprom

# Do the cells wire the inputs PARC says each gate has? Tim found MC10100
# ignoring its common strobe on three of four gates; this checks all of them.
# The BACKPLANE_CASE_ALIASES table, re-derived from PARC's own .bp files:
# a case variant may be merged ONLY where every board that uses it agrees on
# the pin. The table's comment promised this check long before it existed.
case-variants:
	@python3 $(TOOLS)/sil_backplane.py --case-variants

cell-check:
	@python3 $(TOOLS)/sil_check_cells.py
	@python3 $(TOOLS)/sil_check_polarity.py

# Combinational feedback. A loop that can oscillate stops the whole machine
# settling, and the cause has every time been a cell modelled as a latch that
# is not one -- so this asks the question directly, from the cell files, rather
# than waiting for Verilator to fail to converge and name a signal on some
# other board.
loop-check:
	@python3 $(TOOLS)/sil_loops.py

prom-test:
	@$(MAKE) -C verilator prom-test

alu-test:
	@$(MAKE) -C verilator alu-test

# The RTL ALU against the C emulator's -- see verilator/Makefile.
alu-diff:
	@$(MAKE) -C verilator alu-diff

# The RTL boot interface against the C emulator's.
cpreg-diff:
	@$(MAKE) -C verilator cpreg-diff

# The RTL microinstruction load path against the C emulator's, all 36 bits.
mir-diff:
	@$(MAKE) -C verilator mir-diff

# A whole microinstruction jammed into the MIR and read back off the register.
mirreg-diff:
	@$(MAKE) -C verilator mirreg-diff

# The Control section executing microinstruction cycles.
run-test:
	@$(MAKE) -C verilator run-test

# Four boards, with a jammed microinstruction's fields reaching the datapath.
datapath-test:
	@$(MAKE) -C verilator datapath-test

# A jammed Write-IM depositing into IM, both halves.
writeim-test:
	@$(MAKE) -C verilator writeim-test

# And its operand: the DATA comes from CPReg, once the MIR clock is held.
operand-test:
	@$(MAKE) -C verilator operand-test

# One microinstruction at a time -- the primitive Boot0 is built out of.
step-test:
	@$(MAKE) -C verilator step-test

# THE MACHINE COMPUTES: PARC's own ALU prologue, Q and ALUFM verified.
compute-test:
	@$(MAKE) -C verilator compute-test

# Do PARC's IRTable entries satisfy our IM parity generators? Diagnostic.
parity-probe:
	@$(MAKE) -C verilator parity-probe

# THE MACHINE EXECUTES MICROCODE OUT OF IM.
exec-test:
	@$(MAKE) -C verilator exec-test

# PARC's block loader: a 17-byte hunk walked into IM, read back word for word.
boot0-test:
	@$(MAKE) -C verilator boot0-test

# A WHOLE WORLD preloaded into IM through the derived field map, then run.
# Four boards: the world spends its life in the trap vectors, which is correct
# for a machine with no memory section and no IFU.
#   make exec-world EXECARGS='+cycles=2000000'
exec-world:
	@$(MAKE) -C verilator exec-world EXECARGS="$(EXECARGS)"

# The same world on NINE boards -- memory section, storage array and IFU as
# well -- with IM, ALUFM and IFUM all loaded and read back. This is the rung
# where the machine dispatches an Alto opcode.
#   make exec-world9 EXECARGS='+cycles=2000000'
exec-world9:
	@$(MAKE) -C verilator exec-world9 EXECARGS="$(EXECARGS)"

# THE MACHINE EXECUTES MICROCODE OUT OF IM WITH PARITY ENABLED -- the rung the
# project could never reach. See docs/verilog-handoff.md, "IM PARITY".
exec-parity:
	@$(MAKE) -C verilator exec-parity

# THE FAULT TASK SERVICES THE STACK UNDERFLOW: with tasking on the machine is
# no longer held and sequences through real Alto opcode handlers.
exec-tasking:
	@$(MAKE) -C verilator exec-tasking

# THE FAULT TASK RUNS ITS OWN HANDLER, once MemX's fault counter is armed and
# TPC[15] points at AEmu's FAULTTASK.
exec-init:
	@$(MAKE) -C verilator exec-init

# PARC'S BOOT CHAIN: Initial calls Bootstrap, which waits for the control
# processor. The first two stages of a real Dorado boot, in generated RTL.
exec-boot:
	@$(MAKE) -C verilator exec-boot

# The IM/ALUFM/IFUM field maps, derived from the wire lists and checked
# against HM Table 11d, HM Table 20 and cpu.c.


# PARC's SendViaMIR: Boot0's inner loop, loading words into IM.
sendmir:
	@$(MAKE) -C verilator sendmir

# PARC's own DoDoradoMicroInst replayed. A diagnostic, not a gate.
startseq:
	@$(MAKE) -C verilator startseq

# The assembled machine settles and its clock runs -- fast, event-scheduler.
converge-test:
	@$(MAKE) -C verilator converge-test

# The terminal microcomputer -- the keyboard's own processor, as RTL.
term-test:
	@$(MAKE) -C verilator term-test

# A Trident drive at the end of the cable -- presence and 3600 RPM rotation.
trident-test:
	@$(MAKE) -C verilator trident-test

# Does the RTL keyboard put each key where the C emulator's key_map puts it?
# A formula was tried first and is WRONG -- word 2 carries only fifteen keys --
# so the two tables are compared file against file on every run.
key-matrix-check:
	@python3 $(TOOLS)/check_key_matrix.py

# Does the assembled machine run? Rebuilds the harness it tests.
machine-test:
	@$(MAKE) -C verilator machine-test

# Does the BaseBoard's 6502 come out of reset and fetch its reset vector?
baseboard-test:
	@$(MAKE) -C verilator baseboard-test

muffler-test:
	@$(MAKE) -C verilator muffler-test

strap-test:
	@$(MAKE) -C verilator strap-test

osc-test:
	@$(MAKE) -C verilator osc-test

task-test:
	@$(MAKE) -C verilator task-test

taskrun-test:
	@$(MAKE) -C verilator taskrun-test

refdecode-test:
	@$(MAKE) -C verilator refdecode-test

memrun-test:
	@$(MAKE) -C verilator memrun-test

# The memory section in a machine, with its clock running.
mem-test:
	@$(MAKE) -C verilator mem-test

# The STORAGE ARRAY in a machine: PARC's msa board, 144 MK4096 DRAMs, with its
# address path inverting and the memory size arriving over the backplane.
storage-test:
	@$(MAKE) -C verilator storage-test

readback-test:
	@$(MAKE) -C verilator readback-test

display-test:
	@$(MAKE) -C verilator display-test

disk-test:
	@$(MAKE) -C verilator disk-test

# The same loop aimed at DISKTAG (014B): the decode must DISCRIMINATE.
disk-tag-test:
	@$(MAKE) -C verilator disk-tag-test

# A DISKMUFF write that clears the index wakeup.
disk-muff-test:
	@$(MAKE) -C verilator disk-muff-test

# A real read command: it loads, and it starts.
disk-cmd-test:
	@$(MAKE) -C verilator disk-cmd-test

# A DISKRAM write steps the format-RAM address.
disk-ram-test:
	@$(MAKE) -C verilator disk-ram-test

# All sixteen format words of HM p.98, loaded and verified.
disk-format-test:
	@$(MAKE) -C verilator disk-format-test

# The read direction: the board drives IOB back at the processor.
disk-input-test:
	@$(MAKE) -C verilator disk-input-test

# THE DISK/ETHERNET READ MULTIPLEXER, checked against both C models. A netlist
# property, so it is read from the wire list directly rather than by running a
# machine: the muffler reaches IOB on bit 15 alone (HM pp.101-102, disk.c's
# 0x0001) and the host number occupies IOB.00-07, the high byte (ethernet.c's
# local_host << 8).
disk-read-check:
	@cd .. && python3 tools/sil_check_diskread.py

# THE TRIDENT DRIVE INTERFACE: the list a drive model has to satisfy, taken
# from the packages that translate between ECL and the cable rather than from
# a pinout document. Gated because a generator change can silently drop a
# cable signal.
disk-interface-check:
	@cd .. && python3 tools/sil_disk_interface.py

# THE IM MAP: which of ContB's 144 F10415A packages holds which microinstruction
# field bit, for which of the four banks. What a bench needs to PRELOAD a whole
# world instead of walking it over the control-processor bus.
im-map-check:
	@cd .. && python3 tools/sil_im_map.py

# IM PARITY: the convention, derived from data and checked against PARC's own
# hand-coded microinstructions. The IRTable's five-byte format carries explicit
# P015/P1631 bits, and all eight entries carry ODD parity over the 17-bit half.
im-parity-check:
	@cd .. && python3 tools/sil_im_parity.py

# A WORD INTO THE STORAGE ARRAY: the msa board standalone, where the bench owns
# the DRAM strobes, so a write can be driven without a memory reference.
msa-test:
	@$(MAKE) -C verilator msa-test

# THE IFU in a machine: no emulator microcode dispatches without it, so no
# world runs without it.
ifu-test:
	@$(MAKE) -C verilator ifu-test

# The IFU with REAL MICROCODE running: does it come out of hold and fetch?
ifufetch-test:
	@$(MAKE) -C verilator ifufetch-test

# The REAL firmware driving BaseBd+ContA+ContB+ProcH+ProcL. Diagnostic.
firmware-probe:
	@$(MAKE) -C verilator firmware-probe
boards: backplane

# The boards first, then the top module -- it re-runs the SAME classification
# the board modules were emitted with, so it must not see a stale one.
generated-boards:
	@mkdir -p generated
	@python3 $(TOOLS)/sil_gen_all.py --sil $(SIL) --dict '$(ECLDICT)' \
		--out generated

# Which boards the machine has. A working monochrome Dorado by default;
# override to bring the machine up a board at a time, e.g.
#   make backplane MACHINE=--boards=ProcH,ProcL
MACHINE ?=
backplane: generated-boards
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_backplane.v \
		$(MACHINE)
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_baseboard.v \
		--boards=BaseBd --module=dorado_baseboard >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_control.v \
		--boards=ContA,ContB --module=dorado_control >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_proc.v \
		--boards=ContA,ContB,ProcH,ProcL --module=dorado_proc >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_boot.v \
		--boards=BaseBd,ContA,ContB,ProcH,ProcL --module=dorado_boot >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_mem.v \
		--boards=ContA,ContB,ProcH,ProcL,MemC,MemD,MemX \
		--module=dorado_mem >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_storage.v \
		--boards=ContA,ContB,ProcH,ProcL,MemC,MemD,MemX,msa \
		--module=dorado_storage >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_ifu.v \
		--boards=ContA,ContB,ProcH,ProcL,MemC,MemD,MemX,IFU \
		--module=dorado_ifu >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_display.v \
		--boards=ContA,ContB,ProcH,ProcL,MemC,MemD,MemX,msa,DispY \
		--module=dorado_display >/dev/null
	@python3 $(TOOLS)/sil_backplane.py --emit generated/dorado_disk.v \
		--boards=ContA,ContB,ProcH,ProcL,MemC,MemD,MemX,msa,DskEth \
		--module=dorado_disk >/dev/null

cells:
	@python3 $(TOOLS)/sil_emit_cells.py --sil $(SIL) \
		--dict '$(ECLDICT)' --dict '$(TTLDICT)' --out cells

cells-force:
	@python3 $(TOOLS)/sil_emit_cells.py --sil $(SIL) \
		--dict '$(ECLDICT)' --dict '$(TTLDICT)' --out cells --force

lint:
	@$(MAKE) -C verilator lint

clean-generated:
	rm -f generated/*.v
