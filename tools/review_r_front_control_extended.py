#!/usr/bin/env python3
"""Review consistent pre-event selectors, complete clock coverage and failed STA.

Digital component ordering and its15ns implementation budget are separate
evidence classes. A correct component that misses timing remains unqualified
for native deployment, original60ns cadence and hardware speed.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import tempfile

import fit_control_clock as fixture
from generate_r_front_control_candidate import generate
from review_r_front_control import read_run
from review_r_front_control_native import archive
from test_r_front_control_extended_matrix import BUILD,ROOT,check,CANDIDATE


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def read(p):return json.loads(p.read_text())


def timing(fit,masked=False):
    state=read(fit/'run-state.json');assert state['status']=='TERMINAL_REVIEW_REQUIRED'
    assert [s['stage']for s in state['stages']]==['map','fit','sta','corners']and all(s['returncode']==0 for s in state['stages'])
    source=read(fit/'provenance.json');assert source['clock_period_ns']==15 and not source['assembly_permitted']
    expected='ef0abf3eed4497350a1ada19ab8a00c0ddc5732ed81390722844eac1ddb0efea'if masked else CANDIDATE
    assert source['tested_candidate_sha256']==expected
    for n,h in source['files'].items():
        raw=(fit/n).read_bytes()
        if n=='clock_fit.qsf':raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        assert hashlib.sha256(raw).hexdigest()==h,(n,'Unexpected compiled-source change')
    assert(fit/'clock.sdc').read_text()==fixture.SDC
    log=(fit/'corners.log').read_text();corners=re.findall(r'CONTROL_CLOCK_CORNER (\S+)',log);assert len(corners)==len(set(corners))==4
    data=[]
    for c in corners:
        row=dict(corner=c)
        for mode in ['setup','hold','mpw','evaluator']:
            s=(fit/f'clock-{mode}-{c}.rpt').read_text();values=[float(x)for x in re.findall(r'; Slack\s*;\s*(-?\d+\.\d+)',s)]
            assert values,(c,mode);row[mode]=min(values)
        ucp=(fit/f'clock-ucp-{c}.rpt').read_text()
        for label in ['Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths']:
            assert re.search(r'; '+re.escape(label)+r'\s*; 0\s*; 0\s*;',ucp),(c,label)
        data.append(row)
    assert min(r['setup']for r in data)==(-4.184 if masked else -8.512)
    assert min(r['hold']for r in data)==(0.154 if masked else 0.105)
    assert min(r['mpw']for r in data)==(6.115 if masked else 6.116)
    worst=(fit/'clock-evaluator-7_slow_1100mv_-40c.rpt').read_text()
    for text in (['launch[293]~DUPLICATE','result[1119]~reg0','18.863','20']if masked else['launch[878]','result[339]~reg0','23.170','34']):assert text in worst
    summary=(fit/'clock_fit.fit.summary').read_text()
    alms,regs=(4041,3260)if masked else(3923,3261)
    assert f'{alms:,} / 41,910'in summary and f'Total registers : {regs}'in summary
    assert re.search(r'; Total MLAB memory bits\s*; 9,216\s*;', (fit/'clock_fit.fit.rpt').read_text(encoding='latin-1'))
    path=dict(input_launch=293,input_after_pin=291,input_signal="CPAddr.0'",result_bit=1119,result_state_bit=563,result_signal='dR high-byte input latch',logic_levels=20,data_delay_ns=18.863)if masked else dict(input_launch=878,input_state_bit=72,input_signal='ContA f14 Switcha',result_bit=339,result_signal='faults[0]',logic_levels=34,data_delay_ns=23.170)
    return dict(status='REJECTED_15NS_COMPONENT_SETUP',corners=data,ALMs=alms,registers=regs,MLAB_bits=9216,
                worst=path,
                changed_input_only='Quartus version annotation',constraints_unchanged=True,unconstrained_paths=0,
                native_timing_qualified=False,speedup_qualified=False)


def review(out,fit):
    assert not out.exists();dirs=[]
    with tempfile.TemporaryDirectory()as temp:
        p=Path(temp)/'correct.sv';generate(p,old_control_capture=True);assert sha(p)==CANDIDATE
        old=Path(temp)/'old.sv';generate(old);assert sha(old)=='a20b9e52134e671444e3e826ca907f3525cfad192944d04811171c099f67c32d'
        changed='''// Capture data keeps the pre-event MIR AND ContB selector epoch.
 // LOW CP rMIR clears both boards; new ASEL/BSEL can close the task clock,
 // but must not overwrite its old TaskingOn/Off data during that capture.
 wire [7:0] capture_local=local_decode(cq,conta_pins(producer_pins(joint_q,pins_after)));'''
        assert p.read_text().replace(changed,'wire [7:0] capture_local=local_decode(cq,conta_pins(capture_after));')==old.read_text()
    cases=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-control-extended-{engine}-v1-20261023';r,log=read_run(d);dirs.append(d)
        assert r['status']=='R_FRONT_LOCAL_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
        assert 'R_FRONT_LOCAL_SOLVER check=6680 shared=1'in log and 'R front solver unsettled/repeated'in log
        cases.append(dict(directory=d.name,status='REJECTED_MIXED_CONTROL_EPOCH',check=6680,engine=engine))
    d=BUILD/'r-front-control-extended-trace-verilator-v2-20261023';r,log=read_run(d);dirs.append(d)
    assert r['solver_rejection_deferred_for_diagnostic']and r['status']=='R_FRONT_LOCAL_PROBE_FAILED'
    assert 'JOINT_SHARED_DIFF ca_bit=40 child=0 joint=1'in log and 'JOINT_SHARED_DIFF ca_bit=41 child=0 joint=1'in log
    assert 'Diagnostic retains solver rejection after full reference comparison'in log and 'R_FRONT_DIFF bit='not in log
    # The two first corrected extended runs still miss downstream I/O clock
    # coverage because the maintenance fixture leaves rMIR asserted.
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-control-extended-{engine}-v3-20261023';r,log=read_run(d);dirs.append(d)
        assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['summary']=='checks=11291 writes=160 reads=4388 epochs=320'
        missing=[int(c)for c,n in re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)if int(n)==0]
        assert missing==[53,54];cases.append(dict(directory=d.name,status='INSUFFICIENT_IO_CLOCK_COVERAGE',missing=missing))
    matrixdir=BUILD/'r-front-control-extended-matrix-v1-20261023';matrix=read(matrixdir/'run-state.json')
    assert matrix['status']=='PASS_OLD_CONTROL_CAPTURE_PROFILE_MATRIX'and not matrix['errors']and len(matrix['cases'])==14
    for row in matrix['cases']:
        d=ROOT/row['directory'];assert check(d,row['extended'])['run_state_sha256']==row['run_state_sha256'];dirs.append(d)
    dirs.append(matrixdir)
    auditdir=BUILD/'r-front-control-clock-audit-v1-20261023';a=read(auditdir/'results.json');dirs.append(auditdir)
    assert a['status']=='PASS_ORIGINAL_CLOCK_CHANNEL_SOURCE_AUDIT'and not a['unused_MemC_half_clock_is_constant']
    for n,h in a['inputs'].items():
        p=BUILD/'r-front-control-reference-v2-20261023'/n.removeprefix('reference/')if n.startswith('reference/')else ROOT/n
        assert sha(p)==h,n
    stage=BUILD/'r-front-control-native-stage-v9-20261023';s=read(stage/'r-front-control-native.json');dirs.append(stage)
    assert s['pre_event_control_capture']and s['extended_component_directed']and not s['assembly_permitted']
    for n,h in s['source_hashes'].items():assert sha(stage/n)==h,n
    lint=BUILD/'r-front-control-native-structural-candidate-v9-20261023';l=read(lint/'run-state.json');dirs.append(lint)
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['structural_warnings_visible']
    log=(lint/'lint.log').read_text();assert log.count('%Warning-UNOPTFLAT:')==1 and log.count('%Warning-MULTIDRIVEN:')==1
    t=timing(fit);dirs.append(fit)
    # Preserve the exact prepared input copy as well as Quartus readback.
    dirs.append(BUILD/'r-front-control-old-capture-fit-v1-20261023')
    mask=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-control-tasking-mask-{engine}-v1-20261023';r,log=read_run(d);dirs.append(d)
        assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['masked_tasking_switch_clock']and r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
        assert sha(d/'inputs/tb_r_front_local.sv')==sha(BUILD/f'r-front-control-extended-{engine}-v4-20261023/inputs/tb_r_front_local.sv')
        mask.append(dict(directory=d.name,candidate_sha256=r['executed_candidate_sha256'],scope='Two balanced full comparisons only; broader timing/NoDispatch mask proof remains open'))
    assert mask[0]['candidate_sha256']==mask[1]['candidate_sha256']
    dirs += [BUILD/'r-front-control-tasking-mask-tool-snapshot-v1-20261023',BUILD/'r-front-control-tasking-mask-fit-v1-20261023']
    maskfit=BUILD/'r-front-control-tasking-mask-fit-terminal-v1-20261023';mt=timing(maskfit,True);dirs.append(maskfit)
    d=BUILD/'r-front-control-native-cold-v2-20261023';native=read(d/'run-state.json');dirs.append(d)
    assert native['status']=='TERMINAL_NATIVE844_COLD_PREFIX_REVIEW_REQUIRED'and native['returncode']==0
    log=(d/'test.log').read_text();assert 'NATIVE844_PREFIX samples=37059195 root_edges=9264799 CP_edges=211 no_guest_state_import=1'in log
    assert 'HOST_LIMIT host=250000000 guest=37059195'in log
    ip=json.loads(gzip.decompress((d/'provenance.json.gz').read_bytes()))
    for n,h in ip['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
    dirs.append(BUILD/'r-front-control-matrix-tools-frozen-v1-20261023')
    out.mkdir(parents=True)
    result=dict(status='PASS_BOUNDED_OLD_CONTROL_CAPTURE_AND_ALL_CLOCKS_TIMING_REJECTED',scope=__doc__,candidate_sha256=CANDIDATE,
        prior_candidate_sha256='a20b9e52134e671444e3e826ca907f3525cfad192944d04811171c099f67c32d',capture_change_inverse_byte_identical=True,
        original_counterexamples=cases,diagnostic_shared_bits=[40,41],primary=matrix['cases'],clock_source_audit_sha256=sha(auditdir/'results.json'),
        physical_Qs=844,RAM_bits=9216,clock_channels=100,unexercised_extended_clocks=[],
        legacy_baseline=dict(checks=10121,writes=242,reads=4166,epochs=391),extended=dict(checks=11300,writes=238,reads=4368,epochs=392),
        timing=t,masked_timing=mt,native_stage_sha256=sha(stage/'r-front-control-native.json'),native_elaboration_sha256=sha(lint/'run-state.json'),
        unpromoted_task_clock_mask=mask,old_native_prefix=dict(run=d.name,host_ticks=250000000,enabled_samples=37059195,root_edges=9264799,CP_edges=211,stage='v1',full_boot_qualified=False),
        original_early_RBase_overlap_still_unqualified=True,full_boot_qualified=False,ordinary60ns_qualified=False,hardware_speedup_claimed=False,
        next='Remove task/control event feedback with original capture/masking semantics and prove all source/clock cases; retain15ns budget, native ROM/timers/SDRAM and full A-F gates.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,list(dict.fromkeys(dirs))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--fit',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();result,dirs=review(a.output.resolve(),a.fit.resolve());print(result['status'])
    if a.archive:archive(a.archive.resolve(),a.output.resolve(),dirs,['review_r_front_control_extended.py','generate_r_front_control_candidate.py','audit_r_front_control_clocks.py','test_r_front_control_extended_matrix.py','fit_r_front_control.py','run_frozen_r_front_control.py'])
