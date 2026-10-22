#!/usr/bin/env python3
"""Reject stale writes, orphan fills, group aliases and wrong capture at16 lines."""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(positive, out):
    result = json.loads((positive / 'result.json').read_text())
    assert result['status'] == 'PASS_NATIVE_SDRAM_CAPACITY_COMPONENT'
    provenance = json.loads(gzip.decompress((positive / 'provenance.json.gz').read_bytes()))
    for name, h in provenance['frozen_inputs'].items():
        assert sha(positive / 'inputs' / name) == h, name
    assert not out.exists(), 'Observe the recorded control run'
    out.mkdir(parents=True)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[], cases=[])

    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            p = subprocess.Popen(command, cwd=out, stdout=log, stderr=subprocess.STDOUT)
            row = dict(stage=label, pid=p.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            code = p.wait()
            row.update(returncode=code, finished=time.time())
            save()
            return code

    source = (positive / 'inputs/dorado_storage_sdram_burst.sv').read_text()
    stale = "if(line_valid[i] && line_tag[i]==23'(26'(req_addr)>>3))line_valid[i]<=0;"
    assert source.count(stale) == 1
    assert source.count('if(!aborted && !reset)begin') == 1
    assert source.count("26'(req_addr)>>3") == 2
    assert source.count('line_tag[fill_line]<=address[25:3];') == 1
    assert source.count('delay_count<=2;state<=READ_WAIT;') == 1
    variants = {
        'unmutated-harness': (source, None),
        'stale-write': (source.replace(stale, "if(1'b0)line_valid[i]<=0;"), 'read '),
        'orphan-fill': (source.replace('if(!aborted && !reset)begin', 'if(!reset)begin'), 'orphan burst'),
        'aliased-group-bit': (source.replace("26'(req_addr)>>3", "26'(req_addr)>>4")
                             .replace('line_tag[fill_line]<=address[25:3];',
                                      'line_tag[fill_line]<=address[25:4];'), 'read '),
        'early-capture': (source.replace('delay_count<=2;state<=READ_WAIT;',
                                        'delay_count<=1;state<=READ_WAIT;'), 'read '),
    }
    actions = json.loads((positive / 'run-state.json').read_text())['actions']
    template = next(a['command'] for a in actions if a['stage'] == 'lines16-compile')
    save()
    try:
        for name, (text, diagnostic) in variants.items():
            inputs = out / name / 'inputs'
            shutil.copytree(positive / 'inputs', inputs)
            # Check actual stale data before the capacity workload can detect
            # the missing invalidation through its physical-read count alone.
            tb = inputs / 'tb_capacity.sv'
            anchor = '        // Geometry-derived capacity probe, before the unchanged coherence suite.\n'
            body = tb.read_text()
            assert body.count(anchor) == 1
            body = body.replace(anchor, '''        // Focused write/read oracle, also run against unmutated RTL.
        seed_line(a0);transfer(0,a0,0);
        transfer(1,a0+3,16'h369c);transfer(0,a0+3,0);
''' + anchor)
            tb.write_text(body)
            mutated = inputs / 'dorado_storage_sdram_burst.sv'
            mutated.write_text(text)
            obj = out / name / 'obj'
            command = [arg.replace(str(positive / 'inputs'), str(inputs))
                       .replace(str(positive / 'obj-lines16'), str(obj)) for arg in template]
            assert execute(command, name + '-compile') == 0
            rc = execute([str(obj / 'test-capacity')], name + '-test')
            log = (out / (name + '-test.log')).read_text()
            if diagnostic is None:
                assert rc == 0 and 'STORAGE-SDRAM-BURST PASS' in log, (name, rc, log[-1500:])
            else:
                assert rc != 0 and diagnostic in log, (name, rc, log[-1500:])
            state['cases'].append(dict(name=name, rejected=diagnostic is not None, returncode=rc,
                                      mutated_source_sha256=sha(mutated), testbench_sha256=sha(tb),
                                      diagnostic=diagnostic, unmutated_pass=diagnostic is None))
            save()
            print(name, 'PASS' if diagnostic is None else 'rejected', flush=True)
        for name, h in provenance['frozen_inputs'].items():
            assert sha(positive / 'inputs' / name) == h, name
        state.update(status='PASS_NATIVE_SDRAM_CAPACITY_REJECTION_CONTROLS', finished=time.time(),
                     positive_result_sha256=sha(positive / 'result.json'),
                     positive_inputs_unchanged=True, FPGA_or_running_core_changed=False)
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--positive', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.positive.resolve(), a.output.resolve())
