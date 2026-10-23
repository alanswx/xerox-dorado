#!/usr/bin/env python3
"""Qualify bounded owner admission, payload/response ordering and reset drain.

Use an independent descriptor FIFO, memory oracle and delayed raw-port model in
both simulators. Physical SDRAM, original MSA/native128 and native reset-mux
integration are separate gates. No native wrapper selects this queue yet.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(output,controls):
    assert not output.exists(),'Inspect recorded work'
    output.mkdir(parents=True);inputs=output/'inputs';inputs.mkdir()
    for source,name in [(ROOT/'fpga/mister/dorado_storage_queue.sv','queue.sv'),
        (ROOT/'verilog/verilator/tb_storage_queue.sv','tb.sv'),(Path(__file__),Path(__file__).name)]:
        shutil.copyfile(source,inputs/name)
    frozen={p.name:sha(p)for p in inputs.iterdir()}
    (output/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs=frozen),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})
    def save():
        temp=output/'state.tmp';temp.write_text(json.dumps(state,indent=2)+'\n');os.replace(temp,output/'run-state.json')
    def execute(label,command):
        command=list(map(str,command))
        with(output/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row);save()
            row.update(returncode=p.wait(),finished=time.time());save()
        return row['returncode']
    def case(label,engine,source,groups=8,depth=4,diagnostic=None,address_bits=4):
        label=engine+'-'+label
        if engine=='icarus':
            obj=output/(label+'.vvp')
            command=['iverilog','-g2012','-s','tb_storage_queue','-Ptb_storage_queue.GROUPS='+str(groups),
                '-Ptb_storage_queue.DEPTH='+str(depth),'-Ptb_storage_queue.GROUP_ADDR_BITS='+str(address_bits),'-o',obj,source,inputs/'tb.sv']
            run_command=['vvp',obj]
        else:
            obj=output/('obj-'+label)
            command=['verilator','--binary','--timing','--autoflush','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
                '--top-module','tb_storage_queue','-GGROUPS='+str(groups),'-GDEPTH='+str(depth),'-GGROUP_ADDR_BITS='+str(address_bits),
                '--Mdir',obj,'-o','test-queue',source,inputs/'tb.sv']
            run_command=[obj/'test-queue']
        assert execute(label+'-compile',command)==0,label
        assert 'UNOPTFLAT'not in(output/(label+'-compile.log')).read_text()
        rc=execute(label+'-test',run_command);log=(output/(label+'-test.log')).read_text()
        if diagnostic:
            alternatives=[diagnostic]if isinstance(diagnostic,str)else diagnostic
            found=[s for s in alternatives if s in log];assert rc!=0 and found,(label,rc,log[-2000:])
            state['results'][label]=dict(rejected=True,diagnostic=found)
        else:
            assert rc==0 and 'STORAGE_QUEUE PASS'in log,(label,rc,log[-2000:])
            line=re.search(r'^STORAGE_QUEUE PASS.*$',log,re.M).group()
            state['results'][label]=dict(passed=True,summary=line)
        save();print(label,state['results'][label],flush=True)
    save()
    try:
        for engine in ['icarus','verilator']:
            for groups,depth,address_bits in [(8,4,4),(8,1,4),(3,2,4),(8,3,4),(128,4,16)]:
                case(f'g{groups}d{depth}',engine,inputs/'queue.sv',groups,depth,address_bits=address_bits)
        if controls:
            body=(inputs/'queue.sv').read_text()
            changes=[
                ('reset-before-drain','(reset || reset_draining) && owned==0','(reset || reset_draining)',
                 ['QUEUE_TEST backend reset before physical drain','QUEUE_TEST reset drain timeout']),
                ('drop-queued-on-reset','if(reset)reset_draining<=1;',
                 'if(reset)begin reset_draining<=1;queued_count<=0;end','STORAGE_QUEUE ownership accounting'),
                ('wrong-owner','owner<=head_owner;active<=1;',"owner<=head_owner ^ 1'b1;active<=1;",
                 ['QUEUE_TEST wrong response owner','STORAGE_QUEUE unowned completion']),
                ('live-payload','assign raw_req_data=head_data;','assign raw_req_data=req_data[owner*16+:16];','QUEUE_TEST raw payload/order'),
                ('overfill','wire room=owned<DEPTH || retiring;',"wire room=1'b1;",'STORAGE_QUEUE ownership accounting'),
                ('duplicate-group','eligible=req_valid & ~reserved;','eligible=req_valid;',
                 ['QUEUE_TEST accepted duplicate group','STORAGE_QUEUE duplicate group']),
                ('sticky-reservation','reserved[owner]<=0;active<=0;','active<=0;', 'QUEUE_TEST admission deadline'),
                ('held-reset-response','active && (reset || reset_draining || rsp_ready[owner])','active && rsp_ready[owner]', 'QUEUE_TEST reset drain timeout'),
            ]
            for label,old,new,diagnostic in changes:
                assert body.count(old)==1,old
                bad=output/(label+'.sv');bad.write_text(body.replace(old,new))
                for engine in ['icarus','verilator']:case(label,engine,bad,diagnostic=diagnostic)
        assert all(sha(inputs/name)==digest for name,digest in frozen.items())
        for groups,depth in [(8,4),(8,1),(3,2),(8,3),(128,4)]:
            label=f'g{groups}d{depth}'
            assert state['results']['icarus-'+label]['summary']==state['results']['verilator-'+label]['summary']
        state.update(status='PASS_STORAGE_QUEUE_PROTOCOL_TWO_SIMULATORS',finished=time.time(),inputs_unchanged=True,
            native_physical_timing_or_firmware_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True);parser.add_argument('--controls',action='store_true')
    args=parser.parse_args();run(args.output.resolve(),args.controls)
