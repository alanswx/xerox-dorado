# Explicit All Paths hold optimization: rejected

The existing V3 builder2166275 completed all nine stages normally. The full31-check device summary still has minimum setup+0.686ns and hold-0.044ns; detailed slow100C/-40C hold remains-0.044/-0.008ns. No assembly or hardware load occurred.

The fitted settings report confirms All Paths was already the effective Optimize Hold Timing default. Adding the assignment did not repair the actual row/column-to-saved-address path. Do not repeat this setting, waive the path, or claim a timing pass from the successful tool exits.

The309-member receipt includes all302 terminal report/log/manifest files, readback hashes and absent process checks, the exact staged builder/stager, and the collection script. All503 source hashes were read back against the frozen local stage. Existing rejected V2 evidence remains unchanged.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/native57-hold-all-paths-rejected`
