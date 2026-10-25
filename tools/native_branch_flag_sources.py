#!/usr/bin/env python3
"""Bind original ProcH per-task branch flags and their compiled storage cells.

These 80 bits remain outside the 844-bit processor/control owner vector.
This source inventory does not qualify capture phases or interpret a sampled
carry as the carry of a particular instruction.
"""
# Whole original packages include their shared TIOA delay stages.
import gzip
import json
from pathlib import Path
import re

from sil_netlist import load_board
from sil_to_verilog import vname
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BOARD='src/fpga/mister/snapshot_native_processor/ProcH_full_snapshot.sv'
WIRELIST='chm/sil/ProcH-Rev-Ce.dm!1_/ProcH-Rev-Ce.wl'
PACKAGES={'e13':('F10145A',64),'f17':('MC10173',4),
          'g11':('MC10176',6),'h11':('MC10176',6)}


def bindings(source):
    provenance=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    native=source/'inputs'
    assert sha(native/BOARD)==provenance['inputs'][BOARD]
    board=load_board(str(ROOT/WIRELIST));raw=(native/BOARD).read_text()
    pins={(p['pkg'],p['pin']):net for net,row in board.nets.items() for p in row['pins']}
    rows={};sources={WIRELIST:sha(ROOT/WIRELIST),BOARD:sha(native/BOARD)}
    for pkg,(kind,width) in PACKAGES.items():
        assert board.packages[pkg]['type']==kind
        pattern=r'^  cell_'+kind+r'_snapshot u_'+pkg+r' \(.*?^  \);'
        matches=re.findall(pattern,raw,re.M|re.S)
        assert len(matches)==1,(pkg,len(matches))
        actual={int(p):net for p,net in re.findall(r'\.p(\d+)\((\w+)\)',matches[0])}
        original={pin:net for (package,pin),net in pins.items() if package==pkg}
        assert set(actual)==set(original),(pkg,actual,original)
        for pin,net in original.items():
            name=vname(net)
            assert actual[pin] in [name,name+'__drv',name+f'__{pkg}_{pin}'],(pkg,pin,net,actual[pin])
        cells=[name for name in provenance['used_hdl'] if Path(name).stem=='cell_'+kind+'_snapshot']
        assert len(cells)==1,(pkg,cells)
        cell=cells[0];assert sha(native/cell)==provenance['inputs'][cell]
        sources[cell]=sha(native/cell)
        rows[pkg]=dict(kind=kind,stored_bits=width,original_pins=original,
                       native_pins=actual,cell_source=cell,cell_sha256=sha(native/cell))
    cell=(native/rows['e13']['cell_source']).read_text()
    assert 'wire [3:0] a = {p10, p9, p7, p6};' in cell
    assert 'd_lat<={p12,p11,p4,p5};' in re.sub(r'\s+','',cell)
    assert [rows['e13']['original_pins'][p] for p in [10,9,7,6]]==[
        "CurrLast.0'","CurrLast.1'","CurrLast.2'","CurrLast.3'"]
    assert rows['e13']['original_pins'][13]=="BCWrite'"
    assert rows['f17']['original_pins'][7]=="Clock1'Cb"
    assert rows['f17']['original_pins'][15]=='ALUCarry'
    # The saved arithmetic carry/overflow enter the ALU-mode mux via the
    # already-translated g18. They are distinct from f17's visible flags and
    # the per-task e13 file. Do not collapse the three storage boundaries.
    assert 'assign ProcH11_sil_pl_13=r_front_q[91];' in raw
    assert 'assign ProcH11_sil_pl_12=r_front_q[92];' in raw
    mux=re.findall(r'^  cell_MC10158 u_e14 \(.*?^  \);',raw,re.M|re.S)
    assert len(mux)==1 and '.p11(ProcH11_sil_pl_13)' in mux[0] and '.p13(ProcH11_sil_pl_12)' in mux[0]
    return dict(scope=__doc__,packages=rows,source_hashes=sources,
                physical_bits=80,existing_g18_carry_bit=91,existing_g18_overflow_bit=92,
                owner_vector_contains_this_family=False,
                per_task_file_address='Physical slot is logical task XOR15',
                physical_timing_qualified=False,phase_contract_qualified=False)


