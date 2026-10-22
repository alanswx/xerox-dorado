#!/usr/bin/env python3
"""Distribute the common step selection across one MSA readiness reduction.

AND_i(step ? advance_i : hold_i) equals
step ? AND_i(advance_i) : AND_i(hold_i). Register the latter on the SAME
host edge, retaining original address/data and request/response epochs.
The individual ready registers remain simulation oracles and may synthesize
away. No CPU, cache, Map, ECC, SDRAM protocol or guest timing is changed.
"""
import argparse,hashlib,json,re,shutil
from pathlib import Path
from prepare_storage_ready_prediction import replace

def sha(b):return hashlib.sha256(b).hexdigest()

def group_outputs(raw):
 s=raw.decode()
 s=replace(s,'    output wire ready,','    output wire ready,\n    output wire ready_hold_next,ready_advance_next,')
 s=replace(s,'    wire ready_hold_next=','    assign ready_hold_next=')
 s=replace(s,'    wire ready_advance_next=','    assign ready_advance_next=')
 return s.encode()

def board_reduction(raw):
 s=raw.decode();assert s.count('  assign storage_ready = &storage_group_ready;')==1
 s=replace(s,'  assign storage_ready = &storage_group_ready;','''  wire [15:0] ready_hold_next,ready_advance_next;
  reg ready_q=0;
  // Distribute the common step mux AFTER the two16-way reductions.
  // This is the same post-edge equation, not another pipeline stage.
  wire ready_hold_all=&ready_hold_next;
  wire ready_advance_all=&ready_advance_next;
  always @(posedge storage_clk)begin
    if(storage_reset)ready_q<=0;
    else ready_q<=storage_step ? ready_advance_all : ready_hold_all;
  end
  assign storage_ready = !storage_reset && ready_q;
  // synthesis translate_off
  always @(negedge storage_clk)if(!storage_reset && storage_ready!==(&storage_group_ready))
    $fatal(1,"STORAGE_BOARD_READY_REDUCTION differs from original per-group post-edge equation");
  // synthesis translate_on''')
 pattern=r'\.ready\(storage_group_ready\[(\d+)\]\),'
 s,n=re.subn(pattern,lambda m:m[0]+f'\n    .ready_hold_next(ready_hold_next[{m[1]}]),.ready_advance_next(ready_advance_next[{m[1]}]),',s)
 assert n==16,n
 return s.encode()

def prepare(parent,out,fixed_capture=False):
 assert not out.exists()
 p=json.loads((parent/'provenance.json').read_text());s=json.loads((parent/'run-state.json').read_text())
 assert s['status']=='PASS_POST_EDGE_READY_ORIGINAL_MSA_COMPONENT'and len(s['actions'])==8
 for n,h in p['inputs'].items():assert sha((parent/'inputs'/n).read_bytes())==h,n
 trace=parent/'positive-raw.csv';assert trace.read_bytes()==(Path(p['parent'])/'burst-raw.csv').read_bytes()
 out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(parent/'inputs',inputs)
 for n,fn in [('msa_ddram.v',board_reduction),('dorado_storage_group.sv',group_outputs)]:
  f=inputs/n;f.write_bytes(fn(f.read_bytes()))
 if fixed_capture:
  from storage_capture_decode import decode_capture,allow_66mhz
  f=inputs/'dorado_storage_sdram_burst.sv';f.write_bytes(allow_66mhz(decode_capture(f.read_bytes())))
  (inputs/'storage_capture_decode.py').write_bytes(Path(__file__).with_name('storage_capture_decode.py').read_bytes())
 (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 record=dict(scope=__doc__,parent=str(parent),parent_provenance_sha256=sha((parent/'provenance.json').read_bytes()),
  trace_reference=str(trace),trace_sha256=sha(trace.read_bytes()),group_count=16,registered_board_ready=1,
  added_host_cycles=0,original_board_oracle=True,native_qualified=False,hardware_qualified=False,
  inputs={str(f.relative_to(inputs)):sha(f.read_bytes())for f in inputs.rglob('*')if f.is_file()})
 if fixed_capture:record['fixed_lane_capture']=True
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('PREPARED_SAME_EDGE_BOARD_READY_REDUCTION',flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['parent','output']:p.add_argument('--'+n,type=Path,required=True)
 p.add_argument('--fixed-capture',action='store_true')
 a=p.parse_args();prepare(a.parent.resolve(),a.output.resolve(),a.fixed_capture)
