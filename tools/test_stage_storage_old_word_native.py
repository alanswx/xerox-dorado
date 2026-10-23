#!/usr/bin/env python3
"""Corrupt frozen old-word/early-read evidence and reject before native output."""
import argparse
import gzip
import json
from pathlib import Path
import shutil

from stage_storage_old_word_native import stage, RECEIPTS, ROOT, sha


def run(config,out):
    assert not out.exists()
    args={n:Path(v)for n,v in json.loads(config.read_text()).items()if n!='output'}
    out.mkdir(parents=True);original={n:args[n]for n in RECEIPTS};frozen={}
    for n,(relative,h) in RECEIPTS.items():
        manifest=json.loads(gzip.decompress((ROOT/'fpga/mister/reports'/relative/'manifest.json.gz').read_bytes()))
        for m in manifest['members']:
            name=m['name']
            if name.startswith(('tools/','freeze')):continue
            source=original[n]/name;frozen[source]=sha(source.read_bytes())
            target=out/n/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target)
        args[n]=out/n
    r=stage(**args,output=out/'positive')
    assert len(r['source_hashes'])==503 and len(r['modified_RTL_sources'])==6
    assert r['old_word_falling']and r['early_read_response']and not r['assembly_permitted']

    def update(path,fun):
        value=json.loads(path.read_text());fun(value);path.write_text(json.dumps(value,indent=2)+'\n')

    unit=out/'old_word/unit-v1'
    def mutate(name):
        if name=='failed-unit':update(unit/'run-state.json',lambda j:j.update(status='TERMINAL_FAILED'))
        elif name=='missing-fast-result':update(unit/'run-state.json',lambda j:j['results'].pop('icarus-w16-fast1'))
        elif name=='rebound-wrong-edge':
            f=unit/'inputs/dorado_storage_group.sv';s=f.read_text();old='always @(negedge host_clk)\n        if(preserve_old_word)';assert s.count(old)==1
            f.write_text(s.replace(old,'always @(posedge host_clk)\n        if(preserve_old_word)'))
            update(unit/'provenance.json',lambda j:j['inputs'].update({'dorado_storage_group.sv':sha(f.read_bytes())}))
        elif name=='rebound-wrong-reference':
            f=unit/'inputs/reference.sv';f.write_bytes(f.read_bytes()+b'\n// changed reference\n')
            update(unit/'provenance.json',lambda j:j['inputs'].update({'reference.sv':sha(f.read_bytes())}))
        elif name=='removed-consumer-check':
            f=unit/'inputs/tb.sv';s=f.read_text();assert 'OLD_WORD_FALLING consumed old word differs'in s
            f.write_text(s.replace('$fatal(1,"OLD_WORD_FALLING consumed old word differs")','$display("removed check")'))
            update(unit/'provenance.json',lambda j:j['inputs'].update({'tb.sv':sha(f.read_bytes())}))
        elif name=='changed-MSA-host-trace':
            f=out/'old_word/board-early-v1/falling-old-word-raw.csv';f.write_bytes(f.read_bytes()+b'\n')
        elif name=='failed-early-negative':
            update(out/'early/miter-v1/run-state.json',lambda j:j['results']['reply-rearmed-at-tail-verilator'].update(rejected=False))
        elif name=='premature-native-claim':update(unit/'provenance.json',lambda j:j.update(native_qualified=True))
        else:raise ValueError(name)

    rows=[]
    for name in ['failed-unit','missing-fast-result','rebound-wrong-edge','rebound-wrong-reference',
                 'removed-consumer-check','changed-MSA-host-trace','failed-early-negative','premature-native-claim']:
        before={f:f.read_bytes()for n in RECEIPTS for f in(out/n).rglob('*')if f.is_file()}
        mutate(name);rejected=False
        try:stage(**args,output=out/name)
        except (AssertionError,KeyError):rejected=True
        assert rejected and not(out/name).exists(),name
        rows.append(dict(name=name,rejected=True))
        for f,b in before.items():
            if f.read_bytes()!=b:f.write_bytes(b)
        print(name,'rejected',flush=True)
    assert all(sha(f.read_bytes())==h for f,h in frozen.items())
    result=dict(status='PASS_OLD_WORD_EARLY_STAGE_EIGHT_REJECTION_CONTROLS',cases=rows,
                source_count=503,RTL_changes=6,original_inputs_unchanged=True)
    (out/'controls.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['config','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();print(run(a.config.resolve(),a.output.resolve())['status'])
