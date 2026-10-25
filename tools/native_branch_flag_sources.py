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
