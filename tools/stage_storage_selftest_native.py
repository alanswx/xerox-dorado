#!/usr/bin/env python3
"""Stage a pre-guest raw SDRAM diagnostic on the reviewed passive-observer core.

Replace only zero-fill with write/read address patterns and verified zero-fill.
Guest storage release stays low on failure. No guest CPU or disk state is loaded.
"""
import argparse,hashlib,json
from pathlib import Path
from stage_boot_fault_observer import WRAPPER,TOP,substitute
ROOT=Path(__file__).resolve().parents[1]
SELFTEST='src/fpga/mister/dorado_storage_selftest.sv'
def sha(data):return hashlib.sha256(data).hexdigest()
def expose(name,raw):
 if name==WRAPPER:
  raw=substitute(raw,'    output wire [15:0] boot_error_sources,','    output wire [127:0] storage_selftest_observation,\n    output wire [15:0] boot_error_sources,',name)
  raw=substitute(raw,'  dorado_storage_init #(.ADDR_BITS(RAW_BITS)) u_initialize(','  wire init_write,selftest_failed;\n  wire [15:0] init_data;\n  dorado_storage_selftest #(.ADDR_BITS(RAW_BITS)) u_initialize(',name)
  raw=substitute(raw,'.rsp_valid(init_response),.rsp_ready(init_response_ready));', '.rsp_valid(init_response),.rsp_ready(init_response_ready),\n    .req_write(init_write),.req_data(init_data),.rsp_data(sd),\n    .failed(selftest_failed),.observation(storage_selftest_observation));',name)
  raw=substitute(raw,".req_write(storage_initialized ? rw : 1'b1)",'.req_write(storage_initialized ? rw : init_write)',name)
  raw=substitute(raw,".req_data(storage_initialized ? wd : 16'd0)",'.req_data(storage_initialized ? wd : init_data)',name)
 elif name==TOP:
  raw=substitute(raw,'wire [15:0] boot_error_sources;','wire [15:0] boot_error_sources;\nwire [127:0] storage_selftest_observation;\nwire storage_selftest_view=status[15] && status[14:11]>=4 && status[14:11]<8;\nwire [31:0] storage_selftest_page=storage_selftest_observation[32*status[12:11]+:32];',name)
  raw=substitute(raw,'    .boot_error_sources(boot_error_sources),','    .storage_selftest_observation(storage_selftest_observation),\n    .boot_error_sources(boot_error_sources),',name)
  raw=substitute(raw,'boot_fault_view ? boot_fault_page : speed_monitor_active ?', 'boot_fault_view ? boot_fault_page : storage_selftest_view ? storage_selftest_page : speed_monitor_active ?',name)
 return raw

def stage(parent,qualification,component,out,project):
 assert not out.exists() and project.is_absolute()
 manifest=json.loads((parent/'sdram-burst-native.json').read_text());q=json.loads(qualification.read_text())
 assert q['status']=='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW' and q['passive_boot_fault_observer']
 assert q['source_hashes']==manifest['source_hashes'] and len(manifest['source_hashes'])==500
 c=json.loads((component/'run-state.json').read_text());p=json.loads((component/'provenance.json').read_text())
 assert c['status']=='PASS_RAW_MEMORY_SELFTEST_PIN_COMPONENT'
 assert sha((ROOT/'fpga/mister/dorado_storage_selftest.sv').read_bytes())==p['inputs']['fpga/mister/dorado_storage_selftest.sv']
 for name,digest in p['inputs'].items():assert sha((component/'inputs'/name).read_bytes())==digest
 output={}
 for name,digest in manifest['source_hashes'].items():
  raw=(parent/name).read_bytes();assert sha(raw)==digest,name
  if Path(name).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(manifest['project'].encode(),str(project).encode())
  output[name]=expose(name,raw)
 output[SELFTEST]=(ROOT/'fpga/mister/dorado_storage_selftest.sv').read_bytes()
 output['core/Dorado.qsf']+=('\n# Pre-guest raw-memory diagnostic and verified zero-fill.\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/SELFTEST)+'\n').encode()
 out.mkdir(parents=True)
 for name,raw in output.items():
  p=out/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
 record=dict(manifest,project=str(project),raw_storage_selftest=True,
  selftest_parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),
  selftest_parent_qualification_sha256=sha(qualification.read_bytes()),
  selftest_component_provenance_sha256=sha((component/'provenance.json').read_bytes()),
  selftest_deltas=[WRAPPER,TOP,SELFTEST,'core/Dorado.qsf'],
  source_hashes={n:sha(v)for n,v in output.items()},
  status='STAGED_RAW_STORAGE_SELFTEST_REVIEW_REQUIRED',assembly_permitted=False)
 (out/'sdram-burst-native.json').write_text(json.dumps(record,indent=2)+'\n')
 for name in [Path(__file__).name,'build_r_front_ready_native.py']:(out/name).write_bytes((ROOT/'tools'/name).read_bytes())
 print(record['status'],len(output))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for name in ['parent','qualification','component','output','project']:p.add_argument('--'+name,type=Path,required=True)
 a=p.parse_args();stage(a.parent.resolve(),a.qualification.resolve(),a.component.resolve(),a.output.resolve(),a.project)
