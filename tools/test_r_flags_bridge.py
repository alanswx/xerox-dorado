#!/usr/bin/env python3
"""Reject incomplete or rehashed flag-factorization proof evidence."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import shutil
from audit_control_clock_fit import flags_bridge


def run(qualified,output):
    original=json.loads((qualified/'results.json').read_text());flags_bridge(qualified,original)
    output.mkdir(parents=True,exist_ok=False);cases=[]
    for name in ['actual_counterexample','bad_stale_ram_control','changed_candidate',
                 'changed_latch_updates','missing_parent_arrival','changed_smt_source']:
        d=output/name;d.mkdir();record=copy.deepcopy(original)
        for n in ['original-operand.sv.txt','original-flags.sv.txt','inherited-mask-gate.json',
                  'inherited-visible-gate.json','dorado_r_front_cone.sv',
                  'executed-inputs/verilog/rtl/dorado_local_r_operand_step.sv',
                  'executed-inputs/verilog/rtl/dorado_local_r_flags_step.sv']:
            p=d/n;p.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(qualified/n,p)
        for n in ['visible-proof','flags-proof']:shutil.copytree(qualified/n,d/n)
        proof=json.loads((d/'flags-proof/results.json').read_text())
        if name=='actual_counterexample':proof['cases'][0]['result']='sat'
        elif name=='bad_stale_ram_control':proof['cases'][1]['result']='unsat'
        elif name=='changed_candidate':
            with (d/'flags-proof/candidate.sv').open('a')as f:f.write('\n// changed candidate\n')
        elif name=='changed_latch_updates':proof['transform']['physical_latch_updates_unchanged']=False
        elif name=='missing_parent_arrival':record['previous_visible_arrivals'].pop()
        elif name=='changed_smt_source':
            with (d/'flags-proof/original.smt2').open('a')as f:f.write('\n; changed theorem\n')
        p=d/'flags-proof/results.json';p.write_text(json.dumps(proof,indent=2)+'\n')
        record['flags_proof_sha256']=hashlib.sha256(p.read_bytes()).hexdigest()
        (d/'results.json').write_text(json.dumps(record,indent=2)+'\n')
        try:flags_bridge(d,record)
        except AssertionError as error:cases.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError(('Damaged proof accepted',name))
    result=dict(status='PASS_R_FLAGS_BRIDGE_CONTROLS',cases=cases)
    (output/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(cases))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('qualified',type=Path)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.qualified.resolve(),a.output.resolve())
