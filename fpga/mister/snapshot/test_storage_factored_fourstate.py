#!/usr/bin/env python3
"""Four-state procedural/priority equivalence against the qualified serial cell.

Inputs include deliberately unreachable restored states and X/Z controls. Only
the original simulation assertions about legal guest advances are removed from
BOTH cells for this algebra check. Valid guest/DRAM behavior is tested separately
by test_storage_factored.py against untouched original RTL and actual DRAM cells.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_storage_factored as gen


def assertion_free(source):
    pattern=r'    // synthesis translate_off\n.*?    // synthesis translate_on\n'
    assert len(re.findall(pattern,source,re.S))==1
    return re.sub(pattern,'',source,flags=re.S)


def fixture():
    top='''module fourstate;
 reg clk=0,advance=0,hold=0,scan_step=0,serial_in=0,reset=0;
 reg [1:0] ras_n=3,cas_n=3,we_n=3,req_ready=0,rsp_valid=0;
 reg [15:0] addr[0:1],data[0:1],rsp_data[0:1];
 wire [127:0] physical_old[0:1],physical_new[0:1];
 wire [54:0] ports_old[0:1],ports_new[0:1];
 integer ticks=0,seed=32'h51a7face;reg [31:0] noise;
'''
    for i,width in enumerate((8,12)):
        for side,module in [('old','dorado_storage_group_reference'),('new','dorado_storage_group_snapshot')]:
            name=f'{side}{i}';top+=f''' wire [{width-1}:0] dout_{name};
 wire [15:0] req_addr_{name},req_data_{name};
 wire ready_{name},valid_{name},wr_{name},rsp_{name},frozen_{name},drained_{name},serial_{name};
 {module} #(.ADDR_BITS(16),.WIDTH({width})) {name}(
 .state_guest_advance(advance),.state_hold(hold),.state_scan_step(scan_step),.state_serial_in(serial_in),
 .state_serial_out(serial_{name}),.state_frozen(frozen_{name}),.state_drain_ready(drained_{name}),
 .host_clk(clk),.machine_clk(clk),.reset(reset),.ras_n(ras_n[{i}]),.cas_n(cas_n[{i}]),.we_n(we_n[{i}]),
 .addr(addr[{i}]),.data_in(data[{i}][{width-1}:0]),.data_out(dout_{name}),
 .ready(ready_{name}),.req_valid(valid_{name}),.req_write(wr_{name}),.req_addr(req_addr_{name}),.req_data(req_data_{name}),
 .req_ready(req_ready[{i}]),.rsp_valid(rsp_valid[{i}]),.rsp_ready(rsp_{name}),.rsp_data(rsp_data[{i}]));
 assign ports_{side}[{i}]={{serial_{name},drained_{name},frozen_{name},rsp_{name},wr_{name},valid_{name},ready_{name},req_addr_{name},req_data_{name},{{{16-width}{{1'b0}}}},dout_{name}}};
'''
            packed='{'+','.join(name+'.'+n for n,_ in reversed(gen.base.FIELDS))+'}'
            bits=sum(w for _,w in gen.base.fields(width))
            top+=f" assign physical_{side}[{i}]={{{128-bits}'b0,{packed}}};\n"
    top+=''' task edge_and_check;
 begin
  #2;clk=1;#2;ticks=ticks+1;
  for(integer unit=0;unit<2;unit=unit+1)begin
   if(physical_old[unit]!==physical_new[unit])$fatal(1,"four-state physical mismatch tick=%0d unit=%0d",ticks,unit);
   if(ports_old[unit]!==ports_new[unit])$fatal(1,"four-state port mismatch tick=%0d unit=%0d",ticks,unit);
  end
  clk=0;#2;
 end endtask
 initial begin
  for(integer u=0;u<2;u=u+1)begin addr[u]=0;data[u]=16'h95a7;rsp_data[u]=16'hc3e1;end
  advance=1;edge_and_check;
  hold=1'bx;cas_n=0;edge_and_check;
  hold=1'bz;cas_n=3;edge_and_check;
  hold=1;scan_step=1;serial_in=1'bx;
  repeat(140)edge_and_check;
  for(integer n=0;n<12000;n=n+1)begin
   noise=$random(seed);{advance,hold,scan_step,serial_in,reset,ras_n,cas_n,we_n,req_ready,rsp_valid}=noise[14:0];
   for(integer u=0;u<2;u=u+1)begin addr[u]=$random(seed);data[u]=$random(seed);rsp_data[u]=$random(seed);end
   case(n%19)
    0:hold=1'bx;1:hold=1'bz;2:scan_step=1'bx;3:advance=1'bx;
    4:reset=1'bx;5:serial_in=1'bx;6:serial_in=1'bz;
    7:cas_n[0]=1'bx;8:we_n[1]=1'bz;9:req_ready[0]=1'bx;
    10:rsp_valid[1]=1'bx;11:rsp_data[0][7]=1'bz;12:addr[1][15]=1'bx;
   endcase
   edge_and_check;
  end
  $display("FOUR-STATE FACTORED PASS ticks=%0d physical=111/123",ticks);$finish;
 end
endmodule
'''
    return top


def run(out):
    out.mkdir(parents=True,exist_ok=False);reference,_=gen.base.generate();candidate,manifest=gen.generate()
    (out/'qualified-reference.sv').write_text(reference);(out/'factored.sv').write_text(candidate)
    reference=assertion_free(reference).replace('module dorado_storage_group_snapshot #(','module dorado_storage_group_reference #(',1)
    for name,raw in [('reference.sv',reference),('fixture.sv',fixture())]:(out/name).write_text(raw)
    results=[]
    cases=[('candidate',None,None),('x_hold_coercion','if(state_hold)begin',"if(state_hold !== 1'b0)begin"),
           ('priority_loss','write_done_next=1;','write_done_next=0;'),
           ('scan_enable_loss','if(update_enable[12]) cache_data<=cache_data_next;',"if(update_enable[12] && !state_hold) cache_data<=cache_data_next;")]
    for name,before,after in cases:
        case=out/name;case.mkdir();source=assertion_free(candidate)
        if before:assert source.count(before)==1;source=source.replace(before,after,1)
        (case/'source.sv').write_text(source)
        command=['iverilog','-g2012','-s','fourstate','-o',str(case/'fixture.vvp'),str(out/'fixture.sv'),str(out/'reference.sv'),str(case/'source.sv')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'fixture.vvp')],capture_output=True,text=True,timeout=30)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log);print(name+': '+log.strip(),flush=True)
        if before:assert result.returncode and 'four-state physical mismatch'in log
        else:assert result.returncode==0 and 'FOUR-STATE FACTORED PASS'in log
        results.append(dict(case=name,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,manifest=manifest,cases=results,
        executed_hashes={str(p.relative_to(out)):hashlib.sha256(p.read_bytes()).hexdigest()for p in out.rglob('*')if p.is_file()and p.suffix in ('.sv','.json','.txt')},
        tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
