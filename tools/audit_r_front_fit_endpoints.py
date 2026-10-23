#!/usr/bin/env python3
"""Separate native consumers from unused consistency exports in an existing fit.

Keep the complete registered fixture and its failing timing reports. This is
an additional read-only TimeQuest query, with the original 15 ns SDC, over ALL
keeper endpoints except the two explicitly audited unused diagnostic exports.
It does not waive a path, refit the design, or qualify native timing/cadence.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from fit_r_front_control import FIELDS
from review_r_front_windows import timing

ROOT = Path(__file__).resolve().parents[1]


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(fit, stage, out):
    full = timing(fit, 'r_front_control_physical_forward')
    record = json.loads((stage / 'r-front-control-native.json').read_text())
    for n, h in record['source_hashes'].items():
        assert sha(stage / n) == h, n
    native = stage / 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    raw = native.read_text()
    for signal in ['r_front_unsettled', 'r_front_shared_mismatch']:
        # Declaration and evaluator connection only: no synthesizable consumer.
        assert len(re.findall(r'\b' + signal + r'\b', raw)) == 2, signal
        assert re.search(r'wire[^;]*\b' + signal + r'\b[^;]*;', raw), signal
        port = 'unsettled' if signal.endswith('unsettled') else 'shared_state_mismatch'
        assert f'.{port}({signal})' in raw
    offset = 0
    fields = []
    for n, w in reversed([(n, w) for n, w in FIELDS if n not in ['source_pins', 'fa_n']]):
        fields.append(dict(name=n, first=offset, last=offset + w - 1))
        offset += w
    assert offset == 1400
    diagnostic = {r['first']: r['name'] for r in fields if r['name'] in ['unsettled', 'shared_state_mismatch']}
    assert diagnostic == {94: 'shared_state_mismatch', 355: 'unsettled'}
    out.mkdir(parents=True, exist_ok=False)
    # get_keepers includes non-combinational destinations such as RAM, not only
    # the result registers. Keep input staging and internal memory endpoints.
    tcl = r'''project_open clock_fit
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four conditions: $corners"}
set output [file normalize [lindex $quartus(args) 0]]
foreach corner $corners {
 create_timing_netlist
 set_operating_conditions $corner
 read_sdc
 update_timing_netlist
 set all [get_keepers *]
 set diagnostic_names {}
 set inventory [open [file join $output keepers-$corner.tsv] w]
 foreach_in_collection node $all {
  set name [get_object_info -name $node]
  set diagnostic [regexp {(^|\|)result\[(94|355)\](~reg[0-9]+)?$} $name]
  puts $inventory "$diagnostic\t$name"
  if {$diagnostic} {lappend diagnostic_names $name}
 }
 close $inventory
 if {[llength $diagnostic_names]<2} {error "Missing diagnostic endpoints: $diagnostic_names"}
 set diagnostics [get_keepers $diagnostic_names]
 set functional [remove_from_collection $all $diagnostics]
 if {[get_collection_size $all] != [expr {[get_collection_size $functional]+[get_collection_size $diagnostics]}]} {error "Endpoint partition incomplete"}
 puts "ENDPOINT_COVERAGE $corner all=[get_collection_size $all] functional=[get_collection_size $functional] diagnostic=[get_collection_size $diagnostics]"
 foreach mode {setup hold} {
  report_timing -$mode -npaths 10 -detail full_path -file [file join $output all-$mode-$corner.rpt]
  report_timing -$mode -to $functional -npaths 10 -detail full_path -file [file join $output functional-$mode-$corner.rpt]
 }
 report_ucp -file [file join $output ucp-$corner.rpt]
 delete_timing_netlist
}
project_close
'''
    (out / 'audit.tcl').write_text(tcl)
    result = dict(scope=__doc__, status='PREPARED_READ_ONLY_ENDPOINT_AUDIT',
                  fit=str(fit), full_fit=full, native_stage=str(stage),
                  native_manifest_sha256=sha(stage / 'r-front-control-native.json'),
                  native_backplane_sha256=sha(native), fields=fields,
                  excluded_diagnostic_result_bits=diagnostic,
                  native_diagnostic_consumers=0, audit_tcl_sha256=sha(out / 'audit.tcl'),
                  all_keeper_endpoints_retained_except_explicit_diagnostics=True,
                  SDC_sha256=sha(fit / 'clock.sdc'), constraints_changed=False,
                  native_timing_qualified=False, ordinary60ns_qualified=False)
    (out / 'prepared.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'])


def review(out):
    p = json.loads((out / 'prepared.json').read_text())
    assert sha(out / 'audit.tcl') == p['audit_tcl_sha256']
    log = (out / 'audit.log').read_text()
    assert 'Quartus Prime TimeQuest Timing Analyzer was successful' in log
    coverage = re.findall(r'ENDPOINT_COVERAGE (\S+) all=(\d+) functional=(\d+) diagnostic=(\d+)', log)
    assert len(coverage) == 4 and len({r[0] for r in coverage}) == 4
    rows = []
    for corner, total, functional, diagnostic in coverage:
        inventory = (out / f'keepers-{corner}.tsv').read_text().splitlines()
        assert len(inventory) == int(total)
        excluded = [line.split('\t', 1)[1] for line in inventory if line.startswith('1\t')]
        assert len(excluded) == int(diagnostic) and int(total) == int(functional) + int(diagnostic)
        assert {int(re.search(r'result\[(\d+)\]', n)[1]) for n in excluded} == {94, 355}
        # Real RAM endpoints must be in the retained collection, not silently
        # dropped by restricting this query to shell result registers.
        ram = [line.split('\t', 1)[1] for line in inventory if 'r_file_ports:files|' in line]
        assert ram and all('0\t' + n in inventory for n in ram)
        row = dict(corner=corner, all_keepers=int(total), functional_keepers=int(functional),
                   diagnostic_keepers=excluded, retained_memory_keepers=len(ram))
        for group in ['all', 'functional']:
            for mode in ['setup', 'hold']:
                text = (out / f'{group}-{mode}-{corner}.rpt').read_text()
                values = re.findall(r'; Slack\s*;\s*(-?\d+\.\d+)', text)
                assert values
                row[f'{group}_{mode}_ns'] = min(map(float, values))
                row[f'{group}_{mode}_worst_to'] = re.search(r'^; To Node[ \t]+; ([^;\n]+);', text, re.M)[1].strip()
        ucp = (out / f'ucp-{corner}.rpt').read_text()
        for label in ['Illegal Clocks', 'Unconstrained Clocks', 'Unconstrained Input Ports',
                      'Unconstrained Input Port Paths', 'Unconstrained Output Ports', 'Unconstrained Output Port Paths']:
            assert re.search(r'; ' + re.escape(label) + r'\s*; 0\s*; 0\s*;', ucp)
        rows.append(row)
    p.update(status='PASS_READ_ONLY_ENDPOINT_AUDIT', corners=rows,
             functional_setup_min_ns=min(r['functional_setup_ns'] for r in rows),
             functional_hold_min_ns=min(r['functional_hold_ns'] for r in rows),
             full_fixture_still_fails=any(r['all_setup_ns'] < 0 for r in rows),
             functional_isolated_budget_pass=all(r['functional_setup_ns'] >= 0 and r['functional_hold_ns'] >= 0 for r in rows))
    (out / 'results.json').write_text(json.dumps(p, indent=2) + '\n')
    print(json.dumps({k: p[k] for k in ['status', 'functional_setup_min_ns', 'functional_hold_min_ns', 'full_fixture_still_fails', 'functional_isolated_budget_pass']}))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fit', type=Path)
    parser.add_argument('--stage', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--review', action='store_true')
    args = parser.parse_args()
    if args.review:
        review(args.output.resolve())
    else:
        prepare(args.fit.resolve(), args.stage.resolve(), args.output.resolve())
