#!/usr/bin/env python3
"""Reject actual corrupted mux proof before creating any native stage."""
import argparse
import json
from pathlib import Path
import shutil

from stage_decoded_mux_native import stage, sha


def run(config,out):
    assert not out.exists()
    args={n:Path(v) for n,v in json.loads(config.read_text()).items() if n!='output'}
    original=args['mux']
    selected=[p for p in original.rglob('*') if p.is_file()
              and not any(n.startswith('obj-') for n in p.relative_to(original).parts)
              and (p.relative_to(original).parts[0]=='inputs' or p.suffix in ['.json','.log','.v'])]
    frozen={p:sha(p.read_bytes()) for p in selected}
    out.mkdir(parents=True);fixture=out/'mux'
    for p in selected:
        target=fixture/p.relative_to(original);target.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(p,target)
    args['mux']=fixture
    record=stage(**args,output=out/'positive')
    assert len(record['source_hashes'])==503 and len(record['modified_RTL_sources'])==5
    assert record['decoded_mux'] and not record['assembly_permitted']

    def update(name,f):
        p=fixture/name;j=json.loads(p.read_text());f(j);p.write_text(json.dumps(j,indent=2)+'\n')

    def mutate(label):
        if label=='failed-result':
            update('run-state.json',lambda j:j.update(status='TERMINAL_FAILED'))
        elif label=='incomplete-four-state':
            update('provenance.json',lambda j:j.update(four_state_cases=65536))
        elif label=='rebound-wrong-alias':
            p=fixture/'inputs/cell_MU10164.v';b=p.read_bytes();assert b.count(b'(sel==line)')==1
            p.write_bytes(b.replace(b'(sel==line)',b'(sel==(7-line))'))
            update('provenance.json',lambda j:j['inputs'].update({'cell_MU10164.v':sha(p.read_bytes())}))
        elif label=='rebound-wrong-reference':
            p=fixture/'inputs/reference_MC10164.v';p.write_bytes(p.read_bytes()+b'\n// changed reference\n')
            update('provenance.json',lambda j:j['inputs'].update({'reference_MC10164.v':sha(p.read_bytes())}))
        elif label=='changed-stimulus':
            p=fixture/'inputs/tb.sv';p.write_text(p.read_text().replace('16777216','65536'))
            update('provenance.json',lambda j:j['inputs'].update({'tb.sv':sha(p.read_bytes())}))
        elif label=='missing-four-state-pass':
            p=fixture/'icarus-positive-test.log';p.write_text(p.read_text().replace('PASS four_state=16777216','partial'))
        elif label=='nonrejecting-unknown-index':
            update('run-state.json',lambda j:j['results']['icarus-unknown-index'].update(rejected=False))
        elif label=='missing-fatal-loop-check':
            update('run-state.json',lambda j:[a['command'].remove('-Werror-UNOPTFLAT') for a in j['actions'] if a['label']=='verilator-positive-compile'])
        elif label=='actual-circular-warning':
            p=fixture/'verilator-positive-compile.log';p.write_text(p.read_text()+'\n%Warning-UNOPTFLAT: actual circular logic\n')
        else:raise ValueError(label)

    results=[]
    for label in ['failed-result','incomplete-four-state','rebound-wrong-alias',
                  'rebound-wrong-reference','changed-stimulus','missing-four-state-pass',
                  'nonrejecting-unknown-index','missing-fatal-loop-check','actual-circular-warning']:
        before={p:p.read_bytes() for p in fixture.rglob('*') if p.is_file()}
        mutate(label);rejected=False
        try:stage(**args,output=out/label)
        except (AssertionError,KeyError):rejected=True
        assert rejected and not (out/label).exists(),label
        results.append(dict(name=label,rejected=True))
        for p,b in before.items():
            if p.read_bytes()!=b:p.write_bytes(b)
        print(label,'rejected',flush=True)
    assert all(sha(p.read_bytes())==h for p,h in frozen.items())
    result=dict(status='PASS_DECODED_MUX_STAGE_NINE_REJECTION_CONTROLS',cases=results,
                original_inputs_unchanged=True,source_count=503,RTL_changes=5)
    (out/'controls.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--config',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();print(run(a.config.resolve(),a.output.resolve())['status'])
