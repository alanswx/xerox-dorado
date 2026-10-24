#!/usr/bin/env python3
"""Use the proved sparse fall on the actual cached control direct path.

Keep all original844-Q/9216-RAM/100-clock reference checks. The existing
generic cache's fast-path certificate selects the shortcut only for a stable
HIGH->LOW transfer; any X/Z input retains the full evaluator. This checks the
existing committed-owner caller only, NOT a cache shared by uncommitted packet
events. No native timing or clock-rate selection follows from this test.
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
from test_r_front_forward_view import sha

WRAPPER='''module dorado_control_certified_fall_step #(parameter integer WAVES=5)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,
 output wire [155:0] state_next,output wire [55:0] clock_events,repeated_events,
 output wire unsettled,input wire refresh_before_run_capture,output wire [93:0] flags,
 input wire entry_canonical,output reg using_sparse);
 wire [362:0] full_result,fall_result;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) generic_step(
  state_q,clock_before,clock_after,pins_before,pins_after,
  full_result[362:207],full_result[206:151],full_result[150:95],full_result[94],
  refresh_before_run_capture,full_result[93:0]);
 dorado_control_fall_step falling(state_q,pins_after,refresh_before_run_capture,
  fall_result[362:207],fall_result[206:151],fall_result[150:95],fall_result[94],fall_result[93:0]);
 always @*begin
  using_sparse=0;
  case(^{state_q,clock_before,clock_after,pins_before,pins_after,refresh_before_run_capture,entry_canonical})
   1'b0,1'b1:using_sparse=entry_canonical && clock_before && !clock_after && pins_before==pins_after;
   default:using_sparse=0;
  endcase
 end
 assign {state_next,clock_events,repeated_events,unsettled,flags}=using_sparse ? fall_result : full_result;
endmodule
'''


def transform(raw,helper,mutation=None):
    old=''' dorado_control_hold_atomic_step #(.WAVES(WAVES)) direct_arrival(
  state_q,clock_before,clock_after,pins_before,pins_after,
  direct_q,direct_events,direct_repeated,direct_unsettled,
  refresh_before_run_capture,direct_flags);'''
    new=old.replace('dorado_control_hold_atomic_step','dorado_control_certified_fall_step',1).replace(
        'refresh_before_run_capture,direct_flags);','refresh_before_run_capture,direct_flags,fast_path,);',1)
    assert raw.count(old)==1
    if mutation=='wrong-cram':
        assert helper.count('next_cram,state_q[29:0]')==1
        helper=helper.replace('next_cram,state_q[29:0]','!next_cram,state_q[29:0]',1)
    elif mutation=='drop-clock':
        helper=helper.replace("assign events=56'(capture_cram)<<8;","assign events=56'b0;",1)
    result=raw.replace(old,new,1)+'\n'+helper+'\n'+WRAPPER
    assert result[:-(len(helper)+len(WRAPPER)+2)].replace(new,old,1)==raw
    return result


def run(source,proof,out,mutation):
    assert not out.exists()
    p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_CERTIFIED_FALL_AND_PARALLEL_RISE_FULL_CONTROL'
    assert len(p['cases'])==11 and all(c['passed'] for c in p['cases'])
    for name,digest in p['files'].items():
        assert sha(proof/name)==digest
    s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_SELECTOR_COMPLETE_ORIGINAL_COMPARISON'
    assert s['original_testbench_and_references_unchanged'] and s['all100_clock_counts_match']
    raw=(source/'candidate.sv').read_text()
    for name in ['dorado_control_hold_atomic_step','dorado_control_terminal_body']:
        assert module(raw,name)==module((proof/'native-control.sv').read_text(),name)
    helper=(proof/'dorado_control_fall_step.sv').read_text()
    actual=transform(raw,helper,mutation)
    out.mkdir(parents=True)
    paths=[];rows=[]
    for row in s['source_files']:
        path=source/row['path'];assert sha(path)==row['output_sha256']
        dest=out/row['path'];dest.parent.mkdir(parents=True,exist_ok=True)
        dest.write_bytes(actual.encode() if path.name=='candidate.sv' else path.read_bytes())
        paths.append(dest);rows.append(dict(path=row['path'],parent_sha256=sha(path),output_sha256=sha(dest)))
    tb=out/'inputs/tb_r_front_local.sv';original_tb=tb.read_text()
    marker='\nendmodule'
    addition='''
 integer sparse_falls=0;
 always @(posedge host_clk)if(advance && cp_ready && dut.control.direct_arrival.using_sparse)sparse_falls=sparse_falls+1;
 final $display("CONTROL_FALL_COVERAGE captures=%0d",sparse_falls);
'''
    assert original_tb.count(marker)==1;tb.write_text(original_tb.replace(marker,addition+marker,1))
    assert tb.read_text().replace(addition,'',1)==original_tb
    for row in rows:
        row['output_sha256']=sha(out/row['path'])
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    engine=s['engine'];defines=[v for v in s['actions'][0]['command'] if v.startswith('-D')]
    commands=([
        ['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*map(str,paths)],
        ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        engine=engine,mutation=mutation,source_files=rows,actions=[],original_checks_and_references_retained=True,
        bench_change='passive coverage counter only; inverse byte-identical',
        native_integrated=False,uncommitted_packet_cache_context_qualified=False,
        timing_qualified=False,firmware_qualified=False,speedup_qualified=False,hardware_changed=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    for label,command in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            proc=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,command=command,pid=proc.pid,started=time.time());state['actions'].append(row);save()
            code=proc.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    if mutation:
        passed=state['actions'][0]['returncode']==0 and code!=0 and re.search(r'(mismatch|differs|R front solver unsettled/repeated)',log,re.I) is not None
    else:
        passed=code==0 and 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392' in log
        if passed:
            clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
            assert len(clocks)==100 and clocks==re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
            coverage=re.search(r'^CP_PREFIX_CACHE (.+)$',log,re.M)[1]
            assert coverage==s['cache_coverage']
            uses=int(re.search(r'CONTROL_FALL_COVERAGE captures=(\d+)',log)[1]);assert uses>0
            state.update(sparse_falls=uses,all100_clock_counts_match=True,cache_coverage=coverage)
    state.update(status=('PASS_CONTROL_FALL_ORIGINAL_COUNTEREXAMPLE' if mutation else 'PASS_CONTROL_FALL_COMPLETE_ORIGINAL_CAPTURE')
        if passed else 'TERMINAL_CONTROL_FALL_REFERENCE_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


def classify_negative(out):
    """Preserve an earlier driver's unrecognized original-assertion rejection."""
    state=json.loads((out/'run-state.json').read_text())
    assert state['mutation'] in ['wrong-cram','drop-clock']
    assert state['actions'][0]['returncode']==0 and state['actions'][-1]['returncode']!=0
    log=(out/'test.log').read_text()
    check=2 if state['mutation']=='wrong-cram' else 14
    assert f'R_FRONT_LOCAL_SOLVER check={check} ' in log
    assert 'R front solver unsettled/repeated' in log
    target=out/'negative-classification.json';assert not target.exists()
    result=dict(status='PASS_CONTROL_FALL_ORIGINAL_REJECTION_CLASSIFICATION',
        mutation=state['mutation'],original_assertion='R front solver unsettled/repeated',original_check=check,
        run_state_sha256=sha(out/'run-state.json'),test_log_sha256=sha(out/'test.log'),
        original_driver_state_and_all_checks_preserved=True,no_reexecution_or_assertion_change=True)
    target.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


