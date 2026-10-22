#!/usr/bin/env python3
"""Reject damaged real component/source evidence before native read-capture fitting."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

from stage_storage_read_word_native import stage, BACKEND, WRAPPER, load


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def run(config, output):
    assert not output.exists()
    args = {n: Path(v) for n, v in load(config).items() if n != 'output'}
    q = load(args['qualification'])
    output.mkdir(parents=True)
    frozen = {str(p): digest(p) for n, d in args.items() if n != 'project'
              for p in ([d] if d.is_file() else d.rglob('*'))
              if p.is_file() and not any(x.startswith('obj-') for x in p.parts)}
    # Copy just bound inputs and observations; no executables/objects/databases.
    fixture = output / 'fixture'
    for n in ['parent', 'generated', 'miter', 'pins', 'board']:
        src = args[n]
        if n == 'parent':
            names = list(q['source_hashes']) + ['timing-paths-native.json']
        else:
            names = [str(p.relative_to(src)) for p in src.rglob('*') if p.is_file()
                     and (p.relative_to(src).parts[0] == 'inputs' or
                          p.suffix in ['.json', '.gz', '.sv', '.csv', '.log'])
                     and not any(x.startswith('obj-') for x in p.relative_to(src).parts)]
        for name in names:
            target = fixture / n / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(src / name, target)
    shutil.copyfile(args['qualification'], fixture / 'qualification.json')
    local = dict(args, **{n: fixture / n for n in ['parent', 'generated', 'miter', 'pins', 'board']},
                 qualification=fixture / 'qualification.json')
    positive = stage(**local, output=output / 'positive')
    assert len(positive['source_hashes']) == 503
    assert set(positive['modified_RTL_sources']) == {BACKEND, WRAPPER}
    assert positive['capture_read_word'] and not positive['assembly_permitted']

    def change_json(path, transform):
        value = load(path)
        transform(value)
        path.write_text(json.dumps(value, indent=2) + '\n')

    def mutate(label):
        if label == 'changed-parent':
            (fixture / 'parent' / BACKEND).write_text('// changed original\n')
        elif label == 'changed-qualification':
            change_json(fixture / 'qualification.json', lambda j: j.update(minimum_setup_ns=99))
        elif label == 'changed-generated':
            (fixture / 'generated/dorado_storage_sdram_burst.sv').write_text('// changed candidate\n')
        elif label == 'failed-miter':
            change_json(fixture / 'miter/run-state.json', lambda j: j.update(status='TERMINAL_FAILED'))
        elif label == 'missing-negative':
            change_json(fixture / 'miter/run-state.json', lambda j: j['results'].pop('early-response-verilator'))
        elif label == 'missing-fatal-loop-gate':
            change_json(fixture / 'miter/run-state.json', lambda j:
                [a['command'].remove('-Werror-UNOPTFLAT') for a in j['actions']
                 if a['label'] == 'verilator-compile'])
        elif label == 'circular-warning':
            with (fixture / 'miter/verilator-compile.log').open('a') as f:
                f.write('\n%Warning-UNOPTFLAT: actual circular dependency\n')
        elif label == 'pin-cycle-mismatch':
            change_json(fixture / 'pins/run-state.json', lambda j:
                j['results']['overlap-on'][1].update(host_ticks=443))
        elif label == 'missing-66mhz':
            change_json(fixture / 'pins/run-state.json', lambda j: j.update(physical_clocks_tested=[57142857]))
        elif label == 'board-response-cycle-change':
            with (fixture / 'board/overlap-on-raw.csv').open('a') as f:
                f.write('\nchanged host timestamp\n')
        elif label == 'changed-bound-input':
            with (fixture / 'miter/inputs/tb_storage_read_word_capture.sv').open('a') as f:
                f.write('\n// changed stimulus\n')
        else:
            raise ValueError(label)

    cases = []
    for label in ['changed-parent', 'changed-qualification', 'changed-generated', 'failed-miter',
                  'missing-negative', 'missing-fatal-loop-gate', 'circular-warning',
                  'pin-cycle-mismatch', 'missing-66mhz', 'board-response-cycle-change', 'changed-bound-input']:
        # Restore a fresh independent copy after each actual evidence corruption.
        originals = {p: p.read_bytes() for p in fixture.rglob('*') if p.is_file()}
        mutate(label)
        rejected = False
        try:
            stage(**local, output=output / label)
        except (AssertionError, KeyError):
            rejected = True
        assert rejected and not (output / label).exists(), label
        cases.append(dict(name=label, rejected=True))
        for p, raw in originals.items():
            if p.read_bytes() != raw:
                p.write_bytes(raw)
        print(label, 'rejected', flush=True)
    assert all(digest(Path(n)) == h for n, h in frozen.items())
    result = dict(status='PASS_READ_WORD_NATIVE_STAGE_AND_ELEVEN_REJECTION_CONTROLS',
                  cases=cases, original_inputs_unchanged=True, positive_source_count=503,
                  stage_tool_sha256=digest(Path(__file__).with_name('stage_storage_read_word_native.py')))
    (output / 'controls.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(run(args.config.resolve(), args.output.resolve())['status'])
