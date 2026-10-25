#!/usr/bin/env python3
"""Retain bounded full-processor CP passes and the rejected interior-RAM epoch.

The original graph observes a transient STK read between identical RM endpoints.
All original event assertions remain. A two-cell memory control diagnoses this
counterexample; it does not qualify arbitrary memory, native timing or a core.
"""
import argparse
import json
from pathlib import Path
import re

from generate_r_front_cp_source_reference import generate
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_shared_control_capture.py','generate_r_front_cp_source_reference.py',
       'generate_cp_block_capture.py','generate_cp_prblock_capture.py',
       'test_r_front_cp_source.py','review_r_front_cp_source.py']
PASS='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE'


def read(p):return json.loads(p.read_text())


def actual_profile(d):
    gates=set()
    for p in (d/'timed').glob('cell_*.v'):
        gates.update(map(int,re.findall(r'assign\s+#\((\d+)\)',p.read_text())))
    if not gates:return None
    text=(d/'timed/run_control_original_ff.sv').read_text()
    phase=module(text,'original_SE10231')
    ff=set(map(int,re.findall(r'<=#\((\d+)\)',text.replace(phase,''))))
    ph=set(map(int,re.findall(r'<=#\((\d+)\)',phase)))
    assert len(gates)==len(ff)==len(ph)==1,(d,gates,ff,ph)
    return [next(iter(gates)),next(iter(ff)),next(iter(ph))]


def initial(d):
    value=int((d/'initial.hex').read_text().strip(),16)
    return value>>9216,value&((1<<9216)-1)


