#!/usr/bin/env python3
"""K1115A original/signed-arithmetic oracle/complete state restore tests."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import generate_oscillator as gen
HERE=Path(__file__).resolve().parent
FIXTURE='''module oscillator_fixture(
input wire host_clk,ref_clk,advance,freeze,held,request,write,individual,
input wire [17:0] restore_data[10],
input wire [1:0] index,input wire [17:0] data,
output wire [17:0] result[10],output wire [32:0] physical[10],original_state[10],
output wire [9:0] frozen,done,error,outputs,original_outputs);
genvar i;
generate for(i=0;i<10;i=i+1)begin:cells
localparam integer SP=i==9?8:i/3==0?2:i/3==1?4:16;
localparam integer FREQ=i==9?23530:i%3==0?10000:i%3==1?20000:50000;
cell_K1115A_snapshot #(.FREQ_KHZ(FREQ),.SYSPER(SP)) candidate(
.state_host_clk(host_clk),.state_advance(advance),.state_freeze(freeze),.state_guest_held(held),
.state_request(request),.state_write(write),.state_index(index),.state_data(individual?restore_data[i]:data),
.state_frozen(frozen[i]),.state_done(done[i]),.state_error(error[i]),.state_result(result[i]),
.sys_clk(ref_clk),.p8(outputs[i]),.p7(1'b0),.p14(1'b1));
cell_K1115A #(.FREQ_KHZ(FREQ),.SYSPER(SP)) original(
.sys_clk(ref_clk),.p8(original_outputs[i]),.p7(1'b0),.p14(1'b1));
assign physical[i]={candidate.osc,candidate.acc};
assign original_state[i]={original.osc,original.acc};
end endgenerate
endmodule
'''


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source,m=gen.generate()
    for n in ('generate_oscillator.py','test_oscillator.py','test_oscillator.cpp'):(out/n).write_bytes((HERE/n).read_bytes())
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('ungated','!state_hold && state_advance','!state_hold','state oracle differs'),
        ('ignore_freeze','!state_hold && state_advance','state_advance','state oracle differs'),
        ('lost_high','{osc,acc[31:18]}<=state_data[14:0]',"{osc,acc[31:18]}<=15'b0",'state oracle differs'),
        ('running_access','state_hold && state_frozen && state_request','state_request','request handshake differs'),
        ('invalid_alias','!state_index[1]',"1'b1",'request handshake differs'),
        ('unsigned_math','acc + STEP >= SYS_KHZ','$unsigned(acc + STEP) >= SYS_KHZ','state oracle differs'),
        ('wrong_threshold','acc + STEP >= SYS_KHZ','acc + STEP > SYS_KHZ','state oracle differs'),
        ('lost_output','osc <= ~osc',"osc <= 1'b0",'state oracle differs'),
    ]
    results=[]
    for name,before,after,diagnostic in cases:
        d=out/name;d.mkdir();text=source
        if before:assert text.count(before)==1,(name,before);text=text.replace(before,after,1)
        (d/'fixture.sv').write_text(FIXTURE);(d/'cell_K1115A_snapshot.sv').write_text(text)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','oscillator_fixture','-Wno-fatal','--Mdir',str(d/'obj'),str(d/'fixture.sv'),str(d/'cell_K1115A_snapshot.sv'),str(gen.ROOT/'verilog/cells/cell_K1115A.v'),str(HERE/'test_oscillator.cpp')]
        (d/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(d/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        p=subprocess.run([str(d/'obj/Voscillator_fixture')],capture_output=True,text=True,timeout=60);text=p.stdout+p.stderr;(d/'test.txt').write_text(text)
        if diagnostic:assert p.returncode and diagnostic in text,(name,text)
        else:assert p.returncode==0 and'OSCILLATOR PASS'in text,text
        print(name+': '+text.strip(),flush=True);results.append(dict(case=name,returncode=p.returncode,expected_failure=diagnostic))
    original=(gen.ROOT/'verilog/cells/cell_K1115A.v').read_text();controls=[]
    for name,text in [('extra_state',original.replace('endmodule','reg hidden;\nendmodule')),('changed_math',original.replace('>= SYS_KHZ','> SYS_KHZ'))]:
        try:gen.generate(text)
        except ValueError:controls.append(dict(case=name,rejected=True))
        else:raise AssertionError('changed original accepted')
    paths=[HERE/n for n in ('test_oscillator.py','test_oscillator.cpp','generate_oscillator.py')]+[gen.ROOT/'verilog/cells/cell_K1115A.v']
    (out/'results.json').write_text(json.dumps(dict(status='PASS',parameter_modes=[dict(sysper=sp,freq_khz=f)for sp in (2,4,16)for f in (10000,20000,50000)]+[dict(sysper=8,freq_khz=23530)],manifest=m,cases=results,source_controls=controls,source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
