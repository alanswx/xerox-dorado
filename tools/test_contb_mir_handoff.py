#!/usr/bin/env python3
"""Isolate the ContB handoff gap with instruction data already stable.

The changed raw IM data first settles while RepeatCur holds the clock HIGH.
The subsequent simultaneous root rise / CBHold release still creates a pulse
in the illustrative original gate model. This diagnostic intentionally exposes
the unqualified candidate; it does not prove a physical board pulse or timing.
"""
import argparse
from pathlib import Path
import sys
from unittest.mock import patch
import test_contb_mir as parent


def run(out,engine):
    marker='  p[0]=1;event_pins(p);\n  for(integer trial=0;trial<512;trial++)begin'
    assert parent.TB.count(marker)==1
    probe='''  saved=q;p[45:28]=18'h2aaaa;event_pins(p);
  if(q!==saved)$fatal(1,"ContB stable handoff setup changed held state");
  $display("CONTB_STABLE_HANDOFF raw_IM_stable=1 before=%h",p);
  p[0]=1;p[3]=0;event_pins(p);
  $fatal(1,"ContB stable handoff diagnostic unexpectedly accepted");
  for(integer trial=0;trial<512;trial++)begin'''
    with patch.object(parent,'TB',parent.TB.replace(marker,probe)):
        return parent.run(out,engine)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine))
