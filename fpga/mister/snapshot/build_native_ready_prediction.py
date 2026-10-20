#!/usr/bin/env python3
"""Stage Serial73 NEXT-ready; run only source/state-qualified capacity/timing.

No assembly/deployment. Full service coordination, machine snapshots and the
original 60 ns clock migration remain open. Never restart a previous run.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[3]
PARENT='2026-10-20-snapshot-native-disk-pause-source'
QUALIFIED='2026-10-20-snapshot-ready-prediction'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports,destination):
    from generate_serial_boards import raw_receipt
    import generate_native_ready_prediction as change
    parent=json.loads(raw_receipt(reports/PARENT,'staged-source/disk-pause-integration.json'))
    qual={label:json.loads(raw_receipt(reports/QUALIFIED,label+'/results.json'))for label in('fourstate','msa','eight-msa')}
    assert all(q['status']=='PASS'for q in qual.values())
    assert [len(qual[x]['cases'])for x in qual]==[8,3,3]
    assert qual['eight-msa']['required_external_register_bits']==8 and qual['eight-msa']['physical_board_bits']==12304
    assert json.loads(raw_receipt(reports/QUALIFIED,'notes/verification.json'))['status']=='PASS'
    old=parent['project'];new=str(destination or out);out.mkdir(parents=True,exist_ok=False);hashes={}
    assets={change.PREFIX+'MSA__msa_board.sv':'msa_board.sv',change.PREFIX+'MSA__storage_group.sv':'storage_group.sv'}
    for name,digest in parent['source_hashes'].items():
        raw=raw_receipt(reports/PARENT,'staged-source/'+name);assert sha(raw)==digest,name
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(old,new).encode()
        if name in assets:
            member=assets[name];raw=raw_receipt(reports/QUALIFIED,'eight-msa/executed-inputs/'+member)
            assert sha(raw)==qual['eight-msa']['executed_inputs'][member]
            assert raw==raw_receipt(reports/QUALIFIED,'msa/candidate/'+member)
        elif name=='core/Dorado.sv':raw=change.core(raw.decode()).encode()
        elif name==change.WRAPPER:raw=change.wrapper(raw.decode()).encode()
        elif name==change.BACKPLANE:raw=change.backplane(raw.decode()).encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw);hashes[name]=sha(raw)
    assert len(hashes)==662
    name=change.PREFIX+'assets.json';record=json.loads((out/name).read_text())
    for path in assets:record['assets'][Path(path).name]=hashes[path]
    (out/name).write_text(json.dumps(record,indent=2)+'\n');hashes[name]=sha((out/name).read_bytes())
    for name in('chipwidth-integration.json','ready-path-integration.json','memory-pause-integration.json','network-pause-integration.json','disk-pause-integration.json'):
        (out/name).write_bytes(raw_receipt(reports/PARENT,'staged-source/'+name))
    record=dict(scope=__doc__,project=new,parent_project=old,parent_report=PARENT,qualified_report=QUALIFIED,
        verified_parent_inputs=parent['source_hashes'],source_hashes=hashes,qualified_proofs=qual,
        capacity_only=True,assembly_permitted=False,real_pause_barriers=False,new_required_wrapper_bits=8,
        new_clocks=0,new_external_ports=0,schema_id='af0b647cb43a3a62',
        builder_sha256=sha(Path(__file__).read_bytes()),generator_sha256=sha(Path(change.__file__).read_bytes()),
        limits='Native integration candidate. Actual routed timing/capacity, eight-register state access, full coordinator/snapshots and faster clocks remain unqualified.')
    (out/'ready-prediction-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Staged Serial73 NEXT-ready:',len(hashes),'inputs;',new,flush=True)


def run(out,quartus):
    record=json.loads((out/'ready-prediction-integration.json').read_text())
    assert record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers']
    assert record['builder_sha256']==sha(Path(__file__).read_bytes())
    assert record['generator_sha256']==sha(Path(__file__).with_name('generate_native_ready_prediction.py').read_bytes())
    for name,digest in record['source_hashes'].items():assert sha((out/name).read_bytes())==digest,name
    source=json.loads((out/'source-audit.json').read_text());state=json.loads((out/'state-audit.json').read_text())
    assert source['status']=='PASS_NATIVE_READY_PREDICTION_SOURCE'and len(source['failure_controls'])>=12
    assert source['source_hashes']==record['source_hashes']
    assert state['status']=='PASS_NATIVE_READY_PREDICTION_STATE_ACCESS_PENDING'and len(state['failure_controls'])>=18
    assert state['new_required_wrapper_bits']==8 and state['totals']['WRAPPER ACCESS PENDING']==132619
    assert state['source_hashes']==record['source_hashes']
    assert not any((out/(name+'.log')).exists()for name in('map','fit','sta','clocks','corners','crossings')),'observe existing run; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
        ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
        ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial73ReadyPrediction '+label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Serial73 terminal; independent capacity/timing review required. No assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path);p.add_argument('--destination',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.stage:stage(a.stage.resolve(),a.reports.resolve(),a.destination)
    elif a.run:run(a.run.resolve(),a.quartus.resolve())
    else:p.error('Specify --stage or --run')
