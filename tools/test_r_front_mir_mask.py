#!/usr/bin/env python3
"""Experiment with removing task-priority data from the MIR clock cone.

The ordinary clock masks SwitchUp data changes on its rising edge. In LOW
root windows that ordinary clock cannot rise; NoDispatch can asynchronously
set SwitchUp's inverted Q. Keep the original before-clock view and test this
after-clock mask against the complete independent original graph.

This is a hypothesis under test, not a native/timing or general-skew proof.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import json
import os
from pathlib import Path
import sys
import re
import subprocess
import threading
import time
from unittest.mock import patch

import test_r_front_commands as commands
from generate_r_front_commands_reference import ROOT, CANDIDATE


def matrix(a):
    out = a.output.resolve()
    assert not out.exists()
    out.mkdir(parents=True)
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), cases=[])
    lock = threading.Lock()
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    save()
    cases = [(p, e, False) for p in commands.parent.PROFILES for e in ['icarus', 'verilator']]
    cases += [('balanced', e, True) for e in ['icarus', 'verilator']]
    def execute(case):
        profile, engine, frozen = case
        d = out/(profile+'-'+engine+('-frozen' if frozen else '-original'))
        cmd = [sys.executable, str(Path(__file__).resolve()), '--base', str(a.base.resolve()),
               '--output', str(d), '--engine', engine, '--profile', profile, '--late-no-dispatch']
        if frozen:
            cmd.append('--frozen-switch')
        with (out/(d.name+'.log')).open('w') as f:
            process = subprocess.Popen(cmd, cwd=ROOT, stdout=f, stderr=subprocess.STDOUT)
            row = dict(profile=profile, engine=engine, frozen=frozen, directory=d.name,
                       command=cmd, pid=process.pid, started=time.time())
            with lock:
                state['cases'].append(row)
                save()
            rc = process.wait()
        with lock:
            row.update(returncode=rc, finished=time.time())
            save()
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] != 'RUNNING' and len(s['actions']) == 2 and s['actions'][0]['returncode'] == 0, d
        for n, digest in s['inputs'].items():
            assert hashlib.sha256((d/'inputs'/n).read_bytes()).hexdigest() == digest, (d, n)
        log = (d/'test.log').read_text()
        if frozen:
            assert rc != 0 and s['status'] == 'R_FRONT_LOCAL_PROBE_FAILED', d
            assert 'R_FRONT_DIFF bit=772' in log and 'R front state mismatch check=6279' in log, d
            row['qualification'] = 'REJECTED_FROZEN_SWITCH_MIR_CLOCK'
        else:
            assert rc == 0 and s['status'] == 'PASS_R_FRONT_LOCAL_PROBE', d
            assert s['summary'] == 'checks=13515 writes=328 reads=5608 epochs=797', d
            assert 'MIR_MASK_LATE_NODISPATCH root=0 MIRcaptures=1 SwitchUp_n=1' in log, d
            assert 'R_FRONT_COMMANDS_DIRECTED commands=8 FF=12 JCN=4 intervals=120 WriteIM=16' in log, d
            clocks = re.findall(r'^R_FRONT_LOCAL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
            assert len(clocks) == 68 and {int(c) for c, n in clocks} == set(range(68)), d
            assert all(int(n) > 0 for c, n in clocks), d
            assert s['executed_candidate_sha256'] == 'e695318064d80a9e308ff5e76e3fd57d86bb55412a448b163da135856883dd0e', d
            row.update(qualification='PASS_BOUNDED_MIR_MASK_ORDERING', summary=s['summary'], clocks=clocks)
        row['run_state_sha256'] = hashlib.sha256((d/'run-state.json').read_bytes()).hexdigest()
        with lock:
            save()
        print(row['qualification'], d.name, flush=True)
    failures = []
    with ThreadPoolExecutor(max_workers=3) as pool:
        futures = {pool.submit(execute, c): c for c in cases}
        for f in as_completed(futures):
            try:
                f.result()
            except Exception as error:
                failures.append(dict(case=futures[f], error=repr(error)))
                print('QUALIFICATION_FAILURE', futures[f], repr(error), flush=True)
    primary = [r for r in state['cases'] if r.get('qualification') == 'PASS_BOUNDED_MIR_MASK_ORDERING']
    for profile in commands.parent.PROFILES:
        pair = [r for r in primary if r['profile'] == profile]
        if len(pair) != 2 or pair[0]['clocks'] != pair[1]['clocks']:
            failures.append(dict(profile=profile, error='Engine counts disagree or a result is missing'))
    state.update(status='MIR_MASK_ORDERING_FAILED' if failures else 'PASS_BOUNDED_R_FRONT_MIR_MASK_ORDERING',
                 finished=time.time(), failures=failures, primary=primary,
                 negative_cases=[r for r in state['cases'] if r.get('qualification') == 'REJECTED_FROZEN_SWITCH_MIR_CLOCK'],
                 physical_bits=792, ram_bits=9216, clocks=68,
                 full_arrival_contract_qualified=False, physical_timing_qualified=False,
                 native_integrated=False, speedup_qualified=False)
    save()
    (out/'results.json').write_text(json.dumps(state, indent=2)+'\n')
    print(state['status'], len(primary), len(state['negative_cases']), flush=True)
    return 1 if failures else 0


def run(a):
    original = (ROOT/CANDIDATE).read_text()
    raw = original
    edits = [
        ('clock_levels(input [103:0] q,input base,input [129:0] p)',
         'clock_levels(input [103:0] q,input base,input [129:0] p,input mir_switch_q)'),
        ('mir_clock=normal0 | repeat_cur | p[125]',
         'mir_clock=normal0 | (p[69]&mir_switch_q) | p[125]'),
        ('previous=clock_levels(q,clock_before,pins_before)',
         'previous=clock_levels(q,clock_before,pins_before,q[43])'),
        ('levels=clock_levels(q,clock_after,pins_after)',
         'levels=clock_levels(q,clock_after,pins_after,mir_switch_view)'),
        ('reg [2:0] old_memory;', 'reg [2:0] old_memory;reg mir_switch_view,mir_switch_before;'),
        ('q=state_q;n=q;previous=clock_levels', 'mir_switch_view=state_q[43];q=state_q;n=q;previous=clock_levels'),
        ('settled=(n==q && levels==previous);previous=levels;q=n;',
         'mir_switch_before=mir_switch_view;'+('' if a.frozen_switch else 'if(q[69])mir_switch_view=1;')+
         'settled=(n==q && levels==previous && mir_switch_view==mir_switch_before);previous=levels;q=n;'),
    ]
    for before, after in edits:
        assert raw.count(before) == 1, before
        raw = raw.replace(before, after)
    inverse = raw
    for before, after in reversed(edits):
        inverse = inverse.replace(after, before)
    assert inverse == original
    path = a.output.resolve().parent/(a.output.name+'-mir-mask.sv')
    assert not path.exists()
    path.write_text(raw)
    a.mutation = None
    a.directed = True
    original_bench = commands.parent.bench
    def bench(*args, **kwargs):
        text = original_bench(*args, **kwargs)
        if a.late_no_dispatch:
            marker = ' reg [314:0] p;'
            assert text.count(marker) == 1
            text = text.replace(marker, ' integer late_mir_before;\n'+marker)
            marker = '   // IM data settles'
            assert text.count(marker) == 1
            directed = r'''
   // A pending higher task stores SwitchUp's inverted Q LOW. Hold settles
   // during HIGH; then NoDispatch asynchronously sets that Q in a LOW-root
   // window, creating the real MIR capture clock while normal Clock0 is LOW.
   if(!cb)event_step(1,p);
   p=SEED;p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[204:202]=3'b110;p[261:254]=8'h01;event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer n=0;n<32;n++)event_step(!cb,p);
   event_step(1,p);p[20]=1;event_step(1,p);found=0;
   for(integer n=0;n<96&&!found;n++)begin
     event_step(!cb,p);if(cb&&!q[75])found=1;
   end
   if(!found)$fatal(1,"MIR mask setup did not create higher-task SwitchUp");
   p[69]=1;event_step(1,p);found=0;
   for(integer n=0;n<96&&!found;n++)begin
     event_step(!cb,p);if(!cb&&!q[75]&&!clocks[67]&&!clocks[59])found=1;
   end
   if(!found)$fatal(1,"MIR mask setup missed the LOW clock window");
   p[261:254]=8'he6;event_step(0,p);late_mir_before=counts[59];
   p[292]=1;p[291:289]=7;p[288:282]=8;event_step(0,p);
   p[292]=0;event_step(0,p);p[292]=1;event_step(0,p);
   if(!q[75]||counts[59]-late_mir_before!=1||q[773]||!q[772]||!q[779]||q[778]||q[775]||!q[774]||!q[783]||!q[782])
     $fatal(1,"MIR mask lost NoDispatch-triggered actual MIR capture");
   $display("MIR_MASK_LATE_NODISPATCH root=%b MIRcaptures=%0d SwitchUp_n=%b",cb,counts[59]-late_mir_before,q[75]);
   event_step(1,p);p[20]=0;p[69]=0;event_step(1,p);cp_write(0,0);
   p=SEED;event_step(1,p);
'''
            text = text.replace(marker, directed+'\n'+marker)
        return text
    with patch.object(commands, 'CANDIDATE', str(path.relative_to(ROOT))), patch.object(commands.parent, 'bench', bench):
        rc = commands.run(a)
    p = a.output.resolve()/'run-state.json'
    record = json.loads(p.read_text())
    record.update(experiment='MIR_clock_mask_without_task_priority_data',
                  parent_candidate_sha256=hashlib.sha256(original.encode()).hexdigest(),
                  source_edits=edits, inverse_byte_identical=True,
                  late_no_dispatch=a.late_no_dispatch, frozen_switch_mutation=a.frozen_switch,
                  full_arrival_contract_qualified=False, physical_timing_qualified=False,
                  speedup_qualified=False)
    p.write_text(json.dumps(record, indent=2)+'\n')
    return rc


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], default='verilator')
    p.add_argument('--profile', choices=commands.parent.PROFILES, default='balanced')
    p.add_argument('--iterations', type=int, default=1024)
    p.add_argument('--late-no-dispatch', action='store_true')
    p.add_argument('--frozen-switch', action='store_true', help='Diagnostic: omit asynchronous NoDispatch effect on the MIR clock')
    p.add_argument('--matrix', action='store_true')
    a = p.parse_args()
    sys.exit(matrix(a) if a.matrix else run(a))
