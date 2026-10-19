#!/usr/bin/env python3
"""Audit full ContA isolated source closure, routing and state-access cost."""
import argparse
import json
from pathlib import Path
import re
import shutil
import tempfile

import fit_conta_full as fit
import generate_conta_full as gen
from audit_f10145 import normalized_qsf,require
from audit_f10415_enable import CORNERS


def audit(directory):
    provenance=json.loads((directory/'provenance.json').read_text())
    for name,sha in provenance['source_hashes'].items():
        require(fit.digest(gen.ROOT/name)==sha,'source provenance differs: '+name)
    for name,sha in provenance['compiled_files'].items():
        if not name.endswith('.qsf'):require(fit.digest(directory/name)==sha,'fitted input differs: '+name)
    source=directory/'baseline/ContA_original.v'
    board,manifest,clones=gen.generate(source)
    require(provenance['manifest']==manifest,'access manifest differs')
    require(fit.digest(source)==provenance['source_board_sha256'],'original board differs')
    results={}
    with tempfile.TemporaryDirectory(prefix='dorado-conta-inputs-',dir='/tmp')as temp:
        reference=Path(temp)/'inputs';fit.prepare(source,reference)
        for variant in ('baseline','snapshot'):
            case=directory/variant
            require(normalized_qsf((case/'snapshot_fit.qsf').read_text())==normalized_qsf((reference/variant/'snapshot_fit.qsf').read_text()),'QSF source/macro/constraint differs')
            require((case/'ContA_snapshot.sv').read_text()==board,'fitted adapter differs')
            for name,text in clones.items():require((case/name).read_text()==text,'fitted cloned cell differs: '+name)
            summary=(case/'snapshot_fit.fit.summary').read_text()
            require('Fitter Status : Successful'in summary,'fit not successful')
            row={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                              ('block_ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                match=re.search(label+r' : ([\d,]+)',summary);require(match is not None,'missing resource: '+key)
                row[key]=int(match[1].replace(',',''))
            require(row['block_ram_bits']==row['ram_blocks']==0,'logic register files unexpectedly use block RAM')
            require({p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==CORNERS,'missing timing condition')
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
                    require((case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async timing path')
                ucp=(case/f'bank-ucp-{corner}.rpt').read_text()
                for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    require(re.search('; '+label+r'\s*; 0\s*; 0\s*;',ucp)is not None,'unconstrained path: '+label)
                row['timing'][corner]=checks
            for label in ('map','fit'):
                require(not re.search(r'^Critical Warning',(case/(label+'.log')).read_text(),re.M),'critical '+label+' warning')
            results[variant]=row
    return dict(status='PASS',scope=provenance['scope'],results=results,
        delta={key:results['snapshot'][key]-results['baseline'][key]for key in ('alms','registers','block_ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(value for checks in results['snapshot']['timing'].values()for value in checks.values()),
        state_count_scope='1353 non-diagnostic modeled bits, not a count of fitted baseline registers; access can retain otherwise-pruned model history. Resource totals include shell/admin pruning.',
        limits='Isolated board; no full-core clock distribution, global barriers, physical snapshot or original60ns qualification')


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/ContA_snapshot.sv','wire state_hold=state_freeze && state_guest_held;',"wire state_hold=1'b0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('qsf','snapshot/snapshot_fit.qsf','DORADO_F10145_LOGIC=1','DORADO_F10145_LOGIC=0'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).  Worst case slack is','(1 violated).  Worst case slack is'),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-conta-audit-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out)
            target=out/path;text=target.read_text();require(before in text,'missing control anchor')
            target.write_text(text.replace(before,after))
            try:audit(out)
            except ValueError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise ValueError('damaged fit evidence accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory',type=Path);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();result=audit(args.directory.resolve())
    if args.negative_controls:result['failure_controls']=controls(args.directory.resolve())
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
