#!/usr/bin/env python3
"""Review the next processor-phase transfer without promoting pending work.

Preserve complete task/sparse Icarus outcomes, the SAME live full-core fit and
the separate122-input/109-output processor proof. Keep the unknown-default
diagnostic-only rejection distinct from a real state/event counterexample.
"""
import argparse
import json
from pathlib import Path
import re

from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_native_phase_and_task import proof,review as phase_review,TOOLS as PHASE_TOOLS
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[*PHASE_TOOLS,Path(__file__).name,'qualify_native_processor_terminal.py']


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    base=phase_review(out/'phase-task')
    d=BUILD/'native-processor-terminal-proof-v2-20261025'
    p=proof(d,3,'PASS_ACTUAL_NATIVE_PROCESSOR_TERMINAL_CLOSURE')
    assert p['arbitrary_binary_input_bits']==122 and p['compared_outputs']==109
    assert p['old_waves']==16 and p['candidate_waves']==5 and p['unknown_fallback_retained']
    before=(d/'native-processor-before.sv').read_text();after=(d/'native-processor-after.sv').read_text()
    from qualify_native_processor_terminal import transform
    from prove_cp_prefix_cache import module
    assert after==transform(before)
    # The actual CP/root split wrapper is left byte-identical. Only its
    # atomic evaluator changes; no input-arrival sequence is collapsed.
    assert module(before,'dorado_processor_capture_step')==module(after,'dorado_processor_capture_step')
    retain(d,out/d.name)
    failed=BUILD/'native-processor-terminal-proof-v1-20261025'
    assert read(failed/'failure.json')['status']=='REJECTED_WRONG_ORDERED_MODULE_SELECTION'
    retain(failed,out/failed.name)
    good=BUILD/'native-processor-terminal-xz-v1-20261025';g=checked(good)
    assert g['status']=='PASS_NATIVE_PROCESSOR_TERMINAL_XZ'and g['variable_bits']==122 and g['compared_bits']==109
    assert g['proof_state_sha256']==sha(d/'run-state.json')
    assert 'known=3 unknown=732 checks=735 outputs=109'in(good/'test.log').read_text()
    retain(good,out/good.name)
    bad=BUILD/'native-processor-terminal-bad-xz-v1-20261025';b=checked(bad)
    assert b['status']=='TERMINAL_NATIVE_TASK_TERMINAL_XZ_NOT_ESTABLISHED'
    assert b['actions'][0]['returncode']==0 and b['actions'][-1]['returncode']==1
    text=(bad/'test.log').read_text();assert re.findall(r'output=(\d+) ',text)==['0']
    assert 'actual=0 expected=x'in text
    retain(bad,out/bad.name)
    replay=[];pending=[]
    for mode in ['full','words']:
        for engine in ['icarus','verilator']:
            run=BUILD/f'native-processor-terminal-{mode}-{engine}-v1-20261025';s,row=observed(run)
            assert s['processor_terminal_proof_state_sha256']==sha(d/'run-state.json')
            assert sha(run/'inputs/verilog/rtl/dorado_processor_capture_step.sv')==sha(d/'native-processor-after.sv')
            assert s['original_assertions_and_reference_unchanged']and s['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            if s['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
                target=out/'pending-source-observations'/run.name;target.mkdir(parents=True)
                for source in s['source_files']:
                    f=target/source['path'];f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes((run/source['path']).read_bytes())
                (target/'run-state-at-observation.json').write_text(json.dumps(s,indent=2)+'\n')
                (target/'process-observation.json').write_text(json.dumps(row,indent=2)+'\n')
            else:
                assert s['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
                pattern=r'^(?:PACKET_CLOCK|FULL_PROCESSOR_PACKET_PASS|CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|CACHED_PACKET_COVERAGE|NATIVE_R_PACKET_COVERAGE) .*$'
                assert s['outcomes']==re.findall(pattern,(Path(s['source'])/'test.log').read_text(),re.M)
                retain(run,out/run.name)
            replay.append(row)
    result=dict(status='REVIEWED_NATIVE_PROCESSOR_TERMINAL_PROOF_AND_PENDING_REPLAYS',scope=__doc__,
        previous_phase_task_review=base,processor_proof=p,ordered_CP_root_wrapper_unchanged=True,
        four_state_checks=735,all109_fields_and_unknowns_compared=True,
        bad_unknown_fallback_diagnostic_counterexample=True,bad_unknown_fallback_state_or_event_counterexample=False,
        bad_unknown_trial_original_status=b['status'],bad_unknown_trial_not_promoted_to_functional_pass=True,
        original_one_wave_negative_retained=True,processor_replays=replay,pending_verified_live_replays=pending,
        processor_candidate_in_current_Quartus_job=False,processor_candidate_native_selected=False,
        firmware_or_60ns_or_SDRAM_timing_qualified=False,hardware_changed=False,measured_speed_gain=False,
        full_goal_active=True,next_action='Collect SAME task-terminal full fit and finish processor replays; use its real worst path to decide the next complete native candidate, retaining storage and address-consistency barriers')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-processor-terminal',extra_tools=TOOLS)
    print(r['status'],flush=True)
