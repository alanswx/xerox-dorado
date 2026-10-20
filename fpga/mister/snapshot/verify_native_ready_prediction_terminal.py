#!/usr/bin/env python3
"""Replay Serial73 terminal rejection using extracted sources and raw reports.

Re-stage all actual input bytes, run the independent inverse audit, elaborate
fresh actual-QSF native XML and repeat capacity, all-device/crossing and
registered-readiness reviews with their corruption controls. No Quartus run.
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
    for row in json.loads(gzip.decompress((report/'archive-manifest.json.gz').read_bytes())):
        name=Path(row['raw_name']);assert not name.is_absolute()and'..'not in name.parts
        raw=(report/row['stored']).read_bytes()
        if row['stored']!=row['raw_name']:raw=gzip.decompress(raw)
        assert len(raw)==row['raw_bytes']and hashlib.sha256(raw).hexdigest()==row['raw_sha256']
        path=out/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    qual=out/'qualification';proof=qual/'proof';tools=qual/'review-tools'
    frozen=json.loads((tools/'frozen-tools.json').read_text())
    for name,digest in frozen.items():assert hashlib.sha256((tools/name).read_bytes()).hexdigest()==digest,name
    (tools/'fpga/mister/reports').symlink_to(reports,target_is_directory=True)
    scripts=tools/'fpga/mister/snapshot';terminal=out/'terminal-fit'
    record=json.loads((terminal/'ready-prediction-integration.json').read_text())
    state=json.loads((terminal/'terminal-state.json').read_text())
    assert not state['builder_live']and not state['project_processes']and not state['prepared_inputs_changed']
    assert state['ready_path_tool_success']and not state['assembly_started']and not state['rbfs']
    rebuilt=out/'rebuilt-source';classification=out/'rebuilt-classification';review=out/'rebuilt-review';review.mkdir()
    def command(label,args):
        cmd=['python3',str(scripts/(label+'.py')),*map(str,args)]
        (review/(label+'-command.json')).write_text(json.dumps(cmd,indent=2)+'\n')
        with(review/(label+'.log')).open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
    command('build_native_ready_prediction',['--stage',rebuilt,'--destination',record['project']])
    assert json.loads((rebuilt/'ready-prediction-integration.json').read_text())==record
    for name in record['source_hashes']:assert(rebuilt/name).read_bytes()==(terminal/name).read_bytes(),name
    command('audit_native_ready_prediction',['--project',terminal,'--output',review/'source-audit.json'])
    command('inventory_native_ready_prediction',[terminal,'--output',classification])
    command('audit_serial_chipwidth_capacity',[terminal,'--source-audit',review/'source-audit.json',
        '--classification',classification/'classification.json','--output',review/'capacity-audit.json'])
    command('audit_serial_chipwidth_timing',[terminal/'core','--baseline',qual/'native59-timing',
        '--allow-timing-failure','--output',review/'timing-audit.json'])
    command('audit_adapter_crossings',[terminal/'core','--allow-timing-failure','--output',review/'crossings-audit.json'])
    command('audit_ready_prediction_timing',[terminal,'--output',review/'ready-path-audit.json'])
    compared=[]
    for name in('source-audit','capacity-audit','timing-audit','crossings-audit','ready-path-audit'):
        before=json.loads((proof/(name+'.json')).read_text());after=json.loads((review/(name+'.json')).read_text())
        # Timing source paths change on extraction; all raw bytes and clocks/
        # coverage/controls are independently checked, not just the verdict.
        if name=='timing-audit':before.pop('input_hashes');after.pop('input_hashes')
        assert before==after,'terminal review differs: '+name
        compared.append(dict(review=name,status=after['status'],failure_controls=len(after['failure_controls'])))
    before=json.loads((qual/'classification/classification.json').read_text());after=json.loads((classification/'classification.json').read_text())
    for row in(before,after):row.pop('xml_sha256');row.pop('normalized_xml_sha256')
    assert before==after,'fresh complete native classification differs'
    assert json.loads((review/'timing-audit.json').read_text())['status']=='REJECTED_TIMING'
    result=dict(status='PASS_TERMINAL_REJECTION_REPLAY',scope=__doc__,archive_readback=checked,
        frozen_sources=len(frozen),actual_rebuilt_source_files=len(record['source_hashes']),
        exact_reviews=compared,complete_native_classification=True,required_bits=sum(after['totals'].values()),
        limits='Serial73 remains timing-rejected. No assembly, FPGA load, full snapshot or speed qualification.')
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('report',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');a=p.parse_args()
    run(a.report.resolve(),a.output.resolve(),a.reports.resolve())
