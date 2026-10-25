#!/usr/bin/env python3
"""Qualify the85-bit falling guard on the ACTUAL native shared-control source.

Compare all363 state/event/flag outputs AND432 shared capture-frame bits.
Derive the HIGH identity certificate from that same native transfer; do not
assume arbitrary state is canonical. Preserve source and failed proofs. This
is a native-source functional gate, not FPGA timing/firmware/speed selection.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
CONTROL='src/verilog/rtl/dorado_r_front_control_step.sv'
NAMES=['dorado_capture_certified_fall_step','dorado_capture_hold_atomic_step',
       'dorado_capture_terminal_body','dorado_control_fall_step']
MASK=int('3e001ffffc0f1000002000001fffffffffffc0000000000000000000000000000000000000000007800000000000000078',16)
OLD_MASK=int('3e001ffffc0f1060002000001fffffffffffc00000000000000000000000000000000000000000078000000007fff00078',16)
TOP='''module native_fall_guard_contract #(parameter integer WAVES=DEPTH)(
 input wire [155:0] q,input wire cb,ca,input wire [397:0] pb,pa,
 input wire refresh,entry,output wire equivalent,certificate,old_sparse,new_sparse,
 output wire [794:0] actual,expected,output wire witnessed_change);
 wire [155:0] canonical;wire [55:0] canonical_events,canonical_repeated;wire canonical_unsettled;
 dorado_capture_hold_atomic_step #(.WAVES(WAVES)) identity(q,1'b1,1'b1,pb,pb,
  canonical,canonical_events,canonical_repeated,canonical_unsettled,refresh,,);
 assign certificate=(canonical==q) && !(|canonical_events) && !(|canonical_repeated) && !canonical_unsettled;
 wire accepted_entry=entry && certificate;
 dorado_capture_certified_fall_step #(.WAVES(WAVES)) original(q,cb,ca,pb,pa,
  expected[362:207],expected[206:151],expected[150:95],expected[94],refresh,expected[93:0],
  accepted_entry,old_sparse,expected[794:363]);
 native_phase_fall_guard #(.WAVES(WAVES)) changed(q,cb,ca,pb,pa,
  actual[362:207],actual[206:151],actual[150:95],actual[94],refresh,actual[93:0],
  CANDIDATE_ENTRY,new_sparse,actual[794:363]);
 assign equivalent=actual==expected;
 assign witnessed_change=accepted_entry && cb && !ca && (pb[333]!=pa[333]) && (pb[334]!=pa[334]) &&
  new_sparse && !old_sparse && (|actual[206:151]) && equivalent;
endmodule
'''
HIGH='''module native_high_closure(
 input wire [155:0] q,input wire cb,input wire [397:0] pb,pa,input wire refresh,
 output wire identity_preserved,output wire [155:0] next_q,checked_q,
 output wire [55:0] events,repeated,output wire unsettled);
 wire [55:0] first_events,first_repeated;wire first_unsettled;
 dorado_capture_hold_atomic_step #(.WAVES(5)) produce(q,cb,1'b1,pb,pa,
  next_q,first_events,first_repeated,first_unsettled,refresh,,);
 dorado_capture_hold_atomic_step #(.WAVES(5)) check_high(next_q,1'b1,1'b1,pa,pa,
  checked_q,events,repeated,unsettled,refresh,,);
 assign identity_preserved=first_unsettled ||
   ((checked_q==next_q) && !(|events) && !(|repeated) && !unsettled);
endmodule
'''


def transform(raw,mask=MASK,sparse_frames=False):
    original=module(raw,NAMES[0])
    token=f"FALL_STABLE_PIN_SUPPORT=398'h{OLD_MASK:x};"
    assert original.count(token)==1
    replacement=f"FALL_STABLE_PIN_SUPPORT=(WAVES==5 || WAVES==24) ? 398'h{mask:x} : 398'h{OLD_MASK:x};"
    actual=original.replace(token,replacement,1)
    assert actual.replace(replacement,token,1)==original
    if sparse_frames:
        marker=' wire [362:0] full_result,fall_result;'
        extra=''' wire [431:0] generic_frames;
 wire [47:0] fall_before=state_q[47:0],fall_after=fall_result[254:207];
 wire [11:0] fall_captures={3'b0,fall_result[159],8'b0};
 wire [431:0] sparse_frames={{3{fall_after,fall_after,12'b0}},fall_after,fall_before,fall_captures};
 assign capture_frames=using_sparse ? sparse_frames:generic_frames;
'''
        assert actual.count(marker)==1;actual=actual.replace(marker,marker+'\n'+extra,1)
        old='refresh_before_run_capture,full_result[93:0],capture_frames);'
        assert actual.count(old)==1;actual=actual.replace(old,'refresh_before_run_capture,full_result[93:0],generic_frames);',1)
    return raw.replace(original,actual,1)


def run(stage,out,yosys,sparse_frames=False):
    assert not out.exists();out.mkdir(parents=True)
    manifest=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,manifest['source_hashes'])
    raw=(stage/CONTROL).read_text();candidate=transform(raw,sparse_frames=sparse_frames)
    source='\n'.join(module(raw,n)for n in NAMES)
    actual=module(candidate,NAMES[0]).replace('module '+NAMES[0]+' #','module native_phase_fall_guard #',1)
    (out/'original.sv').write_text(source);(out/'actual.sv').write_text(actual)
    (out/'native-control-before.sv').write_text(raw);(out/'native-control-after.sv').write_text(candidate)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        source_control_sha256=sha(stage/CONTROL),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        old_guard_bits=102,new_guard_bits=85,removed_input_bits=[i for i in range(398)if OLD_MASK>>i&1 and not MASK>>i&1],
        symbolic_input_bits=956,compared_result_bits=795,shared_capture_frame_bits=432,
        HIGH_certificate_derived_from_actual_native_source=True,assumptions=0,
        sparse_shared_capture_frames=sparse_frames,
        actions=[],files={p.name:sha(p)for p in out.iterdir()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for mode,waves in [('positive',5),('positive',24),('wrong-selector',5),('without-certificate',5),('cover-clock-and-data',5),('post-HIGH-identity',5)]:
        d=out/f'{mode}-w{waves}';d.mkdir()
        (d/'original.sv').write_text(source)
        mask=MASK&~(1<<345)if mode=='wrong-selector'else MASK
        modified=module(transform(raw,mask,sparse_frames),NAMES[0]).replace('module '+NAMES[0]+' #','module native_phase_fall_guard #',1)
        (d/'actual.sv').write_text(modified)
        high=mode=='post-HIGH-identity'
        (d/'top.sv').write_text(HIGH if high else TOP.replace('DEPTH',str(waves)).replace('CANDIDATE_ENTRY','entry'if mode=='without-certificate'else'accepted_entry'))
        positive=mode=='positive'or high;cover=mode=='cover-clock-and-data'
        signal='identity_preserved'if high else('witnessed_change'if cover else'equivalent');value=0 if cover else 1
        script=f'''read_verilog -sv -defer /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top {'native_high_closure'if high else'native_fall_guard_contract'}
proc
flatten
opt
check -assert
sat -timeout 120 -prove {signal} {value} -{'verify'if positive else'falsify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            row=dict(label=f'{mode}-w{waves}',mode=mode,waves=waves,pid=child.pid,started=time.time(),command=cmd,
                     files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(row);save();code=child.wait()
        marker='no model found: SUCCESS!'if positive else'model found: FAIL!'
        passed=code==0 and marker in(d/'proof.log').read_text()
        row.update(returncode=code,passed=passed,log_sha256=sha(d/'proof.log'),finished=time.time());save()
        if not passed:break
    passed=len(state['actions'])==6 and all(r['passed']for r in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_PHASE_FALL_GUARD'if passed else'TERMINAL_NATIVE_PHASE_FALL_GUARD_NOT_ESTABLISHED',
        finished=time.time(),native_selected=False,timing_or_speed_qualified=False)
    save();print(state['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--sparse-frames',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve(),a.sparse_frames))
