#!/usr/bin/env python3
"""Connect the qualified RIOT block RAM implementation to five native chips.

Use the compact descriptor parent, keeping the unsuccessful publication-grant
experiment separate. Only the converted RIOT wrapper changes child type; add
the two exact qualified modules. Preserve all other HDL, ROMs, clocks, timers,
guest barriers, project timing policy, and original nonconverted RIOT models.
"""
import argparse
import json
from pathlib import Path
import shutil

from prepare_riot_blockram import RIOT,HELPER
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
CELL='src/verilog/cells/cell_MCS6532.v'
MODULE='src/verilog/rtl/dorado_riot_blockram_ce.sv'
RAM='src/verilog/rtl/dorado_riot_ram.sv'


def prepare(parent,component,tests,proof,baseboard,mapping,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    m=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,m['source_hashes'])
    assert m['compact_operand_tag_qualified']and m['operand_view_tag_support_bits']==465
    assert m['clock_enable_conversion_staged']and not m.get('shared_publication_grant_staged')
    receipts={}
    for name,d,status in [('component',component,'PREPARED_NATIVE_RIOT_BLOCK_RAM_COMPONENT'),
                          ('tests',tests,'PASS_COMPLETE_NATIVE_RIOT_BLOCK_RAM_COMPONENT'),
                          ('proof',proof,'PASS_RESET_HISTORY_RIOT_RAM_INDUCTION'),
                          ('baseboard',baseboard,'PASS_ORIGINAL_ROM_BASEBOARD_RIOT_BLOCK_RAM'),
                          ('mapping',mapping,'COLLECTED_RIOT_RAM_MAPPING')]:
        state=d/('results.json'if name in ['component','mapping']else'run-state.json')
        r=json.loads(state.read_text());verify(d,r['files']);assert r['status']==status
        receipts[name]=dict(directory=str(d),state_sha256=sha(state))
    c=json.loads((component/'results.json').read_text());t=json.loads((tests/'run-state.json').read_text())
    p=json.loads((proof/'run-state.json').read_text());b=json.loads((baseboard/'run-state.json').read_text())
    assert c['original_RIOT_sha256']==m['source_hashes'][RIOT]
    assert t['component_state_sha256']==b['component_state_sha256']==sha(component/'results.json')
    assert b['component_tests_state_sha256']==sha(tests/'run-state.json')
    assert p['helper_sha256']==c['helper_sha256']==sha(ROOT/HELPER)
    assert len(p['actions'])==3 and all(a['passed']for a in p['actions'])and p['assumptions']==0
    maplog=(mapping/'map.log').read_text()
    assert 'Parameter RAM_BLOCK_TYPE set to M10K'in maplog
    assert 'Parameter READ_DURING_WRITE_MODE_MIXED_PORTS set to OLD_DATA'in maplog
    observed=json.loads((mapping/'results.json').read_text())
    assert observed['state']['source_hashes']['ram.sv']==c['helper_sha256']
    output={n:(parent/n).read_bytes()for n in m['source_hashes']}
    raw=output[CELL].decode();before=module(raw,'cell_MCS6532_ce')
    a='M6532_ce u_riot (.guest_advance(guest_advance),'
    z='dorado_riot_blockram_ce u_riot (.guest_advance(guest_advance),'
    assert before.count(a)==1
    after=before.replace(a,z,1)
    assert after.replace(z,a,1)==before
    output[CELL]=raw.replace(before,after,1).encode()
    output[MODULE]=(component/'candidate.sv').read_bytes();output[RAM]=(component/'ram.sv').read_bytes()
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
            output[n]=raw.replace(m['project'].encode(),str(project).encode())
    output['core/Dorado.qsf']+=(''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+str(project/n)+'\n'for n in [MODULE,RAM])).encode()
    out.mkdir(parents=True)
    for n,raw in output.items():
        q=out/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(raw)
    for n in [Path(__file__).name,'build_r_front_ready_native.py']:shutil.copy2(ROOT/'tools'/n,out/n)
    changed={n for n,h in m['source_hashes'].items()if Path(n).suffix in ['.sv','.v']and sha(out/n)!=h}
    assert changed=={CELL}and len(output)==580
    result={**m,'scope':__doc__,'project':str(project),'parent_project':m['project'],'parent_stage':str(parent),
            'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
            'source_hashes':{n:sha(out/n)for n in output},'native_RIOT_block_ram_staged':True,
            'RIOT_block_ram_receipts':receipts,'RIOT_block_ram_helper_sha256':c['helper_sha256'],
            'RIOT_block_ram_module_sha256':c['candidate_module_sha256'],
            'RIOT_block_ram_original_cells':5,'RIOT_block_ram_default_init_7800':0,
            'additional_native_lint_sources':[MODULE,RAM],
            'incremental_functional_HDL_changes':[CELL,MODULE,RAM],
            'functional_HDL_changes':sorted(set(m['functional_HDL_changes'])|{CELL,MODULE,RAM}),
            'assembly_permitted':False,'physical_timing_qualified':False,'firmware_qualified':False,'hardware_speedup_qualified':False}
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_NATIVE_RIOT_BLOCK_RAM',len(output),'inputs',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','component','tests','proof','baseboard','mapping','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.component.resolve(),a.tests.resolve(),a.proof.resolve(),a.baseboard.resolve(),a.mapping.resolve(),a.output.resolve(),a.project)
