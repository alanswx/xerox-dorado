#!/usr/bin/env python3
"""Qualify completed full IFU source, state RAM geometry and all-corner fits."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import shutil
import tempfile

CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
BOARD_SHA='b7f3b3275e8428fe03d6f3f645259a67205bcc382223267578503d492667d161'
PROM_SHA='691f366c2aeaf7241f9b30868f225b0a49508a0954c23e561db73bcf9ac0fc0c'


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def normalized(text):
    return '\n'.join(line for line in text.splitlines()if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def memories(text,manifest):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,h in enumerate(headers):
        match=re.search(r'\bsnapshot_bank_(\d+)\|',h[1])
        if not match or 'altsyncram:'not in h[1]:continue
        bank=int(match[1]);assert bank<len(manifest['banks']),'unreviewed IFUM macro bank'
        width=manifest['banks'][bank]['width']
        section=text[h.end():headers[i+1].start()if i+1<len(headers)else len(text)];values={}
        for key,wanted in [('OPERATION_MODE','DUAL_PORT'),('RAM_BLOCK_TYPE','M10K'),('WIDTH_A',str(width)),('WIDTH_B',str(width)),
                           ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
            actual=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section)
            assert actual==[wanted],('IFUM macro differs',bank,key,actual)
            values[key]=wanted
        rows.append(dict(bank=bank,instance=h[1].strip(),parameters=values))
    assert len(rows)==11 and {r['bank']for r in rows}==set(range(11)),'missing/extra IFUM macro'
    return rows


def physical_ram(text,manifest):
    i=text.find('; Fitter RAM Summary');assert i>=0,'missing physical RAM table'
    table=text[i:].split('\n\n',1)[0];header=None;rows=[]
    for line in table.splitlines():
        if not line.startswith('; '):continue
        fields=[x.strip()for x in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields
        elif fields and 'snapshot_bank_'in fields[0]:
            assert header and len(header)==len(fields),'malformed physical RAM row'
            rows.append(dict(zip(header,fields)))
    assert len(rows)==11,'physical IFUM RAM count differs'
    seen=set()
    for row in rows:
        m=re.search(r'\bsnapshot_bank_(\d+)\|',row['Name']);assert m,'unknown physical IFUM bank'
        bank=int(m[1]);assert bank<11 and bank not in seen,'duplicate/out-of-range physical IFUM bank'
        seen.add(bank);width=manifest['banks'][bank]['width']
        for key,wanted in {'Type':'M10K block','Mode':'Simple Dual Port','Port A Depth':'1024','Port B Depth':'1024',
            'Port A Width':str(width),'Port B Width':str(width),'Size':str(width*1024),
            'Implementation Bits':str(width*1024),'M10K blocks':'1','MLAB cells':'0',
            'Clock Mode':'Single Clock','Port A Input Registers':'yes','Port A Output Registers':'no',
            'Port B Input Registers':'yes','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}.items():
            assert row.get(key)==wanted,('physical IFUM RAM differs',bank,key,row.get(key))
    assert seen==set(range(11));return rows


def audit(directory):
    record=json.loads((directory/'provenance.json').read_text());manifest=record['manifest'];results={}
    assert manifest['original_sha256']==BOARD_SHA and manifest['logical_state_bits']==28653
    assert len(manifest['ff'])==94 and len(manifest['banks'])==11 and manifest['phase']['width']==4
    assert manifest['index_bits']==18 and manifest['row_bits']==11
    assert [r['width']for r in manifest['banks']]==[1,5,1,1,4,1,5,2,1,4,2]
    assert all(r['ce']=='OMITTED'for r in manifest['banks'])
    for name,expected in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(directory/name)==expected,'compiled input differs: '+name
    for variant in ('baseline','snapshot'):
        case=directory/variant
        assert digest(case/'IFU_original.v')==BOARD_SHA
        assert digest(case/'IFU_snapshot.sv')==manifest['generated_sha256']
        for name,expected in manifest['cloned_sources'].items():assert digest(case/name)==expected,'qualified clone differs'
        for name,expected in manifest['original_sources'].items():assert digest(case/Path(name).name)==expected,'qualified original cell differs'
        assert manifest['immutable_proms'][0]['sha256']==PROM_SHA and digest(case/manifest['immutable_proms'][0]['file'])==PROM_SHA
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/SDC changed'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
            resources[key]=int(m[1].replace(',',''))
        assert resources['ram_bits']==27648 and resources['ram_blocks']==(11 if variant=='snapshot'else 4),'RAM geometry/packing differs'
        assert {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==CORNERS,'missing corner'
        timing={}
        for corner in sorted(CORNERS):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text();m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                assert m and int(m[1])==0 and float(m[2])>=0,'timing violation/missing path '+mode+'/'+corner
                timing[corner][mode]=float(m[2])
            for mode in ('recovery','removal'):
                assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
            text=(case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',text),'unconstrained '+label
        warnings={}
        for phase in ('map','fit'):
            text=(case/(phase+'.log')).read_text();assert 'Critical Warning'not in text,'critical warning'
            warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
        row=dict(resources=resources,timing=timing,warnings=warnings)
        if variant=='snapshot':
            row['macros']=memories((case/'snapshot_fit.map.rpt').read_text(),manifest)
            row['physical_memories']=physical_ram((case/'snapshot_fit.fit.rpt').read_text(errors='replace'),manifest)
        results[variant]=row
    for phase in ('map','fit'):
        new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(new)-{'10036','13024','13410'}),('unreviewed warning increase',phase,new)
    return dict(status='PASS',scope=record['scope'],results=results,
        delta={key:results['snapshot']['resources'][key]-results['baseline']['resources'][key]for key in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for row in results['snapshot']['timing'].values()for v in row.values()),
        state_count_scope='28653 modeled mutable bits/256immutable PROM bits; resource deltas include shell/admin/otherwise-pruned setup/history, not a full-core snapshot budget.')


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/IFU_snapshot.sv','state_hold && state_frozen && state_request','state_request'),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 11','Total RAM Blocks : 12'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('missing_corner','snapshot/bank-setup-7_slow_1100mv_100c.rpt',None,None),
        ('warning','snapshot/map.log',None,None),
        ('prom','snapshot/verilog/proms/packages/IFU-a06.mem',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-ifu-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out,ignore=shutil.ignore_patterns('db','incremental_db'))
            p=out/path
            if name=='missing_corner':p.unlink()
            elif name=='prom':p.write_bytes(b'00\n')
            else:
                text=p.read_text()
                if before:assert before in text; text=text.replace(before,after)
                else:text+='\nWarning (10999): unreviewed inference\n'
                p.write_text(text)
            try:audit(out)
            except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged full IFU fit accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('directory',type=Path)
    parser.add_argument('--output',type=Path,required=True);parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();result=audit(args.directory.resolve())
    if args.negative_controls:result['failure_controls']=controls(args.directory.resolve())
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
