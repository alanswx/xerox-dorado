#!/usr/bin/env python3
"""Prove a registered read frontier against the actual four-bank RM/STK owner.

RAM contents are unconstrained and never reset. Read/write descriptors vary
without protocol assumptions. Induct on each enabled response bank's equality
with the original memory, then require every ready18-bit word to equal the
current physical port. Negative cases must expose actual stale/wrong read data.
Read-only exports in the proof add observers, not another production RAM owner.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
MEM='src/verilog/rtl/dorado_r_file_ports.sv';BUFFER='verilog/rtl/dorado_r_read_buffer.sv'
TOP='''module read_buffer_contract(input wire clk,reset,request,
 input wire [31:0] ra,wa,input wire [3:0] rs,ws,
 input wire commit,input wire [1:0] wn,input wire [15:0] wd,
 output wire safe_data,ready,output wire [17:0] response,physical_word);
 wire [35:0] banks,observed_banks,tag,saved_banks;wire valid;
 wire [15:0] rd;wire [1:0] rp;
 dorado_r_file_ports files(clk,commit,wn,wa,ws,wd,ra,rs,rd,rp,banks,tag[31:0],observed_banks);
 dorado_r_read_buffer buffer(clk,reset,request,ra,rs,banks,commit,wn,wa,ws,wd,
  valid,ready,response,tag,saved_banks);
 assign physical_word={rp,rd};assign safe_data=!ready || response==physical_word;
 reg descriptor_match;
 always @*begin
  descriptor_match=(rs==tag[35:32]);
  for(integer b=0;b<4;b=b+1)begin
   if(!rs[b] && ra[8*b+:8]!=tag[8*b+:8])descriptor_match=0;
   if(valid && !tag[32+b])assert(saved_banks[9*b+:9]==observed_banks[9*b+:9]);
   if(valid && tag[32+b])assert(tag[8*b+:8]==0);
  end
  assert(ready==(valid && !reset && descriptor_match));
  assert(safe_data);
 end
 reg past_valid=0;
 wire [31:0] normalized;
 genvar b;
 generate for(b=0;b<4;b=b+1)begin:normal
  assign normalized[8*b+:8]=rs[b] ? 8'b0:ra[8*b+:8];
 end endgenerate
 always @(posedge clk)begin
  past_valid<=1;
  if(past_valid)begin
   if($past(reset))assert(!valid);
   else if($past(request))begin
    assert(valid);assert(tag==$past({rs,normalized}));
   end else begin
    assert(valid==$past(valid));assert(tag==$past(tag));
   end
  end
 end
endmodule
'''


def reference(raw):
    token='output wire [15:0] read_data,output wire [1:0] read_parity);'
    replaced='''output wire [15:0] read_data,output wire [1:0] read_parity,
 output wire [35:0] physical_banks,input wire [31:0] observer_address,output wire [35:0] observer_banks);'''
    assert raw.count(token)==1
    extra=''' assign physical_banks={stk_l[read_address[31:24]],rm_l[read_address[23:16]],stk_h[read_address[15:8]],rm_h[read_address[7:0]]};
 assign observer_banks={stk_l[observer_address[31:24]],rm_l[observer_address[23:16]],stk_h[observer_address[15:8]],rm_h[observer_address[7:0]]};
'''
    after=raw.replace(token,replaced,1).replace('endmodule',extra+'endmodule',1)
    assert after.replace(replaced,token,1).replace(extra,'',1)==raw
    return after


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    raw=(stage/MEM).read_text();actual=(ROOT/BUFFER).read_text()
    (out/'memory-before.sv').write_text(raw);(out/'memory-reference.sv').write_text(reference(raw));(out/Path(BUFFER).name).write_text(actual)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),memory_sha256=sha(stage/MEM),buffer_sha256=sha(ROOT/BUFFER),
        initial_RAM_bits_unconstrained=9216,initial_response_data_and_tag_unconstrained=True,
        auxiliary_valid_initializes_false=True,assumptions=0,original_RAM_owner_update_unchanged=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    mutations=[('contracts',None),('stale-write',('if(write_commit && !write_n[i/2]',"if(1'b0 && write_commit && !write_n[i/2]")),
        ('wrong-parity',('{~^byte_data,byte_data}','{^byte_data,byte_data}')),
        ('ignore-address-tag',('(response_tag==live_tag)',"1'b1")),
        ('drop-STK-high',"(response_tag[33] ? 9'b0:bank_data[17:9])")]
    save()
    for label,mutation in mutations:
        d=out/label;d.mkdir();candidate=actual
        if isinstance(mutation,tuple):
            a,b=mutation;assert candidate.count(a)==1;candidate=candidate.replace(a,b,1)
        elif mutation:
            assert candidate.count(mutation)==1;candidate=candidate.replace(mutation,"9'b0",1)
        (d/'memory.sv').write_text(reference(raw));(d/'buffer.sv').write_text(candidate);(d/'top.sv').write_text(TOP)
        positive=mutation is None
        sat='sat -timeout 120 -seq 3 -tempinduct -verify -prove-asserts'if positive else'sat -timeout 120 -seq 5 -prove safe_data 1 -falsify -show-ports -dump_json /work/counterexample.json'
        script=f'''read_verilog -formal -sv /work/memory.sv /work/buffer.sv /work/top.sv
hierarchy -check -top read_buffer_contract
proc
flatten
async2sync
chformal -lower
memory_map
opt
check -assert
{sat}
'''
        (d/'proof.ys').write_text(script);env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);a=dict(label=label,positive=positive,pid=p.pid,command=cmd,started=time.time(),files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            state['actions'].append(a);save();code=p.wait()
        marker='Induction step proven: SUCCESS!'if positive else'model found: FAIL!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(state['actions'])==len(mutations)and all(a['passed']for a in state['actions'])
    state.update(status='PASS_REGISTERED_R_READ_FRONTIER'if passed else'TERMINAL_REGISTERED_R_READ_FRONTIER_NOT_ESTABLISHED',finished=time.time(),
        native_caller_or_original60ns_chronology_integrated=False,FPGA_or_firmware_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
