#!/usr/bin/env python3
"""Read-only nine-family primitive qualification: retained state, source, N4, timing."""
import argparse,hashlib,json,re,shutil,tempfile
from collections import Counter
from pathlib import Path
import generate_baseboard_ff as gen

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def normalized(text):
    return '\n'.join(line for line in text.splitlines()if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def retained_state(text,manifest):
    start=text.find('; Analysis & Synthesis Resource Utilization by Entity');assert start>=0,'missing synthesis state table'
    table=text[start:].split('\n\n',1)[0];header=None;rows=[]
    for line in table.splitlines():
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Compilation Hierarchy Node':header=f
        elif f and '_snapshot:case'in f[0]:
            assert header and len(header)==len(f),'malformed synthesis state row';rows.append(dict(zip(header,f)))
    assert len(rows)==9,'retained family count differs';seen=set();result=[]
    for row in rows:
        k=row['Entity Name'].removesuffix('_snapshot');assert k in manifest and k not in seen,'duplicate/unknown retained family';seen.add(k)
        w=manifest[k]['logical_state_bits'];assert row['Dedicated Logic Registers']==f'{w} ({w})','state registers dropped/added'
        assert row['Block Memory Bits']=='0','state moved into unreviewed memory'
        result.append(dict(kind=k,retained_state_bits=w,hierarchy=row['Full Hierarchy Name']))
    assert sum(r['retained_state_bits']for r in result)==70,'retained state total differs'
    return result


def audit(directory):
    record=json.loads((directory/'provenance.json').read_text());results={}
    assert set(record['manifest'])==set(gen.FIELDS),'family scope differs'
    assert sum(m['logical_state_bits']for m in record['manifest'].values())==70,'state count differs'
    for k,m in record['manifest'].items():
        assert m==gen.generate_one(k)[1],'source/schema/binding differs'
        for v in ('baseline','snapshot'):
            assert digest(directory/v/(k+'.v'))==gen.SHA[k],'original source differs'
            assert digest(directory/v/(k+'_snapshot.sv'))==m['generated_sha256'],'qualified adapter differs'
    for name,sha in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(directory/name)==sha,'compiled input differs: '+name
    corners={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
    for variant in ('baseline','snapshot'):
        case=directory/variant
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/constraint differs'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing '+key
            resources[key]=int(m[1].replace(',',''))
        assert resources['ram_blocks']==0,'unexpected RAM inference'
        assert {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==corners
        timing={}
        for corner in sorted(corners):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text()
                m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
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
        mapping=(case/'snapshot_fit.map.rpt').read_text(encoding='latin1');param=re.search(r'; Parameter Settings for User Entity Instance: cell_CLOCKGEN(?:_snapshot)?:case8\.cell_inst(.*?)(?:Note:)',mapping,re.S)
        assert param and re.search(r'; N\s*; 4\s*;',param[1]),'actual CLOCKGEN N4 differs'
        assert '; Fitter RAM Summary'not in(case/'snapshot_fit.fit.rpt').read_text(encoding='latin1'),'unexpected physical RAM'
        row=dict(resources=resources,timing=timing,warnings=warnings)
        if variant=='snapshot':row['retained_state']=retained_state(mapping,record['manifest'])
        results[variant]=row
    for phase in ('map','fit'):
        new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(new)-{'10036','13024','13410'}),('unreviewed warnings',phase,new)
    return dict(status='PASS',scope=record['scope'],results=results,
        delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k]for k in ('alms','registers','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for row in results['snapshot']['timing'].values()for v in row.values()))


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/cell_SN74LS259_snapshot.sv','if(state_hold)','if(1\'b0)'),
        ('original','snapshot/cell_CLOCKGEN.v','parameter integer N = 16','parameter integer N = 4'),
        ('state_width','snapshot/snapshot_fit.map.rpt','25 (25)','24 (24)'),
        ('binding','snapshot/snapshot_fit.map.rpt',None,None),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-dsketh-ff-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out,ignore=shutil.ignore_patterns('db','incremental_db'));p=out/path;text=p.read_text(encoding='latin1')
            if name=='binding':
                h=re.search(r'; Parameter Settings for User Entity Instance: cell_CLOCKGEN_snapshot:case8\.cell_inst(.*?)(?:Note:)',text,re.S);assert h
                part,n=re.subn(r'(; N\s*; )4(\s*;)',r'\g<1>8\2',h[1]);assert n==1;text=text[:h.start(1)]+part+text[h.end(1):]
            elif before:assert before in text; text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed inference\n'
            p.write_text(text,encoding='latin1')
            try:audit(out)
            except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged BaseBoard FF fit evidence accepted: '+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args();r=audit(a.directory.resolve())
    if a.negative_controls:r['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(r,indent=2)+'\n');print('BASEBOARD_FF FIT AUDIT PASS;minimum slack',r['minimum_snapshot_slack_ns'])
