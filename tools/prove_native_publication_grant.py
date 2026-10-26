#!/usr/bin/env python3
"""Prove the source-bound native grant factoring and retain its real negative.

Exact reversible edits bind the theorem to the entire caller and backplane.
Only the publication enable changes; all publication RHSs and RAM consumers
are unchanged. Prove its 1,230 owner/before bits for arbitrary current/next
data, and exhaust four-state shell controls using Verilog procedural-if rules.
An omitted Ready condition must publish different data in the negative case.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from stage_native_publication_grant import CALLER, BACKPLANE, binding, caller, backplane
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TOP = '''module grant_contract #(parameter EXTERNAL_READY_GRANT=1, BROKEN=0)(
 input wire reset,pack_ready,storage_ready,control_ready,
 input wire [1229:0] current_data,next_data,
 output wire old_commit,new_commit,output wire safe,
 output reg [1229:0] old_data,new_data);
 wire original_advance=ROOT;
 wire contb_advance=BROKEN ? (~reset && pack_ready && storage_ready) : original_advance;
 assign old_commit=OLD;
 assign new_commit=NEW;
 always @*begin
  old_data=current_data;new_data=current_data;
  if(old_commit)old_data=next_data;
  if(new_commit)new_data=next_data;
 end
 assign safe=(old_commit==new_commit) && (old_data==new_data);
endmodule
'''
GENERIC = '''module generic_contract(
 input wire contb_advance,control_ready,input wire [1229:0] current_data,next_data,
 output wire old_commit,new_commit,output reg [1229:0] old_data,new_data);
 localparam EXTERNAL_READY_GRANT=0;
 assign old_commit=OLD;
 assign new_commit=NEW;
 always @*begin
  old_data=current_data;new_data=current_data;
  if(old_commit)old_data=next_data;
  if(new_commit)new_data=next_data;
 end
endmodule
'''
TB = '''module tb;
 reg reset,pack_ready,storage_ready,control_ready;
 reg [1229:0] current_data,next_data;
 wire old_commit,new_commit,safe;wire [1229:0] old_data,new_data;
 grant_contract dut(.*);
 wire generic_old,generic_new;wire [1229:0] generic_before,generic_after;
 // Independently free standalone advance, including Z and advance=1 while
 // Ready=0. This caller has no external readiness-qualified grant.
 generic_contract generic_dut(
  pack_ready,control_ready,current_data,next_data,
  generic_old,generic_new,generic_before,generic_after);
 wire bad_old,bad_new;wire [1229:0] bad_before,bad_after;
 grant_contract #(.BROKEN(1)) negative_dut(
  reset,pack_ready,storage_ready,control_ready,current_data,next_data,
  bad_old,bad_new,,bad_before,bad_after);
 function automatic logic four(input integer n);
  case(n) 0:four=0;1:four=1;2:four=1'bx;3:four=1'bz;endcase
 endfunction
 integer checks=0;
 initial begin
  for(integer pattern=0;pattern<5;pattern++)begin
   case(pattern)
    0:begin current_data=0;next_data='1;end
    1:begin current_data='1;next_data=0;end
    2:begin current_data='x;next_data='z;end
    3:begin current_data='z;next_data='x;end
    4:begin current_data={615{2'b10}};next_data={615{2'b01}};end
   endcase
   for(integer n=0;n<256;n++)begin
    reset=four(n%4);pack_ready=four((n/4)%4);
    storage_ready=four((n/16)%4);control_ready=four((n/64)%4);#1;
    if(old_commit!==new_commit || old_data!==new_data)
     $fatal(1,"native grant mismatch controls=%0d pattern=%0d",n,pattern);
    if(generic_old!==generic_new || generic_before!==generic_after)
     $fatal(1,"generic independent guard mismatch");
    checks++;
   end
  end
  reset=0;pack_ready=1;storage_ready=1;control_ready=0;
  current_data=0;next_data='1;#1;
  if(bad_old!==1'b0 || bad_new!==1'b1 || bad_before!==0 || bad_after!=={1230{1'b1}})
   $fatal(1,"missing Ready negative did not publish wrong owner data");
  $display("PUBLICATION_GRANT_PASS four_state_cases=%0d generic_cases=%0d owner_bits=1230 negative_wrong_data=1",checks,checks);
  $finish;
 end
endmodule
'''


def run(stage, out, yosys):
    assert not out.exists()
    m = json.loads((stage/'r-front-control-native.json').read_text())
    verify(stage, m['source_hashes'])
    assert m['shared_publication_grant_staged'] and not m['assembly_permitted']
    parent = Path(m['parent_stage'])
    p = json.loads((parent/'r-front-control-native.json').read_text())
    verify(parent, p['source_hashes'])
    assert sha(parent/'r-front-control-native.json') == m['parent_manifest_sha256']
    assert (stage/CALLER).read_text() == caller((parent/CALLER).read_text())
    assert (stage/BACKPLANE).read_text() == backplane((parent/BACKPLANE).read_text())
    changes = {n for n,h in p['source_hashes'].items() if Path(n).suffix in ['.v','.sv'] and m['source_hashes'][n] != h}
    assert changes == {CALLER, BACKPLANE}
    bound = binding(stage)
    old = (parent/CALLER).read_text()
    new = (stage/CALLER).read_text()
    old_gate = re.findall(r'assign owner_commit=([^;]+);', old)
    new_gate = re.findall(r'assign owner_commit=([^;]+);', new)
    root = re.findall(r'wire machine_enable = ([^;]+);', (stage/'core/Dorado.sv').read_text())[0]
    assert len(old_gate) == len(new_gate) == 1
    # Parent's local always guard equals the old owner gate. Every RHS and
    # all owner_commit consumers are byte-identical under reversible edits.
    assert 'if('+old_gate[0]+')begin' in old
    top = TOP.replace('ROOT', root).replace('OLD', old_gate[0].replace('contb_advance', 'original_advance')).replace('NEW', new_gate[0])
    top += GENERIC.replace('OLD',old_gate[0]).replace('NEW',new_gate[0])
    out.mkdir(parents=True)
    (out/'contract.sv').write_text(top)
    (out/'tb.sv').write_text(TB)
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    shutil.copy2(Path(__file__).with_name('stage_native_publication_grant.py'), out/'stage_native_publication_grant.py')
    state = dict(status='RUNNING', started=time.time(), stage=str(stage),
                 stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                 parent_manifest_sha256=sha(parent/'r-front-control-native.json'),
                 source_binding=bound, exact_changed_HDL=sorted(changes),
                 arbitrary_owner_bits=1230, assumptions=0, actions=[])
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    def command(label, cmd, env=None, marker=None):
        with (out/(label+'.log')).open('x') as log:
            child = subprocess.Popen(cmd, stdout=log, stderr=log, env=env)
            action = dict(label=label, pid=child.pid, command=cmd, started=time.time())
            state['actions'].append(action);save()
            code = child.wait()
        raw = (out/(label+'.log')).read_text()
        action.update(returncode=code, finished=time.time(), log_sha256=sha(out/(label+'.log')))
        save()
        assert code == 0 and (marker is None or marker in raw), (label, raw[-1800:])
    save()
    command('four-state-compile', ['iverilog','-g2012','-s','tb','-o',str(out/'test.vvp'),str(out/'contract.sv'),str(out/'tb.sv')])
    command('four-state-test', ['vvp',str(out/'test.vvp')], marker='PUBLICATION_GRANT_PASS four_state_cases=1280 generic_cases=1280 owner_bits=1230 negative_wrong_data=1')
    for label, broken in [('binary-proof',False),('missing-ready-negative',True)]:
        d = out/label;d.mkdir()
        (d/'contract.sv').write_text(top)
        script = 'read_verilog -sv /work/contract.sv\n'
        if broken:
            script += 'chparam -set BROKEN 1 grant_contract\n'
        script += 'prep -top grant_contract\nflatten\nopt\ncheck -assert\n'
        script += ('sat -prove safe 1 -falsify -show-ports -dump_json /work/counterexample.json\n' if broken else
                   'sat -verify -prove safe 1 -show-ports\n')
        (d/'proof.ys').write_text(script)
        env = os.environ.copy();env['YOWASP_MOUNT']='/work='+str(d)
        env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache')
        command(label, [str(yosys),'-Q','-T','-s','/work/proof.ys'], env,
                'model found: FAIL!' if broken else 'no model found: SUCCESS!')
    state.update(status='PASS_NATIVE_SHARED_PUBLICATION_GRANT', finished=time.time(),
                 four_state_native_cases=1280, four_state_generic_cases=1280,
                 negative_publishes_wrong_owner_data=True,
                 all_publication_RHS_and_RAM_equations_unchanged=True,
                 physical_timing_or_firmware_or_hardware_speed_qualified=False,
                 files={str(q.relative_to(out)):sha(q) for q in out.rglob('*')
                        if q.is_file() and q.name not in ['run-state.json','test.vvp']})
    save();print(state['status'], flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:
        p.add_argument('--'+n, type=Path, required=True)
    a=p.parse_args()
    run(a.stage.resolve(), a.output.resolve(), a.yosys.resolve())
