#!/usr/bin/env python3
"""Stage a private native fit of h14/l14 data from stored control fields.

h14 owns RBase/task/NextMacroDly; l14 owns StkP[0:5]. Preserve the other
capture data, transparent windows, actual clocks, physical owners, memory,
native cadence and constraints. Bounded passing histories do not establish
an exhaustive old/new epoch invariant or authorize assembly.
"""
import argparse
import json
from pathlib import Path

from probe_r_front_pointer_capture import candidate, adapter
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def stage(parent,out,project):
    assert not out.exists() and project.is_absolute()
    original=json.loads((parent/'r-front-control-native.json').read_text())
    assert original['physical_Q_export_separate_from_closed_storage']
    assert original['closed_latch_encoding_bits']==88 and original['canonical_visible_R_owners']==16
    assert original['frequency_hz']==57142857 and original['sysper']==4 and not original['assembly_permitted']
    for n,h in original['source_hashes'].items():assert sha(parent/n)==h,n
    changed={};gates=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-pointer-capture-{engine}-v1-20261023'
        r=json.loads((d/'run-state.json').read_text())
        assert r['status']=='PASS_R_FRONT_POINTER_CAPTURE_PROBE' and not r.get('mutation')
        assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392' and r['actions'][-1]['returncode']==0
        assert r['original_reference_unchanged'] and r['all_checks_unchanged']
        for row in r['source_files']:
            p=d/row['path'];assert sha(p)==row['output_sha256']
            if row['changed']:
                if p.name in changed:assert sha(changed[p.name])==sha(p)
                changed[p.name]=p
        audit=BUILD/f'r-front-pointer-epoch-audit-{engine}-v1-20261023'
        a=json.loads((audit/'run-state.json').read_text())
        assert a['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and a['actions'][-1]['returncode']==0
        assert a['coverage'][:2]==[2181,2180]
        for mutation in ['zero_pointer','invert_pointer']:
            bad=BUILD/f'r-front-pointer-capture-{mutation}-{engine}-v1-20261023'
            b=json.loads((bad/'run-state.json').read_text())
            assert b['mutation']==mutation and b['status']=='R_FRONT_POINTER_CAPTURE_PROBE_FAILED'
            assert b['actions'][0]['returncode']==0 and b['actions'][-1]['returncode']!=0
            assert 'R front resolved source mismatch check=15' in (bad/'test.log').read_text()
        gates.append(dict(engine=engine,run=str(d),run_state_sha256=sha(d/'run-state.json'),
                          D_at_clock_run=str(audit),D_at_clock_state_sha256=sha(audit/'run-state.json'),
                          D_at_clock_coverage=a['coverage']))
    assert set(changed)=={'candidate.sv','dorado_local_r_front_ready_step.sv','dorado_local_r_front_step.sv'}
    data={}
    for n in original['source_hashes']:
        raw=(parent/n).read_bytes()
        if Path(n).suffix in {'.qsf','.qip','.qpf','.sdc','.tcl'}:
            raw=raw.replace(original['project'].encode(),str(project).encode())
        data[n]=raw
    for name,p in changed.items():
        target='src/verilog/rtl/'+('dorado_r_front_control_step.sv' if name=='candidate.sv' else name)
        old=(parent/target).read_text()
        expected=candidate(old) if name=='candidate.sv' else adapter(old,name,None)
        assert p.read_text()==expected,(target,'Unexpected change from staged flag-window parent')
        data[target]=p.read_bytes()
    out.mkdir(parents=True)
    for n,raw in data.items():
        p=out/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    r=dict(original)
    r.update(scope=__doc__,project=str(project),source_hashes={n:sha(out/n) for n in data},
             parent_stage=str(parent),parent_manifest_sha256=sha(parent/'r-front-control-native.json'),
             pointer_capture_epoch_gates=gates,pointer_capture_D_bits=12,
             exhaustive_epoch_invariant_qualified=False,profile_matrix_qualified=False,
             physical_timing_qualified=False,native_consumer_setup_hold_qualified=False,
             firmware_qualified=False,assembly_permitted=False,speedup_qualified=False)
    (out/'r-front-control-native.json').write_text(json.dumps(r,indent=2)+'\n')
    print('STAGED_POINTER_CAPTURE_NATIVE_EXPERIMENT',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();stage(a.parent.resolve(),a.output.resolve(),a.project)
