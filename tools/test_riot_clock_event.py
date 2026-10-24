#!/usr/bin/env python3
"""Qualify explicit BaseBoard RIOT events against the PHI2-clocked part.

Compare all73 scalar state bits and128 RAM bytes after every host edge,
including arbitrary bus changes, pauses, reset-without-edge, all prescalers,
IRQ clears and RAM reads/writes. Keep the vendor implementation unchanged
for Verilator; lower only array assignment syntax if Icarus cannot parse it.
No whole BaseBoard/native timing/6502/firmware or speed pass follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
MUTATIONS = {
    'ignore-consume': ('consume && phi_rise', "1'b1 && phi_rise"),
    'ignore-edge': ('consume && phi_rise', "consume && 1'b1"),
    'reset-without-edge': ('!commit || reset_n_at_edge', 'reset_n_at_edge'),
    'late-event': ('wire commit=consume && phi_rise;',
                   'reg commit=0; always @(posedge clk)commit<=consume && phi_rise;'),
}


def portable_array_syntax(raw):
    match = re.search(r"riot_ram <= '\{(.*?)\};", raw, re.S)
    assert match
    values = re.findall(r"8'h([0-9A-Fa-f]{2})", match[1])
    assert len(values) == 128
    explicit = 'begin\n'+''.join(f"riot_ram[{i}] <= 8'h{v};\n" for i, v in enumerate(values))+'end'
    changed = raw[:match.start()]+explicit+raw[match.end():]
    old = "riot_ram <= '{128{8'h00}};"
    assert changed.count(old) == 1
    changed = changed.replace(old, "for(integer reset_index=0;reset_index<128;reset_index++)riot_ram[reset_index]<=8'h00;", 1)
    return changed


def run(out, observed_stage=None):
    assert not out.exists()
    out.mkdir(parents=True)
    inputs = out/'inputs'
    inputs.mkdir()
    files = {'M6532.sv': ROOT/'verilog/vendor/riot/M6532.sv',
             'dorado_riot_clock_event.sv': ROOT/'verilog/rtl/dorado_riot_clock_event.sv',
             'tb_riot_clock_event.sv': ROOT/'verilog/verilator/tb_riot_clock_event.sv'}
    for name, path in files.items():
        shutil.copy2(path, inputs/name)
    if observed_stage is not None:
        manifest=json.loads((observed_stage/'provenance.json').read_text())
        for name,digest in manifest['files'].items():
            assert sha(observed_stage/name)==digest
        for name,source in [('M6532.sv','inputs/src/verilog/vendor/riot/M6532.sv'),
                            ('dorado_riot_clock_event.sv','inputs/experiment/dorado_riot_clock_event.sv')]:
            shutil.copy2(observed_stage/source,inputs/name)
        original=(ROOT/'verilog/vendor/riot/M6532.sv').read_text().replace('module M6532','module M6532_original',1)
        (inputs/'M6532-reference.sv').write_text(original)
        (inputs/'M6532-reference-icarus.sv').write_text(portable_array_syntax(original))
        p=inputs/'tb_riot_clock_event.sv'
        raw=p.read_text().replace('M6532 reference(', 'M6532_original reference(',1)
        raw=raw.replace('dut.u_riot.riot_stuff.old_pa7','dut.u_riot.old_pa7')
        raw=raw.replace(' wire irq,oe,ref_irq,ref_oe;',' wire irq,oe,ref_irq,ref_oe;\n wire [1096:0] observed;')
        assert raw.count('dout,pa,pb,irq,oe);')==1
        raw=raw.replace('dout,pa,pb,irq,oe);','dout,pa,pb,irq,oe,observed);',1)
        fields=['out_a','out_b','dir_a','dir_b','d_out','interrupt','timer','prescaler','incr','rollover','irq_en','edge_detect','old_pa7']
        expressions=['dut.u_riot.'+f for f in fields]+[f'dut.u_riot.riot_ram[{i}]' for i in range(128)]
        raw=raw.replace('   state_checks=state_checks+1;',
            '   if(observed !== {'+','.join(expressions)+'})$fatal(1,"RIOT_EVENT observation port mismatch");\n   state_checks=state_checks+1;',1)
        p.write_text(raw)
    raw = (inputs/'dorado_riot_clock_event.sv').read_text()
    for name, (before, after) in MUTATIONS.items():
        assert raw.count(before) == 1
        (inputs/(name+'.sv')).write_text(raw.replace(before, after, 1))
    (inputs/'M6532-icarus.sv').write_text(portable_array_syntax((inputs/'M6532.sv').read_text()))
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        scalar_bits=73, RAM_bytes=128, original_vendor_source_unchanged_for_Verilator=observed_stage is None,
        original_vendor_reference_unchanged_for_Verilator=True,
        Icarus_array_lowering_only=True, native_integrated=False, hardware_changed=False,
        timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        observed_stage=str(observed_stage) if observed_stage else None,
        observed_scope_change_compared_to_UNMODIFIED_reference=observed_stage is not None,
        inputs={p.name: sha(p) for p in inputs.iterdir()}, cases=[])

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for engine in ['icarus', 'verilator']:
        for variant in ['positive', *MUTATIONS]:
            d = out/(engine+'-'+variant)
            d.mkdir()
            paths = [inputs/('dorado_riot_clock_event.sv' if variant == 'positive' else variant+'.sv'),
                     inputs/('M6532-icarus.sv' if engine == 'icarus' else 'M6532.sv'), inputs/'tb_riot_clock_event.sv']
            if observed_stage is not None:
                paths.append(inputs/('M6532-reference-icarus.sv' if engine=='icarus' else 'M6532-reference.sv'))
            if engine == 'icarus':
                commands = [['iverilog', '-g2012', '-s', 'tb_riot_clock_event', '-o', str(d/'test.vvp'), *map(str, paths)],
                            ['vvp', '-i', str(d/'test.vvp')]]
            else:
                commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_riot_clock_event',
                             '--Mdir', str(d/'obj'), '-o', 'test', *map(str, paths)], [str(d/'obj/test')]]
            row = dict(engine=engine, variant=variant, expected_rejection=variant != 'positive', actions=[])
            state['cases'].append(row)
            save()
            for label, command in zip(['compile', 'test'], commands):
                with (d/(label+'.log')).open('x') as log:
                    p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
                    action = dict(label=label, command=command, pid=p.pid, started=time.time())
                    row['actions'].append(action)
                    save()
                    action.update(returncode=p.wait(), finished=time.time())
                    save()
                if action['returncode'] != 0:
                    break
            log = (d/'test.log').read_text() if (d/'test.log').exists() else ''
            passed = action['returncode'] == 0 and 'RIOT_EVENT_PASS ' in log if variant == 'positive' else (
                row['actions'][0]['returncode'] == 0 and action['returncode'] != 0 and 'RIOT_EVENT ' in log)
            row.update(passed=passed, test_log_sha256=sha(d/'test.log') if (d/'test.log').exists() else None)
            save()
            print(engine, variant, 'PASS' if passed else 'FAIL', flush=True)
            if not passed:
                state.update(status='TERMINAL_RIOT_EVENT_NOT_ESTABLISHED', finished=time.time())
                save()
                return 1
    state.update(status='PASS_RIOT_EXPLICIT_EVENT_ORIGINAL_STATE_AND_RAM', finished=time.time())
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--observed-stage',type=Path)
    a = p.parse_args()
    raise SystemExit(run(a.output.resolve(),a.observed_stage.resolve() if a.observed_stage else None))
