#!/usr/bin/env python3
"""Review the terminal matched MSA clock-control experiment, including phase policy.

This measures component area/timing. It does not qualify an ALTCLKCTRL simulation
model, a native pause barrier, complete snapshots, a whole-core fit or speed.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
from audit_storage_factored_fit import CORNERS
import audit_msa_clocked as source_audit


def sha(raw): return hashlib.sha256(raw).hexdigest()


def audit(root, reports, overrides=None):
    overrides = overrides or {}
    raw = lambda n: overrides.get(n, (root/n).read_bytes())
    read = lambda n: raw(n).decode('latin1')
    provenance = json.loads(read('provenance.json'))
    assert 'Matched MSA clock-control run terminal' in read('driver.log')
    for name, digest in provenance['compiled_files'].items():
        if not name.endswith('.qsf'):
            assert sha(raw(name)) == digest, 'compiled input changed: '+name
    top = read('clocked/top.sv')
    assert 'always @(negedge host_clk)actual_advance<=advance_r;' in top, 'guest phase changed'
    assert '.ena(advance_r || ff_clock_request)' in top, 'raw intent must precede falling-edge gate sample'
    assert '.ena_register_mode("falling edge")' in top, 'wrong gate phase'
    assert '.clock_type("Global Clock")' in top and '.inclk({3\'b000,host_clk})' in top
    assert read('clocked/bank.sdc') == ('create_clock -name host_clock -period 20.000 [get_ports host_clk]\n'
        'derive_clock_uncertainty\n'
        'set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports host_clk]]\n'
        'set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports host_clk]]\n'
        'set_output_delay -clock host_clock -max 2.000 [all_outputs]\n'
        'set_output_delay -clock host_clock -min 0.000 [all_outputs]\n'), 'clock/IO contract changed'
    board = read('clocked/msa_board.sv')
    assert board.count(source_audit.NEW) == 78
    assert read('baseline/msa_board.sv') == board.replace(source_audit.NEW, source_audit.OLD)
    source_audit.audit(root/'clocked', provenance['manifest'], reports,
                       {n: raw('clocked/'+n) for n in ['msa_board.sv', *provenance['manifest']['cloned_sources']]})
    results = {}
    for label in ('baseline', 'clocked'):
        qsf = read(label+'/snapshot_fit.qsf')
        suffix = '\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
        assert qsf in (provenance['expected_qsf'], provenance['expected_qsf']+suffix,
                       provenance['expected_qsf']+suffix+'\n'), 'unapproved fixture policy'
        for name, key in [('top.sv','expected_top'), ('bank.sdc','expected_sdc'), ('audit.tcl','expected_audit')]:
            assert read(label+'/'+name) == provenance[key], 'fixture/timing input differs'
        summary = read(label+'/snapshot_fit.fit.summary')
        mapping = read(label+'/snapshot_fit.map.summary')
        assert 'Fitter Status : Successful' in summary and 'Device : 5CSEBA6U23I7' in summary
        assert 'Analysis & Synthesis Status : Successful' in mapping
        for text in ('Total virtual pins : 991', 'Total block memory bits : 0 /', 'Total DSP Blocks : 0 /'):
            assert text in summary, 'resource/coverage changed'
        area = re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / 41,910', summary)
        regs = re.search(r'Total registers : (\d+)', summary)
        assert area and regs
        timing = []
        for corner in CORNERS:
            for kind in ('setup', 'hold', 'mpw'):
                text = read(f'{label}/bank-{kind}-{corner}.rpt')
                match = re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)', text)
                assert match and int(match[1]) > 0 and int(match[2]) == 0 and float(match[3]) >= 0, 'timing violation/missing report'
                timing.append(dict(corner=corner,kind=kind,slack_ns=float(match[3])))
            for kind in ('recovery','removal'):
                assert 'Nothing to report.' in read(f'{label}/bank-{kind}-{corner}.rpt')
            coverage = read(f'{label}/bank-ucp-{corner}.rpt')
            for prop in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                match = re.search(r';\s*'+prop+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;', coverage)
                assert match and match.groups() == ('0','0'), 'unconstrained fixture path'
            clocks = read(f'{label}/bank-clocks-{corner}.rpt')
            assert re.findall(r'(?m)^;\s*(\S+)\s*;\s*(Base|Generated)\s*;',clocks) == [('host_clock','Base')]
            assert re.search(r';\s*host_clock\s*;\s*Base\s*;\s*20\.000\s*;\s*50\.0 MHz\s*;',clocks)
        results[label] = dict(alms=int(area[1].replace(',','')),registers=int(regs[1]),timing=timing,
                              minimum_slack_ns=min(t['slack_ns'] for t in timing))
    mapped = read('clocked/snapshot_fit.map.rpt')
    assert re.search(r';\s*ENA_REGISTER_MODE\s*;\s*falling edge\s*;',mapped), 'mapped gate phase differs'
    assert re.search(r';\s*CLOCK_TYPE\s*;\s*Global Clock\s*;',mapped)
    fitted = read('clocked/snapshot_fit.fit.rpt')
    assert re.search(r'altclkctrl:ff_clock_control\|altclkctrl_euh:auto_generated\|outclk\s*;\s*CLKCTRL_G6\s*;\s*1029\s*;\s*Global Clock',fitted), 'dedicated clock not fitted'
    assert 'with 866 fanout uses global clock CLKCTRL_G6' in fitted
    delta = results['clocked']['alms'] - results['baseline']['alms']
    assert delta >= 0, 'area now improves; review disposition'
    return dict(status='REJECTED_NO_AREA_BENEFIT',scope=__doc__,results=results,additional_alms=delta,
                physical_bits=2690,fields=458,actual_global_clock='CLKCTRL_G6 / 866 fanout',hardware_changed=False)


def controls(root, reports):
    cases = [('late_enable','clocked/top.sv','.ena(advance_r || ff_clock_request)', '.ena(actual_advance || ff_clock_request)'),
             ('wrong_phase','clocked/top.sv','falling edge','none'),
             ('loose_clock','clocked/bank.sdc','20.000','40.000'),
             ('bad_hold','clocked/bank-hold-7_slow_1100mv_100c.rpt','(0 violated)','(1 violated)'),
             ('mapped_wrong_phase','clocked/snapshot_fit.map.rpt','; falling edge   ;','; none           ;')]
    results = []
    for name, path, before, after in cases:
        original = (root/path).read_bytes(); assert before.encode() in original
        damaged = original.replace(before.encode(),after.encode(),1)
        record = copy.deepcopy(json.loads((root/'provenance.json').read_text()))
        if path in record['compiled_files']:
            record['compiled_files'][path] = sha(damaged)
            if path.endswith('top.sv'): record['expected_top'] = damaged.decode()
            if path.endswith('bank.sdc'): record['expected_sdc'] = damaged.decode()
        try: audit(root,reports,{path:damaged,'provenance.json':json.dumps(record).encode()})
        except AssertionError as error:
            results.append(dict(case=name,rejected=True,diagnostic=str(error),damaged_metadata_rehashed=True))
        else: raise AssertionError('damaged fit accepted: '+name)
    return results


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--reports',type=Path,default=Path(__file__).resolve().parents[3]/'fpga/mister/reports')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory,a.reports);result['failure_controls']=controls(a.directory,a.reports)
    result['input_hashes']={str(x.relative_to(a.directory)):sha(x.read_bytes()) for x in a.directory.rglob('*') if x.is_file()}
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],additional_alms=result['additional_alms'],minimum_slack_ns=min(r['minimum_slack_ns'] for r in result['results'].values()),failure_controls=len(result['failure_controls']))))
