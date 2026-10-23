#!/usr/bin/env python3
"""Reject incomplete/rehashed arithmetic-equivalence evidence before timing review."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import shutil
from audit_control_clock_fit import stack_adder_bridge


def run(qualified, output):
    original=json.loads((qualified/'results.json').read_text())
    stack_adder_bridge(qualified,original)
    output.mkdir(parents=True,exist_ok=False)
    cases=[]
    for name in ['missing_wiring_coverage','failed_original_compile','missing_engine',
                 'missing_inherited_profile','changed_reference_cell','extra_logic_change']:
        d=output/name;d.mkdir()
        for rel in ['inherited-mask-gate.json','adder-transform.json','original-cone.sv.txt',
                    'dorado_r_front_cone.sv']:
            shutil.copyfile(qualified/rel,d/rel)
        shutil.copytree(qualified/'adder-equivalence',d/'adder-equivalence')
        record=copy.deepcopy(original)
        def edit(rel,change,key=None):
            p=d/rel;q=json.loads(p.read_text());change(q)
            p.write_text(json.dumps(q,indent=2)+'\n')
            if key:record[key]=hashlib.sha256(p.read_bytes()).hexdigest()
        if name=='missing_wiring_coverage':
            edit('adder-equivalence/results.json',lambda q:q['cases'][0].update(physical_wiring_checks=0),'adder_equivalence_sha256')
        elif name=='failed_original_compile':
            edit('adder-equivalence/results.json',lambda q:q['actions'][0].update(returncode=1),'adder_equivalence_sha256')
        elif name=='missing_engine':
            edit('adder-equivalence/results.json',lambda q:q['cases'][3].update(engine='icarus'),'adder_equivalence_sha256')
        elif name=='missing_inherited_profile':
            edit('inherited-mask-gate.json',lambda q:q['arrivals'].pop(),'inherited_mask_gate_sha256')
            record['inherited_arrivals'].pop()
        elif name=='changed_reference_cell':
            with (d/'adder-equivalence/inputs/cell_MC10182.v').open('a') as f:f.write('\n// changed reference\n')
        elif name=='extra_logic_change':
            edit('adder-transform.json',lambda q:q.update(inserted=q['inserted']+' // extra change\n'),'adder_transform_sha256')
            shutil.copyfile(d/'adder-transform.json',d/'adder-equivalence/transform.json')
        (d/'results.json').write_text(json.dumps(record,indent=2)+'\n')
        try:stack_adder_bridge(d,record)
        except AssertionError as error:cases.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError(('Damaged proof accepted',name))
    result=dict(status='PASS_STACK_ADDER_BRIDGE_CONTROLS',cases=cases)
    (output/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(cases))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('qualified',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.qualified.resolve(),a.output.resolve())
