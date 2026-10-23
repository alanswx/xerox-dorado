#!/usr/bin/env python3
"""Check a decoded MC/MU10164 mux against the qualified native cells.

The rejected native path crosses several data muxes after the ALU. Decode
their controls before the late data arrives, preserving the package pins.
This is a component experiment, not a change to the canonical cells or core.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def sha(data):
    return hashlib.sha256(data).hexdigest()


def decoded(raw):
    old = b'  assign p15 = (~p2) & d[sel];'
    assert raw.count(old) == 1
    return raw.replace(old, b'''  // Predecode stable controls; late data sees an AND/reduction network.
  (* keep *) wire [7:0] selected;
  genvar line;
  generate for(line=0;line<8;line=line+1) begin: decode_line
    assign selected[line]=(~p2) & (sel==line);
  end endgenerate
  wire selected_data=|(d & selected);
  // An unknown array index returns X even when all data inputs are zero.
  // Self equality is one for binary controls and X for an unknown control,
  // retaining that Verilog behavior without changing binary hardware logic.
  assign p15=(sel==sel) ? selected_data : ((~p2) & 1'bx);''')


def bench():
    return r'''`timescale 1ns/1ps
module tb;
reg [7:0] d;
reg [2:0] s;
reg e;
wire a,b,c,f;
cell_MC10164 dut(.p6(d[0]),.p5(d[1]),.p4(d[2]),.p3(d[3]),
 .p11(d[4]),.p12(d[5]),.p13(d[6]),.p14(d[7]),
 .p7(s[0]),.p9(s[1]),.p10(s[2]),.p2(e),.p15(a));
reference_MC10164 refa(.p6(d[0]),.p5(d[1]),.p4(d[2]),.p3(d[3]),
 .p11(d[4]),.p12(d[5]),.p13(d[6]),.p14(d[7]),
 .p7(s[0]),.p9(s[1]),.p10(s[2]),.p2(e),.p15(b));
cell_MU10164 alias_dut(.p6(d[0]),.p5(d[1]),.p4(d[2]),.p3(d[3]),
 .p11(d[4]),.p12(d[5]),.p13(d[6]),.p14(d[7]),
 .p7(s[0]),.p9(s[1]),.p10(s[2]),.p2(e),.p15(c),.p1(1'b0),.p8(1'b0),.p16(1'b0));
reference_MU10164 refb(.p6(d[0]),.p5(d[1]),.p4(d[2]),.p3(d[3]),
 .p11(d[4]),.p12(d[5]),.p13(d[6]),.p14(d[7]),
 .p7(s[0]),.p9(s[1]),.p10(s[2]),.p2(e),.p15(f),.p1(1'b0),.p8(1'b0),.p16(1'b0));
integer i,j,k,n,count=0;
reg [23:0] pattern;
reg expected;
function four(input [1:0] v);
 case(v) 0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
endfunction
task check;
 begin
 #1;
 expected=(~e)&d[s];
 if(a!==b || c!==f || a!==expected || c!==expected)
  $fatal(1,"MC10164_DECODED differs d=%b s=%b e=%b candidate=%b/%b reference=%b/%b expected=%b",d,s,e,a,c,b,f,expected);
 count=count+1;
 end
endtask
initial begin
 for(i=0;i<4096;i=i+1)begin {e,s,d}=i[11:0];check;end
 $display("MC10164_DECODED PASS binary=4096 aliases=2");
 if($test$plusargs("four_state"))begin
  // All 4^12 input combinations, including disabled and unknown indexes.
  for(j=0;j<16777216;j=j+1)begin
   pattern=j[23:0];
   for(k=0;k<8;k=k+1)d[k]=four(pattern[2*k+:2]);
   for(k=0;k<3;k=k+1)s[k]=four(pattern[16+2*k+:2]);
   e=four(pattern[22+:2]);check;
  end
  $display("MC10164_DECODED PASS four_state=16777216 aliases=2");
 end
 $finish;
end
endmodule
'''


def run(parent, out):
    assert not out.exists()
    recipe = json.loads((parent / 'timing-paths-native.json').read_text())
    bodies = {}
    for cell in ['MC10164', 'MU10164']:
        name = 'src/verilog/cells/cell_' + cell + '.v'
        raw = (parent / name).read_bytes()
        assert sha(raw) == recipe['source_hashes'][name]
        bodies[cell] = raw
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    inputs.mkdir()
    for cell, raw in bodies.items():
        (inputs / ('reference_' + cell + '.v')).write_bytes(raw.replace(
            ('module cell_' + cell + ' (').encode(), ('module reference_' + cell + ' (').encode()))
        (inputs / ('cell_' + cell + '.v')).write_bytes(decoded(raw))
    (inputs / 'tb.sv').write_text(bench())
    (inputs / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    frozen = {p.name: sha(p.read_bytes()) for p in inputs.iterdir()}
    (out / 'provenance.json').write_text(json.dumps(dict(scope=__doc__,
        parent_manifest_sha256=sha((parent/'timing-paths-native.json').read_bytes()),
        original_hashes={k: sha(v) for k,v in bodies.items()}, inputs=frozen,
        binary_cases=4096, four_state_cases=16777216, aliases=2,
        registers_changed=False, native_qualified=False), indent=2)+'\n')
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), actions=[], results={})

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    def execute(cmd, label):
        with (out/(label+'.log')).open('x') as log:
            p = subprocess.Popen(list(map(str,cmd)), stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, pid=p.pid, command=list(map(str,cmd)), started=time.time())
            state['actions'].append(row);save()
            row.update(returncode=p.wait(), finished=time.time());save()
        return row['returncode'], (out/(label+'.log')).read_text()

    def case(engine, label, candidate, negative=False, four_state=False):
        label=engine+'-'+label
        sources=[candidate,inputs/'cell_MU10164.v',inputs/'reference_MC10164.v',
                 inputs/'reference_MU10164.v',inputs/'tb.sv']
        if engine=='icarus':
            exe=out/(label+'.vvp')
            cmd=['iverilog','-g2012','-s','tb','-o',exe,*sources]
            launch=['vvp',exe]+(['+four_state'] if four_state else [])
        else:
            obj=out/('obj-'+label)
            cmd=['verilator','--binary','--timing','-j','2','-Wno-fatal',
                 '-Werror-UNOPTFLAT','--top-module','tb','--Mdir',obj,'-o','test-mux',*sources]
            launch=[obj/'test-mux']
        rc,log=execute(cmd,label+'-compile')
        assert rc==0 and 'UNOPTFLAT' not in log,(label,log[-2000:])
        rc,log=execute(launch,label+'-test')
        if negative:
            assert rc!=0 and 'MC10164_DECODED differs' in log,(label,log)
            state['results'][label]=dict(rejected=True,diagnostic=log)
        else:
            assert rc==0 and 'PASS binary=4096' in log,(label,log)
            if four_state:assert 'PASS four_state=16777216' in log,(label,log)
            state['results'][label]=dict(passed=True,output=log)
        save();print(label,'rejected' if negative else 'PASS',flush=True)

    save()
    try:
        for engine in ['verilator','icarus']:
            case(engine,'positive',inputs/'cell_MC10164.v',four_state=engine=='icarus')
            body=(inputs/'cell_MC10164.v').read_bytes()
            for label,old,new in [
                ('wrong-select',b'(sel==line)',b'(sel==(7-line))'),
                ('lost-enable',b'(~p2) & (sel==line)',b'(sel==line)'),
                ('unknown-index',b'(sel==sel) ? selected_data : ((~p2) & 1\'bx)',b'selected_data')]:
                if label=='unknown-index' and engine!='icarus':continue
                assert body.count(old)==1
                bad=out/(label+'.v');bad.write_bytes(body.replace(old,new))
                case(engine,label,bad,negative=True,four_state=label=='unknown-index')
        assert all(sha((inputs/n).read_bytes())==h for n,h in frozen.items())
        state.update(status='PASS_MC10164_DECODED_BINARY_AND_FOUR_STATE',finished=time.time(),
                     inputs_unchanged=True,native_qualified=False)
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
    return state


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--parent',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    print(run(a.parent.resolve(),a.output.resolve())['status'])
