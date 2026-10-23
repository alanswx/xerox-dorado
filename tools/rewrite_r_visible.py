#!/usr/bin/env python3
"""Use the existing closed read-latch values while visible R is transparent.

Clock1'Bb = ppClk | pClock1En and PreSHCP'B = ~pClock1En.
MC10173 is LOW-transparent, so the read and visible windows cannot overlap
in the Boolean capture model. This changes no state, clock or data epoch.
"""
import hashlib
from pathlib import Path

PARENT_SHA='d4d39beb23d07dd54d978b56bb7fded9703b667291fff22707f5f438d73f9f30'
EDITS=[]
for gate,out,flag,a,b in [(0,'579:572',589,'547:540','563:556'),
                         (1,'587:580',591,'555:548','571:564')]:
    old=f'   if(!r_visible_n[{gate}]) state_next[{out}]=state_next[{flag}] ? state_next[{a}] : state_next[{b}];'
    new=f'   if(!r_visible_n[{gate}]) state_next[{out}]=state_q[{flag}] ? state_q[{a}] : state_q[{b}];'
    EDITS.append((old,new))


def rewrite(path):
    raw=path.read_text();before=hashlib.sha256(raw.encode()).hexdigest()
    assert before==PARENT_SHA
    for old,new in EDITS:
        assert raw.count(old)==1;raw=raw.replace(old,new)
    inverse=raw
    for old,new in EDITS:inverse=inverse.replace(new,old)
    assert hashlib.sha256(inverse.encode()).hexdigest()==PARENT_SHA
    path.write_text(raw)
    return dict(parent_sha256=before,output_sha256=hashlib.sha256(raw.encode()).hexdigest(),
                changes=EDITS,extra_architectural_state=False,native_integrated=False,
                scope=__doc__)
