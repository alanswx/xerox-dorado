#!/usr/bin/env python3
"""Read-only matched MCS6532 wrapper/RIOT state, binding and timing audit."""
import argparse,json,re,shutil,tempfile
from pathlib import Path
import fit_mcs6532 as fit
WRAPPER_SHA='5cc858c9465df03ee72bbb3a45e4b6ea6eab571872ae497285f3d32ed75337c0'
RIOT_SHA='43c9d1670fcbb98ad4681883981930817e2882a5ab32790f8285961c3ae0c1b1'


def retained(text,variant):
 wanted={'baseline':{'|cell_MCS6532:cell_inst|':(1091,1),'|M6532:u_riot|':(1090,1090)},
         'snapshot':{'|cell_MCS6532_snapshot:cell_inst|':(1121,5),'|M6532_snapshot:u_riot|':(1116,1116)}}[variant]
 start=text.find('; Analysis & Synthesis Resource Utilization by Entity');assert start>=0,'missing wrapper synthesis table'
 rows={}
 for l in text[start:].split('\n\n',1)[0].splitlines():
  f=[x.strip()for x in l.split(';')[1:-1]]
  if not f or f[0]not in wanted:continue
  assert f[0]not in rows and f[2]=='%d (%d)'%wanted[f[0]]and f[3]=='0','wrapper/RIOT retained-state count differs'
  rows[f[0]]=dict(total_registers=wanted[f[0]][0],own_registers=wanted[f[0]][1],block_memory_bits=0)
 assert set(rows)==set(wanted),'missing wrapper/RIOT entity'
 return rows


def audit(root):
 q=fit.audit(root);p=json.loads((root/'provenance.json').read_text());m=p['manifest']
 assert m==json.loads((root/'qualified-manifest.json').read_text()),'qualified wrapper schema differs'
 assert m['original_sha256']==WRAPPER_SHA and m['logical_state_bits']==1098
 assert m['phase']==dict(name='phase_d',width=1,word=133,initial=0)
 assert m['parameters']==dict(child_init_7800=0)
 assert m['valid_indices']==list(range(134))and m['invalid_indices']==list(range(134,256))
 child=m['child_manifest'];assert child['original_sha256']==RIOT_SHA and child['logical_state_bits']==1097
 assert child['parameters']==dict(init_7800=0)and child['generated_sha256']==m['child_sha256']
 assert fit.digest(root/'executed-sources/fit_mcs6532.py')==p['builder_sha256'],'executed builder differs'
 q['state_retention']={}
 for variant in ('baseline','snapshot'):
  d=root/variant
  for name,h in [('cell_MCS6532.v',WRAPPER_SHA),('M6532.sv',RIOT_SHA),('cell_MCS6532_snapshot.sv',m['generated_sha256']),('M6532_snapshot.sv',m['child_sha256'])]:
   assert fit.digest(d/name)==h,'compiled wrapper/RIOT differs:'+name
  mapping=(d/'snapshot_fit.map.rpt').read_text(errors='replace');q['state_retention'][variant]=retained(mapping,variant)
  headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for User Entity Instance:\s*([^\n;]+)',mapping));bindings=[]
  for i,h in enumerate(headers):
   if ':u_riot'not in h[1]:continue
   section=mapping[h.end():headers[i+1].start()if i+1<len(headers)else len(mapping)]
   bindings+=re.findall(r'; init_7800\s*;\s*(\w+)\s*;',section)
  assert bindings==['0'],'actual child parameter binding differs'
  assert '; Fitter RAM Summary'not in(d/'snapshot_fit.fit.rpt').read_text(errors='replace'),'unexpected wrapper RAM placement'
 q['retention_scope']='Snapshot total1121 = all1098modeled bits plus23interface bits. Child1116 =1097state+18response+shared frozen; wrapper own5 =phase_d+done+error+phase_result bit+response selector. Two identical frozen registers merge; unused child done/error prune. Synthesis counts, not fitted unique-register totals or global snapshot cost.'
 q['limits']='Matched isolated wrapper only; full BaseBoard/global FPGA save/load and clock/speed changes remain unqualified.'
 return q


def controls(root):
 rows=fit.controls(root)
 for name in ('phase_register','child_register','parameter_binding','missing_corner','schema'):
  with tempfile.TemporaryDirectory(prefix='dorado-mcs6532-audit-',dir='/tmp')as tmp:
   d=Path(tmp)/'evidence';shutil.copytree(root,d,ignore=shutil.ignore_patterns('db','incremental_db'))
   if name in ('phase_register','child_register'):
    p=d/'snapshot/snapshot_fit.map.rpt';s=p.read_text();lines=s.splitlines(keepends=True)
    token,before,after=('|cell_MCS6532_snapshot:cell_inst|','1121 (5)','1120 (4)')if name=='phase_register'else('|M6532_snapshot:u_riot|','1116 (1116)','1115 (1115)')
    for i,l in enumerate(lines):
     if token in l and before in l:lines[i]=l.replace(before,after,1);break
    else:raise AssertionError('missing wrapper register failure anchor')
    p.write_text(''.join(lines))
   elif name=='parameter_binding':
    p=d/'snapshot/snapshot_fit.map.rpt';s=p.read_text();changed,n=re.subn(r'(; init_7800\s*;\s*)0(\s*;)',r'\g<1>1\2',s,count=1);assert n==1;p.write_text(changed)
   elif name=='missing_corner':(d/'snapshot/bank-setup-7_slow_1100mv_100c.rpt').unlink()
   else:
    p=d/'qualified-manifest.json';m=json.loads(p.read_text());m['phase']['word']=132;p.write_text(json.dumps(m))
   try:audit(d)
   except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
   else:raise AssertionError('damaged MCS6532 evidence accepted:'+name)
 return rows


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
 a=p.parse_args();q=audit(a.directory.resolve())
 if a.negative_controls:q['failure_controls']=controls(a.directory.resolve())
 a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
