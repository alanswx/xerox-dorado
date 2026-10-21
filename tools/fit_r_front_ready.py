#!/usr/bin/env python3
"""Fit the fully qualified Ready/R-front/R-memory component in the15ns shell.

Preserve functional RAM ports, all752 Qs,59 events/repeats and every invariant.
Only source_pins/fa_n remain unregistered test observations, as in the prior
functional fixture. No assembly, deployment or native speed claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import fit_control_clock as fixture
from fit_r_front import RTL as CHILD_RTL,quartus_cone

ROOT=Path(__file__).resolve().parents[1]
RTL=['dorado_r_front_ready_step.sv','dorado_task_ready_step.sv',*CHILD_RTL]
FIELDS=[('next_q',752),('events',59),('repeated',59),('unsettled',1),
 ('write_n',2),('latch_n',2),('t_latch_n',2),('bypass',2),('parity_ok',4),('faults',4),
 ('r_read_n',2),('r_visible_n',2),('r_flags',4),('rm_flags_n',2),('address',32),('select_n',4),
 ('forwarded_bypass',2),('r_write_n',2),('port_address',32),('port_select_n',4),
 ('file_read_data',16),('file_read_parity',2),('source_pins',202),('fa_n',8),('ready_flags',30)]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(qualified,out):
 gate=json.loads((qualified/'results.json').read_text());assert gate['status']=='PASS_R_FRONT_READY_ARRIVAL_CONTRACT'
 assert gate['physical_bits']==752 and gate['ram_bits']==9216 and len(gate['profiles'])==6
 assert len(gate['profiles'][0]['cases'])==10 and all(r['cases'][0]['returncode']==0 for r in gate['profiles'])
 assert all(r['returncode']!=0 for r in gate['profiles'][0]['cases'][1:])
 for n,h in gate['inputs'].items():
  assert sha(qualified/'executed-inputs'/n)==h,n
  if (ROOT/n).exists():assert sha(ROOT/n)==h,('changed source',n)
 original=(qualified/'dorado_r_front_cone.sv').read_text();synthesis=quartus_cone(original)
 stored=[n for n,w in FIELDS if n not in ['source_pins','fa_n']]
 assert sum(w for n,w in FIELDS if n in stored)==1021
 top='''module clock_fit(input wire host_clk,input wire [1246:0] pins,output reg [1020:0] result);
 reg [1246:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [751:0] q;
 wire [245:0] before_controls,after_controls;
 wire before_clock,after_clock,advance;
 assign {q,before_clock,after_clock,before_controls,after_controls,advance}=launch;
'''
 top+=''.join(' wire '+(''if w==1 else f'[{w-1}:0] ')+n+';\n'for n,w in FIELDS)
 top+=' dorado_r_front_ready_step evaluator(host_clk,advance,q,before_clock,after_clock,before_controls,after_controls,\n  '+','.join(n for n,w in FIELDS)+');\n'
 top+=' always @(posedge host_clk) result<={'+','.join(stored)+'};\nendmodule\n'
 qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
  ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in [*RTL,'dorado_r_front_cone.sv']))
 out.mkdir(parents=True,exist_ok=False)
 for n,raw in {'top.sv':top,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():(out/n).write_text(raw)
 for n in RTL:(out/n).write_bytes((qualified/'executed-inputs/verilog/rtl'/n).read_bytes())
 (out/'dorado_r_front_cone.sv').write_text(synthesis);(out/'qualified-cone.sv.txt').write_text(original)
 for n in ['fit_control_clock.py','fit_r_front.py','fit_r_front_ready.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
 record=dict(scope=__doc__,component='r_front_ready',clock_period_ns=15,ram_bits=9216,
             input_staging_bits=1247,launch_bits=1247,result_bits=1021,
             observation_outputs_registered=False,observation_only_outputs=['source_pins','fa_n'],
             files={p.name:sha(p)for p in out.iterdir()if p.is_file()},assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
 print('Prepared752-Q/9216-RAM component:1247 launches/1021 results,15ns;no assembly/deploy')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);a=p.add_mutually_exclusive_group(required=True)
 a.add_argument('--prepare',type=Path);a.add_argument('--run',type=Path);p.add_argument('--qualified',type=Path)
 p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));args=p.parse_args()
 if args.prepare:
  assert args.qualified;prepare(args.qualified.resolve(),args.prepare.resolve())
 else:fixture.run(args.run.resolve(),args.quartus.resolve())
