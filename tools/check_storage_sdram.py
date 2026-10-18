#!/usr/bin/env python3
"""Qualify the raw SDRAM port against the independent XSDS chip model.

The timing/refresh mutations must compile and fail at runtime. A compile
failure is not evidence that the model detected an invalid controller.
"""
import argparse
import json
from pathlib import Path
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path,
                        default=Path('build/rtl-executive/sdram-qualification'))
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    source = (root / 'fpga/mister/dorado_storage_sdram.sv').read_text()
    controls = []
    for name, changes, diagnostic, clock_delay in [
        ('short-rcd', [('T_RCD=clocks(21)', 'T_RCD=1')], 'tRCD', 0),
        ('blocked-refresh', [('IDLE:if(refresh_due)begin',
         'IDLE:if(refresh_due && !response_pending)begin')], 'refresh', 0),
        ('rising-capture', [('delay_count<=3;state<=READ_WAIT;',
                            'delay_count<=2;state<=READ_WAIT;'),
                           ('rsp_data<=dq_capture;', 'rsp_data<=dq_in;')],
         'read 00000000 got', 6),
    ]:
        mutated = source
        for old, new in changes:
            if mutated.count(old) != 1:
                raise RuntimeError(f'Mutation target changed: {old}')
            mutated = mutated.replace(old, new)
        candidate = out / (name+'.sv')
        candidate.write_text(mutated)
        obj = out / ('obj_'+name)
        log_path = out / (name+'.txt')
        with log_path.open('w') as log:
            result = subprocess.run([
                'make', '-C', 'verilog/verilator', 'storage-sdram-test',
                'SDRAM_TEST_RTL='+str(candidate), 'SDRAM_TEST_OBJ='+str(obj),
                f'SDRAM_TEST_CLOCK_DELAY={clock_delay}',
            ], cwd=root, stdout=log, stderr=subprocess.STDOUT)
        log = log_path.read_text()
        if not (result.returncode != 0 and (obj/'tb_storage_sdram').is_file()
                and 'Verilog $stop' in log and diagnostic.lower() in log.lower()):
            raise RuntimeError(f'{name} was not rejected at runtime; see {log_path}')
        controls.append(dict(control=name, compiled=True, rejected=True,
                             reason=diagnostic))
        print(name, 'runtime rejected as expected', flush=True)
    (out/'negative-controls.json').write_text(json.dumps(controls, indent=2)+'\n')
    for hz in (20000000, 33333333, 50000000):
        log_path = out / f'clock-{hz}.txt'
        with log_path.open('w') as log:
            subprocess.run([
                'make', '-C', 'verilog/verilator', 'storage-sdram-test',
                f'SDRAM_TEST_HZ={hz}', 'SDRAM_TEST_OBJ='+str(out/f'obj_{hz}'),
            ], cwd=root, stdout=log, stderr=subprocess.STDOUT, check=True)
        if 'STORAGE-SDRAM PASS:' not in log_path.read_text():
            raise RuntimeError(f'No completed simulation in {log_path}')
        print(hz, 'pin-model PASS', flush=True)
    log_path = out/'late-clock.txt'
    with log_path.open('w') as log:
        subprocess.run([
            'make', '-C', 'verilog/verilator', 'storage-sdram-test',
            'SDRAM_TEST_HZ=50000000', 'SDRAM_TEST_CLOCK_DELAY=6',
            'SDRAM_TEST_OBJ='+str(out/'obj_late_clock'),
        ], cwd=root, stdout=log, stderr=subprocess.STDOUT, check=True)
    if 'STORAGE-SDRAM PASS:' not in log_path.read_text():
        raise RuntimeError(f'No completed simulation in {log_path}')
    print('50MHz with6ns physical clock delay pin-model PASS', flush=True)


if __name__ == '__main__':
    main()
