#!/usr/bin/env python3
"""Check actual Cyclone V memory inference for the qualified RIOT RAM candidate.

Mapping only: never fit, assemble, load, restart an existing project or change
the native fit. Keep exact inputs and terminal reports for later integration.
"""
import argparse
import base64
import json
from pathlib import Path
import subprocess

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def start(tests,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    t=json.loads((tests/'run-state.json').read_text());verify(tests,t['files'])
    assert t['status']=='PASS_RESET_MASKED_RIOT_BLOCK_RAM_COMPONENT'
    helper=ROOT/'verilog/rtl/dorado_riot_ram.sv'
    assert sha(helper)==t['production_helper_sha256']
    out.mkdir(parents=True)
    (out/'ram.sv').write_bytes(helper.read_bytes())
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY dorado_riot_ram
set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files
set_global_assignment -name SYSTEMVERILOG_FILE ram.sv
'''
    (out/'Ram.qsf').write_text(qsf)
    inputs={n:base64.b64encode((out/n).read_bytes()).decode()for n in ['ram.sv','Ram.qsf']}
    runner='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path.cwd();files=json.loads((p/'input-hashes.json').read_text())
assert all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in files.items())
s={'status':'RUNNING','started':time.time(),'source_hashes':files,'assembly_permitted':False}
def save():(p/'run-state.json').write_text(json.dumps(s,indent=2)+'\\n')
with(p/'map.log').open('xb')as log:
 child=subprocess.Popen(['/home/alans/intelFPGA_lite/17.0/quartus/bin/quartus_map','--read_settings_files=on','--write_settings_files=off','Ram'],stdout=log,stderr=log)
 s['mapper_pid']=child.pid;save();code=child.wait()
s.update(status='TERMINAL_MAP_COMPLETE'if code==0 else'TERMINAL_MAP_REJECTED',returncode=code,finished=time.time());save()
'''
    hashes={n:sha(out/n)for n in inputs}
    script='''from pathlib import Path
import base64,json,subprocess,time
p=Path(PROJECT);assert not p.exists(),'Observe existing project; do not restart'
p.mkdir()
for n,data in INPUTS.items():(p/n).write_bytes(base64.b64decode(data))
(p/'input-hashes.json').write_text(json.dumps(HASHES))
(p/'map.py').write_text(RUNNER)
with(p/'builder.log').open('xb')as log:
 child=subprocess.Popen(['python3',str(p/'map.py')],cwd=p,stdout=log,stderr=log,start_new_session=True)
process=subprocess.run(['ps','-p',str(child.pid),'-o','pid=,etime=,stat=,args='],capture_output=True,text=True)
assert process.returncode==0
print(json.dumps({'status':'STARTED_RIOT_MEMORY_MAPPING','project':str(p),'builder_pid':child.pid,'process':process.stdout.strip(),'source_hashes':HASHES,'assembly_permitted':False}))
'''.replace('PROJECT',repr(str(project))).replace('INPUTS',repr(inputs)).replace('HASHES',repr(hashes)).replace('RUNNER',repr(runner))
    (out/'launch.py').write_text(script)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=script,text=True,capture_output=True)
    (out/'launch.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    r=json.loads(remote.stdout);r['tests_state_sha256']=sha(tests/'run-state.json')
    (out/'launch-observation.json').write_text(json.dumps(r,indent=2)+'\n')
    print(r['status'],r['builder_pid'],flush=True)


def collect(source,out):
    assert not out.exists();out.mkdir(parents=True)
    launch=json.loads((source/'launch-observation.json').read_text())
    script='''from pathlib import Path
import base64,hashlib,json,subprocess,time
p=Path(PROJECT);s=json.loads((p/'run-state.json').read_text())
assert s['status']in ['TERMINAL_MAP_COMPLETE','TERMINAL_MAP_REJECTED']
assert s['source_hashes']==HASHES and all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in HASHES.items())
files=[p/'run-state.json',p/'map.log',p/'map.py',*list((p/'output_files').glob('*.rpt')),*list((p/'output_files').glob('*.summary'))]
assert not list(p.rglob('*.sof'))and not list(p.rglob('*.rbf'))
print(json.dumps({'state':s,'files':{str(q.relative_to(p)):base64.b64encode(q.read_bytes()).decode()for q in files if q.is_file()}}))
'''.replace('PROJECT',repr(launch['project'])).replace('HASHES',repr(launch['source_hashes']))
    (out/'collect.py').write_text(script)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=script,text=True,capture_output=True)
    (out/'collect.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    r=json.loads(remote.stdout)
    for n,b in r['files'].items():
        assert not Path(n).is_absolute()and'..'not in Path(n).parts
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(base64.b64decode(b))
    summary=(out/'output_files/Ram.map.summary').read_text()
    log=(out/'map.log').read_text()
    assert r['state']['returncode']==0 and '1,024' in summary and 'altsyncram' in log
    result=dict(status='COLLECTED_RIOT_RAM_MAPPING',project=launch['project'],state=r['state'],map_summary=summary,
                native_integration_or_full_timing_or_firmware_qualified=False,
                files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--tests',type=Path);p.add_argument('--project',type=Path)
    p.add_argument('--collect',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if a.collect:collect(a.collect.resolve(),a.output.resolve())
    else:
        assert a.tests and a.project
        start(a.tests.resolve(),a.output.resolve(),a.project)
