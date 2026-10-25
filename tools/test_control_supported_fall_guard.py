#!/usr/bin/env python3
"""Replay the arbitrary-change fall guard witness and all 954 X/Z inputs.

Use four changed-input backgrounds, including changes on every omitted guard
pin. Compare all363 actual outputs with case equality. The negative unknown
fallback must fail actual results, not just the sparse-path debug output.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_control_fall_data_guard_four_state import TB
from test_r_front_forward_view import sha


def run(proof,out,mutation,direct_pins=False):
    assert not out.exists()
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_SOURCE_BOUND_SUPPORTED_FALL_GUARD'
    assert len(p['cases'])==5 and all(r['passed'] for r in p['cases'])
    for row in p['cases']:
        d=proof/(row['mode']+'-w'+str(row['waves']));verify(d,row['files'])
        assert sha(d/'proof.log')==row['log_sha256']
    cover=proof/'cover-data-clock-w5'
    signals={s['name']:s for s in json.loads((cover/'counterexample.json').read_text())['signal']}
    def value(name):
        s=signals[name]
        return int(s['data'][0],2) if 'data' in s else int(s['wave'][0],2)
    assert all(value(n)==1 for n in ['witnessed_data_change','certificate','new_sparse','equivalent'])
    assert value('old_sparse')==0 and value('actual')==value('expected')
    out.mkdir(parents=True)
    source=proof/'positive-w5'
    for name in ['atomic.sv','fall.sv','original.sv','top.sv']:
        shutil.copy2(source/name,out/name)
    if direct_pins:
        top=(out/'top.sv').read_text()
        old_port='input wire [155:0] q,input wire [397:0] before_pins,changes,'
        new_port='input wire [155:0] q,input wire [397:0] before_pins,after_pins,'
        old_wire='wire [397:0] after_pins=before_pins ^ changes;'
        new_wire='wire [397:0] changes=before_pins ^ after_pins;'
        assert top.count(old_port)==top.count(old_wire)==1
        changed=top.replace(old_port,new_port,1).replace(old_wire,new_wire,1)
        assert changed.replace(new_wire,old_wire,1).replace(new_port,old_port,1)==top
        (out/'top.sv').write_text(changed)
    actual=(source/'actual.sv').read_text()
    if mutation:
        assert actual.count('default:using_sparse=0;')==1
        actual=actual.replace('default:using_sparse=0;','default:using_sparse=1;',1)
    (out/'actual.sv').write_text(actual)
    ignored=((1<<398)-1)^int(p['mask_hex'],16)
    witness=(1<<119)|((1<<26) if p.get('phase_audit') else 0)
    tb=TB
    old="case(trial)1:base[10:2]^=9'h1f8;2:base[10:2]=9'h1f8;3:base[10:2]=9'h008;endcase"
    before=value('before_pins') if direct_pins else 0
    new=f"case(trial)1:base[399:2]=398'h{before^ignored:x};2:base[399:2]=398'h{before^witness^(1<<348):x};3:base[399:2]=398'h{before^witness:x};endcase"
    assert tb.count(old)==1
    tb=tb.replace(old,new,1).replace('[564:0]','[953:0]').replace('[8:0] changes','[397:0] changes')
    tb=tb.replace('b<565','b<954').replace('4520','7632').replace('4524','7636')
    tb=tb.replace("156'hQ",f"156'h{value('q'):x}").replace("398'hP",f"398'h{value('before_pins'):x}")
    tb=tb.replace("9'hC",f"398'h{before^value('changes'):x}").replace("1'bR",f"1'b{value('refresh')}")
    tb=tb.replace('CHECK_GUARD',"1'b0" if mutation else "1'b1")
    if direct_pins:
        tb=tb.replace('wire [397:0] changes;','wire [397:0] after_pins;')
        tb=tb.replace('{q,before_pins,changes,refresh,entry}','{q,before_pins,after_pins,refresh,entry}')
        tb=tb.replace('dut(q,before_pins,changes,refresh,entry','dut(q,before_pins,after_pins,refresh,entry')
    (out/'tb.sv').write_text(tb)
    for tool in [Path(__file__),Path(__file__).with_name('test_control_fall_data_guard_four_state.py')]:
        shutil.copy2(tool,out/tool.name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,proof=str(proof),
               proof_state_sha256=sha(proof/'run-state.json'),mutation=mutation,variable_input_bits=954,
               compared_control_bits=363,known_cases=4,XZ_cases=7632,direct_before_after_pins=direct_pins,
               files={p.name:sha(p) for p in out.iterdir()},actions=[],native_integrated=False,hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    commands=[('compile',['iverilog','-g2012','-s','tb_fall_data_four_state','-o',str(out/'test.vvp'),
                         *[str(out/n) for n in ['atomic.sv','fall.sv','original.sv','actual.sv','top.sv','tb.sv']]]),
              ('test',['vvp','-i',str(out/'test.vvp')])]
    for label,cmd in commands:
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:
            break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=state['actions'][0]['returncode']==0 and ((code!=0 and 'FALL_DATA_XZ result mismatch' in log) if mutation else
                                                  (code==0 and 'FALL_DATA_FOUR_STATE_PASS known=4 unknown=7632 checks=7636' in log))
    state.update(status=('PASS_SUPPORTED_FALL_UNKNOWN_COUNTEREXAMPLE' if mutation else 'PASS_SUPPORTED_FALL_FOUR_STATE') if passed else
                 'TERMINAL_SUPPORTED_FALL_FOUR_STATE_NOT_ESTABLISHED',finished=time.time(),
                 test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None)
    save();print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:
        p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--mutation',choices=['bad-unknown'])
    p.add_argument('--direct-pins',action='store_true',help='Inject X/Z independently on actual before/after pins, avoiding XOR encoding of unknowns.')
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.mutation,a.direct_pins))
