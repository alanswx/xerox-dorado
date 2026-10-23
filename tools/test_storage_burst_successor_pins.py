#!/usr/bin/env python3
"""Check burst successors with the independent XSDS command/pin memory model.

Retain the existing rank/bank/beat, write coherence, reset and held-refresh
tests. Add cold sequential bursts starting at every requested beat, with and
without response stalls. Timing-model success is not fitted FPGA qualification.
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

from test_storage_early_pins import STREAM

ROOT = Path(__file__).resolve().parents[1]
CHAIN = r'''
    integer successor_accepts=0;
    reg [7:0] successor_beats=0;
    always @(posedge clk)if(req_valid && req_ready && buffered.dut.state==6)begin
        successor_accepts++;
        successor_beats[req_addr[2:0]]=1;
    end
    task automatic cold_chain(input int unsigned base,input int first,stall);
        int sent=0,received=0,ticks=0,before_reads;
        bit held=0;
        reg [15:0] held_value;
        seed_line(base);before_reads=physical_reads;
        @(negedge clk);
        while(received<8)begin
            req_valid=sent<8;req_write=0;req_addr=26'(base+32'((first+sent)&7));
            rsp_ready=stall==0 || ticks%11<6;
            @(posedge clk);
            if(held && (!rsp_valid || rsp_data!==held_value))
                $fatal(1,"CHAIN response changed under hold");
            held=rsp_valid && !rsp_ready;held_value=rsp_data;
            if(rsp_valid && rsp_ready)begin
                if(received>=sent)$fatal(1,"CHAIN orphan completion");
                if(rsp_data!==expected[base+32'((first+received)&7)])
                    $fatal(1,"CHAIN data/order got%h expected%h",rsp_data,expected[base+32'((first+received)&7)]);
                received++;
            end
            if(req_valid && req_ready)sent++;
            if(sent-received<0 || sent-received>1)$fatal(1,"CHAIN request ownership");
            ticks++;if(ticks>2000)$fatal(1,"CHAIN deadline");
            @(negedge clk);
        end
        req_valid=0;rsp_ready=0;
        if(physical_reads!=before_reads+1)$fatal(1,"CHAIN duplicate physical fill");
        $display("COLD_CHAIN base=%h first=%0d stall=%0d ticks=%0d",base,first,stall,ticks);
    endtask
'''


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(generated, output, controls):
    assert not output.exists(), 'Inspect existing recorded work'
    proof = json.loads((generated / 'provenance.json').read_text())
    assert proof['burst_read_successor']
    output.mkdir(parents=True)
    inputs = output / 'inputs'
    inputs.mkdir()
    for name, digest in proof['generated_hashes'].items():
        assert sha(generated / name) == digest
        shutil.copyfile(generated / name, inputs / name)
    for name, source in {
        'original-tb.sv': ROOT / 'verilog/verilator/tb_storage_sdram_burst.sv',
        'xsds_model.sv': ROOT / 'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
        'dorado_storage_sdram.sv': ROOT / 'fpga/mister/dorado_storage_sdram.sv',
        Path(__file__).name: Path(__file__),
        'test_storage_early_pins.py': ROOT / 'tools/test_storage_early_pins.py',
    }.items():
        shutil.copyfile(source, inputs / name)
    tb = (inputs / 'original-tb.sv').read_text()
    tb = tb.replace('parameter integer CACHE_LINES=4,',
                    'parameter integer CACHE_LINES=4,\n    parameter integer RESPONSE_OVERLAP=1,\n'
                    '    parameter integer BURST_READ_SUCCESSOR=0,')
    old = '.CLOCK_HZ(CLOCK_HZ),.CACHE_LINES(CACHE_LINES)) dut('
    assert tb.count(old) == 1
    tb = tb.replace(old, '.CLOCK_HZ(CLOCK_HZ),.CACHE_LINES(CACHE_LINES),'
                    '.RESPONSE_OVERLAP(RESPONSE_OVERLAP),.CAPTURE_READ_WORD(1),'
                    '.EARLY_READ_RESPONSE(1),.BURST_READ_SUCCESSOR(BURST_READ_SUCCESSOR)) dut(')
    anchor = '    initial begin\n        int before_reads,r0,r1;'
    assert tb.count(anchor) == 1
    tb = tb.replace(anchor, STREAM + CHAIN + anchor)
    anchor = '        if(chip0.error_count!=0 || chip1.error_count!=0 || chip0.warning_count!=0 || chip1.warning_count!=0)'
    assert tb.count(anchor) == 1
    tb = tb.replace(anchor, '''        for(int owner=0;owner<8;owner++)for(int first=0;first<8;first++)
            cold_chain((32'(owner)<<23)+32'h10000+32'(first*16),first,first%2);
        stream(a0,b0,64,0);stream(a0,b0,96,1);
        if((BURST_READ_SUCCESSOR && successor_beats!==8'hfe) ||
           (!BURST_READ_SUCCESSOR && successor_accepts!=0))$fatal(1,"CHAIN forwarding coverage");
        $display("SUCCESSOR_PINS forwarded=%0d beats=%h",successor_accepts,successor_beats);
''' + anchor)
    (inputs / 'tb.sv').write_text(tb)
    frozen = {path.name: sha(path) for path in inputs.iterdir()}
    (output / 'provenance.json').write_text(json.dumps(dict(
        scope=__doc__, inputs=frozen, generated_proof=proof), indent=2) + '\n')
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), actions=[], results={})

    def save():
        temp = output / 'state.tmp'
        temp.write_text(json.dumps(state, indent=2) + '\n')
        os.replace(temp, output / 'run-state.json')

    def execute(label, command):
        command = list(map(str, command))
        with (output / (label + '.log')).open('x') as log:
            process = subprocess.Popen(command, cwd=inputs, stdout=log, stderr=subprocess.STDOUT)
            action = dict(label=label, pid=process.pid, command=command, started=time.time())
            state['actions'].append(action)
            save()
            action.update(returncode=process.wait(), finished=time.time())
            save()
        return action['returncode']

    def case(label, enabled, hz, source=None, diagnostic=None):
        obj = output / ('obj-' + label)
        command = ['verilator', '--binary', '--timing', '--autoflush', '-j', '2',
                   '+define+DORADO_CORRECT_READ_CAPTURE', '--top-module', 'tb_storage_sdram_burst',
                   '-Wno-fatal', '-Werror-UNOPTFLAT', '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC',
                   '-Wno-WIDTHEXPAND', '-Wno-REALCVT', '-Wno-PINMISSING', '-Wno-DEFPARAM',
                   '-GCLOCK_HZ=' + str(hz), '-GBURST_READ_SUCCESSOR=' + str(enabled),
                   '--Mdir', obj, '-o', 'test-pins', inputs / 'dorado_storage_sdram.sv',
                   source or inputs / 'dorado_storage_sdram_burst.sv',
                   inputs / 'tb.sv', inputs / 'xsds_model.sv']
        assert execute(label + '-compile', command) == 0, label
        assert 'UNOPTFLAT' not in (output / (label + '-compile.log')).read_text()
        rc = execute(label + '-test', [obj / 'test-pins'])
        log = (output / (label + '-test.log')).read_text()
        if diagnostic:
            assert rc != 0 and diagnostic in log, (label, rc, log[-1800:])
            state['results'][label] = dict(rejected=True, diagnostic=diagnostic)
        else:
            assert rc == 0 and 'STORAGE-SDRAM-BURST PASS' in log, (label, rc, log[-1800:])
            rows = re.findall(r'COLD_CHAIN base=(\w+) first=(\d+) stall=(\d+) ticks=(\d+)', log)
            assert len(rows) == 64
            count, beats = re.search(r'SUCCESSOR_PINS forwarded=(\d+) beats=(\w+)', log).groups()
            assert (int(count) > 0) == bool(enabled)
            state['results'][label] = dict(chains=[dict(base=int(base, 16), first=int(first),
                stall=int(stall), ticks=int(ticks)) for base, first, stall, ticks in rows],
                forwarded=int(count), beat_mask=beats)
        save()
        print(label, 'rejected' if diagnostic else dict(forwarded=int(count), chains=len(rows)), flush=True)

    save()
    try:
        for hz in [57142857, 66666667]:
            case('off-' + str(hz), 0, hz)
            case('on-' + str(hz), 1, hz)
        if controls:
            body = (inputs / 'dorado_storage_sdram_burst.sv').read_text()
            old = '                rsp_data<=dq_capture;\n                response_pending<=1;'
            assert body.count(old) == 1
            bad = output / 'corrupt-forward.sv'
            bad.write_text(body.replace(old, "                rsp_data<=dq_capture ^ 16'h0001;\n                response_pending<=1;"))
            case('corrupt-forward', 1, 57142857, bad, 'CHAIN data/order')
        assert all(sha(inputs / name) == digest for name, digest in frozen.items())
        state.update(status='PASS_BURST_SUCCESSOR_INDEPENDENT_PHYSICAL_PINS', finished=time.time(),
                     frozen_inputs_unchanged=True, native_fit_or_firmware_qualified=False)
        save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED', error=repr(error), finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['generated', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--controls', action='store_true')
    args = parser.parse_args()
    run(args.generated.resolve(), args.output.resolve(), args.controls)
