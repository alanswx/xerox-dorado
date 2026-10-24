#!/usr/bin/env python3
"""Stage the fully proved shorter control evaluation in the native machine.

Only the atomic combinational control implementation changes after path
relocation. Preserve all533 source identities, original owner/cache contracts,
physical SRAM ports, clock/capture phases and SDC constraints. Check complete
363-output equivalence, unchanged full references and exhaustive single-input
X/Z coverage before permitting an isolated timing fit. No assembly or deploy.
"""
import argparse
import json
from pathlib import Path

from shorten_control_terminal_wave import transform, BODY
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

CONTROL = 'src/verilog/rtl/dorado_r_front_control_step.sv'


def read(path):
    return json.loads(path.read_text())


def verify_files(directory, state):
    for name, digest in state['files'].items():
        assert sha(directory/name) == digest, (directory, name)


def stage(parent, evidence, out, project):
    assert not out.exists() and project.is_absolute()
    manifest = read(parent/'r-front-control-native.json')
    assert manifest['frequency_hz'] == 57142857 and manifest['sysper'] == 4
    assert manifest['balanced_task_priority_proven'] and not manifest['assembly_permitted']
    for name, digest in manifest['source_hashes'].items():
        assert sha(parent/name) == digest, name
    original = (parent/CONTROL).read_text()
    changed = transform(original)
    proofs = []
    for variant, expected in [('positive', 'PASS_SHORT_CONTROL_ALL_OUTPUTS'),
                              ('negative', 'PASS_SHORT_CONTROL_COUNTEREXAMPLES')]:
        d = evidence/f'control-short-{variant}-proof-v2-20261024'
        p = read(d/'run-state.json')
        assert p['status'] == expected
        assert [(r['waves'], r['passed']) for r in p['results']] == [(5, True), (24, True)]
        assert p['compared_output_bits'] == 363 and p['arbitrary_binary_input_and_state_bits'] == 955
        assert not p['assumptions_added'] and p['extra_state_or_guest_cycles'] == 0
        verify_files(d, p)
        assert (d/'original.sv').read_text() == original
        assert (d/'candidate.sv').read_text() == transform(original, variant == 'negative')
        proofs.append(dict(directory=str(d), state_sha256=sha(d/'run-state.json'), status=p['status']))
    comparisons = []
    for engine in ['icarus', 'verilator']:
        d = evidence/f'control-short-{engine}-v2-20261024'
        p = read(d/'run-state.json')
        assert p['status'] == 'PASS_SHORT_CONTROL_COMPLETE_ORIGINAL_COMPARISON'
        assert p['all100_clock_counts_match'] and p['original_testbench_and_references_unchanged']
        verify_files(d, p)
        component_original = Path(p['source'])/'candidate.sv'
        assert module(component_original.read_text(), 'dorado_control_hold_atomic_step') == module(original, 'dorado_control_hold_atomic_step')
        assert (d/'candidate.sv').read_text() == transform(component_original.read_text())
        for row in p['source_files']:
            assert row['changed'] == (row['path'] == 'candidate.sv')
        negative = evidence/f'control-short-bad-state-{engine}-v2-20261024'
        n = read(negative/'run-state.json')
        verify_files(negative, n)
        assert n['actions'][0]['returncode'] == 0 and n['actions'][-1]['returncode'] != 0
        assert 'R front state commit changed settled Q check=0' in (negative/'test.log').read_text()
        assert (negative/'candidate.sv').read_text() == transform(component_original.read_text(), True)
        assert (negative/'inputs/tb_r_front_local.sv').read_bytes() == (d/'inputs/tb_r_front_local.sv').read_bytes()
        comparisons.append(dict(engine=engine, directory=str(d), state_sha256=sha(d/'run-state.json'),
            cache_coverage=p['cache_coverage'], negative_directory=str(negative), negative_state_sha256=sha(negative/'run-state.json'),
            negative_original_reference_check=0,
            negative_reader_missed_state_commit_message=n['status'] == 'TERMINAL_SHORT_CONTROL_TEST_NOT_ESTABLISHED'))
    for variant, expected in [('four-state', 'PASS_SHORT_CONTROL_FOUR_STATE'),
                              ('bad-unknown', 'PASS_SHORT_CONTROL_SIMULATOR_COUNTEREXAMPLE')]:
        d = evidence/f'control-short-{variant}-v2-20261024'
        p = read(d/'run-state.json')
        assert p['status'] == expected
        verify_files(d, p)
        if variant == 'four-state':
            assert 'checks=1974 unknown_checks=1910 input_bits=955 waves=4/5/6/24 outputs=363 diagnostics=12/15/18/72' in (d/'test.log').read_text()
        assert module((d/'candidate.sv').read_text(), BODY) == module(changed, BODY)
    out.mkdir(parents=True)
    for name in manifest['source_hashes']:
        raw = changed.encode() if name == CONTROL else (parent/name).read_bytes()
        if Path(name).suffix in ['.qsf', '.qpf', '.qip', '.tcl', '.sdc']:
            raw = raw.replace(manifest['project'].encode(), str(project).encode())
        p = out/name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    changes = [name for name in manifest['source_hashes'] if
        (out/name).read_bytes().replace(str(project).encode(), manifest['project'].encode()) != (parent/name).read_bytes()]
    assert changes == [CONTROL]
    record = {**manifest, 'project': str(project), 'scope': __doc__, 'parent_stage': str(parent),
        'parent_manifest_sha256': sha(parent/'r-front-control-native.json'),
        'shortened_control_terminal_wave': dict(binary_transfer_passes=4, original_requested_passes=[5, 24],
            compared_outputs=363, proofs=proofs, original_graph_comparisons=comparisons,
            four_state_checks=1974, unknown_input_checks=1910, independent_unknown_input_bits=955,
            unchanged_parameter_modes_tested=[4, 6], original_per_wave_diagnostics_preserved=True,
            unknown_fallback='exact original requested-depth body', extra_state_or_guest_cycles=0,
            SRAM_or_capture_changes=False),
        'source_hashes': {name: sha(out/name) for name in manifest['source_hashes']},
        'physical_timing_qualified': False, 'firmware_qualified': False, 'speedup_qualified': False,
        'assembly_permitted': False, 'hardware_changed': False}
    (out/'r-front-control-native.json').write_text(json.dumps(record, indent=2)+'\n')
    for name in [Path(__file__).name, 'shorten_control_terminal_wave.py']:
        (out/name).write_bytes((Path(__file__).parent/name).read_bytes())
    print('STAGED_PROVED_SHORT_CONTROL', len(manifest['source_hashes']), 'sources', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'evidence', 'output', 'project']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    stage(a.parent.resolve(), a.evidence.resolve(), a.output.resolve(), a.project)
