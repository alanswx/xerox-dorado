#!/usr/bin/env python3
"""Verify the matched native image handoff and report diagnostic limits."""
import argparse
import gzip
import hashlib
import itertools
import json
from pathlib import Path
import re
import struct


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def inspect(control,candidate,q_probe):
    records=[];states=[];logs=[]
    for directory in [control,candidate]:
        record=json.loads(gzip.decompress((directory/'provenance.json.gz').read_bytes()))
        for name,digest in record['inputs'].items():assert sha(directory/'executed-inputs'/name)==digest,name
        records.append(record);states.append(json.loads((directory/'run-state.json').read_text()))
        logs.append((directory/'test.log').read_text())
    for key in ['external_inputs','native_io_inputs','native_memory_inputs','native_ifu_inputs']:
        assert records[0][key]==records[1][key],key
    assert records[0]['native_cpu_baseline_inputs'] and not records[1]['native_cpu_baseline_inputs']
    assert all(r['handoff_only']and r['handoff_trace']and not r['probe_only']for r in records)
    assert states[0]['actions'][-1]['returncode']==0
    assert 'LOADED EB PASS: 3600 original IM and 256 IFUM' in logs[0]
    assert states[1]['actions'][-1]['returncode']==1
    assert 'bad=3856 entry=00000238 reached=0' in logs[1]
    assert all('INITIAL ROM PASS:'in log for log in logs)
    reads=[[l for l in log.splitlines()if l.startswith('BOOT_DIAG_RX_CPU ')]for log in logs]
    assert len(reads[0])==len(reads[1])==640 and reads[0]==reads[1]
    common=records[0]['inputs'].keys()&records[1]['inputs'].keys()
    differences=[name for name in sorted(common)if records[0]['inputs'][name]!=records[1]['inputs'][name]]
    task_samples={task:int(count)for task,count in re.findall(r'BOOT_DIAG_TASK_SAMPLES task=(\w+) samples=(\d+)',logs[0])}
    assert task_samples['0']==2153 and task_samples['c']==5431273
    assert all(count==0 for task,count in task_samples.items()if task not in('0','c'))
    qlog=(q_probe/'test.log').read_text()
    checks={pkg:int(count)for pkg,count in re.findall(r'NATIVE_Q_CAPTURE_CHECK (\S+) checks=(\d+)',qlog)}
    assert len(checks)==91
    qstate=json.loads((q_probe/'run-state.json').read_text())
    assert qstate['status']=='TERMINAL_FAILED'and qstate['actions'][-1]['returncode']==0
    assert 'NATIVE_Q_CAPTURE_MISMATCH'not in qlog
    eb=Path(__file__).resolve().parents[1]/'chm/dorado/CedarDorado.eb!6'
    expected=list(struct.unpack('>15688H',eb.read_bytes()[512:]))
    early=[int(v,16)for v in re.findall(r'CIA=d5e .*task=0 .*alu=(\w+)',qlog)]
    final=re.findall(r'CIA=d5f .*task=0 .*alu=(\w+)',qlog)
    assert len(early)==15688 and early[0]==0x200 and len(final)==1
    sums=early[1:]+[int(final[0],16)]
    deltas=[(v-(sums[i-1]if i else 0))&65535 for i,v in enumerate(sums)]
    bad=[i for i,(a,b)in enumerate(zip(deltas,expected))if a!=b];suspect=[]
    for _,group in itertools.groupby(enumerate(bad),lambda p:p[1]-p[0]):
        indices=[v for _,v in group];first,last=indices[0],indices[-1]
        difference=(sum(deltas[first:last+1])-sum(expected[first:last+1]))&65535
        if difference:suspect.append(dict(VM_word=hex(first+512),length=last-first+1,sum16_difference=hex(difference)))
    return dict(scope=__doc__,status='NATIVE_BASELINE_IMAGE_PASS_CANDIDATE_HANDOFF_FAILURE',
        baseline_IM_words_verified=3600,baseline_IFUM_entries_verified=256,
        candidate_IM_IFUM_mismatches=3856,candidate_Cedar_entry_reached=False,
        shared_IO_memory_IFU_sources_verified=True,first_CPU_receive_records_identical=640,
        changed_common_frozen_inputs=differences,baseline_post_handoff_task_samples=task_samples,
        Q_probe=dict(status=qstate['status'],sampled_packages=sum(v>0 for v in checks.values()),
            unobserved_packages=[p for p,v in checks.items()if not v],
            mismatch_observed=False,equivalence_qualified=False),
        PCT_checksum=dict(final_early_ALU=final[0],suspect_ranges=suspect,
            consumed_MD_verified=False,warning='CIA-change operands are unsettled; use this to place a phase trace'),
        hardware_cause_established=False,desktop_qualified=False,speedup_qualified=False,
        inputs={str(path):sha(path)for path in[control/'provenance.json.gz',control/'run-state.json',control/'test.log',
            candidate/'provenance.json.gz',candidate/'run-state.json',candidate/'test.log',q_probe/'run-state.json',q_probe/'test.log',eb,Path(__file__)]})


