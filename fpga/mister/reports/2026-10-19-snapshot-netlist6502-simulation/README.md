# Actual netlist 6502 state access: simulation and classification

The generated adapter preserves every one of the 1,725 internal `q` nodes
of the actual BaseBoard vendor CPU. It retains the original combinational
MOS netlist, iterative relaxation and initial node values. The original
76,486-byte CRLF source is unchanged and hash pinned. This does not replace
the CPU with a behavioral 6502 or serialize architectural registers alone.

The bounded local interface has 96 words: 95 x 18 bits and a final 15 bits.
Indices 96..127 return an error without writing. Administrative access
requires freeze, externally confirmed hold and entry acknowledgment; all
normal state advances use the actual guest advance. A global barrier is
not installed by this primitive.

The original/candidate simulation passes 50,972 host edges, 7,832 held
edges, 6,747 accepted requests and 4,218 writes. It exercises all 1,725 bits
as isolated set bits with complete physical-vector checks, every 7-bit
index, all 16,384 external pin vectors, dense data patterns, invalid and
entry/resume requests, candidate-only destructive continuation, restoration,
first-edge agreement and 20,000 additional steps. The independent state
oracle uses its own bit/word mapping and checks the pre-edge vendor next-node
vector; the untouched vendor CPU independently checks continuation and
outputs. This is not independent validation of the MOS netlist itself.

Eight broken RTL variants fail: missing advance gating, missing freeze,
running access, invalid-index acceptance, lost last word, word alias,
wrong relaxation source and lost restore bit. Two changed-source guards
reject. Fresh original XML classifies the sole sequential vector and all
derived combinational candidates; eight damaged inventories/schemas reject.
The first classifier rejected parameterized MUX module names; the final
version explicitly recognizes the six actual variants. Earlier XML remains.

No standalone routed fit, full BaseBoard integration or FPGA save/load is
qualified here. Next: matched routed timing/state-retention qualification,
RIOT and other BaseBoard primitives, complete board tests and machine-wide
barriers/ARM/disk bundle integration.

Run `test_6502.py --output NEW_DIRECTORY --mutations` and
`classify_6502.py --output ANOTHER_NEW_DIRECTORY` from this directory's
repository tools. Raw source/XML/log output is losslessly gzip compressed.
