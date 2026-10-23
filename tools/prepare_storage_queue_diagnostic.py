#!/usr/bin/env python3
"""Prepare a full-width native diagnostic differing only in storage transport.

Clone an immutable qualified5c65 diagnostic, retaining ROMs, clock defines,
media, boot/input/device stimulus and passive observers. Bind the tested queue
wrapper/backend and added queue source. This is simulation preparation, not
native fit, timing, firmware or performance qualification.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil

from prepare_storage_queue_native import transform,WRAPPER

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(control,generated,output):
    assert not output.exists(),'Observe existing diagnostic'
    r=json.loads(gzip.decompress((control/'provenance.json.gz').read_bytes()))
    assert r['stage_manifest_name']=='timing-paths-native.json'
    assert r['passive_storage_traffic_observer'] and r['host_clock_hz']==57142857
    assert r['normal_baseboard_boot_presses']==3
    for n,h in r['inputs'].items():assert sha(control/'inputs'/n)==h,n
    p=json.loads((generated/'provenance.json').read_text())
    assert p['parent_qualification_sha256']==r['qualification_sha256']
    for n,h in p['generated_hashes'].items():assert sha(generated/n)==h,n
    for n,h in p['parent_source_hashes'].items():assert r['inputs'][n]==h,n
    assert (generated/'dorado_native_machine.sv').read_bytes()==transform((control/'inputs'/WRAPPER).read_bytes())
    receipt=ROOT/'fpga/mister/reports/2026-10-22-storage-row-writes/component/manifest.json.gz'
    evidence=json.loads(gzip.decompress(receipt.read_bytes()))
    assert evidence['pipelined_row_writes_component_qualified']
    assert evidence['native_reset_init_bounded_address_qualified']
    for prefix,names in [
        ('prepared-native-glue/',['dorado_native_machine.sv','dorado_storage_queue.sv','dorado_storage_selftest.sv','dorado_storage_sdram_burst.sv']),
    ]:
        for n in names:
            rows=[m for m in evidence['members']if m['name']==prefix+n]
            assert len(rows)==1 and rows[0]['blob']==sha(generated/n),n
    output.mkdir(parents=True);shutil.copytree(control/'inputs',output/'inputs')
    original=dict(r['inputs'])
    for local,n in [
        ('dorado_native_machine.sv',WRAPPER),
        ('dorado_storage_sdram_burst.sv','src/fpga/mister/dorado_storage_sdram_burst.sv'),
        ('dorado_storage_queue.sv','src/fpga/mister/dorado_storage_queue.sv'),
    ]:
        shutil.copyfile(generated/local,output/'inputs'/n);r['inputs'][n]=sha(output/'inputs'/n)
    queue='src/fpga/mister/dorado_storage_queue.sv'
    assert queue not in r['used_hdl'];r['used_hdl'].append(queue)
    changed=sorted(n for n in r['inputs']if r['inputs'][n]!=original.get(n))
    assert changed==sorted([WRAPPER,queue,'src/fpga/mister/dorado_storage_sdram_burst.sv'])
    name='prepare_storage_queue_diagnostic.py';shutil.copyfile(Path(__file__),output/'inputs'/name)
    r['inputs'][name]=sha(output/'inputs'/name)
    shutil.copyfile(control/'host-cycle-limit.txt',output/'host-cycle-limit.txt')
    if r.get('passive_storage_window_observer'):
        shutil.copyfile(control/'window-control.txt',output/'window-control.txt')
        r['window_control_path']=str(output/'window-control.txt')
    r.update(status='PREPARED_FULL_NATIVE_QUEUE_TRANSPORT_DIAGNOSTIC',scope=__doc__,
        control=str(control),control_provenance_sha256=sha(control/'provenance.json.gz'),
        transport_provenance_sha256=sha(generated/'provenance.json'),
        transport_component_receipt_sha256=sha(receipt),modified_or_added_native_sources=changed,
        used_hdl_from_qualified_control_plus_queue=True,
        native_group_count=128,native_group_address_bits=16,native_raw_address_bits=23,
        native_fitted_timing_firmware_or_speed_qualified=False)
    (output/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    print(r['status'],len(r['used_hdl']),'HDL sources; three transport source changes')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['control','generated','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.control.resolve(),a.generated.resolve(),a.output.resolve())
