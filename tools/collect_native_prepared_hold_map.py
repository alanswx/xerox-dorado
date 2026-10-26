#!/usr/bin/env python3
"""Collect completed Hold/Ready mapping while observing its existing fitter.

Compare synthesis to synthesis. Mapping success/register counts do not establish
placed timing or useful speed. No build is started, restarted or modified.
"""
import argparse
import base64
import json
from pathlib import Path
import re
import subprocess

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def collect(stage,launch,parent_timing,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    l=json.loads((launch/'launch-observation.json').read_text())
    ready_mode=m.get('parallel_Ready_target_comparison_staged',False)
    source_count=582 if ready_mode else 581
    assert m['prepared_MemC_Hold_staged']and len(m['source_hashes'])==source_count
    assert l['manifest_sha256']==sha(stage/'r-front-control-native.json')and l['project']==m['project']
    parent=json.loads((parent_timing/'results.json').read_text())
    assert parent['exact_stage_manifest_sha256']==m['parent_manifest_sha256']and parent['source_files_verified']==source_count-1
    query='''from pathlib import Path
import base64,hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert hashlib.sha256((p/'r-front-control-native.json').read_bytes()).hexdigest()==MANIFEST
assert len(m['source_hashes'])==SOURCE_COUNT and all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())
r=json.loads((p/'run-state.json').read_text());assert r['builder_pid']==BUILDER and not r['assembly_permitted']
assert r['stages'][0]['stage']=='map'and r['stages'][0]['returncode']==0
ps=[]
if r['status']=='RUNNING':
 for pid in [r['builder_pid'],r['stages'][-1]['pid']]:
  s=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,stat=,args='],capture_output=True,text=True)
  ps.append({'pid':pid,'returncode':s.returncode,'process':s.stdout.strip()})
 assert any(s['returncode']==0 for s in ps)
assert not list((p/'core/output_files').glob('*.rbf'))and not list((p/'core/output_files').glob('*.sof'))
names=['map.log','core/output_files/Dorado.map.summary','core/output_files/Dorado.map.rpt','run-state.json']
print(json.dumps({'observed':time.time(),'state':r,'processes':ps,'sources_verified':SOURCE_COUNT,
 'files':{n:base64.b64encode((p/n).read_bytes()).decode()for n in names},
 'hashes':{n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in names}}))
'''.replace('PROJECT',repr(m['project'])).replace('MANIFEST',repr(sha(stage/'r-front-control-native.json'))).replace('BUILDER',str(l['builder_pid'])).replace('SOURCE_COUNT',str(source_count))
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    assert remote.returncode==0,remote.stderr
    r=json.loads(remote.stdout);files=r.pop('files');out.mkdir(parents=True)
    (out/'collection-query.py').write_text(query)
    for name,data in files.items():
        assert not Path(name).is_absolute()and'..'not in Path(name).parts
        p=out/'completed-map'/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(base64.b64decode(data));assert sha(p)==r['hashes'][name]
    (out/'observation.json').write_text(json.dumps(r,indent=2)+'\n')
    summary=(out/'completed-map/core/output_files/Dorado.map.summary').read_text()
    before=(parent_timing/'terminal/core/output_files/Dorado.map.summary').read_text()
    def count(text,label):
        rows=re.findall(r'^'+label+r' : ([\d,]+)$',text,re.M);assert len(rows)==1,label
        return int(rows[0].replace(',',''))
    registers=[count(text,'Total registers')for text in [before,summary]]
    memories=[count(text,'Total block memory bits')for text in [before,summary]]
    table=(out/'completed-map/core/output_files/Dorado.map.rpt').read_text();riots=[]
    for line in table.splitlines():
        fields=[s.strip()for s in line.split(';')]
        if len(fields)>9 and'native_ram'in fields[1]and fields[2]=='M10K block':
            assert fields[3:9]==['Simple Dual Port','128','8','128','8','1024'];riots.append(fields[1])
    assert len(riots)==5
    (out/'parent-map.summary').write_text(before)
    result=dict(status='COLLECTED_READY_MODE_NATIVE_MAPPING_AND_SAME_JOB_STATE'if ready_mode else'COLLECTED_PREPARED_HOLD_NATIVE_MAPPING_AND_SAME_JOB_STATE',
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_files_verified=source_count,
                same_builder=r['state'],processes=r['processes'],observed=r['observed'],
                synthesis_registers_before=registers[0],synthesis_registers_after=registers[1],
                synthesis_register_delta=registers[1]-registers[0],memory_bits_before=memories[0],memory_bits_after=memories[1],
                original_five_RIOT_M10K_instances=riots,placement_timing_or_speed_qualified=False,
                files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],registers,memories,flush=True)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','launch','parent-timing','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();collect(a.stage.resolve(),a.launch.resolve(),a.parent_timing.resolve(),a.output.resolve())
