#!/usr/bin/env python3
"""Bound whole-loader instruction work and count actual protocol replays.

Requires already qualified instruction view1 and a terminal bounded observer.
Issues are not drained retirements. Never infer speed from selected phases.
"""
import argparse
from collections import Counter
import gzip
import hashlib
import json
from pathlib import Path
import re

import analyse_loader_trace as loader


def lines_from(records,source):
    pending='';lower=0
    for row in records:
        if row.get('event')!='chunk'or row.get('source')!=source:continue
        if not pending:lower=row['previous_read_end_ns']
        pending+=row['text']
        while '\n'in pending:
            text,pending=pending.split('\n',1)
            yield dict(text=text,earliest_ns=lower,latest_ns=row['read_end_ns'])
            lower=row['previous_read_end_ns']


def analyse(records,sizes,clock_hz):
    if records[0].get('event')!='ready'or records[-1].get('event')!='end':
        raise ValueError('terminal bounded observer required')
    chain=loader.analyse(loader.observed_lines(records),sizes)
    main=[];previous=None;issued=host=0
    for row in lines_from(records,'main'):
        text=row['text']
        m=re.search(r'machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})',text)
        if not m:continue
        errors=[re.findall(r'\b'+key+r'=(\d+)\b',text)for key in ('crc_bad','rx_bad','aborted')]
        if not all(len(values)==1 and int(values[0])==0 for values in errors):
            raise ValueError('physical CRC/RX/abort error')
        values=tuple(int(x,16)for x in m.groups())
        if values[1]&16:raise ValueError('reset/pack-not-ready status during observed interval')
        if previous:
            di=(values[0]-previous['raw_issued'])&0xffffffff
            dh=(values[2]-previous['raw_host'])&0xffffffff
            upper=(row['latest_ns']-previous['earliest_ns'])/1e9
            lower=(row['earliest_ns']-previous['latest_ns'])/1e9
            if not(0<upper<32 and 0<dh<0x80000000 and di<=dh):
                raise ValueError('counter gap/reset/rate guard failed')
            if lower<=0 or not(dh/upper<=clock_hz*1.02 and dh/lower>=clock_hz*.98):
                raise ValueError('host progress inconsistent with declared clock')
            issued+=di;host+=dh
        sample=dict(**row,raw_issued=values[0],raw_host=values[2],status=values[1],issued=issued,host=host)
        main.append(sample);previous=sample
    slo,shi=chain['start']['earliest_ns'],chain['start']['latest_ns']
    elo,ehi=chain['end']['earliest_ns'],chain['end']['latest_ns']
    before=[x for x in main if x['latest_ns']<=slo]
    after=[x for x in main if x['earliest_ns']>=ehi]
    inside=[x for x in main if x['earliest_ns']>=shi and x['latest_ns']<=elo]
    if not before or not after or len(inside)<20:raise ValueError('missing whole-loader counter brackets')
    first,last=inside[0],inside[-1]
    di=last['issued']-first['issued'];dh=last['host']-first['host']
    minimum=di;maximum=after[0]['issued']-before[-1]['issued']
    low=(last['earliest_ns']-first['latest_ns'])/1e9
    high=(last['latest_ns']-first['earliest_ns'])/1e9
    if not(0<low<=high and 0<minimum<=maximum):raise ValueError('invalid counter/timing bounds')
    wire=[x for x in lines_from(records,'wire')if x['latest_ns']>=slo and x['earliest_ns']<=ehi]
    counts=Counter(x['text'].split(' ',1)[0]for x in wire if x['text'])
    replay_phase=Counter();phase=None
    for row in wire:
        m=re.match(r'STP_TRANSFER (\S+) ',row['text'])
        if m:phase=m[1]
        if row['text'].startswith('FTP_REPLAY '):replay_phase[phase]+=1
    return dict(status='PASS: terminal whole-loader counter and protocol accounting',loader=chain,
        counter_scope='Previously qualified view1 StartCycle issues, not pipeline-drained retirement. Unwrap adjacent32bit deltas with <32s gap, reset/rate/host-clock guards.',
        loader_issue_bounds=[minimum,maximum],interior=dict(samples=len(inside),issued=di,host_edges=dh,
            elapsed_visibility_bounds_s=[low,high],issues_per_s_bounds=[di/high,di/low],
            host_hz_bounds=[dh/high,dh/low],issues_per_host_edge=di/dh,
            issues_per_s_from_declared_clock=di*clock_hz/dh),
        wire_log_prefix_counts=dict(counts),replays_by_phase=dict(replay_phase),
        limits='One observed boot. Whole-loader issue bounds and regular-file visibility only; no physical payload hashes, causal replay diagnosis, pinned repeat benchmark, speed promotion or desktop qualification.')


def read_observer(path):
    raw=gzip.decompress(path.read_bytes())if path.suffix=='.gz'else path.read_bytes()
    return [json.loads(x)for x in raw.splitlines()]


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('observer',type=Path)
    parser.add_argument('--stp-root',type=Path,required=True);parser.add_argument('--idle-proof',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    proof=json.loads(args.idle_proof.read_text())
    if proof['selected_instruction_view']!=1 or proof['host_clock_hz']!=50000000:
        raise ValueError('qualified view1/50MHz idle proof required')
    result=analyse(read_observer(args.observer),loader.expected_files(args.stp_root),proof['host_clock_hz'])
    result['inputs_sha256']={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in (args.observer,args.idle_proof,Path(__file__).resolve())}
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print('Whole-loader issues:',result['loader_issue_bounds'],'interior/s:',result['interior']['issues_per_s_bounds'],
          'replays:',result['replays_by_phase'])
