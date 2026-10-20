#!/usr/bin/env python3
"""Registered NEXT-ready across eight original-oracle MSAs and real SDRAM.

The existing eight-board stimulus and physical state/pin/raw-traffic checks
remain intact. Add one external required register per MSA, check it against
all actual post-edge frontend flags, and compare the native enable equations.
Guest requests pass through the new gate; both original and candidate boards
receive that same accepted advance. The old gate must agree at every edge.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import subprocess
import generate_msa_ready_prediction as gen
import test_eight_msa_sdram_pause as base

HERE=Path(__file__).resolve().parent


def fixture(manifest):
    leaf,bench,metadata=base.fixture(manifest)
    leaf=gen.once(leaf,'output wire storage_ready,ref_storage_ready,',
        'output wire flags_next,flags_current,storage_ready,ref_storage_ready,')
    leaf=gen.once(leaf,'.state_memory_drained(drained)',
        '.state_memory_flags_next(flags_next),.state_memory_drained(drained)')
    flags=[]
    for g in range(16):
        p=f'dut.u_storage_{g}'
        flags.append(f'(!{p}.waiting && {p}.cache_valid && (!{p}.write_pending || {p}.write_done))')
    leaf=gen.once(leaf,'endmodule','assign flags_current=\n    '+' &&\n    '.join(flags)+';\nendmodule')
    bench=gen.once(bench,'wire [7:0] unit_ready,unit_ref_ready,',
        'wire [7:0] flags_next,flags_current;\n    reg [7:0] cached_flags=0;\n    always @(posedge clk)cached_flags<=flags_next;\n    wire [7:0] unit_ready,unit_ref_ready,')
    bench=gen.once(bench,'msa_board_fixture leaf(\n',
        'msa_board_fixture leaf(\n.flags_next(flags_next[g]),.flags_current(flags_current[g]),\n')
    bench=gen.once(bench,'.advance(advance),','.advance(accepted_advance),')
    code='''
    wire all_guest_hold=global_hold || pause_guest_hold || group_hold || group_fault;
    wire old_native_gate=!reset && initialized && storage_ready && !all_guest_hold;
    wire new_native_gate=!reset && initialized && (&cached_flags) && !all_guest_hold;
    wire accepted_advance=advance && new_native_gate;
    integer prediction_edges=0,prediction_scans=0,prediction_transitions=0;
    reg [7:0] previous_flags=0;
    always @(posedge clk or negedge clk)begin
        #3;
        check(cached_flags===flags_current,"registered MSA readiness differs");
        check(old_native_gate===new_native_gate,"registered native advance gate differs");
        check(!advance || accepted_advance,"new gate lost an original guest advance");
        prediction_edges++;
        if(|unit_scan)prediction_scans++;
        if(cached_flags!=previous_flags)prediction_transitions++;
        previous_flags=cached_flags;
    end
'''
    bench=gen.once(bench,'    task tick(input bit guest,input bit ordinary);',code+'\n    task tick(input bit guest,input bit ordinary);')
    # The original check remains, with an additional requirement that the real
    # new guest gate admits every requested old-ready advance.
    bench=gen.once(bench,'check(!guest||(!pause_guest_hold&&storage_ready&&ref_storage_ready),',
        'check(!guest||(new_native_gate&&!pause_guest_hold&&storage_ready&&ref_storage_ready),')
    bench=gen.once(bench,'        $display("EIGHT MSA SDRAM PAUSE PASS',
        '''        check(prediction_edges>100000 && prediction_scans>1000 && prediction_transitions>100,
              "registered readiness coverage missing");
        $display("EIGHT MSA READY PREDICTION PASS edges=%0d scans=%0d transitions=%0d required_registers=8",
                 prediction_edges,prediction_scans,prediction_transitions);
        $display("EIGHT MSA SDRAM PAUSE PASS''')
    metadata.update(required_external_registers=8,original_state_bits=12304,
        new_gate_drives_guest=True,original_equation_checked=True)
    return leaf,bench,metadata


def run(out,mutations):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out.mkdir(parents=True,exist_ok=False)
    inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=gen.generate();leaf,bench,metadata=fixture(manifest)
    parent=gen.prior.parent
    sources={'msa_board.sv':board,**helpers,'leaf.sv':leaf,'tb.sv':bench,
        'memory_pause_barrier.sv':(HERE/'memory_pause_barrier.sv').read_text(),
        'msa_state_group.sv':base.transport.group_source()}
    for n in('dorado_storage_arb.sv','dorado_storage_sdram.sv'):
        sources[n]=(parent.base.ROOT/'fpga/mister'/n).read_text()
    sources['msa_ddram.v']=parent.base.original(parent.base.SOURCE).decode()
    sources['dorado_storage_group.sv']=parent.compact.original.original_source().decode()
    sources.update({n:r.decode()for n,r in parent.base.original_cells().items()})
    sources['xsds_128mbyte_sdram_model.sv']=(parent.base.ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv').read_text()
    for n,s in sources.items():(inputs/n).write_text(s)
    for p in(Path(__file__),Path(gen.__file__),Path(base.__file__),HERE/'test_msa_sdram_pause.sv'):
        (inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('current_instead_of_next','storage_group.sv',
         '!ready_flags_next[3] && ready_flags_next[1] &&\n        (!ready_flags_next[0] || ready_flags_next[2])',
         '!waiting && cache_valid && (!write_pending || write_done)'),
        ('wrong_register_edge','tb.sv','always @(posedge clk)cached_flags<=flags_next;',
         'always @(negedge clk)cached_flags<=flags_next;')]
    results=[]
    for label,target,before,after in cases:
        case=out/label;case.mkdir();paths={n:inputs/n for n in sources}
        if target:
            paths[target]=case/target;paths[target].write_text(gen.once(sources[target],before,after))
        command=['verilator','--binary','--timing','--assert','-j','4','--top-module','tb','-Wno-fatal',
            '--Mdir',str(case/'obj'),'-o','test',*map(str,paths.values())]
        (case/'commands.json').write_text(json.dumps([command,[str(case/'obj/test')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/test')],capture_output=True,text=True,timeout=300)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:1500],flush=True)
        if target:assert result.returncode and'registered MSA readiness differs'in text,text
        else:assert not result.returncode and all(s in text for s in('EIGHT MSA READY PREDICTION PASS',
            'EIGHT MSA SDRAM PAUSE PASS','EIGHT MSA QUARANTINE PASS')),text
        results.append(dict(case=label,returncode=result.returncode))
    for n,v in(('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,
        physical_board_bits=12304,required_external_register_bits=8,msa_boards=8,raw_groups=128,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Complete eight-MSA/SDRAM prototype. Native wrapper integration, access for eight added registers, actual capacity/timing and FPGA boot remain required.'))):
        (out/(n+'.json')).write_text(json.dumps(v,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
