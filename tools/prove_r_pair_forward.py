#!/usr/bin/env python3
"""Prove packet forwarding against the original four-bank write/read equations.

The packet certificate requires the later read to use the SAME physical bank
addresses and selection. Four arbitrary9-bit words therefore represent exactly
those addressed cells; every other original SRAM cell is unchanged by either
schedule. Compare all18 read bits after the original per-bank write, including
odd parity, for all bank selections/write gates. No guest-state restriction.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
TOP='''module forwarding_equivalence(
 input wire [35:0] words,input wire [15:0] data,input wire [1:0] write_n,
 input wire [3:0] select_n,output wire equal,output wire [17:0] actual,expected);
 wire [8:0] wh={~^data[7:0],data[7:0]},wl={~^data[15:8],data[15:8]};
 wire [8:0] rmh=words[8:0],stkh=words[17:9],rml=words[26:18],stkl=words[35:27];
 wire [8:0] old_h=(select_n[0] ? 9'b0:rmh)|(select_n[1] ? 9'b0:stkh);
 wire [8:0] old_l=(select_n[2] ? 9'b0:rml)|(select_n[3] ? 9'b0:stkl);
 wire [8:0] new_rmh=(!write_n[0] && !select_n[0]) ? wh:rmh;
 wire [8:0] new_stkh=(!write_n[0] && !select_n[1]) ? wh:stkh;
 wire [8:0] new_rml=(!write_n[1] && !select_n[2]) ? wl:rml;
 wire [8:0] new_stkl=(!write_n[1] && !select_n[3]) ? wl:stkl;
 wire [8:0] new_h=(select_n[0] ? 9'b0:new_rmh)|(select_n[1] ? 9'b0:new_stkh);
 wire [8:0] new_l=(select_n[2] ? 9'b0:new_rml)|(select_n[3] ? 9'b0:new_stkl);
 wire [17:0] original_read={old_l[8],old_h[8],old_l[7:0],old_h[7:0]};
 dorado_r_pair_read_forward actual_step(original_read,data,write_n,select_n,actual,);
 assign expected={new_l[8],new_h[8],new_l[7:0],new_h[7:0]};
 assign equal=actual==expected;
endmodule
'''


def run(out,yosys):
    assert not out.exists();source=ROOT/'verilog/rtl/dorado_r_pair_read_forward.sv';ram=ROOT/'build/rtl-executive/pure-processor-verilator-v1-20261024/inputs/verilog/rtl/dorado_r_file_ports.sv'
    raw=source.read_text();original=ram.read_text()
    # Bind the word abstraction to the actual bank/odd-parity/OR semantics.
    for part in ["data_h={~^write_data[7:0],write_data[7:0]}","data_l={~^write_data[15:8],write_data[15:8]}",
                 "if(!select_n[0]) rm_h[address[7:0]]<=data_h;","if(!select_n[1]) stk_h[address[15:8]]<=data_h;",
                 "if(!select_n[2]) rm_l[address[23:16]]<=data_l;","if(!select_n[3]) stk_l[address[31:24]]<=data_l;",
                 "(read_select_n[0] ? 9'b0 : rm_h[read_address[7:0]]) |", "(read_select_n[3] ? 9'b0 : stk_l[read_address[31:24]])"]:
        assert part in original,part
    out.mkdir(parents=True);(out/'original-file-ports.sv').write_text(original);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source_sha256=sha(source),original_RAM_source_sha256=sha(ram),
        compared_read_bits=18,arbitrary_word_bits=36,bank_selects_arbitrary=True,requires_same_physical_addresses_and_bank_selection=True,
        hardware_changed=False,cases=[])

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    for variant in ['positive','drop-forward','bad-parity']:
        d=out/variant;d.mkdir();actual=raw
        if variant=='drop-forward':actual=actual.replace('assign forwarded[0]=!write_n[0] && !(&select_n[1:0]);',"assign forwarded[0]=1'b0;",1)
        if variant=='bad-parity':actual=actual.replace('~^write_data[7:0]','^write_data[7:0]',1)
        (d/'actual.sv').write_text(actual);(d/'top.sv').write_text(TOP)
        flag='verify' if variant=='positive' else 'falsify'
        (d/'proof.ys').write_text(f'''read_verilog -sv /work/actual.sv /work/top.sv
hierarchy -check -top forwarding_equivalence
proc
flatten
opt
check -assert
sat -prove equal 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        files={p.name:sha(p) for p in d.iterdir() if p.is_file()}
        with (d/'proof.log').open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env);row=dict(variant=variant,pid=p.pid,command=cmd,files=files,started=time.time())
            state['cases'].append(row);save();code=p.wait()
        text=(d/'proof.log').read_text();passed=code==0 and ('no model found: SUCCESS!' if variant=='positive' else 'model found: FAIL!') in text
        row.update(passed=passed,returncode=code,finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        if not passed:break
    passed=len(state['cases'])==3 and all(v['passed'] for v in state['cases'])
    state.update(status='PASS_ORIGINAL_BANK_WRITE_FORWARDING' if passed else 'TERMINAL_PACKET_FORWARD_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--yosys',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.output.resolve(),a.yosys.resolve()))
