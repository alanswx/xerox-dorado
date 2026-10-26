#!/usr/bin/env python3
"""Induct on the actual native operand function behind the prepared88-bit view.

The source-bound epoch theorem permits arbitrary late-input cofactors. Use
one fixed cofactor to evaluate both current and saved descriptors. Prove every
ready844-bit view equals the complete native function, for arbitrary owners,
before pins/root/read word, request, reset and paired/unknown bypass controls.
Wrong-tag/flag cases must expose actual wrong physical outputs.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from integrate_native_operand_buffer import HELPER
from qualify_native_ready_compare import SINGLE
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def instance(name,held,root,pins,data,result):
    return f''' {SINGLE} {name}(.host_clk(1'b0),.advance(1'b0),.held_q({held}),
  .clock_before({root}),.clock_after({root}),.pins_before({pins}),.pins_after({pins}),
  .refresh_before_run_capture(1'b0),.cp_reset(1'b0),.external_file_read({data}),
  .interior_read_data(18'b0),.interior_read_valid(1'b0),.entry_fast(1'b0),
  .entry_prefix(269'b0),.entry_prefix_cp(11'b0),.entry_after_cp(11'b0),.pair_fall(1'b0),
  .physical_state({result}));
'''


TOP='''module operand_buffer_contract(input wire clk,reset,request,use_pair,known,
 input wire [843:0] held,raw_bypass,input wire root,input wire [400:0] pins,
 input wire [17:0] data,output wire safe_view,ready,output wire [843:0] actual,expected);
 wire [843:0] current_function,saved_function;
 wire [843:0] raw=(use_pair || !known) ? raw_bypass:current_function;
 wire valid;wire [1263:0] tag;wire [87:0] fields;
 CURRENT
 SAVED
 dorado_native_operand_buffer buffered(clk,reset,request,use_pair,known,
  held,root,pins,data,raw,ready,actual,valid,tag,fields);
 assign expected=raw;assign safe_view=!ready || actual==expected;
 always @*begin
  if(valid)assert(fields=={saved_function[595:588],saved_function[571:540],saved_function[403:372],saved_function[115:100]});
  assert(safe_view);
  assert(ready==(!reset && (use_pair || !known || (valid && tag==({held,root,pins,data}&SUPPORT)))));
 end
endmodule
'''.replace('CURRENT',instance('current','held','root','pins','data','current_function')).replace('SAVED',instance('saved','tag[1263:420]','tag[419]','tag[418:18]','tag[17:0]','saved_function'))


def run(epoch,out,yosys,candidate_helper=None,support=None):
    assert not out.exists();out.mkdir(parents=True)
    e=json.loads((epoch/'run-state.json').read_text());verify(epoch,e['files'])
    assert e['status']=='PASS_ACTUAL_NATIVE_PRE_EVENT_OPERAND_VIEW'and e['assumptions']==0
    assert e['host_clock_varies_independently']and e['native_constant_inputs']=={'advance':0,'pair_fall':0}
    assert all(a['passed']and a['returncode']==0 for a in e['actions'])
    source=epoch/'pre-event-view';verify(source,e['actions'][0]['files'])
    helper=candidate_helper or ROOT/HELPER
    candidate=helper.read_text();(out/Path(HELPER).name).write_text(candidate)
    masks=re.findall(r"OPERAND_TAG_SUPPORT=1264'h([0-9a-f]+);",candidate)
    assert len(masks)==1
    mask=int(masks[0],16)
    if candidate_helper:
        assert support
        proposed=json.loads((support/'run-state.json').read_text());verify(support,proposed['files'])
        assert proposed['status']=='DERIVED_NATIVE_OPERAND_SUPPORT_REQUIRES_PROOF'and proposed['binary_context_support']
        assert proposed['epoch_state_sha256']==sha(epoch/'run-state.json')and proposed['mask_hex']==masks[0]
        assert mask.bit_count()==proposed['support_bits']
        pattern=r"OPERAND_TAG_SUPPORT=1264'h[0-9a-f]+;"
        assert re.sub(pattern,'MASK',candidate)==re.sub(pattern,'MASK',(ROOT/HELPER).read_text())
    else:assert 0<mask.bit_count()<=1264
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,epoch_proof_state_sha256=sha(epoch/'run-state.json'),
        helper_sha256=sha(helper),helper_source=str(helper),original_helper_sha256=sha(ROOT/HELPER),
        support_state_sha256=sha(support/'run-state.json')if support else None,
        tag_bits=1264,buffered_physical_bits=88,compared_physical_bits=844,assumptions=0,
        epoch_host_clock_independence_proved=True,
        tag_support_bits=mask.bit_count(),tag_support_hex=masks[0],
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,mutation in [('contracts',None),('ignore-held-owner-tag',('saved_tag==current_tag','saved_tag[419:0]==current_tag[419:0]')),
                           ('wrong-branch-flag',('raw_view[595:588],raw_view[571:540]',"(raw_view[595:588]^8'h10),raw_view[571:540]"))]:
        d=out/label;d.mkdir()
        for f in source.glob('*.sv'):
            if f.name!='top.sv':shutil.copy2(f,d/f.name)
        actual=candidate
        if mutation:
            a,b=mutation;assert actual.count(a)==1;actual=actual.replace(a,b,1)
        (d/'buffer.sv').write_text(actual);(d/'top.sv').write_text(TOP.replace('SUPPORT',"1264'h"+masks[0]))
        positive=mutation is None
        sat='sat -timeout 120 -seq 3 -tempinduct -verify -prove-asserts'if positive else'sat -timeout 120 -seq 4 -prove safe_view 1 -falsify -show-ports -dump_json /work/counterexample.json'
        script=f'''read_verilog -formal -sv -defer {' '.join('/work/'+f.name for f in sorted(d.glob('*.sv')))}
hierarchy -check -top operand_buffer_contract
proc
flatten
async2sync
chformal -lower
memory_map
opt
check -assert
{sat}
'''
        (d/'proof.ys').write_text(script);env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            a=dict(label=label,negative=not positive,pid=child.pid,command=cmd,started=time.time(),files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(a);save();code=child.wait()
        marker='Induction step proven: SUCCESS!'if positive else'model found: FAIL!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==3 and all(a['passed']for a in state['actions'])
    state.update(status='PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW'if passed else'TERMINAL_PREPARED_OPERAND_VIEW_NOT_ESTABLISHED',finished=time.time(),
        original60ns_schedule_or_firmware_or_timing_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['epoch','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    for n in ['candidate-helper','support']:p.add_argument('--'+n,type=Path)
    a=p.parse_args();raise SystemExit(run(a.epoch.resolve(),a.output.resolve(),a.yosys.resolve(),a.candidate_helper,a.support))
