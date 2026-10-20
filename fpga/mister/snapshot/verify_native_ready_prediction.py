#!/usr/bin/env python3
"""Rebuild Serial73 native sources, inverse review and fresh XML from receipts.

Immutable older qualification receipts are shared dependencies. Compare all
662 actual staged files and complete state decisions; only XML path-dependent
hashes differ. No Quartus run or generated build executable is invoked.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import subprocess
import archive_receipts
from generate_serial_boards import ROOT


def run(report,out,reports):
    checked=archive_receipts.readback(report);out.mkdir(parents=True,exist_ok=False)
    rows=json.loads(gzip.decompress((report/'archive-manifest.json.gz').read_bytes()))
    for row in rows:
        name=Path(row['raw_name']);assert not name.is_absolute()and'..'not in name.parts
        raw=(report/row['stored']).read_bytes()
        if row['stored']!=row['raw_name']:raw=gzip.decompress(raw)
        assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    proof=out/'proof-source';frozen=json.loads((proof/'frozen-tools.json').read_text())
    for name,digest in frozen.items():assert hashlib.sha256((proof/name).read_bytes()).hexdigest()==digest,name
    (proof/'fpga/mister/reports').symlink_to(reports,target_is_directory=True)
    scripts=proof/'fpga/mister/snapshot';expected=out/'staged-source';record=json.loads((expected/'ready-prediction-integration.json').read_text())
    rebuilt=out/'rebuilt-source';inventory=out/'rebuilt-inventory'
    commands=[['python3',str(scripts/'build_native_ready_prediction.py'),'--stage',str(rebuilt),'--destination',record['project']],
        ['python3',str(scripts/'audit_native_ready_prediction.py'),'--project',str(rebuilt),'--output',str(out/'rebuilt-source-audit.json')],
        ['python3',str(scripts/'inventory_native_ready_prediction.py'),str(rebuilt),'--output',str(inventory)]]
    (out/'rebuild-commands.json').write_text(json.dumps(commands,indent=2)+'\n')
    for i,cmd in enumerate(commands):
        with(out/('rebuild-'+str(i)+'.log')).open('w')as log:
            subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
    actual=json.loads((rebuilt/'ready-prediction-integration.json').read_text());assert actual==record,'staging metadata differs'
    for name in record['source_hashes']:assert(expected/name).read_bytes()==(rebuilt/name).read_bytes(),name
    assert json.loads((expected/'source-audit.json').read_text())==json.loads((out/'rebuilt-source-audit.json').read_text()),'source audit differs'
    before=json.loads((out/'inventory/classification.json').read_text());after=json.loads((inventory/'classification.json').read_text())
    for row in(before,after):
        row.pop('xml_sha256');row.pop('normalized_xml_sha256')
    assert before==after,'complete native state classification differs'
    result=dict(status='PASS',scope=__doc__,archive_readback=checked,frozen_sources=len(frozen),
        actual_rebuilt_file_bytes=len(record['source_hashes']),source_audit_exact=True,classification_exact_except_xml_path_hashes=True,
        required_bits=sum(after['totals'].values()),new_required_wrapper_bits=8,
        limits='Source and fresh native XML only; actual Quartus capacity/timing, state access and FPGA qualification remain separate.')
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('report',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');a=p.parse_args()
    run(a.report.resolve(),a.output.resolve(),a.reports.resolve())
