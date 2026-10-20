#!/usr/bin/env python3
"""Terminal equal-coverage area comparison, still a failed fit, never timing.

The area source audit proves the four optimizer changes and unchanged RTL.
For the shared capacity-report reader only, expose its 23 inherited source
controls separately from its four area-policy controls. This is explicit
report-schema adaptation, not a claim that the actual QSF uses the old policy.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
from audit_serial_area import EXPECTED
import audit_serial_capacity as cap


def review(current,baseline,classification,equal_coverage):
    source=json.loads((current/'terminal-source-audit.json').read_text())
    assert source['status']=='PASS_SOURCE_AREA_OPTIONS_ONLY'and len(source['failure_controls'])==27
    approved=[dict(name=n,before=o,after=v)for n,o,v in EXPECTED]
    assert source['area_options']==approved
    assert [r['case']for r in source['failure_controls'][-4:]]==['area_'+n for n,_,_ in EXPECTED]
    assert equal_coverage['status']=='PASS_IDENTICAL_RTL_AND_CONSTRAINTS'and equal_coverage['checked_files']==659
    assert equal_coverage['changed_files']==['core/Dorado.qsf']and equal_coverage['approved_optimizer_changes']==approved
    # Only the source-audit envelope is adapted. Actual fit summaries, errors,
    # hardware state, nineteen scopes and three transport rows are read intact.
    inherited=copy.deepcopy(source);inherited['status']='PASS_SOURCE_ONLY';inherited['failure_controls']=inherited['failure_controls'][:23]
    before_source=json.loads((baseline/'source-audit-final.json').read_text())
    before=cap.review(baseline,before_source,classification)
    after=cap.review(current,inherited,classification)
    assert before['alms_needed']==49250 and after['alms_needed']<before['alms_needed']
    reports=(current/'core/output_files/Dorado.fit.rpt').read_text(encoding='latin1')
    assert 'Aggressive Area optimization mode selected'in reports and 'Minimize Area with Chains'in reports
    mapping=(current/'core/output_files/Dorado.map.rpt').read_text(encoding='latin1')
    assert '; Optimization Technique'in mapping and '; Area 'in mapping
    result=dict(status='REJECTED_CAPACITY_AREA_REDUCED',scope=__doc__,baseline_alms=before['alms_needed'],
                area_alms=after['alms_needed'],available_alms=after['available_alms'],
                reduction_alms=before['alms_needed']-after['alms_needed'],excess_alms=after['excess_alms'],
                area_labs=after['labs_needed'],available_labs=after['available_labs'],area_options=approved,
                equal_coverage_files=equal_coverage['checked_files'],area_diagnostics=after,
                failure_controls=cap.controls(current,inherited,classification),
                next='Prototype a shared glitch-free physical FF clock enable for guest advances/scans; retain all physical state and guest cadence; qualify before native use.',
                limits='No legal placed core, STA, assembler, physical memory qualification, FPGA reload or speed improvement.')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('current',type=Path);p.add_argument('--baseline',type=Path,required=True);p.add_argument('--classification',type=Path,required=True);p.add_argument('--equal-coverage',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=review(a.current,a.baseline,json.loads(a.classification.read_text()),json.loads(a.equal_coverage.read_text()))
    paths=[Path(__file__),a.classification,a.equal_coverage,*[x for d in (a.current,a.baseline)for x in d.rglob('*')if x.is_file()]]
    result['input_hashes']={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k]for k in ('status','baseline_alms','area_alms','reduction_alms','excess_alms')}))
