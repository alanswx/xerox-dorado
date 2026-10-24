#!/usr/bin/env python3
"""Replay split paired/single functions with every original processor check.

Use the geometry-wide950-bit control theorem; full source-bound cache history
and timing qualification must additionally bind the new packet wiring before
selection. Preserve every original result/RAM/clock/reset/geometry assertion.
"""
import argparse
from pathlib import Path
from generate_split_r_packet import packet
from test_cached_r_packet import run

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--mutation',choices=['early-write','drop-forward','stale-intermediate','stale-root','wrong-fall-cram','drop-fall-clock'])
    a=p.parse_args()
    raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.engine,a.mutation,True,True,
        packet_builder=packet,extra_tools=[Path(__file__),Path(__file__).with_name('generate_split_r_packet.py')],variant='split-pair'))
