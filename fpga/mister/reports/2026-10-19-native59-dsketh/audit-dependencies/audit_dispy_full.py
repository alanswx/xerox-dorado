#!/usr/bin/env python3
"""Read-only actual field-display fit/source/physical-memory/timing audit."""
import argparse
from collections import Counter
import hashlib,json,re,shutil,tempfile
from pathlib import Path
CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
BOARD_SHA='48ae129bd7523b5efb9db87d8f43cc314186f10fc0a4fe52f681b5c345836b4d'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def normalized(s):return '\n'.join(l for l in s.splitlines()if l.strip()and l!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def ram_table(text,m,variant):
    i=text.find('; Fitter RAM Summary');assert i>=0,'missing physical RAM table'
    header=None;rows=[]
    for line in text[i:].split('\n\n',1)[0].splitlines():
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and '|cell_'in f[0]or f and '|dorado_snapshot_f10415_word_bank:'in f[0]:
            assert header and len(header)==len(f);rows.append(dict(zip(header,f)))
    assert len(rows)==12,'wrong display RAM row count';seen=set();sites=set()
    for r in rows:
        matches=[]
        for entry in m['memories']:
            k=entry['kind'];pkg=entry['package']
            if k in ('cell_K1115A','cell_F10145A'):continue
            token=('dorado_snapshot_f10415_word_bank:'+entry['instance']if variant=='snapshot'and k=='cell_F10415A'else k+('_snapshot'if variant=='snapshot'else'')+':'+pkg)+'|'
            if '|'+token in r['Name']:matches.append(entry)
        assert len(matches)==1 and matches[0]['package']not in seen,'duplicate/unknown physical memory';e=matches[0];seen.add(e['package'])
        raw=e['kind']=='cell_F10415A';width=1 if variant=='baseline'and e['package']=='u_f04'else e['width']
        values={'Type':'M10K block'if raw else'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
            'Port A Depth':str(e['words']),'Port B Depth':str(e['words']),'Port A Width':str(width),'Port B Width':str(width),
            'Size':str(width*e['words']),'Implementation Bits':str(width*e['words']),
            'Implementation Port A Depth':str(e['words']),'Implementation Port B Depth':str(e['words']),
            'Implementation Port A Width':str(width),'Implementation Port B Width':str(width),
            'Port A Output Registers':'no','Port B Output Registers':'no',
            'M10K blocks':'1'if raw else'0','MLAB cells':'0'if raw else str(width*8)}
        if raw:values.update({'Port A Input Registers':'yes','Port B Input Registers':'yes','Mixed Width RDW Mode':'Old data'if variant=='snapshot'else"Don't care"})
        else:values.update({'Port A Input Registers':'yes','Port B Input Registers':'no'})
        for k,w in values.items():assert r.get(k)==w,('physical display RAM differs',e['package'],k,r.get(k))
        if raw:
            assert re.fullmatch('M10K_X[0-9]+_Y[0-9]+_N[0-9]+',r['Location']),'display word RAM not M10K'
            if variant=='snapshot':assert r['Location']not in sites,'independent state write ports unexpectedly share a block'
            sites.add(r['Location'])
    assert len(sites)==(3 if variant=='snapshot'else 1),'word RAM packing differs'
    assert sum(int(r['MLAB cells'])for r in rows)==(288 if variant=='snapshot'else 264)
    return rows


def word_macros(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,h in enumerate(headers):
        if '|dorado_snapshot_f10415_word_bank:display_word_'not in h[1]or'altsyncram:'not in h[1]:continue
        section=text[h.end():headers[i+1].start()if i+1<len(headers)else len(text)];values={}
        for k,w in [('OPERATION_MODE','DUAL_PORT'),('WIDTH_A','1'),('WIDTH_B','1'),('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
            a=re.findall(r'; '+k+r'\s*; (\w+)\s*;',section);assert a==[w],('display word macro differs',k,a);values[k]=w
        rows.append(dict(instance=h[1].strip(),parameters=values))
    assert len(rows)==3;return rows


def audit(root):
    p=json.loads((root/'provenance.json').read_text());m=p['manifest'];result={}
    assert p['kind']=='DispY'and m['original_sha256']==p['source_board_sha256']==BOARD_SHA
    assert m==json.loads((root/'qualified-manifest.json').read_text()),'qualified simulation/classification schema differs'
    assert m['parameters']==dict(SYSPER=16)
    assert m['logical_state_bits']==13993 and len(m['ff'])==140 and len(m['memories'])==16
    assert next(x for x in m['memories']if x['kind']=='cell_K1115A')['parameters']==dict(FREQ_KHZ=50000,SYSPER=16)
    assert all(x['ce'].startswith('OMITTED')for x in m['memories']if x['kind']=='cell_F10415A')
    for n,h in p['compiled_files'].items():
        if not n.endswith('.qsf'):assert digest(root/n)==h,'compiled source/constraint differs:'+n
    for v in ('baseline','snapshot'):
        c=root/v;assert digest(c/'DispY_original.v')==BOARD_SHA and digest(c/'DispY_snapshot.sv')==m['generated_sha256']
        for n,h in m['original_sources'].items():assert digest(c/Path(n).name)==h,'original display cell differs'
        for n,h in m['cloned_sources'].items():assert digest(c/n)==h,'qualified display clone differs'
        for asset in m['immutable_proms']:assert digest(c/asset['file'])==asset['sha256'],'display PROM differs'
        assert normalized((c/'snapshot_fit.qsf').read_text())==normalized(p['expected_qsf'][v]),'QSF/macro/constraint changed'
        summary=(c/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for k,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            a=re.search(label+r' : ([\d,]+)',summary);assert a;resources[k]=int(a[1].replace(',',''))
        assert resources['ram_bits']==3072 and resources['ram_blocks']==(3 if v=='snapshot'else 1),'word RAM capacity/packing differs'
        assert {f.name[len('bank-setup-'):-4]for f in c.glob('bank-setup-*.rpt')}==CORNERS
        timing={}
        for corner in sorted(CORNERS):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(c/f'bank-{mode}-{corner}.rpt').read_text();a=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                assert a and int(a[1])==0 and float(a[2])>=0,('display timing violation/missing path',v,corner,mode)
                timing[corner][mode]=float(a[2])
            for mode in ('recovery','removal'):assert(c/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',(c/f'bank-ucp-{corner}.rpt').read_text()),'unconstrained display fit:'+label
        warnings={}
        for mode in ('map','fit'):
            text=(c/(mode+'.log')).read_text();assert'Critical Warning'not in text;warnings[mode]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
        result[v]=dict(resources=resources,timing=timing,warnings=warnings,physical_memories=ram_table((c/'snapshot_fit.fit.rpt').read_text(errors='replace'),m,v))
    for mode in ('map','fit'):
        extra=Counter(result['snapshot']['warnings'][mode])-Counter(result['baseline']['warnings'][mode]);assert not(set(extra)-{'10036','13024','13410'}),('unreviewed display warning increase',mode,extra)
    return dict(status='PASS',scope=p['scope'],results=result,word_macros=word_macros((root/'snapshot/snapshot_fit.map.rpt').read_text()),
        minimum_snapshot_slack_ns=min(x for r in result['snapshot']['timing'].values()for x in r.values()),
        physical_scope='Enabled isolated snapshot retains all9x256x4 MLAB RAMs/3x1024x1 M10K RAMs. Baseline u_f04 prunes3unused bits and packs3word planes into1M10K; snapshot administrative access retains them and requires3independent M10Ks. Original no_rw_check baseline mixed-port mode is DONT_CARE; snapshot uses modeled NBA OLD_DATA, independently tested. No enabled whole-core cost/full snapshot/timer or speed claim.')


def controls(root):
    rows=[]
    for name,path,before,after in [('source','snapshot/DispY_snapshot.sv','state_hold && state_frozen && state_request','state_request'),('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),('constraint','snapshot/bank.sdc','20.000','18.000'),('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),('physical','snapshot/snapshot_fit.fit.rpt','; Old data','; New data'),('placement','snapshot/snapshot_fit.fit.rpt','M10K_X','MLAB_X'),('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),('prom','snapshot/verilog/proms/packages/DispY-i15.mem',None,None)]:
        with tempfile.TemporaryDirectory(prefix='dorado-display-fault-',dir='/tmp')as tmp:
            d=Path(tmp)/'evidence';shutil.copytree(root,d,ignore=shutil.ignore_patterns('db','incremental_db'));f=d/path
            if name=='prom':f.write_bytes(b'0\n')
            else:
                text=f.read_text(errors='replace');assert before in text;f.write_text(text.replace(before,after))
            try:audit(d)
            except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged display fit accepted:'+name)
    return rows

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args();q=audit(a.directory.resolve())
    if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
