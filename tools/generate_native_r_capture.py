#!/usr/bin/env python3
"""Extract the four original native visible-R cells without replacing their Qs.

Keep every original physical pin connection and the exact native MC10173 cell.
Only the administrative interface is tied inactive and common host/commit
signals are bound. The capture-frame input represents actual operand D/S/C
epochs, not predicted visible-R output data. Full-board clock wiring is separate.
"""
import hashlib
from pathlib import Path
import re

FROZEN='build/rtl-executive/control-short-native-jumper-boot-v2-20261024/inputs/src'
CELL='fpga/mister/snapshot_native_conta/cell_MC10173_snapshot.v'


def generate(root):
    root=Path(root);base=root/FROZEN;records=[];modules=[]
    for board,start in [('ProcH',0),('ProcL',8)]:
        path=base/f'fpga/mister/snapshot_native_processor/{board}_full_snapshot.sv';text=path.read_text()
        blocks=[]
        for pkg,offset in [('g06',0),('g07',4)]:
            m=re.search(r'  cell_MC10173_snapshot u_'+pkg+r' \(.*?^  \); // MC10173',text,re.M|re.S)
            assert m,(board,pkg)
            old=m[0];pins={int(p):v for p,v in re.findall(r'\.p(\d+)\((\w+)\)',old)}
            expected={7:'PreSHCP_p_B',9:'RbBypassDly'}
            for i,(out,a,b) in enumerate([(1,5,6),(2,3,4),(15,12,13),(14,10,11)]):
                expected[out]=f'R_{start+offset+i:02d}'
                expected[a]=f'dR_{start+offset+i:02d}'
                expected[b]=f'dRm_{start+offset+i:02d}'
            assert pins==expected,(board,pkg,pins)
            # No guest D/S/C connection is edited by this extraction.
            new=old
            for pattern,value in [
                (r'\.state_host_clk\([^)]*\)', '.state_host_clk(clk)'),
                (r'\.state_advance\([^)]*\)', '.state_advance(commit)'),
                (r'\.state_hold\([^)]*\)', ".state_hold(1'b0)"),
                (r'\.state_write\([^)]*\)', ".state_write(1'b0)"),
                (r'\.state_data\([^)]*\)', ".state_data(18'b0)"),
                (r'\.state_result\([^)]*\)', '.state_result()'),
                (r'\.sys_clk\([^)]*\)', '.sys_clk(clk)')]:
                new,n=re.subn(pattern,value,new,count=1);assert n==1
            assert {int(p):v for p,v in re.findall(r'\.p(\d+)\((\w+)\)',new)}==pins
            blocks.append(new)
            records.append(dict(board=board,package=pkg,source=str(path.relative_to(root)),
                                source_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),pins=pins,
                                original_instance_sha256=hashlib.sha256(old.encode()).hexdigest()))
        aliases=[]
        for i in range(8):
            number=start+i
            aliases.extend([f' wire dR_{number:02d}=data_a[{i}];',f' wire dRm_{number:02d}=data_b[{i}];',
                            f' wire R_{number:02d};assign visible[{i}]=R_{number:02d};'])
        modules.append('''module dorado_native_R_BOARD(
 input wire clk,commit,input wire [7:0] data_a,data_b,input wire select_b,clock_n,
 output wire [7:0] visible);
 wire PreSHCP_p_B=clock_n;wire RbBypassDly=select_b;
ALIASES
CELLS
endmodule
'''.replace('BOARD',board).replace('ALIASES','\n'.join(aliases)).replace('CELLS','\n'.join(blocks)))
    wrapper='''`default_nettype none
module dorado_native_r_capture(
 input wire clk,commit,input wire [35:0] frame,output wire [15:0] visible);
 dorado_native_R_ProcH high(clk,commit,frame[7:0],frame[23:16],frame[32],frame[34],visible[7:0]);
 dorado_native_R_ProcL low(clk,commit,frame[15:8],frame[31:24],frame[33],frame[35],visible[15:8]);
endmodule
'''
    cell=base/CELL;raw=cell.read_text()
    assert raw.count('reg [3:0] q;')==1 and 'if (!p7) q <= d;'in raw
    assert 'else if (state_advance) begin'in raw
    return wrapper+'\n'.join(modules)+'`default_nettype wire\n',raw,dict(
        status='GENERATED_ORIGINAL_FOUR_NATIVE_R_CELLS',packages=records,
        native_cell_source=str(cell.relative_to(root)),native_cell_sha256=hashlib.sha256(raw.encode()).hexdigest(),
        state_bits=16,original_pin_connections_unchanged=True,administrative_interface_inactive=True,
        no_reset_or_initialization_added=True,full_board_clock_hookup_qualified=False)


