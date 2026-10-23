#!/usr/bin/env python3
"""Assemble one fully reviewed old-word/early-response fit; never refit or program."""
import argparse
from pathlib import Path

from assemble_timing_paths_native import (
    OLD_WORD_GUARD_CASES as GUARD_CASES,
    run as shared_run,
    validate as shared_validate,
)


def validate(project):
    return shared_validate(project, old_word=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', type=Path, required=True)
    parser.add_argument('--quartus', type=Path,
                        default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    parser.add_argument('--check-only', action='store_true')
    args = parser.parse_args()
    if args.check_only:
        validate(args.project.resolve())
        print('PASS_NATIVE_OLD_WORD_EARLY_ASSEMBLY_INPUT_GUARD; no Quartus call')
    else:
        shared_run(args.project.resolve(), args.quartus, old_word=True)
