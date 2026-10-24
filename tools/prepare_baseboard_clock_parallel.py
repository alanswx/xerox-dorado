#!/usr/bin/env python3
"""Prepare an isolated single-crystal-transfer BaseBoard consumer candidate.

Bind to the package-tested six-Q implementation and the separately proved
source contract: at most one BB transition and consistent BB endpoint.
Do not select this candidate or change native hardware/clock/guest timing.
"""
import argparse
import json
from pathlib import Path
import shutil

from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BODY = ''' reg [12:0] crystal_transfer,root_transfer;
 reg [5:0] current_levels,prior_levels,selected_state;
 reg current_debug,prior_debug,rising;
 reg [6:0] selected_events;
 always @*begin
  // Exactly one possible crystal rise, independent of its packet position.
  crystal_transfer=step(state_q,6'b000000,6'b010000,1'b0,1'b0);
  state_next=state_q;states_after_steps=0;events_at_steps=0;
  roots_after_steps=0;max_after_steps=0;mcpre_after_steps=0;aux_after_steps=0;
  current_levels=source_before;prior_levels=source_before;
  current_debug=debug_at_boundary;prior_debug=debug_before;
  selected_state=state_q;selected_events=0;root_transfer=0;rising=0;
  if(consume)for(integer index=0;index<7;index++)begin
   if(index==0 || index<=int'(event_count))begin
    if(index==0)begin
     prior_levels=source_before;current_levels=source_before;
     prior_debug=debug_before;current_debug=debug_at_boundary;
    end else begin
     prior_levels=(index==1) ? source_before : source_after_events[6*(index-2)+:6];
     current_levels=source_after_events[6*(index-1)+:6];
     prior_debug=(index==1) ? debug_at_boundary : debug_at_events[index-2];
     current_debug=debug_at_events[index-1];
    end
    rising=!prior_levels[4] && current_levels[4];
    selected_state=(!source_before[4] && current_levels[4]) ? crystal_transfer[5:0] : state_q;
    selected_events=rising ? crystal_transfer[12:6] : 7'b0;
    // Only root edge bits are used here; they do not depend on physical Qs.
    root_transfer=step(state_q,prior_levels,current_levels,prior_debug,current_debug);
    selected_events[4:3]=root_transfer[10:9];
    events_at_steps[7*index+:7]=selected_events;
    states_after_steps[6*index+:6]=selected_state;
    roots_after_steps[index]=current_levels[0]|current_levels[1]|current_debug;
    max_after_steps[index]=(selected_state[3:0]==4'hf);
    mcpre_after_steps[index]=!selected_state[4];aux_after_steps[index]=!selected_state[5];
   end
  end
  // Endpoint is an independent source output, never a last-slot priority mux.
  if(consume)state_next=(!source_before[4] && source_after[4]) ? crystal_transfer[5:0] : state_q;
 end
'''


def prepare(reference, contract, out):
    assert not out.exists()
    r = json.loads((reference/'run-state.json').read_text())
    c = json.loads((contract/'run-state.json').read_text())
    assert r['status'] == 'PASS_BASEBOARD_CLOCK_EVENT_CONSUMER_REFERENCES'
    assert len(r['cases']) == 12 and all(row['passed'] for row in r['cases'])
    assert c['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS'
    assert c['at_most_one_BB_transition_per_interval_proven']
    assert c['independent_BB_and_Ether_endpoints_proven']
    for name, digest in r['inputs'].items():
        assert sha(reference/'inputs'/name) == digest
    assert sha(contract/'merge-positive/source.sv') == r['inputs']['dorado_primary_clock_events.sv']
    out.mkdir(parents=True)
    for name in ['dorado_primary_clock_events.sv', 'dorado_baseboard_clock_step.sv']:
        shutil.copy2(reference/'inputs'/name, out/('original-'+name))
    raw = (out/'original-dorado_baseboard_clock_step.sv').read_text()
    begin = raw.index(' reg [5:0] cursor,levels_cursor;')
    end = raw.index('\nendmodule', begin)
    candidate = raw[:begin]+BODY+raw[end:]
    candidate = candidate.replace('state_q,source_before,', 'state_q,source_before,source_after,', 1)
    candidate = candidate.replace('// Original BaseBoard clock-cone consumer;', '// Single-crystal-transfer BaseBoard clock-cone candidate;', 1)
    (out/'dorado_baseboard_clock_step.sv').write_text(candidate)
    primary = (out/'original-dorado_primary_clock_events.sv').read_text()
    assert primary.count('  levels_after=cursor;') == 1
    primary = primary.replace('  levels_after=cursor;', '''  // BB/Ether endpoints are independent of where their records were inserted.
  levels_after={levels_before[5]^(ether_due && advance && !reset),
                levels_before[4]^(bb_due && advance && !reset),cursor[3:0]};''', 1)
    (out/'dorado_primary_clock_events.sv').write_text(primary)
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    state = dict(status='PREPARED_SINGLE_BB_TRANSFER_CANDIDATE', scope=__doc__,
        reference=str(reference), reference_state_sha256=sha(reference/'run-state.json'),
        source_contract=str(contract), source_contract_state_sha256=sha(contract/'run-state.json'),
        native_consumers_integrated=False, hardware_changed=False, extra_guest_cycles=0,
        files={p.name: sha(p) for p in out.iterdir() if p.is_file()})
    (out/'results.json').write_text(json.dumps(state, indent=2)+'\n')
    print(state['status'], flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['reference', 'contract', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    prepare(a.reference.resolve(), a.contract.resolve(), a.output.resolve())
