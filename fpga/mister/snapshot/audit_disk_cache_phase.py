#!/usr/bin/env python3
"""Independent inverse review of the opposite-edge disk cache-tag prototype.

Only two host-falling-edge implementation registers and the exact cache source
substitution are allowed. Every original tag/host/serial/buffer process, port,
parameter and clock remains byte-identical to the qualified native receipt.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt

INSERT='''  // Cache capture excludes a guest edge: preserve the preceding half-cycle
  // values, giving the guest-clock source a full half-cycle hold separation.
  reg [11:0] cache_source_cyl = INITIAL_CYL;
  reg [5:0] cache_source_head = INITIAL_HEAD;
  always @(negedge clk_sys) begin
    cache_source_cyl <= cyl; cache_source_head <= head;
  end
'''


def reference(reports):
    return raw_receipt(reports/'2026-10-19-snapshot-serial-chipwidth',
                       'terminal-fit/src/fpga/mister/dorado_pack_hps.sv')


def audit(raw,reports):
    source=raw.decode();before=reference(reports)
    if source.count(INSERT)!=1:raise ValueError('opposite-edge state/clock/reset contract differs')
    restored=source.replace(INSERT,'',1)
    new='cached_cyl <= cache_source_cyl; cached_head <= cache_source_head;'
    if restored.count(new)!=1:raise ValueError('cache capture sources differ')
    restored=restored.replace(new,'cached_cyl <= cyl; cached_head <= head;',1)
    if restored.encode()!=before:raise ValueError('unapproved original disk logic/port/clock/parameter edit')
    return dict(status='PASS_DISK_PHASE_SOURCE_ONLY',scope=__doc__,
                original_sha256=hashlib.sha256(before).hexdigest(),candidate_sha256=hashlib.sha256(raw).hexdigest(),
                original_register_bits=131816,new_required_wrapper_snapshot_bits=18,
                unchanged_original_ports=True,unchanged_original_processes=True,new_timing_exceptions=0,
                limits='Source proof only; additional state needs inventory and snapshot access; native integration/timing separate.')


def controls(raw,reports):
    cases=[('wrong_edge','always @(negedge clk_sys)','always @(posedge clk_sys)'),
           ('wrong_source','cache_source_cyl <= cyl;','cache_source_cyl <= cached_cyl;'),
           ('lost_head_bit','reg [5:0] cache_source_head','reg [4:0] cache_source_head'),
           ('wrong_init','cache_source_cyl = INITIAL_CYL','cache_source_cyl = 0'),
           ('original_tag_edit','if (cyltag_d && !cyltag_n)','if (!cyltag_n)'),
           ('old_capture','cached_cyl <= cache_source_cyl;','cached_cyl <= cyl;')]
    source=raw.decode();out=[]
    for name,before,after in cases:
        assert source.count(before)==1,name;damaged=source.replace(before,after,1).encode()
        try:audit(damaged,reports)
        except ValueError as e:out.append(dict(case=name,rejected=True,reason=str(e),damaged_sha256=hashlib.sha256(damaged).hexdigest()))
        else:raise AssertionError('damaged disk source accepted: '+name)
    return out


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('source',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    raw=a.source.read_bytes();result=audit(raw,a.reports.resolve());result['failure_controls']=controls(raw,a.reports.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
