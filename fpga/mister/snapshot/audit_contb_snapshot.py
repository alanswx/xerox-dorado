#!/usr/bin/env python3
"""Audit the isolated actual-ContB snapshot fit and damaged evidence controls."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import tempfile

import fit_contb_snapshot as fit
import generate_contb as gen
from audit_f10415 import require
from audit_f10415_enable import CORNERS
from test_contb_snapshot import fixture

ROOT = Path(__file__).resolve().parents[3]


def project_lines(text):
    # Quartus appends its own version marker when closing a project.
    marker='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    return [line.strip() for line in text.splitlines() if line.strip() and line.strip()!=marker]


def audit(directory):
    provenance = json.loads((directory/'provenance.json').read_text())
    for name, digest in provenance['source_hashes'].items():
        require(hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest,
                'build provenance differs: '+name)
    board, manifest = gen.generate()
    require(manifest==provenance['manifest'], 'board manifest differs')
    _, _, ports = fixture(manifest)
    # Compare the entire project assignment set, including all source/SDC
    # inclusions. Matching one SDC cannot establish coverage if another file
    # was added to waive paths or if the fitted board was omitted.
    with tempfile.TemporaryDirectory(prefix='dorado-contb-project-',dir='/tmp') as tmp:
        expected=Path(tmp)/'expected'
        fit.prepare(expected)
        projects={variant:(expected/variant/'snapshot_fit.qsf').read_text()
                  for variant in ('baseline','snapshot')}
    results = {}
    for variant in ('baseline', 'snapshot'):
        case = directory/variant
        for name, digest in provenance['source_hashes'].items():
            if name.endswith(('.v', '.sv')):
                require(hashlib.sha256((case/Path(name).name).read_bytes()).hexdigest()==digest,
                        'fitted source differs: '+name)
        require((case/'ContB_snapshot.sv').read_text()==board, 'generated board differs')
        require((case/'top.sv').read_text()==fit.top(ports), 'clock/fixture differs')
        require((case/'bank.sdc').read_text()==fit.SDC, 'constraints differ')
        require((case/'audit.tcl').read_text()==fit.AUDIT, 'timing script differs')
        qsf=(case/'snapshot_fit.qsf').read_text()
        require(project_lines(qsf)==project_lines(projects[variant]), 'project assignments differ')
        require('5CSEBA6U23I7' in qsf and 'OPTIMIZE_HOLD_TIMING ALL_PATHS' in qsf,
                'device/hold optimization differs')
        macro='VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"'
        require((macro in qsf)==(variant=='snapshot'), 'variant macro differs')
        text=(case/'snapshot_fit.fit.summary').read_text()
        require('Fitter Status : Successful' in text, 'fit incomplete')
        row={}
        for key,label in (('alms',r'Logic utilization \(in ALMs\)'),
                          ('registers','Total registers'),
                          ('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')):
            match=re.search(label+r' : ([\d,]+)',text)
            require(match is not None,'missing resource: '+key)
            row[key]=int(match[1].replace(',',''))
        require(row['ram_bits']==147456 and row['ram_blocks']==16,'IM geometry/packing differs')
        corners={p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}
        require(corners==CORNERS,'missing/unexpected timing corner')
        row['timing']={}
        for corner in sorted(CORNERS):
            checks={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text()
                match=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                require(match is not None,'missing timing: '+mode+'/'+corner)
                require(int(match[1])==0 and float(match[2])>=0,'timing violation: '+mode+'/'+corner)
                checks[mode]=float(match[2])
            for mode in ('recovery','removal'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text()
                require(text.strip().endswith('Nothing to report.'),'unexpected async path: '+mode)
                checks[mode]='N/A: no asynchronous paths'
            text=(case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports',
                          'Unconstrained Input Port Paths','Unconstrained Output Ports',
                          'Unconstrained Output Port Paths'):
                require(re.search(r'; '+label+r'\s*; 0\s*; 0\s*;',text) is not None,
                        'missing timing coverage: '+label+'/'+corner)
            clocks=(case/f'bank-clocks-{corner}.rpt').read_text()
            require(re.search(r'; host_clock\s*; Base\s*; 20\.000\s*;',clocks) is not None,
                    'host clock differs')
            row['timing'][corner]=checks
        if variant=='snapshot':
            text=(case/'snapshot_fit.map.rpt').read_text()
            for parameter,value in (('OPERATION_MODE','DUAL_PORT'),('WIDTH_A','18'),('WIDTH_B','18'),
                                    ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),
                                    ('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')):
                require(re.findall(r'; '+parameter+r'\s*; (\w+)\s*;',text)==[value]*8,
                        'RAM mode differs: '+parameter)
        results[variant]=row
    delta={k:results['snapshot'][k]-results['baseline'][k]
           for k in ('alms','registers','ram_bits','ram_blocks')}
    return dict(status='PASS',scope=provenance['scope'],results=results,delta=delta,
                limits='IM access on isolated actual ContB only; shell overhead included. Other board state/global barrier/whole-core timing/physical snapshots not qualified.')


def controls(directory):
    rows=[]
    cases=(
        ('project','snapshot/snapshot_fit.qsf','SDC_FILE bank.sdc','SDC_FILE waived.sdc','project assignments differ'),
        ('clock','snapshot/top.sv','.ena(run_r)','.ena(run)','clock/fixture differs'),
        ('board','snapshot/ContB_snapshot.sv','state_bank_q_0[0]','state_bank_q_0[1]','generated board differs'),
        ('constraints','snapshot/bank.sdc','20.000','19.000','constraints differ'),
        ('packing','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 16','Total RAM Blocks : 17','IM geometry/packing differs'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','DONT_CARE','RAM mode differs'),
        ('hold','snapshot/bank-hold-MIN_fast_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('coverage','snapshot/bank-ucp-7_slow_1100mv_100c.rpt','; Unconstrained Clocks','; Missing Clocks','missing timing coverage'),
    )
    for name,relative,before,after,expected in cases:
        with tempfile.TemporaryDirectory(prefix='dorado-contb-fit-audit-',dir='/tmp') as tmp:
            out=Path(tmp)/'evidence'
            shutil.copytree(directory,out)
            path=out/relative
            text=path.read_text()
            require(before in text,'negative fixture differs: '+name)
            path.write_text(text.replace(before,after,1))
            try:
                audit(out)
            except ValueError as e:
                require(expected in str(e),'wrong failure: '+name+' '+str(e))
                rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:
                raise ValueError('damaged evidence accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory',type=Path)
    parser.add_argument('--negative-controls',action='store_true')
    parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    result=audit(args.directory)
    if args.negative_controls:
        result['failure_controls']=controls(args.directory)
    text=json.dumps(result,indent=2)+'\n'
    if args.output:
        args.output.write_text(text)
    print(text)
