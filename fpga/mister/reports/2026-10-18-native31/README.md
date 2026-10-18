# Native31: 50 MHz / SYSPER4, storage selection tree

Frozen build: `misterubuntu:/home/alans/mister/Dorado_Native31_20261018`.
RBF SHA256 `ca5572a080cf06655e83497c089a74e6c3c066d47bc9390d44b44b4ddc4d7705`.
Native30 diagnostics plus the tested balanced arbiter, PLL40 ->50MHz.
Initial Quartus17 implicit-generate failure was corrected before the final
map/fit; build.py + resume.py record the recipe. Final inputs are hashed.

Fit succeeds:35,170/41,910 ALMs (84%),305 RAM blocks,35 DSPs;15m05s.
All29 standard STA checks pass, minimum+0.080ns, core setup+1.210ns.
The fitted core clock is20.000ns/50MHz. Zero illegal/unconstrained clocks.
Framework external ports remain unconstrained as detailed in the report.
No new post-fit exception: native26-before-review constraints retained.
Both fast/slow100-path hold reports have zero violations. The inferred
EccComputeErr combinational-loop warning is the same original DskEth cell
as Native29. Fmax56.88MHz is a report, not a qualified operating target.
The new worst core setup path is MSA address/readiness -> falling-edge
network_step,7.673ns data against a10ns relationship (+1.210ns slack).
The old22.666ns arbitration/address-latched path no longer dominates.
Detailed reports are compressed without changing their contents.

Hardware: loaded through sole-owner Main on MiSTer14 around03:48UTC.
Original firmware/BFN113, Cedar login and fast Guest input pass; real
Basic.Loadees transfers are underway. Viewers/Echo/mouse pending. Main21722, server21719. Per-core1080p30 request,
fresh corrected Cedar seed, original firmware/real disk/serial Ethernet.
No snapshot restore or host guest-state completion.

Main's existing OSD option selects coherent enabled/host edge counts.
Selection scripts call its verified user_io_status_set at input_poll;
they never create a second SPI owner. count sampler reads Main's log only.
Firmware sample over20s: deltaH999,988,506, deltaM264,614,436,
enabled fraction26.4617%, measured fabric~50.01MHz. This is a halted-CPU
firmware workload, not Cedar performance or a retired-instruction rate.
The SYSPER4 instruction ceiling is6.25M/s before pauses,37.5% of original
Dorado; it is50% higher than the old33.333MHz ceiling. Do not claim50%
measured guest speedup. Counter wrap is85.9s at50MHz; sample intervals<60s.

Arbiter independent scoreboard and all9,437,184-bit storage equivalence:
see ../2026-10-18-arbiter-tree/. Clock redesign still requires the original
family/latch contracts in docs/verilog-clock-plan.md.

HDMI was initially black while Main's internal capture showed Cedar login.
Normal Main video_fb_enable(1,0), then(0,0), at input_poll restored actual
HDMI without reset or guest-state writes. This repeats Native26's host-video
workaround; root cause remains open. Actual screenshot is fpga-hdmi-after-guest.png.
Fresh UIO23 timing confirms3333332ticks at100MHz:30.000012Hz,74.25003MHz
with2200x1125totals. Source field timing varies with backing stalls/workload.

Additional original-board destination audit (slow corner): processor setup
+2.527ns/17.182ns data/21logic levels; memory+2.537ns/17.172ns; I/O
+2.932ns/16.772ns, each against20ns. All worst paths start ContB b24.qb
(ASEL0) and end continuous DMuxData d_pre captures (ProcL k22, MemC l24,
DskEth l23). Thus moving the backend gate alone does not establish66.667MHz
operation: the sampled diagnostic/data-setup cones also need accurate enable
translation/physical optimization. No new exceptions or circuit changes were
made for these reports. Details: board-paths.tcl and compressed reports.
All34 Basic.Loadees complete; single n auto-completes profile choice to No
and advances to BootEssentials (actual fpga-bootessentials.png).
