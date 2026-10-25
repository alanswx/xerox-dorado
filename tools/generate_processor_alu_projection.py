#!/usr/bin/env python3
"""Project actual ALU/phase outputs from a supplied physical processor state.

Share original gate wiring, replace storage with read-only physical Q views,
and remove illustrative delays. This computes an early producer frame without
committing any guest state. Delayed original primitives remain the oracle.
"""
import json
from pathlib import Path
import re

from generate_r_front_cone import PIN_Q
from test_external_r_packet import sha

GRAPHS=['processor_alu_reference','processor_alu_proch','processor_alu_procl',
        'r_front_control_conta','contb_hold_reference','contb_hold_memc','contb_mir_reference']


def generate(reference,out):
    assert not out.exists();out.mkdir(parents=True);timed=reference/'timed';chunks=[]
    source={};primitive_headers={}
    for path in sorted(timed.iterdir()):
        if path.name.startswith('cell_')and path.suffix=='.v':
            raw=path.read_text();raw=re.sub(r'assign\s*#\(\d+\)\s*','assign ',raw)
            assert not re.search(r'#\(\d+\)',raw),path
            chunks.append(raw.replace('cell_','alu_comb_'));source[path.name]=sha(path)
        elif path.suffix=='.sv':
            raw=path.read_text()
            for name,header in re.findall(r'module (original_\w+)(.*?)\);',raw,re.S):
                if name in primitive_headers:assert primitive_headers[name]==header
                primitive_headers[name]=header
    needed=set()
    for name in GRAPHS:
        path=timed/(name+'.sv');raw=path.read_text();source[path.name]=sha(path)
        needed.update(re.findall(r'\b(original_\w+) \w+\(',raw))
    for original in sorted(needed):
        kind=original.removeprefix('original_')
        if kind in ['F10000_ready','F10000_gated']:kind='F10000'
        width,outputs=PIN_Q[kind]
        header=primitive_headers[original].replace('output reg','output wire')
        raw='module '+original.replace('original_','alu_sampled_',1)+header+');\n assign state_q=fixture_state;\n'
        if kind in ['MC10231','SE10231']:raw+=' wire ca=p9|p6,cb=p9|p11;\n'
        if kind=='F10000':raw+=' wire clk=p4|p13;\n'
        if kind=='F10145A':raw+=' wire [3:0] address={p10,p9,p7,p6};\n'
        for pin,expression in outputs.items():raw+=f' assign p{pin}={expression};\n'
        chunks.append(raw+'endmodule\n')
    for name in GRAPHS:
        raw=(timed/(name+'.sv')).read_text().replace('original_','alu_sampled_').replace('cell_','alu_comb_')
        for graph in sorted(GRAPHS,key=len,reverse=True):
            raw=re.sub(r'\b'+graph+r'\b','alu_projection_'+graph,raw)
        chunks.append(raw)
    path=out/'projection.sv';path.write_text('\n'.join(chunks))
    manifest=dict(scope=__doc__,source_files=source,stored_state_bits=json.loads((reference/'manifest.json').read_text())['physical_bits'],
                  source_manifest_sha256=sha(reference/'manifest.json'),source_gate_wiring_shared=True,
                  projection_sha256=sha(path),state_updates=False,native_qualified=False)
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');return manifest
