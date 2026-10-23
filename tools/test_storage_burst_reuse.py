#!/usr/bin/env python3
"""Check received/current/future beats and completed-line service during BL8.

Extend the independently scored successor protocol bench, retaining its small
pin-driven functional memory. Physical XSDS timing and original MSA are separate
gates. The candidate stays default-off and is not selected by a native wrapper.
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
from prepare_storage_burst_reuse import transform

ROOT=Path(__file__).resolve().parents[1]
PROBES=r'''
    integer past_reuse=0,current_reuse=0,future_reuse=0,other_reuse=0,empty_reuse=0,future_aborts=0;
    integer pending_age=0;
    always @(posedge clk)begin
        if(reset || !pending || rsp_valid)pending_age=0;
        else pending_age++;
        if(pending_age>128)$fatal(1,"REUSE requested beat never completed");
    end
    always @(posedge clk)if(ENABLED && !reset && dut.state==6 && dut.delay_count==0)begin
        if(!dut.aborted && dut.fill_tag!==read_base[25:3])
            $fatal(1,"REUSE physical fill identity changed");
        if(req_valid && req_ready)begin
            if(!rsp_valid)empty_reuse++;
            if(dut.lookup_hit)other_reuse++;
            else if(dut.fill_valid[req_addr[2:0]])past_reuse++;
            else if(dut.fill_beat==req_addr[2:0])current_reuse++;
            else future_reuse++;
        end
    end
    task automatic reuse_probe(input reg[25:0] base,input integer first,wanted,other,abort_future);
        reg [25:0] other_address;
        begin
            other_address=base^26'h80000;
            // A fresh completed line occupies the slot before the cold fill,
            // so it cannot be the next victim in this four-line configuration.
            operation(0,other_address,0);
            rsp_ready=0;offer(0,base+26'(first),0);
            while(!rsp_valid)@(negedge clk);
            rsp_ready=1;
            offer(0,other!=0 ? other_address+26'(wanted) : base+26'(wanted),0);
            rsp_ready=0;
            if(abort_future!=0 && ENABLED)begin
                if(!dut.fill_waiting)$fatal(1,"REUSE future abort not exercised");
                reset=1;repeat(2)@(negedge clk);reset=0;future_aborts++;
                repeat(30)begin @(negedge clk);if(rsp_valid)$fatal(1,"REUSE stale future reply");end
            end else begin
                while(!rsp_valid)@(negedge clk);
                repeat(4)@(negedge clk);
                rsp_ready=1;@(negedge clk);rsp_ready=0;
            end
            operation(0,base+26'(first),0);
        end
    endtask
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def bench(text):
    text=replace_once(text,'.BURST_READ_SUCCESSOR(ENABLED)) dut(',
        '.BURST_READ_SUCCESSOR(0),.BURST_READ_REUSE(ENABLED)) dut(')
    text=replace_once(text,'''                    if(!ENABLED || req_write || !rsp_valid || !rsp_ready ||
                       dut.aborted || dut.refresh_due ||
                       req_addr[25:3]!=dut.address[25:3] || req_addr[2:0]!=dut.fill_beat)''', '''                    if(!ENABLED || req_write || dut.aborted || dut.refresh_due ||
                       !(dut.lookup_hit || req_addr[25:3]==dut.fill_tag))''')
    anchor='    initial begin\n        integer owner,first,kind,beat,r0,r1;'
    text=replace_once(text,anchor,PROBES+anchor)
    text=replace_once(text,'for(kind=0;kind<5;kind++)','for(kind=0;kind<4;kind++)')
    text=replace_once(text,"excluded!==5'h1f","excluded!==5'h0f")
    anchor='        // Start just ahead of the natural refresh threshold'
    text=replace_once(text,anchor,'''        for(first=0;first<8;first++)for(beat=0;beat<8;beat++)
            reuse_probe(26'h90000+26'((first*8+beat)*16),first,beat,0,0);
        for(beat=0;beat<8;beat++)reuse_probe(26'ha0000+26'(beat*16),0,beat,1,0);
        reuse_probe(26'hb0000,0,7,0,1);
'''+anchor)
    text=replace_once(text,"(ENABLED && forward_beats!==8'hfe)","(ENABLED && forward_beats!==8'hff)")
    anchor='        $display("BURST_SUCCESSOR PASS enabled='
    text=replace_once(text,anchor,'''        if(ENABLED && (past_reuse==0 || current_reuse==0 || future_reuse==0 ||
            other_reuse==0 || empty_reuse==0 || future_aborts!=1))
            $fatal(1,"REUSE coverage missing past=%0d current=%0d future=%0d other=%0d empty=%0d aborts=%0d",past_reuse,current_reuse,future_reuse,other_reuse,empty_reuse,future_aborts);
        $display("REUSE_COVERAGE past=%0d current=%0d future=%0d other=%0d empty=%0d aborts=%0d",past_reuse,current_reuse,future_reuse,other_reuse,empty_reuse,future_aborts);
'''+anchor)
    return text


def run(generated,parent,output,controls):
    assert not output.exists(),'Inspect existing work'
    proof=json.loads((generated/'provenance.json').read_text());assert proof['burst_read_reuse']
    for name,digest in proof['generated_hashes'].items():assert sha(generated/name)==digest
    assert transform(parent.read_bytes())==(generated/'dorado_storage_sdram_burst.sv').read_bytes()
    output.mkdir(parents=True);inputs=output/'inputs';inputs.mkdir()
    for source,name in [(generated/'dorado_storage_sdram_burst.sv','candidate.sv'),
        (parent,'parent.sv'),(ROOT/'verilog/verilator/tb_storage_burst_successor.sv','original-tb.sv'),
        (Path(__file__),Path(__file__).name),(ROOT/'tools/prepare_storage_burst_reuse.py','prepare.py')]:
        shutil.copyfile(source,inputs/name)
    (inputs/'tb.sv').write_text(bench((inputs/'original-tb.sv').read_text()))
    frozen={p.name:sha(p)for p in inputs.iterdir()}
    (output/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs=frozen,
        generated_proof=proof),indent=2)+'\n')
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
            binary=output/(label+'.vvp')
            command=['iverilog','-g2012','-s','tb_storage_burst_successor',
                '-Ptb_storage_burst_successor.ENABLED='+str(enabled),
                '-Ptb_storage_burst_successor.CLOCK_HZ='+str(hz),'-o',binary,source,inputs/'tb.sv']
            run_command=['vvp',binary]
        else:
            obj=output/('obj-'+label)
            command=['verilator','--binary','--timing','--autoflush','-j','2','-Wno-fatal',
                '-Werror-UNOPTFLAT','--top-module','tb_storage_burst_successor',
                '-GENABLED='+str(enabled),'-GCLOCK_HZ='+str(hz),'--Mdir',obj,'-o','test-reuse',source,inputs/'tb.sv']
            run_command=[obj/'test-reuse']
        assert execute(label+'-compile',command)==0,label
        assert 'UNOPTFLAT'not in(output/(label+'-compile.log')).read_text()
        rc=execute(label+'-test',run_command);log=(output/(label+'-test.log')).read_text()
        if diagnostic:
            alternatives=[diagnostic]if isinstance(diagnostic,str)else diagnostic
            found=[s for s in alternatives if s in log];assert rc!=0 and found,(label,rc,log[-2000:])
            state['results'][label]=dict(rejected=True,diagnostic=found)
        else:
            assert rc==0 and 'BURST_SUCCESSOR PASS'in log,(label,rc,log[-2000:])
            coverage=re.search(r'REUSE_COVERAGE.*',log).group()
            state['results'][label]=dict(passed=True,output=log,coverage=coverage)
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
                ('stale-valid-mask','fill_valid<=0;fill_waiting<=1;',"fill_valid<=8'hff;fill_waiting<=1;",'SUCCESSOR data/order'),
                ('wrong-fill-tag','wire [22:0] fill_tag=address[25:3];','wire [22:0] fill_tag=req_addr[25:3];','REUSE physical fill identity'),
                ('wrong-received-word','rsp_data<=fill_lookup_data;','rsp_data<=dq_capture;','SUCCESSOR data/order'),
                ('wrong-future-beat','fill_reply_beat<=req_addr[2:0];fill_waiting<=1;',"fill_reply_beat<=req_addr[2:0]+3'd1;fill_waiting<=1;",['SUCCESSOR data/order','REUSE requested beat never completed']),
                ('accept-before-primary','&& !req_write && !fill_waiting &&','&& !req_write &&', 'SUCCESSOR overwrote outstanding request'),
                ('allow-write','delay_count==0 && !aborted && !req_write && !fill_waiting &&',
                 'delay_count==0 && !aborted && !fill_waiting &&',['SUCCESSOR accepted forbidden','SUCCESSOR exclusion failed']),
                ('ignore-held-response','(!response_pending || (RESPONSE_OVERLAP && rsp_ready)) &&',
                 '(!response_pending || RESPONSE_OVERLAP) &&',['SUCCESSOR overwrote outstanding request','SUCCESSOR response changed']),
                ('wrong-completed-line','rsp_data<=lookup_data;response_pending<=1;cache_hit<=1;',
                 'rsp_data<=dq_capture;response_pending<=1;cache_hit<=1;','SUCCESSOR data/order'),
                ('overwrite-physical-owner','// No raw/physical ownership changes for a reused read.',
                 '// Broken physical owner.\n                    address<=offered_address;','REUSE physical fill identity'),
            ]
            for label,old,new,diagnostic in changes:
                assert body.count(old)==1,old
                bad=output/(label+'.sv');bad.write_text(body.replace(old,new))
                for engine in ['icarus','verilator']:case(label,engine,bad,diagnostic=diagnostic)
        assert all(sha(inputs/name)==digest for name,digest in frozen.items())
        state.update(status='PASS_BURST_READ_REUSE_PROTOCOL_TWO_SIMULATORS',finished=time.time(),
            inputs_unchanged=True,independent_physical_pins_or_native_qualified=False);save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());save();raise


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['generated','parent','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--controls',action='store_true');args=parser.parse_args()
    run(args.generated.resolve(),args.parent.resolve(),args.output.resolve(),args.controls)
