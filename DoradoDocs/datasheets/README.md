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
| `ON_MECL_DataBook_DL122_rev7.pdf` | the whole MECL 10K family, 474 pages | the parts no per-part sheet is mirrored for -- MC10124's common strobe, MC10125's differential inputs, MC10195's XNOR truth table |
| `MC10181.pdf` | 4-bit ALU / function generator | the full 16-row function table, both columns -- this is the Dorado's ALU, and the table is transcribed straight into `cell_MC10181` |

The polarity rule these establish is written up in `docs/verilog-handoff.md`;
it found two wrong cells (`cell_MC10105` had its senses reversed, 31 packages
in the machine, and `cell_MC10103` gave one gate's two outputs the same
expression).

## Second batch (2026-08-19): the parts the RTL could not model

Fifteen parts, added because their cells were stubbed for want of a pinout or
a truth table. Every file below is a real PDF whose text layer names the part
and which carries a DIP pin assignment; the "diagram" column says what else is
on it.

| file | part | pinout | logic diagram / truth table | source |
|---|---|---|---|---|
| `MC10163.pdf` | Error detection-correction circuit (MemD, 4 pkgs) | yes, 3 packages | logic diagram + syndrome/check-bit generator tables (Figs 1-6) | 1978 MECL data book pp.109-114 |
| `MC10179.pdf` | Look-ahead carry block (MemC, 1 pkg) | yes | logic diagram with the G/P equations drawn out; Fig 1 shows it wired to four MC10181 | 1978 MECL data book pp.135-136 |
| `MC10182.pdf` | 2-bit ALU / function generator (ProcL, 3 pkgs) | yes | POSITIVE LOGIC function table (4 logic + 4 arithmetic) and the full gate-level diagram | 1978 MECL data book pp.142-144 |
| `MC1650.pdf` | Dual A/D comparator, MECL III (DskEth, 4 pkgs) | yes | latch truth table + 8 characterisation figures | 1978 MECL data book pp.215-224 |
| `MC1660.pdf` | Dual 4-input OR/NOR, MECL III | yes | logic diagram, X = A+B+C+D and its complement | 1978 MECL data book p.231 |
| `MC1662.pdf` | **Quad 2-input NOR**, MECL III | yes | logic diagram -- this is the sheet DL122 rev 7 had deleted, so the function no longer has to be inferred | 1978 MECL data book p.232 |
| `MC1664.pdf` | Quad 2-input OR, MECL III | yes | logic diagram; the non-inverting counterpart of MC1662, which pins the polarity pair | 1978 MECL data book p.233 |
| `MC1672.pdf` | Triple 2-input XOR, MECL III (IFU, MemC) | yes | logic diagram | 1978 MECL data book p.239 |
| `MC1674.pdf` | Triple 2-input XNOR, MECL III (MemX) | yes | logic diagram | 1978 MECL data book p.240 |
| `MC1690.pdf` | UHF prescaler type-D flip-flop, MECL III | yes | flip-flop truth table + toggle-frequency test circuit | 1978 MECL data book pp.244-245 (bonus: this is the BaseBoard clock divider chain) |
| `F9401.pdf` | CRC generator/checker (DskEth, 2 pkgs) | connection diagram (scan) | **Table 1, the eight selectable polynomials against S2/S1/S0** -- extracts cleanly as text -- plus block diagram, pin function list and the generate/check timing figures | 1976 Fairchild Macrologic pp.26-29 |
| `8T98.pdf` | Hex 3-state inverter (DskEth, 3 pkgs) | yes | FUNCTION TABLE; note the 8T97/8T98 split their enables 4+2, unlike 8T95/8T96 | 1981 Signetics Logic Interface pp.72-75 |
| `MC12040.pdf` | Phase-frequency detector (BaseBd, 1 pkg) | PIN ASSIGNMENT | logic diagram + truth table + applications section | 1989 DL122 rev 4 pp.512-515 |
| `MC14521B.pdf` | 24-stage CMOS frequency divider (BaseBd, 1 pkg) | PIN ASSIGNMENT | LOGIC DIAGRAM of the whole flip-flop chain, plus the RC-oscillator figures | 1988 Motorola CMOS Logic Data pp.328-333 |
| `MC10318.pdf` | 8-bit video D/A converter (DispY, 1 pkg) | PIN CONNECTIONS | equivalent circuit (Fig 5) and the 75-ohm / TTL-input connection figures | 1979 Motorola Interface ICs pp.503-508 |
| `SN74123.pdf` | Dual retriggerable monostable (DskEth, 2 pkgs) | yes | function table and the pulse-width equations | ti.com SDLS043 |
| `CD4051.pdf` | CMOS analog mux (BaseBd, 2 pkgs) | yes | truth table; modern TI CD405xB sheet, pin-compatible with the RCA original | ti.com SCHS047O |

