#!/usr/bin/env python3
"""Measure the qualified task/Ready/processor component in the 15ns shell.

No assembly/deployment; native arrivals and whole-machine timing remain open.
Run Quartus only on misterubuntu.
"""
import argparse
import hashlib
import json
from pathlib import Path
import fit_control_clock as fixture

ROOT=Path(__file__).resolve().parents[1]
RTL=['dorado_task_ready_capture_step.sv','dorado_task_ready_step.sv','dorado_task_capture_step.sv']
TOP='''module clock_fit(input wire host_clk,input wire [277:0] pins,
 output reg [188:0] result);
 reg [277:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [103:0] q,next_q;
 wire before_clock,after_clock;
 wire [85:0] before_controls,after_controls;
 wire [26:0] events,repeated;
 wire unsettled;wire [29:0] ready_flags;
 assign {q,before_clock,after_clock,before_controls,after_controls}=launch;
 dorado_task_ready_capture_step evaluator(q,before_clock,after_clock,
   before_controls,after_controls,next_q,events,repeated,unsettled,ready_flags);
 always @(posedge host_clk) result<={next_q,events,repeated,unsettled,ready_flags};
endmodule
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(qualified,out):
 record=json.loads((qualified/'results.json').read_text());assert record['status']=='PASS_TASK_READY_CAPTURE'
 assert len(record['profiles'])==3 and len(record['rejected_mutations'])==5
 for n,h in record['inputs'].items():assert sha(qualified/'executed-inputs'/n)==h,n
 out.mkdir(parents=True,exist_ok=False)
 qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
                         ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in RTL))
 for n,raw in {'top.sv':TOP,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
                'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():
  (out/n).write_text(raw)
 for n in RTL:
  p=qualified/'executed-inputs/verilog/rtl'/n;assert sha(p)==sha(ROOT/'verilog/rtl'/n),n
  (out/n).write_bytes(p.read_bytes())
 for n in ['fit_control_clock.py','fit_task_ready_capture.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
 provenance=dict(scope=__doc__,component='task_ready_capture',clock_period_ns=15,
                 input_staging_bits=278,launch_bits=278,result_bits=189,
                 files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
                 assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 print('Prepared104-Q task/Ready component;278 launches/189 results;15ns;no assembly/deploy')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);a=p.add_mutually_exclusive_group(required=True)
 a.add_argument('--prepare',type=Path);a.add_argument('--run',type=Path)
 p.add_argument('--qualified',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
 args=p.parse_args()
 if args.prepare:
  assert args.qualified;prepare(args.qualified.resolve(),args.prepare.resolve())
 else:fixture.run(args.run.resolve(),args.quartus.resolve())
