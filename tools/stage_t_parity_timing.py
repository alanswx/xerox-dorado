#!/usr/bin/env python3
"""Stage the parity reference, including PARC-annotated parity-arc controls.

The inherited clock/data graph retains its illustrative delays. Optional PARC
min/max controls apply only to F10000 clock-to-Q, MC10170 A and SRAM read
arrival. They are correlated experiments, not independent-skew or board timing
qualification. One conservative SRAM output delay models every read update.
"""
import argparse
import hashlib
import json
from pathlib import Path

from stage_t_file_timing import stage as stage_t


def stage(inputs,reference,out,gate_ps,ff_ps,phase_ps,parc=None):
    record=stage_t(inputs,reference,out,gate_ps,ff_ps,phase_ps)
    bounds={'min':(1000,1000,2000),'max':(5500,6600,9900)}
    qdelay,pdelay,rdelay=bounds[parc]if parc else(ff_ps,gate_ps,0)
    name='t_parity_original_ff.sv';text=(inputs/'verilog/verilator'/name).read_text()
    assert text.count('parameter integer DELAY_PS=1000')==1
    (out/name).write_text(text.replace('parameter integer DELAY_PS=1000','parameter integer DELAY_PS='+str(qdelay)))
    if parc:
        dictionary=(inputs/'chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze').read_text()
        assert 'CLK (13 4) (1 5.5)'in dictionary and 'ADDR (6 7 9 10) (2.0 9.9'in dictionary
        assert '>2 : 1 6.6 ]'in dictionary
        p=out/'cell_MC10170.v';text=p.read_text()
        assert text.count(f'assign #({gate_ps}) p2')==1
        p.write_text(text.replace(f'assign #({gate_ps}) p2',f'assign #({pdelay}) p2'))
        p=out/'t_file_original_storage.sv';text=p.read_text()
        assert text.count('assign {p14,p15,p1,p2}=')==1
        p.write_text(text.replace('assign {p14,p15,p1,p2}=',f'assign #({rdelay}) {{p14,p15,p1,p2}}='))
    record.update(parity_scope=__doc__,parc_parity_control=parc,f10000_clock_q_ps=qdelay,
                  mc10170_a_ps=pdelay,sram_read_ps=rdelay,
                  source_setup_hold_ns=dict(F10000_D11=(1.7,1.1)))
    record['files']={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in out.iterdir()if p.suffix in('.sv','.v')}
    (out/'timing-profile.json').write_text(json.dumps(record,indent=2)+'\n')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inputs',type=Path,required=True)
    p.add_argument('--reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--gate-ps',type=int,default=1000);p.add_argument('--ff-ps',type=int,default=1000)
    p.add_argument('--phase-ps',type=int,default=1000);p.add_argument('--parc',choices=['min','max']);a=p.parse_args()
    stage(a.inputs.resolve(),a.reference.resolve(),a.output.resolve(),a.gate_ps,a.ff_ps,a.phase_ps,a.parc)
