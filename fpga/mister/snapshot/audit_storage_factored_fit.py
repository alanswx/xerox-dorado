#!/usr/bin/env python3
"""Read completed matched frontend fits; reject a correct but larger variant."""
import argparse
import hashlib
import json
from pathlib import Path
import re

CORNERS=('7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c')


def audit(root,overrides=None):
    overrides=overrides or {};read=lambda n:overrides.get(n,(root/n).read_text(encoding='latin1'))
    provenance=json.loads(read('provenance.json'));results={}
    assert 'Matched fit terminal'in read('driver.log')
    for name,digest in provenance['compiled_files'].items():
        if not name.endswith('.qsf'):assert hashlib.sha256((root/name).read_bytes()).hexdigest()==digest,'compiled input changed: '+name
    for label in ('baseline','factored'):
        qsf=read(label+'/snapshot_fit.qsf');expected=provenance['expected_qsf']
        suffix='\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
        assert qsf in (expected,expected+suffix,expected+suffix+'\n'),'unapproved fixture policy'
        for name,key in [('top.sv','expected_top'),('bank.sdc','expected_sdc'),('audit.tcl','expected_audit')]:assert read(label+'/'+name)==provenance[key],'fixture/timing input differs'
        summary=read(label+'/snapshot_fit.fit.summary');mapping=read(label+'/snapshot_fit.map.summary')
        assert 'Fitter Status : Successful'in summary and 'Device : 5CSEBA6U23I7'in summary
        assert 'Analysis & Synthesis Status : Successful'in mapping
        assert 'Total virtual pins : 221'in summary and 'Total block memory bits : 0 /'in summary and 'Total DSP Blocks : 0 /'in summary
        area=re.search(r'Logic utilization \(in ALMs\) : (\d+) / 41,910',summary);assert area
        regs=re.search(r'Total registers : (\d+)',summary);map_regs=re.search(r'Total registers : (\d+)',mapping);assert regs and map_regs
        timing=[]
        for corner in CORNERS:
            for kind in ('setup','hold','mpw'):
                text=read(f'{label}/bank-{kind}-{corner}.rpt')
                match=re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
                assert match and int(match[1])>0 and int(match[2])==0 and float(match[3])>=0,'timing violation/missing report'
                timing.append(dict(corner=corner,kind=kind,slack_ns=float(match[3])))
            for kind in ('recovery','removal'):assert 'Nothing to report.'in read(f'{label}/bank-{kind}-{corner}.rpt')
            coverage=read(f'{label}/bank-ucp-{corner}.rpt')
            for prop in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                match=re.search(r';\s*'+prop+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',coverage)
                assert match and match.groups()==('0','0'),'unconstrained fixture path'
            clocks=read(f'{label}/bank-clocks-{corner}.rpt')
            assert re.findall(r'(?m)^;\s*(\S+)\s*;\s*(Base|Generated)\s*;',clocks)==[('host_clock','Base')]
            assert re.search(r';\s*host_clock\s*;\s*Base\s*;\s*20\.000\s*;\s*50\.0 MHz\s*;',clocks),'clock changed'
        results[label]=dict(alms=int(area[1]),registers=int(regs[1]),synthesis_registers=int(map_regs[1]),timing=timing,minimum_slack_ns=min(r['slack_ns']for r in timing))
    assert results['baseline']['synthesis_registers']==results['factored']['synthesis_registers']
    difference=results['factored']['alms']-results['baseline']['alms']
    assert difference>=0,'experiment now has an area saving; review disposition'
    return dict(status='REJECTED_NO_AREA_BENEFIT',scope=__doc__,results=results,additional_alms=difference,
                hardware_changed=False,limits='Matched component measurement only. No integration, legal complete-core fit or speed improvement.')


def controls(root):
    cases=[('loose_clock','factored/bank.sdc','20.000','40.000'),
           ('changed_policy','factored/snapshot_fit.qsf','HIGH PERFORMANCE EFFORT','Aggressive Area'),
           ('bad_hold','factored/bank-hold-7_slow_1100mv_100c.rpt','(0 violated)','(1 violated)'),
           ('missing_timing','factored/bank-setup-MIN_fast_1100mv_-40c.rpt','Worst case slack is','Missing slack was')]
    results=[]
    for name,path,before,after in cases:
        source=(root/path).read_text(encoding='latin1');assert before in source
        try:audit(root,{path:source.replace(before,after,1)})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged matched fit accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory);result['failure_controls']=controls(a.directory)
    result['input_hashes']={str(x.relative_to(a.directory)):hashlib.sha256(x.read_bytes()).hexdigest()for x in a.directory.rglob('*')if x.is_file()}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],baseline_alms=result['results']['baseline']['alms'],factored_alms=result['results']['factored']['alms'],additional_alms=result['additional_alms'],minimum_slack_ns=min(v['minimum_slack_ns']for v in result['results'].values()))))
