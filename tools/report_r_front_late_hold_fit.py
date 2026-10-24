#!/usr/bin/env python3
"""Prepare read-only four-corner queries against the completed component fit.

The original query failed on a hierarchical-only filter for top-level launch
registers. Preserve that result and query the SAME fitted netlist with flat
and hierarchical names. Keep full registered-result and owner-only reports
separate; no field, timing constraint or hardware logic is removed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from test_r_front_forward_view import sha

TCL=r'''project_open clock_fit
set outdir late-hold-flat-v5-reports
file mkdir $outdir
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four conditions"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 puts "LATE_HOLD_CORNER $name"
 set launches [get_registers {*launch*}]
 set results [get_registers {*result*}]
 set late_names {}
 foreach_in_collection reg $launches {
  set rn [get_register_info -name $reg]
  if {[regexp {(^|\|)launch\[388\](~.*)?$} $rn]} {
   # TimeQuest's default escaping accepts the returned literal bus name.
   # Adding backslashes made v3 search for backslashes in the node name.
   lappend late_names $rn
  }
 }
 set owner_names {}
 foreach_in_collection reg $results {
  set rn [get_register_info -name $reg]
  if {[regexp {(^|\|)result\[([0-9]+)\]} $rn dummy prefix bit] && $bit>=556 && $bit<=1399} {
   lappend owner_names $rn
  }
 }
 if {[llength $late_names]==0 || [llength $owner_names]==0} {error "Missing late/owner register names"}
 set late [get_registers $late_names]
 set owners [get_registers $owner_names]
 if {[get_collection_size $late]==0 || [get_collection_size $owners]==0} {error "Missing late/owner collections"}
 puts "LATE_HOLD_COVERAGE launch=[get_collection_size $launches] results=[get_collection_size $results] late=[get_collection_size $late] owners=[get_collection_size $owners]"
 foreach mode {setup hold recovery removal} {
  report_timing -$mode -npaths 10 -detail full_path -file $outdir/clock-$mode-$name.rpt
 }
 report_timing -setup -from $launches -to $results -npaths 10 -detail full_path -file $outdir/clock-evaluator-$name.rpt
 report_timing -setup -from $launches -to $owners -npaths 32 -detail full_path -file $outdir/clock-owner-results-$name.rpt
 report_timing -setup -from $late -to $owners -npaths 32 -detail full_path -file $outdir/clock-late-hold-owners-$name.rpt
 report_timing -setup -from $late -to $results -npaths 32 -detail full_path -file $outdir/clock-late-hold-all-$name.rpt
 report_min_pulse_width -file $outdir/clock-mpw-$name.rpt
 report_clocks -file $outdir/clock-clocks-$name.rpt
 report_ucp -file $outdir/clock-ucp-$name.rpt
 delete_timing_netlist
}
project_close
'''


def prepare(source,out):
    assert not out.exists()
    r=json.loads((source/'run-state.json').read_text())
    assert r['status']=='TERMINAL_FAILED'
    assert [(v['stage'],v['returncode'])for v in r['stages']]==[('map',0),('fit',0),('sta',0),('corners',3)]
    assert 'Missing late Hold launch bit388'in(source/'corners.log').read_text()
    p=json.loads((source/'provenance.json').read_text())
    for name,h in p['files'].items():
        raw=(source/name).read_bytes()
        if name=='clock_fit.qsf':
            raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        assert hashlib.sha256(raw).hexdigest()==h,name
    out.mkdir(parents=True)
    (out/'audit_late_hold_flat.tcl').write_text(TCL)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (out/'query-provenance.json').write_text(json.dumps(dict(
        status='PREPARED_READ_ONLY_SAME_FIT_REPORT_QUERY',scope=__doc__,
        source=str(source),source_state_sha256=sha(source/'run-state.json'),
        source_provenance_sha256=sha(source/'provenance.json'),query_sha256=sha(out/'audit_late_hold_flat.tcl'),
        refit_permitted=False,source_or_SDC_changes=False,assembly_permitted=False,native_qualified=False),indent=2)+'\n')
    print('PREPARED_READ_ONLY_SAME_FIT_REPORT_QUERY',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();prepare(a.source.resolve(),a.output.resolve())
