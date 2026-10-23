#!/usr/bin/env python3
"""Compose original local Control-A producers with the full processor graph.

Original package/pin wiring supplies all feedback and board crossings. No
candidate next-state equation supplies a reference arrival. Physical Q map
retains the786-Q parent and appends d17 at791:786. Not native qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path

from generate_r_front_mir_reference import generate as parent_generate
from generate_conta_local_reference import COMB
from generate_processor_capture_reference import graph
from inventory_mir_clock_boundaries import ROOT


def generate(root,out):
 out.mkdir(parents=True,exist_ok=False);base=out/'parent-source-audit';parent_generate(root,base)
 manifest=json.loads((base/'manifest.json').read_text());old=manifest['boards'][0]
 spec=dict(bn=old['board'],comb=sorted(set(old['packages'])-set(old['sequential'])|set(COMB)),
  seq={**old['sequential'],'d17':(98,6)},module='r_front_local_conta',bindings={},
  clock_taps=old['clock_taps']+["clk0'Cc"],flag_taps=old['flag_taps']+['StopTasks','RWTPCorRWIM','NextMacro','jcnt'])
 bindings=dict(old['boundary_bindings'])
 bindings.update({'ContA03.sil+2':'pins[310]','ContA28.sil+2':'pins[297]'})
 external=graph(root,out=None,inspect=True,**spec)
 extras={n:311+i for i,n in enumerate(sorted(set(external)-set(bindings)))}
 assert len(extras)==4,extras
 spec['bindings']={n:bindings[n]if n in bindings else f'pins[{extras[n]}]'for n in external}
 indexed=[v for v in spec['bindings'].values()if v.startswith('pins[')]
 assert len(indexed)==len(set(indexed))
 record=graph(root,out=out,**spec)
 p=out/'r_front_local_conta.sv';raw=p.read_text()
 raw=raw.replace('input wire [27:0] pins','input wire [314:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
 for n in ['a09','b09','c09','d09']:raw=raw.replace('original_F10000 '+n+'(','original_F10000_ready '+n+'(')
 p.write_text(raw)
 for n in ['proch','procl']:(out/f'r_front_ready_{n}.sv').write_bytes((base/f'r_front_ready_{n}.sv').read_bytes())
 raw=(base/'r_front_mir_reference.sv').read_text()
 edits=[('r_front_mir_reference','r_front_local_reference',1),('[785:0]','[791:0]',2),
  ('input wire [309:0] pins','input wire [314:0] pins',1),('[66:0] clocks','[67:0] clocks',1),
  ('wire [22:0] cc','wire [23:0] cc',1),('wire [58:0] fc','wire [62:0] fc',1),
  ('r_front_mir_conta c(','r_front_local_conta c(',1),
  ('{fixture_state[785:764],','{fixture_state[791:786],fixture_state[785:764],',1),
  ('{state_q[785:764],','{state_q[791:786],state_q[785:764],',1),
  ('assign clocks={cc[22:15]','assign clocks={cc[23],cc[22:15]',1),
  ('cpu_pins=pins[245:0];','cpu_pins=pins[245:0];cpu_pins[65]=fc[59];cpu_pins[1]=fc[60];cpu_pins[212]=fc[61];cpu_pins[235]=fc[62];',1)]
 for a,b,n in edits:assert raw.count(a)==n,(a,raw.count(a));raw=raw.replace(a,b)
 (out/'r_front_local_reference.sv').write_text(raw)
 manifest['boards'][0]=record
 manifest.update(physical_bits=792,pin_count=315,clock_channels=68,
  packages=sum(len(b['packages'])for b in manifest['boards']),
  original_pins=sum(b['original_pins']for b in manifest['boards']),
  extra_boundaries=extras,local_producers_internal=['StopTasks','RWTPCorRWIM'],
  scope=__doc__,generator_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
 assert sum(b['physical_bits']for b in manifest['boards'])==792
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');return manifest


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
 a=p.parse_args();r=generate(a.root.resolve(),a.output.resolve());print('R_FRONT_LOCAL_REFERENCE',r['physical_bits'],r['pin_count'],r['extra_boundaries'])
