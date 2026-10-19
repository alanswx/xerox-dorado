#!/usr/bin/env python3
"""Audit exact shared-port sources, full retained state and matched timing."""
import argparse
import json
from pathlib import Path
import re
import fit_riot
import generate_riot as original
import generate_riot_shared_port as candidate


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
            if row['Entity Name'] == 'M6532_snapshot':
                hits.append(row)
    assert len(hits) == 1, 'RIOT hierarchy missing/duplicate'
    row = hits[0]
    assert row['Dedicated Logic Registers'] == '1118 (1118)', 'mutable RIOT bits pruned/added'
    assert row['Block Memory Bits'] == '0', 'resettable RAM implementation changed'
    return dict(physical_bits=1097, private_transport_bits=21, synthesized_registers=1118,
                combinational_aluts=int(re.match(r'\d+', row['Combinational ALUTs'])[0]))


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory', type=Path)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    directory = a.directory.resolve()
    result = fit_riot.audit(directory)
    result['retained_state'] = {}
    for name, gen in [('baseline', original), ('snapshot', candidate)]:
        expected, manifest = gen.generate()
        assert (directory/name/'M6532_snapshot.sv').read_bytes() == expected.encode(), 'adapter differs'
        assert manifest['logical_state_bits'] == 1097
        text = (directory/name/'snapshot_fit.map.rpt').read_text(errors='replace')
        result['retained_state'][name] = retained(text)
        bad = text.replace('1118 (1118)', '1117 (1117)')
        assert bad != text
        try:
            retained(bad)
        except AssertionError as e:
            assert 'mutable RIOT bits' in str(e)
        else:
            raise AssertionError('lost synthesis bit accepted')
    result['failure_controls'] = fit_riot.controls(directory)
    result['state_retention_controls'] = ['baseline lost bit rejected', 'snapshot lost bit rejected']
    a.output.write_text(json.dumps(result, indent=2)+'\n')
    print('SHARED RIOT AUDIT PASS: all1118registers both variants; '
          f"ALM delta {result['delta']['alms']}; all4conditions minimum {result['minimum_snapshot_slack_ns']}ns;7bad controls reject")


if __name__ == '__main__':
    main()
