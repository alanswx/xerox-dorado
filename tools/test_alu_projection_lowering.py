#!/usr/bin/env python3
"""Compare every read-only ALU output before and after native syntax lowering.

Also replay the complete1198-state/80-flag composition with only the projection
source replaced. No original primitive, input record or comparison changes.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from lower_processor_alu_projection import generate,ports
from test_native_quartus_cone import renamed
from test_external_r_packet import sha
from review_processor_branch_composition import checked

TOP='alu_projection_processor_alu_reference'


def run(source,out,engine,native_capture=False,mutation=None):
    assert not out.exists();out.mkdir(parents=True)
    parent=checked(source)
    assert parent['status']=='PASS_ORIGINAL_ALU_PROCESSOR_COMPOSITION'
    assert parent['optimized_flag_transfer_checked']and parent['reference']['physical_bits']==1198
    generate(source/'projection',out/'lowered')
    old=(source/'projection/projection.sv').read_text()
    new=(out/'lowered/projection.sv').read_text()
    header=re.search(r'module '+TOP+r'\((.*?)\);',old,re.S)[1]
    items=ports(header);inputs=[(n,w)for d,n,w in items if d=='input'];outputs=[(n,w)for d,n,w in items if d=='output']
    total_in=sum(w for n,w in inputs);total_out=sum(w for n,w in outputs)
    assert total_in==1632
    tb='''`timescale 1ns/1ps
module tb_projection_lowering;
 reg [31:0] rng=32'h11981632;integer cases=0;
'''
    for n,w in inputs:tb+=f' reg [{w-1}:0] {n}=0;\n'
    for prefix in ['old_','new_']:
        for n,w in outputs:tb+=f' wire [{w-1}:0] {prefix+n};\n'
        tb+=' '+prefix+TOP+' '+prefix+'view('+','.join('.'+n+'('+n+')'for n,w in inputs)+','
        tb+=','.join('.'+n+'('+prefix+n+')'for n,w in outputs)+');\n'
    a='{'+','.join('old_'+n for n,w in outputs)+'}'
    b='{'+','.join('new_'+n for n,w in outputs)+'}'
    tb+=' initial begin\n for(integer i=0;i<1024;i++)begin\n'
    for n,w in inputs:
        tb+=f'  for(integer bit_no=0;bit_no<{w};bit_no++)begin\n'
        tb+=f'   rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;{n}[bit_no]=rng[0];\n  end\n'
    tb+=f'''  #1;
  if((^{a})===1'bx || (^{b})===1'bx || {a}!=={b})
   $fatal(1,"ALU_PROJECTION_LOWERING mismatch case=%0d",i);
  cases++;
 end
 $display("ALU_PROJECTION_LOWERING_PASS cases=%0d input_bits={total_in} output_bits={total_out}",cases);$finish;
 end
endmodule
'''
    (out/'old.sv').write_text(renamed(old,'old_'));(out/'new.sv').write_text(renamed(new,'new_'))
    (out/'tb.sv').write_text(tb)
    # Only the read-only projection is substituted in the *same* program.
    replay=out/'replay';shutil.copytree(source,replay,ignore=shutil.ignore_patterns('obj','test.vvp','*.log','run-state.json'))
    target=replay/'projection/projection.sv';target.write_text(new)
    original_tb=(source/'tb.sv').read_text();replay_tb=original_tb.replace(str(source),str(replay))
    if native_capture:
        # Keep the candidate's own prior flag state in the physical frame;
        # never refresh it from the original oracle at each transaction.
        replay_tb,count=re.subn(r' alu_projection_processor_alu_reference early_view\(.*?\);\n','',replay_tb,count=1,flags=re.S)
        assert count==1
        before=''' dorado_branch_condition_frame_step candidate(tq,before_root,full.original.processor_roots[1],
  before_pins,projected_pins,full.branch_pins,next_q,events,unsettled);'''
        after=''' dorado_processor_flag_capture candidate(
  .physical_before({projection_q[1197:924],tq,projection_q[843:0]}),
  .producer_pins_after(pins),.source_clock_before(projection_root),
  .flag_clock_before(before_root),.flag_clock_after(full.original.processor_roots[1]),
  .flag_pins_before(before_pins),.flag_pins_after(full.branch_pins),
  .flags_next(next_q),.clock_events(events),.unsettled(unsettled),.flag_pins_early(projected_pins));'''
        assert replay_tb.count(before)==1;replay_tb=replay_tb.replace(before,after,1)
        if mutation=='late-source-root':
            marker='.source_clock_before(projection_root)'
            assert replay_tb.count(marker)==1
            replay_tb=replay_tb.replace(marker,'.source_clock_before(root)',1)
        rtl=Path(__file__).resolve().parents[1]/'verilog/rtl/dorado_processor_flag_capture.sv'
        shutil.copy2(rtl,replay/rtl.name)
        frame=rtl.with_name('dorado_branch_condition_frame_step.sv')
        normalize=lambda text:re.sub(r'\s+','',re.sub(r'/\*.*?\*/','',text,flags=re.S))
        assert normalize(frame.read_text())==normalize((replay/'branch-step.sv').read_text())
        shutil.copy2(frame,replay/'branch-step.sv')
    (replay/'tb.sv').write_text(replay_tb)
    replay_paths=sorted((replay/'reference/timed').iterdir())
    replay_paths=[p for p in replay_paths if p.suffix in ['.sv','.v']and p.name!='processor_alu_flag_views.sv']
    replay_paths += [replay/n for n in ['machines.sv','tb.sv','reference/base/branch/cone.sv','branch-step.sv','projection/projection.sv']]
    if native_capture:replay_paths.append(replay/'dorado_processor_flag_capture.sv')
    miter_paths=[out/n for n in ['old.sv','new.sv','tb.sv']]
    helpers=out/'executed-tools';helpers.mkdir()
    for name in [Path(__file__).name,'lower_processor_alu_projection.py']:
        shutil.copy2(Path(__file__).with_name(name),helpers/name)
    state=dict(status='RUNNING',scope=__doc__,engine=engine,started=time.time(),source=str(source),
               parent_state_sha256=sha(source/'run-state.json'),input_bits=total_in,output_bits=total_out,
               native_flag_capture_interface=native_capture,
               mutation=mutation,
               source_root_and_derived_flag_clock_separate=True,
               original_reference_and_checks_unchanged=True,actions=[],
               files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save();code=1
    for label,top,paths in [('miter','tb_projection_lowering',miter_paths),('replay','tb_alu_composition',replay_paths)]:
        commands=([['iverilog','-g2012','-s',top,'-o',str(out/(label+'.vvp')),*map(str,paths)],['vvp',str(out/(label+'.vvp'))]]if engine=='icarus'else
                  [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module',top,'--Mdir',str(out/('obj_'+label)),'-o','test',*map(str,paths)],[str(out/('obj_'+label)/'test')]])
        for phase,command in zip(['compile','test'],commands):
            name=label+'-'+phase
            with(out/(name+'.log')).open('x')as log:
                child=subprocess.Popen(command,stdout=log,stderr=log)
                row=dict(label=name,command=command,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
            row.update(returncode=code,log_sha256=sha(out/(name+'.log')),finished=time.time());save()
            if code:break
        if code:break
    if not code:
        miter=(out/'miter-test.log').read_text();replayed=(out/'replay-test.log').read_text()
        assert f'ALU_PROJECTION_LOWERING_PASS cases=1024 input_bits={total_in} output_bits={total_out}'in miter
        lines=lambda text:re.findall(r'^(?:ALU_COMPOSITION_CLOCK|ALU_FUNCTION_WRITE_COVERAGE|ORIGINAL_ALU_COMPOSITION_PASS) .*$',text,re.M)
        assert lines(replayed)==lines((source/'test.log').read_text())
    status='PASS_ALU_PROJECTION_NATIVE_LOWERING'if code==0 else'TERMINAL_ALU_PROJECTION_LOWERING_FAILED'
    if mutation:
        status='NATIVE_ALU_MUTATION_NOT_REJECTED'if code==0 else'NATIVE_ALU_MUTATION_REJECTED_REVIEW_REQUIRED'
    state.update(status=status,returncode=code,finished=time.time())
    save();print(state['status']);return code


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--native-capture',action='store_true')
    p.add_argument('--mutation',choices=['late-source-root'])
    a=p.parse_args();assert not a.mutation or a.native_capture
    raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.engine,a.native_capture,a.mutation))
