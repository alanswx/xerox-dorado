#!/usr/bin/env python3
"""Stage board-ready reduction and fixed-lane capture at a tested faster clock.

Start directly from qualified corrected-BL8/MC1668 sources. Preserve CPU,
cache, Map, ECC, SYSPER4 and all peripheral logic. Retune the host PLL and its
physical-time watchdog/controller parameter together; keep absolute SDRAM
pin timing budgets. Source staging is not faster-core qualification.
"""
import argparse,hashlib,json
from pathlib import Path
from prepare_storage_ready_prediction import address_preview,board_preview,group_prediction
from prepare_storage_board_ready import board_reduction,group_outputs
from storage_capture_decode import decode_capture,allow_66mhz
from stage_storage_ready_native import BACKPLANE,MSA,GROUP,ADDR,PREVIEW,BASE,connect_backplane,ready_audit
from review_storage_board_ready import review
ROOT=Path(__file__).resolve().parents[1]
BACKEND='src/fpga/mister/dorado_storage_sdram_burst.sv'
PLL='core/rtl/pll/pll_0002.v'
def sha(b):return hashlib.sha256(b).hexdigest()
def js(p):return json.loads(p.read_text())
def replace(b,a,c):assert b.count(a)==1,(a,b.count(a));return b.replace(a,c)

