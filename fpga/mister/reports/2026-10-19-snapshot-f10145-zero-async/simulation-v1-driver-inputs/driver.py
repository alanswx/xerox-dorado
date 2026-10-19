#!/usr/bin/env python3
"""Original/state oracle plus explicit zero-write-reply checks for the candidate."""
import argparse
import hashlib
import json
from pathlib import Path
import generate_f10145_zero_write as gen
import test_f10145_shared_write as qualified


def run(out):
    side = out.with_name(out.name+'-driver-inputs')
    side.mkdir(parents=True, exist_ok=False)
    original = Path(__file__).parent/'test_f10145_snapshot.cpp'
    text = original.read_text()
    assert text.count('    return d.result;') == 1
    changed = text.replace('    return d.result;',
                           '    if (write) check(d.result==0,"private write result not zero");\n    return d.result;', 1)
    cpp = side/'test_f10145_snapshot.cpp'
    cpp.write_text(changed)
    (side/'original-test.cpp').write_bytes(original.read_bytes())
    (side/'driver.py').write_bytes(Path(__file__).read_bytes())
    (side/'provenance.json').write_text(json.dumps(dict(scope=__doc__,
        original_cpp_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),
        derived_cpp_sha256=hashlib.sha256(cpp.read_bytes()).hexdigest(),
        driver_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()), indent=2)+'\n')
    extra = [('lost_zero_reply', 'state_write ? 10\'b0 : ', '', 'private write result not zero')]
    qualified.run(out, generator=gen, cpp_override=cpp, extra_cases=extra)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
