#!/usr/bin/env python3
"""Reject damaged board-reduction evidence without changing original receipts."""
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(b):return hashlib.sha256(b).hexdigest()
def write(p,b):
 f=p.with_name(p.name+'.new');f.write_bytes(b);os.replace(f,p)
def replace(p,old,new):
 b=p.read_bytes();assert b.count(old)>=1;write(p,b.replace(old,new,1))
def rehash(root,n):
 p=root/'provenance.json';q=json.loads(p.read_text());q['files'][n]=sha((root/n).read_bytes());write(p,(json.dumps(q,indent=2)+'\n').encode())
def run(a):
 out=a.output;assert not out.exists();out.mkdir(parents=True)
 before={str(f.relative_to(a.root)):sha(f.read_bytes())for f in a.root.rglob('*')if f.is_file()};cases=[]
 def case(name,change,diagnostic):
  root=out/name;shutil.copytree(a.root,root,copy_function=os.link);change(root)
  cmd=[sys.executable,str(ROOT/'tools/review_storage_board_ready.py'),'--root',str(root),'--output',str(root/'must-not-pass.json')]
  for n in ['component','groups','parent_component','parent_groups','parent_fit','parent_qualification']:cmd+=['--'+n.replace('_','-'),str(getattr(a,n))]
  with(out/(name+'.log')).open('x')as log:r=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT)
  t=(out/(name+'.log')).read_text();assert r.returncode!=0 and diagnostic in t and not(root/'must-not-pass.json').exists(),(name,r.returncode,t[-1000:])
  cases.append(dict(case=name,rejected=True,returncode=r.returncode,diagnostic=diagnostic));print(name,'rejected',flush=True)
 def negative_slack(root):
  p=root/'ready-gate-setup-7_slow_1100mv_100c.rpt';b=p.read_bytes();m=re.search(rb'Worst case slack is (-?[0-9.]+)',b);assert m;write(p,b[:m.start(1)]+b'-0.001'+b[m.end(1):])
 def weak_clock(root):replace(root/'clock.sdc',b'15.000',b'16.000');rehash(root,'clock.sdc')
 def wrong_board(root):replace(root/'msa_ddram.v',b'storage_step ? ready_advance_all : ready_hold_all',b'&storage_group_ready');rehash(root,'msa_ddram.v')
 def no_gate(root):replace(root/'top.sv',b'.ena(enable)',b".ena(1'b1)");rehash(root,'top.sv')
 def unconstrained(root):
  p=root/'ready-ucp-7_slow_1100mv_100c.rpt';b=p.read_bytes();m=re.search(rb'(; Unconstrained Input Ports\s*;\s*)0',b);assert m;write(p,b[:m.end()-1]+b'1'+b[m.end():])
 case('missing-corner',lambda r:(r/'ready-predict-hold-MIN_fast_1100mv_-40c.rpt').unlink(),'FileNotFoundError')
 case('negative-gate-slack',negative_slack,'Negative slack')
 case('weakened-clock-rehashed',weak_clock,'Component recipe differs')
 case('missing-board-ready',lambda r:replace(r/'audit.log',b'msa_ddram:u_msa|ready_q',b'msa_ddram:u_msa|missing'),'AssertionError')
 case('unconstrained-input',unconstrained,'Unconstrained paths')
 case('extra-ready-epoch-rehashed',wrong_board,'Component recipe differs')
 case('missing-terminal-proof',lambda r:(r/'terminal-input-check.json').unlink(),'FileNotFoundError')
 case('bypassed-gate-rehashed',no_gate,'Component recipe differs')
 assert before=={str(f.relative_to(a.root)):sha(f.read_bytes())for f in a.root.rglob('*')if f.is_file()}
 (out/'controls.json').write_text(json.dumps(dict(status='PASS_BOARD_READY_REVIEW_REJECTION_CONTROLS',cases=cases,original_evidence_unchanged=True,reviewer_sha256=sha((ROOT/'tools/review_storage_board_ready.py').read_bytes())),indent=2)+'\n')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','component','groups','parent-component','parent-groups','parent-fit','parent-qualification','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args()
 for n,v in vars(a).items():setattr(a,n,v.resolve())
 run(a)
