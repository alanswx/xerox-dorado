#!/usr/bin/env python3
"""Unchanged original/physical-state/write-window oracle on shared-write F10145."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_f10145_shared_write as gen
import test_f10145_snapshot as qualified

HERE = Path(__file__).resolve().parent


def run(output):
    output.mkdir(parents=True, exist_ok=False)
    inputs = output/'executed-inputs'
    inputs.mkdir()
    dependencies = [HERE/n for n in ('generate_f10145_shared_write.py', 'generate_f10145.py',
                                    'test_f10145_shared_write.py', 'test_f10145_snapshot.py',
                                    'test_f10145_snapshot.cpp')]+[gen.ORIGINAL]
    for p in dependencies:
        (inputs/p.name).write_bytes(p.read_bytes())
    text, manifest = gen.generate()
    cases = [('candidate', None, None, None),
             ('restore_alias', 'state_hold ? state_index[3:0] : a_lat',
              "state_hold ? 4'b0 : a_lat", 'RAM bit walk differs'),
             ('current_address', 'state_hold ? state_index[3:0] : a_lat',
              'state_hold ? state_index[3:0] : a', 'ordinary oracle state differs'),
             ('ungated_write', '!state_hold && state_advance && p13',
              '!state_hold && p13', 'held guest state changed'),
             ('lost_ce', 'p13 && !we_d && ce_lat;',
              'p13 && !we_d;', 'ordinary oracle state differs'),
             ('running_access', 'state_hold && state_frozen && state_request',
              'state_request', 'running request accepted'),
             ('entry_access', 'state_hold && state_frozen && state_request',
              'state_hold && state_request', 'entry request accepted'),
             ('invalid_access', "state_index <= 5'd16", "1'b1", 'invalid request accepted'),
             ('window_restore', '{ce_lat,d_lat,a_lat,we_d} <= state_data;',
              "{ce_lat,d_lat,a_lat,we_d} <= 10'b0;", 'write-window bit walk differs')]
    records = []
    for name, before, after, expected in cases:
        case = output/name
        case.mkdir()
        candidate = text
        if before:
            assert candidate.count(before) == 1, (name, before)
            candidate = candidate.replace(before, after, 1)
        (case/'fixture.sv').write_text(qualified.FIXTURE)
        (case/'cell.sv').write_text(candidate)
        command = ['verilator', '--cc', '--exe', '--build', '-j', '4',
                   '--top-module', 'f10145_snapshot_fixture', '-Wno-fatal', '-DDORADO_F10145_LOGIC',
                   '--Mdir', str(case/'obj'), str(case/'fixture.sv'), str(case/'cell.sv'),
                   str(inputs/'cell_F10145A.v'), str(inputs/'test_f10145_snapshot.cpp')]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as f:
            subprocess.run(command, stdout=f, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vf10145_snapshot_fixture')], capture_output=True, text=True, timeout=60)
        diagnostic = result.stdout+result.stderr
        (case/'test.txt').write_text(diagnostic)
        print(name+': '+diagnostic.strip(), flush=True)
        if expected:
            assert result.returncode and expected in diagnostic, (name, diagnostic)
        else:
            assert not result.returncode and 'F10145 SNAPSHOT PASS' in diagnostic, diagnostic
        records.append(dict(case=name, returncode=result.returncode, expected_failure=expected))
    (output/'results.json').write_text(json.dumps(dict(status='PASS', manifest=manifest, cases=records,
        source_hashes={str(p.relative_to(gen.ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                       for p in dependencies}, scope=__doc__), indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
