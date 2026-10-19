#!/usr/bin/env python3
"""Independent IFU serial/parallel source, FF/RAM and all-corner fit audit."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_ifu_ff_scan as fit
import fit_ifu_full as reference_fit
import generate_ifu_full as reference
import generate_ifu_ff_scan as candidate
from test_ifu_full import fixture
from audit_baseboard_full import table
from audit_ifu_full import normalized,CORNERS,physical_ram,memories


def physical(mapping,fitted,manifest,board,variant):
    ff = {r['package']:r for r in manifest['ff']}
    seen = set()
    for row in table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node'):
        m = re.fullmatch(r'\|(cell_\w+_snapshot):(u_\w+)\|',row['Compilation Hierarchy Node'])
        if not m or m[2] not in ff:
            continue
        r = ff[m[2]]
        assert m[1]==r['kind']+'_snapshot' and m[2] not in seen
        seen.add(m[2])
        assert row['Dedicated Logic Registers']==f"{r['width']} ({r['width']})", 'physical FF retention differs'
        assert row['Block Memory Bits']=='0'
    assert seen==set(ff) and sum(r['width'] for r in ff.values())==974
    bank_registers={}
    for row in table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node'):
        match=re.fullmatch(r'\|dorado_snapshot_f10415_word_bank:snapshot_bank_(\d+)\|',row['Compilation Hierarchy Node'])
        if not match:
            continue
        bank=int(match[1])
        assert bank<11 and bank not in bank_registers
        entry=manifest['banks'][bank]
        # Each saved_q pipeline remains dedicated. Synthesis shares three
        # identical port flags: all at bank 0 in parallel, and one/two at
        # banks 0/10 in serial. The flag total and every pipeline width stay.
        shared=({0:3} if variant=='baseline' else {0:1,10:2}).get(bank,0)
        count=entry['width']+shared
        assert row['Dedicated Logic Registers']==f'{count} ({count})', 'IFUM pipeline retention differs'
        assert row['Block Memory Bits']==str(1024*entry['width'])
        bank_registers[bank]=row
    assert set(bank_registers)==set(range(11))
    rows=physical_ram(fitted,manifest)
    all_rows=table(fitted,'Fitter RAM Summary','Name')
    assert len(all_rows)==11, 'unexpected IFU RAM row count'
    locations=set()
    for row in rows:
        assert re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',row['Location']), 'IFUM RAM not M10K'
        assert row['Location'] not in locations, 'independently addressed IFUM groups share a block'
        locations.add(row['Location'])
        for port in ('A','B'):
            assert row['Implementation Port '+port+' Depth']=='1024', 'IFUM implementation depth differs'
            assert row['Implementation Port '+port+' Width']==row['Port '+port+' Width'], 'IFUM implementation width differs'
    assert sum(int(r['Implementation Bits']) for r in rows)==27648
    return dict(ff_packages=94,physical_ff_bits=974,physical_ram_rows=11,
                logical_ram_bits=27648,physical_replica_bits=27648,word_m10ks=11,
                word_memories=rows,word_macros=memories(mapping,manifest),pipeline_registers=bank_registers)


def audit(root,source_root):
    record=json.loads((root/'provenance.json').read_text())
    assert record['scan_driver_sha256']==fit.sha(Path(fit.__file__)), 'executed runner differs'
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):
            assert fit.sha(root/name)==digest, 'compiled input differs: '+name
    old,old_manifest,_=reference.generate(source_root)
    new,manifest,clones=candidate.generate(source_root)
    old_manifest=json.loads(json.dumps(old_manifest))
    manifest=json.loads(json.dumps(manifest))
    assert record['manifest']==manifest and record['reference_manifest']==old_manifest
    assert manifest['logical_state_bits']==28653 and manifest['ff_bits']==974
    assert [r['width'] for r in manifest['banks']]==[1,5,1,1,4,1,5,2,1,4,2]
    assert all(r['ce']=='OMITTED' for r in manifest['banks'])
    results={}
    with tempfile.TemporaryDirectory(prefix='IFU-scan-reference-',dir='/tmp') as temp:
        expected=Path(temp)/'inputs'
        reference_fit.prepare(source_root,expected)
        for variant,board in [('baseline',old),('snapshot',new)]:
            case=root/variant
            qsf=(expected/variant/'snapshot_fit.qsf').read_text()
            if variant=='baseline':
                qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
            else:
                qsf+='set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
            for port in ('system_reset','scan_hold_required','scan_fault'):
                qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
            assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(qsf), 'project policy differs'
            assert (case/'bank.sdc').read_bytes()==(expected/variant/'bank.sdc').read_bytes(), 'timing constraints differ'
            assert (case/'audit.tcl').read_bytes()==(expected/variant/'audit.tcl').read_bytes(), 'corner script differs'
            assert fit.sha(case/'IFU_original.v')==manifest['original_sha256']
            assert (case/'IFU_snapshot.sv').read_bytes()==board.encode(), 'board source differs'
            assert (case/'top.sv').read_text()==fit.shell(fixture(source_root,old_manifest)[2]['ports'],old_manifest,variant=='snapshot'), 'shell differs'
            for name,text in clones.items():
                if name!='ff_scan_controller.sv' or variant=='snapshot':
                    assert (case/name).read_bytes()==text.encode(), 'helper source differs: '+name
            for name,digest in manifest['original_sources'].items():
                assert fit.sha(case/Path(name).name)==digest, 'ordinary original cell differs'
            for prom in manifest['immutable_proms']:
                assert fit.sha(case/prom['file'])==prom['sha256'], 'immutable IFU PROM differs'
            summary=(case/'snapshot_fit.fit.summary').read_text()
            assert 'Fitter Status : Successful' in summary
            resources={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                              ('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                m=re.search(label+r' : ([\d,]+)',summary)
                assert m
                resources[key]=int(m[1].replace(',',''))
            assert resources['ram_bits']==27648 and resources['ram_blocks']==11, 'enabled word RAM capacity differs'
            mapping=(case/'snapshot_fit.map.rpt').read_text(encoding='latin1')
            fitted=(case/'snapshot_fit.fit.rpt').read_text(encoding='latin1')
            state=physical(mapping,fitted,manifest,board,variant)
            assert {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}==CORNERS
            timing={}
            for corner in sorted(CORNERS):
                timing[corner]={}
                for mode in ('setup','hold','mpw'):
                    text=(case/f'bank-{mode}-{corner}.rpt').read_text(encoding='latin1')
                    m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is ([-\d.]+)',text)
                    assert m and int(m[1])==0 and float(m[2])>=0, 'timing violation: '+mode
                    timing[corner][mode]=float(m[2])
                for mode in ('recovery','removal'):
                    assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
                text=(case/f'bank-ucp-{corner}.rpt').read_text()
                for name in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    assert re.search('; '+name+r'\s*; 0\s*; 0\s*;',text), 'unconstrained coverage: '+name
            warnings={}
            for phase in ('map','fit'):
                text=(case/(phase+'.log')).read_text()
                assert 'Critical Warning' not in text
                warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
            results[variant]=dict(resources=resources,physical=state,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        extra=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(extra)-{'10036'}), 'unreviewed '+phase+' warning increase: '+str(extra)
    return dict(status='PASS_ISOLATED_FIT',scope=__doc__,results=results,
                delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k] for k in ('alms','registers','ram_bits','ram_blocks')},
                minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values() for v in c.values()),
                limits='Isolated IFU only; no full-core capacity/timing, global barrier, physical snapshot, clock/timer migration or speed qualification.')


def controls(root,source_root):
    result=[]
    for name,path,before,after,reason in [
        ('source','snapshot/IFU_snapshot.sv',' && !scan_busy','','compiled input differs'),
        ('constraint','snapshot/bank.sdc','20.000','18.000','compiled input differs'),
        ('policy','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0','policy'),
        ('ff_retention','snapshot/snapshot_fit.map.rpt','13 (13)','12 (12)','physical FF retention differs'),
        ('pipeline_retention','snapshot/snapshot_fit.map.rpt','4 (4)','3 (3)','IFUM pipeline retention differs'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('old_data_macro','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA','IFUM macro differs'),
        ('old_data_physical','snapshot/snapshot_fit.fit.rpt','; Old data','; New data','physical IFUM RAM differs'),
        ('memory_site','snapshot/snapshot_fit.fit.rpt','M10K_X','MLAB_X','IFUM RAM not M10K'),
    ]:
        with tempfile.TemporaryDirectory(prefix='IFU-scan-bad-',dir='/tmp') as temp:
            case=Path(temp)/'fit'
            shutil.copytree(root,case,ignore=shutil.ignore_patterns('db','incremental_db'))
            p=case/path
            text=p.read_text(encoding='latin1')
            assert before in text, 'failure-control target absent: '+name
            if name=='pipeline_retention':
                # MC10173 FFs are also four bits. Damage only a RAM pipeline
                # row so the control exercises the intended retention check.
                token='|dorado_snapshot_f10415_word_bank:snapshot_bank_4|'
                lines=text.splitlines(keepends=True)
                for i,line in enumerate(lines):
                    if line.startswith('; ') and token in line and before in line:
                        lines[i]=line.replace(before,after,1)
                        break
                else:
                    raise AssertionError('pipeline failure-control row absent')
                text=''.join(lines)
            else:
                text=text.replace(before,after)
            p.write_text(text,encoding='latin1')
            try:
                audit(case,source_root)
            except AssertionError as e:
                assert reason in str(e), 'unexpected rejection: '+str(e)
                result.append(dict(case=name,rejected=True,diagnostic=str(e),expected_stage=reason))
            else:
                raise AssertionError('damaged IFU scan evidence accepted: '+name)
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory',type=Path)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    result=audit(a.directory.resolve(),a.source_root.resolve())
    result['failure_controls']=controls(a.directory.resolve(),a.source_root.resolve())
    result['tool_sha256']=fit.sha(Path(__file__))
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('IFU SERIAL FIT PASS:delta',result['delta'],'minimum',result['minimum_snapshot_slack_ns'],'ns;9bad controls reject')
