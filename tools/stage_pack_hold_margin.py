#!/usr/bin/env python3
"""Stage a calibrated stronger pack hold contract with identical RTL and clocks.

Require the completed all32-bit/all4-corner proof of stricter hold checking,
unchanged setup, data and skew. Add only its exact SDC and a QSF include;
preserve every compiled source, seed, clock/barrier/capture and firmware path.
No timing or hardware qualification follows until the new native fit is reviewed.
"""
import argparse
import json
from pathlib import Path

from test_r_front_forward_view import sha


def stage(parent, calibration, out, project):
    assert not out.exists() and project.is_absolute()
    m = json.loads((parent/'r-front-control-native.json').read_text())
    c = json.loads((calibration/'run-state.json').read_text())
    assert c['status'] == 'PASS_STRONGER_PACK_HOLD_CONTRACT_CALIBRATION' and c['returncode'] == 0
    assert c['manifest_sha256'] == sha(parent/'r-front-control-native.json')
    assert len(c['corners']) == 4 and c['minimum_delay_strengthening_ns'] == 0.250
    for row in c['corners']:
        assert row['setup_all32_paths_verified'] and row['hold_all32_paths_verified']
        assert row['setup'][0] == row['setup'][1] and abs(row['hold'][0]-row['hold'][1]-0.370) < 1e-6
    for name, digest in c['files'].items():
        assert sha(calibration/name) == digest
    assert len(m['source_hashes']) == 533 and not m['assembly_permitted']
    for name, digest in m['source_hashes'].items():
        assert sha(parent/name) == digest
    added = 'core/pack_generation_hold.sdc'
    qsf = 'core/Dorado.qsf'
    line = '\n# Calibrated stricter disk-generation capture hold relationship.\nset_global_assignment -name SDC_FILE pack_generation_hold.sdc\n'
    assert 'pack_generation_hold.sdc' not in (parent/qsf).read_text()
    out.mkdir(parents=True)
    for name in m['source_hashes']:
        data = (parent/name).read_bytes()
        if Path(name).suffix in ['.qsf', '.qpf', '.qip', '.tcl', '.sdc']:
            data = data.replace(m['project'].encode(), str(project).encode())
        if name == qsf:
            data += line.encode()
        path = out/name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        restored = data.replace(str(project).encode(), m['project'].encode())
        if name == qsf:
            assert restored.endswith(line.encode())
            restored = restored[:-len(line.encode())]
        assert restored == (parent/name).read_bytes(), name
    (out/added).write_bytes((calibration/'pack_generation_hold.sdc').read_bytes())
    record = {**m, 'project': str(project), 'parent_stage': str(parent),
              'parent_manifest_sha256': sha(parent/'r-front-control-native.json'),
              'pack_hold_margin': dict(calibration=str(calibration),
                  calibration_sha256=sha(calibration/'run-state.json'), minimum_delay_ns=0.250,
                  calibrated_derived_uncertainty_ns=0.120, effective_stricter_relationship_ns=0.370,
                  complete_capture_bits=32, corners=4, RTL_byte_unchanged=True,
                  original_setup_clocks_seed_barriers_unchanged=True, extra_guest_cycles=0),
              'source_hashes': {name: sha(out/name) for name in [*m['source_hashes'], added]},
              'assembly_permitted': False, 'physical_timing_qualified': False,
              'firmware_qualified': False, 'speedup_qualified': False, 'hardware_changed': False}
    (out/'r-front-control-native.json').write_text(json.dumps(record, indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print('STAGED_CALIBRATED_PACK_HOLD_MARGIN', len(record['source_hashes']), flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'calibration', 'output', 'project']:
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    stage(args.parent.resolve(), args.calibration.resolve(), args.output.resolve(), args.project)
