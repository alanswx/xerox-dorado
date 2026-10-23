#!/usr/bin/env python3
"""Run WRITE streaming/ACK/reset oracle against independent XSDS chip pins.

Replace the protocol test double with actual chip models. Seed only the finite
fixture's initial test patterns before controller startup; every subsequent
write goes through real command/DQ pins. Compare the physical write count and
stored value at EACH consumed write ACK. Native IO timing remains separate.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prepare_storage_response_pipeline import replace_once
from test_storage_row_writes import bench as protocol_bench

ROOT=Path(__file__).resolve().parents[1]
CHIPS=r'''
    reg [25:0] logical_address[0:2047];
    reg [15:0] logical_value[0:2047];
    integer logical_used=0;
    function automatic [15:0] logical_read(input reg [25:0] address);
        reg [15:0] value;
        begin
            value=pattern(address);
            for(integer j=0;j<logical_used;j++)
                if(logical_address[j]==address)value=logical_value[j];
            logical_read=value;
        end
    endfunction
    wire chip_clk=~clk;
    assign chip_data=dq_oe ? dq_out : 16'hzzzz;
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("ROW_RANK0"),
        .INIT_UNWRITTEN_TO_X(0),.tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip0(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(chip_data));
    as4c32m16sb_6tin_chip_model #(.CHIP_NAME("ROW_RANK1"),
        .INIT_UNWRITTEN_TO_X(0),.tRC_MIN(63.0),.tRFC_MIN(63.0),.tRCD_MIN(21.0),.tRP_MIN(21.0),
        .tRRD_MIN(14.0),.tMRD_MIN(14.0),.tWR_MIN(14.0),.tAC_MAX(6.0)) chip1(
        .Clk(chip_clk),.Cke(1'b1),.Cs_n(~ncs),.Ras_n(nras),.Cas_n(ncas),.We_n(nwe),
        .Ba(ba),.Addr(a),.Ldqm(a[11]),.Udqm(a[12]),.Dq(chip_data));
    integer physical_reads=0,physical_writes=0,refresh0=0,refresh1=0;
    reg [25:0] read_base=0;
    always @(posedge chip_clk)begin
        if(command==3'b101)begin
            physical_reads++;
            read_base={ncs,ba,13'(ncs ? chip1.open_row[int'(ba)] : chip0.open_row[int'(ba)]),a[9:0]};
        end
        if(command==3'b100)physical_writes++;
        if(command==3'b001)begin if(ncs)refresh1++;else refresh0++;end
    end
    function automatic [15:0] physical_read(input reg [25:0] address);
        begin
            if(address[25])physical_read=chip1.mem_read(int'(address[24:23]),int'(address[22:10]),int'(address[9:0]));
            else physical_read=chip0.mem_read(int'(address[24:23]),int'(address[22:10]),int'(address[9:0]));
        end
    endfunction
    task automatic seed_range(input reg[25:0] base,input integer count);
        reg [25:0] address;
        begin
            for(integer j=0;j<count;j++)begin
                address=base+26'(j);
                if(address[25])chip1.mem_write(int'(address[24:23]),int'(address[22:10]),int'(address[9:0]),pattern(address),0,0);
                else chip0.mem_write(int'(address[24:23]),int'(address[22:10]),int'(address[9:0]),pattern(address),0,0);
            end
        end
    endtask
    task automatic seed_initial_patterns;
        begin
            for(integer owner=0;owner<8;owner++)begin
                seed_range((26'(owner)<<23)+26'h20000,128);
                seed_range((26'(owner)<<23)+26'hc0000,32);
                seed_range((26'(owner)<<23)+26'hf0000,32);
            end
            seed_range(26'h40000,128);seed_range(26'h50000,32);
            seed_range(26'h60000,128);seed_range(26'h70000,8);
            seed_range(26'h90000,1024);seed_range(26'h10000,1024);
            seed_range(26'ha0000,128);seed_range(26'hb0000,8);seed_range(26'h30000,8);
            seed_range(26'hd0000,32);seed_range(26'he0000,32);seed_range(26'he0400,8);
            seed_range(26'h100000,32);seed_range(26'h110000,48);
        end
    endtask
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def bench(original):
    text=protocol_bench(original)
    text=replace_once(text,"    reg [15:0] req_data=0,chip_data=0;","    reg [15:0] req_data=0;\n    wire [15:0] chip_data;")
    start=text.index('    reg [25:0] physical_address[0:2047]')
    end=text.index('    integer ticks=0,accepted=',start)
    text=text[:start]+CHIPS+text[end:]
    text=replace_once(text,'        repeat(5)@(negedge clk);reset=0;wait(initialized);',
        '        #1;seed_initial_patterns();\n        repeat(5)@(negedge clk);reset=0;wait(initialized);')
    anchor='        $display("ROW_WRITE_COVERAGE enabled='
    text=replace_once(text,anchor,'''        if(chip0.error_count!=0 || chip1.error_count!=0 || chip0.warning_count!=0 || chip1.warning_count!=0)
            $fatal(1,"ROW_WRITE independent chip timing/model error");
'''+anchor)
    return text


def run(generated,output,controls):
    assert not output.exists(),'Inspect existing work'
    proof=json.loads((generated/'provenance.json').read_text());assert proof['pipelined_row_writes']
    for name,digest in proof['generated_hashes'].items():assert sha(generated/name)==digest
    output.mkdir(parents=True);inputs=output/'inputs';inputs.mkdir()
    for source,name in [(generated/'dorado_storage_sdram_burst.sv','candidate.sv'),
        (ROOT/'verilog/verilator/tb_storage_burst_successor.sv','original-tb.sv'),
        (ROOT/'tools/test_storage_row_writes.py','test_storage_row_writes.py'),
        (ROOT/'tools/test_storage_burst_reuse.py','test_storage_burst_reuse.py'),
        (ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv','xsds.sv'),
        (Path(__file__),Path(__file__).name)]:shutil.copyfile(source,inputs/name)
    (inputs/'tb.sv').write_text(bench((inputs/'original-tb.sv').read_text()))
    frozen={p.name:sha(p)for p in inputs.iterdir()}
    (output/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs=frozen,generated_proof=proof),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})
    def save():
        temp=output/'state.tmp';temp.write_text(json.dumps(state,indent=2)+'\n');os.replace(temp,output/'run-state.json')
    def execute(label,command):
        command=list(map(str,command))
        with(output/(label+'.log')).open('x')as log:
            p=subprocess.Popen(command,cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            row=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(row);save()
            row.update(returncode=p.wait(),finished=time.time());save()
        return row['returncode']
    def case(label,enabled,hz,source=None,diagnostic=None):
        obj=output/('obj-'+label)
        command=['verilator','--binary','--timing','--autoflush','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
            '-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-PINMISSING',
            '--top-module','tb_storage_burst_successor','-GROW_WRITES='+str(enabled),'-GCLOCK_HZ='+str(hz),
            '--Mdir',obj,'-o','test-pins',source or inputs/'candidate.sv',inputs/'xsds.sv',inputs/'tb.sv']
        assert execute(label+'-compile',command)==0,label
        assert 'UNOPTFLAT'not in(output/(label+'-compile.log')).read_text()
        rc=execute(label+'-test',[obj/'test-pins']);log=(output/(label+'-test.log')).read_text()
        if diagnostic:
            found=[s for s in diagnostic if s in log];assert rc!=0 and found,(label,rc,log[-2200:])
            state['results'][label]=dict(rejected=True,diagnostic=found)
        else:
            assert rc==0 and 'BURST_SUCCESSOR PASS'in log,(label,rc,log[-2200:])
            coverage=re.search(r'^ROW_WRITE_COVERAGE.*$',log,re.M).group()
            state['results'][label]=dict(passed=True,coverage=coverage,output=log)
        save();print(label,state['results'][label].get('coverage','rejected'),flush=True)
    save()
    try:
        for hz in [57142857,66666667]:
            case('off-'+str(hz),0,hz);case('on-'+str(hz),1,hz)
        if controls:
            body=(inputs/'candidate.sv').read_text()
            mutations=[
                ('ACK-before-command','            if(req_write)begin\n                for(integer i=0;i<CACHE_LINES;i=i+1)',
                 '            if(req_write)begin\n                if(PIPELINED_ROW_WRITES)response_pending<=1;\n                for(integer i=0;i<CACHE_LINES;i=i+1)', ['ROW_WRITE ACK before chip capture']),
                ('wrong-chip-data','command<=WRITE;dq_out<=req_data;dq_oe<=1;',
                 "command<=WRITE;dq_out<=req_data ^ 16'h0001;dq_oe<=1;",['ROW_WRITE chip captured wrong data']),
            ]
            for label,old,new,diagnostic in mutations:
                assert body.count(old)==1
                bad=output/(label+'.sv');bad.write_text(body.replace(old,new))
                case(label,1,57142857,bad,diagnostic)
        assert all(sha(inputs/name)==digest for name,digest in frozen.items())
        state.update(status='PASS_PIPELINED_ROW_WRITES_INDEPENDENT_PHYSICAL_PINS',finished=time.time(),inputs_unchanged=True,
            native_io_timing_firmware_or_speed_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['generated','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--controls',action='store_true');args=parser.parse_args()
    run(args.generated.resolve(),args.output.resolve(),args.controls)
