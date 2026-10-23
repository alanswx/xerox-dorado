# Conditional RunRefresh capture ordering

Status: **PASS_CONDITIONAL_CONTROL_TRANSFER**. This is a transfer component;
native clock generation, full processor integration and FPGA timing remain open.
The preceding [root-only experiment](../experiment/README.md) remains incomplete,
including its two required profile failures and raw parity handoff gap.

The original Control-A l06.B RunRefresh Q drives l03's asynchronous Set inputs
directly. Its capture clock reaches l03 through f13. The additional AND in the
WantRunRfsh -> g04 -> dMemRun path and OR in h03 -> ClearMemStop matter:
newly available source Qs do not imply new data at those sinks. See HM §5.12
for RunRefresh's architectural role; the internal routes come from the original
ContA-Rev-Cd wirelist, not an invented reset or extra architectural cycle.

The generated `dorado_control_hold_ordered_step` adds one non-architectural
input, `refresh_before_run_capture`. Under the existing illustrative timing
profiles it is derived independently as `phase_FF_delay < gate_delay`. At
equal times the original event model captures before the nonblocking Q update.
The test supplies this descriptor from declared delays, never reference outputs.
It describes the release/capture order; it is not a software-visible control.

The component retains all 156 original Qs, 56 clock channels and 94 flags.
Only l03's two sink samples use the new helper. Its zero descriptor invokes the
old transfer unchanged. All baseline functions remain byte-identical, including
the 24 inherited source functions. The existing physical wiring audit passes;
16 additional pin checks bind the direct Set, data and Clear routes.

| Profile | Gate / ordinary FF / phase FF, ps | Descriptor | Icarus | Verilator |
|---|---|---:|---|---|
| balanced | 1000 / 1000 / 1000 | 0 | pass | pass |
| fast_gates | 250 / 1500 / 1000 | 0 | pass | pass |
| later_control | 1750 / 400 / 1250 | 1 | pass | pass |
| ca_later_control | 1000 / 2000 / 1000 | 0 | pass | pass |

Each comparison checks 5,678 events, every physical Q, every clock count and
all selected flags. All 56 clocks fire at least eight times. Counts and state
coverage agree between engines within each profile. The original gate reference
and full test sequence are unchanged except for the candidate instance/input.

Six negative comparisons fail on the intended physical bits in both engines:

- Wrong late arrival in the early profile fails at event 403, Q14/Q15; it also
  misses original memory phase clock 51.
- Wrong early arrival in the balanced profile fails at event 403, Q14/Q15;
  it predicts an extra memory phase clock 51.
- Forwarding new WantRunRfsh onto dMemRun before g04 propagation fails at
  event 331, Q14/Q15. This distinguishes source-Q availability from sink data.

The failed first two-descriptor prototype is retained with its exact compiled
inputs. The lossless archive has 3,033 members, 321 unique blobs and 1,269,036
compressed blob bytes. `summary.json` names every selected terminal run and
hashes its state. Frozen source, reference, timing, testbench and log bytes are
in `manifest.json.gz` and `blobs/`.

Reproduce a complete comparison in a fresh output directory:

```
python3 tools/test_control_hold_ordered.py --output /tmp/dorado-control-order-test \
  --engine verilator --profile later_control
```

Review the preserved working-tree runs and reproduce the component/source audit:

```
python3 tools/review_control_hold_ordered.py --output /tmp/dorado-control-order-review
```

Next: supply this contract from the actual FPGA clock partition, integrate the
full processor and close actual IM/parity, MCR/hold-data, DMD and B-bus sources.
No maintained RTL replacement, native fit/load, physical timing waiver or
hardware speed gain is claimed. Illustrative digital delays do not establish
original analog Set recovery, minimum pulse width or board skew.
