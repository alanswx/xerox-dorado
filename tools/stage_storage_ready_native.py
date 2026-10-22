#!/usr/bin/env python3
"""Integrate qualified post-edge readiness into all128 original native groups.

Keep the corrected BL8 parent, original CPU/cache/map/ECC, services and clocks.
The parent's contb_advance is already the actual network_step/global-gate epoch.
This stages sources only; full timing, firmware and hardware remain unqualified.
"""
import argparse,hashlib,json,re
from pathlib import Path
from prepare_storage_ready_prediction import address_preview,board_preview,group_prediction
from review_storage_ready_prediction import review

ROOT=Path(__file__).resolve().parents[1]
BASE='src/build/rtl-executive/native16-tx-cable/'
BACKPLANE=BASE+'dorado_native_backplane.v'
MSA=BASE+'msa_ddram.v'
GROUP='src/fpga/mister/dorado_storage_group.sv'
ADDR='src/verilog/cells/cell_MK4096P_addr.v'
PREVIEW='src/fpga/mister/dorado_storage_addr_preview.sv'

def sha(b):return hashlib.sha256(b).hexdigest()

def connect_backplane(raw):
 s=raw.decode()
 pattern=r'(msa_ddram #\(\.SYSPER\(SYSPER\)\) b_msa\w* \(\n    \.sys_clk\(sys_clk\),\n    \.storage_clk\(storage_clk\),)'
 s,n=re.subn(pattern,r'\1\n    .storage_step(contb_advance),',s)
 assert n==8,n
 return s.encode()

def ready_audit():
 return '''project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {lappend corners [get_operating_conditions_info $op -name]}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 set predicted [get_registers {*|u_storage_*|ready_q*}]
 set step [get_registers {*network_step*}]
 if {[get_collection_size $predicted]<128 || [get_collection_size $step]<1} {error "Missing native ready/step state"}
 puts "NATIVE_READY_CORNER $name [get_collection_size $predicted]"
 foreach_in_collection r $predicted {puts "NATIVE_READY_REGISTER [get_node_info -name $r]"}
 foreach_in_collection r $step {puts "NATIVE_STEP_REGISTER [get_node_info -name $r]"}
 foreach mode {setup hold} {
  report_timing -$mode -to $predicted -npaths 256 -detail full_path -file native-ready-predict-$mode-$name.rpt
  report_timing -$mode -from $predicted -to $step -npaths 256 -detail full_path -file native-ready-gate-$mode-$name.rpt
  report_timing -$mode -from $step -to $predicted -npaths 256 -detail full_path -file native-ready-step-$mode-$name.rpt
 }
 delete_timing_netlist
}
project_close
'''.encode()

def stage(parent,qualification,component,groups,fit,fit_qualification,out,project):
 assert not out.exists()and project.is_absolute()
 r=json.loads((parent/'sdram-burst-native.json').read_text())
 q=json.loads(qualification.read_text())
 assert q['status']=='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW'
 assert q['source_hashes']==r['source_hashes']and r['corrected_sdram_read_capture']
 assert r['frequency_mhz']==50 and r['sysper']==4 and r['state_admin']=='INACTIVE'
 fq=json.loads(fit_qualification.read_text())
 assert json.loads(json.dumps(review(fit,component,groups)))==fq,'Component fit qualification differs'
 p=json.loads((component/'provenance.json').read_text());inputs=component/'inputs'
 output={}
 for n,h in r['source_hashes'].items():
  b=(parent/n).read_bytes();assert sha(b)==h,n
  if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:b=b.replace(r['project'].encode(),str(project).encode())
  output[n]=b
 original=dict(output)
 assert output[GROUP]==(inputs/'original_storage_group.sv').read_bytes()
 assert output[ADDR]==(inputs/'cells/cell_MK4096P_addr.v').read_bytes()
 output[GROUP]=group_prediction(output[GROUP])
 output[MSA]=board_preview(output[MSA])
 output[PREVIEW]=address_preview(output[ADDR])
 for n,tested in [(GROUP,'dorado_storage_group.sv'),(MSA,'msa_ddram.v'),(PREVIEW,'addr_preview.sv')]:
  assert sha(output[n])==p['inputs'][tested],n
 output[BACKPLANE]=connect_backplane(output[BACKPLANE])
 # The existing top and machine wrapper deliver the same falling-edge decision
 # used by the native clock gate, not a delta-late sample of gated sys_clk.
 assert b'.contb_advance(network_step)'in output['core/Dorado.sv']
 assert b'.contb_advance(contb_advance)'in output[BASE+'dorado_native_machine.sv']
 output['core/Dorado.qsf']+=('\n# Original address state plus tested post-edge preview.\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/PREVIEW)+'\n').encode()
 output['core/audit-storage-ready.tcl']=ready_audit()
 changed={n for n in original if output[n]!=original[n]}
 assert changed=={GROUP,MSA,BACKPLANE,'core/Dorado.qsf'}
 assert set(output)-set(original)=={PREVIEW,'core/audit-storage-ready.tcl'}
 out.mkdir(parents=True)
 for n,b in output.items():
  f=out/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(b)
 record=dict(status='STAGED_STORAGE_READY_NATIVE_REVIEW_REQUIRED',scope=__doc__,
  project=str(project),source_hashes={n:sha(b)for n,b in output.items()},
  parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),
  parent_qualification_sha256=sha(qualification.read_bytes()),
  component_provenance_sha256=sha((component/'provenance.json').read_bytes()),
  groups_provenance_sha256=sha((groups/'provenance.json').read_bytes()),
  component_fit_qualification_sha256=sha(fit_qualification.read_bytes()),
  modified_sources=sorted(changed),added_sources=sorted(set(output)-set(original)),
  group_count=128,memory_boards=8,step_source='contb_advance=network_step',
  frequency_mhz=50,sysper=4,state_admin='INACTIVE',
  CPU_cache_map_ECC_services_clocks_unchanged=True,
  assembly_permitted=False,firmware_qualified=False,hardware_speedup_qualified=False)
 (out/'storage-ready-native.json').write_text(json.dumps(record,indent=2)+'\n')
 for n in [Path(__file__).name,'build_r_front_ready_native.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 print(record['status'],len(output),'sources,128 groups',flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','qualification','component','groups','fit','fit-qualification','output','project']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();stage(a.parent.resolve(),a.qualification.resolve(),a.component.resolve(),a.groups.resolve(),a.fit.resolve(),a.fit_qualification.resolve(),a.output.resolve(),a.project)
