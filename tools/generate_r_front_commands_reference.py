#!/usr/bin/env python3
"""Close original ContA count-branch and IM/TPC command producers.

Add a14/a21/b15/d19/f22/f23/d20/k11 to the original 792-Q graph. Keep all
physical MIR copies, wired-OR outputs and clock/data epochs. No native use.
"""
import argparse
import hashlib
import json
from pathlib import Path

from generate_processor_capture_reference import graph
from generate_r_front_next_macro_reference import generate as parent_generate
from inventory_mir_clock_boundaries import ROOT

CANDIDATE = 'verilog/rtl/dorado_r_front_commands_step.sv'
PARENT = 'verilog/rtl/dorado_r_front_next_macro_step.sv'
CONTROL = 'verilog/rtl/dorado_conta_local_step.sv'
FUNCTIONS = ''' function automatic [2:0] memory_commands(input [21:0] mir);
  reg return_group,read_im_tpc,wim_n,wtpc_n;
  begin
   // e21 Return'a plus d20's actual JCN.2'a/3'a/4' decoder inputs.
   // k11.9 NANDs RIM' and RTPC'. Use d24's copy of JCN.2 here.
   return_group=!mir[9]&mir[8]&mir[10]&mir[19]&mir[18];
   read_im_tpc=return_group&mir[15]&!mir[11];
   wim_n=!(return_group&mir[15]&mir[14]&mir[11]);
   wtpc_n=!(return_group&mir[15]&!mir[14]&mir[11]);
   memory_commands={wtpc_n,wim_n,read_im_tpc};
  end
 endfunction
'''
BRANCH = ''' function automatic count_branch(input [21:0] mir,input [316:0] p);
  reg ffok_n,fa0_n,fb6_n,ff_branch,cond_branch_n,j_branch;
  begin
   // f24/a21 wired-OR FA=0', a19 FB=6', b15 and d19 Q3,
   // f22/f23 wired-OR CondBr'a, a14.15 and d21's inverter.
   ffok_n=!p[205]|!(mir[9]|mir[8]|mir[12]|mir[13]);
   fa0_n=ffok_n|(!p[294]&(mir[2]|mir[3]));
   fb6_n=({mir[1],mir[0],mir[7]}!=3'd6);
   ff_branch=!fa0_n&!fb6_n&({mir[6],mir[5],mir[4]}==3'd3);
   cond_branch_n=!(mir[15]|mir[14]|mir[8])|(mir[10]&mir[18]&mir[19])|mir[9];
   j_branch=!cond_branch_n&({mir[10],mir[19],mir[18]}==3'd3);
   count_branch=ff_branch|j_branch;
  end
 endfunction
'''


def candidate(root, output):
    original = (root/PARENT).read_text()
    control = (root/CONTROL).read_text()
    assert hashlib.sha256(original.encode()).hexdigest() == 'cac2c60601194c10f24ba785d620c84ce2d961b32c9682af4d2f6a9670f6a733'
    assert hashlib.sha256(control.encode()).hexdigest() == 'e109ac28840e67e0415a04e1f121384e980ad0e8aa4bbbc9de875e6a5212f934'
    edits = [
        ('module dorado_r_front_next_macro_step', 'module dorado_r_front_commands_step'),
        ('[314:0]', '[316:0]'),
        ('dorado_conta_local_step conta(', 'dorado_conta_commands_step conta('),
        (' function automatic [7:0] local_decode', FUNCTIONS+BRANCH+' function automatic [7:0] local_decode'),
        ('reg [245:0] r;reg [7:0] d;', 'reg [245:0] r;reg [7:0] d;reg [2:0] m;'),
        ('d=local_decode(producer,conta_pins(p));r=p[245:0];', 'd=local_decode(producer,conta_pins(p));m=memory_commands(mir);r=p[245:0];r[8]=m[1];'),
        ('r[235]=!p[311]', 'r[235]=count_branch(mir,p)'),
        (' reg [245:0] after_child;', ' wire [2:0] settled_commands=memory_commands(conta_next[97:76]);\n reg [245:0] after_child;'),
        ('after_child=after_raw;', 'after_child=after_raw;after_child[8]=settled_commands[1];'),
    ]
    raw = original
    for before, after in edits:
        assert before in raw, before
        if before != '[314:0]':
            assert raw.count(before) == 1, before
        raw = raw.replace(before, after)
    inverse = raw
    for before, after in reversed(edits):
        inverse = inverse.replace(after, before)
    assert inverse == original
    edits_c = [
        ('module dorado_conta_local_step', 'module dorado_conta_commands_step'),
        (' function automatic [7:0] local_decode', FUNCTIONS+' function automatic [7:0] local_decode'),
        ('reg [8:0] c;reg [7:0] d;', 'reg [8:0] c;reg [7:0] d;reg [2:0] m;'),
        ('d=local_decode(q,p);c=p[8:0];', 'd=local_decode(q,p);m=memory_commands(q[97:76]);c=p[8:0];c[8]=m[1];'),
        ('reg settled;reg [7:0] old_local;', 'reg settled;reg [7:0] old_local;reg [2:0] old_memory;'),
        ('old_local=local_decode(state_q,pins_after);', 'old_local=local_decode(state_q,pins_after);old_memory=memory_commands(state_q[97:76]);'),
        ('!old_local[0],pins_after[128]', '!old_local[0],old_memory[0]'),
    ]
    next_c = control
    for before, after in edits_c:
        assert next_c.count(before) == 1, before
        next_c = next_c.replace(before, after)
    inverse = next_c
    for before, after in reversed(edits_c):
        inverse = inverse.replace(after, before)
    assert inverse == control
    output.write_text(raw+'\n'+next_c)


