#!/usr/bin/env python3
"""Compose original Control-A MIR/CP producers with all original R-front gates.

Reference only. Actual register outputs cross to processor inputs; no candidate
next-state equations supply these arrivals. ASEL.0'a retains its own boundary
rather than assuming it equals the separate ASEL.0' fanout.
"""
import argparse,hashlib,json,re
from pathlib import Path
from generate_r_front_ready_reference import generate as ready_generate
from generate_r_front_reference import specs as front_specs
from generate_task_ready_capture_reference import READY_COMB
from generate_processor_capture_reference import graph
from generate_mir_ready_reference import COMB as MIR_COMB
from inventory_mir_clock_boundaries import ROOT,PACKAGES
from test_mir_capture import BINDINGS as MIR_BINDINGS


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);base=out/'ready-source-audit';ready_generate(root,base)
 plans,_,_=front_specs(root);s=plans[0];s={k:v for k,v in s.items()if not k.startswith('_')}
 s['module']='r_front_mir_conta';s['comb']=sorted(set(s['comb']+READY_COMB+MIR_COMB))
 s['seq'].update({**{p:(48+4*i,4)for i,p in enumerate(['a09','b09','c09','d09'])},
  'j02':(64,6),'i02':(70,6),**{p:(76+2*i,2)for i,p in enumerate(PACKAGES)}})
 old=json.loads((base/'manifest.json').read_text())['boards'][0]
 s['clock_taps']=old['clock_taps']+[*[f"h*clk0'C{x}"for x in 'abcdef'],"CPStrb0'","CPStrb1'"]
 s['flag_taps']=old['flag_taps']+[*[f'FF.{i}'for i in range(8)],"FFok'a","PrBlock'",'NoDispatch','rStop','StopAtT1','rCT',"Block'"]
 bindings={n:v for n,v in s['bindings'].items()if v not in ("1'b0","1'b1")}
 bindings.update({n:re.sub(r'pins\[(\d+)\]',lambda m:f'pins[{246+int(m[1])}]',v)for n,v in MIR_BINDINGS.items()})
 for n in ['brMIR'+x for x in 'abc']+[f"h*clk0'C{x}"for x in 'abcdef']:bindings.pop(n)
 bindings.update({**{f'CPOut.{i}':f'pins[{281+i}]'for i in range(1,8)},
  **{f"CPAddr.{i}'":f'pins[{291-i}]'for i in range(3)},"CPStrb'":'pins[292]',
  "ASEL.0'a":'pins[294]',"BSEL.0'":'pins[205]','StopMIRClk':'pins[293]',
  'ContA18.sil+3':'pins[243]','ContA18.sil+4':'pins[244]',"dBlock'":'pins[263]'})
 external=graph(root,out=None,inspect=True,**{**s,'bindings':{}})
 extras={n:295+i for i,n in enumerate(sorted(set(external)-set(bindings)))}
 assert len(extras)==15,extras
 s['bindings']={n:bindings[n]if n in bindings else f'pins[{extras[n]}]'for n in external}
 record=graph(root,out=out,**s);p=out/'r_front_mir_conta.sv';raw=p.read_text()
 raw=raw.replace('input wire [27:0] pins','input wire [309:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
 for name in ['a09','b09','c09','d09']:raw=raw.replace('original_F10000 '+name+'(','original_F10000_ready '+name+'(')
 p.write_text(raw)
 for name in ['proch','procl']:(out/f'r_front_ready_{name}.sv').write_bytes((base/f'r_front_ready_{name}.sv').read_bytes())
 raw=(base/'r_front_ready_reference.sv').read_text()
 edits=[('module r_front_ready_reference','module r_front_mir_reference',1),('[751:0]','[785:0]',2),
  ('input wire [245:0] pins','input wire [309:0] pins',1),('[58:0] clocks','[66:0] clocks',1),
  ('wire [14:0] cc','wire [22:0] cc',1),('wire [43:0] fc','wire [58:0] fc',1),
  ('r_front_ready_conta c(','r_front_mir_conta c(',1),
  ('{fixture_state[751:736],fixture_state[79:64],fixture_state[31:0]}','{fixture_state[785:764],fixture_state[763:752],fixture_state[751:736],fixture_state[79:64],fixture_state[31:0]}',1),
  ('{state_q[751:736],state_q[79:64],state_q[31:0]}','{state_q[785:764],state_q[763:752],state_q[751:736],state_q[79:64],state_q[31:0]}',1),
  ('assign clocks={cc[14:13]','assign clocks={cc[22:15],cc[14:13]',1),
  ('assign ready_flags=fc[43:14]','assign ready_flags=fc[43:14]',1)]
 for a,b,n in edits:assert raw.count(a)==n,(a,raw.count(a));raw=raw.replace(a,b)
 # Only processor instances/source observations use the actual producer overlay.
 raw=raw.replace('en_h,pins,next_raw','en_h,cpu_pins,next_raw').replace('en_l,pins,next_raw','en_l,cpu_pins,next_raw')
 raw=re.sub(r'(assign source_pins\[\d+\]=)pins\[',r'\1cpu_pins[',raw)
 extra=''' reg [245:0] cpu_pins;
 always @* begin
  cpu_pins=pins[245:0];
  cpu_pins[0]=fc[54];cpu_pins[2]=fc[56];cpu_pins[7]=fc[55];cpu_pins[70]=fc[57];
  cpu_pins[66]=pins[263];cpu_pins[67]=fc[58];cpu_pins[176]=fc[53];cpu_pins[209]=fc[52];
'''
 for i in range(8):extra+=f'  cpu_pins[{19-i}]=fc[{44+i}];\n'
 extra+=' end\n'
 marker=' wire en_h=fc[3]';assert raw.count(marker)==1
 raw=raw.replace(marker,extra+marker,1)
 (out/'r_front_mir_reference.sv').write_text(raw)
 manifest=json.loads((base/'manifest.json').read_text());manifest['boards'][0]=record
 manifest.update(physical_bits=786,pin_count=310,clock_channels=67,extra_boundaries=extras,native_integrated=False,
  scope=__doc__,original_pins=sum(b['original_pins']for b in manifest['boards']),
  packages=sum(len(b['packages'])for b in manifest['boards']),
  generator_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
 assert sum(b['physical_bits']for b in manifest['boards'])==786
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');return manifest


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 r=generate(a.root.resolve(),a.output.resolve());print('R_FRONT_MIR_REFERENCE',r['physical_bits'],r['pin_count'],r['extra_boundaries'])
