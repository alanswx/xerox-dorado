#!/usr/bin/env python3
"""Review original924-state composition and preserve the real transfer failure.

The wiring reference passes, but the optimized80-bit transfer fails with actual
processor/control producers. No native replacement, timing or speed is selected.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from review_baseboard_clock_consumer import verify
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_processor_branch_reference.py','test_processor_branch_composition.py',
       'diagnose_processor_trace_replay.py',Path(__file__).name]


def read(path):return json.loads(path.read_text())


def checked(directory):
    s=read(directory/'run-state.json');verify(directory,s['files'])
    assert s['actions'][0]['returncode']==0
    assert all('returncode'in a for a in s['actions'])
    for a in s['actions']:
        if 'log_sha256'in a:assert sha(directory/(a['label']+'.log'))==a['log_sha256']
    return s


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    original=BUILD/'cp-original-program-balanced-verilator-v2-20261025'
    raw=BUILD/'processor-raw-input-recording-v1-20261025';r=checked(raw)
    assert r['status']=='PASS_PASSIVE_RAW_INPUT_RECORDING'
    assert r['source_state_sha256']==sha(original/'run-state.json')
    assert r['actions'][-1]['returncode']==0
    for n in ['transactions.hex','initial.hex']:assert sha(raw/n)==sha(original/n)
    # The source program/reference/checkers are byte-identical after removing
    # precisely the three passive recording additions and output relocation.
    tb=(raw/'inputs/tb_r_front_local.sv').read_text()
    tb,n=re.subn(r'\n integer raw_trace;reg \[400:0\] raw_before;reg raw_before_root;\n initial begin raw_trace=.*?end\n','',tb,count=1)
    assert n==1
    marker='     raw_before=rp;raw_before_root=root;\n';assert tb.count(marker)==1;tb=tb.replace(marker,'',1)
    tb,n=re.subn(r'     \$fdisplay\(raw_trace,[^\n]+\n     if\(checks<20\)\$display\([^\n]+\n','',tb,count=1)
    assert n==1 and tb.replace(str(raw),str(original))==(original/'inputs/tb_r_front_local.sv').read_text()
    old_lines=(original/'transactions.hex').read_text().splitlines()
    raw_lines=(raw/'raw-inputs.hex').read_text().splitlines()
    assert len(old_lines)==len(raw_lines)==14884
    event=14;owner=(int(old_lines[event],16)>>(9216+1626+556))&65535
    physical=(int(raw_lines[event],16)>>556)&65535
    assert owner==0 and physical==0x632f
    good=[BUILD/f'processor-branch-composition-{e}-v5-20261025'for e in ['verilator','icarus']]
    states=[checked(d)for d in good]
    assert all(s['status']=='PASS_ORIGINAL_PROCESSOR_BRANCH_COMPOSITION'and s['returncode']==0 for s in states)
    assert states[0]['outcomes']==states[1]['outcomes']
    summary=states[0]['outcomes'][-1]
    assert 'events=14884 states=924 RAM=9216 clocks=104 observations=541 legacy_fields=842 closed_producer_fields=2'in summary
    for d in good:
        assert sha(d/'raw-inputs.hex')==sha(raw/'raw-inputs.hex')
        assert sha(d/'transactions.hex')==sha(original/'transactions.hex')
    for p in (good[0]/'reference').rglob('*'):
        if not p.is_file():continue
        peer=good[1]/p.relative_to(good[0])
        if p.name=='manifest.json':
            assert json.loads(p.read_text().replace(str(good[0]),'RUN'))==json.loads(peer.read_text().replace(str(good[1]),'RUN'))
        else:assert sha(p)==sha(peer)
    negative=BUILD/'processor-branch-carry-negative-icarus-v2-20261025';n=checked(negative)
    assert n['returncode']!=0 and n['mutation']=='carry-feedback'
    log=(negative/'test.log').read_text()
    assert re.findall(r'^COMPOSITION_STATE_DIFF event=(\d+) bit=(\d+) full=([01]) divided=([01])$',log,re.M)==[('17','91','1','0')]
    assert 'original circuits disagree event=17'in log
    failures=[]
    for engine in ['verilator','icarus']:
        d=BUILD/f'processor-branch-transfer-{engine}-v1-20261025';s=checked(d)
        assert s['returncode']!=0 and s['optimized_branch_transfer_checked']
        assert (d/'branch-step.sv').read_text()==(ROOT/'verilog/rtl/dorado_branch_condition_step.sv').read_text()
        log=(d/'test.log').read_text()
        diff=re.findall(r'^COMPOSITION_TRANSFER_DIFF event=(\d+) bit=(\d+) predicted=([01]) original=([01])$',log,re.M)
        assert diff==[('19','67','0','1')]
        failures.append(dict(engine=engine,directory=d.name,differences=diff))
    trial=BUILD/'processor-branch-transfer-preclose-verilator-v1-20261025';s=checked(trial)
    assert s['returncode']!=0 and s['experimental_preclose_latch_capture']
    log=(trial/'test.log').read_text()
    assert re.findall(r'^COMPOSITION_TRANSFER_DIFF event=(\d+) bit=(\d+) predicted=([01]) original=([01])$',log,re.M)==[
        ('23','65','1','0'),('23','66','1','0'),('23','67','1','0')]
    runs=[]
    for d in sorted(BUILD.glob('processor-branch-*-20261025')):
        if d.name.startswith('processor-branch-review-'):continue
        retain(d,out/d.name);runs.append(d.name)
    retain(raw,out/raw.name)
    live=[]
    for name in ['native-cp-read-j19-boot-v1-20261025','control-short-native-jumper-boot-v2-20261024']:
        d=BUILD/name;s=read(d/'run-state.json');a=s['actions'][-1]
        assert s['status']=='RUNNING'and a['stage']=='test'and 'returncode'not in a
        ps=subprocess.check_output(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True).strip()
        assert str(d)in ps
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        live.append(dict(directory=name,pid=a['pid'],ps=ps,observed=time.time(),host_limit=5000000000,
                         progress=(d/'test.log').read_text().splitlines()[-6:]))
    result=dict(status='REVIEWED_ORIGINAL_924_COMPOSITION_AND_REJECTED_OPTIMIZED_FLAG_INTEGRATION',scope=__doc__,
                summary=summary,all104_clocks=states[0]['outcomes'][:-1],
                original_program_and_assertions_unchanged=True,original_traces_byte_identical=True,
                physical_versus_retained_owner_witness=dict(event=14,dR_owner=owner,dR_physical=physical),
                carry_feedback_negative=dict(event=17,actual_global_bit=91),
                original_reference_composition_passes_both_engines=True,
                optimized_transfer_integration_failures=failures,
                preclose_latch_trial_rejected=dict(event=23,bits=[65,66,67]),
                next_action='Derive actual early/late flag-data and latch-close epochs from the coupled graph; qualify that transfer before native integration and full-core timing',
                original_ALU_datapath_integrated=False,native_replacement_qualified=False,
                source_arrivals_unknowns_and_physical_timing_qualified=False,
                hardware_speedup_qualified=False,hardware_changed=False,full_goal_active=True,
                retained_runs=runs+[raw.name],native_observations=live)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='processor-branch-composition',extra_tools=TOOLS)
    print(r['status'])
