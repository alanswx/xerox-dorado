#!/usr/bin/env python3
"""Integrate tested closed-storage/open-Q operand logic in a private native core.

HM section 2.3/Table 4 cadence is unchanged (57.142857 MHz, SYSPER=4).
Board consumers receive physical Q; only the central owner stores the closed
latch encoding. Keep all sixteen original visible-R latch owners and the
existing native pin-history/advance contract. This is experimental staging,
not native arrival/timing/firmware qualification or assembly permission.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from stage_r_front_ready_native import BACKPLANE
from test_r_front_task_event import replace
from test_r_front_forward_matrix import check

ROOT = Path(__file__).resolve().parents[1]


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(parent, fit, matrix, out, project, memory_inputs=(), split_physical_view=False, control_inputs=(), address_epochs=()):
    assert not out.exists() and project.is_absolute()
    original = json.loads((parent / 'r-front-control-native.json').read_text())
    assert original['status'] == 'STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT'
    assert original['frequency_hz'] == 57142857 and original['sysper'] == 4
    assert not original['assembly_permitted'] and original['native_visible_R_bits'] == 16
    for n, h in original['source_hashes'].items():
        assert sha(parent / n) == h, n
    gate = json.loads((matrix / 'run-state.json').read_text())
    assert gate['status'] == 'PASS_R_FRONT_PHYSICAL_FORWARD_PROFILE_MATRIX'
    assert len(gate['cases']) == 12 and not gate['errors']
    for row in gate['cases']:
        assert check(ROOT / row['directory'])['run_state_sha256'] == row['run_state_sha256']
    for n, h in gate['source_hashes'].items():
        assert sha(ROOT / n) == h, n
    f = json.loads((fit / 'provenance.json').read_text())
    assert f['component'] == 'r_front_control_physical_forward'
    for n, h in f['files'].items():
        raw = (fit / n).read_bytes()
        if n == 'clock_fit.qsf':
            raw = re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$', b'', raw)
        assert hashlib.sha256(raw).hexdigest() == h, n
    rtl = re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE (\S+)$', (fit / 'clock_fit.qsf').read_text(), re.M)
    rtl = [n for n in rtl if n != 'top.sv']
    assert len(rtl) == 29 and len(set(rtl)) == 29
    data = {}
    for n in original['source_hashes']:
        raw = (parent / n).read_bytes()
        if Path(n).suffix in {'.qsf', '.qpf', '.qip', '.tcl', '.sdc'}:
            raw = raw.replace(original['project'].encode(), str(project).encode())
        data[n] = raw
    bp = data[BACKPLANE].decode()
    bp = replace(bp, 'wire [843:0] r_front_q;reg [571:0] r_front_lower_q;',
                 'wire [843:0] r_front_q,r_front_held;reg [571:0] r_front_lower_q;')
    bp = replace(bp, 'assign r_front_q={r_front_upper_q,r_front_visible_r_ProcL,r_front_visible_r_ProcH,r_front_lower_q};',
                 '''// Only the owner registers contain closed MC10173 encoding.
  // The existing board port carries physical Q, including open-latch D.
  assign r_front_held={r_front_upper_q,r_front_visible_r_ProcL,r_front_visible_r_ProcH,r_front_lower_q};''')
    bp = replace(bp, '.advance(contb_advance),.state_q(r_front_q),',
                 '.advance(contb_advance),.held_q(r_front_held),')
    bp = replace(bp, '.state_next(r_front_next),.clock_events(r_front_events)',
                 '.held_next(r_front_next),.clock_events(r_front_events)')
    bp = replace(bp, '.shared_state_mismatch(r_front_shared_mismatch),.control_flags(r_front_control_flags));',
                 '.shared_state_mismatch(r_front_shared_mismatch),.control_flags(r_front_control_flags),.physical_state(r_front_q));')
    data[BACKPLANE] = bp.encode()
    for n in rtl:
        data['src/verilog/rtl/' + n] = (fit / n).read_bytes()
    projection = []
    if memory_inputs:
        from test_r_front_memory_inputs import memory
        assert len(memory_inputs) == 2
        target = 'src/verilog/rtl/dorado_local_r_memory_step.sv'
        expected, used = memory(data[target].decode())
        for engine, run in zip(['icarus', 'verilator'], memory_inputs):
            proof = json.loads((run / 'run-state.json').read_text())
            assert proof['status'] == 'PASS_R_FRONT_MEMORY_INPUT_PROJECTION' and proof['engine'] == engine
            assert proof['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
            assert proof['actions'][-1]['returncode'] == 0
            for row in proof['source_files']:
                assert sha(run / row['path']) == row['output_sha256']
            assert (run / 'adapters/dorado_local_r_memory_step.sv').read_text() == expected
            assert proof['literal_function_dependencies'] == used
            projection.append(dict(engine=engine, directory=str(run), run_state_sha256=sha(run / 'run-state.json')))
        data[target] = expected.encode()
    address_projection = []
    if address_epochs:
        from probe_r_front_address_epoch import front
        assert len(address_epochs) == 2 and control_inputs
        target = 'src/verilog/rtl/dorado_local_r_front_step.sv'
        expected = front(data[target].decode())
        for engine, run in zip(['icarus', 'verilator'], address_epochs):
            proof = json.loads((run / 'run-state.json').read_text())
            assert proof['status'] == 'PASS_R_FRONT_ADDRESS_EPOCH_PROBE' and proof['engine'] == engine
            assert proof['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
            assert proof['actions'][-1]['returncode'] == 0 and proof['original_reference_unchanged']
            for row in proof['source_files']:
                assert sha(run / row['path']) == row['output_sha256']
            assert (run / 'adapters/dorado_local_r_front_step.sv').read_text() == expected
            address_projection.append(dict(engine=engine, directory=str(run), run_state_sha256=sha(run / 'run-state.json')))
        data[target] = expected.encode()
    control_projection = []
    if control_inputs:
        from test_r_front_control_inputs import candidate
        assert len(control_inputs) == 2 and projection
        target = 'src/verilog/rtl/dorado_r_front_control_step.sv'
        expected, bits = candidate(data[target].decode())
        for engine, run in zip(['icarus', 'verilator'], control_inputs):
            proof = json.loads((run / 'run-state.json').read_text())
            assert proof['status'] == 'PASS_R_FRONT_CONTROL_INPUT_PROJECTION' and proof['engine'] == engine
            assert proof['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
            assert proof['actions'][-1]['returncode'] == 0 and proof['complete_control_projection']
            for row in proof['source_files']:
                assert sha(run / row['path']) == row['output_sha256']
            assert (run / 'candidate.sv').read_text() == expected
            assert proof['equivalent_held_control_bits'] == bits
            control_projection.append(dict(engine=engine, directory=str(run), run_state_sha256=sha(run / 'run-state.json')))
        data[target] = expected.encode()
    qsf = data['core/Dorado.qsf'].decode()
    # Existing 28 declarations retain the same paths. Add only the helper.
    extra = ['src/verilog/rtl/' + n for n in rtl]
    added = [n for n in extra if n not in original['extra_RTL_sources']]
    assert added == ['src/verilog/rtl/dorado_r_read_window_step.sv']
    for n in added:
        qsf += f'\nset_global_assignment -name SYSTEMVERILOG_FILE {project}/{n}\n'
    data['core/Dorado.qsf'] = qsf.encode()
    split_annotations = []
    if split_physical_view:
        # The lint path crosses whole physical-Q buses through functions that
        # use disjoint bits. Expose that partition to Verilator without changing
        # any expression. Quartus ignores these comments; inverse-check bytes.
        for n, tokens in [(BACKPLANE, ['r_front_q,r_front_held']),
                          ('src/verilog/rtl/dorado_r_front_control_step.sv',
                           ['state_q,state_next', 'before_child=child_pins'])]:
            raw = data[n].decode()
            original_raw = raw
            for token in tokens:
                head, tail = token.split(',', 1) if ',' in token else token.split('=', 1)
                separator = ',' if ',' in token else '='
                replacement = head + ' /* verilator split_var */' + separator + tail
                raw = replace(raw, token, replacement)
                split_annotations.append(dict(path=n, signal=head))
            assert raw.replace(' /* verilator split_var */', '') == original_raw
            data[n] = raw.encode()
    out.mkdir(parents=True)
    for n, raw in data.items():
        p = out / n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    record = dict(original)
    record.update(scope=__doc__, project=str(project), frequency_mhz=57.142857,
                  source_hashes={n: sha(out / n) for n in data},
                  extra_RTL_sources=extra, parent_stage=str(parent),
                  parent_manifest_sha256=sha(parent / 'r-front-control-native.json'),
                  component_fit=str(fit), component_provenance_sha256=sha(fit / 'provenance.json'),
                  profile_matrix_sha256=sha(matrix / 'run-state.json'),
                  physical_Q_export_separate_from_closed_storage=True,
                  closed_latch_encoding_bits=80, canonical_visible_R_owners=16,
                  actual_native_pin_history_contract_unchanged=True,
                  source_and_phase_contract_qualified=False, packed_view_dependency_partition_qualified=False,
                  native_consumer_setup_hold_qualified=False, firmware_qualified=False,
                  current_memory_input_projection=projection,
                  current_control_input_projection=control_projection,
                  stack_address_pre_edge_experiment=address_projection,
                  physical_view_split_annotations=split_annotations,
                  physical_timing_qualified=False, assembly_permitted=False,
                  future60ns_partition_qualified=False, speedup_qualified=False)
    (out / 'r-front-control-native.json').write_text(json.dumps(record, indent=2) + '\n')
    print('STAGED_PHYSICAL_OPERAND_NATIVE_EXPERIMENT')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent', 'fit', 'matrix', 'output', 'project']:
        p.add_argument('--' + n, type=Path, required=True)
    p.add_argument('--memory-inputs-icarus', type=Path)
    p.add_argument('--memory-inputs-verilator', type=Path)
    p.add_argument('--split-physical-view', action='store_true')
    p.add_argument('--control-inputs-icarus', type=Path)
    p.add_argument('--control-inputs-verilator', type=Path)
    p.add_argument('--address-epoch-icarus', type=Path)
    p.add_argument('--address-epoch-verilator', type=Path)
    a = p.parse_args()
    assert bool(a.memory_inputs_icarus) == bool(a.memory_inputs_verilator)
    runs = [a.memory_inputs_icarus.resolve(), a.memory_inputs_verilator.resolve()] if a.memory_inputs_icarus else []
    assert bool(a.control_inputs_icarus) == bool(a.control_inputs_verilator)
    controls = [a.control_inputs_icarus.resolve(), a.control_inputs_verilator.resolve()] if a.control_inputs_icarus else []
    assert bool(a.address_epoch_icarus) == bool(a.address_epoch_verilator)
    epochs = [a.address_epoch_icarus.resolve(), a.address_epoch_verilator.resolve()] if a.address_epoch_icarus else []
    prepare(a.parent.resolve(), a.fit.resolve(), a.matrix.resolve(), a.output.resolve(), a.project, runs, a.split_physical_view, controls, epochs)
