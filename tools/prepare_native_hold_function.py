#!/usr/bin/env python3
"""Extract the actual staged Hold386 combinational function at its audited cut.

Keep exact primitive module bytes and needed output equations/bindings. Native
unwired MECL inputs are explicitly LOW, as physical pulldowns and the native
two-state build require; report every such pin. This is not a new protocol
premise. The independent raw producer remains the four-state buffer fallback.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from audit_native_memc_hold_producer import BOARD, BACKPLANE
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
HELPER='verilog/rtl/dorado_memc_hold_buffer.sv'


def prepare(source,out):
    assert not out.exists()
    r=json.loads((source/'results.json').read_text())
    assert r['status']=='AUDITED_ACTUAL_MEMC_HOLD386_COMBINATIONAL_BOUNDARIES'
    stage=Path(r['stage']);verify(stage,r['source_hashes'])
    assert r['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    leaves=sorted(r['boundary_nets']);visited=r['visited_nets'];assert len(leaves)==132
    bank={};paths={}
    for p in r['source_hashes']:
        if p in [BOARD,BACKPLANE]:continue
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule',(stage/p).read_text(),re.M|re.S):
            assert match[1]not in bank;bank[match[1]]=match[0]+'\n';paths[match[1]]=p
    cells={}
    for net,row in visited.items():
        if row['kind']=='cell':cells.setdefault(row['instance'],{})[row['pin']]=net
    body=['`default_nettype none',f'module dorado_memc_hold_function(input wire [{len(leaves)-1}:0] descriptor,output wire hold_value);']
    for net in sorted(set(leaves)|set(visited)):body.append(' wire '+net+';')
    for i,net in enumerate(leaves):body.append(f' assign {net}=descriptor[{i}];')
    used=set();pulldowns=[]
    for net,row in visited.items():
        if row['kind']=='assignment':body.append(' assign '+net+'='+row['expression']+';')
    for name,outputs in sorted(cells.items()):
        cell=r['selected_instances'][name];kind=cell['module'];used.add(kind)
        pins=set().union(*(set(cell['expanded_output_inputs'][p])for p in outputs))
        args=['.'+p+'('+n+')'for p,n in outputs.items()]
        for p in sorted(pins):
            expr=cell['pins'].get(p)
            if not expr:
                pulldowns.append(dict(instance=name,module=kind,pin=p));expr="1'b0"
            args.append('.'+p+'('+expr+')')
        body.append(' '+kind+' '+name+'('+','.join(args)+');')
    body+=[' assign hold_value=MemC15_sil_pl_2;','endmodule','`default_nettype wire']
    # Every original visited assignment/output and boundary is emitted exactly
    # once. No sequential primitive is part of the extracted function.
    for kind in used:
        assert not re.search(r'\b(?:always|initial|function|task)\b',re.sub(r'//[^\n]*','',bank[kind])),kind
    out.mkdir(parents=True)
    (out/'function.sv').write_text('\n'.join(body)+'\n')
    for kind in sorted(used):(out/(kind+'.v')).write_text(bank[kind])
    shutil.copy2(ROOT/HELPER,out/'buffer.sv');shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='PREPARED_SOURCE_BOUND_NATIVE_HOLD_FUNCTION',scope=__doc__,audit_results_sha256=sha(source/'results.json'),
                stage_manifest_sha256=r['stage_manifest_sha256'],stage=str(stage),context_bits=len(leaves),
                context_layout=leaves,context_boundaries=r['boundary_nets'],visited_nets=len(visited),
                primitive_modules=sorted(used),primitive_sources={paths[k]:sha(stage/paths[k])for k in used},
                explicit_original_unwired_pulldowns=pulldowns,buffer_sha256=sha(ROOT/HELPER),
                no_new_guest_owner_or_clock_or_readiness_bypass=True,
                source_function_induction_or_native_integration_or_speed_qualified=False,
                files={p.name:sha(p)for p in out.iterdir()if p.is_file()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(leaves),len(used),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.source.resolve(),a.output.resolve())
