#!/usr/bin/env python3
"""Original task pipeline plus control/processor capture/address graph.

Reference uses original combinational pin functions and independent event FFs.
Ready/IsNext storage, MIR block/tasking decode and memory-Hold producers remain
boundary pins. Next, SwitchUp and CTask are internal feedback, never supplied
by the candidate or the test fixture.
"""
import argparse
import hashlib
import json
from pathlib import Path

from generate_processor_capture_reference import graph, specs as capture_specs
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]
TASK_SEQ = {'h08': (32, 4), 'h09': (36, 4), 'f14': (40, 2),
            'f15': (42, 2), 'j08': (44, 2), 'j09': (46, 2)}
TASK_COMB = ['a02', 'd18', 'h14', 'h13', 'h10', 'h11', 'i10', 'j10', 'j11',
             'g15', 'f16', 'f17', 'a10', 'b10', 'c10', 'd10', 'a11', 'b11',
             'c11', 'd11', 'd12', 'c12', 'd13', 'e12', 'e13', 'f10', 'f11',
             'g10', 'g11', 'i11']


def specs():
    result = capture_specs()
    c = result[0]
    c['comb'] = sorted(set(c['comb'] + TASK_COMB))
    c['seq'] = {**c['seq'], **TASK_SEQ}
    c['module'] = 'task_conta'
    bindings = c['bindings']
    del bindings['SwitchUp']
    bindings['Hold'] = 'pins[69]'
    bindings.update({'StopTasks': 'pins[65]', "dBlock'": 'pins[66]', "Block'": 'pins[67]',
                     'rCT': 'pins[70]', 'rMIRa': "1'b0", 'ContA28.sil+2': "1'b0",
                     'ContA28.sil+3': "1'b0", 'ContA28.sil+4': "1'b0",
                     "WTPC'": "1'b1", "FF=TOffIsOK": "1'b0",
                     'FF=TaskingOff': "1'b0"})
    for task in range(1, 16):
        bindings[f'TWReq.{task:02}'] = f'pins[{19+task}]'
        bindings[f'Ready.{task:02}'] = f'pins[{34+task}]'
        bindings[f"IsNext.{task:02}'"] = f'pins[{49+task}]'
    bindings.update({'sCTask.' + str(i): f'pins[{71+i}]' for i in range(4)})
    c['clock_taps'] += ['@f14.ca']
    c['flag_taps'] += ['Next.' + str(i) for i in range(4)]
    c['flag_taps'] += ["BNTGtCT'a", 'Switcha', 'SwitchUp']
    for low, spec in zip([False, True], result[1:]):
        spec['module'] = 'task_procl' if low else 'task_proch'
        spec['seq'] = {**spec['seq'], ('i14' if low else 'i12'): (16, 4)}
        spec['comb'] = sorted(set(spec['comb'] + ['a18', 'a19', 'j18'] + (['k10'] if low else [])))
        spec['bindings']['PRhold'] = 'pins[68]'
        spec['bindings']["BNTGtCT'a"] = 'priority_n'
        for i in range(4):
            spec['bindings']['Next.' + str(i)] = f'next_raw[{i}]'
        if not low:
            spec['bindings']['Holda'] = 'pins[68]'
            spec['bindings']["TIOAWriteEn'"] = "1'b1"
        spec['clock_taps'] += ["DblClock'Bc" if low else "DblClock'Da"]
    return result


def generate(root, out, inspect=False):
    if inspect:
        for spec in specs():
            external = graph(root, out=None, inspect=True, **spec)
            print(spec['bn'], 'unbound', sorted(set(external) - set(spec['bindings'])),
                  'extra', sorted(set(spec['bindings']) - set(external)))
        return
    out.mkdir(parents=True, exist_ok=False)
    records = [graph(root, out=out, **s) for s in specs()]
    # Widen the inherited fixture API only; physical graph/port equations stay
    # extracted from the original wire lists. Real Next/priority are ports,
    # driven from the control graph's actual net taps in the wrapper below.
    for spec in specs():
        path = out / (spec['module'] + '.sv')
        text = path.read_text()
        assert text.count('input wire [27:0] pins') == 1
        text = text.replace('input wire [27:0] pins',
                            'input wire [75:0] pins,input wire [3:0] next_raw,input wire priority_n')
        path.write_text(text)
    # Audit the same actual BaseBoard fanout and named backplane seams as the
    # qualified capture generator, without borrowing candidate expressions.
    from generate_processor_capture_reference import generate as generate_capture
    capture_manifest = generate_capture(root, out / 'capture-source-audit')
    for spec in specs()[1:]:
        b = load_board(str(root / 'chm/sil' / (spec['bn'] + '.dm!1_') / (spec['bn'] + '.wl')))
        c = load_board(str(root / 'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
        assert c.nets["BNTGtCT'a"]['backplane'] == {'C173'}
        assert b.nets["BNTGtCT'a"]['backplane'] == {'C173'}
        for i in range(4):
            assert c.nets['Next.' + str(i)]['backplane'] == {['E6','E7','E10','E11'][i]}
            assert b.nets['Next.' + str(i)]['backplane'] == {['E6','E7','E10','E11'][i]}
    body = '''`timescale 1ns/1ps
module task_capture_reference(input wire fixture_load,input wire [87:0] fixture_state,
 input wire base,input wire [75:0] pins,output wire [87:0] state_q,
 output wire [24:0] clocks,output wire [8:0] flags,output wire [3:0] next_raw);
 wire [2:0] bases;
 cell_MC10210 base_fanout(.p7(base),.p11(base),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(1'b0),
   .p2(bases[2]),.p12(bases[1]),.p13(bases[0]));
 wire [12:0] cc;wire [5:0] ch,cl;wire [12:0] fc;wire [2:0] fh,fl;
 wire en_h=fc[3],en_l=fc[4],start_a=fc[5],priority_n=fc[10];
 task_conta c(fixture_load,{fixture_state[79:64],fixture_state[31:0]},bases[2],1'b0,1'b0,
   pins,next_raw,priority_n,{state_q[79:64],state_q[31:0]},cc,fc);
 task_proch h(fixture_load,{fixture_state[83:80],fixture_state[47:32]},bases[1],start_a,en_h,
   pins,next_raw,priority_n,{state_q[83:80],state_q[47:32]},ch,fh);
 task_procl l(fixture_load,{fixture_state[87:84],fixture_state[63:48]},bases[0],start_a,en_l,
   pins,next_raw,priority_n,{state_q[87:84],state_q[63:48]},cl,fl);
 assign next_raw=fc[9:6];
 assign clocks={cl[5],ch[5],cc[12],cc[11:9],cl[4:0],ch[4:0],cc[8:0]};
 assign flags={fl,fh,fc[2:0]};
endmodule
'''
    (out / 'task_capture_reference.sv').write_text(body)
    manifest = dict(boards=records, physical_bits=sum(r['physical_bits'] for r in records),
                    packages=sum(len(r['packages']) for r in records),
                    original_pins=sum(r['original_pins'] for r in records),
                    source_audit=capture_manifest,
                    generator_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print('TASK_CAPTURE_REFERENCE', manifest['physical_bits'], 'Qs', manifest['packages'],
          'packages', manifest['original_pins'], 'pins')
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--inspect', action='store_true')
    args = parser.parse_args()
    if not args.inspect and not args.output:
        parser.error('Specify --output or --inspect')
    generate(args.root.resolve(), args.output.resolve() if args.output else None, args.inspect)
