#!/usr/bin/env python3
"""Bind the real shared CP bus and its original MIR set decoders.

The native seam has20 direct CP aliases and18 decoded MIR set inputs, all
driven by13 physical CP wires. Normal memory MIR data stays independent.
Extract the reference from original package pins, including the shared h03 OR
gate. This changes no owner, clock, decoder arrival or native RTL selection.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from generate_run_control_reference import model_ports
from prove_native_cp_context import BP
from sil_netlist import load_board
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BOARD_NAMES={'BaseBd':'BaseBd-Rev-Am','ContA':'ContA-Rev-Cd','ContB':'ContB-Rev-Cd'}
CP_NAMES=[*[f'CPOut.{i}' for i in range(9)],*[f"CPAddr.{i}'" for i in range(3)],"CPStrb'"]
SET_NAMES=[*[f'sFF.{i}' for i in range(8)],*[f'sJCN.{i}' for i in range(8)],'sIMRH','sBLOCK']
SOURCE_PACKAGES=['g01','h01','i01','h03']


def wire_name(name):
    return name.replace('.','_').replace("'",'_p_')


def functions(bindings,sets,decode_terms,mutation=None):
    expressions=[]
    for bit,enable,selector in decode_terms:
        a,b=(11,10) if mutation=='swap-selector' else (10,11)
        expressions.append(f"  data[{bit}]=active & cp[{enable}] & ({{cp[{a}],cp[{b}]}}==2'd{selector});")
    active='cp[9] | cp[12]' if mutation=='wrong-enable' else '~(cp[9] | cp[12])'
    lines=[' function automatic [17:0] cp_set_decode(input [12:0] cp);',
           '  reg [17:0] data;reg active;',f'  begin data=0;active={active};',*expressions,
           '   cp_set_decode=data;end',' endfunction',
           ' function automatic [12:0] cp_bus(input [400:0] pins);','  begin']
    for bit,name in enumerate(CP_NAMES):
        index=min(row['index'] for row in bindings if row['net']==name)
        lines.append(f'   cp_bus[{bit}]=pins[{index}];')
    lines+=['  end',' endfunction',
            ' function automatic [400:0] cp_source_pins(input [400:0] pins,input [12:0] cp);',
            '  reg [400:0] result;reg [17:0] data;','  begin result=pins;data=cp_set_decode(cp);']
    for row in bindings:lines.append(f"   result[{row['index']}]=cp[{CP_NAMES.index(row['net'])}];")
    for row in sets:lines.append(f"   result[{row['index']}]=data[{SET_NAMES.index(row['net'])}];")
    lines+=['   cp_source_pins=result;end',' endfunction']
    return '\n'.join(lines)+'\n'


def generate(native,out):
    assert not out.exists()
    metadata=native/'r-front-control-native.json';m=json.loads(metadata.read_text())
    assert sha(native/BP)==m['source_hashes'][BP]
    bp=(native/BP).read_text()
    rows=re.findall(r'assign r_front_live\[(\d+)\]=r_front_pins_(\w+)\[(\d+)\];',bp)
    all_rows=re.findall(r'assign r_front_live\[(\d+)\]=([^;]+);',bp)
    assert len(all_rows)==401 and {int(i) for i,e in all_rows}==set(range(401))
    wired={int(i) for i,b,s in rows}
    assert all(e in ["1'b0","1'b1"] for i,e in all_rows if int(i) not in wired)
    maps={};source_files={BP:sha(native/BP)}
    for board in sorted({b for i,b,s in rows}):
        paths=list((native/'src/fpga/mister').rglob(board+'_full_snapshot.sv'));assert len(paths)==1
        path=paths[0];relative=str(path.relative_to(native));assert sha(path)==m['source_hashes'][relative]
        source_files[relative]=sha(path)
        maps[board]=dict((int(i),v) for i,v in re.findall(r'assign r_front_pins\[(\d+)\]=([^;]+);',path.read_text()))
    cp_by_wire={wire_name(n):n for n in CP_NAMES};set_by_wire={wire_name(n):n for n in SET_NAMES}
    bindings=[];sets=[]
    for i,board,slot in rows:
        wire=maps[board][int(slot)]
        row=dict(index=int(i),board=board,slot=int(slot),wire=wire)
        if wire in cp_by_wire:bindings.append(dict(row,net=cp_by_wire[wire]))
        if board=='ContA' and wire in set_by_wire:sets.append(dict(row,net=set_by_wire[wire]))
    assert len(bindings)==20 and len(sets)==18 and {r['net'] for r in bindings}==set(CP_NAMES)
    assert [r['index'] for r in sets]==list(range(264,282))
    boards={key:load_board(str(ROOT/'chm/sil'/(name+'.dm!1_')/(name+'.wl'))) for key,name in BOARD_NAMES.items()}
    connections=[]
    for name in CP_NAMES:
        base=boards['BaseBd'].nets[name]
        drivers=[p for p in base['pins'] if p['dir']=='out'];assert len(drivers)==1
        assert len(base['backplane'])==1
        users=[]
        for board in {r['board'] for r in bindings if r['net']==name}:
            net=boards[board].nets[name]
            assert net['backplane']==base['backplane'] and not any(p['dir']=='out' for p in net['pins'])
            assert '.%s(%s)'%(wire_name(name),wire_name(name)) in bp
            users.append(board)
        connections.append(dict(net=name,backplane=sorted(base['backplane']),driver=drivers[0],consumers=sorted(users)))
    board=boards['ContA'];selected=set(SOURCE_PACKAGES)
    pin_net={(p['pkg'],p['pin']):n for n,net in board.nets.items() for p in net['pins'] if p['pkg'] in selected}
    kinds={p:board.packages[p]['type'] for p in SOURCE_PACKAGES}
    assert kinds=={'g01':'MC10172','h01':'MC10172','i01':'MC10172','h03':'MC10103'}
    assert pin_net['h03',10]==CP_NAMES[9] and pin_net['h03',11]==CP_NAMES[12]
    assert all(pin_net[p,15]==pin_net['h03',14] for p in ['g01','h01','i01'])
    ports={kind:model_ports(ROOT,kind) for kind in set(kinds.values())}
    for (pkg,pin),name in pin_net.items():
        original=next(p for p in board.nets[name]['pins'] if p['pkg']==pkg and p['pin']==pin)
        assert ports[kinds[pkg]][pin]==('input' if original['dir']=='in' else 'output')
    decode_terms=[]
    for i,name in enumerate(SET_NAMES):
        drivers=[p for p in board.nets[name]['pins'] if p['dir']=='out'];assert len(drivers)==1
        d=drivers[0];pkg=d['pkg'];pin=d['pin'];assert pkg in ['g01','h01','i01']
        assert pin_net[pkg,7]==CP_NAMES[11] and pin_net[pkg,9]==CP_NAMES[10]
        if pin in [13,12,11,10]:enable=pin_net[pkg,14];selector=[13,12,11,10].index(pin)
        else:assert pin in [6,5,4,3];enable=pin_net[pkg,2];selector=[6,5,4,3].index(pin)
        decode_terms.append((i,CP_NAMES.index(enable),selector))
    # Build an independent original-cell reference from the same wire list.
    nets=sorted(set(pin_net.values())|set(CP_NAMES));names={n:'n'+str(i) for i,n in enumerate(nets)}
    original_drivers={n:[p for p in board.nets[n]['pins'] if p['pkg'] in selected and p['dir']=='out'] for n in nets}
    external=[n for n in nets if not original_drivers[n]]
    aux=sorted(set(external)-set(CP_NAMES));assert set(aux)=={'dRun','rStop','RunRefresh'}
    ref=['`default_nettype none','module original_cp_source(input wire [400:0] pins,input wire [12:0] cp,',
         ' input wire [2:0] aux,output reg [400:0] result);']
    ref+=[' wire '+names[n]+'; // '+n for n in nets]
    for n in external:
        value='cp[%d]'%CP_NAMES.index(n) if n in CP_NAMES else 'aux[%d]'%aux.index(n)
        ref.append(' assign '+names[n]+'='+value+';')
    for n in nets:
        if original_drivers[n]:
            ds=[names[n]+'_'+p['pkg']+'_'+str(p['pin']) for p in original_drivers[n]]
            ref+=[' wire '+d+';' for d in ds]+[' assign '+names[n]+'='+' | '.join(ds)+';']
    for pkg in SOURCE_PACKAGES:
        args=[]
        for pin,direction in sorted(ports[kinds[pkg]].items()):
            n=pin_net.get((pkg,pin))
            if n is None:
                if direction=='input':args.append(".p%d(1'b0)"%pin)
            else:args.append('.p%d('%pin+names[n]+('' if direction=='input' else '_'+pkg+'_'+str(pin))+')')
        ref.append(' cell_'+kinds[pkg]+' '+pkg+'('+','.join(args)+');')
    ref.append(' always @*begin result=pins;')
    for row in bindings+sets:ref.append(f"  result[{row['index']}]={names[row['net']]};")
    ref+=[' end','endmodule','`default_nettype wire']
    out.mkdir(parents=True)
    for relative in source_files:
        p=out/'native'/relative;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(native/relative,p)
    for name in BOARD_NAMES.values():
        for ext in ['wl','lc']:
            relative='chm/sil/'+name+'.dm!1_/'+name+'.'+ext
            p=out/'inputs'/relative;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ROOT/relative,p)
    for kind in set(kinds.values()):
        p=out/('cell_'+kind+'.v');shutil.copy2(ROOT/'verilog/cells'/p.name,p)
    (out/'original.sv').write_text('\n'.join(ref)+'\n')
    (out/'functions.sv').write_text(functions(bindings,sets,decode_terms))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    manifest=dict(status='GENERATED_SOURCE_BOUND_NATIVE_CP_INPUTS',scope=__doc__,native_manifest_sha256=sha(metadata),
                  native_source_hashes=source_files,physical_CP_wires=13,CP_bus_names=CP_NAMES,
                  direct_aliases=bindings,decoded_MIR_sets=sets,decoder_terms=decode_terms,
                  CP_connections=connections,packages=kinds,unused_shared_gate_inputs=aux,
                  actual_raw_CPOut8_slot=344,normal_memory_MIR_data_slots=list(range(246,264)),
                  preserved_pin_width=401,source_CP_slots=38,unconstrained_non_CP_slots=363,
                  source_domain_input_pins=376,owned_fixture_external_input_pins=397,
                  other_producer_relationships_qualified=False,
                  state_or_clock_changes=False,arrival_order_or_timing_qualified=False,native_selected=False,
                  files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(manifest['status'],'13 wires /20 aliases /18 set bits; CPOut8=slot344',flush=True)
    return manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['native','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();generate(a.native.resolve(),a.output.resolve())
