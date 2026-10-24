#!/usr/bin/env python3
"""Preserve watchdog-startup diagnostics without claiming native boot or speed.

Review terminal isolated-board runs, including rejected/insufficient earlier
observations. Snapshot exact live native handles once; never archive a mutable
live log as a terminal result or change any run's original bound.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
RUNS=[
    'native-baseboard-reset-sysper4-v1-20261023',
    'native-baseboard-reset-sysper16-v1-20261023',
    'native-baseboard-reset-gated-sysper4-v1-20261023',
    'native-baseboard-midas-gated-sysper4-v1-20261023',
    'native-baseboard-startup-gated-sysper4-v1-20261023',
    'native-baseboard-watchdog-sysper4-v1-20261023',
    'native-baseboard-watchdog-sysper4-v2-20261023',
    'native-baseboard-watchdog-finish-cp-sysper4-v1-20261023',
]


def read(p): return json.loads(p.read_text())


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    directories=[];rows=[]
    for name in RUNS:
        d=BUILD/name;r=read(d/'run-state.json')
        assert r['status']!='RUNNING',name
        assert r['original_board_and_cells_unchanged'] and r['original_ROM_unchanged']
        assert not r['hardware_changed'] and not r['live_runs_changed']
        assert sha(d/'tb_native_baseboard_reset.sv')==r['fixture_sha256']
        for n,h in r['inputs'].items(): assert sha(d/'inputs'/n)==h,(name,n)
        assert r['actions'][0]['returncode']==0
        log=(d/'test.log').read_text()
        passed=r['actions'][-1]['returncode']==0
        expected_failure=name in [RUNS[4],RUNS[5]]
        assert passed!=expected_failure,(name,r['status'])
        if name==RUNS[4]:
            assert 'address=f3eb data=8e' in log and 'reset_n=0' in log
            assert 'unexpected second Reset' in log
        elif name==RUNS[5]:
            assert 'pc=f3e9 bank_n=111 resets=0' in log
            assert 'CP progress did not follow the observed watchdog startup phase' in log
        else:
            assert 'BB_RESET_CLEAR_EXIT' in log
        if name==RUNS[6]:
            assert 'BB_WATCHDOG_FIRST_PHASE ticks=139809848 phases=1048576 counter=1048576 resets=396' in log
            assert 'pc=f3e9 bank_n=111 resets=396' in log
        if name==RUNS[7]:
            # A DDR-induced strobe is not a FinishCP instruction sequence.
            assert 'BB_RESET_ROM_FINISH_CP' in log
        row=dict(directory=name,status=r['status'],test_returncode=r['actions'][-1]['returncode'],
                 state_sha256=sha(d/'run-state.json'),test_sha256=sha(d/'test.log'))
        for marker in ['BB_RESET_CLEAR_EXIT','BB_RESET_MIDAS_RETURN','BB_WATCHDOG_FIRST_PHASE',
                       'BB_RESET_FIRST_CP_WRITE','BB_RESET_ROM_FINISH_CP','BB_RESET_BOUND']:
            row[marker]=re.findall(r'^'+marker+r'[^\n]*',log,re.M)
        rows.append(row);directories.append(d)

    observations=[]
    for d in sorted(BUILD.glob('native-baseboard-queue-*readonly-v*-20261023')):
        assert (d/'result.json').exists(),d
        dest=out/d.name;dest.mkdir()
        for p in d.iterdir():
            if p.suffix in ['.json','.cpp','.lldb','.log','.py']:
                (dest/p.name).write_bytes(p.read_bytes())
        observations.append(dict(directory=d.name,result_sha256=sha(d/'result.json'),
                                 scope='Read-only debugger observation at eval entry, not a settled-state or wall-time benchmark'))
    watchdog=read(BUILD/'native-baseboard-queue-watchdog-readonly-v1-20261023/result.json')
    c=watchdog['counters']
    assert c['guest_ticks']==80112008 and c['watchdog_counter']==600841
    assert c['watchdog_phase_a']==c['watchdog_phase_b']==0
    assert c['watchdog_counter']<1048576
    # Input-only and optimized-away aliases are not CPU package observations.
    observations.append(dict(early_alias_limit='Do not interpret early decoded res/rdy/phi/ir aliases; some corresponding no[] nodes are tied off. Use actual package/owner fields.'))
    disassembly=ROOT/'chm/disassembly/bb_F000-FFFF.s'
    (out/'original-bb_F000-FFFF.s').write_bytes(disassembly.read_bytes())

    live=[]
    for kind in ['5c65','queue']:
        d=BUILD/f'native-cedar-{kind}-skip-wait-v1-20261023'
        r=read(d/'run-state.json');action=r['actions'][-1]
        assert r['status']=='RUNNING' and 'returncode' not in action
        ps=subprocess.check_output(['ps','-p',str(action['pid']),'-o','pid=,etime=,state=,command='],text=True)
        assert str(d/'obj/native-cedar') in ps
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        dest=out/('observed-native-'+kind);dest.mkdir()
        for n in ['run-state.json','provenance.json.gz','host-cycle-limit.txt','window-control.txt']:
            (dest/n).write_bytes((d/n).read_bytes())
        # Only the immutable copied prefix enters the receipt.
        with (d/'test.log').open('rb') as h:
            h.seek(max(0,(d/'test.log').stat().st_size-30000));raw=h.read()
        if not raw.endswith(b'\n'):raw=raw[:raw.rfind(b'\n')+1]
        (dest/'test-prefix-tail.log').write_bytes(raw)
        (dest/'ps.txt').write_text(ps)
        live.append(dict(kind=kind,pid=action['pid'],observed=time.time(),ps=ps.strip(),
                         state_sha256=sha(dest/'run-state.json'),bound=5000000000))
    result=dict(status='REVIEWED_ISOLATED_ORIGINAL_WATCHDOG_STARTUP',scope=__doc__,
                isolated_runs=rows,read_only_inspections=observations,native_observations=live,
                first_watchdog_phase_ticks=139809848,first_watchdog_phase_counter=1048576,
                initial_watchdog_resets=396,original_native_limits_changed=False,
                firmware_qualified=False,native_Cedar_qualified=False,CP_root_ordering_fixed=False,
                timing_qualified=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result,directories


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r,dirs=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),dirs,label='baseboard-watchdog',extra_tools=[
        'diagnose_native_baseboard_reset.py','inspect_native_baseboard_process.py',Path(__file__).name])
    print(r['status'])
