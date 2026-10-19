#!/usr/bin/env python3
"""Prepare matched isolated full-state ContB fits, preserving the IM-only fit.

Both projects keep the same host clock, registered I/O budgets and original
baseline. The complete candidate replaces all sequential cells with exact-
enable host-clock adapters. No production board or FPGA is changed.
"""
import argparse
import hashlib
import json
from pathlib import Path

import fit_contb_snapshot as base
import generate_contb_full as gen
from test_contb_full import fixture

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
SDC = base.SDC
AUDIT = base.AUDIT


def top(ports):
    text = base.top(ports)
    for before,after in (
        ('ContB_snapshot_experiment #',gen.MODULE+' #'),
        ('input wire [13:0] state_index,','input wire [14:0] state_index,'),
        ('reg [35:0] admin;','reg [36:0] admin;'),
        ('wire [13:0] index_r;','wire [14:0] index_r;'),
    ):
        text = gen.replace_once(text,before,after)
    return text


def prepare(out):
    base.prepare(out)
    board,manifest,clones=gen.generate()
    _,_,ports=fixture(manifest)
    provenance=json.loads((out/'provenance.json').read_text())
    for p in (Path(__file__),HERE/'generate_contb_full.py',HERE/'test_contb_full.py'):
        provenance['source_hashes'][str(p.relative_to(ROOT))]=hashlib.sha256(p.read_bytes()).hexdigest()
    provenance['manifest']=manifest
    provenance['scope']='Full modeled isolated ContB state access; original baseline versus exact-advance host-clock candidate;50MHz/registered shell/2ns max0ns min I/O; no whole-machine timing or FPGA programming'
    (out/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
    for variant in ('baseline','snapshot'):
        case=out/variant
        (case/'ContB_snapshot.sv').write_text(board)
        (case/'top.sv').write_text(top(ports))
        qsf=(case/'snapshot_fit.qsf').read_text()
        for name,source in clones.items():
            (case/name).write_text(source)
            qsf+=f'set_global_assignment -name SYSTEMVERILOG_FILE {name}\n'
        (case/'snapshot_fit.qsf').write_text(qsf)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare',type=Path,required=True)
    args=parser.parse_args()
    prepare(args.prepare.resolve())
