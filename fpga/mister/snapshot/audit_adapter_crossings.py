#!/usr/bin/env python3
"""Read every19-board input/output setup/hold crossing in all four corners.

Verify register coverage and endpoint scopes, retaining each reported violation.
Known positive individual reports test rejection controls even if another scope
in the complete design fails. No physical timing exception is introduced here.
"""
import argparse
import json
from pathlib import Path
import re
from audit_storage_factored_fit import CORNERS
from audit_serial_chipwidth_timing import load

SCOPES={n.lower():f'|dorado_serial_{n}:b_{n}|'for n in 'ContA ContB ProcH ProcL IFU MemC MemD MemX DskEth DispY BaseBd'.split()}
SCOPES.update({label:f'|dorado_serial_MSA:b_{name}|'for label,name in [('msa','msa'),('msaodd','msaOdd'),('msa1','msa1'),('msa1odd','msa1Odd'),('msa2','msa2'),('msa2odd','msa2Odd'),('msa3','msa3'),('msa3odd','msa3Odd')]})


def report(text,scope,direction,mode,allow_failure=False):
    m=re.search(r'Found (\d+) '+mode+r' paths \((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
    if not m:
        assert 'Nothing to report.'in text,'missing crossing timing'
        return dict(paths=0,violations=0,minimum_slack_ns=None)
    rows=[];header=None
    for line in text.splitlines():
        fields=[x.strip()for x in line.split(';')[1:-1]]if line.startswith('; ')else[]
        if fields[:3]==['Slack','From Node','To Node']:
            assert header is None,'duplicate crossing summary';header=fields;continue
        if header and not line.strip():break
        if header and fields:
            assert len(fields)==len(header),'crossing table geometry differs';rows.append(dict(zip(header,fields)))
    assert header is not None and len(rows)==int(m[1])>0,'missing crossing paths'
    for row in rows:
        assert (scope in row['From Node'],scope in row['To Node'])==((True,False)if direction=='out'else(False,True)),'crossing endpoint escaped board scope'
        assert row['Launch Clock']and row['Latch Clock'],'missing crossing clock'
    minimum=min(float(r['Slack'])for r in rows);assert minimum==float(m[3]),'crossing summary slack differs'
    failed=int(m[2])>0 or minimum<0
    assert allow_failure or not failed,'violated crossing timing'
    return dict(paths=len(rows),violations=int(m[2]),minimum_slack_ns=minimum)


def audit(root,prefix,allow_failure):
    log=load(root.parent/'crossings.log');records=[]
    for corner in CORNERS:
        for label,scope in SCOPES.items():
            m=re.search(r'ADAPTER_CROSSING_CORNER '+re.escape(corner)+' '+label+r' board=(\d+) others=(\d+)',log)
            assert m and int(m[1])>0 and int(m[2])>0,'missing board/other register coverage'
            for direction in ('in','out'):
                for mode in ('setup','hold'):
                    name=f'{prefix}-{label}-{direction}-{mode}-{corner}.rpt'
                    record=report(load(root/name),scope,direction,mode,allow_failure)
                    records.append(dict(board=label,corner=corner,direction=direction,mode=mode,**record))
    assert len(records)==304
    violations=[r for r in records if r['violations']or(r['minimum_slack_ns']is not None and r['minimum_slack_ns']<0)]
    return dict(status='REJECTED_CROSSING_TIMING'if violations else'PASS_REPORTED_ADAPTER_CROSSINGS',scope=__doc__,records=records,
                reports=304,boards=19,corners=4,violations=violations,sequential_exclusions=[])


def controls(root,prefix):
    # This exact actual fast-corner report passes before any corruption.
    label='memc';scope=SCOPES[label];text=load(root/f'{prefix}-{label}-in-setup-MIN_fast_1100mv_100c.rpt')
    before=report(text,scope,'in','setup');assert before['minimum_slack_ns']>0
    out=[]
    for name in ('violation','missing','scope'):
        if name=='violation':damaged=text.replace('(0 violated)','(1 violated)',1)
        elif name=='missing':damaged=text.replace('Worst case slack is','Missing slack was',1)
        else:damaged=text.replace(scope,'|dorado_serial_MemD:b_MemD|')
        try:report(damaged,scope,'in','setup')
        except AssertionError as e:out.append(dict(case=name,rejected=True,reason=str(e)))
        else:raise AssertionError('damaged passing crossing accepted: '+name)
    return out


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--prefix',default='serial61');p.add_argument('--allow-timing-failure',action='store_true');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory,a.prefix,a.allow_timing_failure);result['failure_controls']=controls(a.directory,a.prefix)
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k]for k in ('status','reports','boards','corners','violations','failure_controls')}))
