# Frozen IFUM grouping review

All27 original1024x1 packages,24 data/3parity bits and27 read-pipeline bits
are accounted for against the original IFU wire list. Seven damaged board
controls reject. The two programming words contain11/16bits, but their
registered address replicas do not prove equal through pure scalar logic.
Keep all eleven independently addressed groups, widths1/5/1/1/4/1/5/2/1/4/2.

Every original CE port is omitted in both wire list and generated board.
This audit does not change it to an explicit zero; any such tie-off requires
separate qualification. Stateful or unknown outputs remain independent atoms.

Review only: no RAM adapter, state restore, complete IFU coverage, physical
snapshot or timing qualification. Frozen sources and exact executed inputs
are compressed without content changes.
