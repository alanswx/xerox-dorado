#!/usr/bin/env python3
"""Record the terminal rejected all-board fit; never treats it as qualification."""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re


def load(path):
    raw = path.read_bytes()
    return (gzip.decompress(raw) if path.suffix == '.gz' else raw).decode('latin1')


def rows(text):
    title = '; Fitter Resource Utilization by Entity'
    assert title in text
    section = text[text.index(title):].split('\n\n', 1)[0]
    header, result = None, []
    for line in section.splitlines():
        if not line.startswith('; '):
            continue
        fields = [x.strip() for x in line.split(';')[1:-1]]
        if fields and fields[0] == 'Compilation Hierarchy Node':
            header = fields
        elif fields and header:
            assert len(fields) == len(header)
            result.append(dict(zip(header, fields)))
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--directory', type=Path, required=True)
    p.add_argument('--baseline-fit', type=Path, required=True)
    p.add_argument('--integration-audit', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    directory = a.directory.resolve()
    log = load(directory/'fit.log')
    summary = load(directory/'Dorado.fit.summary')
    fit = load(directory/'Dorado.fit.rpt')
    assert 'Fitter Status : Failed' in summary
    assert 'Quartus Prime Fitter was unsuccessful. 2 errors' in log
    assert 'Error (11802): Can\'t fit design in device.' in log
    labs = re.search(r'Error \(170012\): Fitter requires (\d+) LABs.*device contains only (\d+) LABs', log)
    alms = re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / ([\d,]+)', summary)
    assert labs and alms
    needed, available = (int(x.replace(',', '')) for x in alms.groups())
    assert needed > available and int(labs[1]) > int(labs[2])
    assert 'Analysis & Synthesis was successful' in load(directory/'Dorado.map.summary') or (
        'Analysis & Synthesis Status : Successful' in load(directory/'Dorado.map.summary'))
    audit = json.loads(a.integration_audit.read_text())
    assert audit['status'] == 'PASS' and audit['boards'] == 11
    assert set(range(15)).issubset(r['case'] for r in audit['broken_controls'])
    current_rows = rows(fit)
    previous = {r['Entity Name']: r for r in rows(load(a.baseline_fit))
                if r['Entity Name'].endswith('_full_snapshot_experiment') or
                r['Entity Name'] == 'BaseBd_m_Rev_m_Am_nominal'}
    boards = []
    for r in current_rows:
        name = r['Entity Name']
        if not name.endswith('_full_snapshot_experiment'):
            continue
        old = previous['BaseBd_m_Rev_m_Am_nominal' if name == 'BaseBd_full_snapshot_experiment' else name]
        estimate = float(r['ALMs needed [=A-B+C]'].split()[0])
        before = float(old['ALMs needed [=A-B+C]'].split()[0])
        boards.append(dict(module=name, failed_fit_alm_estimate=estimate,
                           baseline_alm_attribution=before, difference=round(estimate-before, 1)))
    assert len(boards) == len({r['module'] for r in boards}) == 11
    ri = [r for r in current_rows if r['Entity Name'] == 'M6532_snapshot']
    assert len(ri) == 5
    riot_estimate = sum(float(r['ALMs needed [=A-B+C]'].split()[0]) for r in ri)
    paths = [a.integration_audit, a.baseline_fit, *sorted(directory.iterdir()), Path(__file__)]
    result = dict(status='REJECTED_CAPACITY', scope=__doc__,
                  required_labs=int(labs[1]), available_labs=int(labs[2]),
                  alms_needed=needed, available_alms=available, excess_alms=needed-available,
                  excess_percent=100*(needed/available-1),
                  boards=boards, five_riot_failed_fit_alm_estimate=round(riot_estimate, 1),
                  attribution_limit='Failed-fit hierarchy estimates guide experiments; not a legal placed design, routed timing, or a sum predicting the next fit.',
                  input_hashes={str(path): hashlib.sha256(path.read_bytes()).hexdigest()
                                for path in paths if path.is_file() and path.resolve() != a.output.resolve()})
    a.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f'CAPACITY REJECTED: {needed}/{available} ALMs; excess {needed-available}; '
          f'{labs[1]}/{labs[2]} LABs; five RIOT attribution {riot_estimate:.1f} ALMs')


if __name__ == '__main__':
    main()
