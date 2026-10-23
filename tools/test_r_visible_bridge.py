#!/usr/bin/env python3
"""Reject incomplete or rehashed visible-R equivalence receipts."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import shutil
from audit_control_clock_fit import visible_bridge


def run(qualified,output):
    original=json.loads((qualified/'results.json').read_text())
    visible_bridge(qualified,original)
    output.mkdir(parents=True,exist_ok=False);cases=[]
    for name in ['overlap_counterexample','missing_original_board','extra_state',
                 'missing_inherited_profile','changed_unrelated_dependency','changed_candidate']:
        d=output/name;d.mkdir();record=copy.deepcopy(original)
        for n in ['original-operand.sv.txt','inherited-mask-gate.json','dorado_r_front_cone.sv',
                  'executed-inputs/verilog/rtl/dorado_local_r_operand_step.sv']:
            p=d/n;p.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(qualified/n,p)
        shutil.copytree(qualified/'visible-proof',d/'visible-proof')
        proof=json.loads((d/'visible-proof/results.json').read_text())
        if name=='overlap_counterexample':proof['cases'][0]['result']='sat'
        elif name=='missing_original_board':proof['original_clock_wiring'].pop()
        elif name=='extra_state':proof['transform']['extra_architectural_state']=True
        elif name=='missing_inherited_profile':record['inherited_arrivals'].pop()
        elif name=='changed_unrelated_dependency':record['inputs']['verilog/rtl/dorado_task_ready_step.sv']='0'*64
        elif name=='changed_candidate':
            with (d/'visible-proof/candidate.sv').open('a')as f:f.write('\n// changed candidate\n')
        p=d/'visible-proof/results.json';p.write_text(json.dumps(proof,indent=2)+'\n')
        record['visible_proof_sha256']=hashlib.sha256(p.read_bytes()).hexdigest()
        (d/'results.json').write_text(json.dumps(record,indent=2)+'\n')
        try:visible_bridge(d,record)
        except AssertionError as error:cases.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError(('Damaged proof accepted',name))
    result=dict(status='PASS_VISIBLE_R_BRIDGE_CONTROLS',cases=cases)
    (output/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(cases))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('qualified',type=Path)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.qualified.resolve(),a.output.resolve())
