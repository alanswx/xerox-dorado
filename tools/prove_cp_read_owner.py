#!/usr/bin/env python3
"""Prove the common828-bit publication and physical SRAM read/write barrier.

Use the tested synthesized caller, tagged read stage and actual four256x9
SRAM arrays. All owner, context, tag, buffer and SRAM power-up values are
unconstrained except the auxiliary IDLE/monitor validity initializers. Probe
ports are FORMAL ONLY; they add no architectural state or physical read port.
The next processor payload and original16 external R aliases are abstracted;
full processor/original gate correctness and native integration remain separate.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

TOP='''module contract(input wire clk,reset,want,prefix_ready,root,need,accept,
 input wire [843:0] next_q,input wire [400:0] after_pins,input wire [7:0] vh,vl,
 input wire [31:0] ba,ia,input wire [3:0] bs,isel,
 input wire [1:0] wn,input wire [15:0] wd,
 output wire owner_holds,memory_holds,
 output wire [827:0] owned,output wire [9215:0] memory,
 output reg [827:0] previous_owned,output reg [9215:0] previous_memory,
 output wire commit,ready,output reg previous_commit);
 FUNCTIONS
 wire [843:0] held;wire [400:0] before_pins;wire before_root;
 wire [17:0] before_data,capture_data;wire valid,active,reading,busy,match;
 wire [31:0] physical_address;wire [3:0] physical_select;wire [401:0] context;
 dorado_cp_read_caller dut(clk,reset,want,prefix_ready,root,after_pins,next_q,vh,vl,
  need,accept,ba,ia,bs,isel,wn,wd,
  held,before_pins,before_root,ready,commit,before_data,capture_data,valid,
  active,reading,busy,match,physical_address,physical_select,memory,owned,context);
 function automatic [17:0] peek(input [9215:0] mem,input [31:0] addr,input [3:0] sel);
  reg [8:0] h,l;
  reg [8:0] rh[0:255],sh[0:255],rl[0:255],sl[0:255];
  begin
   // Same1024 physical words, represented as word arrays rather than a
   // packed arithmetic bit shift or a long priority-OR lookup chain.
   for(integer n=0;n<256;n++)begin
    rh[n]=mem[9*n+:9];sh[n]=mem[2304+9*n+:9];
    rl[n]=mem[4608+9*n+:9];sl[n]=mem[6912+9*n+:9];
   end
   h=(sel[0] ? 9'b0:rh[addr[7:0]]) | (sel[1] ? 9'b0:sh[addr[15:8]]);
   l=(sel[2] ? 9'b0:rl[addr[23:16]]) | (sel[3] ? 9'b0:sl[addr[31:24]]);
   peek={l[8],h[8],l[7:0],h[7:0]};
  end
 endfunction
 reg past_valid=0;
 reg [401:0] previous_context,expected_context;
 reg [827:0] expected_owned;
 always @(posedge clk)begin
  past_valid<=1;previous_owned<=owned;previous_memory<=memory;
  previous_context<=context;previous_commit<=commit;
  expected_owned<={next_q[843:588],next_q[571:0]};
  expected_context<={cp_source_pins(after_pins,cp_bus(after_pins)),root};
 end
 assign owner_holds=!past_valid || previous_commit || owned==previous_owned;
 assign memory_holds=!past_valid || previous_commit || memory==previous_memory;
 always @*begin
  assert(owner_holds);assert(memory_holds);
  assert(commit==(want && ready));
  assert(!ready || (prefix_ready && !reset));
  assert(!need || valid || !ready);
  assert(!busy || match || !ready);
  assert(need || busy || !prefix_ready || reset || ready);
  assert({held[843:588],held[571:0]}==owned);
  assert(held[587:572]=={vl,vh});
  assert({before_pins,before_root}==context);
  // Logical observation of the same SRAM state, never an additional DUT port.
  assert(before_data==peek(memory,ba,bs));
  assert(!valid || capture_data==peek(memory,ia,isel));
  if(past_valid)begin
   if(previous_commit)begin assert(owned==expected_owned);assert(context==expected_context);end
   else assert(context==previous_context);
  end
 end
endmodule
'''


def instrument_memory(raw):
    original=raw
    a='output wire [15:0] read_data,output wire [1:0] read_parity);'
    b=a[:-2]+',output wire [9215:0] formal_memory);'
    assert raw.count(a)==1;raw=raw.replace(a,b,1)
    probe=''' for(genvar n=0;n<256;n++)begin: formal_probe
  assign formal_memory[9*n+:9]=rm_h[n];
  assign formal_memory[2304+9*n+:9]=stk_h[n];
  assign formal_memory[4608+9*n+:9]=rm_l[n];
  assign formal_memory[6912+9*n+:9]=stk_l[n];
 end
'''
    assert raw.count('endmodule')==1;raw=raw.replace('endmodule',probe+'endmodule',1)
    assert raw.replace(probe,'',1).replace(b,a,1)==original
    return raw


def instrument_caller(raw):
    original=raw
    a='output wire [31:0] physical_address,output wire [3:0] physical_select);'
    b=a[:-2]+',output wire [9215:0] formal_memory,output wire [827:0] formal_owned,output wire [401:0] formal_context);'
    assert raw.count(a)==1;raw=raw.replace(a,b,1)
    a2='physical_read[15:0],physical_read[17:16]);';b2='physical_read[15:0],physical_read[17:16],formal_memory);'
    assert raw.count(a2)==1;raw=raw.replace(a2,b2,1)
    probe=' assign formal_owned={r_front_upper_q,r_front_lower_q};\n assign formal_context={r_front_before,r_front_clock_before};\n'
    assert raw.count('endmodule')==1;raw=raw.replace('endmodule',probe+'endmodule',1)
    assert raw.replace(probe,'',1).replace(b2,a2,1).replace(b,a,1)==original
    return raw


def run(source,cp,out,yosys):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE'and s['synthesized_read_owner']and s['owner_mutation']is None
    for r in s['source_files']:assert sha(source/r['path'])==r['output_sha256']
    out.mkdir(parents=True);shutil.copy2(Path(__file__),out/Path(__file__).name)
    caller=instrument_caller((source/'dorado_cp_read_caller.sv').read_text())
    memory=instrument_memory((source/'inputs/verilog/rtl/dorado_r_file_ports.sv').read_text())
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
               no_assumptions=True,architectural_owner_bits=828,external_visible_R_bits=16,stored_context_bits=402,SRAM_bits=9216,
               architectural_powerup_unconstrained=True,native_integrated=False,ordinary_pair_path_qualified=False,cases=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    mutations={None:None,'early-owner':('if(contb_advance && control_ready)begin','if(contb_advance && prefix_ready)begin'),
               'early-ram':('files(storage_clk,owner_commit,','files(storage_clk,contb_advance,')}
    for mutation,replacement in mutations.items():
        d=out/(mutation or'contracts');d.mkdir();raw=caller
        if replacement:
            a,b=replacement;assert raw.count(a)==1;raw=raw.replace(a,b,1)
        (d/'caller.sv').write_text(raw);(d/'memory.sv').write_text(memory)
        for name in ['dorado_r_interior_read_stage.sv','dorado_r_interior_read_tagged_stage.sv']:
            (d/name).write_bytes((source/'verilog/rtl'/name).read_bytes())
        (d/'contract.sv').write_text(TOP.replace('FUNCTIONS',(cp/'functions.sv').read_text()))
        sat='sat -timeout 120 '
        if mutation:
            goal='owner_holds'if mutation=='early-owner'else'memory_holds'
            sat+=f'-seq 4 -prove {goal} 1 -falsify -show-ports -dump_json /work/{d.name}/counterexample.json'
        else:sat+='-seq 3 -tempinduct -verify -prove-asserts'
        (d/'proof.ys').write_text(f'''read_verilog -formal -sv /work/{d.name}/caller.sv /work/{d.name}/memory.sv /work/{d.name}/dorado_r_interior_read_stage.sv /work/{d.name}/dorado_r_interior_read_tagged_stage.sv /work/{d.name}/contract.sv
hierarchy -check -top contract
proc
flatten
async2sync
memory_map
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
    passed=len(state['cases'])==3 and all(r['passed']for r in state['cases'])
    state.update(status='PASS_CP_READ_OWNER_AND_SRAM_BARRIER'if passed else'TERMINAL_CP_READ_OWNER_PROOF_NOT_ESTABLISHED',finished=time.time())
    state['files']={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()and p.name!='run-state.json'};save()
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','cp-source','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.cp_source.resolve(),a.output.resolve(),a.yosys.resolve()))
