#!/usr/bin/env python3
"""Experimental ordered input/root epochs for ProcL j19's two clock inputs.

Retain before, changed-input/before-root and after-root clocks. Endpoint-only
comparison loses a LOW interval between two HIGH endpoints. Capture from the
data preceding the selected edge. No guest owner, stage, reset or RAM port is
added. Other packages retain their qualified capture logic. Full source-arrival
and unknown-input qualification remain required before native selection.
"""
from pathlib import Path

from sil_netlist import load_board


def route(root, gate_ps, ff_ps):
    """Bind the simultaneous CP/root diagnostic's two D routes to PARC wiring.

    This is a declared uniform-delay contract, not an arbitrary-arrival proof.
    Preserve separate overflow/underflow epochs even though their clocks match.
    """
    checks = {
        'BaseBd-Rev-Am': {
            ('j02',3): 'BaseBd12.sil+1', ('l01',11): 'BaseBd12.sil+1',
            ('l01',13): "CLK.pl'"},
        'ContA-Rev-Cd': {
            ('h03',11): "CPStrb'", ('h03',14): 'ContA02.sil+1',
            ('h01',15): 'ContA02.sil+1', ('h01',10): 'sBLOCK',
            ('g16',13): 'sBLOCK', ('g16',12): 'brMIRc', ('g16',15): "PrBlock'"},
        'ProcL-Rev-Ci': {
            ('l01',11): "CLK.pl'", ('l01',14): "ppClk'c",
            ('g13',5): "ppClk'c", ('g13',3): "PreClock0'D",
            ('j18',5): "PreClock0'D", ('j18',4): "Clock0'Dc",
            ('l10',13): "PrBlock'", ('l10',14): 'StkSela', ('l10',15): "StkSel'a",
            ('h19',7): 'StkSela', ('h19',3): "StkPWriteEn'",
            ('j19',6): "StkPWriteEn'", ('j19',11): "StkPWriteEn'",
            ('j19',9): "Clock0'Dc", ('j19',7): 'ProcL24.sil+10',
            ('j19',10): 'ProcL24.sil+11', ('j19',2): "OVFLerr'", ('j19',15): "UFLerr'",
            ('h17',9): "StkSel'a", ('h17',7): 'ProcL24.sil+10',
            ('k19',12): "StkSel'a", ('k19',9): 'ProcL24.sil+1',
            ('k19',11): 'ProcL24.sil+1', ('k19',6): 'ProcL24.sil+1',
            ('j20',11): 'ProcL24.sil+1', ('j20',12): 'ProcL24.sil+1',
            ('j20',15): 'ProcL24.sil+11'}
    }
    types = {
        'BaseBd-Rev-Am': {'j02':'MC10210','l01':'MC10210'},
        'ContA-Rev-Cd': {'h03':'MC10103','h01':'MC10172','g16':'MC10231'},
        'ProcL-Rev-Ci': {'l01':'SE10210','g13':'SE10210','j18':'SE10210',
                         'l10':'MC1660','h19':'MC10102','j19':'MC10231',
                         'h17':'MC10105','k19':'MC10101','j20':'MC10117'}}
    boards = {}
    for name, rows in checks.items():
        board = load_board(str(Path(root)/'chm/sil'/(name+'.dm!1_')/(name+'.wl')))
        boards[name] = board
        pins = {(p['pkg'],p['pin']):net for net,row in board.nets.items() for p in row['pins']}
        assert all(pins.get(key)==net for key,net in rows.items()), name
        assert all(board.packages[pkg]['type']==kind for pkg,kind in types[name].items()), name
    # The generated machine joins clock nets by signal name: PARC's board
    # connector positions differ (BaseBd C13, ProcL C9). Do not invent a
    # pin-number equality as proof of a physical clock backplane connection.
    for left, right, net in [('ContA-Rev-Cd','ProcL-Rev-Ci',"PrBlock'")]:
        assert boards[left].nets[net]['backplane']==boards[right].nets[net]['backplane']
    clock = 5*gate_ps
    overflow = 4*gate_ps+ff_ps
    underflow = 5*gate_ps+ff_ps
    return dict(clock_arrival_ps=clock, enable_fall_ps=overflow,
                overflow_D_arrival_ps=overflow, underflow_D_arrival_ps=underflow,
                clock_LOW_ps=clock-overflow, overflow_setup_ps=clock-overflow,
                underflow_D_after_clock_ps=underflow-clock,
                diagnostic_epoch_order_valid=overflow<clock<underflow,
                source_routes={name:{pkg+'.'+str(pin):net for (pkg,pin),net in rows.items()}
                               for name,rows in checks.items()},
                package_types=types, simultaneous_CP_and_root_sources=True,
                clock_connector_positions={name:sorted(boards[name].nets["CLK.pl'"]['backplane'])
                                           for name in ['BaseBd-Rev-Am','ProcL-Rev-Ci']},
                clock_connection_is_named_net_fixture=True,
                physical_clock_backplane_route_qualified=False,
                other_data_inputs_stable=True, physical_pulse_acceptance_qualified=False,
                arbitrary_source_arrivals_qualified=False)


def rewrite(raw):
    original=raw;changes=[]
    def change(before,after):
        nonlocal raw
        assert raw.count(before)==1,before
        raw=raw.replace(before,after,1);changes.append((before,after))
    before='wire [56:0] before_clocks,after_clocks;'
    change(before,before+'\n wire [56:0] pre_front_clocks;')
    before='.source_pins(pre_source),.fa_n(pre_fa),.front_data(pre_data));'
    change(before,before.replace('.front_data(pre_data)', '.front_data(pre_data),.clocks(pre_front_clocks)'))
    before='wire [11:0] captures=after_clocks[56:45] & ~before_clocks[56:45];'
    after='''wire [1:0] j19_legacy_edges=after_clocks[56:55] & ~before_clocks[56:55];
 wire [1:0] j19_input_edges=pre_front_clocks[56:55] & ~before_clocks[56:55];
 wire [1:0] j19_root_edges=after_clocks[56:55] & ~pre_front_clocks[56:55];
 wire [11:0] captures={j19_input_edges|j19_root_edges,
                      after_clocks[54:45] & ~before_clocks[54:45]};'''
    change(before,after)
    for bit in range(2):
        before=f'if(captures[{10+bit}]) front_next[{98+bit}]=pre_data[{42+bit}];'
        # Preserve every legacy capture's data. For the extra CP/root pulse,
        # h17's overflow D arrives with h19's falling enable (4G+FF), before
        # Clock0 at5G. Underflow travels through k19+j20 (5G+FF), AFTER Clock0;
        # it must retain the before-epoch D. Shared clock does not imply shared D.
        use_pre='(j19_legacy_edges[0] | j19_root_edges[0])' if bit==0 else 'j19_legacy_edges[1]'
        after=f'if(captures[{10+bit}]) front_next[{98+bit}]={use_pre} ? pre_data[{42+bit}]:before_front_data[{42+bit}];'
        change(before,after)
    restored=raw
    for before,after in reversed(changes):restored=restored.replace(after,before,1)
    assert restored==original
    return raw