def word(ram,base,n):
    return ((ram>>(base+4*n))&15)|(((ram>>(base+1024+4*n))&15)<<4)|(((ram>>(base+(2304 if base%4608==2048 else 4096)+n))&1)<<8)


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    reference=out/'independent-reference';manifest=generate(ROOT,reference)
    assert manifest['physical_bits']==844 and manifest['ram_bits']==9216 and manifest['clock_channels']==100
    runs=[];retained=[]
    for d in sorted(BUILD.glob('r-front-cp-source-*20261024')):
        if d.name=='r-front-cp-source-reference-v1-20261024':
            for p in reference.glob('*.sv'):assert p.read_bytes()==(d/p.name).read_bytes()
            retained.append(d);continue
        f=d/'run-state.json'
        if not f.exists():
            assert d.name=='r-front-cp-source-block-verilator-v1-20261024'
            runs.append(dict(directory=d.name,status='GENERATION_FAILED_BEFORE_RUN_STATE',simulation_executed=False))
            retained.append(d);continue
        s=read(f);assert s['status'] not in ['PREPARING','RUNNING'],d.name
        verify(d,s.get('files',{}))
        for row in s.get('source_files',[]):assert sha(d/row['path'])==row['output_sha256'],(d.name,row['path'])
        actions=s.get('actions',[])
        for a in actions:
            assert a.get('returncode') is not None,(d.name,a)
            assert sha(d/(a['label']+'.log'))==a['log_sha256']
        if 'source_files' in s:
            for p in reference.glob('*.sv'):assert p.read_bytes()==(d/'reference'/p.name).read_bytes(),(d.name,p.name)
            assert s['physical_Q_bits']==844 and s['physical_RAM_bits']==9216 and s['clock_channels']==100
            assert s['all_original_event_assertions_retained'] and not s['native_integrated'] and not s['physical_timing_qualified']
        profile=actual_profile(d)
        declared=[s.get('declared_gate_delay_ps'),s.get('declared_ff_delay_ps'),s.get('declared_phase_delay_ps')]
        corrected=profile is not None and profile!=declared
        if corrected:
            assert s.get('profile') is None and profile==[1000,1000,1000] and declared==[1750,400,1250]
        elif profile is not None:assert profile==declared
        log=(d/'test.log').read_text()if(d/'test.log').exists()else''
        if s['status']==PASS:
            assert len(actions)==2 and all(a['returncode']==0 for a in actions)
            assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',log,re.M))==100
            assert re.search(r'FULL_PROCESSOR_CP_(?:BLOCK|SOURCE|COLLISION)_PASS',log)
        else:assert not actions or actions[-1]['returncode']!=0 or s['status']=='TERMINAL_PREPARATION_FAILED'
        runs.append(dict(directory=d.name,status=s['status'],state_sha256=sha(f),engine=s.get('engine'),
                         actual_profile_ps=profile,historical_profile_metadata_corrected=corrected,
                         test_passed=s['status']==PASS,ready_fixture=s.get('initial_ready_fixture','ffff'),
                         equal_indexed_banks=s.get('equal_RM_STK_fixture_data',False),
                         matched_intermediate_data=s.get('matched_intermediate_port_data',False)))
        retained.append(d)
    def run(name):
        d=BUILD/name;s=read(d/'run-state.json');return d,s,(d/'test.log').read_text()
    _,all_s,all_log=run('r-front-cp-source-all-verilator-v3-20261024')
    assert all_s['status']==PASS and 'checks=12481 words=4096 Qs=844 RAM=9216 clocks=100 sets=3ffff' in all_log
    _,coll_s,coll_log=run('r-front-cp-source-collisions-verilator-v2-20261024')
    assert coll_s['status']==PASS and coll_s['trace_context_discontinuities']==128
    assert len(re.findall(r'^FULL_CP_FIXTURE_BOUNDARY ',coll_log,re.M))==128
    assert 'collisions=128 Qs=844 RAM=9216 clocks=100 sets=3ffff' in coll_log
    without,ws,_=run('r-front-cp-source-block-without-icarus-v4-20261024');assert ws['status']==PASS
    traces={}
    for engine,version in [('icarus',5),('verilator',2)]:
        d,s,log=run(f'r-front-cp-source-ready0-later-prblock-{engine}-v{version}-20261024')
        assert s['status']!=PASS and actual_profile(d)==[1750,400,1250]
        assert 'operand gate output mismatch check=194' in log
        assert 'R_flags=0/1 RM_flags=3/2' in log
        assert 'C=1 D9=0 Q2=0' in log and 'predicted_aux=0' in log
        rows=[]
        for line in log.splitlines():
            m=re.search(r'^FULL_CP_SRAM_ARRIVAL t=([\d.]+) address=(\w+) select=(\w+) data=(\w+)',line)
            if m:rows.append(dict(time_ns=float(m[1]),address=m[2],select=m[3],data=m[4]))
        assert any(r['select']=='5' and r['data']=='632f' for r in rows)
        assert rows[-1]['select']=='a' and rows[-1]['data']=='46f4'
        data=min(r['time_ns']for r in rows if r['select']=='5' and r['data']=='632f')
        q=min(float(m[1])for m in re.finditer(r'^FULL_CP_RFLAG_ARRIVAL t=([\d.]+).*l07_C=11 .* Q=01 ',log,re.M))
        close=min(float(m[1])for m in re.finditer(r'^FULL_CP_RFLAG_ARRIVAL t=([\d.]+).*l07_C=00 .* Q=01 ',log,re.M))
        assert abs(q-data-.4)<1e-6 and abs(close-data-2.1)<1e-6
        traces[engine]=dict(SRAM_changes=rows,data_arrival_ns=data,flag_Q_arrival_ns=q,latch_close_ns=close,
                            Q_before_close_margin_ns=round(close-q,6))
        matched,ms,mlog=run(f'r-front-cp-source-ready0-later-matched-ports-{engine}-v1-20261024')
        assert ms['status']==PASS and ms['matched_intermediate_port_data']
        owner,ram=initial(d);new_owner,new_ram=initial(matched);assert owner==new_owner
        changed=ram^new_ram
        allowed=sum(15<<b for b in [2048,3072,6656,7680])|(1<<4352)|(1<<8960)
        assert changed and changed&~allowed==0
        assert [word(ram,0,255),word(ram,4608,15)]==[0xf4,0x46]
        assert [word(ram,2048,0),word(ram,6656,0)]==[0x2f,0x163]
        assert [word(new_ram,2048,0),word(new_ram,6656,0)]==[0xf4,0x46]
        assert 'R_flags=0/0 RM_flags=3/3' in mlog
    _,inactive,ilog=run('r-front-cp-source-ready0-later-inactive-icarus-v1-20261024')
    assert inactive['status']==PASS and not inactive['CP_strobe_pulsed']
    for d in retained:retain(d,out/d.name)
    result=dict(status='REVIEWED_BOUNDED_CP_PASSES_AND_REJECTED_ENDPOINT_ONLY_RAM_CAPTURE',scope=__doc__,runs=runs,
                full_processor_Qs=844,RAM_bits=9216,clock_channels=100,all_original_event_assertions_retained=True,
                actual_balanced_profile_full_words=4096,continuous_history_checks=12481,fresh_collision_fixtures=128,
                inherited_ffff_block_disabled_control_also_passed=True,TPC_negative_nonvacuous=False,
                earlier_profile_metadata_wrong=True,compiled_sources_and_historical_states_unchanged=True,
                current_strict_profile_ps=[1750,400,1250],SRAM_read_delay_ps=1000,
                PrBlock_g16_capture_descriptor_only=True,interior_RAM_counterexample_both_engines=traces,
                matched_two_STK_cells_diagnostic_passes_both_engines=True,all_other_memory_and_owner_bits_unchanged=True,
                equal_same_index_bank_data_control_still_failed=True,inactive_CP_control_passed=True,
                required_fix='Represent BEFORE/interior RAM/CLOSING/AFTER epochs; stage the same physical port before owner publication; preserve all16 visible R latches and clock/data/Set captures.',
                endpoint_only_capture_qualified=False,arbitrary_owner_or_arrival_qualified=False,
                native_integrated=False,physical_timing_qualified=False,new_RBF_ready=False,hardware_changed=False,
                last_readonly_FPGA_core='SMC777',full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='full-processor-cp-interior-ram',extra_tools=TOOLS)
    print(r['status'])
