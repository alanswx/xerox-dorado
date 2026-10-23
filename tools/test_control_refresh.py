#!/usr/bin/env python3
"""Replay the captured RunRefresh-release mismatch with a source-only edge.

The fixture is the actual pre-edge state from joint event403. Pins reconstruct
the deterministic trial35 input. Compare release versus held WantRunRfsh and
alter only phase-FF delay as a causal diagnostic, not a new operating waiver.
"""
import argparse
from pathlib import Path
import re
import sys
from unittest.mock import patch
import test_control_hold as parent

CAPTURED_STATE=int('300000426f4000a2a82a0000000080f6030dc46',16)
PROFILES={'balanced':(1000,1000,1000),'early_phase':(1750,400,1250),'delayed_phase':(1750,400,2000),
          'early_bounded':(1750,2000,1500)}


def captured_pins():
    p=0x128|(1<<292)|(7<<289)|(1<<263)|(1<<(326+7))|(1<<(326+8))|(1<<(326+46))|(1<<(326+28))|(1<<(326+60))|(1<<(326+61))
    p|=1<<6;rng=0x156ca046
    def field(lo,width,value):
        nonlocal p
        mask=((1<<width)-1)<<lo;p=(p&~mask)|((value<<lo)&mask)
    def random_word():
        nonlocal rng
        rng=(rng^(rng<<13))&0xffffffff;rng=(rng^(rng>>17))&0xffffffff;rng=(rng^(rng<<5))&0xffffffff;return rng
    for trial in range(36):
        field(246,8,random_word());field(254,8,random_word());field(326+28,18,random_word())
        for bit,value in [(4,(trial>>4)&1),(5,1-((trial>>5)&1)),(6,(trial>>6)&1),(3,(trial>>7)&1),
                          (326+60,1-(trial&1)),(326+61,1-((trial>>1)&1)),(326+62,(trial>>2)&1),(326+63,(trial>>3)&1)]:field(bit,1,value)
    return p


def run(out,engine,profile,hold_want):
    raw,count=re.subn(r'localparam \[397:0\] IDLE=.*?;',"localparam [397:0] IDLE=398'h"+format(captured_pins(),'x')+';',parent.TB,count=1,flags=re.S);assert count==1
    assert raw.count('fixture[150]=1;')==1;raw=raw.replace('fixture[150]=1;','')
    start=raw.index(" initial begin\n  seed({156{1'b1}});")
    value=CAPTURED_STATE|((1<<19)if hold_want else 0)
    raw=raw[:start]+f''' initial begin
  seed(156'h{value:x});event_step(0,reference_pins);
  $display("CONTROL_HOLD_REFRESH source_only_edge=1 held_WantRunRfsh={int(hold_want)} phase_delay_ps={PROFILES[profile][2]}");
  event_step(1,reference_pins);
  $display("CONTROL_HOLD PASS checks=%0d physical=156 clocks=56 refresh_hold={int(hold_want)}",checks);$finish;
 end
endmodule
'''
    with patch.object(parent,'TB',raw),patch.dict(parent.PROFILES,PROFILES):
        return parent.run(out,engine,profile)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--profile',choices=PROFILES,default='early_phase');p.add_argument('--hold-want',action='store_true')
    a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.profile,a.hold_want))
