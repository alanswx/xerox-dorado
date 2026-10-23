#!/usr/bin/env python3
"""Check the original MemC phase/hold clock and selected unused gate inputs.

Verify all three wired-OR Hold drivers, phase-register complement paths and
distinct phase-1 clocks. The full CA/MCR and hold-data producers remain open.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch
import audit_r_front_command_boundaries as dependencies
from generate_contb_hold_reference import ROOT,BOARD,SEQ
from sil_netlist import load_board

MC='chm/sil/'+BOARD+'.dm!1_/'+BOARD+'.wl'


def run(reference,out):
    assert not out.exists();manifest=json.loads((reference/'manifest.json').read_text());selected=manifest['boards'][1]
    b=load_board(str(ROOT/MC));pins={(p['pkg'],p['pin']):n for n,r in b.nets.items()for p in r['pins']}
    expected={('l02',6):"CLK.mc'",('l02',9):"MemClkEnable'a",('l02',7):'MemSH',
              ('l02',2):"ppSH'",('l02',3):"ppFH'",('l02',13):'Hia',('l02',14):'Hia',('l02',15):'Hia',
              ('l06',12):"ppFH'",('l06',15):"preFH'x",('l06',14):"preSH'x",
              ('l01',7):"CLK.mc'",('l01',5):"MemClkEnable'a",('l01',11):"CLKEnable'b",('l01',3):"ppclk2'a",('l01',12):'IgnoreProc',
              ('h12',10):"ppclk2'a",('h12',11):"preFH'x",('h12',12):"preClk1'B",('h12',14):"preClk1'D",
              ('i18',11):"preClk1'D",('i18',14):"clk1'Dc",('j18',9):'IgnoreProc',('j18',11):"preClk1'D",('j18',12):"PrClk1'Da",
              ('e22',9):"PrClk1'Da",('e22',5):'DisHold',('e22',12):'DisHold',('e22',7):'MemC15.sil+2',('e22',10):"dMDhold'",
              ('e23',6):"PrClk1'Da",('e23',11):"clk1'Dc",('e23',4):'DisHold',('e23',12):'DisHold',
              ('e23',7):'MemC15.sil+4',('e23',10):'MemC15.sil+5',
              ('e22',3):'Hold',('e22',14):'Hold',('e23',2):'Hold',('d23',9):'Hold',('d23',12):'PRhold',('d23',14):'CBHold'}
    for key,n in expected.items():assert pins[key]==n,(key,n)
    for key in [('l02',11),('l06',13),('h12',9),('i18',9),('i18',10),('j18',10),('d23',10),('d23',11)]:assert key not in pins,key
    assert selected['sequential']=={k:list(v)for k,v in SEQ.items()}
    assert {(p['pkg'],p['pin'])for p in b.nets['Hold']['pins']if p['dir']=='out'}=={('e22',3),('e22',14),('e23',2)}
    assert {(p['pkg'],p['pin'])for p in b.nets['Hia']['pins']if p['dir']=='out'}=={('l02',14),('l02',15)}
    assert 'assign cb_pins={pins[55:4],memory_flags[1],pins[2:1],cb_root};'in (reference/'contb_hold_reference.sv').read_text()
    m=dict(boards=[selected]);unused=manifest['memory_extra_boundaries'];assert len(unused)==8;controls=[]
    with patch.object(dependencies,'BOARD',MC):
        records,cells=dependencies.audit(ROOT,m,boundaries=unused)
        for n in ['MemSH',"dMDhold'"]:
            try:dependencies.audit(ROOT,m,boundaries=[n])
            except AssertionError as error:
                assert 'SELECTED REGISTER'in str(error),error;controls.append(dict(case=n+'_must_remain_live',diagnostic=str(error)))
            else:raise AssertionError('Live register source classified unused')
        exported=copy.deepcopy(m);exported['boards'][0]['flag_taps'].append("LdVNV'")
        try:dependencies.audit(ROOT,exported,boundaries=unused)
        except AssertionError as error:
            assert 'OBSERVED OUTPUT'in str(error),error;controls.append(dict(case='LdVNV_exported',diagnostic=str(error)))
        else:raise AssertionError('Exported output classified unused')
        for label,kind,before,after in [
            ('DisBR_into_phase1','MC10105','assign p15 =  (p12 | p13);','assign p15 =  (p12 | p13 | p9);'),
            ('VicInPair_into_hold_clock','SE10210','assign p12 = (p9 | p10 | p11);','assign p12 = (p9 | p10 | p11 | p5);')]:
            path=ROOT/'verilog/cells'/('cell_'+kind+'.v');raw=path.read_text();assert raw.count(before)==1;changed=raw.replace(before,after);original=Path.read_text
            def read(p,*args,**kwargs):return changed if p==path else original(p,*args,**kwargs)
            with patch.object(Path,'read_text',read):
                try:dependencies.audit(ROOT,m,boundaries=unused)
                except AssertionError as error:
                    assert 'SELECTED REGISTER'in str(error),error;controls.append(dict(case=label,diagnostic=str(error)))
                else:raise AssertionError('Changed live dependency accepted')
    out.mkdir(parents=True);sources={MC,MC.replace('.wl','.lc'),*cells}
    for module in list(sys.modules.values()):
        n=getattr(module,'__file__',None)
        if n and Path(n).resolve().parent==ROOT/'tools':sources.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes={}
    for n in sorted(sources):
        p=out/'inputs'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=hashlib.sha256(p.read_bytes()).hexdigest()
    (out/'reference-manifest.json').write_bytes((reference/'manifest.json').read_bytes())
    r=dict(status='PASS_SELECTED_MEMC_PHASE_HOLD_BOUNDARIES',scope=__doc__,physical_pin_checks=len(expected),unwired_low_pin_checks=8,
           unused_boundaries=records,rejected_controls=controls,inputs=hashes,
           obsolete_top_input=dict(pin=3,name='CBHold',actual_source='MemC d23.14 -> coupled memory_flags[1]'),
           original_hia='l02.Q1 OR !l02.Q1 drives its own asynchronous reset; binary Hia=1/Q1=0 is physical wiring, not added initialization',
           remaining_producers=['CA MemSH/clock enables/BNT/UseDMD/reset','DisHold from original MCR k08.Q5','reference/Md/miscellaneous hold data','IM/parity/DMD'],
           native_integrated=False,physical_timing_qualified=False,full_producer_arrival_contract_qualified=False)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(records),len(controls))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reference.resolve(),a.output.resolve())
