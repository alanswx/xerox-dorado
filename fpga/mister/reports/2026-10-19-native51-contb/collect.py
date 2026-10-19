"""Qualify completed Native51 timing/source evidence; no hardware programming.

Run on misterubuntu ONLY after build and the extra ContB crossing audit finish.
"""
from pathlib import Path
from collections import Counter
import argparse
import hashlib
import json
import re
import shlex
import tarfile

CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c',
         'MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
MODES=('setup','hold','recovery','removal','mpw','sdram-input-setup',
       'sdram-input-hold','sdram-output-setup','sdram-output-hold',
       'contb-in-setup','contb-in-hold','contb-out-setup','contb-out-hold')


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def assignments(text,core,approved):
    marker='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    result=[];seen=set()
    for line in text.splitlines():
        line=line.strip()
        if not line or line==marker:continue
        words=shlex.split(line,comments=True)
        if len(words)==4 and words[:3]==['set_global_assignment','-name','SYSTEMVERILOG_FILE']:
            path=Path(words[3])
            path=(path if path.is_absolute()else core/path).resolve()
            # Quartus appends relative aliases for the four added source
            # files. Deduplicate ONLY those exact approved physical files;
            # every other source, setting and timing assignment stays exact.
            if path in approved:
                if path in seen:continue
                seen.add(path)
                line='set_global_assignment -name SYSTEMVERILOG_FILE '+str(path)
        result.append(line)
    return result


