#!/usr/bin/env python3
"""Check real whole-loader accounting and reject misleading counter evidence."""
import argparse
import copy
import json
from pathlib import Path
import re

from analyse_loader_progress import analyse,read_observer

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]


def run():
    manifest=json.loads((HERE/'loader_fixture_manifest.json').read_text())
    sizes={name:row['bytes']for name,row in manifest.items()};fixtures=[]
    for directory,expected,replay in [
        ('2026-10-19-native49b-repeat',[510928930,514372068],0),
        ('2026-10-19-native52-field',[530134786,533504938],56),
    ]:
        path=ROOT/'fpga/mister/reports'/directory/'loader-observer.jsonl.gz'
        rows=read_observer(path);result=analyse(rows,sizes,50000000)
        assert result['loader_issue_bounds']==expected
        assert sum(result['replays_by_phase'].values())==replay
        assert result['interior']['host_hz_bounds'][0]<50000000<result['interior']['host_hz_bounds'][1]
        fixtures.append(dict(fixture=directory,issue_bounds=expected,replay_events=replay))
    # Use the last complete actual observer, retaining real timestamps and
    # packet data. Only the stated damaged evidence is changed in each case.
    controls=[];indices=[i for i,r in enumerate(rows)if r.get('source')=='main'and'machine='in r.get('text','')]
    target=indices[10]
    for name in ('live_observer','counter_jump','host_reset','large_gap','crc_error','missing_bracket'):
        damaged=copy.deepcopy(rows)
        if name=='live_observer':damaged=damaged[:-1]
        elif name=='counter_jump':
            text=damaged[target]['text'];m=re.search('machine=([0-9a-f]{8})',text)
            value=(int(m[1],16)+0x80000000)&0xffffffff
            damaged[target]['text']=text[:m.start(1)]+f'{value:08x}'+text[m.end(1):]
        elif name=='host_reset':damaged[target]['text']=re.sub('cpu=[0-9a-f]{8}','cpu=00000000',damaged[target]['text'],count=1)
        elif name=='large_gap':damaged[target]['read_end_ns']+=40_000_000_000
        elif name=='crc_error':damaged[target]['text']=damaged[target]['text'].replace('crc_bad=0','crc_bad=01',1)
        else:
            start=result['loader']['start']['earliest_ns']
            damaged=[r for r in damaged if not(r.get('source')=='main'and r['read_end_ns']>=start)]
        try:analyse(damaged,sizes,50000000)
        except ValueError as error:controls.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged counter evidence accepted: '+name)
    return dict(status='PASS',fixtures=fixtures,failure_controls=controls,
        scope='Actual recorded view1 loader counters and split log bounds; no FPGA changes, physical payload or performance promotion')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path)
    args=parser.parse_args();result=run()
    if args.output:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print('LOADER PROGRESS PASS: two actual whole-loader fixtures and six damaged-evidence controls')
