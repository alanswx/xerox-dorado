#!/usr/bin/env python3
"""Preserve the complete prefix support proof and exact safety-proof handles.

Support equivalence is established; a native key change still needs temporal
safety, four-state checks, context integration, full timing and firmware gates.
Never replace the complete objective with a narrower cache-component result.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from audit_cp_prefix_key_support import PREFIX, MITER
from prove_cp_cache_key_projection import projected, TOP
from prove_cp_prefix_cache import module
from prove_native_cp_context import RTL
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    support = BUILD/'cp-prefix-key-support-v1-20261024'
    r = read(support/'run-state.json')
    assert r['status'] == 'PASS_BINARY_CP_PREFIX_SUPPORT_AUDIT'
    assert r['compared_prefix_bits'] == 269 and r['key_bits'] == 567
    assert r['arbitrary_binary_input_bits'] == 1134 and r['input_assumptions'] == 'none'
    assert all(a['returncode'] == 0 for a in r['actions'])
    assert r['whole_prefix_projection_proven'] and r['missing_owner_bit_counterexample']
    stage = Path(r['stage'])
    assert sha(stage/'r-front-control-native.json') == r['manifest_sha256']
    assert sha(support/'actual-source.sv') == sha(stage/RTL) == r['actual_RTL_sha256']
    source = (support/'actual-source.sv').read_text()
    assert (support/'atomic.sv').read_text() == module(source, 'dorado_control_hold_atomic_step')+module(source, 'dorado_control_terminal_body')
    assert (support/'prefix.sv').read_text() == PREFIX
    care = set(r['cared_key_bits'])
    assert len(care) == 269 and 138 not in care and all(i in care for i in range(1, 12))
    assert set(r['unused_key_bits']) == set(range(567))-care
    for label, bits, marker in [('positive', care, 'no model found: SUCCESS!'),
                                ('missing-owner-bit', care-{max(care)}, 'model found: FAIL!')]:
        mask = sum(1 << i for i in bits)
        assert (support/(label+'.sv')).read_text() == MITER.replace('MASK', format(mask, 'x'))
        assert marker in (support/(label+'.log')).read_text()
    assert (support/'missing-owner-bit-counterexample.json').exists()
    retain(support, out/support.name)
    proofs = []
    for variant in ['positive', 'negative']:
        d = BUILD/f'cp-cache-key-projection-{variant}-v1-20261024'
        state = read(d/'run-state.json')
        assert state['compared_control_output_bits'] == 475 and state['compared_prefix_bits'] == 269
        assert state['support_state_sha256'] == sha(support/'run-state.json')
        assert (d/'top.sv').read_text() == TOP
        actual = (d/'actual-source.sv').read_text()
        bits = care if variant == 'positive' else care-{max(care)}
        assert (d/'candidate-cache.sv').read_text() == projected(module(actual, 'dorado_cp_prefix_cache'), bits)
        for name, digest in state['source_hashes'].items():
            assert sha(d/name) == digest
        dest = out/d.name
        if state['status'] == 'RUNNING':
            ps = subprocess.check_output(['ps', '-p', str(state['driver_pid'])+','+str(state['pid']),
                '-o', 'pid=,ppid=,etime=,state=,command='], text=True)
            processes = {int(line.split(None, 1)[0]): line.split(None, 4) for line in ps.splitlines()}
            assert d.name in processes[state['driver_pid']][4]
            assert int(processes[state['pid']][1]) == state['driver_pid']
            assert 'yowasp-yosys -Q -T -s /work/proof.ys' in processes[state['pid']][4]
            dest.mkdir(parents=True)
            (dest/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
            (dest/'ps.txt').write_text(ps)
            row = dict(directory=d.name, driver_pid=state['driver_pid'], pid=state['pid'],
                observed=time.time(), ps=ps.strip(), child_PPId_verified=True)
            for name in state['source_hashes']:
                shutil.copy2(d/name, dest/name)
            raw = (d/'proof.log').read_bytes()
            if raw and not raw.endswith(b'\n'):
                raw = raw[:raw.rfind(b'\n')+1]
            (dest/'proof-prefix.log').write_bytes(raw)
            row['terminal'] = False
        else:
            expected = 'PASS_CP_CACHE_KEY_INDUCTIVE_SAFETY' if variant == 'positive' else 'PASS_CP_CACHE_KEY_NEGATIVE_CONTROL'
            assert state['status'] == expected and state['returncode'] == 0
            ps = subprocess.run(['ps', '-p', str(state['pid']), '-o', 'pid=,command='], capture_output=True, text=True)
            assert ps.returncode == 1 and not ps.stdout.strip()
            assert sha(d/'proof.log') == state['log_sha256']
            retain(d, dest)
            row = dict(directory=d.name, status=state['status'], terminal=True,
                unbounded_safety_proven=state['unbounded_safety_proven'])
        proofs.append(row)
    rejected = BUILD/'cp-prefix-key-support-review-v1-20261024'
    assert read(rejected/'failure.json')['status'] == 'REJECTED_REVIEW_EXPECTED_HOST_OUTPUT_PATH_IN_WASM_CHILD_ARGV'
    for name in ['failure.json', 'rejected-review.py']:
        dest = out/('rejected-'+rejected.name)/name
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(rejected/name, dest)
    result = dict(status='REVIEWED_BINARY_PREFIX_SUPPORT_AND_CACHE_SAFETY_PROGRESS',
        support_source=dict(stage=r['stage'], actual_RTL_sha256=r['actual_RTL_sha256'],
            complete_prefix_bits=269, complete_key_bits=567, cared_key_bits=sorted(care),
            unused_key_bits=sorted(set(range(567))-care), arbitrary_binary_inputs=1134,
            all_saved_CP_bits_still_compared=True, critical_before_pin125_unused=True,
            whole_prefix_projection_proven=True, required_bit_counterexample=True),
        cache_safety_proofs=proofs,
        required_next_gates=['Unbounded complete475-output cache safety', 'Four-state guard behavior',
            'Exact owned-context integration', 'Full native timing', 'Original-firmware Cedar boot',
            'Original60ns capture/event integration', 'Separate faster SDRAM',
            'Interactive input, Rollback and file persistence', 'Three boots and two-hour mixed-I/O stress'],
        native_cache_changed=False, four_state_qualified=False, native_timing_qualified=False,
        firmware_qualified=False, speedup_qualified=False, assembly_permitted=False,
        hardware_changed=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    result = review(a.output.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='cp-prefix-key-support',
        extra_tools=['audit_cp_prefix_key_support.py', 'prove_cp_cache_key_projection.py', Path(__file__).name])
    print(result['status'], flush=True)
