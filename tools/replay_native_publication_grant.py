#!/usr/bin/env python3
"""Replay full original native programs behind the source-proved shared grant.

Preserve every reference, assertion, preparation edge and original architectural
result. Feed the native opt-in caller advance&&ready, as the real outer grant
does. Assert the original commit condition on every host edge. The full native
caller differs only by its two read-only before-state observation exports.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from stage_native_publication_grant import CALLER, caller
from test_external_r_packet import sha


def run(source, stage, proof, out, engine):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER' and s['engine']==engine
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_NATIVE_SHARED_PUBLICATION_GRANT'
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert s['buffer_helper_sha256']==m['source_hashes']['src/verilog/rtl/dorado_native_operand_buffer.sv']
    native=(stage/CALLER).read_text()
    native=native.replace(',output wire [400:0] native_before_pins,output wire native_before_root','',1)
    native=native.replace(' assign native_before_pins=r_front_before;assign native_before_root=r_front_clock_before;\n','',1)
    assert native==caller((source/'caller.sv').read_text())
    out.mkdir(parents=True);paths=[];rows=[]
    for row in s['source_files']:
        n=row['path'];assert sha(source/n)==row['output_sha256']
        raw=(source/n).read_text();text=raw
        if n=='caller.sv':text=caller(raw)
        elif n=='tb.sv':
            text=raw.replace(str(source),str(out))
            before='dorado_r_front_owned_packet dut(host_clk,advance,use_pair,'
            after='dorado_r_front_owned_packet #(.EXTERNAL_READY_GRANT(1)) dut(host_clk,advance && ready,use_pair,'
            assert text.count(before)==1;text=text.replace(before,after,1)
            observer=''' always @(posedge host_clk)begin
  if(owner_commit !== (advance && ready))$fatal(1,"SHARED_GRANT differs from original publication condition");
 end
'''
            assert text.count('endmodule')==1;text=text.replace('endmodule',observer+'endmodule',1)
            restored=text.replace(observer,'',1).replace(after,before,1).replace(str(out),str(source))
            assert restored==raw
            assert text.count('$fatal')==raw.count('$fatal')+1
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_text(text);paths.append(q)
        rows.append(dict(path=n,parent_sha256=row['output_sha256'],output_sha256=sha(q)))
    for n in s['files']:
        if n.startswith('executed-tools/'):continue
        q=out/n
        if q.exists():continue
        q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source/n,q)
    for name in [Path(__file__).name,'stage_native_publication_grant.py']:
        q=out/'executed-tools'/name;q.parent.mkdir(exist_ok=True)
        shutil.copy2(Path(__file__).with_name(name),q)
    state=dict(status='RUNNING',started=time.time(),source=str(source),engine=engine,stage=str(stage),
               source_state_sha256=sha(source/'run-state.json'),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
               grant_proof_state_sha256=sha(proof/'run-state.json'),source_files=rows,actions=[],
               original_assertions_and_reference_unchanged=True,native_cells_j19_single_SRAM_retained=True,
               files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
              [['verilator','--binary','--timing','--top-module','tb_r_front_packet','-Wno-fatal','-j','2','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log)
            action=dict(label=label,pid=child.pid,command=cmd,started=time.time())
            state['actions'].append(action);save();code=child.wait()
        action.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    metrics=re.findall(r'^OPERAND_BUFFER_PREPARATION edges=(\d+)$',log,re.M)
    same=outcomes(log)==outcomes((source/'test.log').read_text())
    covered=metrics==[str(s['operand_prepare_edges'])]
    passed=code==0 and same and covered
    state.update(status='PASS_SHARED_GRANT_COMPLETE_NATIVE_CALLER'if passed else'TERMINAL_SHARED_GRANT_REPLAY_NOT_ESTABLISHED',
                 returncode=code,finished=time.time(),original_results_match=same,
                 preparation_coverage_match=covered,outcomes=outcomes(log),
                 physical_timing_or_firmware_or_hardware_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','stage','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args()
    raise SystemExit(run(a.source.resolve(),a.stage.resolve(),a.proof.resolve(),a.output.resolve(),a.engine))
