#!/usr/bin/env python3
"""Reject actual corrupted native cache timing/source/memory/counter evidence."""
import argparse,hashlib,json,os,re,shutil,subprocess,sys,tempfile
from pathlib import Path
from review_storage_cache_native import review
from test_review_storage_board_native import dependencies
ROOT=Path(__file__).resolve().parents[1]
CASES=('missing-original-IFU','negative-SDRAM-output','wrong-half-cycle','rehashed-small-buffer',
       'rehashed-disconnected-hit','wrong-cache-old-data','cache-bank-alias','missing-tag-bit',
       'missing-buffer-bit','missing-hit-counter-bit','negative-hit-event','wrong-derived-clock',
       'new-warning','snapshot-enabled','failed-fit')
def sha(b):return hashlib.sha256(b).hexdigest()
def run(root,recipe,counter,qualified,out):
 assert not out.exists();out.mkdir(parents=True)
 expected=json.loads(qualified.read_text());assert json.loads(json.dumps(review(root,recipe,counter)))==expected
 def hashes(d):return {str(f.relative_to(d)):sha(f.read_bytes())for f in d.rglob('*')if f.is_file()}
 before=hashes(root);counter_before=hashes(counter)
 def write(p,b):
  tmp=p.with_name(p.name+'.mutation');tmp.write_bytes(b);os.replace(tmp,p)
 def replace(p,old,new):
  b=p.read_bytes();assert b.count(old)==1,(p,old,b.count(old));write(p,b.replace(old,new))
 def negative(p):
  b=p.read_bytes();m=re.search(rb'Worst case slack is\s+([-+\d.]+)',b);assert m;replace(p,m[0],b'Worst case slack is -0.001')
 corner='7_slow_1100mv_100c';fitfile='core/output_files/Dorado.fit.rpt';mapfile='core/output_files/Dorado.map.rpt'
 def source(p,old,new):
  n='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv';replace(p/n,old,new)
  for file,key in [('storage-cache-native.json','source_hashes'),('collected-ready-input-check.json','actual')]:
   f=p/file;r=json.loads(f.read_bytes());r[key][n]=sha((p/n).read_bytes());write(f,json.dumps(r).encode())
 def phase(p):
  f=p/f'core/storage-sample-output-setup-{corner}.rpt'
  rows=[s for s in f.read_bytes().splitlines()if len(s.split(b';')[1:-1])==8 and b'; 8.750 'in s]
  assert len(rows)==256;replace(f,rows[0],rows[0].replace(b'; 8.750 ',b'; 17.500 ',1))
 def cache_row(p,bank,fn):
  f=p/fitfile;rows=[s for s in f.read_bytes().splitlines()if b':b_MemD|'in s and f':snapshot_bank_{bank}|'.encode()in s and b'; Old data'in s]
  assert len(rows)==1;replace(f,rows[0],fn(rows[0]))
 def alias(p):
  rows=[s for s in(p/fitfile).read_bytes().splitlines()if b':b_MemD|'in s and b':snapshot_bank_3|'in s and b'; Old data'in s];assert len(rows)==1
  sites=re.findall(rb'M10K_X\d+_Y\d+_N\d+',rows[0]);assert len(sites)==2
  def change(row):
   old=re.findall(rb'M10K_X\d+_Y\d+_N\d+',row);assert len(old)==2
   for a,b in zip(old,sites):row=row.replace(a,b)
   return row
  cache_row(p,8,change)
 def missing(p,pattern):
  b=p.read_bytes();m=re.search(pattern,b,re.M);assert m;replace(p,m[0],b'')
 def clock(p):
  f=p/'core/native-clocks.rpt';rows=[s for s in f.read_bytes().splitlines()if b'dorado_sdram_clk 'in s and b'; Generated'in s];assert len(rows)==1
  replace(f,rows[0],rows[0].replace(b'17.500',b'15.000'))
 def snapshot(p):
  f=p/mapfile;m=re.search(rb'; SNAPSHOT_PAUSE\s*; 0\s*;',f.read_bytes());assert m;replace(f,m[0],m[0].replace(b'; 0 ',b'; 1 ',1))
 def failed(p):
  f=p/'run-state.json';r=json.loads(f.read_bytes());r['stages'][1]['returncode']=1;write(f,json.dumps(r).encode())
 mutations=[
  lambda p,c:(p/f'core/native59-ifu-in-setup-{corner}.rpt').unlink(),
  lambda p,c:negative(p/f'core/native59-sdram-output-setup-{corner}.rpt'),
  lambda p,c:phase(p),
  lambda p,c:source(p,b'.CACHE_LINES(16)) u_sdram(',b'.CACHE_LINES(8)) u_sdram('),
  lambda p,c:source(p,b'assign storage_events={(storage_initialized && backing_cache_hit),',b"assign storage_events={1'b0,"),
  lambda p,c:cache_row(p,3,lambda row:row.replace(b'Old data',b'New data')),
  lambda p,c:alias(p),
  lambda p,c:missing(p/'storage-ready-reports-v1/backend.log',rb'^BURST_REGISTER .*\|line_tag\[15\]\[0\]\n'),
  lambda p,c:missing(p/'storage-ready-reports-v1/backend.log',rb'^BURST_REGISTER .*\|line_words\[15\]\[0\]\n'),
  lambda p,c:missing(c/'audit.log',rb'^CACHE_HIT_COUNTER_REGISTER 7_slow_1100mv_100c .*count\[11\]\[0\]\n'),
  lambda p,c:negative(c/f'cache-hit-event-setup-{corner}.rpt'),
  lambda p,c:clock(p),
  lambda p,c:write(p/'fit.log',(p/'fit.log').read_bytes()+b'\nWarning (999999): unexpected new warning\n'),
  lambda p,c:snapshot(p),lambda p,c:failed(p)]
 cases=[]
 for name,mutate in zip(CASES,mutations):
  with tempfile.TemporaryDirectory(prefix='dorado-cache-review-bad-')as tmp:
   p=Path(tmp)/'root';c=Path(tmp)/'counter';shutil.copytree(root,p,copy_function=os.link);shutil.copytree(counter,c,copy_function=os.link)
   mutate(p,c)
   cmd=[sys.executable,str(ROOT/'tools/review_storage_cache_native.py'),'--root',str(p),'--recipe',str(recipe),'--counter',str(c),'--output',str(Path(tmp)/'bad.json')]
   result=subprocess.run(cmd,capture_output=True,text=True);log=result.stdout+result.stderr
   (out/(name+'.log')).write_text(log)
   assert result.returncode!=0 and('AssertionError'in result.stderr or'FileNotFoundError'in result.stderr),(name,log[-1500:])
   cases.append(dict(name=name,rejected=True,returncode=result.returncode,log_sha256=sha(log.encode())));print(name,'rejected',flush=True)
 assert json.loads(json.dumps(review(root,recipe,counter)))==expected
 assert before==hashes(root)and counter_before==hashes(counter)
 deps=dependencies();deps['tools/audit_storage_cache_native.tcl']=sha((ROOT/'tools/audit_storage_cache_native.tcl').read_bytes())
 result=dict(status='PASS_NATIVE_STORAGE_CACHE_REVIEW_REJECTION_CONTROLS',cases=cases,positive_qualification_sha256=sha(qualified.read_bytes()),recipe_config_sha256=sha(recipe.read_bytes()),reviewer_dependencies=deps,original_evidence_unchanged=True)
 (out/'controls.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(cases))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['root','recipe','counter','qualified','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();run(a.root.resolve(),a.recipe.resolve(),a.counter.resolve(),a.qualified.resolve(),a.output.resolve())
