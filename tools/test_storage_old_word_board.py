#!/usr/bin/env python3
"""Replay the original MSA with the falling-edge old-word transport capture.

The separately frozen capture-control fixture has no early-response mode.
Replay its exact raw trace with the original group, then require identical
host epochs, data/order, original outputs and all9437184 stored bits after
changing only the group's old-word capture. No native timing claim.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from test_storage_old_word_falling import GROUP_SHA, transform


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(board,out,early=False):
    assert not out.exists()
    p=json.loads(gzip.decompress((board/'provenance.json.gz').read_bytes()))
    for n,h in p['inputs'].items():assert sha(board/'inputs'/n)==h,n
    state=json.loads((board/'run-state.json').read_text())
    assert state['status']=='PASS_EARLY_SELECTED_READ_ORIGINAL_MSA'
    raw=(board/'inputs/dorado_storage_group.sv').read_bytes();assert hashlib.sha256(raw).hexdigest()==GROUP_SHA
    prefix='early-on' if early else 'capture-control'
    tb=(board/('inputs/'+prefix+'-tb.sv')).read_text()
    fixture=(board/('inputs/'+prefix+'-fixture.sv')).read_text()
    assert '.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)' in tb
    # The earlier control selected CAPTURE=0; its separately qualified
    # CAPTURE=1 form has identical response cycles. Replay that assertion
    # here too, using the exact old host trace as the independent gate.
    old='.CAPTURE_READ_WORD(0),.EARLY_READ_RESPONSE(0)'
    if early:assert '.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(1)' in fixture
    else:assert fixture.count(old)==1
    out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(board/'inputs',inputs)
    (inputs/(prefix+'-fixture.sv')).write_text(fixture if early else fixture.replace(old,'.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(0)'))
    (inputs/'candidate-group.sv').write_bytes(transform(raw))
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    frozen={str(f.relative_to(inputs)):sha(f) for f in inputs.rglob('*') if f.is_file()}
    (out/'provenance.json').write_text(json.dumps(dict(scope=__doc__,
        source_board_provenance_sha256=sha(board/'provenance.json.gz'),
        original_group_sha256=GROUP_SHA,inputs=frozen,
        original_trace_sha256=sha(board/(prefix+'-raw.csv')),early_read_response=early,
        native_qualified=False),indent=2)+'\n')
    run=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})

    def save():(out/'run-state.json').write_text(json.dumps(run,indent=2)+'\n')

    def execute(label,cmd):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(list(map(str,cmd)),cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=child.pid,command=list(map(str,cmd)),started=time.time());run['actions'].append(row);save()
            row.update(returncode=child.wait(),finished=time.time());save()
        return row['returncode'],(out/(label+'.log')).read_text()

    def case(label,group):
        obj=out/('obj-'+label)
        names=['msa_ddram.v','msa.v','dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv',
               'dorado_storage_addr_preview.sv','xsds_model.sv','dorado_storage_arb.sv',
               prefix+'-tb.sv',prefix+'-fixture.sv']
        cmd=['verilator','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_board',
             '-GADDR_BITS=16','-GPREFETCH_NEXT=1','-GINTERLEAVE_GROUPS=1','-DSTORAGE_USE_SDRAM',
             '-Wno-fatal','-Werror-UNOPTFLAT','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC',
             '-Wno-WIDTHEXPAND','-Wno-REALCVT','-I'+str(inputs/'cells'),'--Mdir',obj,'-o','test-old-word',
             group,*[inputs/n for n in names]]
        rc,log=execute(label+'-compile',cmd);assert rc==0 and 'UNOPTFLAT'not in log,(label,log[-2000:])
        trace=out/(label+'-raw.csv')
        rc,log=execute(label+'-test',[obj/'test-old-word','+raw_trace='+str(trace)])
        assert rc==0 and 'all 9437184 stored bits identical' in log,(label,log[-2000:])
        assert trace.read_bytes()==(board/(prefix+'-raw.csv')).read_bytes(),label
        line=next(s for s in log.splitlines()if 'STORAGE-BOARD PASS' in s)
        run['results'][label]=dict(passed=True,pass_line=line,raw_trace_sha256=sha(trace),exact_host_trace=True)
        save();print(label,line,flush=True)

    save()
    try:
        case('reference',inputs/'dorado_storage_group.sv')
        case('falling-old-word',inputs/'candidate-group.sv')
        assert all(sha(inputs/n)==h for n,h in frozen.items())
        run.update(status='PASS_OLD_WORD_FALLING_ORIGINAL_MSA_EXACT_HOST_TRACE',finished=time.time(),
                   inputs_unchanged=True,all_original_stored_bits=9437184,native_qualified=False);save()
    except BaseException as e:
        run.update(status='TERMINAL_FAILED',finished=time.time(),error=repr(e));save();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['board','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--early',action='store_true')
    a=p.parse_args();run(a.board.resolve(),a.output.resolve(),a.early)
