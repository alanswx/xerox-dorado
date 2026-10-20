#!/usr/bin/env python3
"""Complete MSA NEXT-ready through destructive scan, restore and resumed traffic.

Keep the immutable chip-width test's state/pin/raw-traffic comparisons. Check
the external aggregate register after each host edge, including every shifted
bit, and independently vary live initialization and disk-ready guards.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_ready_prediction as gen
import test_msa_chipwidth as base

HERE=Path(__file__).resolve().parent


def fixture(manifest):
    top,header,metadata=base.fixture(manifest)
    top=gen.once(top,' input wire clk,ref_clk,advance,',
        ' input wire gate_initialized,gate_pack,\n input wire clk,ref_clk,advance,')
    top=gen.once(top,'output wire storage_ready,ref_storage_ready,',
        'output wire cached_flags,flags_current,old_gate,new_gate,storage_ready,ref_storage_ready,')
    top=gen.once(top,'.state_global_hold(global_hold),',
        '.state_frontend_hold(global_hold || (freeze && held)),.state_global_hold(global_hold),')
    top=gen.once(top,'.state_memory_drained(drained)',
        '.state_memory_flags_next(flags_next),.state_memory_drained(drained)')
    terms=[]
    for g in range(16):
        p=f'dut.u_storage_{g}'
        terms.append(f'(!{p}.waiting && {p}.cache_valid && (!{p}.write_pending || {p}.write_done))')
    code='''wire flags_next;
reg aggregate_ready=0;
always @(posedge clk)aggregate_ready<=flags_next;
assign cached_flags=aggregate_ready;
assign flags_current='''+' &&\n    '.join(terms)+''';
assign old_gate=!reset && gate_initialized && gate_pack && storage_ready && !dut.state_hold;
assign new_gate=!reset && gate_initialized && gate_pack && aggregate_ready && !dut.state_hold;
'''
    return gen.once(top,'endmodule',code+'endmodule'),header,metadata


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=gen.generate();top,header,metadata=fixture(manifest)
    parent=gen.prior.parent
    cpp=gen.raw_receipt(gen.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth',
        'msa-v1/executed-inputs/test_msa_chipwidth.cpp').decode()
    probe='''static unsigned long long readiness_checks=0;
static void prediction_probe(){
 check(d.cached_flags==d.flags_current,"registered MSA readiness differs");
 for(unsigned initialized=0;initialized<2;++initialized)for(unsigned pack=0;pack<2;++pack){
  d.gate_initialized=initialized;d.gate_pack=pack;d.eval();
  check(d.old_gate==d.new_gate,"registered native advance gate differs");++readiness_checks;
 }
 d.gate_initialized=d.gate_pack=1;d.eval();
}
'''
    cpp=gen.once(cpp,'static void prepare(){',probe+'static void prepare(){')
    cpp=gen.once(cpp,' if(normal)compare();\n d.clk=d.ref_clk=0;d.eval();',
        ' if(normal)compare();\n prediction_probe();\n d.clk=d.ref_clk=0;d.eval();\n prediction_probe();')
    cpp=gen.once(cpp,'  std::printf("CHIPWIDTH MSA PASS',
        '  std::printf("MSA READY PREDICTION PASS readiness_checks=%llu required_registers=1\\n",readiness_checks);\n  std::printf("CHIPWIDTH MSA PASS')
    (inputs/'test.cpp').write_text(cpp)
    (inputs/'msa_ddram.v').write_bytes(parent.base.original(parent.base.SOURCE))
    (inputs/'dorado_storage_group.sv').write_bytes(parent.compact.original.original_source())
    originals=parent.base.original_cells()
    for n,r in originals.items():(inputs/n).write_bytes(r)
    for p in(Path(__file__),Path(gen.__file__),Path(base.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    assets={'msa_board.sv':board,**helpers}
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('wrong_aggregate_group','msa_board.sv','assign state_memory_flags_next=&state_storage_flags_next;',
         'assign state_memory_flags_next=&state_storage_flags_next[14:0];'),
        ('lost_hold_guard','fixture.sv','aggregate_ready && !dut.state_hold;',
         'aggregate_ready;')]
    results=[]
    for label,target,before,after in cases:
        case=out/label;case.mkdir();sources={**assets,'fixture.sv':top}
        if target:sources[target]=gen.once(sources[target],before,after)
        for n,s in sources.items():(case/n).write_text(s)
        (case/'parameters.h').write_text(header)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_board_fixture','-Wno-fatal','--assert',
            '--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),*[str(case/n)for n in sources],
            str(inputs/'msa_ddram.v'),str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in originals],str(inputs/'test.cpp')]
        (case/'commands.json').write_text(json.dumps([cmd,[str(case/'obj/Vmsa_board_fixture')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_board_fixture')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if target:assert result.returncode and('registered MSA readiness differs'in text or'registered native advance gate differs'in text),text
        else:assert not result.returncode and'MSA READY PREDICTION PASS'in text and'CHIPWIDTH MSA PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    for n,v in(('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,
        required_external_register_bits=1,original_physical_board_bits=1538,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='One original-oracle complete MSA. Native integration/state access, capacity/timing and FPGA qualification remain open.'))):
        (out/(n+'.json')).write_text(json.dumps(v,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
