#!/usr/bin/env python3
"""Independent ContB serial/parallel source, FF/IM retention and timing audit."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_contb_ff_scan as fit
import fit_contb_full as reference_fit
import test_contb_full as reference_test
import generate_contb_ff_scan as gen
from audit_baseboard_full import table
from audit_f10415_enable import CORNERS


def normalized(text):
    return '\n'.join(line for line in text.splitlines() if line.strip() and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def physical(mapping,fitted,manifest,variant):
    ff={'u_'+r['package']:r for r in manifest['ff']};seen=set();banks={};parent=None
    for row in table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node'):
        node=row['Compilation Hierarchy Node']
        match=re.fullmatch(r'\|(cell_\w+_snapshot):(u_\w+)\|',node)
        if match:
            assert match[2] in ff and match[2] not in seen
            entry=ff[match[2]];seen.add(match[2])
            assert match[1]==entry['kind']+'_snapshot'
            assert row['Dedicated Logic Registers']==f"{entry['width']} ({entry['width']})", 'physical FF retention differs'
            assert row['Block Memory Bits']=='0'
        match=re.fullmatch(r'\|dorado_snapshot_f10415_enable_bank:snapshot_bank_(\d+)\|',node)
        if match:
            bank=int(match[1]);assert bank<8 and bank not in banks
            # 18 canonical saved_q bits and two private reply flags per bank.
            # Three identical port flags are shared; exact ownership is checked.
            shared=({0:3} if variant=='baseline' else {0:1,7:2}).get(bank,0)
            own=20+shared
            assert row['Dedicated Logic Registers']==f'{own} ({own})', 'IM pipeline retention differs'
            assert row['Block Memory Bits']=='18432'
            banks[bank]=row
        if node=='|ContB_full_snapshot_experiment:board|':
            assert parent is None
            # Invalid auxiliary done/error become the same private flag in
            # the serial adapter, so synthesis shares that register.
            own=19 if variant=='baseline' else 7
            assert re.fullmatch(r'\d+ \('+str(own)+r'\)',row['Dedicated Logic Registers']), 'board response/reset retention differs'
            parent=row
    assert seen==set(ff) and len(banks)==8 and parent is not None
    rows=table(fitted,'Fitter RAM Summary','Name');assert len(rows)==8, 'unexpected IM memory count'
    seen=set();locations=set()
    for row in rows:
        match=re.search(r'\|dorado_snapshot_f10415_enable_bank:snapshot_bank_(\d+)\|',row['Name'])
        assert match and int(match[1])<8 and match[1] not in seen
        seen.add(match[1])
        expected={'Type':'AUTO','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
            'Port A Depth':'1024','Port B Depth':'1024','Port A Width':'18','Port B Width':'18',
            'Implementation Port A Depth':'1024','Implementation Port B Depth':'1024',
            'Implementation Port A Width':'18','Implementation Port B Width':'18',
            'Size':'18432','Implementation Bits':'18432','M10K blocks':'2','MLAB cells':'0',
            'Port A Input Registers':'yes','Port B Input Registers':'yes',
            'Port A Output Registers':'no','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}
        for key,value in expected.items():assert row[key]==value, 'IM geometry/latency differs: '+key
        sites=row['Location'].split(', ')
        assert len(sites)==2 and len(set(sites))==2 and not (set(sites)&locations), 'IM RAM shares/misses a block'
        assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',site) for site in sites), 'IM RAM not M10K'
        locations.update(sites)
    assert len(locations)==16
    for parameter,value in [('OPERATION_MODE','DUAL_PORT'),('WIDTH_A','18'),('WIDTH_B','18'),
                            ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
        assert re.findall(r'; '+parameter+r'\s*; (\w+)\s*;',mapping)==[value]*8, 'IM macro differs: '+parameter
    return dict(physical_ff_bits=209,physical_pipeline_bits=144,physical_ram_bits=147456,
                physical_ram_blocks=16,physical_memories=rows,pipeline_registers=banks,board_private_state=parent)


def audit(root,source_root):
    record=json.loads((root/'provenance.json').read_text())
    assert record['scan_driver_sha256']==fit.sha(Path(fit.__file__)), 'executed runner differs'
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert fit.sha(root/name)==digest, 'compiled input differs: '+name
    old,old_manifest,_=gen.reference(source_root);new,manifest,clones=gen.generate(source_root)
    assert record['manifest']==manifest and record['reference_manifest']==old_manifest
    results={}
    with tempfile.TemporaryDirectory(prefix='contb-scan-reference-',dir='/tmp') as temp:
        expected=Path(temp)/'inputs'
        with gen.source_context(source_root):
            reference_fit.prepare(expected)
            ports=reference_test.fixture(old_manifest)[2]
        for variant,board in [('baseline',old),('snapshot',new)]:
            case=root/variant
            qsf=(expected/variant/'snapshot_fit.qsf').read_text()
            if variant=='baseline':qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
            else:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
            for port in ('system_reset','scan_hold_required','scan_fault'):qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
            assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(qsf), 'project policy differs'
            assert (case/'top.sv').read_text()==fit.shell(ports,variant=='snapshot'), 'clock/shell differs'
            for name in ('bank.sdc','audit.tcl'):assert (case/name).read_bytes()==(expected/variant/name).read_bytes(), 'timing policy differs'
            assert fit.sha(case/Path(gen.SOURCE).name)==gen.BOARD_SHA
            assert (case/'ContB_snapshot.sv').read_bytes()==board.encode()
            for name,text in clones.items():
                if name!='ff_scan_controller.sv' or variant=='snapshot':assert (case/name).read_bytes()==text.encode(), 'helper differs: '+name
            for name,digest in manifest['grouping_sources'].items():assert fit.sha(case/Path(name).name)==digest, 'original cell differs'
            summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful' in summary
            resources={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                              ('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                match=re.search(label+r' : ([\d,]+)',summary);assert match
                resources[key]=int(match[1].replace(',',''))
            assert resources['ram_bits']==147456 and resources['ram_blocks']==16
            state=physical((case/'snapshot_fit.map.rpt').read_text(encoding='latin1'),
                           (case/'snapshot_fit.fit.rpt').read_text(encoding='latin1'),manifest,variant)
            assert {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')}==CORNERS
            timing={}
            for corner in sorted(CORNERS):
                timing[corner]={}
                for mode in ('setup','hold','mpw'):
                    text=(case/f'bank-{mode}-{corner}.rpt').read_text(encoding='latin1')
                    match=re.search(r'\((\d+) violated\)\.\s+Worst case slack is ([-\d.]+)',text)
                    assert match and int(match[1])==0 and float(match[2])>=0, 'timing violation: '+mode
                    timing[corner][mode]=float(match[2])
                for mode in ('recovery','removal'):assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
                text=(case/f'bank-ucp-{corner}.rpt').read_text()
                for name in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    assert re.search('; '+name+r'\s*; 0\s*; 0\s*;',text), 'unconstrained coverage: '+name
                assert re.search(r'; host_clock\s*; Base\s*; 20\.000\s*;',
                                 (case/f'bank-clocks-{corner}.rpt').read_text()), 'host clock differs'
            warnings={}
            for phase in ('map','fit'):
                text=(case/(phase+'.log')).read_text();assert 'Critical Warning' not in text
                warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
            results[variant]=dict(resources=resources,physical=state,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        extra=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not (set(extra)-{'10036'}), 'unreviewed warning increase: '+str(extra)
    return dict(status='PASS_ISOLATED_FIT',scope=__doc__,results=results,
        delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k] for k in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values() for v in c.values()),
        limits='Isolated ContB only; no whole-core capacity/timing, global hold, physical machine snapshot, clock migration or speed claim.')


def controls(root,source_root):
    results=[]
    cases=[('source','snapshot/ContB_snapshot.sv',' && !scan_busy','','compiled input differs'),
        ('constraint','snapshot/bank.sdc','20.000','18.000','compiled input differs'),
        ('policy','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0','policy'),
        ('ff_retention','snapshot/snapshot_fit.map.rpt','13 (13)','12 (12)','physical FF retention differs'),
        ('pipeline_retention','snapshot/snapshot_fit.map.rpt','20 (20)','19 (19)','IM pipeline retention differs'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('old_data_macro','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA','IM macro differs'),
        ('old_data_physical','snapshot/snapshot_fit.fit.rpt','; Old data','; New data','IM geometry/latency differs'),
        ('memory_site','snapshot/snapshot_fit.fit.rpt','M10K_X','MLAB_X','IM RAM not M10K'),
        ('clock','snapshot/top.sv','.ena(run_r)','.ena(run)','compiled input differs')]
    for name,path,before,after,reason in cases:
        with tempfile.TemporaryDirectory(prefix='contb-scan-bad-',dir='/tmp') as temp:
            case=Path(temp)/'fit';shutil.copytree(root,case,ignore=shutil.ignore_patterns('db','incremental_db'))
            p=case/path;text=p.read_text(encoding='latin1');assert before in text
            p.write_text(text.replace(before,after),encoding='latin1')
            try:audit(case,source_root)
            except AssertionError as error:
                assert reason in str(error), 'unexpected rejection: '+str(error)
                results.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged ContB fit accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();result=audit(a.directory.resolve(),a.source_root.resolve())
    result['failure_controls']=controls(a.directory.resolve(),a.source_root.resolve());result['tool_sha256']=fit.sha(Path(__file__))
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('CONTB SERIAL FIT PASS:delta',result['delta'],'minimum',result['minimum_snapshot_slack_ns'],'ns;10bad controls reject')
