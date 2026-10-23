#!/usr/bin/env python3
"""Exercise queued admission with independent SDRAM pins and native128 fanout.

Pins mode reuses the independently scored descriptor/reset test with physical
XSDS chips instead of its delayed raw test double. Fanout mode compares the
qualified native arbiter to DEPTH4 admission, holding burst reuse enabled in
both. No CPU, native initialization mux or Cedar executes in either fixture.
"""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prepare_storage_response_pipeline import replace_once

ROOT=Path(__file__).resolve().parents[1]


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def fanout_bench(text):
    text=replace_once(text,'module tb_storage_burst_fanout;',
        'module tb_storage_burst_fanout #(parameter integer QUEUED=0);')
    start=text.index('    dorado_storage_arb #(')
    end=text.index('    tb_xsds_storage #',start)
    old=text[start:end]
    q=replace_once(old,'dorado_storage_arb #','dorado_storage_queue #')
    q=replace_once(q,'.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)) arb(',
        '.INTERLEAVE_GROUPS(1),.DEPTH(4)) arb(\n        .enable(1\'b1),.backend_reset(backing_reset),.reset_draining(),.idle(),')
    new='    wire backing_reset;wire [1:0] arb_state;\n    generate if(QUEUED)begin:q\n'+q+'        assign arb_state=arb.state;\n    end else begin:control\n'+old+'        assign backing_reset=reset;assign arb_state=arb.state;\n    end endgenerate\n'
    text=text[:start]+new+text[end:]
    text=replace_once(text,'.clk(clk),.reset(reset),.req_valid(rv)',
        '.clk(clk),.reset(backing_reset),.req_valid(rv)')
    text=replace_once(text,'else if(arb.state==2)','else if(arb_state==2)')
    text=replace_once(text,'    reg active=0;', '''    reg active=0;
    integer raw_issued=0;
    reg [22:0] golden_raw_address[0:1919];
    reg [15:0] golden_raw_data[0:1919];
    reg golden_raw_write[0:1919];''')
    old='''                    if(!rv || !rr || ra!==({7'b0,addresses[16*g+:16]}<<7 | 23'(g)) ||
                       rw!==writing[g] || wd!==write_data[16*g+:16])
                        $fatal(1,"FANOUT raw ownership/payload mismatch");'''
    text=replace_once(text,old,'''                    golden_raw_address[accepted]=({7'b0,addresses[16*g+:16]}<<7)|23'(g);
                    golden_raw_data[accepted]=write_data[16*g+:16];
                    golden_raw_write[accepted]=writing[g];''')
    old='''                held_data[16*g+:16]=read_data[16*g+:16];
            end
        end
    end'''
    text=replace_once(text,old,'''                held_data[16*g+:16]=read_data[16*g+:16];
            end
            if(rv && rr)begin
                if(raw_issued>=accepted || ra!==golden_raw_address[raw_issued] ||
                   wd!==golden_raw_data[raw_issued] || rw!==golden_raw_write[raw_issued])
                    $fatal(1,"QUEUE_FANOUT raw admission order/payload changed");
                raw_issued++;
            end
        end
    end''')
    text=replace_once(text,'accepted!=1920 || completed!=accepted || physical_writes!=640',
        'accepted!=1920 || completed!=accepted || raw_issued!=accepted || physical_writes!=640')
    return text