def stage(a):
 out=a.output;assert not out.exists()and a.project.is_absolute()
 mhz={57142857:'57.142857',66666667:'66.666667'}[a.clock_hz]
 p=js(a.parent/'sdram-burst-native.json');q=js(a.parent_qualification)
 assert q['status']=='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW'and q['source_hashes']==p['source_hashes']
 assert p['corrected_sdram_read_capture']and p['frequency_mhz']==50 and p['sysper']==4 and p['state_admin']=='INACTIVE'
 fq=review(a.fit,a.component,a.groups,a.parent_component,a.parent_groups,a.parent_fit,a.parent_fit_qualification)
 assert json.loads(json.dumps(fq))==js(a.fit_qualification)
 combined=js(a.combined/'provenance.json');cs=js(a.combined/'run-state.json')
 assert cs['status']=='PASS_MSA_SAME_EDGE_BOARD_READY_REDUCTION'and len(cs['actions'])==8 and combined['fixed_lane_capture']
 for n,h in combined['inputs'].items():assert sha((a.combined/'inputs'/n).read_bytes())==h,n
 assert(a.combined/'positive-raw.csv').read_bytes()==(a.component/'positive-raw.csv').read_bytes()
 pins=js(a.pins/'provenance.json');ps=js(a.pins/'run-state.json');assert ps['status']=='PASS_FIXED_LANE_CAPTURE_PIN_MATRIX_AND_66MHZ'
 for n,h in pins['inputs'].items():assert sha((a.pins/'fixture'/n).read_bytes())==h,n
 for directory,full in [(a.trace,False),(a.full,True)]:
  tp=js(directory/'provenance.json');ts=js(directory/'run-state.json');assert ts['status']=='PASS_NATIVE_FIXED_CAPTURE_MEMORY_TRACE_PINS'
  assert tp['address_bits']==(23 if full else 14)
  for n,h in tp['inputs'].items():assert sha((directory/'inputs'/n).read_bytes())==h,n
  assert len(ts['actions'])==(2 if full else 16)
  if full:assert '-GCLOCK_HZ='+str(a.clock_hz)in ts['actions'][0]['command']
 output={}
 for n,h in p['source_hashes'].items():
  b=(a.parent/n).read_bytes();assert sha(b)==h,n
  if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:b=b.replace(p['project'].encode(),str(a.project).encode())
  output[n]=b
 original=dict(output)
 output[GROUP]=group_outputs(group_prediction(output[GROUP]))
 output[MSA]=board_reduction(board_preview(output[MSA]))
 output[PREVIEW]=address_preview(output[ADDR])
 output[BACKPLANE]=connect_backplane(output[BACKPLANE])
 output[BACKEND]=allow_66mhz(decode_capture(output[BACKEND]))
 for n,tested in [(GROUP,'dorado_storage_group.sv'),(MSA,'msa_ddram.v'),(PREVIEW,'addr_preview.sv')]:assert output[n]==(a.combined/'inputs'/tested).read_bytes(),n
 for d in [a.trace,a.full]:assert output[BACKEND]==(d/'inputs/backend.sv').read_bytes()
 assert(a.combined/'inputs/dorado_storage_sdram_burst.sv').read_bytes()==(a.pins/'fixture/fpga/mister/dorado_storage_sdram_burst.sv').read_bytes()
 output[PLL]=replace(output[PLL],b'output_clock_frequency0("50.000000 MHz")',('output_clock_frequency0("'+mhz+' MHz")').encode())
 output['core/Dorado.qsf']=replace(output['core/Dorado.qsf'],b'DORADO_HOST_CLOCK_HZ=50000000',('DORADO_HOST_CLOCK_HZ='+str(a.clock_hz)).encode())
 output['core/Dorado.qsf']+=('\n# Same-edge board-ready preview; fixed-lane capture validated separately.\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(a.project/PREVIEW)+'\n').encode()
 output['core/dorado_clock_config.json']=(json.dumps(dict(sys_mhz=mhz,sys_hz=a.clock_hz,hps_timeout_seconds=4))+'\n').encode()
 output['core/elaborated-host-clock.txt']=('PENDING NEW MAP READBACK: HOST_CLOCK_HZ='+str(a.clock_hz)+', HOST_TIMEOUT_CLOCKS='+str(4*a.clock_hz)+'. This is a staging expectation, not a compiler report.\n').encode()
 output['core/sdram_timing.sdc']=replace(output['core/sdram_timing.sdc'],b'# Raw SDRAM backend,20..50MHz host;',b'# Raw SDRAM candidate,20..66.667MHz host;')
 audit=ready_audit().replace(b'{*|u_storage_*|ready_q*}',b'{*|msa_ddram:b_*|ready_q*}').replace(b'[get_collection_size $predicted]<128',b'[get_collection_size $predicted]<8')
 output['core/audit-storage-ready.tcl']=audit
 changed={n for n in original if output[n]!=original[n]}
 expected={GROUP,MSA,BACKPLANE,BACKEND,PLL,'core/Dorado.qsf','core/dorado_clock_config.json','core/elaborated-host-clock.txt','core/sdram_timing.sdc'};assert changed==expected,changed
 assert set(output)-set(original)=={PREVIEW,'core/audit-storage-ready.tcl'}
 out.mkdir(parents=True)
 for n,b in output.items():
  f=out/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(b)
 record=dict(status='STAGED_BOARD_READY_FASTER_NATIVE_REVIEW_REQUIRED',scope=__doc__,project=str(a.project),
  source_hashes={n:sha(b)for n,b in output.items()},parent_manifest_sha256=sha((a.parent/'sdram-burst-native.json').read_bytes()),
  parent_qualification_sha256=sha(a.parent_qualification.read_bytes()),component_fit_qualification_sha256=sha(a.fit_qualification.read_bytes()),
  combined_provenance_sha256=sha((a.combined/'provenance.json').read_bytes()),pin_provenance_sha256=sha((a.pins/'provenance.json').read_bytes()),
  trace_provenance_sha256=sha((a.trace/'provenance.json').read_bytes()),full_provenance_sha256=sha((a.full/'provenance.json').read_bytes()),
  modified_sources=sorted(changed),frequency_mhz=float(mhz),frequency_hz=a.clock_hz,sysper=4,state_admin='INACTIVE',
  ordinary_issue_ceiling_per_second=a.clock_hz/8,nominal_ordinary_ns=round(8000000000/a.clock_hz),group_count=128,board_ready_bits=8,
  CPU_cache_map_ECC_peripheral_logic_unchanged=True,assembly_permitted=False,firmware_qualified=False,hardware_speedup_qualified=False)
 (out/'storage-board-native.json').write_text(json.dumps(record,indent=2)+'\n')
 for n in [Path(__file__).name,'build_r_front_ready_native.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 print(record['status'],len(output),'sources',flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','parent-qualification','component','groups','fit','fit-qualification','parent-component','parent-groups','parent-fit','parent-fit-qualification','combined','pins','trace','full','output','project']:p.add_argument('--'+n,type=Path,required=True)
 p.add_argument('--clock-hz',type=int,choices=[57142857,66666667],default=66666667)
 a=p.parse_args()
 for n,v in vars(a).items():
  if isinstance(v,Path)and n!='project':setattr(a,n,v.resolve())
 stage(a)
