# Generate the Dorado RTL from PARC's own Sil design-automation output.
#
#   make boards   regenerate verilog/generated/*.v from chm/sil, including
#                 the backplane top module that wires them together
#   make cells    (re)emit missing cell skeletons -- never overwrites a
#                 hand-written model; use `make cells-force` deliberately
#   make lint     every board must still elaborate under Verilator
#
# The inputs are all original Xerox data; see docs/verilog-from-sil.md.
SIL     = ../chm/sil
ECLDICT = $(SIL)/msa-Rev-Bg.dm!1_/ecldict.analyze
TTLDICT = $(SIL)/msa-Rev-Bg.dm!1_/ttldict.analyze
TOOLS   = ../tools

.PHONY: boards backplane cells cells-force lint clean-generated
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
