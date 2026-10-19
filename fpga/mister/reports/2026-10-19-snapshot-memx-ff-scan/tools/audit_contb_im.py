#!/usr/bin/env python3
"""Prove logical fanout equivalence before grouping the actual ContB IM RAM.

Only scalar combinational pin expressions are expanded. Sequential outputs,
unknown functions and board inputs remain distinct atoms. Equal canonical
expressions prove equivalence; unequal expressions do not prove inequality.
This does not qualify clock timing, snapshot ports, freeze or a board fit.
"""
import argparse
import ast
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT/'tools'))
from sil_netlist import load_board
from sil_to_verilog import vname

ADDRESS = (2, 3, 4, 5, 6, 7, 9, 10, 11, 12)
SIL = ROOT/'chm/sil/ContB-Rev-Cd.dm!1_/ContB-Rev-Cd.wl'
GENERATED = ROOT/'verilog/generated/ContB-Rev-Cd.v'


def negate(a):
    if isinstance(a, int):
        return 1-a
    return a[1] if a[0] == '~' else ('~', a)


def combine(op, *args):
    terms = []
    for a in args:
        terms.extend(a[1:] if isinstance(a, tuple) and a[0] == op else (a,))
    if op in ('|', '&'):
        identity = 0 if op == '|' else 1
        if 1-identity in terms:
            return 1-identity
        terms = sorted(set(a for a in terms if a != identity), key=repr)
        if not terms:
            return identity
        return terms[0] if len(terms) == 1 else (op, *terms)
    if op == '^':
        parity = sum(a for a in terms if isinstance(a, int)) & 1
        counts = Counter(a for a in terms if not isinstance(a, int))
        terms = sorted((a for a, count in counts.items() if count & 1), key=repr)
        result = 0 if not terms else terms[0] if len(terms) == 1 else ('^', *terms)
        return negate(result) if parity else result
    raise ValueError('unsupported canonical operator '+op)


def expression(text):
    text = re.sub(r"1'[bB]([01])", r'\1', text)
    node = ast.parse(text.strip(), mode='eval').body
    def valid(n):
        if isinstance(n, ast.Name):
            return True
        if isinstance(n, ast.Constant):
            return type(n.value) is int and n.value in (0, 1)
        if isinstance(n, ast.UnaryOp) and isinstance(n.op, ast.Invert):
            return valid(n.operand)
        return (isinstance(n, ast.BinOp) and isinstance(n.op, (ast.BitOr, ast.BitAnd, ast.BitXor))
                and valid(n.left) and valid(n.right))
    if not valid(node):
        raise ValueError('not a supported scalar expression: '+text)
    return node


def evaluate(node, get):
    if isinstance(node, ast.Name):
        return get(node.id)
    if isinstance(node, ast.Constant):
        return node.value
    if isinstance(node, ast.UnaryOp):
        return negate(evaluate(node.operand, get))
    op = '|' if isinstance(node.op, ast.BitOr) else '&' if isinstance(node.op, ast.BitAnd) else '^'
    return combine(op,
                   evaluate(node.left, get), evaluate(node.right, get))


def instances(text):
    return [(kind, name, dict(re.findall(r'\.(\w+)\(([^()]*)\)', body)))
            for kind, name, body in re.findall(
                r'\b(cell_\w+)\s+(\w+)\s*\((.*?)\)\s*;', text, re.S)]


