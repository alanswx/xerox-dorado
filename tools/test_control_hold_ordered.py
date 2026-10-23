#!/usr/bin/env python3
"""Test transfer logic conditional on independently specified arrival order.

Same original gate reference and full156-Q/56-clock/94-flag comparison. Derive
descriptors from source routes and predeclared delay values, never expected
outputs. Root-only failures remain valid; native phase generation is separate.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch
import test_control_hold as parent
from generate_control_hold_ordered import generate

MUTATIONS={
    # A newly captured WantRunRfsh Q can arrive before the run clock, but its
    # extra g04 AND delay prevents the new value reaching l03 D in time.
    'new_Want_data':('    d_mem_run=q[5] | (q[19] & !q[21]);\n',
                     '    d_mem_run=q[5] | (pins[4] & !q[21]);\n'),
}


def descriptor(profile):
    gate,ordinary_ff,phase_ff=parent.PROFILES[profile]
    # l06 Q reaches Set directly; f13 adds one clock-distribution gate. Equal
    # simulation times retain old Q at capture before nonblocking Q updates.
    return int(phase_ff<gate)


def run(out,engine,profile,forced=None,mutation=None):
    expected=descriptor(profile);arrival=expected if forced is None else forced
    before='dorado_control_hold_step dut(q,before_root,after_root,before_pins,after_pins,next_q,events,repeated,unsettled,flags);'
    after=f"dorado_control_hold_ordered_step dut(q,before_root,after_root,before_pins,after_pins,next_q,events,repeated,unsettled,1'b{arrival},flags);"
    assert parent.TB.count(before)==1;bench=parent.TB.replace(before,after)
    with patch.object(parent,'TB',bench),patch.object(parent,'candidate_generate',generate),patch.dict(parent.MUTATIONS,MUTATIONS):
        rc=parent.run(out,engine,profile,mutation=mutation)
    path=out/'run-state.json';r=json.loads(path.read_text())
    r.update(experiment='explicit_original_control_arrival',expected_control_arrival=expected,executed_control_arrival=arrival,
             forced_arrival=forced,arrival_derivation='phase_FF < gate; direct l06.B to l03 Set versus f13 capture clock; data has an additional g04/h03 gate',
             root_only_clock_generation_qualified=False)
    path.write_text(json.dumps(r,indent=2)+'\n');return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='icarus')
    p.add_argument('--profile',choices=parent.PROFILES,default='later_control');p.add_argument('--force-arrival',type=int,choices=range(2))
    p.add_argument('--mutation',choices=MUTATIONS)
    a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.profile,a.force_arrival,a.mutation))
