#!/usr/bin/env python3
"""Stage proven falling old-word capture and early reply on the mux candidate.

Rebuild from exact qualified690f sources; six RTL files change in total.
Clocks/constraints/Main/daemon remain unchanged. Requires immutable component
proofs including the combined original-MSA replay. Fit/reports only, no assembly.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from stage_decoded_mux_native import stage as mux_stage
from prepare_storage_early_read import transform as early_transform
from test_storage_old_word_falling import transform as old_word_transform, GROUP
from stage_storage_read_word_native import BACKEND, WRAPPER

ROOT=Path(__file__).resolve().parents[1]
RECEIPTS={
    'early':('2026-10-22-storage-early-read/component','137fbdd338ce9daf32be2753525fc47ab1e7d092e8f52da7a03c763632557eb8'),
    'old_word':('2026-10-22-storage-old-word-falling/component','352517cbba41f0b5b90f7241a6b8b8215218ebb784adbb32bb40a0b48714ad5e'),
}


def sha(b):return hashlib.sha256(b).hexdigest()


def check_component(name,working):
    path,digest=RECEIPTS[name];receipt=ROOT/'fpga/mister/reports'/path
    raw=(receipt/'manifest.json.gz').read_bytes();assert sha(raw)==digest
    manifest=json.loads(gzip.decompress(raw))
    for member in manifest['members']:
        n=member['name']
        if n.startswith(('tools/','freeze')):continue
        f=working/n
        assert f.is_file() and sha(f.read_bytes())==member['blob'],(name,n)
    return raw


def stage(parent,qualification,generated,miter,pins,board,direct_alu,mux,early,old_word,output,project):
    assert not output.exists()
    frozen={name:check_component(name,path) for name,path in [('early',early),('old_word',old_word)]}
    unit=json.loads((old_word/'unit-v1/run-state.json').read_text())
    assert unit['status']=='PASS_OLD_WORD_FALLING_GROUP_TEMPORAL_TWO_SIMULATORS'
    assert len(unit['results'])==18 and all('returncode'in a for a in unit['actions'])
    for engine in ['icarus','verilator']:
        for width in [2,8,12,16]:
            for fast in [0,1]:assert unit['results'][f'{engine}-w{width}-fast{fast}']['passed']
        assert unit['results'][engine+'-w16-fast1-bad']['rejected']
    for n in ['board-v1','board-early-v1']:
        state=json.loads((old_word/n/'run-state.json').read_text())
        assert state['status']=='PASS_OLD_WORD_FALLING_ORIGINAL_MSA_EXACT_HOST_TRACE'
        assert state['all_original_stored_bits']==9437184
        assert state['results']['reference']['raw_trace_sha256']==state['results']['falling-old-word']['raw_trace_sha256']
    assert (old_word/'unit-v1/inputs/dorado_storage_group.sv').read_bytes()==old_word_transform((parent/GROUP).read_bytes())
    r=mux_stage(parent,qualification,generated,miter,pins,board,direct_alu,mux,output,project)
    assert len(r['modified_RTL_sources'])==5
    (output/BACKEND).write_bytes(early_transform((output/BACKEND).read_bytes()))
    assert (output/BACKEND).read_bytes()==(early/'generated/dorado_storage_sdram_burst.sv').read_bytes()
    wrapper=(output/WRAPPER).read_bytes();old=b'.CAPTURE_READ_WORD(1)) u_sdram('
    assert wrapper.count(old)==1
    (output/WRAPPER).write_bytes(wrapper.replace(old,b'.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(1)) u_sdram('))
    (output/GROUP).write_bytes(old_word_transform((output/GROUP).read_bytes()))
    for name,raw in frozen.items():
        target=output/'evidence/old-word-and-early'/(name+'-manifest.json.gz')
        target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
        r['evidence_hashes']['old-word-and-early/'+target.name]=sha(raw)
    r.update(status='STAGED_OLD_WORD_FALLING_EARLY_NATIVE_REVIEW_REQUIRED',scope=__doc__,
        modified_RTL_sources=sorted(r['modified_RTL_sources']+[GROUP]),
        source_hashes={n:sha((output/n).read_bytes()) for n in r['source_hashes']},
        old_word_falling=True,early_read_response=True,response_valid_cycles_unchanged=False,
        architectural_capture_cycles_changed=False,new_transport_control_bits_expected=128,
        frozen_component_receipts={n:dict(path=path,sha256=h)for n,(path,h)in RECEIPTS.items()})
    (output/'timing-paths-native.json').write_text(json.dumps(r,indent=2)+'\n')
    for n in [Path(__file__).name,'prepare_storage_early_read.py','test_storage_old_word_falling.py']:
        (output/n).write_bytes((ROOT/'tools'/n).read_bytes())
    assert len(r['modified_RTL_sources'])==6 and not r['assembly_permitted']
    return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','qualification','generated','miter','pins','board','direct-alu','mux','early','old-word','output','project']:
        p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=stage(**{n:v if n=='project'else v.resolve()for n,v in vars(a).items()})
    print(r['status'],len(r['source_hashes']),'sources; six RTL changes; no assembly/load')
