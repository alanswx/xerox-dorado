#!/usr/bin/env python3
"""Fresh elaborated inventory outside the eleven qualified board adapters.

Uses the exact staged project's source files, macros and wrapper parameters.
Review inventory only: no access implementation, field exclusions, completeness
claim, machine pause or state imports. Top-level MiSTer input/video/network and
ARM services are outside dorado_native_machine and remain separate work.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT/'tools'))
from sil_state_inventory import inventory

BOARDS = ('ContA', 'ContB', 'ProcH', 'ProcL', 'IFU', 'MemC', 'MemD',
          'MemX', 'DskEth', 'DispY', 'BaseBd')


def run(build, output):
    output.mkdir(parents=True, exist_ok=False)
    sources, macros = [], []
    qsf = build/'core/Dorado.qsf'
    for line in qsf.read_text().splitlines():
        if not line.startswith('set_global_assignment -name '):
            continue
        words = shlex.split(line)
        if len(words) != 4 or words[:2] != ['set_global_assignment', '-name']:
            continue
        kind, value = words[2:]
        if kind == 'VERILOG_MACRO':
            macros.append(value)
        if kind in ('VERILOG_FILE', 'SYSTEMVERILOG_FILE'):
            path = (qsf.parent/value).resolve()
            # The wrapper's entire source hierarchy lives in staged src/.
            # Core/ contains the FPGA shell/PLL/vendor HPS interface instead.
            if path.is_relative_to(build/'src') and path not in sources:
                sources.append(path)
    assert sources and len(macros) == len(set(macros)), 'project source/macro list differs'
    includes = sorted({str(p.parent) for p in sources})
    xml = output/'native-machine.xml'
    command = ['verilator', '--xml-only', '--timing', '--top-module',
               'dorado_native_machine', '-Wno-fatal', '--xml-output', str(xml),
               '--Mdir', str(output/'obj_xml'),
               *['-I'+p for p in includes], *['+define+'+m for m in macros],
               *map(str, sources)]
    hashes = {str(p.relative_to(build)): hashlib.sha256(p.read_bytes()).hexdigest()
              for p in sources + [qsf]}
    provenance = dict(scope=__doc__, build=str(build), command=command,
                      sources=hashes, macros=macros,
                      tools={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                             for p in (Path(__file__).resolve(), ROOT/'tools/sil_state_inventory.py')})
    (output/'provenance.json').write_text(json.dumps(provenance, indent=2)+'\n')
    with (output/'elaborate.log').open('w') as f:
        subprocess.run(command, cwd=build/'src', stdout=f, stderr=subprocess.STDOUT, check=True)
    for name, expected in hashes.items():
        assert hashlib.sha256((build/name).read_bytes()).hexdigest() == expected, 'source changed: '+name
    result = inventory(xml.read_bytes())
    prefixes = tuple('dorado_native_machine.m.b_'+b+'.' for b in BOARDS)
    support = [r for r in result['candidates'] if not r['path'].startswith(prefixes)]
    assert len(result['candidates']) > len(support) > 0
    (output/'complete-review-inventory.json').write_text(json.dumps(result, indent=2)+'\n')
    groups = {}
    for row in support:
        instance = row['path'][:-(len(row['name'])+1)]
        parts = instance.split('.')
        scope = (instance+'.(self)' if len(parts) == 1 or parts == ['dorado_native_machine', 'm']
                 else '.'.join(parts[:3] if parts[1] == 'm' else parts[:2]))
        item = groups.setdefault(scope, dict(candidate_counts=Counter(), candidate_bits=Counter()))
        item['candidate_counts'][row['category']] += 1
        item['candidate_bits'][row['category']] += row['bits']
    reviewed = dict(status='REVIEW ONLY', scope=__doc__, xml_sha256=result['xml_sha256'],
                    total_elaborated_instances=result['instances'],
                    board_paths_separately_qualified=list(prefixes), groups=groups,
                    candidates=support)
    (output/'machine-support-review.json').write_text(json.dumps(reviewed, indent=2)+'\n')
    print(json.dumps(dict(status=reviewed['status'], support_candidates=len(support), groups=groups), indent=2))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build-root', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.build_root.resolve(), a.output.resolve())
