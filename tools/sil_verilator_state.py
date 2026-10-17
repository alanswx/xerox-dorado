#!/usr/bin/env python3
"""Describe POD state in one exact Verilated tb_exec build.

Timing coroutines, pointers, file handles and strings remain freshly constructed.
The generated descriptor is only valid for the C++ model it fingerprints.
"""
import argparse
import hashlib
from pathlib import Path
import re
import subprocess

ap = argparse.ArgumentParser(description=__doc__)
ap.add_argument('--obj-dir', required=True, type=Path)
ap.add_argument('-o', '--output', required=True, type=Path)
a = ap.parse_args()
vr = subprocess.check_output(['verilator', '-V'], text=True)
inc = Path(re.search(r'VERILATOR_ROOT\s*=\s*(\S+)', vr)[1]) / 'include'
syms = (a.obj_dir / 'Vtb_exec__Syms.h').read_text()
objects = dict((name, typ) for typ, name in
               re.findall(r'\b(Vtb_exec_\w+)\s+(TOP(?:__\w+)?);', syms))
classes = set(objects.values())
lines = ['#include "Vtb_exec__Syms.h"', '#include <cstddef>', '#include <cstdio>', 'int main(){',
         'printf("SIZE %zu\\n",sizeof(Vtb_exec__Syms));']
for obj, typ in objects.items():
    h = (a.obj_dir / (typ + '.h')).read_text()
    for line in h.splitlines():
        m = re.match(r'\s*(?:(?:[CSIQ]Data)(?:/\*.*?\*/)?|VlUnpacked<.*>|VlWide<.*>)\s+(\w+)\s*;', line)
        if not m:
            # Clock/data ports are POD too. Verilator declares them with
            # macros instead of CData/SData. Omitting them leaves stale
            # handshake outputs and can lose the first clock after restore.
            m = re.match(r'\s*VL_(?:IN|OUT|INOUT)(?:8|16|64|W)?\((\w+),', line)
        if not m:
            continue
        name = m[1]
        # SV file descriptors belong to this process, not to the guest.
        if name.endswith(('__DOT__fd', '__DOT__imfd', '__DOT__cycle_limit_fd')):
            continue
        lines.append(f'printf("%zu %zu\\n",offsetof(Vtb_exec__Syms,{obj})+offsetof({typ},{name}),sizeof((({typ}*)nullptr)->{name}));')
lines += ['}']
source = a.obj_dir / 'rtl_state_layout.cpp'
exe = a.obj_dir / 'rtl_state_layout'
source.write_text('\n'.join(lines) + '\n')
subprocess.run(['c++', '-std=c++20', '-w', '-I'+str(a.obj_dir), '-I'+str(inc),
                '-I'+str(inc/'vltstd'), str(source), '-o', str(exe)], check=True)
layout = subprocess.check_output([str(exe)], text=True)
size = int(layout.splitlines()[0].split()[1])
ranges = []
for line in layout.splitlines()[1:]:
    offset, length = map(int, line.split())
    ranges.append((offset, length))
ranges.sort()
merged = []
for offset, length in ranges:
    if merged and sum(merged[-1]) == offset:
        merged[-1][1] += length
    else:
        merged.append([offset, length])
hash_ = hashlib.sha256(layout.encode())
for path in sorted([*a.obj_dir.glob('Vtb_exec*.cpp'), *a.obj_dir.glob('Vtb_exec*.h')]):
    hash_.update(path.read_bytes())
a.output.write_text('// Generated exact-build RTL state descriptor.\n#pragma once\n'
    + f'static constexpr size_t rtl_state_size={size};\n'
    + f'static constexpr char rtl_state_fingerprint[]="{hash_.hexdigest()}";\n'
    + 'static constexpr unsigned rtl_state_ranges[][2]={\n'
    + ''.join('{%d,%d},\n' % tuple(r) for r in merged) + '};\n')
