#!/usr/bin/env python3
"""Check frozen sources, routed timing and resource scope for the file adapter."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import tempfile

import fit_f10145 as fit
import generate_f10145 as gen
from audit_f10415_enable import CORNERS


def require(ok,why):
    if not ok:raise ValueError(why)


def normalized_qsf(text):
    return '\n'.join(line for line in text.splitlines()
                     if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def audit(directory):
    provenance=json.loads((directory/'provenance.json').read_text())
    generated,manifest=gen.generate()
    require(provenance['manifest']==manifest,'generated manifest differs')
    for name,sha in provenance['source_hashes'].items():
        require(fit.digest(gen.ROOT/name)==sha,'source provenance differs: '+name)
    for name,sha in provenance['compiled_files'].items():
        path=directory/name
        if path.suffix=='.qsf':continue # Exact known version marker/blank lines checked below.
        require(fit.digest(path)==sha,'fitted input differs: '+name)
    results={}
    for variant in ('baseline','snapshot'):
        case=directory/variant
        require((case/'cell_F10145A_snapshot.sv').read_text()==generated,'fitted generated file differs')
        require((case/'top.sv').read_text()==fit.TOP,'top fixture differs')
        require((case/'bank.sdc').read_text()==fit.SDC,'timing constraints differ')
        require((case/'audit.tcl').read_text()==fit.AUDIT,'timing audit differs')
        # Rebuild a fresh preparation to compare every QSF setting, allowing
        # only the exact documented version assignment and blank lines.
        with tempfile.TemporaryDirectory(prefix='dorado-f10145-qsf-',dir='/tmp')as temp:
            reference=Path(temp)/'inputs';fit.prepare(reference)
            require(normalized_qsf((case/'snapshot_fit.qsf').read_text())==normalized_qsf((reference/variant/'snapshot_fit.qsf').read_text()),'QSF constraints/macros/sources differ')
        summary=(case/'snapshot_fit.fit.summary').read_text()
        require('Fitter Status : Successful'in summary,'fit not successful')
        row={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                          ('block_ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            match=re.search(label+r' : ([\d,]+)',summary)
            require(match is not None,'resource missing: '+key)
            row[key]=int(match[1].replace(',',''))
        require(row['block_ram_bits']==row['ram_blocks']==0,'small file unexpectedly uses block RAM')
        require({p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==CORNERS,'missing timing corner')
        row['timing']={}
        for corner in sorted(CORNERS):
            checks={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text()
                match=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                require(match is not None,'missing timed paths: '+mode+'/'+corner)
                require(int(match[1])==0 and float(match[2])>=0,'timing violation: '+mode+'/'+corner)
                checks[mode]=float(match[2])
            for mode in ('recovery','removal'):
                require((case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected asynchronous path')
            text=(case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports',
                          'Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                require(re.search('; '+label+r'\s*; 0\s*; 0\s*;',text)is not None,'unconstrained paths: '+label)
            row['timing'][corner]=checks
        require(not re.search(r'^Critical Warning', (case/'fit.log').read_text(),re.M),'critical fitter warning')
        results[variant]=row
    return dict(status='PASS',scope=provenance['scope'],results=results,
        delta={key:results['snapshot'][key]-results['baseline'][key]for key in ('alms','registers','block_ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for check in results['snapshot']['timing'].values()for v in check.values()),
        limits='One isolated cell with matched registered shell; not complete-board/core area, physical snapshots or original60ns timing qualification')


def controls(directory):
    result=[]
    for name,path,before,after in [
        ('source','snapshot/cell_F10145A_snapshot.sv','mem[a_lat] <= d_lat;',"mem[a_lat] <= 4'b0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('qsf','snapshot/snapshot_fit.qsf','DORADO_F10145_LOGIC=1','DORADO_F10145_LOGIC=0'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).  Worst case slack is','(1 violated).  Worst case slack is')]:
        with tempfile.TemporaryDirectory(prefix='dorado-f10145-audit-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out)
            target=out/path;text=target.read_text();require(before in text,'control anchor missing')
            target.write_text(text.replace(before,after))
            try:audit(out)
            except ValueError as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise ValueError('damaged fit evidence accepted: '+name)
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory',type=Path);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();result=audit(args.directory.resolve())
    if args.negative_controls:result['failure_controls']=controls(args.directory.resolve())
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
