#!/usr/bin/env python3
"""Compare original-ROM native CPU map captures; component evidence only."""
import argparse
import collections
import gzip
import hashlib
import json
from pathlib import Path
import re


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inspect(control, candidate):
    records = []
    dumps = []
    logs = []
    evidence = {}
    for directory in (control, candidate):
        provenance = directory / 'provenance.json.gz'
        record = json.loads(gzip.decompress(provenance.read_bytes()))
        for name, digest in record['inputs'].items():
            assert sha(directory / 'executed-inputs' / name) == digest, name
        state = json.loads((directory / 'run-state.json').read_text())
        assert state['status'] != 'RUNNING', directory
        assert all(action['returncode'] == 0 for action in state['actions'])
        assert record['probe_only']
        log = (directory / 'test.log').read_text()
        assert 'INITIAL ROM PASS:' in log
        dump = (directory / 'register-map-inspection.txt').read_text().splitlines()
        assert 'MAP-PLANES a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 d04 d05 d06 d07 d08 d09 d10 d11 d12 d13' in dump
        words = {}
        for line in dump:
            if line.startswith(('RM ', 'MAP ')):
                kind, address, value = line.split()
                assert (kind, address) not in words
                words[kind, address] = value
        assert collections.Counter(k[0] for k in words) == {'RM': 256, 'MAP': 16384}
        records.append(record)
        dumps.append((dump[0], words))
        logs.append(log)
        for name in ('provenance.json.gz', 'run-state.json', 'test.log', 'register-map-inspection.txt'):
            evidence[str(directory / name)] = sha(directory / name)
    for key in ('external_inputs', 'native_io_inputs', 'native_memory_inputs', 'native_ifu_inputs'):
        assert records[0][key] == records[1][key], key
    assert records[0]['native_cpu_baseline_inputs'] and not records[1]['native_cpu_baseline_inputs']
    assert dumps[0][0] == dumps[1][0]
    differences = [key for key, value in dumps[0][1].items() if value != dumps[1][1][key]]
    map_differences = [key for key in differences if key[0] == 'MAP']
    rp_differences = [key for key in map_differences if dumps[0][1][key][:16] != dumps[1][1][key][:16]]
    flag_differences = [key for key in map_differences if dumps[0][1][key][16:] != dumps[1][1][key][16:]]
    result = dict(scope=__doc__, inspection=dumps[0][0],
        RM_words_compared=256, RM_mismatches=sum(key[0] == 'RM' for key in differences),
        map_rows_compared=16384, map_row_mismatches=len(map_differences),
        real_page_plane_mismatches=len(rp_differences), other_plane_mismatches=len(flag_differences),
        mismatching_real_pages_zero_in_candidate=all(dumps[1][1][key][:16] == '0'*16 for key in rp_differences),
        first_map_differences=[dict(raw_row_column_address=key[1], control=dumps[0][1][key],
            candidate=dumps[1][1][key]) for key in map_differences[:16]],
        shared_IO_memory_IFU_verified=True, hardware_cause_established=False,
        desktop_qualified=False, speedup_qualified=False,
        warning='Map dump addresses are raw DRAM row/column indices, not virtual page numbers.')
    storage = [[dict(re.findall(r'(\w+)=([^ ]+)', line)) for line in log.splitlines()
        if line.startswith('BOOT_DIAG_STORAGE_ADDRESS ')] for log in logs]
    if any(storage):
        assert len(storage[0]) == len(storage[1]) == records[0]['storage_window_samples']
        assert [row['sample'] for row in storage[0]] == [row['sample'] for row in storage[1]]
        result['storage_trace'] = dict(rows_per_run=len(storage[0]),
            first_sample=int(storage[0][0]['sample']), last_sample=int(storage[0][-1]['sample']),
            differences={key: sum(a[key] != b[key] for a, b in zip(*storage))
                for key in storage[0][0] if key != 'sample'},
            warning='DRAM address/data divergence is observed, not an independent proof of the first bad capture.')
    phases = [[dict(re.findall(r'(\w+)=([^ ]+)', line)) for line in log.splitlines()
        if line.startswith('BOOT_DIAG_PHASE ')] for log in logs]
    if any(phases):
        assert all(len(rows) == 6000 for rows in phases)
        # Before all six error sources are enabled, the two startup histories
        # can differ by a few host samples. Establish the offset from the
        # actual map-write events rather than assuming equal sample numbers.
        writes = [re.findall(r'MAPWRITE #(\d+) @(\d+) pc=(\w+)', log) for log in logs]
        pairs = [(a, b) for a, b in zip(*writes) if a[1] != '0' and b[1] != '0']
        assert pairs and all(a[0] == b[0] and a[2] == b[2] for a, b in pairs)
        offsets = {int(a[1])-int(b[1]) for a, b in pairs}
        assert len(offsets) == 1, offsets
        offset = offsets.pop()
        candidate_rows = {int(row['sample']): row for row in phases[1]}
        matched = [(a, candidate_rows[int(a['sample'])-offset]) for a in phases[0]
            if int(a['sample'])-offset in candidate_rows]
        counts = {key: sum(a[key] != b[key] for a, b in matched) for key in phases[0][0] if key != 'sample'}
        first = {}
        for key in ('B', 'BMux', 'Mapbuf', 'written'):
            first[key] = next((dict(control=a, candidate=b) for a, b in matched if a[key] != b[key]), None)
        first_bad_write = next(((a, b) for a, b in matched
            if a['pending'] == b['pending'] == '1' and a['written'] != b['written']), None)
        result['phase_trace'] = dict(rows_per_run=6000, control_minus_candidate_host_samples=offset,
            alignment_map_write_pairs=len(pairs), aligned_rows=len(matched), differences=counts,
            first_difference=first,
            actual_pending_writes=[[
                row for row in rows if row['pending'] == '1'] for rows in phases])
        if first_bad_write:
            sample = int(first_bad_write[0]['sample'])
            result['phase_trace']['first_bad_sampled_write'] = dict(
                control=first_bad_write[0], candidate=first_bad_write[1],
                preceding_window=[dict(control=a, candidate=b) for a, b in matched
                    if sample-96 <= int(a['sample']) <= sample])
    result['evidence_sha256'] = evidence
    result['analyzer_sha256'] = sha(Path(__file__))
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('control', 'candidate', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    result = inspect(args.control, args.candidate)
    with args.output.open('x') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    print('Map rows:', result['map_row_mismatches'], 'mismatches; RM:', result['RM_mismatches'])
