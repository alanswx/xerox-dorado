#!/usr/bin/env python3
"""Bind joint control crossings and inherited functions to original sources.

Trace the distinct registered enables, MemSH path, UseDMD gate and shared
reset. Record the RunRefresh release/clock ordering limit without declaring
the failed arrival profile valid, irrelevant, or physically characterized.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from generate_control_hold_candidate import PARENTS,ROOT
from sil_netlist import load_board


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def functions(raw):
    found={}
    for f in re.findall(r' function automatic.*?endfunction',raw,re.S):
        name=re.match(r' function automatic\s+(?:\[[^]]+\]\s+)?(\w+)\(',f)[1]
        assert name not in found,name;found[name]=f
    return found


def run(reference,candidate,out):
    assert not out.exists();m=json.loads((reference/'manifest.json').read_text());assert m['physical_bits']==156
    boards={name:load_board(str(ROOT/'chm/sil'/(name+'.dm!1_')/(name+'.wl')))for name in ['ContA-Rev-Cd','ContB-Rev-Cd','MemC-Rev-Be']}
    a=boards['ContA-Rev-Cd'];b=boards['ContB-Rev-Cd'];c=boards['MemC-Rev-Be']
    def pins(board):return {(p['pkg'],p['pin']):n for n,r in board.nets.items()for p in r['pins']}
    ap,bp,mp=pins(a),pins(b),pins(c)
    checks={('k01',14):"CLKEnable'a",('k02',15):"CLKEnable'a",('k01',3):"CLKEnable'b",('k02',2):"CLKEnable'b",
            ('l03',14):"MemClkEnable'a",('l04',15):"MemClkEnable'a",('k03',3):'MemSH',('k03',14):"StartCycle'a",
            ('k04',14):"StartCycle'",('a02',13):"BNTGtCT'a",('a02',11):"SwitchUp'",('f15',15):"SwitchUp'",
            ('i04',15):'UseDMD',('i04',12):'ContA28.sil+7',('i04',13):'SetRun',('l17',2):'ContA28.sil+7',
            ('j02',14):'rMIRa',('k03',6):"prepreclk'd",('l01',3):"prepreclk'd",('l01',7):"CLK.ca'",
            ('l06',9):"prepreclk'd",('l06',15):'RunRefresh',('l06',10):'WantRunRfsh',('f13',11):"prepreclk'a",
            ('l01',13):"prepreclk'a",('f13',14):"preRunClk'Bc",('l03',9):"preRunClk'Bc",('l03',5):'RunRefresh',('l03',12):'RunRefresh'}
    for key,n in checks.items():assert ap[key]==n,(key,n)
    for key in [('a02',9),('a02',10),('l01',5),('l01',6)]:assert key not in ap,key
    for key,n in {('b23',14):"ASEL.0'a",('g24',14):"BSEL.0'",('h03',2):'rMIRa'}.items():assert bp[key]==n
    assert mp[('l02',6)]=="CLK.mc'"and mp[('l02',7)]=='MemSH'
    assert a.packages['i04']['type']=='MC10104'and a.packages['l06']['type']=='SE10231'and a.packages['f13']['type']=='SE10210'
    assert 'assign p15 = (p12 & p13);'in (ROOT/'verilog/cells/cell_MC10104.v').read_text()
    for n in ['rMIRa',"ASEL.0'a","BSEL.0'",'StopMIRClk']:
        assert a.nets[n]['backplane']and b.nets[n]['backplane']
    assert a.nets['rMIRa']['backplane']==b.nets['rMIRa']['backplane']=={'C177'}
    ca=m['boards'][0]['sequential'];assert ca['k01']==[8,2]and ca['k02']==[10,2]and ca['l03']==[14,2]and ca['l04']==[16,2]
    assert ca['k03']==[22,2]and ca['j02']==[64,6]and ca['f15']==[42,2]and ca['l17']==[104,6]
    current=functions(candidate.read_text());reused=[]
    for path,h in PARENTS.items():
        p=ROOT/path;assert sha(p)==h;raw=p.read_text()
        if 'r_front' in path:raw=raw[raw.index('module dorado_conta_l17_step'):]
        for name,f in functions(raw).items():
            target='cb_'+name if 'mir_step' in path else name
            expected=f
            if 'mir_step' in path:
                for word in ['stop_mir','levels_at','transfer']:expected=re.sub(r'\b'+word+r'\b','cb_'+word,expected)
            assert current[target]==expected,(path,name)
            reused.append(dict(parent=path,function=name,candidate_function=target,sha256=hashlib.sha256(expected.encode()).hexdigest()))
    out.mkdir(parents=True);sources={*PARENTS,'tools/audit_control_hold_wiring.py','tools/generate_control_hold_candidate.py','tools/generate_control_hold_reference.py','verilog/cells/cell_MC10104.v'}
    for bn in boards:
        for ext in ['wl','lc']:sources.add('chm/sil/'+bn+'.dm!1_/'+bn+'.'+ext)
    hashes={}
    for n in sorted(sources):
        p=out/'inputs'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=sha(p)
    r=dict(status='PASS_JOINT_CONTROL_SOURCE_WIRING',candidate_sha256=sha(candidate),scope=__doc__,inputs=hashes,
           CA_pin_checks=len(checks),CB_pin_checks=3,MemC_pin_checks=2,inherited_functions=reused,
           derived_crossings=dict(CLKEnable_a='!CA.Q9 | CA.Q11',CLKEnable_b='!CA.Q8 | CA.Q10',MemClkEnable_a='!CA.Q15 | CA.Q17',
               MemSH='!CA.Q22',StartCycle_a='!CA.Q23; keep internal !Q25 distinct',BNTGtCT_a='CA.Q43',UseDMD='CA.l17.Q0 AND SetRun',
               rMIRa='CA.j02.Q4 OR CB.h03.Q0',ASEL0_a='!CB.b23.Q1',BSEL0='!CB.g24.Q1'),
           MemSH_epoch='MemC l02 A clocks directly from CLK.mc; new CA MemSH passes the additional l01 fanout and k03 clock-to-Q. Analog skew remains outside this proof.',
           refresh_limit=dict(set_source='CA l06.B RunRefresh',set_sinks='CA l03 A/B asynchronous Set',
               source_clock='CAl01 -> l06 common clock',sink_clock='CAl01 -> f13 -> l03 common clock',
               illustrative_release_arrival='common fanout + phase FF delay',illustrative_clock_arrival='common fanout + f13 gate delay',
               caveat='A phase FF earlier than f13 can release Set before capture. The failed matrix remains failed; minimum pulse/recovery and physical skew are unqualified.'),
           full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],len(reused),'inherited functions')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reference',type=Path,required=True);p.add_argument('--candidate',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reference.resolve(),a.candidate.resolve(),a.output.resolve())