def pins_bench(text):
    text=replace_once(text,'reg clk=0;always #5 clk=~clk;',
        'reg clk=0;always #(500000000.0/57142857) clk=~clk;')
    text=replace_once(text,'    reg sv=0;\n    reg [15:0] sd=0;',
        '    wire sv;\n    wire [15:0] sd;')
    start=text.index('    reg [15:0] physical[0:WORDS-1]')
    end=text.index('    integer ticks=0,admitted=',start)
    new='''    reg [15:0] oracle[0:WORDS-1];
    reg raw_admit=1;
    integer physical_writes=0;
    wire physical_read,physical_write,backend_ready;
    assign rr=backend_ready && raw_admit;
    tb_xsds_storage #(.ADDR_BITS(RA)) memory(
        .clk(clk),.reset(backend_reset),.req_valid(rv && raw_admit),.req_write(rw),.rsp_ready(sr),
        .req_ready(backend_ready),.rsp_valid(sv),.req_addr(ra),.req_data(wd),.rsp_data(sd),
        .physical_read(physical_read),.physical_write(physical_write));
    always @(posedge clk)if(physical_write)physical_writes++;
    function automatic [15:0] peek16(input integer address);
        reg [63:0] value;
        begin value=memory.peek64(address/4);peek16=value[16*(address%4)+:16];end
    endfunction
'''
    text=text[:start]+new+text[end:]
    text=replace_once(text,'for(integer a=0;a<WORDS;a++)begin physical[a]=0;oracle[a]=0;end',
        'for(integer a=0;a<WORDS;a++)oracle[a]=0;')
    text=replace_once(text,'physical[a]!==oracle[a]','peek16(a)!==oracle[a]')
    text=replace_once(text,"physical[0]!==16'hab00","peek16(0)!==16'hab00")
    text=replace_once(text,'wait(!draining);@(negedge clk);',
        'wait(!draining && memory.initialized);@(negedge clk);')
    text=replace_once(text,'        ticks++;accepted_mask=0;', '''        ticks++;accepted_mask=0;
        if(backend_reset && issued!=retired)$fatal(1,"QUEUE_PINS backend reset before accepted completion");''')
    text=replace_once(text,'        $display("STORAGE_QUEUE PASS',
        '        memory.check_model();\n        $display("STORAGE_QUEUE PASS')
    return text


