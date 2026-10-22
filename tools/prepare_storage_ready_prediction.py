#!/usr/bin/env python3
"""Prepare a post-edge ready predictor from an exact qualified MSA component.

Original row/column state and raw request/data/output epochs are unchanged.
Only a preview output and a registered, post-edge-equivalent ready are added.
This is a component experiment; no native wrapper or hardware is changed.
"""
import argparse,hashlib,json,re,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(b):return hashlib.sha256(b).hexdigest()
def replace(s,old,new):
 assert s.count(old)==1,(old,s.count(old));return s.replace(old,new)

def address_preview(source):
 s=source.decode()
 s=replace(s,'module cell_MK4096P_addr #','module cell_MK4096P_addr_preview #')
 s=replace(s,'output wire [2*ROW_BITS-1:0] addr','output wire [2*ROW_BITS-1:0] addr,\n    output wire [2*ROW_BITS-1:0] addr_after_edge')
 # Same original RAS/CAS capture equations; preview does not drive stored Q.
 s=replace(s,'  assign addr = {row, col};','''  assign addr = {row, col};
  assign addr_after_edge = {(~p4 & ras_d) ? a : row,
                           (~p15 & cas_d) ? a : col};''')
 return s.encode()

def group_prediction(source):
 s=source.decode();assert 'DEFER_READS'not in s
 s=replace(s,'    input wire host_clk, machine_clk, reset,','''    input wire host_clk, machine_clk, reset,
    input wire machine_step,
    input wire [ADDR_BITS-1:0] addr_after_edge,''')
 old='    assign ready=!reset && !waiting && hit && (!write_pending || write_done);'
 s=replace(s,old,'''    wire ready_reference=!reset && !waiting && hit && (!write_pending || write_done);
    reg ready_q=0;
    assign ready=!reset && ready_q;''')
 anchor='    // These are cell_MK4096P_6\'s cycles, on the paused machine clock.'
 block='''    // Predict exactly the existing host state updates. In particular, a
    // response and a write-completion flag retain their original priority.
    reg pred_waiting,pred_valid,pred_write_done;
    reg [ADDR_BITS-1:0] pred_cache_addr;
    always @*begin
        pred_waiting=waiting;pred_valid=cache_valid;
        pred_cache_addr=cache_addr;pred_write_done=write_done;
        if(!write_pending)pred_write_done=0;
        if(req_valid && req_ready)pred_waiting=1;
        if(waiting && rsp_valid)begin
            pred_waiting=0;pred_valid=1;pred_cache_addr=saved_addr;
            if(saved_write)pred_write_done=1;
        end
    end
    // Keep the machine-step selection AFTER both comparisons. Its falling-
    // edge arrival must not become a half-cycle address/comparator path.
    wire advance_write_pending=!ras_n && !cas_n && !we_n && (cas_d || we_d);
    wire ready_hold_next=!pred_waiting && pred_valid &&
        pred_cache_addr==addr && (!write_pending || pred_write_done);
    wire ready_advance_next=!pred_waiting && pred_valid &&
        pred_cache_addr==addr_after_edge && (!advance_write_pending || pred_write_done);
    always @(posedge host_clk)begin
        if(reset)ready_q<=0;
        else ready_q<=machine_step ? ready_advance_next : ready_hold_next;
    end

'''
 s=replace(s,anchor,block+anchor)
 s=replace(s,'    // synthesis translate_off','''    // synthesis translate_off
    always @(negedge host_clk)if(!reset && ready!==ready_reference)
        $fatal(1,"STORAGE_READY_PREDICTION ready differs from original post-edge equation");''')
 return s.encode()

def board_preview(source):
 s=source.decode();assert s.count('cell_MK4096P_addr #(.ROW_BITS(8))')==16
 s=replace(s,'input wire storage_clk, storage_reset,','input wire storage_clk, storage_reset,\n    input wire storage_step,')
 pattern=r'cell_MK4096P_addr #\(\.ROW_BITS\(8\)\) u_dram_addr_(\d+) \(\n(.*?)\n  \);'
 def cell(m):
  n=m[1];ports=m[2]
  assert ports.count('.addr(dram_addr_'+n+')')==1
  return ('wire [15:0] dram_addr_next_'+n+';\n  cell_MK4096P_addr_preview #(.ROW_BITS(8)) u_dram_addr_'+n+' (\n'+ports+',\n    .addr_after_edge(dram_addr_next_'+n+')\n  );')
 s,n=re.subn(pattern,cell,s,flags=re.S);assert n==16
 pattern=r'(dorado_storage_group #\([^\n]+\) u_storage_\d+ \(\n)(.*?)(\n  \);)'
 def group(m):
  a=re.findall(r'\.addr\(dram_addr_(\d+)\)',m[2]);assert len(a)==1
  return m[1]+m[2]+',\n    .machine_step(storage_step),.addr_after_edge(dram_addr_next_'+a[0]+')'+m[3]
 s,n=re.subn(pattern,group,s,flags=re.S);assert n==16
 return s.encode()

def prepare(reference,out):
 assert not out.exists()
 state=json.loads((reference/'run-state.json').read_text());assert state['status']=='PASS_NATIVE_MSA_BL1_BL8_PHYSICAL_MODEL'
 import gzip
 proof=json.loads(gzip.decompress((reference/'provenance.json.gz').read_bytes()))
 for n,h in proof['inputs'].items():assert sha((reference/'inputs'/n).read_bytes())==h,n
 out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(reference/'inputs',inputs)
 old_group=(inputs/'dorado_storage_group.sv').read_bytes()
 (inputs/'original_storage_group.sv').write_bytes(old_group)
 (inputs/'dorado_storage_group.sv').write_bytes(group_prediction(old_group))
 (inputs/'addr_preview.sv').write_bytes(address_preview((inputs/'cells/cell_MK4096P_addr.v').read_bytes()))
 (inputs/'msa_ddram.v').write_bytes(board_preview((inputs/'msa_ddram.v').read_bytes()))
 s=(inputs/'tb.sv').read_text();s=replace(s,'.storage_clk(clk),.storage_reset(reset),','.storage_clk(clk),.storage_reset(reset),.storage_step(run),')
 (inputs/'tb.sv').write_text(s)
 (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 record=dict(scope=__doc__,parent=str(reference),parent_provenance_sha256=sha((reference/'provenance.json.gz').read_bytes()),original_board_oracle=True,group_count=16,address_bits=16,native_qualified=False,hardware_qualified=False,inputs={str(p.relative_to(inputs)):sha(p.read_bytes())for p in inputs.rglob('*')if p.is_file()})
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('PREPARED_POST_EDGE_READY_COMPONENT',flush=True)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__)
 for n in ['reference','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();prepare(a.reference.resolve(),a.output.resolve())
