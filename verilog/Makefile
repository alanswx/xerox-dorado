# Generate the Dorado RTL from PARC's own Sil design-automation output.
#
#   make boards   regenerate verilog/generated/*.v from chm/sil, including
#                 the backplane top module that wires them together
#   make cells    (re)emit missing cell skeletons -- never overwrites a
#                 hand-written model; use `make cells-force` deliberately
#   make proms    regenerate proms/*.mem and the per-package images
#   make lint       every board must still elaborate under Verilator
#   make cell-check every cell wires the inputs PARC says its gates have
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
#   make startseq     PARC's own boot sequence replayed (diagnostic, not a gate)
#   make machine-test the assembled machine clocks and its clock tree runs
#   make baseboard-test the BaseBoard's 6502 boots from its own EPROMs
#
# The inputs are all original Xerox data; see docs/verilog-from-sil.md.
SIL     = ../chm/sil
ECLDICT = $(SIL)/msa-Rev-Bg.dm!1_/ecldict.analyze
TTLDICT = $(SIL)/msa-Rev-Bg.dm!1_/ttldict.analyze
TOOLS   = ../tools

.PHONY: boards backplane proms cells cells-force lint cell-check loop-check \
        prom-test alu-test alu-diff cpreg-diff mir-diff mirreg-diff run-test \
        writeim-test \
        datapath-test \
        startseq \
        machine-test \
        baseboard-test \
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
cell-check:
	@python3 $(TOOLS)/sil_check_cells.py

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

# PARC's own DoDoradoMicroInst replayed. A diagnostic, not a gate.
startseq:
	@$(MAKE) -C verilator startseq

# Does the assembled machine run? Needs the harness built first.
machine-test:
	@$(MAKE) -C verilator machine-test

# Does the BaseBoard's 6502 come out of reset and fetch its reset vector?
baseboard-test:
	@$(MAKE) -C verilator baseboard-test
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
