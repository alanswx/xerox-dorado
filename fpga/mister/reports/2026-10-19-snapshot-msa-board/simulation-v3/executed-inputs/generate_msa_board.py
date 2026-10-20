#!/usr/bin/env python3
"""Complete physical serial MSA board, from the frozen native source receipts.

All 866 original FF/history and 1824 frontend bits form one 2690-bit chain.
Original pins, package parameters, memory groups and normal equations remain.
An external coordinator must stop guest advances, drain raw memory, then hold
all consumers. This component does not implement native/ARM/service barriers.
Its 2690-cycle access exceeds the old board transport's 2048-cycle deadline.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import generate_msa_ff as ttl
import generate_snapshot_ff as ff
import generate_contb_full as contb
import generate_storage_group as storage
import classify_native_msa as native
from generate_serial_boards import raw_receipt,rename,once

ROOT=Path(__file__).resolve().parents[3]
MODULE='dorado_serial_MSA'
SOURCE='src/build/rtl-executive/native16-tx-cable/msa_ddram.v'
SHA='7ede320ffd5cbca8b8e83d97b7399f0a600f58e25cdaeb9c474238793ce85906'
INSTANCES=re.compile(r'\b(cell_\w+)(\s*(?:#\(.*?\))?\s+)(u_\w+)\s*\((.*?)\)\s*;',re.S)


def original(path):
    return raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-ff','native-source/'+path)


def original_cells():
    board=original(SOURCE).decode()
    return {m[1]+'.v':original('src/verilog/cells/'+m[1]+'.v')for m in INSTANCES.finditer(board)}


def generate():
    raw=original(SOURCE);assert hashlib.sha256(raw).hexdigest()==SHA
    source=raw.decode();text=once(source,'module msa_ddram #(','module '+MODULE+' #(')
    text=once(text,'    input  wire sys_clk,','''    input wire state_guest_advance,state_freeze,state_guest_held,
    input wire state_global_hold,state_global_abort,state_system_reset,
    input wire state_request,state_write,input wire [23:0] state_index,
    input wire [17:0] state_data,
    output wire state_ready,state_frozen,state_done,state_error,
    output wire state_scan_hold_required,state_scan_fault,state_memory_drained,
    output wire [17:0] state_result,
    input  wire sys_clk,''')
    if re.search(r'\b(always\w*|initial|final)\b',ff.masked_comments(source)):
        raise ValueError('unreviewed board-local state process')
    cells=original_cells();clones={};rows=[];layout=[];offset=0
    for kind in native.FIELDS:
        raw_cell=cells[kind+'.v']
        if kind in ttl.FIELDS:helper=ttl.generate_one(kind,raw_cell.decode())
        elif kind in ff.FIELDS:helper=ff.generate_one(kind,raw_cell.decode())
        else:
            assert kind=='cell_MC10176'
            assert raw_cell==(ROOT/'verilog/cells/cell_MC10176.v').read_bytes(),'current MC10176 differs from frozen source'
            old_root=contb.im.ROOT
            try:
                contb.im.ROOT=ROOT;helper=contb.ff_sources()['cell_MC10176_snapshot.v']
            finally:contb.im.ROOT=old_root
        old_module=re.search(r'\bmodule\s+(\w+)',ff.masked_comments(helper))[1]
        new_module=MODULE+'__'+old_module
        clones[kind+'.sv']=rename(helper,{old_module:new_module})
        matches=[m for m in INSTANCES.finditer(source)if m[1]==kind]
        if {m[3]for m in matches}!={'u_'+p for p in native.PACKAGES[kind]}:
            raise ValueError('frozen MSA package layout differs: '+kind)
        for match in matches:
            name=match[3];fields=list(native.FIELDS[kind].items());width=sum(w for _,w in fields)
            # Native.FIELDS is a coverage table; the helper's packing order is
            # the qualified generator's explicit order, not XML alphabetical order.
            if kind in ttl.FIELDS:fields=ttl.fields(kind)
            elif kind in ff.FIELDS:fields=ff.FIELDS[kind]
            else:fields=contb.FIELDS[kind]
            row=dict(package=name,kind=kind,index=len(rows),lsb=offset,width=width,parameters=match[2].strip(),fields=[])
            field_offset=0
            for field,bits in fields:
                row['fields'].append(dict(name=field,width=bits,lsb=field_offset))
                layout.append(dict(path=name+'.'+field,width=bits,lsb=offset+field_offset));field_offset+=bits
            assert field_offset==width<=18
            start=re.search(r'\b'+kind+r'\s*(?:#\(.*?\))?\s+'+name+r'\s*\(',text,re.S)
            assert start;end=text.index(');',start.end())
            instance=text[start.start():end];instance=re.sub(r'^'+kind+r'\b',new_module,instance,count=1)
            at=instance.index('(',instance.index(name))+1
            state_data=f'ff_scan_next[{offset}+:{width}]'
            if width<18:state_data=f"{{{18-width}'b0,{state_data}}}"
            ports=f'''
    .state_host_clk(storage_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_write(scan_step),.state_data({state_data}),
    .state_result(state_ff_result[{len(rows)}]),'''
            instance=instance[:at]+ports+instance[at:];text=text[:start.start()]+instance+text[end:]
            rows.append(row);offset+=width
    assert len(rows)==78 and offset==866
    frontend,frontend_manifest=storage.generate();assert storage.original_source()==original('src/fpga/mister/dorado_storage_group.sv')
    old_module=frontend_manifest['module'];new_module=MODULE+'__'+old_module
    clones['storage_group.sv']=rename(frontend,{old_module:new_module});groups=[]
    for group in range(16):
        name='u_storage_'+str(group);width=12 if group in(0,1,14,15)else 8
        pattern=r'\bdorado_storage_group\s*#\(\.ADDR_BITS\(16\),\s*\.WIDTH\('+str(width)+r'\)\)\s+'+name+r'\s*\('
        start=re.search(pattern,text);assert start,'unreviewed native storage binding: '+name
        end=text.index(');',start.end());instance=text[start.start():end]
        instance=once(instance,'dorado_storage_group',new_module)
        at=instance.index('(',instance.index(name))+1
        serial_in='scan_serial_in'if group==15 else f'state_storage_serial[{group+1}]'
        ports=f'''
    .state_guest_advance(state_guest_advance),.state_hold(state_hold),.state_scan_step(scan_step),
    .state_serial_in({serial_in}),.state_serial_out(state_storage_serial[{group}]),
    .state_frozen(state_storage_frozen[{group}]),.state_drain_ready(state_storage_drained[{group}]),'''
        instance=instance[:at]+ports+instance[at:];text=text[:start.start()]+instance+text[end:]
        start_offset=offset
        for field,bits in storage.fields(width):layout.append(dict(path=name+'.'+field,width=bits,lsb=offset));offset+=bits
        groups.append(dict(package=name,width=width,lsb=start_offset,state_bits=offset-start_offset))
    assert offset==2690 and len(layout)==458
    expected={p.removeprefix('dorado_native_machine.m.b_msa.'):v['width']for p,v in native.expected_fields().items()if p.startswith('dorado_native_machine.m.b_msa.')}
    assert {r['path']:r['width']for r in layout}==expected and len(expected)==len(layout)
    controller=(ROOT/'fpga/mister/snapshot/ff_scan_controller.sv').read_text()
    clones['ff_scan_controller.sv']=controller
    packed='{'+','.join(f"state_ff_result[{r['index']}][{r['width']-1}:0]"for r in reversed(rows))+'}'
    fabric=f'''
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
    text=once(text,'endmodule',fabric+'endmodule')
    manifest=dict(scope=__doc__,module=MODULE,source=SOURCE,original_sha256=SHA,generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                  logical_state_bits=2690,ff_bits=866,frontend_bits=1824,ff=rows,frontends=groups,layout=layout,
                  scan=dict(state_bits=2690,words=150,word_bits=18,last_word_bits=8,completion_edges=2690),
                  parameters=dict(SYSPER=4),frontend_manifest=frontend_manifest,
                  original_cell_sha256={n:hashlib.sha256(raw).hexdigest()for n,raw in cells.items()},
                  cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in clones.items()},
                  native_transport_integration='PENDING: 2690 exceeds existing 2048-cycle budget; no native/global barrier implementation')
    return text,manifest,clones


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);source,manifest,clones=generate()
    (a.output/'msa_board.sv').write_text(source)
    for name,body in clones.items():(a.output/name).write_text(body)
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
