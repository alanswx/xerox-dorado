#!/usr/bin/env python3
"""Audit shared-write F10145 sources, physical register retention and all corners."""
import argparse
import json
from pathlib import Path
import shutil
import tempfile
import fit_f10145
import fit_f10145_shared_write as experiment
import fit_riot
import generate_f10145 as original
import generate_f10145_shared_write as candidate


def retained(text):
    title = '; Analysis & Synthesis Resource Utilization by Entity'
    assert title in text
    section = text[text.index(title):].split('\n\n', 1)[0]
    header, hits = None, []
    for line in section.splitlines():
        if not line.startswith('; '):
            continue
        fields = [v.strip() for v in line.split(';')[1:-1]]
        if fields and fields[0] == 'Compilation Hierarchy Node':
            header = fields
        elif header and len(fields) == len(header):
            row = dict(zip(header, fields))
            if row['Entity Name'] == 'cell_F10145A_snapshot':
                hits.append(row)
    assert len(hits) == 1, 'file hierarchy missing/duplicate'
    row = hits[0]
    assert row['Dedicated Logic Registers'] == '87 (87)', 'mutable file bits pruned/added'
    assert row['Block Memory Bits'] == '0', 'physical memory placement changed'
    return dict(physical_bits=74, private_transport_bits=13, synthesized_registers=87,
                combinational_aluts=int(row['Combinational ALUTs'].split()[0]))


def audit(directory):
    result = fit_riot.audit(directory)
    record = json.loads((directory/'provenance.json').read_text())
    with tempfile.TemporaryDirectory(prefix='f10145-shared-policy-', dir='/tmp') as temp:
        reference = Path(temp)/'inputs'
        experiment.prepare(reference)
        expected = json.loads((reference/'provenance.json').read_text())
    assert record['manifest'] == expected['manifest'] and record['reference_manifest'] == expected['reference_manifest'], 'manifest differs'
    assert record['source_hashes'] == expected['source_hashes'], 'tool/original source provenance differs'
    assert record['expected_qsf'] == expected['expected_qsf'], 'project policy differs'
    result['retained_state'] = {}
    for name, gen in [('baseline', original), ('snapshot', candidate)]:
        source, manifest = gen.generate()
        case = directory/name
        assert (case/'cell_F10145A_snapshot.sv').read_bytes() == source.encode(), 'adapter differs'
        assert (case/'top.sv').read_text() == fit_f10145.TOP, 'fixture differs'
        assert (case/'bank.sdc').read_text() == fit_f10145.SDC, 'constraints differ'
        assert (case/'audit.tcl').read_text() == fit_f10145.AUDIT, 'corner audit differs'
        assert manifest['logical_state_bits'] == 74
        result['retained_state'][name] = retained((case/'snapshot_fit.map.rpt').read_text(errors='replace'))
    return result


def controls(directory):
    records = []
    cases = [
        ('source', 'snapshot/cell_F10145A_snapshot.sv', 'state_hold && state_frozen && state_request', 'state_request'),
        ('clock', 'snapshot/bank.sdc', '20.000', '18.000'),
        ('policy', 'snapshot/snapshot_fit.qsf', 'DORADO_F10145_LOGIC=1', 'DORADO_F10145_LOGIC=0'),
        ('hold', 'snapshot/bank-hold-7_slow_1100mv_-40c.rpt', '(0 violated).', '(1 violated).'),
        ('warning', 'snapshot/map.log', None, None),
        ('baseline_state', 'baseline/snapshot_fit.map.rpt', '87 (87)', '86 (86)'),
        ('snapshot_state', 'snapshot/snapshot_fit.map.rpt', '87 (87)', '86 (86)'),
    ]
    for name, path, before, after in cases:
        with tempfile.TemporaryDirectory(prefix='f10145-shared-control-', dir='/tmp') as temp:
            root = Path(temp)/'fit'
            shutil.copytree(directory, root, ignore=shutil.ignore_patterns('db', 'incremental_db'))
            p = root/path
            text = p.read_text(errors='replace')
            if before:
                assert before in text
                text = text.replace(before, after)
            else:
                text += '\nWarning (10999): unreviewed inference\n'
            p.write_text(text)
            try:
                audit(root)
            except AssertionError as e:
                records.append(dict(case=name, rejected=True, diagnostic=str(e)))
            else:
                raise AssertionError('damaged F10145 fit accepted: '+name)
    return records


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory', type=Path)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    root = a.directory.resolve()
    result = audit(root)
    result['failure_controls'] = controls(root)
    a.output.write_text(json.dumps(result, indent=2)+'\n')
    print('SHARED F10145 AUDIT PASS: all87registers both variants; '
          f"ALM delta {result['delta']['alms']}; all4conditions minimum {result['minimum_snapshot_slack_ns']}ns;7bad controls reject")
