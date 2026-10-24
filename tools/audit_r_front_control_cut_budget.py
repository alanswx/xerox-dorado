#!/usr/bin/env python3
"""Reject the nonoverlapped two-edge cut as a 60 ns native scheduler.

Bind the arithmetic to the frozen native clock-generator source and original
N=16 digital phase table. A normal instruction consumes two 30 ns source
clock periods. A nonoverlapped two-host-edge transaction at EACH distributed
root transition consumes eight 15 ns ticks per instruction, or 120 ns.
This rejects that specific schedule. It does not rule out a phase-specific
partition or an overlapped pipeline with proved state forwarding/consumers.
"""
import argparse
import gzip
import json
from pathlib import Path

from test_r_front_forward_view import sha


def phases(n):
    half=max(1,n//2);ecp_low=2*n//16;scp_low=4*n//16
    scp=[int(not(scp_low<=p<scp_low+half))for p in range(n)]
    ecp=[int(not(ecp_low<=p<ecp_low+half))for p in range(n)]
    root=[s|e for s,e in zip(scp,ecp)]
    transitions=sum(root[p]!=root[(p-1)%n]for p in range(n))
    return dict(n=n,scp_n=scp,ecp=ecp,inactive_debug_root=root,
                root_transitions_per_source_period=transitions)


def audit(source,out):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    clock='src/verilog/cells/cell_CLOCKGEN.v';p=source/'inputs'/clock
    assert sha(p)==r['inputs'][clock]
    raw=p.read_text()
    for statement in ['parameter integer N = 16','phase + 1\'b1',
                      'localparam integer ECP_LO = (2 * N) / 16;',
                      'localparam integer SCP_LO = (4 * N) / 16;']:
        assert statement in raw,statement
    original=phases(16);current=phases(4);target=phases(2)
    assert original['scp_n']==[int(v)for v in '1111000000001111']
    assert original['ecp']==[int(v)for v in '1100000000111111']
    assert all(p['root_transitions_per_source_period']==2 for p in [original,current,target])
    assert r['host_clock_hz']==57142857
    assert current['n']*2==8
    target_tick_ns=15;source_periods_per_instruction=2
    transitions_per_instruction=2*source_periods_per_instruction
    required_ticks_per_instruction=60//target_tick_ns
    nonoverlapped_ticks=2*transitions_per_instruction
    assert required_ticks_per_instruction==4 and nonoverlapped_ticks==8
    row=dict(status='REJECT_NONOVERLAPPED_CONTROL_CUT_FOR_60NS',scope=__doc__,
             source=str(source),source_provenance_sha256=sha(source/'provenance.json.gz'),
             clock_source=clock,clock_source_sha256=sha(p),phase_tables=[original,current,target],
             source_periods_per_instruction=source_periods_per_instruction,
             root_transitions_per_instruction=transitions_per_instruction,
             qualified_instruction_host_ticks=8,qualified_ordinary_instruction_ns=140,
             target_host_clock_hz=200000000/3,target_host_tick_ns=target_tick_ns,
             target_instruction_ns=60,target_instruction_host_ticks=required_ticks_per_instruction,
             nonoverlapped_control_cut_host_ticks_per_transition=2,
             nonoverlapped_instruction_host_ticks=nonoverlapped_ticks,
             nonoverlapped_instruction_ns=nonoverlapped_ticks*target_tick_ns,
             nonoverlapped_nominal_issue_ceiling_per_second=1e9/(nonoverlapped_ticks*target_tick_ns),
             small_N_phase_quantization_still_unqualified=True,
             derived_root_table_scope='Inactive debug source only; no claim of complete native clock/Run/Step behavior.',
             native_schedule_rejected=True,quartus_build_started=False,
             next_required_work='Bind control/operand boundaries to existing original capture edges, or prove overlapped initiation/state forwarding and all external consumers.',
             complete_profile_matrix_still_rejected=True,native_qualified=False,
             assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    q=out/'inputs'/clock;q.parent.mkdir(parents=True);q.write_bytes(p.read_bytes())
    (out/'source-provenance.json.gz').write_bytes((source/'provenance.json.gz').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (out/'results.json').write_text(json.dumps(row,indent=2)+'\n')
    print(row['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();audit(a.source.resolve(),a.output.resolve())
