#!/usr/bin/env python3
"""Measure the 404-bit T-data/task-file evaluator using the qualified 15ns shell.

This is an isolated implementation-budget measurement, without assembly or
deployment. It does not establish native Dorado timing or instruction speed.
Run Quartus only on misterubuntu.
"""
import argparse
import hashlib
import json
from pathlib import Path

import fit_control_clock as fixture

ROOT = Path(__file__).resolve().parents[1]


def prepare(out):
    out.mkdir(parents=True, exist_ok=False)
    top = fixture.TOP
    for before, after, count in [('[51:0]', '[659:0]', 3), ('[50:0]', '[462:0]', 1),
                                 ('[31:0]', '[403:0]', 1),
                                 ('wire [8:0] before_controls,after_controls,events,repeated;',
                                  'wire [126:0] before_controls,after_controls;\n wire [24:0] events,repeated;\n wire [1:0] write_n,latch_n,t_latch_n,bypass;', 1),
                                 ('dorado_control_clock_step', 'dorado_t_file_step', 1)]:
        assert top.count(before) == count, before
        top = top.replace(before, after)
    top = top.replace('next_q,events,repeated,unsettled);', 'next_q,events,repeated,unsettled,write_n,latch_n,t_latch_n,bypass);')
    top = top.replace('result<={next_q,events,repeated,unsettled};', 'result<={next_q,events,repeated,unsettled,write_n,latch_n,t_latch_n,bypass};')
    qsf = fixture.QSF.replace('dorado_control_clock_step.sv', 'dorado_t_file_step.sv')
    qsf += 'set_global_assignment -name SYSTEMVERILOG_FILE dorado_task_capture_step.sv\n'
    files = {'top.sv': top, 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
             'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n', 'audit.tcl': fixture.AUDIT}
    for name, raw in files.items():
        (out / name).write_text(raw)
    for name in ['tools/fit_control_clock.py', 'tools/fit_t_file.py',
                 'verilog/rtl/dorado_t_file_step.sv','verilog/rtl/dorado_task_capture_step.sv']:
        (out / Path(name).name).write_bytes((ROOT / name).read_bytes())
    record = dict(scope=__doc__, component='t_file', clock_period_ns=15,
                  input_staging_bits=660, launch_bits=660, result_bits=463,
                  files={p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                         for p in out.iterdir() if p.is_file()},
                  assembly_permitted=False, native_qualified=False)
    (out / 'provenance.json').write_text(json.dumps(record, indent=2) + '\n')
    print('Prepared 404-bit evaluator: 15ns, 660 launch bits, 463 result bits; no assembly/deploy')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument('--prepare', type=Path)
    action.add_argument('--run', type=Path)
    parser.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args = parser.parse_args()
    if args.prepare:
        prepare(args.prepare.resolve())
    else:
        fixture.run(args.run.resolve(), args.quartus.resolve())
