#!/usr/bin/env python3
"""Inspect all444 native read-capture categories; never grant assembly permission."""
import argparse
import json
from pathlib import Path

from inspect_timing_paths_native import inspect


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'parent', 'qualification', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    assert not args.output.exists()
    result = inspect(args.root.resolve(), args.parent.resolve(), args.qualification.resolve(), read_word=True)
    result.update(scope=__doc__, capture_read_word=True,
        status='REJECT_NATIVE_READ_WORD_CAPTURE' if result['status'] == 'REJECT_NATIVE_TIMING_PATHS'
               else 'READ_WORD_CAPTURE_TIMING_POSITIVE_FULL_PREASSEMBLY_REVIEW_STILL_REQUIRED')
    recipe = json.loads((args.root / 'timing-paths-native.json').read_text())
    if recipe.get('ALU_direct_carry', False):
        result.update(ALU_direct_carry=True,
            status='REJECT_NATIVE_DIRECT_CARRY_CAPTURE' if result['status'] == 'REJECT_NATIVE_READ_WORD_CAPTURE'
                   else 'DIRECT_CARRY_CAPTURE_TIMING_POSITIVE_FULL_PREASSEMBLY_REVIEW_STILL_REQUIRED')
    if recipe.get('decoded_mux', False):
        result.update(decoded_mux=True,
            status='REJECT_NATIVE_DECODED_MUX' if result['status'] == 'REJECT_NATIVE_DIRECT_CARRY_CAPTURE'
                   else 'DECODED_MUX_TIMING_POSITIVE_FULL_PREASSEMBLY_REVIEW_STILL_REQUIRED')
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(result['detailed_paths']), 'categories; setup',
          result['minimum_setup_ns'], 'hold', result['minimum_hold_ns'])
