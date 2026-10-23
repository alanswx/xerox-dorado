#!/usr/bin/env python3
"""Read bounded original-native storage traces without altering a running job.

Compare raw operation/data prefixes independently of host latency. Initialization,
enabled fabric edges, StartCycle issues and Cedar113 release remain distinct.
A state-file RUNNING marker is not process-liveness evidence. An unpaired final
request at the8192-request trace cap is retained as incomplete, never completed.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import time


def digest(raw):return hashlib.sha256(raw).hexdigest()


def parse(raw):
    # A concurrent writer may leave one partial record; parse complete lines.
    partial=bool(raw and not raw.endswith(b'\n'))
    if partial:raw=raw[:raw.rfind(b'\n')+1]
    text=raw.decode(errors='replace')
    accepted=[];completed=[];active=None;previous_tick=-1
    latencies={'read':[],'write':[]};fanout={};pc_counts={};groups={}
    for line in text.splitlines():
        if not line.startswith(('NATIVE_TRAFFIC_A ','NATIVE_TRAFFIC_R ')):continue
        fields=dict(re.findall(r'(\w+)=([^ ]+)',line));kind=line.split()[0][-1]
        tick=int(fields['tick']);address=int(fields['address'],16);writing=int(fields['write'])
        assert tick>=previous_tick,'Nonmonotonic captured raw events';previous_tick=tick
        assert 0<=address<(1<<23) and writing in [0,1]
        if kind=='A':
            assert active is None,'Raw request overwrote captured outstanding owner'
            row=dict(tick=tick,address=address,write=writing,data=int(fields['data'],16),pc=int(fields['pc'],16),
                fanout=int(fields['fanout']),state=int(fields['state']))
            assert row['fanout']<=128
            accepted.append(row);active=row
            group=address&127;groups[group]=groups.get(group,0)+1
            fanout[row['fanout']]=fanout.get(row['fanout'],0)+1
            cia=(row['pc']>>12)&4095;pc_counts[cia]=pc_counts.get(cia,0)+1
        else:
            assert active is not None,'Orphan captured completion'
            assert (address,writing)==(active['address'],active['write']),'Captured owner/order changed'
            latency=int(fields['latency']);assert latency==tick-active['tick'] and latency>0
            row=dict(request=active,response_tick=tick,response_data=int(fields['data'],16),latency=latency)
            completed.append(row);latencies['write'if writing else'read'].append(latency);active=None
    progress=[]
    for line in text.splitlines():
        if line.startswith('NATIVE_TRAFFIC_PROGRESS '):
            f=dict(re.findall(r'(\w+)=([^ ]+)',line));progress.append({k:int(v,8 if k=='BFN' else 10)for k,v in f.items()})
    initialized=any(re.search(r'^NATIVE_PROGRESS .* initialized=1\b',l)for l in text.splitlines())
    return dict(log_prefix_sha256=digest(raw),trailing_partial_record=partial,
        accepted=accepted,completed=completed,unpaired_request=active,progress=progress,
        physical_initialization_observed_complete=initialized,
        actual_Cedar113_key_release_observed='NATIVE_C_RELEASED 'in text,
        terminal_marker_observed='NATIVE_DIAGNOSTIC_STOP 'in text,
        captured_group_counts=groups,captured_admission_fanout=fanout,captured_CIA_counts=pc_counts,
        latency={kind:dict(count=len(v),sum=sum(v),minimum=min(v)if v else None,
            maximum=max(v)if v else None,mean=sum(v)/len(v)if v else None)for kind,v in latencies.items()})


def load(run):
    source=(run/'provenance.json.gz').read_bytes();p=json.loads(gzip.decompress(source))
    for n,h in p['inputs'].items():assert digest((run/'inputs'/n).read_bytes())==h,n
    log=run/'test.log';size=log.stat().st_size
    with log.open('rb')as f:raw=f.read(size)
    assert len(raw)==size
    r=parse(raw);r.update(run=str(run),source_inventory_unchanged=True,
        provenance_sha256=digest(source),bounded_log_bytes=size,
        recorded_run_status=json.loads((run/'run-state.json').read_text())['status'],
        liveness_verified_by_this_tool=False)
    return r


def compare(a,b):
    count=min(len(a['completed']),len(b['completed']));first=None
    for i in range(count):
        left=a['completed'][i];right=b['completed'][i]
        fields=['address','write','data']
        same=all(left['request'][n]==right['request'][n]for n in fields)
        if not left['request']['write']:same &= left['response_data']==right['response_data']
        if not same:first=dict(index=i,control=left,candidate=right);break
    def ticks(r):
        if not count:return None
        return r['completed'][count-1]['response_tick']-r['completed'][0]['request']['tick']+1
    at=ticks(a);bt=ticks(b)
    def budget(r):
        rows=r['completed'][:count]
        if not rows:return None
        service=sum(row['latency']for row in rows)
        gaps=[rows[i]['request']['tick']-rows[i-1]['response_tick']for i in range(1,count)]
        assert all(g>=0 for g in gaps)
        elapsed=ticks(r);assert elapsed==service+sum(gaps)+1
        return dict(consumed_raw_service_ticks=service,unowned_raw_ticks=sum(gaps),
            same_edge_handoffs=sum(g==0 for g in gaps),positive_gaps=sum(g>0 for g in gaps),
            limitation='Unowned raw time also includes guest execution and demand gaps; it is not exclusively arbitration overhead.')
    return dict(compared_completed_prefix=count,ordered_operation_data_agree=first is None if count else None,
        first_disagreement=first,control_prefix_ticks=at,candidate_prefix_ticks=bt,
        control_prefix_budget=budget(a),candidate_prefix_budget=budget(b),
        prefix_tick_reduction_fraction=(at-bt)/at if at and first is None else None,
        actual_firmware_comparison_qualified=False,
        limitation='Only captured prefix. Native power-up reads are not Cedar execution; prefix caps and different progress endpoints remain explicit.')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--control',type=Path,required=True)
    p.add_argument('--candidate',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();assert not a.output.exists(),'Preserve previous observation'
    control=load(a.control.resolve());candidate=load(a.candidate.resolve())
    result=dict(scope=__doc__,observed=time.time(),control=control,candidate=candidate,
        comparison=compare(control,candidate),hardware_speed_or_goal_qualified=False)
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result['comparison'],indent=2))
    for label,r in [('control',control),('candidate',candidate)]:
        print(label,'init_complete',r['physical_initialization_observed_complete'],'last_progress',r['progress'][-1]if r['progress']else None)
