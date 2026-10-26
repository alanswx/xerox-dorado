#!/usr/bin/env python3
"""Stage the fully qualified465-bit operand tag on the common-clock native core.

Change only the descriptor mask. Preserve every original readiness/storage
condition, guest owner, RAM/write edge, clock, timer and SDC policy. Reuse the
global advance routing assignment as a separate, already-observed setting.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
HELPER='src/verilog/rtl/dorado_native_operand_buffer.sv'


def prepare(parent,selection,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert p['clock_enable_conversion_staged']and p['physical_global_advance_assignment']and p['operand_view_tag_support_bits']==909
    s=json.loads((selection/'results.json').read_text());assert s['status']=='SELECTED_QUALIFIED_COMPACT_NATIVE_OPERAND_DESCRIPTOR'
    helper=ROOT/'verilog/rtl/dorado_native_operand_buffer.sv'
    assert sha(helper)==s['selected_sha256']and sha(parent/HELPER)==s['before_sha256']
    pattern=r"OPERAND_TAG_SUPPORT=1264'h[0-9a-f]+;"
    assert re.sub(pattern,'MASK',helper.read_text())==re.sub(pattern,'MASK',(parent/HELPER).read_text())
    output={n:(parent/n).read_bytes()for n in p['source_hashes']};output[HELPER]=helper.read_bytes()
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:output[n]=raw.replace(p['project'].encode(),str(project).encode())
    out.mkdir(parents=True)
    for n,raw in output.items():
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(raw)
    for name in [Path(__file__).name,'stage_native_clock_enables.py','build_r_front_ready_native.py']:shutil.copy2(ROOT/'tools'/name,out/name)
    proof=ROOT/'build/rtl-executive/native-operand-binary-mask-proof-v1-20261025'
    result={**p,'scope':__doc__,'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
        'clock_enable_lint_source_stage':p['parent_stage'],'clock_enable_lint_source_manifest_sha256':p['parent_manifest_sha256'],
        'source_hashes':{n:sha(out/n)for n in output},'operand_view_tag_support_bits':465,
        'operand_buffer_proof':str(proof),'operand_buffer_proof_state_sha256':sha(proof/'run-state.json'),
        'compact_operand_tag_qualified':True,'compact_operand_selection_sha256':sha(selection/'results.json'),
        'compact_operand_replays':s['replays'],'incremental_functional_HDL_changes':[HELPER],
        'assembly_permitted':False,'physical_timing_qualified':False,'firmware_qualified':False,'hardware_speedup_qualified':False}
    assert len(result['source_hashes'])==578
    assert all(result['source_hashes'][n]==h for n,h in p['source_hashes'].items()if n!=HELPER and Path(n).suffix not in ['.qsf','.qip','.qpf','.sdc','.tcl'])
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n');print('STAGED_QUALIFIED_COMPACT_NATIVE_OPERAND_DESCRIPTOR',len(output),'inputs',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','selection','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.selection.resolve(),a.output.resolve(),a.project)
