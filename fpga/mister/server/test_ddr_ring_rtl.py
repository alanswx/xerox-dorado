#!/usr/bin/env python3
"""Actual C mailbox library against the RTL engine and independent DDR/wire oracles."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def run(output, mutations):
    output.mkdir(parents=True, exist_ok=True)
    cc = os.environ.get('CC', 'cc')
    source = ROOT/'fpga/mister/dorado_ethernet_ddr.sv'
    host = output/'host.o'
    subprocess.run([cc, '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', '-c',
                    str(HERE/'dorado_ddr_ring.c'), '-o', str(host)], check=True)
    original = source.read_text()
    cases = [('zero', original, 0, None), ('wrap', original, 0xfffffff0, None)]
    if mutations:
        changes = [
            ('bad_tx_byte_order', '{tx_data[7:0],tx_data[15:8]}', 'tx_data', 'TX content/length/order'),
            ('bad_rx_epoch', 'if(read_data!=active_epoch)protocol_fault(5);',
             'if(1\'b0)protocol_fault(5);', 'unsolicited/busy wire commit'),
            ('false_stop', 'STOP_MAGIC:write_word(0,0,8\'hff,STOP_STATUS);',
             'STOP_MAGIC:begin state<=STOPPED;end', 'stopped before invalidating magic'),
        ]
        for name, before, after, diagnostic in changes:
            assert original.count(before) == 1, name
            cases.append((name, original.replace(before, after), 0xfffffff0, diagnostic))
    results = []
    for name, text, initial, diagnostic in cases:
        case = output/name
        case.mkdir(exist_ok=True)
        sv = case/'dorado_ethernet_ddr.sv'
        sv.write_text(text)
        command = ['verilator', '--cc', '--exe', '--build', '-j', '4',
                   '--top-module', 'dorado_ethernet_ddr', '-Wno-fatal', '-Wno-PROCASSINIT',
                   '-GPOLL_CYCLES=4', f'-GINITIAL_COUNT={initial}', '--Mdir', str(case/'obj'),
                   str(sv), str(HERE/'test_ddr_ring_rtl.cpp'),
                   '-CFLAGS', f'-std=c++17 -I{HERE} -DDDR_INITIAL_COUNT={initial}u',
                   '-LDFLAGS', str(host)]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vdorado_ethernet_ddr')], text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=20)
        (case/'test.txt').write_text(result.stdout)
        if diagnostic:
            assert result.returncode and diagnostic in result.stdout, (name, result.returncode, result.stdout)
        else:
            assert not result.returncode, (name, result.stdout)
            assert all(s in result.stdout for s in ('TRAFFIC PASS', 'STOP PASS', 'FAULT PASS'))
        results.append(dict(case=name, initial_count=initial, returncode=result.returncode,
                            expected_failure=diagnostic, source_sha256=hashlib.sha256(text.encode()).hexdigest()))
        print(name+': '+result.stdout.strip(), flush=True)
    (output/'results.json').write_text(json.dumps(results, indent=2)+'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    if args.output:
        run(args.output.resolve(), args.mutations)
    else:
        with tempfile.TemporaryDirectory(prefix='dorado-ddr-ring-', dir='/tmp') as tmp:
            run(Path(tmp), args.mutations)
