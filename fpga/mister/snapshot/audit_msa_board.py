#!/usr/bin/env python3
"""Invert only MSA administrative edits; compare helpers to committed qualifications.

Original board bytes, package pins/parameters and raw-memory grouping must
return exactly. Helper alpha-renames must preserve their already-qualified
bodies. This is a source proof, not core timing or native service integration.
"""
import argparse
import json
from pathlib import Path
import re
import generate_msa_board as gen
from generate_serial_boards import raw_receipt,rename


def once(text,before,after):
    if text.count(before)!=1:raise ValueError('administrative source shape differs: '+before[:80])
    return text.replace(before,after,1)


def helper_receipts():
    reports=gen.ROOT/'fpga/mister/reports'
    global_=reports/'2026-10-19-snapshot-serial-board-global'
    result={}
    for kind,board in [('cell_MC10141','ContA'),('cell_F10016','ContA'),('cell_MC10176','ContB')]:
        old=raw_receipt(global_,f'generation-v3/{board}__{kind}_snapshot.v').decode()
        result[kind+'.sv']=rename(old,{f'dorado_serial_{board}__{kind}_snapshot':f'{gen.MODULE}__{kind}_snapshot'})
    old=raw_receipt(global_,'generation-v3/ContB__ff_scan_controller.sv').decode()
    result['ff_scan_controller.sv']=rename(old,{'dorado_serial_ContB__dorado_ff_scan_controller':'dorado_ff_scan_controller'})
    for kind in gen.ttl.FIELDS:
        old=raw_receipt(reports/'2026-10-19-snapshot-msa-ff',f'simulation-v2/candidate/{kind}_msa_snapshot.sv').decode()
        result[kind+'.sv']=rename(old,{kind+'_msa_snapshot':gen.MODULE+'__'+kind+'_msa_snapshot'})
    old=raw_receipt(reports/'2026-10-19-snapshot-storage-group','simulation-v4-native/candidate/storage_group_snapshot.sv').decode()
    result['storage_group.sv']=rename(old,{'dorado_storage_group_snapshot':gen.MODULE+'__dorado_storage_group_snapshot'})
    return result


