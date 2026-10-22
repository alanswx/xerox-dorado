#!/usr/bin/env python3
"""Stage response/request overlap on a minimal four-line native baseline.

Exact reviewed93ad inputs, same57MHz/SYSPER4/clocks/constraints/CPU/cache/Map/ECC
and peripherals. Change only the generated arbiter/backend and the wrapper's
two opt-in enables plus16->4 backing-buffer capacity. Fit/report only: no
assembly or programming is authorized by this staging result.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from prepare_storage_response_pipeline import EXPECTED, transform_arb, transform_backend

ROOT = Path(__file__).resolve().parents[1]
QUALIFICATION = '73792e73e19acf858f2a06ec314b294ea9b8f12c7174978015d601031a68c335'
WRAPPER = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load(path):
    return json.loads(path.read_text())


def stage(parent, qualification, generated, pins, board, out, project, parallel_lookup=False):
    assert not out.exists() and project.is_absolute()
    assert sha(qualification) == QUALIFICATION, 'Require exact reviewed cache16 qualification'
    q = load(qualification)
    assert q['status'] == 'PASS_NATIVE_STORAGE_CACHE_PREASSEMBLY_REVIEW'
    assert q['frequency_hz'] == 57142857 and q['sysper'] == 4 and q['state_admin'] == 'INACTIVE'
    assert len(q['source_hashes']) == 503
    recipe = load(parent / 'storage-cache-native.json')
    assert recipe['source_hashes'] == q['source_hashes']
    preparation = load(generated / 'provenance.json')
    assert preparation['parent_hashes'] == EXPECTED and preparation['defaults_off']
    assert preparation.get('parallel_lookup', False) == parallel_lookup
    backend_transform = transform_backend
    if parallel_lookup:
        from prepare_storage_response_parallel import transform_parallel
        backend_transform = transform_parallel
    assert load(pins / 'run-state.json')['status'] == 'PASS_RESPONSE_OVERLAP_PHYSICAL_COMPONENT'
    assert load(board / 'run-state.json')['status'] == 'PASS_RESPONSE_OVERLAP_ORIGINAL_MSA'
    for folder in [pins, board]:
        state = load(folder / 'run-state.json')
        assert all('returncode' in row for row in state['actions'])
        if folder == pins:
            assert state['frozen_inputs_unchanged']
        else:
            assert state['default_off_trace_byte_identical']
            assert state['canonical_operation_data_order_identical']
        provenance = json.loads(gzip.decompress((folder / 'provenance.json.gz').read_bytes()))
        for name, digest in provenance['inputs'].items():
            assert sha(folder / 'inputs' / name) == digest, (folder, name)
        for name, digest in preparation['generated_hashes'].items():
            assert sha(folder / 'inputs' / name) == digest, (folder, name)
    assert load(pins / 'run-state.json')['results']['accept-through-held-response']['rejected']
    assert load(pins / 'run-state.json')['results']['disabled-overlap']['rejected']
    assert load(board / 'run-state.json')['results']['old-response-owner']['rejected']
    files = {}
    for name, digest in q['source_hashes'].items():
        source = parent / name
        assert sha(source) == digest, name
        raw = source.read_bytes()
        if source.suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            raw = raw.replace(recipe['project'].encode(), str(project).encode())
        files[name] = raw
    original = dict(files)
    for name, transform in [('dorado_storage_arb.sv', transform_arb),
                            ('dorado_storage_sdram_burst.sv', backend_transform)]:
        key = 'src/fpga/mister/' + name
        assert q['source_hashes'][key] == EXPECTED[name]
        assert sha(generated / name) == preparation['generated_hashes'][name]
        text = transform(original[key].decode())
        assert text.encode() == (generated / name).read_bytes(), 'Regenerate exact experiment'
        files[key] = text.encode()
    text = files[WRAPPER].decode()
    for old, new in [
        ('.CACHE_LINES(16)) u_sdram(', '.CACHE_LINES(4),.RESPONSE_OVERLAP(1)) u_sdram('),
        ('.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1)) u_arb(',
         '.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1)) u_arb('),
    ]:
        assert text.count(old) == 1, old
        text = text.replace(old, new)
    files[WRAPPER] = text.encode()
    changed = sorted(name for name in files if files[name] != original[name])
    assert changed == sorted([WRAPPER, *['src/fpga/mister/' + n for n in EXPECTED]])
    out.mkdir(parents=True)
    for name, raw in files.items():
        target = out / name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(raw)
    record = dict(status='STAGED_RESPONSE_OVERLAP_NATIVE_REVIEW_REQUIRED', scope=__doc__,
                  project=str(project), parent_project=recipe['project'],
                  parent_qualification_sha256=sha(qualification),
                  parent_source_hashes=q['source_hashes'],
                  source_hashes={name: hashlib.sha256(raw).hexdigest() for name, raw in files.items()},
                  modified_RTL_sources=changed, raw_buffer_lines=4, response_overlap=1,
                  frequency_hz=57142857, frequency_mhz=57.142857, sysper=4, state_admin='INACTIVE',
                  pins_result_sha256=sha(pins / 'run-state.json'),
                  board_result_sha256=sha(board / 'run-state.json'),
                  preparation_sha256=sha(generated / 'provenance.json'),
                  nominal_ordinary_ns=140, assembly_permitted=False,
                  CPU_clocks_cache_Map_ECC_peripheral_timing_unchanged=True,
                  native_timing_firmware_speed_qualified=False)
    if parallel_lookup:
        record['parallel_lookup'] = True
    (out / 'storage-response-native.json').write_text(json.dumps(record, indent=2) + '\n')
    scripts = ['build_r_front_ready_native.py', 'stage_storage_response_native.py',
               'prepare_storage_response_pipeline.py']
    if parallel_lookup:
        scripts.append('prepare_storage_response_parallel.py')
    for name in scripts:
        (out / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'qualification', 'generated', 'pins', 'board', 'output', 'project']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--parallel-lookup', action='store_true')
    args = parser.parse_args()
    result = stage(args.parent.resolve(), args.qualification.resolve(), args.generated.resolve(),
                   args.pins.resolve(), args.board.resolve(), args.output.resolve(), args.project,
                   args.parallel_lookup)
    print(result['status'], len(result['source_hashes']), 'frozen sources', flush=True)
