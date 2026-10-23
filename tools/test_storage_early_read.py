#!/usr/bin/env python3
"""Qualify early selected-word replies against the exact capture parent.

Compare all physical commands/drain/cache observations and selected data under
synchronized requests in both simulators. Check exact eight-beat latency gains,
full-burst request exclusion, stable replies, refresh and resets before/after
early acknowledgement. Physical-memory/independent-pin and MSA gates remain.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prepare_storage_early_read import CAPTURE_BACKEND

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(generated,reference,out):
    assert not out.exists() and sha(reference)==CAPTURE_BACKEND
    g=json.loads((generated/'provenance.json').read_text());assert g['early_read_response']
    for n,h in g['generated_hashes'].items():assert sha(generated/n)==h,n
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    old=reference.read_text();needle='module dorado_storage_sdram_burst #('
    assert old.count(needle)==1
    (inputs/'reference.sv').write_text(old.replace(needle,'module dorado_storage_sdram_burst_reference #('))
    shutil.copyfile(reference,inputs/'capture-parent.sv')
    shutil.copyfile(generated/'dorado_storage_sdram_burst.sv',inputs/'candidate.sv')
    for f in [ROOT/'verilog/verilator/tb_storage_early_read.sv',Path(__file__),ROOT/'tools/prepare_storage_early_read.py']:
        shutil.copyfile(f,inputs/f.name)
    frozen={p.name:sha(p) for p in inputs.iterdir()}
    (out/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs=frozen,parent=g,
        capture_parent_sha256=CAPTURE_BACKEND,reference_module_rename_only=True),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})

    def save():
        t=out/'run-state.tmp';t.write_text(json.dumps(state,indent=2)+'\n');os.replace(t,out/'run-state.json')

    def execute(label,command):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(list(map(str,command)),cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=p.pid,command=list(map(str,command)),started=time.time())
            state['actions'].append(row);save()
            row.update(returncode=p.wait(),finished=time.time());save()
        return row['returncode']

    def case(label,engine,source,early=1,negative=False):
        if engine=='icarus':
            binary=out/(label+'.vvp')
            compile=['iverilog','-g2012','-s','tb_storage_early_read',
                     '-Ptb_storage_early_read.EARLY='+str(early),'-o',binary,
                     inputs/'reference.sv',source,inputs/'tb_storage_early_read.sv']
            command=['vvp',binary]
        else:
            obj=out/('obj-'+label)
            compile=['verilator','--binary','--timing','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
                     '--top-module','tb_storage_early_read','-GEARLY='+str(early),'--Mdir',obj,
                     '-o','test-early',inputs/'reference.sv',source,inputs/'tb_storage_early_read.sv']
            command=[obj/'test-early']
        assert execute(label+'-compile',compile)==0,label
        assert 'UNOPTFLAT' not in (out/(label+'-compile.log')).read_text(),label
        code=execute(label+'-test',command);text=(out/(label+'-test.log')).read_text()
        if negative:
            assert code!=0 and 'EARLY_READ' in text,(label,text[-1800:])
            state['results'][label]=dict(rejected=True,diagnostic=text)
        else:
            assert code==0 and 'EARLY_READ_MITER PASS early='+str(early) in text,(label,text[-1800:])
            state['results'][label]=dict(passed=True,output=text)
        save();print(label,'rejected'if negative else text.splitlines()[0],flush=True)

    save()
    try:
        for engine in ['icarus','verilator']:
            case(engine+'-off',engine,inputs/'candidate.sv',early=0)
            case(engine+'-on',engine,inputs/'candidate.sv')
        body=(inputs/'candidate.sv').read_text()
        changes=[
            ('reply-rearmed-at-tail','if(!EARLY_READ_RESPONSE)response_pending<=1;','response_pending<=1;'),
            ('accept-before-drain','initialized && state==IDLE &&','initialized && (state==IDLE || state==READ_WAIT) &&'),
            ('idle-before-drain','(state==IDLE || state==REF_PRE1','(state==IDLE || state==READ_WAIT || state==REF_PRE1'),
            ('wrong-selected-word','if(fill_beat==address[2:0])begin',"if(fill_beat==((address[2:0]+3'd1)&3'd7))begin"),
        ]
        for label,old,new in changes:
            assert body.count(old)==1,old
            bad=out/(label+'.sv');bad.write_text(body.replace(old,new))
            for engine in ['icarus','verilator']:case(label+'-'+engine,engine,bad,negative=True)
        assert all(sha(inputs/n)==h for n,h in frozen.items())
        state.update(status='PASS_EARLY_SELECTED_READ_MITER_TWO_SIMULATORS',finished=time.time(),
                     inputs_unchanged=True,physical_model_native_timing_firmware_qualified=False)
        save()
    except BaseException as e:
        state.update(status='TERMINAL_FAILED',error=repr(e),finished=time.time());save();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['generated','reference','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.generated.resolve(),a.reference.resolve(),a.output.resolve())
