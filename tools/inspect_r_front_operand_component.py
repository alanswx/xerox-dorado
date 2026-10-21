#!/usr/bin/env python3
"""Verify original-image/map gates and the output-only operand observer."""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

from inspect_r_front_native_handoff import repaired_handoff
from inspect_r_front_map_capture import inspect as inspect_map
from stage_r_front_operand_monitor import verify


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def negative_readout(positive,negative):
    records=[]
    for path in [positive,negative]:
        r=json.loads(gzip.decompress((path/'provenance.json.gz').read_bytes()))
        for n,h in r['inputs'].items():assert sha(path/'executed-inputs'/n)==h,n
        records.append(r)
    state=json.loads((negative/'run-state.json').read_text())
    assert state['status']=='TERMINAL_FAILED'and len(state['actions'])==2
    assert [a['returncode']for a in state['actions']]==[0,1]
    text=(negative/'test.log').read_text()
    assert 'OPERAND_MONITOR_PIN_ALIAS_MISMATCH'in text
    assert 'loaded-eb: 'not in text,'Negative failed for a guest image error'
    changed={n for n in records[0]['inputs']if records[0]['inputs'][n]!=records[1]['inputs'][n]}
    name='fpga/mister/snapshot_native_processor/ProcL_full_snapshot.sv'
    assert changed=={name}and records[0]['inputs'].keys()==records[1]['inputs'].keys()
    good=(positive/'executed-inputs'/name).read_text()
    marker='  assign operand_debug=';before,tail=good.split(marker)
    T='{'+','.join(f'T_{i:02d}'for i in range(8,16))+'}'
    MD='{'+','.join(f'Md_{i:02d}'for i in range(8,16))+'}'
    bad=tail.replace(T,'SWAP_TRACE_ONLY').replace(MD,T).replace('SWAP_TRACE_ONLY',MD)
    assert before+marker+bad==(negative/'executed-inputs'/name).read_text()
    return dict(status='PASS_SWAPPED_READOUT_REJECTED',changed_inputs=[name],
                core_logic_changed=False,diagnostic='OPERAND_MONITOR_PIN_ALIAS_MISMATCH',
                provenance_sha256=sha(negative/'provenance.json.gz'),log_sha256=sha(negative/'test.log'))


def inspect(parent,native,image,image_control,map_run,map_control,negative,tested_native=None):
    source=verify(parent,native)
    result=repaired_handoff(image_control,image,native)
    mapped=inspect_map(map_control,map_run)
    assert mapped['RM_mismatches']==mapped['map_row_mismatches']==0
    assert mapped['map_rows_compared']==16384 and mapped['RM_words_compared']==256
    phase=mapped['phase_trace'];assert phase['aligned_rows']==6000
    assert all(phase['differences'][n]==0 for n in ['R','B','BMux','Mapbuf','written'])
    checks={}
    tested_manifest=sha(native/'r-front-ready-native.json')
    if tested_native:
        old=json.loads((tested_native/'r-front-ready-native.json').read_text())
        current=json.loads((native/'r-front-ready-native.json').read_text())
        assert old['source_hashes'].keys()==current['source_hashes'].keys()
        differences=[]
        for name,key in old['source_hashes'].items():
            raw=(tested_native/name).read_bytes();assert hashlib.sha256(raw).hexdigest()==key
            if Path(name).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
                raw=raw.replace(old['project'].encode(),current['project'].encode())
            if raw!=(native/name).read_bytes():differences.append(name)
        # Correcting the compiled emu file does not alter any board/evaluator
        # or fixture source. Keep that narrower reuse explicit in the proof.
        assert set(differences)=={'core/Dorado.sv','src/fpga/mister/Dorado.sv'}
        tested_manifest=sha(tested_native/'r-front-ready-native.json')
    for path in [image,map_run]:
        state=json.loads((path/'run-state.json').read_text())
        assert state['status'].startswith('TERMINAL_')and all(a['returncode']==0 for a in state['actions'])
        r=json.loads(gzip.decompress((path/'provenance.json.gz').read_bytes()))
        assert r['operand_monitor_native_manifest_sha256']==tested_manifest
        values=re.findall(r'OPERAND_MONITOR_PIN_ALIAS_PASS checks=(\d+)',(path/'test.log').read_text())
        assert len(values)==1 and int(values[0])>100000
        checks[path.name]=int(values[0])
    bad=negative_readout(image,negative)
    result.update(passive_operand_monitor=True,operand_source_review=source,
        operand_pin_alias_checks=checks,operand_readout_negative=bad,
        original_initial_map_phase_review=mapped)
    if tested_native:
        result['operand_port_test_reuse']=dict(tested_manifest_sha256=tested_manifest,
            unchanged_board_evaluator_wrapper_and_constraint_sources=True,
            differing_files=sorted(differences),
            scope='Existing exact board/RTL/pin tests; corrected emu readout needs separate compiled-source and hardware marker checks')
    for directory,names in [(map_run,['provenance.json.gz','run-state.json','test.log','register-map-inspection.txt']),
                            (negative,['provenance.json.gz','run-state.json','test.log'])]:
        result['inputs'].update({str(directory/n):sha(directory/n)for n in names})
    result['observer_analyzer_sha256']=sha(Path(__file__))
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','native','image','image-control','map-run','map-control','negative','output']:
        p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--tested-native',type=Path,help='Explicit reuse when only the compiled/archive emu copies changed outside the tested board fixture')
    a=p.parse_args()
    r=inspect(a.parent.resolve(),a.native.resolve(),a.image.resolve(),a.image_control.resolve(),a.map_run.resolve(),a.map_control.resolve(),a.negative.resolve(),a.tested_native.resolve()if a.tested_native else None)
    with a.output.open('x')as f:json.dump(r,f,indent=2);f.write('\n')
    print(r['status'],'source/alias/original map+image gates; not a native desktop')
