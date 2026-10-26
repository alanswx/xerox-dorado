#!/usr/bin/env python3
"""Collect the terminal clock-enable routing rejection without restarting it."""
import argparse
import io
import json
from pathlib import Path
import subprocess
import tarfile

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def collect(stage,launch,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    l=json.loads((launch/'launch-observation.json').read_text())
    assert l['project']==m['project']and l['manifest_sha256']==sha(stage/'r-front-control-native.json')
    source_count=580 if m.get('native_RIOT_block_ram_staged')else 578
    assert m['clock_enable_conversion_staged']and len(m['source_hashes'])==source_count
    query='''from pathlib import Path
import hashlib,io,json,sys,tarfile,time
p=Path(PROJECT)
s=json.loads((p/'run-state.json').read_text())
assert s['status']=='TERMINAL_FAILED'and s['builder_pid']==BUILDER
assert [(a['stage'],a['returncode'])for a in s['stages']]==[('map',0),('fit',3)]
assert not s['assembly_permitted']
m=json.loads((p/'r-front-control-native.json').read_text())
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']}
assert len(actual)==SOURCE_COUNT and actual==m['source_hashes']
assert not list((p/'core/output_files').glob('*.rbf'))and not list((p/'core/output_files').glob('*.sof'))
files=sorted(set([f for f in p.glob('*.log')]+[p/'run-state.json',p/'r-front-control-native.json',p/'build_r_front_ready_native.py']+
 [f for f in(p/'core/output_files').iterdir()if f.suffix in ['.rpt','.summary','.smsg']]+[f for f in(p/'core').iterdir()if f.suffix in ['.qsf','.sdc','.tcl']]))
hashes={str(f.relative_to(p)):hashlib.sha256(f.read_bytes()).hexdigest()for f in files}
observation={'observed':time.time(),'run_state':s,'source_readback':actual,'collected_hashes':hashes,'assembly_or_hardware_changed':False}
b=(json.dumps(observation,indent=2)+'\\n').encode()
with tarfile.open(fileobj=sys.stdout.buffer,mode='w|gz')as archive:
 for f in files:archive.add(f,arcname=str(f.relative_to(p)),recursive=False)
 info=tarfile.TarInfo('collection.json');info.size=len(b);archive.addfile(info,io.BytesIO(b))
'''.replace('PROJECT',repr(m['project'])).replace('BUILDER',str(l['builder_pid'])).replace('SOURCE_COUNT',str(source_count))
    (out/'collection.py').write_text(query);bundle=out/'terminal.tgz'
    with bundle.open('xb')as dest:
        p=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query.encode(),stdout=dest,stderr=subprocess.PIPE)
    (out/'collection.stderr').write_bytes(p.stderr);assert p.returncode==0,p.stderr
    terminal=out/'terminal';terminal.mkdir()
    with tarfile.open(bundle)as archive:
        assert all(v.isfile()and not Path(v.name).is_absolute()and'..'not in Path(v.name).parts for v in archive.getmembers())
        archive.extractall(terminal,filter='data')
    o=json.loads((terminal/'collection.json').read_text());verify(terminal,o['collected_hashes'])
    assert o['source_readback']==m['source_hashes']
    summary=(terminal/'core/output_files/Dorado.fit.summary').read_text()
    log=(terminal/'fit.log').read_text()
    assert 'Fitter Status : Failed'in summary and 'terminated due to routing congestion'in log
    result=dict(status='COLLECTED_TERMINAL_NATIVE_CLOCK_ENABLE_ROUTING_REJECTION',stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_files_verified=source_count,
        terminal_state=o['run_state'],collection_sha256=sha(terminal/'collection.json'),bundle_sha256=sha(bundle),
        fit_summary=summary,routing_congestion=True,all_device_timing_available=False,
        physical_timing_or_firmware_or_hardware_speed_qualified=False,hardware_changed=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','launch','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();collect(a.stage.resolve(),a.launch.resolve(),a.output.resolve())
