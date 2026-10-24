#!/usr/bin/env python3
"""Preserve a completed bounded native128-group diagnostic without extending it.

Require terminal status, absent original driver/test handles, exact frozen
inputs and unchanged media. Validate exclusive gate totals, all128 request
groups,129 fanout bins and raw owner completion counters against the actual
compiled observer. A completed diagnostic is NOT Cedar, persistence, pinned
workload equivalence, hardware speed or two-hour hardware qualification.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess

from test_r_front_forward_view import sha


def collect(run,out):
    assert not out.exists()
    state=json.loads((run/'run-state.json').read_text())
    assert state['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC' and state['reason']=='HOST_LIMIT'
    assert state['firmware_qualified'] is False and state['desktop_qualified'] is False
    assert all(a['returncode']==0 for a in state['actions'])
    test=state['actions'][-1];assert test['stage']=='test'
    pids=[state['driver_pid'],test['pid']]
    ps=subprocess.run(['ps','-p',','.join(map(str,pids)),'-o','pid=,etime=,stat=,command='],capture_output=True,text=True)
    assert ps.returncode==1 and not ps.stdout.strip(), 'Observe the same processes until they exit'
    p=json.loads(gzip.decompress((run/'provenance.json.gz').read_bytes()))
    for name,digest in p['inputs'].items():
        assert sha(run/'inputs'/name)==digest,name
    medium=Path(p['artifacts']['medium']['path']);assert sha(medium)==p['artifacts']['medium']['sha256']
    bound=int((run/'host-cycle-limit.txt').read_text());assert bound==5000000000
    fixture=(run/'inputs/harness/tb_native_cedar.sv').read_text()
    for text in ['else if(!storage_ready)traffic_classes[5]++;','else traffic_classes[6]++;',
                 'if(step && instruction_signals[4] && !traffic_issue_previous)traffic_issues++;',
                 'if(!traffic_active)$fatal(1,"NATIVE_TRAFFIC orphan response");',
                 'if(traffic_active)$fatal(1,"NATIVE_TRAFFIC uncompleted raw owner");',
                 'if(!reset && (pack_fault || key_overflow || tx_bad || tx_dropped || rx_bad))$fatal(1,"native device fault");']:
        assert text in fixture,text
    raw=(run/'test.log').read_bytes();assert raw.endswith(b'\n')
    text=raw.decode();assert 'FATAL:' not in text and '%Fatal' not in text
    stop=re.findall(r'^NATIVE_DIAGNOSTIC_STOP reason=\s*HOST_LIMIT host=(\d+) guest=(\d+) cpu=([0-9a-f]+)$',text,re.M)
    assert len(stop)==1 and int(stop[0][0])==bound
    rows=re.findall(r'^NATIVE_TRAFFIC_TOTAL (.+)$',text,re.M);assert len(rows)==1
    totals={k:int(v) for k,v in re.findall(r'(\w+)=(\d+)',rows[0])}
    gates={int(k):int(v) for k,v in re.findall(r'^NATIVE_TRAFFIC_GATE class=(\d+) ticks=(\d+)$',text,re.M)}
    raw_blocks={int(k):int(v) for k,v in re.findall(r'^NATIVE_TRAFFIC_RAW_BLOCK class=(\d+) ticks=(\d+)$',text,re.M)}
    fanout={int(k):int(v) for k,v in re.findall(r'^NATIVE_TRAFFIC_FANOUT pending=(\d+) ticks=(\d+)$',text,re.M)}
    groups={int(k):dict(requests=int(n),offer_wait=int(w)) for k,n,w in re.findall(
        r'^NATIVE_TRAFFIC_GROUP group=(\d+) requests=(\d+) offer_wait=(\d+)$',text,re.M)}
    assert set(gates)==set(range(8)) and sum(gates.values())==totals['ticks']==bound+1
    assert set(raw_blocks)==set(range(6)) and set(fanout)==set(range(129))
    assert sum(raw_blocks.values())==sum(fanout.values())==totals['ticks']-gates[0]-gates[1]
    assert set(groups)==set(range(128)) and all(v['requests']>0 for v in groups.values())
    assert sum(v['requests'] for v in groups.values())==totals['accepted']==totals['completed']==totals['first_count']
    assert totals['reads']+totals['writes']==totals['accepted']
    # Posedge classes include the last step sampled by the stop's negedge.
    # The negedge prints guest_ticks before its nonblocking increment. Thus
    # the difference is that final Boolean step, not always one enabled edge.
    for source in ['reg step=0;always @(negedge host_clk)step<=source_enable;',
                   'host_ticks<=host_ticks+1;', 'guest_ticks<=guest_ticks+1;']:
        assert source in fixture,source
    final_enabled_step=gates[2]-int(stop[0][1]);assert final_enabled_step in [0,1]
    progress=re.findall(r'^NATIVE_PROGRESS .+$',text,re.M);assert progress
    last=dict(re.findall(r'(\w+)=([^ ]+)',progress[-1]));assert int(last['host'])==bound
    assert last['fault']=='00000000:00000000' and last['crc']==last['rx']=='0'
    out.mkdir(parents=True)
    for name in p['inputs']:
        source=run/'inputs'/name;dest=out/'inputs'/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source,dest)
    for name in ['run-state.json','provenance.json.gz','host-cycle-limit.txt','window-control.txt',
                 'test.log','compile.log','host-ethernet.log','host-io.log']:
        shutil.copy2(run/name,out/name)
    (out/'process-readback.txt').write_text(f'ps returncode={ps.returncode}\nabsent driver/test={pids}\n'+ps.stdout+ps.stderr)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='PRESERVED_TERMINAL_NATIVE_128_GROUP_DIAGNOSTIC',source_run=str(run),
        original_bound=bound,original_handles_absent=pids,all_frozen_input_hashes_verified=len(p['inputs']),
        unchanged_medium_sha256=sha(medium),terminal_reason='HOST_LIMIT',enabled_machine_edges=int(stop[0][1]),
        final_CPU_debug=stop[0][2],totals=totals,
        gate_ticks=dict(reset=gates[0],physical_initialization=gates[1],enabled_machine_edge=gates[2],
            simultaneous_disk_storage=gates[3],disk_only=gates[4],storage_only=gates[5],other=gates[6],reserved_unused=gates[7]),
        raw_block_ticks=dict(burst_fill=raw_blocks[0],refresh=raw_blocks[1],lookup=raw_blocks[2],
            response_backpressure=raw_blocks[3],other=raw_blocks[4],not_blocked=raw_blocks[5]),
        fanout=fanout,groups=groups,mean_raw_first_word_ticks=totals['first_latency_sum']/totals['first_count'],
        counter_boundary='Posedge accounting includes the final Boolean step; the negedge stop prints guest_ticks before its nonblocking increment',
        final_enabled_step_at_stop=final_enabled_step,
        receive_errors=int(last['rx']),receive_packet_total_not_recorded=True,TX_packets=int(last['tx']),TX_CRC_errors=int(last['crc']),
        actual_device_and_independent_SDRAM_fault_assertions_retained=True,
        paired_workloads_previously_diverge_at_raw_request=1536,pinned_workload_comparison_qualified=False,
        firmware_qualified=False,desktop_qualified=False,persistence_qualified=False,hardware_speed_qualified=False,
        two_hour_hardware_qualification=False,hardware_changed=False,original_bound_extended=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ['status','enabled_machine_edges','totals','gate_ticks']}),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['run','output']:
        p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();collect(a.run.resolve(),a.output.resolve())
