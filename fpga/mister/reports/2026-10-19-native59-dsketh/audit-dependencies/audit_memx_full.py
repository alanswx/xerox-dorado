#!/usr/bin/env python3
"""Read-only complete MemX fit audit: sources,21physical map planes,timing."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import shutil
import tempfile

CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
BOARD_SHA='709e14c98631a0923251f7a93ba5075fbecf27b6dac3347f52086c2b513c1816'


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def normalized(s):return '\n'.join(l for l in s.splitlines()if l.strip()and l!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def memories(text,manifest,variant):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    cell='cell_MosRam'+('_snapshot'if variant=='snapshot'else'');expected={r['package']for r in manifest['rams']}
    for i,h in enumerate(headers):
        m=re.search(r'\b'+cell+r':(u_\w+)\|',h[1])
        if not m or 'altsyncram:'not in h[1]:continue
        package=m[1];assert package in expected,'unknown map macro socket'
        section=text[h.end():headers[i+1].start()if i+1<len(headers)else len(text)];values={}
        for key,wanted in [('OPERATION_MODE','DUAL_PORT'),('RAM_BLOCK_TYPE','AUTO'),('WIDTH_A','1'),('WIDTH_B','1'),
            ('NUMWORDS_A','65536'),('NUMWORDS_B','65536'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
            actual=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section);assert actual==[wanted],('map macro differs',package,key,actual);values[key]=wanted
        rows.append(dict(package=package,instance=h[1].strip(),parameters=values))
    assert len(rows)==21 and {r['package']for r in rows}==expected,'missing/extra map macro'
    return rows


def physical_ram(text,manifest,variant):
    i=text.find('; Fitter RAM Summary');assert i>=0,'missing physical RAM table'
    table=text[i:].split('\n\n',1)[0];header=None;rows=[];cell='cell_MosRam'+('_snapshot'if variant=='snapshot'else'')
    for l in table.splitlines():
        if not l.startswith('; '):continue
        f=[x.strip()for x in l.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and re.search(r'\b'+cell+r':u_\w+\|',f[0]):
            assert header and len(header)==len(f),'bad physical map row';rows.append(dict(zip(header,f)))
    expected={r['package']for r in manifest['rams']};seen=set();locations=set()
    assert len(rows)==21,'physical map plane count differs'
    for r in rows:
        package=re.search(r'\b'+cell+r':(u_\w+)\|',r['Name'])[1]
        assert package in expected and package not in seen,'duplicate/unknown physical map plane';seen.add(package)
        values={'Type':'AUTO','Mode':'Simple Dual Port','Clock Mode':'Single Clock','Port A Depth':'65536','Port B Depth':'65536',
            'Port A Width':'1','Port B Width':'1','Size':'65536','Implementation Bits':'65536','M10K blocks':'8','MLAB cells':'0',
            'Implementation Port A Depth':'65536','Implementation Port B Depth':'65536','Implementation Port A Width':'1','Implementation Port B Width':'1',
            'Port A Input Registers':'yes','Port B Input Registers':'yes','Port A Output Registers':'no','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}
        for k,w in values.items():assert r.get(k)==w,('physical map plane differs',package,k,r.get(k))
        sites=[x.strip()for x in r['Location'].split(',')]
        assert len(sites)==8 and len(set(sites))==8 and not locations.intersection(sites),'missing/aliased physical M10K locations'
        assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',x)for x in sites),'non-M10K map placement'
        locations.update(sites)
    assert seen==expected and len(locations)==168;return rows


def audit(root):
    p=json.loads((root/'provenance.json').read_text());m=p['manifest'];result={}
    assert m['original_sha256']==BOARD_SHA and m['logical_state_bits']==1378312
    assert len(m['ff'])==74 and len(m['files'])==10 and len(m['rams'])==21 and m['phase']['width']==14
    assert len(m['immutable_proms'])==8 and m['index_bits']==24 and m['row_bits']==17
    assert m['map_history_bits']==483 and all(r['address_bits']==16 and r['words']==65536 for r in m['rams'])
    for n,h in p['compiled_files'].items():
        if not n.endswith('.qsf'):assert digest(root/n)==h,'compiled input differs:'+n
    for v in ('baseline','snapshot'):
        c=root/v;assert digest(c/'MemX_original.v')==BOARD_SHA and digest(c/'MemX_snapshot.sv')==m['generated_sha256']
        for n,h in m['cloned_sources'].items():assert digest(c/n)==h,'qualified clone differs'
        for n,h in m['original_sources'].items():assert digest(c/Path(n).name)==h,'original cell differs'
        for prom in m['immutable_proms']:assert digest(c/prom['file'])==prom['sha256'],'immutable PROM differs'
        assert normalized((c/'snapshot_fit.qsf').read_text())==normalized(p['expected_qsf'][v]),'QSF/macro/SDC changed'
        summary=(c/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for k,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            match=re.search(label+r' : ([\d,]+)',summary);assert match,'missing resource:'+k;resources[k]=int(match[1].replace(',',''))
        assert resources['ram_bits']==1376256 and resources['ram_blocks']==168,'map RAM geometry/packing differs'
        assert {f.name[len('bank-setup-'):-4]for f in c.glob('bank-setup-*.rpt')}==CORNERS,'missing corner'
        timing={}
        for corner in sorted(CORNERS):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(c/f'bank-{mode}-{corner}.rpt').read_text();match=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                assert match and int(match[1])==0 and float(match[2])>=0,'timing violation:'+v+'/'+mode+'/'+corner
                timing[corner][mode]=float(match[2])
            for mode in ('recovery','removal'):assert(c/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
            text=(c/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',text),'unconstrained:'+label
        warnings={}
        for label in ('map','fit'):
            text=(c/(label+'.log')).read_text();assert 'Critical Warning'not in text,'critical warning';warnings[label]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
        result[v]=dict(resources=resources,timing=timing,warnings=warnings,macros=memories((c/'snapshot_fit.map.rpt').read_text(),m,v),
            physical=physical_ram((c/'snapshot_fit.fit.rpt').read_text(errors='replace'),m,v))
    for label in ('map','fit'):
        new=Counter(result['snapshot']['warnings'][label])-Counter(result['baseline']['warnings'][label]);assert not(set(new)-{'10036','13024','13410'}),('unreviewed warning increase',label,new)
    return dict(status='PASS',scope=p['scope'],results=result,minimum_snapshot_slack_ns=min(x for row in result['snapshot']['timing'].values()for x in row.values()),
        delta={k:result['snapshot']['resources'][k]-result['baseline']['resources'][k]for k in ('alms','registers','ram_bits','ram_blocks')},
        state_scope='1378312 modeled mutable bits/2048immutable PROM bits; resource counts include shell/admin/pruning, not an enabled whole-core snapshot budget')


def controls(root):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/MemX_snapshot.sv','state_hold && state_frozen && state_request','state_request'),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),
        ('physical_old_data','snapshot/snapshot_fit.fit.rpt','; Old data','; New data'),
        ('physical_location','snapshot/snapshot_fit.fit.rpt','M10K_X','MLAB_X'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 168','Total RAM Blocks : 167'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('prom','snapshot/verilog/proms/packages/MemX-g15.mem',None,None),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-memx-control-',dir='/tmp')as tmp:
            d=Path(tmp)/'evidence';shutil.copytree(root,d,ignore=shutil.ignore_patterns('db','incremental_db'));f=d/path
            if name=='prom':f.write_bytes(b'00\n')
            else:
                s=f.read_text(errors='replace')
                if before:assert before in s,'missing fault anchor:'+name;s=s.replace(before,after)
                else:s+='\nWarning (10999): unreviewed inference\n'
                f.write_text(s)
            try:audit(d)
            except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged MemX evidence accepted:'+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--negative-controls',action='store_true');a=p.parse_args();q=audit(a.directory.resolve())
    if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
