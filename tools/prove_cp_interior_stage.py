#!/usr/bin/env python3
"""Prove the one-port stage's acknowledgement, snapshot and reset contracts.

Unconstrained binary traffic/data, no protocol assumptions, arbitrary initial
buffer contents. This does not prove caller descriptor freezing, original gate
arrival margins, processor capture fidelity or native machine publication.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
TOP='''module contract(input wire clk,reset,start,need,accept,finish,
 input wire [31:0] before_address,interior_address,
 input wire [3:0] before_select,interior_select,input wire [17:0] data);
 wire [31:0] address;wire [3:0] selected;wire active,reading,valid,ready;
 wire [17:0] before_data,capture_data;
 dorado_r_interior_read_stage stage(clk,reset,start,need,accept,finish,
  before_address,interior_address,before_select,interior_select,data,
  address,selected,active,reading,valid,ready,before_data,capture_data);
 always @*begin
  assert(reading==(active && !valid));
  assert(!valid || (active && ready));
  assert(!reading || !ready);
  assert(active || (ready==!need));
  assert(address==(reading ? interior_address:before_address));
  assert(selected==(reading ? interior_select:before_select));
 end
 reg past_valid=0;
 always @(posedge clk)begin
  past_valid<=1;
  if(past_valid)begin
   if($past(reset))begin assert(!active);assert(!valid);end
   else begin
    if($past(!active && start && need))begin
     assert(reading);assert(!ready);assert(before_data==$past(data));
    end
    if($past(reading && !accept))begin
     assert(reading);assert(!valid);assert(!ready);
    end
    if($past(reading && accept))begin
     assert(valid);assert(ready);assert(capture_data==$past(data));
    end
    if($past(valid && !finish))begin assert(valid);assert(ready);end
    if($past(valid && finish))begin assert(!active);assert(!valid);end
   end
   // Only an accepted read may change the capture buffer. Reset invalidates
   // the phase; it leaves both data snapshots untouched.
   if(!$past(reading && accept && !reset))assert(capture_data==$past(capture_data));
   if(!$past(!active && start && need && !reset))assert(before_data==$past(before_data));
  end
 end
endmodule
'''


def run(out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    source=ROOT/'verilog/rtl/dorado_r_interior_read_stage.sv';original=source.read_text()
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,
               source_sha256=sha(source),no_assumptions=True,initial_buffer_bits_unconstrained=36,
               stage_phase_initializes_idle=True,native_integrated=False,cases=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    mutations={None:None,'early-ready':('assign ready=(phase==DONE) ||','assign ready=(phase!=IDLE) ||'),
               'stale-capture':('capture_data<=port_data;','capture_data<=before_data;'),
               'ordinary-stall':('assign ready=(phase==DONE) || ((phase==IDLE) && !need);','assign ready=(phase==DONE);')}
    for mutation,replacement in mutations.items():
        d=out/(mutation or 'contracts');d.mkdir();actual=original
        if replacement:
            a,b=replacement;assert actual.count(a)==1;actual=actual.replace(a,b,1)
        (d/'stage.sv').write_text(actual);(d/'contract.sv').write_text(TOP)
        sat='sat -timeout 120 -prove-asserts '
        sat+=('-seq 4 -falsify -show-ports -show ready -show valid -show active -show reading -show before_data -show capture_data -dump_json /work/'+d.name+'/counterexample.json' if mutation else '-seq 3 -tempinduct -verify')
        (d/'proof.ys').write_text(f'''read_verilog -formal -sv /work/{d.name}/stage.sv /work/{d.name}/contract.sv
hierarchy -check -top contract
proc
flatten
async2sync
opt
check -assert
{sat}
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s',f'/work/{d.name}/proof.ys']
        row=dict(mutation=mutation,command=cmd,files={p.name:sha(p)for p in d.iterdir()},started=time.time())
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        log=(d/'proof.log').read_text()
        passed=code==0 and ('model found: FAIL!' in log if mutation else 'Induction step proven: SUCCESS!' in log)
        row.update(returncode=code,passed=passed,log_sha256=sha(d/'proof.log'),finished=time.time());save()
        print(mutation or 'contracts','PASS'if passed else'FAIL',flush=True)
        if not passed:break
    passed=len(state['cases'])==4 and all(v['passed']for v in state['cases'])
    state.update(status='PASS_INTERIOR_STAGE_CONTRACTS_AND_NEGATIVES'if passed else'TERMINAL_INTERIOR_STAGE_CONTRACT_NOT_ESTABLISHED',finished=time.time())
    state['files']={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()and p.name!='run-state.json'}
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.output.resolve(),a.yosys.resolve()))
