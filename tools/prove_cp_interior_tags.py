#!/usr/bin/env python3
"""Prove stale port descriptors cannot publish a buffered R SRAM read.

Address/select inputs vary arbitrarily without protocol assumptions. A separate
native-caller gate must still freeze architectural owners and block SRAM writes
while a read is pending; this component proof does not establish that wiring.
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
 input wire [31:0] ba,ia,input wire [3:0] bs,isel,input wire [17:0] data,
 input wire want_commit,write_enable,output wire stale_data_blocked);
 wire [31:0] addr;wire [3:0] sel;wire active,reading,valid,ready,busy,tag_match;
 wire [17:0] before_data,capture_data;
 dorado_r_interior_read_tagged_stage stage(clk,reset,start,need,accept,finish,
  ba,ia,bs,isel,data,addr,sel,active,reading,valid,ready,before_data,capture_data,busy,tag_match);
 wire [71:0] descriptor={ba,ia,bs,isel};
 // Independent transaction-acceptance observer. This is a monitor, never
 // initialized or used by the DUT. It records the public request at start.
 reg [71:0] accepted_descriptor;
 always @(posedge clk)if(!reset && !busy && start && need)accepted_descriptor<=descriptor;
 wire match=!busy || (accepted_descriptor==descriptor);
 wire commit=want_commit && ready;
 wire memory_write=commit && write_enable;
 assign stale_data_blocked=!busy || match || (!ready && !valid);
 always @*begin
  assert(tag_match==match);
  assert(!ready || !reset);
  assert(!valid || (active && busy && ready && match));
  assert(!busy || match || (!ready && !valid && !active && !reading));
  assert(!reading || (active && !ready && !valid));
  assert(busy || reset || (ready==!need));
  assert(addr==(reading ? ia:ba));assert(sel==(reading ? isel:bs));
  assert(!busy || match || !commit);
  assert(!busy || match || !memory_write);
  assert(!reading || (!commit && !memory_write));
 end
 reg past_valid=0;
 always @(posedge clk)begin
  past_valid<=1;
  if(past_valid)begin
   if($past(reset || (busy && !match)))begin assert(!busy);assert(!valid);end
   else begin
    if($past(!busy && start && need))begin
     assert(busy);assert(before_data==$past(data));
     if(match && !reset)begin assert(reading);assert(!ready);end
    end
    if($past(reading && !accept))begin assert(busy);assert(!valid);end
    if($past(reading && accept))begin
     assert(busy);assert(capture_data==$past(data));
     if(match && !reset)begin assert(valid);assert(ready);end
    end
    if($past(valid && !finish) && match && !reset)begin assert(valid);assert(ready);end
    if($past(valid && finish))begin assert(!busy);assert(!valid);end
   end
  end
 end
endmodule
'''


def run(out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    parent=ROOT/'verilog/rtl/dorado_r_interior_read_stage.sv';source=ROOT/'verilog/rtl/dorado_r_interior_read_tagged_stage.sv'
    original=source.read_text();shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,
               parent_sha256=sha(parent),source_sha256=sha(source),no_assumptions=True,
               initial_data_and_tag_bits_unconstrained=108,auxiliary_phase_initializes_idle=True,
               negative_target_actual_ready_or_valid=True,native_integrated=False,cases=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    mutations={None:None,'ignore-before-address':('descriptor_q==descriptor','descriptor_q[39:0]==descriptor[39:0]'),
               'ignore-interior-address':('descriptor_q==descriptor','{descriptor_q[71:40],descriptor_q[7:0]}=={descriptor[71:40],descriptor[7:0]}'),
               'ignore-before-select':('descriptor_q==descriptor','{descriptor_q[71:8],descriptor_q[3:0]}=={descriptor[71:8],descriptor[3:0]}'),
               'ignore-interior-select':('descriptor_q==descriptor','descriptor_q[71:4]==descriptor[71:4]'),
               'ignore-ready-tag':('assign ready=raw_ready && tag_match && !reset;','assign ready=raw_ready && !reset;'),
               'ignore-valid-tag':('assign capture_valid=raw_valid && tag_match && !reset;','assign capture_valid=raw_valid && !reset;')}
    for mutation,replacement in mutations.items():
        d=out/(mutation or'contracts');d.mkdir();raw=original
        if replacement:
            a,b=replacement;assert raw.count(a)==1;raw=raw.replace(a,b,1)
        (d/'tagged.sv').write_text(raw);(d/'parent.sv').write_bytes(parent.read_bytes());(d/'contract.sv').write_text(TOP)
        sat='sat -timeout 120 '
        sat+=('-seq 5 -prove stale_data_blocked 1 -falsify -show-ports -show ready -show valid -show busy -show tag_match -show before_data -show capture_data -dump_json /work/'+d.name+'/counterexample.json'if mutation else'-seq 3 -tempinduct -verify -prove-asserts')
        (d/'proof.ys').write_text(f'''read_verilog -formal -sv /work/{d.name}/parent.sv /work/{d.name}/tagged.sv /work/{d.name}/contract.sv
hierarchy -check -top contract
proc
flatten
async2sync
opt
check -assert
{sat}
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s',f'/work/{d.name}/proof.ys'];row=dict(mutation=mutation,command=cmd,files={p.name:sha(p)for p in d.iterdir()},started=time.time())
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        log=(d/'proof.log').read_text();passed=code==0 and ('model found: FAIL!'in log if mutation else'Induction step proven: SUCCESS!'in log)
        row.update(returncode=code,passed=passed,log_sha256=sha(d/'proof.log'),finished=time.time());save()
        print(mutation or'contracts','PASS'if passed else'FAIL',flush=True)
        if not passed:break
    passed=len(state['cases'])==len(mutations)and all(v['passed']for v in state['cases'])
    state.update(status='PASS_INTERIOR_DESCRIPTOR_TAGS_AND_NEGATIVES'if passed else'TERMINAL_INTERIOR_TAG_CONTRACT_NOT_ESTABLISHED',finished=time.time())
    state['files']={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()and p.name!='run-state.json'}
    save();return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.output.resolve(),a.yosys.resolve()))
