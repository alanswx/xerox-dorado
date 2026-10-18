#!/usr/bin/env python3
"""Inventory snapshot candidates from unflattened Verilator XML, without ABI offsets.

This is a review inventory, not a save/load implementation or a completeness
proof. Include the actual wrapper/device RTL in the XML scope. External RAM,
ARM services, primitive-internal state and combinational latches require review.
"""
import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET


def constant(text, signed_type=False):
    match = re.fullmatch(r"(\d+)'(s?)([hbd])([0-9a-f]+)", text)
    if not match:
        return int(text)
    width, signed, base, digits = match.groups()
    value = int(digits, {'h': 16, 'b': 2, 'd': 10}[base])
    if (signed or signed_type) and value & (1 << (int(width)-1)):
        value -= 1 << int(width)
    return value


def destinations(node):
    # Index/select operands are READS. Only the selected base is written.
    if node.tag == 'varref':
        return {node.get('name')}
    if node.tag in ('arraysel', 'sel', 'wordsel'):
        return destinations(node[0])
    if node.tag == 'concat':
        return set().union(*(destinations(child) for child in node))
    raise ValueError('Unreviewed assignment destination: '+ET.tostring(node, encoding='unicode'))


def inventory(xml):
    root = ET.fromstring(xml)
    modules = {node.get('name'): node for node in root.findall('.//module')}
    types = {node.get('id'): node for node in root.findall('.//typetable/*')}
    files = {node.get('id'): node.get('filename') for node in root.findall('./files/file')}

    def shape(id_):
        node = types[id_]
        if node.tag == 'basicdtype':
            if node.get('name') in ('string', 'real', 'double', 'chandle'):
                raise ValueError('Non-bit state needs explicit serialization: '+ET.tostring(node, encoding='unicode'))
            return abs(int(node.get('left', 0))-int(node.get('right', 0)))+1, []
        if node.tag in ('unpackarraydtype', 'packarraydtype'):
            width, dimensions = shape(node.get('sub_dtype_id'))
            bounds = [constant(c.get('name'), types[c.get('dtype_id')].get('signed') == 'true')
                      for c in node.findall('./range/const')]
            if len(bounds) != 2:
                raise ValueError('Nonconstant array bound')
            size = abs(bounds[0]-bounds[1])+1
            return (width, [bounds]+dimensions) if node.tag == 'unpackarraydtype' else (width*size, dimensions)
        if node.get('sub_dtype_id'):
            return shape(node.get('sub_dtype_id'))
        if node.tag == 'structdtype':
            members = [shape(c.get('sub_dtype_id')) for c in node if c.tag == 'memberdtype']
            if not members or any(d for _, d in members):
                raise ValueError('Unreviewed structure state')
            return sum(w for w, _ in members), []
        raise ValueError('Unreviewed state type: '+ET.tostring(node, encoding='unicode'))

    descriptions = {}
    for name, module in modules.items():
        declarations = {v.get('name'): v for v in module.findall('./var')}
        writers = defaultdict(set)
        for process in module:
            if process.tag not in ('always', 'initial', 'initialstatic', 'func', 'task'):
                continue
            local = {}
            for declaration in process.findall('.//var'):
                variable = declaration.get('name')
                if variable in local or variable in declarations:
                    raise ValueError(f'Unreviewed shadowed process variable {name}.{variable}')
                key = '@'+process.get('loc', '')+'.'+variable
                local[variable] = key
                declarations[key] = declaration
            edges = sorted({s.get('edgeType') for s in process.findall('./sentree/senitem')})
            edge_sensitive = any(edge not in ('COMBO', 'TRUE') for edge in edges)
            for assign in process.iter():
                if assign.tag == 'readmem':
                    # $readmemh/$readmemb populate ROM/RAM without an assignment.
                    target = assign.find('./varref')
                    if target is None or target.get('name') not in declarations:
                        raise ValueError('Unreviewed readmem destination')
                    writers[target.get('name')].add('initial_only_candidate' if
                        process.tag in ('initial', 'initialstatic') else 'subroutine_review')
                    continue
                if assign.tag not in ('assign', 'assigndly', 'contassign'):
                    continue
                if len(assign) != 2:
                    raise ValueError('Unreviewed assignment form')
                kind = ('subroutine_review' if process.tag in ('func', 'task') else
                        'initial_only_candidate' if process.tag != 'always' else
                        'sequential' if edge_sensitive or assign.tag == 'assigndly' else
                        'combinational_review')
                for target in destinations(assign[1]):
                    target = local.get(target, target)
                    if target not in declarations:
                        raise ValueError(f'Unresolved module destination {name}.{target}')
                    writers[target].add(kind)
        descriptions[name] = []
        for variable, kinds in sorted(writers.items()):
            declaration = declarations[variable]
            if declaration.get('param') == 'true':
                continue
            width, dimensions = shape(declaration.get('dtype_id'))
            count = 1
            for high, low in dimensions:
                count *= abs(high-low)+1
            category = ('sequential' if 'sequential' in kinds else
                        'subroutine_review' if 'subroutine_review' in kinds else
                        'combinational_review' if 'combinational_review' in kinds else
                        'initial_only_candidate')
            loc = declaration.get('loc', '').split(',')
            descriptions[name].append(dict(name=variable, category=category,
                width=width, unpacked_dimensions=dimensions, bits=width*count,
                source=files.get(loc[0], loc[0]), line=int(loc[1]) if len(loc)>1 else None,
                review='UNCLASSIFIED: save, drain, immutable or reconstructible decision required'))
    records = []
    seen = set()
    for cell in root.findall('.//cells//cell'):
        path, name = cell.get('hier'), cell.get('submodname')
        if not path or path in seen or name not in descriptions:
            raise ValueError('Missing/duplicate cell path or module: '+str(cell.attrib))
        seen.add(path)
        for entry in descriptions[name]:
            records.append(dict(path=path+'.'+entry['name'], module=name, **entry))
    if not records:
        raise ValueError('No instance state candidates found')
    records.sort(key=lambda row: row['path'])
    counts = Counter(row['category'] for row in records)
    bits = Counter()
    for row in records:
        bits[row['category']] += row['bits']
    return dict(schema='dorado-state-review-v1', xml_sha256=hashlib.sha256(xml).hexdigest(),
        status='REVIEW ONLY; no state access or complete snapshot claim',
        scope='Exactly the elaborated XML hierarchy; external state is not included',
        instances=len(seen), candidate_counts=dict(counts), candidate_bits=dict(bits),
        candidates=records)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('xml', type=Path)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    result = inventory(args.xml.read_bytes())
    args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k: v for k, v in result.items() if k != 'candidates'}, indent=2))


if __name__ == '__main__':
    main()
