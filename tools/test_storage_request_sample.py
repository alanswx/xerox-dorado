#!/usr/bin/env python3
"""Test opposite-edge address sampling against the original144-chip MSA.

The native address latches update only on machine-clock rising edges, which
are gated host rising edges. A host falling-edge sample therefore equals the
old address at the following host rising edge. Preserve the request epoch,
and assert equality whenever a request is accepted. No timing waiver.
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

ROOT = Path(__file__).resolve().parents[1]
GROUP = 'src/fpga/mister/dorado_storage_group.sv'
MSA = 'src/build/rtl-executive/native16-tx-cable/msa_ddram.v'


def sha(b):
    return hashlib.sha256(b).hexdigest()


def once(s, before, after):
    assert s.count(before) == 1, (before, s.count(before))
    return s.replace(before, after)


def sample_group(raw):
    s = raw.decode()
    s = once(s, 'parameter integer WIDTH=16',
             'parameter integer WIDTH=16,\n    parameter integer HOST_ADDR_SAMPLE=0')
    s = once(s, '    wire hit=cache_valid && cache_addr==addr;', '''    // Native row/column latches change on gated host rising edges only.
    // Sample the stable OLD address half a host cycle before the original
    // saved_addr edge. This changes physical timing, not the accepted value
    // or request/response cycle. Keep the generic interface disabled by default.
    reg [ADDR_BITS-1:0] addr_before_host=0;
    generate if(HOST_ADDR_SAMPLE!=0)begin
        always @(negedge host_clk)addr_before_host<=addr;
    end endgenerate
    wire hit=cache_valid && cache_addr==addr;''')
    s = once(s, 'saved_addr<=addr;',
             'saved_addr<=HOST_ADDR_SAMPLE!=0 ? addr_before_host : addr;')
    s = once(s, '    // synthesis translate_off', '''    // synthesis translate_off
    always @(posedge host_clk)if(HOST_ADDR_SAMPLE!=0 && !reset && req_valid && req_ready)
        if(addr_before_host!==addr)
            $fatal(1,"STORAGE_REQUEST_SAMPLE differs from original accepted address");''')
    return s.encode()


def sample_board(raw):
    s, count = re.subn(r'(dorado_storage_group #\([^\n]+)(\) u_storage_\d+)',
                       r'\1,.HOST_ADDR_SAMPLE(1)\2', raw.decode())
    assert count == 16
    return s.encode()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent','native','run']:
        p.add_argument('--'+n, type=Path, required=True)
    a = p.parse_args()
    a.parent, a.native, a.run = a.parent.resolve(), a.native.resolve(), a.run.resolve()
    old = json.loads((a.parent/'provenance.json').read_text())
    state = json.loads((a.parent/'run-state.json').read_text())
    assert state['status'] == 'PASS_MSA_SAME_EDGE_BOARD_READY_REDUCTION'
    assert old['fixed_lane_capture']
    for n,h in old['inputs'].items():
        assert sha((a.parent/'inputs'/n).read_bytes()) == h, n
    native = json.loads((a.native/'storage-board-native.json').read_text())
    for name, staged in [('dorado_storage_group.sv',GROUP),('msa_ddram.v',MSA)]:
        assert (a.parent/'inputs'/name).read_bytes() == (a.native/staged).read_bytes()
        assert sha((a.native/staged).read_bytes()) == native['source_hashes'][staged]
    a.run.mkdir(parents=True, exist_ok=False)
    inputs = a.run/'inputs'
    shutil.copytree(a.parent/'inputs', inputs)
    original_board = (inputs/'msa_ddram.v').read_bytes()
    original_group = (inputs/'dorado_storage_group.sv').read_bytes()
    (inputs/'dorado_storage_group.sv').write_bytes(sample_group(original_group))
    (inputs/'msa_ddram.v').write_bytes(sample_board(original_board))
    (inputs/'test_storage_request_sample.py').write_bytes(Path(__file__).read_bytes())
    (inputs/'original_group.sv').write_bytes(original_group.replace(b'module dorado_storage_group #', b'module dorado_storage_group_original #'))
    shutil.copyfile(ROOT/'verilog/verilator/tb_storage_request_sample.sv',inputs/'unit.sv')
    proof = dict(scope=__doc__, parent_provenance_sha256=sha((a.parent/'provenance.json').read_bytes()),
                 native_manifest_sha256=sha((a.native/'storage-board-native.json').read_bytes()),
                 trace_reference=str(a.parent/'positive-raw.csv'),
                 trace_sha256=sha((a.parent/'positive-raw.csv').read_bytes()),
                 original_group_sha256=sha(original_group), original_board_sha256=sha(original_board),
                 inputs={str(f.relative_to(inputs)):sha(f.read_bytes()) for f in inputs.rglob('*') if f.is_file()})
    (a.run/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n')
    result = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[],
                  native_timing_qualified=False, firmware_qualified=False)

    def save():
        (a.run/'run-state.json').write_text(json.dumps(result,indent=2)+'\n')

    def execute(argv, label, negative=False):
        row = dict(stage=label, command=list(map(str,argv)), started=time.time())
        result['actions'].append(row)
        with (a.run/(label+'.log')).open('xb') as log:
            child = subprocess.Popen(row['command'],cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row['pid'] = child.pid
            save()
            row.update(returncode=child.wait(),finished=time.time())
        save()
        text = (a.run/(label+'.log')).read_text(errors='replace')
        if negative:
            assert row['returncode'] != 0 and 'STORAGE_REQUEST_SAMPLE' in text, (label,text[-2500:])
        else:
            assert row['returncode'] == 0, (label,text[-2500:])

    def case(name, group, board, negative=False):
        obj = a.run/('obj-'+name)
        sources = [board,group]+[inputs/n for n in ['msa.v','dorado_storage_arb.sv',
            'dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv','xsds_model.sv',
            'tb.sv','burst-fixture.sv','addr_preview.sv']]
        cmd = ['verilator','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_board',
            '-GADDR_BITS=16','-GPREFETCH_NEXT=1','-GINTERLEAVE_GROUPS=1','-DSTORAGE_USE_SDRAM',
            '-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT',
            '-I'+str(inputs/'cells'),'--Mdir',str(obj),'-o','sample-test',*map(str,sources)]
        execute(cmd,name+'-compile')
        execute([obj/'sample-test','+raw_trace='+str(a.run/(name+'-raw.csv'))],name+'-test',negative)
        if not negative:
            assert 'all 9437184 stored bits identical' in (a.run/(name+'-test.log')).read_text()
            assert (a.run/(name+'-raw.csv')).read_bytes() == (a.parent/'positive-raw.csv').read_bytes(), 'Raw value/host epoch changed'
        print(name, 'rejected' if negative else 'original bits/exact host trace PASS', flush=True)

    try:
        save()
        case('positive',inputs/'dorado_storage_group.sv',inputs/'msa_ddram.v')
        board = a.run/'default-board.v'
        board.write_bytes(original_board)
        case('default-disabled',inputs/'dorado_storage_group.sv',board)
        group = (inputs/'dorado_storage_group.sv').read_text()
        wrong = a.run/'wrong-edge.sv'
        wrong.write_text(once(group,'@(negedge host_clk)addr_before_host','@(posedge host_clk)addr_before_host'))
        # The board arbiter masks this defect in its particular trace. Retain
        # that passing control; the direct immediate-accept oracle must fail.
        case('wrong-edge-board',wrong,inputs/'msa_ddram.v')
        def unit(name, source, diagnostic=None):
            obj=a.run/('obj-'+name)
            execute(['verilator','--binary','--timing','--autoflush','-j','2',
                     '--top-module','tb_storage_request_sample','-Wno-fatal','-Wno-PINMISSING',
                     '--Mdir',str(obj),'-o','unit-test',str(inputs/'unit.sv'),
                     str(inputs/'original_group.sv'),str(source)],name+'-compile')
            with(a.run/(name+'-test.log')).open('xb')as log:
                child=subprocess.Popen([str(obj/'unit-test')],stdout=log,stderr=subprocess.STDOUT)
                row=dict(stage=name+'-test',pid=child.pid,command=[str(obj/'unit-test')],started=time.time())
                result['actions'].append(row);save();row.update(returncode=child.wait(),finished=time.time());save()
            text=(a.run/(name+'-test.log')).read_text()
            if diagnostic:assert row['returncode']!=0 and diagnostic in text,(name,text)
            else:assert row['returncode']==0 and 'STORAGE_REQUEST_UNIT PASS' in text,(name,text)
            print(name,'rejected'if diagnostic else'immediate-accept oracle PASS',flush=True)
        unit('unit-positive',inputs/'dorado_storage_group.sv')
        unit('unit-wrong-edge',wrong,'STORAGE_REQUEST_SAMPLE')
        no_contract=a.run/'wrong-edge-no-contract.sv'
        text=wrong.read_text()
        start=text.index('    always @(posedge host_clk)if(HOST_ADDR_SAMPLE!=0')
        stop=text.index('    always @(negedge host_clk)if(!reset',start)
        no_contract.write_text(text[:start]+text[stop:])
        unit('unit-wrong-edge-no-contract',no_contract,'STORAGE_REQUEST_ORACLE')
        future=a.run/'future-address.sv';future.write_text(once(group,'addr_before_host<=addr;','addr_before_host<=addr_after_edge;'))
        unit('unit-future-address',future,'STORAGE_REQUEST_SAMPLE')
        result.update(status='PASS_STORAGE_REQUEST_OPPOSITE_EDGE_SAMPLE',finished=time.time(),
                      added_request_cycles=0, original_bits=9437184, exact_host_trace=True,
                      board_cases=3, immediate_accept_cases=4, rejected_controls=3)
        save()
    except BaseException:
        result.update(status='TERMINAL_FAILED',finished=time.time())
        save()
        raise


if __name__ == '__main__':
    main()
