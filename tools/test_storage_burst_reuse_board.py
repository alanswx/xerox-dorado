#!/usr/bin/env python3
"""Compare burst reuse to original144-chip MSA outputs and all9,437,184 bits.

Use the immutable preceding control's exact board/pin fixture and group source.
Control-off must reproduce its complete host/raw trace. Enabled reuse may change
host latency but not canonical operations, data, architectural outputs or arrays.
"""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(generated,board,output,controls):
    assert not output.exists(),'Inspect recorded work'
    proof=json.loads((generated/'provenance.json').read_text());assert proof['burst_read_reuse']
    for name,digest in proof['generated_hashes'].items():assert sha(generated/name)==digest
    review=json.loads((board/'review-terminal-v1.json').read_text())
    assert review['source_hashes_rechecked'] and review['control']['returncode']==0
    original=board/'control-inputs'
    hashes=json.loads((board/'control-sources.json').read_text())
    for name,digest in hashes.items():assert sha(original/name)==digest,name
    command=json.loads((board/'run-state.json').read_text())['actions'][0]['command']
    old_obj=command[command.index('--Mdir')+1]
    output.mkdir(parents=True)
    (output/'parent-review.json').write_text(json.dumps(review,indent=2)+'\n')
    (output/'parent-provenance.json').write_text(json.dumps(dict(
        scope=__doc__,generated_proof=proof,original_sources=hashes,
        original_control_trace_sha256=sha(board/'control-raw.csv')),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})
    def save():
        temp=output/'state.tmp';temp.write_text(json.dumps(state,indent=2)+'\n');os.replace(temp,output/'run-state.json')
    def execute(label,command,cwd):
        command=list(map(str,command))
        with(output/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,cwd=cwd,stdout=log,stderr=subprocess.STDOUT)
            action=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(action);save()
            action.update(returncode=p.wait(),finished=time.time());save()
        return action['returncode']
    save()
    try:
        for label,enabled,bad in [('off',0,False),('on',1,False)]+([('corrupt-reply',1,True)]if controls else[]):
            inputs=output/(label+'-inputs');shutil.copytree(original,inputs)
            for name in proof['generated_hashes']:shutil.copyfile(generated/name,inputs/name)
            fixture=inputs/'early-on-fixture.sv';text=fixture.read_text()
            old='.BURST_READ_SUCCESSOR(0)) dut(';assert text.count(old)==1
            fixture.write_text(text.replace(old,f'.BURST_READ_SUCCESSOR(0),.BURST_READ_REUSE({enabled})) dut('))
            if bad:
                path=inputs/'dorado_storage_sdram_burst.sv';text=path.read_text()
                old='rsp_data<=dq_capture;response_pending<=1;fill_waiting<=0;';assert text.count(old)==1
                path.write_text(text.replace(old,"rsp_data<=dq_capture ^ 16'h0001;response_pending<=1;fill_waiting<=0;"))
            frozen={str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()}
            (output/(label+'-sources.json')).write_text(json.dumps(dict(inputs=frozen),indent=2)+'\n')
            obj=output/('obj-'+label)
            compile_command=[v.replace(str(original),str(inputs)).replace(old_obj,str(obj))for v in command]
            assert execute(label+'-compile',compile_command,inputs)==0,label
            assert 'UNOPTFLAT'not in(output/(label+'-compile.log')).read_text(),label
            trace=output/(label+'-raw.csv')
            rc=execute(label+'-test',[obj/'test-old-word','+raw_trace='+str(trace)],inputs)
            log=(output/(label+'-test.log')).read_text()
            if bad:
                assert rc!=0 and ('MSA outputs differ'in log or 'raw backing differs'in log),(rc,log[-2000:])
                state['results'][label]=dict(rejected=True,diagnostic=log[-2000:])
            else:
                assert rc==0 and 'all 9437184 stored bits identical'in log,(label,rc,log[-2000:])
                rows=list(csv.DictReader(trace.open()));reference=list(csv.DictReader((board/'control-raw.csv').open()))
                assert [(r['kind'],r['address'],r['value'])for r in rows]==[(r['kind'],r['address'],r['value'])for r in reference]
                if not enabled:assert trace.read_bytes()==(board/'control-raw.csv').read_bytes()
                state['results'][label]=dict(passed=True,host_ticks=int(rows[-1]['host_cycle']),
                    trace_sha256=sha(trace),canonical_operation_data_and_full_arrays_agree=True)
            assert all(sha(inputs/name)==digest for name,digest in frozen.items())
            save();print(label,state['results'][label],flush=True)
        state.update(status='PASS_BURST_READ_REUSE_ORIGINAL_MSA',finished=time.time(),
            native_cpu_firmware_or_hardware_speed_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['generated','board','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--controls',action='store_true');args=parser.parse_args()
    run(args.generated.resolve(),args.board.resolve(),args.output.resolve(),args.controls)
