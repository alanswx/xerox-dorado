#!/usr/bin/env python3
"""Replay the actual failing RBase edge from its checked full-processor prefix.

Restore the separately recorded physical reference Qs and closed owner Qs at
transaction760, plus its real SRAM and source context. Run original event761
with all existing per-event assertions. Optional ordinary-FF delay changes are
diagnostics, not a part specification or a qualification of another profile.
Never change original gates, RAM behavior, processor transfer or assertions.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from record_cp_source_program import main_program
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def bench(raw,out,owner,physical,memory,pins):
    start,end,original=main_program(raw)
    program=''' initial begin
  monitor=0;advance=0;cp_reset=1;load=0;root=0;
  rp=401'hPINS;pa=rp;p=rp;fixture=844'hPHYSICAL;
  fixture_memory=9216'hMEMORY;
  for(integer i=0;i<256;i++)begin
   owner.files.rm_h[i]={fixture_memory[4096+i],fixture_memory[1024+4*i+:4],fixture_memory[4*i+:4]};
   owner.files.stk_h[i]={fixture_memory[4352+i],fixture_memory[3072+4*i+:4],fixture_memory[2048+4*i+:4]};
   owner.files.rm_l[i]={fixture_memory[8704+i],fixture_memory[5632+4*i+:4],fixture_memory[4608+4*i+:4]};
   owner.files.stk_l[i]={fixture_memory[8960+i],fixture_memory[7680+4*i+:4],fixture_memory[6656+4*i+:4]};
  end
  owner.r_front_lower_q=844'hCLOSED;
  owner.r_front_upper_q=(844'hCLOSED)>>588;
  visible_h=(844'hCLOSED)>>572;visible_l=(844'hCLOSED)>>580;
  owner.r_front_before=rp;owner.r_front_clock_before=0;
  #100;host_clk=1;load=1;#100;host_clk=0;load=0;#100;
  load=1;#100;load=0;#100;
  if(reference!==fixture)begin
   for(integer bit_no=0;bit_no<844;bit_no++)if(reference[bit_no]!==fixture[bit_no])
    $display("RBASE_RESTORE_DIFF bit=%0d expected=%b actual=%b",bit_no,fixture[bit_no],reference[bit_no]);
   $fatal(1,"RBASE_RESTORE physical reference mismatch");
  end
  if(stored_q!==844'hCLOSED)$fatal(1,"RBASE_RESTORE closed owner mismatch");
  if(memory_image()!==fixture_memory || reference_memory!==fixture_memory)$fatal(1,"RBASE_RESTORE SRAM mismatch");
  cp_reset=0;#100;checks=761;monitor=1;
  for(integer bit_no=0;bit_no<8;bit_no++)data_changed[bit_no]=$realtime;
  data_valid=8'hff;clock_valid=0;
  $display("RBASE_RESTORE_PASS physical_Q=844 closed_Q=844 RAM=9216 before_root=0");
  event_step(1,p);
  $display("RBASE_SINGLE_ORIGINAL_EVENT_PASS check=761");$finish;
 end'''
    for a,b in [('PINS',f'{pins:0101x}'),('PHYSICAL',f'{physical:0211x}'),('MEMORY',f'{memory:02304x}'),('CLOSED',f'{owner:0211x}')]:
        program=program.replace(a,b)
    actual=raw[:start]+program+raw[end:]
    assert actual.replace(program,original,1)==raw
    for path in re.findall(r'\$fopen\("([^\"]+)"',actual):
        actual=actual.replace('$fopen("'+path+'"','$fopen("'+str(out/Path(path).name)+'"',1)
    return actual


def run(source,out,ff_ps,phase_ps=1250):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='TERMINAL_FULL_PROCESSOR_CP_SOURCE_NOT_ESTABLISHED'
    log=(source/'test.log').read_text()
    assert 'R front resolved source mismatch check=761'in log
    row=re.search(r'ORIGINAL_CP_CROSSING check=760 .*expected_Q=([0-9a-f]+) reference_Q=([0-9a-f]+)',log)
    assert row
    closed=int(row[1],16);physical=int(row[2],16)
    records=(source/'transactions.hex').read_text().splitlines();assert len(records)==761
    v=int(records[-1],16);memory=v&((1<<9216)-1);v>>=9216
    result=v&((1<<2470)-1);v>>=2470;pins=v&((1<<401)-1)
    assert result>>1626==closed
    out.mkdir(parents=True);paths=[];rows=[]
    for r in s['source_files']:
        name=r['path'];src=source/name;assert sha(src)==r['output_sha256']
        text=src.read_text()
        if name=='inputs/tb_r_front_local.sv':text=bench(text,out,closed,physical,memory,pins)
        elif (ff_ps!=400 or phase_ps!=1250) and name.startswith('timed/'):
            if name=='timed/run_control_original_ff.sv':
                phase=module(text,'original_SE10231');rest=text.replace(phase,'PHASE_SENTINEL',1)
                rest=rest.replace('<=#(400)','<=#('+str(ff_ps)+')')
                phase=phase.replace('<=#(1250)','<=#('+str(phase_ps)+')')
                text=rest.replace('PHASE_SENTINEL',phase,1)
            else:
                text=text.replace('<=#(400)','<=#('+str(ff_ps)+')').replace('DELAY_PS=400','DELAY_PS='+str(ff_ps))
        p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text);paths.append(p)
        rows.append(dict(path=name,parent_sha256=sha(src),output_sha256=sha(p)))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,source=str(source),source_state_sha256=sha(source/'run-state.json'),
               source_log_sha256=sha(source/'test.log'),source_trace_sha256=sha(source/'transactions.hex'),
               FF_delay_ps=ff_ps,phase_delay_ps=phase_ps,gate_delay_unchanged_ps=1750,
               closed_owner=f'{closed:0211x}',physical_reference=f'{physical:0211x}',before_pins=f'{pins:0101x}',
               source_files=rows,files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},
               actions=[],started=time.time(),full_program_qualified=False,native_integrated=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save();code=1
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_r_front_local','-o',str(out/'test.vvp'),*map(str,paths)]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with (out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            r=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(r);save();code=child.wait()
        r.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else''
    state.update(status='OBSERVED_RBASE_SINGLE_EDGE_RESULT',returncode=code,
                 restored_original_fixture='RBASE_RESTORE_PASS'in text,
                 original_single_event_passed='RBASE_SINGLE_ORIGINAL_EVENT_PASS'in text,
                 reproduced_source_mismatch='R front resolved source mismatch check=761'in text,finished=time.time())
    save();print(json.dumps({k:state[k]for k in ['status','FF_delay_ps','restored_original_fixture','original_single_event_passed','reproduced_source_mismatch']}),flush=True)
    return 0 if state['restored_original_fixture']else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--ff-ps',type=int,default=400)
    p.add_argument('--phase-ps',type=int,default=1250)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.ff_ps,a.phase_ps))
