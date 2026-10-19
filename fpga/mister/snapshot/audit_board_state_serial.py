#!/usr/bin/env python3
"""Independent isolated serial transport source, retention, policy and timing audit."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_board_state_serial as fit
from audit_baseboard_full import table,normalized,CORNERS


def audit(root):
    record=json.loads((root/'provenance.json').read_text())
    assert fit.sha(Path(fit.__file__))==record['driver_sha256'],'executed runner differs'
    for n,h in record['compiled_files'].items():
        if not n.endswith('.qsf'):assert fit.sha(root/n)==h,'compiled input differs: '+n
    results={}
    with tempfile.TemporaryDirectory(prefix='serial-transport-reference-',dir='/tmp')as tmp:
        expected=Path(tmp)/'fit';fit.prepare(expected)
        for variant in ('baseline','snapshot'):
            case=root/variant
            for n in ('snapshot_fit.qsf','bank.sdc','audit.tcl','top.sv'):
                a=(case/n).read_text();b=(expected/variant/n).read_text()
                assert (normalized(a)==normalized(b) if n.endswith('.qsf') else a==b),'project/clock policy differs: '+n
            for n in ('board_state_fabric.sv','board_state_uio.sv','board_state_fabric_serial.sv','board_state_uio_serial.sv'):
                assert (case/n).read_bytes()==(fit.HERE/n).read_bytes(),'qualified transport differs'
            summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful' in summary
            resources={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                hit=re.search(label+r' : ([\d,]+)',summary);assert hit
                resources[key]=int(hit[1].replace(',',''))
            assert resources['ram_bits']==resources['ram_blocks']==0,'unexpected transport RAM'
            rows=table((case/'snapshot_fit.map.rpt').read_text(encoding='latin1'),
                       'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node')
            expected_regs={'snapshot_fit':('1038 (672)' if variant=='baseline' else '1110 (726)'),
                'dorado_board_state_fabric'+('_serial' if variant=='snapshot' else ''):('33 (33)' if variant=='baseline' else '42 (42)'),
                'dorado_board_state_uio'+('_serial' if variant=='snapshot' else ''):('333 (333)' if variant=='baseline' else '342 (342)')}
            retained=[]
            for row in rows:
                wanted=expected_regs.pop(row['Entity Name'],None)
                assert wanted and row['Dedicated Logic Registers']==wanted,'transport register retention differs: '+row['Entity Name']
                assert row['Block Memory Bits']=='0';retained.append(row)
            assert not expected_regs
            # Shell prunes unused bus[35] launch/stage FFs in both cases;
            # baseline also ignores27 service/child ownership inputs (54FFs).
            # New UIO prunes only the five permanently-zero high status bits.
            timing={}
            assert {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}==CORNERS
            for corner in sorted(CORNERS):
                timing[corner]={}
                for mode in ('setup','hold','mpw'):
                    text=(case/f'bank-{mode}-{corner}.rpt').read_text(encoding='latin1')
                    hit=re.search(r'\((\d+) violated\)\.\s+Worst case slack is ([-\d.]+)',text)
                    assert hit and int(hit[1])==0 and float(hit[2])>=0,'timing violation: '+mode
                    timing[corner][mode]=float(hit[2])
                for mode in ('recovery','removal'):assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
                text=(case/f'bank-ucp-{corner}.rpt').read_text()
                for n in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    assert re.search('; '+n+r'\s*; 0\s*; 0\s*;',text),'unconstrained path: '+n
                assert re.search(r'; host_clock\s*; Base\s*; 20\.000\s*;',
                    (case/f'bank-clocks-{corner}.rpt').read_text()),'host clock differs'
            warnings={}
            for phase in ('map','fit'):
                text=(case/(phase+'.log')).read_text();assert 'Critical Warning' not in text
                warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
            results[variant]=dict(resources=resources,retention=retained,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        extra=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(extra)-{'10036'}),'unreviewed warning increase: '+str(extra)
    return dict(status='PASS_ISOLATED_FIT',scope=__doc__,results=results,
        delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k] for k in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values() for v in c.values()),
        limits='Prototype schema constant; isolated transport contract only, no native wrapper/ARM barrier, full-core capacity, physical snapshot or speed claim.')


def controls(root):
    cases=[('source','snapshot/board_state_fabric_serial.sv','RESPONSE_LIMIT=2048','RESPONSE_LIMIT=16','compiled input differs'),
        ('reset','snapshot/board_state_fabric_serial.sv','if(system_reset)begin','if(system_reset || transport_reset)begin','compiled input differs'),
        ('schema','snapshot/board_state_uio_serial.sv',"2:if(din!=16'd2)","2:if(din!=16'd1)",'compiled input differs'),
        ('constraint','snapshot/bank.sdc','20.000','18.000','compiled input differs'),
        ('policy','snapshot/snapshot_fit.qsf','OPTIMIZE_HOLD_TIMING ALL_PATHS','OPTIMIZE_HOLD_TIMING OFF','project/clock policy'),
        ('hold_state','snapshot/snapshot_fit.map.rpt','42 (42)','41 (41)','transport register retention'),
        ('status_state','snapshot/snapshot_fit.map.rpt','342 (342)','341 (341)','transport register retention'),
        ('hold_timing','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('clock','snapshot/top.sv','posedge host_clk','negedge host_clk','compiled input differs')]
    results=[]
    for name,path,before,after,why in cases:
        with tempfile.TemporaryDirectory(prefix='serial-transport-bad-',dir='/tmp')as tmp:
            case=Path(tmp)/'fit';shutil.copytree(root,case)
            p=case/path;text=p.read_text(encoding='latin1');assert before in text
            p.write_text(text.replace(before,after),encoding='latin1')
            try:audit(case)
            except AssertionError as error:
                assert why in str(error),(name,error)
                results.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged transport fit accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--negative-controls',action='store_true')
    a=p.parse_args();result=audit(a.directory.resolve())
    if a.negative_controls:result['failure_controls']=controls(a.directory.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('SERIAL TRANSPORT FIT AUDIT PASS',result['delta'],'minimum slack',result['minimum_snapshot_slack_ns'])