def run(mode,generated,fixture,output,controls,row_writes=False):
    assert not output.exists(),'Inspect existing recorded work'
    proof=json.loads((generated/'provenance.json').read_text());assert proof['burst_read_reuse']
    if row_writes:assert proof['pipelined_row_writes']
    for name,digest in proof['generated_hashes'].items():assert sha(generated/name)==digest
    output.mkdir(parents=True);inputs=output/'inputs';inputs.mkdir()
    for name in proof['generated_hashes']:shutil.copyfile(generated/name,inputs/name)
    for source,name in [(ROOT/'fpga/mister/dorado_storage_queue.sv','queue.sv'),
        (fixture,'original-fixture.sv'),(ROOT/'verilog/verilator/tb_storage_queue.sv','queue-protocol.sv'),
        (ROOT/'verilog/verilator/tb_storage_burst_fanout.sv','fanout.sv'),
        (ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv','xsds.sv'),
        (Path(__file__),Path(__file__).name)]:shutil.copyfile(source,inputs/name)
    text=(inputs/'original-fixture.sv').read_text()
    text=replace_once(text,'.BURST_READ_SUCCESSOR(0)) dut(',
        '.BURST_READ_SUCCESSOR(0),.BURST_READ_REUSE(1)) dut(')
    if row_writes:
        text=replace_once(text,'parameter integer ADDR_BITS=20)(',
            'parameter integer ADDR_BITS=20,parameter integer ROW_WRITES=0)(')
        text=replace_once(text,'.BURST_READ_REUSE(1)) dut(',
            '.BURST_READ_REUSE(1),.PIPELINED_ROW_WRITES(ROW_WRITES)) dut(')
    (inputs/'memory.sv').write_text(text)
    original=(inputs/('fanout.sv'if mode=='fanout'else'queue-protocol.sv')).read_text()
    tb=fanout_bench(original)if mode=='fanout'else pins_bench(original)
    if row_writes:
        if mode=='fanout':
            tb=replace_once(tb,'parameter integer QUEUED=0);','parameter integer QUEUED=0,ROW_WRITES=0);')
            tb=replace_once(tb,'.ADDR_BITS(23)) memory(','.ADDR_BITS(23),.ROW_WRITES(ROW_WRITES)) memory(')
        else:
            tb=replace_once(tb,'DEPTH=4,GROUP_ADDR_BITS=4);','DEPTH=4,GROUP_ADDR_BITS=4,ROW_WRITES=0);')
            tb=replace_once(tb,'.ADDR_BITS(RA)) memory(','.ADDR_BITS(RA),.ROW_WRITES(ROW_WRITES)) memory(')
    (inputs/'tb.sv').write_text(tb)
    frozen={p.name:sha(p)for p in inputs.iterdir()}
    (output/'provenance.json').write_text(json.dumps(dict(scope=__doc__,mode=mode,inputs=frozen,
        generated_proof=proof,pipelined_row_writes=row_writes),indent=2)+'\n')
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
    def case(label,parameters,source=None,diagnostic=None):
        obj=output/('obj-'+label);top='tb_storage_burst_fanout'if mode=='fanout'else'tb_storage_queue'
        command=['verilator','--binary','--timing','--autoflush','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
            '-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-PINMISSING',
            '--top-module',top,'--Mdir',obj,'-o','test',*parameters,
            source or inputs/'queue.sv',inputs/'dorado_storage_arb.sv',inputs/'dorado_storage_sdram_burst.sv',
            inputs/'memory.sv',inputs/'xsds.sv',inputs/'tb.sv']
        assert execute(label+'-compile',command)==0,label
        assert 'UNOPTFLAT'not in(output/(label+'-compile.log')).read_text()
        run_command=[obj/'test']+(['+trace='+str(output/(label+'.csv'))]if mode=='fanout'else[])
        rc=execute(label+'-test',run_command);log=(output/(label+'-test.log')).read_text()
        if diagnostic:
            found=[s for s in diagnostic if s in log];assert rc!=0 and found,(label,rc,log[-2000:])
            state['results'][label]=dict(rejected=True,diagnostic=found)
        else:
            marker='BURST_FANOUT PASS'if mode=='fanout'else'STORAGE_QUEUE PASS'
            assert rc==0 and marker in log,(label,rc,log[-2000:])
            summary=re.search('^'+marker+'.*$',log,re.M).group()
            state['results'][label]=dict(passed=True,summary=summary)
            if mode=='fanout':
                rows=re.findall(r'FANOUT_ROUND round=(\d+) mode=(\d+) groups=(\d+) ticks=(\d+) physical_reads=(\d+) forwarded=(\d+) hits=(\d+)',log)
                assert len(rows)==24
                state['results'][label].update(host_ticks=sum(int(r[3])for r in rows),
                    classes={int(k):int(v)for k,v in re.findall(r'FANOUT_CLASS id=(\d+) ticks=(\d+)',log)},trace_sha256=sha(output/(label+'.csv')))
        save();print(label,state['results'][label],flush=True)
    save()
    try:
        if mode=='fanout':
            case('control',['-GQUEUED=0']);case('queue',['-GQUEUED=1'])
            if row_writes:
                case('stream-unqueued',['-GQUEUED=0','-GROW_WRITES=1'])
                case('stream-queued',['-GQUEUED=1','-GROW_WRITES=1'])
            traces=[]
            for label in ['control','queue']+(['stream-unqueued','stream-queued']if row_writes else[]):
                with(output/(label+'.csv')).open()as stream:rows=list(csv.DictReader(stream))
                traces.append({kind:[{k:v for k,v in r.items()if k!='tick'and not(kind=='R'and r['write']=='1'and k=='data')}
                    for r in rows if r['kind']==kind]for kind in ['A','R']})
            assert all(t==traces[0]for t in traces[1:]),'Admission/completion order or data changed'
        else:
            case('depth4',['-GDEPTH=4']);case('depth3',['-GDEPTH=3'])
            if row_writes:
                case('stream-depth4',['-GDEPTH=4','-GROW_WRITES=1'])
                case('stream-depth3',['-GDEPTH=3','-GROW_WRITES=1'])
        if controls:
            body=(inputs/'queue.sv').read_text()
            if mode=='fanout':
                old='owner<=head_owner;active<=1;';new="owner<=head_owner ^ 1'b1;active<=1;"
                parameters=['-GQUEUED=1'];diagnostic=['FANOUT wrong owner','FANOUT response changed','STORAGE_QUEUE unowned completion','STORAGE_QUEUE unowned issue']
            else:
                old='(reset || reset_draining) && owned==0';new='(reset || reset_draining)'
                parameters=['-GDEPTH=4'];diagnostic=['QUEUE_PINS backend reset before accepted completion','QUEUE_TEST reset drain timeout']
            assert body.count(old)==1
            bad=output/'bad-queue.sv';bad.write_text(body.replace(old,new))
            case('bad-queue',parameters,bad,diagnostic)
        assert all(sha(inputs/name)==digest for name,digest in frozen.items())
        state.update(status='PASS_STORAGE_QUEUE_'+mode.upper(),finished=time.time(),inputs_unchanged=True,
            native_boot_reset_mux_or_hardware_speed_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode',choices=['fanout','pins'],required=True)
    for name in ['generated','fixture','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--controls',action='store_true');parser.add_argument('--row-writes',action='store_true')
    args=parser.parse_args()
    run(args.mode,args.generated.resolve(),args.fixture.resolve(),args.output.resolve(),args.controls,args.row_writes)
