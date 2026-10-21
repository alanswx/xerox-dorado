#!/usr/bin/env python3
"""Replay the map comparisons using only the receipt's frozen files."""
import argparse
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile


def main(output):
    receipt = Path(__file__).resolve().parent
    manifest = json.loads(gzip.decompress((receipt/'manifest.json.gz').read_bytes()))
    versions = {'storage-address': (18, 'map-comparison-final.json'),
                'map-phase': (19, 'map-capture-comparison.json')}
    names = {f'r-front-ready-{kind}-{arm}-v{version}-20261021'
             for kind, (version, _) in versions.items() for arm in ('control', 'candidate')}
    extracted = 0
    with tempfile.TemporaryDirectory(prefix='dorado-map-replay-') as scratch:
        root = Path(scratch)
        for member in manifest['members']:
            name = Path(member['name'])
            selected = (name.parts[:1] == ('simulation',) and len(name.parts) > 2
                        and name.parts[1] in names)
            if not selected and member['name'] != 'tools/inspect_r_front_map_capture.py':
                continue
            assert not name.is_absolute() and '..' not in name.parts
            blob = manifest['blobs'][member['blob']]
            stored = (receipt/blob['stored']).read_bytes()
            assert hashlib.sha256(stored).hexdigest() == blob['stored_sha256']
            raw = gzip.decompress(stored)
            assert hashlib.sha256(raw).hexdigest() == member['blob']
            assert len(raw) == blob['raw_bytes']
            # The receipt shortens the frozen-directory name to "inputs".
            # Restore the runner's original layout for the frozen analyzer.
            target_name = Path(*name.parts[:2], 'executed-inputs', *name.parts[3:]) \
                if selected and name.parts[2] == 'inputs' else name
            target = root/target_name
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(raw)
            extracted += 1
        spec = importlib.util.spec_from_file_location('frozen_map_analysis', root/'tools/inspect_r_front_map_capture.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        results = {}
        for kind, (version, filename) in versions.items():
            runs = [root/'simulation'/f'r-front-ready-{kind}-{arm}-v{version}-20261021'
                    for arm in ('control', 'candidate')]
            result = module.inspect(*runs)
            expected = json.loads((runs[1]/filename).read_text())
            for key in expected:
                if key != 'evidence_sha256':
                    assert result[key] == expected[key], (kind, key)
            # Extraction changes path prefixes, but every consumed byte must
            # match the originally analyzed evidence, including duplicates.
            assert sorted(result['evidence_sha256'].values()) == sorted(expected['evidence_sha256'].values())
            results[kind] = dict(RM_mismatches=result['RM_mismatches'],
                map_row_mismatches=result['map_row_mismatches'],
                analysis_identical=True, consumed_evidence_hashes_identical=True)
    result = dict(status='PASS_FROZEN_MAP_ANALYSIS_REPLAY', extracted_members=extracted,
                  comparisons=results, hardware_qualified=False, speedup_qualified=False)
    if output:
        with output.open('x') as stream:
            json.dump(result, stream, indent=2)
            stream.write('\n')
    print(json.dumps(result))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    main(parser.parse_args().output)
