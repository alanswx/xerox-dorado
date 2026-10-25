#!/usr/bin/env python3
"""Compare the real child and parallel R-fault bit across every X/Z input.

Four deterministic binary backgrounds, both known selector values, and X/Z at
every one of1517 inputs. Known selector branches have exactly the reference
function's arguments; unknown selector must use the unchanged original fault.
A deliberately inverted unknown fallback must fail the actual comparison.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_parallel_r_fault import INPUTS,input_declarations,child_call,helper
from test_r_front_forward_view import sha

TB='''`timescale 1ns/1ps
module tb_fault_four_state;
 localparam integer WIDTH=1517,LATE=473;
 reg [WIDTH-1:0] stimulus,base;
 WIRES;
 assign {NAMES}=stimulus;
 wire [3:0] expected;wire actual;
 REFERENCE
 dorado_parallel_r_fault_step candidate(BINDINGS,.original_fault(expected[1]),.fault(actual));
 integer checks=0,known=0,unknown=0,unknown_selectors=0,low=0,high=0;
 reg [31:0] seed=32'hdeca0123;
 task automatic compare(input integer trial,input integer bit_no,input integer mode);
  begin
   #1;
   if(actual!==expected[1])$fatal(1,"FAULT_COFACTOR mismatch trial=%0d bit=%0d mode=%0d late=%b actual=%b expected=%b",trial,bit_no,mode,pins_after[209],actual,expected[1]);
   checks++;
   case(pins_after[209])1'b0:low++;1'b1:high++;default:unknown_selectors++;endcase
  end
 endtask
 initial begin
  for(integer t=0;t<4;t++)begin
   for(integer b=0;b<WIDTH;b++)begin
    seed={seed[30:0],seed[31]^seed[21]^seed[1]^seed[0]};
    case(t)0:base[b]=0;1:base[b]=1;default:base[b]=seed[0];endcase
   end
   for(integer flag=0;flag<2;flag++)begin
    stimulus=base;stimulus[LATE]=1'(flag);compare(t,-1,flag);
    if(expected[1]!==1'b0 && expected[1]!==1'b1)$fatal(1,"FAULT_COFACTOR binary reference unknown");
    known++;
   end
   base[LATE]=1'(t%2);
   for(integer b=0;b<WIDTH;b++)begin
    stimulus=base;stimulus[b]=1'bx;compare(t,b,2);unknown++;
    stimulus=base;stimulus[b]=1'bz;compare(t,b,3);unknown++;
   end
  end
  if(known!=8 || unknown!=12136 || unknown_selectors!=8 || low==0 || high==0)$fatal(1,"FAULT_COFACTOR coverage");
  $display("FAULT_COFACTOR_FOUR_STATE_PASS inputs=1517 known=%0d unknown=%0d selector_unknown=%0d low=%0d high=%0d checks=%0d",known,unknown,unknown_selectors,low,high,checks);
  $finish;
 end
endmodule
'''


def run(proof,out,mutation):
    assert not out.exists();s=json.loads((proof/'run-state.json').read_text())
    assert s['status']=='PASS_PARALLEL_R_FAULT_ALL_BINARY_INPUTS' and all(v['passed'] for v in s['cases'])
    for n,h in s['files'].items():assert sha(proof/n)==h
    source=Path(s['source']);source_state=json.loads((source/'run-state.json').read_text())
    out.mkdir(parents=True);paths=[]
    for row in source_state['source_files']:
        rel='inputs/'+row['path'];src=proof/rel;assert sha(src)==s['files'][rel]
        dest=out/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dest)
        if row['path']!='candidate.sv':paths.append(dest)
    wires=';\n '.join('wire '+(f'[{w-1}:0] ' if w>1 else '')+n for n,w in INPUTS)
    tb=TB.replace('WIRES',wires).replace('NAMES',','.join(n for n,w in INPUTS)).replace(
        'REFERENCE',child_call('reference','pins_after','expected')).replace('BINDINGS',','.join('.'+n+'('+n+')' for n,w in INPUTS))
    (out/'helper.sv').write_text(helper(mutation));(out/'tb.sv').write_text(tb)
    paths.extend([out/'helper.sv',out/'tb.sv'])
    for n in [Path(__file__).name,'generate_parallel_r_fault.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        mutation=mutation,files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},actions=[],hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_fault_four_state','-o',str(out/'test.vvp'),*map(str,paths)]),('test',['vvp','-i',str(out/'test.vvp')])]:
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:break
    text=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=state['actions'][0]['returncode']==0 and ((code!=0 and 'FAULT_COFACTOR mismatch' in text) if mutation else
        (code==0 and 'FAULT_COFACTOR_FOUR_STATE_PASS inputs=1517 known=8 unknown=12136 selector_unknown=8' in text))
    state.update(status=('PASS_PARALLEL_FAULT_UNKNOWN_COUNTEREXAMPLE' if mutation else 'PASS_PARALLEL_FAULT_FOUR_STATE') if passed else 'TERMINAL_PARALLEL_FAULT_FOUR_STATE_NOT_ESTABLISHED',
        finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--proof',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutation',choices=['bad-unknown'])
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.mutation))
