# Part datasheets for the cell library

Manufacturer data sheets for the parts on the Dorado boards, kept here because
they answer questions PARC's own files cannot. `chm/sil/*/ecldict.analyze` is
the authority on PIN ASSIGNMENTS -- it is what ANALYZE used to check the
boards -- and `verilog/cells/PARTS.md` gives each part's function from the
MECL Pocket Book. Neither states OUTPUT POLARITY per pin, and that is exactly
what a cell model has to get right.

| file | part | what it settled |
|---|---|---|
| `MC10101.pdf` | Quad OR/NOR | EclDict role `OUT` is the INVERTING output (pins labelled A-bar-OUT..D-bar-OUT), `o` the non-inverting one |
| `MC10212.pdf` | Dual 3-input 3-output OR/NOR | the same rule again, independently: pins 3,4,12,13 barred, 2 and 14 not |
| `MC10117.pdf` | Dual 2-wide 2-3-input OR-AND | the `u`/`v` role letters, which the OR/NOR rule does not reach: `u` inverting, `v` not. Also that pin 9 is common to both gates |
| `MC10180.pdf` | Dual 2-bit adder/subtractor | the M0/M1 mode encoding -- a select input inverts its operand, so one part does add, subtract, reverse-subtract and negate |
| `MC10181.pdf` | 4-bit ALU / function generator | the full 16-row function table, both columns -- this is the Dorado's ALU, and the table is transcribed straight into `cell_MC10181` |

The polarity rule these establish is written up in `docs/verilog-handoff.md`;
it found two wrong cells (`cell_MC10105` had its senses reversed, 31 packages
in the machine, and `cell_MC10103` gave one gate's two outputs the same
expression).

## Where to get more

Motorola's per-part technical data sheets are the useful form -- they carry
the DIP pin assignment drawn out, with bars on the inverting outputs. The
`futurlec.com/Datasheet/Motorola/<PART>.pdf` pattern works for some parts and
404s for others; a search for "<part> datasheet pdf" finds the rest.

**Not useful, so do not spend time on them:** the MECL Pocket Book scan that
`PARTS.md` cites (archive.org, `bitsavers_motoroladaok_3820706`) is OCR of
functions and schematics with no pinout tables, and the Fairchild 100K data
books on bitsavers are the wrong FAMILY -- the Dorado's Fairchild parts
(F10016, F10145A, F10415A, F10470) are ECL 10K, not 100K.

Save anything fetched HERE, not in a scratch directory. Two of these were
fetched into a session temp folder first and would have been lost.
