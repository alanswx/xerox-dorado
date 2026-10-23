#!/usr/bin/env python3
"""Express ProcL j17/k17/l17's exact wired arithmetic as one six-bit adder.

All three MC10182s have M/S0/S1 and the initial carry LOW. Preserve every
connected output net, including the two internal carries. No register, clock,
RAM port or capture epoch changes. The original reference cells stay intact.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
PARENT_SHA = '60d63614c927570d9fbed383e837955510fe10242a3ef74574ff485e0e6647c5'


def rewrite(path):
    original = path.read_text()
    assert hashlib.sha256(original.encode()).hexdigest() == PARENT_SHA
    start = original.index('module r_front_procl_sampled(')
    end = original.index('\nendmodule', start)
    body = original[start:end]
    rows = {}
    for pkg in ['j17', 'k17', 'l17']:
        matches = re.findall(r'^ comb_MC10182 '+pkg+r'\((.*)\);$', body, re.M)
        assert len(matches) == 1, pkg
        pins = {int(n): value for n, value in re.findall(r'\.p(\d+)\(([^()]+)\)', matches[0])}
        assert all(pins[n] == "1'b0" for n in [7, 9, 10]), (pkg, pins)
        connected_outputs = {n for n in [2, 3, 4, 14, 15] if n in pins}
        assert connected_outputs == ({2, 4, 14} if pkg != 'l17' else {4, 14}), (pkg, pins)
        rows[pkg] = pins
    assert rows['j17'][13] == "1'b0"
    assert rows['k17'][13] == rows['j17'][2].removesuffix('_j17_2')
    assert rows['l17'][13] == rows['k17'][2].removesuffix('_k17_2')
    for pkg, sink in [('j17', 'k17'), ('k17', 'l17')]:
        assert 'assign '+rows[sink][13]+'='+rows[pkg][2]+';' in body
    a = [rows[p][pin] for p in ['l17', 'k17', 'j17'] for pin in [12, 5]]
    b = [rows[p][pin] for p in ['l17', 'k17', 'j17'] for pin in [11, 6]]
    outputs = [rows[p][pin] for p in ['l17', 'k17', 'j17'] for pin in [14, 4]]
    assert 'stack_add_a' not in original
    replacement = ''' // Exact ProcL six-bit StkP+RSTK arithmetic; original carry nets retained.
 wire [5:0] stack_add_a={'''+','.join(a)+'''};
 wire [5:0] stack_add_b={'''+','.join(b)+'''};
 wire [6:0] stack_add_sum={1'b0,stack_add_a}+{1'b0,stack_add_b};
 wire [2:0] stack_add_low2={1'b0,stack_add_a[1:0]}+{1'b0,stack_add_b[1:0]};
 wire [4:0] stack_add_low4={1'b0,stack_add_a[3:0]}+{1'b0,stack_add_b[3:0]};
 assign {'''+','.join(outputs)+'''}=stack_add_sum[5:0];
 assign '''+rows['j17'][2]+'''=stack_add_low2[2];
 assign '''+rows['k17'][2]+'''=stack_add_low4[4];
'''
    old_lines = []
    rewritten = body
    for pkg in ['j17', 'k17', 'l17']:
        line = re.search(r'^ comb_MC10182 '+pkg+r'\(.*\);\n', body, re.M)[0]
        old_lines.append(line)
        assert rewritten.count(line) == 1
        rewritten = rewritten.replace(line, replacement if pkg == 'j17' else '')
    raw = original[:start]+rewritten+original[end:]
    # Removing the three original lines and the new arithmetic must leave
    # identical source, including every existing net alias and observation.
    stripped = rewritten.replace(replacement, '')
    expected = body
    for line in old_lines:
        expected = expected.replace(line, '')
    assert stripped == expected, 'Unaccounted processor-cone change'
    assert raw[:start] == original[:start] and raw[start+len(rewritten):] == original[end:]
    path.write_text(raw)
    return dict(scope=__doc__, parent_sha256=PARENT_SHA,
                output_sha256=hashlib.sha256(raw.encode()).hexdigest(),
                original_instances=old_lines, inserted=replacement,
                operand_a=a, operand_b=b, sum_outputs=outputs,
                connected_outputs_preserved=True, non_arithmetic_source_unchanged=True,
                architectural_state_added=False, native_integrated=False)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('cone', type=Path)
    p.add_argument('--record', type=Path, required=True)
    a = p.parse_args()
    r = rewrite(a.cone.resolve())
    assert not a.record.exists()
    a.record.write_text(json.dumps(r, indent=2)+'\n')
