#!/usr/bin/env python3
"""Add illustrative MC1668 arrival delay to the original R operand graph.

Inherited parity-arc controls remain targeted correlated experiments; the new
R latches/flags use the ordinary illustrative FF delay. No board timing claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
from stage_t_parity_timing import stage as stage_parity


def stage(inputs,reference,out,gate_ps,ff_ps,phase_ps,parc=None):
    record=stage_parity(inputs,reference,out,gate_ps,ff_ps,phase_ps,parc)
    name='r_operand_original_ff.sv';text=(inputs/'verilog/verilator'/name).read_text()
    assert text.count('parameter integer DELAY_PS=1000')==1
    (out/name).write_text(text.replace('parameter integer DELAY_PS=1000','parameter integer DELAY_PS='+str(ff_ps)))
    record.update(r_operand_scope=__doc__,mc1668_clock_q_ps=ff_ps)
    record['files']={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in out.iterdir()if p.suffix in('.sv','.v')}
    (out/'timing-profile.json').write_text(json.dumps(record,indent=2)+'\n')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inputs',type=Path,required=True)
    p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--gate-ps',type=int,default=1000);p.add_argument('--ff-ps',type=int,default=1000)
    p.add_argument('--phase-ps',type=int,default=1000);p.add_argument('--parc',choices=['min','max']);a=p.parse_args()
    stage(a.inputs.resolve(),a.reference.resolve(),a.output.resolve(),a.gate_ps,a.ff_ps,a.phase_ps,a.parc)
