#!/usr/bin/env python3
"""Connect the qualified prepared Hold candidate without changing guest clocks.

Export the exact132 audited MemC boundary values. Only native CE caller input
pin386 uses the prepared value; its context-ready term is an additional whole
machine publication barrier. Every original guard/raw pin/source remains. This
is an isolated experiment until full publication, timing and wait tests pass.
"""
import argparse
import json
from pathlib import Path
import shutil

from audit_native_memc_hold_producer import BOARD, BACKPLANE
from prepare_native_hold_function import HELPER
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
TARGET='src/verilog/rtl/dorado_memc_hold_buffer.sv'


def prepare(parent,function,proof,tests,out,project):
    assert not out.exists()and project.is_absolute()
    m=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,m['source_hashes'])
    f=json.loads((function/'results.json').read_text());verify(function,f['files'])
    p=json.loads((proof/'run-state.json').read_text());t=json.loads((tests/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_NATIVE_HOLD_BUFFER_INDUCTION'and all(a['passed']for a in p['actions'])
    assert t['status']=='PASS_DUAL_ENGINE_SOURCE_HOLD_BUFFER_AND_NEGATIVES'and all(a['passed']for a in t['actions'])
    assert p['function_receipt_sha256']==t['function_receipt_sha256']==sha(function/'results.json')
    assert f['stage_manifest_sha256']==sha(parent/'r-front-control-native.json')
    assert p['helper_sha256']==t['helper_sha256']==sha(ROOT/HELPER)==f['buffer_sha256']
    assert not m['assembly_permitted']and m['clock_enable_conversion_staged']
    board=(parent/BOARD).read_text();header='    input wire [843:0] r_front_q,'
    assert board.count(header)==1
    board=board.replace(header,'    output wire [131:0] hold_source_context,\n'+header,1)
    anchor='  assign r_front_pins[386]=MemC15_sil_pl_2;';assert board.count(anchor)==1
    export='  assign hold_source_context={'+','.join(reversed(f['context_layout']))+'};\n'
    board=board.replace(anchor,export+anchor,1)
    original=(parent/BACKPLANE).read_text();before,after=original.split('module dorado_native_backplane_ce #',1)
    ce,tail=after.split('endmodule',1);edits=[]
    def change(a,b):
        nonlocal ce
        assert ce.count(a)==1,a
        ce=ce.replace(a,b,1);edits.append((a,b))
    change('.cache_reads_ready(memc_reads_ready),','.cache_reads_ready(memc_reads_ready),.hold_source_context(memc_hold_source_context),')
    change('  wire memc_reads_ready;','''  wire memc_reads_ready;
  wire [131:0] memc_hold_source_context;
  wire memc_hold_ready,memc_prepared_hold;
  wire [400:0] prepared_hold_pins={r_front_live[400:387],memc_prepared_hold,r_front_live[385:0]};
  dorado_memc_hold_buffer hold_frontier(storage_clk,storage_reset,
   memc_hold_source_context,r_front_live[386],memc_hold_ready,memc_prepared_hold,,,,);''')
    change('.reset(storage_reset),.pins_after(r_front_live),','.reset(storage_reset),.pins_after(prepared_hold_pins),')
    change('.payload_ready(memc_reads_ready && r_front_alias_ok','.payload_ready(memc_hold_ready && memc_reads_ready && r_front_alias_ok')
    restored=ce
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==after.split('endmodule',1)[0]
    backplane=before+'module dorado_native_backplane_ce #'+ce+'endmodule'+tail
    out.mkdir(parents=True)
    for name in m['source_hashes']:
        raw=(parent/name).read_bytes()
        if Path(name).suffix in ['.qsf','.qpf','.qip','.tcl','.sdc']:
            raw=raw.replace(m['project'].encode(),str(project).encode())
        if name==BOARD:raw=board.encode()
        if name==BACKPLANE:raw=backplane.encode()
        if name=='core/Dorado.qsf':raw+=('\n# Prepared native MemC Hold experiment; all original barriers remain.\nset_global_assignment -name SYSTEMVERILOG_FILE "'+str(project/TARGET)+'"\n').encode()
        path=out/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    target=out/TARGET;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ROOT/HELPER,target)
    changed=[n for n in m['source_hashes']if(out/n).read_bytes().replace(str(project).encode(),m['project'].encode())!=(parent/n).read_bytes()]
    assert set(changed)=={BOARD,BACKPLANE,'core/Dorado.qsf'}
    record={**m,'project':str(project),'parent_stage':str(parent),'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
            'scope':__doc__,'prepared_MemC_Hold_staged':True,'prepared_MemC_Hold_selected_or_speed_qualified':False,
            'prepared_MemC_Hold_context_layout':f['context_layout'],
            'prepared_MemC_Hold_function_receipt_sha256':sha(function/'results.json'),
            'prepared_MemC_Hold_induction_sha256':sha(proof/'run-state.json'),
            'prepared_MemC_Hold_tests_sha256':sha(tests/'run-state.json'),
            'prepared_MemC_Hold_backplane_reversible_edits':edits,
            'prepared_MemC_Hold_export':export,'prepared_MemC_Hold_header_original':header,
            'additional_native_lint_sources':[*m.get('additional_native_lint_sources',[]),TARGET],
            'source_hashes':{n:sha(out/n)for n in [*m['source_hashes'],TARGET]},
            'guest_clock_schedule_changed':False,'original_readiness_barriers_retained':True,
            'additional_host_preparation_stalls_need_measurement':True,'assembly_permitted':False}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n');shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(parent/'build_r_front_ready_native.py',out/'build_r_front_ready_native.py')
    print('STAGED_NATIVE_PREPARED_MEMC_HOLD',len(record['source_hashes']),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','function','proof','tests','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.function.resolve(),a.proof.resolve(),a.tests.resolve(),a.output.resolve(),a.project)
