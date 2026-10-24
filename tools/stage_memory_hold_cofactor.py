#!/usr/bin/env python3
"""Stage the proved late-Hold atomic cofactors in the actual native caller.

Keep branch results as combinational synthesis boundaries so the timing test
measures late selection, not an optimizer's inverse refactoring. Removing only
those attributes recovers the exact proved rewrite. All original owners,
cache policy, SRAMs/ports, clocks, capture epochs and constraints are unchanged.
This permits an isolated fit, never assembly, native boot or a speed claim.
"""
import argparse
import json
from pathlib import Path

from cofactor_memory_hold import transform
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

CONTROL='src/verilog/rtl/dorado_r_front_control_step.sv'


def kept(raw):
    replacements=[
        (' wire [155:0] low_q,high_q,unknown_q;',
         ' (* keep = "true" *) wire [155:0] low_q,high_q;\n wire [155:0] unknown_q;'),
        (' wire [55:0] low_events,high_events,unknown_events,low_repeated,high_repeated,unknown_repeated;',
         ' (* keep = "true" *) wire [55:0] low_events,high_events,low_repeated,high_repeated;\n wire [55:0] unknown_events,unknown_repeated;'),
        (' wire low_unsettled,high_unsettled,unknown_unsettled;',
         ' (* keep = "true" *) wire low_unsettled,high_unsettled;\n wire unknown_unsettled;'),
        (' wire [93:0] low_flags,high_flags,unknown_flags;',
         ' (* keep = "true" *) wire [93:0] low_flags,high_flags;\n wire [93:0] unknown_flags;'),
    ]
    result=raw
    for old,new in replacements:
        assert result.count(old)==1;result=result.replace(old,new,1)
    restored=result
    for old,new in replacements:restored=restored.replace(new,old,1)
    assert restored==raw
    return result


def read(path):return json.loads(path.read_text())


def stage(parent,evidence,out,project):
    assert not out.exists()and project.is_absolute()
    manifest=read(parent/'r-front-control-native.json')
    assert manifest['frequency_hz']==57142857 and manifest['sysper']==4
    assert manifest['balanced_task_priority_proven']and not manifest['assembly_permitted']
    for name,digest in manifest['source_hashes'].items():assert sha(parent/name)==digest,name
    proofs=[]
    for variant in ['positive','negative']:
        d=evidence/f'atomic-memory-hold-{variant}-proof-v2-20261024';p=read(d/'run-state.json')
        assert p['status']==('PASS_LATE_MEMORY_HOLD_ATOMIC_EQUIVALENCE'if variant=='positive'else'PASS_LATE_MEMORY_HOLD_COUNTEREXAMPLES')
        assert [(r['waves'],r['passed'])for r in p['results']]==[(5,True),(24,True)]
        assert p['compared_output_bits']==363 and p['arbitrary_binary_input_and_state_bits']==955 and not p['assumptions_added']
        for name,digest in p['files'].items():assert sha(d/name)==digest,name
        assert (d/'candidate.sv').read_text()==transform((d/'original.sv').read_text(),variant=='negative')
        proofs.append(dict(directory=str(d),state_sha256=sha(d/'run-state.json'),status=p['status']))
    positive=evidence/'atomic-memory-hold-positive-proof-v2-20261024'
    original=(positive/'original.sv').read_text();proved=(positive/'candidate.sv').read_text()
    assert module(original,'dorado_control_hold_atomic_step')==module((parent/CONTROL).read_text(),'dorado_control_hold_atomic_step')
    comparisons=[]
    for engine in ['icarus','verilator']:
        d=evidence/f'atomic-memory-hold-{engine}-v1-20261024';p=read(d/'run-state.json')
        assert p['status']=='PASS_MEMORY_HOLD_COMPLETE_ORIGINAL_COMPARISON'
        assert p['all100_clock_counts_match']and p['original_testbench_and_references_unchanged']
        assert (d/'candidate.sv').read_text()==proved
        for name,digest in p['files'].items():assert sha(d/name)==digest,name
        for row in p['source_files']:
            assert row['changed']==(row['path']=='candidate.sv')
            assert sha(d/row['path'])==row['output_sha256']
        bad=evidence/f'atomic-memory-hold-bad-selection-{engine}-v1-20261024';n=read(bad/'run-state.json')
        assert n['actions'][0]['returncode']==0 and n['actions'][-1]['returncode']!=0
        assert (bad/'candidate.sv').read_text()==transform(original,True)
        assert 'R front resolved source mismatch check=15'in(bad/'test.log').read_text()
        assert (bad/'inputs/tb_r_front_local.sv').read_bytes()==(d/'inputs/tb_r_front_local.sv').read_bytes()
        comparisons.append(dict(engine=engine,directory=str(d),state_sha256=sha(d/'run-state.json'),
            cache_coverage=p['cache_coverage'],negative_directory=str(bad),
            negative_state_sha256=sha(bad/'run-state.json'),negative_original_reference_check=15,
            negative_first_diagnostic_parser_too_narrow=n['status']=='TERMINAL_MEMORY_HOLD_TEST_NOT_ESTABLISHED'))
    for variant,status in [('four-state','PASS_MEMORY_HOLD_FOUR_STATE'),('bad-unknown','PASS_MEMORY_HOLD_SIMULATOR_COUNTEREXAMPLE')]:
        d=evidence/f'atomic-memory-hold-{variant}-v1-20261024';p=read(d/'run-state.json')
        assert p['status']==status
        for name,digest in p['files'].items():assert sha(d/name)==digest,name
    changed=kept(transform((parent/CONTROL).read_text()))
    out.mkdir(parents=True)
    for name in manifest['source_hashes']:
        raw=changed.encode()if name==CONTROL else(parent/name).read_bytes()
        if Path(name).suffix in ['.qsf','.qpf','.qip','.tcl','.sdc']:
            raw=raw.replace(manifest['project'].encode(),str(project).encode())
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    changes=[name for name in manifest['source_hashes']if(out/name).read_bytes().replace(str(project).encode(),manifest['project'].encode())!=(parent/name).read_bytes()]
    assert changes==[CONTROL]
    record={**manifest,'project':str(project),'scope':__doc__,'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
        'late_memory_hold_cofactor':dict(pin=386,compared_outputs=363,proofs=proofs,
            original_graph_comparisons=comparisons,four_state_checks=128,unknown_selector_checks=64,
            combinational_branch_boundaries_kept=True,unknown_fallback_kept=False,
            synthesis_attributes_inverse_byte_identical=True,
            source_proof=str(positive),source_proof_state_sha256=sha(positive/'run-state.json'),
            extra_state_or_guest_cycles=0,SRAM_or_capture_changes=False),
        'source_hashes':{name:sha(out/name)for name in manifest['source_hashes']},
        'physical_timing_qualified':False,'firmware_qualified':False,
        'speedup_qualified':False,'assembly_permitted':False,'hardware_changed':False}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    for name in [Path(__file__).name,'cofactor_memory_hold.py']:
        (out/name).write_bytes((Path(__file__).parent/name).read_bytes())
    print('STAGED_PROVED_ATOMIC_MEMORY_HOLD_COFACTORS',len(manifest['source_hashes']),'sources',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['parent','evidence','output','project']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();stage(a.parent.resolve(),a.evidence.resolve(),a.output.resolve(),a.project)
