#!/usr/bin/env python3
"""Compare closed/open l07 storage against independent original MC1668 latches.

All per-bit old/next clock/data/Set/held combinations occur; late open data,
closing data changes and closed-Set persistence are checked. Reset is tied LOW
as on the original boards, avoiding unspecified simultaneous Set/Reset.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
FILES = ['verilog/rtl/dorado_r_flag_window_step.sv', 'verilog/verilator/tb_r_flag_window.sv',
         'verilog/verilator/r_operand_original_ff.sv']


def run(out, engine, mutation):
    out.mkdir(parents=True, exist_ok=False)
    records, sources = [], []
    for name in FILES:
        p = out / Path(name).name
        raw = (ROOT / name).read_text()
        original = raw
        if name.endswith('dorado_r_flag_window_step.sv'):
            if mutation == 'omit_close':
                raw = raw.replace('held_q[bit_no] : physical_q[bit_no]', 'held_q[bit_no] : held_q[bit_no]')
            elif mutation == 'ignore_set':
                raw = raw.replace("(set_before[bit_no] ? 1'b1 : held_q[bit_no])", 'held_q[bit_no]')
            elif mutation == 'set_over_clock':
                raw = raw.replace('clock_before[bit_no] ? data_before[bit_no] :', "set_before[bit_no] ? 1'b1 : clock_before[bit_no] ? data_before[bit_no] :")
            elif mutation == 'omit_new_set':
                raw = raw.replace('!clock_after[bit_no] && set_after[bit_no]', "1'b0")
            assert (raw != original) == bool(mutation)
        p.write_text(raw)
        records.append(dict(path=p.name, original_sha256=hashlib.sha256(original.encode()).hexdigest(),
                            output_sha256=hashlib.sha256(raw.encode()).hexdigest()))
        sources.append(str(p))
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', pid=os.getpid(), engine=engine, mutation=mutation, started=time.time(), inputs=records, actions=[],
                 native_qualified=False, source_pause_contract_qualified=False, assembly_permitted=False)
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    commands = [['iverilog', '-g2012', '-s', 'tb_r_flag_window', '-o', str(out / 'test.vvp'), *sources],
                ['vvp', '-i', str(out / 'test.vvp')]] if engine == 'icarus' else [
        ['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_r_flag_window',
         '--Mdir', str(out / 'obj'), '-o', 'test', *sources], [str(out / 'obj/test')]]
    save()
    for i, cmd in enumerate(commands):
        with (out / ('compile.log' if i == 0 else 'test.log')).open('w') as log:
            p = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
            row = dict(command=cmd, pid=p.pid, started=time.time())
            state['actions'].append(row); save()
            rc = p.wait(); row.update(returncode=rc, finished=time.time()); save()
        if rc:
            break
    text = (out / 'test.log').read_text() if (out / 'test.log').exists() else ''
    passed = not rc and 'R_FLAG_WINDOW PASS checks=327680 transitions=65536 per_bit_states=128' in text
    state.update(status='PASS_R_FLAG_WINDOW' if passed else 'R_FLAG_WINDOW_FAILED', finished=time.time()); save()
    print(state['status'], flush=True)
    return rc if rc else (0 if passed else 1)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], required=True)
    p.add_argument('--mutation', choices=['omit_close', 'ignore_set', 'set_over_clock', 'omit_new_set'])
    a = p.parse_args()
    raise SystemExit(run(a.output.resolve(), a.engine, a.mutation))
