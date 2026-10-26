#!/usr/bin/env python3
"""Qualify an IM-data-independent falling frontier on the actual native path.

Remove only the 36 IM data pins from three existing sparse-fall predicates.
Keep every capture, Hold/Ready guard and fallback. Compare all 2,970 actual
single-path outputs for arbitrary inputs under its independently computed
entry-certificate contract; an invalid entry must expose a counterexample.
The native cache induction is required separately before this contract can be
used as a history theorem. No new state, host tick, or guest cycle is added.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from probe_native_control_phase_cut import DATA_PINS
from prove_cp_prefix_cache import module
from prove_native_cp_context import expression, project_source
from qualify_native_ready_compare import CONTROL, SINGLE, ports
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

HELPER = 'src/verilog/rtl/dorado_ready_target_compare.sv'
CALLER = 'src/verilog/rtl/caller.sv'
CERTIFICATE = '''
 wire [268:0] phase_identity;
 dorado_control_hold_atomic_step #(.WAVES(5)) phase_check(joint_q,clock_before,clock_before,
  source_before[397:0],source_before[397:0],phase_identity[268:113],phase_identity[112:57],
  phase_identity[56:1],phase_identity[0],refresh_before_run_capture,);
 assign phase_certificate=phase_identity=={joint_q,113'b0};
'''


def transform(raw, names):
    edits = []
    changed = raw
    for name in names:
        before = module(raw, name)
        found = re.search(r'localparam \[397:0\] FALL_STABLE_PIN_SUPPORT=[^;]+;', before)
        assert found, name
        declaration = found[0]
        masks = re.findall(r"398'h([0-9a-f]+)", declaration)
        assert masks
        new = declaration
        for mask in set(masks):
            value = int(mask, 16)
            assert all(value >> i & 1 for i in DATA_PINS)
            value &= ~sum(1 << i for i in DATA_PINS)
            new = new.replace("398'h" + mask, "398'h" + format(value, 'x'))
        after = before.replace(declaration, new, 1)
        assert after != before and after.replace(new, declaration, 1) == before
        changed = changed.replace(before, after, 1)
        edits.append(dict(module=name, before=declaration, after=new))
    restored = changed
    for edit in reversed(edits):
        before = module(restored, edit['module'])
        restored = restored.replace(before, before.replace(edit['after'], edit['before'], 1), 1)
    assert restored == raw
    return changed, edits


def run(stage, component, closure, cache_proof, out, yosys):
    assert not out.exists()
    manifest = stage / 'r-front-control-native.json'
    state = json.loads(manifest.read_text())
    verify(stage, state['source_hashes'])
    receipts = []
    for directory, expected in [(component, 'PASS_CERTIFIED_CONTROL_CAPTURE_WITH_CHANGING_IM_DATA'),
                                (closure, 'PASS_ORIGINAL_CONTROL_PRODUCER_PHASE_CLOSURE'),
                                (cache_proof, 'PASS_ACTUAL_NATIVE_CONTROL_PHASE_CERTIFICATE_INDUCTION')]:
        receipt = json.loads((directory / 'run-state.json').read_text())
        assert receipt['status'] == expected
        assert receipt['source_control_sha256'] == sha(stage / CONTROL)
        if directory == cache_proof:
            assert receipt['actual_Hold_independent_LC_projection_included']
        verify(directory, receipt['files'])
        for row in receipt['actions']:
            d = directory / row['label']
            verify(d, row['files'])
            assert row['passed'] and sha(d / 'proof.log') == row['log_sha256']
        receipts.append(dict(directory=str(directory), receipt_sha256=sha(directory / 'run-state.json')))
    raw = (stage / CONTROL).read_text()
    helper = (stage / HELPER).read_text()
    top_owner = module(raw, 'dorado_r_front_control_step')
    single_owner = module(raw, SINGLE)
    for name in ['cq', 'joint_q', 'held_next', 'conta_next']:
        assert expression(top_owner, name) == expression(single_owner, name), name
    owner_assignments = lambda text: re.findall(r'\bassign\s+state_next\[\d+:\d+\]\s*=\s*\w+\[\d+:\d+\];', text)
    assert owner_assignments(top_owner) == owner_assignments(single_owner)
    projection = project_source(raw, (stage / CALLER).read_text())
    # The older projection builder predates the registered capture exports.
    # Give those actual signals their real widths as arbitrary inputs; they
    # occupy the same operand fields and cannot move any control owner bit.
    assert 'wire [31:0] captured_read_next;' in single_owner
    assert 'wire [7:0] captured_r_flags_next;' in single_owner
    projection = projection.replace('input wire [7:0] closed_r_flags_next,',
                                    'input wire [7:0] closed_r_flags_next,captured_r_flags_next,', 1)
    projection = projection.replace('input wire [31:0] closed_read_next,closed_t_inputs_next,',
                                    'input wire [31:0] closed_read_next,closed_t_inputs_next,captured_read_next,', 1)
    assert 'captured_read_next,' in projection and 'closed_r_flags_next,captured_r_flags_next,' in projection
    new_control, control_edits = transform(raw, ['dorado_control_certified_fall_step', 'dorado_capture_certified_fall_step'])
    new_helper, helper_edits = transform(helper, ['dorado_ready_target_compare'])
    bank = {}
    for name in state['source_hashes']:
        if not name.startswith('src/verilog/rtl/') or Path(name).suffix not in ['.v', '.sv']:
            continue
        for found in re.finditer(r'^module\s+(\w+)\b.*?^endmodule', (stage / name).read_text(), re.M | re.S):
            assert found[1] not in bank
            bank[found[1]] = found[0] + '\n'
    chosen = set()

    def collect(name):
        if name in chosen:
            return
        chosen.add(name)
        for other in re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(', bank[name], re.M):
            if other in bank:
                collect(other)

    collect(SINGLE)
    # Duplicate only changed nodes and their ancestors. Every other exact
    # module is shared by both graphs, avoiding duplicate elaboration of the
    # large processor while preserving the complete 2,970-output comparison.
    dependencies = {name: set(re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(', bank[name], re.M)) & chosen
                    for name in chosen}
    affected = {'dorado_control_certified_fall_step', 'dorado_capture_certified_fall_step',
                'dorado_ready_target_compare'} & chosen
    while True:
        more = affected | {name for name in chosen if dependencies[name] & affected}
        if more == affected:
            break
        affected = more
    assert SINGLE in affected
    reference = {}
    pattern = re.compile(r'\b(' + '|'.join(sorted(affected, key=len, reverse=True)) + r')\b')
    for name in affected:
        source = bank[name]
        if name == SINGLE:
            end = source.index(');')
            source = source[:end] + ',output wire phase_certificate' + source[end:]
            source = source.replace('\nendmodule', CERTIFICATE + '\nendmodule', 1)
        reference['reference__' + name] = pattern.sub(lambda m: 'reference__' + m[0], source)
    for name in ['dorado_control_certified_fall_step', 'dorado_capture_certified_fall_step']:
        bank[name] = module(new_control, name)
    bank['dorado_ready_target_compare'] = module(new_helper, 'dorado_ready_target_compare')
    assert 'dorado_control_hold_atomic_step' in chosen
    rows = ports(module(raw, SINGLE))
    args = [[], []]
    declarations = []
    inbits = outbits = 0
    for direction, width, name in rows:
        if direction == 'input':
            declarations.append(f' wire [{width-1}:0] {name}=stimulus[{inbits}+:{width}];')
            inbits += width
            for a in args:
                a.append(f'.{name}({name})')
        else:
            for i, target in enumerate(['expected', 'actual']):
                args[i].append(f'.{name}({target}[{outbits}+:{width}])')
            outbits += width
    assert inbits == 1982 and outbits == 2970
    args[0].append('.phase_certificate(certificate)')
    top = f'''module native_phase_cut_contract(input wire [1981:0] stimulus,
 output wire [2969:0] expected,actual,output wire certificate,equal,contract_valid);
{chr(10).join(declarations)}
 reference__{SINGLE} original({','.join(args[0])});
 {SINGLE} candidate({','.join(args[1])});
 assign contract_valid=!entry_fast || certificate;
 assign equal=CONDITION;
endmodule
'''
    out.mkdir(parents=True)
    (out / 'candidate-control.sv').write_text(new_control)
    (out / 'candidate-ready.sv').write_text(new_helper)
    shutil.copy2(Path(__file__), out / Path(__file__).name)
    record = dict(status='RUNNING', started=time.time(), driver_pid=os.getpid(),
                  scope=__doc__, source_manifest_sha256=sha(manifest),
                  source_control_sha256=sha(stage / CONTROL), source_helper_sha256=sha(stage / HELPER),
                  receipts=receipts, edits=control_edits + helper_edits,
                  arbitrary_binary_input_bits=inbits, compared_output_bits=outbits,
                  reference_duplicated_modules=sorted(affected),
                  reference_shared_unchanged_modules=sorted(chosen - affected),
                  contract='entry_fast implies exact original identity of actual joint_q/source_before/root',
                  SAT_assumptions=0, extra_host_or_guest_edges=0, actions=[],
                  files={p.name: sha(p) for p in out.iterdir()},
                  native_staged_or_selected=False, timing_or_speed_or60ns_qualified=False)

    def save():
        (out / 'run-state.json').write_text(json.dumps(record, indent=2) + '\n')

    save()
    for label, condition in [('actual-caller-owner-projection', None),
                             ('complete-native-contract', '!contract_valid || expected==actual'),
                             ('without-entry-certificate', 'expected==actual')]:
        d = out / label
        d.mkdir()
        if condition is None:
            (d / 'top.sv').write_text(projection)
        else:
            for name, source in {**{n: bank[n] for n in chosen}, **reference}.items():
                (d / (name + '.sv')).write_text(source)
            (d / 'top.sv').write_text(top.replace('CONDITION', condition))
        positive = label != 'without-entry-certificate'
        entity = 'owned_projection' if condition is None else 'native_phase_cut_contract'
        sources = ' '.join('/work/' + p.name for p in sorted(d.glob('*.sv')))
        (d / 'proof.ys').write_text(f'''read_verilog -sv -defer {sources}
hierarchy -check -top {entity}
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{'verify' if positive else 'falsify'} -show-ports -dump_json /work/counterexample.json
''')
        env = os.environ.copy()
        env['YOWASP_CACHE_DIR'] = str(out.parent / 'formal-tool-cache')
        env['YOWASP_MOUNT'] = '/work=' + str(d)
        command = [str(yosys), '-Q', '-T', '-s', '/work/proof.ys']
        row = dict(label=label, command=command, started=time.time(),
                   files={p.name: sha(p) for p in d.iterdir()})
        with (d / 'proof.log').open('x') as log:
            child = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
            row['pid'] = child.pid
            record['actions'].append(row)
            save()
            code = child.wait()
        marker = 'no model found: SUCCESS!' if positive else 'model found: FAIL!'
        row.update(returncode=code, passed=code == 0 and marker in (d / 'proof.log').read_text(),
                   finished=time.time(), log_sha256=sha(d / 'proof.log'))
        save()
        print(label, row['passed'], flush=True)
        if not row['passed']:
            break
    passed = len(record['actions']) == 3 and all(a['passed'] for a in record['actions'])
    record.update(status='PASS_COMPLETE_NATIVE_CONTROL_PHASE_CUT_CONTRACT' if passed else
                  'TERMINAL_NATIVE_CONTROL_PHASE_CUT_NOT_ESTABLISHED', finished=time.time())
    save()
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'component', 'closure', 'cache-proof', 'output', 'yosys']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    raise SystemExit(run(a.stage.resolve(), a.component.resolve(), a.closure.resolve(),
                         a.cache_proof.resolve(), a.output.resolve(), a.yosys.resolve()))
