#!/usr/bin/env python3
"""Independent four-option area policy plus unchanged Serial61 source audit.

The source reviewer receives an explicitly rolled-back QSF after checking each
allowed setting exactly. Every inherited damaged-source control runs through
this area reviewer; an area-policy mismatch cannot mask a broken-source test.
No area, timing, native barrier, hardware restore or speed qualification here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import audit_serial_core as base

EXPECTED=[('OPTIMIZATION_MODE','"HIGH PERFORMANCE EFFORT"','"Aggressive Area"'),
          ('QII_AUTO_PACKED_REGISTERS','NORMAL','"Minimize Area with Chains"'),
          ('OPTIMIZATION_TECHNIQUE','SPEED','AREA'),
          ('PHYSICAL_SYNTHESIS_COMBO_LOGIC_FOR_AREA','OFF','ON')]


def audit(baseline,project,overrides=None):
    overrides=dict(overrides or {});qsf=overrides.get('core/Dorado.qsf',(project/'core/Dorado.qsf').read_text())
    record=json.loads((project/'area-integration.json').read_text());serial=json.loads((project/'serial-integration.json').read_text())
    base.check(record['changes']==[dict(name=n,before=o,after=v)for n,o,v in EXPECTED],'area change inventory differs')
    base.check(record['boards']==19 and record['physical_board_bits']==1966264 and record['capacity_only']and not record['assembly_permitted'],'area scope differs')
    base.check(hashlib.sha256((project/'serial-integration.json').read_bytes()).hexdigest()==record['original_serial_integration_sha256'],'original stage record differs')
    base.check(record['original_qsf_sha256']==serial['source_hashes']['core/Dorado.qsf'],'original QSF receipt differs')
    for name,old,new in EXPECTED:
        before='set_global_assignment -name '+name+' '+old+'\n';after='set_global_assignment -name '+name+' '+new+'\n'
        base.check(qsf.count(after)==1 and before not in qsf,'area optimization differs: '+name)
        qsf=qsf.replace(after,before,1)
    overrides['core/Dorado.qsf']=qsf
    result=base.audit(baseline,project,overrides)
    result.update(status='PASS_SOURCE_AREA_OPTIONS_ONLY',area_options=record['changes'],scope=__doc__)
    return result


def controls(baseline,project):
    # First prove that the undamaged area project passes. Every inherited
    # mutation is then reviewed with only that mutation changed.
    audit(baseline,project)
    results=base.controls(baseline,project,reviewer=audit)
    source=(project/'core/Dorado.qsf').read_text()
    for name,_,value in EXPECTED:
        before='set_global_assignment -name '+name+' '+value+'\n';after='set_global_assignment -name '+name+' UNREVIEWED\n'
        base.check(source.count(before)==1,'missing area control anchor: '+name)
        try:audit(baseline,project,{'core/Dorado.qsf':source.replace(before,after,1)})
        except ValueError as error:results.append(dict(case='area_'+name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged area policy accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('baseline',type=Path);p.add_argument('project',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args()
    result=audit(a.baseline.resolve(),a.project.resolve())
    if a.negative_controls:result['failure_controls']=controls(a.baseline.resolve(),a.project.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],boards=19,failure_controls=len(result.get('failure_controls',[])))))
