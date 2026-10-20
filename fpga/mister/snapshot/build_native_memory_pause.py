#!/usr/bin/env python3
"""Stage real native memory-pause wiring from immutable Serial65 inputs.

Other services remain virtual and wrapper state access remains pending. Keep
actual periods/IO/hold constraints and all existing area policy unchanged.
No assembly or hardware deployment is permitted by this driver.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,raw_receipt
import generate_native_memory_pause as change

REPORT='2026-10-19-snapshot-serial-ready'
QUALIFIED='2026-10-20-snapshot-eight-msa-sdram-pause'


def sha(raw):return hashlib.sha256(raw).hexdigest()


def stage(out,reports,destination):
    parent=reports/REPORT;qualified=reports/QUALIFIED
    record=json.loads(raw_receipt(parent,'terminal-fit/ready-path-integration.json'))
    proof=json.loads(raw_receipt(qualified,'qualified/results.json'))
    assert proof['status']=='PASS'and (proof['msa_boards'],proof['raw_groups'],proof['shared_pause_coordinators'])==(8,128,1)
    assert len(proof['cases'])>=4
    old=record['project'];new=str(destination or out);out.mkdir(parents=True,exist_ok=False)
    replacements={change.PREFIX+'MSA__msa_board.sv':'msa_board.sv',
        change.PREFIX+'memory_pause_barrier.sv':'memory_pause_barrier.sv',
        'src/fpga/mister/dorado_storage_arb.sv':'dorado_storage_arb.sv',
        'src/fpga/mister/dorado_storage_sdram.sv':'dorado_storage_sdram.sv'}
    hashes={}
    for name,digest in record['source_hashes'].items():
        raw=raw_receipt(parent,'terminal-fit/'+name);assert sha(raw)==digest,name
        if Path(name).suffix in ('.qsf','.qpf','.qip','.sdc','.tcl'):raw=raw.decode().replace(old,new).encode()
        if name in replacements:
            source=replacements[name];raw=raw_receipt(qualified,'qualified/executed-inputs/'+source)
            assert sha(raw)==proof['executed_inputs'][source]
        elif name=='core/Dorado.sv':raw=change.core(raw.decode()).encode()
        elif name==change.BP:raw=change.backplane(raw.decode()).encode()
        elif name==change.WRAPPER:raw=change.wrapper(raw.decode()).encode()
        elif name=='core/Dorado.qsf':raw+=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {new}/{change.PREFIX}memory_pause_barrier.sv\n'.encode()
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw);hashes[name]=sha(raw)
    assert len(hashes)==659
    # Historical integration records are inputs to the independent inverse
    # reviewer, not claims that these new sources are the old experiment.
    for metadata in ('ready-path-integration.json','chipwidth-integration.json'):
        (out/metadata).write_bytes(raw_receipt(parent,'terminal-fit/'+metadata))
    newname=change.PREFIX+'memory_pause_barrier.sv';raw=raw_receipt(qualified,'qualified/executed-inputs/memory_pause_barrier.sv')
    assert sha(raw)==proof['executed_inputs']['memory_pause_barrier.sv'];(out/newname).write_bytes(raw);hashes[newname]=sha(raw)
    assets_path=change.PREFIX+'assets.json';assets=json.loads((out/assets_path).read_text())
    for name in ('MSA__msa_board.sv','memory_pause_barrier.sv'):assets['assets'][name]=hashes[change.PREFIX+name]
    assets['limits']='Real memory-pause coordinator; remaining service acknowledgments virtual, wrapper state access and full snapshots pending.'
    (out/assets_path).write_text(json.dumps(assets,indent=2)+'\n');hashes[assets_path]=sha((out/assets_path).read_bytes())
    result=dict(scope=__doc__,project=new,parent_project=old,parent_report=REPORT,
        verified_parent_inputs=record['source_hashes'],source_hashes=hashes,qualified_report=QUALIFIED,
        qualified_inputs=proof['executed_inputs'],schema_id='af0b647cb43a3a62',physical_board_bits=1957048,
        boards=19,capacity_only=True,assembly_permitted=False,real_memory_pause=True,real_pause_barriers=False,
        new_state_bits=2,new_ports=0,new_clocks=0,foreign_service_inputs=5,
        quarantine_reseed='Complete FPGA configuration only; both live snapshot-control reset inputs pinned low.',
        driver_sha256=sha(Path(__file__).read_bytes()),generator_sha256=sha(Path(change.__file__).read_bytes()),
        limits='Original board-only schema; wrapper/ARM/disk-paired state, complete snapshots, legal timing and hardware qualification remain required.')
    (out/'memory-pause-integration.json').write_text(json.dumps(result,indent=2)+'\n')
    print('Staged real native memory pause:',len(hashes),'inputs;',new,flush=True)


def run(out,quartus):
    record=json.loads((out/'memory-pause-integration.json').read_text())
    assert record['capacity_only']and not record['assembly_permitted']and not record['real_pause_barriers']
    assert record['real_memory_pause']and sha(Path(__file__).read_bytes())==record['driver_sha256']
    assert sha(Path(change.__file__).read_bytes())==record['generator_sha256']
    for name,digest in record['source_hashes'].items():assert sha((out/name).read_bytes())==digest,name
    assert not(out/'map.log').exists(),'observe existing run; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
        ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
        ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial69MemoryPause',label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Native memory pause terminal; independent sources/capacity/timing/state review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path);p.add_argument('--run',type=Path);p.add_argument('--destination',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.stage:stage(a.stage.resolve(),a.reports.resolve(),a.destination)
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --stage or --run')
