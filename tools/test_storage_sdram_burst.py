#!/usr/bin/env python3
"""Test the isolated raw BL8 buffer against independent physical SDRAM models.

Freeze every input and record each process. Existing native boot runs, the
qualified BL1 backend and FPGA/Main/daemon are not changed by this experiment.
Bus workload timings are component measurements, not native instruction speed.
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

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(out, controls,correct_read_capture=False):
    assert not out.exists(), 'Observe the existing recorded run'
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    files = ['fpga/mister/dorado_storage_sdram.sv',
             'fpga/mister/dorado_storage_sdram_burst.sv',
             'verilog/verilator/tb_storage_sdram_burst.sv',
             'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
             'tools/' + Path(__file__).name]
    for name in files:
        p = inputs / name
        p.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / name, p)
    if correct_read_capture:
        from stage_storage_read_capture import correct_capture
        for name in files[:2]:
            path=inputs/name;path.write_bytes(correct_capture(path.read_bytes()))
        helper=inputs/'tools/stage_storage_read_capture.py';helper.write_bytes((ROOT/'tools/stage_storage_read_capture.py').read_bytes());files.append('tools/stage_storage_read_capture.py')
    provenance = dict(scope=__doc__, inputs={n: sha(inputs / n) for n in files},
                      native_qualified=False, hardware_qualified=False,read_capture_corrected=correct_read_capture)
    (out / 'provenance.json.gz').write_bytes(gzip.compress(
        (json.dumps(provenance, indent=2) + '\n').encode(), mtime=0))
    state = dict(status='RUNNING', driver_pid=os.getpid(), actions=[], started=time.time())
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            p = subprocess.Popen(command, cwd=out, stdout=log, stderr=subprocess.STDOUT)
            a = dict(stage=label, pid=p.pid, command=command, started=time.time())
            state['actions'].append(a)
            save()
            code = p.wait()
            a.update(returncode=code, finished=time.time())
            save()
            return code
    def case(label, hz=50000000, delay=0, enabled=1, lines=4, source=None, model=None, diagnostic=None):
        obj = out / ('obj-' + label)
        command = ['verilator', '--binary', '--timing', '--autoflush', '-j', '4',
                   '--top-module', 'tb_storage_sdram_burst', '-Wno-fatal',
                   '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC', '-Wno-WIDTHEXPAND',
                   '-Wno-REALCVT', '-Wno-PINMISSING', '-Wno-DEFPARAM',
                   '-GCLOCK_HZ=' + str(hz), '-GCLOCK_DELAY_NS=' + str(delay),
                   '-GUSE_BURST=' + str(enabled), '-GCACHE_LINES=' + str(lines),
                   '--Mdir', str(obj), '-o', 'test-burst',
                   str(inputs / files[0]), str(source or inputs / files[1]),
                   str(inputs / files[2]), str(model or inputs / files[3])]
        if correct_read_capture:command.insert(1,'+define+DORADO_CORRECT_READ_CAPTURE')
        assert execute(command, label + '-compile') == 0, label
        code = execute([str(obj / 'test-burst')], label + '-test')
        log = (out / (label + '-test.log')).read_text()
        if diagnostic:
            assert code != 0 and diagnostic.lower() in log.lower(), (label, code, log[-1000:])
        else:
            assert code == 0 and 'STORAGE-SDRAM-BURST PASS' in log, (label, code, log[-1000:])
        print(label, 'expected runtime rejection' if diagnostic else 'PASS', flush=True)
    save()
    try:
        case('baseline', enabled=0)
        for lines in [1, 2, 4]:
            case('burst-lines' + str(lines), lines=lines)
        case('burst-late-clock', delay=6)
        for hz in [20000000, 33333333]:
            case('burst-clock' + str(hz), hz=hz)
        if controls:
            source = (inputs / files[1]).read_text()
            for label, old, new, diagnostic, delay in [
                ('stale-write', "if(line_valid[i] && line_tag[i]==23'(26'(req_addr)>>3))line_valid[i]<=0;",
                 "if(1'b0)line_valid[i]<=0;", 'read ', 0),
                ('orphan-fill', 'if(!aborted && !reset)begin', 'if(!reset)begin', 'orphan burst', 0),
                ('early-capture', 'delay_count<=2;state<=READ_WAIT;' if correct_read_capture else 'delay_count<=3;state<=READ_WAIT;',
                 'delay_count<=1;state<=READ_WAIT;' if correct_read_capture else 'delay_count<=2;state<=READ_WAIT;', 'read ', 6),
                ('single-beat-mode', "MODE_WORD=13'h223", "MODE_WORD=13'h220", 'read ', 0),
                ('short-rcd', 'T_RCD=clocks(21)', 'T_RCD=1', 'tRCD', 0),
            ]:
                assert source.count(old) == 1, (label, old)
                p = out / (label + '.sv')
                p.write_text(source.replace(old, new))
                case(label, source=p, diagnostic=diagnostic, delay=delay)
            model = (inputs / files[3]).read_text()
            start = model.index('                    // Natural read completion retires the sequencer only.')
            end = model.index("                    burst.is_write = 1'b0;", start)
            end += len("                    burst.is_write = 1'b0;")
            old_model = model[:start] + '                    stop_burst();' + model[end:]
            p = out / 'old-model-hold-glitch.sv'
            p.write_text(old_model)
            case('old-model-hold-glitch', model=p, diagnostic='read output hold tOH')
        state.update(status='PASS_ISOLATED_SDRAM_BURST_PIN_MODEL', finished=time.time(),
                     native_qualified=False, hardware_qualified=False)
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--controls', action='store_true')
    p.add_argument('--correct-read-capture',action='store_true')
    a = p.parse_args()
    run(a.output.resolve(), a.controls,a.correct_read_capture)
