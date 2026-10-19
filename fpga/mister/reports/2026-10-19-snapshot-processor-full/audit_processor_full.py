#!/usr/bin/env python3
"""Audit complete processor-model fit sources, MLAB placement and timing."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile

import fit_processor_full as fit
import generate_processor_full as gen

CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c',
         'MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}


def require(ok,why):
    if not ok:raise ValueError(why)


def normalized_qsf(text):
    return '\n'.join(line for line in text.splitlines() if line.strip() and
        line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def ram_table(text,title,manifest,variant):
    start=text.find('; '+title);require(start>=0,'missing RAM table: '+title)
    lines=text[start:].split('\n\n',1)[0].splitlines();header=None;rows=[]
    for line in lines:
        if not line.startswith('; '):continue
        fields=[x.strip() for x in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields
        elif fields and '|cell_' in fields[0]:
            require(header is not None and len(fields)==len(header),'bad RAM table row')
            rows.append(dict(zip(header,fields)))
    require(len(rows)==len(manifest['rams']),'unexpected RAM count')
    expected={r['package']:r for r in manifest['rams']};seen=set()
    for row in rows:
        hits=[name for name,r in expected.items() if '|'+r['kind']+('_snapshot' if variant=='snapshot' else '')+':'+name+'|' in row['Name']]
        require(len(hits)==1 and hits[0]not in seen,'unexpected/duplicated RAM instance')
        name=hits[0];seen.add(name);width=expected[name]['width']
        values={'Type':'MLAB','Mode':'Simple Dual Port','Port A Depth':'256','Port B Depth':'256',
                'Port A Width':str(width),'Port B Width':str(width),'Size':str(256*width)}
        if title=='Fitter RAM Summary':
            values.update({'Clock Mode':'Single Clock','Port A Output Registers':'no',
                'Port B Input Registers':'no','Port B Output Registers':'no',
                'Implementation Bits':str(256*width),'M10K blocks':'0','MLAB cells':str(8*width)})
        for key,value in values.items():
            require(row.get(key)==value,'RAM geometry/placement differs: '+name+'/'+key)
    require(seen==set(expected),'missing modeled RAM instance')
    return rows


def audit(directory):
    provenance=json.loads((directory/'provenance.json').read_text());kind=provenance['kind']
    for name,expected in provenance['source_hashes'].items():
        require(fit.digest(gen.ROOT/name)==expected,'source provenance differs: '+name)
    for name,expected in provenance['compiled_files'].items():
        if not name.endswith('.qsf'):
            require(fit.digest(directory/name)==expected,'compiled input differs: '+name)
    source=directory/'baseline/Processor_original.v';board,manifest,clones=gen.generate(kind,source)
    require(manifest==provenance['manifest'],'modeled state manifest differs')
    require(fit.digest(source)==provenance['source_board_sha256'],'original board differs')
    results={}
    with tempfile.TemporaryDirectory(prefix='dorado-processor-inputs-',dir='/tmp') as temp:
        reference=Path(temp)/'inputs';fit.prepare(kind,source,reference)
        for variant in ('baseline','snapshot'):
            case=directory/variant
            require(normalized_qsf((case/'snapshot_fit.qsf').read_text())==normalized_qsf((reference/variant/'snapshot_fit.qsf').read_text()),'QSF source/macro/constraint differs')
            require((case/'Processor_snapshot.sv').read_text()==board,'fitted adapter differs')
            for name,text in clones.items():
                require((case/name).read_text()==text,'fitted clone differs: '+name)
            summary=(case/'snapshot_fit.fit.summary').read_text()
            require('Fitter Status : Successful' in summary,'unsuccessful fit')
            row={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                              ('block_ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                m=re.search(label+r' : ([\d,]+)',summary);require(m is not None,'missing resource: '+key)
                row[key]=int(m[1].replace(',',''))
            require(row['block_ram_bits']==row['ram_blocks']==0,'unexpected M10K placement')
            mapping=(case/'snapshot_fit.map.rpt').read_text(errors='replace')
            fitted=(case/'snapshot_fit.fit.rpt').read_text(errors='replace')
            row['memory_tables']={title:ram_table(text,title,manifest,variant) for title,text in
                [('Analysis & Synthesis RAM Summary',mapping),('Fitter RAM Summary',fitted)]}
            m=re.search(r'; Total MLAB memory bits\s*; ([\d,]+)\s*;',fitted)
            require(m is not None and int(m[1].replace(',',''))==4608,'physical MLAB state bits differ')
            require({p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}==CORNERS,'missing timing condition')
            row['timing']={}
            for corner in sorted(CORNERS):
                checks={}
                for mode in ('setup','hold','mpw'):
                    text=(case/f'bank-{mode}-{corner}.rpt').read_text()
                    m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                    require(m is not None,'missing timed paths: '+mode+'/'+corner)
                    require(int(m[1])==0 and float(m[2])>=0,'timing violation: '+mode+'/'+corner)
                    checks[mode]=float(m[2])
                for mode in ('recovery','removal'):
                    require((case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected asynchronous path')
                text=(case/f'bank-ucp-{corner}.rpt').read_text()
                for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports',
                              'Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    require(re.search('; '+label+r'\s*; 0\s*; 0\s*;',text)is not None,'unconstrained paths: '+label)
                row['timing'][corner]=checks
            row['warning_counts']={}
            for phase in ('map','fit'):
                text=(case/(phase+'.log')).read_text(errors='replace')
                require(not re.search(r'^\s*Critical Warning',text,re.M),'critical '+phase+' warning')
                row['warning_counts'][phase]=dict(Counter(re.findall(r'^\s*Warning \((\d+)\):',text,re.M)))
            results[variant]=row
    for phase in ('map','fit'):
        before=Counter(results['baseline']['warning_counts'][phase]);after=Counter(results['snapshot']['warning_counts'][phase])
        added=after-before
        # Access retains otherwise-pruned histories and baseline admin outputs
        # are constant. Only those known unused/stuck-output warning deltas vary.
        for code in ('10036','13024','13410'):added.pop(code,None)
        require(not added,'unreviewed warning increase: '+phase+'/'+str(added))
    return dict(status='PASS',kind=kind,scope=provenance['scope'],results=results,
        delta={k:results['snapshot'][k]-results['baseline'][k] for k in ('alms','registers','block_ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for checks in results['snapshot']['timing'].values() for v in checks.values()),
        state_count_scope=f"{manifest['logical_state_bits']} modeled mutable bits, not fitted register count;512 immutable PROM bits pinned separately. Counts include shell/admin pruning.",
        limits='Isolated board; no full-core clock distribution, global barriers, physical snapshots, original60ns cadence or speed qualification')


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/Processor_snapshot.sv','wire state_hold=state_freeze && state_guest_held;',"wire state_hold=1'b0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('qsf','snapshot/snapshot_fit.qsf','DORADO_F10145_LOGIC=1','DORADO_F10145_LOGIC=0'),
        ('mlab','snapshot/snapshot_fit.map.rpt','; MLAB ;','; M10K ;'),
        ('physical_mlab','snapshot/snapshot_fit.fit.rpt','; MLAB ;','; M10K ;'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).  Worst case slack is','(1 violated).  Worst case slack is'),
        ('extra_warning','snapshot/map.log','Analysis & Synthesis','Analysis & Synthesis'),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-processor-control-',dir='/tmp') as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out)
            target=out/path;text=target.read_text(errors='replace')
            if name=='extra_warning':text+='\nWarning (10999): unreviewed RAM inference failure\n'
            else:
                require(before in text,'missing control anchor: '+name);text=text.replace(before,after)
            target.write_text(text)
            try:audit(out)
            except ValueError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise ValueError('damaged processor fit evidence accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();result=audit(args.directory.resolve())
    if args.negative_controls:result['failure_controls']=controls(args.directory.resolve())
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
