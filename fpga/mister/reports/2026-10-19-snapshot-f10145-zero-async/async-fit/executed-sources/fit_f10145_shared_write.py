#!/usr/bin/env python3
"""Matched enabled F10145 write-port folding, preserving original constraints."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import fit_riot


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(out):
    import fit_f10145 as qualified
    import generate_f10145_shared_write as gen
    qualified.prepare(out)
    source, manifest = gen.generate()
    (out/'snapshot/cell_F10145A_snapshot.sv').write_text(source)
    qsf = out/'baseline/snapshot_fit.qsf'
    macro = 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
    assert macro not in qsf.read_text()
    qsf.write_text(qsf.read_text()+macro)
    p = out/'provenance.json'
    r = json.loads(p.read_text())
    r['reference_manifest'] = r['manifest']
    r['manifest'] = manifest
    r['scope'] = (__doc__+' Both variants expose the same74physical bits/17words; baseline is the '
                  'qualified enabled adapter. Same50MHz opposite-edge shell/10ns embedded paths, '
                  '2ns/0ns virtual IO, logic RAM. Not a full-core area/timing qualification.')
    r['builder_sha256'] = digest(Path(fit_riot.__file__))
    r['experiment_builder_sha256'] = digest(Path(__file__))
    r['expected_qsf'] = {v: (out/v/'snapshot_fit.qsf').read_text() for v in ('baseline', 'snapshot')}
    executed = out/'executed-sources'
    executed.mkdir()
    paths = [gen.ROOT/n for n in r['source_hashes']]
    paths += [Path(__file__).resolve(), Path(fit_riot.__file__), Path(gen.__file__)]
    for path in paths:
        shutil.copyfile(path, executed/path.name)
        r['source_hashes'][str(path.relative_to(gen.ROOT))] = digest(path)
    r['compiled_files'] = {str(path.relative_to(out)): digest(path)
                           for path in out.glob('*/*') if path.is_file()}
    p.write_text(json.dumps(r, indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--prepare', type=Path)
    p.add_argument('--run', type=Path)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare:
        prepare(a.prepare.resolve())
    elif a.run:
        r = json.loads((a.run/'provenance.json').read_text())
        assert digest(Path(__file__)) == r['experiment_builder_sha256']
        fit_riot.run(a.run.resolve(), a.quartus)
    else:
        p.error('Specify --prepare or --run')
