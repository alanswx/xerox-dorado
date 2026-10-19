#!/usr/bin/env python3
"""Matched shared-write logic/MLAB placement, same enabled ABI and constraints."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import fit_riot


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(out):
    import fit_f10145_shared_write as previous
    import generate_f10145_shared_write as shared
    import generate_f10145_mlab as mlab
    previous.prepare(out)
    reference, old_manifest = shared.generate()
    changed, new_manifest = mlab.generate()
    semantic = []
    for variant, source in [('baseline', reference), ('snapshot', changed)]:
        case = out/variant
        path = case/'cell_F10145A_snapshot.sv'
        path.write_text(source)
        result = subprocess.run(['verilator', '-E', '-DDORADO_F10145_LOGIC', str(path)],
                                capture_output=True, text=True, check=True)
        (case/'semantic-preprocess.sv').write_text(result.stdout)
        text = re.sub(r'^`line[^\n]*', '', result.stdout, flags=re.M)
        text = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S)
        hints = re.findall(r'\(\*\s*ramstyle\s*=\s*"([^"]+)"\s*\*\)', text)
        assert hints == (['logic'] if variant == 'baseline' else ['MLAB'])
        text = re.sub(r'\(\*\s*ramstyle\s*=\s*"(?:logic|MLAB)"\s*\*\)', '', text)
        semantic.append(re.sub(r'\s+', '', text))
    assert semantic[0] == semantic[1], 'functional RTL changed beyond the RAM hint'
    p = out/'provenance.json'
    record = json.loads(p.read_text())
    record.update(manifest=new_manifest, reference_manifest=old_manifest,
                  scope=__doc__+' Physical-placement experiment only. Strict collision semantics, '
                  'unchanged50MHz opposite-edge shell/SDC and full74bit ABI. No global snapshot or deployment.',
                  experiment_builder_sha256=digest(Path(__file__)),
                  hint_only_semantic_sha256=hashlib.sha256(semantic[0].encode()).hexdigest())
    for source in (Path(__file__).resolve(), Path(mlab.__file__)):
        shutil.copyfile(source, out/'executed-sources'/source.name)
        record['source_hashes'][str(source.relative_to(shared.ROOT))] = digest(source)
    record['compiled_files'] = {str(path.relative_to(out)): digest(path)
                                for path in out.glob('*/*') if path.is_file()}
    p.write_text(json.dumps(record, indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--prepare', type=Path)
    p.add_argument('--run', type=Path)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare:
        prepare(a.prepare.resolve())
    elif a.run:
        record = json.loads((a.run/'provenance.json').read_text())
        assert digest(Path(__file__)) == record['experiment_builder_sha256']
        fit_riot.run(a.run.resolve(), a.quartus)
    else:
        p.error('Specify --prepare or --run')
