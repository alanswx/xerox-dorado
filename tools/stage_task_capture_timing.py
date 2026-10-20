#!/usr/bin/env python3
"""Stage explicit-delay original cells for task-pipeline ordering experiments.

Zero-delay event simulators can order the same clock/data feedback differently.
These deterministic delay profiles expose arrival order without treating delta
cycles as physical time. Delays are illustrative, not vendor characterization,
board-skew limits, an analog proof or 60ns operating qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re


def stage(inputs, reference, out, gate_ps, ff_ps, phase_ps):
    assert min(gate_ps, ff_ps, phase_ps) > 0
    out.mkdir(parents=True, exist_ok=False)
    manifest = json.loads((reference / 'manifest.json').read_text())
    cells = {'verilog/cells/cell_MC10210.v'}
    for board in manifest['boards']:
        cells.update(board['combinational_sources'])
    sources = {}
    for name in sorted(cells):
        raw = (inputs / name).read_text()
        raw = re.sub(r'`timescale[^\n]*\n', '', raw)
        raw, count = re.subn(r'\bassign\s+(p\d+)\b',
                             lambda m: 'assign #(' + str(gate_ps) + ') ' + m[1], raw)
        assert count, name
        sources[Path(name).name] = '`timescale 1ps/1ps\n' + raw
    for name in ['run_control_original_ff.sv', 'processor_capture_original_ff.sv']:
        raw = (inputs / 'verilog/verilator' / name).read_text()
        raw = re.sub(r'`timescale[^\n]*\n', '', raw)
        # Preserve the independently written FF functions. Only clock-to-Q
        # arrival changes. SE10231 supplies the processor phase pair.
        chunks = re.split(r'(module original_SE10231\b.*?endmodule)', raw, flags=re.S)
        for i, text in enumerate(chunks):
            delay = phase_ps if text.startswith('module original_SE10231') else ff_ps
            chunks[i] = text.replace('<=', '<=#(' + str(delay) + ') ')
        sources[name] = '`timescale 1ps/1ps\n' + ''.join(chunks)
    for path in reference.glob('*.sv'):
        sources[path.name] = path.read_text()
    for name, raw in sources.items():
        (out / name).write_text(raw)
    record = dict(scope=__doc__, gate_delay_ps=gate_ps, ff_delay_ps=ff_ps,
                  processor_phase_delay_ps=phase_ps,
                  files={name: hashlib.sha256((out / name).read_bytes()).hexdigest()
                         for name in sorted(sources)},
                  physical_timing_qualified=False)
    (out / 'timing-profile.json').write_text(json.dumps(record, indent=2) + '\n')
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inputs', type=Path, required=True)
    parser.add_argument('--reference', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--gate-ps', type=int, default=1000)
    parser.add_argument('--ff-ps', type=int, default=1000)
    parser.add_argument('--phase-ps', type=int, default=1000)
    args = parser.parse_args()
    stage(args.inputs.resolve(), args.reference.resolve(), args.output.resolve(),
          args.gate_ps, args.ff_ps, args.phase_ps)
