#!/usr/bin/env python3
"""Factor native frontend scan/normal D and clock-enable inference explicitly.

Preserves every physical register, initial value, update priority, scan bit and
interface equation. Only the existing clocked process is transposed into a
combinational enable/value process and enabled registers. No new sequential
state or storage policy. Area/timing benefit requires matched FPGA evidence.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import generate_storage_group as base


def generate():
    original,reference=base.generate();text=original
    start=text.index('    always @(posedge host_clk)begin\n')
    end=text.index('    generate if(ADDR_BITS!=16',start)
    process=text[start:end]
    assert process.startswith('    always @(posedge host_clk)begin\n')and process.endswith('    end\n')
    body=process[len('    always @(posedge host_clk)begin\n'):-len('    end\n')]
    names=[n for n,_ in base.FIELDS];offset={n:i for i,n in enumerate(names)}
    packed='{'+','.join(reversed(names))+'}'
    scan=f'{packed} <= {{state_serial_in,state_bits[STATE_BITS-1:1]}};'
    assert body.count(scan)==1;body=body.replace(scan,'__PHYSICAL_SCAN__')
    replaced=[]
    def assignment(match):
        name,rhs=match[1],match[2];assert name in offset;replaced.append(name)
        return f"begin {name}_next={rhs};update_enable[{offset[name]}]=1'b1;end "
    body=re.sub(r'\b(\w+)\s*<=([^;]+);',assignment,body)
    assert set(replaced)==set(names)and '<='not in body
    next_packed='{'+','.join(n+'_next'for n in reversed(names))+'}'
    body=body.replace('__PHYSICAL_SCAN__',f"begin {next_packed}={{state_serial_in,state_bits[STATE_BITS-1:1]}};update_enable=15'h7fff;end")
    declarations='    reg [14:0] update_enable;\n'
    for name,width in base.FIELDS:
        shape=''if width==1 else f'[{width}-1:0] '
        declarations+='    reg '+shape+name+'_next;\n'
    defaults='        update_enable=0;\n'+''.join(f'        {n}_next=0;\n'for n in names)
    registers='    always @(posedge host_clk)begin\n'+''.join(f'        if(update_enable[{i}]) {n}<={n}_next;\n'for i,n in enumerate(names))+'    end\n'
    replacement=declarations+'    always_comb begin\n'+defaults+body+'    end\n'+registers
    text=text[:start]+replacement+text[end:]
    # The transformation preserves every byte outside the one process.
    assert text.replace(replacement,process,1)==original
    manifest=dict(scope=__doc__,module='dorado_storage_group_snapshot',reference=reference,
                  reference_sha256=hashlib.sha256(original.encode()).hexdigest(),generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                  physical_bits=dict(width8=111,width12=123),private_bits=1,fields=base.FIELDS,
                  transformed_process_sha256=hashlib.sha256(process.encode()).hexdigest(),sequential_fields=names,
                  limits='Component experiment only. No native integration, pause implementation, area/timing or speed claim.')
    return text,manifest


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    source,manifest=generate();(a.output/'storage_group_snapshot.sv').write_text(source);(a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
