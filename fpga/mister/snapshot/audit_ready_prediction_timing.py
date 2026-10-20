#!/usr/bin/env python3
"""Review actual Serial73 cached-readiness registers and all 32 path groups.

Require every added register in all four device conditions and explicit paths
into/out of the cache and to/from the falling-edge guest-advance register.
Retain every reported timing failure. This is not a full-design timing gate.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from audit_storage_factored_fit import CORNERS
from audit_serial_chipwidth_timing import load

FLAG=re.compile(r'\|snapshot_msa_flags_ready\[([0-7])\]$')
STEP=re.compile(r'\|network_step(?:~[^|]+)?$')
DIRECTIONS=('into','out','cache-to-step','step-to-cache')


def summary(text,direction,mode):
    match=re.search(r'Found (\d+) '+mode+r' paths \((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
    assert match and int(match[1])>0,'missing nonempty readiness timing group'
    rows=[];header=None
    for line in text.splitlines():
        fields=[x.strip()for x in line.split(';')[1:-1]]if line.startswith('; ')else[]
        if fields[:3]==['Slack','From Node','To Node']:
            assert header is None,'duplicate readiness summary';header=fields;continue
        if header and not line.strip():break
        if header and fields:
            assert len(fields)==len(header),'readiness table geometry differs'
            rows.append(dict(zip(header,fields)))
    assert header and len(rows)==int(match[1]),'missing readiness paths'
    for row in rows:
        start,end=row['From Node'],row['To Node']
        if direction in('into','step-to-cache'):assert FLAG.search(end),'readiness endpoint escaped cache'
        else:assert FLAG.search(start),'readiness source escaped cache'
        if direction=='cache-to-step':assert STEP.search(end),'cached-ready path escaped guest advance'
        if direction=='step-to-cache':assert STEP.search(start),'guest-advance path escaped predictor'
        assert row['Launch Clock']and row['Latch Clock'],'missing readiness clock'
    minimum=min(float(row['Slack'])for row in rows)
    assert minimum==float(match[3]),'readiness summary slack differs'
    return dict(paths=len(rows),violations=int(match[2]),minimum_slack_ns=minimum,summary=rows)


def audit(root,overrides=None):
    overrides=overrides or{}
    def read(name):return overrides[name]if name in overrides else load(root/name)
    log=read('ready-path.log');registers={};records=[]
    for corner in CORNERS:
        counts=re.findall(r'^READY_PREDICTION_CORNER '+re.escape(corner)+r' flags=(\d+) step=(\d+)$',log,re.M)
        assert len(counts)==1 and int(counts[0][0])==8 and int(counts[0][1])>0,'missing actual eight-register coverage'
        names=re.findall(r'^READY_PREDICTION_REGISTER '+re.escape(corner)+r' (.+)$',log,re.M)
        assert len(names)==len(set(names))==8,'duplicate/missing actual cache register'
        bits=[]
        for name in names:
            match=FLAG.search(name);assert match,'unexpected mapped cache register: '+name
            bits.append(int(match[1]))
        assert sorted(bits)==list(range(8)),'missing cache bit'
        registers[corner]=sorted(names)
        for direction in DIRECTIONS:
            for mode in('setup','hold'):
                name=f'core/ready73-{direction}-{mode}-{corner}.rpt'
                result=summary(read(name),direction,mode)
                records.append(dict(corner=corner,direction=direction,mode=mode,**result))
    assert len(records)==32
    violations=[r for r in records if r['violations']or r['minimum_slack_ns']<0]
    return dict(status='REJECTED_READY_PREDICTION_TIMING'if violations else'PASS_REPORTED_READY_PREDICTION_TIMING',scope=__doc__,
        actual_registers=registers,records=records,violations=violations,groups=32,
        minimum_slack_ns=min(r['minimum_slack_ns']for r in records),
        limits='Registered-readiness paths only. Capacity, all-device/native IO and board crossing timing remain separate gates; no deployment or speed claim.')


def controls(root):
    original=audit(root);passing=next((r for r in original['records']if r['violations']==0 and r['minimum_slack_ns']>0),None)
    assert passing,'no passing group for rejection controls'
    path=f'core/ready73-{passing["direction"]}-{passing["mode"]}-{passing["corner"]}.rpt'
    text=load(root/path);log=load(root/'ready-path.log');results=[]
    wrong_scope=text.replace('|snapshot_msa_flags_ready[','|unreviewed_flags[')
    cases=[('bad_scope',path,wrong_scope),('missing_summary',path,text.replace('Worst case slack is','Missing slack was',1)),
        ('missing_register', 'ready-path.log',re.sub(r'^READY_PREDICTION_REGISTER [^\n]+\n','',log,count=1,flags=re.M)),
        ('wrong_count','ready-path.log',log.replace('flags=8','flags=7',1)),
        ('unknown_register','ready-path.log',log.replace('|snapshot_msa_flags_ready[0]','|snapshot_msa_flags_ready[8]',1))]
    for label,name,damaged in cases:
        try:audit(root,{name:damaged})
        except AssertionError as e:results.append(dict(case=label,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged registered-readiness evidence accepted: '+label)
    damaged=text.replace('(0 violated)','(1 violated)',1);assert damaged!=text
    result=audit(root,{path:damaged})
    assert result['status']=='REJECTED_READY_PREDICTION_TIMING'and any(r['violations']for r in result['violations'])
    results.append(dict(case='reported_violation',rejected=True,diagnostic='Explicitly retained as REJECTED_READY_PREDICTION_TIMING.'))
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory);result['failure_controls']=controls(a.directory)
    result['input_hashes']={str(p.relative_to(a.directory)):hashlib.sha256(p.read_bytes()).hexdigest()
        for p in [a.directory/'ready-path.log',*sorted((a.directory/'core').glob('ready73-*.rpt'))]}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k]for k in('status','groups','minimum_slack_ns','failure_controls')}))
