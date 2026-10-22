# Opposite-edge storage address sample: component pass

The rejected native fit captured row/column address into saved_addr at a host rising edge. Native address latches change only on gated host rising edges. Sampling on the intervening host falling edge supplies the same old address at the original acceptance edge while separating the physical hold path by half a cycle. The optional HOST_ADDR_SAMPLE parameter defaults to0; no production group file or live FPGA is changed.

Frozen V2 passes all9,437,184 original144-chip storage bits and byte-identical raw request traces including host timestamps. Enabled and default-disabled modes pass. An independent immediate-accept unit compares the public request/address/data/ready interfaces against the qualified group and rejects the wrong-edge implementation even after removing the diagnostic assertion. Three runtime controls reject wrong-edge, wrong-edge without the assertion, and future-address sampling. Accepted request cycles are unchanged.

The initial V1 coverage failure is retained: the board arbiter masked the wrong-edge defect. V2 deliberately retains that passing board case and adds the independent immediate-accept oracle; it does not mislabel the masked case as a rejected control.

The854-member receipt includes both terminal component attempts, exact inputs/logs/traces, and all503 immutable V4 staged sources. V4 combines only the tested disk-write window and this address sample with the reviewed57MHz parent. Ubuntu builder2338117 at `/home/alans/mister/Dorado_PackWindow57Sample_20261022` was still running at archival time. Detailed all-device timing, RAM inference, half-cycle setup/hold and original firmware review remain required before assembly or deployment. This is no native timing or speed qualification.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/address-sample`

The same V4 run subsequently completed: [terminal timing and initial RAM review](../native57-sample-terminal/README.md). No restart or hardware load occurred.
