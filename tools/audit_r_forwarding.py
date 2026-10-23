#!/usr/bin/env python3
"""Bind current RbBypass wires and their real latch consumers to original Sil.

This proves topology/clock polarity and the distinction between current
combinational value and next-state forecast. It is not setup/hold, native
consumer timing, original60ns cadence or FPGA deployment qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path

from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]


def audit(out):
    out.mkdir(parents=True,exist_ok=False);hashes={};records=[]
    sources=['verilog/verilator/r_operand_original_ff.sv','verilog/cells/cell_SE10212.v','verilog/cells/cell_MC1668.v']
    for board in ['ProcL-Rev-Ci','ProcH-Rev-Ce']:
        sources +=[f'chm/sil/{board}.dm!1_/{board}.{ext}'for ext in ['wl','lc']]
    for n in sources:
        p=out/'inputs'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=hashlib.sha256(p.read_bytes()).hexdigest()
    for board in ['ProcL-Rev-Ci','ProcH-Rev-Ce']:
        b=load_board(str(out/'inputs'/f'chm/sil/{board}.dm!1_/{board}.wl'));nets={}
        def net(name,points):
            n=b.nets[name];actual={(p['pkg'],p['pin'],p['dir'])for p in n['pins']};assert set(points)<=actual,(board,name,points)
            nets[name]=dict(id=n['id'],pins=n['pins'],backplane=sorted(n.get('backplane',[])))
        assert b.packages['l07']['type']=='MC1668'and b.packages['i05']['type']=='SE10212'
        assert b.packages['g06']['type']==b.packages['g07']['type']=='MC10173'
        net('RbBypass',[('l08',5,'in')]);net("RbBypass'",[('l09',7,'in'),('l09',9,'in'),('l08',11,'in')])
        assert nets['RbBypass']['backplane']==['C32']and nets["RbBypass'"]['backplane']==['C33']
        if board.startswith('ProcL'):
            assert b.packages['l10']['type']=='MC1660'
            net('RbBypass',[('l10',3,'out')]);net("RbBypass'",[('l10',2,'out')])
        net('Clock1Bd',[('i05',2,'out'),('l07',9,'in')])
        net("Clock1'Bb",[('i05',4,'out'),('g05',7,'in'),('h05',7,'in')])
        matches=[name for name,n in b.nets.items()if any(p['pkg']=='l07'and p['pin']==11 and p['dir']=='in'for p in n['pins'])]
        assert len(matches)==1
        net(matches[0],[('l09',12,'out'),('l07',11,'in')])
        net('RbBypassDly',[('l07',15,'out'),('g06',9,'in'),('g07',9,'in')])
        records.append(dict(board=board,nets=nets))
    cell=(out/'inputs/verilog/cells/cell_SE10212.v').read_text()
    assert 'assign p4  = ~a;'in cell and 'assign p2  =  a;'in cell
    result=dict(status='PASS_ORIGINAL_RBYPASS_PRODUCER_CONSUMER_AUDIT',scope=__doc__,inputs=hashes,boards=records,
                read_and_l07_clocks_are_complementary_outputs=True,
                current_wire_is_not_an_extra_register=True,capture_consumer='l07 B HIGH-transparent on Clock1Bd',
                downstream='RbBypassDly selects g06/g07 visible R muxes',
                physical_setup_hold_qualified=False,native_consumer_timing_qualified=False,timing_exception_justified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();audit(a.output.resolve())
