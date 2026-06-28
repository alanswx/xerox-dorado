# CHM Basic Disks

This directory holds selected Alto basic disk streams from the CHM PARC
archive.

Important: `*.BFS!N` files here are Palo-readable BFS transfer streams, not
ContrAlto/Palo AAR sector images. Use `AltoInfo/palo/src/par -ibfs -1 FILE ...`
to load them. A modifying Palo run without `-obfs` saves the writable copy as
an AAR image, which `dsk2trident` can then map into a Trident pack.

`BcplProg.BFS!13` is used by `make -C dorado lisp-bcplprog-loader-image` as an
installed Alto OS layout whose Swat/Swatee labels and serials must be preserved.
