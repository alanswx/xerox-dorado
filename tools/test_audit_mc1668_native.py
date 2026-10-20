#!/usr/bin/env python3
"""Challenge terminal native review with source, memory and timing damage.

No Quartus or hardware access. Controls qualify this evidence reader, not the
firmware or FPGA. Source changes remain rejected after metadata is rehashed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
from audit_mc1668_native import audit,CELL


def run(source,qualified,initial,out):
    passing=audit(source,qualified,initial);assert passing['status']=='PASS_NATIVE_MC1668_BUILD'
    out.mkdir(parents=True,exist_ok=False);rows=[]
    cases=('sdc_rehashed','cell_rehashed','missing_detail','negative_detail','wrong_old_data','incomplete_source_coverage')
    for name in cases:
        root=out/name;shutil.copytree(source,root)
        if name in('sdc_rehashed','cell_rehashed'):
            path='core/native_timing.sdc'if name=='sdc_rehashed'else CELL
            p=root/path;p.write_bytes(p.read_bytes()+b'\n// altered after qualification\n'if name=='cell_rehashed'else p.read_bytes()+b'\nset_false_path -from [get_registers *]\n')
            digest=hashlib.sha256(p.read_bytes()).hexdigest()
            for file,key in(('mc1668-candidate.json','source_hashes'),('collected-input-check.json','actual')):
                p=root/file;r=json.loads(p.read_text());r[key][path]=digest;p.write_text(json.dumps(r,indent=2)+'\n')
        elif name in('missing_detail','negative_detail'):
            p=root/'core/native59-proch-in-hold-7_slow_1100mv_100c.rpt'
            if name=='missing_detail':p.unlink()
            else:
                text,n=re.subn(r'(Worst case slack is\s*)[-+\d.]+',r'\g<1>-1.000',p.read_text());assert n==1;p.write_text(text)
        elif name=='wrong_old_data':
            p=root/'core/output_files/Dorado.map.rpt';text=p.read_text()
            headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for User Entity Instance:\s*([^\n;]+)',text))
            hits=0
            for i,h in enumerate(headers):
                if ':b_ContB|'not in h[1]or'snapshot_bank_0|'not in h[1]or'altsyncram:'not in h[1]:continue
                end=headers[i+1].start()if i+1<len(headers)else len(text)
                section,n=re.subn(r'(; READ_DURING_WRITE_MODE_MIXED_PORTS\s*; )OLD_DATA',r'\g<1>NEW_DATA',text[h.start():end]);assert n==1
                text=text[:h.start()]+section+text[end:];hits+=1;break
            assert hits==1;p.write_text(text)
        else:
            p=root/'mc1668-candidate.json';r=json.loads(p.read_text());r['source_hashes'].pop(CELL);p.write_text(json.dumps(r,indent=2)+'\n')
        try:audit(root,qualified,initial)
        except (AssertionError,FileNotFoundError)as error:rows.append(dict(case=name,rejected=True,reason=str(error)))
        else:raise AssertionError('Damaged evidence passed: '+name)
    result=dict(status='PASS_NATIVE_MC1668_READER_CONTROLS',baseline_rbf=passing['rbf_sha256'],cases=rows,quartus_run=False,hardware_access=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(rows),'damaged/rehashed evidence cases rejected')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('source',type=Path);p.add_argument('--qualified',type=Path,required=True);p.add_argument('--initial',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    run(a.source.resolve(),a.qualified.resolve(),a.initial.resolve(),a.output.resolve())
