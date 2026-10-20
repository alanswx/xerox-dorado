#!/usr/bin/env python3
"""Complete MSA compact bridge candidate; original 866 FF/history bits retained.

Only the implementation bridge registers change. Original address latch
equations remain byte-identical; a pure combinational output predicts actual
row/column changes on the next guest edge. Native transport/schema is separate.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import generate_msa_board as base
import generate_storage_compact as compact
from test_msa_transport import hoist_msa_declarations


def generate():
    board,old,helpers=base.generate();board,_=hoist_msa_declarations(board)
    manifest=copy.deepcopy(old);reference_board=board
    front,front_manifest=compact.generate()
    front=base.rename(front,{'dorado_storage_group_compact':base.MODULE+'__dorado_storage_group_snapshot'})
    helpers['storage_group.sv']=front
    addr=helpers['cell_MK4096P_addr.sv'];reference_addr=addr
    addr=base.once(addr,'    output wire [2*ROW_BITS-1:0] addr\n',
                   '    output wire addr_will_change,\n    output wire [2*ROW_BITS-1:0] addr\n')
    prediction='  assign addr_will_change=((~p4 & ras_d) && (row != a)) || ((~p15 & cas_d) && (col != a));\n'
    addr=base.once(addr,'  assign addr = {row, col};\n','  assign addr = {row, col};\n'+prediction)
    assert addr.replace('    output wire addr_will_change,\n','',1).replace(prediction,'',1)==reference_addr
    helpers['cell_MK4096P_addr.sv']=addr
    for i in range(16):
        board=base.once(board,f'  wire [15:0] dram_addr_{i};',f'  wire [15:0] dram_addr_{i};wire dram_change_{i};')
        board=base.once(board,f'    .addr(dram_addr_{i})\n',f'    .addr(dram_addr_{i}),.addr_will_change(dram_change_{i})\n')
        board=base.once(board,f'.addr(dram_addr_{i}),\n',f'.addr(dram_addr_{i}),.addr_will_change(dram_change_{i}),\n')
    board=base.once(board,'.STATE_BITS(2690)','.STATE_BITS(1650)')
    # No existing pin/parameter/equation is changed by the administrative wiring.
    inverse=board.replace('.STATE_BITS(1650)','.STATE_BITS(2690)')
    for i in range(16):
        inverse=inverse.replace(f';wire dram_change_{i};',';').replace(f',.addr_will_change(dram_change_{i})','')
    assert inverse==reference_board
    layout=[r for r in manifest['layout'] if not r['path'].startswith('u_storage_')];offset=866;groups=[]
    for group in manifest['frontends']:
        row=dict(group);width=row['width'];row['lsb']=offset
        for name,bits in compact.fields(width):layout.append(dict(path=row['package']+'.'+name,width=bits,lsb=offset));offset+=bits
        row['state_bits']=offset-row['lsb'];groups.append(row)
    assert offset==1650 and len(layout)==378
    manifest.update(scope=__doc__,reference_generated_sha256=hashlib.sha256(reference_board.encode()).hexdigest(),
        generated_sha256=hashlib.sha256(board.encode()).hexdigest(),logical_state_bits=1650,ff_bits=866,frontend_bits=784,
        frontends=groups,layout=layout,frontend_manifest=front_manifest,
        scan=dict(state_bits=1650,words=92,word_bits=18,last_word_bits=12,completion_edges=1650),
        cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in helpers.items()},
        native_transport_integration='PENDING: new actual-state schema/word bounds required, no complete/native snapshot qualification')
    return board,manifest,helpers


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);board,manifest,helpers=generate()
    for name,source in {'msa_board.sv':board,**helpers}.items():(a.output/name).write_text(source)
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
