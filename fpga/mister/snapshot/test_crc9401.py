#!/usr/bin/env python3
"""F9401 original and independent CRC oracle, PRECAP0/1, complete restoration."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import generate_crc9401 as gen
HERE=Path(__file__).resolve().parent
FIXTURE='''module crc9401_fixture(
input wire host_clk,ref_clk,advance,freeze,held,request,write,individual,
input wire [7:0] pins,input wire [1:0] index,input wire [17:0] data,restore_data[2],
output wire [17:0] result[2],output wire [18:0] physical[2],original_state[2],
output wire [1:0] frozen,done,error,output wire [3:0] outputs,original_outputs);
genvar i;generate for(i=0;i<2;i=i+1)begin:cells
cell_F9401_snapshot #(.PRECAP(i)) candidate(.state_host_clk(host_clk),.state_advance(advance),
.state_freeze(freeze),.state_guest_held(held),.state_request(request),.state_write(write),
.state_index(index),.state_data(individual?restore_data[i]:data),.state_frozen(frozen[i]),.state_done(done[i]),.state_error(error[i]),.state_result(result[i]),.sys_clk(ref_clk),PORTS);
cell_F9401 #(.PRECAP(i)) original(.sys_clk(ref_clk),REFPORTS);
assign physical[i]={candidate.cwe_pre,candidate.d_pre,candidate.cp_d,candidate.r};
assign original_state[i]={original.cwe_pre,original.d_pre,original.cp_d,original.r};
end endgenerate endmodule
'''
PORTS=".p1(pins[0]),.p2(pins[1]),.p4(pins[2]),.p3(pins[3]),.p5(pins[4]),.p8(pins[5]),.p10(pins[6]),.p11(pins[7]),.p7(1'b0),.p14(1'b1),.p12(outputs[2*i]),.p13(outputs[2*i+1])"
FIXTURE=FIXTURE.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)

def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source,m=gen.generate()
    for n in ('generate_crc9401.py','test_crc9401.py','test_crc9401.cpp'):(out/n).write_bytes((HERE/n).read_bytes())
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('ungated','!state_hold && state_advance','!state_hold','state oracle differs'),
        ('ignore_freeze','!state_hold && state_advance','state_advance','state oracle differs'),
        ('running_access','state_hold && state_frozen && state_request','state_request','request handshake differs'),
        ('invalid_alias','!state_index[1]',"1'b1",'request handshake differs'),
        ('lost_high','cwe_pre<=state_data[0]',"cwe_pre<=1'b0",'state oracle differs'),
        ('ignore_precap','PRECAP ? d_pre : p11','p11','state oracle differs'),
        ('wrong_edge','cp_d && !p1','!cp_d && p1','state oracle differs'),
        ('wrong_crc',"16'h8004", "16'h8000",'state oracle differs')]
    rows=[]
    for name,before,after,why in cases:
        d=out/name;d.mkdir();text=source
        if before:assert before in text;text=text.replace(before,after,1)
        (d/'fixture.sv').write_text(FIXTURE);(d/'cell_F9401_snapshot.sv').write_text(text)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','crc9401_fixture','-Wno-fatal','--Mdir',str(d/'obj'),str(d/'fixture.sv'),str(d/'cell_F9401_snapshot.sv'),str(gen.ROOT/'verilog/cells/cell_F9401.v'),str(HERE/'test_crc9401.cpp')]
        (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(d/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        p=subprocess.run([str(d/'obj/Vcrc9401_fixture')],capture_output=True,text=True,timeout=60);text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        if why:assert p.returncode and why in text,(name,text)
        else:assert p.returncode==0 and'CRC9401 PASS'in text,text
        rows.append(dict(case=name,returncode=p.returncode,expected_failure=why))
    controls=[];original=(gen.ROOT/'verilog/cells/cell_F9401.v').read_text()
    for name,text in [('extra_state',original.replace('endmodule','reg hidden;\nendmodule')),('changed_priority',original.replace('if (p4)','if (!p4)'))]:
        try:gen.generate(text)
        except ValueError:controls.append(dict(case=name,rejected=True))
        else:raise AssertionError('unreviewed CRC original accepted')
    paths=[HERE/n for n in ('generate_crc9401.py','test_crc9401.py','test_crc9401.cpp','generate_snapshot_ff.py')]+[gen.ROOT/'verilog/cells/cell_F9401.v']
    (out/'results.json').write_text(json.dumps(dict(status='PASS',manifest=m,cases=rows,source_controls=controls,source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
