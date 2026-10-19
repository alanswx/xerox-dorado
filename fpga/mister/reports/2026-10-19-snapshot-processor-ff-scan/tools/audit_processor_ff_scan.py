#!/usr/bin/env python3
"""Independent source, physical FF/RAM and timing audit for serial processor fits."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_processor_ff_scan as fit
import fit_processor_full as reference_fit
import generate_processor_full as reference
import generate_processor_ff_scan as candidate
from test_processor_full import fixture
from audit_baseboard_full import table
from audit_enabled_core import ports
from audit_processor_full import normalized_qsf, CORNERS


def physical(mapping,fitted,manifest,board):
    expected = {r['package']:r for r in manifest['ff']}
    found = set()
    entities = table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node')
    for row in entities:
        match = re.fullmatch(r'\|(cell_\w+_snapshot):(u_\w+)\|',row['Compilation Hierarchy Node'])
        if not match or match[2] not in expected:
            continue
        field = expected[match[2]]
        assert match[1] == field['kind']+'_snapshot' and match[2] not in found
        found.add(match[2])
        assert row['Dedicated Logic Registers'] == f"{field['width']} ({field['width']})", 'physical FF retention differs'
        assert row['Block Memory Bits'] == '0'
    assert found == set(expected), 'missing physical FF helper'
    files = {r['package']:r for r in manifest['files']}
    rams = {r['package']:r for r in manifest['rams']}
    rows = table(fitted,'Fitter RAM Summary','Name')
    assert len(rows) == 2*len(files)+len(rams), 'unexpected RAM count'
    seen = set()
    for row in rows:
        small = re.search(r'\|cell_F10145A_snapshot:(u_\w+)\|altdpram:(mem(?:__dual)?_rtl_0)\|',row['Name'])
        if small:
            package,replica = small.groups()
            assert package in files
            key = (package,replica)
            width = 4 if replica=='mem_rtl_0' else sum(bool(v) for p,v in ports(board,package) if p in ('p2','p1','p15','p14'))
            depth,cells = 16,width
        else:
            match = re.search(r'\|(cell_MB7071H|cell_F10414)_snapshot:(u_\w+)\|',row['Name'])
            assert match and match[2] in rams, 'unexpected RAM hierarchy'
            package = match[2]
            assert match[1] == rams[package]['kind']
            key = (package,'ordinary_and_admin')
            width,depth = rams[package]['width'],256
            cells = 8*width
        assert key not in seen, 'duplicate RAM replica'
        seen.add(key)
        values = {'Type':'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
                  'Port A Depth':str(depth),'Port B Depth':str(depth),'Port A Width':str(width),'Port B Width':str(width),
                  'Implementation Port A Depth':str(depth),'Implementation Port B Depth':str(depth),
                  'Implementation Port A Width':str(width),'Implementation Port B Width':str(width),
                  'Size':str(depth*width),'Implementation Bits':str(depth*width),'M10K blocks':'0','MLAB cells':str(cells),
                  'Port A Input Registers':'yes','Port A Output Registers':'no',
                  'Port B Input Registers':'no','Port B Output Registers':'no','MIF':'None'}
        for key_name,value in values.items():
            assert row[key_name] == value, 'RAM geometry/latency differs: '+key_name
        sites = [s.strip() for s in row['Location'].split(',')]
        assert sites and all(re.fullmatch(r'LAB_X\d+_Y\d+_N0',s) for s in sites), 'unplaced/invalid MLAB'
    assert seen == {(p,r) for p in files for r in ('mem_rtl_0','mem__dual_rtl_0')} | {(p,'ordinary_and_admin') for p in rams}
    return dict(ff_packages=len(found),physical_ff_bits=sum(r['width'] for r in expected.values()),
                logical_small_ram_bits=64*len(files),logical_large_ram_bits=sum(r['width']*256 for r in rams.values()),
                ram_rows=len(rows),physical_replica_bits=sum(int(r['Implementation Bits']) for r in rows),
                limit='Exact FF-helper synthesis counts and all physical RAM geometries/latencies checked. Other physical state/transport coverage is independently classified and restored; private register merging is allowed.')


def audit(root):
    record = json.loads((root/'provenance.json').read_text())
    kind = record['kind']
    assert record['scan_driver_sha256'] == fit.sha(Path(fit.__file__)), 'executed driver differs'
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):
            assert fit.sha(root/name) == digest, 'compiled input differs: '+name
    source = root/'baseline/Processor_original.v'
    old,old_manifest,_ = reference.generate(kind,source)
    new,manifest,clones = candidate.generate(kind,source)
    assert manifest == record['manifest'] and old_manifest == record['reference_manifest']
    assert fit.sha(source) == record['source_board_sha256']
    results = {}
    with tempfile.TemporaryDirectory(prefix='processor-scan-reference-',dir='/tmp') as temp:
        expected = Path(temp)/'inputs'
        reference_fit.prepare(kind,source,expected)
        for variant,board in [('baseline',old),('snapshot',new)]:
            case = root/variant
            qsf = (expected/variant/'snapshot_fit.qsf').read_text()
            if variant == 'baseline':
                qsf += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
            else:
                qsf += 'set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
            for port in ('system_reset','scan_hold_required','scan_fault'):
                qsf += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
            assert normalized_qsf((case/'snapshot_fit.qsf').read_text()) == normalized_qsf(qsf), 'project policy differs'
            assert (case/'bank.sdc').read_bytes() == (expected/variant/'bank.sdc').read_bytes(), 'timing constraint differs'
            assert (case/'audit.tcl').read_bytes() == (expected/variant/'audit.tcl').read_bytes(), 'corner audit differs'
            assert (case/'Processor_snapshot.sv').read_bytes() == board.encode(), 'board source differs'
            assert (case/'top.sv').read_text() == fit.shell(fixture(source,old_manifest)[2]['ports'],old_manifest,variant=='snapshot'), 'shell differs'
            for name,text in clones.items():
                if name != 'ff_scan_controller.sv' or variant == 'snapshot':
                    assert (case/name).read_bytes() == text.encode(), 'helper source differs: '+name
            for cell in manifest['original_sources']:
                assert (case/(cell+'.v')).read_bytes() == (root/'baseline'/(cell+'.v')).read_bytes(), 'ordinary cell differs'
            for prom in manifest['immutable_proms']:
                assert fit.sha(case/prom['file']) == prom['sha256'], 'immutable PROM differs'
            summary = (case/'snapshot_fit.fit.summary').read_text()
            assert 'Fitter Status : Successful' in summary
            resources = {}
            for name,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks')]:
                match = re.search(label+r' : ([\d,]+)',summary)
                assert match
                resources[name] = int(match[1].replace(',',''))
            mapping = (case/'snapshot_fit.map.rpt').read_text(encoding='latin1')
            fitted = (case/'snapshot_fit.fit.rpt').read_text(encoding='latin1')
            state = physical(mapping,fitted,manifest,board)
            assert state['physical_ff_bits'] == manifest['ff_bits']
            assert {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')} == CORNERS
            timing = {}
            for corner in sorted(CORNERS):
                timing[corner] = {}
                for mode in ('setup','hold','mpw'):
                    text = (case/f'bank-{mode}-{corner}.rpt').read_text(encoding='latin1')
                    match = re.search(r'\((\d+) violated\)\.\s+Worst case slack is ([-\d.]+)',text)
                    assert match and int(match[1]) == 0 and float(match[2]) >= 0, 'timing violation: '+mode
                    timing[corner][mode] = float(match[2])
                for mode in ('recovery','removal'):
                    assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
                text = (case/f'bank-ucp-{corner}.rpt').read_text()
                for name in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    assert re.search('; '+name+r'\s*; 0\s*; 0\s*;',text), 'unconstrained coverage: '+name
            warnings = {}
            for phase in ('map','fit'):
                text = (case/(phase+'.log')).read_text()
                assert 'Critical Warning' not in text
                warnings[phase] = dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
            results[variant] = dict(resources=resources,physical=state,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        increased = Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not (set(increased)-{'10036'}), 'unreviewed '+phase+' warning increase: '+str(increased)
    return dict(status='PASS_ISOLATED_FIT',scope=__doc__,kind=kind,results=results,
                delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k] for k in ('alms','registers','ram_blocks')},
                minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values() for v in c.values()),
                limits='No full-core resource/timing, transport timeout/global barrier, physical snapshot, original-firmware boot or speed qualification.')


def controls(root):
    result = []
    for name,path,before,after,reason in [
        ('source','snapshot/Processor_snapshot.sv',' && !scan_busy','','compiled input differs'),
        ('constraint','snapshot/bank.sdc','20.000','18.000','compiled input differs'),
        ('policy','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0','policy'),
        ('ff_retention','snapshot/snapshot_fit.map.rpt','13 (13)','12 (12)','physical FF retention differs'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).','timing violation'),
        ('ram_latency','snapshot/snapshot_fit.fit.rpt','; no                     ; no                      ;','; yes                    ; no                      ;','RAM geometry/latency differs'),
    ]:
        with tempfile.TemporaryDirectory(prefix='processor-scan-bad-',dir='/tmp') as temp:
            case = Path(temp)/'fit'
            shutil.copytree(root,case,ignore=shutil.ignore_patterns('db','incremental_db'))
            p = case/path
            text = p.read_text(encoding='latin1')
            assert before in text, 'failure-control target absent: '+name
            p.write_text(text.replace(before,after),encoding='latin1')
            try:
                audit(case)
            except AssertionError as e:
                assert reason in str(e), 'unexpected failure-control rejection: '+str(e)
                result.append(dict(case=name,rejected=True,diagnostic=str(e),expected_stage=reason))
            else:
                raise AssertionError('damaged processor evidence accepted: '+name)
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    result = audit(a.directory.resolve())
    result['failure_controls'] = controls(a.directory.resolve())
    result['tool_sha256'] = fit.sha(Path(__file__))
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(result['kind'],'SERIAL FIT PASS:delta',result['delta'],'minimum',result['minimum_snapshot_slack_ns'],'ns;6bad controls reject')
