#!/usr/bin/env python3
"""Wire the corrected read caller and original16-R cells into a private native tree.

Preserve the exact original ROM, device/storage source, boot stimulus and media.
This is a single-event native integration diagnostic. The paired engine remains
in source but is disabled until other boards' phase/event composition is proved.
No FPGA image selection, timing waiver, firmware patch or guest state import.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import time

from generate_native_r_capture import rewire_board
from trace_r_front_transactions import layout
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'


def backplane(raw,fields):
    original=raw;edits=[]
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    first=raw.index('  // Experimental physical state; original visible-R latches remain owners.')
    last=raw.index('  wire [400:0] r_front_pins_ContA;',first)
    old=raw[first:last]
    new='''  // One common caller owner; original16 visible-R cells stay in boards.
  wire [843:0] r_front_q,r_front_held,r_front_next;
  wire [571:0] r_front_lower_q=r_front_held[571:0];
  wire [255:0] r_front_upper_q=r_front_held[843:588];
  wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL;
  wire r_front_clock_before;wire [400:0] r_front_before,r_front_live;
  wire [15:0] r_front_data;wire [1:0] r_front_parity;
  wire [99:0] r_front_events,r_front_repeated;wire r_front_unsettled;
  wire [93:0] r_front_control_flags;wire r_front_shared_mismatch,r_front_alias_ok;
  wire [2469:0] r_first_result,r_last_result;
  wire [35:0] r_capture_frame;wire r_capture_commit;
  wire r_read_needed,r_read_active,r_reading,r_read_valid,r_read_busy,r_read_match;
  wire [1:0] r_pair_forwarded;
'''
    change(old,new)
    for board in ['ProcH','ProcL']:
        marker=f'.r_front_visible_r(r_front_visible_r_{board}),'
        change(marker,marker+'\n    .r_capture_frame(r_capture_frame),.r_capture_commit(r_capture_commit),')
    start=raw.index('  dorado_r_front_control_step r_front_evaluator(')
    end=raw.index(');',start)+2;old=raw[start:end]
    def f(name):
        r=fields[name];return f'r_first_result[{r["high"]}:{r["low"]}]'
    assigns='\n'.join('  assign '+out+'='+f(name)+';'for name,out in [
        ('held_next','r_front_next'),('physical_state','r_front_q'),('file_read_data','r_front_data'),
        ('file_read_parity','r_front_parity'),('clock_events','r_front_events'),('repeated_events','r_front_repeated'),
        ('unsettled','r_front_unsettled'),('shared_state_mismatch','r_front_shared_mismatch'),('control_flags','r_front_control_flags')])
    new=assigns+'''
  dorado_r_front_owned_packet r_front_dispatch(
   .storage_clk(storage_clk),.contb_advance(contb_advance),.use_pair(1'b0),
   .clock_after(CLK_ca_p_),.refresh(1'b0),.reset(storage_reset),.pins_after(r_front_live),
   .r_front_visible_r_ProcH(r_front_visible_r_ProcH),.r_front_visible_r_ProcL(r_front_visible_r_ProcL),
   .first_result(r_first_result),.last_result(r_last_result),.control_ready(control_ready),
   .forwarded(r_pair_forwarded),.r_front_held(r_front_held),.read_accept(1'b1),
   .payload_ready(r_front_alias_ok && !r_front_unsettled && !r_front_shared_mismatch && !(|r_front_repeated)),
   .read_needed(r_read_needed),.stage_active(r_read_active),.stage_reading(r_reading),.stage_valid(r_read_valid),
   .stage_busy(r_read_busy),.stage_tag_match(r_read_match),.owner_commit(r_capture_commit),.native_R_frame(r_capture_frame),
   .native_before_pins(r_front_before),.native_before_root(r_front_clock_before));'''
    change(old,new)
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def prepare(baseline,component,out,host_limit=250000000):
    assert isinstance(host_limit,int) and host_limit>0
    assert not out.exists()
    p=json.loads(gzip.decompress((baseline/'provenance.json.gz').read_bytes()));verify(baseline/'inputs',p['inputs'])
    c=json.loads((component/'run-state.json').read_text());verify(component,c['files'])
    assert c['status']=='PASS_COMBINED_CP_COHERENT_PACKETS'and c['actual_native_visible_R_cells']
    assert c['summary'].startswith('transactions=14884 commits=10423 pairs=4461')
    proof=ROOT/'build/rtl-executive/native-R-capture-proof-v2-20261025/run-state.json'
    assert json.loads(proof.read_text())['status']=='PASS_ORIGINAL_NATIVE_R_CAPTURE_AND_NEGATIVES'
    four=ROOT/'build/rtl-executive/native-R-four-state-icarus-v1-20261025/run-state.json'
    assert json.loads(four.read_text())['status']=='PASS_NATIVE_R_FOUR_STATE_CAPTURE'
    out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(baseline/'inputs',inputs)
    old_candidate=ROOT/'build/rtl-executive/supported-fall-guard-owned-verilator-v1-20261024/candidate.sv'
    _,fields=layout(old_candidate.read_text())
    changes={}
    def write(name,text):
        dest=inputs/name;before=sha(dest)if dest.exists()else None
        dest.parent.mkdir(parents=True,exist_ok=True);dest.write_text(text)
        changes[name]=dict(before_sha256=before,after_sha256=sha(dest))
    for board in ['ProcH','ProcL']:
        name=f'src/fpga/mister/snapshot_native_processor/{board}_full_snapshot.sv'
        write(name,rewire_board((inputs/name).read_text(),board))
    write(BP,backplane((inputs/BP).read_text(),fields))
    used=list(p['used_hdl'])
    for row in c['source_files']:
        name=row['path'];source=component/name
        assert sha(source)==row['output_sha256']
        if name.startswith('adapters/') or name.startswith('inputs/verilog/rtl/'):
            target='src/verilog/rtl/'+Path(name).name
        elif name=='candidate.sv':target='src/verilog/rtl/dorado_r_front_control_step.sv'
        elif name=='dorado_r_front_cone.sv':target='src/verilog/rtl/dorado_r_front_cone.sv'
        elif name in ['caller.sv','forward.sv','dorado_r_interior_read_stage.sv','dorado_r_interior_read_tagged_stage.sv']:
            target='src/verilog/rtl/'+name
        else:continue
        text=source.read_text()
        if name=='caller.sv':
            a='output wire [35:0] native_R_frame);'
            b='output wire [35:0] native_R_frame,output wire [400:0] native_before_pins,output wire native_before_root);'
            assert text.count(a)==1;text=text.replace(a,b,1)
            view=' assign native_before_pins=r_front_before;assign native_before_root=r_front_clock_before;\n'
            assert text.count('endmodule')==1;text=text.replace('endmodule',view+'endmodule',1)
            assert text.replace(view,'',1).replace(b,a,1)==source.read_text()
        write(target,text)
        if target not in used:used.append(target)
    tb=inputs/'harness/tb_native_cedar.sv';raw=tb.read_text()
    old='dut.m.r_front_evaluator.child.child.child.files'
    assert raw.count(old)>=9
    actual=raw.replace(old,'dut.m.r_front_dispatch.files')
    assert actual.replace('dut.m.r_front_dispatch.files',old)==raw
    monitor='''
  // Original four R cells obey the SAME whole-machine publication barrier.
  reg [15:0] native_R_paused;
  always @(posedge host_clk)if(!reset && !control_ready)begin
   native_R_paused={dut.m.r_front_visible_r_ProcL,dut.m.r_front_visible_r_ProcH};
   if(dut.m.r_capture_commit)$fatal(1,"NATIVE_R commit while whole machine unready");
   #1;if({dut.m.r_front_visible_r_ProcL,dut.m.r_front_visible_r_ProcH}!==native_R_paused)
    $fatal(1,"NATIVE_R original cells changed while whole machine held");
  end
'''
    assert actual.count('endmodule')==1;write('harness/tb_native_cedar.sv',actual.replace('endmodule',monitor+'endmodule',1))
    host=(inputs/'harness/host.cpp').read_text();assert 'fopen(medium,"rb")'in host and 'fwrite('not in host
    for n in p['inputs']:
        if '/proms/eprom/'in n:assert sha(inputs/n)==p['inputs'][n]
    (out/'host-cycle-limit.txt').write_text(str(host_limit)+'\n');(out/'window-control.txt').write_text('0 2\n')
    for name in [Path(__file__).name,'generate_native_r_capture.py']:
        shutil.copy2(Path(__file__).with_name(name),inputs/name)
    record={**p,'status':'PREPARED_PRIVATE_NATIVE_CP_READ_AND_ORIGINAL_R_CELLS','scope':__doc__,
            'prepared':time.time(),'inputs':{str(f.relative_to(inputs)):sha(f)for f in inputs.rglob('*')if f.is_file()},
            'used_hdl':used,'baseline_run':str(baseline),'component':str(component),'component_state_sha256':sha(component/'run-state.json'),
            'native_R_proof_sha256':sha(proof),'native_R_four_state_sha256':sha(four),
            'changes':changes,'host_limit':host_limit,'window_control_path':str(out/'window-control.txt'),
            'window_initial_control':'0 2\n','original_ROM_bytes_unchanged':True,
            'actual_four_native_R_cells_retained':True,'one_SRAM_owner':True,'paired_mode_enabled':False,
            'paired_mode_requires_other_board_phase_composition':True,'payload_gate_includes_native_alias_settling_repeat_and_shared_checks':True,
            'full_device_event_retention_qualified':False,'firmware_qualified':False,'physical_timing_qualified':False,
            'experimental_j19_ordered_epochs':c.get('experimental_j19_ordered_epochs',False),
            'j19_full_source_arrival_and_unknown_qualification':False,
            'new_branch_condition_family_integrated':False,
            'assembly_permitted':False,'hardware_changed':False,'original_run_limits_changed':False,'full_goal_active':True}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print(record['status'],len(used),'HDL sources',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['baseline','component','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--host-limit',type=int,default=250000000,help='Fixed bound for a NEW diagnostic; never alters an existing run')
    a=p.parse_args();prepare(a.baseline.resolve(),a.component.resolve(),a.output.resolve(),a.host_limit)
