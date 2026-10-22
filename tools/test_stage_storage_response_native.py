#!/usr/bin/env python3
"""Reject drifted clock, experiment, component, qualification and control inputs."""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil
import tempfile

from stage_storage_response_native import stage


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(parent, qualification, generated, pins, board, output, parallel_lookup=False):
    assert not output.exists()
    q = json.loads(qualification.read_text())
    cases = []
    with tempfile.TemporaryDirectory(prefix='dorado-response-stage-') as temporary:
        root = Path(temporary)
        baseline = root / 'parent'
        for name in q['source_hashes']:
            target = baseline / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(parent / name, target)
        shutil.copyfile(parent / 'storage-cache-native.json', baseline / 'storage-cache-native.json')
        project = Path('/home/alans/mister/Dorado_ResponseStageControl_20261022')
        result = stage(baseline, qualification, generated, pins, board, root / 'positive', project,
                       parallel_lookup)
        assert len(result['source_hashes']) == 503 and len(result['modified_RTL_sources']) == 3

        def reject(label, **replacements):
            out = root / label
            arguments = dict(parent=baseline, qualification=qualification, generated=generated,
                             pins=pins, board=board, out=out, project=project,
                             parallel_lookup=parallel_lookup)
            arguments.update(replacements)
            try:
                stage(**arguments)
            except AssertionError as exc:
                assert not out.exists(), 'Rejected source created a native project'
                cases.append(dict(name=label, rejected=True, diagnostic=str(exc)))
            else:
                raise AssertionError('Corrupt source accepted: ' + label)

        clock = baseline / 'src/fpga/mister/Dorado.sv'
        original = clock.read_bytes()
        assert b'MACHINE_SYSPER = 4' in original
        clock.write_bytes(original.replace(b'MACHINE_SYSPER = 4', b'MACHINE_SYSPER = 2'))
        reject('changed-native-clock')
        clock.write_bytes(original)

        bad_generated = root / 'generated'
        shutil.copytree(generated, bad_generated)
        backend = bad_generated / 'dorado_storage_sdram_burst.sv'
        data = backend.read_text()
        assert data.count('delay_count<=2;state<=READ_WAIT;') == 1
        backend.write_text(data.replace('delay_count<=2;state<=READ_WAIT;', 'delay_count<=1;state<=READ_WAIT;'))
        reject('changed-read-capture', generated=bad_generated)

        bad_board = root / 'board'
        record = json.loads(gzip.decompress((board / 'provenance.json.gz').read_bytes()))
        for name in record['inputs']:
            target = bad_board / 'inputs' / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(board / 'inputs' / name, target)
        for name in ['provenance.json.gz', 'run-state.json']:
            shutil.copyfile(board / name, bad_board / name)
        group = bad_board / 'inputs/dorado_storage_group.sv'
        group.write_text(group.read_text() + '\n// changed after component qualification\n')
        reject('changed-qualified-component-input', board=bad_board)

        bad_pins = root / 'pins'
        record = json.loads(gzip.decompress((pins / 'provenance.json.gz').read_bytes()))
        for name in record['inputs']:
            target = bad_pins / 'inputs' / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(pins / 'inputs' / name, target)
        shutil.copyfile(pins / 'provenance.json.gz', bad_pins / 'provenance.json.gz')
        state = json.loads((pins / 'run-state.json').read_text())
        state['results']['disabled-overlap']['rejected'] = False
        (bad_pins / 'run-state.json').write_text(json.dumps(state))
        reject('missing-negative-control', pins=bad_pins)

        bad_q = root / 'active-qualification.json'
        active = dict(q, state_admin='ACTIVE')
        bad_q.write_text(json.dumps(active))
        reject('changed-qualified-parent', qualification=bad_q)
        if parallel_lookup:
            reject('parallel-recipe-with-serial-transform', parallel_lookup=False)

    assert len(cases) == (6 if parallel_lookup else 5)
    assert all(sha(parent / name) == digest for name, digest in q['source_hashes'].items())
    result = dict(status='PASS_RESPONSE_NATIVE_STAGING_CONTROLS', cases=cases,
                  original_sources_unchanged=True, positive_source_count=503,
                  positive_modified_RTL_count=3, hardware_changed=False)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(cases), 'rejected cases', flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'qualification', 'generated', 'pins', 'board', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--parallel-lookup', action='store_true')
    args = parser.parse_args()
    run(args.parent.resolve(), args.qualification.resolve(), args.generated.resolve(),
        args.pins.resolve(), args.board.resolve(), args.output.resolve(), args.parallel_lookup)
