#!/usr/bin/env python3
"""Independent MemC serial/parallel source, FF/RAM and all-corner fit audit."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_memc_ff_scan as fit
import fit_memc_full as reference_fit
import generate_memc_full as reference
import generate_memc_ff_scan as candidate
from test_memc_full import fixture
from audit_baseboard_full import table
from audit_enabled_core import ports
from audit_memc_full import normalized,CORNERS,ram_table


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
    assert seen==set(ff) and sum(r['width'] for r in ff.values())==507
    file_registers={};ram_registers={};parent=None
    ram_packages={r['package'] for r in manifest['rams']}
    file_packages={r['package'] for r in manifest['files']}
    for row in table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node'):
        node=row['Compilation Hierarchy Node']
        match=re.fullmatch(r'\|cell_MB7071H_snapshot:(u_\w+)\|',node)
        if match:
            package=match[1]
            assert package in ram_packages and package not in ram_registers
            # Only the private registered reply lives in logic here. All
            # physical state in these sockets is asynchronous RAM.
            assert row['Dedicated Logic Registers']=='4 (4)', 'async RAM reply retention differs'
            ram_registers[package]=row
        match=re.fullmatch(r'\|cell_F10145A_snapshot:(u_\w+)\|',node)
        if match:
            package=match[1]
            assert package in file_packages and package not in file_registers
            flag_owner='u_d06' if variant=='baseline' else 'u_l04'
            own=21 if package==flag_owner else 20
            # Both fits retain the same inferred MLAB input-register counts.
            # These descend from the private RAM ports, outside the exact
            # ten physical write-window + ten private reply registers.
            two_ports={'u_g02','u_g03','u_h02','u_h03','u_i02','u_i03','u_i14','u_k03','u_l04'}
            child=8 if package in two_ports else 4
            assert row['Dedicated Logic Registers']==f'{own+child} ({own})', 'file write-window retention differs'
            file_registers[package]=row
        elif node=='|MemC_full_snapshot_experiment:bank|':
            assert parent is None
            # Five physical phase bits plus private response state; private
            # reply padding is pruned, while each physical bit is retained.
            own=27 if variant=='baseline' else 21
            assert re.fullmatch(r'\d+ \('+str(own)+r'\)',row['Dedicated Logic Registers']), 'board phase retention differs'
            parent=row
    assert set(ram_registers)==ram_packages and set(file_registers)==file_packages and parent is not None
    without_files='\n'.join(line for line in fitted.split('\n') if 'cell_F10145A_snapshot:' not in line)
    rows=ram_table(without_files,'Fitter RAM Summary',manifest,'snapshot')
    for row in rows:
        sites=row['Location'].split(', ')
        assert len(sites)==8 and all(re.fullmatch(r'LAB_X\d+_Y\d+_N0',site) for site in sites), 'async RAM location differs'
        for port in ('A','B'):
            assert row['Implementation Port '+port+' Depth']=='256', 'async RAM implementation depth differs'
            assert row['Implementation Port '+port+' Width']=='4', 'async RAM implementation width differs'
        assert row['Port A Input Registers']=='yes', 'async RAM write latency differs'
    all_rows=table(fitted,'Fitter RAM Summary','Name')
    assert len(all_rows)==67, 'unexpected MemC RAM row count'
    files={r['package']:r for r in manifest['files']}
    file_rows=[r for r in all_rows if 'cell_F10145A_snapshot:' in r['Name']]
    observed=set()
    for row in file_rows:
        m=re.search(r'\|cell_F10145A_snapshot:(u_\w+)\|altdpram:(mem(?:__dual)?_rtl_0)\|',row['Name'])
        assert m and m[1] in files and m.groups() not in observed
        observed.add(m.groups())
        width=4 if m[2]=='mem_rtl_0' else sum(bool(v) for p,v in ports(board,m[1]) if p in ('p2','p1','p15','p14'))
        expected={'Type':'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
                  'Port A Depth':'16','Port B Depth':'16','Port A Width':str(width),'Port B Width':str(width),
                  'Implementation Port A Depth':'16','Implementation Port B Depth':'16',
                  'Implementation Port A Width':str(width),'Implementation Port B Width':str(width),
                  'Size':str(16*width),'Implementation Bits':str(16*width),'MLAB cells':str(width),'M10K blocks':'0',
                  'Port A Input Registers':'yes','Port A Output Registers':'no',
                  'Port B Input Registers':'no','Port B Output Registers':'no','MIF':'None'}
        for key,value in expected.items():
            assert row[key]==value, 'file geometry/latency differs: '+key
        assert re.fullmatch(r'LAB_X\d+_Y\d+_N0',row['Location']), 'file MLAB location differs'
    assert observed=={(p,r) for p in files for r in ('mem_rtl_0','mem__dual_rtl_0')}
    return dict(ff_packages=63,physical_ff_bits=507,physical_ram_rows=67,
                logical_ram_bits=22976,physical_replica_bits=21504+sum(int(r['Implementation Bits']) for r in file_rows),
                async_ram_memories=rows,async_ram_registers=ram_registers,
                file_memories=file_rows,file_registers=file_registers,board_phase=parent)


def audit(root,source_root):
    record=json.loads((root/'provenance.json').read_text())
    assert record['scan_driver_sha256']==fit.sha(Path(fit.__file__)), 'executed runner differs'
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):
            assert fit.sha(root/name)==digest, 'compiled input differs: '+name
    old,old_manifest,_=reference.generate('MemC',source_root/candidate.SOURCE)
    new,manifest,clones=candidate.generate(source_root)
    old_manifest=json.loads(json.dumps(old_manifest))
    manifest=json.loads(json.dumps(manifest))
    assert record['manifest']==manifest and record['reference_manifest']==old_manifest
    assert manifest['logical_state_bits']==23718 and manifest['ff_bits']==507
    assert len(manifest['ff'])==63 and len(manifest['files'])==23 and len(manifest['rams'])==21
    assert manifest['phase']['width']==5 and not manifest['immutable_proms']
    results={}
    with tempfile.TemporaryDirectory(prefix='MemC-scan-reference-',dir='/tmp') as temp:
        expected=Path(temp)/'inputs'
        reference_fit.prepare('MemC',source_root/candidate.SOURCE,expected)
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
            assert fit.sha(case/'MemC_original.v')==manifest['original_sha256']
            assert (case/'MemC_snapshot.sv').read_bytes()==board.encode(), 'board source differs'
            assert (case/'top.sv').read_text()==fit.shell(fixture(source_root/candidate.SOURCE,old_manifest)[2]['ports'],old_manifest,variant=='snapshot'), 'shell differs'
            for name,text in clones.items():
                if name!='ff_scan_controller.sv' or variant=='snapshot':
                    assert (case/name).read_bytes()==text.encode(), 'helper source differs: '+name
            for name,digest in manifest['original_sources'].items():
                assert fit.sha(case/(name+'.v'))==digest, 'ordinary original cell differs'
            for prom in manifest.get('immutable_proms',[]):
                assert fit.sha(case/prom['file'])==prom['sha256'], 'immutable MemC PROM differs'
            summary=(case/'snapshot_fit.fit.summary').read_text()
            assert 'Fitter Status : Successful' in summary
            resources={}
            for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),
                              ('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
                m=re.search(label+r' : ([\d,]+)',summary)
                assert m
                resources[key]=int(m[1].replace(',',''))
            assert resources['ram_bits']==0 and resources['ram_blocks']==0, 'enabled word RAM capacity differs'
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
                limits='Isolated MemC only; no full-core capacity/timing, global barrier, physical snapshot, clock/timer migration or speed qualification.')


def controls(root,source_root):
    result=[]
    for name,path,before,after,reason in [
        ('source','snapshot/MemC_snapshot.sv',' && !scan_busy','','compiled input differs'),
        ('constraint','snapshot/bank.sdc','20.000','18.000','compiled input differs'),
        ('policy','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0','policy'),
        ('ff_retention','snapshot/snapshot_fit.map.rpt','13 (13)','12 (12)','physical FF retention differs'),
        ('alu_retention','snapshot/snapshot_fit.map.rpt','4 (4)','3 (3)','physical FF retention differs'),
        ('file_retention','snapshot/snapshot_fit.map.rpt','28 (20)','27 (19)','file write-window retention differs'),
        ('phase_retention','snapshot/snapshot_fit.map.rpt',None,None,'board phase retention differs'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('ram_geometry','snapshot/snapshot_fit.fit.rpt','; MLAB ;','; M10K ;','RAM geometry differs'),
        ('memory_site','snapshot/snapshot_fit.fit.rpt','LAB_X','M10K_X','async RAM location differs'),
        ('ram_helper_source','snapshot/cell_MB7071H_snapshot.sv','state_hold ? state_data :',"state_hold ? 4'b0 :",'compiled input differs'),
    ]:
        with tempfile.TemporaryDirectory(prefix='MemC-scan-bad-',dir='/tmp') as temp:
            case=Path(temp)/'fit'
            shutil.copytree(root,case,ignore=shutil.ignore_patterns('db','incremental_db'))
            p=case/path
            text=p.read_text(encoding='latin1')
            if name=='alu_retention':
                lines=text.splitlines(keepends=True)
                for i,line in enumerate(lines):
                    if line.startswith('; ') and line.split(';')[1].strip().startswith('|cell_F100181_snapshot:'):
                        assert before in line
                        lines[i]=line.replace(before,after,1)
                        break
                else:
                    raise AssertionError('ALU failure-control row absent')
                text=''.join(lines)
            elif name=='phase_retention':
                lines=text.splitlines(keepends=True)
                for i,line in enumerate(lines):
                    if line.startswith('; ') and line.split(';')[1].strip()=='|MemC_full_snapshot_experiment:bank|':
                        assert '(21)' in line
                        lines[i]=line.replace('(21)','(20)',1)
                        break
                else:
                    raise AssertionError('phase failure-control row absent')
                text=''.join(lines)
            else:
                assert before in text, 'failure-control target absent: '+name
                text=text.replace(before,after)
            p.write_text(text,encoding='latin1')
            try:
                audit(case,source_root)
            except AssertionError as e:
                assert reason in str(e), 'unexpected rejection: '+str(e)
                result.append(dict(case=name,rejected=True,diagnostic=str(e),expected_stage=reason))
            else:
                raise AssertionError('damaged MemC scan evidence accepted: '+name)
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
    print('MemC SERIAL FIT PASS:delta',result['delta'],'minimum',result['minimum_snapshot_slack_ns'],'ns;11bad controls reject')
