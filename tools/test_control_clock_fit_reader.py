#!/usr/bin/env python3
"""Reject damaged isolated clock-fit evidence, including rehashed input changes."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
from audit_control_clock_fit import audit


def run(source, qualified, output):
    assert audit(source,qualified,False)['status']=='PASS_COMPONENT_TIMING'
    output.mkdir(parents=True,exist_ok=False)
    cases=[]
    def changed_input(directory,name,edit):
        p=directory/name;p.write_text(edit(p.read_text()))
        digest=hashlib.sha256(p.read_bytes()).hexdigest()
        for file,key in [('provenance.json','files'),('collected-input-check.json','actual')]:
            p=directory/file;q=json.loads(p.read_text());q[key][name]=digest
            p.write_text(json.dumps(q,indent=2)+'\n')
    for name in ['wrong_dimensions','relaxed_period_rehashed','changed_evaluator_rehashed',
                 'changed_dependency_rehashed','missing_corner','missing_evaluator','negative_setup','negative_evaluator']:
        d=output/name;shutil.copytree(source,d)
        if name=='wrong_dimensions':
            p=d/'provenance.json';q=json.loads(p.read_text());q['launch_bits']-=1;p.write_text(json.dumps(q))
        elif name=='relaxed_period_rehashed':
            changed_input(d,'clock.sdc',lambda s:s.replace('15.000','20.000'))
        elif name.startswith('changed_'):
            evaluator=json.loads((d/'provenance.json').read_text())['component']
            target='dorado_'+evaluator+'_step.sv' if name.startswith('changed_evaluator') else 'dorado_task_capture_step.sv'
            changed_input(d,target,lambda s:s+'\n// changed qualified source\n')
        elif name=='missing_corner':
            (d/'clock-hold-MIN_fast_1100mv_-40c.rpt').unlink()
        elif name=='missing_evaluator':
            (d/'clock-evaluator-7_slow_1100mv_100c.rpt').unlink()
        else:
            mode='evaluator' if name=='negative_evaluator' else 'setup'
            p=d/('clock-'+mode+'-7_slow_1100mv_100c.rpt');s=p.read_text()
            s,n=re.subn(r'(Worst case slack is )-?\d+\.\d+',r'\g<1>-0.001',s,flags=re.I);assert n==1
            s,n=re.subn(r'(;\s*Slack\s*;\s*)-?\d+\.\d+',r'\g<1>-0.001',s);assert n>=1
            p.write_text(s)
        try:
            result=audit(d,qualified,False)
        except (AssertionError,FileNotFoundError) as error:
            cases.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:
            assert result['status']=='REJECTED_COMPONENT_TIMING',(name,'damaged evidence accepted')
            cases.append(dict(case=name,rejected=True,diagnostic=result['status']))
    (output/'results.json').write_text(json.dumps(dict(status='PASS',cases=cases),indent=2)+'\n')
    print('FIT_READER_CONTROLS PASS',len(cases),'damaged/rehashed cases rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--qualified',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.directory.resolve(),a.qualified.resolve(),a.output.resolve())
