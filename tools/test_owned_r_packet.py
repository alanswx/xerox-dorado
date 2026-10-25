#!/usr/bin/env python3
"""Replay all original processor transactions through actual native ownership.

Only828 closed bits and the401-pin/one-root snapshots move into the caller.
The original16 visible-R aliases retain an external owner. Preserve every
original assertion, all2470 result bits,844 owners,9216 RAM bits and100 clocks.
All403 geometry tests remain: before-root corruption is explicitly injected
into the owned snapshot for that one negative test, outside legal publication.
That injection does not support the legal-history cache equivalence theorem.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_owned_r_packet import caller,cache
from generate_parallel_r_fault import packet
from generate_cached_r_packet import stage,cache as generic_cache
from test_r_front_forward_view import sha
from prove_cp_prefix_cache import module


def fall_guard_source(raw,review):
    """Apply only the exact fully reviewed control guard definition."""
    r=json.loads((review/'results.json').read_text())
    assert r['status']=='REVIEWED_SOURCE_BOUND_FALL_DATA_GUARD' and r['checked_wave_depths']==[5,24]
    assert r['no_result_or_state_bit_masked'] and r['actual_HIGH_identity_certificate_required']
    proof=review/'fall-data-guard-proof-v1-20261024';p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_FALL_DATA_GUARD'
    for n,h in p['files'].items():assert sha(proof/n)==h
    for row in p['cases']:
        d=proof/(row['mode']+'-w'+str(row['waves']));assert row['passed'] and row['returncode']==0
        for n,h in row['files'].items():assert sha(d/n)==h
        assert sha(d/'proof.log')==row['log_sha256']
    for name,status in [('fall-data-guard-four-state-v1-20261024','PASS_FALL_DATA_FOUR_STATE'),('fall-data-guard-bad-unknown-v1-20261024','PASS_FALL_DATA_UNKNOWN_COUNTEREXAMPLE')]:
        d=review/name;s=json.loads((d/'run-state.json').read_text());assert s['status']==status
        for n,h in s['files'].items():assert sha(d/n)==h
        assert sha(d/'test.log')==s['test_log_sha256'] and s['proof_state_sha256']==sha(proof/'run-state.json')
    for name in ['dorado_control_hold_atomic_step','dorado_control_terminal_body']:
        assert module(raw,name)==module((proof/'atomic.sv').read_text(),name)
    old=module(raw,'dorado_control_certified_fall_step');assert old==(proof/'original.sv').read_text()
    new=(proof/'actual.sv').read_text().replace('module dorado_control_data_guard_step #','module dorado_control_certified_fall_step #',1)
    assert raw.count(old)==1;actual=raw.replace(old,new,1);assert actual.replace(new,old,1)==raw
    return actual


def bench(original):
    actual=original;changes=[]
    def edit(old,new):
        nonlocal actual
        assert actual.count(old)==1,old
        actual=actual.replace(old,new,1);changes.append((old,new))
    edit(' reg [843:0] held_q=0;reg cb=1,ca=1,refresh=0;',
         ' wire [843:0] held_q;reg [843:0] initial_owner;reg [15:0] visible_r=0;reg cb=1,ca=1,refresh=0;')
    edit(' wire [2469:0] first_result,last_result;wire ready;wire [1:0] forwarded;',
         ' wire [2469:0] first_result,last_result;wire ready;wire [1:0] forwarded;\n'
         ' wire [843:0] last_held_next=last_result[2469:1626];')
    edit(' dorado_r_front_packet_step dut(host_clk,advance,use_pair,held_q,cb,ca,pb,pa,refresh,first_result,last_result,ready,forwarded,reset);',
         ' dorado_r_front_owned_packet dut(host_clk,advance,use_pair,ca,refresh,reset,pa,visible_r[7:0],visible_r[15:8],first_result,last_result,ready,forwarded,held_q);')
    edit(' always @(posedge host_clk)if(advance && ready)held_q<=last_result[2469:1626];',
         ' always @(posedge host_clk)if(advance && ready)visible_r<=last_held_next[587:572];')
    edit('  {held_q,old_memory}=initial_image[0];',
         '''  {initial_owner,old_memory}=initial_image[0];
  dut.r_front_lower_q=initial_owner[571:0];dut.r_front_upper_q=initial_owner[843:588];visible_r=initial_owner[587:572];
  {old_q,rb,ra,before_pins,after_pins,expected_first,first_memory}=records[0];
  dut.r_front_before=before_pins;dut.r_front_clock_before=rb;''')
    marker='   if(held_q!==old_q)$fatal(1,"PACKET old closed owner mismatch record=%0d",idx);'
    edit(marker,marker+'''
   if(dut.r_front_before!==before_pins || dut.r_front_clock_before!==rb)$fatal(1,"OWNED_PACKET unpublished input context record=%0d",idx);''')
    edit('     pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;',
         '     pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;dut.r_front_clock_before=geometry_cb;')
    edit('     if(k<401)pa[k]=!pa[k];else if(k==401)cb=0;else ca=0;',
         '''     // k401 deliberately violates the publication contract to test
     // the unchanged geometry guard; never claim cache equivalence here.
     if(k<401)pa[k]=!pa[k];else if(k==401)begin cb=0;dut.r_front_clock_before=0;end else ca=0;''')
    edit('    pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;#2;',
         '    pa=geometry_pa;cb=geometry_cb;ca=geometry_ca;dut.r_front_clock_before=geometry_cb;#2;')
    # Wait a delta-independent interval before every storage edge. The old
    # fixture drove advance and the edge together; new input guards may have
    # a different number of continuous-assignment deltas.
    actual=actual.replace('host_clk=1;','#1;host_clk=1;')
    for name in ['files','entry_ready','entry_fast','entry_cache','first_r_write_n']:
        actual=actual.replace('dut.'+name,'dut.packet.'+name)
    restored=actual
    for name in ['files','entry_ready','entry_fast','entry_cache','first_r_write_n']:
        restored=restored.replace('dut.packet.'+name,'dut.'+name)
    restored=restored.replace('#1;host_clk=1;','host_clk=1;')
    for old,new in reversed(changes):restored=restored.replace(new,old,1)
    assert restored==original
    return actual


def trace_links(bench_text):
    name=re.search(r'\$readmemh\("([^\"]*transactions.hex)"',bench_text)[1]
    count=0;previous=None
    for text in Path(name).read_text().splitlines():
        value=int(text,16);value >>= 9216
        result=value & ((1<<2470)-1);value >>= 2470
        after=value & ((1<<401)-1);value >>= 401
        before=value & ((1<<401)-1);value >>= 401
        ra=value & 1;value >>= 1;rb=value & 1;old=value >> 1
        if previous is not None:assert (old,before,rb)==previous,(count,'trace publication discontinuity')
        previous=(result>>1626,after,ra);count+=1
    assert count==14884
    return dict(records=count,consecutive_owner_pin_root_links=count-1,discontinuities=0,trace=str(Path(name)),trace_sha256=sha(Path(name)))


def run(proof,four_state,bad_unknown,out,engine,specialized,mutation,fall_guard=None):
    assert not out.exists();p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_OWNED_PACKET_CANONICAL_CONTEXT'
    for n,h in p['files'].items():assert sha(proof/n)==h
    for directory,status in [(four_state,'PASS_OWNED_CONTEXT_FOUR_STATE'),(bad_unknown,'PASS_OWNED_CONTEXT_UNKNOWN_COUNTEREXAMPLE')]:
        s=json.loads((directory/'run-state.json').read_text());assert s['status']==status and s['proof_state_sha256']==sha(proof/'run-state.json')
        for n,h in s['files'].items():assert sha(directory/n)==h
        assert sha(directory/'test.log')==s['test_log_sha256']
        assert s['actions'][0]['returncode']==0
    source=Path(p['source']);s=json.loads((source/'run-state.json').read_text())
    assert sha(source/'run-state.json')==p['source_state_sha256'] and s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS'
    for n,h in s['files'].items():assert sha(source/n)==h
    raw=(source/'candidate.sv').read_text();bp=(proof/'native-backplane.v').read_text()
    parent_packet=(source/'packet.sv').read_text();assert parent_packet==packet(raw)+'\n'+stage(raw,True)+'\n'+generic_cache(raw)
    if fall_guard is not None:raw=fall_guard_source(raw,fall_guard)
    actual=packet(raw,mutation)+'\n'+stage(raw,True,mutation)+'\n'+cache(raw,specialized)
    out.mkdir(parents=True);paths=[];rows=[]
    for row in s['source_files']:
        src=source/row['path'];assert sha(src)==row['output_sha256']
        dest=out/row['path'];dest.parent.mkdir(parents=True,exist_ok=True)
        if row['path']=='candidate.sv':dest.write_text(raw)
        else:shutil.copy2(src,dest)
        paths.append(dest);rows.append(dict(row,parent_sha256=sha(src),output_sha256=sha(dest)))
    tb=(source/'tb.sv').read_text();links=trace_links(tb)
    for n,v in {'packet.sv':actual,'caller.sv':caller(raw,bp),'tb.sv':bench(tb)}.items():(out/n).write_text(v);paths.append(out/n)
    shutil.copy2(source/'forward.sv',out/'forward.sv');paths.append(out/'forward.sv')
    for n in [Path(__file__).name,'generate_owned_r_packet.py','prove_owned_r_packet.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,specialized=specialized,mutation=mutation,
        proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),four_state_state_sha256=sha(four_state/'run-state.json'),bad_unknown_state_sha256=sha(bad_unknown/'run-state.json'),
        fall_data_guard_review=str(fall_guard) if fall_guard is not None else None,
        fall_data_guard_review_sha256=sha(fall_guard/'results.json') if fall_guard is not None else None,
        source=str(source),source_state_sha256=sha(source/'run-state.json'),trace_links=links,source_files=rows,
        files={str(v.relative_to(out)):sha(v) for v in out.rglob('*') if v.is_file()},actions=[],stored_owner_bits=828,external_original_R_alias_bits=16,
        single_physical_SRAM_bits=9216,all_original_assertions_retained=True,geometry_tests=403,
        geometry_before_root_test_is_explicit_contract_violation=True,hardware_changed=False,native_integrated=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
        [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save()
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    if mutation:passed=state['actions'][0]['returncode']==0 and code!=0 and re.search(r'PACKET (?:first|second|hold|committed|invalid geometry committed)',log) is not None
    else:
        m=re.search(r'FULL_PROCESSOR_PACKET_PASS (.+)',log)
        passed=code==0 and m is not None and m[1]==s['summary']
        if passed:
            clocks=re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
            assert len(clocks)==100 and clocks==re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
            assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in log
            coverage=re.search(r'^CACHED_PACKET_COVERAGE (.+)$',log,re.M)[1];assert coverage==s['cache_coverage']
            state.update(summary=m[1],cache_coverage=coverage,all100_clock_counts_match=True)
    state.update(status=('PASS_OWNED_PACKET_COUNTEREXAMPLE' if mutation else 'PASS_OWNED_FULL_PROCESSOR_PACKETS') if passed else 'TERMINAL_OWNED_PACKET_NOT_ESTABLISHED',finished=time.time(),
        test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','four-state','bad-unknown','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True);p.add_argument('--specialized',action='store_true')
    p.add_argument('--mutation',choices=['early-write','drop-forward','stale-intermediate','invert-low','stale-low'])
    p.add_argument('--fall-data-guard',type=Path,help='Apply only the exact reviewed data-only fall guard; no other source changes')
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.four_state.resolve(),a.bad_unknown.resolve(),a.output.resolve(),a.engine,a.specialized,a.mutation,
        a.fall_data_guard.resolve() if a.fall_data_guard else None))
