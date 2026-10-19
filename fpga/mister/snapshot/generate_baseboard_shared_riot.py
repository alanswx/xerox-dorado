#!/usr/bin/env python3
"""Complete qualified BaseBoard with only its RIOT RAM-port implementation folded."""
import copy
import hashlib
import generate_baseboard_full as qualified
import generate_riot_shared_port as shared


def __getattr__(name):
    return getattr(qualified, name)


def generate(root):
    board, manifest, clones = qualified.generate(root)
    manifest = copy.deepcopy(manifest)
    text, child = shared.generate()
    old = clones['M6532_snapshot.sv']
    assert hashlib.sha256(old.encode()).hexdigest() == child['qualified_predecessor_sha256']
    clones['M6532_snapshot.sv'] = text
    manifest['cloned_sources']['M6532_snapshot.sv'] = child['generated_sha256']
    for package in manifest['packages']:
        if package['type'] == 'RIOT':
            package['schema']['child_manifest'] = child
            package['schema']['child_sha256'] = child['generated_sha256']
    manifest['area_experiment'] = __doc__
    assert manifest['logical_state_bits'] == 40366
    return board, manifest, clones
