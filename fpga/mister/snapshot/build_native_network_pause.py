#!/usr/bin/env python3
"""Stage Serial71 real network/memory pause, with original Serial69 timing policy.

Capacity/timing experiment only. No assembly/deployment; full Main coordinator,
physical DDR visibility and complete machine snapshots remain required.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,raw_receipt
import generate_native_network_pause as change

PARENT='2026-10-20-snapshot-native-memory-pause-rejected'
QUALIFIED='2026-10-20-snapshot-network-barrier'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports,destination):
    report=reports/PARENT;qualified=reports/QUALIFIED
    parent=json.loads(raw_receipt(report,'terminal-fit/memory-pause-integration.json'))
    state=json.loads(raw_receipt(report,'terminal-fit/terminal-state.json'))
    assert not state['builder_live']and not state['project_processes']and state['prepared_inputs_changed']==[]
    proof=json.loads(raw_receipt(qualified,'wire/qualification.json'))
    control=json.loads(raw_receipt(qualified,'fourstate/results.json'))
    assert proof['status']=='PASS'and proof['real_network_pause_barrier']and proof['new_required_state_bits']==6
    assert proof['state_review']['observed_bits']==34344 and len(proof['cases'])==8
    assert control['status']=='PASS'and len(control['cases'])==8
    old=parent['project'];new=str(destination or out);out.mkdir(parents=True,exist_ok=False)
    assets={'src/fpga/mister/dorado_ethernet_ddr.sv':'dorado_ethernet_ddr.sv',
        'src/fpga/mister/dorado_ethernet_uio.sv':'dorado_ethernet_uio.sv',
        change.PREFIX+'network_pause_barrier.sv':'network_pause_barrier.sv'}
    hashes={}
    for name,digest in parent['source_hashes'].items():
        raw=raw_receipt(report,'terminal-fit/'+name);assert sha(raw)==digest,name
        if Path(name).suffix in('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(old,new).encode()
        if name in assets:
            source=assets[name];raw=raw_receipt(qualified,'wire/executed-inputs/'+source)
            assert sha(raw)==proof['inputs'][source]
        elif name=='core/Dorado.sv':raw=change.core(raw.decode()).encode()
        elif name==change.WRAPPER:raw=change.wrapper(raw.decode()).encode()
        elif name=='core/Dorado.qsf':raw+=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {new}/{change.PREFIX}network_pause_barrier.sv\n'.encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw);hashes[name]=sha(raw)
    assert len(hashes)==660
    name=change.PREFIX+'network_pause_barrier.sv';raw=raw_receipt(qualified,'wire/executed-inputs/network_pause_barrier.sv')
    assert sha(raw)==proof['inputs']['network_pause_barrier.sv']==control['inputs']['network_pause_barrier.sv']
    (out/name).write_bytes(raw);hashes[name]=sha(raw)
    for metadata in('chipwidth-integration.json','ready-path-integration.json'):
        (out/metadata).write_bytes(raw_receipt(report,'terminal-fit/'+metadata))
    # Preserve the upstream record as provenance. The independent reviewer
    # reconstructs it with exact inverse sources before invoking its auditor.
    upstream=dict(parent);upstream.update(project=new,source_hashes={n:hashes[n]for n in parent['source_hashes']})
    (out/'memory-pause-integration.json').write_text(json.dumps(upstream,indent=2)+'\n')
    record=dict(scope=__doc__,project=new,parent_project=old,parent_report=PARENT,qualified_report=QUALIFIED,
        verified_parent_inputs=parent['source_hashes'],source_hashes=hashes,qualified_inputs=proof['inputs'],
        capacity_only=True,assembly_permitted=False,real_memory_pause=True,real_network_pause=True,real_pause_barriers=False,
        new_top_state_bits=6,new_wrapper_state_bits=0,new_clocks=0,new_external_ports=0,
        foreign_service_inputs=5,schema_id='af0b647cb43a3a62',
        builder_sha256=sha(Path(__file__).read_bytes()),generator_sha256=sha(Path(change.__file__).read_bytes()),
        limits='Memory and network adapter boundaries only; external permission still owns physical DDR fence, Main/disk/input/video, wrapper state access, atomic full save/restore and all-device timing.')
    (out/'network-pause-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Staged Serial71 network/memory pause:',len(hashes),'inputs;',new,flush=True)


def run(out,quartus):
    record=json.loads((out/'network-pause-integration.json').read_text())
    assert record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers']
    assert sha(Path(__file__).read_bytes())==record['builder_sha256']
    assert sha(Path(change.__file__).read_bytes())==record['generator_sha256']
    for name,digest in record['source_hashes'].items():assert sha((out/name).read_bytes())==digest,name
    assert not(out/'map.log').exists(),'observe existing run; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
        ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
        ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial71NetworkPause',label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Native network pause terminal; independent source/capacity/timing/state review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path);p.add_argument('--run',type=Path);p.add_argument('--destination',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args()
    if a.stage:stage(a.stage.resolve(),a.reports.resolve(),a.destination)
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --stage or --run')
