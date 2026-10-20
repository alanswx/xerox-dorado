#!/usr/bin/env python3
"""Complete MSA with raw requests before the falling-edge clock-enable boundary.

The simulation contract model matches the native falling-edge sampling choice.
Native hardware must use dedicated ALTCLKCTRL, not a fabric AND gate. No native
integration, vendor-netlist equivalence, area or timing claim from this test.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_clocked as gen
import test_msa_board as base

HERE=Path(__file__).resolve().parent


def fixture(manifest):
    top,header,metadata=base.fixture(manifest)
    top=gen.base.once(top,' input wire clk,ref_clk,advance,freeze,held,global_hold,global_abort,system_reset,request,write,reset,',
                     ' input wire ff_other_scan,\n input wire clk,ref_clk,advance,freeze,held,global_hold,global_abort,system_reset,request,write,reset,')
    top=gen.base.once(top,' output wire [17:0] result,',
                     ' output wire ff_clock,ff_gate_open,output reg actual_advance=0,output reg [31:0] ff_edges=0,\n output wire [17:0] result,')
    top=gen.base.once(top,' wire reference_machine_clk=ref_clk && advance;',' wire reference_machine_clk=ref_clk && actual_advance;')
    top=gen.base.once(top,' wire candidate_machine_clk=clk && advance;',' wire candidate_machine_clk=clk && actual_advance;')
    top=gen.base.once(top,'.state_guest_advance(advance)','.state_guest_advance(actual_advance)')
    marker=' wire reference_machine_clk=ref_clk && actual_advance;'
    code=''' wire ff_request;
 dorado_ff_clock_model gate(.clk(clk),.enable(advance || ff_request || ff_other_scan),.gated_clk(ff_clock),.gate_open(ff_gate_open));
 always @(negedge clk)actual_advance<=advance;
 always @(posedge ff_clock)ff_edges<=ff_edges+1'b1;
'''
    top=gen.base.once(top,marker,marker+'\n'+code)
    top=gen.base.once(top,'dorado_serial_MSA #(.SYSPER(4)) dut(\n',
                     'dorado_serial_MSA #(.SYSPER(4)) dut(\n.state_ff_clk(ff_clock),.state_ff_clock_request(ff_request),\n')
    return top,header,metadata


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,helpers=gen.generate();top,header,metadata=fixture(manifest)
    for p in(Path(__file__),Path(gen.__file__),Path(base.__file__),HERE/'generate_msa_board.py',HERE/'ff_clock_falling_model.sv'):
        (inputs/p.name).write_bytes(p.read_bytes())
    cpp=(HERE/'test_msa_board.cpp').read_text();(inputs/'test_msa_board_original.cpp').write_text(cpp)
    cpp=gen.base.once(cpp,' d.clk=d.ref_clk=0;d.req_ready=d.rsp_valid=0;',
        ' const auto previous_edges=d.ff_edges;const bool source_high=d.clk,previous_level=d.ff_clock;\n d.req_ready=d.rsp_valid=0;')
    cpp=gen.base.once(cpp,' d.eval();\n}\nstatic void cycle',
        ' d.eval();\n if(source_high)check(d.ff_edges==previous_edges&&d.ff_clock==previous_level,"FF clock changed while source high");\n}\nstatic void cycle')
    cpp=gen.base.once(cpp,' const bool shifting=d.scan_step;',
        ' d.clk=d.ref_clk=0;d.eval();\n check(bool(d.actual_advance)==advance,"falling-edge guest enable differs");\n const bool shifting=d.scan_step;const unsigned before_ff=d.ff_edges;\n check(!d.ff_clock,"FF clock high before source edge");')
    cpp=gen.base.once(cpp,' if(shifting)++scan_edges;',
        ' check(d.ff_edges==before_ff+unsigned(advance||shifting||d.ff_other_scan),"FF clock edge contract differs");\n if(shifting)++scan_edges;')
    cpp=gen.base.once(cpp,'  enter();same(saved,"hold changed physical state");',
        '  enter();same(saved,"hold changed physical state");\n'
        '  for(unsigned n=0;n<32;++n){d.ff_other_scan=n&1;cycle(false,false);same(saved,"other-board scan clock changed physical state");check(d.ready&&!d.busy&&!d.done,"other-board scan admitted local transaction");}\n'
        '  d.ff_other_scan=0;')
    cpp=gen.base.once(cpp,' d.clk=d.ref_clk=0;d.eval();\n}\nstatic void settle',
        ' // Leave the source high so the next raw request precedes its falling edge.\n}\nstatic void settle')
    (inputs/'test_msa_clocked.cpp').write_text(cpp)
    (inputs/'msa_ddram.v').write_bytes(gen.base.original(gen.base.SOURCE));(inputs/'dorado_storage_group.sv').write_bytes(gen.base.storage.original_source())
    originals=gen.base.original_cells()
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    assets={'msa_board.sv':board,**helpers,'ff_clock_model.sv':(HERE/'ff_clock_falling_model.sv').read_text()}
    cases=[('candidate',None,None,None,None)]
    if mutations:cases+=[
        ('missing_scan_clock','msa_board.sv','assign state_ff_clock_request=scan_step;',"assign state_ff_clock_request=1'b0;",'FF clock edge contract differs'),
        ('lost_local_hold','msa_board.sv','wire state_hold=state_global_hold ||','wire state_hold=', 'held raw request/response leak'),
        ('wrong_ff_clock','msa_board.sv','.state_host_clk(state_ff_clk)', '.state_host_clk(storage_clk)', 'ordinary physical state differs'),
        ('runt_gate','ff_clock_model.sv','assign gated_clk=clk & gate_open;','assign gated_clk=clk & enable;', 'FF clock changed while source high'),
    ]
    results=[]
    for label,target,before,after,why in cases:
        case=out/label;case.mkdir();sources=dict(assets)
        if target:
            assert before in sources[target];sources[target]=sources[target].replace(before,after,1)
        for name,body in sources.items():(case/name).write_text(body)
        (case/'fixture.sv').write_text(top);(case/'parameters.h').write_text(header)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_board_fixture','-Wno-fatal','--assert',
                 '--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),*[str(case/n)for n in sources],
                 str(inputs/'msa_ddram.v'),str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in originals],str(inputs/'test_msa_clocked.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_board_fixture')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        if why:assert result.returncode and why in text,(label,text)
        else:assert result.returncode==0 and 'MSA BOARD PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    for name,value in [('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},physical_bits=2690,
        limits='Raw request before falling edge, then latched guest flag; simulation contract model, no vendor-netlist equivalence/native barriers/area/timing or speed qualification.'))]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