def ucp(text):
    values={}
    for name in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports',
                 'Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
        match=re.search(r';\s*'+name+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',text)
        assert match,'missing coverage: '+name
        values[name]=list(map(int,match.groups()))
    assert values['Illegal Clocks']==values['Unconstrained Clocks']==[0,0]
    return values


def collect(root,baseline,archive):
    core=root/'core'
    record=json.loads((root/'contb-integration.json').read_text())
    metadata=json.loads((root/'src/fpga/mister/snapshot_native/assets.json').read_text())
    assert record['snapshot_admin'].startswith('INACTIVE') and record['extra_timing_exceptions']=='None'
    for name,digest in record['source_hashes'].items():
        if name!='core/Dorado.qsf':assert sha(root/name)==digest,'fitted source differs: '+name
    expected=(baseline/'core/Dorado.qsf').read_text().replace(str(baseline),str(root))
    for name in metadata['assets']:
        expected+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(root/'src/fpga/mister/snapshot_native'/name)+'\n'
    approved={(root/'src/fpga/mister/snapshot_native'/n).resolve()for n in metadata['assets']}
    assert assignments((core/'Dorado.qsf').read_text(),core,approved)==assignments(expected,core,approved),'project assignments changed'
    for name in ('Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc'):
        assert (core/name).read_text().replace(str(root),str(baseline))==(baseline/'core'/name).read_text(),'constraints differ: '+name
    assert (core/'device-corners.tcl').read_text().replace('native51-','native49-')==(baseline/'core/device-corners.tcl').read_text().replace(str(baseline),str(root))
    ram_text=(core/'output_files/Dorado.map.rpt').read_text()
    # Anchor real parameter tables; the table of contents repeats these titles
    # but contains no fitted parameters and must never count as an instance.
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for User Entity Instance:\s*([^\n;]+)',ram_text))
    memory=[]
    for i,header in enumerate(headers):
        if 'snapshot_bank_'not in header[1] or 'altsyncram:'not in header[1]:continue
        section=ram_text[header.end():headers[i+1].start()if i+1<len(headers)else len(ram_text)]
        modes={}
        for key,value in (('OPERATION_MODE','DUAL_PORT'),('WIDTH_A','18'),('WIDTH_B','18'),
                          ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')):
            found=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section)
            assert found==[value],('instruction RAM inference differs',header[1],key,found)
            modes[key]=value
        memory.append(dict(instance=header[1].strip(),parameters=modes))
    assert len(memory)==8 and {int(re.search(r'snapshot_bank_(\d)',r['instance'])[1])for r in memory}==set(range(8))
    warnings={}
    for label in ('map','fit'):
        current_text=(root/(label+'.log')).read_text()
        assert 'Critical Warning'not in current_text,'critical warning: '+label
        current=Counter(re.findall(r'Warning \((\d+)\)',current_text))
        previous=Counter(re.findall(r'Warning \((\d+)\)',(baseline/(label+'.log')).read_text()))
        new=current-previous
        assert not(set(new)-{'10036'}),('unreviewed warning increase',label,new)
        warnings[label]=dict(increased=dict(new),decreased=dict(previous-current),
                             review='Only unused-object warnings increase; redundant fanout/admin/clock wires in enabled-board replacement. Other baseline warning categories unchanged or reduced.')
    changed=[];added=[]
    for p in (root/'src').rglob('*'):
        if not p.is_file():continue
        name=p.relative_to(root/'src');old=baseline/'src'/name
        if not old.exists():added.append(str(name))
        elif p.read_bytes()!=old.read_bytes():changed.append(str(name))
    assert set(changed)==set(record['source_changes']) and set(added)==set(record['added_sources']),'unintended source delta'
    reports=[]
    for corner in sorted(CORNERS):
        for mode in MODES:
            p=core/f'native51-{mode}-{corner}.rpt'
            text=p.read_text()
            match=re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
            assert match,'missing timed paths: '+p.name
            count,violations,slack=int(match[1]),int(match[2]),float(match[3])
            assert count>0 and violations==0 and slack>=0,(p.name,match.groups())
            reports.append(dict(report=p.name+'.gz',paths=count,slack_ns=slack))
        current=ucp((core/f'native51-ucp-{corner}.rpt').read_text())
        previous=ucp((baseline/f'core/native49-ucp-{corner}.rpt').read_text())
        assert current==previous,'changed unconstrained I/O requires review: '+corner
    assert len(reports)==52
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(core/'output_files/Dorado.sta.summary').read_text())
    assert len(checks)==31 and min(float(s)for _,s in checks)>=0,checks
    assert 'Fitter Status : Successful' in (core/'output_files/Dorado.fit.summary').read_text()
    assert 'Assembler was successful' in (root/'asm.log').read_text()
    crossing_log=(root/'contb-crossings.log').read_text()
    found=re.findall(r'CONTB_CROSSING_CORNER (\S+) board=(\d+) others=(\d+)',crossing_log)
    assert {x[0]for x in found}==CORNERS and len(found)==4 and all(int(a)>0 and int(b)>0 for _,a,b in found)
    rbf=core/'output_files/Dorado.rbf'
    assert rbf.stat().st_size>1000000
    q=dict(hardware='NOT DEPLOYED',frequency_mhz=50,sysper=4,
           operating_conditions_checked=4,path_categories_checked=52,timing_checks=31,
           minimum_slack_ns=min(r['slack_ns']for r in reports),illegal_clocks=0,unconstrained_clocks=0,
           unconstrained_io_summary=current,
           unconstrained_io_scope='Unchanged existing MiSTer/framework I/O coverage from frozen49; no new unconstrained ports/paths. SDRAM I/O and all ContB crossing reports are timed.',
           extra_timing_exceptions='None',raw_backend='SDRAM, no raw line buffer',raw_line_cache_entries=0,
           qsf_normalization='Blank lines, exact Quartus version marker and duplicate absolute/relative aliases of only the four approved added sources; no other source/setting/SDC changes accepted',
           storage_defer_reads=False,storage_prefetch=True,storage_interleave=True,network_ddr=True,
           required_sdram_mb=128,guest_ram_mib=8,raw_backing_mib=16,
           snapshot_admin='INACTIVE; full-core normal-operation adapter qualification only, no physical save/load',
           instruction_memories=memory,warning_comparison=warnings,
           change='Frozen49 plus qualified ContB host-clock/exact-advance adapters; same other boards/storage/Main/server/clock settings/constraints',
           reports=reports,rbf_sha256=sha(rbf))
    (root/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    inputs=[core/n for n in ('Dorado.sv','Dorado.qsf','Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','device-corners.tcl','audit-contb-crossings.tcl')]
    inputs+=[p for p in (root/'src').rglob('*')if p.is_file()]
    (root/'compiled-inputs.json').write_text(json.dumps({str(p.relative_to(root)):sha(p)for p in inputs},indent=2)+'\n')
    items=[*sorted(core.glob('native51-*.rpt')),core/'native-unconstrained.rpt',
           core/'output_files/Dorado.sta.summary',core/'output_files/Dorado.fit.summary',
           core/'output_files/Dorado.map.rpt',core/'Dorado.qsf',core/'audit-contb-crossings.tcl',
           *[root/(label+'.log')for label in ('map','fit','asm','sta','audit','device-corners')],
           root/'qualification.json',root/'compiled-inputs.json',root/'contb-integration.json',root/'contb-crossings.log']
    with tarfile.open(archive,'w:gz')as tar:
        for p in items:tar.add(p,arcname=p.name)
    print(json.dumps({k:v for k,v in q.items()if k!='reports'},indent=2))
    print((core/'output_files/Dorado.fit.summary').read_text())


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project',type=Path,default=Path('/home/alans/mister/Dorado_Native51_v3_20261019'))
    parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native49_20261019'))
    parser.add_argument('--archive',type=Path,default=Path('/tmp/native51-evidence.tar.gz'))
    args=parser.parse_args()
    collect(args.project,args.baseline,args.archive)
