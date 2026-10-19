#!/usr/bin/env python3
"""Read-only routed netlist6502 audit, including every exported node register."""
import argparse
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_6502 as fit

SHA='8ae05dff81a1c2971e03d2e01c4c3cb09b4f111c40e54b7e9e437f1d8add46da'


def audit(root):
    q=fit.audit(root);p=json.loads((root/'provenance.json').read_text());m=p['manifest']
    assert m==json.loads((root/'qualified-manifest.json').read_text()),'qualified simulation schema differs'
    assert m['original_sha256']==SHA and m['logical_state_bits']==1725
    assert m['fields']==[dict(name='q',width=1725,lsb=0)]
    assert m['words']==[dict(index=i,lsb=18*i,width=min(18,1725-18*i))for i in range(96)]
    assert m['valid_indices']==list(range(96))and m['invalid_indices']==list(range(96,128))
    assert fit.digest(root/'executed-sources/fit_6502.py')==p['builder_sha256'],'executed builder differs'
    retained={}
    for variant,kind,wanted in [('baseline','chip_6502',767),('snapshot','chip_6502_snapshot',1746)]:
        d=root/variant;assert fit.digest(d/'chip_6502.v')==SHA,'original vendor bytes differ'
        assert fit.digest(d/'chip_6502_snapshot.sv')==m['generated_sha256'],'qualified node adapter differs'
        text=(d/'snapshot_fit.map.rpt').read_text(errors='replace')
        start=text.find('; Analysis & Synthesis Resource Utilization by Entity');assert start>=0
        rows=[]
        for line in text[start:].split('\n\n',1)[0].splitlines():
            fields=[s.strip()for s in line.split(';')[1:-1]]
            if fields and fields[0]=='|'+kind+':cell_inst|':rows.append(fields)
        assert len(rows)==1 and rows[0][2]==f'{wanted} ({wanted})'and rows[0][3]=='0','node-state register retention differs'
        assert '; Fitter RAM Summary'not in(d/'snapshot_fit.fit.rpt').read_text(errors='replace'),'unexpected RAM placement'
        retained[variant]=dict(synthesis_registers=wanted,block_memory_bits=0,
            meaning='1725node registers plus18response/3handshake registers'if variant=='snapshot'else'767normal-state registers after original netlist constant/alias pruning')
    q['state_retention']=retained
    q['limits']='Isolated enabled CPU adapter/shell. Synthesis entity counts checked; Fitter totals include duplication/shell. No full BaseBoard/global FPGA save/load or speed qualification.'
    return q


def controls(root):
    rows=fit.controls(root)
    for name in ('node_register','missing_corner','schema'):
        with tempfile.TemporaryDirectory(prefix='dorado-netlist6502-state-',dir='/tmp')as tmp:
            d=Path(tmp)/'evidence';shutil.copytree(root,d,ignore=shutil.ignore_patterns('db','incremental_db'))
            if name=='node_register':
                p=d/'snapshot/snapshot_fit.map.rpt';text=p.read_text();lines=text.splitlines(keepends=True)
                for i,line in enumerate(lines):
                    if '|chip_6502_snapshot:cell_inst|'in line and '1746 (1746)'in line:
                        lines[i]=line.replace('1746 (1746)','1745 (1745)',1);break
                else:raise AssertionError('missing node register failure anchor')
                p.write_text(''.join(lines))
            elif name=='missing_corner':(d/'snapshot/bank-setup-7_slow_1100mv_100c.rpt').unlink()
            else:
                p=d/'qualified-manifest.json';m=json.loads(p.read_text());m['words'][1]['lsb']=0;p.write_text(json.dumps(m))
            try:audit(d)
            except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged netlist6502 evidence accepted:'+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();q=audit(a.directory.resolve())
    if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
