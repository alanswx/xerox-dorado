# Complete modeled CRC9401 snapshot primitive PASS

Original F9401 is byte-identical to frozen Native57. Both actual DskEth PRECAP
modes retain all19bits: CRC16, sampled clock, sampled data and sampled CWE.
Two18-bit transport words carry18+1bits; padding reads zero/is ignored on writes,
indices2/3 reject. Original polynomial selection, reset/preset priority and
falling-edge behavior remain unchanged. No production cell is modified.

Original RTL plus independent bitwise CRC oracle agree for108,190host edges,
1,123held edges/1,120requests/576restored cell words, both modes/all8polynomial
selects/all19state bits, destructive mutations/individual restore, quiet release,
first resumed edge and20,000further samples. Eight broken RTL variants and two
changed original-source guards reject. Fresh XML verifies actual PRECAP0/1,
classifies every sequential field and rejects ten damaged schemas in each mode.
All sampled histories are saved even when unused by PRECAP0.

Matched50MHz isolated fits pass all four operating conditions/min+0.170ns;
no illegal/unconstrained clocks or I/O, no RAM/extra timing exceptions.
Resources baseline65->97ALMs/65->201registers; counts include shell/admin and
are not a full-board or enabled whole-machine estimate. Five damaged source,
constraint, macro, hold and warning evidence controls reject. Exact staged
builder and raw reports are preserved; CRC clock/wire behavior is unchanged.

This qualifies one primitive for pending DskEth state work. It does not provide
full DskEth/global save/load, an FPGA snapshot, clock correction or a speed gain.
