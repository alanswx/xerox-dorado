#!/usr/bin/env python3
"""Correct the CL2 receiver stage in the reviewed passive-burst native stage.

The Micron-authored reference launches at CL-1 and the receiver samples at CL.
Preserve legacy inputs; change only the raw controller's one-cycle-late wait.
"""
import argparse,hashlib,json
from pathlib import Path
from stage_native_sdram_burst import BACKEND
from storage_capture_source import correct_capture
ROOT=Path(__file__).resolve().parents[1]
def sha(data):return hashlib.sha256(data).hexdigest()

def stage(parent,qualification,component,out,project):
 assert not out.exists() and project.is_absolute()
 r=json.loads((parent/'sdram-burst-native.json').read_text());q=json.loads(qualification.read_text())
 assert q['status']=='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW' and q['raw_burst_observer']
 assert q['source_hashes']==r['source_hashes'] and len(r['source_hashes'])==501
 c=json.loads((component/'run-state.json').read_text());p=json.loads((component/'provenance.json').read_text())
 assert c['status']=='PASS_RAW_MEMORY_BURST_TRACE_PIN_COMPONENT' and c['read_capture_corrected']
 for n,h in p['inputs'].items():assert sha((component/'inputs'/n).read_bytes())==h
 output={}
 for n,h in r['source_hashes'].items():
  raw=(parent/n).read_bytes();assert sha(raw)==h,n
  if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(r['project'].encode(),str(project).encode())
  output[n]=correct_capture(raw)if n==BACKEND else raw
 assert sha(output[BACKEND])==p['inputs']['fpga/mister/dorado_storage_sdram_burst.sv']
 out.mkdir(parents=True)
 for n,raw in output.items():
  f=out/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(raw)
 record=dict(r,project=str(project),corrected_sdram_read_capture=True,
  corrected_capture_parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),
  corrected_capture_parent_qualification_sha256=sha(qualification.read_bytes()),
  burst_observer_component_provenance_sha256=sha((component/'provenance.json').read_bytes()),
  corrected_capture_deltas=[BACKEND],source_hashes={n:sha(v)for n,v in output.items()},
  status='STAGED_CORRECTED_SDRAM_READ_CAPTURE_REVIEW_REQUIRED',assembly_permitted=False)
 (out/'sdram-burst-native.json').write_text(json.dumps(record,indent=2)+'\n')
 for n in [Path(__file__).name,'build_r_front_ready_native.py','storage_capture_source.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 print(record['status'],len(output))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','qualification','component','output','project']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();stage(a.parent.resolve(),a.qualification.resolve(),a.component.resolve(),a.output.resolve(),a.project)
