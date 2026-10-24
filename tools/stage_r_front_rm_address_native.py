#!/usr/bin/env python3
"""Stage the bounded old-RBase low RM address capture experiment.

Change only four data inputs to g10/h10 capture, retaining current clocks,
other address inputs, transparent windows, all state/RAM and native cadence.
Capture-pin comparisons have no distinct old/new epoch samples; keep that
coverage gap explicit and prohibit assembly or native selection.
"""
import argparse
import json
from pathlib import Path

from probe_r_front_rm_address_epoch import address
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def stage(parent,out,project):
    assert not out.exists() and project.is_absolute()
    original=json.loads((parent/'r-front-control-native.json').read_text())
    assert original['pointer_capture_D_bits']==12 and original['canonical_visible_R_owners']==16
    assert original['closed_latch_encoding_bits']==88 and not original['assembly_permitted']
    assert original['frequency_hz']==57142857 and original['sysper']==4
    for n,h in original['source_hashes'].items():assert sha(parent/n)==h,n
    selected=None;gates=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-rm-address-{engine}-v1-20261023';r=json.loads((d/'run-state.json').read_text())
        assert r['status']=='PASS_R_FRONT_RM_ADDRESS_EPOCH_PROBE' and not r['mutation']
        assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392' and r['actions'][-1]['returncode']==0
        assert r['original_reference_and_checks_unchanged'] and r['only_four_RBase_capture_inputs_changed']
        changed=[]
        for row in r['source_files']:
            p=d/row['path'];assert sha(p)==row['output_sha256']
            if row['changed']:changed.append(p)
        assert len(changed)==1 and changed[0].name=='dorado_local_r_address_step.sv'
        if selected:assert sha(selected)==sha(changed[0])
        selected=changed[0]
        bad=BUILD/f'r-front-rm-address-zero-{engine}-v1-20261023';b=json.loads((bad/'run-state.json').read_text())
        assert b['status']=='R_FRONT_RM_ADDRESS_EPOCH_PROBE_FAILED' and b['actions'][0]['returncode']==0 and b['actions'][-1]['returncode']!=0
        assert 'R front resolved source mismatch check=1434' in (bad/'test.log').read_text()
        audit=BUILD/f'r-front-rm-address-pin-audit-{engine}-v1-20261023';a=json.loads((audit/'run-state.json').read_text())
        assert a['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and a['RM_capture_coverage']==[4393,0]
        assert a['low_RM_address_D_at_clock_checked'] and a['actions'][-1]['returncode']==0
        gates.append(dict(engine=engine,run=str(d),run_state_sha256=sha(d/'run-state.json'),
                          pin_audit=str(audit),pin_audit_state_sha256=sha(audit/'run-state.json'),
                          actual_clock_edges=a['RM_capture_coverage'][0],distinct_epoch_samples=0))
    target='src/verilog/rtl/dorado_local_r_address_step.sv'
    assert selected.read_text()==address((parent/target).read_text())
    data={}
    for n in original['source_hashes']:
        raw=(parent/n).read_bytes()
        if Path(n).suffix in {'.qsf','.qip','.qpf','.sdc','.tcl'}:raw=raw.replace(original['project'].encode(),str(project).encode())
        data[n]=raw
    data[target]=selected.read_bytes();out.mkdir(parents=True)
    for n,raw in data.items():
        p=out/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    r=dict(original)
    r.update(scope=__doc__,project=str(project),source_hashes={n:sha(out/n) for n in data},parent_stage=str(parent),
             parent_manifest_sha256=sha(parent/'r-front-control-native.json'),low_RM_RBase_capture_gates=gates,
             low_RM_RBase_capture_data_bits=4,distinct_RBase_capture_epoch_samples=0,profile_matrix_qualified=False,
             exhaustive_epoch_invariant_qualified=False,physical_timing_qualified=False,native_consumer_setup_hold_qualified=False,
             firmware_qualified=False,assembly_permitted=False,speedup_qualified=False)
    (out/'r-front-control-native.json').write_text(json.dumps(r,indent=2)+'\n');print('STAGED_RM_ADDRESS_NATIVE_EXPERIMENT',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();stage(a.parent.resolve(),a.output.resolve(),a.project)
