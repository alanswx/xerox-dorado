#!/usr/bin/env python3
"""Bound the real FPGA Cedar base loader from read-only protocol-log observations.

Checks the pinned34-transfer sequence, last-file data coverage and the actual
guest ACK header/peer, not just a server-side completion message. These are
log-visibility bounds, not wire timestamps, payload hashes or desktop proof.
"""
import argparse
import gzip
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parent.parent
REFERENCE=ROOT/'fpga/mister/reports/2026-10-19-native49b-repeat/loader-timing.json'
TRANSFER=re.compile(r'^STP_TRANSFER (\S+) \((\d+) bytes\)$')
QUEUE=re.compile(r'^FTP_QUEUE type=0o([0-7]+) id=([0-9a-f]{8}) bytes=(\d+) d=([0-7]+)/([0-7]+)/([0-7]+) ')


def lines(rows,source='wire'):
    pending='';earliest=None;result=[]
    for row in rows:
        if row.get('event')!='chunk' or row.get('source')!=source:continue
        if not pending:earliest=row['previous_read_end_ns']
        pending+=row['text']
        while '\n'in pending:
            text,pending=pending.split('\n',1)
            result.append(dict(text=text,earliest_ns=earliest,latest_ns=row['read_end_ns']))
            earliest=row['previous_read_end_ns']
    return result


def packet(event):
    match=re.fullmatch(r'TX n=(\d+): (.*)',event['text'])
    if not match or int(match[1])!=16:return None
    words=match[2].split()
    if len(words)!=16 or not all(re.fullmatch('[0-7]+',w)for w in words):return None
    words=[int(w,8)for w in words]
    if words[3]!=0o22:return None
    return (words[4]<<16)|words[5],tuple(words[9:12])


def analyze(events,reference):
    expected=[(path,r['bytes'])for path,r in reference['transfers'].items()]
    assert len(expected)==34 and expected[0][0].endswith('/Basic.Loadees') and expected[-1][0].endswith('/End.bcd')
    starts=[i for i,e in enumerate(events)if TRANSFER.fullmatch(e['text']) and TRANSFER.fullmatch(e['text'])[1]==expected[0][0]]
    assert len(starts)==1,'missing or repeated Basic.Loadees start'
    begin=starts[0];actual=[];transfers={};last=None
    for i in range(begin,len(events)):
        event=events[i];match=TRANSFER.fullmatch(event['text'])
        if not match:continue
        path,size=match[1],int(match[2]);actual.append((path,size))
        transfers[path]=dict(event,bytes=size)
        if path==expected[-1][0]:last=i;break
    assert actual==expected,'incomplete/different pinned base-loader transfer sequence'
    assert last is not None
    peer=None;base=None;probe=None;ranges={};ack=None
    size=expected[-1][1]
    for event in events[last+1:]:
        match=QUEUE.match(event['text'])
        if match:
            type_,id_,count=int(match[1],8),int(match[2],16),int(match[3])
            destination=tuple(int(match[i],8)for i in (4,5,6))
            if peer is None:peer=destination
            if destination!=peer:continue
            if type_==0o20 and probe is None:
                if base is None:base=id_
                offset=(id_-base)&0xffffffff
                if offset<size:
                    assert count>0 and offset+count<=size,'last-file data overruns pinned payload'
                    assert offset not in ranges or ranges[offset]==count,'inconsistent retransmission'
                    ranges[offset]=count
            if type_==0o21 and count==0 and probe is None:
                probe=id_
        if probe is not None and packet(event)==(probe,peer):
            ack=event;break
    position=0
    for offset,count in sorted(ranges.items()):
        assert offset==position,'missing/overlapping final-file data range'
        position+=count
    assert position==size,'incomplete final-file data'
    assert probe is not None and ack is not None,'missing actual final guest ACK for exact peer/probe'
    start=events[begin]
    lower=(ack['earliest_ns']-start['latest_ns'])/1e9
    upper=(ack['latest_ns']-start['earliest_ns'])/1e9
    assert 0<lower<=upper,'invalid observation time bounds'
    return dict(status='PASS: observed base-loader transfer chain and final guest ACK',
                files=len(actual),bcd_files=sum(p.endswith('.bcd')for p,_ in actual),
                payload_bytes=sum(n for _,n in actual),start=start,
                end={k:ack[k]for k in ('earliest_ns','latest_ns')},
                elapsed_lower_s=lower,elapsed_upper_s=upper,final_peer=list(peer),
                final_probe=f'{probe:08x}',final_data_ranges=ranges,transfers=transfers,
                limits='Regular-file protocol-log visibility bounds only; no direct wire timestamps, physical payload byte/hash proof, module START or desktop qualification. Pin/repeat workloads before speed claims.')


def controls(events,reference):
    baseline=analyze(events,reference)
    peer=tuple(baseline['final_peer']);probe=int(baseline['final_probe'],16)
    last=next(i for i,e in enumerate(events)if e['text']==baseline['transfers'][next(reversed(baseline['transfers']))]['text'])
    queues=[(i,QUEUE.match(e['text']))for i,e in enumerate(events[last+1:],last+1)]
    data=[(i,m)for i,m in queues if m and int(m[1],8)==0o20 and int(m[3])==1478]
    assert data
    remove_id=data[1][1][2]
    cases=[('missing_ack',[e for e in events if packet(e)!=(probe,peer)],'missing actual final guest ACK'),
           ('data_gap',[e for i,e in enumerate(events)if not(i>last and (m:=QUEUE.match(e['text']))and m[2]==remove_id)],'data'),
           ('wrong_peer',[dict(e,text=e['text'].replace(f'{peer[2]:06o}',f'{peer[2]+1:06o}'))if packet(e)==(probe,peer)else e for e in events],'missing actual final guest ACK')]
    result=[]
    for name,changed,diagnostic in cases:
        try:analyze(changed,reference)
        except AssertionError as error:
            assert diagnostic in str(error),(name,str(error))
            result.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged loader evidence accepted: '+name)
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('observer',type=Path)
    parser.add_argument('--reference',type=Path,default=REFERENCE)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args()
    data=args.observer.read_bytes()
    if args.observer.suffix=='.gz':data=gzip.decompress(data)
    rows=[json.loads(l)for l in data.decode().splitlines()]
    reference=json.loads(args.reference.read_text())
    events=lines(rows);result=analyze(events,reference)
    if args.negative_controls:result['failure_controls']=controls(events,reference)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['elapsed_lower_s'],result['elapsed_upper_s'],flush=True)
