#!/usr/bin/env python3
"""Same-falling-edge native advance copies with eight complete MSA/SDRAM oracles.

Retain every original pin/state/raw-traffic, pending-read/write pause, scan and
quarantine check. Only candidate MC10176 payloads opt into the proved phase
contract; original physical boards and storage oracles remain unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import resource
import subprocess
import generate_phase_payload as payload
import test_eight_msa_ready_prediction as base

HERE=Path(__file__).resolve().parent


def fixture(manifest):
    leaf,bench,metadata=base.fixture(manifest)
    leaf=base.gen.once(leaf,' input wire clk,ref_clk,advance,', ' input wire local_advance,\n input wire clk,ref_clk,advance,')
    leaf=base.gen.once(leaf,'.state_guest_advance(advance)', '.state_guest_advance(local_advance)')
    bench=base.gen.once(bench,'.advance(accepted_advance),',
        '.advance(accepted_advance),.local_advance(board_phase[g]),')
    bench=base.gen.once(bench,'wire accepted_advance=advance && new_native_gate;', '''reg accepted_advance=0;
    always @(negedge clk)accepted_advance<=advance && new_native_gate;
    wire [18:0] board_phase;
    dorado_board_phase_enable copies(.clk(clk),.permit(advance && new_native_gate),.advance(board_phase));''')
    bench=base.gen.once(bench,'check(!advance || accepted_advance,"new gate lost an original guest advance");',
        'check(board_phase==={19{accepted_advance}},"MSA local advance phase differs");')
    # Requests must be stable before the falling edge. Setting them at that
    # edge races the real FF capture. Candidate and frozen original leaves
    # now receive the respective actually captured phase, with no extra
    # guest instruction during any intervening service edge.
    bench=base.gen.once(bench,'@(negedge clk);advance=guest;',
        '''integer issued_before;
        issued_before=guest_edges;
        // A caller can itself be running at a falling edge. Advance time
        // before changing permission so the current edge cannot capture a
        // second guest instruction through simulator scheduling order.
        #1;advance=guest; @(negedge clk);#1;
        for(integer age=0;guest && !accepted_advance;age++)begin
            check(age<20000,"MSA phase admission stalled");
            @(posedge clk);#1;if(ordinary)compare();
            @(negedge clk);#1;
        end
        check(accepted_advance===guest,"MSA captured permission lost requested guest");''')
    bench=base.gen.once(bench,'if(ordinary)compare();\n    endtask',
        'if(ordinary)compare();\n        check(guest_edges==issued_before+int\'(guest),"MSA request issued an extra or missing guest edge");\n    endtask')
    bench=base.gen.once(bench,'if(advance)guest_edges++;', 'if(accepted_advance)guest_edges++;')
    bench=base.gen.once(bench,'.guest_held(held && !advance)', '.guest_held(held && !accepted_advance)')
    metadata.update(required_phase_registers=19,native_falling_capture=True,candidate_payload_families=['MC10176'])
    return leaf,bench,metadata


def assets():
    board,manifest,helpers=base.gen.generate();helpers=helpers.copy()
    old=helpers['cell_MC10176.sv'];helpers['cell_MC10176.sv']=payload.transform(old)
    assert payload.inverse(helpers['cell_MC10176.sv'],old)==old
    module=re.search(r'\bmodule\s+(\w+)',helpers['cell_MC10176.sv'])[1]
    board,count=re.subn(r'\b'+module+r'(\s*#\(\s*\.PRECAP\([^)]*\))\s*\)',
        lambda m:module+m[1]+',.PHASE_QUALIFIED(1))',board)
    board,defaults=re.subn(r'\b'+module+r'(\s+u_\w+\s*\()',
        lambda m:module+' #(.PHASE_QUALIFIED(1))'+m[1],board)
    count+=defaults
    assert count==12,count
    leaf,bench,metadata=fixture(manifest);parent=base.gen.prior.parent
    sources={'msa_board.sv':board,**helpers,'leaf.sv':leaf,'tb.sv':bench,
        'memory_pause_barrier.sv':(HERE/'memory_pause_barrier.sv').read_text(),
        'msa_state_group.sv':base.base.transport.group_source(),
        'board_phase_enable.sv':(HERE/'board_phase_enable.sv').read_text()}
    for n in('dorado_storage_arb.sv','dorado_storage_sdram.sv'):
        sources[n]=(parent.base.ROOT/'fpga/mister'/n).read_text()
    sources['msa_ddram.v']=parent.base.original(parent.base.SOURCE).decode()
    sources['dorado_storage_group.sv']=parent.compact.original.original_source().decode()
    sources.update({n:r.decode()for n,r in parent.base.original_cells().items()})
    sources['xsds_128mbyte_sdram_model.sv']=(parent.base.ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv').read_text()
    return sources,metadata,count


def run(out,mutations):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out.mkdir(parents=True,exist_ok=False)
    sources,metadata,count=assets()
    inputs=out/'executed-inputs';inputs.mkdir()
    for n,s in sources.items():(inputs/n).write_text(s)
    for p in(Path(__file__),Path(payload.__file__),Path(base.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None)]
    if mutations:cases +=[
        ('wrong_phase','board_phase_enable.sv','always @(negedge clk)local_step',
         'always @(posedge clk)local_step','MSA local advance phase differs'),
        ('lost_payload_gate','cell_MC10176.sv','else if(state_advance)begin',
         "else if(1'b1)begin",'ordinary physical MSA state differs')]
    results=[]
    for label,target,before,after,expected in cases:
        case=out/label;case.mkdir();paths={n:inputs/n for n in sources}
        if target:
            paths[target]=case/target;paths[target].write_text(base.gen.once(sources[target],before,after))
        cmd=['verilator','--binary','--timing','--assert','-j','4','--top-module','tb','-Wno-fatal',
            '--Mdir',str(case/'obj'),'-o','test',*map(str,paths.values())]
        (case/'commands.json').write_text(json.dumps([cmd,[str(case/'obj/test')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/test')],capture_output=True,text=True,timeout=300)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:1800],flush=True)
        if expected:assert result.returncode and expected in text,text
        else:assert not result.returncode and all(s in text for s in('EIGHT MSA READY PREDICTION PASS',
            'EIGHT MSA SDRAM PAUSE PASS','EIGHT MSA QUARANTINE PASS')),text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=expected))
    (out/'results.json').write_text(json.dumps(dict(status='PASS_EIGHT_MSA_PHASE',scope=__doc__,cases=results,metadata=metadata,
        physical_bits=12304,phase_bits=19,readiness_bits=8,helper_instances=count*8,
        compiled_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Complete eight-MSA/SDRAM component integration; full native integration, state access, actual Main controls and routed timing remain required.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
