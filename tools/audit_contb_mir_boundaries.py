#!/usr/bin/env python3
"""Check original ContB clock/reset seams and unused selected gate halves.

Clock-enable copies and named backplane pins are distinct. The local ContB
reset contribution joins ContA's driver; CBHold is not Hold or PRhold. The nine
extra package inputs are unused only in this selected component, never globally.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch
import audit_r_front_command_boundaries as dependencies
from generate_contb_mir_reference import BOARD,SEQ,ROOT
from generate_control_clock_reference import SEQUENTIAL as CA_CLOCK_SEQ
from sil_netlist import load_board

CB='chm/sil/'+BOARD+'.dm!1_/'+BOARD+'.wl'
CA='chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'
MC='chm/sil/MemC-Rev-Be.dm!1_/MemC-Rev-Be.wl'


def run(reference,out):
    assert not out.exists();manifest=json.loads((reference/'manifest.json').read_text());selected=manifest['boards'][0]
    b=load_board(str(ROOT/CB));a=load_board(str(ROOT/CA))
    pins={(p['pkg'],p['pin']):n for n,r in b.nets.items()for p in r['pins']}
    expected={('l01',5):"CLK.cb'",('l01',6):"CLK.cb'",('l01',7):"CLKEnable'a",('l01',11):"StartCycle'a",
              ('g13',7):"prepreclk2'",('g13',11):"ppclk2'",('g13',5):"bSC'",
              ('g13',2):"preclk0'C",('g13',3):"preclk0'B",('g13',4):"preclk0'D",
              ('c20',5):"preclk0'C",('c20',7):'StopMIRClkC',
              ('i20',5):"preclk0'D",('i20',7):'StopMIRClkBD',
              ('j05',5):"preclk0'B",('j05',7):'StopMIRClkBD',
              ('d24',4):'CBHold',('d24',2):"CBHold'",('b01',12):"BNTGtCT'a",('b01',9):'True',
              ('b01',15):'ContB13.sil+2',('c22',10):'ContB13.sil+2',('c22',13):'ContB13.sil+1',
              ('d23',5):'ContB13.sil+1',('d23',6):"CBHold'",('d23',13):'RepeatCurCc',
              ('g04',4):'True',('c04',2):'IMRHPEenable',('c04',3):'IMLHPEenable',
              ('g03',2):'StopMIRClkEn',('h03',2):'rMIRa',('k02',13):'StopMIRClk',
              ('k02',12):'StopMIRClkBD',('k02',14):'StopMIRClkC'}
    for key,n in expected.items():assert pins[key]==n,(key,n)
    assert ('g04',7)not in pins and ('g04',9)not in pins
    assert "assign p4  = ~(p9 ^ p7);"in (ROOT/'verilog/cells/cell_MC10195.v').read_text()
    assert selected['sequential']=={k:list(v)for k,v in SEQ.items()}
    seams=[]
    for n,ca_p,cb_p,drivers in [
        ("CLKEnable'a",'C16','C8',{('k01',14),('k02',15)}),
        ("StartCycle'a",'C24','C21',{('k03',14)}),
        ("BNTGtCT'a",'C173','C173',{('a02',13)}),
        ('rMIRa','C177','C177',{('j02',14)})]:
        assert a.nets[n]['backplane']=={ca_p}and b.nets[n]['backplane']=={cb_p}
        assert {(p['pkg'],p['pin'])for p in a.nets[n]['pins']if p['dir']=='out'}==drivers
        seams.append(dict(signal=n,ContA_pin=ca_p,ContB_pin=cb_p,ContA_drivers=sorted(drivers)))
    assert b.nets['CBHold']['backplane']=={'E43'} and a.nets['Hold']['backplane']=={'E26'}
    for board in ['ProcH-Rev-Ce','ProcL-Rev-Ci']:
        proc=load_board(str(ROOT/'chm/sil'/(board+'.dm!1_')/(board+'.wl')))
        assert proc.nets['PRhold']['backplane']=={'E42'}
    assert {(p['pkg'],p['pin'])for p in b.nets['rMIRa']['pins']if p['dir']=='out'}=={('h03',2)}
    memory=load_board(str(ROOT/MC));mp={(p['pkg'],p['pin']):n for n,r in memory.nets.items()for p in r['pins']}
    memory_pins={('d23',9):'Hold',('d23',12):'PRhold',('d23',13):'MXHold',('d23',14):'CBHold',
                 ('e22',3):'Hold',('e22',14):'Hold',('e23',2):'Hold',
                 ('e22',9):"PrClk1'Da",('e23',6):"PrClk1'Da",('e23',11):"clk1'Dc",
                 ('j18',12):"PrClk1'Da",('j18',9):'IgnoreProc',('j18',11):"preClk1'D",
                 ('i18',14):"clk1'Dc",('i18',11):"preClk1'D",('h12',14):"preClk1'D",
                 ('h12',10):"ppclk2'a",('h12',11):"preFH'x",('l01',3):"ppclk2'a",
                 ('l01',7):"CLK.mc'",('l01',5):"MemClkEnable'a",('l01',11):"CLKEnable'b"}
    for key,n in memory_pins.items():assert mp[key]==n,(key,n)
    assert ('d23',10)not in mp and ('d23',11)not in mp
    assert memory.packages['d23']['type']=='MC10210'
    assert memory.packages['e22']['type']==memory.packages['e23']['type']=='MC10231'
    assert {(p['pkg'],p['pin'])for p in memory.nets['Hold']['pins']if p['dir']=='out'}=={('e22',3),('e22',14),('e23',2)}
    assert memory.nets['Hold']['backplane']=={'E27'} and memory.nets['CBHold']['backplane']=={'E30'}
    assert CA_CLOCK_SEQ['k01']==(8,2)and CA_CLOCK_SEQ['k02']==(10,2)and CA_CLOCK_SEQ['k03']==(22,2)
    assert all(a.packages[p]['type']=='MC10231'for p in ['k01','k02','k03'])
    unused={**selected['extra_boundaries'],'CRamClock':9};assert len(unused)==10
    controls=[]
    with patch.object(dependencies,'BOARD',CB):
        records,cells=dependencies.audit(ROOT,manifest,boundaries=unused)
        for n in ['CBHold','dASEL.0']:
            try:dependencies.audit(ROOT,manifest,boundaries=[n])
            except AssertionError as error:
                assert any(s in str(error)for s in ['OBSERVED OUTPUT','SELECTED REGISTER']),error
                controls.append(dict(case=n+'_must_remain_live',diagnostic=str(error)))
            else:raise AssertionError('Live component input classified unused: '+n)
        m=copy.deepcopy(manifest);m['boards'][0]['flag_taps'].append('RBMux.04')
        try:dependencies.audit(ROOT,m,boundaries=unused)
        except AssertionError as error:
            assert 'OBSERVED OUTPUT'in str(error),error
            controls.append(dict(case='RBMux04_exported',diagnostic=str(error)))
        else:raise AssertionError('Exported output classified unused')
        for name,kind,before,after in [
            ('BMux04_into_RepeatCur','MC10197','assign p15 = p9 & p12;','assign p15 = p9 & (p12 | p11);'),
            ('dRA00_into_RepeatCur','MC10195','assign p13 = ~(p9 ^ p10);','assign p13 = ~(p9 ^ p10) | p12;')]:
            path=ROOT/'verilog/cells'/('cell_'+kind+'.v');old=path.read_text();assert old.count(before)==1;changed=old.replace(before,after)
            original=Path.read_text
            def read(p,*args,**kwargs):return changed if p==path else original(p,*args,**kwargs)
            with patch.object(Path,'read_text',read):
                try:dependencies.audit(ROOT,manifest,boundaries=unused)
                except AssertionError as error:
                    assert any(s in str(error)for s in ['OBSERVED OUTPUT','SELECTED REGISTER']),error
                    controls.append(dict(case=name,diagnostic=str(error)))
                else:raise AssertionError('Changed live gate dependency accepted: '+name)
    out.mkdir(parents=True)
    sources={CB,CA,MC,CB.replace('.wl','.lc'),CA.replace('.wl','.lc'),MC.replace('.wl','.lc'),*cells}
    for bn in ['ProcH-Rev-Ce','ProcL-Rev-Ci']:sources.add('chm/sil/'+bn+'.dm!1_/'+bn+'.wl')
    for m in list(sys.modules.values()):
        n=getattr(m,'__file__',None)
        if n and Path(n).resolve().parent==ROOT/'tools':sources.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes={}
    for n in sorted(sources):
        p=out/'inputs'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=hashlib.sha256(p.read_bytes()).hexdigest()
    (out/'reference-manifest.json').write_bytes((reference/'manifest.json').read_bytes())
    r=dict(status='PASS_SELECTED_CONTB_CLOCK_RESET_BOUNDARIES',scope=__doc__,unused_boundaries=records,rejected_controls=controls,
           seams=seams,physical_pin_checks=len(expected),inputs=hashes,
           upstream_clock_enable="ContA k01.Q1 inverted (Q9) OR k02.Q1 (Q11); not its internal Q6 OR !Q7",
           upstream_start_cycle="ContA k03.14 = !Q23; distinct from internal StartCycle' at k04.14 = !Q25",
           shared_reset='ContA j02.14 OR ContB h03.2; original wired-OR named backplane C177',
           distinct_memory_holds=dict(CBHold='E43',Hold='E26',PRhold='E42'),
           memory_producer=dict(board='MemC-Rev-Be',physical_pin_checks=len(memory_pins),
               Hold='e23.Q0 OR !e22.Q0 OR !e22.Q1 (three original wired-OR drivers)',
               fanout='d23.12/13/14 are separate PRhold/MXHold/CBHold outputs of Hold OR 0 OR 0',
               named_seams=dict(Hold='MemC E27 -> ContA E26',CBHold='MemC E30 -> ContB E43',PRhold='MemC E42 -> processor E42'),
               captures="e22 both halves/e23 A use PrClk1'Da; e23 B uses clk1'Dc; upstream phase/hold data not yet qualified"),
           true_source='ContB g04.4 MC10195; unwired input pins7/9 LOW, ~(0 ^ 0) = 1',
           full_producer_arrival_contract_qualified=False,physical_timing_qualified=False,native_integrated=False)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(records),len(controls))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reference.resolve(),a.output.resolve())
