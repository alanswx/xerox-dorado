#!/usr/bin/env python3
"""Freeze and run the production native machine with external memory/media models.

The exact staged wrapper/boards/helpers and original EPROMs are unmodified.
The harness supplies an independent physical SDRAM model, normal PS/2 input,
raw Pup service and HPS block transfers. No CPU emulator/state/IOCB bridge.
This is a native-machine diagnostic, not full MiSTer sys_top/Main/video proof.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import posixpath
import re
import shutil
import subprocess
import time
import os

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(stage,mapping,out,medium,qualification=None):
    assert not out.exists()
    manifests=[stage/name for name in ('storage-board-native.json',
        'r-front-ready-native.json','mc1668-candidate.json') if (stage/name).exists()]
    assert len(manifests)==1, 'Select one exact staged native recipe'
    manifest=manifests[0]
    r=json.loads(manifest.read_text())
    qualification=qualification or stage/'qualification.json'
    q=json.loads(qualification.read_text())
    expected_status={'r-front-ready-native.json':'PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW',
        'mc1668-candidate.json':'PASS_NATIVE_MC1668_BUILD',
        'storage-board-native.json':'PASS_NATIVE_STORAGE_BOARD_PREASSEMBLY_REVIEW'}[manifest.name]
    assert q['status']==expected_status
    assert q['source_hashes']==r['source_hashes']and r['sysper']==4
    assert r['state_admin']=='INACTIVE'
    host_hz=r.get('frequency_hz',int(round(r['frequency_mhz']*1000000)))
    if manifest.name=='storage-board-native.json':
        assert host_hz==q['frequency_hz']==57142857 and q['sysper']==4
        assert sha(mapping)==q['reviewed_evidence_hashes']['core/output_files/Dorado.map.rpt']
    else:
        assert host_hz==50000000 and r['frequency_mhz']==50
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    hashes={}
    for name,h in r['source_hashes'].items():
        p=stage/name;assert sha(p)==h,name
        target=inputs/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target)
        hashes[name]=h
    text=mapping.read_text();table=text[text.index('; Analysis & Synthesis Source Files Read'):].split('\n\n',1)[0]
    used=[]
    for line in table.splitlines():
        fields=[v.strip()for v in line.split(';')[1:-1]]
        if len(fields)!=5 or fields[1]!='yes':continue
        absolute=posixpath.normpath(fields[3]);prefix=r['project']+'/'
        if not absolute.startswith(prefix+'src/'):continue
        name=absolute[len(prefix):]
        if Path(name).suffix not in ['.v','.sv']:continue
        assert name in hashes,name
        if name not in used:used.append(name)
    wrapper='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
    assert wrapper in used and len(used)>100,len(used)
    definitions=[]
    for line in (stage/'core/Dorado.qsf').read_text().splitlines():
        m=re.fullmatch(r'set_global_assignment -name VERILOG_MACRO (.+)',line)
        if m:definitions.append(m[1].strip('"'))
    assert [d for d in definitions if d.startswith('DORADO_HOST_CLOCK_HZ=')]==[
        'DORADO_HOST_CLOCK_HZ='+str(host_hz)]
    extra={
        'harness/tb_native_cedar.sv':ROOT/'verilog/verilator/native_cedar/tb_native_cedar.sv',
        'harness/host.cpp':ROOT/'verilog/verilator/native_cedar/host.cpp',
        'harness/xsds_model.sv':ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
        'host/ethernet.c':ROOT/'dorado/src/ethernet.c','host/io.c':ROOT/'dorado/src/io.c',
        'run_native_cedar_memory.py':Path(__file__),
    }
    for p in (ROOT/'dorado/include').glob('*.h'):extra['host/include/'+p.name]=p
    for name,p in extra.items():
        dest=inputs/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dest);hashes[name]=sha(dest)
    # Literal readmem paths in the compiled original boards resolve from src/.
    for p in (inputs/'src').rglob('*'):
        if p.is_file()and p.suffix in ['.v','.sv']:
            for raw in re.findall(r'\.INIT_FILE\("([^"]+)"\)',p.read_text()):
                assert (inputs/'src'/raw).is_file(),('Missing original ROM',raw)
    artifacts={
        'medium':medium.resolve(),'aemu':ROOT/'dorado/worlds/aemu.eb',
        'cedar':ROOT/'chm/dorado/CedarDorado.eb!6','alto':ROOT/'chm/bootfiles/NETEXEC.BOOT!8',
        'stp':ROOT/'chm/cedar/stp-root',
    }
    for n,p in artifacts.items():assert p.exists(),(n,p)
    (out/'host-cycle-limit.txt').write_text('1500000000\n')
    record=dict(status='PREPARED_EXACT_NATIVE_MEMORY_DIAGNOSTIC',stage=str(stage),
        stage_manifest_name=manifest.name,stage_manifest_sha256=sha(manifest),qualification_sha256=sha(qualification),
        mapping_sha256=sha(mapping),inputs=hashes,used_hdl=used,defines=definitions,
        host_clock_hz=host_hz,
        artifacts={n:dict(path=str(p),sha256=sha(p)if p.is_file()else None)for n,p in artifacts.items()},
        no_CPU_state_or_guest_IOCB_completion=True,complete_MiSTer_Main_video_qualified=False)
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print(record['status'],len(used),'compiled-used native HDL inputs; exact staged source bytes')


def run(out):
    r=json.loads(gzip.decompress((out/'provenance.json.gz').read_bytes()));inputs=out/'inputs'
    for name,h in r['inputs'].items():assert sha(inputs/name)==h,name
    for item in r['artifacts'].values():
        if item['sha256']:assert sha(Path(item['path']))==item['sha256']
    state_path=out/'run-state.json';assert not state_path.exists(),'Observe existing recorded job'
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
    def save():state_path.write_text(json.dumps(state,indent=2)+'\n')
    def execute(command,label,cwd):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,cwd=cwd,stdout=log,stderr=subprocess.STDOUT)
            a=dict(stage=label,pid=p.pid,command=command,started=time.time());state['actions'].append(a);save()
            code=p.wait();a.update(returncode=code,finished=time.time());save();assert code==0,(label,code)
    save()
    try:
        objects=[]
        for name in ['ethernet','io']:
            obj=out/(name+'.o');objects.append(str(obj))
            execute(['cc','-O2','-std=c11','-ffunction-sections','-fdata-sections','-I'+str(inputs/'host/include'),'-c',str(inputs/'host'/f'{name}.c'),'-o',str(obj)],'host-'+name,out)
        command=['verilator','--binary','--timing','--autoflush','--top-module','tb_native_cedar','-Wno-fatal','-Wno-PINMISSING','-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN','-Wno-LATCH','-Wno-TIMESCALEMOD',
                 '-Iverilog/cells','-Iverilog/generated','-Iverilog/vendor/6502','-Iverilog/vendor/riot',
                 *['-D'+d for d in r['defines']], '--Mdir',str(out/'obj'),'-o','native-cedar','-j','4',
                 '-CFLAGS','-std=c++17 -I'+str(inputs/'host/include'),'-LDFLAGS',' '.join(objects)+' -Wl,-dead_strip',
                 *[str(inputs/n)for n in r['used_hdl']],str(inputs/'harness/xsds_model.sv'),str(inputs/'harness/tb_native_cedar.sv'),str(inputs/'harness/host.cpp')]
        execute(command,'compile',inputs/'src')
        command=[str(out/'obj/native-cedar'),'+limit-file='+str(out/'host-cycle-limit.txt')]
        command+=['+'+n+'='+item['path']for n,item in r['artifacts'].items()]
        execute(command,'test',inputs/'src')
        log=(out/'test.log').read_text();stop=re.findall(r'NATIVE_DIAGNOSTIC_STOP reason=\s*(\w+)[^\n]+',log)
        assert len(stop)==1,'No terminal diagnostic reason'
        state.update(status='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC',reason=stop[0],finished=time.time(),firmware_qualified=False,desktop_qualified=False);save()
        print(state['status'],stop[0],flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());save();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);mode=p.add_mutually_exclusive_group(required=True)
    mode.add_argument('--prepare',type=Path);mode.add_argument('--run',type=Path)
    p.add_argument('--stage',type=Path);p.add_argument('--mapping',type=Path);p.add_argument('--medium',type=Path)
    p.add_argument('--qualification',type=Path,help='Completed review when stored outside the frozen stage')
    a=p.parse_args()
    if a.prepare:
        assert a.stage and a.mapping and a.medium
        prepare(a.stage.resolve(),a.mapping.resolve(),a.prepare.resolve(),a.medium.resolve(),
                a.qualification.resolve()if a.qualification else None)
    else:run(a.run.resolve())
