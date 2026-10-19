#!/usr/bin/env python3
"""Read-only F10470 fit audit, including actual physical OLD_DATA memories."""
import argparse
import json
from pathlib import Path
import re
import shutil
import tempfile

import fit_f10470_word as base


def physical(text):
    start=text.find('; Fitter RAM Summary');assert start>=0,'missing physical RAM table'
    table=text[start:].split('\n\n',1)[0];header=None;rows=[]
    for line in table.splitlines():
        if not line.startswith('; '):continue
        f=[x.strip()for x in line.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and re.search(r'\bbank_[0-5]\|',f[0]):
            assert header and len(header)==len(f),'bad physical row'
            rows.append(dict(zip(header,f)))
    assert len(rows)==6,'physical bank count differs';seen=set()
    for row in rows:
        bank=int(re.search(r'\bbank_([0-5])\|',row['Name'])[1]);assert bank not in seen,'duplicate physical bank'
        seen.add(bank);width=base.WIDTHS[bank]
        want={'Type':'M10K block','Mode':'Simple Dual Port','Port A Depth':'4096','Port B Depth':'4096',
              'Port A Width':str(width),'Port B Width':str(width),'Size':str(4096*width),
              'Implementation Bits':str(4096*width),'M10K blocks':str((width+1)//2),'MLAB cells':'0',
              'Clock Mode':'Single Clock','Port A Input Registers':'yes','Port B Input Registers':'yes',
              'Port A Output Registers':'no','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}
        for key,value in want.items():assert row.get(key)==value,('physical F10470 RAM differs',bank,key,row.get(key))
    assert seen==set(range(6));return rows


def audit(root):
    q=base.audit(root)
    q['snapshot_physical_memories']=physical((root/'snapshot/snapshot_fit.fit.rpt').read_text(errors='replace'))
    return q


def controls(root):
    result=[]
    for name,path,before,after in [
        ('source','snapshot/f10470_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 23','Total RAM Blocks : 24'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
        ('physical_old_data','snapshot/snapshot_fit.fit.rpt','; Old data','; New data'),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-f10470-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(root,out);p=out/path;text=p.read_text(errors='replace')
            if before:assert before in text,'missing control anchor:'+name;text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed inference\n'
            p.write_text(text)
            try:audit(out)
            except AssertionError as e:result.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged F10470 evidence accepted:'+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();q=audit(a.directory.resolve())
    if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
