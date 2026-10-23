#!/usr/bin/env python3
"""Test early selected-word replies against independent physical SDRAM chips.

Retain both-rank/bank/beat, neighbor coherence, reset, refresh and backpressure
checks. Full physical drain still precedes successor acceptance. Compare exact
control/candidate work at57/66MHz; report latency without a native speed claim.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]

STREAM = r'''
    task automatic stream(input int unsigned base,other,input int count,mode);
        int sent=0,received=0,ticks=0,overlaps=0,streak=0,longest=0,last_accept=-2;
        reg [15:0] queued_value[0:127];
        bit queued_write[0:127];
        bit held=0;
        reg [15:0] held_value;
        @(negedge clk);
        while(received<count)begin
            req_valid=sent<count;
            req_write=mode!=0 && sent%5==2;
            req_addr=26'((mode!=0 && sent%3==0 ? other : base)+32'((sent*3)&7));
            req_data=16'hb600 ^ 16'(sent);
            rsp_ready=mode==0 || ticks%11<7;
            @(posedge clk);
            if(held && (!rsp_valid || rsp_data!==held_value))
                $fatal(1,"STREAM response changed under backpressure");
            if(rsp_valid && !rsp_ready && req_ready)
                $fatal(1,"STREAM accepted through unconsumed response");
            held=rsp_valid && !rsp_ready;held_value=rsp_data;
            // Consume OLD response before accepting/updating the successor.
            if(rsp_valid && rsp_ready)begin
                if(received>=sent)$fatal(1,"STREAM orphan response");
                if(!queued_write[received] && rsp_data!==queued_value[received])
                    $fatal(1,"STREAM data/order mismatch at %0d got%h want%h",
                        received,rsp_data,queued_value[received]);
                received++;
            end
            if(req_valid && req_ready)begin
                queued_write[sent]=req_write;
                if(req_write)expected[int'(req_addr)]=req_data;
                else begin
                    if(!expected.exists(int'(req_addr)))$fatal(1,"STREAM unseeded address");
                    queued_value[sent]=expected[int'(req_addr)];
                end
                if(last_accept==ticks-1)streak++;else streak=1;
                if(streak>longest)longest=streak;
                last_accept=ticks;sent++;
                if(rsp_valid && rsp_ready)overlaps++;
            end
            if(sent-received<0 || sent-received>1)$fatal(1,"STREAM occupancy exceeds one");
            ticks++;if(ticks>5000)$fatal(1,"STREAM deadline");
            @(negedge clk);
        end
        req_valid=0;rsp_ready=0;req_write=0;
        if(sent!=count)$fatal(1,"STREAM incomplete");
        if(mode==0)begin
            if(RESPONSE_OVERLAP && (overlaps<32 || longest<8))
                $fatal(1,"STREAM overlap throughput not exercised");
            if(!RESPONSE_OVERLAP && (overlaps!=0 || longest>1))
                $fatal(1,"STREAM baseline unexpectedly overlapped");
        end
        $display("RESPONSE_STREAM overlap=%0d mode=%0d requests=%0d host_ticks=%0d simultaneous=%0d longest=%0d",
            RESPONSE_OVERLAP,mode,count,ticks,overlaps,longest);
    endtask
'''


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(generated, out, controls):
    assert not out.exists(), 'Inspect the existing recorded process/result'
    parent = json.loads((generated / 'provenance.json').read_text())
    for name, digest in parent['generated_hashes'].items():
        assert sha(generated / name) == digest, name
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    inputs.mkdir()
    for name in parent['generated_hashes']:
        shutil.copyfile(generated / name, inputs / name)
    for name, source in {
        'dorado_storage_sdram.sv': 'fpga/mister/dorado_storage_sdram.sv',
        'original-tb.sv': 'verilog/verilator/tb_storage_sdram_burst.sv',
        'xsds_model.sv': 'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
        Path(__file__).name: 'tools/' + Path(__file__).name,
        'prepare_storage_read_word_capture.py': 'tools/prepare_storage_read_word_capture.py',
        'prepare_storage_early_read.py': 'tools/prepare_storage_early_read.py',
    }.items():
        shutil.copyfile(ROOT / source, inputs / name)
    tb = (inputs / 'original-tb.sv').read_text()
    tb = tb.replace('parameter integer CACHE_LINES=4,',
                    'parameter integer CACHE_LINES=4,\n    parameter integer RESPONSE_OVERLAP=0,\n    parameter integer CAPTURE_READ_WORD=0,\n    parameter integer EARLY_READ_RESPONSE=0,')
    old = '.CLOCK_HZ(CLOCK_HZ),.CACHE_LINES(CACHE_LINES)) dut('
    assert tb.count(old) == 1
    tb = tb.replace(old, '.CLOCK_HZ(CLOCK_HZ),.CACHE_LINES(CACHE_LINES),.RESPONSE_OVERLAP(RESPONSE_OVERLAP),.CAPTURE_READ_WORD(CAPTURE_READ_WORD),.EARLY_READ_RESPONSE(EARLY_READ_RESPONSE)) dut(')
    anchor = '    initial begin\n        int before_reads,r0,r1;'
    assert tb.count(anchor) == 1
    tb = tb.replace(anchor, STREAM + anchor)
    anchor = '        if(chip0.error_count!=0 || chip1.error_count!=0 || chip0.warning_count!=0 || chip1.warning_count!=0)'
    assert tb.count(anchor) == 1
    tb = tb.replace(anchor,
        '        stream(a0,b0,64,0);\n        stream(a0,b0,96,1);\n' + anchor)
    (inputs / 'tb.sv').write_text(tb)
    frozen = {p.name: sha(p) for p in inputs.iterdir() if p.is_file()}
    (out / 'provenance.json.gz').write_bytes(gzip.compress(
        (json.dumps(dict(scope=__doc__, inputs=frozen, parent=parent,
                         host_clock_hz=57142857, hardware_changed=False), indent=2) + '\n').encode(), mtime=0))
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[], results={})

    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            process = subprocess.Popen(command, cwd=inputs, stdout=log, stderr=subprocess.STDOUT)
            action = dict(label=label, pid=process.pid, command=command, started=time.time())
            state['actions'].append(action)
            save()
            rc = process.wait()
            action.update(returncode=rc, finished=time.time())
            save()
        return rc

    def case(label, enabled, source=None, rejection=None, capture=1, clock_hz=57142857, early=0):
        obj = out / ('obj-' + label)
        command = ['verilator', '--binary', '--timing', '--autoflush', '-j', '4',
                   '+define+DORADO_CORRECT_READ_CAPTURE', '--top-module', 'tb_storage_sdram_burst',
                   '-Wno-fatal', '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC', '-Wno-WIDTHEXPAND',
                   '-Wno-REALCVT', '-Wno-PINMISSING', '-Wno-DEFPARAM',
                   '-GCLOCK_HZ=' + str(clock_hz), '-GRESPONSE_OVERLAP=' + str(enabled), '-GCAPTURE_READ_WORD=' + str(capture), '-GEARLY_READ_RESPONSE=' + str(early),
                   '--Mdir', str(obj), '-o', 'test-response',
                   str(inputs / 'dorado_storage_sdram.sv'),
                   str(source or inputs / 'dorado_storage_sdram_burst.sv'),
                   str(inputs / 'tb.sv'), str(inputs / 'xsds_model.sv')]
        assert execute(command, label + '-compile') == 0, label
        rc = execute([str(obj / 'test-response')], label + '-test')
        log = (out / (label + '-test.log')).read_text()
        if rejection:
            assert rc != 0 and rejection in log, (label, rc, log[-1200:])
            state['results'][label] = dict(rejected=True, diagnostic=rejection)
        else:
            assert rc == 0 and 'STORAGE-SDRAM-BURST PASS' in log, (label, rc, log[-1200:])
            rows = re.findall(r'RESPONSE_STREAM overlap=(\d+) mode=(\d+) requests=(\d+) host_ticks=(\d+) simultaneous=(\d+) longest=(\d+)', log)
            assert len(rows) == 2
            state['results'][label] = [dict(zip(
                ['enabled', 'mode', 'requests', 'host_ticks', 'simultaneous', 'longest'], map(int, row))) for row in rows]
        save()
        print(label, state['results'][label], flush=True)

    save()
    try:
        case('default-off', 0)
        case('capture-control', 1)
        case('early-on', 1, early=1)
        case('control-66', 1, clock_hz=66666667)
        case('early-66', 1, clock_hz=66666667, early=1)
        if controls:
            text = (inputs / 'dorado_storage_sdram_burst.sv').read_text()
            old = '(!response_pending || (RESPONSE_OVERLAP && rsp_ready))'
            assert text.count(old) == 1
            path = out / 'accept-through-held-response.sv'
            path.write_text(text.replace(old, '(!response_pending || RESPONSE_OVERLAP)'))
            case('accept-through-held-response', 1, path, 'response not held', capture=1)
            old='if(fill_beat==address[2:0])begin'
            assert text.count(old)==1
            path=out/'wrong-selected-word.sv'
            path.write_text(text.replace(old,"if(fill_beat==((address[2:0]+3'd1)&3'd7))begin"))
            case('wrong-selected-word',1,path,'read ',early=1)
            old='if(EARLY_READ_RESPONSE && !aborted && !reset)'
            assert text.count(old)==1
            path=out/'reply-after-abort.sv'
            path.write_text(text.replace(old,'if(EARLY_READ_RESPONSE && !reset)'))
            case('reply-after-abort',1,path,'orphan burst response after reset',early=1)
        assert all(sha(inputs / name) == digest for name, digest in frozen.items())
        # Cache-hit streams retain throughput. Mixed-stream changes are
        # recorded, not forced to improve under a host-tick-dependent receiver.
        for control,candidate in [('capture-control','early-on'),('control-66','early-66')]:
            assert state['results'][control][0]==state['results'][candidate][0]
        state.update(status='PASS_EARLY_SELECTED_READ_PHYSICAL_PINS',finished=time.time(),
                     frozen_inputs_unchanged=True,physical_clocks_tested=[57142857,66666667],
                     native_fit_or_firmware_qualified=False)
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generated', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--controls', action='store_true')
    args = parser.parse_args()
    run(args.generated.resolve(), args.output.resolve(), args.controls)
