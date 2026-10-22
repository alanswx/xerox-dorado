#!/usr/bin/env python3
"""Reject altered source, clock, coverage and timing evidence for ready prediction."""
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def changed(path,data):
 temp=path.with_name(path.name+'.mutated');temp.write_bytes(data);os.replace(temp,path)
def replace(path,old,new):
 b=path.read_bytes();assert b.count(old)>=1;changed(path,b.replace(old,new,1))
def rehash(root,name):
 p=root/'provenance.json';r=json.loads(p.read_text());r['files'][name]=hashlib.sha256((root/name).read_bytes()).hexdigest();changed(p,(json.dumps(r,indent=2)+'\n').encode())
def run(source,component,groups,out):
 assert not out.exists();out.mkdir(parents=True)
 original={str(p.relative_to(source)):hashlib.sha256(p.read_bytes()).hexdigest()for p in source.rglob('*')if p.is_file()}
 cases=[]
 def case(name,mutation,diagnostic):
  root=out/name;shutil.copytree(source,root,copy_function=os.link);mutation(root)
  with(out/(name+'.log')).open('x')as log:
   cmd=[sys.executable,str(ROOT/'tools/review_storage_ready_prediction.py'),'--root',str(root),'--component',str(component),'--groups',str(groups),'--output',str(root/'must-not-pass.json')]
   r=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT)
  text=(out/(name+'.log')).read_text();assert r.returncode!=0 and diagnostic in text,(name,r.returncode,text[-1500:]);assert not(root/'must-not-pass.json').exists()
  cases.append(dict(case=name,rejected=True,returncode=r.returncode,diagnostic=diagnostic));print(name,'rejected',flush=True)
 def negative_slack(root):
  p=root/'ready-gate-setup-7_slow_1100mv_100c.rpt';b=p.read_bytes();m=re.search(rb'Worst case slack is (-?[0-9.]+)',b);assert m;changed(p,b[:m.start(1)]+b'-0.001'+b[m.end(1):])
 def weak_clock(root):replace(root/'clock.sdc',b'15.000',b'16.000');rehash(root,'clock.sdc')
 def unconstrained(root):
  p=root/'ready-ucp-7_slow_1100mv_100c.rpt';b=p.read_bytes();m=re.search(rb'(; Unconstrained Input Ports\s*;\s*)0',b);assert m;changed(p,b[:m.end()-1]+b'1'+b[m.end():])
 def old_address(root):replace(root/'dorado_storage_group.sv',b'pred_cache_addr==addr_after_edge',b'pred_cache_addr==addr');rehash(root,'dorado_storage_group.sv')
 def no_gate(root):replace(root/'top.sv',b'.ena(enable)',b".ena(1'b1)");rehash(root,'top.sv')
 case('missing-corner',lambda r:(r/'ready-predict-hold-MIN_fast_1100mv_-40c.rpt').unlink(),'FileNotFoundError')
 case('negative-gate-slack',negative_slack,'Negative slack')
 case('weakened-clock-rehashed',weak_clock,'Fit source/configuration differs')
 case('missing-ready-bit',lambda r:replace(r/'audit.log',b'u_storage_13|ready_q',b'u_storage_12|ready_q'),'Ready bit/copy coverage differs')
 case('unconstrained-input',unconstrained,'Unconstrained paths')
 case('wrong-prediction-rehashed',old_address,'Fit source/configuration differs')
 case('missing-terminal-proof',lambda r:(r/'terminal-input-check.json').unlink(),'FileNotFoundError')
 case('bypassed-gate-rehashed',no_gate,'Fit source/configuration differs')
 assert original=={str(p.relative_to(source)):hashlib.sha256(p.read_bytes()).hexdigest()for p in source.rglob('*')if p.is_file()},'Control mutated original evidence'
 (out/'controls.json').write_text(json.dumps(dict(status='PASS_READY_REVIEW_REJECTION_CONTROLS',cases=cases,original_evidence_unchanged=True,reviewer_sha256=hashlib.sha256((ROOT/'tools/review_storage_ready_prediction.py').read_bytes()).hexdigest()),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','component','groups','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();run(a.root.resolve(),a.component.resolve(),a.groups.resolve(),a.output.resolve())
