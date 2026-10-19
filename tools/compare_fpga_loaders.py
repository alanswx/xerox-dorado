#!/usr/bin/env python3
"""Compare observed Cedar loader work, clock progress and protocol replay.

Inputs must already be qualified as instruction-view-1 captures from the
50 MHz native core. Log visibility brackets counters and transfer endpoints;
this does not establish physical payload correctness or an RTL cause.
"""
import argparse
import gzip
import json
from collections import Counter
from pathlib import Path
import re

from analyze_fpga_loader import REFERENCE, QUEUE, analyze, lines

COUNTERS=re.compile(r'^Dorado net: .* machine=([0-9a-f]+) state=([0-9a-f]+) cpu=([0-9a-f]+)$')


def measure(rows,reference):
    wire=lines(rows)
    loader=analyze(wire,reference)
    samples=[]
    for event in lines(rows,'main'):
        match=COUNTERS.fullmatch(event['text'])
        if match:
            samples.append(dict(event,issued=int(match[1],16),host=int(match[3],16)))
    assert len(samples)>2,'missing counter samples'
    for index,sample in enumerate(samples):
        if not index:
            sample.update(total_issues=0,total_host=0)
            continue
        previous=samples[index-1]
        lower=(sample['earliest_ns']-previous['latest_ns'])/1e9
        upper=(sample['latest_ns']-previous['earliest_ns'])/1e9
        assert 0<lower<=upper<32,'counter gap cannot qualify 32-bit unwrap'
        host=(sample['host']-previous['host'])&0xffffffff
        issues=(sample['issued']-previous['issued'])&0xffffffff
        # Allow log buffering/visibility jitter; reject reset, wrong clock or
        # ambiguous wrap rather than silently treating it as useful work.
        assert 45_000_000*lower<=host<=55_000_000*upper,'host clock/reset discontinuity'
        assert issues<=host/8+32,'instruction counter reset/view discontinuity'
        sample.update(total_issues=previous['total_issues']+issues,
                      total_host=previous['total_host']+host)

    def bracket(endpoint):
        before=[s for s in samples if s['latest_ns']<=endpoint['earliest_ns']]
        after=[s for s in samples if s['earliest_ns']>=endpoint['latest_ns']]
        assert before and after,'counter samples do not bracket loader endpoint'
        return before[-1],after[0]

    start_before,start_after=bracket(loader['start'])
    end_before,end_after=bracket(loader['end'])
    inner=[s for s in samples if s['earliest_ns']>=loader['start']['latest_ns']
           and s['latest_ns']<=loader['end']['earliest_ns']]
    assert len(inner)>2,'missing interior counter interval'
    first,last=inner[0],inner[-1]
    host=last['total_host']-first['total_host']
    issues=last['total_issues']-first['total_issues']
    elapsed_lower=(last['earliest_ns']-first['latest_ns'])/1e9
    elapsed_upper=(last['latest_ns']-first['earliest_ns'])/1e9
    active=[e for e in wire if loader['start']['earliest_ns']<=e['earliest_ns']<=loader['end']['latest_ns']]
    prefixes=Counter(e['text'].split(' ',1)[0]for e in active)
    queues=[QUEUE.match(e['text'])for e in active]
    duplicate=Counter((m[1],m[2],m[4],m[5],m[6])for m in queues if m)
    boundaries=list(loader['transfers'].items())+[('FINAL_GUEST_ACK',loader['end'])]
    phases=[]
    for (path,a),(_,b)in zip(boundaries,boundaries[1:]):
        events=[e for e in active if a['earliest_ns']<=e['earliest_ns']<b['earliest_ns']]
        phases.append(dict(path=path,
            elapsed_lower_s=(b['earliest_ns']-a['latest_ns'])/1e9,
            elapsed_upper_s=(b['latest_ns']-a['earliest_ns'])/1e9,
            replay_events=sum(e['text'].startswith('FTP_REPLAY ')for e in events)))
    return dict(loader=loader,
        counter_scope='Previously qualified view1 StartCycle issues, not drained retirement. Adjacent 32-bit host deltas unwrap at <32s gaps; reset/rate guards apply.',
        loader_issue_bounds=[end_before['total_issues']-start_after['total_issues'],
                             end_after['total_issues']-start_before['total_issues']],
        interior=dict(samples=len(inner),issued=issues,host_edges=host,
                      elapsed_visibility_bounds_s=[elapsed_lower,elapsed_upper],
                      issues_per_s_bounds=[issues/elapsed_upper,issues/elapsed_lower],
                      host_hz_bounds=[host/elapsed_upper,host/elapsed_lower],
                      issues_per_host_edge=issues/host),
        wire_log_prefix_counts=dict(prefixes),
        repeated_queue_headers=sum(n-1 for n in duplicate.values()),
        phases=phases,
        limits='One observed boot each; endpoint counters are bounded by adjacent samples. Replay events and phase durations correlate but do not establish the source of packet loss or an RTL regression. Manual pre-login delays vary; repeat with fixed input cadence.')


def controls(rows,reference):
    no_main=[r for r in rows if r.get('source')!='main']
    changed=[];mutated=False
    for row in rows:
        row=dict(row)
        if row.get('source')=='main' and not mutated and 'machine='in row.get('text',''):
            row['text']=re.sub(r'machine=([0-9a-f]+)',lambda m:f'machine={int(m[1],16)^0x80000000:08x}',row['text'],count=1)
            mutated=True
        changed.append(row)
    assert mutated
    results=[]
    for name,damaged in [('missing_main_counters',no_main),('reset_instruction_counter',changed)]:
        try:measure(damaged,reference)
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged counter evidence accepted: '+name)
    return results


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('observers',nargs='+',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--reference',type=Path,default=REFERENCE)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args()
    reference=json.loads(args.reference.read_text());result={}
    for path in args.observers:
        data=path.read_bytes()
        if path.suffix=='.gz':data=gzip.decompress(data)
        rows=[json.loads(line)for line in data.decode().splitlines()]
        record=measure(rows,reference)
        if args.negative_controls:record['failure_controls']=controls(rows,reference)
        result[str(path)]=record
        print(path,record['loader_issue_bounds'],record['interior']['issues_per_s_bounds'],
              'replays',record['wire_log_prefix_counts'].get('FTP_REPLAY',0),flush=True)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
