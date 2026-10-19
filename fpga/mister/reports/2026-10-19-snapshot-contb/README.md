# Actual ContB IM grouping for snapshot access

`snapshot/audit_contb_im.py` checks all144 F10415A packages against the
original ContB wire list, then follows scalar combinational fanout logic in
the actual generated board and unchanged cell sources. Unsupported functions,
state and board inputs remain distinct atoms; matching Boolean expressions
are a sufficient equivalence proof, not a claim that every unequal expression
is different. Sources and tool hashes are pinned here.

Literal clock/address/WE/CE connections produce124 small groups. Following
the buffers proves eight18-bit groups: two separately written halves in each
of four banks. Each left half includes IMLH and each right half includes IMRH.
The complete physical IM has34 instruction bits plus2 stored parity bits:
147,456 RAM bits (18KiB), plus144 registered read-pipeline bits. The existing
isolated34-bit access prototype is not a complete physical IM implementation.

`snapshot/test_contb_im.py` checks1,506 Boolean expressions/48,192 truth cases
against Python's independent native bitwise evaluation. It accepts the actual
board and rejects missing parity, changed address/write/output/clock pins and
a changed fanout source for their intended reasons. A common inversion of
ALL address copies preserves grouping and is deliberately NOT rejected;
grouping does not establish cell function or logical bank selection. Existing
sil_im_map/original-cell qualifications cover separate semantics.

No RAM cell, generated board, clock or production build is changed. No state
port, global freeze, board fit, physical save/load or fast restore is qualified
here. Next use these real18-bit half-bank strobes and preserve each read
pipeline in a measured actual-board access experiment. Normal backend pauses
must still hold RAM read outputs; a host scanning clock must not silently add
guest read edges during those pauses. Full snapshots must also preserve the
rest of ContB and the other boards/devices/ARM state.
