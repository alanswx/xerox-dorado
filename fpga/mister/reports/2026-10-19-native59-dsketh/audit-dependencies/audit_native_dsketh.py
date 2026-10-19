#!/usr/bin/env python3
"""Qualify terminal Native59 sources, inferred memories and all-corner timing.

Read completed Quartus evidence only; never invoke STA or program hardware.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import tarfile

from audit_native_conta import CORNERS, assignments, sha, ucp
from build_native_dsketh import CROSSINGS
from audit_memc_full import ram_table as memc_ram
from audit_native_processor import processor_ram
from audit_ifu_full import memories, physical_ram
from audit_memd_full import memories as memd_macros
from audit_memx_full import memories as memx_macros, physical_ram as memx_physical
from audit_dispy_full import word_macros

BOARDS=('conta','contb','proch','procl','ifu','memc','memd','memx','dispy','dsketh')
MODES=('setup','hold','recovery','removal','mpw','sdram-input-setup',
       'sdram-input-hold','sdram-output-setup','sdram-output-hold')+tuple(
       f'{b}-{direction}-{mode}' for b in BOARDS for direction in ('in','out')
       for mode in ('setup','hold'))
TARGET=Path('src/fpga/mister/snapshot_native_dsketh')


def check_assignments(root,baseline,metadata,actual=None):
    expected=(baseline/'core/Dorado.qsf').read_text().replace(str(baseline),str(root))
    for name in metadata['assets']:
        if name not in metadata['shared_assets']:
            expected+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(root/TARGET/name)+'\n'
    approved={(root/TARGET/name).resolve() for name in metadata['assets']
              if name not in metadata['shared_assets']}
    for directory in ('snapshot_native','snapshot_native_conta','snapshot_native_processor','snapshot_native_ifu','snapshot_native_memc','snapshot_native_memd','snapshot_native_memx','snapshot_native_dispy'):
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
        ('wrong_source',actual.replace('DskEth_full_snapshot.sv','DskEth_wrong_snapshot.sv')),
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
        assert sha(root/'src'/name)==sha(baseline/'src'/name)==wanted,'compiled immutable PROM differs: '+name
        rows.append(dict(file=name,sha256=wanted))
    return rows


def native_cache_physical(text,manifest,packing):
    """Check actual full-core packing, including the two reviewed shared pairs.

    The isolated MemD fit has independent banks. Native58's inactive-admin
    fit packs banks3/5 and8/10 together; Native59 keeps them independent. Per-logical-row block counts repeat
    those shared locations; summing them would overcount physical storage.
    No inferred/physical geometry or OLD_DATA requirement is relaxed.
    """
    assert packing in ('paired38','independent40')
    start=text.find('; Fitter RAM Summary');assert start>=0,'missing cache RAM table'
    rows=[];header=None
    for line in text[start:].split('\n\n',1)[0].splitlines():
        if not line.startswith('; '):continue
        fields=[x.strip()for x in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields
        elif fields and ':b_MemD|'in fields[0]and 'snapshot_bank_'in fields[0]:
            assert header and len(header)==len(fields),'malformed native cache row'
            rows.append(dict(zip(header,fields)))
    assert len(rows)==14 and len(manifest['banks'])==14,'native cache bank count differs'
    shared={3,5,8,10}if packing=='paired38'else set()
    groups={};seen=set()
    for row in rows:
        m=re.search(r'\bsnapshot_bank_(\d+)\|',row['Name']);assert m,'unknown native cache'
        bank=int(m[1]);assert bank<14 and bank not in seen,'duplicate native cache bank'
        seen.add(bank);width=manifest['banks'][bank]['width']
        blocks=3 if bank in shared else(width+1)//2
        wanted={'Type':'M10K block','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
            'Port A Depth':'4096','Port B Depth':'4096','Port A Width':str(width),'Port B Width':str(width),
            'Implementation Port A Depth':'4096','Implementation Port B Depth':'4096',
            'Implementation Port A Width':str(width),'Implementation Port B Width':str(width),
            'Size':str(width*4096),'Implementation Bits':str(width*4096),
            'M10K blocks':str(blocks),'MLAB cells':'0','Port A Input Registers':'yes',
            'Port A Output Registers':'no','Port B Input Registers':'yes','Port B Output Registers':'no',
            'Mixed Width RDW Mode':'Old data','ECC Mode':'Off','ECC Pipeline Registers':'No'}
        for key,value in wanted.items():
            assert row.get(key)==value,('native cache differs',bank,key,row.get(key))
        sites=tuple(sorted(x.strip()for x in row['Location'].split(',')))
        assert len(sites)==len(set(sites))==blocks,'native cache location count differs'
        assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',s)for s in sites),'invalid native cache location'
        groups.setdefault(sites,[]).append(bank)
    expected={frozenset([b])for b in range(14)if b not in shared}
    if shared:expected|={frozenset([3,5]),frozenset([8,10])}
    assert {frozenset(b)for b in groups.values()}==expected,'unreviewed native cache sharing'
    occupied=set();physical=[]
    for sites,banks in groups.items():
        assert not occupied.intersection(sites),'partially overlapping native cache groups'
        occupied.update(sites)
        width=sum(manifest['banks'][b]['width']for b in banks)
        assert len(sites)==(width+1)//2,'native cache group capacity differs'
        physical.append(dict(banks=sorted(banks),locations=list(sites),logical_bits=4096*width,
                             depth_mode_capacity_bits=8192*len(sites)))
    assert sum(g['logical_bits']for g in physical)==294912,'native cache bits differ'
    assert len(occupied)==(38 if packing=='paired38'else 40),'native cache physical count differs'
    return dict(rows=rows,groups=physical,unique_m10ks=len(occupied),logical_bits=294912,
        packing=packing,scope='Inactive-admin full core: all14 logical cache banks retain geometry and OLD_DATA. Native58 banks3/5 and8/10 each share three M10Ks; groups are disjoint and capacity checked. Isolated enabled-state packing is qualified separately.')


def dispy_physical(text,manifest,variant):
    start=text.find('; Fitter RAM Summary');assert start>=0,'missing physical RAM summary'
    header=None;rows=[]
    for line in text[start:].split('\n\n',1)[0].splitlines():
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and ':b_DispY|'in f[0] and ('|cell_'in f[0]or'|dorado_snapshot_f10415_word_bank:'in f[0]):
            assert header and len(header)==len(f),'bad native display RAM row';rows.append(dict(zip(header,f)))
    expected={r['package']:r for r in manifest['memories']if r['kind']in('cell_MB7071H','cell_F10415A')and r['package']!='u_f04'}
    assert len(rows)==len(expected)==11,'unexpected inactive display RAM pruning/count'
    seen=set();word_sites=set();fifo_sites={}
    for row in rows:
        hits=[]
        for pkg,r in expected.items():
            kind=r['kind']
            token='dorado_snapshot_f10415_word_bank:'+r['instance']if variant=='snapshot'and kind=='cell_F10415A'else kind+('_snapshot'if variant=='snapshot'else'')+':'+pkg
            if '|'+token+'|'in row['Name']:hits.append(r)
        assert len(hits)==1 and hits[0]['package']not in seen,'duplicate/unknown native display RAM'
        r=hits[0];seen.add(r['package']);raw=r['kind']=='cell_F10415A'
        wanted={'Type':'M10K block'if raw else'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
            'Port A Depth':str(r['words']),'Port B Depth':str(r['words']),'Port A Width':str(r['width']),'Port B Width':str(r['width']),
            'Size':str(r['words']*r['width']),'Implementation Bits':str(r['words']*r['width']),
            'Implementation Port A Depth':str(r['words']),'Implementation Port B Depth':str(r['words']),
            'Implementation Port A Width':str(r['width']),'Implementation Port B Width':str(r['width']),
            'Port A Input Registers':'yes','Port A Output Registers':'no','Port B Input Registers':'yes'if raw else'no','Port B Output Registers':'no',
            'M10K blocks':'1'if raw else'0','MLAB cells':'0'if raw else'32'}
        if raw:wanted['Mixed Width RDW Mode']='Old data'if variant=='snapshot'else"Don't care"
        for k,value in wanted.items():assert row.get(k)==value,('native display RAM differs',r['package'],k,row.get(k))
        if raw:
            assert re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',row['Location']),'native display word RAM location differs'
            word_sites.add(row['Location'])
        else:
            sites=tuple(sorted(x.strip()for x in row['Location'].split(',')))
            assert sites and all(re.fullmatch(r'LAB_X\d+_Y\d+_N\d+',x)for x in sites),'native display FIFO placement differs'
            fifo_sites.setdefault(sites,[]).append(r['package'])
    assert seen==set(expected)and 1<=len(word_sites)<=3
    return dict(rows=rows,logical_word_bits=3072,logical_fifo_bits=8192,word_sites=sorted(word_sites),
        fifo_physical_groups=[dict(locations=list(k),packages=v)for k,v in fifo_sites.items()],
        scope='Normal operation with administrative inputs inactive: u_f04 is entirely pruned, equivalent FIFO replicas may share physical storage and word planes may pack. Every remaining logical plane/geometry/physical type/word read-during-write checked. Enabled complete13,993bit display access is separately qualified; no whole-machine save/load cost claim.')


def dsketh_physical(text):
    # Native58 uses nine separate M10Ks, unlike the five shared blocks in
    # the isolated enabled adapter experiment. h10/h15 output registers
    # remain in fabric; every other retained ROM has an M10K output register.
    start=text.find('; Fitter RAM Summary');assert start>=0,'missing disk/Ethernet ROM table'
    header=None;rows={};sites=set()
    widths=dict(u_b14=4,u_h09=4,u_h10=4,u_h11=4,u_h14=4,u_h15=4,u_h16=2,u_l10=3,u_l15=3)
    for line in text[start:].split('\n\n',1)[0].splitlines():
        if not line.startswith('; '):continue
        fields=[s.strip()for s in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields;continue
        if not fields or ':b_DskEth|'not in fields[0]:continue
        assert header and len(fields)==len(header),'malformed disk/Ethernet ROM row'
        row=dict(zip(header,fields));match=re.search(r'\|cell_MCM10149:(u_\w+)\|',fields[0])
        assert match and match[1]in widths and match[1]not in rows,'unreviewed disk/Ethernet RAM'
        pkg=match[1];width=widths[pkg]
        expected={'Type':'AUTO','Mode':'ROM','Clock Mode':'Single Clock',
            'Port A Depth':'256','Port A Width':'4','Size':'1024',
            'Implementation Port A Depth':'256','Implementation Port A Width':str(width),
            'Implementation Bits':str(width*256),'M10K blocks':'1','MLAB cells':'0',
            'Port A Input Registers':'yes','Port A Output Registers':'no'if pkg in ('u_h10','u_h15')else'yes',
            'ECC Mode':'Off','ECC Pipeline Registers':'No'}
        for key,value in expected.items():assert row.get(key)==value,('native disk/Ethernet ROM differs',pkg,key,row.get(key))
        site=row['Location'];assert re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',site)and site not in sites,'disk/Ethernet ROM sharing/placement differs'
        sites.add(site);rows[pkg]=row
    assert set(rows)==set(widths)and len(sites)==9
    assert sum(int(r['Implementation Bits'])for r in rows.values())==8192
    return dict(rows=list(rows.values()),unique_m10ks=9,implemented_bits=8192,
        scope='Actual inactive-admin Native58 packing: nine distinct M10Ks, immutable ROM geometry and fabric/block output-register selection checked. All14source assets hash checked separately.')


def dsketh_evidence(mapping,fitted,manifest,variant):
    """Check ordinary-state retention with admin disabled, not enabled scan cost.

    All 210 child entities must retain their baseline register counts. The
    133 surviving mutable helpers must have their qualified adapter kinds;
    eight disconnected/pruned helpers must stay absent in both projects.
    Immutable PROM contents are checked separately against compiled assets.
    """
    assert variant in ('baseline','snapshot')
    helpers={r['package']:r for r in manifest['ff']+manifest['memories']}
    assert len(helpers)==141 and manifest['logical_state_bits']==2656
    assert manifest['parameters']==dict(SYSPER=4)
    assert len(manifest['immutable_proms'])==14
    assert sum(r['words']*r['width']for r in manifest['immutable_proms'])==12032
    title='; Analysis & Synthesis Resource Utilization by Entity'
    start=mapping.find(title);assert start>=0,'missing disk/Ethernet entity table'
    header=None;rows={}
    for line in mapping[start:].split('\n\n',1)[0].splitlines():
        if not line.startswith('; '):continue
        fields=[s.strip()for s in line.split(';')[1:-1]]
        if fields and fields[0]=='Compilation Hierarchy Node':header=fields;continue
        if not header or len(fields)!=len(header):continue
        row=dict(zip(header,fields))
        if ':b_DskEth|'not in row['Full Hierarchy Name']:continue
        match=re.fullmatch(r'\|(\w+):(u_\w+)\|',fields[0])
        if not match:continue
        kind,pkg=match.groups()
        assert pkg not in rows,'duplicate disk/Ethernet entity'
        if pkg in helpers:
            wanted=helpers[pkg]['kind']+('_snapshot'if variant=='snapshot'else'')
            assert kind==wanted,('wrong disk/Ethernet helper',pkg,kind,wanted)
            assert row['Block Memory Bits']=='0','mutable disk/Ethernet file moved to block RAM'
            kind=helpers[pkg]['kind']
        count=re.fullmatch(r'(\d+) \((\d+)\)',row['Dedicated Logic Registers'])
        assert count and count[1]==count[2],'unexpected nested disk/Ethernet state'
        rows[pkg]=dict(kind=kind,registers=int(count[1]),block_memory_bits=int(row['Block Memory Bits']))
    pruned={'u_c01','u_a02','u_a09','u_a03','u_a10','u_a11','u_a07','u_a06'}
    assert len(rows)==210 and set(helpers)-set(rows)==pruned,'unexpected disk/Ethernet child/helper pruning'
    # Quartus selects j02 versus k02 as the surviving hierarchy label for pure
    # combinational mux logic. Neither owns state; all state-bearing names
    # remain exact. Source comparison pins both original mux connections.
    alias='u_j02'if variant=='baseline'else'u_k02'
    assert rows.get(alias)==dict(kind='cell_MC10174',registers=0,block_memory_bits=0),'combinational representative unexpectedly owns state'
    rows['merged_pure_mux']=rows.pop(alias)
    # Strip unrelated physical rows while retaining the original table header.
    physical='\n'.join(line for line in fitted.split('\n')
                       if '|cell_'not in line or ':b_DskEth|'in line)
    roms=dsketh_physical(physical)
    start=mapping.find('; Analysis & Synthesis RAM Summary')
    assert start>=0,'missing disk/Ethernet inferred memory table'
    header=None;inferred={}
    for line in mapping[start:].split('\n\n',1)[0].splitlines():
        if not line.startswith('; '):continue
        fields=[s.strip()for s in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields;continue
        if not header or len(fields)!=len(header)or ':b_DskEth|'not in fields[0]:continue
        row=dict(zip(header,fields))
        match=re.search(r'\|cell_MCM10149:(u_\w+)\|',fields[0])
        assert match and match[1]not in inferred,'unexpected disk/Ethernet inferred RAM'
        for key,value in dict(Type='AUTO',Mode='ROM',Size='1024').items():
            assert row[key]==value,('disk/Ethernet ROM inference differs',key,row[key])
        assert row['Port A Depth']=='256'and row['Port A Width']=='4'
        assert row['Port B Depth']==row['Port B Width']=='--'
        inferred[match[1]]=dict(words=256,width=4)
    assert set(inferred)=={re.search(r'cell_MCM10149:(u_\w+)\|',r['Name'])[1]for r in roms['rows']}
    return dict(state=rows,inferred_roms=inferred,immutable_rom_placement=roms,
        scope='All133 surviving mutable helpers/all210 child entity register counts compared with frozen58; same eight pruned helpers explicitly checked. Admin inactive; original normal-state pruning retained. Nine inferred immutable ROMs in nine separate M10Ks; all14 PROM contents checked independently. Not enabled whole-core snapshot cost.')


def collect(root,baseline,archive):
    core=root/'core';record=json.loads((root/'dsketh-integration.json').read_text())
    metadata=json.loads((root/TARGET/'assets.json').read_text())
    assert sha(Path(__file__).parent/'build_native_dsketh-executed.py')==metadata['builder_sha256'], 'executed builder differs'
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
    # Only report labels change; compare every constraint/script body.
    assert (core/'device-corners.tcl').read_text().replace('native59-','native58-')==(baseline/'core/device-corners.tcl').read_text().replace(str(baseline),str(root)), 'corner script differs'
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
    memd_memories=dict(macros=memd_macros(memd_sections(mapping),memd_manifest),physical={})
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        memd_memories['physical'][variant]=native_cache_physical(
            (project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace'),memd_manifest,'paired38'if variant=='baseline'else'independent40')
    memx_manifest=json.loads((root/'src/fpga/mister/snapshot_native_memx/assets.json').read_text())['manifests']['MemX']
    assert memx_manifest['logical_state_bits']==1378312
    assert memx_manifest['map_history_bits']==483 and len(memx_manifest['rams'])==21
    assert len(memx_manifest['immutable_proms'])==8
    prom_evidence=check_proms(root,baseline,memx_manifest)
    memx_memories={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        project_mapping=(project/'core/output_files/Dorado.map.rpt').read_text()
        project_fitted=(project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace')
        memx_memories[variant]=dict(macros=memx_macros(board_parameters(project_mapping,'MemX'),memx_manifest,'snapshot'),
            physical=memx_physical(map_rows(project_fitted),memx_manifest,'snapshot'))
    dispy_manifest=json.loads((root/'src/fpga/mister/snapshot_native_dispy/assets.json').read_text())['manifests']['DispY']
    assert dispy_manifest['parameters']==dict(SYSPER=16) and dispy_manifest['logical_state_bits']==13993
    assert len(dispy_manifest['immutable_proms'])==2
    prom_evidence+=check_proms(root,baseline,dispy_manifest)
    dispy_memories={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        dispy_memories[variant]=dispy_physical((project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace'),dispy_manifest,'snapshot')
    dispy_word_macros=word_macros(board_parameters(mapping,'DispY'))
    dsketh_manifest=metadata['manifests']['DskEth']
    prom_evidence+=check_proms(root,baseline,dsketh_manifest)
    dsketh_memories={}
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        dsketh_memories[variant]=dsketh_evidence(
            (project/'core/output_files/Dorado.map.rpt').read_text(errors='replace'),
            (project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace'),dsketh_manifest,variant)
    assert dsketh_memories['baseline']['state']==dsketh_memories['snapshot']['state'],'disk/Ethernet normal state retention differs'
    assert dsketh_memories['baseline']['inferred_roms']==dsketh_memories['snapshot']['inferred_roms']
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
            prefix='native59'
            p=core/f'{prefix}-{mode}-{corner}.rpt';reports.append(timed(p.read_text(),p.name))
        current=ucp((core/f'native59-ucp-{corner}.rpt').read_text())
        assert current==ucp((baseline/f'core/native58-ucp-{corner}.rpt').read_text()),'unconstrained coverage changed'
    assert len(reports)==196
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(core/'output_files/Dorado.sta.summary').read_text())
    assert len(checks)==31 and min(float(s)for _,s in checks)>=0,checks
    summary=(core/'output_files/Dorado.fit.summary').read_text()
    assert 'Fitter Status : Successful'in summary
    assert 'Assembler was successful'in (root/'asm.log').read_text()
    crossing_log=(root/'adapter-crossings.log').read_text()
    crossings=re.findall(r'ADAPTER_CROSSING_CORNER (\S+) (conta|contb|proch|procl|ifu|memc|memd|memx|dispy|dsketh) board=(\d+) others=(\d+)',crossing_log)
    assert {(c,b)for c,b,_,_ in crossings}=={(c,b)for c in CORNERS for b in BOARDS} and len(crossings)==40
    assert all(int(a)>0 and int(b)>0 for _,_,a,b in crossings)
    rbf=core/'output_files/Dorado.rbf';assert rbf.stat().st_size>1000000
    resources={}
    for key,label in (('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks'),('dsp_blocks','Total DSP Blocks')):
        m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
        resources[key]=int(m[1].replace(',',''))
    failures=qsf_controls(root,baseline,metadata)
    original=(core/'native59-proch-in-hold-7_slow_1100mv_-40c.rpt').read_text()
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
        ('memd_physical_new_data',memd_fitted,lambda t:native_cache_physical(t,memd_manifest,'independent40')),
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
    # Damage actual row fields through their table header, never unrelated RAM.
    for name,key,value in [('packing','M10K blocks','3'),('implementation_width','Implementation Port A Width','2'),
                           ('location','Location','MLAB_X0_Y0_N0'),('duplicate_location','Location',None),
                           ('unreviewed_sharing','Location','M10K_X0_Y0_N0, M10K_X0_Y1_N0, M10K_X0_Y2_N0')]:
        start=memd_fitted.index('; Fitter RAM Summary')
        lines=memd_fitted[start:].split('\n\n',1)[0].splitlines(keepends=True);header=None
        for i,line in enumerate(lines):
            fields=[x.strip()for x in line.split(';')[1:-1]]
            if fields and fields[0]=='Name':header=fields
            if ':b_MemD|'in line and 'snapshot_bank_3|'in line:
                assert header and len(header)==len(fields)
                col=header.index(key)+1;parts=line.split(';')
                replacement=value if value is not None else ', '.join([fields[header.index(key)].split(',')[0].strip()]*3)
                parts[col]=' '+replacement+' ';lines[i]=';'.join(parts);break
        else:raise AssertionError('missing cache physical fault anchor')
        try:native_cache_physical(''.join(lines),memd_manifest,'independent40')
        except AssertionError as error:failures.append(dict(case='memd_physical_'+name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged cache packing accepted: '+name)
    for variant,project in [('baseline',baseline),('snapshot',root)]:
        parameter_text=board_parameters((project/'core/output_files/Dorado.map.rpt').read_text(),'MemX')
        broken=parameter_text.replace('; OLD_DATA','; NEW_DATA',1)
        assert broken!=parameter_text,'missing map macro fault anchor'
        try:memx_macros(broken,memx_manifest,'snapshot')
        except AssertionError as error:failures.append(dict(case=variant+'_memx_new_data',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged map macro accepted')
        fitted_text=map_rows((project/'core/output_files/Dorado.fit.rpt').read_text(errors='replace'))
        for kind,before,after in [('old_data','; Old data','; New data'),('placement','M10K_X','MLAB_X')]:
            lines=fitted_text.splitlines(keepends=True)
            for i,line in enumerate(lines):
                if ':b_MemX|'in line and '|cell_MosRam'in line and before in line:
                    lines[i]=line.replace(before,after,1);break
            else:raise AssertionError('missing map physical fault anchor: '+kind)
            try:memx_physical(''.join(lines),memx_manifest,'snapshot')
            except AssertionError as error:failures.append(dict(case=variant+'_memx_'+kind,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged physical map accepted')
    damaged=json.loads(json.dumps(memx_manifest))
    damaged['immutable_proms'][0]['sha256']='0'*64
    try:check_proms(root,baseline,damaged)
    except AssertionError as error:failures.append(dict(case='memx_prom_hash',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('wrong compiled PROM accepted')
    selected=board_parameters(mapping,'DispY')
    broken=selected.replace('; OLD_DATA','; NEW_DATA',1);assert broken!=selected
    try:word_macros(broken)
    except AssertionError as error:failures.append(dict(case='dispy_macro_new_data',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged display macro accepted')
    for name,before,after in [('dispy_physical_new_data','; Old data','; New data'),('dispy_physical_placement','M10K_X','MLAB_X')]:
        lines=fitted.splitlines(keepends=True)
        for i,line in enumerate(lines):
            if ':b_DispY|'in line and '|dorado_snapshot_f10415_word_bank:'in line and before in line:
                lines[i]=line.replace(before,after,1);break
        else:raise AssertionError('missing actual display failure anchor')
        try:dispy_physical(''.join(lines),dispy_manifest,'snapshot')
        except AssertionError as error:failures.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged display physical RAM accepted')
    damaged=json.loads(json.dumps(dispy_manifest));damaged['immutable_proms'][0]['sha256']='0'*64
    try:check_proms(root,baseline,damaged)
    except AssertionError as error:failures.append(dict(case='dispy_prom_hash',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged display PROM accepted')
    for name,anchor,before,after in [
        ('dsketh_register','|cell_F10145A_snapshot:u_d08|','74 (74)','73 (73)'),
        ('dsketh_helper','|cell_F10145A_snapshot:u_d08|','cell_F10145A_snapshot','cell_F10145A'),
        ('dsketh_rom_width','|cell_MCM10149:u_b14|',None,None),
    ]:
        lines=mapping.splitlines(keepends=True)
        if name=='dsketh_rom_width':
            start=mapping.index('; Analysis & Synthesis RAM Summary');end=mapping.index('\n\n',start)
            header=None;offset=0
            for i,line in enumerate(lines):
                position=offset;offset+=len(line)
                if not(start<=position<end):continue
                fields=[s.strip()for s in line.split(';')[1:-1]]
                if fields and fields[0]=='Name':header=fields
                if ':b_DskEth|'in line and anchor in line:
                    assert header;parts=line.split(';');parts[header.index('Port A Width')+1]=' 3 ';lines[i]=';'.join(parts);break
            else:raise AssertionError('missing disk/Ethernet ROM failure anchor')
        else:
            for i,line in enumerate(lines):
                if ':b_DskEth|'in line and anchor in line and before in line:
                    lines[i]=line.replace(before,after,1);break
            else:raise AssertionError('missing disk/Ethernet helper failure anchor')
        broken=''.join(lines);assert broken!=mapping
        try:
            evidence=dsketh_evidence(broken,fitted,dsketh_manifest,'snapshot')
            assert evidence['state']==dsketh_memories['baseline']['state'],'disk/Ethernet retained registers differ'
        except AssertionError as error:failures.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged disk/Ethernet evidence accepted: '+name)
    damaged=json.loads(json.dumps(dsketh_manifest));damaged['immutable_proms'][0]['sha256']='0'*64
    try:check_proms(root,baseline,damaged)
    except AssertionError as error:failures.append(dict(case='dsketh_prom_hash',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged disk/Ethernet PROM accepted')
    q=dict(status='PASS',hardware='NOT DEPLOYED',frequency_mhz=50,sysper=4,
        timing_checks=31,path_categories_checked=196,operating_conditions_checked=4,
        minimum_slack_ns=min(r['slack_ns']for r in reports),illegal_clocks=0,unconstrained_clocks=0,
        unconstrained_io_summary=current,extra_timing_exceptions='None',resources=resources,
        unconstrained_io_scope='Unchanged MiSTer framework I/O coverage; all SDRAM and ten adapter crossing categories timed.',
        snapshot_admin='INACTIVE; normal-operation integration only, no machine snapshot or speed qualification',
        change=metadata['scope'],instruction_memories=instruction_ram(mapping),processor_memories=ram_evidence,
        ifu_memories=ifu_memories,ifu_memory_bits=27648,memd_memories=memd_memories,memd_memory_bits=294912,memx_memories=memx_memories,memx_memory_bits=1376256,immutable_proms=prom_evidence,dispy_memories=dispy_memories,dispy_word_macros=dispy_word_macros,dispy_admin="INACTIVE; u_f04 is pruned, equivalent FIFO replicas may share storage. Complete enabled scope separately qualified.",memc_memories=memc_memories,memc_mlab_bits=21504,memc_mlab_cells=672,processor_memory_bits=9216,processor_mlab_cells=288,warning_comparison=warnings,
        failure_controls=failures,reports=reports,rbf_sha256=sha(rbf))
    q['dsketh_memories']=dsketh_memories
    parent=json.loads((baseline/'qualification.json').read_text())
    for name,value in dict(storage_prefetch=True,storage_interleave=True,storage_defer_reads=False,
                           network_ddr=True,raw_line_cache_entries=0,required_sdram_mb=128,
                           guest_ram_mib=8,raw_backing_mib=16).items():
        assert parent[name]==value,'unreviewed baseline memory/transport policy: '+name
        q[name]=value
    assert parent['raw_backend'].startswith('SDRAM')
    q['raw_backend']=parent['raw_backend']
    q['policy_scope']='Inherited from qualified58, with exact unchanged outer wrapper, project assignments and all other source bytes verified above.'
    (root/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    inputs=[p for p in (root/'src').rglob('*')if p.is_file()]
    inputs+=[core/name for name in ('Dorado.sv','Dorado.qsf','Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','device-corners.tcl','audit-adapter-crossings.tcl')]
    (root/'compiled-inputs.json').write_text(json.dumps({str(p.relative_to(root)):sha(p)for p in inputs},indent=2)+'\n')
    items=inputs+[Path(__file__).parent/'build_native_dsketh-executed.py',Path(__file__),root/'qualification.json',root/'compiled-inputs.json',root/'dsketh-integration.json']
    items+=sorted(core.glob('native59-*.rpt'))+[core/'native-unconstrained.rpt']
    items+=[core/'output_files'/name for name in ('Dorado.sta.summary','Dorado.fit.summary','Dorado.map.rpt','Dorado.fit.rpt')]
    items+=[root/(phase+'.log')for phase in ('map','fit','asm','sta','audit','device-corners','adapter-crossings')]
    with tarfile.open(archive,'w:gz')as tar:
        for p in items:
            arcname=str(p.relative_to(root))if p.is_relative_to(root)else'executed-audit/'+p.name
            tar.add(p,arcname=arcname)
    print(json.dumps({k:v for k,v in q.items()if k not in ('reports','processor_memories','instruction_memories')},indent=2))


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project',type=Path,default=Path('/home/alans/mister/Dorado_Native59_20261019'))
    parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native58_20261019'))
    parser.add_argument('--archive',type=Path,default=Path('/tmp/native59-evidence.tar.gz'))
    args=parser.parse_args();collect(args.project.resolve(),args.baseline.resolve(),args.archive)
