#!/usr/bin/env python3
"""Eight complete MSAs, one coordinator/router and real 128-group SDRAM.

Generate the leaves from the already qualified complete-MSA fixture, removing
its local coordinator. Every leaf shares one pause boundary and actual arbiter/
SDRAM idle outputs. Original frozen boards remain independent traffic/state
oracles; host software never completes a raw transaction.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import resource
import subprocess
import generate_msa_late_freeze as gen
import test_msa_late_freeze as component
import test_msa_chipwidth_transport as transport

HERE=Path(__file__).resolve().parent


def fixture(manifest):
    leaf,_,metadata=component.fixture(manifest)
    start=leaf.index('dorado_memory_pause_barrier pause_barrier(')
    stop=leaf.index(');',start)+2
    leaf=leaf[:start]+leaf[stop:]
    leaf=gen.parent.base.once(leaf,'output wire pause_ready,pause_frontend_hold,pause_guest_hold,pause_fault,',
        'input wire pause_ready,pause_frontend_hold,pause_guest_hold,pause_fault,')
    header=leaf[leaf.index('(\n')+2:leaf.index(');')]
    shared={'clk','ref_clk','advance','freeze','held','global_hold','global_abort','system_reset','write','reset','drain_mode',
            'state_index','state_data','request','pause_ready','pause_frontend_hold','pause_guest_hold','pause_fault',
            'arbiter_idle','raw_service_idle'}
    pins={};widths={}
    for line in re.split(r',\s*(?=(?:input|output)\s+wire\b)',header.strip()):
        line=line.strip().rstrip(',');match=re.fullmatch(r'(input|output) wire\s*(\[[\d:]+\])?\s*(.*)',line)
        assert match,line
        width=match[2]or''
        for name in match[3].split(','):
            name=name.strip();array=re.fullmatch(r'(\w+)(\[\d+\])?',name);assert array,name
            key=array[1];unpacked=array[2]or'';pins[key]=match[1];widths[key]=(width,unpacked)
    declarations=[];connections=[]
    unit={'storage_ready':'unit_ready[g]','ref_storage_ready':'unit_ref_ready[g]',
        'ready':'unit_ready_scan[g]','frozen':'unit_frozen[g]','done':'unit_done[g]',
        'error':'unit_error[g]','hold_required':'unit_hold[g]','fault':'unit_fault[g]',
        'drained':'unit_drained[g]','busy':'unit_busy[g]','scan_step':'unit_scan[g]',
        'result':'unit_result[g*18+:18]','request':'unit_request[g]',
        'write':'unit_write','state_index':'unit_index','state_data':'unit_data',
        'global_abort':'global_abort || group_fault || pause_fault'}
    for name,(width,unpacked)in widths.items():
        if name in shared:value=unit.get(name,name)
        elif name in unit:value=unit[name]
        else:
            if unpacked:
                assert unpacked=='[16]';declarations.append(f'wire {width} {name}[128];');value=name+'[g*16+:16]'
            else:
                assert width;hi,lo=map(int,width[1:-1].split(':'));assert lo==0
                bits=hi+1;kind='logic'if name=='stimulus'else'wire'
                declarations.append(f'{kind} [{bits*8-1}:0] {name};');value=f'{name}[g*{bits}+:{bits}]'
        connections.append('.'+name+'('+value+')')
    code='''logic clk,advance,freeze,held,global_hold,global_abort,system_reset,write,reset,drain_mode,request;
    wire ref_clk;
    logic [23:0] state_index;logic [17:0] state_data;logic transport_reset=0;
    wire pause_ready,pause_frontend_hold,pause_guest_hold,pause_fault,arbiter_idle,raw_service_idle;
    wire [7:0] unit_ready,unit_ref_ready,unit_ready_scan,unit_frozen,unit_done,unit_error;
    wire [7:0] unit_hold,unit_fault,unit_drained,unit_busy,unit_scan,unit_request;
    wire [143:0] unit_result;wire [23:0] unit_index;wire [17:0] unit_data;
    wire unit_write,group_frozen,group_hold,group_fault;
    wire storage_ready=&unit_ready,ref_storage_ready=&unit_ref_ready;
    wire frozen=&unit_frozen,drained=&unit_drained;
    wire ready=pause_ready && group_frozen && !group_hold && !group_fault;
    wire done,error;wire [17:0] result;
'''+ '\n    '.join(declarations)+'''
    for(genvar g=0;g<8;g=g+1)begin:boards
        msa_board_fixture leaf(
'''+',\n'.join(connections)+''');
    end
    dorado_memory_pause_barrier pause_barrier(
        .clk(clk),.machine_reseed(system_reset),.owner_hold(global_hold || group_hold),
        .guest_held(held && !advance),.abort(global_abort || group_fault),
        .frontends_drained(drained),.arbiter_idle(arbiter_idle),.raw_service_idle(raw_service_idle),
        .frontends_frozen(frozen),.guest_hold(pause_guest_hold),.frontend_hold(pause_frontend_hold),
        .ready(pause_ready),.fault(pause_fault));
    dorado_msa_state_group_chipwidth state_group(
        .clk(clk),.system_reset(system_reset),.transport_reset(transport_reset),
        .scan_abort(global_abort || pause_fault),.request(request && pause_ready),.write(write),
        .index(state_index),.data(state_data),.unit_frozen(unit_frozen),.unit_done(unit_done),
        .unit_error(unit_error),.unit_hold_required(unit_hold),.unit_fault(unit_fault),.unit_result(unit_result),
        .frozen(group_frozen),.hold_required(group_hold),.fault(group_fault),.done(done),.error(error),.result(result),
        .unit_request(unit_request),.unit_index(unit_index),.unit_write(unit_write),.unit_data(unit_data));
'''
    bench=(HERE/'test_msa_sdram_pause.sv').read_text()
    bench=bench.replace('@DECLARATIONS@',code).replace('    msa_board_fixture dut(.*);\n','')
    bench=bench.replace('wire [255:0] address_bus,data_bus,response_bus;','wire [2047:0] address_bus,data_bus,response_bus;')
    bench=bench.replace('g<16;','g<128;').replace('.GROUPS(16)','.GROUPS(128)')
    # All eight state images are checked together, including unselected boards.
    bench=bench.replace('STATE_BITS=1538,SCAN_WORDS=86','STATE_BITS=12304,SCAN_WORDS=688')
    bench=bench.replace('[1537:0]','[12303:0]').replace('1538\'h@FF_MASK@',"12304'h@FF_MASK@")
    bench=bench.replace('canonical_bit[1538]', 'canonical_bit[12304]')
    mapping=[u*1538+p for u in range(8)for p in metadata['canonical_bit']]
    mask=sum(1<<(u*1538+p)for u in range(8)for p in metadata['canonical_bit'][:866])
    mapped=',\n'.join(','.join(map(str,mapping[i:i+64]))for i in range(0,len(mapping),64))
    bench=bench.replace('@FF_MASK@',format(mask,'x')).replace('@CANONICAL_BITS@',mapped)
    # Each unit's final 18-bit word contains only eight valid bits.
    old='for(integer b=0;b<18&&index*18+b<STATE_BITS;b++)value[b]=saved[canonical_bit[index*18+b]];'
    new='for(integer b=0;b<18&&(index%86)*18+b<1538;b++)value[b]=saved[canonical_bit[(index/86)*1538+(index%86)*18+b]];'
    assert bench.count(old)==1;bench=bench.replace(old,new,1)
    bench=bench.replace('state_index=24\'(i);',"state_index=24'(((i/86)<<8)|(i%86));")
    bench=bench.replace('age<1600','age<1700').replace('#100000000;','#500000000;')
    bench=bench.replace('MSA SDRAM PAUSE PASS','EIGHT MSA SDRAM PAUSE PASS')
    quarantine='''
    task quarantine_all;
        reg [12303:0] partial;
        integer age,writes_before,refresh_before;
        @(negedge clk);global_hold=1;freeze=1;held=1;
        age=0;
        while(!pause_ready)begin tick(0,0);age++;check(age<10000,"quarantine setup timeout");end
        check(ready,"quarantine scan not ready");
        @(negedge clk);state_index=24'h300;state_data=18'h29a15;write=1;request=1;
        tick(0,0);request=0;repeat(7)tick(0,0);
        check(unit_hold==8'h08,"quarantine did not begin on selected member");
        @(negedge clk);transport_reset=1;repeat(4)tick(0,0);
        check(group_fault&&pause_fault&&pause_guest_hold&&pause_frontend_hold&&!ready,
              "router reset did not quarantine all consumers");
        check(unit_fault==8'h08,"selected scan did not quarantine");
        partial=dut_state;writes_before=raw_writes;refresh_before=refreshes;
        @(negedge clk);transport_reset=0;global_hold=0;global_abort=0;freeze=0;held=0;
        for(integer n=0;n<1500;n++)begin
            stimulus=stimulus^448'h13579bdf2468ace0;
            tick(0,0);
            check(pause_guest_hold&&pause_frontend_hold&&pause_fault&&group_fault&&!ready,
                  "router reset quarantine released without reseed");
            check(frozen&&dut_state===partial,"unselected consumer escaped quarantine");
            check(!req_valid&&!rsp_ready&&arbiter_idle&&raw_service_idle,
                  "quarantine leaked actual raw traffic");
        end
        check(raw_writes==writes_before&&refreshes>refresh_before+4,
              "quarantine changed raw memory or stopped physical refresh");
        $display("EIGHT MSA QUARANTINE PASS member=3 consumers=8 partial_state_held=1 refresh_live=1");
    endtask
'''
    bench=gen.parent.base.once(bench,'    initial begin\n        advance=0;',quarantine+'\n    initial begin\n        advance=0;')
    bench=gen.parent.base.once(bench,'        check(chip0.error_count==0',
        '        quarantine_all();\n        check(chip0.error_count==0')
    return leaf,bench,metadata


def run(out,mutations):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out.mkdir(parents=True,exist_ok=False)
    inputs=out/'executed-inputs';inputs.mkdir();board,manifest,helpers=gen.generate()
    leaf,bench,metadata=fixture(manifest)
    sources={'msa_board.sv':board,**helpers,'leaf.sv':leaf,'tb.sv':bench,
        'memory_pause_barrier.sv':(HERE/'memory_pause_barrier.sv').read_text(),
        'msa_state_group.sv':transport.group_source()}
    for n in ('dorado_storage_arb.sv','dorado_storage_sdram.sv'):
        sources[n]=(gen.parent.base.ROOT/'fpga/mister'/n).read_text()
    sources['msa_ddram.v']=gen.parent.base.original(gen.parent.base.SOURCE).decode()
    sources['dorado_storage_group.sv']=gen.parent.compact.original.original_source().decode()
    sources.update({n:r.decode()for n,r in gen.parent.base.original_cells().items()})
    sources['xsds_128mbyte_sdram_model.sv']=(gen.parent.base.ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv').read_text()
    for n,s in sources.items():(inputs/n).write_text(s)
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('early_freeze','msa_board.sv','wire frontend_hold=state_frontend_hold || state_scan_hold_required;',
         'wire frontend_hold=state_global_hold || state_scan_hold_required;'),
        ('wrong_member','msa_state_group.sv','unit_request=(admit && valid_index)?route',"unit_request=(admit && valid_index)?8'b1"),
        ('nonsticky_hold','memory_pause_barrier.sv','assign frontend_hold=phase==FROZEN || phase==QUARANTINE;',
         'assign frontend_hold=(phase==FROZEN && frontends_drained) || phase==QUARANTINE;')]
    results=[]
    for label,target,before,after in cases:
        case=out/label;case.mkdir();paths={n:inputs/n for n in sources}
        if target:
            assert sources[target].count(before)==1;paths[target]=case/target;paths[target].write_text(sources[target].replace(before,after,1))
        command=['verilator','--binary','--timing','--assert','-j','4','--top-module','tb','-Wno-fatal',
            '--Mdir',str(case/'obj'),'-o','test',*map(str,paths.values())]
        (case/'commands.json').write_text(json.dumps([command,[str(case/'obj/test')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/test')],capture_output=True,text=True,timeout=240)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:700],flush=True)
        if target:assert result.returncode and any(s in text for s in ('ordinary MSA raw controls differ','real raw-service snapshot word differs','real raw-service scan timeout','rotation lost real raw service hold')),text
        else:assert not result.returncode and 'EIGHT MSA SDRAM PAUSE PASS'in text and 'EIGHT MSA QUARANTINE PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,
        physical_bits=12304,msa_boards=8,raw_groups=128,shared_pause_coordinators=1,
        executed_inputs={n:hashlib.sha256((inputs/n).read_bytes()).hexdigest()for n in sources},
        limits='Actual eight MSA/128-group physical memory path. Native main-board wiring, full wrapper/ARM state, fit/timing and complete FPGA snapshot remain pending.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
