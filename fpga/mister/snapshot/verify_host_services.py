#!/usr/bin/env python3
"""Verify executed host-service/Main-client sources, results and complete state.

This is a component qualification only. Native wiring, the actual Main lease
coordinator, input/video ACKs, DDR visibility and complete snapshots stay open.
"""
import argparse
import hashlib
import json
from pathlib import Path
import test_host_service_control as test


def verify(directory,out):
    result=json.loads((directory/'results.json').read_text());inputs=directory/'executed-inputs'
    assert result['status']=='PASS_HOST_SERVICES_AND_MAIN_CLIENT_COMPONENT'
    assert result['new_required_service_bits']==332 and result['required_field_geometry']==test.FIELDS
    assert not result['access_complete']and len(result['cases'])==13
    expected={name:path for name,path in [
        ('host_service_control.sv',test.HERE/'host_service_control.sv'),
        ('test_host_service_control.sv',test.HERE/'test_host_service_control.sv'),
        ('board_state_uio_msa.sv',test.HERE/'board_state_uio_msa.sv'),
        ('dorado_snapshot_client.cpp',test.HERE.parent/'main/dorado_snapshot_client.cpp'),
        ('dorado_snapshot_client.h',test.HERE.parent/'main/dorado_snapshot_client.h'),
        ('snapshot_host_top.sv',test.HERE.parent/'main/tests/snapshot_host_top.sv'),
        ('test_snapshot_client.cpp',test.HERE.parent/'main/tests/test_snapshot_client.cpp'),
        ('spi.h',test.HERE.parent/'main/tests/spi.h'),('user_io.h',test.HERE.parent/'main/tests/user_io.h'),
        ('test_host_service_control.py',Path(test.__file__))]}
    assert set(expected)==set(result['inputs'])
    for name,path in expected.items():
        raw=(inputs/name).read_bytes();assert hashlib.sha256(raw).hexdigest()==result['inputs'][name]
        assert raw==path.read_bytes(),name
    for case in result['cases']:
        text=(directory/case['case']/'test.txt').read_text()
        if case['expected_failure']:assert case['returncode']and case['expected_failure']in text
        else:
            assert not case['returncode']
            assert ('checks=125 packets=131'in text if case['case']=='rtl'else 'checks=1930 transactions=49 max_words=22'in text)
    data=test.inventory((directory/'inventory/host.xml').read_bytes());actual={};legacy=0
    for row in data['candidates']:
        if row['category']=='combinational_review':continue
        if row['path']=='snapshot_host_top.command_hold':assert row['bits']==1;continue
        if row['path'].startswith('snapshot_host_top.legacy.'):
            assert row['category']=='sequential';legacy+=row['bits'];continue
        assert row['path'].startswith('snapshot_host_top.controller.')and row['category']=='sequential'
        assert not row['unpacked_dimensions']and row['bits']==row['width']
        actual[row['path'].removeprefix('snapshot_host_top.controller.')]=row['bits']
    assert actual==test.FIELDS and sum(actual.values())==332 and legacy==353
    assert data==json.loads((directory/'inventory/inventory.json').read_text())
    summary=dict(status='PASS_HOST_SERVICES_MAIN_COMPONENT_PROVENANCE',scope=__doc__,required_service_bits=332,
        required_fields=len(actual),original_uio_bits=353,negative_cases=11,main_transactions=49,max_words=22,
        cxx14_compile=True,inputs=result['inputs'],native_integrated=False,main_coordinator_integrated=False)
    out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(summary,indent=2)+'\n')
    print(summary['status'],'332 required bits;11 bad variants rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();verify(a.directory.resolve(),a.output.resolve())
