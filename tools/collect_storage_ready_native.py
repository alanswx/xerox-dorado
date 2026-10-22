#!/usr/bin/env python3
"""Collect terminal native readiness evidence without fitting or programming.

Optionally follow the existing read-only reporter, verifying its live process.
Preserve original terminal failures and raw reports for review; collection is
never timing/firmware/speed qualification. Exclude images and build databases.
"""
import argparse,gzip,hashlib,json,os,tarfile,time
from pathlib import Path
def sha(b):return hashlib.sha256(b).hexdigest()

def collect(project,wait):
 follow=project/'storage-ready-reports-v1';state_path=follow/'run-state.json'
 while True:
  extra=json.loads(state_path.read_text());d=Path('/proc')/str(extra['driver_pid']);alive=d.exists()
  if not extra['status'].startswith(('WAITING','RUNNING'))and not alive:break
  assert wait and alive,'Reporter not terminal; observe existing process'
  assert str(project)in(d/'cmdline').read_bytes().replace(b'\0',b' ').decode()
  time.sleep(10)
 build=json.loads((project/'run-state.json').read_text())
 assert build['status']!='RUNNING'
 assert extra['status']in ['TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED','TERMINAL_FAILED']
 pids={build['builder_pid'],extra['driver_pid']}|{a['pid']for a in build['stages']+extra['actions']}
 assert all(not(Path('/proc')/str(pid)).exists()for pid in pids),'Recorded work still live'
 r=json.loads((project/'storage-ready-native.json').read_text())
 actual={n:sha((project/n).read_bytes())for n in r['source_hashes']};assert actual==r['source_hashes']and len(actual)==503
 check=dict(status='TERMINAL_NATIVE_READY_INPUT_READBACK',actual=actual,absent_pids=sorted(pids),original_build_status=build['status'],report_status=extra['status'],timing_qualified=False,firmware_qualified=False,assembly_collected=False)
 (project/'collected-ready-input-check.json').write_text(json.dumps(check,indent=2)+'\n')
 files=set(actual)
 for directory in [project,project/'core',project/'core/output_files',follow]:
  files.update(str(f.relative_to(project))for f in directory.iterdir()if f.is_file()and f.suffix in ['.log','.json','.py','.tcl','.rpt','.summary'])
 out=project/'storage-ready-terminal-v1';assert not out.exists();out.mkdir();rows=[]
 for n in sorted(files):
  assert not Path(n).is_absolute()and '..'not in Path(n).parts
  assert Path(n).suffix not in ['.rbf','.sof','.qdb','.o','.a','.pack']
  b=(project/n).read_bytes();assert not b.startswith(b'\x7fELF'),n
  stored=n+'.gz';f=out/stored;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(gzip.compress(b,mtime=0));assert gzip.decompress(f.read_bytes())==b
  rows.append(dict(raw_name=n,stored=stored,raw_bytes=len(b),raw_sha256=sha(b)))
 (out/'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows,indent=2)+'\n').encode(),mtime=0))
 archive=project/'storage-ready-terminal-v1.tar.gz'
 with tarfile.open(archive,'w:gz')as t:t.add(out,arcname='receipt')
 receipt=dict(status='TERMINAL_NATIVE_READY_RECEIPT_READBACK',members=len(rows),archive=str(archive),archive_sha256=sha(archive.read_bytes()),original_build_status=build['status'],report_status=extra['status'],timing_qualified=False,firmware_qualified=False)
 (project/'collection-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt),flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--wait',action='store_true');a=p.parse_args();collect(a.project.resolve(),a.wait)