def audit(text=None, cell_sources=None):
    text = GENERATED.read_text() if text is None else text
    cells = instances(text)
    ram = [(name, pins) for kind, name, pins in cells if kind == 'cell_F10415A']
    board = load_board(str(SIL))
    expected = {pkg for pkg, record in board.packages.items()
                if record['type'] == 'F10415A'}
    if len(ram) != 144 or {name[2:] for name, _ in ram} != expected:
        raise ValueError('all144 original IM packages, including parity, are required')
    sil_pins = {(p['pkg'], p['pin']): name for name, net in board.nets.items()
                for p in net['pins']}
    driver = {}
    sources = {}
    def add(name, fn):
        if name in driver:
            raise ValueError('unresolved multiple driver '+name)
        driver[name] = fn
    for target, rhs in re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', text):
        node = expression(rhs)
        add(target, lambda resolve, node=node: evaluate(node, resolve))
    for kind, name, pins in cells:
        path = ROOT/'verilog/cells'/f'{kind}.v'
        source = ((cell_sources or {}).get(kind, path.read_text()))
        sources[str(path.relative_to(ROOT))] = hashlib.sha256(source.encode()).hexdigest()
        code = re.sub(r'/\*.*?\*/|//[^\n]*', '', source, flags=re.S)
        inputs = set(re.findall(r'\binput\s+(?:wire\s+)?(p\d+)\b', code))
        local = {}
        for variable, rhs in re.findall(r'\bwire\s+(\w+)\s*=\s*([^;]+);', code):
            try:
                local[variable] = expression(rhs)
            except (ValueError, SyntaxError):
                pass
        def closed(node, seen=frozenset()):
            for n in ast.walk(node):
                if not isinstance(n, ast.Name) or n.id in inputs:
                    continue
                if n.id not in local or n.id in seen or not closed(local[n.id], seen | {n.id}):
                    return False
            return True
        for output, rhs in re.findall(r'\bassign\s+(p\d+)\s*=\s*([^;]+);', code):
            if output not in pins:
                continue
            try:
                node = expression(rhs)
            except (ValueError, SyntaxError):
                continue
            if not closed(node):
                continue  # state/internal logic is an opaque atom, never zero
            def fn(resolve, node=node, pins=pins, local=local):
                def get(p):
                    if p in local:
                        return evaluate(local[p], get)
                    return resolve(pins.get(p, "1'b0"))
                return evaluate(node, get)
            add(pins[output], fn)
    active = set()
    cache = {}
    def resolve(name):
        name = name.strip()
        if name in ("1'b0", "1'b1"):
            return int(name[-1])
        if name in cache:
            return cache[name]
        if name in active:
            raise ValueError('combinational cycle in IM pin cone '+name)
        active.add(name)
        result = driver[name](resolve) if name in driver else ('net', name)
        active.remove(name)
        cache[name] = result
        return result
    groups = defaultdict(list)
    literal = defaultdict(list)
    for name, pins in ram:
        if pins.get('sys_clk') != 'sys_clk':
            raise ValueError('IM guest clock changed: '+name)
        pkg = name[2:]
        field, ce, we = (sil_pins[(pkg, p)] for p in (1, 14, 13))
        if not re.fullmatch(r"CS[0-3]'[A-Za-z]+", ce):
            raise ValueError('unreviewed chip select '+ce)
        # Compare the generated pins with the original wire list. Net-name
        # escaping is the existing generator's, including tied-output suffixes.
        for pin in (*ADDRESS, 13, 14, 15):
            if pins.get(f'p{pin}') != vname(sil_pins[(pkg, pin)]):
                raise ValueError(f'original pin mismatch {pkg}.{pin}')
        if pins.get('p1') != vname(field)+f'__{pkg}_1':
            raise ValueError(f'original output mismatch {pkg}.1')
        bank = int(ce[2])
        raw = tuple(pins[f'p{p}'] for p in (*ADDRESS, 13, 14))
        key = tuple(resolve(n) for n in raw)
        literal[raw].append(pkg)
        groups[key].append(dict(package=pkg, field=field, bank=bank, we=we, ce=ce))
    result = []
    for key, members in groups.items():
        banks = {m['bank'] for m in members}
        if len(banks) != 1:
            raise ValueError('different banks grouped together')
        result.append(dict(width=len(members), bank=next(iter(banks)), members=members,
                           canonical_pins=key))
    result.sort(key=lambda r: (r['bank'], r['members'][0]['field']))
    left = {f'd{field}.{i}' for field, width in
            (('RSTK', 4), ('ALUF', 4), ('BSEL', 3), ('LC', 3), ('ASEL', 3))
            for i in range(width)} | {'dIMLH'}
    right = {f'd{field}.{i}' for field in ('FF', 'JCN') for i in range(8)} | {"dBlock'", 'dIMRH'}
    if len(result) != 8 or any(g['width'] != 18 for g in result):
        raise ValueError('actual IM fanout does not prove eight18-bit groups')
    seen = set()
    for group in result:
        fields = {m['field'] for m in group['members']}
        if fields not in (left, right):
            raise ValueError('IM half/parity coverage mismatch')
        group['half'] = 'left' if fields == left else 'right'
        key = (group['bank'], group['half'])
        if key in seen:
            raise ValueError('duplicate IM half/bank')
        seen.add(key)
    return dict(scope='Logical actual ContB IM grouping only; no hardware state access/freeze/timing qualification',
                ram_packages=len(ram), ram_bits=len(ram)*1024,
                physical_word_bits=36, data_bits=34, parity_bits=2,
                read_pipeline_bits=len(ram), literal_groups=len(literal),
                canonical_groups=len(result), groups=result, sources=sources,
                generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                wirelist_sha256=hashlib.sha256(SIL.read_bytes()).hexdigest(),
                locations_sha256=hashlib.sha256(SIL.with_suffix('.lc').read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = audit()
    output = json.dumps(result, indent=2)+'\n'
    if args.output:
        args.output.write_text(output)
    print(json.dumps({k: v for k, v in result.items() if k not in ('groups', 'sources')}, indent=2))
    print('Group widths:', [g['width'] for g in result['groups']])
