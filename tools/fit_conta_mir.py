#!/usr/bin/env python3
"""Measure coupled Control-A clock/task/MIR/CP/Ready logic in a15ns shell.

Retain all physical state, events and invariants. The shell adds only fixture
staging. Native arrivals, remaining producers and60ns operation stay open;
no assembly or deployment. Run Quartus only on misterubuntu.
"""
import argparse,hashlib,json
from pathlib import Path
import fit_control_clock as fixture
ROOT=Path(__file__).resolve().parents[1]
TOP='''module clock_fit(input wire host_clk,input wire [355:0] pins,
 output reg [185:0] result);
 reg [355:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [97:0] q,next_q;
 wire before_clock,after_clock;
 wire [127:0] before_controls,after_controls;
 wire [21:0] events,repeated;
 wire unsettled;
 wire [38:0] flags;
 wire [3:0] next_task;
 assign {q,before_clock,after_clock,before_controls,after_controls}=launch;
 dorado_conta_mir_step evaluator(q,before_clock,after_clock,before_controls,after_controls,
   next_q,events,repeated,unsettled,flags,next_task);
 always @(posedge host_clk) result<={next_q,events,repeated,unsettled,flags,next_task};
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(qualified,out):
 r=json.loads((qualified/'results.json').read_text())
 assert r['status']=='PASS_CONTA_MIR_COUPLING'and len(r['profiles'])==3 and len(r['rejected_mutations'])==7 and len(r['actions'])==40
 assert not r['native_or_timing_qualified']
 for n,h in r['inputs'].items():assert sha(qualified/'inputs'/n)==h,n
 out.mkdir(parents=True,exist_ok=False)
 qsf=fixture.QSF.replace('dorado_control_clock_step.sv','dorado_conta_mir_step.sv')
 for n,raw in {'top.sv':TOP,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():(out/n).write_text(raw)
 p=qualified/'inputs/verilog/rtl/dorado_conta_mir_step.sv';assert sha(p)==sha(ROOT/'verilog/rtl/dorado_conta_mir_step.sv')
 (out/p.name).write_bytes(p.read_bytes())
 for n in ['fit_control_clock.py','fit_conta_mir.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
 record=dict(scope=__doc__,component='conta_mir',clock_period_ns=15,input_staging_bits=356,launch_bits=356,result_bits=186,
  files={p.name:sha(p)for p in out.iterdir()if p.is_file()},assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('PREPARED_CONTA_MIR_15NS_COMPONENT',flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);g=p.add_mutually_exclusive_group(required=True);g.add_argument('--prepare',type=Path);g.add_argument('--run',type=Path)
 p.add_argument('--qualified',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare:assert a.qualified;prepare(a.qualified.resolve(),a.prepare.resolve())
 else:fixture.run(a.run.resolve(),a.quartus.resolve())
