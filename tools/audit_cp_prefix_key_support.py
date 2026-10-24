#!/usr/bin/env python3
"""Find and prove the binary support of the actual cached CP-prefix function.

The wide cache guard compares the complete 567-bit transaction key. Derive
the combinational input cone of ALL269 cached prefix bits from the exact
staged atomic RTL, then prove that arbitrary changes outside that cone cannot
change any cached bit. Retain a cared-bit corruption as a negative control.
This is a dependency audit only: it does not modify the cache, remove its
four-state checks, qualify a native guard or establish timing/boot/speed.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from prove_cp_prefix_cache import module
from prove_native_cp_context import RTL
from test_r_front_forward_view import sha

PREFIX = '''module prefix_from_key(input wire [566:0] key,output wire [268:0] result);
 wire [397:0] before_pins=key[410:13];
 wire [397:0] prefix_pins={before_pins[397:293],key[11:1],before_pins[281:0]};
 wire [155:0] q;
 wire [55:0] events,repeated;
 wire unsettled;
 dorado_control_hold_atomic_step #(.WAVES(5)) actual(
  key[566:411],key[12],key[12],before_pins,prefix_pins,
  q,events,repeated,unsettled,key[0],);
 assign result={q,events,repeated,unsettled};
endmodule
'''
MITER = '''module prefix_support_equivalence(
 input wire [566:0] original_key,alternate_key,
 output wire equal,output wire [268:0] different);
 localparam [566:0] CARE=567'hMASK;
 wire [566:0] projected_key=(original_key & CARE)|(alternate_key & ~CARE);
 wire [268:0] original_result,projected_result;
 prefix_from_key original(original_key,original_result);
 prefix_from_key projected(projected_key,projected_result);
 assign different=original_result^projected_result;
 assign equal=~|different;
endmodule
'''


def run(stage, out, yosys):
    assert not out.exists()
    manifest = stage/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    for name, digest in m['source_hashes'].items():
        assert sha(stage/name) == digest
    raw = (stage/RTL).read_text()
    guard = module(raw, 'dorado_cp_prefix_cache')
    ordered = module(raw, 'dorado_control_hold_ordered_step')
    assert 'wire [566:0] current_key={current_q,before_pins,before_root,prefix_cp,refresh};' in guard
    assert 'wire [10:0] prefix_cp=split_event ? after_pins[292:282] : before_pins[292:282];' in guard
    assert 'assign prefix_live={raw_prefix_q,raw_prefix_events,raw_prefix_repeated,raw_prefix_unsettled};' in ordered
    assert '  state_q,clock_before,clock_before,pins_before,prefix_pins,' in ordered
    assert '  {pins_before[397:293],pins_after[292:282],pins_before[281:0]} : pins_before;' in ordered
    atomic = module(raw, 'dorado_control_hold_atomic_step')
    assert 'dorado_control_terminal_body' in atomic
    atomic += module(raw, 'dorado_control_terminal_body')
    out.mkdir(parents=True)
    (out/'actual-source.sv').write_text(raw)
    (out/'atomic.sv').write_text(atomic)
    (out/'prefix.sv').write_text(PREFIX)
    (out/'manifest.json').write_bytes(manifest.read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    env = os.environ.copy()
    env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
    env['YOWASP_MOUNT'] = '/work='+str(out)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
        stage=str(stage), manifest_sha256=sha(manifest), actual_RTL_sha256=sha(stage/RTL),
        compared_prefix_bits=269, key_bits=567, scope=__doc__, actions=[],
        cache_RTL_modified=False, native_timing_qualified=False, firmware_qualified=False,
        speedup_qualified=False, assembly_permitted=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    def action(label, script):
        (out/(label+'.ys')).write_text(script)
        command = [str(yosys), '-Q', '-T', '-s', '/work/'+label+'.ys']
        with (out/(label+'.log')).open('x') as log:
            p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
            row = dict(label=label, command=command, pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            row.update(returncode=p.wait(), finished=time.time())
            save()
        assert row['returncode'] == 0, label
        return (out/(label+'.log')).read_text()

    save()
    try:
        action('cone', '''read_verilog -sv /work/atomic.sv /work/prefix.sv
hierarchy -check -top prefix_from_key
proc
flatten
opt
check -assert
write_json /work/cone.json
''')
        net = json.loads((out/'cone.json').read_text())['modules']['prefix_from_key']
        drivers = {}
        for cell in net['cells'].values():
            ins = {b for name, bits in cell['connections'].items()
                   if cell['port_directions'][name] == 'input' for b in bits if isinstance(b, int)}
            for name, bits in cell['connections'].items():
                if cell['port_directions'][name] == 'output':
                    for b in bits:
                        if isinstance(b, int):
                            assert b not in drivers
                            drivers[b] = ins
        key = net['ports']['key']['bits']
        reached = set()
        pending = list(net['ports']['result']['bits'])
        while pending:
            bit = pending.pop()
            if not isinstance(bit, int) or bit in reached:
                continue
            reached.add(bit)
            pending.extend(drivers.get(bit, ()))
        care = [i for i, bit in enumerate(key) if bit in reached]
        assert care and len(care) < 567
        mask = sum(1 << i for i in care)
        state.update(cared_key_bits=care, unused_key_bits=[i for i in range(567) if i not in care],
            before_pin125_in_support=138 in care, cared_key_count=len(care))
        save()
        for label, value in [('positive', mask), ('missing-owner-bit', mask & ~(1 << max(care)))]:
            (out/(label+'.sv')).write_text(MITER.replace('MASK', format(value, 'x')))
            flag = 'verify' if label == 'positive' else 'falsify'
            log = action(label, f'''read_verilog -sv /work/atomic.sv /work/prefix.sv /work/{label}.sv
hierarchy -check -top prefix_support_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/{label}-counterexample.json
''')
            marker = 'no model found: SUCCESS!' if label == 'positive' else 'model found: FAIL!'
            assert marker in log
        state.update(status='PASS_BINARY_CP_PREFIX_SUPPORT_AUDIT', finished=time.time(),
            arbitrary_binary_input_bits=1134, input_assumptions='none',
            whole_prefix_projection_proven=True, missing_owner_bit_counterexample=True,
            four_state_cache_or_guard_rewrite_qualified=False)
    except BaseException:
        state.update(status='TERMINAL_CP_PREFIX_SUPPORT_NOT_ESTABLISHED', finished=time.time())
        save()
        raise
    save()
    print(json.dumps({k: state[k] for k in ['status', 'cared_key_count', 'before_pin125_in_support']}), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    run(a.stage.resolve(), a.output.resolve(), a.yosys.resolve())
