#!/usr/bin/env python3
"""Measure the bounded R-front/MIR composition in the existing 15 ns shell.

Every physical Q, event, functional memory port and invariant stays registered.
Only source_pins/fa_n are test observations outside the registered budget, as
in the qualified parent. This is an implementation-budget experiment, not
complete producer-arrival, native timing, boot or speed qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import fit_control_clock as fixture
from fit_r_front import quartus_cone
from fit_r_front_ready import RTL as PARENT_RTL, FIELDS as PARENT_FIELDS
from review_r_front_mir import checked, review

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
RTL=['dorado_r_front_mir_step.sv','dorado_conta_mir_step.sv',*PARENT_RTL]
FIELDS=[(n,{'next_q':786,'events':67,'repeated':67}.get(n,w))for n,w in PARENT_FIELDS]+[
 ('conta_next',98),('shared_state_mismatch',1)]
ARRIVALS=[f'r-front-mir-arrival-{profile}-{engine}-v{version}-20261023'
 for profile,version in [('parc_parity_min',27),('parc_parity_max',28),('later_r_memory',29)]
 for engine in ['icarus','verilator']]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def qualify(out):
 assert not out.exists();out.mkdir(parents=True)
 old=review(out/'ordering-review.json',
  ['r-front-mir-front-directed-v23-20261023','r-front-mir-front-directed-icarus-v24-20261023'],
  ['r-front-mir-low-cp-v25-20261023','r-front-mir-low-cp-icarus-v26-20261023'])
 rows=[];baseline=None;clocks=None;last=None
 for name in ARRIVALS:
  d,s,log=checked(name)
  assert s['status']=='PASS_COMPOSITION_PROBE'and s['post_mir_pins']==2,name
  assert all(s[k]for k in ['random_external_producers','CP_bus_sweep','directed_RBase_StkP','LOW_root_CP']),name
  assert all(a['returncode']==0 for a in s['actions'])and len(s['actions'])==2,name
  assert sha(d/'candidate.sv')==old['candidate_sha256'],name
  assert s['summary']=='checks=11507 writes=182 reads=4406 epochs=584',name
  summary=re.search(r'^R_FRONT_MIR_PROBE PASS ([^\n]+)$',log,re.M)[1]
  clock_rows=re.findall(r'^R_FRONT_MIR_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
  assert len(clock_rows)==67 and all(int(n)>0 for ch,n in clock_rows),name
  assert 'R_FRONT_MIR_CHANGED front=d33ffffffffffffffffffffff Ready=ffff'in log,name
  assert 'R_FRONT_MIR_DIRECTED RBase=32 StkP=256'in log,name
  if baseline is None:baseline=summary;clocks=clock_rows
  assert summary==baseline and clock_rows==clocks,name
  if s['delay_profile']['parc_parity_control']:
   assert '-DPARC_PARITY_TIMING'in s['actions'][0]['command'],name
  for n,h in s['inputs'].items():
   if (ROOT/n).exists():assert sha(ROOT/n)==h,('changed input',name,n)
  rows.append(dict(run=name,profile=s['profile'],engine=s['engine'],summary=summary,
                   delay_profile=s['delay_profile'],run_state_sha256=sha(d/'run-state.json')))
  last=(d,s)
 assert len({(r['profile'],r['engine'])for r in rows})==6
 d,s=last;inputs=out/'executed-inputs'
 for n,h in s['inputs'].items():
  p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((d/'inputs'/n).read_bytes());assert sha(p)==h
 parent=BUILD/'r-front-ready-qualified-v1-20261020'
 for n in ['dorado_r_front_cone.sv','dorado_r_front_cone.json']:(out/n).write_bytes((parent/n).read_bytes())
 record=dict(status='PASS_BOUNDED_R_FRONT_MIR_BUDGET_GATE',scope=__doc__,inputs=s['inputs'],
  physical_bits=786,ram_bits=9216,total_physical_bits=10002,arrivals=rows,
  bounded_original_graph_checks=True,all_R_memory_bits_compared=True,
  r_read_write_nonoverlap_checked=True,post_commit_source_epoch_checked=True,
  ram_arrays_flattened=False,full_producer_arrival_contract_qualified=False,
  native_integrated=False,physical_timing_qualified=False,speedup_qualified=False,
  ordering_review_sha256=sha(out/'ordering-review.json'))
 (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
 print(record['status'],flush=True)


def top_source():
 stored=[n for n,w in FIELDS if n not in ['source_pins','fa_n']]
 assert sum(w for n,w in FIELDS if n in stored)==1170
 top='''module clock_fit(input wire host_clk,input wire [1408:0] pins,output reg [1169:0] result);
 reg [1408:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [785:0] q;
 wire [309:0] before_controls,after_controls;
 wire before_clock,after_clock,advance;
 assign {q,before_clock,after_clock,before_controls,after_controls,advance}=launch;
'''
 top+=''.join(' wire '+(''if w==1 else f'[{w-1}:0] ')+n+';\n'for n,w in FIELDS)
 top+=' dorado_r_front_mir_step evaluator(host_clk,advance,q,before_clock,after_clock,before_controls,after_controls,\n  '+','.join(n for n,w in FIELDS)+');\n'
 top+=' always @(posedge host_clk) result<={'+','.join(stored)+'};\nendmodule\n'
 return top


def prepare(qualified,out):
 gate=json.loads((qualified/'results.json').read_text())
 assert gate['status']=='PASS_BOUNDED_R_FRONT_MIR_BUDGET_GATE'and len(gate['arrivals'])==6
 assert not gate['full_producer_arrival_contract_qualified']and not gate['native_integrated']
 for n,h in gate['inputs'].items():assert sha(qualified/'executed-inputs'/n)==h,n
 original=(qualified/'dorado_r_front_cone.sv').read_text();synthesis=quartus_cone(original)
 out.mkdir(parents=True,exist_ok=False)
 qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
  ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in [*RTL,'dorado_r_front_cone.sv']))
 for n,raw in {'top.sv':top_source(),'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
  'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():(out/n).write_text(raw)
 for n in RTL:
  p=qualified/'executed-inputs/verilog/rtl'/n
  assert sha(p)==sha(ROOT/'verilog/rtl'/n),n
  (out/n).write_bytes(p.read_bytes())
 (out/'dorado_r_front_cone.sv').write_text(synthesis);(out/'qualified-cone.sv.txt').write_text(original)
 for n in ['fit_control_clock.py','fit_r_front.py','fit_r_front_ready.py','fit_r_front_mir.py','review_r_front_mir.py']:
  (out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
 record=dict(scope=__doc__,component='r_front_mir',clock_period_ns=15,ram_bits=9216,
  input_staging_bits=1409,launch_bits=1409,result_bits=1170,
  observation_outputs_registered=False,observation_only_outputs=['source_pins','fa_n'],
  files={p.name:sha(p)for p in out.iterdir()if p.is_file()},assembly_permitted=False,native_qualified=False,
  full_producer_arrival_contract_qualified=False)
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
 print('PREPARED_BOUNDED_R_FRONT_MIR_15NS_BUDGET',sha(out/'top.sv'),flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);g=p.add_mutually_exclusive_group(required=True)
 g.add_argument('--qualify',type=Path);g.add_argument('--prepare',type=Path);g.add_argument('--run',type=Path)
 p.add_argument('--qualified',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
 a=p.parse_args()
 if a.qualify:qualify(a.qualify.resolve())
 elif a.prepare:assert a.qualified;prepare(a.qualified.resolve(),a.prepare.resolve())
 else:fixture.run(a.run.resolve(),a.quartus.resolve())