def audit(board,manifest,clones):
    if clones!=helper_receipts():raise ValueError('helper body differs from committed qualification')
    # Check shared owner/link hooks independently before erasing the fabric.
    for required in (
        'wire state_hold=state_global_hold || (state_freeze && state_guest_held) || state_scan_hold_required;',
        'assign state_ready=state_frozen && scan_ready && !state_global_abort;',
        '.freeze(state_freeze && !state_global_abort)',
        'ff_scan_next={state_storage_serial[0],ff_scan_bits[865:1]}',
        'dorado_ff_scan_controller #(.STATE_BITS(2690)) u_ff_scan(',
        '.request(state_request && state_ready)',
    ):
        if board.count(required)!=1:raise ValueError('global owner/chain hook differs: '+required)
    text=once(board,'module '+gen.MODULE+' #(','module msa_ddram #(')
    ports='''    input wire state_guest_advance,state_freeze,state_guest_held,
    input wire state_global_hold,state_global_abort,state_system_reset,
    input wire state_request,state_write,input wire [23:0] state_index,
    input wire [17:0] state_data,
    output wire state_ready,state_frozen,state_done,state_error,
    output wire state_scan_hold_required,state_scan_fault,state_memory_drained,
    output wire [17:0] state_result,
'''
    text=once(text,ports,'')
    marker='\n  wire [17:0] state_ff_result[0:77];'
    if text.count(marker)!=1:raise ValueError('missing/duplicate state fabric')
    start=text.index(marker);end=text.index('endmodule',start)
    packed='{'+','.join(f"state_ff_result[{r['index']}][{r['width']-1}:0]"for r in reversed(manifest['ff']))+'}'
    allowed=f'''
  wire [17:0] state_ff_result[0:77];
  wire [865:0] ff_scan_bits={packed};
  wire [865:0] ff_scan_next={{state_storage_serial[0],ff_scan_bits[865:1]}};
  wire [15:0] state_storage_serial,state_storage_frozen,state_storage_drained;
  wire scan_serial_in,scan_step,scan_ready,scan_busy;
  wire state_hold=state_global_hold || (state_freeze && state_guest_held) || state_scan_hold_required;
  reg state_boundary_ready=1'b0;
  always @(posedge storage_clk)state_boundary_ready<=state_hold && !state_system_reset;
  assign state_frozen=state_boundary_ready && (&state_storage_frozen) && !state_system_reset;
  assign state_ready=state_frozen && scan_ready && !state_global_abort;
  assign state_memory_drained=&state_storage_drained;
  dorado_ff_scan_controller #(.STATE_BITS(2690)) u_ff_scan(
    .clk(storage_clk),.reset(state_system_reset),.freeze(state_freeze && !state_global_abort),
    .guest_held(state_guest_held),.request(state_request && state_ready),.write(state_write),
    .index(state_index),.data(state_data),.serial_out(ff_scan_bits[0]),
    .serial_in(scan_serial_in),.scan_step(scan_step),.ready(scan_ready),
    .hold_required(state_scan_hold_required),.busy(scan_busy),.done(state_done),.error(state_error),
    .fault(state_scan_fault),.result(state_result));
'''
    if text[start:end]!=allowed:raise ValueError('unreviewed administrative fabric logic')
    text=text[:start]+text[end:]
    for row in manifest['ff']:
        kind,name,width,offset,index=row['kind'],row['package'],row['width'],row['lsb'],row['index']
        old_module=re.search(r'\bmodule\s+(\w+)',gen.ff.masked_comments(clones[kind+'.sv']))[1]
        text=once(text,old_module+(' '+row['parameters']if row['parameters']else'')+' '+name+' (',
                       kind+(' '+row['parameters']if row['parameters']else'')+' '+name+' (')
        state_data=f'ff_scan_next[{offset}+:{width}]'
        if width<18:state_data=f"{{{18-width}'b0,{state_data}}}"
        hook=f'''
    .state_host_clk(storage_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(scan_step),.state_data({state_data}),
    .state_result(state_ff_result[{index}]),'''
        text=once(text,hook,'')
    for i in range(16):
        width=12 if i in(0,1,14,15)else 8
        text=once(text,f'{gen.MODULE}__dorado_storage_group_snapshot #(.ADDR_BITS(16), .WIDTH({width})) u_storage_{i} (',
                       f'dorado_storage_group #(.ADDR_BITS(16), .WIDTH({width})) u_storage_{i} (')
        serial_in='scan_serial_in'if i==15 else f'state_storage_serial[{i+1}]'
        hook=f'''
    .state_guest_advance(state_guest_advance),.state_hold(state_hold),.state_scan_step(scan_step),
    .state_serial_in({serial_in}),.state_serial_out(state_storage_serial[{i}]),
    .state_frozen(state_storage_frozen[{i}]),.state_drain_ready(state_storage_drained[{i}]),'''
        text=once(text,hook,'')
    if text.encode()!=gen.original(gen.SOURCE):raise ValueError('ordinary board pins/parameters/processes differ after inverse')
    return dict(status='PASS_SOURCE_ONLY',scope=__doc__,ordinary_board_byte_identity_after_inverse=True,
                qualified_helper_bodies=8,physical_bits=2690,original_packages=78,frontend_groups=16)


def controls(board,manifest,clones):
    results=[]
    for name,before,after in [
        ('force_hold','state_global_hold || (state_freeze', '(state_freeze'),
        ('abort','state_freeze && !state_global_abort','state_freeze'),
        ('chain','state_storage_serial[0],ff_scan_bits[865:1]',"1'b0,ff_scan_bits[865:1]"),
        ('pin','.p4(MemAd_1)', '.p4(MemAd_2)'),
        ('ordinary_extra','endmodule','assign unexpected=state_request;\nendmodule'),
        ('field_hook','.state_advance(state_guest_advance)',".state_advance(1'b1)"),
    ]:
        if before not in board:
            if name=='pin':
                before='.a({';after='.a(~{'
            if before not in board:raise ValueError('missing source control target: '+name)
        broken=board.replace(before,after,1)
        try:audit(broken,manifest,clones)
        except ValueError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged MSA source accepted: '+name)
    broken=dict(clones);name='cell_SN74166.sv';broken[name]=once(broken[name],'assign p13 = q[7];','assign p13 = q[0];')
    try:audit(board,manifest,broken)
    except ValueError as error:results.append(dict(case='helper',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('damaged qualified helper accepted')
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    board,manifest,clones=gen.generate();result=audit(board,manifest,clones);result['failure_controls']=controls(board,manifest,clones)
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],qualified_helper_bodies=8,failure_controls=len(result['failure_controls']))))
