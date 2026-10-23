#!/usr/bin/env python3
"""Trace unused l17-graph package-half inputs and retain its actual live B bus.

The newly resolved RBMux.04 reaches l17.D2. Eight new input seams feed unused
halves in this selected graph; eight earlier unused inputs also remain. These
signals remain live in the full Dorado and must not be tied off globally.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch
from audit_r_front_command_boundaries import audit
from generate_conta_local_reference import plan
from generate_r_front_l17_reference import EXTRAS
from inventory_mir_clock_boundaries import ROOT,BOARD
from sil_netlist import load_board


def run(reference,out):
    assert not out.exists();manifest=json.loads((reference/'manifest.json').read_text());bindings=manifest['boards'][0]['boundary_bindings']
    _,older=plan(ROOT)
    unused={n for n in older if n in bindings}|{'Call12','Call15'}|(set(EXTRAS)-{'BMux.04'})
    assert len(unused)==16
    board=load_board(str(ROOT/BOARD))
    pin_net={(p['pkg'],p['pin']):n for n,net in board.nets.items()for p in net['pins']}
    expected={('l17',9):"clk1'Da",('j19',2):"clk1'Da",('j19',5):"preclk1'Da",
              ('f13',4):"preclk1'Da",('f13',5):"preFH'",('f13',6):"prepreclk'a",('f13',7):"bCLKEnable'a",
              ('l06',3):"preFH'",('d17',10):'ContA28.sil+2',('l17',4):'ContA28.sil+2',
              ('l17',6):'ContA28.sil+1',('d17',4):'ContA28.sil+1',('l17',7):'ContA28.sil+5',
              ('g03',2):'ContA28.sil+5',('g03',4):'ContA28.sil+1',('g03',5):'RBMux.04',
              ('e01',3):'RBMux.04',('e01',6):'BMux.04',('e01',9):'TrueAC',('g05',13):'TrueAC'}
    for key,value in expected.items():assert pin_net[key]==value,(key,value)
    for key in [('j19',6),('j19',7),('g05',9),('g05',10),('l17',12)]:assert key not in pin_net,key
    assert manifest['boards'][0]['sequential']['l06']==[20,2]
    for pkg,kind in [('l17','MC10176'),('j19','SE10212'),('f13','SE10210'),('l06','SE10231'),
                     ('g03','MC10104'),('e01','MC10197'),('g05','MC10195')]:assert board.packages[pkg]['type']==kind
    assert "assign p13 = ~(p9 ^ p10);"in (ROOT/'verilog/cells/cell_MC10195.v').read_text()
    assert {(p['pkg'],p['pin'])for p in board.nets['TrueAC']['pins']if p['dir']=='out'}=={('g05',13)}
    records,cells=audit(ROOT,manifest,boundaries=unused);controls=[]
    for observe in [True,False]:
        m=copy.deepcopy(manifest)
        if not observe:
            for n in ['RBMux.04','ContA28.sil+5']:m['boards'][0]['flag_taps'].remove(n)
        try:audit(ROOT,m,boundaries=['BMux.04'])
        except AssertionError as e:
            assert ('OBSERVED OUTPUT'if observe else'SELECTED REGISTER')in str(e),e
            controls.append(dict(case='BMux04_live_observation'if observe else'BMux04_actual_l17_input',diagnostic=str(e)))
        else:raise AssertionError('Live B bus classified unused')
    for label,kind,before,after in [
        ('BMux12_into_RBMux04','MC10197','assign p3  = p9 & p6;','assign p3  = p9 & (p6 | p7);'),
        ('CPReg07_into_l17_feedback','MC10104','assign p2 = (p4 & p5);','assign p2 = (p4 & p5) | p6;')]:
        p=ROOT/'verilog/cells'/('cell_'+kind+'.v');raw=p.read_text();assert raw.count(before)==1;changed=raw.replace(before,after);original=Path.read_text
        def read(path,*args,**kwargs):return changed if path==p else original(path,*args,**kwargs)
        with patch.object(Path,'read_text',read):
            try:audit(ROOT,manifest,boundaries=unused)
            except AssertionError as e:
                assert any(x in str(e)for x in ['OBSERVED OUTPUT','SELECTED REGISTER']),e
                controls.append(dict(case=label,diagnostic=str(e)))
            else:raise AssertionError('Live malformed dependency accepted')
    m=copy.deepcopy(manifest);m['boards'][0]['flag_taps'].append('RBMux.12')
    try:audit(ROOT,m,boundaries=unused)
    except AssertionError as e:
        assert 'OBSERVED OUTPUT'in str(e),e;controls.append(dict(case='RBMux12_exported',diagnostic=str(e)))
    else:raise AssertionError('Exported B bus classified unused')
    out.mkdir(parents=True)
    names={BOARD,BOARD.replace('.wl','.lc'),'verilog/cells/cell_MC10195.v',*cells}
    for module in list(sys.modules.values()):
        n=getattr(module,'__file__',None)
        if n and Path(n).resolve().parent==ROOT/'tools':names.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes={}
    for n in sorted(names):
        p=out/'inputs'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=hashlib.sha256(p.read_bytes()).hexdigest()
    (out/'reference-manifest.json').write_bytes((reference/'manifest.json').read_bytes())
    r=dict(status='PASS_L17_SELECTED_BOUNDARIES',scope=__doc__,unused_boundaries=records,rejected_controls=controls,
           live_boundary=dict(name='BMux.04',pin=317,via='e01 RBMux.04 -> g03 -> l17.D2'),inputs=hashes,
           physical_clock_and_feedback_pin_checks=len(expected),
           actual_trueac=dict(source='g05.13 MC10195',unwired_inputs_low=[9,10],expression='~(0 ^ 0) = 1'),
           l17_clock_scope='clk1 Da follows preFH complement of physical Q20; full profile comparison remains separate from this wiring audit',
           full_machine_boundaries_inactive=False,native_integrated=False)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(records),len(controls))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reference.resolve(),a.output.resolve())
