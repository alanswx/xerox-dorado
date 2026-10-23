#!/usr/bin/env python3
"""Bind conservative staging's refresh contract to the frozen native cells.

The current native l06/l03 clocks are Boolean-identical and sampled on the
same host edge. Nonblocking Q updates leave old RunRefresh at l03 Set during
capture. This qualifies that existing native transform only, not original
analog arrival order or the future60ns partition. Never alter a live run.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT=Path(__file__).resolve().parents[1]
SOURCES=['src/fpga/mister/snapshot_native/cell_MC10231_snapshot.v',
         'src/fpga/mister/snapshot_native_conta/cell_SE10231_snapshot.v',
         'src/verilog/cells/cell_SE10210.v']
BOARD='src/fpga/mister/snapshot_native_conta/ContA_full_snapshot.sv'
TB=r'''`timescale 1ns/1ps
module tb_native_refresh_order;
 reg host=0,hold_state=1,write_state=1,advance=1,root=0,stop_run=0,want=0,cpu_run=0;
 reg [17:0] source_seed=0,sink_seed=0;wire refresh,ma,mb,pre_d,pre_a,sink_clock;
 wire [17:0] source_state,sink_state;
 integer checks=0,release_cases=0;
 cell_SE10210 l01(.p7(root),.p11(root),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(1'b0),.p3(pre_d),.p13(pre_a));
 cell_SE10210 f13(.p7(1'b0),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(stop_run),.p11(pre_a),.p14(sink_clock));
 wire dmem=cpu_run|(want&!refresh);
 cell_SE10231_snapshot l06(.state_host_clk(host),.state_advance(advance),.state_hold(hold_state),.state_write(write_state),
  .state_data(source_seed),.state_result(source_state),.sys_clk(host),.p6(1'b0),.p7(1'b0),.p9(pre_d),.p10(want),.p11(stop_run),.p13(1'b0),.p15(refresh));
 cell_MC10231_snapshot l03(.state_host_clk(host),.state_advance(advance),.state_hold(hold_state),.state_write(write_state),
  .state_data(sink_seed),.state_result(sink_state),.sys_clk(host),.p4(1'b0),.p5(refresh),.p6(1'b0),.p7(dmem),.p9(sink_clock),
  .p10(dmem),.p11(1'b0),.p12(refresh),.p13(1'b0),.p2(ma),.p15(mb));
 task automatic tick;begin #9;host=1;#9;host=0;#1;end endtask
 initial begin
  for(integer sr=0;sr<2;sr++)for(integer rf=0;rf<2;rf++)for(integer wr=0;wr<2;wr++)for(integer cr=0;cr<2;cr++)begin
   reg old_refresh,old_mem,expected;reg old_d;
   hold_state=1;write_state=1;root=0;stop_run=sr[0];want=wr[0];cpu_run=cr[0];
   old_refresh=rf[0];old_mem=rf[0];old_d=cpu_run|(want&!old_refresh);
   source_seed={12'b0,stop_run,stop_run,1'b0,want,1'b0,old_refresh};
   sink_seed={12'b0,stop_run,stop_run,old_d,old_d,old_mem,old_mem};tick();
   hold_state=0;write_state=0;tick();
   if((pre_d|stop_run)!==sink_clock)$fatal(1,"Native source and sink clocks diverge");
   expected=old_refresh?1'b1:(!stop_run?old_d:old_mem);
   if(ASSUME_EARLY && !stop_run)expected=want?1'b1:old_d;
   root=1;tick();
   if(ma!==expected||mb!==expected)$fatal(1,"NATIVE_REFRESH_ORDER mismatch check=%0d old_refresh=%b want=%b cpu=%b stop=%b expected=%b actual=%b%b",checks,old_refresh,want,cpu_run,stop_run,expected,ma,mb);
   if(!stop_run&&old_refresh&&!want&&!cpu_run)release_cases++;
   tick();if(ma!==expected||mb!==expected)$fatal(1,"Native clock history introduced another capture");checks++;
  end
  if(checks!=16||release_cases!=1)$fatal(1,"Native refresh contract coverage incomplete");
  $display("NATIVE_REFRESH_ORDER PASS checks=%0d release_cases=%0d old_Set_at_capture=1",checks,release_cases);$finish;
 end
endmodule
'''


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(native_run,out,early):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'inputs';hashes={}
    for n in SOURCES+[BOARD]:
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((native_run/'inputs'/n).read_bytes());hashes[n]=sha(p)
    raw=(inputs/BOARD).read_text();bound={}
    expected={'l01':{3:'prepreclk_p_d',7:'CLK_ca_p_',11:'CLK_ca_p_',13:'prepreclk_p_a'},
              'f13':{10:'StopRunClock',11:'prepreclk_p_a',14:'preRunClk_p_Bc'},
              'l06':{9:'prepreclk_p_d',11:'StopRunClock',15:'RunRefresh'},
              'l03':{5:'RunRefresh',9:'preRunClk_p_Bc',12:'RunRefresh'}}
    for pkg,pins in expected.items():
        m=re.search(r'^  (cell_\w+)([^\n]*) u_'+pkg+r'\s*\(\n(.*?)^  \);',raw,re.M|re.S);assert m
        observed={int(k):v for k,v in re.findall(r'\.p(\d+)\(([^()]*)\)',m[3])}
        assert all(observed[k]==v for k,v in pins.items()),pkg
        assert m[1]==('cell_SE10210'if pkg in ['l01','f13']else'cell_SE10231_snapshot'if pkg=='l06'else'cell_MC10231_snapshot')
        assert not m[2].strip(),('Unexpected capture parameters',pkg,m[2])
        bound[pkg]=dict(kind=m[1],pins=pins)
        if pkg=='l01':assert all(i not in observed for i in [5,6,9,10])
        if pkg=='f13':assert 9 not in observed
    gate=(inputs/SOURCES[2]).read_text()
    assert re.search(r'assign\s+p3\s*=\s*\(p5\s*\|\s*p6\s*\|\s*p7\)',gate)
    assert re.search(r'assign\s+p14\s*=\s*\(p9\s*\|\s*p10\s*\|\s*p11\)',gate)
    tb=out/'tb.sv';tb.write_text(TB.replace('ASSUME_EARLY',"1'b1"if early else"1'b0"))
    commands=[['iverilog','-g2012','-s','tb_native_refresh_order','-o',str(out/'test.vvp'),str(tb),*[str(inputs/n)for n in SOURCES]],['vvp',str(out/'test.vvp')]]
    actions=[]
    for i,cmd in enumerate(commands):
        p=subprocess.run(cmd,capture_output=True,text=True);(out/('compile.log'if not i else'test.log')).write_text(p.stdout+p.stderr)
        actions.append(dict(command=cmd,returncode=p.returncode))
        if p.returncode:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=actions[-1]['returncode']==0 and 'NATIVE_REFRESH_ORDER PASS checks=16 release_cases=1 old_Set_at_capture=1'in log
    r=dict(status='PASS_FROZEN_NATIVE_REFRESH_ORDER'if passed else'NATIVE_REFRESH_ORDER_FAILED',inputs=hashes,actions=actions,physical_clock_bindings=bound,
           testbench_sha256=sha(tb),assume_early=early,scope=__doc__,native_refresh_before_run_capture=0,
           original_analog_order_qualified=False,future60ns_partition_qualified=False,live_run_changed=False)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status']);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--native-run',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--assume-early',action='store_true')
    a=p.parse_args();sys.exit(run(a.native_run.resolve(),a.output.resolve(),a.assume_early))
