#!/usr/bin/env python3
"""Compare a stateless full processor evaluator on original capture traffic.

The parallel test instance consumes the same one physical RM/STK read port;
its address, select, writes, all844 closed/physical Qs and every output are
compared before each original owner commit. Remove ONLY the private clone's
cache and derive its prefix/certificate from the current call, using the proved
pure function. Keep the original cached evaluator, all assertions, retries,
reference models and SRAM owner intact. This does not yet compose two events
or qualify within-packet memory forwarding or physical timing.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha


def pure_top(raw,pure):
    old=module(raw,'dorado_r_front_control_step');top=old
    first=top.index(' dorado_cp_prefix_cache cp_guard(')
    last=top.index(' dorado_control_hold_ordered_step control(',first)
    block=top[first:last]
    start=pure.index(' wire split=');end=pure.index(' dorado_control_hold_ordered_step',start)
    guard=pure[start:end]
    # Alpha-renaming only: bind the proved pure guard to the real processor
    # projection. Its fresh_prefix wire is this call's cp_arrival output.
    mapping={'state_q':'joint_q','pins_before':'pins_before[397:0]','pins_after':'pins_after[397:0]',
             'refresh':'refresh_before_run_capture','fresh_prefix':'cp_prefix_live',
             'using_direct':'fresh_direct','prefix_cp':'cp_prefix_cp_saved','split':'fresh_split'}
    guard=re.sub(r'\b('+'|'.join(mapping)+r')\b',lambda m:mapping[m[0]],guard)
    guard=guard.replace('pins_before[397:0][292:282]','pins_before[292:282]').replace('pins_after[397:0][292:282]','pins_after[292:282]')
    guard=guard.replace(' wire [268:0] cp_prefix_live;',' reg fresh_direct;')
    guard=guard.replace(' wire [10:0] cp_prefix_cp_saved=', ' assign cp_prefix_cp_saved=')
    guard+=''' assign cp_ready=1'b1;assign cp_cache_hit=1'b0;
 assign cp_fast=fresh_direct;
 assign cp_prefix_saved=cp_prefix_live;
 assign cp_after_cp_saved=pins_after[292:282];
'''
    top=top[:first]+guard+top[last:]
    top=top.replace('module dorado_r_front_control_step #','module dorado_r_front_pure_step #',1)
    assert top.replace('module dorado_r_front_pure_step #','module dorado_r_front_control_step #',1).replace(guard,block,1)==old
    return top


def outputs(raw):
    header=raw[raw.index(' input wire host_clk'):raw.index('\n);')]
    result=[];direction=None;width=''
    for token in header.replace('\n',' ').split(','):
        match=re.fullmatch(r'\s*(?:(input|output)\s+wire\s*(\[\d+:\d+\])?\s*)?(\w+)\s*',token)
        assert match,token
        if match[1]:direction=match[1];width=match[2] or ''
        if direction=='output':result.append((match[3],width))
    assert len(result)==31,len(result)
    return result


def run(source,proof,out):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());assert s['status']=='PASS_FULL_PROCESSOR_SINGLE_OWNER_MEMORY_SEAM'
    r=json.loads((proof/'run-state.json').read_text());assert r['status']=='PASS_STATELESS_CONTROL_PREFIX_ALL_OUTPUTS'
    for name,h in r['files'].items():assert sha(proof/name)==h
    pure=(proof/'pure.sv').read_text();raw=(source/'candidate.sv').read_text();top=pure_top(raw,pure)
    ports=outputs(top)
    decl='\n'.join(f' wire {width} pure_{name};' for name,width in ports)
    bindings=dict(host_clk='host_clk',advance='advance',held_q='stored_q',clock_before='cb',clock_after='ca',pins_before='pb',pins_after='pa',
        refresh_before_run_capture="1'b0",cp_reset='cp_reset',external_file_read='external_file_read')
    bindings.update({name:'pure_'+name for name,_ in ports})
    addition=decl+'\n dorado_r_front_pure_step pure_processor(\n  '+',\n  '.join(f'.{name}({value})' for name,value in bindings.items())+');\n'
    # Resolve original short bench names from its unchanged positional mapping.
    marker=' dorado_r_front_control_step dut('
    invocation=(source/'inputs/tb_r_front_local.sv').read_text().split(marker,1)[1].split(');',1)[0]
    args=[v.strip() for v in invocation.replace('\n','').split(',')]
    header=module(raw,'dorado_r_front_control_step');header=header[header.index(' input wire host_clk'):header.index('\n);')]
    names=[re.search(r'(\w+)\s*$',v)[1] for v in header.replace('\n',' ').split(',')]
    original=dict(zip(names,args));assert len(names)==len(args)
    checks='\n'.join(f'     if(pure_{name}!=={original[name]})$fatal(1,"Pure full processor mismatch check=%0d output={name}",checks);'
                     for name,_ in ports if name!='cp_ready')
    sample='     #100;prepare_cp;#100;'
    out.mkdir(parents=True);rows=[];paths=[]
    for row in s['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];result=p.read_text()
        if p.name=='candidate.sv':result+='\n'+top+'\n'
        if p.name=='tb_r_front_local.sv':
            old=result;assert result.count(marker)==1 and result.count(sample)==1
            result=result.replace(marker,addition+marker,1).replace(sample,sample+'\n'+checks,1)
            assert result.replace(addition,'',1).replace(sample+'\n'+checks,sample,1)==old
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(result);paths.append(q)
        rows.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=p.read_text()!=result))
    assert sum(v['changed'] for v in rows)==2
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,engine='verilator',source=str(source),proof=str(proof),
        source_state_sha256=sha(source/'run-state.json'),proof_state_sha256=sha(proof/'run-state.json'),source_files=rows,actions=[],
        compared_output_ports=len(ports)-1,all844_closed_and_physical_Qs_checked=True,all_original_assertions_retained=True,
        one_physical_SRAM_owner=True,pure_instance_auxiliary_state_bits=0,
        two_event_composition_or_forwarding_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    defines=[v for v in s['actions'][0]['command'] if v.startswith('-D')]
    commands=[['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
        '--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]]
    save()
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=p.pid,started=time.time())
            state['actions'].append(row);save();code=p.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=code==0 and 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392' in log
    if passed:
        pattern=r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$'
        assert len(re.findall(pattern,log,re.M))==100 and re.findall(pattern,log,re.M)==re.findall(pattern,(source/'test.log').read_text(),re.M)
        assert re.search(r'^CP_PREFIX_CACHE (.+)$',log,re.M)[1]==s['cache_coverage']
    state.update(status='PASS_FULL_PROCESSOR_STATELESS_EVALUATOR' if passed else 'TERMINAL_PURE_PROCESSOR_NOT_ESTABLISHED',finished=time.time(),
        test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','proof','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.proof.resolve(),a.output.resolve()))
