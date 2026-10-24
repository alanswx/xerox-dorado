#!/usr/bin/env python3
"""Fit the full entry-cached packet, preserving both results and SRAM owner.

Identical15ns/2ns-I/O constraints, no exceptions/retiming and4943 result bits
as the rejected fresh-prefix budget. One additional staged reset input belongs
to the real entry cache. Prefix preparation and cache state remain in the fitted
logic; no uncommitted owner is substituted into its canonical comparison.
No assembly/native/hardware/60ns timing qualification follows from this budget.
"""
import argparse
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from fit_r_front_packet import TOP as OLD_TOP
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]


def prepare(tests,out,equivalence=None):
    assert not out.exists();s=json.loads((tests/'run-state.json').read_text())
    assert s['status']=='PASS_ENTRY_CACHED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    assert s['all100_clock_counts_match'] and 'resets=1' in s['cache_coverage']
    equivalence_hash=None
    if s.get('pair_direct',False):
        assert equivalence is not None
        e=json.loads((equivalence/'run-state.json').read_text())
        assert e['status']=='PASS_PAIR_DIRECT_CACHE_READY_AND_PERMISSION_INDUCTION'
        for n,h in e['files'].items():assert sha(equivalence/n)==h
        for row in e['cases']:
            d=equivalence/row['mode'];assert row['passed'] and row['returncode']==0
            for n,h in row['files'].items():assert sha(d/n)==h
            assert sha(d/'proof.log')==row['log_sha256']
        assert (tests/'packet.sv').read_text().startswith((equivalence/'new-packet.sv').read_text()+'\n')
        equivalence_hash=sha(equivalence/'run-state.json')
    out.mkdir(parents=True);paths=[]
    for row in s['source_files']:
        p=tests/row['path'];assert sha(p)==row['output_sha256'];paths.append(row['path'])
    for n,h in s['files'].items():assert sha(tests/n)==h
    for n in [*paths,'packet.sv','forward.sv']:
        p=out/n;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(tests/n,p)
    top=OLD_TOP.replace('[1650:0]','[1651:0]').replace('wire advance,use_pair,cb,ca,refresh;','wire reset,advance,use_pair,cb,ca,refresh;').replace(
        '{advance,use_pair,held_q,cb,ca,pb,pa,refresh}=launch;','{reset,advance,use_pair,held_q,cb,ca,pb,pa,refresh}=launch;').replace(
        'first_result,last_result,ready,forwarded);','first_result,last_result,ready,forwarded,reset);')
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in [*paths,'packet.sv','forward.sv']))
    report=fixture.AUDIT.replace('CONTROL_CLOCK_','CACHED_PACKET_').replace(' set launches [get_registers {*launch*}]',
        ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=4943} {error "Incomplete cached packet results"}
 puts "CACHED_PACKET_OUTPUT_BITS [get_collection_size $ports]"
 set launches [get_registers {*launch*}]''')
    for n,v in {'top.sv':top,'clock_fit.qsf':qsf,'clock.sdc':fixture.SDC,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':report}.items():(out/n).write_text(v)
    for n in ['fit_control_clock.py',Path(__file__).name]:shutil.copy2(ROOT/'tools'/n,out/n)
    r=dict(scope=__doc__,clock_period_ns=15,input_bits=1652,result_bits=4943,subevent_result_bits=2470,
        tests=str(tests),tests_state_sha256=sha(tests/'run-state.json'),pair_direct=s.get('pair_direct',False),
        equivalence_state_sha256=equivalence_hash,actual_entry_cache_and_auxiliary_state_retained=True,
        single_physical_RAM_owner_bits=9216,native_canonical_match_specialization_used=False,
        assembly_permitted=False,native_qualified=False,hardware_changed=False,hardware_requires_user_release=True,
        files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n');print('PREPARED_COMPLETE_ENTRY_CACHED_PACKET_15NS',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--tests',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--equivalence',type=Path)
    a=p.parse_args();prepare(a.tests.resolve(),a.output.resolve(),a.equivalence.resolve() if a.equivalence else None)
