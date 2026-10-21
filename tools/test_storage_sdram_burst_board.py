#!/usr/bin/env python3
"""Compare complete native MSA storage through BL1 and BL8 physical backends.

Use the exact frozen native MSA/group/arbiter/cell sources and the independent
original 144-chip board oracle. Only the external raw backend differs. This
is one-board memory qualification, not a native firmware or speed gate.
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

from sil_storage_ddram import storage64_frontend

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(native, out,correct_read_capture=False):
    assert not out.exists(), 'Observe the recorded existing run'
    record = json.loads(gzip.decompress((native / 'provenance.json.gz').read_bytes()))
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    files = {
        'msa_ddram.v': 'src/build/rtl-executive/native16-tx-cable/msa_ddram.v',
        'msa14.v': 'src/verilog/generated/msa.v',
        'dorado_storage_group.sv': 'src/fpga/mister/dorado_storage_group.sv',
        'dorado_storage_arb.sv': 'src/fpga/mister/dorado_storage_arb.sv',
        'dorado_storage_sdram.sv': 'src/fpga/mister/dorado_storage_sdram.sv',
    }
    inputs.mkdir()
    native_hashes = {}
    for name, relative in files.items():
        source = native / 'inputs' / relative
        assert sha(source) == record['inputs'][relative], relative
        shutil.copy2(source, inputs / name)
        native_hashes[relative] = sha(source)
    cells = inputs / 'cells'
    cells.mkdir()
    for source in (native / 'inputs/src/verilog/cells').iterdir():
        if source.is_file():
            relative = 'src/verilog/cells/' + source.name
            assert sha(source) == record['inputs'][relative], relative
            shutil.copy2(source, cells / source.name)
            native_hashes[relative] = sha(source)
    (inputs / 'msa.v').write_text(storage64_frontend((inputs / 'msa14.v').read_text()))
    for name, source in {
        'dorado_storage_sdram_burst.sv': 'fpga/mister/dorado_storage_sdram_burst.sv',
        'xsds_model.sv': 'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv',
        'original-tb.sv': 'verilog/verilator/tb_storage_board.sv',
        'original-fixture.sv': 'verilog/verilator/fixtures/xsds_storage.sv',
        'sil_storage_ddram.py': 'tools/sil_storage_ddram.py',
        Path(__file__).name: 'tools/' + Path(__file__).name,
    }.items():
        shutil.copy2(ROOT / source, inputs / name)
    if correct_read_capture:
        from stage_storage_read_capture import correct_capture
        for n in ['dorado_storage_sdram.sv','dorado_storage_sdram_burst.sv']:
            f=inputs/n;f.write_bytes(correct_capture(f.read_bytes()))
        shutil.copy2(ROOT/'tools/stage_storage_read_capture.py',inputs/'stage_storage_read_capture.py')
    tb = (inputs / 'original-tb.sv').read_text()
    guard = ('    initial if(external_memory.u_storage_0.DEFER_READS!=DEFER_READS)\n'
             '        $fatal(1,"testbench/generator deferred-read options differ");\n')
    assert tb.count(guard) == 1 and 'DEFER_READS' not in (inputs / 'dorado_storage_group.sv').read_text()
    (inputs / 'tb.sv').write_text(tb.replace(guard, '    // Exact native group has no deferred-read option.\n'))
    fixture = (inputs / 'original-fixture.sv').read_text()
    (inputs / 'baseline-fixture.sv').write_text(fixture)
    assert fixture.count('dorado_storage_sdram #(') == 1
    (inputs / 'burst-fixture.sv').write_text(fixture.replace('dorado_storage_sdram #(', 'dorado_storage_sdram_burst #('))
    provenance = dict(scope=__doc__, native_inputs=native_hashes,
                      inputs={str(p.relative_to(inputs)): sha(p) for p in inputs.rglob('*') if p.is_file()},
                      original_board_address_bits=16, raw_group_count=16,
                      oracle_power_on='Fixture explicitly defines unwritten physical cells as zero',
                      native_qualified=False,read_capture_corrected=correct_read_capture)
    (out / 'provenance.json.gz').write_bytes(gzip.compress((json.dumps(provenance, indent=2) + '\n').encode(), mtime=0))
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[])
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            p = subprocess.Popen(command, cwd=inputs, stdout=log, stderr=subprocess.STDOUT)
            action = dict(stage=label, pid=p.pid, command=command, started=time.time())
            state['actions'].append(action)
            save()
            code = p.wait()
            action.update(returncode=code, finished=time.time())
            save()
            assert code == 0, (label, code)
    save()
    try:
        for label in ['baseline', 'burst']:
            obj = out / ('obj-' + label)
            execute(['verilator', '--binary', '--timing', '--autoflush', '-j', '4',
                     '--top-module', 'tb_storage_board', '-GADDR_BITS=16',
                     '-GPREFETCH_NEXT=1', '-GINTERLEAVE_GROUPS=1', '-DSTORAGE_USE_SDRAM',
                     '-Wno-fatal', '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC', '-Wno-WIDTHEXPAND',
                     '-Wno-REALCVT', '-I' + str(cells), '--Mdir', str(obj), '-o', 'board-test',
                     *[str(inputs / name) for name in ['msa_ddram.v', 'msa.v', 'dorado_storage_group.sv',
                       'dorado_storage_arb.sv', 'dorado_storage_sdram.sv', 'dorado_storage_sdram_burst.sv',
                       'xsds_model.sv', 'tb.sv', label + '-fixture.sv']]], label + '-compile')
            execute([str(obj / 'board-test'), '+raw_trace=' + str(out / (label + '-raw.csv'))], label + '-test')
            text = (out / (label + '-test.log')).read_text()
            assert 'STORAGE-BOARD PASS:' in text, text[-1000:]
            print(label, text[text.index('STORAGE-BOARD PASS:'):].splitlines()[0], flush=True)
        state.update(status='PASS_NATIVE_MSA_BL1_BL8_PHYSICAL_MODEL', finished=time.time(), native_qualified=False)
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--native', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--correct-read-capture',action='store_true')
    a = p.parse_args()
    run(a.native.resolve(), a.output.resolve(),a.correct_read_capture)
