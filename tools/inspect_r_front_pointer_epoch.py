#!/usr/bin/env python3
"""Add actual package D-at-clock checks to a terminal pointer-epoch probe.

ProcL h14 holds RBase[0:3], a task bit and NextMacroDly; l14 holds StkP[0:5].
This audit leaves every original assertion intact and checks the twelve
capture inputs at the actual delayed original package clock edges. Also
count where the old-control and resolved-control views differ, so a passing
state comparison is not mistaken for coverage of an unexercised distinction.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from test_r_front_forward_view import sha
from test_r_front_task_event import replace


def bench(raw, low_cp=False, rm_address=False):
    added = '''
 reg [5:0] audit_h14=0,audit_l14=0;
 integer audit_h_edges=0,audit_l_edges=0,audit_h_different=0,audit_l_different=0;
 always @(posedge original.l.h14.p9) if(monitor) begin
   audit_h_edges=audit_h_edges+1;
   if({original.l.h14.p12,original.l.h14.p11,original.l.h14.p10,
       original.l.h14.p7,original.l.h14.p6,original.l.h14.p5}!==audit_h14)
     $fatal(1,"Original h14 capture D epoch mismatch check=%0d",checks);
 end
 always @(posedge original.l.l14.p9) if(monitor) begin
   audit_l_edges=audit_l_edges+1;
   if({original.l.l14.p12,original.l.l14.p11,original.l.l14.p10,
       original.l.l14.p7,original.l.l14.p6,original.l.l14.p5}!==audit_l14)
     $fatal(1,"Original l14 capture D epoch mismatch check=%0d",checks);
 end
'''
    raw = replace(raw, ' task automatic event_step(input next_root,input [400:0] pins);',
                  added + ' task automatic event_step(input next_root,input [400:0] pins);')
    raw = replace(raw, '     {root,rp}={next_root,pins};', '''
     audit_h14=dut.child.child.captured_front_data[17:12];
     audit_l14=dut.child.child.captured_front_data[23:18];
     if(events[47] && audit_h14!==dut.child.child.pre_data[17:12]) audit_h_different=audit_h_different+1;
     if(events[48] && audit_l14!==dut.child.child.pre_data[23:18]) audit_l_different=audit_l_different+1;
     {root,rp}={next_root,pins};''')
    raw = replace(raw, '   $display("R_FRONT_LOCAL_PROBE PASS ', '''
   if(audit_h_edges<100 || audit_l_edges<100)$fatal(1,"Original pointer package capture coverage incomplete");
   $display("POINTER_CAPTURE_EPOCH_AUDIT h14_edges=%0d l14_edges=%0d h14_distinct=%0d l14_distinct=%0d",
     audit_h_edges,audit_l_edges,audit_h_different,audit_l_different);
   $display("R_FRONT_LOCAL_PROBE PASS ''')
    if low_cp:
        raw = replace(raw, '   if(audit_h_edges<100 || audit_l_edges<100)', '''
   // Additional maintenance seam stress: CP writes while the root is LOW.
   // Do not force Qs or relax any original clock/state/RAM comparison.
   cp_write(0,40);
   for(integer bank=0;bank<4;bank=bank+1)
     for(integer data=0;data<128;data=data+1) begin
       if(cb)event_step(0,p);
       p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(0,p);
       p[292]=0;event_step(0,p);p[292]=1;event_step(0,p);
     end
   if(audit_h_edges<100 || audit_l_edges<100)''')
    if rm_address:
        raw=replace(raw,' reg [5:0] audit_h14=0,audit_l14=0;', '''
 reg [3:0] audit_rm_data=0;
 integer audit_rm_edges=0,audit_rm_distinct=0;
 wire [3:0] audit_resolved_rm=dut.child.child.child.child.low_address_data(
   dut.child.child.child.child.state_q,dut.child.child.child.child.pins_after);
 always @(posedge original.l.g10.p9) if(monitor) begin
   audit_rm_edges=audit_rm_edges+1;
   if({original.l.h10.p10,original.l.h10.p7,original.l.g10.p10,original.l.g10.p7}!==audit_rm_data)
     $fatal(1,"Original low RM address capture D epoch mismatch check=%0d",checks);
 end
 reg [5:0] audit_h14=0,audit_l14=0;''')
        raw=replace(raw,'     {root,rp}={next_root,pins};', '''
     audit_rm_data=dut.child.child.child.child.captured_low;
     if(events[34] && audit_rm_data!==audit_resolved_rm) audit_rm_distinct=audit_rm_distinct+1;
     {root,rp}={next_root,pins};''')
        raw=replace(raw,'   if(audit_h_edges<100 || audit_l_edges<100)', '''
   if(audit_rm_edges<100)$fatal(1,"Original low RM address capture coverage incomplete");
   $display("RM_ADDRESS_CAPTURE_AUDIT edges=%0d distinct=%0d",audit_rm_edges,audit_rm_distinct);
   if(audit_h_edges<100 || audit_l_edges<100)''')
    return raw


def run(source, out, low_cp=False, rm_address=False):
    r = json.loads((source / 'run-state.json').read_text())
    expected_status='PASS_R_FRONT_RM_ADDRESS_EPOCH_PROBE' if rm_address else 'PASS_R_FRONT_POINTER_CAPTURE_PROBE'
    assert r['status'] == expected_status and not r.get('mutation')
    assert r['actions'][-1]['returncode'] == 0
    build = r['actions'][0]['command']
    out.mkdir(parents=True, exist_ok=False)
    paths, records = [], []
    for row in r['source_files']:
        p = source / row['path']; assert sha(p) == row['output_sha256']
        q = out / row['path'];q.parent.mkdir(parents=True, exist_ok=True)
        raw = bench(p.read_text(),low_cp,rm_address) if p.name == 'tb_r_front_local.sv' else p.read_text()
        q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'], parent_sha256=sha(p), output_sha256=sha(q), changed=sha(p)!=sha(q)))
    assert sum(x['changed'] for x in records) == 1
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines = [x for x in build if x.startswith('-D')]
    engine = r['engine']
    commands = [['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
                ['vvp','-i',str(out/'test.vvp')]] if engine == 'icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state = dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,parent=str(source),
                 parent_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
                 all_original_checks_retained=True,extra_low_root_CP_transactions=low_cp,
                 low_RM_address_D_at_clock_checked=rm_address,native_timing_qualified=False)
    def save(): (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,cmd in enumerate(commands):
        with (out/('compile.log' if i==0 else 'test.log')).open('w') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    m=re.search(r'POINTER_CAPTURE_EPOCH_AUDIT h14_edges=(\d+) l14_edges=(\d+) h14_distinct=(\d+) l14_distinct=(\d+)',log)
    summary=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    rm=re.search(r'RM_ADDRESS_CAPTURE_AUDIT edges=(\d+) distinct=(\d+)',log)
    passed=not rc and m and summary
    if rm_address:passed=passed and rm
    if passed and not low_cp:
        assert summary[1]=='checks=11300 writes=238 reads=4368 epochs=392'
    state.update(status='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' if passed else 'POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED',
                 coverage=list(map(int,m.groups())) if m else None,summary=summary[1] if summary else None,
                 RM_capture_coverage=list(map(int,rm.groups())) if rm else None,finished=time.time());save()
    print(state['status'],state['coverage'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--low-cp',action='store_true')
    p.add_argument('--rm-address',action='store_true')
    a=p.parse_args();sys.exit(run(a.source.resolve(),a.output.resolve(),a.low_cp,a.rm_address))
