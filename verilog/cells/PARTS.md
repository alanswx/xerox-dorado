# Part functions for the Dorado cell library

Function descriptions from Motorola's **MECL Pocket Book** (bitsavers scan,
`archive.org/stream/bitsavers_motoroladaok_3820706/MECL_Pocket_Book_djvu.txt`).
Pin assignments come from PARC's own `EclDict.Analyze` / `TtlDict.Analyze`,
NOT from here -- see `docs/verilog-from-sil.md` for why the two sources are
kept separate.

The two corroborate each other in a useful way. The datasheet says MC10100 is
a "Quad 2-input NOR Gate **with Strobe**", and PARC's dictionary independently
describes it as `a,IN,4,5 > a,c,9 > a,OUT,2` -- four gates each with two
inputs, plus one **common** pin shared by all four. That common pin *is* the
strobe. Neither source alone would have told us that with confidence.

## MECL 10K, as used on the Dorado

| part | function | on the boards |
|---|---|---|
| MC10100 | Quad 2-input NOR with strobe | 5 |
| MC10101 | Quad OR/NOR | 8 |
| MC10102 | Quad 2-input NOR | 6 |
| MC10103 | Quad 2-input OR | 3 |
| MC10104 | Quad 2-input AND | 1 |
| MC10105 | Triple 2-3-2 input OR/NOR | - |
| MC10106 | Triple 4-3-3 input NOR | 2 |
| MC10107 | Triple 2-input Exclusive OR / Exclusive NOR | - |
| MC10109 | Dual 4-5-input OR/NOR | 1 |
| MC10110 | Dual 3-input 3-output OR | - |
| MC10111 | Dual 3-input 3-output NOR | - |
| MC10113 | Quad Exclusive OR | 1 |
| MC10115 | Quad Line Receiver | - |
| MC10116 | Triple Line Receiver | - |
| MC10117 | Dual 2-wide 2-3-input OR-AND / OR-AND-INVERT | 1 |
| MC10118 | Dual 2-wide 3-input OR-AND | 3 |
| MC10119 | 4-wide 4-3-3-3-input OR-AND | 3 |
| MC10121 | 4-wide OR-AND / OR-AND-INVERT | 2 |
| MC10123 | Triple 4-3-3 input Bus Driver | - |
| MC10131 | Dual Type D Master-Slave Flip-Flop | - |
| MC10141 | Four-Bit Universal Shift Register | 6 |
| MC10158 | Quad 2-input Multiplexer (NON-inverting) | 88 |
| MC10159 | Quad 2-input Multiplexer (INVERTING) | 10 |
| MC10160 | 12-Bit Parity Generator/Checker | - |
| MC10161 | Binary to 1-8 Decoder (Low) | 2 |
| MC10162 | Binary to 1-8 Decoder (High) | - |
| MC10164 | 8-Line Multiplexer | 19 |
| MC10170 | 9-2-Bit Parity Generator/Checker | 8 |
| MC10173 | Quad 2-input Multiplexer/Latch | 81 |
| MC10174 | Dual 4-to-1 Multiplexer | 106 |
| MC10175 | Quint Latch | 2 |
| MC10176 | Hex D Master-Slave Flip-Flop | 307 |
| MC10181 | 4-Bit ALU / Function Generator | 8 |
| MC10195 | Hex Inverter/Buffer | 1 |
| MC10197 | Hex AND Gate | 2 |
| MC10210 | High-Speed Dual 3-input 3-output OR | 162 (as SE10210) |
| MC10211 | High-Speed Dual 3-input 3-output NOR | 29 |
| MC10212 | High-Speed Dual 3-input 3-output OR/NOR | 2 |
| MC10231 | High-Speed Dual Type D Master-Slave Flip-Flop | 152 |

Second sources are the same part: `MU10164` = `MC10164`, `SE10210` =
`MC10210`, `SE10211` = `MC10211`, `SE10231` = `MC10231`. The dictionary's
alias table records these (`>` separates equivalents), which is why the
boards mix MC/MU/SE/F prefixes for one function.

## Two conventions that matter for the models

**MECL is positive logic with two outputs.** An OR/NOR part brings out both
the OR and its complement; the dictionary marks them `OUT` and `o`. Where a
part is named just "NOR" or just "OR", only one is bonded out.

**Open emitters wire-OR.** Tying two MECL outputs together is a legal,
intentional OR -- the Dorado does it on 91 nets on ProcH alone. The generator
emits those as an explicit OR (see `sil_to_verilog.py`), so the cells
themselves never need to model it.

## Not MECL

The boards also carry TTL (`SN74166`, `SN74S174`, ...), memories (`MosRam`,
`MK4096P-6`, `MB7071H`), the BaseBoard's `MCS6502` + five `MCS6532` RIOTs,
and analog parts (`LM3911+20K`, `CA3140`). `TtlDict.Analyze` covers the TTL
pinouts; the memories and analog parts have neither dictionary entry nor a
model yet, and their ports come from the wire lists alone.
