#!/usr/bin/env python3
"""Audit matched complete-state ContA scan fits; not full-core qualification."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_conta_ff_scan as fit
import fit_conta_full as reference_fit
import generate_conta_full as reference
import generate_conta_ff_scan as candidate
from audit_baseboard_full import table
from audit_enabled_core import ports
from audit_f10415_enable import CORNERS


def normalized(text):
    return '\n'.join(x for x in text.splitlines() if x.strip() and
                     x != 'set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def physical(mapping, fitted, manifest, board):
    entities = table(mapping,'Analysis & Synthesis Resource Utilization by Entity','Compilation Hierarchy Node')
    ff = {r['package']:r for r in manifest['ff']}
    observed = set()
    for row in entities:
        match = re.fullmatch(r'\|(cell_\w+_snapshot):(u_\w+)\|',row['Compilation Hierarchy Node'])
        if not match or match[2] not in ff:
            continue
        r = ff[match[2]]
        assert match[1] == r['kind']+'_snapshot' and r['package'] not in observed
        observed.add(r['package'])
        assert row['Dedicated Logic Registers'] == f"{r['width']} ({r['width']})", 'physical FF bits differ'
        assert row['Block Memory Bits'] == '0'
    assert observed == set(ff) and sum(r['width'] for r in ff.values()) == 681
    expected = {r['package']:r for r in manifest['files']}
    memories = table(fitted,'Fitter RAM Summary','Name')
    assert len(memories) == 18, 'RAM replica count differs'
    seen = set()
    for row in memories:
        m = re.search(r'\|cell_F10145A_snapshot:(u_\w+)\|altdpram:(mem(?:__dual)?_rtl_0)\|',row['Name'])
        assert m and m[1] in expected and m.groups() not in seen
        seen.add(m.groups())
        width = 4 if m[2] == 'mem_rtl_0' else sum(bool(v) for p,v in ports(board,m[1]) if p in ('p2','p1','p15','p14'))
        wanted = {'Type':'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
                  'Port A Depth':'16','Port B Depth':'16','Port A Width':str(width),'Port B Width':str(width),
                  'Implementation Port A Depth':'16','Implementation Port B Depth':'16',
                  'Implementation Port A Width':str(width),'Implementation Port B Width':str(width),
                  'Size':str(16*width),'Implementation Bits':str(16*width),'M10K blocks':'0','MLAB cells':str(width),
                  'Port A Input Registers':'yes','Port A Output Registers':'no',
                  'Port B Input Registers':'no','Port B Output Registers':'no','MIF':'None'}
        for key,value in wanted.items():
            assert row[key] == value, 'RAM geometry/latency differs: '+key
        assert re.fullmatch(r'LAB_X\d+_Y\d+_N0',row['Location'])
    assert seen == {(p,r) for p in expected for r in ('mem_rtl_0','mem__dual_rtl_0')}
    return dict(ff_packages=75,physical_ff_bits=681,logical_ram_bits=576,
                physical_replica_bits=sum(int(r['Implementation Bits']) for r in memories),ram_replicas=18,
                limit='FF helper synthesis counts and RAM placement checked. Other state access/coverage is separately tested/classified; no distinct-register requirement for private transport aliases.')


def audit(root):
    record = json.loads((root/'provenance.json').read_text())
    assert record['scan_driver_sha256'] == fit.sha(Path(fit.__file__)), 'executed builder differs'
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):
            assert fit.sha(root/name) == digest, 'compiled input differs: '+name
    source = root/'baseline/ContA_original.v'
    old, old_manifest, _ = reference.generate(source)
    new, manifest, clones = candidate.generate(source,async_files=True)
    assert manifest == record['manifest'] and old_manifest == record['reference_manifest']
    assert fit.sha(source) == record['source_board_sha256']
    results = {}
    with tempfile.TemporaryDirectory(prefix='conta-scan-reference-',dir='/tmp') as temp:
        prepared = Path(temp)/'inputs'
        reference_fit.prepare(source,prepared)
        for variant,board in [('baseline',old),('snapshot',new)]:
            case = root/variant
            qsf = (prepared/variant/'snapshot_fit.qsf').read_text()
            if variant == 'baseline':
                qsf += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
            else:
                qsf += 'set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
            for port in ('system_reset','scan_hold_required','scan_fault'):
                qsf += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
            assert normalized((case/'snapshot_fit.qsf').read_text()) == normalized(qsf), 'project policy differs'
            assert (case/'bank.sdc').read_bytes() == (prepared/variant/'bank.sdc').read_bytes(), 'constraint differs'
            assert (case/'audit.tcl').read_bytes() == (prepared/variant/'audit.tcl').read_bytes(), 'corner script differs'
            assert (case/'ContA_snapshot.sv').read_bytes() == board.encode(), 'board source differs'
            assert (case/'top.sv').read_text() == fit.shell(
                __import__('test_conta_full').fixture(source,old_manifest)[2]['ports'],manifest['module'],variant=='snapshot'), 'shell differs'
            for name,text in clones.items():
                if name != 'ff_scan_controller.sv' or variant == 'snapshot':
                    assert (case/name).read_bytes() == text.encode(), 'helper source differs: '+name
            for kind in old_manifest['original_sources']:
                assert (case/(kind+'.v')).read_bytes() == (root/'baseline'/(kind+'.v')).read_bytes(), 'ordinary cell differs'
            summary = (case/'snapshot_fit.fit.summary').read_text()
            assert 'Fitter Status : Successful' in summary
            resources = {}
            for name,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks')]:
                match = re.search(label+r' : ([\d,]+)',summary)
                assert match
                resources[name] = int(match[1].replace(',',''))
            mapping = (case/'snapshot_fit.map.rpt').read_text(encoding='latin1')
            fitted = (case/'snapshot_fit.fit.rpt').read_text(encoding='latin1')
            memory = physical(mapping,fitted,manifest,board)
            timing = {}
            assert {p.name[len('bank-setup-'):-4] for p in case.glob('bank-setup-*.rpt')} == CORNERS
            for corner in sorted(CORNERS):
                timing[corner] = {}
                for mode in ('setup','hold','mpw'):
                    text = (case/f'bank-{mode}-{corner}.rpt').read_text(encoding='latin1')
                    match = re.search(r'\((\d+) violated\)\.\s+Worst case slack is ([-\d.]+)',text)
                    assert match and int(match[1]) == 0 and float(match[2]) >= 0, 'timing violation: '+mode
                    timing[corner][mode] = float(match[2])
                for mode in ('recovery','removal'):
                    assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.')
                ucp = (case/f'bank-ucp-{corner}.rpt').read_text()
                for name in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                    assert re.search('; '+name+r'\s*; 0\s*; 0\s*;',ucp), 'unconstrained coverage: '+name
            warnings = {}
            for phase in ('map','fit'):
                log = (case/(phase+'.log')).read_text()
                assert 'Critical Warning' not in log
                warnings[phase] = dict(Counter(re.findall(r'Warning \((\d+)\)',log)))
            results[variant] = dict(resources=resources,physical=memory,timing=timing,warnings=warnings)
    # The pre-existing ContA response index truncation is common. The first
    # scan trial also has one range-safe multiplication into its10-bit offset.
    # Pin its source/guard and prove all accepted values fit; waive no timing.
    increased = Counter(results['snapshot']['warnings']['map'])-Counter(results['baseline']['warnings']['map'])
    if increased.get('10230'):
        text = clones['ff_scan_controller.sv']
        assert increased['10230'] == 1 and 'first_bit <= index*18;' in text
        assert 'if (index >= WORDS)' in text and 'localparam integer WORDS = (STATE_BITS+17)/18;' in text
        assert max(i*18 for i in range(38)) == 666 < 2**10
        log = (root/'snapshot/map.log').read_text()
        additions = [s for s in log.splitlines() if s.startswith('Warning (10230)') and 'ff_scan_controller.sv(' in s]
        assert len(additions) == 1 and 'size 32 to match size of target (10)' in additions[0]
        increased.pop('10230')
    assert not (set(increased)-{'10036'}), 'unreviewed map warning'
    assert not (Counter(results['snapshot']['warnings']['fit'])-Counter(results['baseline']['warnings']['fit']))
    return dict(status='PASS_ISOLATED_FIT',scope=__doc__,results=results,
                delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k] for k in ('alms','registers','ram_blocks')},
                minimum_snapshot_slack_ns=min(v for c in results['snapshot']['timing'].values() for v in c.values()),
                limits='No complete-core area/timing, protocol timeout/global barrier, system-reset entry or physical snapshot qualification.')


def controls(root):
    result = []
    for name,path,before,after in [
        ('source','snapshot/ContA_snapshot.sv','scan_bits[680:1]',"{scan_bits[680:2],1'b0}"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('policy','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('ff_retention','snapshot/snapshot_fit.map.rpt','13 (13)','12 (12)'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('ram_latency','snapshot/snapshot_fit.fit.rpt','; no                     ; no                      ;','; yes                    ; no                      ;'),
    ]:
        with tempfile.TemporaryDirectory(prefix='conta-scan-bad-',dir='/tmp') as temp:
            case = Path(temp)/'fit'
            shutil.copytree(root,case,ignore=shutil.ignore_patterns('db','incremental_db'))
            p = case/path
            raw = p.read_text(encoding='latin1')
            assert before in raw, 'failure-control target absent: '+name
            p.write_text(raw.replace(before,after),encoding='latin1')
            try:
                audit(case)
            except AssertionError as e:
                result.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:
                raise AssertionError('damaged ContA scan evidence accepted: '+name)
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    result = audit(a.directory.resolve())
    result['failure_controls'] = controls(a.directory.resolve())
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('CONTA SERIAL FIT PASS:delta',result['delta'],'minimum',result['minimum_snapshot_slack_ns'],'ns;6bad controls reject')
