#!/usr/bin/env python3
"""Check routed async RAM placement, source provenance and all timing corners."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile

import fit_async256 as fit
import generate_async256 as gen
CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c',
         'MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}


def normalized_qsf(text):
    return '\n'.join(line for line in text.splitlines() if line.strip() and
        line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def require(ok, why):
    if not ok:
        raise ValueError(why)


def table(text, title):
    start=text.find('; '+title)
    require(start>=0,'missing actual table: '+title)
    lines=text[start:].split('\n\n',1)[0].splitlines()
    header=None;rows=[]
    for line in lines:
        if not line.startswith('; '):continue
        fields=[x.strip() for x in line.split(';')[1:-1]]
        if fields and fields[0]=='Name':header=fields
        elif fields and fields[0].startswith('cell_'):
            require(header is not None and len(fields)==len(header),'bad RAM table row')
            rows.append(dict(zip(header,fields)))
    require(len(rows)==2,'expected exactly two RAMs')
    return rows


def memory_tables(mapping, fitted, variant):
    result={}
    for title,text in [('Analysis & Synthesis RAM Summary',mapping),('Fitter RAM Summary',fitted)]:
        rows=table(text,title);seen=set()
        for row in rows:
            kind=next((k for k in gen.SPECS if row['Name'].startswith('cell_'+k+('_snapshot' if variant=='snapshot' else '')+':')),None)
            require(kind is not None and kind not in seen,'unexpected/duplicated physical RAM')
            seen.add(kind);width=gen.SPECS[kind]['width']
            expected={'Type':'MLAB','Mode':'Simple Dual Port','Port A Depth':'256',
                      'Port B Depth':'256','Port A Width':str(width),'Port B Width':str(width),
                      'Size':str(256*width)}
            if title=='Fitter RAM Summary':
                expected.update({'Clock Mode':'Single Clock','Port A Output Registers':'no',
                    'Port B Input Registers':'no','Port B Output Registers':'no',
                    'Implementation Bits':str(256*width),'M10K blocks':'0',
                    'MLAB cells':str(8*width)})
            for key,value in expected.items():
                require(row.get(key)==value,'RAM geometry/placement differs: '+title+'/'+kind+'/'+key)
        require(seen==set(gen.SPECS),'missing RAM family')
        result[title]=rows
    match=re.search(r'; Total MLAB memory bits\s*; ([\d,]+)\s*;',fitted)
    require(match is not None and int(match[1].replace(',',''))==1280,'physical MLAB state bits differ')
    return result


def audit(directory):
    provenance=json.loads((directory/'provenance.json').read_text())
    require(provenance['manifests']=={k:gen.generate(k)[1] for k in gen.SPECS},'generated manifest differs')
    for name,expected in provenance['source_hashes'].items():
        require(fit.digest(gen.ROOT/name)==expected,'source provenance differs: '+name)
    for name,expected in provenance['compiled_files'].items():
        if Path(name).suffix!='.qsf':
            require(fit.digest(directory/name)==expected,'fitted source/constraint differs: '+name)
    results={}
    with tempfile.TemporaryDirectory(prefix='dorado-async256-audit-',dir='/tmp') as temp:
        reference=Path(temp)/'inputs';fit.prepare(reference)
        for variant in ('baseline','snapshot'):
            case=directory/variant
            require(normalized_qsf((case/'snapshot_fit.qsf').read_text())==normalized_qsf((reference/variant/'snapshot_fit.qsf').read_text()),'QSF source/constraints/macros differ')
            summary=(case/'snapshot_fit.fit.summary').read_text()
            require('Fitter Status : Successful'in summary,'unsuccessful fit')
            row={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                              ('block_ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                m=re.search(label+r' : ([\d,]+)',summary);require(m is not None,'missing resource '+key)
                row[key]=int(m[1].replace(',',''))
            require(row['block_ram_bits']==row['ram_blocks']==0,'async RAMs unexpectedly use M10K')
            mapping=(case/'snapshot_fit.map.rpt').read_text(errors='replace')
            fitted=(case/'snapshot_fit.fit.rpt').read_text(errors='replace')
            row['memory_tables']=memory_tables(mapping,fitted,variant)
            row['warning_counts']={}
            for phase in ('map','fit'):
                text=(case/(phase+'.log')).read_text(errors='replace')
                require(not re.search(r'^\s*Critical Warning',text,re.M),'critical '+phase+' warning')
                counts=Counter(re.findall(r'^\s*Warning \((\d+)\):',text,re.M))
                if phase=='map':
                    require(set(counts)<={'10036','13024','13410'},'unexpected synthesis warning: '+str(counts))
                row['warning_counts'][phase]=dict(counts)
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
            results[variant]=row
    before=Counter(results['baseline']['warning_counts']['fit'])
    after=Counter(results['snapshot']['warning_counts']['fit'])
    require(not(after-before),'new fitter warning: '+str(after-before))
    return dict(status='PASS',scope=provenance['scope'],results=results,
        delta={k:results['snapshot'][k]-results['baseline'][k] for k in ('alms','registers','block_ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for checks in results['snapshot']['timing'].values() for v in checks.values()),
        limits='Isolated pair with matched registered shell and active access. No complete-board/core resource estimate, physical snapshot, global barrier or original60ns qualification.')


def controls(directory):
    result=[]
    for name,path,before,after in [
        ('source','snapshot/cell_MB7071H_snapshot.sv','mem[state_address] <= state_write_data;',"mem[state_address] <= '0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('qsf','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('mlab','snapshot/snapshot_fit.map.rpt','; MLAB ;','; M10K ;'),
        ('fitted_mlab','snapshot/snapshot_fit.fit.rpt','; MLAB ;','; M10K ;'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).  Worst case slack is','(1 violated).  Worst case slack is')]:
        with tempfile.TemporaryDirectory(prefix='dorado-async256-control-',dir='/tmp') as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out)
            target=out/path;text=target.read_text(errors='replace');require(before in text,'missing control anchor '+name)
            target.write_text(text.replace(before,after))
            try:
                audit(out)
            except ValueError as error:
                result.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:
                raise ValueError('damaged fit evidence accepted: '+name)
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();result=audit(args.directory.resolve())
    if args.negative_controls:
        result['failure_controls']=controls(args.directory.resolve())
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
