#!/usr/bin/env python3
"""Stage tested diagnostic mux decoding on the exact qualified native parent.

Combine existing carry/read-word proofs with the two decoded mux cells.
No clock, capture, register, board connection, constraint or ARM changes.
This stage permits one fit and report collection, never assembly or loading.
"""
import argparse
import hashlib
import json
from pathlib import Path

from stage_direct_carry_capture_native import stage as carry_stage
from test_mc10164_decoded import decoded, bench


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def check_mux(parent, mux):
    provenance = json.loads((mux/'provenance.json').read_text())
    state = json.loads((mux/'run-state.json').read_text())
    assert state['status']=='PASS_MC10164_DECODED_BINARY_AND_FOUR_STATE'
    assert state['inputs_unchanged'] and not provenance['registers_changed']
    assert not provenance['native_qualified']
    assert provenance['binary_cases']==4096 and provenance['four_state_cases']==16777216
    assert provenance['aliases']==2
    assert provenance['parent_manifest_sha256']==sha((parent/'timing-paths-native.json').read_bytes())
    for n,h in provenance['inputs'].items():
        assert sha((mux/'inputs'/n).read_bytes())==h,n
    assert (mux/'inputs/tb.sv').read_text()==bench()
    result={}
    for cell in ['MC10164','MU10164']:
        n='src/verilog/cells/cell_'+cell+'.v'
        raw=(parent/n).read_bytes()
        assert sha(raw)==provenance['original_hashes'][cell]
        assert (mux/('inputs/reference_'+cell+'.v')).read_bytes()==raw.replace(
            ('module cell_'+cell+' (').encode(),('module reference_'+cell+' (').encode())
        assert (mux/('inputs/cell_'+cell+'.v')).read_bytes()==decoded(raw)
        result[n]=decoded(raw)
    for engine in ['icarus','verilator']:
        label=engine+'-positive'
        assert state['results'][label]['passed']
        log=(mux/(label+'-test.log')).read_text()
        assert 'MC10164_DECODED PASS binary=4096 aliases=2' in log
        if engine=='icarus':
            assert 'MC10164_DECODED PASS four_state=16777216 aliases=2' in log
        for negative in ['wrong-select','lost-enable']+(['unknown-index'] if engine=='icarus' else []):
            assert state['results'][engine+'-'+negative]['rejected']
    assert len(state['results'])==7 and all(a['returncode']==0 for a in state['actions'] if a['label'].endswith('-compile'))
    assert all('returncode' in a for a in state['actions'])
    action=[a for a in state['actions'] if a['label']=='verilator-positive-compile']
    assert len(action)==1 and '-Werror-UNOPTFLAT' in action[0]['command']
    for f in mux.glob('*compile.log'):
        assert 'UNOPTFLAT' not in f.read_text(),f
    return result,provenance,state


def stage(parent, qualification, generated, miter, pins, board, direct_alu, mux, output, project):
    assert not output.exists()
    changes,provenance,state=check_mux(parent,mux)
    record=carry_stage(parent,qualification,generated,miter,pins,board,direct_alu,output,project)
    assert len(record['modified_RTL_sources'])==3
    for n,b in changes.items():(output/n).write_bytes(b)
    record.update(status='STAGED_DECODED_MUX_NATIVE_FIT_REVIEW_REQUIRED',scope=__doc__,
        modified_RTL_sources=sorted(record['modified_RTL_sources']+list(changes)),
        source_hashes={n:sha((output/n).read_bytes()) for n in record['source_hashes']},
        decoded_mux=True, mux_four_state_cases=16777216,
        decoded_mux_provenance_sha256=sha((mux/'provenance.json').read_bytes()),
        decoded_mux_state_sha256=sha((mux/'run-state.json').read_bytes()))
    for f in mux.iterdir():
        if f.is_file() and f.suffix in ['.json','.log','.v']:
            target=output/'evidence/decoded-mux'/f.name
            target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(f.read_bytes())
            record['evidence_hashes']['decoded-mux/'+f.name]=sha(f.read_bytes())
    (output/'timing-paths-native.json').write_text(json.dumps(record,indent=2)+'\n')
    for n in [Path(__file__).name,'test_mc10164_decoded.py']:
        (output/n).write_bytes(Path(__file__).with_name(n).read_bytes())
    assert len(record['modified_RTL_sources'])==5 and not record['assembly_permitted']
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','qualification','generated','miter','pins','board','direct-alu','mux','output','project']:
        p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args()
    d={n:(v if n=='project' else v.resolve()) for n,v in vars(a).items()}
    result=stage(**d)
    print(result['status'],len(result['source_hashes']),'sources; five RTL changes; no assembly/load')
