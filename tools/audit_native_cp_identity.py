#!/usr/bin/env python3
"""Bind the conditional CP identity proof to the staged native pin registers.

This confirms a source seam, not a native runtime proof or implementation.
A fast path still needs an explicit canonical/last-settled guard, first-step
fallback and qualified maintenance-only staging with all board consumers held.
"""
import argparse
import json
from pathlib import Path
import re

from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def audit(stage,identity,bad,out):
    assert not out.exists()
    good=json.loads((identity/'run-state.json').read_text())
    negative=json.loads((bad/'run-state.json').read_text())
    assert good['status']=='PASS_CONDITIONAL_SETTLED_CP_IDENTITY'and good['proof_succeeded']
    assert good['conditional_settled_identity']and not good['identity_before_pin_perturbation']
    assert negative['status']=='PASS_RTL_COUNTEREXAMPLE_CONTROL'and negative['counterexample_found']
    assert negative['identity_before_pin_perturbation']
    for directory,r in [(identity,good),(bad,negative)]:
        for name,h in r['source_files'].items():assert sha(directory/name)==h,name
    record=json.loads((stage/'r-front-control-native.json').read_text())
    name='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    path=stage/name;assert sha(path)==record['source_hashes'][name]
    raw=path.read_text()
    block='''  always @(posedge storage_clk)if(contb_advance)begin
    r_front_lower_q<=r_front_next[571:0];r_front_upper_q<=r_front_next[843:588];
    r_front_before<=r_front_live;r_front_clock_before<=CLK_ca_p_;
  end'''
    assert raw.count(block)==1
    assert len(re.findall(r'\br_front_before\s*<=',raw))==1
    assert len(re.findall(r'\br_front_clock_before\s*<=',raw))==1
    for snippet in ['.held_q(r_front_held)',
                    '.clock_before(r_front_clock_before),.clock_after(CLK_ca_p_)',
                    '.pins_before(r_front_before),.pins_after(r_front_live)',
                    '.held_next(r_front_next)']:
        assert raw.count(snippet)==1,snippet
    qsf=stage/'core/Dorado.qsf'
    assert record['project']+'/'+name in qsf.read_text()
    out.mkdir(parents=True)
    for n,p in [('native-backplane.v',path),('native-source-manifest.json',stage/'r-front-control-native.json'),
                ('native.qsf',qsf),('identity.json',identity/'run-state.json'),('negative.json',bad/'run-state.json')]:
        (out/n).write_bytes(p.read_bytes())
    result=dict(status='SOURCE_BOUND_NATIVE_PREVIOUS_PIN_SEAM_CONFIRMED',scope=__doc__,
        source_stage=str(stage),source_backplane=name,source_sha256=sha(path),
        source_stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        identity_proof_sha256=sha(identity/'run-state.json'),bad_before_proof_sha256=sha(bad/'run-state.json'),
        native_before_pins_are_saved_previous_after_argument=True,
        native_before_clock_is_saved_previous_after_argument=True,
        old_state_and_pin_registers_share_commit_enable=True,
        identity_requires_last_atomic_transaction_settled=True,
        simultaneous_CP_root_still_requires_ordered_prefix=True,
        runtime_canonical_guard_implemented=False,first_step_fallback_implemented=False,
        conditional_maintenance_pipeline_implemented=False,all_board_pause_consumers_qualified=False,
        source_seam_is_staged_not_deployed=True,native_qualified=False,hardware_changed=False,speedup_qualified=False,
        next_action='Qualify ordinary prefix bypass and a CP-event-only staged path, with exact saved pins/root/state, first-step/settled guards, single SRAM commit and global pause validation; preserve60ns ordinary budget.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['stage','identity','bad','output','archive']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();result=audit(args.stage.resolve(),args.identity.resolve(),args.bad.resolve(),args.output.resolve())
    append_archive(args.archive.resolve(),args.output.resolve(),[],label='cp-identity-native-seam',extra_tools=[Path(__file__).name])
    print(result['status'],flush=True)
