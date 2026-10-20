#!/usr/bin/env python3
"""Add original transparent latch/SRAM models to an explicit-arrival graph.

All delay profiles remain illustrative digital-order experiments. They do not
characterize ECL setup/hold, SRAM pulse requirements or native 60ns operation.
"""
import argparse
import hashlib
import json
from pathlib import Path

from stage_task_capture_timing import stage as stage_tasks


def stage(inputs, reference, out, gate_ps, ff_ps, phase_ps):
    record = stage_tasks(inputs, reference, out, gate_ps, ff_ps, phase_ps)
    name = 't_file_original_storage.sv'
    raw = (inputs / 'verilog/verilator' / name).read_text()
    assert raw.count('parameter integer DELAY_PS=1000') == 1
    raw = raw.replace('parameter integer DELAY_PS=1000', 'parameter integer DELAY_PS=' + str(ff_ps))
    (out / name).write_text(raw)
    record.update(storage_scope=__doc__, latch_delay_ps=ff_ps,
                  ram_semantics='Level sensitive while CE and WE are low; no fabric clock')
    record['files'][name] = hashlib.sha256((out / name).read_bytes()).hexdigest()
    (out / 'timing-profile.json').write_text(json.dumps(record, indent=2) + '\n')
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inputs', type=Path, required=True)
    parser.add_argument('--reference', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--gate-ps', type=int, default=1000)
    parser.add_argument('--ff-ps', type=int, default=1000)
    parser.add_argument('--phase-ps', type=int, default=1000)
    args = parser.parse_args()
    stage(args.inputs.resolve(), args.reference.resolve(), args.output.resolve(),
          args.gate_ps, args.ff_ps, args.phase_ps)
