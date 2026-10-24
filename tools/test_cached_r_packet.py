#!/usr/bin/env python3
"""Replay complete packets with one entry cache and a direct second root.

Keep every2470-bit subevent result, all100 clocks and the entire9216-bit RAM
oracle. Cache preparation/reset may change only auxiliary state, never owners
or SRAM. Pair mode requires entry identity; noncanonical entries use the exact
single-event staged fallback. The second event uses its proved fresh LOW-root
identity, without sharing the entry cache. Source/device timestamps and native
physical timing remain unqualified.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_cached_r_packet import packet,stage,cache
from test_r_front_forward_view import sha


def bench(raw):
    header=' reg host_clk=0,advance=0,use_pair=0;'
    assert raw.count(header)==1
    decl='''
 reg reset=0;integer prefix_wait_edges=0,fast_commits=0,staged_commits=0,cache_resets=0;
 reg [843:0] reset_owner;
'''
    raw=raw.replace(header,header+decl,1)
    old='first_result,last_result,ready,forwarded);'
    assert raw.count(old)==1;raw=raw.replace(old,old[:-2]+',reset);',1)
    helper=''' task automatic prepare_entry;
  integer waits;reg [9215:0] saved_memory;reg [843:0] saved_owner;
  begin
   waits=0;
   while(!dut.entry_ready && waits<4)begin
    saved_memory=memory_image();saved_owner=held_q;advance=0;
    host_clk=1;#2;host_clk=0;#2;
    if(memory_image()!==saved_memory || held_q!==saved_owner)$fatal(1,"CACHED_PACKET preparation committed architectural state record=%0d",idx);
    prefix_wait_edges++;waits++;
   end
   if(dut.entry_ready!==1'b1)$fatal(1,"CACHED_PACKET entry did not become ready record=%0d",idx);
  end
 endtask
'''
    assert raw.count(' endfunction')==1;raw=raw.replace(' endfunction',' endfunction\n'+helper,1)
    loop='  while(idx<COUNT)begin'
    reset='''
   if(idx>=1024 && cache_resets==0)begin
    old_memory=memory_image();reset_owner=held_q;reset=1;advance=1;#2;
    if(ready!==1'b0)$fatal(1,"CACHED_PACKET reset left ready asserted");
    host_clk=1;#2;host_clk=0;advance=0;#2;reset=0;#2;
    if(memory_image()!==old_memory || held_q!==reset_owner)$fatal(1,"CACHED_PACKET reset changed architectural state");
    if(dut.entry_cache.canonical_valid || dut.entry_cache.cache_valid)$fatal(1,"CACHED_PACKET stale context survived reset");
    cache_resets++;
   end
'''
    raw=raw.replace(loop,loop+reset,1)
    before='use_pair=eligible;take=1;#2;';assert raw.count(before)==1
    raw=raw.replace(before,before+'prepare_entry;#2;',1)
    fallback='port_fallbacks++;use_pair=0;ca=ra;#2;';assert raw.count(fallback)==1
    raw=raw.replace(fallback,fallback+'prepare_entry;#2;',1)
    mark='   expected_q=expected_last[2469:1626];';assert raw.count(mark)==1
    raw=raw.replace(mark,'   if(dut.entry_fast)fast_commits++;else staged_commits++;\n'+mark,1)
    mark='  if(pairs<1000 || paired_writes==0 || forwarded_bytes==0)'
    extra='''  if(prefix_wait_edges==0 || fast_commits==0 || staged_commits==0 || cache_resets!=1)$fatal(1,"CACHED_PACKET cache coverage incomplete");
  $display("CACHED_PACKET_COVERAGE fast=%0d staged=%0d prepare_edges=%0d resets=%0d",fast_commits,staged_commits,prefix_wait_edges,cache_resets);
'''
    assert raw.count(mark)==1;raw=raw.replace(mark,extra+mark,1)
    # The canonical tag is also a performance contract: it must describe the
    # just-committed owner/root, even when a wrong tag would only add waits.
    mark='   if(held_q!==expected_q)$fatal(1,"PACKET committed owner mismatch record=%0d",idx);'
    extra='''
   if(dut.entry_cache.canonical_root!==ca)$fatal(1,"CACHED_PACKET committed root tag mismatch record=%0d",idx);
   if(dut.entry_cache.canonical_pins!==pa[397:0])$fatal(1,"CACHED_PACKET committed pin tag mismatch record=%0d",idx);
'''
    assert raw.count(mark)==1;raw=raw.replace(mark,mark+extra,1)
    return raw


def run(source,proof,out,engine,mutation,pair_direct=False):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_FULL_PROCESSOR_PACKET_SINGLE_COMMIT' and s['quartus_syntax'] and s['geometry_checks']
    p=json.loads((proof/'run-state.json').read_text());induction=next(v for v in p['cases'] if v['mode']=='induction')
    assert induction['passed'] and induction['returncode']==0 and sha(proof/'induction/proof.log')==induction['log_sha256']
    for n,h in induction['files'].items():assert sha(proof/'induction'/n)==h
    out.mkdir(parents=True);paths=[];rows=[]
    for row in s['source_files']:
        q=source/row['path'];assert sha(q)==row['output_sha256']
        dest=out/row['path'];dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(q.read_bytes());paths.append(dest)
        rows.append(dict(path=row['path'],parent_sha256=sha(q),output_sha256=sha(dest)))
    raw=(source/'candidate.sv').read_text();actual=packet(raw,mutation,pair_direct)+'\n'+stage(raw)+'\n'+cache(raw,mutation=='stale-root')
    (out/'packet.sv').write_text(actual);paths.append(out/'packet.sv')
    for n in ['forward.sv']:
        assert sha(source/n)==s['files'][n];shutil.copy2(source/n,out/n);paths.append(out/n)
    (out/'tb.sv').write_text(bench((source/'tb.sv').read_text()));paths.append(out/'tb.sv')
    for name in [Path(__file__).name,'generate_cached_r_packet.py']:shutil.copy2(Path(__file__).with_name(name),out/name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,mutation=mutation,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),induction_proof=str(proof/'induction'),induction_log_sha256=induction['log_sha256'],
        source_files=rows,files={q.name:sha(q) for q in [out/'packet.sv',out/'forward.sv',out/'tb.sv',out/Path(__file__).name,out/'generate_cached_r_packet.py']},
        actions=[],pair_direct=pair_direct,entry_cache_only=True,native_canonical_match_specialization_used=False,second_root_requires_derived_identity=True,
        one_physical_SRAM_owner=True,all2470_bits_per_subevent_checked=True,source_and_device_timestamp_qualified=False,
        native_integrated=False,timing_qualified=False,hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else
        [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save()
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=child.pid,started=time.time())
            state['actions'].append(row);save();code=child.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    text=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    if mutation:passed=state['actions'][0]['returncode']==0 and code!=0 and re.search(r'(PACKET (?:first|second|hold|committed|invalid geometry committed)|CACHED_PACKET)',text) is not None
    else:
        m=re.search(r'FULL_PROCESSOR_PACKET_PASS (.+)',text);passed=code==0 and m is not None
        if passed:
            assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in text
            clocks=re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==100 and clocks==re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
            state.update(summary=m[1],cache_coverage=re.search(r'^CACHED_PACKET_COVERAGE (.+)$',text,re.M)[1],all100_clock_counts_match=True)
    state.update(status=('PASS_CACHED_PACKET_COUNTEREXAMPLE' if mutation else 'PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS') if passed else
        'TERMINAL_CACHED_PACKET_NOT_ESTABLISHED',finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['verilator','icarus'],default='verilator');p.add_argument('--mutation',choices=['early-write','drop-forward','stale-intermediate','stale-root'])
    p.add_argument('--pair-direct',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve(),a.engine,a.mutation,a.pair_direct))
