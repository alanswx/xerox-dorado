#!/usr/bin/env python3
"""Check four-state fallback of the stateless, fresh CP-prefix function.

Exercise X and Z in every955-input bit at both original5/24-wave parameters.
Keep every475 result bit and require the ordered path for unknown inputs.
The deliberately broken unknown guard must reject with no assertion changes.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from test_r_front_forward_view import sha

TB='''`timescale 1ns/1ps
module tb_pure_prefix;
 reg [954:0] stimulus,base;
 wire [155:0] q;wire before_root,after_root,refresh;
 wire [397:0] before_pins,after_pins;
 assign {q,before_root,after_root,before_pins,after_pins,refresh}=stimulus;
 wire [474:0] actual[0:1],expected[0:1];wire [1:0] direct;
 wire split=(before_root!=after_root)&&(before_pins[292:282]!=after_pins[292:282]);
 wire [10:0] prefix_cp=split ? after_pins[292:282] : before_pins[292:282];
 for(genvar w=0;w<2;w++)begin: modes
  wire [268:0] prefix;
  dorado_control_hold_ordered_step #(.WAVES(w==0?5:24)) reference_step(
   q,before_root,after_root,before_pins,after_pins,
   expected[w][474:319],expected[w][318:263],expected[w][262:207],expected[w][206],refresh,
   expected[w][205:112],expected[w][111:56],expected[w][55:0],1'b0,prefix,prefix_cp,after_pins[292:282],prefix);
  dorado_control_pure_prefix_step #(.WAVES(w==0?5:24)) actual_step(
   q,before_root,after_root,before_pins,after_pins,refresh,actual[w],direct[w]);
 end
 integer known=0,unknown_checks=0;
 initial begin
  stimulus={156'hQ_VALUE,1'b1,1'b0,398'hPIN_VALUE,398'hPIN_VALUE,1'b1};#2;
  for(integer w=0;w<2;w++)begin
   if(actual[w]!==expected[w] || direct[w]!==1'b1)$fatal(1,"PURE_PREFIX binary witness w=%0d",w);
   known=known+1;
  end
  base=stimulus;
  for(integer i=0;i<955;i++)for(integer kind=0;kind<2;kind++)begin
   stimulus=base;stimulus[i]=kind ? 1'bz:1'bx;#2;
   for(integer w=0;w<2;w++)begin
    if(direct[w]!==1'b0 || actual[w]!==expected[w])
     $fatal(1,"PURE_PREFIX unknown fallback bit=%0d kind=%0d waves=%0d selected=%b",i,kind,w==0?5:24,direct[w]);
    unknown_checks=unknown_checks+1;
   end
  end
  $display("PURE_PREFIX_FOUR_STATE_PASS known=%0d unknown=%0d inputs=955 outputs=475 waves=5/24",known,unknown_checks);
  $finish;
 end
endmodule
'''


def run(proof,witness,out,bad):
    assert not out.exists()
    state=json.loads((proof/'run-state.json').read_text())
    for name,h in state['files'].items():assert sha(proof/name)==h
    values={n:int(b,2) for n,b in re.findall(r'\\(q|pins)\s+--\s+--\s+([01]+)',witness.read_text())}
    assert set(values)=={'q','pins'}
    pure=(proof/'pure.sv').read_text()
    if bad:
        assert pure.count('default:using_direct=0;')==1;pure=pure.replace('default:using_direct=0;','default:using_direct=1;',1)
    out.mkdir(parents=True)
    (out/'control.sv').write_bytes((proof/'control.sv').read_bytes());(out/'pure.sv').write_text(pure)
    (out/'tb.sv').write_text(TB.replace('Q_VALUE',format(values['q'],'x')).replace('PIN_VALUE',format(values['pins'],'x')))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,proof=str(proof),
        pure_source_sha256=sha(proof/'pure.sv'),witness_sha256=sha(witness),bad_unknown=bad,
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()},actions=[],hardware_changed=False,native_integrated=False)

    def save():(out/'run-state.json').write_text(json.dumps(result,indent=2)+'\n')

    save()
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_pure_prefix','-o',str(out/'test.vvp'),*map(str,[out/'control.sv',out/'pure.sv',out/'tb.sv'])]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,command=cmd,pid=p.pid,started=time.time());result['actions'].append(row);save();code=p.wait()
            row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    compiled=result['actions'][0]['returncode']==0
    passed=compiled and ((code!=0 and 'PURE_PREFIX unknown fallback' in log) if bad else
        (code==0 and 'PURE_PREFIX_FOUR_STATE_PASS known=2 unknown=3820 inputs=955 outputs=475 waves=5/24' in log))
    result.update(status=('PASS_PURE_PREFIX_UNKNOWN_COUNTEREXAMPLE' if bad else 'PASS_PURE_PREFIX_FULL_FOUR_STATE') if passed else
        'TERMINAL_PURE_PREFIX_FOUR_STATE_NOT_ESTABLISHED',finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(result['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['proof','witness','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--bad-unknown',action='store_true');a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.witness.resolve(),a.output.resolve(),a.bad_unknown))
