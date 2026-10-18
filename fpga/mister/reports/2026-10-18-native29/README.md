# Native29: first clock-speed increment

Frozen project: `misterubuntu:/home/alans/mister/Dorado_Native29_20261018`.
Native28 logic (Native26 CPU/storage plus passive fault observation and the
host key-event queue), PLL33.333333 ->40MHz, SYSPER remains4. Instruction
ceiling5.0M/s versus4.167M/s; actual retired throughput not measured.

Timing:29 checks pass, minimum+0.019ns; core setup+1.499ns. Core Fmax42.55MHz.
The new worst core path is MSA address/readiness through arbitration and
payload selection into address_latched:22.666ns data,25ns relationship.
The earlier Native26 half-cycle network_step path is not this fit's worst.
Zero illegal/unconstrained clocks. Framework external input/output paths
remain unconstrained as enumerated in the UCP report.

Exact same-cell F10145A write-address hold review: MemD h15 and MemC j07,
bit1, each1source/4own MLAB address sinks. Address capture uses!p13 and RAM
commit uses p13&&!we_d&&ce_lat, mutually exclusive. Only those hold pairs
are cut, with exact collection-size assertions. Setup, read, data and
enable paths remain timed. RBF unchanged by post-fit SDC review.

RBF SHA256:
`a963f81c353b240a5528bdffb1b1d6c43df3dd0e8aa37afed38e142292337acf`.

Loaded on MiSTer14 through Main12701, same immutable1fb2... binary and sole
SPI owner. Fresh file server20653, immutable1d5..., new server.log. Fresh
corrected seed f3bce2a8... copied to Cedar29_20261018/cedar-work.pack. Original
firmware emits fresh station042/BFN113; real HDMI displays Cedar login.
Fast `Guest\n\n` input (0.1s transitions) arrives as Guest.pa, proceeds through
Grapevine-down fallback and requests Basic.Loadees from Cedar6.1/Top.
No guest-state import, memory repair or host IOCB completion. Initial cold
firmware black HDMI is not an OS screenshot; fpga-login/after-guest are real
HDMI with complete glyph rows. Full Viewers/Echo/mouse qualification pending.

INI selects the validated-form1920x1080p30/74.25MHz modeline at core load;
fresh physical-frequency read remains pending. Native26-field-rate.json is
a read-only source-scaler cadence baseline, NOT a CPU/HDMI rate measurement.

Reproduction: build.py clones frozen Native28, changes the PLL, restores
pre-fit constraints, runs map/fit/asm/STA/clock audit. Use reviewed.sdc only
for this exact placed design; re-review endpoints after another placement.
Actual packet/keyboard/serial regressions are in input-gates/ethernet-gates.