### Lower priority (analog / mixed-signal, likely substituted)

| file | part | contents | source |
|---|---|---|---|
| `AM2615.pdf` | AMD Am2615/9615 **dual differential line receiver** (7 pkgs) | connection diagram, logic diagram, threshold characteristics, fail-safe note. AMD's own sheet says it is equivalent to the Fairchild 9615 | 1977 AMD Linear and Interface pp.152-157 |
| `CA3140.pdf` | BiMOS op-amp (5 pkgs) | pinout, schematic, full characterisation | renesas.com |
| `LM339.pdf` | Quad differential comparator (1 pkg) | pinout, schematic | ti.com SLCS006Z |
| `MPQ6002.pdf` | Motorola NPN/PNP quad, 14-pin (5 pkgs) | package pin diagram and the type-A/complementary arrangement; the sheet is filed under MPQ6001/D and covers both | pccomponents.com mirror |
| `LM3911.pdf` | National temperature controller (6 pkgs, as `LM3911+20K`) | 8-page National sheet, schematic + physical dimensions. **Watermarked** by the mirror that has it; text extracts but is interleaved with the watermark | 100y.com.tw mirror |

## The data books, and what else is in them

`Motorola_MECL_DataBook_1978.pdf` (13 MB, 266 pages) is kept whole because it
is the ONLY MECL III source we have, and because it is the reason the newer
book failed us. DL122 rev 7 says so itself, on its page 4 under **"Deleted
Devices / DATA SHEETS DELETED"**:

    MC10137   MC1648   MC1650   MC1651   MC1658   MC1660   MC1662   MC1670   MC1692

That is the whole MECL III set the Dorado uses. MC10163, MC10179 and MC10182
are not even in rev 7's deleted list -- they went in some earlier revision --
so all three were unmodellable from the book we had. Every `MC1*` sheet in the
table above was cut from the 1978 book. Its MECL III section begins at PDF page
206; other parts it carries, by PDF page:

    MC10163 109-114   MC10179 135-136   MC10180 137-138   MC10181 139-141
    MC10182 142-144   MC10183 145-149   MC1648  207-214   MC1650  215-224
    MC1654  225-226   MC1658  227-230   MC1660  231       MC1662  232
    MC1664  233       MC1666  234       MC1668  235       MC1670  236-238
    MC1672  239       MC1674  240       MC1678  241-242   MC1688  243
    MC1690  244-245   MC1692  246-247   MC1694  248       MC1697  249

Source: `bitsavers.org/components/motorola/_dataBooks/1978_Motorola_MECL.pdf`.

The other books were fetched, cut from, and NOT kept (they are 20-30 MB each
and each yielded one part). Re-download from bitsavers if more is needed:

| book | gave us | page |
|---|---|---|
| `motorola/_dataBooks/1989_DL122r4_Motorola_MECL_Device_Data.pdf` | MC12040 | 512-515 |
| `motorola/_dataBooks/1988_Motorola_CMOS_Logic_Data.pdf` | MC14521B | 328-333 |
| `motorola/_dataBooks/1979_Motorola_Interface_Integrated_Circuits.pdf` | MC10318 | 503-508 |
| `fairchild/_dataBooks/1976_Fairchild_Macrologic.pdf` | 9401 | 26-29 |
| `signetics/_dataBooks/1981_Signetics_Logic_Interface.pdf` | 8T95/96/97/98 | 72-75 |
| `amd/_dataBooks/1977_AMD_Linear_and_Interface_Data_Book.pdf` | Am2615/9615 | 152-157 |

