#!/usr/bin/env python3
"""Bind native root polarity and classify exact15ns source packet geometry.

Use all160000 independently referenced source packets, every CPU rotation
and the full300us period. Check constant debug plus every possible single
debug transition at the boundary or a source record. Retain a two-transition
counterexample so arbitrary debug timing is never silently restricted.
This audits source packets, not CP input epochs or full control equivalence.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil

from sil_netlist import load_board
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def audit(tests,stage,out):
    assert not out.exists()
    s=json.loads((tests/'run-state.json').read_text())
    assert s['status']=='PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES'
    assert len(s['cases'])==12 and all(c['passed'] for c in s['cases'])
    for name,digest in s['inputs'].items():
        assert sha(tests/'inputs'/name)==digest
    manifest=stage/'r-front-control-native.json';m=json.loads(manifest.read_text())
    for name,digest in m['source_hashes'].items():
        assert sha(stage/name)==digest
    wl=ROOT/'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl'
    board=load_board(str(wl))
    pins={(p['pkg'],p['pin']):n for n,net in board.nets.items() for p in net['pins']}
    assert board.packages['j02']['type']==board.packages['l01']['type']=='MC10210'
    for pin,net in [(5,"StartClockPulse'"),(6,'EndClockPulse'),(7,'dStartClockPulse')]:
        assert pins['j02',pin]==net
    assert pins['j02',3]==pins['l01',7]==pins['l01',11]
    assert pins['l01',2]=="CLK.ca'" and pins['l01',4]=="CLK.cb'"
    assert not any((p,k) in pins for p in ['l01'] for k in [5,6,9,10])
    cell=stage/'src/verilog/cells/cell_MC10210.v'
    text=cell.read_text()
    for pin,expr in [(2,'(p5 | p6 | p7)'),(3,'(p5 | p6 | p7)'),(4,'(p5 | p6 | p7)')]:
        assert f'assign p{pin} = {expr};' in text
    bp=stage/'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    assert 'assign CLK_ca_p_ = CLK_ca_p___BaseBd;' in bp.read_text()
    assert '.clock_before(r_front_clock_before),.clock_after(CLK_ca_p_)' in bp.read_text()
    before_bits=[];records=[]
    for line in (tests/'inputs/expected.hex').read_text().splitlines():
        value=int(line,16);count=(value>>180)&7
        assert count<=6
        before=(value>>189)&63
        levels=[(value>>(6*i))&63 for i in range(count)]
        before_bits.append(before);records.append(levels)
    assert len(records)==160000
    hist=Counter();normal=Counter();per_rotation=[Counter() for i in range(8)]
    checked=0;first_double=first_bad_debug=None

    def edges(before,levels,initial,flips):
        debug=initial;previous=bool((before&3) or debug);result=[]
        for i,level in enumerate([before,*levels]):
            if i in flips:debug^=1
            current=bool((level&3) or debug)
            if current!=previous:result.append([i,int(current)])
            previous=current
        return result

    for i,(before,levels) in enumerate(zip(before_bits,records)):
        e=edges(before,levels,0,())
        normal[len(e)]+=1;per_rotation[i//20000][len(e)]+=1
        if len(e)==2 and first_double is None:
            first_double=dict(rotation=i//20000,tick=i%20000,before=before,levels=levels,edges=e)
        for initial in [0,1]:
            for flip in [-1,*range(len(levels)+1)]:
                e=edges(before,levels,initial,() if flip<0 else (flip,))
                assert len(e)<=2 and (len(e)<2 or [x[1] for x in e]==[0,1]),(i,initial,flip,e)
                hist[len(e)]+=1;checked+=1
        if first_bad_debug is None:
            for a in range(len(levels)+1):
                for b in range(a+1,len(levels)+1):
                    e=edges(before,levels,0,(a,b))
                    if len(e)>2 or len(e)==2 and [x[1] for x in e]!=[0,1]:
                        first_bad_debug=dict(rotation=i//20000,tick=i%20000,before=before,levels=levels,
                            initial_debug=0,debug_flip_steps=[a,b],edges=e)
                        break
                if first_bad_debug:break
    assert first_double and first_bad_debug
    assert all(per_rotation[p].get(2,0)==0 for p in [0,2,3,4,6,7])
    assert per_rotation[1][2]>0 and per_rotation[5][2]>0
    out.mkdir(parents=True)
    inputs=out/'inputs';inputs.mkdir()
    for name,path in [('BaseBd.wl',wl),('cell_MC10210.v',cell),('native-backplane.v',bp),
                      ('expected.hex',tests/'inputs/expected.hex')]:
        shutil.copy2(path,inputs/name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='PASS_NATIVE_ROOT_BINDING_AND_PACKET_GEOMETRY',scope=__doc__,
        source_tests=str(tests),source_tests_state_sha256=sha(tests/'run-state.json'),
        native_stage=str(stage),native_manifest_sha256=sha(manifest),
        CLK_ca_p_polarity='same as SCP-prime OR ECP OR debug; j02.p3 -> l01.p7 -> l01.p2',
        intervals=160000,joint_period_ns=300000,CPU_rotations=8,debug_schedules_checked=checked,
        constant_debug_zero_edges=dict(normal),single_debug_transition_edges=dict(hist),
        per_rotation=[dict(c) for c in per_rotation],two_edge_order='fall then rise',
        first_normal_two_edge_packet=first_double,two_debug_transition_counterexample=first_bad_debug,
        actual_CP_or_debug_transition_rate_assumed=False,arbitrary_debug_requires_generic_fallback=True,
        production_rotation_zero_has_at_most_one_normal_root_edge=True,
        all_other_rotations_retained_and_checked=True,control_pin_epoch_order_qualified=False,
        native_consumer_integrated=False,timing_qualified=False,firmware_qualified=False,speedup_qualified=False,
        hardware_changed=False,files={p.name:sha(p) for p in inputs.iterdir()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ['status','intervals','debug_schedules_checked','constant_debug_zero_edges','per_rotation']}),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['tests','stage','output']:
        p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();audit(a.tests.resolve(),a.stage.resolve(),a.output.resolve())
