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


def all_address_bench(raw):
    # channels are the original reference's actual package clocks, already
    # independently compared by the unchanged 100-clock oracle.
    q='dut.child.child.child.child.state_q'
    p='dut.child.child.child.child.pins_after'
    b='dut.child.child.child.child.pins_before'
    a='dut.child.child.child.child.addressed'
    f='dut.child.child.child.child.low_address_data'
    nxt='dut.child.child.child.child.next_raw'
    rows=[('H_l05',33,6,'original.h.l05',f'{a}[601:596]',f'{{{q}[600],{q}[83],{p}[155:152]}}'),
          ('L_g10_h10',34,4,None,f'{a}[605:602]',f'{f}({q},{{{p}[201:160],{b}[159:156],{p}[155:0]}})'),
          ('L_i15',38,6,'original.l.i15',f'{a}[611:606]',f'{{{q}[610],!({q}[630]|{p}[176]),{p}[163:160]}}'),
          ('L_k11',39,6,'original.l.k11',f'{a}[617:612]',f"{{2'b0,{{2{{{p}[165]}}}},{{2{{{p}[164]}}}}}}"),
          ('L_g08',40,6,'original.l.g08',f'{a}[623:618]',f'{p}[171:166]'),
          ('L_j11',41,6,'original.l.j11',f'{a}[629:624]',f'{p}[171:166]'),
          ('L_l11_A',42,1,None,f'{a}[630]',f'(|{nxt}({q}[87:0]))'),
          ('L_l11_B',43,1,None,f'{a}[631]',f'{p}[179]')]
    declarations=[];sample=[];report=[]
    for i,(label,channel,width,pkg,expected,previous) in enumerate(rows):
        actual=('{'+','.join(pkg+'.p'+str(n) for n in [12,11,10,7,6,5])+'}') if pkg else {
            'L_g10_h10':'{original.l.h10.p10,original.l.h10.p7,original.l.g10.p10,original.l.g10.p7}',
            'L_l11_A':'original.l.l11.p7','L_l11_B':'original.l.l11.p10'}[label]
        declarations.append(f'''
 reg [{width-1}:0] audit_address_D{i}=0;
 integer audit_address_edges{i}=0,audit_address_distinct{i}=0;
 always @(posedge clocks[{channel}]) if(monitor) begin
   audit_address_edges{i}=audit_address_edges{i}+1;
   if({actual}!==audit_address_D{i})
     $fatal(1,"Original address D-at-clock mismatch package={label} check=%0d",checks);
 end
''')
        sample.append(f'''     audit_address_D{i}={expected};
     if(events[{channel}] && audit_address_D{i}!=={previous}) audit_address_distinct{i}=audit_address_distinct{i}+1;''')
        report.append(f'''   if(audit_address_edges{i}==0)$fatal(1,"Address pin audit unexercised package={label}");
   $display("ALL_ADDRESS_PIN_AUDIT group={i} package={label} edges=%0d distinct=%0d",audit_address_edges{i},audit_address_distinct{i});''')
    raw=replace(raw,' task automatic event_step(input next_root,input [400:0] pins);',
                ''.join(declarations)+' task automatic event_step(input next_root,input [400:0] pins);')
    raw=replace(raw,'     {root,rp}={next_root,pins};','\n'.join(sample)+'\n     {root,rp}={next_root,pins};')
    return replace(raw,'   $display("R_FRONT_LOCAL_PROBE PASS ', '\n'.join(report)+'\n   $display("R_FRONT_LOCAL_PROBE PASS ')


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


def run(source, out, low_cp=False, rm_address=False, all_address=False, coincident_cp=False):
    r = json.loads((source / 'run-state.json').read_text())
    expected_status=('PASS_R_FRONT_ALL_ADDRESS_CAPTURE' if all_address else
                     'PASS_R_FRONT_RM_ADDRESS_EPOCH_PROBE' if rm_address else 'PASS_R_FRONT_POINTER_CAPTURE_PROBE')
    assert r['status'] == expected_status and not r.get('mutation')
    assert r['actions'][-1]['returncode'] == 0
    build = r['actions'][0]['command']
    out.mkdir(parents=True, exist_ok=False)
    paths, records = [], []
    for row in r['source_files']:
        p = source / row['path']; assert sha(p) == row['output_sha256']
        q = out / row['path'];q.parent.mkdir(parents=True, exist_ok=True)
        raw = bench(p.read_text(),low_cp,rm_address) if p.name == 'tb_r_front_local.sv' else p.read_text()
        if all_address and p.name=='tb_r_front_local.sv':raw=all_address_bench(raw)
        if coincident_cp and p.name=='tb_r_front_local.sv':
            raw=replace(raw,'   if(audit_h_edges<100 || audit_l_edges<100)', '''
   // CP and CPU root clocks are independent on the original boards. Exercise
   // both CP pulse edges on the rising root, retaining the full raw oracle.
   for(integer align=0;align<2;align=align+1)
     for(integer bank=0;bank<4;bank=bank+1)
       for(integer data=0;data<128;data=data+1) begin
         if(cb)event_step(0,p);
         p[292]=1;p[291:289]=~3'(bank);p[288:282]=7'(data);event_step(0,p);
         p[292]=0;event_step(align==0,p);p[292]=1;event_step(align==1,p);
       end
   if(audit_h_edges<100 || audit_l_edges<100)''')
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
    state['all_R_address_D_at_clock_checked']=all_address
    state['extra_coincident_CP_transactions']=coincident_cp
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
    all_pins=re.findall(r'ALL_ADDRESS_PIN_AUDIT group=(\d+) package=(\S+) edges=(\d+) distinct=(\d+)',log)
    passed=not rc and m and summary
    if rm_address:passed=passed and rm
    if all_address:passed=passed and len(all_pins)==8 and all(int(x[2])>0 for x in all_pins)
    if passed and not low_cp and not coincident_cp:
        assert summary[1]=='checks=11300 writes=238 reads=4368 epochs=392'
    state.update(status='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' if passed else 'POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED',
                 coverage=list(map(int,m.groups())) if m else None,summary=summary[1] if summary else None,
                 RM_capture_coverage=list(map(int,rm.groups())) if rm else None,finished=time.time());save()
    if all_address:
        state['all_address_pin_coverage']=[dict(group=int(i),package=p,edges=int(e),distinct=int(d)) for i,p,e,d in all_pins];save()
    print(state['status'],state['coverage'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--low-cp',action='store_true')
    p.add_argument('--rm-address',action='store_true')
    p.add_argument('--all-address',action='store_true')
    p.add_argument('--coincident-cp',action='store_true')
    a=p.parse_args();sys.exit(run(a.source.resolve(),a.output.resolve(),a.low_cp,a.rm_address,a.all_address,a.coincident_cp))
