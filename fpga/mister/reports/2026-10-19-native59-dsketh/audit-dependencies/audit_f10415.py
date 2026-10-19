#!/usr/bin/env python3
"""Check the isolated snapshot-bank inference, packing and four-corner reports."""
import argparse
import hashlib
import json
from pathlib import Path
import re

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def require(condition, why):
    if not condition:
        raise ValueError(why)


def resources(case):
    text = (case/'snapshot_fit.fit.summary').read_text()
    require('Fitter Status : Successful' in text, 'fit did not complete')
    patterns = {
        'alms': r'Logic utilization \(in ALMs\) : ([\d,]+)',
        'registers': r'Total registers : ([\d,]+)',
        'ram_bits': r'Total block memory bits : ([\d,]+)',
        'ram_blocks': r'Total RAM Blocks : ([\d,]+)',
    }
    result = {}
    for key, pattern in patterns.items():
        match = re.search(pattern, text)
        require(match is not None, 'missing resource: '+key)
        result[key] = int(match[1].replace(',', ''))
    require(result['ram_bits'] == 1024*34, 'RAM inference lost or changed geometry')
    require(result['ram_blocks'] == 4, 'RAM packing changed')
    return result


def audit(directory):
    provenance = json.loads((directory/'provenance.json').read_text())
    for name, digest in provenance['source_hashes'].items():
        require(hashlib.sha256((ROOT/name).read_bytes()).hexdigest() == digest,
                'build provenance differs: '+name)
    results = {}
    expected_corners = {'7_slow_1100mv_100c', '7_slow_1100mv_-40c',
                        'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c'}
    for variant in ('baseline', 'snapshot'):
        case = directory/variant
        result = resources(case)
        for name, digest in provenance['source_hashes'].items():
            if name.endswith(('.sv', '.v')):
                require(hashlib.sha256((case/Path(name).name).read_bytes()).hexdigest() == digest,
                        'fitted source differs: '+name)
        corners = {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}
        require(corners == expected_corners, 'missing or unexpected timing corner')
        result['timing'] = {}
        for corner in sorted(corners):
            row = {}
            for mode in ('setup', 'hold', 'mpw'):
                text = (case/f'bank-{mode}-{corner}.rpt').read_text()
                match = re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)', text)
                require(match is not None, 'missing timed paths: '+mode+'/'+corner)
                require(int(match[1]) == 0 and float(match[2]) >= 0,
                        'timing violation: '+mode+'/'+corner)
                row[mode] = float(match[2])
            for mode in ('recovery', 'removal'):
                text = (case/f'bank-{mode}-{corner}.rpt').read_text()
                require(text.strip().endswith('Nothing to report.'),
                        'unexpected asynchronous paths: '+mode)
                row[mode] = 'N/A: no asynchronous controls'
            text = (case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks', 'Unconstrained Clocks', 'Unconstrained Input Ports',
                          'Unconstrained Input Port Paths', 'Unconstrained Output Ports',
                          'Unconstrained Output Port Paths'):
                require(re.search(r'; '+label+r'\s*; 0\s*; 0\s*;', text) is not None,
                        'unconstrained or missing coverage: '+label+'/'+corner)
            result['timing'][corner] = row
        if variant == 'snapshot':
            text = (case/'snapshot_fit.map.rpt').read_text()
            modes = re.findall(r'; READ_DURING_WRITE_MODE_MIXED_PORTS\s*; (\w+)\s*;', text)
            require(len(modes) == 34 and set(modes) == {'OLD_DATA'},
                    'RAM old-data collision mode not preserved')
            result['old_data_ram_instances'] = len(modes)
        results[variant] = result
    results['delta'] = {key: results['snapshot'][key]-results['baseline'][key]
                        for key in ('alms', 'registers', 'ram_bits', 'ram_blocks')}
    return dict(status='PASS', scope=provenance['scope'], results=results,
                limits='Isolated bank, virtual data I/O. No full-board/core area/timing, global freeze, physical snapshot or complete-machine restore claim.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = audit(args.directory.resolve())
    text = json.dumps(result, indent=2)+'\n'
    if args.output:
        args.output.write_text(text)
    print(text, end='')
