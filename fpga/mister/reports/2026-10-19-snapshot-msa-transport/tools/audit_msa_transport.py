#!/usr/bin/env python3
"""Inverse source proof for the twelve-slot extension of the qualified transport.

Only widths/counts, response budget, slot11 decoding and fail-closed child
predicates may differ. The novel MSA router is covered by behavioral tests
and fresh state inventory, not claimed equivalent to a predecessor router.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_serial_boards import raw_receipt
import test_msa_transport as test

HERE=Path(__file__).resolve().parent


def inverse_fabric(text):
    once=test.once
    text=once(text,'// MSA-capable owner: slots0..10 unchanged; slot11 is eight indexed MSAs.\n','')
    text=once(text,'module dorado_board_state_fabric_msa','module dorado_board_state_fabric_serial')
    text=once(text,'RESPONSE_LIMIT=4096','RESPONSE_LIMIT=2048')
    assert text.count('[11:0] board_')==3
    text=text.replace('[11:0] board_','[10:0] board_')
    text=once(text,'[215:0] board_result','[197:0] board_result')
    text=once(text,"board_frozen===12'hfff","board_frozen===11'h7ff")
    text=once(text,"wire children_holding=(board_hold_required!==12'b0);",'wire children_holding=|board_hold_required;')
    text=once(text,"wire children_faulted=(board_fault!==12'b0);",'wire children_faulted=|board_fault;')
    text=once(text,"   11:index_valid=!(|index[23:11]) && index[7:0]<8'd150;\n",'')
    text=once(text,"(accept && board<12 && index_valid)?(12'b1<<board):12'b0","(accept && board<11 && index_valid)?(11'b1<<board):11'b0")
    return once(text,'if(board>=12)','if(board>=11)')


def inverse_uio(text):
    once=test.once
    text=once(text,'// MSA-capable ABI2: twelve slots, twelve frozen bits, 4096-cycle response budget.\n','')
    text=once(text,'module dorado_board_state_uio_msa','module dorado_board_state_uio_serial')
    text=once(text,'RESPONSE_LIMIT=4096','RESPONSE_LIMIT=2048')
    text=once(text,'[11:0] board_frozen','[10:0] board_frozen')
    text=once(text,'[11:0] frozen_snapshot','[10:0] frozen_snapshot')
    text=once(text,"3:dout<=16'd12;","3:dout<=16'd11;")
    return once(text,"4:dout<={4'b0,frozen_snapshot};","4:dout<={5'b0,frozen_snapshot};")


def audit(fabric,uio):
    root=test.msa.ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-transport'
    for old,raw in [('board_state_fabric_serial.sv',inverse_fabric(fabric)),('board_state_uio_serial.sv',inverse_uio(uio))]:
        qualified=raw_receipt(root,'simulation-v6/candidate/'+old)
        assert raw.encode()==qualified,'ordinary serial transport differs after inverse: '+old
    source,manifest,clones=test.msa.generate();normalized,proof=test.hoist_msa_declarations(source)
    assert proof['byte_identity_after_inverse']
    return dict(status='PASS_SOURCE_ONLY',scope=__doc__,predecessor_transport_byte_identity_after_inverse=True,
                msa_declaration_normalization=proof,msa_normalized_sha256=hashlib.sha256(normalized.encode()).hexdigest())


def controls(fabric,uio):
    result=[]
    for name,target,before,after in [
        ('budget','fabric','RESPONSE_LIMIT=4096','RESPONSE_LIMIT=2048'),
        ('legacy_width','fabric','0:index_valid=!(|index[23:12]);',"0:index_valid=1'b1;"),
        ('msa_hole','fabric',"index[7:0]<8'd150","index[7:0]<8'd151"),
        ('child_unknown','fabric',"children_holding=(board_hold_required!==12'b0)",'children_holding=|board_hold_required'),
        ('parser_schema','uio',"6:if(din!=SCHEMA_ID[63:48])valid_packet<=0;",'6:begin end'),
        ('parser_reset','uio','if(system_reset || transport_reset)begin','if(system_reset)begin'),
        ('frozen_width','uio','reg [11:0] frozen_snapshot','reg [10:0] frozen_snapshot'),
    ]:
        f,u=fabric,uio
        if target=='fabric':f=test.once(f,before,after)
        else:u=test.once(u,before,after)
        try:audit(f,u)
        except AssertionError as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('unreviewed transport source accepted: '+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    fabric=(HERE/'board_state_fabric_msa.sv').read_text();uio=(HERE/'board_state_uio_msa.sv').read_text()
    r=audit(fabric,uio);r['failure_controls']=controls(fabric,uio)
    a.output.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(dict(status=r['status'],failure_controls=len(r['failure_controls']))))
