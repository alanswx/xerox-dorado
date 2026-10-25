#!/usr/bin/env python3
"""Check every cone result after the exact full-native Quartus lowering."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from fit_r_front import quartus_cone
from test_external_r_packet import sha


def renamed(raw,prefix):
    names=re.findall(r'\bmodule (\w+)',raw)
    for name in sorted(names,key=len,reverse=True):raw=re.sub(r'\b'+name+r'\b',prefix+name,raw)
    return raw


def run(native,stage,out):
    assert not out.exists();out.mkdir(parents=True)
    path='src/verilog/rtl/dorado_r_front_cone.sv'
    original=(native/'inputs'/path).read_text();lowered=(stage/path).read_text()
    assert lowered==quartus_cone(original)
    header=original.split('module dorado_r_front_cone(',1)[1].split(');',1)[0]
    ports=[];direction=None;width=1
    for token in header.replace('\n',' ').split(','):
        m=re.fullmatch(r'\s*(?:(input|output)\s+wire\s*(?:\[(\d+):0\])?\s*)?(\w+)\s*',token);assert m,token
        if m[1]:direction=m[1];width=int(m[2])+1 if m[2]else 1
        ports.append((direction,m[3],width))
    assert [(n,w)for d,n,w in ports if d=='input']==[('state_q',736),('base',1),('pins',236)]
    outputs=[(n,w)for d,n,w in ports if d=='output'];total=sum(w for n,w in outputs)
    tb='''`timescale 1ns/1ps
module tb_lowering;
 reg [735:0] state_q=0;reg base=0;reg [235:0] pins=0;
 reg [31:0] rng=32'h43504e15;integer checks=0;
'''
    for prefix in ['old_','new_']:
        for name,width in outputs:tb+=f' wire [{width-1}:0] {prefix+name};\n'
        tb+=' '+prefix+'dorado_r_front_cone dut_'+prefix+'(.state_q(state_q),.base(base),.pins(pins),'
        tb+=','.join('.'+n+'('+prefix+n+')'for n,w in outputs)+');\n'
    a='{'+','.join('old_'+n for n,w in outputs)+'}';b='{'+','.join('new_'+n for n,w in outputs)+'}'
    tb+=''' initial begin
  for(integer i=0;i<1024;i++)begin
   for(integer k=0;k<973;k++)begin
    rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;
    if(k<736)state_q[k]=rng[0];else if(k<972)pins[k-736]=rng[0];else base=rng[0];
   end
   #1;
'''+f'''   if($isunknown({a}) || $isunknown({b}) || {a}!=={b})$fatal(1,"QUARTUS_LOWERING mismatch case=%0d",i);
   checks++;
  end
  $display("NATIVE_QUARTUS_CONE_PASS cases=%0d input_bits=973 output_bits={total}",checks);$finish;
 end
endmodule
'''
    (out/'old.sv').write_text(renamed(original,'old_'));(out/'new.sv').write_text(renamed(lowered,'new_'));(out/'tb.sv').write_text(tb)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),original_sha256=sha(native/'inputs'/path),
               lowered_sha256=sha(stage/path),output_bits=total,actions=[],files={p.name:sha(p)for p in out.iterdir()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save();code=1
    for label,command in [('compile',['iverilog','-g2012','-s','tb_lowering','-o',str(out/'test.vvp'),str(out/'old.sv'),str(out/'new.sv'),str(out/'tb.sv')]),('test',['vvp',str(out/'test.vvp')])]:
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log);row=dict(label=label,command=command,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,log_sha256=sha(out/(label+'.log')),finished=time.time());save()
        if code:break
    state.update(status='PASS_NATIVE_QUARTUS_CONE_LOWERING'if code==0 else'TERMINAL_QUARTUS_CONE_LOWERING_FAILED',returncode=code,finished=time.time())
    save();print(state['status']);return code


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['native','stage','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.native.resolve(),a.stage.resolve(),a.output.resolve()))
