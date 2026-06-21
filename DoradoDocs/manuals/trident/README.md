# Century Data Trident manuals

Source directory: https://www.bitsavers.org/pdf/centuryData/Trident/

Pulled locally on 2026-06-21 for Dorado disk-controller bring-up. These
documents complement Dorado Hardware Manual section 9: the Xerox manual
describes the Dorado DSK controller, while these manuals describe the
Century Data Trident drive interface, timing, and mechanics.

## Most relevant

| File | Use |
|---|---|
| `76205-902_Performance_Specification_Models_T25_T50_T80_T200_and_T300_198011.pdf` | T-80/T-300 physical specs and timing. Referenced by `docs/disk-architecture.md`. |
| `Trident_T25_T50_T80_OEM_Reference_Manual.pdf` | Raw T-25/T-50/T-80 drive interface, sector/index timing, read/write timing. |
| `76205-205_T25_T50_T80_Installation_and_Operation_197905.pdf` | T-25/T-50/T-80 operation, configuration, and cabling context. |
| `76205-303_T25_T50_T80_T85_Maintenance_198107.pdf` | T-80 maintenance-level detail and diagnostic behavior. |
| `Trident_T25_T50_T80_schem_Mar81.pdf` | T-25/T-50/T-80 drive schematics. |

## Also useful

| File | Use |
|---|---|
| `76200-402_T200_T300_Theory_of_Operation_198101.pdf` | T200/T300 theory of operation for larger Trident packs. |
| `76206-304_Trident_Disk_Drives_Models_T202_T302_Maintenance_Manual_Apr82.pdf` | Later T202/T302 maintenance reference; useful for cross-checking T300-family behavior. |
| `Trident_1150_Formatter_Performance_Specification_1977.pdf` | Formatter performance reference; keep around for sector timing and interface questions. |