def alu_producers(source,extended=False):
    """Bind the actual native ALU outputs; do not infer their arrival epochs."""
    provenance=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    native=source/'inputs';rows={};hashes={}
    for board_name,revision,packages in [
        ('ProcH','Ce',['d08','d13','e61','f61']),
        ('ProcL','Ci',['d08','e12','e61','f61']+(['e13','e14','e17']if extended else[]))]:
        wl=f'chm/sil/{board_name}-Rev-{revision}.dm!1_/{board_name}-Rev-{revision}.wl'
        path=f'src/fpga/mister/snapshot_native_processor/{board_name}_full_snapshot.sv'
        assert sha(native/path)==provenance['inputs'][path]
        board=load_board(str(ROOT/wl));raw=(native/path).read_text()
        hashes[wl]=sha(ROOT/wl);hashes[path]=sha(native/path)
        for pkg in packages:
            kind=board.packages[pkg]['type'];model=kind+('_snapshot'if kind in ['MC10176','F10145A']else'')
            pattern=r'^  cell_'+model+r'(?: #\([^\n]+\))? u_'+pkg+r' \(.*?^  \);'
            matches=re.findall(pattern,raw,re.M|re.S);assert len(matches)==1,(board_name,pkg)
            actual={int(p):net for p,net in re.findall(r'\.p(\d+)\((\w+)\)',matches[0])}
            original={p['pin']:n for n,r in board.nets.items()for p in r['pins']if p['pkg']==pkg}
            assert set(actual)==set(original),(board_name,pkg)
            for pin,net in original.items():
                name=vname(net)
                assert actual[pin]in[name,name+'__drv',name+f'__{pkg}_{pin}'],(board_name,pkg,pin,actual[pin],name)
            cells=[name for name in provenance['used_hdl']if Path(name).stem=='cell_'+model]
            assert len(cells)==1,(board_name,pkg,model,cells)
            cell=cells[0];assert sha(native/cell)==provenance['inputs'][cell];hashes[cell]=sha(native/cell)
            rows[board_name+'.'+pkg]=dict(kind=kind,original_pins=original,native_pins=actual,cell_source=cell)
            if board_name=='ProcH'and pkg=='d13':
                assert '#(.LEGACY_B_INVERT(0))'in matches[0]
    mode=rows['ProcL.e12'];assert mode['kind']=='MC10176'
    assert mode['original_pins'][9]=="Clock1'Cb" and mode['original_pins'][15]=='aluM'
    assert [mode['original_pins'][p]for p in [5,6,7,10,11,12]]==[f'ALUFdec.{i}'for i in range(6)]
    bp='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    assert sha(native/bp)==provenance['inputs'][bp];text=(native/bp).read_text();hashes[bp]=sha(native/bp)
    for expression in ['assign aluM = aluM__ProcL;', 'assign aluCout = aluCout__ProcH;',
                       'assign aluOut_eq_0_p_ = aluOut_eq_0_p___ProcH | aluOut_eq_0_p___ProcL;']:
        assert expression in text
    result=dict(packages=rows,source_hashes=hashes,
                alu_mode_is_a_registered_ProcL_e12_output=True,ALU_function_register_bits=6,
                ALU_function_clock="ProcL Clock1'Cb (a board-specific derived clock)",
                global_nonzero_is_wired_OR_of_both_bytes=True,SignedCarry_B_invert=0,
                fixture_direct_early_mask_is_not_a_native_binding=True,
                physical_source_arrivals_qualified=False,native_capture_composition_qualified=False)
    if extended:
        assert rows['ProcL.e13']['kind']==rows['ProcL.e14']['kind']=='F10145A'
        assert rows['ProcL.e17']['original_pins'][2]=='aluC0'
        assert rows['ProcL.f61']['original_pins'][5]=='aluC0'
        result.update(function_RAM_physical_bits=128,extra_capture_register_bits=12,
                      carry_input_has_both_e17_and_f61_drivers=True)
    return result


