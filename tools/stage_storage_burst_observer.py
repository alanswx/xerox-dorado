#!/usr/bin/env python3
"""Add an output-only retained first-read-burst trace to the reviewed memory test.

Only a separate capture always block, output ports/wires and unused diagnostic
pages24..27 differ. Original controller state, delays and data paths are intact.
"""
import argparse,hashlib,json
from pathlib import Path
from stage_boot_fault_observer import WRAPPER,TOP,substitute
from stage_native_sdram_burst import BACKEND
ROOT=Path(__file__).resolve().parents[1]

def sha(raw):return hashlib.sha256(raw).hexdigest()
def expose(name,raw):
 if name==BACKEND:
  raw=substitute(raw,'    output wire snapshot_idle,','    output wire [127:0] burst_observation,\n    output wire snapshot_idle,',name)
  capture='''    // Passive first physical read-burst capture, using the SAME sampled input
    // and event as the cache fill. No control/data feedback from this trace.
    reg [95:0] first_burst_words=0;
    reg first_burst_valid=0;
    assign burst_observation={16'hb178,15'b0,first_burst_valid,first_burst_words};
    always @(posedge clk)begin
        if(reset)begin first_burst_words<=0;first_burst_valid<=0;end
        else if(!first_burst_valid && state==READ_WAIT && delay_count==0)begin
            first_burst_words[12*fill_beat+:12]<=dq_capture[11:0];
            if(fill_beat==7 && !aborted)first_burst_valid<=1;
        end
    end
'''
  raw=substitute(raw,'    reg lookup_hit;',capture+'    reg lookup_hit;',name)
 elif name==WRAPPER:
  raw=substitute(raw,'    output wire [127:0] storage_selftest_observation,','    output wire [127:0] storage_burst_observation,\n    output wire [127:0] storage_selftest_observation,',name)
  raw=substitute(raw,'    .initialized(sdram_initialized),.cache_hit(backing_cache_hit),','    .burst_observation(storage_burst_observation),\n    .initialized(sdram_initialized),.cache_hit(backing_cache_hit),',name)
 elif name==TOP:
  raw=substitute(raw,'wire [127:0] storage_selftest_observation;','wire [127:0] storage_selftest_observation;\nwire [127:0] storage_burst_observation;\nwire storage_burst_view=status[15] && status[14:11]>=8 && status[14:11]<12;\nwire [31:0] storage_burst_page=storage_burst_observation[32*status[12:11]+:32];',name)
  raw=substitute(raw,'    .storage_selftest_observation(storage_selftest_observation),','    .storage_burst_observation(storage_burst_observation),\n    .storage_selftest_observation(storage_selftest_observation),',name)
  raw=substitute(raw,'storage_selftest_view ? storage_selftest_page : speed_monitor_active ?', 'storage_selftest_view ? storage_selftest_page : storage_burst_view ? storage_burst_page : speed_monitor_active ?',name)
 return raw

def stage(parent,qualification,component,out,project):
 assert not out.exists() and project.is_absolute()
 r=json.loads((parent/'sdram-burst-native.json').read_text());q=json.loads(qualification.read_text())
 assert q['status']=='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW' and q['raw_storage_selftest']
 assert q['source_hashes']==r['source_hashes'] and len(r['source_hashes'])==501
 c=json.loads((component/'run-state.json').read_text());p=json.loads((component/'provenance.json').read_text())
 assert c['status']=='PASS_RAW_MEMORY_BURST_TRACE_PIN_COMPONENT'
 for n,h in p['inputs'].items():assert sha((component/'inputs'/n).read_bytes())==h
 output={}
 for n,h in r['source_hashes'].items():
  raw=(parent/n).read_bytes();assert sha(raw)==h,n
  if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(r['project'].encode(),str(project).encode())
  output[n]=expose(n,raw)
 assert sha(output[BACKEND])==p['inputs']['fpga/mister/dorado_storage_sdram_burst.sv']
 out.mkdir(parents=True)
 for n,raw in output.items():
  f=out/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(raw)
 record=dict(r,project=str(project),raw_burst_observer=True,
  burst_observer_parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),
  burst_observer_parent_qualification_sha256=sha(qualification.read_bytes()),
  burst_observer_component_provenance_sha256=sha((component/'provenance.json').read_bytes()),
  burst_observer_deltas=[BACKEND,WRAPPER,TOP],source_hashes={n:sha(v)for n,v in output.items()},
  status='STAGED_PASSIVE_RAW_BURST_OBSERVER_REVIEW_REQUIRED',assembly_permitted=False)
 (out/'sdram-burst-native.json').write_text(json.dumps(record,indent=2)+'\n')
 for n in [Path(__file__).name,'build_r_front_ready_native.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
 print(record['status'],len(output))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','qualification','component','output','project']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();stage(a.parent.resolve(),a.qualification.resolve(),a.component.resolve(),a.output.resolve(),a.project)
