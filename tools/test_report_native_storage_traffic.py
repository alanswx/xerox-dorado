#!/usr/bin/env python3
"""Reject malformed captured ownership/order and misleading prefix comparisons."""
import argparse
import hashlib
import json
from pathlib import Path

from report_native_storage_traffic import parse,compare

TRACE='''NATIVE_TRAFFIC_A tick=10 address=000081 write=0 data=0123 mask=2 state=2 fanout=1 pc=30221000
NATIVE_TRAFFIC_R tick=14 address=000081 write=0 data=0456 latency=4 pc=30221000
NATIVE_TRAFFIC_A tick=14 address=000102 write=1 data=0789 mask=4 state=2 fanout=1 pc=30222000
NATIVE_TRAFFIC_R tick=16 address=000102 write=1 data=aaaa latency=2 pc=30222000
NATIVE_TRAFFIC_A tick=16 address=000183 write=0 data=0000 mask=8 state=2 fanout=1 pc=30223000
NATIVE_TRAFFIC_PROGRESS host=100 issues=3 admitted=3 completed=2 reads=2 writes=1 BFN=00000000113
NATIVE_PROGRESS host=100 guest=80 initialized=1
NATIVE_C_RELEASED host=40 guest=20
'''


def run(output):
    assert not output.exists(),'Preserve existing reader-control result'
    a=parse(TRACE.encode());assert len(a['completed'])==2 and a['unpaired_request']['address']==0x183
    assert a['progress'][-1]['BFN']==0o113 and a['actual_Cedar113_key_release_observed']
    assert a['latency']['read']['mean']==4 and a['latency']['write']['mean']==2
    assert compare(a,parse(TRACE.replace('data=aaaa','data=bbbb').encode()))['ordered_operation_data_agree']
    assert not compare(a,parse(TRACE.replace('data=0456','data=0457').encode()))['ordered_operation_data_agree']
    assert not compare(a,parse(TRACE.replace('data=0789','data=0788').encode()))['ordered_operation_data_agree']
    assert compare(parse(b''),a)['ordered_operation_data_agree']is None
    budget=compare(a,a)['control_prefix_budget']
    assert budget['consumed_raw_service_ticks']==6 and budget['unowned_raw_ticks']==0
    assert budget['same_edge_handoffs']==1 and budget['positive_gaps']==0
    cases=[
        ('orphan',TRACE[TRACE.index('NATIVE_TRAFFIC_R'):]),
        ('owner',TRACE.replace('tick=14 address=000081','tick=14 address=000082')),
        ('latency',TRACE.replace('latency=4','latency=5')),
        ('duplicate',TRACE.replace('NATIVE_TRAFFIC_R tick=14',TRACE.splitlines()[0]+'\nNATIVE_TRAFFIC_R tick=14')),
        ('backward',TRACE.replace('NATIVE_TRAFFIC_R tick=16','NATIVE_TRAFFIC_R tick=13')),
    ]
    rows=[]
    for name,text in cases:
        try:parse(text.encode())
        except AssertionError:rows.append(dict(case=name,rejected=True))
        else:raise AssertionError('Malformed trace survived: '+name)
    assert parse((TRACE+'NATIVE_TRAFFIC_A tick=').encode())['trailing_partial_record']
    result=dict(status='PASS_NATIVE_TRAFFIC_READER_CONTROLS',negative_cases=rows,
        write_response_data_ignored=True,read_and_write_corruption_detected=True,
        trace_cap_unpaired_request_preserved=True,partial_record_preserved=True,
        empty_comparison_not_pass=True,native_firmware_qualified=False,
        source_hashes={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in [
            Path(__file__),Path(__file__).with_name('report_native_storage_traffic.py')]})
    output.write_text(json.dumps(result,indent=2)+'\n');print(result['status'])


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    run(p.parse_args().output)
