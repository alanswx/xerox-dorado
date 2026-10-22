#!/usr/bin/env python3
"""Stage qualified component changes on the exact2278 native source baseline.

Activate registered request lookahead, parallel ready comparisons, carry-select
MC10181 logic and the original BaseBoard crystal ratio. Keep all CPU registers,
capture epochs, SDRAM, architectural cache/Map/ECC, other peripherals, PLL,
SYSPER, constraints, Main and daemon. Full native timing/firmware is unqualified;
this stage permits fitting/reporting only, never assembly or programming.
"""
import argparse
import hashlib
import json
from pathlib import Path

from stage_baseboard_crystal_native import stage as crystal_stage, PARENT_QUALIFICATION
from prepare_storage_request_lookahead import transform as request_transform, split_selection, BASELINE_ARB
from prepare_storage_ready_compare import transform as ready_transform, GROUP_SHA
from test_mc10181_carry_select import carry_select, CELL

ROOT = Path(__file__).resolve().parents[1]
ARB = 'src/fpga/mister/dorado_storage_arb.sv'
GROUP = 'src/fpga/mister/dorado_storage_group.sv'
BACKEND = 'src/fpga/mister/dorado_storage_sdram_burst.sv'
WRAPPER = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'


def sha(b):
    return hashlib.sha256(b).hexdigest()


def load(p):
    return json.loads(p.read_text())


