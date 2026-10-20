#!/usr/bin/env python3
"""Complete MSA retaining chip-width cache data and all original FF/history.

The raw16-bit port and original guest pipeline are unchanged. Actual per-MSA
state is1538bits; only unused implementation padding is removed from its
sixteen caches. A new snapshot schema is required before native integration.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import generate_msa_compact as parent
import generate_storage_chipwidth as compact

base=parent.base


def generate():
    board,old,helpers=parent.generate();manifest=copy.deepcopy(old)
    source,frontend=compact.generate()
    helpers['storage_group.sv']=base.rename(source,{'dorado_storage_group_chipwidth':base.MODULE+'__dorado_storage_group_snapshot'})
    board=base.once(board,'.STATE_BITS(1650)','.STATE_BITS(1538)')
    layout=[r for r in manifest['layout']if not r['path'].startswith('u_storage_')];offset=866;groups=[]
    for group in manifest['frontends']:
        row=dict(group);width=row['width'];row['lsb']=offset
        for name,bits in compact.fields(width):layout.append(dict(path=row['package']+'.'+name,width=bits,lsb=offset));offset+=bits
        row['state_bits']=offset-row['lsb'];groups.append(row)
    assert offset==1538 and len(layout)==378
    manifest.update(scope=__doc__,generated_sha256=hashlib.sha256(board.encode()).hexdigest(),
        logical_state_bits=1538,frontend_bits=672,frontends=groups,layout=layout,frontend_manifest=frontend,
        scan=dict(state_bits=1538,words=86,word_bits=18,last_word_bits=8,completion_edges=1538),
        cloned_sources={n:hashlib.sha256(s.encode()).hexdigest()for n,s in helpers.items()},
        native_transport_integration='PENDING: new schema and86-word bounds; no native barrier/full-core qualification')
    return board,manifest,helpers


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);board,manifest,helpers=generate()
    for name,source in {'msa_board.sv':board,**helpers}.items():(a.output/name).write_text(source)
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