FOLD=''' function automatic [35:0] native_r_fold(input [35:0] first,input [35:0] last);
  reg [35:0] result;
  begin
   result=first;
   for(integer half=0;half<2;half++)begin
    // Procedural clock priority retains the first capture if the later
    // clock is X/Z, like the original cell's if(!C) behavior.
    if(!last[34+half])begin
     result[8*half+:8]=last[8*half+:8];result[16+8*half+:8]=last[16+8*half+:8];
     result[32+half]=last[32+half];
    end
    result[34+half]=first[34+half] & last[34+half];
   end
   native_r_fold=result;
  end
 endfunction
'''


def expose(raw):
    """Expose original operand input epochs only in private packet variants."""
    from prove_cp_prefix_cache import module
    changes=[];original=raw
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);changes.append((a,b))
    for name in ['dorado_r_front_prefix_step','dorado_r_front_parallel_fault_step']:
        old=module(raw,name);actual=old
        marker='output wire [155:0] control_owner,output wire [268:0] live_prefix,input wire pair_fall\n);'
        new=marker.replace('pair_fall\n','pair_fall,output wire [35:0] native_R_frame\n')
        assert actual.count(marker)==1;actual=actual.replace(marker,new,1)
        view=''' // Actual state supplied to the original operand child, before its
 // visible-R mux/latch transfer. No predicted visible-R result is used.
 assign native_R_frame={r_visible_n,capture_state_q[591],capture_state_q[589],
                         capture_state_q[555:540],capture_state_q[571:556]};
'''
        assert actual.count('endmodule')==1
        actual=actual.replace('endmodule',view+'endmodule',1)
        assert actual.replace(view,'',1).replace(new,marker,1)==old
        change(old,actual)
    old=module(raw,'dorado_r_front_packet_step');actual=old;edits=[]
    def edit(a,b):
        nonlocal actual
        assert actual.count(a)==1,a
        actual=actual.replace(a,b,1);edits.append((a,b))
    marker='output wire [31:0] interior_port_address,output wire [3:0] interior_port_select_n);'
    edit(marker,marker.replace(');',',output wire [35:0] native_R_frame);'))
    marker=' wire first_after=use_pair ?'
    edit(marker,FOLD+''' wire [35:0] single_native_R,first_native_R,second_native_R;
 assign native_R_frame=use_pair ? native_r_fold(first_native_R,second_native_R):single_native_R;
'''+marker)
    for n,kind in [('single','parallel_fault'),('first','prefix'),('second','prefix')]:
        start=actual.index(' dorado_r_front_'+kind+'_step '+n+'(');end=actual.index(');',start)+2
        a=actual[start:end];b=a[:-2]+',\n  .native_R_frame('+n+'_native_R));'
        edit(a,b)
    restored=actual
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==old;change(old,actual)
    restored=raw
    for a,b in reversed(changes):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def expose_caller(raw):
    a='output wire owner_commit);';b='output wire owner_commit,output wire [35:0] native_R_frame);'
    c='capture_data,stage_valid,read_prepare,read_needed,interior_address,interior_select);'
    d=c.replace(');',',native_R_frame);')
    assert raw.count(a)==raw.count(c)==1
    actual=raw.replace(a,b,1).replace(c,d,1)
    assert actual.replace(d,c,1).replace(b,a,1)==raw
    return actual


def rewire_board(raw,board):
    """Keep the actual four cells in their native boards; bind capture epochs.

    Only their D/S/C inputs and normal-operation commit enable move. Outputs,
    snapshot/admin ports and every unrelated gate/owner remain unchanged.
    """
    assert board in ['ProcH','ProcL'];original=raw;changes=[];half=0 if board=='ProcH'else 1
    def change(a,b):
        nonlocal raw
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);changes.append((a,b))
    marker='    output wire [7:0] r_front_visible_r,'
    new='    input wire [35:0] r_capture_frame,input wire r_capture_commit,\n'+marker
    change(marker,new)
    for pkg,offset in [('g06',0),('g07',4)]:
        m=re.search(r'  cell_MC10173_snapshot u_'+pkg+r' \(.*?^  \); // MC10173',raw,re.M|re.S)
        assert m
        old=m[0];actual=old
        actual,n=re.subn(r'\.state_advance\(state_guest_advance\)', '.state_advance(r_capture_commit)',actual,count=1);assert n==1
        bindings={7:f'r_capture_frame[{34+half}]',9:f'r_capture_frame[{32+half}]'}
        for bit,(a,b)in enumerate([(5,6),(3,4),(12,13),(10,11)]):
            bindings[a]=f'r_capture_frame[{8*half+offset+bit}]'
            bindings[b]=f'r_capture_frame[{16+8*half+offset+bit}]'
        for pin,value in bindings.items():
            actual,n=re.subn(r'\.p'+str(pin)+r'\([^)]*\)',f'.p{pin}({value})',actual,count=1);assert n==1
        old_outputs={p:v for p,v in re.findall(r'\.p(1|2|15|14)\(([^)]*)\)',old)}
        assert {p:v for p,v in re.findall(r'\.p(1|2|15|14)\(([^)]*)\)',actual)}==old_outputs
        change(old,actual)
    restored=raw
    for a,b in reversed(changes):restored=restored.replace(b,a,1)
    assert restored==original
    return raw