def stage(parent, qualification, crystal, generated, arb, board, ready, alu, output, project):
    assert not output.exists() and project.is_absolute()
    assert project.parent == Path('/home/alans/mister') and project.name.startswith('Dorado_'), 'Use the literal Linux build path; do not resolve it through macOS /home'
    assert sha(qualification.read_bytes()) == PARENT_QUALIFICATION
    q = load(qualification)
    g = load(generated / 'provenance.json')
    assert g['request_lookahead'] and g['parallel_ready_compare'] and g['parallel_lookup']
    assert g['parent_hashes']['dorado_storage_group.sv'] == GROUP_SHA
    for n, h in g['generated_hashes'].items():
        assert sha((generated / n).read_bytes()) == h
    a = load(arb / 'run-state.json')
    assert a['status'] == 'PASS_REQUEST_LOOKAHEAD_ARBITER_STATIC_DYNAMIC' and a['inputs_unchanged']
    for label in ['icarus', 'verilator', 'icarus-arrivals', 'verilator-arrivals']:
        assert a['results'][label]['passed']
    for label in ['old-response-owner', 'disabled-overlap', 'ignore-late-arrival']:
        assert a['results'][label]['rejected']
    for label in ['verilator-compile', 'verilator-arrivals-compile']:
        action = [x for x in a['actions'] if x['label'] == label]
        assert len(action) == 1 and action[0]['returncode'] == 0
        assert '-Werror-UNOPTFLAT' in action[0]['command'], 'Circular feedback must be a fatal compile error'
        assert 'UNOPTFLAT' not in (arb / (label + '.log')).read_text(), 'Reject circular component logic before fitting'
    ap = load(arb / 'provenance.json')
    for n, h in ap['inputs'].items():
        assert sha((arb / 'inputs' / n).read_bytes()) == h
    assert sha((arb / 'inputs/dorado_storage_arb.sv').read_bytes()) == g['generated_hashes']['dorado_storage_arb.sv']
    import gzip
    b = load(board / 'run-state.json')
    assert b['status'] == 'PASS_REQUEST_LOOKAHEAD_ORIGINAL_MSA'
    assert b['default_off_trace_byte_identical'] and b['canonical_operation_data_order_identical']
    assert b['results']['old-response-owner']['rejected']
    bp = json.loads(gzip.decompress((board / 'provenance.json.gz').read_bytes()))
    for n, h in bp['inputs'].items():
        assert sha((board / 'inputs' / n).read_bytes()) == h
    for n, h in g['generated_hashes'].items():
        assert sha((board / 'inputs' / n).read_bytes()) == h
    rp = load(ready / 'proof.json')
    assert rp['status'] == 'PASS_READY_COMPARE_ALL_BINARY_AND_ACTUAL_FOUR_STATE'
    assert [(c['name'], c['result']) for c in rp['cases']] == [
        ('positive', 'unsat'), ('bad-response-selector', 'sat'), ('bad-next-address', 'sat')]
    for n, h in rp['inputs'].items():
        assert sha((ready / 'inputs' / n).read_bytes()) == h
    assert (ready / 'inputs/candidate.sv').read_bytes() == (generated / 'dorado_storage_group.sv').read_bytes()
    u, up = load(alu / 'run-state.json'), load(alu / 'provenance.json')
    assert u['status'] == 'PASS_MC10181_CARRY_SELECT_ALL_BINARY_PINS_TWO_SIMULATORS'
    assert len(u['actions']) == 12 and all('returncode' in x for x in u['actions'])
    assert up['binary_cases'] == 16384 and up['outputs'] == 7 and not up['registers_changed']
    for n, h in up['inputs'].items():
        assert sha((alu / 'inputs' / n).read_bytes()) == h
    assert up['original_cell_sha256'] == q['source_hashes'][CELL]
    assert carry_select((parent / CELL).read_bytes()) == (alu / 'inputs/candidate.v').read_bytes()
    assert q['source_hashes'][ARB] == BASELINE_ARB and q['source_hashes'][GROUP] == GROUP_SHA
    expected_arb = request_transform((parent / ARB).read_text())
    if g.get('split_selection', False):
        expected_arb = split_selection(expected_arb)
    assert expected_arb.encode() == (generated / 'dorado_storage_arb.sv').read_bytes()
    assert ready_transform((parent / GROUP).read_bytes()) == (generated / 'dorado_storage_group.sv').read_bytes()
    assert (parent / BACKEND).read_bytes() == (generated / 'dorado_storage_sdram_burst.sv').read_bytes()
    # This also independently rechecks the unchanged clock component proof,
    # parent qualification, all503 source bytes and exact native d08 binding.
    clock = crystal_stage(parent, qualification, crystal, output, project)
    proof_file = output / 'baseboard-crystal-native.json'
    (output / 'evidence/baseboard-crystal-stage.json').write_bytes(proof_file.read_bytes())
    proof_file.unlink()
    for n in ['dorado_storage_arb.sv', 'dorado_storage_group.sv']:
        (output / 'src/fpga/mister' / n).write_bytes((generated / n).read_bytes())
    (output / CELL).write_bytes((alu / 'inputs/candidate.v').read_bytes())
    text = (output / WRAPPER).read_text()
    old = '.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1)) u_arb('
    assert text.count(old) == 1
    (output / WRAPPER).write_text(text.replace(old, '.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)) u_arb('))
    sources = {n: sha((output / n).read_bytes()) for n in q['source_hashes']}
    changed = sorted(n for n in sources if Path(n).suffix in ['.sv', '.v'] and sources[n] != q['source_hashes'][n])
    assert changed == sorted([ARB, GROUP, CELL, WRAPPER,
                               'src/verilog/cells/cell_MC12061.v',
                               'src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'])
    r = dict(status='STAGED_TIMING_PATHS_NATIVE_FIT_REVIEW_REQUIRED', scope=__doc__,
             project=str(project), parent_project=clock['parent_project'],
             parent_qualification_sha256=PARENT_QUALIFICATION, parent_source_hashes=q['source_hashes'],
             source_hashes=sources, modified_RTL_sources=changed,
             component_proofs={str(p): sha(p.read_bytes()) for p in [
                 generated / 'provenance.json', arb / 'provenance.json', arb / 'run-state.json',
                 arb / 'verilator-compile.log', arb / 'verilator-arrivals-compile.log',
                 board / 'provenance.json.gz', board / 'run-state.json', ready / 'proof.json',
                 alu / 'provenance.json', alu / 'run-state.json']},
             frequency_hz=57142857, frequency_mhz=57.142857, sysper=4, nominal_ordinary_ns=140,
             state_admin='INACTIVE', request_lookahead=1, raw_buffer_lines=4, response_overlap=1,
             parallel_lookup=True, parallel_ready_compare=True, ALU_carry_select=True,
             split_selection=g.get('split_selection', False),
             baseboard_crystal_samples_per_instruction=8, assembly_permitted=False,
             firmware_qualified=False, hardware_speedup_qualified=False,
             CPU_registers_captures_cache_Map_ECC_PLL_constraints_other_peripherals_unchanged=True,
             component_host_cycle_cost_vs_current_overlap=b['lookahead_vs_qualified_overlap_host_cycle_change'])
    (output / 'timing-paths-native.json').write_text(json.dumps(r, indent=2) + '\n')
    for n in ['stage_timing_paths_native.py', 'build_r_front_ready_native.py']:
        (output / n).write_bytes((ROOT / 'tools' / n).read_bytes())
    return r


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent', 'qualification', 'crystal', 'generated', 'arb', 'board', 'ready', 'alu', 'output', 'project']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    r = stage(**{n: v if n == 'project' else v.resolve() for n, v in vars(a).items()})
    print(r['status'], len(r['source_hashes']), 'sources;', len(r['modified_RTL_sources']), 'RTL changes; no assembly/load')
