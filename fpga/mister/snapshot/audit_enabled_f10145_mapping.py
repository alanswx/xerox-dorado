#!/usr/bin/env python3
"""Check all enabled-core F10145 inferred RAMs against the board manifests.

Synthesis mapping only: not physical placement, register/window retention,
timing, firmware boot or complete-machine snapshot qualification.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import re
from audit_baseboard_full import table
from audit_enabled_core import ports


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def expected_files(source):
    result, inputs = {}, {}
    for folder, boards in [
        ('snapshot_native_conta', ['ContA']),
        ('snapshot_native_processor', ['ProcH', 'ProcL']),
        ('snapshot_native_memc', ['MemC']), ('snapshot_native_memd', ['MemD']),
        ('snapshot_native_memx', ['MemX']), ('snapshot_native_dsketh', ['DskEth']),
        ('snapshot_native_dispy', ['DispY']),
    ]:
        path = source/'fpga/mister'/folder/'assets.json'
        metadata = json.loads(path.read_text())
        inputs[str(path)] = sha(path)
        manifests = metadata.get('manifests', {'ContA': metadata.get('manifest')})
        assert set(manifests) == set(boards), 'unexpected board manifest set'
        for board, manifest in manifests.items():
            board_source = path.parent/(board+'_full_snapshot.sv')
            inputs[str(board_source)] = sha(board_source)
            board_text = board_source.read_text()
            for row in manifest.get('files', manifest.get('memories', [])):
                if row['kind'] != 'cell_F10145A':
                    continue
                geometry = row.get('ram', dict(words=row.get('words'), width=row.get('width')))
                assert geometry['words'] == 16 and geometry['width'] == 4
                assert row['write_window']['index'] == 16 and row['write_window']['width'] == 10
                key = board+'/'+row['package']
                assert key not in result, 'duplicate manifest package'
                pins = dict(ports(board_text, row['package']))
                connected = [p for p in ('p2', 'p1', 'p15', 'p14') if pins.get(p)]
                assert connected, 'ordinary output entirely disconnected; review required'
                result[key] = dict(words=16, width=4, connected_normal_outputs=connected)
    assert len(result) == 85, 'unexpected frozen-core register-file inventory'
    return result, inputs


def inspect_rows(rows, expected):
    copies = {}
    for row in rows:
        if 'cell_F10145A_snapshot:' not in row['Name']:
            continue
        match = re.search(r'\|(\w+)_full_snapshot_experiment:b_(\w+)\|'
                          r'cell_F10145A_snapshot:(u_\w+)\|altdpram:(mem(?:__dual)?_rtl_0)\|', row['Name'])
        assert match and match[1] == match[2], 'unexpected register-file hierarchy'
        key, replica = match[1]+'/'+match[3], match[4]
        assert key in expected, 'unexpected register-file package'
        group = copies.setdefault(key, set())
        assert replica not in group, 'duplicate read replica'
        group.add(replica)
        # The administrative replica must retain all four bits. The ordinary
        # replica may omit only pins absent from the exact compiled board.
        width = 4 if replica == 'mem_rtl_0' else len(expected[key]['connected_normal_outputs'])
        for field, value in dict(Type='MLAB', Mode='Simple Dual Port',
                                 Size=str(16*width), MIF='None').items():
            assert row[field] == value, 'register-file RAM differs: '+field
        for port in ('A', 'B'):
            assert row['Port '+port+' Depth'] == '16', 'register-file depth differs'
            assert row['Port '+port+' Width'] == str(width), 'register-file width differs'
    assert set(copies) == set(expected), 'missing register-file RAM package'
    assert all(x == {'mem_rtl_0', 'mem__dual_rtl_0'} for x in copies.values()), 'missing read replica'
    return dict(register_files=len(copies), inferred_read_replicas=2*len(copies),
                logical_ram_bits=64*len(copies),
                replica_ram_bits=sum(16*(4+len(r['connected_normal_outputs'])) for r in expected.values()),
                boards=dict(sorted(Counter(k.split('/')[0] for k in copies).items())))


def controls(rows, expected):
    target = next(i for i, r in enumerate(rows) if 'cell_F10145A_snapshot:' in r['Name'])
    results = []
    for name in ('missing_replica', 'duplicate_replica', 'wrong_kind', 'wrong_width', 'wrong_depth', 'initialized_ram', 'wrong_package'):
        bad = copy.deepcopy(rows)
        if name == 'missing_replica':
            bad.pop(target)
        elif name == 'duplicate_replica':
            bad.append(copy.deepcopy(bad[target]))
        elif name == 'wrong_package':
            bad[target]['Name'] = re.sub(r'cell_F10145A_snapshot:u_\w+',
                                         'cell_F10145A_snapshot:u_bad', bad[target]['Name'])
        else:
            field, value = dict(wrong_kind=('Type', 'M10K'), wrong_width=('Port B Width', '3'),
                                wrong_depth=('Port A Depth', '15'), initialized_ram=('MIF', 'invented.mif'))[name]
            bad[target][field] = value
        try:
            inspect_rows(bad, expected)
        except AssertionError as e:
            results.append(dict(case=name, rejected=True, diagnostic=str(e)))
        else:
            raise AssertionError('damaged register-file inference accepted: '+name)
    return results


def run(source, mapping, output):
    expected, inputs = expected_files(source)
    rows = table(mapping.read_text(encoding='latin1'), 'Analysis & Synthesis RAM Summary', 'Name')
    result = inspect_rows(rows, expected)
    result.update(status='PASS_SYNTHESIS_MAPPING_ONLY', scope=__doc__,
                  expected_packages=expected, failure_controls=controls(rows, expected),
                  input_hashes=inputs | {str(mapping): sha(mapping), str(Path(__file__)): sha(Path(__file__))})
    output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"F10145 INFERENCE PASS: {result['register_files']} files, "
          f"{result['inferred_read_replicas']} MLAB replicas;7bad mappings reject;not physical qualification")


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root', type=Path, required=True)
    p.add_argument('--map', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.source_root.resolve(), a.map.resolve(), a.output.resolve())
