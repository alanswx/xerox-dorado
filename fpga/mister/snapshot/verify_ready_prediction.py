#!/usr/bin/env python3
"""Rebuild the readiness prototype using only receipt-extracted proof sources.

Old qualified receipts remain immutable shared dependencies. No working HDL,
generated C++, object file or executable is used to rebuild these tests.
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
    readback=archive_receipts.readback(report)
    out.mkdir(parents=True,exist_ok=False)
    rows=json.loads(gzip.decompress((report/'archive-manifest.json.gz').read_bytes()))
    for row in rows:
        name=Path(row['raw_name']);assert not name.is_absolute()and'..'not in name.parts
        raw=(report/row['stored']).read_bytes()
        if row['stored']!=row['raw_name']:raw=gzip.decompress(raw)
        assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
        path=out/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    proof=out/'proof-source';frozen=json.loads((proof/'frozen-tools.json').read_text())
    for name,digest in frozen.items():assert hashlib.sha256((proof/name).read_bytes()).hexdigest()==digest,name
    (proof/'fpga/mister/reports').symlink_to(reports,target_is_directory=True)
    cases=[('fourstate','test_storage_ready_prediction.py',False),
        ('msa','test_msa_ready_prediction.py',True),('eight-msa','test_eight_msa_ready_prediction.py',True)]
    results=[]
    for label,driver,mutations in cases:
        destination=out/('rebuilt-'+label)
        cmd=['python3',str(proof/'fpga/mister/snapshot'/driver),'--output',str(destination)]
        if mutations:cmd+=['--mutations']
        with(out/(label+'-rebuild.log')).open('w')as log:
            subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        expected=json.loads((out/label/'results.json').read_text())
        actual=json.loads((destination/'results.json').read_text())
        # Four-state logs include simulator source locations, so compare the
        # actual RTL/fixture bytes and diagnostics while retaining both logs.
        if label=='fourstate':
            for result in(expected,actual):
                for case in result['cases']:case.pop('log_sha256')
        assert actual==expected,'rebuilt qualification differs: '+label
        results.append(dict(case=label,status='PASS',exact_results_and_inputs=True,
            rebuilt_results_sha256=hashlib.sha256((destination/'results.json').read_bytes()).hexdigest()))
        print('EXTRACTED REBUILD PASS '+label,flush=True)
    result=dict(status='PASS',archive_readback=readback,frozen_sources=len(frozen),cases=results,
        limits='Component prototype only; native integration, state access and routed timing remain required.')
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('report',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    a=p.parse_args();run(a.report.resolve(),a.output.resolve(),a.reports.resolve())
