#!/usr/bin/env python3
"""Corrupt actual direct-carry evidence and reject it before writing a native stage."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

from stage_direct_carry_capture_native import stage


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(config, output):
    assert not output.exists()
    args = {n: Path(v) for n, v in json.loads(config.read_text()).items() if n != 'output'}
    output.mkdir(parents=True)
    original = args['direct_alu']
    selected = [p for p in original.rglob('*') if p.is_file()
                and not any(n.startswith('obj-') for n in p.relative_to(original).parts)
                and (p.relative_to(original).parts[0] == 'inputs' or p.suffix in ['.json', '.log', '.v'])]
    frozen = {p: sha(p) for p in selected}
    fixture = output / 'direct-alu'
    for p in selected:
        target = fixture / p.relative_to(original)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(p, target)
    local = dict(args, direct_alu=fixture)
    positive = stage(**local, output=output / 'positive')
    assert len(positive['source_hashes']) == 503 and len(positive['modified_RTL_sources']) == 3
    assert positive['ALU_direct_carry'] and not positive['assembly_permitted']

    def update(name, transform):
        p = fixture / name
        value = json.loads(p.read_text())
        transform(value)
        p.write_text(json.dumps(value, indent=2) + '\n')

    def mutate(label):
        if label == 'failed-result':
            update('run-state.json', lambda j: j.update(status='TERMINAL_FAILED'))
        elif label == 'missing-binary-case':
            update('provenance.json', lambda j: j.update(binary_cases=16383))
        elif label == 'changed-candidate':
            with (fixture / 'inputs/candidate.v').open('a') as f:
                f.write('\n// stale candidate\n')
        elif label == 'rebound-wrong-candidate':
            p = fixture / 'inputs/candidate.v'
            s = p.read_text()
            needle = 'group_generate | (group_propagate&Cn)'
            assert s.count(needle) == 1
            p.write_text(s.replace(needle, 'group_generate'))
            update('provenance.json', lambda j: j['inputs'].update({'candidate.v': sha(p)}))
        elif label == 'rebound-wrong-reference':
            p = fixture / 'inputs/reference.v'
            with p.open('a') as f:
                f.write('\n// changed original reference\n')
            update('provenance.json', lambda j: j['inputs'].update({'reference.v': sha(p)}))
        elif label == 'nonrejecting-negative':
            update('run-state.json', lambda j: j['results']['verilator-wrong-GG'].update(rejected=False))
        elif label == 'missing-fatal-loop-check':
            update('run-state.json', lambda j:
                   [a['command'].remove('-Werror-UNOPTFLAT') for a in j['actions']
                    if a['label'] == 'verilator-positive-compile'])
        elif label == 'actual-circular-warning':
            with (fixture / 'verilator-positive-compile.log').open('a') as f:
                f.write('\n%Warning-UNOPTFLAT: actual circular logic\n')
        elif label == 'changed-stimulus':
            with (fixture / 'inputs/tb.sv').open('a') as f:
                f.write('\n// changed executed stimulus\n')
        elif label == 'false-four-state-claim':
            update('provenance.json', lambda j: j.update(four_state_equivalent=True))
        else:
            raise ValueError(label)

    rows = []
    for label in ['failed-result', 'missing-binary-case', 'changed-candidate',
                  'rebound-wrong-candidate', 'rebound-wrong-reference', 'nonrejecting-negative',
                  'missing-fatal-loop-check', 'actual-circular-warning', 'changed-stimulus',
                  'false-four-state-claim']:
        before = {p: p.read_bytes() for p in fixture.rglob('*') if p.is_file()}
        mutate(label)
        rejected = False
        try:
            stage(**local, output=output / label)
        except (AssertionError, KeyError):
            rejected = True
        assert rejected and not (output / label).exists(), label
        rows.append(dict(name=label, rejected=True))
        for p, raw in before.items():
            if p.read_bytes() != raw:
                p.write_bytes(raw)
        print(label, 'rejected', flush=True)
    assert all(sha(p) == h for p, h in frozen.items())
    result = dict(status='PASS_DIRECT_CARRY_CAPTURE_STAGE_TEN_REJECTION_CONTROLS', cases=rows,
                  original_inputs_unchanged=True, source_count=503, RTL_changes=3,
                  stage_tool_sha256=sha(Path(__file__).with_name('stage_direct_carry_capture_native.py')))
    (output / 'controls.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(run(args.config.resolve(), args.output.resolve())['status'])
