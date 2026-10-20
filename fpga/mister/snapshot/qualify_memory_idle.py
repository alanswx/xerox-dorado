#!/usr/bin/env python3
"""Qualify passive idle outputs on the actual arbiter/SDRAM/DDR adapters.

Inverse bytes must match the immutable Serial65 source. Existing independent
transaction and physical-pin scoreboards remain; added external-handshake
monitors reject idle during queued/accepted/unconsumed operations. Autonomous
SDRAM refresh must remain active and report no guest work after drainage.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import subprocess
from generate_serial_boards import ROOT,raw_receipt

MONITOR=r'''
    bit idle_pending=0;
    integer idle_checks=0,idle_quiet_seen=0,idle_busy_seen=0,idle_refresh_seen=0;
    always @(posedge clk)begin
        if(reset)idle_pending=0;
        else begin
            if(ACCEPT)idle_pending=1;
            if(CONSUME)idle_pending=0;
        end
    end
    always @(negedge clk)begin
        #0.001;
        idle_checks++;
        if(snapshot_idle)idle_quiet_seen++;
        if(idle_pending)idle_busy_seen++;
        if((reset || OFFER || idle_pending || RESPONSE) && snapshot_idle)
            $fatal(1,"snapshot idle asserted during queued/pending work");
        EXTRA
    end
'''


def sha(raw):return hashlib.sha256(raw).hexdigest()


def fixture(name,text):
    specs={
        'arb':('.clk(clk),.reset(reset),',
               '(|(valid&accepted))','(|(response&accept_response))','(|valid)','(|response)',''),
        'sdram':('.clk(clk),.reset(reset),','req_valid && req_ready','rsp_valid && rsp_ready','req_valid','rsp_valid',
                 '''if(initialized && !reset && !req_valid && !idle_pending && {nras,ncas,nwe}==3'b001)begin
            if(!snapshot_idle)$fatal(1,"autonomous refresh lost guest-idle acknowledgment");
            idle_refresh_seen++;
        end'''),
        'ddram':('.clk(clk),.reset(reset),','valid && accepted','response && ready','valid','response','')}
    anchor,accept,consume,offer,response,extra=specs[name]
    assert text.count(anchor)==1
    text=text.replace(anchor,'.snapshot_idle(snapshot_idle),'+anchor,1)
    code=MONITOR
    for key,value in [('ACCEPT',accept),('CONSUME',consume),('OFFER',offer),('RESPONSE',response),('EXTRA',extra)]:code=code.replace(key,value)
    check='''if(!idle_quiet_seen || !idle_busy_seen)$fatal(1,"missing idle/busy coverage");
        $display("MEMORY-IDLE PASS checks=%0d quiet=%0d busy=%0d refresh=%0d",idle_checks,idle_quiet_seen,idle_busy_seen,idle_refresh_seen);
        '''
    if name=='sdram':
        anchor='        if(chip0.error_count!=0 || chip1.error_count!=0 ||'
        assert text.count(anchor)==1
        text=text.replace(anchor,'''        repeat(1500)@(negedge clk);
        if(idle_refresh_seen<4)$fatal(1,"missing quiescent autonomous refresh");
        '''+check+anchor,1)
    else:
        anchor='$display("STORAGE-ARB PASS'if name=='arb'else'$display("STORAGE-DDRAM PASS'
        assert text.count(anchor)==1,(name,anchor)
        text=text.replace(anchor,check+anchor,1)
    assert text.count('endmodule')==1
    return text.replace('endmodule','wire snapshot_idle;\n'+code+'\nendmodule',1)


def inverse(name,source):
    port='    output wire snapshot_idle,\n';assert source.count(port)==1;source=source.replace(port,'',1)
    if name=='arb':extra='''    // Observation only. Include queued producers as well as a latched owner.
    assign snapshot_idle=!reset && state==IDLE && !(|req_valid);
'''
    elif name=='sdram':extra='''    // Guest/raw work is idle during autonomous refresh. The physical SDRAM
    // clock and refresh controller must continue while frontends are frozen.
    // This does not claim that banks are closed or controller state is saved.
    assign snapshot_idle=initialized && !reset && !req_valid && !response_pending &&
        (state==IDLE || state==REF_PRE1 || state==REF_PRE_WAIT ||
         state==REF_WAIT0 || state==REF_WAIT1);
'''
    else:extra='''    // Adapter-idle only; physical DDR-fabric write drainage is a separate
    // lifecycle/snapshot requirement. No state or normal handshake changes.
    assign snapshot_idle=!reset && state==IDLE && !req_valid;
'''
    assert source.count(extra)==1
    return source.replace(extra,'',1)


def run(out):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    report=ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-ready';source_proof={};results=[]
    for name in ('arb','sdram','ddram'):
        rtl='dorado_storage_'+name+'.sv';path=ROOT/'fpga/mister'/rtl;source=path.read_text()
        original=raw_receipt(report,'terminal-fit/src/fpga/mister/'+rtl)
        assert inverse(name,source).encode()==original,'unapproved normal logic change: '+name
        source_proof[name]=dict(source_sha256=sha(source.encode()),frozen_original_sha256=sha(original),new_state_bits=0,new_output_ports=['snapshot_idle'])
        (inputs/rtl).write_text(source);bench='tb_storage_'+name+'.sv'
        raw=(ROOT/'verilog/verilator'/bench).read_text();(inputs/('original-'+bench)).write_text(raw)
        (inputs/bench).write_text(fixture(name,raw))
    model=ROOT/'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv'
    (inputs/model.name).write_bytes(model.read_bytes())
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    cases=[('arb128','arb',None,None),('arb16','arb',None,None),('sdram50','sdram',None,None),('ddram','ddram',None,None),
        ('arb_ignores_queued','arb','state==IDLE && !(|req_valid)','state==IDLE'),
        ('sdram_ignores_response','sdram','assign snapshot_idle=initialized && !reset && !req_valid && !response_pending &&',
         'assign snapshot_idle=initialized && !reset && !req_valid &&'),
        ('sdram_blocks_refresh','sdram','(state==IDLE || state==REF_PRE1 || state==REF_PRE_WAIT ||\n         state==REF_WAIT0 || state==REF_WAIT1)','state==IDLE'),
        ('ddram_ignores_response','ddram','state==IDLE && !req_valid','(state==IDLE || state==RESPONSE) && !req_valid')]
    for label,name,before,after in cases:
        case=out/label;case.mkdir();rtl='dorado_storage_'+name+'.sv';source=(inputs/rtl).read_text()
        if before:assert source.count(before)==1;source=source.replace(before,after,1)
        (case/rtl).write_text(source);bench=inputs/('tb_storage_'+name+'.sv');top='tb_storage_'+name
        command=['verilator','--binary','--timing','--assert','-j','4','--top-module',top,'-Wno-fatal',
            '--Mdir',str(case/'obj'),'-o','test']
        if name=='arb':command +=['-GGROUPS='+('16'if label=='arb16'else'128'),'-GPREFETCH_NEXT=1','-GINTERLEAVE_GROUPS=1','-GGROUP_ADDR_BITS=16']
        if name=='sdram':command +=['-GCLOCK_HZ=50000000']
        command +=[str(case/rtl),str(bench)]
        if name=='sdram':command +=[str(inputs/model.name)]
        (case/'commands.json').write_text(json.dumps([command,[str(case/'obj/test')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/test')],capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip()[:500],flush=True)
        if before:assert result.returncode and any(s in text for s in ('snapshot idle asserted','autonomous refresh lost')),text
        else:assert not result.returncode and 'MEMORY-IDLE PASS'in text and 'STORAGE-'in text,text
        results.append(dict(case=label,returncode=result.returncode))
    (out/'qualification.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,source_proof=source_proof,cases=results,
        inputs={p.name:sha(p.read_bytes())for p in inputs.iterdir()},
        limits='Passive per-adapter signals only, not native pause integration, SDRAM bank closure/controller snapshot, physical DDR-fabric write drain or a complete machine snapshot.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
