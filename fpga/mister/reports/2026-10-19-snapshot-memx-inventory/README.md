# Frozen native MemX: unclassified state inventory

Fresh XML from the exact Native55 source, using the generated-scope-aware
inventory tool. Twenty-one original MosRam instances each elaborate64Kx1:
1,376,256 map RAM bits. Their row/column latches, RAS/CAS/write history,
pending data/write, raw read pipeline and held output also need preservation.
The board owns14timing bits:ten clock/lag bits,three map-buffer delay bits
and one module-selector clock history. Do not infer a flat RAM-only snapshot.

Sequential candidate total1,379,362bits includes simulation-only diagnostics
that are not yet classified. Initial-only2363bits includes PROM assets and
constants also not yet classified. No field is approved SAVE/RECOMPUTE here;
no grouping, adapter, restore, timing or full-machine snapshot is qualified.
Exact board/cell/PROM/tool sources and raw XML are preserved. This is the next
board's review input while Native56b MemD integration builds, not completion.
