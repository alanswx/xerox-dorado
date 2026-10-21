#!/usr/bin/env python3
"""Fit the qualified coupled CP/Ready producers in the isolated 15ns shell.

No assembly or deployment. This checks a component implementation budget;
phase/MIR/task/Hold arrivals and native Dorado timing remain unqualified.
Run Quartus only on misterubuntu.
"""
import argparse
import hashlib
import json
from pathlib import Path

import fit_control_clock as fixture

ROOT=Path(__file__).resolve().parents[1]
RTL=['dorado_task_producers_step.sv','dorado_cp_control_step.sv','dorado_task_ready_step.sv']
TOP='''module clock_fit(input wire host_clk,input wire [97:0] pins,
 output reg [65:0] result);
 reg [97:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [27:0] q,next_q;
 wire [34:0] before_controls,after_controls;
 wire [3:0] events,clocks;
 wire [14:0] is_next_n,ready;
 assign {q,before_controls,after_controls}=launch;
 dorado_task_producers_step evaluator(q,before_controls,after_controls,
   next_q,events,clocks,is_next_n,ready);
 always @(posedge host_clk) result<={next_q,events,clocks,is_next_n,ready};
endmodule
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(qualified,out):
 record=json.loads((qualified/'results.json').read_text())
 assert record['status']=='PASS_COUPLED_TASK_PRODUCERS'
 assert len(record['profiles'])==3 and len(record['rejected_mutations'])==3
 assert record['ClrReady_internally_connected'] and not record['native_integrated']
 for n,h in record['inputs'].items():assert sha(qualified/'executed-inputs'/n)==h,n
 out.mkdir(parents=True,exist_ok=False)
 qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
                         ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in RTL))
 for n,raw in {'top.sv':TOP,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
                'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():
  (out/n).write_text(raw)
 for n in RTL:
  path=qualified/'executed-inputs/verilog/rtl'/n
  assert sha(ROOT/'verilog/rtl'/n)==sha(path),n
  (out/n).write_bytes(path.read_bytes())
 for n in ['fit_control_clock.py','fit_task_producers.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
 provenance=dict(scope=__doc__,component='task_producers',clock_period_ns=15,
                 input_staging_bits=98,launch_bits=98,result_bits=66,
                 files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
                 assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 print('Prepared CP/Ready 28Q component;98 launches/66 results;15ns;no assembly/deploy')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);a=p.add_mutually_exclusive_group(required=True)
 a.add_argument('--prepare',type=Path);a.add_argument('--run',type=Path)
 p.add_argument('--qualified',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
 args=p.parse_args()
 if args.prepare:
  assert args.qualified;prepare(args.qualified.resolve(),args.prepare.resolve())
 else:fixture.run(args.run.resolve(),args.quartus.resolve())
