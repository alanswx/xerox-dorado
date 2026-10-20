#!/usr/bin/env python3
"""Same nineteen-board Serial61 RTL with documented Quartus area/packing options.

Only four QSF optimization assignments change. Source geometry, clocks, memory
policy, virtual service contracts and no-assembly scope remain unchanged.
Quartus17's installed assignment metadata confirms these settings are supported;
area mode may reduce timing margin, so all existing timing gates remain required.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import build_serial_core as base

CHANGES=[
 ('OPTIMIZATION_MODE','"HIGH PERFORMANCE EFFORT"','"Aggressive Area"'),
 ('QII_AUTO_PACKED_REGISTERS','NORMAL','"Minimize Area with Chains"'),
 ('OPTIMIZATION_TECHNIQUE','SPEED','AREA'),
 ('PHYSICAL_SYNTHESIS_COMBO_LOGIC_FOR_AREA','OFF','ON'),
]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def stage(baseline,out,assets,receipt):
    base.stage(baseline,out,assets,receipt)
    serial=json.loads((out/'serial-integration.json').read_text());qsf=out/'core/Dorado.qsf';before=qsf.read_text();after=before
    for name,old,new in CHANGES:
        old_line='set_global_assignment -name '+name+' '+old+'\n';new_line='set_global_assignment -name '+name+' '+new+'\n'
        assert after.count(old_line)==1,'unexpected optimization policy: '+name
        after=after.replace(old_line,new_line,1)
    qsf.write_text(after)
    record=dict(scope=__doc__,driver_sha256=sha(Path(__file__)),base_builder_sha256=sha(Path(base.__file__)),
                original_serial_integration_sha256=sha(out/'serial-integration.json'),original_qsf_sha256=hashlib.sha256(before.encode()).hexdigest(),
                changes=[dict(name=n,before=o,after=v)for n,o,v in CHANGES],
                source_hashes={name:sha(out/name)for name in serial['source_hashes']},
                boards=19,physical_board_bits=1966264,capacity_only=True,assembly_permitted=False,
                report_prefix='serial61',limits='No native barriers/complete snapshot. PLL reset proxy not qualified for production; no deployment.')
    (out/'area-integration.json').write_text(json.dumps(record,indent=2)+'\n');print('Four supported area/packing changes; exact nineteen-board RTL and timing constraints retained',flush=True)


def run(out,quartus):
    record=json.loads((out/'area-integration.json').read_text());assert record['capacity_only']and not record['assembly_permitted']
    assert sha(Path(__file__))==record['driver_sha256']and sha(Path(base.__file__))==record['base_builder_sha256']
    assert sha(out/'serial-integration.json')==record['original_serial_integration_sha256']
    for name,digest in record['source_hashes'].items():assert sha(out/name)==digest,'prepared area input changed: '+name
    assert not(out/'map.log').exists(),'observe existing build; do not restart'
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),('quartus_sta',['Dorado'],'sta'),
                            ('quartus_sta',['-t','audit_native_clocks.tcl'],'clocks'),('quartus_sta',['-t','device-corners.tcl'],'corners'),
                            ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'crossings')]:
        print('Serial62Area',label,flush=True)
        with(out/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=out/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Area run terminal; independent source/memory/timing audit required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path);p.add_argument('--baseline',type=Path);p.add_argument('--assets',type=Path);p.add_argument('--baseline-receipt',type=Path)
    p.add_argument('--run',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.stage and a.baseline and a.assets and a.baseline_receipt:stage(a.baseline.resolve(),a.stage.resolve(),a.assets.resolve(),a.baseline_receipt.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --stage with frozen inputs or --run')
