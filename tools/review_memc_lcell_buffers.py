#!/usr/bin/env python3
"""Preserve the corrected all-pin buffer experiment and its stopped predecessor.

The first experiment omitted the measured positive c10.p14 address pin and
was deliberately stopped for that source-coverage gap, not a timeout. Keep
its complete source/proofs and the verified terminal stop. The corrected
experiment covers all168 pins, including that exact critical path. Actual
LCELL preservation, fanout and whole-device timing remain separate gates.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess
from sil_netlist import load_board

from stage_memc_lcell_buffers import MEMC, SIM_MODEL, pins
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def envelope(directory, path):
    r = read(path)
    assert sha(Path(str(path)[:-5])) == r['archive_sha256']
    for row in r['members']:
        p = directory/row['path']
        assert sha(p) == row['sha256'] and p.stat().st_size == row['bytes']
    return r


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    stages = []
    for version in [1, 2]:
        d = BUILD/f'memc-lcell-short-native-stage-v{version}-20261024'
        m = read(d/'r-front-control-native.json')
        parent = Path(m['parent_stage'])
        old = read(parent/'r-front-control-native.json')
        assert sha(parent/'r-front-control-native.json') == m['parent_manifest_sha256']
        assert m['frequency_hz'] == 57142857 and m['sysper'] == 4 and not m['assembly_permitted']
        for name, digest in m['source_hashes'].items():
            assert sha(d/name) == digest
        changes = [name for name in old['source_hashes'] if (d/name).read_bytes().replace(
            m['project'].encode(), old['project'].encode()) != (parent/name).read_bytes()]
        assert changes == [MEMC] and set(m['source_hashes'])-set(old['source_hashes']) == {SIM_MODEL}
        for name in m['source_hashes']:
            if Path(name).suffix in ['.qsf', '.qip']:
                assert 'lcell_sim' not in (d/name).read_text()
        buffers = m['cache_tag_lcell_buffers']
        assert buffers['instances'] == (84 if version == 1 else 168)
        assert buffers['affected_RAMs'] == 21 and buffers['compared_address_bits'] == 168
        assert buffers['four_state_checks'] == 128 and buffers['model_excluded_from_Quartus']
        raw = (d/MEMC).read_text()
        restored = raw
        positive = (d/'address-proof/positive/actual.sv').read_text()
        for binding in buffers['bindings']:
            inverse = binding.get('inverted', True)
            expression = ('~' if inverse else '')+binding['source']
            declaration = f'  (* keep = "true" *) wire {binding["local"]};\n'
            instance = f'  lcell u_{binding["local"]}(.in({expression}),.out({binding["local"]}));\n'
            assert raw.count(declaration) == raw.count(instance) == 1
            assert positive.count(declaration) == positive.count(instance) == 1
            restored = restored.replace(declaration, '', 1).replace(instance, '', 1)
            before = f'.p{binding["pin"]}({binding["local"]})'
            assert restored.count(before) == 1
            restored = restored.replace(before, f'.p{binding["pin"]}({binding["original"]})', 1)
        assert restored == (parent/MEMC).read_text()
        target_packages = {b['package'] for b in buffers['bindings']}
        addresses = []
        for match in re.finditer(r'cell_MB7071H_snapshot u_(\w+) \(.*?\n  \);', raw, re.S):
            if match[1] in target_packages:
                row = pins(match[0])
                addresses.extend(row[str(pin)] for pin in range(7, 15))
        assert 'wire [167:0] next_address={'+','.join(addresses)+'};' in positive
        for proof in buffers['proofs']:
            q = d/'address-proof'/proof['variant']
            assert sha(q/'actual.sv') == proof['source_sha256'] and sha(q/'lcell.v') == proof['model_sha256']
            assert sha(q/'proof.log') == proof['proof_log_sha256'] and proof['returncode'] == 0
            expected = 'no model found: SUCCESS!' if proof['variant'] == 'positive' else 'model found: FAIL!'
            assert expected in (q/'proof.log').read_text()
        assert 'TAG_LCELL_FOUR_STATE_PASS checks=128 outputs=168' in (d/'address-proof/test.log').read_text()
        lint = BUILD/f'memc-lcell-short-native-lint-v{version}-20261024'
        l = read(lint/'run-state.json')
        assert l['status'] == 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
        assert l['stage_manifest_sha256'] == sha(d/'r-front-control-native.json')
        assert (lint/'lint.log').read_text().count('%Warning-UNOPTFLAT:') == 3
        if version == 2:
            critical = buffers['critical_positive_pin_buffered']
            assert critical['package'] == 'c10' and critical['pin'] == 14 and not critical['inverted']
            assert buffers['positive_pins'] == buffers['inverse_pins'] == 84
        else:
            assert not any(b['package'] == 'c10' and b['pin'] == 14 for b in buffers['bindings'])
        retain(d, out/d.name)
        retain(lint, out/lint.name)
        stages.append(dict(directory=d.name, manifest_sha256=sha(d/'r-front-control-native.json'),
                           instances=buffers['instances'], critical_positive_pin_covered=version == 2))
    stopped = BUILD/'memc-lcell-short-stopped-source-gap-v1-20261024'
    stop_envelope = BUILD/'memc-lcell-short-native-v1-20261024-stopped-source-gap.tgz.json'
    e = envelope(stopped, stop_envelope)
    assert e['status'] == 'TERMINAL_VERIFIED_SOURCE_GAP_STOP_COLLECTION'
    assert read(stopped/'run-state.json')['status'] == read(stopped/'physical-report/run-state.json')['status'] == 'TERMINAL_FAILED'
    assert not read(stopped/'physical-report/run-state.json')['actions']
    stop = read(stopped/'source-gap-stop.json')
    assert stop['status'] == 'STOP_REQUESTED_FOR_VERIFIED_SOURCE_COVERAGE_GAP'
    assert stop['signal_sent_to_stage_pid'] == 3079462
    assert read(stopped/'source-gap-input-readback.json')['actual'] == read(stopped/'r-front-control-native.json')['source_hashes']
    retain(stopped, out/stopped.name)
    shutil.copy2(stop_envelope, out/stop_envelope.name)
    live_dir = BUILD/'memc-lcell-short-live-audit-v1-20261024'
    live_envelope = BUILD/'memc-lcell-short-native-v2-20261024-live-audit-v1.tgz.json'
    envelope(live_dir, live_envelope)
    live = read(live_dir/'live-observation-v1.json')
    assert live['run_state']['status'] == 'RUNNING' and 'quartus_fit' in live['stage_command']
    assert live['manifest_sha256'] == stages[1]['manifest_sha256'] and live['source_hashes_verified'] == 534
    assert '93426 logic cells' in live['map_tail'] and '0 errors, 985 warnings' in live['map_tail']
    calibration = live_dir/'calibration/tag-buffer-query-calibration-v2-20261024/audit.log'
    assert 'u_d15|qb|q edges=321 ic=321' in calibration.read_text()
    retain(live_dir, out/live_dir.name)
    shutil.copy2(live_envelope, out/live_envelope.name)
    baseline = BUILD/'native-cedar-5c65-input-reset-v3-20261023'
    progress = [line.rstrip() for line in (baseline/'test.log').open() if line.startswith('NATIVE_PROGRESS ')]
    first_two = next(i for i, line in enumerate(progress) if 'tx=2 ' in line)
    transition = progress[max(0, first_two-1):first_two+10]
    assert 'tx=1 ' in transition[0] and 'host=2630000000 ' in transition[1]
    (out/'baseline-second-transmission-prefix.log').write_text('\n'.join(transition)+'\n')
    native = []
    for name in ['control-short-native-boot-v1-20261024', 'cp-prefix-balanced-native-boot-v1-20261024',
                 'cp-prefix-native-owned-cold-v1-20261024', 'native-cedar-5c65-input-reset-v3-20261023',
                 'native-cedar-queue-input-reset-v3-20261023']:
        source = BUILD/name
        dest = out/('observed-'+name)
        native.append(observation(source, dest))
        for name in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
            shutil.copy2(source/name, dest/name)
    result = dict(status='REVIEWED_ALL_PIN_LCELL_STAGING_AND_LIVE_FIT', scope=__doc__, stages=stages,
        intentionally_stopped_incomplete_source=stop, stopped_raw_members=len(e['members']), live_fit=live,
        direct_fanout_query_calibrated_at_physical_Q=321,
        original_keeper_single_CELL_arc_not_a_load_count=True,
        exploratory_help_and_name_regex_errors_retained=True, physical_buffer_count_from_report_names_unqualified=True,
        baseline_second_good_transmission_first_sample_host=2630000000,
        original_wait_PC_activity_changes_observed=True, packet_acceptance_or_firmware_boot_qualified=False,
        native_simulations=native, native_timing_qualified=False, physical_replication_qualified=False,
        firmware_qualified=False, speedup_qualified=False, assembly_permitted=False,
        hardware_changed=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def clustered(out):
    """Retain the failed all-pin fit and the smaller, source-equivalent fit."""
    assert not out.exists()
    out.mkdir(parents=True)
    failed = BUILD/'memc-lcell-short-failed-v2-20261024'
    receipt = BUILD/'memc-lcell-short-native-v2-20261024-failed.tgz.json'
    collected = envelope(failed, receipt)
    assert collected['status'] == 'TERMINAL_FAILED_NATIVE_FIT_RAW_COLLECTION'
    assert len(collected['members']) == 582 and not collected['completed_physical_timing_available']
    state = read(failed/'run-state.json')
    assert state['status'] == 'TERMINAL_FAILED'
    assert [(s['stage'], s['returncode']) for s in state['stages']] == [('map', 0), ('fit', 3)]
    readback = read(failed/'failed-fit-input-readback.json')
    manifest = read(failed/'r-front-control-native.json')
    assert readback['actual'] == manifest['source_hashes'] and len(readback['actual']) == 534
    assert readback['all_recorded_processes_absent'] and not readback['report_queries_executed']
    assert not readback['completed_physical_timing_available']
    for name, digest in manifest['source_hashes'].items():
        assert sha(failed/name) == digest
    for folder in ['physical-report', 'tag-buffer-report']:
        assert not read(failed/folder/'run-state.json')['actions']
    fit = (failed/'fit.log').read_text()
    all_unrouted = re.findall(r'Info \(170139\): Signal "([^"]+)"', fit)
    assert 'Error (170143)' in fit and 'Error (11802)' in fit
    unrouted = re.findall(r'Info \(170139\): Signal "([^"]+)"', fit.split('Error (170143)', 1)[1])
    assert len(unrouted) == 2
    assert {n.rsplit('|', 1)[1] for n in unrouted} == {'cache_data[0]', 'cache_data[5]'}
    assert all('msa_ddram:b_msaOdd|dorado_storage_group:u_storage_7|' in n for n in unrouted)
    summary = (failed/'core/output_files/Dorado.fit.summary').read_text()
    assert 'Fitter Status : Failed' in summary and '39,337 / 41,910 ( 94 % )' in summary
    assert not (failed/'core/output_files/Dorado.sta.summary').exists()
    retain(failed, out/failed.name)
    shutil.copy2(receipt, out/receipt.name)

    stage = BUILD/'memc-lcell-short-native-stage-v3-20261024'
    m = read(stage/'r-front-control-native.json')
    parent = Path(m['parent_stage'])
    old = read(parent/'r-front-control-native.json')
    assert sha(parent/'r-front-control-native.json') == m['parent_manifest_sha256']
    assert m['frequency_hz'] == 57142857 and m['sysper'] == 4 and not m['assembly_permitted']
    assert len(m['source_hashes']) == 534
    for name, digest in m['source_hashes'].items():
        assert sha(stage/name) == digest
    changes = [name for name in old['source_hashes'] if (stage/name).read_bytes().replace(
        m['project'].encode(), old['project'].encode()) != (parent/name).read_bytes()]
    assert changes == [MEMC] and set(m['source_hashes'])-set(old['source_hashes']) == {SIM_MODEL}
    for name in m['source_hashes']:
        if Path(name).suffix in ['.qsf', '.qip']:
            assert 'lcell_sim' not in (stage/name).read_text()
    buffers = m['cache_tag_lcell_buffers']
    assert buffers['instances'] == 72 and buffers['cluster_size'] == 4
    assert buffers['affected_RAMs'] == 21 and buffers['compared_address_bits'] == 168
    assert buffers['positive_pins'] == buffers['inverse_pins'] == 84
    assert buffers['maximum_RAMs_per_buffer'] == 4
    assert buffers['four_state_checks'] == 128 and buffers['model_excluded_from_Quartus']
    assert not buffers['SRAM_ports_or_capture_changes'] and not buffers['extra_saved_state_or_guest_cycles']
    raw = (stage/MEMC).read_text()
    positive = (stage/'address-proof/positive/actual.sv').read_text()
    original = (parent/MEMC).read_text()
    restored = raw
    locals_ = {}
    ram_ports = {}
    for binding in buffers['bindings']:
        local = binding['local']
        expression = ('~' if binding['inverted'] else '')+binding['source']
        assert local not in locals_ or locals_[local] == expression
        locals_[local] = expression
        key = binding['package'], binding['pin']
        assert key not in ram_ports
        ram_ports[key] = binding
    assert len(ram_ports) == 168 and len(locals_) == 72
    for local, expression in locals_.items():
        declaration = f'  (* keep = "true" *) wire {local};\n'
        instance = f'  lcell u_{local}(.in({expression}),.out({local}));\n'
        assert raw.count(declaration) == raw.count(instance) == 1
        assert positive.count(declaration) == positive.count(instance) == 1
        restored = restored.replace(declaration, '', 1).replace(instance, '', 1)
        assert len({p for (p, pin), b in ram_ports.items() if b['local'] == local}) <= 4
    addresses = []
    for match in list(re.finditer(r'cell_MB7071H_snapshot u_(\w+) \(.*?\n  \);', raw, re.S)):
        package = match[1]
        if package not in {p for p, pin in ram_ports}:
            continue
        block = match[0]
        row = pins(block)
        addresses.extend(row[str(pin)] for pin in range(7, 15))
        for pin in range(7, 15):
            binding = ram_ports[package, pin]
            assert row[str(pin)] == binding['local']
            block = block.replace(f'.p{pin}({binding["local"]})', f'.p{pin}({binding["original"]})', 1)
        assert block in original
        restored = restored.replace(match[0], block, 1)
    assert restored == original
    assert 'wire [167:0] next_address={'+','.join(addresses)+'};' in positive
    critical = buffers['critical_positive_pin_buffered']
    assert critical['package'] == 'c10' and critical['pin'] == 14 and not critical['inverted']
    for proof in buffers['proofs']:
        q = stage/'address-proof'/proof['variant']
        assert sha(q/'actual.sv') == proof['source_sha256'] and sha(q/'lcell.v') == proof['model_sha256']
        assert sha(q/'proof.log') == proof['proof_log_sha256'] and proof['returncode'] == 0
        expected = 'no model found: SUCCESS!' if proof['variant'] == 'positive' else 'model found: FAIL!'
        assert expected in (q/'proof.log').read_text()
    assert 'TAG_LCELL_FOUR_STATE_PASS checks=128 outputs=168' in (stage/'address-proof/test.log').read_text()
    lint = BUILD/'memc-lcell-short-native-lint-v3-20261024'
    l = read(lint/'run-state.json')
    assert l['status'] == 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
    assert l['stage_manifest_sha256'] == sha(stage/'r-front-control-native.json')
    assert (lint/'lint.log').read_text().count('%Warning-UNOPTFLAT:') == 3
    retain(stage, out/stage.name)
    retain(lint, out/lint.name)
    live_path = BUILD/'memc-lcell-short-native-v3-live-v1-20261024.json'
    live = read(live_path)
    assert live['run_state']['status'] == 'RUNNING' and 'quartus_fit' in live['stage_command']
    assert live['source_hashes_verified'] == 534 and live['manifest_sha256'] == sha(stage/'r-front-control-native.json')
    assert live['stage_cwd'] == m['project']+'/core'
    assert len(live['reporters']) == 2 and all(not row['state']['actions'] for row in live['reporters'])
    shutil.copy2(live_path, out/live_path.name)

    terminal = BUILD/'cp-prefix-native-owned-cold-v1-20261024'
    t = read(terminal/'run-state.json')
    assert t['status'] == 'TERMINAL_NATIVE_MEMORY_DIAGNOSTIC'
    assert all(a['returncode'] == 0 for a in t['actions'])
    assert t['actions'][-1]['pid'] == 45198
    ps = subprocess.run(['ps', '-p', str(t['driver_pid'])+',45198', '-o', 'pid=,command='], capture_output=True, text=True)
    assert ps.returncode == 1 and not ps.stdout.strip()
    provenance = json.loads(gzip.decompress((terminal/'provenance.json.gz').read_bytes()))
    assert provenance['host_limit'] == 1000000000 and not provenance['original_run_limits_changed']
    assert not provenance['medium_runtime_writes_persisted']
    assert (terminal/'host-cycle-limit.txt').read_text().strip() == '1000000000'
    assert (terminal/'window-control.txt').read_text() == provenance['window_initial_control']
    for name, digest in provenance['inputs'].items():
        assert sha(terminal/'inputs'/name) == digest
    lines = (terminal/'test.log').read_text().splitlines()
    final = {prefix: next(row for row in reversed(lines) if row.startswith(prefix)) for prefix in [
        'NATIVE_PROGRESS ', 'NATIVE_TRAFFIC_PROGRESS ', 'NATIVE_CP_PAUSE ', 'NATIVE844_BASEBOARD ']}
    assert all('host=1000000000 ' in row for row in final.values())
    assert 'guest=505472968 ' in final['NATIVE_PROGRESS ']
    assert 'issues=0 ' in final['NATIVE_TRAFFIC_PROGRESS '] and 'BFN=00000000000' in final['NATIVE_TRAFFIC_PROGRESS ']
    assert 'pauses=48480 snapshots=48480 ' in final['NATIVE_CP_PAUSE ']
    assert not any(row.startswith('NATIVE844_PREFIX') for row in lines)
    retain(terminal, out/terminal.name)
    rejected = BUILD/'memc-clustered-buffers-review-v1-20261024'
    assert read(rejected/'failure.json')['status'] == 'REJECTED_REVIEW_PARSER_MIXED_FINAL_AND_INTERMEDIATE_ROUTER_ATTEMPTS'
    retain(rejected, out/('rejected-'+rejected.name))
    rejected_v2 = BUILD/'memc-clustered-buffers-review-v2-20261024'
    assert read(rejected_v2/'failure.json')['status'] == 'REJECTED_REVIEW_PARSER_TREATED_CARRIED_SOURCE_REPORT_AS_NEW_TIMING'
    retain(rejected_v2, out/('rejected-'+rejected_v2.name))
    native = []
    for name in ['control-short-native-boot-v1-20261024', 'cp-prefix-balanced-native-boot-v1-20261024',
                 'native-cedar-5c65-input-reset-v3-20261023', 'native-cedar-queue-input-reset-v3-20261023']:
        source = BUILD/name
        dest = out/('observed-'+name)
        native.append(observation(source, dest))
        assert (source/'host-cycle-limit.txt').read_text().strip() == '5000000000'
        for name in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
            shutil.copy2(source/name, dest/name)
    result = dict(status='REVIEWED_FAILED_ALL_PIN_FIT_AND_CLUSTERED_BUFFER_SOURCE',
        rejected_fit=dict(directory=failed.name, raw_members=582, ALMs=39337, utilization_percent=94,
            unrouted_signals=unrouted, all_intermediate_and_final_unrouted=all_unrouted,
            completed_physical_timing_available=False),
        clustered_source=dict(directory=stage.name, source_hashes_verified=534,
            manifest_sha256=sha(stage/'r-front-control-native.json'), RTL_instances=72,
            affected_address_pins=168, maximum_RAMs_per_buffer=4, exact_critical_pin_covered=True,
            arbitrary_binary_equivalence=True, wrong_polarity_counterexample=True, four_state_cases=128),
        frozen_live_fit=live, startup_diagnostic=dict(directory=terminal.name, original_limit=1000000000,
            status=t['status'], final=final, process_handles_absent=True, startup_only=True,
            CPU_instruction_issue_or_firmware_boot_qualified=False),
        ongoing_original_bounded_native_tests=native,
        physical_replication_qualified=False, native_timing_qualified=False, speedup_qualified=False,
        firmware_qualified=False, hardware_changed=False, assembly_permitted=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def address_windows(out):
    """Bind the remaining address-latch window work to the original pins."""
    assert not out.exists()
    out.mkdir(parents=True)
    wl = ROOT/'chm/sil/MemC-Rev-Be.dm!1_/MemC-Rev-Be.wl'
    board = load_board(str(wl))

    def row(package):
        return {p['pin']: name for name, net in board.nets.items() for p in net['pins'] if p['pkg'] == package}

    h12 = row('h12')
    assert board.packages['h12']['type'] == 'SE10210'
    assert {pin: h12[pin] for pin in [2, 3, 5, 10, 11]} == {
        2: 'preFHCb', 3: 'preFHCa', 5: "ppSH'", 10: "ppclk2'a", 11: "preFH'x"}
    assert all(pin not in h12 for pin in [6, 7, 9])
    latches = []
    for package in ['a14', 'a15', 'b14', 'b15', 'c14', 'c15', 'd14', 'd15']:
        p = row(package)
        assert board.packages[package]['type'] == 'MC1668'
        clock = 'preFHCa' if package.endswith('14') else 'preFHCb'
        assert p[7] == p[9] == clock
        for side, d, c, s, r, q, qn in [('a', 6, 7, 5, 4, 2, 3), ('b', 11, 9, 12, 13, 15, 14)]:
            if package.endswith('14'):
                assert s not in p and p[r].startswith('sAad.')
            else:
                assert r not in p and p[s].startswith('sAad.')
            latches.append(dict(package=package, side=side, D=p[d], C=p[c],
                S=p.get(s, 'unwired MECL pulldown=0'), R=p.get(r, 'unwired MECL pulldown=0'),
                Q=p[q], Qn=p[qn], D_drivers=[pin for pin in board.nets[p[d]]['pins'] if pin['dir'] == 'out']))
    high_data = [r for r in latches if r['D'] == 'dAad.7']
    assert high_data and all({p['pkg'] for p in r['D_drivers']} == {'b02', '#5b42'} for r in high_data)
    stage = BUILD/'control-short-native-stage-v1-20261024'
    cell = stage/'src/fpga/mister/snapshot_native_processor/cell_MC1668_snapshot.v'
    source = cell.read_text()
    assert 'if (p7)          qa <= p6;' in source and 'if (p9)          qb <= p11;' in source
    assert 'assign p2 =  qa;' in source and 'always @(posedge state_host_clk)' in source
    copied = {}
    for p in [wl, wl.with_suffix('.lc'), cell, ROOT/'verilog/cells/cell_SE10210.v']:
        shutil.copy2(p, out/p.name)
        copied[p.name] = sha(out/p.name)
    result = dict(status='SOURCE_BOUND_MEMC_ADDRESS_WINDOW_AUDIT', inputs=copied,
        original_latches=latches, original_h12_pins=h12,
        clock_equations={'preFHCb': "ppSH'", 'preFHCa': "ppclk2'a OR preFH'x"},
        clock_sources={name: row(name) for name in ['l02', 'l01', 'l06']},
        original_behavior='C high: transparent D, S/R ignored. C low: low/complement bits have R=sAad, S=0; high/positive bits have S=sAad, R=0; otherwise hold.',
        native_approximation='Canonical Q samples D at enabled host edges while C is high; output pins expose stored Q.',
        required_next_work='Preserve the two different transparent windows, wired-OR D and closed Set before folding address visibility/tag lookup/Hold captures into four15ns intervals. Derive actual pre/post-source ordering before any bypass, prediction or registration.',
        new_reset_or_unconditional_capture_authorized=False, source_phases_integrated=False,
        FPGA_timing_or_speed_qualified=False, hardware_changed=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    failed = BUILD/'memc-address-window-audit-v1-20261024'
    assert read(failed/'failure.json')['status'] == 'REJECTED_INCORRECT_ALL_RESET_PINS_UNWIRED_ASSUMPTION'
    retain(failed, out/('rejected-'+failed.name))
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--address-windows', action='store_true')
    p.add_argument('--clustered', action='store_true')
    a = p.parse_args()
    assert not (a.address_windows and a.clustered)
    r = (clustered(a.output.resolve()) if a.clustered else
         address_windows(a.output.resolve()) if a.address_windows else review(a.output.resolve()))
    label = 'memc-clustered-buffers' if a.clustered else 'memc-address-windows' if a.address_windows else 'memc-lcell-buffers'
    append_archive(a.archive.resolve(), a.output.resolve(), [], label=label, extra_tools=[
        'stage_memc_lcell_buffers.py', 'report_memc_tag_buffers.py', 'collect_failed_native_fit.py', Path(__file__).name])
    print(r['status'], flush=True)
