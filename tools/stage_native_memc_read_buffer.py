#!/usr/bin/env python3
"""Wire the proved cache-tag read register into all21 actual MemC SRAM cells.

The original RAM/write/admin owner remains in each cell. Add only its read
register and an all-addresses-ready publication condition. Other board bytes,
processor holds, SDRAM barrier, clocks, original ROM and constraints stay exact.
Private timing/startup experiment only; no assembly or60ns qualification.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from qualify_memc_read_buffer import CELL,RTL,transform
from review_baseboard_clock_consumer import verify
from review_processor_branch_composition import checked
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BOARD='src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv'


def board(raw):
    original=raw;edits=[]
    a='output wire r_front_alias_ok,';b=a+'output wire cache_reads_ready,'
    assert raw.count(a)==1;raw=raw.replace(a,b,1);edits.append((a,b))
    cells=list(re.finditer(r'^  cell_MB7071H_snapshot u_(\w+) \(.*?^  \); // MB7071H',raw,re.M|re.S))
    assert len(cells)==21
    packages=[m[1]for m in cells]
    for i,m in enumerate(cells):
        a=m[0];b=a.replace('cell_MB7071H_snapshot','dorado_memc_read_buffer',1)
        b=b.replace('(\n','(\n    .read_ready(cache_read_matches['+str(i)+']),\n',1)
        assert raw.count(a)==1;raw=raw.replace(a,b,1);edits.append((a,b))
    a='endmodule';b=' wire [20:0] cache_read_matches;\n assign cache_reads_ready=&cache_read_matches;\n'+a
    assert raw.count(a)==1;raw=raw.replace(a,b,1);edits.append((a,b))
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw,packages


def backplane(raw):
    original=raw;edits=[]
    for a,b in [('.r_front_alias_ok(r_front_alias_MemC),','.r_front_alias_ok(r_front_alias_MemC),.cache_reads_ready(memc_reads_ready),'),
                ('  wire [571:0] r_front_lower_q=', '  wire memc_reads_ready;\n  wire [571:0] r_front_lower_q='),
                ('.payload_ready(r_front_alias_ok && ','.payload_ready(memc_reads_ready && r_front_alias_ok && ')]:
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def prepare(parent,proof,checks,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert len(p['source_hashes'])==576 and p['registered_R_read_buffer_connected']and p['actual_native_single_comparison_connected']
    assert not p['assembly_permitted']and p['frequency_hz']==66666667 and p['sysper']==4
    q=json.loads((proof/'run-state.json').read_text());verify(proof,q['files'])
    assert q['status']=='PASS_ACTUAL_MEMC_TAGGED_SRAM_READ'and q['assumptions']==0
    assert q['source_cell_sha256']==sha(parent/CELL)and q['candidate_sha256']==sha(ROOT/RTL)
    assert(ROOT/RTL).read_text()==transform((parent/CELL).read_text())
    for a in q['actions']:
        d=proof/a['label'];verify(d,a['files'])
        assert a['passed']and a['returncode']==0 and sha(d/'proof.log')==a['log_sha256']
    for engine,d in checks.items():
        s=checked(d);assert s['status']=='PASS_ACTUAL_MEMC_READ_CELL_REPLAY'and s['engine']==engine
        assert s['proof_state_sha256']==sha(proof/'run-state.json')and sha(d/'buffered.sv')==sha(ROOT/RTL)
    after,packages=board((parent/BOARD).read_text());bp=p['native_backplane_binding_source']
    qsf=(parent/'core/Dorado.qsf').read_text()
    for n in [BOARD,bp]:assert str(Path(p['project'])/n)in qsf,n
    added='src/'+RTL
    changed={BOARD:after.encode(),bp:backplane((parent/bp).read_text()).encode(),added:(ROOT/RTL).read_bytes()}
    output={n:(parent/n).read_bytes()for n in p['source_hashes']};output.update(changed)
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:output[n]=raw.replace(p['project'].encode(),str(project).encode())
    output['core/Dorado.qsf']+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/added)+'\n').encode()
    out.mkdir(parents=True)
    for n,raw in output.items():
        d=out/n;d.parent.mkdir(parents=True,exist_ok=True);d.write_bytes(raw)
    for n in [Path(__file__).name,'qualify_memc_read_buffer.py','build_r_front_ready_native.py']:shutil.copy2(ROOT/'tools'/n,out/n)
    result={**p,'scope':__doc__,'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),'source_hashes':{n:sha(out/n)for n in output},
        'extra_RTL_sources':sorted(set(p['extra_RTL_sources'])|{added}),
        'registered_MemC_tag_reads_connected':True,'MemC_tag_read_packages':packages,
        'MemC_tag_read_proof_state_sha256':sha(proof/'run-state.json'),
        'MemC_tag_read_checks':{e:dict(directory=str(d),state_sha256=sha(d/'run-state.json'))for e,d in checks.items()},
        'all21_address_matches_gate_whole_machine_publication':True,'original_MemC_RAM_write_and_admin_updates_unchanged':True,
        'incremental_functional_HDL_changes':sorted(changed),'functional_HDL_changes':sorted(set(p['functional_HDL_changes'])|set(changed)),
        'full60ns_event_batch_connected':False,'assembly_permitted':False,'hardware_speedup_qualified':False,
        'firmware_qualified':False,'physical_timing_qualified':False,'full_goal_active':True}
    assert len(result['source_hashes'])==577
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_ACTUAL_MEMC_TAGGED_READ',len(output),'inputs;21 RAM owners unchanged',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','proof','icarus-check','verilator-check','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.proof.resolve(),{'icarus':a.icarus_check.resolve(),'verilator':a.verilator_check.resolve()},a.output.resolve(),a.project)
