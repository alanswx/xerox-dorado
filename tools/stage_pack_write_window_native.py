#!/usr/bin/env python3
"""Stage only the tested write transport change on the reviewed57MHz source.

Preserve all CPU/cache/map/ECC/board/clock sources and existing timing audits.
No assembly or deployment is permitted by this staging/component evidence.
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PACK = 'src/fpga/mister/dorado_pack_hps.sv'
MACHINE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def stage(a):
    assert not a.output.exists() and a.project.is_absolute()
    parent = json.loads((a.parent/'storage-board-native.json').read_text())
    qualification = json.loads(a.qualification.read_text())
    assert qualification['status'] == 'PASS_NATIVE_STORAGE_BOARD_PREASSEMBLY_REVIEW'
    assert qualification['source_hashes'] == parent['source_hashes']
    assert len(parent['source_hashes']) == 503
    assert qualification['minimum_setup_ns'] > 0 and qualification['minimum_hold_ns'] > 0
    assert parent['frequency_hz'] == 57142857 and parent['sysper'] == 4
    assert parent['state_admin'] == 'INACTIVE'
    test = json.loads((a.component/'run-state.json').read_text())
    assert test['status'] == 'PASS_COMPONENT_ONLY'
    assert test['scenarios'] == 10 and test['rejected_controls'] == 5
    assert len(test['actions']) == 21
    for action in test['actions']:
        if action.get('rejected'):
            assert action['returncode'] != 0
        else:
            assert action['returncode'] == 0
    for n, digest in test['inputs'].items():
        assert sha((a.component/'inputs'/n).read_bytes()) == digest
        assert sha((ROOT/n).read_bytes()) == digest, 'Test current exact source: '+n
    assert (a.component/'inputs/tools/test_pack_write_window.py').read_bytes() == (ROOT/'tools/test_pack_write_window.py').read_bytes()
    original = {}
    output = {}
    for n, digest in parent['source_hashes'].items():
        data = (a.parent/n).read_bytes()
        assert sha(data) == digest, n
        original[n] = data
        if Path(n).suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            data = data.replace(parent['project'].encode(), str(a.project).encode())
        output[n] = data
    output[PACK] = (a.component/'inputs/fpga/mister/dorado_pack_hps.sv').read_bytes()
    before = b'.SECTORS_PER_TRACK(29),.LABEL_WORDS(10))'
    after = b'.SECTORS_PER_TRACK(29),.LABEL_WORDS(10),.DIRTY_BLOCK_WINDOW(1))'
    assert output[MACHINE].count(before) == 1
    output[MACHINE] = output[MACHINE].replace(before, after)
    repair_hold = getattr(a, 'repair_hold', False)
    if repair_hold:
        assert b'OPTIMIZE_HOLD_TIMING' not in output['core/Dorado.qsf']
        output['core/Dorado.qsf'] += b'\n# Repair actual register hold paths; retain all original timing checks.\nset_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS\n'
    changed = set()
    for n, data in output.items():
        if Path(n).suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            data = data.replace(str(a.project).encode(), parent['project'].encode())
        if data != original[n]:
            changed.add(n)
    assert changed == {PACK, MACHINE} | ({'core/Dorado.qsf'} if repair_hold else set()), changed
    a.output.mkdir(parents=True)
    for n, data in output.items():
        p = a.output/n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(data)
    record = dict(status='STAGED_PACK_WINDOW_NATIVE_TIMING_REVIEW_REQUIRED', scope=__doc__,
        project=str(a.project), source_hashes={n:sha(b) for n,b in output.items()},
        modified_sources=sorted(changed), frequency_hz=57142857, frequency_mhz=57.142857,
        sysper=4, state_admin='INACTIVE', parent_manifest_sha256=sha((a.parent/'storage-board-native.json').read_bytes()),
        parent_qualification_sha256=sha(a.qualification.read_bytes()),
        component_state_sha256=sha((a.component/'run-state.json').read_bytes()),
        CPU_cache_map_ECC_board_clocks_unchanged=True, full_track_reads_unchanged=True,
        physical_hold_optimization='ALL_PATHS' if repair_hold else 'parent default',
        assembly_permitted=False, firmware_qualified=False, hardware_speedup_qualified=False)
    (a.output/'pack-window-native.json').write_text(json.dumps(record,indent=2)+'\n')
    for name in [Path(__file__).name, 'build_r_front_ready_native.py']:
        (a.output/name).write_bytes((ROOT/'tools'/name).read_bytes())
    print(record['status'], len(output), 'sources; two adapter edits only', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent','qualification','component','output','project']:
        p.add_argument('--'+n, type=Path, required=True)
    p.add_argument('--repair-hold', action='store_true', help='Ask the fitter to repair all actual hold paths; no timing exceptions')
    a = p.parse_args()
    for name, value in vars(a).items():
        if isinstance(value, Path) and name != 'project':
            setattr(a, name, value.resolve())
    stage(a)
