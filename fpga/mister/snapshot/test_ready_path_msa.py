#!/usr/bin/env python3
"""Actual complete-MSA readiness versus original CPU enable during traffic/restore.

Use actual old/new core expressions and actual frontend ready/drain outputs.
Local combined hold represents the native global hold for this component test;
native owner-to-local-hold implication requires the separate eight-board test.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import test_msa_chipwidth as base
import generate_ready_path as change

HERE=Path(__file__).resolve().parent


def fixture(manifest,new_expression=None):
    top,header,metadata=base.fixture(manifest);old,new=change.expressions()
    top=base.gen.base.once(top,' input wire clk,ref_clk,advance,', ' input wire gate_pack,gate_initialized,\n input wire clk,ref_clk,advance,')
    top=base.gen.base.once(top,' output wire [17:0] result,',' output wire old_gate,new_gate,\n output wire [17:0] result,')
    # storage_ready in the native wrapper also includes initialization.
    old=old.replace('storage_ready','(gate_initialized && storage_ready)').replace('pack_ready','gate_pack').replace('board_state_hold','dut.state_hold')
    new=(new_expression or new).replace('pack_ready','gate_pack').replace('storage_initialized','gate_initialized').replace('snapshot_frontends_ready','drained').replace('board_state_hold','dut.state_hold')
    top=base.gen.base.once(top,'endmodule',f'assign old_gate={old};\nassign new_gate={new};\nendmodule')
    return top,header,metadata


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=base.gen.generate();originals=base.gen.base.original_cells()
    original_run=Path('unused')
    # Reuse the exact already-tested C++ stimulus, adding checks only.
    from generate_serial_boards import raw_receipt
    cpp=raw_receipt(base.gen.base.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth','msa-v1/executed-inputs/test_msa_chipwidth.cpp').decode()
    cpp=base.gen.base.once(cpp,'static void prepare(){','''static void gate_probe(){
 for(unsigned p=0;p<2;++p)for(unsigned i=0;i<2;++i){
  d.gate_pack=p;d.gate_initialized=i;d.eval();
  check(d.old_gate==d.new_gate,"CPU readiness differs");
 }
 d.gate_pack=d.gate_initialized=1;d.eval();
}
static void prepare(){''')
    cpp=base.gen.base.once(cpp,' const bool shifting=d.scan_step;',' gate_probe();\n const bool shifting=d.scan_step;')
    cpp=base.gen.base.once(cpp,' if(normal)compare();\n d.clk=d.ref_clk=0;d.eval();',' if(normal)compare();\n gate_probe();\n d.clk=d.ref_clk=0;d.eval();')
    cpp=cpp.replace('CHIPWIDTH MSA PASS','READY PATH MSA PASS')
    (inputs/'test_ready_path.cpp').write_text(cpp)
    (inputs/'original-stimulus.cpp').write_bytes(raw_receipt(base.gen.base.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth','msa-v1/executed-inputs/test_msa_chipwidth.cpp'))
    (inputs/'msa_ddram.v').write_bytes(base.gen.base.original(base.gen.base.SOURCE));(inputs/'dorado_storage_group.sv').write_bytes(base.gen.compact.original.original_source())
    for n,raw in originals.items():(inputs/n).write_bytes(raw)
    for p in (Path(__file__),Path(change.__file__),Path(base.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    (out/'reference-core.sv').write_text(change.reference());(out/'candidate-core.sv').write_text(change.transform(change.reference()))
    _,new=change.expressions();cases=[('candidate',None)]
    if mutations:cases += [('lost_hold',new.replace(' && !board_state_hold','')),('lost_init',new.replace(' && storage_initialized','')),('wrong_ready',new.replace('snapshot_frontends_ready',"1'b1"))]
    results=[]
    for label,expression in cases:
        case=out/label;case.mkdir();top,header,metadata=fixture(manifest,expression);assets={'msa_board.sv':board,**helpers}
        for n,source in assets.items():(case/n).write_text(source)
        (case/'fixture.sv').write_text(top);(case/'parameters.h').write_text(header)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_board_fixture','-Wno-fatal','--assert','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),*[str(case/n)for n in assets],str(inputs/'msa_ddram.v'),str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in originals],str(inputs/'test_ready_path.cpp')]
        (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_board_fixture')],capture_output=True,text=True,timeout=180);text=result.stdout+result.stderr
        (case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if expression:assert result.returncode and 'CPU readiness differs'in text
        else:assert not result.returncode and 'READY PATH MSA PASS'in text
        results.append(dict(case=label,returncode=result.returncode))
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');(out/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},limits='One actual MSA; native owner implication and full fit remain pending.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
