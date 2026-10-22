#!/usr/bin/env python3
"""Stage an opt-in16-line native backing buffer and passive hit observation.

Start from exact reviewed9e sources; preserve all clocks, CPU, architectural
cache/Map/ECC, storage controller, disk adapter, Main and network logic. Only
the native wrapper's permitted backing-buffer parameter and unwired monitor
event change. New native fit, all-device timing, firmware and performance
review are required. Staging grants no assembly or deployment qualification.
"""
import argparse,gzip,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
BACKEND='src/fpga/mister/dorado_storage_sdram_burst.sv'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def digest(b):return hashlib.sha256(b).hexdigest()
def load(p):return json.loads(p.read_text())
def validate_component(directory,provenance_name,hash_key='inputs'):
 f=directory/provenance_name
 r=json.loads(gzip.decompress(f.read_bytes()))
 for n,h in r[hash_key].items():assert sha(directory/'inputs'/n)==h,n
 return r
def stage(parent,qualification,capacity,controls,board,out,project):
 assert not out.exists()and project.is_absolute()
 q=load(qualification);r=load(parent/'pack-window-native.json')
 assert q['status']=='PASS_NATIVE_PACK_WINDOW_PREASSEMBLY_REVIEW'
 assert q['source_hashes']==r['source_hashes']and len(q['source_hashes'])==503
 assert q['frequency_hz']==r['frequency_hz']==57142857
 assert q['sysper']==r['sysper']==4 and q['state_admin']==r['state_admin']=='INACTIVE'
 c=load(capacity/'result.json');assert c['status']=='PASS_NATIVE_SDRAM_CAPACITY_COMPONENT'
 p=validate_component(capacity,'provenance.json.gz','frozen_inputs')
 assert p['qualified_source_hashes'][BACKEND]==q['source_hashes'][BACKEND]
 assert c['results']['4']['requests']==c['results']['16']['requests']==512
 k=load(controls/'run-state.json');assert k['status']=='PASS_NATIVE_SDRAM_CAPACITY_REJECTION_CONTROLS'
 assert k['positive_result_sha256']==sha(capacity/'result.json')and k['positive_inputs_unchanged']
 assert [x['name']for x in k['cases']]==['unmutated-harness','stale-write','orphan-fill','aliased-group-bit','early-capture']
 assert k['cases'][0]['unmutated_pass']and k['cases'][0]['returncode']==0
 assert all(x['rejected']and x['returncode']!=0 for x in k['cases'][1:])
 b=load(board/'run-state.json');assert b['status']=='PASS_NATIVE_CACHE_CAPACITY_ORIGINAL_MSA_COMPONENT'
 bp=validate_component(board,'provenance.json.gz')
 assert bp['qualification_sha256']==p['qualification_sha256']==sha(qualification)
 assert all(q['source_hashes'][n]==h for n,h in bp['qualified_inputs'].items())
 assert b['frozen_inputs_unchanged']and set(b['results'])=={'lines4','lines16'}
 assert(board/'lines4-raw.csv').read_bytes()==(board/'lines16-raw.csv').read_bytes()
 files={}
 for n,h in q['source_hashes'].items():
  f=parent/n;assert sha(f)==h,n;raw=f.read_bytes()
  if f.suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
   raw=raw.replace(r['project'].encode(),str(project).encode())
  files[n]=raw
 original=dict(files)
 old=b'.CACHE_LINES(4)) u_sdram('
 new=b'.CACHE_LINES(16)) u_sdram('
 assert files[WRAPPER].count(old)==1
 files[WRAPPER]=files[WRAPPER].replace(old,new)
 old_event=b"assign storage_events={1'b0,(sv && sr),(rv && !rr),1'b0,"
 new_event=b"assign storage_events={(storage_initialized && backing_cache_hit),(sv && sr),(rv && !rr),1'b0,"
 assert files[WRAPPER].count(old_event)==1
 files[WRAPPER]=files[WRAPPER].replace(old_event,new_event)
 assert [n for n in original if files[n]!=original[n]]==[WRAPPER]
 # Removing the two explicit edits must recover the relocated parent exactly.
 assert files[WRAPPER].replace(new,old).replace(new_event,old_event)==original[WRAPPER]
 out.mkdir(parents=True)
 for n,raw in files.items():
  f=out/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(raw)
 record=dict(status='STAGED_NATIVE_STORAGE_CACHE_CAPACITY_REVIEW_REQUIRED',scope=__doc__,project=str(project),
  parent_project=r['project'],parent_recipe_sha256=sha(parent/'pack-window-native.json'),
  parent_qualification_sha256=sha(qualification),parent_source_hashes=q['source_hashes'],
  source_hashes={n:digest(raw)for n,raw in files.items()},modified_RTL_sources=[WRAPPER],
  wrapper_changes=['CACHE_LINES4->16','storage_events[8]=storage_initialized && backing_cache_hit'],
  component_provenance_sha256=sha(capacity/'provenance.json.gz'),
  rejection_controls_sha256=sha(controls/'run-state.json'),board_provenance_sha256=sha(board/'provenance.json.gz'),
  frequency_hz=57142857,frequency_mhz=57.142857,sysper=4,state_admin='INACTIVE',
  raw_buffer_lines=16,raw_buffer_bytes=256,architectural_cache_unchanged=True,
  nominal_ordinary_ns=140,nominal_issue_ceiling_per_second=57142857/8,
  controller_CPU_map_ECC_clocks_disk_Main_network_unchanged=True,
  assembly_permitted=False,firmware_qualified=False,hardware_speedup_qualified=False)
 (out/'storage-cache-native.json').write_text(json.dumps(record,indent=2)+'\n')
 for n in ['stage_storage_cache_native.py','build_r_front_ready_native.py']:
  (out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 print(record['status'],len(files),'sources, one wrapper/two explicit edits',flush=True)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','qualification','capacity','controls','board','output','project']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args()
 stage(a.parent.resolve(),a.qualification.resolve(),a.capacity.resolve(),a.controls.resolve(),a.board.resolve(),a.output.resolve(),a.project)
