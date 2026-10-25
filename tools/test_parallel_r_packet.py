#!/usr/bin/env python3
"""Replay the full split packet with only SINGLE R-fault bit1 parallelized.

Require actual1517-input child equivalence, X/Z gate and bad-fallback rejection.
Audit unchanged cache/ready/owner/RAM cones and every cofactor input before
compilation; retain the full original result/storage/clock/reset/geometry bench.
"""
import argparse
from pathlib import Path
from audit_parallel_r_fault import audit,evidence
from generate_parallel_r_fault import packet
from generate_cached_r_packet import stage,cache
from test_cached_r_packet import run

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','proof','fault-proof','four-state','bad-unknown','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--mutation',choices=['early-write','drop-forward','stale-intermediate','stale-root','wrong-fall-cram','drop-fall-clock','invert-low','stale-low','bad-unknown'])
    a=p.parse_args();source=a.source.resolve()
    evidence(a.fault_proof.resolve(),a.four_state.resolve(),a.bad_unknown.resolve(),source)
    raw=(source/'candidate.sv').read_text();audit(raw,packet(raw)+'\n'+stage(raw,True)+'\n'+cache(raw))
    raise SystemExit(run(source,a.proof.resolve(),a.output.resolve(),a.engine,a.mutation,True,True,
        packet_builder=packet,extra_tools=[Path(__file__),*[Path(__file__).with_name(n) for n in ['generate_parallel_r_fault.py','generate_split_r_packet.py','audit_parallel_r_fault.py','audit_split_r_packet.py']]],variant='split-parallel-fault'))
