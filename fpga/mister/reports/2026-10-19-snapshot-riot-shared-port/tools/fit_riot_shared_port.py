#!/usr/bin/env python3
"""Matched qualified-enabled versus shared-port RIOT area/timing experiment."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import fit_riot as qualified
import generate_riot as previous
import generate_riot_shared_port as candidate


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(output):
    qualified.prepare(output)
    reference, old_manifest = previous.generate()
    changed, manifest = candidate.generate()
    for variant, source in [('baseline', reference), ('snapshot', changed)]:
        case = output/variant
        (case/'M6532_snapshot.sv').write_text(source)
        qsf = case/'snapshot_fit.qsf'
        text = qsf.read_text()
        macro = 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        if variant == 'baseline':
            assert macro not in text
            qsf.write_text(text+macro)
        else:
            assert text.count(macro) == 1
    provenance = output/'provenance.json'
    record = json.loads(provenance.read_text())
    for name in ('generate_riot_shared_port.py', 'fit_riot_shared_port.py'):
        source = Path(__file__).parent/name
        shutil.copyfile(source, output/'executed-sources'/name)
        record['source_hashes'][str(source.relative_to(candidate.ROOT))] = digest(source)
    record.update(scope=__doc__+' Both variants expose all1097bits with the same ABI and enabled administrative ports. Same50MHz launch/capture/constraints. Baseline is the qualified snapshot adapter, not the original vendor-only part. No FPGA or full-core area/timing qualification.',
                  manifest=manifest, reference_manifest=old_manifest,
                  experiment_builder_sha256=digest(Path(__file__)),
                  expected_qsf={v: (output/v/'snapshot_fit.qsf').read_text() for v in ('baseline', 'snapshot')})
    record['compiled_files'] = {str(p.relative_to(output)): digest(p)
                                for p in output.glob('*/*') if p.is_file()}
    provenance.write_text(json.dumps(record, indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--prepare', type=Path)
    p.add_argument('--run', type=Path)
    p.add_argument('--audit', type=Path)
    p.add_argument('--output', type=Path)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare:
        prepare(a.prepare.resolve())
    elif a.run:
        record = json.loads((a.run/'provenance.json').read_text())
        assert digest(Path(__file__)) == record['experiment_builder_sha256']
        qualified.run(a.run.resolve(), a.quartus)
    elif a.audit and a.output:
        result = qualified.audit(a.audit.resolve())
        a.output.write_text(json.dumps(result, indent=2)+'\n')
        print(json.dumps(result, indent=2))
    else:
        p.error('Specify --prepare, --run, or --audit/--output')
