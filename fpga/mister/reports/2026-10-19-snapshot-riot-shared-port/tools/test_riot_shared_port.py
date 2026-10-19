#!/usr/bin/env python3
"""Reuse the original/independent full RIOT oracle on shared-port RTL."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_riot_shared_port as gen
import test_riot as qualified

HERE = Path(__file__).resolve().parent


def run(output):
    output.mkdir(parents=True, exist_ok=False)
    text, manifest = gen.generate()
    inputs = output/'executed-inputs'
    inputs.mkdir()
    dependencies = [HERE/n for n in ('generate_riot_shared_port.py', 'generate_riot.py',
                                    'test_riot_shared_port.py', 'test_riot.py', 'test_riot.cpp')]
    dependencies += [gen.SOURCE]
    for p in dependencies:
        (inputs/p.name).write_bytes(p.read_bytes())
    cases = [('candidate', None, None, None),
             ('admin_alias', 'state_hold ? state_index[6:0] : addr',
              'state_hold ? 7\'b0 : addr', 'RIOT state oracle differs'),
             ('guest_write_enable', 'ram_normal && res_n && ce && CS1',
              'ram_normal && res_n && CS1', 'RIOT state oracle differs'),
             ('reset_enable', 'ram_normal && !res_n',
              'ram_normal && ce && !res_n', 'RIOT state oracle differs'),
             ('held_reset', 'ram_normal && !res_n',
              '!res_n', 'RIOT state oracle differs'),
             ('running_access', 'state_hold && state_frozen && state_request',
              'state_request', 'request handshake differs'),
             ('lost_pa7', 'old_pa7<=state_data[0:0]',
              "old_pa7<=1'b0", 'RIOT state oracle differs')]
    records = []
    for name, before, after, expected in cases:
        case = output/name
        case.mkdir()
        candidate = text
        if before:
            assert candidate.count(before) == 1, (name, before)
            candidate = candidate.replace(before, after, 1)
        (case/'fixture.sv').write_text(qualified.FIXTURE)
        (case/'M6532_snapshot.sv').write_text(candidate)
        command = ['verilator', '--cc', '--exe', '--build', '-j', '4',
                   '--top-module', 'riot_fixture', '-Wno-fatal', '--Mdir', str(case/'obj'),
                   str(case/'fixture.sv'), str(case/'M6532_snapshot.sv'),
                   str(inputs/'M6532.sv'), str(inputs/'test_riot.cpp')]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as f:
            subprocess.run(command, stdout=f, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vriot_fixture')], capture_output=True, text=True, timeout=60)
        diagnostic = result.stdout+result.stderr
        (case/'test.txt').write_text(diagnostic)
        print(name+': '+diagnostic.strip(), flush=True)
        if expected:
            assert result.returncode and expected in diagnostic, (name, diagnostic)
        else:
            assert not result.returncode and 'RIOT PASS' in diagnostic, diagnostic
        records.append(dict(case=name, returncode=result.returncode, expected_failure=expected))
    (output/'results.json').write_text(json.dumps(dict(status='PASS', manifest=manifest, cases=records,
        scope='Same complete physical1097bit/unchanged original/independent oracle gate; shared RAM ports only; not routed area or full-core qualification.',
        source_hashes={str(p.relative_to(gen.ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                       for p in dependencies}), indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    args = p.parse_args()
    run(args.output.resolve())
