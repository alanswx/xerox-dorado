#!/usr/bin/env python3
"""Exercise the actual staged native wrappers from original BaseBoard power-up.

Diagnostic live-boundary/clock/hold test, with no disk/network server and a
physical storage-array backend. It cannot qualify Initial/Cedar boot
or speed. Compiles the actual changed native wrappers, not generic boards.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
BENCH = r'''`timescale 1ns/1ps
`default_nettype none
module tb_r_front_ready_native;
 reg host_clk=0,advance=0,reset=1;
 always #10 host_clk=~host_clk;
 wire guest_clk=host_clk & advance;
 wire [31:0] bb,processor;
 wire [4:0] instructions;
 wire board_ready;
 wire [127:0] req_valid,req_write,rsp_ready;
 wire [127:0] req_ready=~rsp_valid | rsp_ready;
 wire [2047:0] req_addr,req_data;
 reg [127:0] rsp_valid=0;
 reg [2047:0] rsp_data=0;
 // Independent raw storage chips; no CPU instructions/IOCBs are serviced.
 reg [15:0] backing [0:127][0:65535];
 always @(posedge host_clk) begin
  for(integer group_no=0;group_no<128;group_no=group_no+1)begin
   if(reset)rsp_valid[group_no]<=0;
   else begin
    if(rsp_valid[group_no] && rsp_ready[group_no])rsp_valid[group_no]<=0;
    if(req_valid[group_no] && req_ready[group_no])begin
     rsp_valid[group_no]<=1;
     rsp_data[16*group_no+:16]<=backing[group_no][req_addr[16*group_no+:16]];
     if(req_write[group_no])backing[group_no][req_addr[16*group_no+:16]]<=req_data[16*group_no+:16];
    end
   end
  end
 end
 dorado_native_backplane #(.SYSPER(4)) dut(
   .sys_clk(guest_clk),.storage_clk(host_clk),.contb_advance(advance),
   .baseboard_boot_n(1'b1),.storage_reset(reset),.storage_ready(board_ready),
   .storage_req_valid(req_valid),.storage_req_write(req_write),.storage_req_ready(req_ready),
   .storage_req_addr(req_addr),.storage_req_data(req_data),.storage_rsp_ready(rsp_ready),
   .storage_rsp_valid(rsp_valid),.storage_rsp_data(rsp_data),
   .baseboard_debug(bb),.processor_debug(processor),.instruction_signals(instructions),
   .RcvData(1'b0),.Collision(1'b0),
   .ClockM0__in(1'b0),.ClockP0__in(1'b0),.DataM0__in(1'b0),.DataP0__in(1'b0));
 integer cycles=0,limit=2000000,cp=0,issues=0,root_edges=0,paused=0,aliases=0,unsettled=0;
 reg prev_cp=0,prev_issue=0,prev_root=0;
 reg [751:0] saved;
 initial begin
  void'($value$plusargs("cycles=%d",limit));
  while(cycles<limit) begin
   @(negedge host_clk);#1;
   cycles=cycles+1;
   if(!advance)begin
     if(dut.r_front_q!==saved)$fatal(1,"native R-front state advanced during backend pause");
     paused=paused+1;
   end
   saved=dut.r_front_q;
   if(cycles>1024 && !dut.r_front_alias_ok)begin
     $display("NATIVE_ALIAS_FAIL cycle=%0d CA=%b PH=%b PL=%b Hhold=%b PRhold=%b LPRhold=%b FFokH=%b FFokL=%b roots=%b%b%b",
        cycles,dut.r_front_alias_ContA,dut.r_front_alias_ProcH,dut.r_front_alias_ProcL,
        dut.b_ProcH.Holda,dut.b_ProcH.PRhold,dut.b_ProcL.PRhold,
        dut.b_ProcH.FFok_p_a,dut.b_ProcL.FFok_p_b,dut.CLK_ca_p_,dut.CLK_ph_p_,dut.CLK_pl_p_);
     $fatal(1,"native boundary alias contract failed");
   end
   if(cycles>1024)aliases=aliases+1;
   if(dut.r_front_unsettled)unsettled=unsettled+1;
   if(dut.CPStrb_p_ && !prev_cp)cp=cp+1;
   if(instructions[4] && !prev_issue)issues=issues+1;
   if(dut.CLK_ca_p_ && !prev_root)root_edges=root_edges+1;
   prev_cp=dut.CPStrb_p_;prev_issue=instructions[4];prev_root=dut.CLK_ca_p_;
   if(cycles%100000==0)$display("NATIVE_STARTUP cycle=%0d bb=%h processor=%h cp=%0d issues=%0d roots=%0d unsettled=%0d",
       cycles,bb,processor,cp,issues,root_edges,unsettled);
   // Gate the machine clock and its matching host-clock storage enables.
   reset=(cycles<32);
   advance=!reset && board_ready && (cycles%100003>=17);
  end
  if(root_edges<1000 || paused<100 || aliases<100000)$fatal(1,"native diagnostic coverage incomplete");
  $display("NATIVE_R_FRONT_STARTUP_BOUNDARIES PASS cycles=%0d cp=%0d issues=%0d roots=%0d pauses=%0d aliases=%0d unsettled=%0d;physical storage array;no boot qualification",
    cycles,cp,issues,root_edges,paused,aliases,unsettled);
  $finish;
 end
endmodule
`default_nettype wire
'''


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(candidate, out):
    r = json.loads((candidate / 'r-front-ready-native.json').read_text())
    for n, h in r['source_hashes'].items():
        assert sha(candidate / n) == h, n
    qsf = (candidate / 'core/Dorado.qsf').read_text()
    sources = set()
    macros = []
    for kind, value in re.findall(r'^set_global_assignment -name (SYSTEMVERILOG_FILE|VERILOG_FILE|VERILOG_MACRO)\s+([^\n]+)', qsf, re.M):
        value = value.strip().strip('"')
        if kind == 'VERILOG_MACRO':
            macros.append(value)
        else:
            n = value.replace(r['project'] + '/', '')
            if n.startswith('../src/'):
                n = n[3:]
            if n.startswith('src/'):
                assert n in r['source_hashes'], n
                sources.add(n)
    # Unreachable FPGA shell source definitions need no vendor simulation
    # models. All reachable board/cell definitions retain the candidate bytes.
    out.mkdir(parents=True, exist_ok=False)
    frozen = out / 'executed-inputs'
    for n in r['source_hashes']:
        if n.startswith('src/'):
            p = frozen / n
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_bytes((candidate / n).read_bytes())
    (frozen / 'tb_r_front_ready_native.sv').write_text(BENCH)
    (frozen / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (out / 'provenance.json').write_text(json.dumps(dict(scope=__doc__,
        candidate_sha256=sha(candidate / 'r-front-ready-native.json'), sources=sorted(sources),
        macros=macros,inputs={str(p.relative_to(frozen)):sha(p) for p in frozen.rglob('*') if p.is_file()},
        firmware_boot_qualified=False), indent=2)+'\n')
    print('Prepared actual native startup diagnostic',len(sources),'source files', flush=True)


def run(out, cycles):
    r = json.loads((out / 'provenance.json').read_text())
    frozen = out / 'executed-inputs'
    for n, h in r['inputs'].items():
        assert sha(frozen / n) == h, n
    state_path = out / 'run-state.json'
    assert not state_path.exists(), 'Observe existing process instead of restarting'
    state = dict(status='RUNNING',actions=[])
    def execute(command, label):
        with (out / (label+'.log')).open('x') as log:
            process = subprocess.Popen(list(map(str,command)),cwd=frozen/'src',stdout=log,stderr=subprocess.STDOUT)
            row = dict(stage=label,pid=process.pid,command=list(map(str,command)),started=time.time())
            state['actions'].append(row)
            state_path.write_text(json.dumps(state,indent=2)+'\n')
            code = process.wait()
            row.update(returncode=code,finished=time.time())
            state_path.write_text(json.dumps(state,indent=2)+'\n')
            assert code==0, (label,out/(label+'.log'))
    cmd = ['verilator','--binary','--timing','--top-module','tb_r_front_ready_native','-j','4',
        '-Wno-fatal','-Wno-PINMISSING','-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN','-Wno-LATCH',
        '--Mdir',out/'obj','-o','test',
        *['+define+'+m for m in r['macros']],
        '-I'+str(frozen/'src'),'-I'+str(frozen/'src/verilog/cells'),
        '-I'+str(frozen/'src/verilog/generated'),frozen/'tb_r_front_ready_native.sv',
        *[frozen/n for n in r['sources']]]
    try:
        execute(cmd,'compile')
        execute([out/'obj/test','+cycles='+str(cycles)],'test')
        assert 'NATIVE_R_FRONT_STARTUP_BOUNDARIES PASS' in (out/'test.log').read_text()
        state['status']='PASS_NATIVE_STARTUP_BOUNDARY_DIAGNOSTIC_ONLY'
    except BaseException:
        state['status']='TERMINAL_FAILED'
        raise
    finally:
        state_path.write_text(json.dumps(state,indent=2)+'\n')
    print(state['status'],flush=True)


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    group=p.add_mutually_exclusive_group(required=True)
    group.add_argument('--prepare',type=Path)
    group.add_argument('--run',type=Path)
    p.add_argument('--candidate',type=Path)
    p.add_argument('--cycles',type=int,default=2000000)
    a=p.parse_args()
    if a.prepare:
        assert a.candidate
        prepare(a.candidate.resolve(),a.prepare.resolve())
    else:
        run(a.run.resolve(),a.cycles)
