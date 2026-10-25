#!/usr/bin/env python3
"""Keep asynchronous PrBlock data distinct from ordinary closing MIR data.

Original ContA g16.B drives ProcL l10 -> StkSela -> h19 -> g16.D9. Its CP
reset can arrive before Clock1 while ordinary clocked MIR data arrives later.
Clock0 address data and Clock1 auxiliary data have separate strict route
descriptors. Default-off generation leaves both original data epochs intact.
"""
from pathlib import Path

from prove_cp_prefix_cache import module
from sil_netlist import load_board

VIEW=''' function automatic cp_prblock_at_capture(input [155:0] source,input [400:0] p);
  begin
   cp_prblock_at_capture=source[97];
   // ContA g16.B: brMIRc Set dominates sBLOCK Reset. No normal data bypass.
   if(source[68] | source[144])cp_prblock_at_capture=1'b1;
   else if(p[281])cp_prblock_at_capture=1'b0;
  end
 endfunction
'''


def rewrite(raw,g16_early=False,address_early=False):
    for name in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
        original=module(raw,name);actual=original
        old=f'module {name} #(parameter integer POST_MIR_PINS=2,'
        new=old+'\n parameter integer CP_PRBLOCK_G16_EARLY='+str(int(g16_early))+',CP_PRBLOCK_ADDRESS_EARLY='+str(int(address_early))+','
        assert actual.count(old)==1;actual=actual.replace(old,new,1)
        marker=' wire [21:0] capture_mir=';assert actual.count(marker)==1
        actual=actual.replace(marker,VIEW+''' wire cp_prblock_capture=cp_prblock_at_capture(joint_q,pins_after);
 wire [21:0] original_capture_mir;
 wire [21:0] capture_mir={
  (CP_PRBLOCK_G16_EARLY && clock_after) ? cp_prblock_capture:original_capture_mir[21],original_capture_mir[20:0]};
 assign original_capture_mir=''',1)
        old_front='child_pins(producer_pins(joint_q,pins_after),cq[75:64],cq[97:76],cq);'
        new_front='child_pins(producer_pins(joint_q,pins_after),cq[75:64],{CP_PRBLOCK_ADDRESS_EARLY ? cp_prblock_capture:cq[97],cq[96:76]},cq);'
        assert actual.count(old_front)==1;actual=actual.replace(old_front,new_front,1)
        restored=actual.replace(new_front,old_front,1).replace(VIEW+''' wire cp_prblock_capture=cp_prblock_at_capture(joint_q,pins_after);
 wire [21:0] original_capture_mir;
 wire [21:0] capture_mir={
  (CP_PRBLOCK_G16_EARLY && clock_after) ? cp_prblock_capture:original_capture_mir[21],original_capture_mir[20:0]};
 assign original_capture_mir=''',marker,1).replace(new,old,1)
        assert restored==original
        raw=raw.replace(original,actual,1)
    return raw


def route(root,gate_ps,ff_ps):
    checks={
        'BaseBd-Rev-Am':{('j02',3):'BaseBd12.sil+1',('l01',11):'BaseBd12.sil+1',('l01',13):"CLK.pl'"},
        'ContA-Rev-Cd':{('h03',11):"CPStrb'",('h03',14):'ContA02.sil+1',('h01',15):'ContA02.sil+1',
                        ('h01',10):'sBLOCK',('g16',13):'sBLOCK',('g16',12):'brMIRc',('g16',15):"PrBlock'"},
        'ProcL-Rev-Ci':{('l01',11):"CLK.pl'",('l01',12):"ppClk'b",('f13',5):"ppClk'b",('f13',3):"PreClock1'D",
                        ('j18',9):"PreClock1'D",('j18',13):"Clock1'Da",('g16',13):"Clock1'Da",
                        ('l10',13):"PrBlock'",('l10',14):'StkSela',('h19',7):'StkSela',('h19',3):"StkPWriteEn'",
                        ('g16',9):"StkPWriteEn'",('i15',11):'StkSela',('i15',9):"Clock0'Da",
                        ('g13',3):"PreClock0'D",('j18',5):"PreClock0'D",('j18',3):"Clock0'Da"}}
    boards={}
    for bn,rows in checks.items():
        b=load_board(str(Path(root)/'chm/sil'/(bn+'.dm!1_')/(bn+'.wl')));boards[bn]=b
        pins={(p['pkg'],p['pin']):n for n,row in b.nets.items()for p in row['pins']}
        assert all(pins[k]==v for k,v in rows.items()),bn
    assert boards['ContA-Rev-Cd'].nets["PrBlock'"]['backplane']==boards['ProcL-Rev-Ci'].nets["PrBlock'"]['backplane']
    clock=5*gate_ps;g16_data=4*gate_ps+ff_ps;address_data=3*gate_ps+ff_ps
    return dict(g16_early=int(g16_data<clock),address_early=int(address_data<clock),
                g16_data_arrival_ps=g16_data,address_data_arrival_ps=address_data,clock_arrival_ps=clock,
                g16_margin_ps=clock-g16_data,address_margin_ps=clock-address_data,
                g16_strict=g16_data!=clock,address_strict=address_data!=clock,
                source_routes={b:{pkg+'.'+str(pin):net for(pkg,pin),net in rows.items()}for b,rows in checks.items()},
                normal_clocked_MIR_data_bypassed=False,scope='Declared digital profile, simultaneous CP/root source transition; analog setup/hold not qualified')
