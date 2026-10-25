#!/usr/bin/env python3
"""Preserve actual ALU/MIR/function-RAM evidence and the full-core timing job.

Original1064-state composition and normal16-address function writes pass;
optimized capture still fails and remains unqualified. Full-core15ns timing
uses the corrected native caller, with eight ticks/120ns, not the new ALU
capture prototype or the future four-tick60ns event batch.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from fit_r_front import quartus_cone
from native_branch_flag_sources import alu_producers
from review_baseboard_clock_consumer import verify
from review_late_hold_and_input_reset import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha
from test_processor_alu_composition import with_write_suffix

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'generate_processor_alu_reference.py','generate_processor_alu_projection.py',
       'test_processor_alu_composition.py','native_branch_flag_sources.py',
       'stage_native_cp_read_timing.py','test_native_quartus_cone.py']


def remote_observation(stage,out):
    r=json.loads((stage/'r-front-control-native.json').read_text());project=r['project']
    assert project=='/home/alans/mister/Dorado_CPRead15_Diag_v2_20261025'
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT)
r=json.loads((p/'r-front-control-native.json').read_text())
s=json.loads((p/'run-state.json').read_text())
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in r['source_hashes']}
assert actual==r['source_hashes']
processes=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid']]+[a['pid']for a in s['stages']if 'returncode'not in a]:
  row=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
  processes.append({'pid':pid,'ps':row.stdout.strip(),'returncode':row.returncode})
 assert processes[0]['returncode']==0 and str(p)in processes[0]['ps']
result={'observed':time.time(),'project':str(p),'run_state':s,'source_readback':actual,'live_processes':processes,
 'builder_log_tail':(p/'builder.log').read_text().splitlines()[-15:]}
print(json.dumps(result))
'''.replace('PROJECT',repr(project))
    (out/'remote-observation.py').write_text(query)
    result=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],
                          input=query,text=True,capture_output=True,check=True)
    observation=json.loads(result.stdout);assert observation['source_readback']==r['source_hashes']
    assert observation['run_state']['builder_pid']==2865831
    (out/'remote-observation.json').write_text(json.dumps(observation,indent=2)+'\n');return observation


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    good=[BUILD/f'processor-alu-mir-writes-{e}-v2-20261025'for e in ['icarus','verilator']]
    states=[checked(d)for d in good]
    assert states[0]['outcomes']==states[1]['outcomes']
    for d,s in zip(good,states):
        assert s['status']=='PASS_ORIGINAL_ALU_PROCESSOR_COMPOSITION'and s['returncode']==0
        assert s['normal_MIR_ALUF_write_suffix']and s['records']==15470 and s['original_prefix_records']==14884
        assert not s['optimized_flag_transfer_checked']
        assert s['reference']['physical_bits']==1064 and s['reference']['clock_channels']==108
        assert s['reference']['actual_instruction_ALUF_pins']==[363,364,365,366]
        assert not any(f'ALUF.{i}'in s['reference']['extra_boundary_pins']for i in range(4))
        log=(d/'test.log').read_text()
        assert 'ALU_FUNCTION_WRITE_COVERAGE addresses=ffff address_changes=0 data_changes=0'in log
        assert 'events=15470 states=1064 RAM=9216 clocks=108 observations=964'in log
        counts=dict(re.findall(r'^ALU_COMPOSITION_CLOCK channel=(\d+) edges=(\d+)$',log,re.M))
        assert len(counts)==108 and counts['105']==counts['106']=='64'
        h=(d/'reference/timed/processor_alu_proch.sv').read_text()
        assert 'cell_MC10170 #(.LEGACY_B_INVERT(0)) d13('in h
        low=(d/'reference/timed/processor_alu_procl.sv').read_text()
        assert 'original_F10000_gated l16('in low and 'original_F10000_gated l18('in low
        assert 'original_F10000 g16('in low
        original=(BUILD/'processor-raw-input-recording-v1-20261025/raw-inputs.hex').read_text().splitlines()
        lines=(d/'raw-inputs.hex').read_text().splitlines();assert lines[:14884]==original
        expected,selectors=with_write_suffix(original);assert lines==expected
        assert [int(x,16)for x in(d/'selectors.hex').read_text().splitlines()]==selectors
    for p in(good[0]/'reference/timed').iterdir():
        if p.suffix in ['.sv','.v']:assert sha(p)==sha(good[1]/'reference/timed'/p.name)
    a=(good[0]/'tb.sv').read_text().replace(str(good[0]),'RUN')
    b=(good[1]/'tb.sv').read_text().replace(str(good[1]),'RUN');assert a==b
    negative=BUILD/'processor-alu-mode-negative-icarus-v1-20261025';s=checked(negative)
    assert s['returncode']!=0 and s['mutation']=='mode-data'
    differences=re.findall(r'^ALU_COMPOSITION_STATE_DIFF event=(\d+) bit=(\d+) full=([01]) split=([01])$',(negative/'test.log').read_text(),re.M)
    assert differences==[('15','929','1','0')]
    native=BUILD/'native-cp-read-j19-boot-v1-20261025'
    inventory=BUILD/'processor-alu-native-binding-v1-20261025'
    binding=alu_producers(native,extended=True)
    assert json.loads(json.dumps(binding))==json.loads((inventory/'results.json').read_text())
    # Retain the unresolved source-arrival problem, not a false capture pass.
    failures=[]
    for engine,event,bits in [('verilator','68',['64']),('icarus','15',['72'])]:
        d=BUILD/f'processor-alu-mir-projection-{engine}-v1-20261025';s=checked(d)
        assert s['returncode']!=0 and s['explicit_pre_root_ALU_projection']
        differences=re.findall(r'^ALU_TRANSFER_DIFF event=(\d+) bit=(\d+) predicted=([01]) original=([01])$',(d/'test.log').read_text(),re.M)
        assert [x[0]for x in differences]==[event]*len(bits) and[x[1]for x in differences]==bits
        failures.append(dict(engine=engine,differences=differences))
    stage=BUILD/'native-cp-read-full15-stage-v2-20261025';r=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,r['source_hashes'])
    assert r['frequency_hz']==66666667 and r['sysper']==4 and r['nominal_ordinary_ns']==120
    assert not r['full60ns_event_batch_connected']and not r['new_branch_condition_family_integrated']and not r['assembly_permitted']
    cone='src/verilog/rtl/dorado_r_front_cone.sv'
    assert(stage/cone).read_text()==quartus_cone((native/'inputs'/cone).read_text())
    lowering=BUILD/'native-cp-read-quartus-lowering-v1-20261025';l=checked(lowering)
    assert l['status']=='PASS_NATIVE_QUARTUS_CONE_LOWERING'and l['output_bits']==1128
    assert 'cases=1024 input_bits=973 output_bits=1128'in(lowering/'test.log').read_text()
    oldfit=BUILD/'native-cp-read-full15-v1-terminal';old=json.loads((oldfit/'run-state.json').read_text())
    assert old['status']=='TERMINAL_FAILED'and old['stages'][0]['returncode']==3
    assert 'syntax error at dorado_r_front_cone.sv(5703)'in(oldfit/'map.log').read_text()
    observation=remote_observation(stage,out)
    dirs=[d for d in sorted(BUILD.glob('processor-alu-*-20261025'))if not d.name.startswith('processor-alu-review-')]
    dirs += [lowering,stage,BUILD/'native-cp-read-full15-stage-v1-20261025',oldfit]
    for d in dirs:retain(d,out/d.name)
    live=[]
    for d in [native,BUILD/'control-short-native-jumper-boot-v2-20261024']:
        s=json.loads((d/'run-state.json').read_text());action=s['actions'][-1]
        assert s['status']=='RUNNING'and'returncode'not in action
        ps=subprocess.check_output(['ps','-p',str(action['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True).strip();assert str(d)in ps
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        progress=[x for x in(d/'test.log').read_text().splitlines()if x.startswith('NATIVE_PROGRESS')][-1]
        live.append(dict(directory=d.name,pid=action['pid'],ps=ps,observed=time.time(),progress=progress))
    assert sha(BUILD/'cedar-work-native-hints.pack')=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='REVIEWED_ACTUAL_ALU_MIR_FUNCTION_WRITES_AND_FULL_CORE_TIMING_DIAGNOSTIC',scope=__doc__,
        comparison_summary=states[0]['outcomes'][-1],all108_clock_counts=states[0]['outcomes'][:-1],
        normal_ALUF_writes=dict(address_mask='ffff',windows_per_RAM=64,address_changes=0,data_changes=0),
        mode_register_negative=dict(event=15,actual_global_bit=929),
        native_ALU_binding=binding,rejected_capture_attempts=failures,full_core_observation=observation,
        full_core_current_nominal_ns=120,target_nominal_ns=60,full_core_assembly_permitted=False,
        native_boot_observations=live,
        remaining_operand_registers_are_boundaries=True,source_arrivals_unknowns_and_capture_not_qualified=True,
        next_action='Follow the same full-core fit and report its critical paths; close actual operand/control-register phases before qualifying the ALU capture projection',
        full_core_timing_qualified=False,firmware_Cedar_or_speed_qualified=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='processor-alu-and-full15',extra_tools=TOOLS)
    print(r['status'])
