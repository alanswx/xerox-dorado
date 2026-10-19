#!/usr/bin/env python3
"""Qualify terminal Native53 sources, inferred memories and all-corner timing.

Read completed Quartus evidence only; never invoke STA or program hardware.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import tarfile

from audit_native_conta import CORNERS, assignments, sha, ucp
from build_native_processor import CROSSINGS

BOARDS=('conta','contb','proch','procl')
MODES=('setup','hold','recovery','removal','mpw','sdram-input-setup',
       'sdram-input-hold','sdram-output-setup','sdram-output-hold')+tuple(
       f'{b}-{direction}-{mode}' for b in BOARDS for direction in ('in','out')
       for mode in ('setup','hold'))
TARGET=Path('src/fpga/mister/snapshot_native_processor')


def check_assignments(root,baseline,metadata,actual=None):
    expected=(baseline/'core/Dorado.qsf').read_text().replace(str(baseline),str(root))
    for name in metadata['assets']:
        if name not in metadata['shared_assets']:
            expected+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(root/TARGET/name)+'\n'
    approved={(root/TARGET/name).resolve() for name in metadata['assets']
              if name not in metadata['shared_assets']}
    for directory in ('snapshot_native','snapshot_native_conta'):
        parent=root/'src/fpga/mister'/directory
        data=json.loads((parent/'assets.json').read_text())
        approved|={(parent/name).resolve() for name in data['assets']
                   if (parent/name).exists()}
    if actual is None:actual=(root/'core/Dorado.qsf').read_text()
    assert assignments(actual,root/'core',approved)==assignments(expected,root/'core',approved), 'project assignments changed'


def qsf_controls(root,baseline,metadata):
    actual=(root/'core/Dorado.qsf').read_text();rows=[]
    for name,text in (
        ('extra_macro',actual+'\nset_global_assignment -name VERILOG_MACRO "UNREVIEWED=1"\n'),
        ('extra_sdc',actual+'\nset_global_assignment -name SDC_FILE unreviewed.sdc\n'),
        ('wrong_source',actual.replace('ProcH_full_snapshot.sv','ProcH_wrong_snapshot.sv')),
    ):
        assert text!=actual
        try:check_assignments(root,baseline,metadata,text)
        except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged project accepted: '+name)
    return rows


def processor_ram(text,title,metadata,variant):
    start=text.find('; '+title);assert start>=0,'missing '+title
    table=text[start:].split('\n\n',1)[0];header=None;rows=[]
    for line in table.splitlines():
        if not line.startswith('; '):continue
        fields=[x.strip()for x in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields
        elif fields and re.search(r':b_Proc[HL]\|',fields[0]):
            assert header is not None and len(header)==len(fields),'bad processor RAM row'
            rows.append(dict(zip(header,fields)))
    expected={(kind,r['package']):r for kind,m in metadata['manifests'].items() for r in m['rams']}
    assert len(rows)==len(expected)==12,'missing/additional processor memory'
    seen=set()
    for row in rows:
        hits=[key for key,r in expected.items() if ':b_'+key[0]+'|' in row['Name'] and
              '|'+r['kind']+('_snapshot' if variant=='snapshot' else '')+':'+key[1]+'|' in row['Name']]
        assert len(hits)==1 and hits[0]not in seen,'unexpected/duplicate processor RAM'
        key=hits[0];seen.add(key);width=expected[key]['width']
        values={'Type':'MLAB','Mode':'Simple Dual Port','Port A Depth':'256','Port B Depth':'256',
                'Port A Width':str(width),'Port B Width':str(width),'Size':str(256*width)}
        if title=='Fitter RAM Summary':
            values.update({'Clock Mode':'Single Clock','Port A Output Registers':'no',
                'Port B Input Registers':'no','Port B Output Registers':'no',
                'Implementation Bits':str(256*width),'M10K blocks':'0','MLAB cells':str(8*width)})
        for name,value in values.items():assert row.get(name)==value,('processor RAM differs',key,name,row.get(name))
    assert seen==set(expected)
    return rows


def instruction_ram(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for User Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,header in enumerate(headers):
        if 'snapshot_bank_'not in header[1] or 'altsyncram:'not in header[1]:continue
        section=text[header.end():headers[i+1].start()if i+1<len(headers)else len(text)]
        modes={}
        for key,value in (('OPERATION_MODE','DUAL_PORT'),('WIDTH_A','18'),('WIDTH_B','18'),
                          ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')):
            found=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section)
            assert found==[value],('instruction RAM differs',header[1],key,found)
            modes[key]=value
        rows.append(dict(instance=header[1].strip(),parameters=modes))
    assert len(rows)==8 and {int(re.search(r'snapshot_bank_(\d)',r['instance'])[1])for r in rows}==set(range(8))
    return rows


def timed(text,name):
    m=re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
    assert m,'missing timed paths: '+name
    count,violations,slack=int(m[1]),int(m[2]),float(m[3])
    assert count>0 and violations==0 and slack>=0,(name,m.groups())
    return dict(report=name+'.gz',paths=count,slack_ns=slack)


def collect(root,baseline,archive):
    core=root/'core';record=json.loads((root/'processor-integration.json').read_text())
    metadata=json.loads((root/TARGET/'assets.json').read_text())
    assert record['snapshot_admin'].startswith('INACTIVE') and record['extra_timing_exceptions']=='None'
    for name,digest in record['source_hashes'].items():
        if name!='core/Dorado.qsf':assert sha(root/name)==digest,'compiled source differs: '+name
    for name,digest in metadata['expected_original_sources'].items():
        assert sha(root/'src'/name)==sha(baseline/'src'/name)==digest,'qualified original differs: '+name
    for name,digest in metadata['assets'].items():
        p=root/'src'/metadata['shared_assets'][name]if name in metadata['shared_assets']else root/TARGET/name
        assert sha(p)==digest,'qualified adapter differs: '+name
    assert (core/'Dorado.sv').read_bytes()==(baseline/'core/Dorado.sv').read_bytes(),'outer clock/wrapper changed'
    for name in ('Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc'):
        assert (core/name).read_text().replace(str(root),str(baseline))==(baseline/'core'/name).read_text(),'constraints differ: '+name
    assert (core/'device-corners.tcl').read_text().replace('native53-','native52-')==(baseline/'core/device-corners.tcl').read_text().replace(str(baseline),str(root))
    assert (core/'audit-adapter-crossings.tcl').read_text()==CROSSINGS,'crossing coverage changed'
    check_assignments(root,baseline,metadata)
    changed=[];added=[]
    for p in (root/'src').rglob('*'):
        if not p.is_file():continue
        name=p.relative_to(root/'src');old=baseline/'src'/name
        if not old.exists():added.append(str(name))
        elif p.read_bytes()!=old.read_bytes():changed.append(str(name))
    assert set(changed)==set(record['source_changes']) and set(added)==set(record['added_sources']),'unintended source delta'
    assert changed==['build/rtl-executive/native16-tx-cable/dorado_native_backplane.v']
    mapping=(core/'output_files/Dorado.map.rpt').read_text()
    fitted=(core/'output_files/Dorado.fit.rpt').read_text(errors='replace')
    memories={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        memories[variant]={title:processor_ram((project/'core/output_files'/file).read_text(errors='replace'),title,metadata,variant)
            for file,title in [('Dorado.map.rpt','Analysis & Synthesis RAM Summary'),('Dorado.fit.rpt','Fitter RAM Summary')]}
    warnings={}
    for phase in ('map','fit'):
        current=(root/(phase+'.log')).read_text();previous=(baseline/(phase+'.log')).read_text()
        assert 'Critical Warning'not in current,'critical '+phase+' warning'
        before=Counter(re.findall(r'Warning \((\d+)\)',previous));after=Counter(re.findall(r'Warning \((\d+)\)',current))
        new=after-before
        assert not(set(new)-{'10036'}),('unreviewed warning increase',phase,new)
        warnings[phase]=dict(increased=dict(new),decreased=dict(before-after),review='Only unused-object10036 increases permitted; other categories unchanged or reduced.')
    reports=[]
    for corner in sorted(CORNERS):
        for mode in MODES:
            p=core/f'native53-{mode}-{corner}.rpt';reports.append(timed(p.read_text(),p.name))
        current=ucp((core/f'native53-ucp-{corner}.rpt').read_text())
        assert current==ucp((baseline/f'core/native52-ucp-{corner}.rpt').read_text()),'unconstrained coverage changed'
    assert len(reports)==100
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(core/'output_files/Dorado.sta.summary').read_text())
    assert len(checks)==31 and min(float(s)for _,s in checks)>=0,checks
    summary=(core/'output_files/Dorado.fit.summary').read_text()
    assert 'Fitter Status : Successful'in summary
    assert 'Assembler was successful'in (root/'asm.log').read_text()
    crossing_log=(root/'adapter-crossings.log').read_text()
    crossings=re.findall(r'ADAPTER_CROSSING_CORNER (\S+) (conta|contb|proch|procl) board=(\d+) others=(\d+)',crossing_log)
    assert {(c,b)for c,b,_,_ in crossings}=={(c,b)for c in CORNERS for b in BOARDS} and len(crossings)==16
    assert all(int(a)>0 and int(b)>0 for _,_,a,b in crossings)
    rbf=core/'output_files/Dorado.rbf';assert rbf.stat().st_size>1000000
    resources={}
    for key,label in (('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks'),('dsp_blocks','Total DSP Blocks')):
        m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
        resources[key]=int(m[1].replace(',',''))
    failures=qsf_controls(root,baseline,metadata)
    original=(core/'native53-proch-in-hold-7_slow_1100mv_-40c.rpt').read_text()
    damaged=re.sub(r'\(0 violated\)','(1 violated)',original,count=1);assert damaged!=original
    try:timed(damaged,'corrupted-hold')
    except AssertionError as error:failures.append(dict(case='violated_hold',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged timing accepted')
    for phase,text,title in [('map',mapping,'Analysis & Synthesis RAM Summary'),('fit',fitted,'Fitter RAM Summary')]:
        # Change only a processor memory row, independently of other core RAM.
        lines=text.splitlines(keepends=True);changed=False
        for i,line in enumerate(lines):
            if ':b_ProcH|'in line and '; MLAB 'in line:
                lines[i]=line.replace('; MLAB ','; M10K ',1);changed=True;break
        assert changed,'missing RAM failure anchor'
        try:processor_ram(''.join(lines),title,metadata,'snapshot')
        except AssertionError as error:failures.append(dict(case=phase+'_wrong_processor_ram',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged processor RAM accepted')
    q=dict(status='PASS',hardware='NOT DEPLOYED',frequency_mhz=50,sysper=4,
        timing_checks=31,path_categories_checked=100,operating_conditions_checked=4,
        minimum_slack_ns=min(r['slack_ns']for r in reports),illegal_clocks=0,unconstrained_clocks=0,
        unconstrained_io_summary=current,extra_timing_exceptions='None',resources=resources,
        unconstrained_io_scope='Unchanged MiSTer framework I/O coverage; all SDRAM and four adapter crossing categories timed.',
        snapshot_admin='INACTIVE; normal-operation integration only, no machine snapshot or speed qualification',
        change=metadata['scope'],instruction_memories=instruction_ram(mapping),processor_memories=memories,
        processor_memory_bits=9216,processor_mlab_cells=288,warning_comparison=warnings,
        failure_controls=failures,reports=reports,rbf_sha256=sha(rbf))
    parent=json.loads((baseline/'qualification.json').read_text())
    for name,value in dict(storage_prefetch=True,storage_interleave=True,storage_defer_reads=False,
                           network_ddr=True,raw_line_cache_entries=0,required_sdram_mb=128,
                           guest_ram_mib=8,raw_backing_mib=16).items():
        assert parent[name]==value,'unreviewed baseline memory/transport policy: '+name
        q[name]=value
    assert parent['raw_backend'].startswith('SDRAM')
    q['raw_backend']=parent['raw_backend']
    q['policy_scope']='Inherited from qualified52, with exact unchanged outer wrapper, project assignments and all other source bytes verified above.'
    (root/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    inputs=[p for p in (root/'src').rglob('*')if p.is_file()]
    inputs+=[core/name for name in ('Dorado.sv','Dorado.qsf','Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','device-corners.tcl','audit-adapter-crossings.tcl')]
    (root/'compiled-inputs.json').write_text(json.dumps({str(p.relative_to(root)):sha(p)for p in inputs},indent=2)+'\n')
    items=inputs+[root/'qualification.json',root/'compiled-inputs.json',root/'processor-integration.json']
    items+=sorted(core.glob('native53-*.rpt'))+[core/'native-unconstrained.rpt']
    items+=[core/'output_files'/name for name in ('Dorado.sta.summary','Dorado.fit.summary','Dorado.map.rpt','Dorado.fit.rpt')]
    items+=[root/(phase+'.log')for phase in ('map','fit','asm','sta','audit','device-corners','adapter-crossings')]
    with tarfile.open(archive,'w:gz')as tar:
        for p in items:tar.add(p,arcname=str(p.relative_to(root)))
    print(json.dumps({k:v for k,v in q.items()if k not in ('reports','processor_memories','instruction_memories')},indent=2))


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project',type=Path,default=Path('/home/alans/mister/Dorado_Native53_20261019'))
    parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native52_20261019'))
    parser.add_argument('--archive',type=Path,default=Path('/tmp/native53-evidence.tar.gz'))
    args=parser.parse_args();collect(args.project.resolve(),args.baseline.resolve(),args.archive)
