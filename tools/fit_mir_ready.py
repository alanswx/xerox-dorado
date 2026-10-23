#!/usr/bin/env python3
"""Fit the qualified MIR/CP/Ready component in the existing isolated15ns shell.

All inputs and functional outputs are retained. Input staging belongs to the
measurement shell, not an added Dorado pipeline. No assembly or deployment.
Run Quartus only on misterubuntu; upstream arrivals/native timing remain open.
"""
import argparse,hashlib,json
from pathlib import Path
import fit_control_clock as fixture
ROOT=Path(__file__).resolve().parents[1]
RTL=['dorado_mir_ready_step.sv','dorado_mir_capture_step.sv','dorado_cp_control_step.sv','dorado_task_ready_step.sv']
TOP='''module clock_fit(input wire host_clk,input wire [231:0] pins,
 output reg [108:0] result);
 reg [231:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [49:0] q,next_q;
 wire [90:0] before_controls,after_controls;
 wire [9:0] events,clocks;
 wire [38:0] flags;
 assign {q,before_controls,after_controls}=launch;
 dorado_mir_ready_step evaluator(q,before_controls,after_controls,
   next_q,events,clocks,flags);
 always @(posedge host_clk) result<={next_q,events,clocks,flags};
endmodule
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(qualified,out):
 r=json.loads((qualified/'results.json').read_text())
 assert r['status']=='PASS_MIR_CP_READY_COUPLED'and len(r['profiles'])==3 and len(r['rejected_mutations'])==6 and len(r['actions'])==36
 assert not r['native_or_timing_or_arrivals_qualified']
 for name,digest in r['inputs'].items():assert sha(qualified/'inputs'/name)==digest,name
 out.mkdir(parents=True,exist_ok=False)
 qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
  ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in RTL))
 for n,raw in {'top.sv':TOP,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():(out/n).write_text(raw)
 for n in RTL:
  p=qualified/'inputs/verilog/rtl'/n;assert sha(ROOT/'verilog/rtl'/n)==sha(p),n
  (out/n).write_bytes(p.read_bytes())
 for n in ['fit_control_clock.py','fit_mir_ready.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 (out/'functional-gate.json').write_bytes((qualified/'results.json').read_bytes())
 record=dict(scope=__doc__,component='mir_ready',clock_period_ns=15,input_staging_bits=232,launch_bits=232,result_bits=109,
  files={p.name:sha(p)for p in out.iterdir()if p.is_file()},assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('PREPARED_MIR_CP_READY_15NS_COMPONENT',flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);g=p.add_mutually_exclusive_group(required=True);g.add_argument('--prepare',type=Path);g.add_argument('--run',type=Path)
 p.add_argument('--qualified',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare:assert a.qualified;prepare(a.qualified.resolve(),a.prepare.resolve())
 else:fixture.run(a.run.resolve(),a.quartus.resolve())
