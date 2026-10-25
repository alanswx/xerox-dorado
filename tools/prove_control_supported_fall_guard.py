#!/usr/bin/env python3
"""Prove a source-supported fall guard with arbitrary398-bit input changes.

Derive the candidate from whole-control or independently proved phase support,
minus the six proved fall-only data inputs. Compare all363 results against the original
398-pin guard. Derive the HIGH certificate from the actual independent
identity function; never assume arbitrary q is canonical. Retain unknown and
non-falling fallbacks byte for byte. No native promotion or timing claim.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from audit_control_pin_support import support
from prove_control_fall_data_guard import candidate, MASK
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

TOP = '''module fall_guard_equivalence #(parameter integer WAVES=WAVES_VALUE)(
 input wire [155:0] q,input wire [397:0] before_pins,changes,
 input wire refresh,entry,output wire equivalent,certificate,old_sparse,new_sparse,
 output wire [362:0] actual,expected,output wire witnessed_data_change);
 wire [397:0] after_pins=before_pins ^ changes;
 wire [155:0] high_q;wire [55:0] high_events,high_repeated;wire high_unsettled;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) identity(q,1'b1,1'b1,before_pins,before_pins,
  high_q,high_events,high_repeated,high_unsettled,refresh,);
 assign certificate=(high_q==q) && !(|high_events) && !(|high_repeated) && !high_unsettled;
 wire certified_entry=entry && certificate;
 dorado_control_certified_fall_step #(.WAVES(WAVES)) original(q,1'b1,1'b0,before_pins,after_pins,
  expected[362:207],expected[206:151],expected[150:95],expected[94],refresh,expected[93:0],certified_entry,old_sparse);
 dorado_control_data_guard_step #(.WAVES(WAVES)) changed(q,1'b1,1'b0,before_pins,after_pins,
  actual[362:207],actual[206:151],actual[150:95],actual[94],refresh,actual[93:0],CANDIDATE_ENTRY,new_sparse);
 assign equivalent=actual==expected;
 // Critical IfuData input119 may change while the real Q30 event fires.
 assign witnessed_data_change=certified_entry && changes[119] && new_sparse && !old_sparse && actual[159];
endmodule
'''


def read(p):
    return json.loads(p.read_text())


def run(source,audit,review,out,yosys,phase_audit=None):
    assert not out.exists()
    s=read(source/'run-state.json');verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['specialized'] and s['mutation'] is None
    reviewed=read(review/'results.json')
    assert reviewed['status']=='REVIEWED_CONTROL_GUARD_PACKET_AND_SUPPORT'
    a=read(audit/'run-state.json');verify(audit,a['files'])
    assert a['status']=='PASS_WHOLE_CONTROL_PIN_SUPPORT' and sha(audit/'cone.json')==a['cone_json_sha256']
    for row in a['actions']:
        assert row['returncode']==0 and sha(audit/(row['label']+'.log'))==row['log_sha256']
        assert sha(audit/(row['label']+'.ys'))==row['script_sha256']
    ports=support(read(audit/'cone.json')['modules']['control_binary_cone'])
    pins=sorted(set(ports['before_pins'])|set(ports['after_pins']))
    assert pins==a['guard_pin_support'] and len(pins)==108
    mask=sum(1<<i for i in pins)&MASK
    assert mask.bit_count()==102 and all((mask>>i)&1 for i in [292,345,346,347]) and not (mask>>119)&1
    raw=(source/'candidate.sv').read_text();atomic,fall=sources(raw)
    assert (audit/'atomic.sv').read_text()==atomic
    if phase_audit is not None:
        phase=read(phase_audit/'run-state.json');verify(phase_audit,phase['files'])
        assert phase['status']=='PASS_CONTROL_FALL_PHASE_SUPPORT' and phase['compared_phase_bits']==57
        assert (phase_audit/'atomic.sv').read_text()==atomic and len(phase['actions'])==4
        for row in phase['actions']:
            assert row['returncode']==0 and sha(phase_audit/(row['label']+'.log'))==row['log_sha256']
            assert sha(phase_audit/(row['label']+'.ys'))==row['script_sha256']
        for name in ['positive-w5','positive-w24']:
            assert 'no model found: SUCCESS!' in (phase_audit/(name+'.log')).read_text()
        mask=int(phase['support_mask_hex'],16)&MASK
        assert mask.bit_count()<102 and not (mask>>26)&1
    original=module(raw,'dorado_control_certified_fall_step')
    actual=candidate(raw,mask=mask).replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
    out.mkdir(parents=True)
    for name,value in {'atomic.sv':atomic,'fall.sv':fall,'original.sv':original,'actual.sv':actual}.items():
        (out/name).write_text(value)
    for tool in [Path(__file__),Path(__file__).with_name('prove_control_fall_data_guard.py')]:
        shutil.copy2(tool,out/tool.name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,
               source=str(source),source_state_sha256=sha(source/'run-state.json'),audit=str(audit),
               audit_state_sha256=sha(audit/'run-state.json'),review=str(review),review_sha256=sha(review/'results.json'),
               compared_bits=363,state_bits=156,clock_channels=56,flag_bits=94,
               phase_audit=str(phase_audit) if phase_audit else None,
               phase_audit_state_sha256=sha(phase_audit/'run-state.json') if phase_audit else None,
               mask_hex=format(mask,'x'),guard_pin_count=mask.bit_count(),omitted_guard_inputs=[i for i in range(398) if not (mask>>i)&1],
               arbitrary_change_bits=398,arbitrary_binary_miter_input_bits=954,
               actual_HIGH_identity_certificate_required=True,unknown_and_nonfalling_fallback_source_unchanged=True,
               files={p.name:sha(p) for p in out.iterdir()},cases=[],native_integrated=False,timing_qualified=False,hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode,waves in [('positive',5),('positive',24),('wrong-selector',5),('without-certificate',5),('cover-data-clock',5)]:
        d=out/(mode+'-w'+str(waves));d.mkdir()
        for name in ['atomic.sv','fall.sv','original.sv']:
            shutil.copy2(out/name,d/name)
        new=candidate(raw,mode=='wrong-selector',mask).replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
        top=TOP.replace('WAVES_VALUE',str(waves)).replace('CANDIDATE_ENTRY','entry' if mode=='without-certificate' else 'certified_entry')
        if phase_audit is not None:
            top=top.replace('changes[119] &&','changes[119] && changes[26] &&',1)
        (d/'actual.sv').write_text(new);(d/'top.sv').write_text(top)
        positive=mode=='positive';flag='verify' if positive else 'falsify'
        signal='witnessed_data_change' if mode=='cover-data-clock' else 'equivalent'
        value=0 if mode=='cover-data-clock' else 1
        (d/'proof.ys').write_text('read_verilog -sv -defer '+' '.join('/work/'+n for n in ['atomic.sv','fall.sv','original.sv','actual.sv','top.sv'])+f'''
hierarchy -check -top fall_guard_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove {signal} {value} -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        row=dict(mode=mode,waves=waves,started=time.time(),command=cmd,files={p.name:sha(p) for p in d.iterdir()})
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env)
            row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        marker='no model found: SUCCESS!' if positive else 'model found: FAIL!'
        row.update(returncode=code,passed=code==0 and marker in (d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not row['passed']:
            break
    passed=len(state['cases'])==5 and all(r['passed'] for r in state['cases'])
    state.update(status='PASS_SOURCE_BOUND_SUPPORTED_FALL_GUARD' if passed else 'TERMINAL_SUPPORTED_FALL_GUARD_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','audit','review','output','yosys']:
        p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--phase-audit',type=Path,help='Require the proved actual HIGH-certificate/initial-clock support before testing its narrower guard.')
    a=p.parse_args()
    raise SystemExit(run(a.source.resolve(),a.audit.resolve(),a.review.resolve(),a.output.resolve(),a.yosys.resolve(),a.phase_audit.resolve() if a.phase_audit else None))
