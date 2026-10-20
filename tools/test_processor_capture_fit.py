#!/usr/bin/env python3
"""Challenge the processor-capture timing reader with damaged evidence.

No Quartus invocation. Copies terminal raw reports and tests missing paths,
profile/dimension errors and changed sources/constraints even after receipts
are rehashed. These controls qualify the reader, not the FPGA circuit.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil

from audit_control_clock_fit import audit, CORNERS


def run(source, qualified, out, expected_component='processor_capture'):
    out.mkdir(parents=True, exist_ok=False)
    baseline = audit(source, qualified, False)
    assert baseline['status'] == 'PASS_COMPONENT_TIMING' and baseline['component'] == expected_component
    evaluator = 'dorado_' + expected_component + '_step.sv'

    def rehash(directory, name):
        digest = hashlib.sha256((directory / name).read_bytes()).hexdigest()
        for filename, key in [('provenance.json', 'files'), ('collected-input-check.json', 'actual')]:
            path = directory / filename
            record = json.loads(path.read_text())
            record[key][name] = digest
            path.write_text(json.dumps(record, indent=2) + '\n')

    cases = ['missing_evaluator', 'missing_corner', 'wrong_profile', 'wrong_dimensions',
             'changed_evaluator_rehashed', 'changed_min_delay_rehashed', 'relaxed_period_rehashed',
             'unconstrained_input', 'negative_setup']
    provenance=json.loads((source/'provenance.json').read_text())
    dependencies=[name for name in provenance['files'] if name.endswith('.sv') and name not in ('top.sv',evaluator)]
    if dependencies:
        cases.append('changed_dependency_rehashed')
    rows = []
    corner = next(iter(CORNERS))
    for name in cases:
        directory = out / name
        shutil.copytree(source, directory)
        if name == 'missing_evaluator':
            (directory / ('clock-evaluator-' + corner + '.rpt')).unlink()
        elif name == 'missing_corner':
            (directory / ('clock-hold-' + corner + '.rpt')).unlink()
        elif name in ['wrong_profile', 'wrong_dimensions']:
            path = directory / 'provenance.json'
            record = json.loads(path.read_text())
            if name == 'wrong_profile':
                record['component'] = 'unknown'
            else:
                record['launch_bits'] = 52
            path.write_text(json.dumps(record, indent=2) + '\n')
        elif name in ['changed_evaluator_rehashed','changed_dependency_rehashed']:
            path = directory / (dependencies[0] if name=='changed_dependency_rehashed' else evaluator)
            path.write_bytes(path.read_bytes() + b'\n// changed after qualification\n')
            rehash(directory, path.name)
        elif name in ['changed_min_delay_rehashed', 'relaxed_period_rehashed']:
            path = directory / 'clock.sdc'
            text = path.read_text()
            before, after = ('-min 0.000', '-min -10.000') if name == 'changed_min_delay_rehashed' else ('-period 15.000', '-period 30.000')
            assert before in text
            path.write_text(text.replace(before, after))
            rehash(directory, path.name)
        elif name == 'unconstrained_input':
            path = directory / ('clock-ucp-' + corner + '.rpt')
            text, count = re.subn(r'(;\s*Unconstrained Input Ports\s*;\s*)0(\s*;\s*)0', r'\g<1>1\g<2>1', path.read_text())
            assert count == 1
            path.write_text(text)
        elif name == 'negative_setup':
            path = directory / ('clock-setup-' + corner + '.rpt')
            text = path.read_text()
            text, count = re.subn(r'(Worst case slack is )-?\d+\.\d+', r'\g<1>-1.000', text, flags=re.I)
            assert count == 1
            text, count = re.subn(r'(;\s*Slack\s*;\s*)-?\d+\.\d+', r'\g<1>-1.000', text)
            assert count > 0
            path.write_text(text)
        try:
            record = audit(directory, qualified, False)
            assert record['status'] == 'REJECTED_COMPONENT_TIMING', (name, record)
            reason = record['status']
        except (AssertionError, FileNotFoundError) as error:
            # A passing reader must not slip through this assertion handler.
            if error.args and isinstance(error.args[0], tuple) and error.args[0][0] == name:
                raise
            reason = type(error).__name__ + ': ' + str(error)
        rows.append(dict(case=name, rejected=True, reason=reason))
    result = dict(status='PASS_' + expected_component.upper() + '_TIMING_READER_CONTROLS',
                  baseline=baseline, cases=rows, quartus_run=False)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(rows), 'damaged evidence cases rejected')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    parser.add_argument('--qualified', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--component', choices=['processor_capture', 'task_capture', 't_file', 't_parity', 'r_operand'], default='processor_capture')
    args = parser.parse_args()
    run(args.directory.resolve(), args.qualified.resolve(), args.output.resolve(), args.component)
