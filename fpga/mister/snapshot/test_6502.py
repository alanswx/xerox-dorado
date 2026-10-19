#!/usr/bin/env python3
"""Netlist6502 original/candidate continuation and complete node-state restore."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_6502 as gen

HERE=Path(__file__).resolve().parent
FIXTURE='''module netlist6502_fixture(
input host_clk,ref_clk,advance,freeze,held,request,write,
input [6:0] index,input [17:0] data,input [13:0] pins,
output [17:0] result,output frozen,done,error,
output [1724:0] physical,original_state,next_state,
output [25:0] outputs,original_outputs);
chip_6502_snapshot candidate(.state_host_clk(host_clk),.state_advance(advance),
.state_freeze(freeze),.state_guest_held(held),.state_request(request),.state_write(write),
.state_index(index),.state_data(data),.state_frozen(frozen),.state_done(done),
.state_error(error),.state_result(result),.clk(ref_clk),PORTS);
chip_6502 original(.clk(ref_clk),REFPORTS);
assign physical=candidate.q;assign original_state=original.q;
assign next_state=candidate.no;
endmodule
'''
PORTS='.phi(pins[0]),.res(pins[1]),.so(pins[2]),.rdy(pins[3]),.nmi(pins[4]),.irq(pins[5]),.dbi(pins[13:6]),.dbo(outputs[7:0]),.rw(outputs[8]),.sync(outputs[9]),.ab(outputs[25:10])'
FIXTURE=FIXTURE.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source,m=gen.generate()
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('ungated','!state_hold && state_advance','!state_hold','node oracle differs'),
        ('ignore_freeze','!state_hold && state_advance','state_advance','node oracle differs'),
        ('running_access','state_hold && state_frozen && state_request','state_request','request handshake differs'),
        ('invalid_alias',"state_index<7'd96","1'b1",'request handshake differs'),
        ('lost_last',"q[1724:1710]<=state_data[14:0]","q[1724:1710]<=15'b0",'node oracle differs'),
        ('word_alias',"7'd1:if(state_write)q[35:18]","7'd1:if(state_write)q[17:0]",'node oracle differs'),
        ('wrong_next','state_advance)q<=no','state_advance)q<=ni','node oracle differs'),
        ('lost_restore_bit',"q[17:0]<=state_data[17:0]","q[17:0]<={state_data[17:1],1'b0}",'node oracle differs')]
    results=[]
    for name,before,after,why in cases:
        d=out/name;d.mkdir();raw=source
        if before:
            assert raw.count(before.encode())==1,(name,before)
            raw=raw.replace(before.encode(),after.encode(),1)
        (d/'chip_6502_snapshot.sv').write_bytes(raw);(d/'fixture.sv').write_text(FIXTURE)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','netlist6502_fixture',
             '-Wno-fatal','--Mdir',str(d/'obj'),str(d/'fixture.sv'),str(d/'chip_6502_snapshot.sv'),
             str(gen.SOURCE),str(HERE/'test_6502.cpp')]
        (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(d/'build.log').open('w')as log:subprocess.run(cmd,check=True,stdout=log,stderr=subprocess.STDOUT)
        p=subprocess.run([str(d/'obj/Vnetlist6502_fixture')],capture_output=True,text=True,timeout=60)
        text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        if why:assert p.returncode and why in text,(name,text)
        else:assert p.returncode==0 and 'NETLIST6502 PASS'in text,text
        results.append(dict(case=name,returncode=p.returncode,expected_failure=why))
    guards=[]
    for name,before,after in [('extra_state',b'initial q =',b'reg hidden;\r\n    initial q ='),
                              ('changed_relaxation',b'q <= no;',b'q <= ni;')]:
        raw=gen.SOURCE.read_bytes();assert before in raw
        try:gen.generate(raw.replace(before,after,1))
        except ValueError:guards.append(dict(case=name,rejected=True))
        else:raise AssertionError('changed vendor source accepted:'+name)
    for n in ('generate_6502.py','test_6502.py','test_6502.cpp'):(out/n).write_bytes((HERE/n).read_bytes())
    paths=[HERE/n for n in ('generate_6502.py','test_6502.py','test_6502.cpp')]+[gen.SOURCE]
    (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(status='PASS',manifest=m,cases=results,source_controls=guards,
        source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
