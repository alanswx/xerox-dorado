#!/usr/bin/env python3
"""Stage requested-beat capture on the exact reviewed690f native sources.

Only the SDRAM backend and its wrapper parameter change, after literal project
path relocation. All captures, CPU/cache/Map/ECC, clocks, constraints and other
devices stay unchanged. This permits a distinct fit only, never assembly/load.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from prepare_storage_read_word_capture import transform, QUALIFIED_BACKEND

ROOT = Path(__file__).resolve().parents[1]
PARENT_QUALIFICATION = 'b9c9a0ff40aa53a0ca755be83fb41b1e139e6479f7b3fc2ec1cdf4d8cd84e440'
BACKEND = 'src/fpga/mister/dorado_storage_sdram_burst.sv'
WRAPPER = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def load(path):
    raw = path.read_bytes()
    return json.loads(gzip.decompress(raw) if path.suffix == '.gz' else raw)


def stage(parent, qualification, generated, miter, pins, board, output, project):
    assert not output.exists()
    assert project.is_absolute() and project.parent == Path('/home/alans/mister')
    assert project.name.startswith('Dorado_')
    assert sha(qualification.read_bytes()) == PARENT_QUALIFICATION
    q = load(qualification)
    assert q['status'] == 'PASS_NATIVE_TIMING_PATHS_PREASSEMBLY_REVIEW'
    assert len(q['source_hashes']) == 503 and len(q['detailed_paths']) == 444
    assert q['minimum_setup_ns'] > 0 and q['minimum_hold_ns'] > 0
    old = load(parent / 'timing-paths-native.json')
    assert q['source_hashes'] == old['source_hashes']
    assert old['frequency_hz'] == 57142857 and old['sysper'] == 4
    assert old['split_selection'] and old['state_admin'] == 'INACTIVE'
    files = {n: (parent / n).read_bytes() for n in q['source_hashes']}
    assert {n: sha(b) for n, b in files.items()} == q['source_hashes']
    assert sha(files[BACKEND]) == QUALIFIED_BACKEND
    g = load(generated / 'provenance.json')
    assert g['capture_read_word'] and g['split_selection'] and g['parallel_ready_compare']
    for n, h in g['generated_hashes'].items():
        assert sha((generated / n).read_bytes()) == h, n
        if n != Path(BACKEND).name:
            assert (generated / n).read_bytes() == files['src/fpga/mister/' + n], n
    assert transform(files[BACKEND]) == (generated / Path(BACKEND).name).read_bytes()

    evidence = {}
    for label, directory, expected in [
        ('miter', miter, 'PASS_SELECTED_READ_CAPTURE_CONTROL_AND_VALID_DATA_TWO_SIMULATORS'),
        ('pins', pins, 'PASS_SELECTED_READ_CAPTURE_PHYSICAL_PINS_AND_IDENTICAL_RESPONSE_CYCLES'),
        ('board', board, 'PASS_SELECTED_READ_CAPTURE_ORIGINAL_MSA'),
    ]:
        state = load(directory / 'run-state.json')
        assert state['status'] == expected, label
        assert all('returncode' in a for a in state['actions']), label
        provenance_path = directory / ('provenance.json' if label == 'miter' else 'provenance.json.gz')
        provenance = load(provenance_path)
        for n, h in provenance['inputs'].items():
            assert sha((directory / 'inputs' / n).read_bytes()) == h, (label, n)
        for path in [directory / 'run-state.json', provenance_path, *directory.glob('*.log')]:
            raw = path.read_bytes()
            if path.suffix == '.log':
                assert b'%Warning-UNOPTFLAT' not in raw and b'%Error-UNOPTFLAT' not in raw, (label, path)
            evidence[label + '/' + path.name] = raw
        if label == 'miter':
            assert provenance['parent'] == g and provenance['reference_module_rename_only']
            assert (directory / 'inputs/qualified-original.sv').read_bytes() == files[BACKEND]
            assert (directory / 'inputs/candidate.sv').read_bytes() == transform(files[BACKEND])
            assert state['inputs_unchanged']
            for engine in ['icarus', 'verilator']:
                assert state['results'][engine]['passed']
                assert 'READ_CAPTURE_MITER PASS' in (directory / (engine + '-test.log')).read_text()
                for name in ['wrong-selected-beat', 'early-response', 'overwrite-held-response']:
                    assert state['results'][name + '-' + engine]['rejected']
            action = [a for a in state['actions'] if a['label'] == 'verilator-compile']
            assert len(action) == 1 and '-Werror-UNOPTFLAT' in action[0]['command']
        elif label == 'pins':
            assert provenance['parent'] == g and state['frozen_inputs_unchanged']
            assert state['physical_clocks_tested'] == [57142857, 66666667]
            results = state['results']
            assert results['qualified-overlap-control'] == results['overlap-on']
            assert results['control-66'] == results['capture-66']
            for name in ['accept-through-held-response', 'disabled-overlap']:
                assert results[name]['rejected']
        else:
            assert provenance['generated'] == g
            assert state['default_off_trace_byte_identical'] and state['canonical_operation_data_order_identical']
            assert state['results']['old-response-owner']['rejected']
            for name in ['capture-control', 'overlap-on']:
                assert state['results'][name]['host_cycles'] == 104113
            a, b = [(directory / (n + '-raw.csv')).read_bytes() for n in ['capture-control', 'overlap-on']]
            assert a == b and a, 'Original MSA data/order AND host timestamps must agree'
            evidence['board/capture-control-raw.csv'] = a
    # Relocate only literal project references; clock/SDC content is otherwise exact.
    relocated = {n: b.replace(old['project'].encode(), str(project).encode()) for n, b in files.items()}
    changed = dict(relocated)
    changed[BACKEND] = transform(files[BACKEND])
    needle = '.CACHE_LINES(4),.RESPONSE_OVERLAP(1)) u_sdram('
    assert changed[WRAPPER].count(needle.encode()) == 1
    changed[WRAPPER] = changed[WRAPPER].replace(needle.encode(),
        b'.CACHE_LINES(4),.RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1)) u_sdram(')
    assert {n for n in changed if changed[n] != relocated[n]} == {BACKEND, WRAPPER}
    output.mkdir(parents=True)
    for n, b in changed.items():
        path = output / n
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(b)
    for n, b in evidence.items():
        path = output / 'evidence' / n
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(b)
    record = dict(old)
    record.update(status='STAGED_READ_WORD_CAPTURE_NATIVE_FIT_REVIEW_REQUIRED', scope=__doc__,
        project=str(project), parent_project=old['project'],
        parent_qualification_sha256=PARENT_QUALIFICATION, parent_source_hashes=q['source_hashes'],
        source_hashes={n: sha(b) for n, b in changed.items()}, capture_read_word=True,
        modified_RTL_sources=[BACKEND, WRAPPER],
        evidence_hashes={n: sha(b) for n, b in evidence.items()},
        generated_provenance_sha256=sha((generated / 'provenance.json').read_bytes()),
        response_valid_cycles_unchanged=True, invalid_response_data_may_change_earlier=True,
        assembly_permitted=False, firmware_qualified=False, hardware_speedup_qualified=False)
    (output / 'timing-paths-native.json').write_text(json.dumps(record, indent=2) + '\n')
    for n in [Path(__file__).name, 'build_r_front_ready_native.py']:
        (output / n).write_bytes((ROOT / 'tools' / n).read_bytes())
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for n in ['parent', 'qualification', 'generated', 'miter', 'pins', 'board', 'output', 'project']:
        parser.add_argument('--' + n, type=Path, required=True)
    args = parser.parse_args()
    record = stage(**{n: p if n == 'project' else p.resolve() for n, p in vars(args).items()})
    print(record['status'], len(record['source_hashes']), 'sources; two RTL changes; no assembly/load')
