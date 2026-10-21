#!/usr/bin/env python3
"""Locate suspect ranges in early PCT data and check settled checksum operands.

PCT samples at CIA changes; task switches can expose unrelated operands.
Its deltas locate a follow-up probe, not consumed data. The bounded phase
trace separately checks MD, A and ALU at the end of each complete iteration.
"""
import argparse
import hashlib
import itertools
import json
from pathlib import Path
import re
import struct


def inspect(pct,phase,eb,allow_mismatch=False):
    image=eb.read_bytes()[512:]
    expected=list(struct.unpack('>'+str(len(image)//2)+'H',image))
    text=pct.read_text()
    early=[int(m[1],16)for m in re.finditer(r'CIA=d5e .*task=0 .*alu=(\w+)',text)]
    final=re.findall(r'CIA=d5f .*task=0 .*alu=(\w+)',text)
    assert len(early)==len(expected) and len(final)==1
    # The first d5e PCT still holds the preceding BP reset, not a checksum.
    assert early[0]==0x200
    sums=early[1:]+[int(final[0],16)]
    deltas=[(v-(sums[i-1]if i else 0))&65535 for i,v in enumerate(sums)]
    bad=[i for i,(a,b)in enumerate(zip(deltas,expected))if a!=b]
    suspect=[];zero_sum_groups=0
    for _,group in itertools.groupby(enumerate(bad),lambda p:p[1]-p[0]):
        indices=[v for _,v in group];first,last=indices[0],indices[-1]
        difference=(sum(deltas[first:last+1])-sum(expected[first:last+1]))&65535
        if difference:
            suspect.append(dict(image_word=first,VM_word=hex(first+512),
                length=last-first+1,sum16_difference=hex(difference)))
        else:zero_sum_groups+=1
    rows=[dict(re.findall(r'(\w+)=([^ ]+)',line))for line in phase.read_text().splitlines()
          if line.startswith('BOOT_DIAG_PHASE ')]
    ends=[r for i,r in enumerate(rows[:-1])if r['CIA']=='d51'and r['task']=='0'and
          rows[i+1]['CIA']!='d51']
    assert ends,'No complete checksum iterations captured'
    prefixes=[0]
    for value in expected:prefixes.append((prefixes[-1]+value)&65535)
    mismatches=[];accumulator_mismatches=[]
    for row in ends:
        # The original loop increments BootDataPtr as it issues Fetch.
        # Read that register to locate the consumed word; the shared VA bus
        # need not retain this reference's address at its completion.
        address=int(row['BP'],16)-1;index=address-512
        assert 0<=index<len(expected),row
        assert row['Hold']=='0',row
        assert (int(row['A'],16)+int(row['MD'],16))&65535==int(row['ALU'],16),row
        if int(row['MD'],16)!=expected[index]:
            mismatches.append(dict(sample=int(row['sample']),VM_word=hex(address),VA_bus=row['VA'],
                observed=row['MD'],expected=f'{expected[index]:04x}'))
        if int(row['A'],16)!=prefixes[index]:
            accumulator_mismatches.append(dict(sample=int(row['sample']),VM_word=hex(address),
                observed=row['A'],expected=f'{prefixes[index]:04x}'))
    if not allow_mismatch:
        assert not mismatches,mismatches
        assert not accumulator_mismatches,accumulator_mismatches
    assert sum(int(r['sum16_difference'],16)for r in suspect)&65535==int(final[0],16)
    return dict(scope=__doc__,status='CHECKSUM_DIAGNOSTIC_NOT_A_BOOT_GATE',
        settled_words_inspected=len(ends),settled_MD_mismatches=mismatches,
        settled_accumulator_mismatches=accumulator_mismatches,
        settled_first_VM_word=hex(int(ends[0]['BP'],16)-1),settled_last_sample=int(ends[-1]['sample']),
        PCT_suspect_ranges=suspect,PCT_zero_sum_transient_groups=zero_sum_groups,
        PCT_final_ALU=final[0],image_sum16=sum(expected)&65535,
        first_suspect_MD_verified={int(row['VM_word'],16)for row in mismatches}==set(range(0xe10,0xe20)),
        other_suspect_MD_verified=False,hardware_cause_established=False,
        inputs={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in[pct,phase,eb,Path(__file__)]})


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in['pct','phase','eb','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--allow-mismatch',action='store_true')
    a=p.parse_args();record=inspect(a.pct,a.phase,a.eb,a.allow_mismatch)
    with a.output.open('x')as f:json.dump(record,f,indent=2);f.write('\n')
    print(json.dumps(dict(status=record['status'],settled_words_inspected=record['settled_words_inspected'],
        MD_mismatches=len(record['settled_MD_mismatches']),
        first_suspect_MD_verified=record['first_suspect_MD_verified'],output=str(a.output))))
