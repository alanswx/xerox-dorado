#!/usr/bin/env python3
"""Prove logical RIOT RAM and registered reads after an accepted reset.

Hidden backing bits remain arbitrary. A ghost reset/read qualifier identifies
when the original uninitialized RAM has acquired defined reset history; it
does not constrain any input or initialize production state. Four-state tests
separately cover the pre-reset X state and unknown controls/addresses/data.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
TOP='''module riot_contract(
 input wire clk,advance,reset,read_enable,write_enable,
 input wire [6:0] address,input wire [7:0] write_data,
 output wire safe_memory,safe_read);
 wire [7:0] read_data;wire [1023:0] logical_memory;
 dorado_riot_ram dut(clk,advance,reset,read_enable,write_enable,address,write_data,read_data,logical_memory);
 reg [7:0] original_memory[0:127];reg [7:0] original_read;
 reg seen_reset=0,qualified_read=0;
 wire [1023:0] original_image;
 for(genvar i=0;i<128;i++)assign original_image[8*i+:8]=original_memory[i];
 always @(posedge clk)if(advance)begin
  if(read_enable)begin original_read<=original_memory[address];qualified_read<=seen_reset;end
  if(reset)begin
   for(integer i=0;i<128;i++)original_memory[i]<=0;
   seen_reset<=1;
  end else if(write_enable)original_memory[address]<=write_data;
 end
 assign safe_memory=!seen_reset || logical_memory==original_image;
 assign safe_read=!qualified_read || read_data==original_read;
 always @*begin assert(safe_memory);assert(safe_read);end
endmodule
'''


def run(tests,out,yosys):
    assert not out.exists()
    t=json.loads((tests/'run-state.json').read_text());verify(tests,t['files'])
    assert t['status']=='PASS_RESET_MASKED_RIOT_BLOCK_RAM_COMPONENT'
    helper=ROOT/'verilog/rtl/dorado_riot_ram.sv'
    assert sha(helper)==t['production_helper_sha256']
    raw=helper.read_text()
    old='output wire [7:0] read_data\n);'
    new='output wire [7:0] read_data,output wire [1023:0] logical_memory\n);'
    assert raw.count(old)==1
    export=' for(genvar i=0;i<128;i++)assign logical_memory[8*i+:8]=written[i] ? backing[i] : 8\'b0;\n'
    extended=raw.replace(old,new,1).replace('endmodule',export+'endmodule',1)
    assert extended.replace(export,'',1).replace(new,old,1)==raw
    out.mkdir(parents=True);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),helper_sha256=sha(helper),
               tests_state_sha256=sha(tests/'run-state.json'),assumptions=0,
               contract='All logical RAM bytes after one accepted reset; read output after a read with reset history. Inputs and hidden backing payload arbitrary.',actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,mutation in [('positive',None),('missing-mask',("read_written ? read_payload : 8'b0","read_payload")),
                           ('new-data',('if(read_enable) read_payload<=backing[address];','if(read_enable) read_payload<=write_enable ? write_data : backing[address];'))]:
        d=out/label;d.mkdir();actual=extended
        if mutation:
            a,b=mutation;assert actual.count(a)==1;actual=actual.replace(a,b,1)
        (d/'ram.sv').write_text(actual);(d/'top.sv').write_text(TOP)
        sat=('sat -timeout 120 -seq 3 -tempinduct -verify -prove-asserts'if mutation is None else
             'sat -timeout 120 -seq 5 -prove-asserts -falsify -show-ports -dump_json /work/counterexample.json')
        script='read_verilog -formal -sv /work/ram.sv /work/top.sv\nprep -top riot_contract\nflatten\nchformal -lower\nmemory_map\nopt\ncheck -assert\n'+sat+'\n'
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_MOUNT']='/work='+str(d);env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache')
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log,env=env)
            action=dict(label=label,pid=child.pid,command=cmd,started=time.time());state['actions'].append(action);save();code=child.wait()
        marker='Induction step proven: SUCCESS!'if mutation is None else'model found: FAIL!'
        action.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not action['passed']:
            state.update(status='TERMINAL_RIOT_RAM_PROOF_NOT_ESTABLISHED',finished=time.time());save()
            raise AssertionError((label,(d/'proof.log').read_text()[-1600:]))
    state.update(status='PASS_RESET_HISTORY_RIOT_RAM_INDUCTION',finished=time.time(),
                 logical_memory_bits=1024,registered_read_bits=8,
                 native_integration_or_physical_timing_or_firmware_qualified=False,
                 files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()and q.name!='run-state.json'})
    save();print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['tests','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.tests.resolve(),a.output.resolve(),a.yosys.resolve())
