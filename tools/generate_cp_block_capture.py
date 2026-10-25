#!/usr/bin/env python3
"""Provide the original asynchronous Block data epoch at TPCBypass capture.

ContA e23.B has asynchronous rMIR Set/sBLOCK Reset. Its normal clocked MIR
data stays in the old snapshot at f15.A capture. CP reset data can instead
reach f15.D before its clock. Default-off generation preserves the old transfer;
the early descriptor requires the original source-route arrival contract.
"""
from pathlib import Path

from prove_cp_prefix_cache import module
from sil_netlist import load_board

VIEW=''' function automatic cp_block_at_tpc(input [155:0] source,input [130:0] p);
  begin
   cp_block_at_tpc=source[93];
   // Original e23.B Set dominates Reset. Retain procedural X semantics.
   // Normal clocked dBlock never bypasses its old capture snapshot here.
   if(CP_TPC_BLOCK_EARLY)begin
    if(source[68] | source[144])cp_block_at_tpc=1'b1;
    else if(p[111])cp_block_at_tpc=1'b0;
   end
  end
 endfunction
'''


def rewrite(raw,early=False):
    atomic=module(raw,'dorado_control_hold_atomic_step')
    terminal=module(raw,'dorado_control_terminal_body')
    changed=terminal
    header='module dorado_control_terminal_body #(parameter integer WAVES=24)('
    new_header='module dorado_control_terminal_body #(parameter integer WAVES=24,parameter integer CP_TPC_BLOCK_EARLY=0)('
    assert changed.count(header)==1
    changed=changed.replace(header,new_header,1)
    signature='function automatic [15:0] task_transfer(input [109:0] q,input [109:0] source,input [21:0] capture,input [130:0] p);'
    new_signature=signature[:-2]+',input tpc_block);'
    assert changed.count(signature)==1
    changed=changed.replace(signature,VIEW+new_signature,1)
    data='if(capture[9])n[10]=(penc==ct) && !source[93];'
    new_data='if(capture[9])n[10]=(penc==ct) && !tpc_block;'
    assert changed.count(data)==1
    changed=changed.replace(data,new_data,1)
    call='task_transfer(q[109:0],state_q[109:0],ac[21:0],task_data_pins(state_q[109:0],old_ap))'
    new_call=call[:-1]+',cp_block_at_tpc(state_q,old_ap))'
    assert changed.count(call)==1
    changed=changed.replace(call,new_call,1)
    restored=changed.replace(new_call,call,1).replace(new_data,data,1).replace(VIEW+new_signature,signature,1).replace(new_header,header,1)
    assert restored==terminal
    old_head='module dorado_control_hold_atomic_step #(parameter integer WAVES=24)('
    new_head='module dorado_control_hold_atomic_step #(parameter integer WAVES=24,parameter integer CP_TPC_BLOCK_EARLY='+str(int(early))+')('
    assert atomic.count(old_head)==1
    a=atomic.replace(old_head,new_head,1)
    replacements=[]
    for waves in ['WAVES','4']:
        old='dorado_control_terminal_body #(.WAVES('+waves+'))'
        new='dorado_control_terminal_body #(.WAVES('+waves+'),.CP_TPC_BLOCK_EARLY(CP_TPC_BLOCK_EARLY))'
        assert a.count(old)==1
        a=a.replace(old,new,1);replacements.append((old,new))
    restored=a.replace(new_head,old_head,1)
    for old,new in replacements:restored=restored.replace(new,old,1)
    assert restored==atomic
    assert raw.count(atomic)==raw.count(terminal)==1
    result=raw.replace(terminal,changed,1).replace(atomic,a,1)
    assert result.replace(changed,terminal,1).replace(a,atomic,1)==raw
    return result


def route(root,gate_ps,ff_ps,cp_minus_root_ps=0):
    specs={}
    for board in ['BaseBd-Rev-Am','ContA-Rev-Cd']:
        b=load_board(str(Path(root)/'chm/sil'/(board+'.dm!1_')/(board+'.wl')))
        specs[board]=(b,{(p['pkg'],p['pin']):n for n,row in b.nets.items() for p in row['pins']})
    checks={
        'BaseBd-Rev-Am':{('j02',3):'BaseBd12.sil+1',('l01',7):'BaseBd12.sil+1',('l01',2):"CLK.ca'"},
        'ContA-Rev-Cd':{('l01',7):"CLK.ca'",('l01',12):"prepreclk'b",('g13',10):"prepreclk'b",
                        ('g13',13):"preclk0'Ca",('d18',7):"preclk0'Ca",('d18',2):"clk0'Cb",('f15',9):"clk0'Cb",
                        ('h03',11):"CPStrb'",('h03',14):'ContA02.sil+1',('h01',15):'ContA02.sil+1',
                        ('h01',10):'sBLOCK',('e23',13):'sBLOCK',('e23',12):'brMIRa',('e23',15):"Block'",
                        ('g15',5):"Block'",('g15',2):'ContA25.sil+5',('f15',7):'ContA25.sil+5',('f15',2):'TPCBypass'}}
    for board,rows in checks.items():
        b,pins=specs[board]
        assert all(pins.get(k)==v for k,v in rows.items())
        for pkg in {p for p,n in rows}:
            assert b.packages[pkg]['type'] in ['MC10210','SE10210','MC10103','MC10172','MC10231','MC1662']
    data=cp_minus_root_ps+3*gate_ps+ff_ps
    clock=5*gate_ps
    return dict(early=int(data<clock),data_arrival_ps=data,clock_arrival_ps=clock,setup_margin_ps=clock-data,
                strict_order=data!=clock,CP_minus_root_ps=cp_minus_root_ps,
                source_routes={board:{pkg+'.'+str(pin):net for (pkg,pin),net in rows.items()} for board,rows in checks.items()},
                scope='Declared original-gate delay profile only; no native analog setup/hold or clock qualification')
