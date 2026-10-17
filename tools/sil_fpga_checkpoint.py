#!/usr/bin/env python3
"""Make per-instance, synthesizable power-up initializers from actual RTL state.

Inputs belong to one exact Verilated build: its POD descriptor, offset map,
headers, unflattened XML, and DORRTL1 checkpoint. No C-emulator state is used.
Output is generated in a separate build directory; original RTL is untouched.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import xml.etree.ElementTree as ET


def unmangle(s):
    return re.sub(r'__0([0-9a-fA-F]{2})', lambda m: chr(int(m[1], 16)),
                  s.replace('__PVT__', '').replace('__DOT__', '.'))


def number(s):
    m = re.fullmatch(r"(\d+)'s?([hbd])([0-9a-f]+)", s)
    if not m:
        raise ValueError(s)
    return int(m[3], {'h': 16, 'b': 2, 'd': 10}[m[2]])


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--checkpoint', type=Path, required=True)
    p.add_argument('--obj-dir', type=Path, required=True)
    p.add_argument('--layout', type=Path, help='optional cached native offset map')
    p.add_argument('--xml', type=Path, help='optional cached Verilator XML for the current RTL')
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--storage-words', type=int, choices=[65536, 131072, 262144], default=262144,
                   help='physical storage retained for this continuation; smaller configurations reject other addresses')
    p.add_argument('--hps-pack', action='store_true', help='connect a real HPS-backed serial Trident medium')
    p.add_argument('--ethernet-wire', action='store_true', help='expose the real Ethernet cable pins for packet-link validation')
    p.add_argument('--ethernet-host', type=lambda s: int(s, 0), default=0xff,
                   help='positive-logic station address straps (default 0xff / 377B; old exports used 0)')
    p.add_argument('--base-pack', type=Path, help='make a disposable disk with the checkpoint media overlay applied')
    a = p.parse_args()
    if not 0 <= a.ethernet_host <= 255:
        p.error('--ethernet-host must be an eight-bit station address')
    a.output.mkdir(parents=True, exist_ok=True)
    header = (a.obj_dir / 'rtl_state.h').read_text()
    fingerprint = re.search(r'rtl_state_fingerprint\[\]="([0-9a-f]+)"', header)[1]
    snapshot = a.checkpoint.read_bytes()
    if snapshot[:8] != b'DORRTL1\0' or snapshot[8:72].decode() != fingerprint:
        p.error('checkpoint is not from this exact Verilated model')
    raw = bytearray(int(re.search(r'rtl_state_size=(\d+)', header)[1]))
    pos = 72
    for off, size in re.findall(r'\{(\d+),(\d+)\}', header):
        off, size = int(off), int(size)
        raw[off:off+size] = snapshot[pos:pos+size]
        pos += size
    if a.base_pack:
        media = bytearray(a.base_pack.read_bytes())
        count, default = struct.unpack_from('<QH', snapshot, pos)
        pos += 10
        if count > len(media)//2 or len(snapshot) != pos + count*10:
            p.error('invalid checkpoint media overlay')
        for _ in range(count):
            word, value = struct.unpack_from('<QH', snapshot, pos)
            pos += 10
            if word*2+2 > len(media):
                p.error('checkpoint media address outside pack')
            struct.pack_into('<H', media, word*2, value)
        media.extend(b'\0' * (-len(media) % 512))
        (a.output / 'executive.pack').write_bytes(media)
    symheader = (a.obj_dir / 'Vtb_exec__Syms.h').read_text()
    objects = {obj: typ for typ, obj in re.findall(r'\b(Vtb_exec_\w+)\s+(TOP(?:__\w+)?);', symheader)}
    if a.layout:
        layout = json.loads(a.layout.read_text())
    else:
        # Obtain native ABI offsets from the matching generated headers, not
        # from a guessed C++ layout or a prior machine's cached offsets.
        lines = ['#include "Vtb_exec__Syms.h"', '#include <cstddef>', '#include <cstdio>', 'int main(){']
        for obj, typ in objects.items():
            lines.append(f'printf("O {obj} {typ} %zu %zu\\n",offsetof(Vtb_exec__Syms,{obj}),sizeof({typ}));')
        for typ in sorted(set(objects.values())):
            cpp = (a.obj_dir / (typ+'.h')).read_text()
            for line in cpp.splitlines():
                match = re.match(r'\s*(?:(?:[CSIQ]Data)(?:/\*.*?\*/)?|VlUnpacked<.*>|VlWide<.*>)\s+(\w+)\s*;', line)
                if match:
                    f = match[1]
                    lines.append(f'printf("F {typ} {f} %zu %zu\\n",offsetof({typ},{f}),sizeof((({typ}*)nullptr)->{f}));')
        lines.append('}')
        source, binary = a.output/'checkpoint_layout.cpp', a.output/'checkpoint_layout'
        source.write_text('\n'.join(lines)+'\n')
        version = subprocess.check_output(['verilator', '-V'], text=True)
        inc = Path(re.search(r'VERILATOR_ROOT\s*=\s*(\S+)', version)[1])/'include'
        subprocess.run(['c++', '-std=c++20', '-w', '-I'+str(a.obj_dir.resolve()), '-I'+str(inc), '-I'+str(inc/'vltstd'), str(source), '-o', str(binary)], check=True)
        layout = {'syms': {}, 'classes': {}}
        for line in subprocess.check_output([str(binary.resolve())], text=True).splitlines():
            kind, typ, field, off, size = line.split()
            if kind == 'O':
                layout['syms'][typ] = [int(off), int(size)]
            else:
                layout['classes'].setdefault(typ, {})[field] = [int(off), int(size)]
        (a.output/'verilated-layout.json').write_text(json.dumps(layout))
    captured = {}
    for obj, (base, _) in layout['syms'].items():
        kind = 'root' if obj == 'TOP' else 'msa' if obj.endswith(('__b_msa', '__b_msaOdd')) else 'ram16'
        cpp = (a.obj_dir / (objects[obj] + '.h')).read_text()
        declarations = {m[4]: m for m in re.finditer(r'(CData|SData|IData|QData)/\*(\d+):0\*/(?:,\s*(\d+)>)?\s+(\w+);', cpp)}
        fields = layout['classes'][objects[obj]] if 'classes' in layout else layout[kind]
        for field, (off, size) in fields.items():
            match = declarations.get(field)
            if not match:
                continue
            width = int(match[2]) + 1
            stride = {'CData': 1, 'SData': 2, 'IData': 4, 'QData': 8}[match[1]]
            count = int(match[3]) if match[3] else 1
            name = unmangle(('' if obj == 'TOP' else obj.removeprefix('TOP__') + '__DOT__') + field)
            if '__V' in name:
                continue
            values = [int.from_bytes(raw[base+off+i*stride:base+off+(i+1)*stride], 'little') & ((1 << width)-1) for i in range(count)]
            captured[name] = (width, values)

    if not a.xml:
        a.xml = a.output/'checkpoint.xml'
        boards = 'ContA-Rev-Cd ContB-Rev-Cd ProcH-Rev-Ce ProcL-Rev-Ci MemC-Rev-Be MemD-Rev-Ca MemX-Rev-Ch msa IFU-Rev-Ch DispY-Rev-Cl DskEth-Rev-Cf'.split()
        command = ['verilator', '--xml-only', '--timing', '--top-module', 'tb_exec', '-Wno-fatal', '-Wno-UNOPTFLAT', '-Wno-MULTIDRIVEN',
                   '+define+SYSPER_OVERRIDE=4', '+define+DORADO_STORAGE_INPUT_DELAY=2', '+define+WORLD', '+define+SCREEN', '+define+FULL',
                   '-Iverilog/cells', '-Iverilog/generated', '--xml-output', str(a.xml), 'verilog/verilator/tb_exec.sv', 'verilog/generated/dorado_full.v']
        command += ['verilog/generated/'+b+'.v' for b in boards] + ['verilog/verilator/dorado_pack.sv', 'verilog/cells/dorado_trident.v']
        with (a.output/'xml.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=log, check=True)
    xml = ET.parse(a.xml).getroot()
    for cell in xml.findall('.//cells//cell'):
        cell.set('hier', unmangle(cell.get('hier')))
    mods = {m.get('name'): m for m in xml.findall('.//module')}
    files = {f.get('id'): Path(f.get('filename')) for f in xml.findall('./files/file')}
    types = {t.get('id'): t for t in xml.findall('./netlist/typetable/*')}

    def dtype(id_):
        t = types[id_]
        if t.tag == 'basicdtype':
            return abs(int(t.get('left', 0))-int(t.get('right', 0)))+1, 1
        if t.tag == 'unpackarraydtype':
            w, n = dtype(t.get('sub_dtype_id'))
            ends = [number(c.get('name')) for c in t.findall('./range/const')]
            return w, n * (abs(ends[0]-ends[1])+1)
        if t.get('sub_dtype_id'):
            return dtype(t.get('sub_dtype_id'))
        raise ValueError(ET.tostring(t))

    # Bit aliases recover registers that Verilator merged with a port/wire or
    # another equal register. Unused setup captures need no initializer.
    parent, parity, known = {}, {}, {}

    def find(x):
        if x not in parent:
            parent[x], parity[x] = x, 0
        if parent[x] == x:
            return x, 0
        root, inv = find(parent[x])
        parity[x] ^= inv
        parent[x] = root
        return root, parity[x]

    def join(x, y, invert=0):
        rx, ix = find(x)
        ry, iy = find(y)
        flip = ix ^ iy ^ invert
        if rx == ry:
            if flip:
                raise ValueError(f'inconsistent alias: {x}, {y}')
            return
        parent[ry], parity[ry] = rx, flip
        if ry in known:
            val = known.pop(ry) ^ flip
            if rx in known and known[rx] != val:
                raise ValueError(f'inconsistent captured bits: {x}, {y}')
            known[rx] = val

    def put(x, val):
        root, inv = find(x)
        val ^= inv
        if root in known and known[root] != val:
            raise ValueError(f'conflicting value: {x}')
        known[root] = val

    def get(x):
        root, inv = find(x)
        return known[root] ^ inv if root in known else None

    zero = ('$constant', 0)
    put(zero, 0)

    def bits(e, scope):
        if e.tag in ('var', 'varref', 'varxref'):
            name = scope + '.' + unmangle(e.get('name'))
            if e.tag == 'varxref':
                name = 'tb_exec.' + unmangle(e.get('dotted')) + '.' + unmangle(e.get('name'))
            w, count = dtype(e.get('dtype_id'))
            return [(name, i, 0) for i in range(w)] if count == 1 else None
        if e.tag == 'const':
            try:
                val = number(e.get('name'))
            except ValueError:
                return None
            w, _ = dtype(e.get('dtype_id'))
            return [(zero[0], zero[1], (val >> i) & 1) for i in range(w)]
        if e.tag in ('not', 'extend', 'extends'):
            b = bits(e[0], scope)
            if b is None:
                return None
            if e.tag == 'not':
                return [(n, i, inv ^ 1) for n, i, inv in b]
            w, _ = dtype(e.get('dtype_id'))
            return (b + ([b[-1]] if e.tag == 'extends' else [(zero[0], zero[1], 0)]) * w)[:w]
        if e.tag == 'concat':
            aa, bb = bits(e[0], scope), bits(e[1], scope)
            return bb + aa if aa is not None and bb is not None else None
        if e.tag == 'sel' and e[1].tag == 'const':
            b = bits(e[0], scope)
            lo = number(e[1].get('name'))
            w = int(e.get('widthConst'))
            return b[lo:lo+w] if b is not None else None
        if e.tag in ('or', 'and', 'xor'):
            aa, bb = bits(e[0], scope), bits(e[1], scope)
            if aa is None or bb is None or len(aa) != len(bb):
                return None
            out = []
            for x, y in zip(aa, bb):
                vx, vy = get(x[:2]), get(y[:2])
                vx = None if vx is None else vx ^ x[2]
                vy = None if vy is None else vy ^ y[2]
                # Only structurally CONSTANT operands may simplify an alias;
                # a captured signal's present value is not a constant circuit.
                cx, cy = find(x[:2])[0] == find(zero)[0], find(y[:2])[0] == find(zero)[0]
                if cx and cy:
                    val = {'or': vx | vy, 'and': vx & vy, 'xor': vx ^ vy}[e.tag]
                    out.append((zero[0], zero[1], val))
                elif cx and ((e.tag == 'or' and vx == 0) or (e.tag == 'and' and vx == 1)):
                    out.append(y)
                elif cy and ((e.tag == 'or' and vy == 0) or (e.tag == 'and' and vy == 1)):
                    out.append(x)
                elif cx and e.tag == 'xor':
                    out.append((y[0], y[1], y[2] ^ vx))
                elif cy and e.tag == 'xor':
                    out.append((x[0], x[1], x[2] ^ vy))
                elif (cx and ((e.tag == 'or' and vx == 1) or (e.tag == 'and' and vx == 0))) or (cy and ((e.tag == 'or' and vy == 1) or (e.tag == 'and' and vy == 0))):
                    out.append((zero[0], zero[1], 1 if e.tag == 'or' else 0))
                else:
                    return None
            return out
        return None

    def connect(aa, bb):
        if aa is None or bb is None or len(aa) != len(bb):
            return
        for x, y in zip(aa, bb):
            join(x[:2], y[:2], x[2] ^ y[2])

    def scopes(node, path):
        yield node, path
        for g in node.findall('genblock'):
            yield from scopes(g, path + '.' + g.get('name'))

    cells = xml.findall('.//cells//cell')
    selected = [c for c in cells if c.get('hier').startswith('tb_exec.m') or c.get('hier') in ('tb_exec.u_pixclk', 'tb_exec.u_keyboard', 'tb_exec.u_term', 'tb_exec.u_drv')]
    aliases, simple_registers, definitions = [], [], {}
    for cell in cells:
        path, module = cell.get('hier'), mods[cell.get('submodname')]
        for node, scope in scopes(module, path):
            for block in node.findall('always'):
                for assign in block.findall('contassign'):
                    aliases.append((assign[0], scope, assign[1], scope))
                    if assign[1].tag == 'varref':
                        definitions[scope + '.' + unmangle(assign[1].get('name'))] = (assign[0], scope)
                sens = block.find('sentree')
                if sens is not None:
                    assigns = block.findall('assigndly')
                    body = block.find('begin')
                    if body is not None:
                        assigns += body.findall('assigndly')
                    for assign in assigns:
                        if assign[-1].tag == 'varref':
                            simple_registers.append((assign, scope))
            for instance in node.findall('instance'):
                child = scope + '.' + unmangle(instance.get('name'))
                submod = mods[instance.get('defName')]
                subvars = {v.get('name'): v for v in submod.findall('var')}
                for port in instance.findall('port'):
                    if len(port) != 1:
                        continue
                    v = subvars.get(port.get('name'))
                    if v is not None:
                        aliases.append((port[0], scope, v, child))

    for _ in range(3):
        for lhs, lp, rhs, rp in aliases:
            connect(bits(lhs, lp), bits(rhs, rp))
        equivalent = {}
        for assign, scope in simple_registers:
            rhs, lhs = bits(assign[0], scope), bits(assign[1], scope)
            if rhs is None or lhs is None:
                continue
            key = tuple((find(x[:2])[0], find(x[:2])[1] ^ x[2]) for x in rhs)
            if key in equivalent:
                connect(lhs, equivalent[key])
            else:
                equivalent[key] = lhs

    state_names = set()
    for cell in cells:
        for node, scope in scopes(mods[cell.get('submodname')], cell.get('hier')):
            for block in node.findall('always'):
                if block.find('sentree') is not None:
                    for assign in block.iter('assigndly'):
                        lhs = assign[-1]
                        while lhs.tag in ('arraysel', 'sel'):
                            lhs = lhs[0]
                        if lhs.tag == 'varref':
                            state_names.add(scope + '.' + unmangle(lhs.get('name')))
    # Some retained C++ wires are evaluation temporaries and are one evaluation
    # behind at a checkpoint. Clocked state takes precedence over those wires.
    stale_wires = []
    for path in sorted(captured, key=lambda n: n not in state_names):
        width, values = captured[path]
        if len(values) == 1:
            for i in range(width):
                bit = (values[0] >> i) & 1
                previous = get((path, i))
                if previous is not None and previous != bit:
                    if path in state_names:
                        raise ValueError(f'clocked state disagrees with alias: {path}[{i}]')
                    stale_wires.append((path, i))
                    continue
                put((path, i), bit)

    def evaluate(e, scope, visiting=None):
        visiting = set() if visiting is None else visiting
        bb = bits(e, scope)
        if bb is not None:
            vals = [get(b[:2]) for b in bb]
            if all(v is not None for v in vals):
                return sum((v ^ b[2]) << i for i, (v, b) in enumerate(zip(vals, bb)))
        if e.tag in ('varref', 'var'):
            key = scope + '.' + unmangle(e.get('name'))
            if key in definitions and key not in visiting:
                rhs, where = definitions[key]
                return evaluate(rhs, where, visiting | {key})
            return None
        vals = [evaluate(c, scope, visiting) for c in e]
        if any(v is None for v in vals):
            return None
        w, _ = dtype(e.get('dtype_id'))
        if e.tag == 'or': result = vals[0] | vals[1]
        elif e.tag == 'and': result = vals[0] & vals[1]
        elif e.tag == 'xor': result = vals[0] ^ vals[1]
        elif e.tag == 'not': result = ~vals[0]
        elif e.tag == 'concat': result = (vals[0] << dtype(e[1].get('dtype_id'))[0]) | vals[1]
        elif e.tag == 'sel': result = vals[0] >> vals[1]
        elif e.tag in ('extend', 'extends'): result = vals[0]
        else: return None
        return result & ((1 << w)-1)

    reconstructed = {}
    for assign, scope in simple_registers:
        name = scope + '.' + unmangle(assign[1].get('name'))
        if name in captured:
            continue
        value = evaluate(assign[0], scope)
        if value is not None:
            reconstructed[name] = value

    generated, report, helper_state = {}, [], {}
    msa_source = files[next(m for m in mods.values() if m.get('origName') == 'msa').get('loc').split(',')[0]].read_text()
    for cell in reversed(selected):
        path, module = cell.get('hier'), mods[cell.get('submodname')]
        source_file = files[module.get('loc').split(',')[0]]
        source = source_file.read_text()
        source_lines = source.splitlines(keepends=True)
        edits, initialized, omitted = [], {}, []
        for node, scope in scopes(module, path):
            variables = {v.get('name'): v for v in node if v.tag in ('var', 'varref')}
            states = set()
            for block in node.findall('always'):
                if block.find('sentree') is None:
                    continue
                for assign in block.iter('assigndly'):
                    lhs = assign[-1]
                    while lhs.tag in ('arraysel', 'sel'):
                        lhs = lhs[0]
                    if lhs.tag == 'varref':
                        states.add(lhs.get('name'))
            for name in sorted(states):
                v = variables.get(name)
                if v is None or v.get('vartype') == 'integer' and name in ('j', 'i'):
                    continue
                full = scope + '.' + unmangle(name)
                width, count = dtype(v.get('dtype_id'))
                if full in captured:
                    cw, values = captured[full]
                    if cw != width or len(values) != count:
                        raise ValueError(f'state dimensions differ: {full}: {(cw,len(values))}, {(width,count)}')
                elif count == 1:
                    bv = [get((full, i)) for i in range(width)]
                    if any(b is None for b in bv):
                        if full not in reconstructed:
                            omitted.append(full)
                            continue
                        values = [reconstructed[full]]
                    else:
                        values = [sum(b << i for i, b in enumerate(bv))]
                else:
                    omitted.append(full)
                    continue
                _, lineno, col, _, _ = v.get('loc').split(',')
                start = sum(map(len, source_lines[:int(lineno)-1])) + int(col)-1
                end = source.index(';', start)
                local = unmangle(name)
                if count > 1:
                    if module.get('origName') == 'cell_MK4096P_6' and a.storage_words < 262144:
                        # Alto occupies the first physical 64K words. The MSA
                        # scatters word-address bits 17 and 16 onto DRAM
                        # address bits 7 and 0 respectively, active low.
                        # Some groups complement ALL address pins.
                        chip = path.rsplit('.', 1)[1]
                        im = re.search(r'\b'+re.escape(chip)+r'\s*\((.*?)\n  \);', msa_source, re.S)
                        pin = re.search(r'\.p10\(([^)]+)\)', im[1])[1]
                        inverted = '_p_' in pin
                        mask = 0x81 if a.storage_words == 65536 else 0x80
                        wanted = 0 if inverted else mask
                        values = [v for i, v in enumerate(values) if i & mask == wanted]
                        count = len(values)
                    mem = hashlib.sha256(full.encode()).hexdigest()[:20] + '.mem'
                    (a.output / mem).write_text(''.join(f'{x:0{(width+3)//4}x}\n' for x in values))
                    reader = '$readmemb' if width == 1 else '$readmemh'
                    init = f'\n  initial {reader}("{a.output / mem}", {local});\n'
                    edits.append((end+1, end+1, init))
                else:
                    literal = f"{width}'h{values[0]:x}"
                    tail = source[start+len(local):end]
                    match = re.match(r'\s*=\s*[^,;]+', tail)
                    if match:
                        edits.append((start+len(local), start+len(local)+match.end(), ' = ' + literal))
                    else:
                        edits.append((end+1, end+1, f'\n  initial {local} = {literal};\n'))
                initialized[full] = values if count > 1 else values[0]
        if module.get('origName') == 'cell_MK4096P_6' and a.storage_words < 262144:
            chip = path.rsplit('.', 1)[1]
            im = re.search(r'\b'+re.escape(chip)+r'\s*\((.*?)\n  \);', msa_source, re.S)
            inverted = '_p_' in re.search(r'\.p10\(([^)]+)\)', im[1])[1]
            mask = 0x81 if a.storage_words == 65536 else 0x80
            wanted = 0 if inverted else mask
            address = '{addr_i[13:8], addr_i[6:1]}' if a.storage_words == 65536 else '{addr_i[13:8], addr_i[6:0]}'
            # Apply length-changing storage edits AFTER location-based edits.
            # They are recorded here and performed below.
            storage_edit = (mask, wanted, address)
        else:
            storage_edit = None
        for child in cell.findall('cell'):
            childpath = child.get('hier')
            if childpath not in generated:
                continue
            _, lineno, _, _, _ = child.get('loc').split(',')
            index = int(lineno)-1
            line = source_lines[index]
            original = mods[child.get('submodname')].get('origName')
            match = re.search(r'\b' + re.escape(original) + r'\b', line)
            if not match:
                raise ValueError(f'cannot specialize instance {childpath}: {line}')
            offset = sum(map(len, source_lines[:index]))
            edits.append((offset+match.start(), offset+match.end(), generated[childpath]))
        newname = 'checkpoint_' + re.sub(r'[^a-zA-Z0-9_]', '_', path.removeprefix('tb_exec.'))
        match = re.search(r'\bmodule\s+(' + re.escape(module.get('origName')) + r')\b', source)
        edits.append((match.start(1), match.end(1), newname))
        for start, end, replacement in sorted(edits, reverse=True):
            source = source[:start] + replacement + source[end:]
        if storage_edit:
            mask, wanted, address = storage_edit
            source = source.replace('mem [0:16383]', f'mem [0:{a.storage_words//16-1}]')
            source = source.replace('wire [13:0] addr = addr_i;',
                f'wire [{11 if a.storage_words==65536 else 12}:0] addr = {address};\n'
                f"  wire address_present = (addr_i & 14'h{mask:x}) == 14'h{wanted:x};")
            source = source.replace('if (write_pending) mem[addr]', 'if (write_pending && address_present) mem[addr]')
            source = source.replace('dout <= dout_r;', "dout <= address_present ? dout_r : 1'b0;")
            source = source.replace('endmodule', '''// synthesis translate_off
  always @(posedge sys_clk)
    if ((write_pending || (!p4 && !p15 && !cas_d && p3)) && !address_present)
      $fatal(1, "storage access outside checkpoint configuration: %m addr=%h", addr_i);
  // synthesis translate_on
endmodule''')
        if initialized.get(path + '.dout_r') == 1 and module.get('origName') in (
                'cell_MK4096P_6', 'cell_MosRam', 'cell_F10470', 'cell_F10415A'):
            # Cyclone V RAM output registers power up at zero. Initializing
            # that register to one makes Quartus 17 turn the ENTIRE memory
            # into flip-flops. Preserve the saved first-cycle value with an
            # external valid mux, then use the normal synchronous RAM output.
            source = re.sub(r'\breg\s+dout_r\s*;', '''reg checkpoint_ram_q = 1'b0;
  reg checkpoint_ram_valid = 1'b0;
  wire dout_r = checkpoint_ram_valid ? checkpoint_ram_q : 1'b1;
  always @(posedge sys_clk) checkpoint_ram_valid <= 1'b1;''', source)
            source = re.sub(r"\binitial\s+dout_r\s*=\s*1'h1\s*;", '', source)
            source = re.sub(r'\bdout_r\s*<=\s*mem\[', 'checkpoint_ram_q <= mem[', source)
        target = a.output / (newname + '.sv')
        target.write_text('// GENERATED from actual RTL checkpoint; do not edit.\n' + source)
        generated[path] = newname
        report.append({'instance': path, 'module': newname, 'initialized': len(initialized), 'absent_from_model': omitted})
        if not path.startswith('tb_exec.m'):
            helper_state.update(initialized)
    tb_fields = ['ckd', 'rfshdiv', 'rfshper', 'pend_cnt', 'pend_half', 'pendulum_r', 'addr_n', 'cpout', 'strb_n', 'setrun', 'setss_n', 'term_hb_d']
    helper_state.update({n: captured['tb_exec.'+n][1][0] for n in tb_fields if 'tb_exec.'+n in captured})
    helper_state.update({n: v[1][0] for n, v in captured.items() if n.startswith('tb_exec.u_pack.') and len(v[1]) == 1})
    # The continuation uses the SAME divide/refresh/pendulum/terminal helpers
    # as tb_exec, with their actual saved phase. Missing external inputs are
    # explicitly zero, matching Verilator's unconnected-input convention.
    m = next(c for c in selected if c.get('hier') == 'tb_exec.m')
    ports = mods[m.get('submodname')].findall('var')
    clock_ports = {v.get('name'): 'mclk' for v in ports if v.get('dir') == 'input' and v.get('name').startswith('CLK_')}
    connections = dict(clock_ports, sys_clk='sys_clk', PixelClkVCO='pixel_clk', RawPixelClk='pixel_clk', KeyboardData='term_ois',
                       SetRun="1'b1", SetSS_p_="1'b1", CPStrb_p_="1'b1", SetRunRfsh="1'b1", RfshPeriod='rfshper',
                       ChipsAre256_s_16K="1'b1", ChipsAre64K="1'b0", Mb0="1'b1", IOReset="1'b0", Pendulum='pendulum_r',
                       AltoTTLVideo='video', AltoHSync='hsync', AltoVSync_p_='vsync_n', HBlank='hblank', VBlank='vblank', HalfLine='halfline',
                       ClockP0__in='drv_cp', ClockM0__in='drv_cm', DataP0__in='drv_dp', DataM0__in='drv_dm',
                       TtlReady_p_='drv_ready_n', TtlOnLine_p_='drv_online_n', TtlTerm_p_='drv_term_n',
                       TtlIndex_p_='drv_index_n', TtlReadOnly_p_="1'b1", TtlEndOfCyl_p_="1'b1", SecIndx0_p_='drv_sec_n', Selected0_p_='drv_sel_n')
    connections['Select0_p_'] = 'disk_sel_n'
    connections.update({f'Host_{i}': f"1'b{(a.ethernet_host >> (7-i)) & 1}" for i in range(8)})
    if a.ethernet_wire:
        connections.update(RcvData='ethernet_rx', XmtData_p_='ethernet_tx_n')
    if a.hps_pack:
        connections.update(TtlEndOfCyl_p_='pack_eoc_n', TtlReadOnly_p_='~pack_readonly',
                           DataP0__in="pack_writing ? 1'b0 : drv_dp", DataM0__in="pack_writing ? 1'b0 : drv_dm",
                           DataP0='write_data_p', DataM0='write_data_m',
                           CylinderTag_p_='cyltag_n', HeadTag_p_='headtag_n', DriveTag_p_='drivetag_n', ContTag_p_='conttag_n')
        for i, name in enumerate(['000', '00', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9']):
            connections[f'TagBus_{name}_p_'] = f'tag_n[{11-i}]'
    for i in range(3):
        connections[f'CPAddr_{i}_p_'] = f"1'b{(helper_state['addr_n'] >> (2-i))&1}"
    for i in range(9):
        connections[f'CPOut_{i}'] = f"1'b{(helper_state['cpout'] >> (8-i))&1}"
    for i in range(1, 4):
        connections[f'SecIndx{i}_p_'] = connections[f'Selected{i}_p_'] = "1'b1"
    bindings = []
    for v in ports:
        name = v.get('name')
        if name in connections:
            bindings.append(f'    .{name}({connections[name]})')
        elif v.get('dir') == 'input':
            bindings.append(f"    .{name}(1'b0)")
    wrapper = f'''// GENERATED continuation of a verified RTL checkpoint, not a cold boot.
module dorado_checkpoint_machine (
    input wire sys_clk, input wire [10:0] ps2_key,
    input wire [15:0] mouse_keys,
    input wire signed [8:0] mouse_dx, mouse_dy,
    input wire mouse_pending,
    output wire mouse_taken,
    output wire signed [8:0] mouse_sent_dx, mouse_sent_dy,
HPS_PORTS
ETH_PORTS
    output wire video, hsync, vsync_n, hblank, vblank, halfline,
    output wire pixel_clk
);
  reg [3:0] ckd = 4'd{helper_state['ckd']};
  always @(posedge sys_clk) ckd <= (ckd == 3) ? 0 : ckd + 1'b1;
  wire mclk = ckd >= 2;
  integer rfshdiv = {helper_state['rfshdiv']};
  reg rfshper = 1'b{helper_state['rfshper']};
  always @(posedge sys_clk) begin
    rfshdiv <= rfshdiv + 1;
    if (rfshdiv >= 1065) begin rfshdiv <= 0; rfshper <= ~rfshper; end
  end
  integer pend_cnt = {helper_state['pend_cnt']};
  reg pendulum_r = 1'b{helper_state['pendulum_r']};
  always @(posedge sys_clk) begin
    if (pend_cnt >= {helper_state['pend_half']-1}) begin pend_cnt <= 0; pendulum_r <= ~pendulum_r; end
    else pend_cnt <= pend_cnt + 1;
  end
  checkpoint_u_pixclk #(.FREQ_KHZ(50000), .SYSPER(8)) u_pixclk
    (.sys_clk(sys_clk), .p8(pixel_clk), .p7(1'b0), .p14(1'b1));
  wire [15:0] kw0, kw1, kw2, kw3;
  checkpoint_u_keyboard u_keyboard (.sys_clk(sys_clk), .reset(1'b0), .ps2_key(ps2_key),
    .key_word0(kw0), .key_word1(kw1), .key_word2(kw2), .key_word3(kw3));
  reg term_hb_d = 1'b{helper_state['term_hb_d']};
  always @(posedge sys_clk) term_hb_d <= hblank;
  wire term_tick = hblank & ~term_hb_d;
  wire term_ois;
  checkpoint_u_term u_term (.sys_clk(sys_clk), .reset(1'b0), .line_tick(term_tick),
    .key_word0(kw0), .key_word1(kw1), .key_word2(kw2), .key_word3(kw3),
    .key_word4(mouse_keys), .mouse_dx(mouse_dx), .mouse_dy(mouse_dy), .mouse_pending(mouse_pending),
    .mouse_taken(mouse_taken), .mouse_sent_dx(mouse_sent_dx), .mouse_sent_dy(mouse_sent_dy),
    .OISData(term_ois));
  wire drv_cp, drv_cm, drv_dp, drv_dm, drv_ready_n, drv_online_n, drv_term_n, drv_index_n, drv_sec_n, drv_sel_n, disk_sel_n;
PACK_INSTANCE
  checkpoint_u_drv #(.SYSPER(8), .SECTORS_PER_REV(117), .WORDS_PER_TRACK(10080)) u_drv
    (.sys_clk(sys_clk), .reset(1'b0), .attached(PACK_ATTACHED), .selected(~disk_sel_n), .data_bit(PACK_BIT), .data_adv(drv_adv),
    .TtlReady_n(drv_ready_n), .TtlOnLine_n(drv_online_n), .TtlTerm_n(drv_term_n),
    .Selected_n(drv_sel_n), .SecIndx_n(drv_sec_n), .TtlIndex_n(drv_index_n),
    .DataP(drv_dp), .DataM(drv_dm), .ClockP(drv_cp), .ClockM(drv_cm));
  checkpoint_m #(.SYSPER(4)) m (
''' + ',\n'.join(bindings) + '\n  );\nendmodule\n'
    if a.hps_pack:
        ports = '''    input wire clk_host, pack_reset, pack_attached, pack_readonly, pack_mounted,
    input wire sd_ack, sd_buff_wr,
    input wire [12:0] sd_buff_addr,
    input wire [15:0] sd_buff_dout,
    output wire [15:0] sd_buff_din,
    output wire [31:0] sd_lba,
    output wire [5:0] sd_blk_cnt,
    output wire sd_rd, sd_wr, pack_ready, pack_fault, pack_dirty,'''
        pack = f'''  wire [11:0] tag_n;
  wire cyltag_n, headtag_n, drivetag_n, conttag_n, pack_eoc_n, pack_writing;
  wire write_data_p, write_data_m, drv_adv, media_bit;
  dorado_pack_hps #(.INITIAL_CYL(12'd{helper_state['tb_exec.u_pack.cur_cyl']}),
                    .INITIAL_HEAD(6'd{helper_state['tb_exec.u_pack.cur_head']})) u_media (
    .clk_sys(clk_host), .machine_clk(sys_clk), .reset(pack_reset),
    .attached(pack_attached), .readonly(pack_readonly), .remounted(pack_mounted),
    .tagbus_n(tag_n), .cyltag_n(cyltag_n), .headtag_n(headtag_n), .drivetag_n(drivetag_n), .conttag_n(conttag_n),
    .selected(~disk_sel_n), .data_adv(drv_adv), .pulse(drv_sector),
    .write_data_p(write_data_p), .write_data_m(write_data_m),
    .data_bit(media_bit), .write_gate(pack_writing), .endofcyl_n(pack_eoc_n), .ready(pack_ready), .fault(pack_fault), .dirty(pack_dirty),
    .sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
    .sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr), .sd_buff_din(sd_buff_din));'''
        wrapper = wrapper.replace('HPS_PORTS', ports).replace('PACK_INSTANCE', '  wire [15:0] drv_sector;\n' + pack).replace('PACK_ATTACHED', 'pack_attached').replace('PACK_BIT', 'media_bit')
        wrapper = wrapper.replace('.ClockP(drv_cp),', '.sector_out(drv_sector), .ClockP(drv_cp),')
    else:
        wrapper = wrapper.replace('HPS_PORTS', '').replace('PACK_INSTANCE', '  wire drv_adv;').replace('PACK_ATTACHED', "1'b1").replace('PACK_BIT', "1'b0")
    wrapper = wrapper.replace('ETH_PORTS', '    input wire ethernet_rx, output wire ethernet_tx_n,' if a.ethernet_wire else '')
    (a.output / 'dorado_checkpoint_machine.sv').write_text(wrapper)
    sources = [str(a.output / (n + '.sv')) for n in generated.values()]
    sources.append(str(a.output / 'dorado_checkpoint_machine.sv'))
    if a.hps_pack:
        sources.append('fpga/mister/dorado_pack_hps.sv')
    (a.output / 'sources.f').write_text('\n'.join(sources)+'\n')
    manifest = {'checkpoint': str(a.checkpoint), 'checkpoint_sha256': hashlib.sha256(snapshot).hexdigest(), 'model_fingerprint': fingerprint, 'storage_words': a.storage_words, 'hps_pack': a.hps_pack, 'top': generated['tb_exec.m'], 'helpers': helper_state, 'optimized_dff_inputs': reconstructed, 'stale_combinational_bits': stale_wires, 'instances': report}
    manifest['ethernet_wire'] = a.ethernet_wire
    manifest['ethernet_host'] = a.ethernet_host
    if a.base_pack:
        manifest['paired_disk_sha256'] = hashlib.sha256(media).hexdigest()
    (a.output / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(f'Generated {len(generated)} instance modules; {sum(r["initialized"] for r in report)} state fields initialized')
    print(f'Absent fields (unused, constant, or unresolved): {sum(len(r["absent_from_model"]) for r in report)}; inspect manifest and validate before hardware use')


if __name__ == '__main__':
    main()
