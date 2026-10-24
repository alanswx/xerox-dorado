#!/usr/bin/env python3
"""Qualify a common CPU/pixel chronology against original divider connections.

Bind the real native wrapper and original BaseBoard wire list. An independent
four-flop divider/absolute1.25ns reference checks every timestamp, mask and
post-event level, all eight rotations, resets, pauses and simultaneous events.
The batch is not connected to native consumers; no FPGA speed claim follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from sil_netlist import load_board
from test_oscillator_events import sha, BP, WRAPPER, DISPLAY

ROOT = Path(__file__).resolve().parents[1]


def run(stage, out, oscillator_source=None):
    assert not out.exists()
    manifest = json.loads((stage/'r-front-control-native.json').read_text())
    for name, digest in manifest['source_hashes'].items():
        assert sha(stage/name) == digest, name
    wl = ROOT/'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl'
    board = load_board(str(wl))
    expected = {
        'g03': ('MC1690', {2: 'BaseBd11.sil+15', 7: 'BaseBd11.sil+2', 11: 'BaseBd11.sil+4'}),
        'g04': ('MC1690', {2: 'BaseBd11.sil+4', 3: "StartClockPulse'", 7: 'BaseBd11.sil+1', 11: 'BaseBd11.sil+3'}),
        'h03': ('MC1690', {2: 'EndClockPulse', 7: 'BaseBd11.sil+2', 11: 'BaseBd11.sil+15'}),
        'h04': ('MC1690', {2: 'DoubClkFeedBack', 3: 'BaseBd11.sil+3', 7: 'BaseBd11.sil+1', 11: 'BaseBd11.sil+4'}),
        'g05': ('MC1660', {2: 'BaseBd11.sil+2', 3: 'BaseBd11.sil+1', 7: 'BaseBd11.sil+14'}),
    }
    for package, (kind, pins) in expected.items():
        assert board.packages[package]['type'] == kind
        actual = {p['pin']: name for name, net in board.nets.items() for p in net['pins'] if p['pkg'] == package}
        assert actual == pins, (package, actual)
    wrapper = (stage/WRAPPER).read_text()
    assert 'cell_K1115A #(.FREQ_KHZ(50000),.SYSPER(2*SYSPER)) u_pixclk' in wrapper
    assert '.RawPixelClk(pixel_clk)' in wrapper
    display = (stage/DISPLAY).read_text()
    b01 = re.search(r'cell_MC10102 u_b01 \(.*?\n  \);', display, re.S)[0]
    assert '.p7(RawPixelClk)' in b01
    out.mkdir(parents=True)
    inputs = out/'inputs'
    inputs.mkdir()
    paths = {
        'dorado_clock_event_batch.sv': ROOT/'verilog/rtl/dorado_clock_event_batch.sv',
        'dorado_oscillator_events.sv': oscillator_source or ROOT/'verilog/rtl/dorado_oscillator_events.sv',
        'tb_clock_event_batch.sv': ROOT/'verilog/verilator/tb_clock_event_batch.sv',
        'native-wrapper.sv': stage/WRAPPER, 'native-backplane.v': stage/BP,
        'native-display.sv': stage/DISPLAY, 'native-manifest.json': stage/'r-front-control-native.json',
        Path(__file__).name: Path(__file__), 'sil_netlist.py': ROOT/'tools/sil_netlist.py',
    }
    for suffix in ['.wl', '.lc', '.bp']:
        p = wl.with_suffix(suffix)
        if p.exists():
            paths[p.name] = p
    for name, p in paths.items():
        (inputs/name).write_bytes(p.read_bytes())
    rtl = (inputs/'dorado_clock_event_batch.sv').read_text()
    variants = {
        'positive': rtl,
        'drop-second': rtl.replace('(pixel_count==2 && pixel_second==', "(1'b0 && pixel_second==", 1),
        'late-cpu': rtl.replace('if(quantum%3==0)', 'if(quantum%3==1)', 1),
        'lose-coincident': rtl.replace('masks[4*event_count+:4]=changed;',
            "masks[4*event_count+:4]=(changed[3] && |changed[2:0]) ? (changed & 4'b0111) : changed;", 1),
        'advance-held': rtl.replace('else if(advance)cpu_phase<=', "else if(1'b1)cpu_phase<=", 1),
        'reverse-order': rtl.replace('levels_after=cursor;', '''levels_after=cursor;
  if(event_count>=2)begin
   offsets[7:0]={offsets[3:0],offsets[7:4]};
   masks[7:0]={masks[3:0],masks[7:4]};
   levels[7:0]={levels[3:0],levels[7:4]};
  end''', 1),
    }
    assert len(set(variants.values())) == 6
    for name, raw in variants.items():
        (inputs/(name+'.sv')).write_text(raw)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        native_manifest_sha256=sha(stage/'r-front-control-native.json'), original_packages=expected,
        inputs={p.name: sha(p) for p in inputs.iterdir()}, cases=[], actions=[],
        native_consumers_connected=False, native_timing_qualified=False, firmware_qualified=False,
        speedup_qualified=False, hardware_changed=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    def execute(label, command):
        with (out/(label+'.log')).open('x') as log:
            p = subprocess.Popen(list(map(str, command)), stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, command=list(map(str, command)), pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            row.update(returncode=p.wait(), finished=time.time())
            save()
        return row['returncode']

    save()
    try:
        for engine in ['icarus', 'verilator']:
            for name in variants:
                label = engine+'-'+name
                sources = [inputs/(name+'.sv'), inputs/'dorado_oscillator_events.sv', inputs/'tb_clock_event_batch.sv']
                if engine == 'icarus':
                    commands = [['iverilog', '-g2012', '-s', 'tb_clock_event_batch', '-o', out/(label+'.vvp'), *sources],
                                ['vvp', out/(label+'.vvp')]]
                else:
                    obj = out/('obj-'+name)
                    commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal',
                                 '--top-module', 'tb_clock_event_batch', '--Mdir', obj, '-o', 'test', *sources], [obj/'test']]
                assert execute(label+'-compile', commands[0]) == 0
                code = execute(label+'-test', commands[1])
                log = (out/(label+'-test.log')).read_text()
                if name == 'positive':
                    assert code == 0 and 'CLOCK_BATCH_ALL_PASS rotations=8' in log
                    rows = re.findall(r'CLOCK_BATCH_PASS phase=(\d+) checks=(\d+) paused=(\d+) coincident=(\d+) full=(\d+) resets=(\d+)', log)
                    assert len(rows) == 8 and {int(r[0]) for r in rows} == set(range(8))
                    assert all(r[1] == '2048' and r[5] == '3' and int(r[2]) > 100 for r in rows)
                    assert any(int(r[3]) > 0 for r in rows) and any(int(r[4]) > 0 for r in rows)
                else:
                    assert code != 0 and re.search(r'CLOCK_BATCH (timeline mismatch|committed phase mismatch)', log)
                state['cases'].append(dict(engine=engine, variant=name, passed=True, returncode=code,
                    expected_rejection=name != 'positive', log_sha256=sha(out/(label+'-test.log'))))
                save()
        state.update(status='PASS_COMMON_CPU_PIXEL_EVENT_BATCH', finished=time.time())
    except BaseException:
        state.update(status='TERMINAL_CLOCK_BATCH_TEST_NOT_ESTABLISHED', finished=time.time())
        save()
        raise
    save()
    print(state['status'], flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--oscillator-source', type=Path)
    a = p.parse_args()
    run(a.stage.resolve(), a.output.resolve(), a.oscillator_source.resolve() if a.oscillator_source else None)
