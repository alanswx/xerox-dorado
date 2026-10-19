#!/usr/bin/env python3
"""Independent Boolean truth oracle and damaged actual-board controls."""
import argparse
import json
from pathlib import Path
import random
import re

import audit_contb_im as im


def truth(node, assignment):
    if isinstance(node, int):
        return node
    if node[0] == 'net':
        return assignment[node[1]]
    if node[0] == '~':
        return truth(node[1], assignment) ^ 1
    values = [truth(n, assignment) for n in node[1:]]
    if node[0] == '|':
        return int(any(values))
    if node[0] == '&':
        return int(all(values))
    return sum(values) & 1


def check():
    rng = random.Random(0x36c0b)
    atoms = ('a', 'b', 'c', 'd', 'e', '0', '1')
    def raw(depth):
        if not depth or rng.randrange(4) == 0:
            return rng.choice(atoms)
        if rng.randrange(4) == 0:
            return '~('+raw(depth-1)+')'
        a = raw(depth-1)
        b = a if rng.randrange(3) == 0 else raw(depth-1)
        return '('+a+rng.choice(('|', '&', '^'))+b+')'
    # Python's native bitwise evaluator is independent of the AST canonicalizer.
    # Mask ~ back to one bit, as these scalar ECL gates do.
    expressions = ['a^a', '~(1^a)', 'a|a', 'a&a', '~(~a)', 'a^b^a^1']
    expressions += [raw(5) for _ in range(1500)]
    comparisons = 0
    fingerprints = {}
    for text in expressions:
        node = im.evaluate(im.expression(text), lambda name: ('net', name))
        code = compile(text, '<independent generated Boolean oracle>', 'eval')
        fingerprint = 0
        for mask in range(32):
            values = {name: (mask >> i) & 1 for i, name in enumerate('abcde')}
            want = eval(code, {'__builtins__': {}}, values) & 1
            assert truth(node, values) == want, (text, mask, node)
            fingerprint |= want << mask
            comparisons += 1
        assert node not in fingerprints or fingerprints[node] == fingerprint
        fingerprints[node] = fingerprint
    baseline = im.audit()
    assert baseline['ram_packages'] == 144 and baseline['canonical_groups'] == 8
    assert baseline['ram_bits'] == 147456 and baseline['read_pipeline_bits'] == 144
    assert {(g['bank'], g['half']) for g in baseline['groups']} == {
        (bank, half) for bank in range(4) for half in ('left', 'right')}
    text = im.GENERATED.read_text()
    cases = [
        ('missing_parity', re.sub(r'\bcell_F10415A\s+u_d06\s*\(.*?\)\s*;', '', text, count=1, flags=re.S), None, 'all144'),
        ('wrong_address', text.replace('.p2(RA_01a)', '.p2(RA_02a)', 1), None, 'original pin mismatch'),
        ('wrong_write_enable', text.replace('.p13(WEL_p_Aa)', '.p13(WER_p_Aa)', 1), None, 'original pin mismatch'),
        ('wrong_output', text.replace('.p1(dASEL_0__a06_1)', '.p1(dASEL_0__a07_1)', 1), None, 'original output mismatch'),
        ('wrong_clock', text.replace('.sys_clk(sys_clk)', '.sys_clk(other_clock)', 1), None, 'IM guest clock changed'),
        ('wrong_fanout_source', text.replace('.p5(dRA_11_p_)', '.p5(dRA_00_p_)', 1), None, 'eight18-bit'),
    ]
    path = im.ROOT/'verilog/cells/cell_MC10195.v'
    source = path.read_text()
    # Inverting ALL copies of an address bit preserves equivalence between
    # copies. This is deliberately outside a grouping proof's coverage; it
    # needs the separate original-cell/bank-address semantics qualification.
    common_inversion = im.audit(cell_sources={'cell_MC10195': source.replace('~(p9 ^ p5)', '(p9 ^ p5)')})
    assert common_inversion['canonical_groups'] == 8
    results = []
    for name, altered, sources, expected in cases:
        assert altered != text or sources
        try:
            im.audit(altered, sources)
        except ValueError as e:
            assert expected in str(e), (name, str(e))
            results.append(dict(case=name, rejected=True, diagnostic=str(e)))
        else:
            raise AssertionError('damaged board accepted: '+name)
    # An unknown or sequential value must not be treated as an unconnected zero.
    assert im.combine('|', ('net', 'q_a'), 0) != im.combine('|', ('net', 'q_b'), 0)
    return dict(status='PASS', boolean_expressions=len(expressions), truth_comparisons=comparisons,
                actual_board_groups=8, group_width=18, failure_controls=results,
                uncovered_control='Common inversion of all address fanouts preserves grouping and is not rejected; this is not a cell/bank-address correctness proof',
                scope='Logical fanout/coverage only; no RAM replacement, freeze, fit or physical snapshot')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = check()
    if args.output:
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(result, indent=2))
