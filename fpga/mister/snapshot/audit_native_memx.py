#!/usr/bin/env python3
"""Qualify terminal Native57 sources, inferred memories and all-corner timing.

Read completed Quartus evidence only; never invoke STA or program hardware.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import tarfile

from audit_native_conta import CORNERS, assignments, sha, ucp
from build_native_memx import CROSSINGS
from audit_memc_full import ram_table as memc_ram
from audit_native_processor import processor_ram
from audit_ifu_full import memories, physical_ram
from audit_memd_full import memories as memd_macros, physical_ram as memd_physical
from audit_memx_full import memories as memx_macros, physical_ram as memx_physical

BOARDS=('conta','contb','proch','procl','ifu','memc','memd','memx')
MODES=('setup','hold','recovery','removal','mpw','sdram-input-setup',
       'sdram-input-hold','sdram-output-setup','sdram-output-hold')+tuple(
       f'{b}-{direction}-{mode}' for b in BOARDS for direction in ('in','out')
       for mode in ('setup','hold'))
TARGET=Path('src/fpga/mister/snapshot_native_memx')


def check_assignments(root,baseline,metadata,actual=None):
    expected=(baseline/'core/Dorado.qsf').read_text().replace(str(baseline),str(root))
    for name in metadata['assets']:
        if name not in metadata['shared_assets']:
            expected+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(root/TARGET/name)+'\n'
    approved={(root/TARGET/name).resolve() for name in metadata['assets']
              if name not in metadata['shared_assets']}
    for directory in ('snapshot_native','snapshot_native_conta','snapshot_native_processor','snapshot_native_ifu','snapshot_native_memc','snapshot_native_memd'):
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
        ('wrong_source',actual.replace('MemX_full_snapshot.sv','MemX_wrong_snapshot.sv')),
    ):
        assert text!=actual
        try:check_assignments(root,baseline,metadata,text)
        except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged project accepted: '+name)
    return rows



def memc_ram_sections(text):
    # Preserve table headings and only MemC memory rows in their tables.
    return '\n'.join(line for line in text.split('\n')
                     if '|cell_'not in line or ':b_MemC|'in line)


def ifu_parameter_sections(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text))
    return ''.join(text[h.start():headers[i+1].start()if i+1<len(headers)else len(text)]
                   for i,h in enumerate(headers)if ':b_IFU|'in h[1])


def ifu_ram_evidence(mapping,fitted,manifest):
    # Filter only unrelated memory rows; preserve physical table and exact IFU rows.
    selected='\n'.join(line for line in fitted.split('\n')
                       if 'snapshot_bank_'not in line or ':b_IFU|'in line)
    macros=memories(ifu_parameter_sections(mapping),manifest)
    physical=physical_ram(selected,manifest)
    return dict(macros=macros,physical=physical,
                scope='11 address groups/27,648bits, OLD_DATA; admin inactive, not enabled whole-core snapshot cost')


def instruction_ram(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for User Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,header in enumerate(headers):
        if 'snapshot_bank_'not in header[1] or 'altsyncram:'not in header[1] or ':b_ContB|'not in header[1]:continue
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


def board_parameters(text,board):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text))
    return ''.join(text[h.start():headers[i+1].start()if i+1<len(headers)else len(text)]
                   for i,h in enumerate(headers)if ':b_'+board+'|'in h[1])


def map_rows(text):
    return '\n'.join(line for line in text.split('\n')
                     if '|cell_MosRam'not in line or ':b_MemX|'in line)


def check_proms(root,baseline,manifest):
    rows=[]
    for prom in manifest['immutable_proms']:
        name=prom['file'];wanted=prom['sha256']
        assert sha(root/'src'/name)==sha(baseline/'src'/name)==wanted,'compiled MemX PROM differs: '+name
        rows.append(dict(file=name,sha256=wanted))
    return rows


def collect(root,baseline,archive):
    core=root/'core';record=json.loads((root/'memx-integration.json').read_text())
    metadata=json.loads((root/TARGET/'assets.json').read_text())
    assert sha(Path(__file__).parent/'build_native_memx-executed.py')==metadata['builder_sha256'], 'executed builder differs'
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
    # Executed builder v1 retains the Native56 report labels in this Tcl.
    # Crossing reports are independently Native57. This is a label only;
    # compare exact scripts and require every expected file/category.
    assert (core/'device-corners.tcl').read_text()==(baseline/'core/device-corners.tcl').read_text().replace(str(baseline),str(root)), 'corner script differs'
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
    processor_metadata=json.loads((root/'src/fpga/mister/snapshot_native_processor/assets.json').read_text())
    ram_evidence={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        ram_evidence[variant]={title:processor_ram((project/'core/output_files'/file).read_text(errors='replace'),title,processor_metadata,'snapshot')
            for file,title in [('Dorado.map.rpt','Analysis & Synthesis RAM Summary'),('Dorado.fit.rpt','Fitter RAM Summary')]}
    ifu_metadata=json.loads((root/'src/fpga/mister/snapshot_native_ifu/assets.json').read_text())
    ifu_manifest=ifu_metadata['manifests']['IFU']
    ifu_memories=ifu_ram_evidence(mapping,fitted,ifu_manifest)
    memc_manifest=json.loads((root/'src/fpga/mister/snapshot_native_memc/assets.json').read_text())['manifests']['MemC']
    memc_memories={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        memc_memories[variant]={title:memc_ram(memc_ram_sections((project/'core/output_files'/file).read_text(errors='replace')),title,memc_manifest,'snapshot')
            for file,title in [('Dorado.map.rpt','Analysis & Synthesis RAM Summary'),('Dorado.fit.rpt','Fitter RAM Summary')]}
    memd_manifest=json.loads((root/'src/fpga/mister/snapshot_native_memd/assets.json').read_text())['manifests']['MemD']
    assert memd_manifest['parameters']==dict(STORAGE_INPUT_DELAY=2)
    assert memd_manifest['logical_state_bits']==296591 and memd_manifest['storage_input_bits']==37
    def memd_sections(text):
        headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text))
        return ''.join(text[h.start():headers[i+1].start()if i+1<len(headers)else len(text)]
                       for i,h in enumerate(headers)if ':b_MemD|'in h[1])
    memd_fitted='\n'.join(line for line in fitted.split('\n')if 'snapshot_bank_'not in line or ':b_MemD|'in line)
    memd_memories=dict(macros=memd_macros(memd_sections(mapping),memd_manifest),
                       physical=memd_physical(memd_fitted,memd_manifest))
    memx_manifest=metadata['manifests']['MemX']
    assert memx_manifest['logical_state_bits']==1378312
    assert memx_manifest['map_history_bits']==483 and len(memx_manifest['rams'])==21
    assert len(memx_manifest['immutable_proms'])==8
    prom_evidence=check_proms(root,baseline,memx_manifest)
    memx_memories={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        project_mapping=(project/'core/output_files/Dorado.map.rpt').read_text()
        project_fitted=(project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace')
        memx_memories[variant]=dict(macros=memx_macros(board_parameters(project_mapping,'MemX'),memx_manifest,variant),
            physical=memx_physical(map_rows(project_fitted),memx_manifest,variant))
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
            prefix='native56'if mode in MODES[:9]else'native57'
            p=core/f'{prefix}-{mode}-{corner}.rpt';reports.append(timed(p.read_text(),p.name))
        current=ucp((core/f'native56-ucp-{corner}.rpt').read_text())
        assert current==ucp((baseline/f'core/native56-ucp-{corner}.rpt').read_text()),'unconstrained coverage changed'
    assert len(reports)==164
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(core/'output_files/Dorado.sta.summary').read_text())
    assert len(checks)==31 and min(float(s)for _,s in checks)>=0,checks
    summary=(core/'output_files/Dorado.fit.summary').read_text()
    assert 'Fitter Status : Successful'in summary
    assert 'Assembler was successful'in (root/'asm.log').read_text()
    crossing_log=(root/'adapter-crossings.log').read_text()
    crossings=re.findall(r'ADAPTER_CROSSING_CORNER (\S+) (conta|contb|proch|procl|ifu|memc|memd|memx) board=(\d+) others=(\d+)',crossing_log)
    assert {(c,b)for c,b,_,_ in crossings}=={(c,b)for c in CORNERS for b in BOARDS} and len(crossings)==32
    assert all(int(a)>0 and int(b)>0 for _,_,a,b in crossings)
    rbf=core/'output_files/Dorado.rbf';assert rbf.stat().st_size>1000000
    resources={}
    for key,label in (('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks'),('dsp_blocks','Total DSP Blocks')):
        m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
        resources[key]=int(m[1].replace(',',''))
    failures=qsf_controls(root,baseline,metadata)
    original=(core/'native57-proch-in-hold-7_slow_1100mv_-40c.rpt').read_text()
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
        try:processor_ram(''.join(lines),title,processor_metadata,'snapshot')
        except AssertionError as error:failures.append(dict(case=phase+'_wrong_processor_ram',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged processor RAM accepted')
    # Restrict a semantic corruption to the IFU parameter tables.
    selected=ifu_parameter_sections(mapping)
    broken=selected.replace('; OLD_DATA', '; NEW_DATA', 1)
    assert broken!=selected,'missing IFU OLD_DATA control anchor'
    try:memories(broken,ifu_manifest)
    except AssertionError as error:failures.append(dict(case='ifu_new_data',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged IFU macro accepted')
    for file,title in [('Dorado.map.rpt','Analysis & Synthesis RAM Summary'),('Dorado.fit.rpt','Fitter RAM Summary')]:
        original=(core/'output_files'/file).read_text(errors='replace');lines=original.splitlines(keepends=True)
        for i,line in enumerate(lines):
            if ':b_MemC|'in line and '; MLAB 'in line:
                lines[i]=line.replace('; MLAB ','; M10K ',1);break
        else:raise AssertionError('missing MemC failure anchor')
        try:memc_ram(memc_ram_sections(''.join(lines)),title,memc_manifest,'snapshot')
        except AssertionError as error:failures.append(dict(case=file+'_wrong_memc_ram',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged MemC RAM accepted')
    for name,text,checker in [
        ('memd_new_data',memd_sections(mapping),lambda t:memd_macros(t,memd_manifest)),
        ('memd_physical_new_data',memd_fitted,lambda t:memd_physical(t,memd_manifest)),
    ]:
        before='; OLD_DATA'if name=='memd_new_data'else'; Old data'
        if name=='memd_new_data':changed=text.replace(before,'; NEW_DATA',1)
        else:
            # The preserved physical table also contains framework RAM
            # (notably the scaler). Corrupt an actual MemD bank row only.
            lines=text.splitlines(keepends=True)
            for i,line in enumerate(lines):
                if ':b_MemD|'in line and 'snapshot_bank_'in line and before in line:
                    lines[i]=line.replace(before,'; New data',1);break
            else:raise AssertionError('missing physical MemD OLD_DATA row')
            changed=''.join(lines)
        assert changed!=text,'missing MemD OLD_DATA control anchor'
        try:checker(changed)
        except AssertionError as error:failures.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged MemD RAM accepted: '+name)
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        parameter_text=board_parameters((project/'core/output_files/Dorado.map.rpt').read_text(),'MemX')
        broken=parameter_text.replace('; OLD_DATA','; NEW_DATA',1)
        assert broken!=parameter_text,'missing map macro fault anchor'
        try:memx_macros(broken,memx_manifest,variant)
        except AssertionError as error:failures.append(dict(case=variant+'_memx_new_data',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged map macro accepted')
        fitted_text=map_rows((project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace'))
        for kind,before,after in [('old_data','; Old data','; New data'),('placement','M10K_X','MLAB_X')]:
            lines=fitted_text.splitlines(keepends=True)
            for i,line in enumerate(lines):
                if ':b_MemX|'in line and '|cell_MosRam'in line and before in line:
                    lines[i]=line.replace(before,after,1);break
            else:raise AssertionError('missing map physical fault anchor: '+kind)
            try:memx_physical(''.join(lines),memx_manifest,variant)
            except AssertionError as error:failures.append(dict(case=variant+'_memx_'+kind,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged physical map accepted')
    damaged=json.loads(json.dumps(memx_manifest))
    damaged['immutable_proms'][0]['sha256']='0'*64
    try:check_proms(root,baseline,damaged)
    except AssertionError as error:failures.append(dict(case='memx_prom_hash',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('wrong compiled PROM accepted')
    q=dict(status='PASS',hardware='NOT DEPLOYED',frequency_mhz=50,sysper=4,
        timing_checks=31,path_categories_checked=164,operating_conditions_checked=4,
        minimum_slack_ns=min(r['slack_ns']for r in reports),illegal_clocks=0,unconstrained_clocks=0,
        unconstrained_io_summary=current,extra_timing_exceptions='None',resources=resources,
        unconstrained_io_scope='Unchanged MiSTer framework I/O coverage; all SDRAM and eight adapter crossing categories timed.',
        snapshot_admin='INACTIVE; normal-operation integration only, no machine snapshot or speed qualification',
        change=metadata['scope'],instruction_memories=instruction_ram(mapping),processor_memories=ram_evidence,
        ifu_memories=ifu_memories,ifu_memory_bits=27648,memd_memories=memd_memories,memd_memory_bits=294912,memx_memories=memx_memories,memx_memory_bits=1376256,immutable_proms=prom_evidence,memc_memories=memc_memories,memc_mlab_bits=21504,memc_mlab_cells=672,processor_memory_bits=9216,processor_mlab_cells=288,warning_comparison=warnings,
        failure_controls=failures,reports=reports,rbf_sha256=sha(rbf))
    parent=json.loads((baseline/'qualification.json').read_text())
    for name,value in dict(storage_prefetch=True,storage_interleave=True,storage_defer_reads=False,
                           network_ddr=True,raw_line_cache_entries=0,required_sdram_mb=128,
                           guest_ram_mib=8,raw_backing_mib=16).items():
        assert parent[name]==value,'unreviewed baseline memory/transport policy: '+name
        q[name]=value
    assert parent['raw_backend'].startswith('SDRAM')
    q['raw_backend']=parent['raw_backend']
    q['policy_scope']='Inherited from qualified56b, with exact unchanged outer wrapper, project assignments and all other source bytes verified above.'
    (root/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    inputs=[p for p in (root/'src').rglob('*')if p.is_file()]
    inputs+=[core/name for name in ('Dorado.sv','Dorado.qsf','Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','device-corners.tcl','audit-adapter-crossings.tcl')]
    (root/'compiled-inputs.json').write_text(json.dumps({str(p.relative_to(root)):sha(p)for p in inputs},indent=2)+'\n')
    items=inputs+[Path(__file__).parent/'build_native_memx-executed.py',Path(__file__),root/'qualification.json',root/'compiled-inputs.json',root/'memx-integration.json']
    items+=sorted(core.glob('native56-*.rpt'))+sorted(core.glob('native57-*.rpt'))+[core/'native-unconstrained.rpt']
    items+=[core/'output_files'/name for name in ('Dorado.sta.summary','Dorado.fit.summary','Dorado.map.rpt','Dorado.fit.rpt')]
    items+=[root/(phase+'.log')for phase in ('map','fit','asm','sta','audit','device-corners','adapter-crossings')]
    with tarfile.open(archive,'w:gz')as tar:
        for p in items:
            arcname=str(p.relative_to(root))if p.is_relative_to(root)else'executed-audit/'+p.name
            tar.add(p,arcname=arcname)
    print(json.dumps({k:v for k,v in q.items()if k not in ('reports','processor_memories','instruction_memories')},indent=2))


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project',type=Path,default=Path('/home/alans/mister/Dorado_Native57_20261019'))
    parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native56b_20261019'))
    parser.add_argument('--archive',type=Path,default=Path('/tmp/native57-evidence.tar.gz'))
    args=parser.parse_args();collect(args.project.resolve(),args.baseline.resolve(),args.archive)
