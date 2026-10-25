#!/usr/bin/env python3
"""Prove data-only DMD06..11 need not serialize the certified falling guard.

Compare every363 actual control result bits, using the original function's
independent HIGH-identity certificate. Vary all six data bits and three adjacent
selector bits; only the six data bits may be omitted from the stable-pin guard.
Other differing inputs reject both guards by source identity. Unknown input
fallback and non-falling paths remain byte-identical. No native selection.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

MASK=((1<<398)-1)^(((1<<6)-1)<<348)


def candidate(raw,bad_guard=False):
    old=module(raw,'dorado_control_certified_fall_step');new=old
    mask=MASK ^ ((1<<345) if bad_guard else 0)
    marker=' wire [362:0] full_result,fall_result;'
    extra=f" localparam [397:0] FALL_STABLE_PIN_SUPPORT=398'h{mask:x};\n"
    assert old.count(marker)==1;new=new.replace(marker,extra+marker,1)
    expr='pins_before==pins_after'
    replacement='((pins_before & FALL_STABLE_PIN_SUPPORT)==(pins_after & FALL_STABLE_PIN_SUPPORT))'
    assert new.count(expr)==1;new=new.replace(expr,replacement,1)
    assert new.replace(replacement,expr,1).replace(extra,'',1)==old
    return new


TOP='''module fall_guard_equivalence #(parameter integer WAVES=WAVES_VALUE)(
 input wire [155:0] q,input wire [397:0] before_pins,input wire [8:0] changes,
 input wire refresh,entry,output wire equivalent,certificate,old_sparse,new_sparse,
 output wire [362:0] actual,expected,output wire witnessed_data_change);
 wire [397:0] after_pins=before_pins ^ (398'(changes)<<345);
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
 // Reach a genuinely different data input with the original Q30 clock event,
 // while selecting the new fast branch and the old generic branch.
 assign witnessed_data_change=certified_entry && changes[8:3]!=0 && changes[2:0]==0 &&
  new_sparse && !old_sparse && actual[159];
endmodule
'''


def run(source,native,out,yosys):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['specialized'] and s['mutation'] is None
    for n,h in s['files'].items():assert sha(source/n)==h
    native_meta=native/'r-front-control-native.json';n=json.loads(native_meta.read_text())
    board='src/fpga/mister/snapshot_native/ContB_full_snapshot.sv'
    assert sha(native/board)==n['source_hashes'][board]
    text=(native/board).read_text()
    for pin,bit in [(345,3),(346,4),(347,5),*[(348+i,6+i) for i in range(6)]]:
        assert f'assign r_front_pins[{pin}]=DMD_{bit:02d};' in text
    raw=(source/'candidate.sv').read_text();atomic,fall=sources(raw)
    original=module(raw,'dorado_control_certified_fall_step')
    actual=candidate(raw).replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
    out.mkdir(parents=True)
    for name,v in {'atomic.sv':atomic,'fall.sv':fall,'original.sv':original,'actual.sv':actual,'ContB_full_snapshot.sv':text}.items():(out/name).write_text(v)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
        native_manifest_sha256=sha(native_meta),compared_bits=363,state_bits=156,clock_channels=56,flag_bits=94,
        omitted_guard_inputs=list(range(348,354)),omitted_native_signals=['DMD_'+str(i).zfill(2) for i in range(6,12)],
        adjacent_selector_inputs_retained=[345,346,347],certificate='Independent actual HIGH->HIGH identity including all owners/events/repeated/unsettled',
        other_input_differences_still_reject_guard=True,unknown_and_nonfalling_fallback_source_unchanged=True,
        files={p.name:sha(p) for p in out.iterdir()},cases=[],native_integrated=False,timing_qualified=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode,waves in [('positive',5),('positive',24),('wrong-selector',5),('without-certificate',5),('cover-data-clock',5)]:
        d=out/(mode+'-w'+str(waves));d.mkdir()
        for name in ['atomic.sv','fall.sv','original.sv']:shutil.copy2(out/name,d/name)
        new=candidate(raw,mode=='wrong-selector').replace('module dorado_control_certified_fall_step #','module dorado_control_data_guard_step #',1)
        top=TOP.replace('WAVES_VALUE',str(waves)).replace('CANDIDATE_ENTRY','entry' if mode=='without-certificate' else 'certified_entry')
        (d/'actual.sv').write_text(new);(d/'top.sv').write_text(top)
        positive=mode=='positive';flag='verify' if positive else 'falsify'
        signal='witnessed_data_change' if mode=='cover-data-clock' else 'equivalent';value=0 if mode=='cover-data-clock' else 1
        (d/'proof.ys').write_text('read_verilog -sv -defer '+' '.join('/work/'+n for n in ['atomic.sv','fall.sv','original.sv','actual.sv','top.sv'])+f'''
hierarchy -check -top fall_guard_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove {signal} {value} -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys'];row=dict(mode=mode,waves=waves,started=time.time(),command=cmd,files={p.name:sha(p) for p in d.iterdir()})
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        marker='no model found: SUCCESS!' if positive else 'model found: FAIL!'
        row.update(returncode=code,passed=code==0 and marker in (d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not row['passed']:break
    passed=len(state['cases'])==5 and all(r['passed'] for r in state['cases'])
    state.update(status='PASS_SOURCE_BOUND_FALL_DATA_GUARD' if passed else 'TERMINAL_FALL_DATA_GUARD_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','native','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.native.resolve(),a.output.resolve(),a.yosys.resolve()))
