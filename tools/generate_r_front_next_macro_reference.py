#!/usr/bin/env python3
"""Close original ContA e22's IFUNext'a/NextMacro decode in the 792-Q graph.

Both wired-OR e22 outputs and d21's inverter are original package instances.
Their inputs are existing distinct MIR physical Qs. No new state or FPGA use.
"""
import argparse
import hashlib
import json
from pathlib import Path

from generate_processor_capture_reference import graph
from generate_r_front_local_reference import generate as parent_generate
from inventory_mir_clock_boundaries import ROOT

PARENT = 'verilog/rtl/dorado_r_front_local_step.sv'
CANDIDATE = 'verilog/rtl/dorado_r_front_next_macro_step.sv'
PARENT_SHA = '0337c3990e0ccd051f14108d1af22d1ea7a5d66cc7337b6c50ba8ef8a1066982'


def candidate(root, output):
    original = (root/PARENT).read_text()
    assert hashlib.sha256(original.encode()).hexdigest() == PARENT_SHA
    before = 'r[212]=!p[313]'
    # ContA e22.13 = JCN.6' | JCN.7' | JCN.5'; e22.4 =
    # JCN.1 | JCN.2'a | JCN.0. Both drive IFUNext'a; d21.14 inverts it.
    # JCN.2'a is d24.B's complement, not d23.B's separate physical copy.
    after = 'r[212]=mir[19]&mir[18]&mir[10]&!mir[8]&mir[15]&!mir[9]'
    assert original.count(before) == 1
    raw = original.replace('module dorado_r_front_local_step', 'module dorado_r_front_next_macro_step', 1)
    raw = raw.replace(before, after)
    assert raw.replace(after, before).replace('module dorado_r_front_next_macro_step',
                                             'module dorado_r_front_local_step', 1) == original
    output.write_text(raw)


def generate(root, out):
    out.mkdir(parents=True, exist_ok=False)
    parent = out/'parent-source-audit'
    manifest = parent_generate(root, parent)
    old = manifest['boards'][0]
    spec = dict(bn=old['board'], comb=sorted((set(old['packages'])-set(old['sequential']))|{'e22'}),
                seq=old['sequential'], module='r_front_next_conta',
                bindings={}, clock_taps=old['clock_taps'], flag_taps=old['flag_taps'])
    external = graph(root, out=None, inspect=True, **spec)
    assert set(external) == set(old['boundary_bindings'])-{"IFUNext'a"}
    spec['bindings'] = {n: old['boundary_bindings'][n] for n in external}
    board = graph(root, out=out, **spec)
    p = out/'r_front_next_conta.sv'
    raw = p.read_text().replace('input wire [27:0] pins',
          'input wire [314:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
    for n in ['a09', 'b09', 'c09', 'd09']:
        raw = raw.replace('original_F10000 '+n+'(', 'original_F10000_ready '+n+'(')
    p.write_text(raw)
    for n in ['proch', 'procl']:
        (out/f'r_front_ready_{n}.sv').write_bytes((parent/f'r_front_ready_{n}.sv').read_bytes())
    top = (parent/'r_front_local_reference.sv').read_text()
    assert top.count('r_front_local_conta c(') == 1
    (out/'r_front_local_reference.sv').write_text(top.replace('r_front_local_conta c(', 'r_front_next_conta c('))
    manifest['boards'][0] = board
    manifest['extra_boundaries'].pop("IFUNext'a")
    manifest.update(scope=__doc__, NextMacro_internal=True, obsolete_external_slot=313,
                    packages=sum(len(b['packages']) for b in manifest['boards']),
                    original_pins=sum(b['original_pins'] for b in manifest['boards']),
                    full_native_producers_closed=False)
    (out/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    return manifest


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, default=ROOT)
    p.add_argument('--candidate', type=Path)
    p.add_argument('--reference', type=Path)
    a = p.parse_args()
    assert a.candidate or a.reference
    if a.candidate:
        assert not a.candidate.exists()
        candidate(a.root.resolve(), a.candidate.resolve())
    if a.reference:
        generate(a.root.resolve(), a.reference.resolve())
