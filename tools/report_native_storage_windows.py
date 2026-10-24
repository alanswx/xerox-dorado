#!/usr/bin/env python3
"""Read passive native firmware windows, retaining partial/aborted captures.

An END proves only an internally consistent observation window. Original
firmware progress, matching workloads, native timing and hardware speed remain
separate evidence. Window controls never alter guest or device state.
"""
import argparse
import json
from pathlib import Path
import re

from report_native_storage_traffic import parse,compare


def fields(line):return dict(re.findall(r'(\w+)=([^ ]+)',line))


def parse_windows(raw):
    partial=bool(raw and not raw.endswith(b'\n'))
    if partial:raw=raw[:raw.rfind(b'\n')+1]
    windows={};active=None
    for line in raw.decode(errors='replace').splitlines():
        if not line.startswith('NATIVE_WINDOW_'):continue
        kind=line.split()[0][14:];f=fields(line)
        if kind=='ARMED':continue
        i=int(f['id'])
        if kind=='BEGIN':
            assert active is None and i not in windows,'Window overwritten or epoch repeated'
            windows[i]=dict(begin=f,records=[],footer=None,gate={},block={},fanout={},groups={},read_class={});active=i
        else:
            assert i in windows,'Window record without begin';w=windows[i]
            if kind in ['A','R']:
                assert active==i,'Traffic after closed window'
                w['records'].append(line.replace('NATIVE_WINDOW_'+kind,'NATIVE_TRAFFIC_'+kind,1))
            elif kind in ['END','ABORT','PARTIAL']:
                assert active==i and w['footer']is None,'Duplicate window footer'
                w['footer']=dict(status=kind,**f);active=None
            elif kind in ['GATE','BLOCK','FANOUT','GROUP','READ_CLASS']:
                assert w['footer']is not None,'Ledger before footer'
                table={'GATE':'gate','BLOCK':'block','FANOUT':'fanout','GROUP':'groups','READ_CLASS':'read_class'}[kind]
                key=int(f['class']if kind in ['GATE','BLOCK','READ_CLASS']else f['pending']if kind=='FANOUT'else f['group'])
                assert key not in w[table],'Duplicate ledger category';w[table][key]=f
            else:raise AssertionError('Unknown window record: '+kind)
    for i,w in windows.items():
        w['trace']=parse(('\n'.join(w.pop('records'))+'\n').encode())
        begin=w['begin'];trace=w['trace'];assert trace['accepted']
        assert trace['accepted'][0]['tick']==int(begin['tick'])
        footer=w['footer'];w['complete']=bool(footer and footer['status']=='END')
        if footer:
            assert len(trace['accepted'])==int(footer['admitted'])
            assert len(trace['completed'])==int(footer['completed'])
            assert int(footer['consume_sum'])==sum(r['latency']for r in trace['completed'])
            assert int(footer['ticks'])==int(footer['end'])-int(footer['begin'])+1
            gate_classes=int(begin.get('gate_classes',7))
            assert gate_classes in (7,8),'Unknown gate ledger schema'
            assert set(w['gate'])==set(range(gate_classes)) and set(w['block'])==set(range(9))
            for table in ['gate','block','fanout']:
                assert sum(int(f['ticks'])for f in w[table].values())==int(footer['ticks']),table
            assert sum(int(f['admitted'])for f in w['groups'].values())==int(footer['admitted'])
            assert set(w['read_class'])==set(range(6))
            assert sum(int(f['admitted'])for f in w['read_class'].values())==int(footer['admitted'])
        if w['complete']:
            assert trace['unpaired_request']is None and int(footer['pending'])==0
            assert int(footer['admitted'])==int(footer['completed'])==int(begin['limit'])
            assert int(footer['first_count'])==int(footer['completed'])
        w['firmware_trigger_observed']=int(begin['trigger'])==2 and int(begin['BFN'],8)==0o113
    return dict(windows=windows,trailing_partial_record=partial,active_window=active,
        hardware_speed_or_firmware_correctness_qualified=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--log',type=Path,required=True)
    p.add_argument('--candidate-log',type=Path);p.add_argument('--epoch',type=int,default=1)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists()
    result=parse_windows(a.log.read_bytes())
    if a.candidate_log:
        candidate=parse_windows(a.candidate_log.read_bytes());left=result['windows'][a.epoch];right=candidate['windows'][a.epoch]
        result=dict(control=result,candidate=candidate,comparison=compare(left['trace'],right['trace']),
            complete_observer_windows=left['complete']and right['complete'],hardware_speed_qualified=False)
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('NATIVE_WINDOW_REPORT',a.output)
