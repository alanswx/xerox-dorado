#!/usr/bin/env python3
"""Read-only routed RIOT audit, including all RAM/control/history registers."""
import argparse
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_riot as fit

SHA='43c9d1670fcbb98ad4681883981930817e2882a5ab32790f8285961c3ae0c1b1'


def audit(root):
    q=fit.audit(root);p=json.loads((root/'provenance.json').read_text());m=p['manifest']
    assert m==json.loads((root/'qualified-manifest.json').read_text()),'qualified simulation schema differs'
    assert m['original_sha256']==SHA and m['logical_state_bits']==1097
    assert m['parameters']==dict(init_7800=0)and m['ram_bits']==1024 and m['control_bits']==73
    fields=[('d_out',8),('out_a',8),('out_b',8),('dir_a',8),('dir_b',8),('interrupt',8),
            ('timer',8),('prescaler',10),('incr',2),('irq_en',2),('edge_detect',1),('rollover',1),('old_pa7',1)]
    expected=[];lsb=0
    for name,width in fields:expected.append(dict(name=name,width=width,lsb=lsb));lsb+=width
    assert lsb==73 and m['fields']==expected
    assert m['words']==[dict(index=i,lsb=8*i,width=8,kind='RAM')for i in range(128)]+[
        dict(index=128+i,lsb=1024+18*i,width=18 if i<4 else 1,kind='CONTROL')for i in range(5)]
    assert m['valid_indices']==list(range(133))and m['invalid_indices']==list(range(133,256))
    assert fit.digest(root/'executed-sources/fit_riot.py')==p['builder_sha256'],'executed builder differs'
    retained={}
    for variant,kind,wanted in [('baseline','M6532',1090),('snapshot','M6532_snapshot',1118)]:
        d=root/variant;assert fit.digest(d/'M6532.sv')==SHA,'original vendor bytes differ'
        assert fit.digest(d/'M6532_snapshot.sv')==m['generated_sha256'],'qualified node adapter differs'
        text=(d/'snapshot_fit.map.rpt').read_text(errors='replace')
        start=text.find('; Analysis & Synthesis Resource Utilization by Entity');assert start>=0
        rows=[]
        for line in text[start:].split('\n\n',1)[0].splitlines():
            fields=[s.strip()for s in line.split(';')[1:-1]]
            if fields and fields[0]=='|'+kind+':cell_inst|':rows.append(fields)
        assert len(rows)==1 and rows[0][2]==f'{wanted} ({wanted})'and rows[0][3]=='0','RIOT state register retention differs'
        assert '; Fitter RAM Summary'not in(d/'snapshot_fit.fit.rpt').read_text(errors='replace'),'unexpected RAM placement'
        retained[variant]=dict(synthesis_registers=wanted,block_memory_bits=0,
            meaning='1097RAM/control/history bits plus18response/3handshake registers'if variant=='snapshot'else'1090normal-state registers after original netlist constant/alias pruning')
    q['state_retention']=retained
    q['limits']='Isolated enabled CPU adapter/shell. Synthesis entity counts checked; Fitter totals include duplication/shell. No enclosing wrapper phase_d/full BaseBoard/global FPGA save/load or speed qualification.'
    return q


def controls(root):
    rows=fit.controls(root)
    for name in ('node_register','missing_corner','schema'):
        with tempfile.TemporaryDirectory(prefix='dorado-RIOT-state-',dir='/tmp')as tmp:
            d=Path(tmp)/'evidence';shutil.copytree(root,d,ignore=shutil.ignore_patterns('db','incremental_db'))
            if name=='node_register':
                p=d/'snapshot/snapshot_fit.map.rpt';text=p.read_text();lines=text.splitlines(keepends=True)
                for i,line in enumerate(lines):
                    if '|M6532_snapshot:cell_inst|'in line and '1118 (1118)'in line:
                        lines[i]=line.replace('1118 (1118)','1117 (1117)',1);break
                else:raise AssertionError('missing node register failure anchor')
                p.write_text(''.join(lines))
            elif name=='missing_corner':(d/'snapshot/bank-setup-7_slow_1100mv_100c.rpt').unlink()
            else:
                p=d/'qualified-manifest.json';m=json.loads(p.read_text());m['words'][1]['lsb']=0;p.write_text(json.dumps(m))
            try:audit(d)
            except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged RIOT evidence accepted:'+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();q=audit(a.directory.resolve())
    if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