def generate(root, out):
    out.mkdir(parents=True, exist_ok=False)
    parent = out/'parent-source-audit'
    manifest = parent_generate(root, parent)
    old = manifest['boards'][0]
    comb = set(old['packages'])-set(old['sequential'])
    comb.update(['a14', 'a21', 'b15', 'd19', 'f22', 'f23', 'd20', 'k11'])
    spec = dict(bn=old['board'], comb=sorted(comb), seq=old['sequential'],
                module='r_front_commands_conta', bindings={}, clock_taps=old['clock_taps'],
                flag_taps=old['flag_taps']+["WIM'", "WTPC'", 'ContA03.sil+2'])
    external = graph(root, out=None, inspect=True, **spec)
    removed = {"CBrOnCnt=0'", 'ContA03.sil+2', "WIM'", "WTPC'"}
    assert set(external) == (set(old['boundary_bindings'])-removed)|{'Call12', 'Call15'}
    spec['bindings'] = {n: old['boundary_bindings'][n] if n in old['boundary_bindings'] else
                         {'Call12': 'pins[315]', 'Call15': 'pins[316]'}[n] for n in external}
    assert len(set(spec['bindings'].values())) == len(spec['bindings'])
    board = graph(root, out=out, **spec)
    p = out/'r_front_commands_conta.sv'
    raw = p.read_text().replace('input wire [27:0] pins',
        'input wire [316:0] pins,input wire [3:0] next_raw,input wire priority_n,freeze,rb_forward,rb_forward_n')
    for n in ['a09', 'b09', 'c09', 'd09']:
        raw = raw.replace('original_F10000 '+n+'(', 'original_F10000_ready '+n+'(')
    p.write_text(raw)
    for n in ['proch', 'procl']:
        (out/f'r_front_ready_{n}.sv').write_bytes((parent/f'r_front_ready_{n}.sv').read_bytes())
    top = (parent/'r_front_local_reference.sv').read_text()
    top = top.replace('[314:0]', '[316:0]').replace('[62:0] fc', '[65:0] fc')
    assert top.count('r_front_next_conta c(') == 1
    top = top.replace('r_front_next_conta c(', 'r_front_commands_conta c(')
    assert top.count('cpu_pins=pins[245:0];') == 1
    top = top.replace('cpu_pins=pins[245:0];', 'cpu_pins=pins[245:0];cpu_pins[8]=fc[63];')
    (out/'r_front_local_reference.sv').write_text(top)
    manifest['boards'][0] = board
    manifest['extra_boundaries'].pop("CBrOnCnt=0'")
    manifest['extra_boundaries'].update(Call12=315, Call15=316)
    manifest.update(scope=__doc__, pin_count=317, count_branch_internal=True,
                    WIM_internal=True, RIM_RTPC_d17_input_internal=True,
                    obsolete_external_slots=[8, 309, 310, 311, 313],
                    packages=sum(len(b['packages']) for b in manifest['boards']),
                    original_pins=sum(b['original_pins'] for b in manifest['boards']),
                    full_native_producers_closed=False)
    (out/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    return manifest


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--candidate', type=Path)
    p.add_argument('--reference', type=Path)
    a = p.parse_args()
    assert a.candidate or a.reference
    if a.candidate:
        assert not a.candidate.exists()
        candidate(ROOT, a.candidate.resolve())
    if a.reference:
        generate(ROOT, a.reference.resolve())
