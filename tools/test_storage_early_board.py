#!/usr/bin/env python3
"""Check early selected-word responses through the original144-chip MSA.

Keep every original serial/ECC output and stored-bit check. Compare canonical
operation/data order while allowing response and host-cycle timing to improve
or regress. No full-core/native timing or firmware claim follows.
"""
import argparse
import csv
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(board, generated, out, controls):
    assert not out.exists(), 'Inspect the recorded run instead of overwriting it'
    provenance = json.loads(gzip.decompress((board / 'provenance.json.gz').read_bytes()))
    record = json.loads((generated / 'provenance.json').read_text())
    for name, digest in provenance['inputs'].items():
        assert sha(board / 'inputs' / name) == digest, name
    for name, digest in record['generated_hashes'].items():
        assert sha(generated / name) == digest, name
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    inputs.mkdir()
    for name in provenance['inputs']:
        target = inputs / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(board / 'inputs' / name, target)
    for name in record['generated_hashes']:
        assert sha(inputs / name) == record['parent_hashes'][name], name
        shutil.copyfile(generated / name, inputs / name)
    fixture = (board / 'inputs/lines4-fixture.sv').read_text()
    tb = (inputs / 'tb.sv').read_text()
    # An observational trace must record the OLD completion before latching
    # a simultaneous successor. No DUT, stimulus or original chip is altered.
    a = tb.index('        if(rv && rr)begin\n            trace_address=')
    z = tb.index('    end\n    final if(raw_trace_fd)', a)
    old = tb[a:z]
    marker = '        if(sv && sr && raw_trace_fd)'
    middle = old.index(marker)
    tb = tb[:a] + old[middle:] + old[:middle] + tb[z:]
    frozen = {str(p.relative_to(inputs)): sha(p) for p in inputs.rglob('*') if p.is_file()}
    (out / 'provenance.json.gz').write_bytes(gzip.compress((json.dumps(dict(
        scope=__doc__, parent_board_provenance_sha256=sha(board / 'provenance.json.gz'),
        parent_board=provenance, generated=record, inputs=frozen,
        trace_observer_change='Consume old response before recording simultaneous new request',
        hardware_changed=False), indent=2) + '\n').encode(), mtime=0))
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[], results={})

    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            process = subprocess.Popen(command, cwd=inputs, stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, pid=process.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            code = process.wait()
            row.update(returncode=code, finished=time.time())
            save()
            return code

    def case(label, enabled, source=None, rejected=False, lookahead=None, capture=None, early=0):
        body = tb.replace('.INTERLEAVE_GROUPS(INTERLEAVE_GROUPS)) arb(',
                          '.INTERLEAVE_GROUPS(INTERLEAVE_GROUPS),.RESPONSE_OVERLAP(' + str(enabled) + ')' +
                          (',.REQUEST_LOOKAHEAD(' + str(enabled if lookahead is None else int(lookahead)) + ')'
                           if record.get('request_lookahead', False) else '') + ') arb(')
        assert body != tb
        test = inputs / (label + '-tb.sv')
        test.write_text(body)
        old = '.CACHE_LINES(4)) dut('
        assert fixture.count(old) == 1
        f = inputs / (label + '-fixture.sv')
        f.write_text(fixture.replace(old, '.CACHE_LINES(4),.RESPONSE_OVERLAP(' + str(enabled) + '),.CAPTURE_READ_WORD(' + str(enabled if capture is None else int(capture)) + '),.EARLY_READ_RESPONSE(' + str(early) + ')) dut('))
        obj = out / ('obj-' + label)
        names = ['msa_ddram.v', 'msa.v', 'dorado_storage_group.sv',
                 'dorado_storage_sdram.sv', 'dorado_storage_sdram_burst.sv', 'dorado_storage_addr_preview.sv',
                 'xsds_model.sv']
        command = ['verilator', '--binary', '--timing', '--autoflush', '-j', '4',
                   '--top-module', 'tb_storage_board', '-GADDR_BITS=16',
                   '-GPREFETCH_NEXT=1', '-GINTERLEAVE_GROUPS=1', '-DSTORAGE_USE_SDRAM',
                   '-Wno-fatal', '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC', '-Wno-WIDTHEXPAND',
                   '-Wno-REALCVT', '-I' + str(inputs / 'cells'), '--Mdir', str(obj), '-o', 'test-board',
                   *[str(inputs / name) for name in names],
                   str(source or inputs / 'dorado_storage_arb.sv'), str(test), str(f)]
        assert execute(command, label + '-compile') == 0, label
        code = execute([str(obj / 'test-board'), '+raw_trace=' + str(out / (label + '-raw.csv'))], label + '-test')
        log = (out / (label + '-test.log')).read_text()
        if rejected:
            assert code != 0, (label, log[-1000:])
            state['results'][label] = dict(rejected=True, returncode=code, diagnostic=log[-1000:])
        else:
            line = next((s for s in log.splitlines() if 'STORAGE-BOARD PASS:' in s), None)
            assert code == 0 and line, (label, code, log[-1800:])
            match = re.search(r'host_cycles=(\d+)', line)
            assert match
            state['results'][label] = dict(pass_line=line, host_cycles=int(match[1]))
        save()
        print(label, state['results'][label], flush=True)

    save()
    try:
        case('default-off', 0)
        assert (out / 'default-off-raw.csv').read_bytes() == (board / 'lines4-raw.csv').read_bytes(), 'Default-off changed frozen trace'
        if record.get('request_lookahead', False):
            case('overlap-control', 1, lookahead=False)
        case('capture-control', 1, lookahead=True, capture=False)
        case('early-on', 1, early=1)
        def operations(path):
            with path.open() as f:
                return [tuple(row[1:]) for row in list(csv.reader(f))[1:]]
        assert operations(out / 'default-off-raw.csv') == operations(out / 'early-on-raw.csv'), 'Canonical raw data/order changed'
        if record.get('request_lookahead', False):
            assert operations(out / 'overlap-control-raw.csv') == operations(out / 'early-on-raw.csv'), 'Lookahead changed canonical raw data/order'
        if controls:
            text = (inputs / 'dorado_storage_arb.sv').read_text()
            selected = 'successor' if record.get('request_lookahead', False) else 'chosen'
            old = f'owner<={selected}_owner;address_latched<={selected}_address;'
            assert text.count(old) == 1
            bad = out / 'old-response-owner.sv'
            bad.write_text(text.replace(old, f'owner<=owner;address_latched<={selected}_address;'))
            case('old-response-owner', 1, source=bad, rejected=True)
        assert all(sha(inputs / n) == h for n, h in frozen.items())
        off, on = (state['results'][n]['host_cycles'] for n in ['default-off', 'early-on'])
        assert on < off
        assert operations(out/'capture-control-raw.csv')==operations(out/'early-on-raw.csv'), 'Early response changed raw operation/data order'
        state.update(status='PASS_EARLY_SELECTED_READ_ORIGINAL_MSA', finished=time.time(),
                     default_off_trace_byte_identical=True, canonical_operation_data_order_identical=True,
                     host_cycle_reduction=1-on/off, native_fit_or_firmware_qualified=False)
        if record.get('request_lookahead', False):
            control = state['results']['overlap-control']['host_cycles']
            state['early_vs_qualified_overlap_host_cycle_change'] = on/control-1
            state['early_vs_capture_control_host_cycle_change'] = on/state['results']['capture-control']['host_cycles']-1
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['board', 'generated', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--controls', action='store_true')
    args = parser.parse_args()
    run(args.board.resolve(), args.generated.resolve(), args.output.resolve(), args.controls)
