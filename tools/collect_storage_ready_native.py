#!/usr/bin/env python3
"""Collect terminal native readiness evidence without fitting or programming.

Optionally follow the existing read-only reporter, verifying its live process.
Preserve original terminal failures and raw reports for review; collection is
never timing/firmware/speed qualification. Exclude images and build databases.
"""
import argparse,gzip,hashlib,json,os,tarfile,time
from pathlib import Path
def sha(b):return hashlib.sha256(b).hexdigest()

def collect(project,wait,attempt=1):
 follow=project/('storage-ready-reports-v'+str(attempt));state_path=follow/'run-state.json'
 while True:
  try:extra=json.loads(state_path.read_text())
  except (FileNotFoundError,json.JSONDecodeError):
   # The separately launched reporter may still be creating its first state.
   # Follow only its recorded, currently live process; never restart it.
   launch=json.loads((follow/'launch.json').read_text());d=Path('/proc')/str(launch['pid'])
   assert wait and d.exists(),'Reporter disappeared before a complete state'
   assert str(project)in(d/'cmdline').read_bytes().replace(b'\0',b' ').decode()
   time.sleep(1);continue
  d=Path('/proc')/str(extra['driver_pid']);alive=d.exists()
  if not extra['status'].startswith(('WAITING','RUNNING'))and not alive:break
  assert wait and alive,'Reporter not terminal; observe existing process'
  assert str(project)in(d/'cmdline').read_bytes().replace(b'\0',b' ').decode()
  time.sleep(10)
 while True:
  try:build=json.loads((project/'run-state.json').read_text())
  except (FileNotFoundError,json.JSONDecodeError):
   launch=json.loads((project/'launch.json').read_text());d=Path('/proc')/str(launch['pid'])
   assert wait and d.exists()and str(project)in(d/'cmdline').read_bytes().replace(b'\0',b' ').decode()
   time.sleep(1);continue
  d=Path('/proc')/str(build['builder_pid'])
  if build['status']!='RUNNING'and not d.exists():break
  assert wait and d.exists()and str(project)in(d/'cmdline').read_bytes().replace(b'\0',b' ').decode()
  time.sleep(10)
 assert extra['status']in ['TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED','TERMINAL_FAILED']
 pids={build['builder_pid'],extra['driver_pid']}|{a['pid']for a in build['stages']+extra['actions']}
 assert all(not(Path('/proc')/str(pid)).exists()for pid in pids),'Recorded work still live'
 name=next(n for n in ['storage-response-native.json','storage-cache-native.json','pack-window-native.json','storage-board-native.json','storage-ready-native.json']if(project/n).exists())
 r=json.loads((project/name).read_text())
 actual={n:sha((project/n).read_bytes())for n in r['source_hashes']};assert actual==r['source_hashes']and len(actual)==503
 check=dict(status='TERMINAL_NATIVE_READY_INPUT_READBACK',actual=actual,absent_pids=sorted(pids),original_build_status=build['status'],report_status=extra['status'],timing_qualified=False,firmware_qualified=False,assembly_collected=False)
 (project/'collected-ready-input-check.json').write_text(json.dumps(check,indent=2)+'\n')
 files=set(actual)
 directories=[project,project/'core',project/'core/output_files',follow]
 if attempt!=1:directories.append(project/'storage-ready-reports-v1')
 directories.extend(d for d in project.glob('failed-*')if d.is_dir())
 for directory in directories:
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
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);p.add_argument('--wait',action='store_true');p.add_argument('--report-attempt',type=int,choices=[1,2],default=1);a=p.parse_args();collect(a.project.resolve(),a.wait,a.report_attempt)
