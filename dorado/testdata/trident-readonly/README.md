# Trident Read-Only Images

This directory holds generated golden Trident pack images used by Make targets.
The raw `.pack` files are intentionally gitignored because they are large.

Use `make trident-readonly-images` from `dorado/` to create the read-only
golden images. Runtime targets such as `make run-alto-disk` and
`make run-tricond-pack` copy these images into `build/run-disks/` and run
against the writable copy, so each run starts from a clean pack.
