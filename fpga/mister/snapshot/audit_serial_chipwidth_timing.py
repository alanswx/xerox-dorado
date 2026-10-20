#!/usr/bin/env python3
"""Review four native device conditions and the unchanged framework IO exceptions.

Native59 already reports unconstrained asynchronous/framework ports. Require
their exact names/comments/counts, zero illegal/unconstrained clocks and the
same actual clock definitions; never present this as every physical IO timed.
Source/constraint identity, capacity and native service barriers are separate.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
from audit_storage_factored_fit import CORNERS


def load(path):
    if path.exists():return path.read_text(encoding='latin1')
    return gzip.decompress(path.with_name(path.name+'.gz').read_bytes()).decode('latin1')


def tables(source,first):
    header=None;rows=[];found=[]
    for line in source.splitlines():
        if header and not line.strip():
            assert rows;found.append(rows);header=None;rows=[]
        if not line.startswith('; '):continue
        fields=[x.strip()for x in line.split(';')[1:-1]]
        if fields and fields[0]==first:assert header is None;header=fields
        elif header:
            assert len(fields)==len(header),'report table geometry differs'
            rows.append(dict(zip(header,fields)))
    if header:
        assert rows;found.append(rows)
    assert found,'missing report table: '+first
    return found


def coverage(text):
    totals={}
    for prop in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
        match=re.search(r';\s*'+prop+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',text);assert match,'missing coverage: '+prop
        totals[prop]=tuple(map(int,match.groups()))
    assert totals['Illegal Clocks']==totals['Unconstrained Clocks']==(0,0),'illegal/unconstrained clock'
    return dict(totals=totals,input_ports=tables(text,'Input Port'),output_ports=tables(text,'Output Port'))


def clocks(text):
    fields=('Clock Name','Type','Period','Frequency','Rise','Fall','Inverted','Master','Source')
    found=tables(text,'Clock Name');assert len(found)==1
    return sorted(({k:r[k]for k in fields}for r in found[0]),key=lambda r:r['Clock Name'])


def audit(root,baseline,prefix='serial61',overrides=None,allow_timing_failure=False):
    overrides=overrides or {};read=lambda n:overrides[n]if n in overrides else load(root/n)
    timings=[];exceptions={};clock_records={}
    for corner in CORNERS:
        for kind in ('setup','hold','recovery','removal','mpw','sdram-input-setup','sdram-input-hold','sdram-output-setup','sdram-output-hold'):
            name=f'{prefix}-{kind}-{corner}.rpt';text=read(name)
            match=re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
            assert match and int(match[1])>0,'missing timing: '+name
            failed=int(match[2])!=0 or float(match[3])<0
            assert allow_timing_failure or not failed,'violated timing: '+name
            timings.append(dict(corner=corner,kind=kind,slack_ns=float(match[3]),violated_reported_paths=int(match[2])))
        actual=coverage(read(f'{prefix}-ucp-{corner}.rpt'));expected=coverage(load(baseline/f'native59-ucp-{corner}.rpt'))
        assert actual==expected,'framework exception ports/counts differ: '+corner
        exceptions[corner]=actual
        actual=clocks(read(f'{prefix}-clocks-{corner}.rpt'));expected=clocks(load(baseline/f'native59-clocks-{corner}.rpt'))
        assert actual==expected,'actual clocks/phase/period differ: '+corner
        clock_records[corner]=actual
    violations=[t for t in timings if t['violated_reported_paths']or t['slack_ns']<0]
    return dict(status='REJECTED_TIMING'if violations else'PASS_REPORTED_TIMING_SAME_FRAMEWORK_IO_EXCEPTIONS',scope=__doc__,timing=timings,violations=violations,
        minimum_slack_ns=min(t['slack_ns']for t in timings),framework_exceptions=exceptions,clocks=clock_records,
        limits='Same inherited unconstrained framework IO, explicitly listed. No complete physical IO, real pause, FPGA snapshot or speed qualification.')


def controls(root,baseline,prefix='serial61'):
    cases=[('bad_hold',f'{prefix}-hold-7_slow_1100mv_100c.rpt','(0 violated)','(1 violated)'),
        ('bad_sdram',f'{prefix}-sdram-output-hold-MIN_fast_1100mv_-40c.rpt','(0 violated)','(1 violated)'),
        ('missing_setup',f'{prefix}-setup-7_slow_1100mv_-40c.rpt','Worst case slack is','Missing slack was'),
        ('different_clock',f'{prefix}-clocks-7_slow_1100mv_100c.rpt','20.000','40.000'),
        ('different_port',f'{prefix}-ucp-7_slow_1100mv_100c.rpt','HDMI_I2C_SDA','CAPACITY_SERVICES_HELD[0]')]
    results=[]
    for name,path,before,after in cases:
        text=load(root/path);assert before in text
        try:audit(root,baseline,prefix,{path:text.replace(before,after,1)})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged timing report accepted: '+name)
    path=f'{prefix}-ucp-7_slow_1100mv_100c.rpt';text=load(root/path)
    marker='; Unconstrained Input Ports';at=text.rfind(marker);assert at>=0 and 'IO_SDA'in text[at:]
    damaged=text[:at]+text[at:].replace('IO_SDA','UNREVIEWED_INPUT',1)
    try:audit(root,baseline,prefix,{path:damaged})
    except AssertionError as error:results.append(dict(case='second_port_table',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('second coverage table mutation accepted')
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--baseline',type=Path,required=True)
    p.add_argument('--prefix',default='serial61');p.add_argument('--output',type=Path,required=True);p.add_argument('--allow-timing-failure',action='store_true');a=p.parse_args()
    result=audit(a.directory,a.baseline,a.prefix,allow_timing_failure=a.allow_timing_failure)
    if result['violations']:
        result['failure_controls']=controls(a.baseline,a.baseline,'native59')
        result['control_scope']='Known passing Native59 baseline; failing actual timing is retained as rejection evidence'
    else:result['failure_controls']=controls(a.directory,a.baseline,a.prefix)
    result['input_hashes']={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for folder in (a.directory,a.baseline)for p in folder.iterdir()if p.is_file()and p.name.startswith((a.prefix,'native59-'))}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],minimum_slack_ns=result['minimum_slack_ns'],failure_controls=len(result['failure_controls']))))
