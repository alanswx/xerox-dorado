#!/usr/bin/env python3
"""Compare all control/pins and valid data against the qualified backend.

Both engines drive the exact old/new modules with common arbitrary DQ input,
all eight requested beats, writes/hits/row/rank changes, aborted bursts and
long held responses. Invalid response data may differ; completion timing may
not. The separate physical SDRAM-model suite remains required before fitting.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prepare_storage_read_word_capture import QUALIFIED_BACKEND

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(generated,reference,out):
    assert not out.exists() and sha(reference)==QUALIFIED_BACKEND
    g=json.loads((generated/'provenance.json').read_text());assert g['capture_read_word']
    assert sha(generated/'dorado_storage_sdram_burst.sv')==g['generated_hashes']['dorado_storage_sdram_burst.sv']
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    old=reference.read_text();needle='module dorado_storage_sdram_burst #('
    assert old.count(needle)==1
    (inputs/'reference.sv').write_text(old.replace(needle,'module dorado_storage_sdram_burst_reference #('))
    shutil.copyfile(reference,inputs/'qualified-original.sv')
    shutil.copyfile(generated/'dorado_storage_sdram_burst.sv',inputs/'candidate.sv')
    for source in [ROOT/'verilog/verilator/tb_storage_read_word_capture.sv',Path(__file__),ROOT/'tools/prepare_storage_read_word_capture.py']:
        shutil.copyfile(source,inputs/source.name)
    frozen={p.name:sha(p)for p in inputs.iterdir()}
    (out/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs=frozen,parent=g,qualified_backend_sha256=QUALIFIED_BACKEND,reference_module_rename_only=True),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})
    def save():
        t=out/'run-state.tmp';t.write_text(json.dumps(state,indent=2)+'\n');os.replace(t,out/'run-state.json')
    def execute(label,command):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(list(map(str,command)),cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=p.pid,command=list(map(str,command)),started=time.time());state['actions'].append(row);save()
            row.update(returncode=p.wait(),finished=time.time());save()
        return row['returncode']
    def case(label,engine,source,diagnostic=None):
        if engine=='icarus':
            binary=out/(label+'.vvp')
            compile=['iverilog','-g2012','-s','tb_storage_read_word_capture','-o',binary,inputs/'reference.sv',source,inputs/'tb_storage_read_word_capture.sv']
            command=['vvp',binary]
        else:
            obj=out/('obj-'+label)
            compile=['verilator','--binary','--timing','-j','4','-Wno-fatal','-Werror-UNOPTFLAT','--top-module','tb_storage_read_word_capture','--Mdir',obj,'-o','test-read-capture',inputs/'reference.sv',source,inputs/'tb_storage_read_word_capture.sv']
            command=[obj/'test-read-capture']
        assert execute(label+'-compile',compile)==0,label
        code=execute(label+'-test',command);text=(out/(label+'-test.log')).read_text()
        if diagnostic:
            assert code!=0 and diagnostic in text,(label,text[-1500:]);state['results'][label]=dict(rejected=True,diagnostic=diagnostic)
        else:
            assert code==0 and 'READ_CAPTURE_MITER PASS' in text,(label,text[-1800:]);state['results'][label]=dict(passed=True,output=text)
        save();print(label,state['results'][label],flush=True)
    save()
    try:
        for engine in ['icarus','verilator']:case(engine,engine,inputs/'candidate.sv')
        body=(inputs/'candidate.sv').read_text()
        changes=[
            ('wrong-selected-beat','if(fill_beat==address[2:0])begin',"if(fill_beat==((address[2:0]+3'd1)&3'd7))begin",'READ_CAPTURE valid data mismatch'),
            ('early-response','if(CAPTURE_READ_WORD)rsp_data<=dq_capture;','if(CAPTURE_READ_WORD)begin rsp_data<=dq_capture;response_pending<=!aborted && !reset;end','READ_CAPTURE control/pin/timing mismatch'),
            ('overwrite-held-response','if(reset)line_valid<=0;','if(reset)line_valid<=0;\n        if(response_pending && !rsp_ready)rsp_data<=dq_capture;','READ_CAPTURE valid data mismatch'),
        ]
        for label,old,new,diagnostic in changes:
            assert body.count(old)==1,old
            bad=out/(label+'.sv');bad.write_text(body.replace(old,new))
            for engine in ['icarus','verilator']:case(label+'-'+engine,engine,bad,diagnostic)
        assert all(sha(inputs/n)==h for n,h in frozen.items())
        state.update(status='PASS_SELECTED_READ_CAPTURE_CONTROL_AND_VALID_DATA_TWO_SIMULATORS',finished=time.time(),inputs_unchanged=True,physical_model_native_timing_firmware_qualified=False);save()
    except BaseException as e:
        state.update(status='TERMINAL_FAILED',error=repr(e),finished=time.time());save();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['generated','reference','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.generated.resolve(),a.reference.resolve(),a.output.resolve())