def repaired_handoff(control,candidate,native=None):
    """Require the repaired component to match the passing native oracle."""
    records=[];logs=[];evidence={}
    for directory in[control,candidate]:
        r=json.loads(gzip.decompress((directory/'provenance.json.gz').read_bytes()))
        for name,digest in r['inputs'].items():assert sha(directory/'executed-inputs'/name)==digest,name
        s=json.loads((directory/'run-state.json').read_text())
        assert s['status']=='TERMINAL_PILOT_COMPONENT_DIAGNOSTIC'
        assert all(a['returncode']==0 for a in s['actions'])
        log=(directory/'test.log').read_text()
        assert 'LOADED EB PASS: 3600 original IM and 256 IFUM'in log
        assert 'entry 238 executed'in log
        assert '960 microinstructions written, 960 read back, 0 wrong'in log
        match=re.search(r'all six error sources enabled for (\d+) samples without error',log)
        assert match and int(match[1])>=100000
        assert r['handoff_only']and r['handoff_trace']and not r['probe_only']
        records.append(r);logs.append(log)
        for name in['provenance.json.gz','run-state.json','test.log','register-map-inspection.txt']:
            evidence[str(directory/name)]=sha(directory/name)
    for key in['external_inputs','native_io_inputs','native_memory_inputs','native_ifu_inputs']:
        assert records[0][key]==records[1][key],key
    assert records[0]['native_cpu_baseline_inputs']and not records[1]['native_cpu_baseline_inputs']
    assert records[1]['native_visible_r_sources']
    reads=[[l for l in log.splitlines()if l.startswith('BOOT_DIAG_RX_CPU ')]for log in logs]
    assert len(reads[0])==len(reads[1])==640 and reads[0]==reads[1]
    tasks=[{task:int(count)for task,count in re.findall(r'BOOT_DIAG_TASK_SAMPLES task=(\w+) samples=(\d+)',log)}for log in logs]
    assert len(tasks[0])==len(tasks[1])==16 and tasks[0]==tasks[1]
    state_dumps=[(d/'register-map-inspection.txt').read_bytes()for d in[control,candidate]]
    assert state_dumps[0]==state_dumps[1],'Post-handoff native RM/map state differs'
    if native:
        stage=json.loads((native/'r-front-ready-native.json').read_text())
        for n,h in stage['source_hashes'].items():assert sha(native/n)==h,n
        assert records[1]['canonical_native_r']and stage['native_visible_R_bits']==16
        for board in['ContA','ProcH','ProcL']:
            n='fpga/mister/'+('snapshot_native_conta'if board=='ContA'else'snapshot_native_processor')+'/'+board+'_full_snapshot.sv'
            assert records[1]['inputs'][n]==stage['source_hashes']['src/'+n],board
        for n,h in stage['source_hashes'].items():
            if n.startswith('src/verilog/rtl/'):
                assert records[1]['inputs'][n[4:]]==h,n
        bp=(native/'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v').read_text()
        logic=bp.split('  // Physical state only;',1)[1].split('endmodule',1)[0]
        # A passive output append can introduce an extra blank separator in
        # the generated fixture. Require every nonblank wiring line verbatim;
        # CPU/evaluator source bytes above remain an exact comparison.
        wiring=lambda text:'\n'.join(line for line in text.splitlines()if line.strip())
        assert wiring(logic)in wiring((candidate/'executed-inputs/verilog/generated/dorado_full.v').read_text())
        evidence[str(native/'r-front-ready-native.json')]=sha(native/'r-front-ready-native.json')
    return dict(status='PASS_REPAIRED_NATIVE_COMPONENT_IMAGE_HANDOFF',
        original_IM_words_verified=3600,original_IFUM_entries_verified=256,Cedar_entry_executed=True,
        identical_CPU_receive_records=640,post_handoff_task_samples=tasks[1],
        final_RM_map_dump_identical=True,canonical_native_R=records[1].get('canonical_native_r',False),
        exact_staged_native_sources_verified=native is not None,
        desktop_qualified=False,hardware_qualified=False,speedup_qualified=False,
        limitation='Direct-ROM/raw-storage component; same post-entry task12 wait as native oracle, no germ/desktop qualification',
        inputs=evidence,analyzer_sha256=sha(Path(__file__)))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in['control','candidate','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--q-probe',type=Path)
    p.add_argument('--expect-repaired',action='store_true')
    p.add_argument('--native',type=Path)
    a=p.parse_args()
    if a.expect_repaired:
        assert a.q_probe is None
        result=repaired_handoff(a.control,a.candidate,a.native)
    else:
        assert a.q_probe and a.native is None
        result=inspect(a.control,a.candidate,a.q_probe)
    with a.output.open('x')as stream:json.dump(result,stream,indent=2);stream.write('\n')
    if a.expect_repaired:print(result['status'],'component only; no hardware/desktop/speed qualification')
    else:print(result['status'],'Q coverage',result['Q_probe']['sampled_packages'],'of91; no equivalence claim')
