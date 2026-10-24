#!/usr/bin/env python3
"""Compare the first actual board-clock consumer with original event packages.

Bind e07/c08/g08/j02 to the original BaseBoard wire list and native rails.
Retain all six physical Qs,64 imported initial states,eight CPU rotations,
boundary debug pulses, every derived edge, transport pauses and full300us
source period. Compare primary packets to the independent absolute schedule.
No6502/RIOT/full-board, analog setup/hold, native fit or speed pass follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from sil_netlist import load_board
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
MUTATIONS = {
    'wrong-reload': [("old_max ? 4'hb", "old_max ? 4'ha")],
    'drop-g08b': [('if(!old_pre && new_pre)begin events[2]=1;', "if(1'b0)begin events[2]=1;")],
    'ignore-debug': [('before_levels[0]|before_levels[1]|before_debug', 'before_levels[2]'),
                     ('after_levels[0]|after_levels[1]|after_debug', 'after_levels[2]')],
    'drop-boundary': [('events_at_steps[7*index+:7]=transfer[12:6];', "events_at_steps[7*index+:7]=(index==0) ? 7'b0 : transfer[12:6];")],
    'advance-held': [('if(consume)for', "if(1'b1)for")],
}


def run(stage, out, candidate=None, schedule=None):
    assert not out.exists()
    manifest = json.loads((stage/'r-front-control-native.json').read_text())
    for name, digest in manifest['source_hashes'].items():
        assert sha(stage/name) == digest
    wl = ROOT/'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl'
    board = load_board(str(wl))
    pins = {(p['pkg'], p['pin']): name for name, net in board.nets.items() for p in net['pins']}
    expected = {
        'e07': ('SN74LS163', {1:'TTLTrue.A',2:'Basebd01.sil+4',3:'TTLTrue.A',4:'TTLTrue.A',5:'GND202',6:'TTLTrue.A',7:'TTLTrue.A',9:'Basebd01.sil+5',10:'TTLTrue.A',15:'Max'}),
        'c08': ('SN74LS04', {1:'Max',2:'Basebd01.sil+5'}),
        'g08': ('SN74LS74', {1:'TTLTrue.A',2:'MCPreClk',3:'Max',4:'TTLTrue.A',6:'MCPreClk',8:'BaseBd10.sil+3',10:'TTLTrue.B',11:'MCPreClk',12:'BaseBd10.sil+3',13:'TTLTrue.B'}),
        'j02': ('MC10210', {5:"StartClockPulse'",6:'EndClockPulse',7:'dStartClockPulse',9:"StartClockPulse'",10:'EndClockPulse',11:'dStartClockPulse'}),
    }
    for package, (kind, connections) in expected.items():
        assert board.packages[package]['type'] == kind
        for pin, net in connections.items():
            assert pins[package, pin] == net, (package, pin, pins[package, pin], net)
    base = stage/'src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
    text = base.read_text()
    assert "assign TTLTrue_A = 1'b1;" in text and "assign TTLTrue_B = 1'b1;" in text
    out.mkdir(parents=True)
    inputs = out/'inputs'
    inputs.mkdir()
    files = {name: ROOT/'verilog/rtl'/name for name in ['dorado_primary_clock_events.sv', 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv', 'dorado_baseboard_clock_step.sv']}
    files.update({name: ROOT/'verilog/verilator'/name for name in ['baseboard_clock_original.sv', 'tb_baseboard_clock_events.sv']})
    files.update({name: ROOT/'verilog/cells'/name for name in ['cell_MC10210.v', 'cell_SN74LS04.v']})
    files.update({'BaseBd.wl': wl, 'BaseBd_nominal.v': base})
    if candidate is not None:
        for name in ['dorado_primary_clock_events.sv', 'dorado_baseboard_clock_step.sv']:
            files[name] = candidate/name
    schedule = schedule or ROOT/'build/rtl-executive/single-bb-primary-reference-v1-20261024'
    s = json.loads((schedule/'run-state.json').read_text())
    assert s['status'] == 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES'
    for name in ['dorado_primary_clock_events.sv', 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv']:
        assert sha(files[name]) == s['inputs'][name]
    assert sha(schedule/'inputs/expected.hex') == s['inputs']['expected.hex']
    files['expected.hex'] = schedule/'inputs/expected.hex'
    for name, path in files.items():
        shutil.copy2(path, inputs/name)
    parallel = 'state_q,source_before,source_after,' in (inputs/'dorado_baseboard_clock_step.sv').read_text()
    if candidate is not None:
        # Add only the endpoint input. Every original package, test and check
        # stays byte-identical, including all intermediate states/edge counts.
        tb = inputs/'tb_baseboard_clock_events.sv'
        old = 'consumer(owners[p],before_sources[p],counts[p]'
        raw_tb = tb.read_text()
        new = 'consumer(owners[p],before_sources[p],after_sources,counts[p]'
        assert raw_tb.count(old)+raw_tb.count(new) == 1
        tb.write_text(raw_tb.replace(old, new, 1) if parallel else raw_tb.replace(new, old, 1))
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    raw = (inputs/'dorado_baseboard_clock_step.sv').read_text()
    # Preserve the original j02 equation while binding its pins as one packet.
    gate = re.search(r'assign p2 = ([^;]+);', (inputs/'cell_MC10210.v').read_text())[1]
    for pin, expression in [('p5','source_and_debug[0]'), ('p6','source_and_debug[1]'), ('p7','source_and_debug[6]')]:
        gate = re.sub(r'\b'+pin+r'\b', expression, gate)
    terms = gate.strip('()').split(' | ')
    assert len(terms) == 3 and 'or j02(root, '+', '.join(terms)+');' in (inputs/'baseboard_clock_original.sv').read_text()
    mutations = dict(MUTATIONS)
    if parallel:
        mutations['drop-boundary'] = [('events_at_steps[7*index+:7]=selected_events;', "events_at_steps[7*index+:7]=(index==0) ? 7'b0 : selected_events;")]
    for name, replacements in mutations.items():
        changed = raw
        for before, after in replacements:
            assert changed.count(before) == 1
            changed = changed.replace(before, after, 1)
        (inputs/(name+'.sv')).write_text(changed)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        physical_state_bits=6, source_lanes=6, initial_states=64, CPU_phases=8,
        source_geometry_or_reference_checks_removed=False, extra_guest_cycles=0,
        native_consumer_integrated=False, analog_timing_qualified=False,
        native_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        hardware_changed=False, cases=[], inputs={p.name: sha(p) for p in inputs.iterdir()})
    state.update(candidate=str(candidate) if candidate else None, schedule=str(schedule),
                 schedule_state_sha256=sha(schedule/'run-state.json'), single_BB_transfer=parallel)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for engine in ['icarus', 'verilator']:
        for variant in ['positive', *MUTATIONS]:
            d = out/(engine+'-'+variant)
            d.mkdir()
            consumer = inputs/('dorado_baseboard_clock_step.sv' if variant == 'positive' else variant+'.sv')
            sources = [inputs/name for name in ['dorado_primary_clock_events.sv', 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv', 'cell_MC10210.v', 'cell_SN74LS04.v', 'baseboard_clock_original.sv', 'tb_baseboard_clock_events.sv']]+[consumer]
            if engine == 'icarus':
                commands = [['iverilog', '-g2012', '-s', 'tb_baseboard_clock_events', '-o', str(d/'test.vvp'), *map(str, sources)],
                            ['vvp', '-i', str(d/'test.vvp'), '+expected='+str(inputs/'expected.hex')]]
            else:
                commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_baseboard_clock_events',
                             '--Mdir', str(d/'obj'), '-o', 'test', *map(str, sources)], [str(d/'obj/test'), '+expected='+str(inputs/'expected.hex')]]
            c = dict(engine=engine, variant=variant, actions=[], expected_rejection=variant != 'positive')
            state['cases'].append(c)
            save()
            for label, command in zip(['compile', 'test'], commands):
                with (d/(label+'.log')).open('x') as log:
                    process = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
                    action = dict(label=label, command=command, pid=process.pid, started=time.time())
                    c['actions'].append(action)
                    save()
                    action.update(returncode=process.wait(), finished=time.time())
                    save()
                if action['returncode'] != 0:
                    break
            log = (d/'test.log').read_text() if (d/'test.log').exists() else ''
            passed = (action['returncode'] == 0 and 'BB_CONSUMER_PASS phases=8 initial_states=64 samples=176384' in log) if variant == 'positive' else (
                c['actions'][0]['returncode'] == 0 and action['returncode'] != 0 and 'BB_CONSUMER ' in log)
            c.update(passed=passed, test_log_sha256=sha(d/'test.log') if (d/'test.log').exists() else None)
            save()
            print(engine, variant, 'PASS' if passed else 'FAIL', flush=True)
            if not passed:
                state.update(status='TERMINAL_BASEBOARD_CONSUMER_NOT_ESTABLISHED', finished=time.time())
                save()
                return 1
    state.update(status='PASS_BASEBOARD_CLOCK_EVENT_CONSUMER_REFERENCES', finished=time.time())
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--candidate', type=Path)
    p.add_argument('--schedule', type=Path)
    args = p.parse_args()
    raise SystemExit(run(args.stage.resolve(), args.output.resolve(),
                        args.candidate.resolve() if args.candidate else None,
                        args.schedule.resolve() if args.schedule else None))
