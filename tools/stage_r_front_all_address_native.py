#!/usr/bin/env python3
"""Stage shared pre-control data at every R-address edge capture.

Keep actual clock functions, h15 transparent data, functional RAM ports,
physical owners, barrier and native cadence. Exact package pin checks cover
the bounded histories; zero distinct epoch samples is not an exhaustive proof.
No assembly, native selection, firmware or speed qualification.
"""
import argparse
import json
from pathlib import Path

from probe_r_front_all_address_capture import adapter
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def stage(parent,out,project):
    assert not out.exists() and project.is_absolute()
    original=json.loads((parent/'r-front-control-native.json').read_text())
    assert original['low_RM_RBase_capture_data_bits']==4 and original['canonical_visible_R_owners']==16
    assert original['closed_latch_encoding_bits']==88 and not original['assembly_permitted']
    assert original['frequency_hz']==57142857 and original['sysper']==4
    for n,h in original['source_hashes'].items():assert sha(parent/n)==h,n
    selected={};gates=[]
    expected=[('H_l05',2180),('L_g10_h10',4393),('L_i15',2180),('L_k11',4393),
              ('L_g08',4393),('L_j11',4393),('L_l11_A',1616),('L_l11_B',32)]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-all-address-capture-{engine}-v1-20261023';r=json.loads((d/'run-state.json').read_text())
        assert r['status']=='PASS_R_FRONT_ALL_ADDRESS_CAPTURE' and not r['mutation']
        assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392' and r['actions'][-1]['returncode']==0
        assert r['original_reference_and_checks_unchanged'] and r['actual_clock_functions_unchanged'] and r['RAM_ports_unchanged']
        for row in r['source_files']:
            p=d/row['path'];assert sha(p)==row['output_sha256']
            if row['changed']:
                if p.name in selected:assert sha(selected[p.name])==sha(p)
                selected[p.name]=p
        bad=BUILD/f'r-front-all-address-invert-bank-{engine}-v1-20261023';b=json.loads((bad/'run-state.json').read_text())
        assert b['status']=='R_FRONT_ALL_ADDRESS_CAPTURE_FAILED' and b['actions'][0]['returncode']==0 and b['actions'][-1]['returncode']!=0
        assert 'R front output mismatch check=15' in (bad/'test.log').read_text()
        audit=BUILD/f'r-front-all-address-pin-audit-{engine}-v1-20261023';a=json.loads((audit/'run-state.json').read_text())
        assert a['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and a['all_R_address_D_at_clock_checked']
        assert [(x['package'],x['edges']) for x in a['all_address_pin_coverage']]==expected
        assert all(x['distinct']==0 for x in a['all_address_pin_coverage']) and a['actions'][-1]['returncode']==0
        gates.append(dict(engine=engine,run=str(d),run_state_sha256=sha(d/'run-state.json'),pin_audit=str(audit),
                          pin_audit_state_sha256=sha(audit/'run-state.json'),pin_coverage=a['all_address_pin_coverage']))
    assert set(selected)=={'dorado_local_r_front_step.sv','dorado_local_r_memory_step.sv','dorado_local_r_address_step.sv'}
    data={}
    for n in original['source_hashes']:
        raw=(parent/n).read_bytes()
        if Path(n).suffix in {'.qsf','.qip','.qpf','.sdc','.tcl'}:raw=raw.replace(original['project'].encode(),str(project).encode())
        data[n]=raw
    for name,p in selected.items():
        target='src/verilog/rtl/'+name
        assert p.read_text()==adapter((parent/target).read_text(),name)
        data[target]=p.read_bytes()
    out.mkdir(parents=True)
    for n,raw in data.items():
        p=out/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    r=dict(original)
    r.update(scope=__doc__,project=str(project),source_hashes={n:sha(out/n) for n in data},parent_stage=str(parent),
             parent_manifest_sha256=sha(parent/'r-front-control-native.json'),all_R_address_capture_gates=gates,
             all_R_address_edge_D_pre_control=True,R_address_edge_D_bits=36,distinct_capture_epoch_samples=0,
             profile_matrix_qualified=False,exhaustive_epoch_invariant_qualified=False,physical_timing_qualified=False,
             native_consumer_setup_hold_qualified=False,firmware_qualified=False,assembly_permitted=False,speedup_qualified=False)
    (out/'r-front-control-native.json').write_text(json.dumps(r,indent=2)+'\n');print('STAGED_ALL_ADDRESS_NATIVE_EXPERIMENT',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();stage(a.parent.resolve(),a.output.resolve(),a.project)
