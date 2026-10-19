#!/usr/bin/env python3
"""Audit enabled half-bank geometry, old-data behavior and routed timing."""
import argparse
import hashlib
import json
from pathlib import Path
import re

import fit_f10415_enable as fit
from audit_f10415 import require

ROOT = Path(__file__).resolve().parents[3]
CORNERS = {'7_slow_1100mv_100c', '7_slow_1100mv_-40c',
           'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c'}


def audit(directory):
    provenance = json.loads((directory/'provenance.json').read_text())
    for name, digest in provenance['source_hashes'].items():
        require(hashlib.sha256((ROOT/name).read_bytes()).hexdigest() == digest,
                'build provenance differs: '+name)
    results = {}
    for variant in ('baseline', 'snapshot'):
        case = directory/variant
        for name, digest in provenance['source_hashes'].items():
            if name.endswith(('.sv', '.v')):
                require(hashlib.sha256((case/Path(name).name).read_bytes()).hexdigest() == digest,
                        'fitted source differs: '+name)
        require((case/'top.sv').read_text() == fit.TOP, 'top fixture differs')
        require((case/'bank.sdc').read_text() == fit.SDC, 'constraints differ')
        require((case/'audit.tcl').read_text() == fit.AUDIT, 'timing audit script differs')
        text = (case/'snapshot_fit.fit.summary').read_text()
        require('Fitter Status : Successful' in text, 'fit incomplete')
        row = {}
        for key, label in (('alms', r'Logic utilization \(in ALMs\)'),
                           ('registers', 'Total registers'),
                           ('ram_bits', 'Total block memory bits'),
                           ('ram_blocks', 'Total RAM Blocks')):
            m = re.search(label+r' : ([\d,]+)', text)
            require(m is not None, 'resource missing: '+key)
            row[key] = int(m[1].replace(',', ''))
        require(row['ram_bits'] == 18432 and row['ram_blocks'] == 2,
                'RAM geometry or packing changed')
        corners = {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}
        require(corners == CORNERS, 'missing or unexpected corner')
        row['timing'] = {}
        for corner in sorted(CORNERS):
            checks = {}
            for mode in ('setup', 'hold', 'mpw'):
                text = (case/f'bank-{mode}-{corner}.rpt').read_text()
                m = re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)', text)
                require(m is not None, 'missing timed paths: '+mode+'/'+corner)
                require(int(m[1]) == 0 and float(m[2]) >= 0, 'timing violation: '+mode+'/'+corner)
                checks[mode] = float(m[2])
            for mode in ('recovery', 'removal'):
                text = (case/f'bank-{mode}-{corner}.rpt').read_text()
                require(text.strip().endswith('Nothing to report.'), 'unexpected async path: '+mode)
                checks[mode] = 'N/A: no asynchronous controls'
            text = (case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks', 'Unconstrained Clocks', 'Unconstrained Input Ports',
                          'Unconstrained Input Port Paths', 'Unconstrained Output Ports',
                          'Unconstrained Output Port Paths'):
                require(re.search(r'; '+label+r'\s*; 0\s*; 0\s*;', text) is not None,
                        'missing timing coverage: '+label+'/'+corner)
            row['timing'][corner] = checks
        if variant == 'snapshot':
            text = (case/'snapshot_fit.map.rpt').read_text()
            for parameter, expected in (
                ('OPERATION_MODE', ['DUAL_PORT']), ('WIDTH_A', ['18']), ('WIDTH_B', ['18']),
                ('NUMWORDS_A', ['1024']), ('NUMWORDS_B', ['1024']),
                ('READ_DURING_WRITE_MODE_MIXED_PORTS', ['OLD_DATA']),
            ):
                values = re.findall(r'; '+parameter+r'\s*; (\w+)\s*;', text)
                require(values == expected, 'RAM mode changed: '+parameter)
            row['inferred_ram'] = 'one1024x18 simple-dual-port macro; common clock, OLD_DATA'
        results[variant] = row
    delta = {key: results['snapshot'][key]-results['baseline'][key]
             for key in ('alms', 'registers', 'ram_bits', 'ram_blocks')}
    return dict(status='PASS', scope=provenance['scope'], results=results, delta=delta,
                limits='Isolated bank only. No actual-board/global barrier/physical snapshots or complete machine restore qualification.')


def check_controls(directory):
    import shutil
    import tempfile
    results = []
    audit(directory)
    for case in ('source', 'constraints', 'geometry', 'old_data', 'hold', 'missing_corner'):
        with tempfile.TemporaryDirectory(prefix='dorado-enabled-bank-audit-', dir='/tmp')as tmp:
            out = Path(tmp)/'evidence'
            shutil.copytree(directory, out)
            if case == 'missing_corner':
                (out/'snapshot/bank-setup-7_slow_1100mv_-40c.rpt').unlink()
                expected = 'missing or unexpected corner'
            else:
                name, before, after, expected = {
                    'source': ('snapshot/f10415_enable_bank.sv', 'saved_q <= state_data;', "saved_q <= 18'b0;", 'fitted source differs'),
                    'constraints': ('snapshot/bank.sdc', '20.000', '19.000', 'constraints differ'),
                    'geometry': ('snapshot/snapshot_fit.fit.summary', 'Total RAM Blocks : 2', 'Total RAM Blocks : 3', 'RAM geometry'),
                    'old_data': ('snapshot/snapshot_fit.map.rpt', 'OLD_DATA', 'NEW_DATA', 'RAM mode changed'),
                    'hold': ('snapshot/bank-hold-7_slow_1100mv_-40c.rpt', '(0 violated).  Worst case slack is', '(1 violated).  Worst case slack is', 'timing violation'),
                }[case]
                path = out/name
                text = path.read_text()
                require(before in text, 'control fixture changed: '+case)
                path.write_text(text.replace(before, after))
            try:
                audit(out)
            except ValueError as e:
                require(expected in str(e), 'wrong failure reason: '+case+' '+str(e))
                results.append(dict(case=case, rejected=True, diagnostic=str(e)))
            else:
                raise ValueError('damaged evidence accepted: '+case)
    return results


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--negative-controls', action='store_true')
    args = parser.parse_args()
    result = audit(args.directory)
    if args.negative_controls:
        result['failure_controls'] = check_controls(args.directory)
    text = json.dumps(result, indent=2)+'\n'
    if args.output:
        args.output.write_text(text)
    print(text, end='')
