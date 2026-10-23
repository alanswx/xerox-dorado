#!/usr/bin/env python3
"""Bind CP address/strobe producers to original BaseBoard wiring/firmware.

This identifies the real producer and firmware ordering, not a universal setup/
hold guarantee for every remote CP transaction. It authorizes no timing waiver,
maintenance tie-off, delayed CP strobe or additional guest microinstruction.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
BOARD='chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am'
ROM='chm/dorado/expanded/doradobaserom.dm!12_/'


def audit(out):
    out.mkdir(parents=True,exist_ok=False)
    sources=[BOARD+'.wl',BOARD+'.lc',ROM+'doradoio.mdefs',ROM+'doradocpint.masm']
    hashes={}
    for n in sources:
        p=out/'inputs'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes());hashes[n]=hashlib.sha256(p.read_bytes()).hexdigest()
    board=load_board(str(out/'inputs'/(BOARD+'.wl')))
    checks={}
    def net(name,points):
        n=board.nets[name];actual={(p['pkg'],p['pin'],p['dir'])for p in n['pins']}
        assert set(points)<=actual,(name,points,actual)
        checks[name]=dict(original_net_id=n['id'],pins=n['pins'])
    assert board.packages['f01']['type']==board.packages['f02']['type']=='MC10124'
    assert board.packages['i62']['type']=='MCS6532'
    assert board.packages['e06']['type']==board.packages['d06']['type']=='SN74LS157'
    for i,out_pin,in_pin,mux_out,mux_in,riot_pin in [(0,12,10,7,5,17),(1,3,7,9,11,18),(2,4,5,12,14,19)]:
        net(f"CPAddr.{i}'",[('f01',out_pin,'out')])
        net(f'TCPABus.{i}',[('f01',in_pin,'in'),('e06',mux_out,'out')])
        net(f'MCPABus.{i}',[('e06',mux_in,'in'),('i62',riot_pin,'out')])
    net("CPStrb'",[('f02',12,'out')]);net('TCPStrb',[('f02',10,'in'),('d06',4,'out')])
    net('MCPStrb',[('d06',2,'in'),('i62',24,'out')])
    net('AHasCP',[('e06',1,'in'),('d06',1,'in'),('c20',7,'out'),('c62',13,'out')])
    definitions=(out/'inputs'/(ROM+'doradoio.mdefs')).read_text()
    for expression in ['MCPBusL = 580+PB','MCPABus = 70','MCPStrobe = 1','Control = 0','ABMux0 = 20','ABMux1 = 30']:
        assert expression in definitions,expression
    code=(out/'inputs'/(ROM+'doradocpint.masm')).read_text()
    code='\n'.join(l.split(';',1)[0].rstrip()for l in code.splitlines())
    match=re.search(r'FinishCP:\s*STA\s+MCPBusL\s+INC\s+MCPBusL\s+DEC\s+MCPBusL\s+RTS',code)
    assert match
    result=dict(status='PASS_ORIGINAL_BASEBOARD_CP_SOURCE_AUDIT',scope=__doc__,inputs=hashes,nets=checks,
                local_producer='i62 MCS6532 port B through d06/e06 ownership muxes and f01/f02 TTL-to-ECL translators',
                firmware_sequence=match[0],firmware_address_write_precedes_strobe_RMW=True,
                physical_setup_hold_qualified=False,remote_CP_waveforms_qualified=False,timing_exception_justified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();audit(a.output.resolve())
