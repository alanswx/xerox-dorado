#!/usr/bin/env python3
"""Read-only audit of terminal matched MemC fits and qualified modeled state."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import shutil
import tempfile

CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
BOARD_SHA='ed1b7f6cf85f6f7382024ba9bb3337a0a78cff5fea6ee7bcfc86b248b3cab54b'


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def normalized(text):
    return '\n'.join(x for x in text.splitlines()if x.strip()and x!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def ram_table(text,title,manifest,variant):
    start=text.find('; '+title);assert start>=0,'missing '+title
    lines=text[start:].split('\n\n',1)[0].splitlines();header=None;rows=[]
    for line in lines:
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and '|cell_'in f[0]:
            assert header and len(header)==len(f),'bad RAM table row'
            rows.append(dict(zip(header,f)))
    expected={r['package']:r for r in manifest['rams']};seen=set()
    assert len(rows)==len(expected)==21,'wrong RAM count'
    for row in rows:
        hits=[n for n in expected if '|cell_MB7071H'+('_snapshot'if variant=='snapshot'else'')+':'+n+'|'in row['Name']]
        assert len(hits)==1 and hits[0]not in seen,'duplicate/unexpected RAM'
        seen.add(hits[0])
        values={'Type':'MLAB','Mode':'Simple Dual Port','Port A Depth':'256','Port B Depth':'256',
                'Port A Width':'4','Port B Width':'4','Size':'1024'}
        if title=='Fitter RAM Summary':values.update({'Clock Mode':'Single Clock',
            'Port A Output Registers':'no','Port B Input Registers':'no','Port B Output Registers':'no',
            'Implementation Bits':'1024','M10K blocks':'0','MLAB cells':'32'})
        for key,want in values.items():assert row.get(key)==want,('RAM geometry differs',hits[0],key,row.get(key))
    assert seen==set(expected);return rows


def audit(root):
    p=json.loads((root/'provenance.json').read_text());m=p['manifest']
    qualified=json.loads((root/'qualified-manifest.json').read_text())
    assert m==qualified,'qualified modeled schema differs'
    assert p['kind']=='MemC'and m['original_sha256']==p['source_board_sha256']==BOARD_SHA
    assert m['logical_state_bits']==23718 and m['ff_bits']==507 and m['phase_bits']==5
    assert len(m['ff'])==63 and len(m['files'])==23 and len(m['rams'])==21 and not m['immutable_proms']
    for name,sha in p['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(root/name)==sha,'compiled input differs: '+name
    results={}
    for variant in ('baseline','snapshot'):
        case=root/variant
        assert digest(case/'MemC_original.v')==BOARD_SHA
        assert digest(case/'MemC_snapshot.sv')==m['generated_sha256']
        for cell,sha in m['original_sources'].items():assert digest(case/(cell+'.v'))==sha,'original cell differs'
        for name,sha in m['cloned_sources'].items():assert digest(case/name)==sha,'qualified clone differs'
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(p['expected_qsf'][variant]),'QSF/macro/constraint differs'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                          ('block_ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            match=re.search(label+r' : ([\d,]+)',summary);assert match,'missing resource '+key
            resources[key]=int(match[1].replace(',',''))
        assert resources['block_ram_bits']==resources['ram_blocks']==0,'unexpected M10K'
        memories={title:ram_table((case/file).read_text(errors='replace'),title,m,variant)
            for file,title in [('snapshot_fit.map.rpt','Analysis & Synthesis RAM Summary'),('snapshot_fit.fit.rpt','Fitter RAM Summary')]}
        fitted=(case/'snapshot_fit.fit.rpt').read_text(errors='replace')
        match=re.search(r'; Total MLAB memory bits\s*; ([\d,]+)\s*;',fitted)
        assert match and int(match[1].replace(',',''))==21504,'MLAB total differs'
        assert {f.name[len('bank-setup-'):-4]for f in case.glob('bank-setup-*.rpt')}==CORNERS,'missing corner'
        timing={}
        for corner in sorted(CORNERS):
            checks={}
            for mode in ('setup','hold','mpw'):
                t=(case/f'bank-{mode}-{corner}.rpt').read_text()
                match=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',t)
                assert match and int(match[1])==0 and float(match[2])>=0,'timing failure '+variant+'/'+mode+'/'+corner
                checks[mode]=float(match[2])
            for mode in ('recovery','removal'):
                assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
            t=(case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',t),'unconstrained '+label
            timing[corner]=checks
        warnings={}
        for phase in ('map','fit'):
            t=(case/(phase+'.log')).read_text(errors='replace');assert 'Critical Warning'not in t,'critical warning'
            warnings[phase]=dict(Counter(re.findall(r'^\s*Warning \((\d+)\):',t,re.M)))
        results[variant]=dict(resources=resources,memories=memories,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(new)-{'10036','13024','13410'}),('unreviewed warning increase',phase,new)
    return dict(status='PASS',scope=p['scope'],results=results,
        delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k]for k in results['baseline']['resources']},
        minimum_snapshot_slack_ns=min(v for checks in results['snapshot']['timing'].values()for v in checks.values()),
        physical_mlab_bits=21504,physical_mlab_cells=672,
        limits='23718 modeled mutable bits, counts include shell/admin/pruning. No full-core integration/barrier, physical machine snapshot or speed claim.')


def controls(root):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/MemC_snapshot.sv','wire state_hold=state_freeze && state_guest_held;',"wire state_hold=1'b0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('qsf','snapshot/snapshot_fit.qsf','DORADO_F10145_LOGIC=1','DORADO_F10145_LOGIC=0'),
        ('mlab','snapshot/snapshot_fit.map.rpt','; MLAB ;','; M10K ;'),
        ('physical_mlab','snapshot/snapshot_fit.fit.rpt','; MLAB ;','; M10K ;'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
        ('schema','qualified-manifest.json','"logical_state_bits": 23718','"logical_state_bits": 23719'),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-memc-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(root,out);target=out/path;text=target.read_text(errors='replace')
            if before:assert before in text,'missing control anchor '+name;text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed RAM inference\n'
            target.write_text(text)
            try:audit(out)
            except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('corrupted MemC fit accepted: '+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();q=audit(a.directory.resolve())
    if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
