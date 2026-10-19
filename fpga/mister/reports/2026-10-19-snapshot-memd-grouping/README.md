# MemD cache RAM logical grouping review

All72F10470 packages are pinned to the original wire list:294,912RAM bits and
72raw read-pipeline bits. Nineteen literal address/write/select groups reduce
to fourteen groups through proven scalar Boolean fanout identities, with
widths3/6/9. Stateful/unknown address replicas remain distinct atoms. The
indexed storage-delay expression, and every assignment to its target, remain
opaque rather than selecting a generate branch. Parameterized cell sources
are included in dependency hashes.

Clock, address, write, select, data and tied-output connections are compared
with the wire list; eight damaged boards reject. Exact source/wire-list and
logical proof evidence is retained, with raw text gzip-compressed unchanged.
This is grouping review only: no RAM rewrite, CE tie-off, full MemD state
classification, global hold, restore or timing qualification is claimed.
