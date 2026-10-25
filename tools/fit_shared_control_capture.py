#!/usr/bin/env python3
"""Budget shared capture on the SAME422-input/4943-output owned15ns fixture.

Require the full original Verilator replay and a terminal or verified-live
identical Icarus replay. Pending peer and producer/arrival coverage prevent
acceptance; this stages component timing only, with one SRAM and no assembly.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

import fit_control_clock as fixture
from fit_owned_r_packet import checked as baseline_checked,TOP
import generate_shared_control_capture as generate
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha


def checked(d,terminal=True):
    s=json.loads((d/'run-state.json').read_text());verify(d,s['files'])
    assert s['enabled'] and s.get('mutation') is None and s['all_original_checks_retained']
    assert s['stored_owner_bits']==828 and s['external_R_bits']==16 and s['SRAM_bits']==9216
    original=Path(s['source']);b=baseline_checked(original)
    assert s['source_state_sha256']==sha(original/'run-state.json')
    for row in s['source_files']:
        name=row['path'];raw=(original/name).read_text();expected=raw
        if name=='candidate.sv':expected=generate.kernel(raw)
        elif name.startswith('adapters/') and Path(name).stem in generate.ADAPTERS:expected=generate.adapter(raw)
        elif name=='adapters/dorado_local_task_capture_step.sv':expected=generate.task_adapter(raw)
        assert (d/name).read_text()==expected and sha(d/name)==row['output_sha256']
    assert (d/'packet.sv').read_text()==generate.kernel((original/'packet.sv').read_text())
    assert (d/'capture.sv').read_text()==generate.control((original/'candidate.sv').read_text())
    for name in ['caller.sv','forward.sv']:assert (d/name).read_bytes()==(original/name).read_bytes()
    tb=(d/'tb.sv').read_text()
    if s['passive_diagnostic']:tb=re.sub(r'^    \$display\("SHARED_CAPTURE_DIAG[^\n]*\n','',tb,flags=re.M)
    if s.get('passive_consumer_snapshot'):tb=re.sub(r'^   if\(idx==14\)\$display\("SHARED_CAPTURE_SAMPLE[^\n]*\n','',tb,flags=re.M)
    assert tb==(original/'tb.sv').read_text(),'Every original bench assertion must remain'
    if terminal:
        assert s['status']=='PASS_FULL_PROCESSOR_SHARED_CAPTURE' and s['all100_clock_counts_match']
        assert s['summary']==b['summary'] and s['cache_coverage']==b['cache_coverage']
        assert len(s['actions'])==2 and all(a['returncode']==0 for a in s['actions'])
        for a in s['actions']:assert sha(d/(a['label']+'.log'))==a['log_sha256']
    return s


def prepare(tests,peer,proof,four_state,out,allow_running_peer=False):
    assert not out.exists();s=checked(tests);other=checked(peer,False);pending=None
    assert s['files']==other['files'] and {s['engine'],other['engine']}=={'verilator','icarus'}
    if other['status']=='RUNNING' and allow_running_peer:
        assert len(other['actions'])==2 and other['actions'][0]['returncode']==0
        a=other['actions'][-1];assert a['label']=='test' and 'returncode' not in a
        ps=subprocess.check_output(['ps','-p',str(a['pid']),'-o','pid=,etime=,stat=,command='],text=True).strip()
        assert str(peer/'test.vvp') in ps
        pending=dict(status='VERIFIED_LIVE_ICARUS',pid=a['pid'],ps=ps,observed=time.time(),
                     state_sha256=sha(peer/'run-state.json'),terminal_pass_required=True)
    else:checked(peer)
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_SHARED_CONTROL_FRAME_RESULTS_AND_CHRONOLOGY' and p['no_assumptions']
    assert p['arbitrary_binary_input_bits']==955 and p['compared_outputs']==363
    assert [r['waves'] for r in p['cases']]==[5,24]
    for r in p['cases']:
        d=proof/f"w{r['waves']}";verify(d,r['files'])
        assert r['passed'] and r['returncode']==0 and sha(d/'proof.log')==r['log_sha256']
    for name in ['dorado_capture_hold_atomic_step','dorado_capture_terminal_body']:
        assert module((tests/'capture.sv').read_text(),name)==module((proof/'actual.sv').read_text(),name)
    x=json.loads((four_state/'run-state.json').read_text());verify(four_state,x['files'])
    assert x['status']=='PASS_SHARED_CAPTURE_FULL_CONTEXT_XZ_FALLBACK'
    assert x['source_state_sha256']==sha(tests/'run-state.json') and x['source_test_log_sha256']==sha(tests/'test.log')
    assert x['caller_context_bits']==1941 and x['original_results_compared']==139
    for a in x['actions']:assert a['returncode']==0 and sha(four_state/(a['label']+'.log'))==a['log_sha256']
    assert 'SHARED_CAPTURE_XZ_PASS known=4 unknown=15528 checks=15532 bad_architectural_Q=15528' in (four_state/'test.log').read_text()
    out.mkdir(parents=True)
    paths=[r['path'] for r in s['source_files']]+['packet.sv','forward.sv','caller.sv','capture.sv']
    for n in paths:
        dest=out/n;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(tests/n,dest)
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
                           ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in paths))
    audit=fixture.AUDIT.replace('CONTROL_CLOCK_','OWNED_PACKET_').replace(' set launches [get_registers {*launch*}]',
        ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=4943} {error "Incomplete owned packet results"}
 puts "OWNED_PACKET_OUTPUT_BITS [get_collection_size $ports]"
 set inputs [get_ports {pins[*]}]
 if {[get_collection_size $inputs]!=422} {error "Incorrect actual caller input domain"}
 puts "OWNED_PACKET_INPUT_BITS [get_collection_size $inputs]"
 set launches [get_registers {*launch*}]''')
    for n,v in {'top.sv':TOP,'clock_fit.qsf':qsf,'clock.sdc':fixture.SDC,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':audit}.items():(out/n).write_text(v)
    for n in ['fit_control_clock.py',Path(__file__).name,'generate_shared_control_capture.py']:
        shutil.copy2(Path(__file__).with_name(n),out/n)
    record=dict(scope=__doc__,clock_period_ns=15,input_bits=422,result_bits=4943,
                stored_architectural_owner_bits=828,owned_input_snapshot_bits=402,original_external_R_alias_bits=16,
                single_physical_SRAM_bits=9216,source_cache_state_bits=1416,extra_architectural_state_bits=0,
                tests=str(tests),tests_state_sha256=sha(tests/'run-state.json'),peer=str(peer),
                peer_state_sha256=None if pending else sha(peer/'run-state.json'),pending_peer_at_staging=pending,
                frame_proof_state_sha256=sha(proof/'run-state.json'),prefix_key_unchanged=True,
                four_state_state_sha256=sha(four_state/'run-state.json'),full_context_XZ_fallback=True,
                whole_packet_final_root_retained=True,refresh_comparison_retained=True,
                assembly_permitted=False,native_qualified=False,source_device_epochs_qualified=False,
                hardware_changed=False,FPGA_available=True,
                files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
    print('PREPARED_SHARED_CAPTURE_422_INPUT_4943_OUTPUT_15NS',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['tests','peer','proof','four-state','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--allow-running-peer',action='store_true')
    a=p.parse_args();prepare(a.tests.resolve(),a.peer.resolve(),a.proof.resolve(),a.four_state.resolve(),a.output.resolve(),a.allow_running_peer)