def operand_producers(source,reference):
    """Bind newly closed physical operand Q pins, without claiming arrivals.

    Use physical output pins rather than snapshot payload positions: payloads
    include extra fabric edge-history/PRECAP state and use different packing.
    """
    from generate_r_front_cone import PIN_Q
    from generate_run_control_reference import model_ports
    from stage_r_front_ready_native import native_net
    provenance=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    native=source/'inputs';rows={};hashes={};physical=set()
    for row in reference['operand_state_layout'].values():
        name,pkg,kind=row['board'],row['package'],row['kind']
        revision={'ProcH':'Ce','ProcL':'Ci'}[name]
        wl=f'chm/sil/{name}-Rev-{revision}.dm!1_/{name}-Rev-{revision}.wl'
        path=f'src/fpga/mister/snapshot_native_processor/{name}_full_snapshot.sv'
        assert sha(native/path)==provenance['inputs'][path]
        board=load_board(str(ROOT/wl));raw=(native/path).read_text()
        assert board.packages[pkg]['type']==kind
        matches=re.findall(r'^  cell_'+kind+r'_snapshot(?: #\([^\n]+\))? u_'+pkg+r' \(.*?^  \);',raw,re.M|re.S)
        assert len(matches)==1,(name,pkg)
        actual={int(pin):net for pin,net in re.findall(r'\.p(\d+)\((\w+)\)',matches[0])}
        original={p['pin']:n for n,r in board.nets.items()for p in r['pins']if p['pkg']==pkg}
        assert set(actual)==set(original),(name,pkg)
        for pin,net in original.items():
            expected=vname(net)
            assert actual[pin]in [expected,expected+'__drv',expected+f'__{pkg}_{pin}'],(name,pkg,pin)
        cells=[n for n in provenance['used_hdl']if Path(n).stem=='cell_'+kind+'_snapshot']
        assert len(cells)==1,(name,pkg,cells)
        cell=cells[0];assert sha(native/cell)==provenance['inputs'][cell]
        width,outputs=PIN_Q[kind];assert width==row['bits']
        directions=model_ports(ROOT,kind)
        pins={}
        for pin,expression in outputs.items():
            bit=re.fullmatch(r'state_q\[(\d+)\]',expression)
            if bit:
                offset=row['global_offset']+int(bit[1]);assert offset not in physical
                assert directions[pin]=='output'
                physical.add(offset);pins[offset]=dict(pin=pin,net=original.get(pin),native_net=actual.get(pin),
                    originally_unconnected=pin not in original,native_physical_pin=f'b_{name}.u_{pkg}.p{pin}')
        assert len(pins)==width,(name,pkg,pins)
        rows[name+'.'+pkg]={**row,'physical_output_pins':pins,'native_pins':actual,
                           'cell_source':cell,'native_instance_parameters':matches[0].splitlines()[0].strip()}
        hashes[wl]=sha(ROOT/wl);hashes[path]=sha(native/path);hashes[cell]=sha(native/cell)
    assert physical==set(range(1064,reference['physical_bits']))
    input_bindings={}
    for token,pin in reference['extra_boundary_pins'].items():
        candidates=[]
        for name,board in reference['boards'].items():
            net=token.removeprefix(name+'.')
            if token.startswith(('ProcH.','ProcL.'))and not token.startswith(name+'.'):continue
            if board['boundary_bindings'].get(net)!=f'pins[{pin}]':continue
            path=f'src/fpga/mister/snapshot_native_processor/{name}_full_snapshot.sv'
            expression=native_net((native/path).read_text(),net)
            candidates.append(dict(board=name,original_net=net,native_net=expression))
        assert candidates,(token,pin)
        input_bindings[token]=dict(pin=pin,producers=candidates)
    return dict(packages=rows,physical_bits=len(physical),source_hashes=hashes,
                additional_projection_inputs=input_bindings,
                physical_outputs_bound=True,physical_arrivals_qualified=False,native_owner_integrated=False,
                snapshot_payload_is_not_physical_Q_layout=True)
