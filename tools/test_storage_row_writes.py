#!/usr/bin/env python3
"""Check same-row WRITE streaming, actual capture-before-ACK and reset drain.

Extend the independently scored burst-reuse protocol fixture, including its
small command-pin memory test double. Independent XSDS timing, original MSA,
queued/native128 transport and native integration remain separate gates.
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
from prepare_storage_row_writes import transform
from test_storage_burst_reuse import bench as reuse_bench

ROOT=Path(__file__).resolve().parents[1]
WRITES=r'''
    integer streamed_writes=0,write_ack_checks=0,held_write_blocks=0;
    integer command_streak=0,longest_write_streak=0;
    integer accepted_write_count=0;
    reg write_owner=0;
    reg [25:0] write_address=0;
    reg [15:0] write_value=0;
    always @(posedge clk)begin
        #1;
        if(!reset && rsp_valid && !pending)$fatal(1,"ROW_WRITE orphan asserted response");
    end
    always @(negedge clk)begin
        if(command==3'b100)begin
            command_streak++;
            if(command_streak>longest_write_streak)longest_write_streak=command_streak;
        end else command_streak=0;
    end
    always @(posedge clk)begin
        if(reset)write_owner=0;
        else begin
            if(rsp_valid && rsp_ready && write_owner)begin
                if(physical_writes<=accepted_write_count)
                    $fatal(1,"ROW_WRITE ACK before chip capture");
                if(physical_read(write_address)!==write_value)
                    $fatal(1,"ROW_WRITE chip captured wrong data");
                write_ack_checks++;write_owner=0;
            end
            if(req_valid && req_ready)begin
                write_owner=req_write;
                if(req_write)begin
                    accepted_write_count=physical_writes;write_address=req_addr;write_value=req_data;
                end
                if(dut.state==7)begin
                    if(!ROW_WRITES || !req_write || !rsp_valid || !rsp_ready ||
                       dut.aborted || dut.refresh_due || req_addr[25:10]!=dut.address[25:10])
                        $fatal(1,"ROW_WRITE forbidden successor");
                    streamed_writes++;
                end
            end
            if(req_valid && req_write && rsp_valid && !rsp_ready && !req_ready)held_write_blocks++;
        end
    end
    task automatic write_stream(input reg[25:0] base,input integer count,mode);
        integer sent,received,local_ticks;
        begin
            sent=0;received=0;local_ticks=0;
            operation(0,base,0);operation(0,base+8,0);
            @(negedge clk);
            while(received<count)begin
                req_valid=sent<count;req_write=mode!=3 || sent%3!=2;
                case(mode)
                0:req_addr=base+26'((sent*3)&31);
                1:req_addr=base+26'((sent*3)&7)+(sent%4==0 ? 26'h400 : 26'd0);
                2:req_addr=base+26'((sent*3)&7)+(26'(sent%8)<<23);
                default:req_addr=base+26'((sent*3)&7);
                endcase
                req_data=16'hc200 ^ 16'(sent*53) ^ 16'(mode*307);
                rsp_ready=mode==0 || local_ticks%13<8;
                @(posedge clk);
                if(rsp_valid && rsp_ready)received++;
                if(req_valid && req_ready)sent++;
                local_ticks++;
                if(local_ticks>count*24+500)$fatal(1,"ROW_WRITE stream deadline");
                @(negedge clk);
            end
            req_valid=0;req_write=0;rsp_ready=0;
            // All neighbors and both primed raw-cache lines must reflect the
            // physical writes, including successors beyond the first line.
            for(integer a=0;a<32;a++)operation(0,base+26'(a),a%3);
            $display("ROW_WRITE_STREAM mode=%0d count=%0d ticks=%0d",mode,count,local_ticks);
        end
    endtask
    task automatic write_abort(input reg[25:0] address,input integer position);
        begin
            rsp_ready=0;offer(1,address,16'hd600^address[15:0]);
            if(position==0)begin
                // Wait for the command-emitting edge, including a cold row's
                // ACT_WAIT. Resetting in SELECT before ACTIVATE merely sets
                // aborted early and cannot test the same-edge reset guard.
                while(!((dut.state==dut.SELECT && dut.open[dut.owner] &&
                         dut.rows[dut.owner]==dut.address[22:10]) ||
                        (dut.state==dut.ACT_WAIT && dut.delay_count==0)))@(negedge clk);
            end else begin
                while(command!=3'b100)@(negedge clk);
                if(position==2)@(negedge clk);
            end
            // Include a single-clock reset overlapping SELECT/WRITE. A longer
            // reset would clear an incorrectly rearmed ACK on its next edge.
            reset=1;repeat(position==0 ? 1 : 2)@(negedge clk);reset=0;
            repeat(30)begin @(negedge clk);if(rsp_valid)$fatal(1,"ROW_WRITE stale reset ACK");end
            operation(0,address,0);
        end
    endtask
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def bench(original):
    text=reuse_bench(original)
    text=replace_once(text,'parameter integer ENABLED=1,','parameter integer ENABLED=1,\n    parameter integer ROW_WRITES=0,')
    text=replace_once(text,'.BURST_READ_REUSE(ENABLED)) dut(','.BURST_READ_REUSE(ENABLED),.PIPELINED_ROW_WRITES(ROW_WRITES)) dut(')
    text=text.replace('[0:255]','[0:2047]').replace('==256','==2048')
    text=replace_once(text,"            other_address=base^26'h80000;", '''            // A changed write latency moves the free-running refresh phase.
            // Give this directed future-abort probe a fresh natural window;
            // do not assume its offer will beat an imminent refresh.
            if(abort_future!=0 && ENABLED)
                while(dut.state!=2 || dut.refresh_count>=dut.REFRESH_CLOCKS-64)@(negedge clk);
            other_address=base^26'h80000;''')
    text=replace_once(text,'physical_writes!=32','physical_writes!=logical_used')
    anchor='    initial begin\n        integer owner,first,kind,beat,r0,r1;'
    text=replace_once(text,anchor,WRITES+anchor)
    anchor='        // Start just ahead of the natural refresh threshold'
    text=replace_once(text,anchor,'''        for(owner=0;owner<8;owner++)
            write_stream((26'(owner)<<23)+26'hc0000,32,0);
        write_stream(26'hd0000,1024,0);
        write_stream(26'he0000,64,1);
        write_stream(26'hf0000,64,2);
        write_stream(26'h100000,64,3);
        for(beat=0;beat<3;beat++)write_abort(26'h110000+26'(beat*16),beat);
'''+anchor)
    anchor='        $display("REUSE_COVERAGE past='
    text=replace_once(text,anchor,'''        if(write_ack_checks<1400 || held_write_blocks==0 ||
           (ROW_WRITES && (streamed_writes<1000 || longest_write_streak<16)) ||
           (!ROW_WRITES && (streamed_writes!=0 || longest_write_streak>1)))
            $fatal(1,"ROW_WRITE coverage missing acks=%0d streamed=%0d streak=%0d",write_ack_checks,streamed_writes,longest_write_streak);
        $display("ROW_WRITE_COVERAGE enabled=%0d acks=%0d streamed=%0d held_blocks=%0d streak=%0d",ROW_WRITES,write_ack_checks,streamed_writes,held_write_blocks,longest_write_streak);
'''+anchor)
    return text


def run(generated,parent,output,controls):
    assert not output.exists(),'Inspect existing work'
    proof=json.loads((generated/'provenance.json').read_text());assert proof['pipelined_row_writes']
    for name,digest in proof['generated_hashes'].items():assert sha(generated/name)==digest
    assert transform(parent.read_bytes())==(generated/'dorado_storage_sdram_burst.sv').read_bytes()
    output.mkdir(parents=True);inputs=output/'inputs';inputs.mkdir()
    for source,name in [(generated/'dorado_storage_sdram_burst.sv','candidate.sv'),(parent,'parent.sv'),
        (ROOT/'verilog/verilator/tb_storage_burst_successor.sv','original-tb.sv'),
        (ROOT/'tools/test_storage_burst_reuse.py','test_storage_burst_reuse.py'),
        (ROOT/'tools/prepare_storage_row_writes.py','prepare.py'),(Path(__file__),Path(__file__).name)]:
        shutil.copyfile(source,inputs/name)
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
            action=dict(label=label,pid=p.pid,command=command,started=time.time());state['actions'].append(action);save()
            action.update(returncode=p.wait(),finished=time.time());save()
        return action['returncode']
    def case(label,engine,source,enabled=1,hz=57142857,diagnostic=None):
        label=engine+'-'+label
        if engine=='icarus':
            obj=output/(label+'.vvp')
            command=['iverilog','-g2012','-s','tb_storage_burst_successor',
                '-Ptb_storage_burst_successor.ROW_WRITES='+str(enabled),'-Ptb_storage_burst_successor.CLOCK_HZ='+str(hz),
                '-o',obj,source,inputs/'tb.sv'];run_command=['vvp',obj]
        else:
            obj=output/('obj-'+label)
            command=['verilator','--binary','--timing','--autoflush','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
                '--top-module','tb_storage_burst_successor','-GROW_WRITES='+str(enabled),'-GCLOCK_HZ='+str(hz),
                '--Mdir',obj,'-o','test',source,inputs/'tb.sv'];run_command=[obj/'test']
        assert execute(label+'-compile',command)==0,label
        assert 'UNOPTFLAT'not in(output/(label+'-compile.log')).read_text()
        rc=execute(label+'-test',run_command);log=(output/(label+'-test.log')).read_text()
        if diagnostic:
            alternatives=[diagnostic]if isinstance(diagnostic,str)else diagnostic
            found=[s for s in alternatives if s in log];assert rc!=0 and found,(label,rc,log[-2200:])
            state['results'][label]=dict(rejected=True,diagnostic=found)
        else:
            assert rc==0 and 'BURST_SUCCESSOR PASS'in log,(label,rc,log[-2200:])
            coverage=re.search(r'^ROW_WRITE_COVERAGE.*$',log,re.M).group()
            state['results'][label]=dict(passed=True,coverage=coverage,output=log)
        save();print(label,state['results'][label].get('coverage','rejected'),flush=True)
    save()
    try:
        for engine in ['icarus','verilator']:
            case('off57',engine,inputs/'candidate.sv',enabled=0)
            case('on57',engine,inputs/'candidate.sv')
            case('on66',engine,inputs/'candidate.sv',hz=66666667)
        if controls:
            body=(inputs/'candidate.sv').read_text()
            changes=[
                ('ACK-before-command','            if(req_write)begin\n                for(integer i=0;i<CACHE_LINES;i=i+1)',
                 '            if(req_write)begin\n                if(PIPELINED_ROW_WRITES)response_pending<=1;\n                for(integer i=0;i<CACHE_LINES;i=i+1)', 'ROW_WRITE ACK before chip capture'),
                ('wrong-successor-data','command<=WRITE;dq_out<=req_data;dq_oe<=1;',
                 "command<=WRITE;dq_out<=req_data ^ 16'h0001;dq_oe<=1;",'ROW_WRITE chip captured wrong data'),
                ('wrong-row','offered_address[25:10]==address[25:10];',"1'b1;",['ROW_WRITE forbidden successor','ROW_WRITE chip captured wrong data']),
                ('ignore-held-ACK','!aborted && req_write && response_pending && rsp_ready &&',
                 '!aborted && req_write && response_pending &&',['ROW_WRITE forbidden successor','SUCCESSOR overwrote outstanding request']),
                ('accept-read','!aborted && req_write && response_pending && rsp_ready &&',
                 '!aborted && response_pending && rsp_ready &&','ROW_WRITE forbidden successor'),
                ('duplicate-write-ACK','if(!PIPELINED_ROW_WRITES && !aborted && !reset)response_pending<=1;',
                 'if(!aborted && !reset)response_pending<=1;',['ROW_WRITE orphan asserted response','SUCCESSOR orphan or duplicate','SUCCESSOR data/order']),
                ('stale-neighbor-line','if(line_valid[i] && line_tag[i]==offered_address[25:3])line_valid[i]<=0;',
                 "if(1'b0)line_valid[i]<=0;",'SUCCESSOR data/order'),
                ('reply-after-reset','if(PIPELINED_ROW_WRITES && !aborted && !reset)response_pending<=1;',
                 'if(PIPELINED_ROW_WRITES && !aborted)response_pending<=1;', ['ROW_WRITE stale reset ACK','ROW_WRITE orphan asserted response']),
            ]
            for label,old,new,diagnostic in changes:
                assert body.count(old)==1,old
                bad=output/(label+'.sv');bad.write_text(body.replace(old,new))
                for engine in ['icarus','verilator']:case(label,engine,bad,diagnostic=diagnostic)
        assert all(sha(inputs/name)==digest for name,digest in frozen.items())
        for label in ['off57','on57','on66']:
            assert state['results']['icarus-'+label]['coverage']==state['results']['verilator-'+label]['coverage']
        state.update(status='PASS_PIPELINED_ROW_WRITES_PROTOCOL_TWO_SIMULATORS',finished=time.time(),
            inputs_unchanged=True,independent_physical_pins_or_native_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['generated','parent','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--controls',action='store_true');args=parser.parse_args()
    run(args.generated.resolve(),args.parent.resolve(),args.output.resolve(),args.controls)