## Parts with no data sheet, and what we know anyway

**`K1115A` (2 pkgs: DispY a05, DskEth j20) -- IDENTIFIED, sheet not obtainable.**
It is not a delay line. It is a **Motorola crystal clock oscillator**, one of the
`K1100A / K1114A / K1115A / K1116A / K1144A / K1145A` family that `ttldict.analyze`
lists together, all 14-pin. Motorola's own description of the series is "crystal
clock oscillators", TTL-compatible output, 250 kHz to 70 MHz; surviving K1115A
parts are marked with a frequency (36.000 MHz on one seen for sale, 10 MHz on
another), so **the frequency is a suffix on the order code and is not recoverable
from the part number** -- read it off the board photo or the schematic.

The wire lists already give the whole port map, and it is the standard 14-pin DIP
oscillator footprint:

    pin 7  -> GND      (DispY GND8, DskEth GND)
    pin 8  -> output   (EclDict names it RFout; the only `o` pin)
    pin 14 -> Vcc      (DispY VCC50, DskEth VCC130)

So `cell_K1115A` can be modelled as a free-running clock source on p8 without the
sheet; only the frequency is missing. A 3-page Motorola scan titled "Crystal Clock
Oscillators" is indexed by datasheetarchive under `K1115AM`, but that site and
alldatasheet both refuse automated fetches (44-byte replies / HTTP 403), and the
part appears in no bitsavers Motorola data book or catalogue -- the K-series was
Component Products, not semiconductors.

**`ADC-MC8BC` (1 pkg) -- not found.** 16-pin, ceramic. Only datasheetarchive
indexes the name, and it is blocked as above. Not in the Motorola, TI, National,
Signetics, Fairchild or AMD data-book indexes on bitsavers; the `ADC-MC` prefix
suggests a modules house (Datel / Micro Networks), whose catalogues bitsavers does
not mirror.

## Where to get more

Motorola's per-part technical data sheets are the useful form -- they carry
the DIP pin assignment drawn out, with bars on the inverting outputs. The
`futurlec.com/Datasheet/Motorola/<PART>.pdf` pattern works for some parts and
404s for others; a search for "<part> datasheet pdf" finds the rest.

**Bitsavers is the best source by a wide margin.** `bitsavers.org/components/
<maker>/_dataBooks/` has the period books for every maker on these boards, they
have a text layer, and `pdftotext -layout` plus a page split (`pdfseparate` +
`pdfunite`, both in poppler) turns one into a per-part sheet in a minute. Grep
the whole book for the part number first; the page with the most hits is the
first page of its sheet, and the sheet runs until the next part's title appears
as the first line of a page.

**Not useful, so do not spend time on them:** the MECL Pocket Book scan that
`PARTS.md` cites (archive.org, `bitsavers_motoroladaok_3820706`) is OCR of
functions and schematics with no pinout tables, and the Fairchild 100K data
books on bitsavers are the wrong FAMILY -- the Dorado's Fairchild parts
(F10016, F10145A, F10415A, F10470) are ECL 10K, not 100K.

**Sites that block us:** `datasheetarchive.com` and `alldatasheet.com` return
44-byte bodies or HTTP 403 to curl and to WebFetch regardless of headers, and
`onsemi.com/pdf/datasheet/*` 403s every request -- so a part that only those
three index is effectively unobtainable here. `ti.com/lit/ds/symlink/<part>.pdf`
and `renesas.com/en/document/dst/<slug>` both serve cleanly.

Save anything fetched HERE, not in a scratch directory. Two of these were
fetched into a session temp folder first and would have been lost.
