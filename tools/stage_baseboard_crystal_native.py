#!/usr/bin/env python3
"""Stage the original BaseBoard crystal ratio on the qualified overlap core.

Only the oscillator implementation and the actually compiled BaseBoard d08
binding change. Preserve the CPU, memory, all other peripherals, PLL, SYSPER,
Main and daemon. This is an unqualified fit candidate, never a load permit.
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PARENT_QUALIFICATION = 'bc31493d83886ca3ccda014a8f4f4c45314270bd99f3f6ecdc2013ab22aa5993'
CELL = 'src/verilog/cells/cell_MC12061.v'
BOARD = 'src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
SCALED_CELL = '30eb1a85271585ada492cd6e65d750ddb1fa78da1d1a030ecf09412acb9fdd2a'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def stage(parent, qualification, component, output, project):
    assert not output.exists(), 'Inspect the existing experiment before another run'
    assert project.is_absolute()
    assert sha(qualification.read_bytes()) == PARENT_QUALIFICATION
    q = json.loads(qualification.read_text())
    assert q['status'] == 'PASS_NATIVE_STORAGE_RESPONSE_PREASSEMBLY_REVIEW'
    recipe = json.loads((parent / 'storage-response-native.json').read_text())
    assert recipe['source_hashes'] == q['source_hashes']
    assert recipe['frequency_hz'] == 57142857 and recipe['sysper'] == 4
    assert recipe['state_admin'] == 'INACTIVE' and recipe['parallel_lookup']
    proof = json.loads((component / 'iverilog-controls.json').read_text())
    assert proof['status'] == 'PASS_BASEBOARD_CRYSTAL_REJECTION_CONTROLS'
    assert len(proof['cases']) == 4 and all(c['rejected'] and c['returncode'] for c in proof['cases'])
    evidence = {}
    for name, digest in proof['input_hashes'].items():
        raw = (ROOT / name).read_bytes()
        assert sha(raw) == digest, name
        evidence['component-inputs/' + name] = raw
    for name in ['iverilog-positive.log', 'verilator-positive.log']:
        raw = (component / name).read_bytes()
        assert b'PASS_BASEBOARD_CRYSTAL_RATIO_AND_LEGACY_DEFAULT' in raw
        for ratio in [2, 4, 8, 16]:
            assert ('sysper=%d instructions=10000 xtal=6000 mcpre=600 mode=0' % ratio).encode() in raw
        evidence['component/' + name] = raw
    for name in ['iverilog-controls.json', 'component-result.json', 'scaled-rom-source.json',
                 'scaled-rom-v2.log', 'scaled-rom-v2-compile.log']:
        evidence['component/' + name] = (component / name).read_bytes()
    assert b'8 of 8 reset-routine bytes match the disassembly' in evidence['component/scaled-rom-v2.log']
    files = {}
    for name, digest in q['source_hashes'].items():
        raw = (parent / name).read_bytes()
        assert sha(raw) == digest, name
        files[name] = raw
    original = dict(files)
    replacement = (ROOT / 'verilog/cells/cell_MC12061.v').read_bytes()
    assert sha(replacement) == SCALED_CELL
    files[CELL] = replacement
    before = b'cell_MC12061 u_d08 ('
    assert files[BOARD].count(before) == 1
    files[BOARD] = files[BOARD].replace(before, b'cell_MC12061 #(.SYSPER(2*SYSPER)) u_d08 (')
    rtl_changes = sorted(n for n in files if files[n] != original[n])
    assert rtl_changes == sorted([CELL, BOARD])
    # The source is also present in historical, uncompiled trees. Bind only
    # the native16 board selected by the actual current QSF.
    qsf = files['core/Dorado.qsf'].decode()
    assert BOARD.removeprefix('src/') in qsf
    for name in files:
        if Path(name).suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            files[name] = files[name].replace(recipe['project'].encode(), str(project).encode())
    output.mkdir(parents=True)
    for name, raw in files.items():
        p = output / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    for name, raw in evidence.items():
        p = output / 'evidence' / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    record = dict(status='STAGED_BASEBOARD_CRYSTAL_NATIVE_UNQUALIFIED', scope=__doc__,
                  project=str(project), parent_project=recipe['project'],
                  parent_qualification_sha256=PARENT_QUALIFICATION,
                  parent_source_hashes=q['source_hashes'],
                  source_hashes={n: sha(b) for n, b in files.items()},
                  evidence_hashes={n: sha(b) for n, b in evidence.items()},
                  modified_RTL_sources=rtl_changes, frequency_hz=57142857,
                  frequency_mhz=57.142857, sysper=4, state_admin='INACTIVE',
                  nominal_ordinary_ns=140, baseboard_crystal_samples_per_instruction=8,
                  crystal_original_hz=10000000, original_MCPreClk_hz=1000000,
                  changed_clock_ratio='MCPreClk per ordinary instruction: 0.1 -> 0.06',
                  CPU_memory_PLL_other_peripherals_unchanged=True,
                  full_native_firmware_and_timing_qualified=False, assembly_permitted=False,
                  limits='Component count and generated-board ROM evidence only. The actual native16 binding requires full timing and original-firmware qualification; no physical1MHz-at-current-slow-core claim.')
    (output / 'baseboard-crystal-native.json').write_text(json.dumps(record, indent=2) + '\n')
    for name in ['stage_baseboard_crystal_native.py', 'build_r_front_ready_native.py']:
        (output / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    return record


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'qualification', 'component', 'output', 'project']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    r = stage(a.parent.resolve(), a.qualification.resolve(), a.component.resolve(),
              a.output.resolve(), a.project)
    print(r['status'], len(r['source_hashes']), 'frozen sources; no assembly/load')
