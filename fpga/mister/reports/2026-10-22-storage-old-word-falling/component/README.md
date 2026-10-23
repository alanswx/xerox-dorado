# Falling-edge preservation of the accepted old word

Targets actual native old_data -> read_pipe hold failures. One accepted-write flag per group records the host rising-edge handshake; old_data captures cache_data on the following falling edge. Acceptance requires waiting=false, excluding a same-edge response/cache update. cache_data is stable until the falling sample; the first old-word consumer follows a later write ACK. Original machine/DRAM and request/response epochs remain. No SDC exception/frequency change.

Exact qualified690f reference: both simulators pass40000 checks for each width2/8/12/16 and variable/minimum response:16 positives/640000 comparisons. Public outputs, address state and other original group state match. old_data may differ in its invalid half-cycle; its original machine consumer is checked. Two actual new-word-instead-of-old variants reject.

Original144-chip MSA/physical backing replay passes with early replies OFF/ON, all9437184 bits/outputs/750reads/7872writes/1111transitions. Original/candidate raw traces INCLUDING host epochs are BYTE-IDENTICAL:104113ticks without early and103659 with early. First fixture guard expected CAPTURE1 but found0; stopped before output. Corrected adapter explicitly selects qualified CAPTURE1 for both and reproduces the old trace. Failure retained.

Functional/component proof, not native hold closure or firmware qualification. Next fit must inspect falling-edge paths and extra control inventory across all conditions. Full context/source/logs/traces preserved. Verify: python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-old-word-falling/component
