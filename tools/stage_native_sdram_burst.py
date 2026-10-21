#!/usr/bin/env python3
"""Stage the separately tested BL8 backing behind the qualified MC1668 machine.

CPU/cache/map/ECC, clocks, Main and daemon remain the qualified parent's bytes.
Only the raw-backend module selection and its QSF source entry change. Do not
assemble or deploy until actual full-device source/memory/timing review passes.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WRAPPER = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
BACKEND = 'src/fpga/mister/dorado_storage_sdram_burst.sv'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def stage(parent, qualification, pins, board, out, project):
    assert not out.exists() and project.is_absolute()
    baseline = json.loads((parent / 'mc1668-candidate.json').read_text())
    review = json.loads(qualification.read_text())
    assert review['status'] == 'PASS_NATIVE_MC1668_BUILD'
    assert review['source_hashes'] == baseline['source_hashes']
    assert baseline['frequency_mhz'] == 50 and baseline['sysper'] == 4
    assert baseline['state_admin'] == 'INACTIVE'
    for directory, status in [(pins, 'PASS_ISOLATED_SDRAM_BURST_PIN_MODEL'),
                              (board, 'PASS_NATIVE_MSA_BL1_BL8_PHYSICAL_MODEL')]:
        state = json.loads((directory / 'run-state.json').read_text())
        assert state['status'] == status and all('returncode' in a for a in state['actions'])
        proof = json.loads(gzip.decompress((directory / 'provenance.json.gz').read_bytes()))
        for name, h in proof['inputs'].items():
            assert sha((directory / 'inputs' / name).read_bytes()) == h, (directory, name)
        tested = directory / 'inputs' / ('fpga/mister/dorado_storage_sdram_burst.sv' if directory == pins
                                         else 'dorado_storage_sdram_burst.sv')
        assert tested.read_bytes() == (ROOT / 'fpga/mister/dorado_storage_sdram_burst.sv').read_bytes()
    comparison = json.loads((board / 'comparison.json').read_text())
    assert comparison['same_raw_request_and_final_oracle_sequence']
    assert comparison['all_guest_storage_bits_compared'] == 9437184
    outputs = {}
    for name, h in baseline['source_hashes'].items():
        raw = (parent / name).read_bytes()
        assert sha(raw) == h, name
        if Path(name).suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            raw = raw.replace(baseline['project'].encode(), str(project).encode())
        outputs[name] = raw
    original = dict(outputs)
    old = 'dorado_storage_sdram #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ)) u_sdram('
    new = 'dorado_storage_sdram_burst #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ),.CACHE_LINES(4)) u_sdram('
    assert outputs[WRAPPER].decode().count(old) == 1
    outputs[WRAPPER] = outputs[WRAPPER].decode().replace(old, new).encode()
    outputs[BACKEND] = (ROOT / 'fpga/mister/dorado_storage_sdram_burst.sv').read_bytes()
    tail = ('\n# Raw BL8 buffer only; unchanged CPU/cache/map/ECC and clocks.\n'
            'set_global_assignment -name SYSTEMVERILOG_FILE ' + str(project / BACKEND) + '\n').encode()
    outputs['core/Dorado.qsf'] += tail
    assert {n for n in original if outputs[n] != original[n]} == {WRAPPER, 'core/Dorado.qsf'}
    assert outputs[WRAPPER].decode().replace(new, old).encode() == original[WRAPPER]
    assert outputs['core/Dorado.qsf'][:-len(tail)] == original['core/Dorado.qsf']
    out.mkdir(parents=True)
    for name, raw in outputs.items():
        p = out / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    record = dict(status='STAGED_NATIVE_SDRAM_BURST_REVIEW_REQUIRED', project=str(project),
                  source_hashes={n: sha(raw) for n, raw in outputs.items()},
                  parent_manifest_sha256=sha((parent / 'mc1668-candidate.json').read_bytes()),
                  parent_qualification_sha256=sha(qualification.read_bytes()),
                  intended_deltas=[WRAPPER, BACKEND, 'core/Dorado.qsf'],
                  state_admin='INACTIVE', frequency_mhz=50, sysper=4, cache_lines=4,
                  CPU_cache_map_ECC_services_clocks_unchanged=True,
                  pin_run_state_sha256=sha((pins / 'run-state.json').read_bytes()),
                  board_run_state_sha256=sha((board / 'run-state.json').read_bytes()),
                  assembly_permitted=False, firmware_qualified=False, speedup_qualified=False)
    (out / 'sdram-burst-native.json').write_text(json.dumps(record, indent=2) + '\n')
    for name in [Path(__file__).name, 'build_r_front_ready_native.py']:
        (out / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    print(record['status'], len(outputs), 'frozen source/config files')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'qualification', 'pins', 'board', 'output', 'project']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    stage(a.parent.resolve(), a.qualification.resolve(), a.pins.resolve(), a.board.resolve(),
          a.output.resolve(), a.project)
