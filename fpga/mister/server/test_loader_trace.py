#!/usr/bin/env python3
"""Test endpoint/timing extraction against actual recorded protocol traffic."""
import gzip
import json
from pathlib import Path
from analyse_loader_trace import analyse, observed_lines

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def main():
    manifest = json.loads((HERE/'loader_fixture_manifest.json').read_text())
    sizes = {name: row['bytes'] for name, row in manifest.items()}
    text = gzip.decompress((ROOT/'fpga/mister/reports/2026-10-19-native47-desktop/server-prefix.log.gz').read_bytes()).decode()
    # Timestamps are synthetic for parser coverage ONLY. They do not measure
    # the historical run, whose server transcript had no timestamps.
    rows = [dict(text=line, earliest_ns=(i+1)*1000000, latest_ns=(i+1)*1000000+1000)
            for i, line in enumerate(text.splitlines())]
    result = analyse(rows, sizes)
    assert result['files'] == 34 and result['bcd_files'] == 33
    assert result['final_probe'] == '002708f6' and result['final_peer'] == [0o442, 0o22, 0o62014]
    # Break records across arbitrary partial lines and confirm reconstruction.
    chunks = []
    for offset in range(0, len(text), 137):
        tick=(len(chunks)+1)*1000000
        chunks.append(dict(event='chunk',source='wire',previous_read_end_ns=tick,
                           read_end_ns=tick+1000,text=text[offset:offset+137]))
    reconstructed = analyse(observed_lines(chunks), sizes)
    assert reconstructed['final_probe'] == result['final_probe']
    cases = {
        'missing final ACK': [dict(r,text=r['text'].replace('FTP_ACK ack=002708f6','FTP_ACK ack=002708f5')) for r in rows],
        'missing file data': [r for r in rows if not r['text'].startswith('FTP_QUEUE type=0o20 id=00270256')],
        'missing earlier file': [r for r in rows if not r['text'].startswith('STP_TRANSFER Cedar6.1/UserProfile/UserProfileImpl.bcd')],
        'missing final probe': [r for r in rows if not r['text'].startswith('FTP_QUEUE type=0o21 id=002708f6')],
    }
    for name, changed in cases.items():
        try:
            analyse(changed, sizes)
        except ValueError:
            pass
        else:
            raise AssertionError(name+' accepted')
    print('LOADER TRACE PASS: actual34-file/33-BCD sequence, exact peer/end ACK, split records and four failure controls; synthetic test times only')


if __name__ == '__main__':
    main()
