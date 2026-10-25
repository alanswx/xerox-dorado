#!/usr/bin/env python3
"""Review the complete fall-guard packet and whole-control support audit.

Both original processor replays must finish. Check every timing corner and
collected source, retaining a timing rejection even when digital checks pass.
The independent support audit is evidence for a future guard, not permission
to alter the present one. No native selection or hardware speed qualification.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from audit_control_pin_support import CONE, SOURCE, MITER, support
from fit_owned_r_packet import checked, TOP
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import collection, CORNERS, verify
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_owned_r_packet import fall_guard_source
from test_r_front_forward_view import sha

BUILD = Path(__file__).resolve().parents[1] / 'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def fit_review(st,prefix):
    p = read(st / 'provenance.json')
    verify(st,p['files'])
    assert p['input_bits'] == 422 and p['result_bits'] == 4943 and not p['assembly_permitted']
    assert (st / 'top.sv').read_text() == TOP
    fit, envelope, receipt = collection(prefix + '-owned-terminal-v1-20261024')
    assert len(receipt['members']) == 92 and read(fit / 'provenance.json') == p
    state = read(fit / 'run-state.json')
    assert state['status'] == 'TERMINAL_REVIEW_REQUIRED' and all(r['returncode'] == 0 for r in state['stages'])
    back = read(fit / 'collected-input-check.json')
    assert back['job_pids_absent'] == [state['builder_pid']] + [r['pid'] for r in state['stages']]
    for name, digest in p['files'].items():
        assert back['actual'][name] == sha(fit / name)
        if sha(fit / name) != digest:
            assert name == 'clock_fit.qsf'
            assert (fit / name).read_bytes() == (st / name).read_bytes() + b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    for kind, width in [('OUTPUT', 4943), ('INPUT', 422)]:
        assert re.findall(r'OWNED_PACKET_' + kind + r'_BITS (\d+)', (fit / 'corners.log').read_text()) == [str(width)] * 4
    corners = []
    for corner in CORNERS:
        row = dict(corner=corner, setup=first_path(fit / f'clock-setup-{corner}.rpt'),
                   hold=first_path(fit / f'clock-hold-{corner}.rpt'),
                   evaluator=first_path(fit / f'clock-evaluator-{corner}.rpt'))
        row['pulse_width_ns'] = float(re.search(r'Worst case slack is (-?[0-9.]+)', (fit / f'clock-mpw-{corner}.rpt').read_text())[1])
        for mode in ['recovery', 'removal']:
            assert 'Nothing to report.' in (fit / f'clock-{mode}-{corner}.rpt').read_text()
        ucp = re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', (fit / f'clock-ucp-{corner}.rpt').read_text())
        assert len(ucp) == 6 and all(a == '0' and b in ['', '0'] for a, b in ucp)
        corners.append(row)
    summary = (fit / 'clock_fit.fit.summary').read_text()
    rpt = (fit / 'clock_fit.fit.rpt').read_text(encoding='latin1')
    area = dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)', summary)[1].replace(',', '')),
                registers=int(re.search(r'Total registers : ([0-9,]+)', summary)[1].replace(',', '')),
                MLAB_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)', rpt)[1].replace(',', '')))
    assert area['MLAB_bits'] == 9216
    passed = all(r['setup']['slack_ns'] >= 0 and r['hold']['slack_ns'] >= 0 and r['pulse_width_ns'] >= 0 for r in corners)
    return fit,envelope,receipt,corners,area,passed


def staging_review(out,collected_fit=False,phase=False):
    """Preserve qualified helper/Verilator evidence without accepting live jobs."""
    from review_control_supported_fall_guard import apply_source
    assert not out.exists()
    prefix = ('phase-' if phase else '') + 'supported-fall-guard'
    direct = BUILD / (prefix + '-review-v2-20261024')
    assert read(direct / 'results.json')['four_state_direct_before_after_pins']
    tests = BUILD / (prefix + '-owned-verilator-v1-20261024')
    peer = BUILD / (prefix + '-owned-icarus-v1-20261024')
    s = checked(tests)
    other = read(peer / 'run-state.json'); verify(peer, other['files'])
    assert other['files'] == s['files'] and other['mutation'] is None
    if other['status'] == 'RUNNING':
        assert len(other['actions']) == 2 and other['actions'][0]['returncode'] == 0
        action = other['actions'][-1]
        assert action['label'] == 'test' and 'returncode' not in action
        ps = subprocess.check_output(['ps','-p',str(action['pid']),'-o','pid=,etime=,stat=,command='],text=True).strip()
        assert str(peer / 'test.vvp') in ps
        observation = dict(status='VERIFIED_LIVE', pid=action['pid'], ps=ps, observed=time.time())
    else:
        checked(peer)
        observation = dict(status=other['status'], observed=time.time())
    original = BUILD / 'owned-specialized-verilator-v2-20261024'
    checked(original)
    assert (tests / 'candidate.sv').read_text() == apply_source((original / 'candidate.sv').read_text(), direct)
    for name in s['files']:
        if Path(name).suffix == '.sv' and name != 'candidate.sv':
            assert sha(original / name) == sha(tests / name)
    st = BUILD / (prefix + '-owned-stage-v1-20261024')
    p = read(st / 'provenance.json'); verify(st, p['files'])
    assert p['tests_state_sha256'] == sha(tests / 'run-state.json')
    assert p['pending_peer_at_staging']['frozen_input_hashes'] == other['files']
    assert (st / 'top.sv').read_text() == TOP and p['input_bits'] == 422 and p['result_bits'] == 4943
    assert not p['assembly_permitted'] and not p['native_qualified']
    assert sha(st / 'candidate.sv') == sha(tests / 'candidate.sv')
    old_stage = BUILD / 'owned-specialized-stage-v1-20261024'
    for name, digest in p['files'].items():
        if Path(name).suffix in ['.sv','.sdc','.qsf','.qpf','.tcl'] and name != 'candidate.sv':
            assert digest == sha(old_stage / name), name
    remote_code = '''from pathlib import Path
import hashlib,json,os,time
d=Path(REMOTE_PROJECT)
p=json.loads((d/'provenance.json').read_text())
s=json.loads((d/'run-state.json').read_text())
actual={};annotations=[]
for name,digest in p['files'].items():
 raw=(d/name).read_bytes();h=hashlib.sha256(raw).hexdigest();actual[name]=h
 if h!=digest:
  suffix=b'\\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
  assert name=='clock_fit.qsf' and raw.endswith(suffix) and hashlib.sha256(raw[:-len(suffix)]).hexdigest()==digest
  annotations.append(name)
pids={}
for pid in [s['builder_pid']]+[r['pid'] for r in s['stages']]:
 try:pids[str(pid)]=dict(executable=os.readlink('/proc/'+str(pid)+'/exe'),command=Path('/proc/'+str(pid)+'/cmdline').read_bytes().replace(b'\\0',b' ').decode())
 except FileNotFoundError:pids[str(pid)]=None
if s['status']=='RUNNING':assert pids[str(s['builder_pid'])] is not None
print(json.dumps(dict(state=s,provenance=p,actual=actual,allowed_annotations=annotations,pids=pids,observed=time.time())))
'''.replace('REMOTE_PROJECT',repr('/home/alans/mister/'+prefix+'-owned-packet-v1-20261024'))
    back = json.loads(subprocess.check_output(['ssh','-o','BatchMode=yes','misterubuntu','python3','-'],input=remote_code,text=True))
    assert back['provenance'] == p
    assert all(r.get('returncode',0) == 0 for r in back['state']['stages'])
    budget=None
    if collected_fit:
        assert back['state']['status']=='TERMINAL_REVIEW_REQUIRED'
        assert all(v is None for v in back['pids'].values())
        fit,envelope,receipt,corners,area,passed=fit_review(st,prefix)
        budget=dict(period_ns=15,input_bits=422,result_bits=4943,corners=corners,area=area,
                    pass_15ns=passed,archive_sha256=receipt['archive_sha256'],members=len(receipt['members']))
    out.mkdir(parents=True)
    for d in [direct,tests,st]:
        retain_text(d,out/d.name)
    if collected_fit:
        retain_text(fit,out/fit.name)
        (out/envelope.name).write_bytes(envelope.read_bytes())
    (out/'read-build-host.py').write_text(remote_code)
    (out/'build-host-observation.json').write_text(json.dumps(back,indent=2)+'\n')
    (out/'Icarus-state-at-observation.json').write_text(json.dumps(other,indent=2)+'\n')
    result = dict(status='REVIEWED_SUPPORTED_GUARD_FIT_PEER_REVIEW_PENDING' if collected_fit else 'REVIEWED_SUPPORTED_GUARD_HELPER_AND_VERILATOR_STAGED',scope=__doc__,
                  guard_pins=read(direct/'results.json')['guard_pin_count'],complete_control_result_bits=363,independent_before_after_XZ_cases=7632,
                  full_Verilator_replay=s['summary'],all_original_checks_retained=True,Icarus=observation,
                  build_host=back['state'],source_readback_exact_except_QSF_version_annotation=True,
                  identical_422_input_4943_output_15ns_fixture=True,
                  final_review_requires_both_terminal_replays_and_complete_corner_reports=True,
                  collected_fit_budget=budget,
                  timing_qualified=False,variant_selected=False,native_integrated=False,
                  hardware_changed=False,hardware_speed_qualified=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


def review(out,supported=False,phase=False):
    assert not out.exists()
    dirs = []
    assert not phase or supported
    prefix = ('phase-' if phase else '') + ('supported-fall-guard' if supported else 'fall-data-guard')
    guard = BUILD / (prefix + ('-review-v2-20261024' if phase else '-review-v1-20261024'))
    assert read(guard / 'results.json')['status'] == ('REVIEWED_SOURCE_BOUND_SUPPORTED_FALL_GUARD' if supported else 'REVIEWED_SOURCE_BOUND_FALL_DATA_GUARD')
    direct = BUILD / (prefix + '-review-v2-20261024')
    if supported:
        from review_control_supported_fall_guard import apply_source
        d = read(direct / 'results.json')
        assert d['status'] == 'REVIEWED_SOURCE_BOUND_SUPPORTED_FALL_GUARD' and d['four_state_direct_before_after_pins']
    prior = BUILD / 'owned-processor-packet-review-v1-20261024'
    assert read(prior / 'results.json')['status'] == 'REVIEWED_SOURCE_BOUND_OWNED_PROCESSOR_PACKET'
    previous_counts = None
    results = []
    for engine in ['icarus', 'verilator']:
        old = BUILD / f'owned-specialized-{engine}-v2-20261024'
        new = BUILD / f'{prefix}-owned-{engine}-v1-20261024'
        before, after = checked(old), checked(new)
        assert before['summary'] == after['summary']
        assert before['trace_links'] == after['trace_links']
        assert after['fall_data_guard_review_sha256'] == sha(guard / 'results.json')
        assert (new / 'candidate.sv').read_text() == fall_guard_source((old / 'candidate.sv').read_text(), guard)
        if supported:
            assert (new / 'candidate.sv').read_text() == apply_source((old / 'candidate.sv').read_text(), direct)
        # The compiled caller, cache, RAM, forwarding and original assertions
        # are identical. Only the pure control helper in candidate.sv changes.
        assert before['files'].keys() == after['files'].keys()
        for name in before['files']:
            if Path(name).suffix == '.sv' and name != 'candidate.sv':
                assert before['files'][name] == after['files'][name], name
        for row in after['source_files']:
            assert sha(new / row['path']) == row['output_sha256']
        counts = re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$', (new / 'test.log').read_text(), re.M)
        assert len(counts) == 100
        old_counts = re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$', (old / 'test.log').read_text(), re.M)
        assert counts == old_counts and (previous_counts is None or counts == previous_counts)
        previous_counts = counts
        assert after['cache_coverage'] == before['cache_coverage']
        results.append(dict(engine=engine, summary=after['summary'], state_sha256=sha(new / 'run-state.json')))
        dirs.append(new)

    st = BUILD / (prefix + '-owned-stage-v1-20261024')
    p = read(st / 'provenance.json')
    verify(st, p['files'])
    assert p['input_bits'] == 422 and p['result_bits'] == 4943 and not p['assembly_permitted']
    assert (st / 'top.sv').read_text() == TOP
    assert checked(Path(p['tests']))['specialized'] and checked(Path(p['peer']))['specialized']
    assert sha(Path(p['tests']) / 'run-state.json') == p['tests_state_sha256']
    pending = p['pending_peer_at_staging']
    assert pending['qualification_still_requires_terminal_pass']
    assert pending['frozen_input_hashes'] == checked(Path(p['peer']))['files']
    old_stage = BUILD / 'owned-specialized-stage-v1-20261024'
    # Equal registered endpoints, constraints and all actual fitted SV except
    # the proved helper. Preparation-tool provenance is allowed to differ.
    for name, digest in p['files'].items():
        if (Path(name).suffix in ['.sv', '.sdc', '.qsf', '.qpf', '.tcl']) and name != 'candidate.sv':
            assert digest == sha(old_stage / name), name
    fit,envelope,receipt,corners,area,passed = fit_review(st,prefix)
    # Passing this registered component budget would still not select a full
    # native core. Preserve either outcome from the complete corner evidence.
    if not supported:
        assert not passed
    dirs.extend([st, fit])

    audit = BUILD / 'whole-control-pin-support-v1-20261024'
    s = read(audit / 'run-state.json')
    verify(audit, s['files'])
    assert s['status'] == 'PASS_WHOLE_CONTROL_PIN_SUPPORT'
    assert s['complete_result_bits'] == 363 and s['arbitrary_binary_miter_input_bits'] == 1751
    assert s['source_state_sha256'] == sha(Path(s['source']) / 'run-state.json')
    raw = (Path(s['source']) / 'candidate.sv').read_text()
    atomic = module(raw, 'dorado_control_hold_atomic_step') + module(raw, 'dorado_control_terminal_body')
    assert (audit / 'atomic.sv').read_text() == atomic and (audit / 'cone-top.sv').read_text() == CONE
    assert s['cone_json_sha256'] == sha(audit / 'cone.json')
    ports = support(read(audit / 'cone.json')['modules']['control_binary_cone'])
    care = sorted(set(ports['before_pins']) | set(ports['after_pins']))
    mask = sum(1 << b for b in care)
    assert s['port_support'] == ports and care == s['guard_pin_support']
    assert len(care) == s['guard_pin_count'] == 108 and 119 not in care and 292 in care
    assert s['support_mask_hex'] == format(mask, 'x')
    assert s['unused_guard_pins'] == sorted(set(range(398)) - set(care))
    assert len(s['actions']) == 4
    for row in s['actions']:
        label = row['label']
        assert row['returncode'] == 0 and row['log_sha256'] == sha(audit / (label + '.log'))
        assert row['script_sha256'] == sha(audit / (label + '.ys'))
        if label == 'cone':
            continue
        waves = 24 if label == 'positive-w24' else 5
        value = mask if label.startswith('positive') else mask & ~(1 << 292)
        assert (audit / (label + '-source.sv')).read_text() == SOURCE.replace('WAVES_VALUE', str(waves))
        assert (audit / (label + '-top.sv')).read_text() == MITER.replace('MASK', format(value, 'x'))
        script = (audit / (label + '.ys')).read_text()
        assert '-set' not in script and '-prove equal 1' in script
        assert ('no model found: SUCCESS!' if label.startswith('positive') else 'model found: FAIL!') in (audit / (label + '.log')).read_text()
    counterexample = read(audit / 'missing-CP292-w5-counterexample.json')
    signals = {v['name']: v for v in counterexample['signal']}
    assert signals['equal']['wave'][0] == '0' and int(signals['different']['data'][0], 2) != 0
    dirs.append(audit)
    out.mkdir(parents=True)
    for d in dirs:
        retain_text(d, out / d.name)
    (out / envelope.name).write_bytes(envelope.read_bytes())
    for name, path in [('prior-owned-review.json', prior / 'results.json'), ('guard-helper-review.json', guard / 'results.json')]:
        (out / name).write_bytes(path.read_bytes())
    if supported:
        retain_text(direct, out / direct.name)
    result = dict(status='REVIEWED_CONTROL_GUARD_PACKET_AND_SUPPORT', scope=__doc__, replays=results,
                  all_original_assertions_and_100_clock_counts_preserved=True, sole_changed_compiled_SV='candidate.sv',
                  timing=dict(period_ns=15, input_bits=422, output_bits=4943, corners=corners, area=area,
                              pass_15ns=passed, archive_sha256=receipt['archive_sha256'], members=92),
                  whole_control_support=dict(pins=108, ignored_pins=290, result_bits=363, waves=[5, 24],
                                             miter_input_bits=1751, required_CP292_counterexample=True,
                                             conservative_sufficient_support_not_minimal=True),
                  combined_102_pin_guard_proof_pending=not supported, four_state_direct_pin_check_required=supported,
                  phase_supported_guard=phase,guard_pin_count=read(guard/'results.json')['guard_pin_count'] if supported else 392,
                  source_CP_device_epochs_qualified=False,
                  variant_selected=False, native60ns_or_hardware_speed_qualified=False,
                  hardware_changed=False, FPGA_user_release_received=True, full_goal_active=True)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['output', 'archive']:
        p.add_argument('--' + n, type=Path, required=True)
    p.add_argument('--supported',action='store_true',help='Review the 102-pin successor and its independent before/after X/Z checks.')
    p.add_argument('--staging-only',action='store_true',help='Record helper/Verilator pass and verify current peer/build handles; no final timing acceptance.')
    p.add_argument('--collected-fit',action='store_true',help='With --staging-only, preserve a fully collected corner review while peer replay remains a separate gate.')
    p.add_argument('--phase',action='store_true',help='With --supported, review the phase-supported successor.')
    a = p.parse_args()
    assert not a.phase or a.supported
    if a.staging_only:
        assert a.supported
        result = staging_review(a.output.resolve(),a.collected_fit,a.phase)
    else:
        assert not a.collected_fit
        result = review(a.output.resolve(),a.supported,a.phase)
    label = 'supported-control-guard-staging' if a.staging_only else ('supported-control-guard-packet' if a.supported else 'control-guard-packet')
    if a.collected_fit:
        label='supported-control-guard-fit-pending-replay'
    if a.phase:
        label='phase-'+label
    append_archive(a.archive.resolve(), a.output.resolve(), [], label=label,
                   extra_tools=['audit_control_pin_support.py', Path(__file__).name])
    print(result['status'], flush=True)