def four_state(proof,out,bad_unknown):
    assert not out.exists()
    s=json.loads((proof/'run-state.json').read_text())
    assert s['status']=='PASS_CERTIFIED_FALL_AND_PARALLEL_RISE_FULL_CONTROL' and all(c['passed'] for c in s['cases'])
    for name,digest in s['files'].items():
        assert sha(proof/name)==digest
    witness=proof.parent/'control-fall-fusion-proof-v1-20261024/fusion-positive-w5/proof.log'
    values={n:int(b,2) for n,b in re.findall(r'\\(q|pins)\s+--\s+--\s+([01]+)',witness.read_text())}
    assert set(values)=={'q','pins'}
    tb='''`timescale 1ns/1ps
module tb_control_fall_unknown;
 reg [955:0] stimulus,base;
 wire [155:0] q;wire before_root,after_root,refresh,certificate;
 wire [397:0] before_pins,after_pins;
 assign {q,before_root,after_root,before_pins,after_pins,refresh,certificate}=stimulus;
 wire [362:0] original[0:1],actual[0:1];wire [1:0] selected;
 for(genvar w=0;w<2;w++)begin: modes
  dorado_control_hold_atomic_step #(.WAVES(w==0 ? 5:24)) reference_step(q,before_root,after_root,before_pins,after_pins,
   original[w][362:207],original[w][206:151],original[w][150:95],original[w][94],refresh,original[w][93:0]);
  dorado_control_certified_fall_step #(.WAVES(w==0 ? 5:24)) actual_step(q,before_root,after_root,before_pins,after_pins,
   actual[w][362:207],actual[w][206:151],actual[w][150:95],actual[w][94],refresh,actual[w][93:0],certificate,selected[w]);
 end
 integer known=0,unknown_checks=0;
 initial begin
  stimulus={156'hQ_VALUE,1'b1,1'b0,398'hPIN_VALUE,398'hPIN_VALUE,1'b1,1'b1};#2;
  for(integer w=0;w<2;w++)begin
   if(actual[w] !== original[w] || selected[w] !== 1'b1)$fatal(1,"CONTROL_FALL_UNKNOWN binary witness w=%0d",w);
   known=known+1;
  end
  base=stimulus;
  for(integer i=0;i<956;i++)for(integer kind=0;kind<2;kind++)begin
   stimulus=base;stimulus[i]=kind ? 1'bz:1'bx;#2;
   for(integer w=0;w<2;w++)begin
    if(selected[w] !== 1'b0 || actual[w] !== original[w])
     $fatal(1,"CONTROL_FALL_UNKNOWN fallback bit=%0d kind=%0d w=%0d selected=%b actual=%h expected=%h",i,kind,w,selected[w],actual[w],original[w]);
    unknown_checks=unknown_checks+1;
   end
  end
  $display("CONTROL_FALL_FOUR_STATE_PASS known=%0d unknown=%0d inputs=956 outputs=363 waves=5/24",known,unknown_checks);
  $finish;
 end
endmodule
'''.replace('Q_VALUE',format(values['q'],'x')).replace('PIN_VALUE',format(values['pins'],'x'))
    out.mkdir(parents=True)
    for name in ['atomic.sv','dorado_control_fall_step.sv']:
        shutil.copy2(proof/name,out/name)
    wrapper=WRAPPER
    if bad_unknown:
        assert wrapper.count('default:using_sparse=0;')==1
        wrapper=wrapper.replace('default:using_sparse=0;','default:using_sparse=1;',1)
    (out/'wrapper.sv').write_text(wrapper);(out/'tb.sv').write_text(tb)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    sources=[out/n for n in ['atomic.sv','dorado_control_fall_step.sv','wrapper.sv','tb.sv']]
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),proof=str(proof),
        proof_state_sha256=sha(proof/'run-state.json'),bad_unknown=bad_unknown,
        inputs=956,outputs=363,all_inputs_receive_X_and_Z=True,actions=[],
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()},native_integrated=False,hardware_changed=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    commands=[['iverilog','-g2012','-s','tb_control_fall_unknown','-o',str(out/'test.vvp'),*map(str,sources)],
              ['vvp','-i',str(out/'test.vvp')]]
    for label,command in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,command=command,pid=p.pid,started=time.time());state['actions'].append(row);save()
            code=p.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=(code==0 and 'CONTROL_FALL_FOUR_STATE_PASS known=2 unknown=3824 inputs=956 outputs=363 waves=5/24' in log) if not bad_unknown else (
        state['actions'][0]['returncode']==0 and code!=0 and 'CONTROL_FALL_UNKNOWN fallback' in log)
    state.update(status=('PASS_CONTROL_FALL_UNKNOWN_COUNTEREXAMPLE' if bad_unknown else 'PASS_CONTROL_FALL_FOUR_STATE') if passed else
        'TERMINAL_CONTROL_FALL_FOUR_STATE_NOT_ESTABLISHED',finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','proof','output']:
        p.add_argument('--'+name,type=Path,required=name=='output')
    p.add_argument('--mutation',choices=['wrong-cram','drop-clock'])
    p.add_argument('--classify-negative',action='store_true')
    p.add_argument('--four-state',action='store_true')
    p.add_argument('--bad-unknown',action='store_true')
    a=p.parse_args()
    if a.classify_negative:
        classify_negative(a.output.resolve())
    elif a.four_state:
        assert a.proof is not None
        raise SystemExit(four_state(a.proof.resolve(),a.output.resolve(),a.bad_unknown))
    else:
        assert a.source is not None and a.proof is not None
        raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.mutation))
