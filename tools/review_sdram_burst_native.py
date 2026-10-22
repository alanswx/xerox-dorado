#!/usr/bin/env python3
"""Review the BL8 native fit against its qualified MC1668 parent.

Require exact source deltas, unchanged RAM behavior/parameters, all original
timing categories and the new raw-buffer register/path inventory. This is
preassembly evidence; original-firmware boot and measured speed are separate.
"""
import argparse
from collections import Counter
import gzip
import hashlib
import json
from pathlib import Path
import re
import sys

from audit_r_front_ready_native import ram_table, memory_parameters
from stage_native_sdram_burst import WRAPPER, BACKEND
import stage_boot_fault_observer as observer
import stage_storage_selftest_native as selftest
import stage_storage_burst_observer as burst_observer
import stage_storage_read_capture as read_capture
from storage_capture_source import legacy_capture, legacy_read_dqm_model

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'fpga/mister/snapshot'))
import audit_native_dsketh as readers
from audit_native_conta import ucp


def raw(path):
    if path.exists():
        return path.read_bytes()
    return gzip.decompress(Path(str(path) + '.gz').read_bytes())


def sha(data):
    return hashlib.sha256(data).hexdigest()


def json_at(path):
    return json.loads(raw(path))


def memory_comparison(before, after, fitted=False, allow_host_video_auto_ids=False):
    title = 'Fitter RAM Summary' if fitted else 'Analysis & Synthesis RAM Summary'
    previous, current = ram_table(before, title), ram_table(after, title)
    renamed = {}
    if allow_host_video_auto_ids:
        # Adding the observer changes two global vendor auto-instance IDs.
        # Keep the full logical video pipeline path and every geometry/mode/
        # register field. Never normalize any Dorado memory instance name.
        prefixes = ['ascal:ascal|altshift_taps:o_dev_rtl_0|shift_taps_5gv:auto_generated|',
                    'vga_out:vga_scaler_out|altshift_taps:din1_rtl_0|shift_taps_quu:auto_generated|']
        def normalized(rows):
            result = {}
            for name, row in rows.items():
                canonical = name
                for prefix in prefixes:
                    if name.startswith(prefix):
                        canonical, count = re.subn(r'^' + re.escape(prefix) + r'altsyncram_[a-z0-9]+:', prefix + 'altsyncram_AUTO:', name)
                        assert count == 1
                        renamed[name] = canonical
                assert canonical not in result
                result[canonical] = dict(row, Name=canonical)
            return result
        previous, current = normalized(previous), normalized(current)
    assert current.keys() == previous.keys() and len(current) == 170, 'RAM inventory differs'
    roms = {n for n in current if '|cell_i2716:' in n}
    assert len(roms) == 4
    for name, row in current.items():
        ignored = {'Location'} if fitted else set()
        if fitted and name in roms:
            # Quartus packs the unchanged four ROMs into a shared seven-M10K
            # pool; each logical row lists only the blocks carrying its bits.
            ignored.add('M10K blocks')
        assert {k: v for k, v in row.items() if k not in ignored} == {
            k: v for k, v in previous[name].items() if k not in ignored}, ('RAM behavior differs', name)
    if fitted:
        sites = set()
        for name in roms:
            row = current[name]
            assert row['Mode'] == 'ROM' and row['Implementation Bits'] == '16384'
            locations = [x.strip() for x in row['Location'].split(',')]
            assert len(locations) == len(set(locations)) == int(row['M10K blocks'])
            assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+', n) for n in locations)
            sites.update(locations)
        assert len(sites) == 7, 'BaseBoard shared ROM allocation differs'
        assert not any(set(r['Location'].split(', ')) & sites for n, r in current.items() if n not in roms)
    else:
        assert memory_parameters(before) == memory_parameters(after), 'RAM parameters differ'
        assert len(memory_parameters(after)) == 165
    return dict(rows=len(current), identical_behavior=True, repacked_ROM_pool_blocks=7 if fitted else None,
                host_video_auto_instance_ids=renamed)


def review(root, parent, parent_qualification, parent_reports, observer_component=None, selftest_component=None, selftest_full_component=None, burst_observer_component=None, burst_observer_full_component=None, cas_reference=None, read_capture_matrix=None, read_capture_board=None):
    record = json_at(root / 'sdram-burst-native.json')
    prior = json_at(parent / 'mc1668-candidate.json')
    qualified = json_at(parent_qualification)
    assert qualified['status'] == 'PASS_NATIVE_MC1668_BUILD'
    assert qualified['source_hashes'] == prior['source_hashes']
    assert record['parent_manifest_sha256'] == sha(raw(parent / 'mc1668-candidate.json'))
    assert record['parent_qualification_sha256'] == sha(raw(parent_qualification))
    assert record['frequency_mhz'] == 50 and record['sysper'] == 4 and record['state_admin'] == 'INACTIVE'
    has_observer = record.get('passive_boot_fault_observer', False)
    has_selftest = record.get('raw_storage_selftest', False)
    has_burst_observer = record.get('raw_burst_observer', False)
    has_correct_capture = record.get('corrected_sdram_read_capture', False)
    assert not has_correct_capture or has_burst_observer
    assert not has_burst_observer or has_selftest
    assert not has_selftest or has_observer
    assert record['cache_lines'] == 4 and len(record['source_hashes']) == 499 + int(has_observer) + int(has_selftest)
    actual = {n: sha(raw(root / n)) for n in record['source_hashes']}
    assert actual == record['source_hashes'], 'Compiled input changed'
    plain_backend = legacy_capture((ROOT / 'fpga/mister/dorado_storage_sdram_burst.sv').read_bytes())
    expected_backend = burst_observer.expose(BACKEND, plain_backend) if has_burst_observer else plain_backend
    if has_correct_capture:expected_backend=read_capture.correct_capture(expected_backend)
    assert actual[BACKEND] == sha(expected_backend)
    observer_proof = None
    if has_observer:
        assert observer_component is not None, 'Missing tested passive observer component'
        s = json_at(observer_component / 'run-state.json')
        p = json_at(observer_component / 'provenance.json')
        assert s['status'] == 'PASS_PASSIVE_BOOT_FAULT_MONITOR'
        assert len(s['actions']) == 10
        negative = {'premature-arm-test', 'late-source-test', 'overwrite-first-test'}
        assert all((a['returncode'] != 0) if a['stage'] in negative else a['returncode'] == 0 for a in s['actions'])
        for n, h in p['inputs'].items():
            assert sha(raw(observer_component / 'inputs' / n)) == h
        assert actual[observer.OBSERVER] == p['inputs']['monitor.sv'] == sha((ROOT / 'fpga/mister/dorado_boot_fault_monitor.sv').read_bytes())
        observer_proof = p
    selftest_proof = None
    selftest_full_proof = None
    if has_selftest:
        assert selftest_component is not None, 'Missing tested raw-memory diagnostic'
        state_test = json_at(selftest_component / 'run-state.json')
        selftest_proof = json_at(selftest_component / 'provenance.json')
        assert state_test['status'] == 'PASS_RAW_MEMORY_SELFTEST_PIN_COMPONENT'
        assert len(state_test['actions']) == 8
        assert all((r['returncode'] != 0) if r['stage'] == 'compare-removed-test' else r['returncode'] == 0 for r in state_test['actions'])
        for n, h in selftest_proof['inputs'].items():
            assert sha(raw(selftest_component / 'inputs' / n)) == h
        assert actual[selftest.SELFTEST] == selftest_proof['inputs']['fpga/mister/dorado_storage_selftest.sv'] == sha((ROOT / 'fpga/mister/dorado_storage_selftest.sv').read_bytes())
        assert record['selftest_component_provenance_sha256'] == sha(raw(selftest_component / 'provenance.json'))
        assert selftest_full_component is not None, 'Missing complete 23-bit raw-memory sweep'
        full_state = json_at(selftest_full_component / 'run-state.json')
        selftest_full_proof = json_at(selftest_full_component / 'provenance.json')
        assert full_state['status'] == 'PASS_RAW_MEMORY_SELFTEST_PIN_COMPONENT' and full_state['address_bits'] == 23
        assert len(full_state['actions']) == 2 and all(a['returncode'] == 0 for a in full_state['actions'])
        for n, h in selftest_full_proof['inputs'].items():
            assert sha(raw(selftest_full_component / 'inputs' / n)) == h
        assert actual[selftest.SELFTEST] == selftest_full_proof['inputs']['fpga/mister/dorado_storage_selftest.sv']
        assert sha(plain_backend) == selftest_full_proof['inputs']['fpga/mister/dorado_storage_sdram_burst.sv']
        full_log = raw(selftest_full_component / 'fault-0.log').decode()
        assert 'STORAGE-SELFTEST PASS fault=0 bits=23 hz=50000000 requests=50331648 completions=50331648' in full_log

    burst_observer_proof = None
    burst_observer_full_proof = None
    if has_burst_observer:
        assert burst_observer_component is not None and burst_observer_full_component is not None
        for directory, full in [(burst_observer_component,False),(burst_observer_full_component,True)]:
            state_trace = json_at(directory / 'run-state.json')
            proof = json_at(directory / 'provenance.json')
            assert state_trace['status'] == 'PASS_RAW_MEMORY_BURST_TRACE_PIN_COMPONENT'
            assert state_trace['burst_observer'] and state_trace['address_bits'] == (23 if full else 14)
            assert state_trace.get('read_capture_corrected',False) == has_correct_capture
            assert len(state_trace['actions']) == (2 if full else 12 if has_correct_capture else 10)
            negatives = {'compare-removed-test','burst-lanes-reversed-test','legacy-late-capture-test'}
            assert all(a['returncode'] != 0 if a['stage'] in negatives else a['returncode'] == 0 for a in state_trace['actions'])
            for n,h in proof['inputs'].items():assert sha(raw(directory / 'inputs' / n)) == h
            assert actual[BACKEND] == proof['inputs']['fpga/mister/dorado_storage_sdram_burst.sv']
            if full:
                assert 'STORAGE-SELFTEST PASS fault=0 bits=23 hz=50000000 requests=50331648 completions=50331648' in raw(directory / 'fault-0.log').decode()
                burst_observer_full_proof = proof
            else:
                assert record['burst_observer_component_provenance_sha256'] == sha(raw(directory / 'provenance.json'))
                burst_observer_proof = proof
    read_capture_qualification = None
    if has_correct_capture:
        assert cas_reference is not None and read_capture_matrix is not None and read_capture_board is not None, 'Missing independently grounded corrected-read qualification'
        def component(directory,status):
            state = json_at(directory / 'run-state.json');proof = json_at(directory / 'provenance.json')
            assert state['status'] == status
            for n,h in proof['inputs'].items():assert sha(raw(directory / 'inputs' / n)) == h
            return state,proof
        cs,cp = component(cas_reference,'PASS_CAS_REFERENCE_VENDOR_AND_CORRECTED_LOCAL_LEGACY_REJECTED')
        assert len(cs['actions']) == 6 and all(a['returncode'] != 0 if a['stage']=='legacy-local-test' else a['returncode']==0 for a in cs['actions'])
        for label in ['vendor-test','corrected-local-test']:
            text = raw(cas_reference / (label+'.log')).decode()
            assert 'delay_ns=26.000' in text and 'delay_ns=46.000' in text and 'CAS_REFERENCE PASS CAS2_AND_CAS3' in text
        text = raw(cas_reference / 'legacy-local-test.log').decode()
        assert 'CAS_LAUNCH_MISMATCH cas=2 delta=46.000' in text and 'CAS_LAUNCH_MISMATCH cas=3 delta=66.000' in text
        model_key='verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv'
        current_model = (ROOT/model_key).read_bytes()
        # Old receipts retain their exact CAS-qualified model. The only
        # permitted alternative is its known extra DQM launch stage; that
        # difference was independently exposed by the later full-range test.
        assert cp['inputs']['corrected-xsds-model.sv'] in {sha(current_model), sha(legacy_read_dqm_model(current_model))}
        assert cp['inputs']['corrected-xsds-model.sv'] == burst_observer_proof['inputs'][model_key] == burst_observer_full_proof['inputs'][model_key]
        ms,mp = component(read_capture_matrix,'PASS_ISOLATED_SDRAM_BURST_PIN_MODEL')
        assert mp['read_capture_corrected'] and len(ms['actions']) == 26
        negatives={'stale-write-test','orphan-fill-test','early-capture-test','single-beat-mode-test','short-rcd-test','old-model-hold-glitch-test'}
        assert all(a['returncode'] != 0 if a['stage'] in negatives else a['returncode']==0 for a in ms['actions'])
        assert mp['inputs'][model_key] == cp['inputs']['corrected-xsds-model.sv']
        assert sha(burst_observer.expose(BACKEND,raw(read_capture_matrix/'inputs/fpga/mister/dorado_storage_sdram_burst.sv'))) == actual[BACKEND]
        bs,bp = component(read_capture_board,'PASS_NATIVE_MSA_BL1_BL8_PHYSICAL_MODEL')
        assert bp['read_capture_corrected'] and len(bs['actions'])==4 and all(a['returncode']==0 for a in bs['actions'])
        assert bp['inputs']['xsds_model.sv'] == cp['inputs']['corrected-xsds-model.sv']
        for label in ['baseline','burst']:
            text = raw(read_capture_board/(label+'-test.log')).decode()
            assert re.search(r'12012\s+complete-board cycles',text) and 'all 9437184 stored bits identical' in text
        read_capture_qualification=dict(cas_reference=cp,pin_matrix=mp,original_board=bp)
    old_type = 'dorado_storage_sdram #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ)) u_sdram('
    new_type = 'dorado_storage_sdram_burst #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ),.CACHE_LINES(4)) u_sdram('
    for name, digest in prior['source_hashes'].items():
        value = raw(parent / name)
        assert sha(value) == digest, ('Qualified parent changed', name)
        if Path(name).suffix in ('.qsf', '.qip', '.qpf', '.sdc', '.tcl'):
            value = value.replace(prior['project'].encode(), record['project'].encode())
        current = raw(root / name)
        if name == WRAPPER:
            assert value.count(old_type.encode()) == 1
            value = value.replace(old_type.encode(), new_type.encode())
        elif name == 'core/Dorado.qsf':
            value += ('\n# Raw BL8 buffer only; unchanged CPU/cache/map/ECC and clocks.\n'
                      'set_global_assignment -name SYSTEMVERILOG_FILE ' + str(Path(record['project']) / BACKEND) + '\n').encode()
        if has_observer:
            value = observer.expose(name, value)
            if name == 'core/Dorado.qsf':
                value += ('\n# Passive original boot first-error diagnostic.\n'
                          'set_global_assignment -name SYSTEMVERILOG_FILE ' + str(Path(record['project']) / observer.OBSERVER) + '\n').encode()
        if has_selftest:
            value = selftest.expose(name, value)
            if name == 'core/Dorado.qsf':
                value += ('\n# Pre-guest raw-memory diagnostic and verified zero-fill.\nset_global_assignment -name SYSTEMVERILOG_FILE ' + str(Path(record['project']) / selftest.SELFTEST) + '\n').encode()
        if has_burst_observer:value = burst_observer.expose(name, value)
        assert current == value, ('Unintended source/config delta', name)
    assert set(actual) - set(prior['source_hashes']) == {BACKEND} | ({observer.OBSERVER} if has_observer else set()) | ({selftest.SELFTEST} if has_selftest else set())
    state = json_at(root / 'run-state.json')
    assert state['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [r['stage'] for r in state['stages']] == ['map', 'fit', 'sta', 'clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings']
    assert all(r['returncode'] == 0 for r in state['stages'])
    extra = json_at(root / 'burst-review-v2-state.json')
    assert extra['status'] == 'TERMINAL_BURST_REPORTS_REVIEW_REQUIRED'
    assert len(extra['actions']) == 1 and extra['actions'][0]['returncode'] == 0
    tcl_name = 'audit_storage_burst_observer_native.tcl' if has_burst_observer else 'audit_storage_selftest_native.tcl' if has_selftest else 'audit_boot_fault_native.tcl' if has_observer else 'audit_sdram_burst_native.tcl'
    assert extra['tcl_sha256'] == sha(raw(root / tcl_name)) == sha((ROOT / 'tools' / tcl_name).read_bytes())
    collected = json_at(root / 'collected-input-check.json')
    assert collected['actual'] == actual
    expected_pids = {state['builder_pid'], extra['driver_pid']} | {r['pid'] for r in state['stages'] + extra['actions']}
    if has_burst_observer:
        fitted_state_early = json_at(root / 'register-inventory-v1/state.json')
        expected_pids |= {fitted_state_early['driver_pid'],fitted_state_early['child_pid']}
    assert set(collected['absent_pids']) == expected_pids
    core = root / 'core'
    mapping = raw(core / 'output_files/Dorado.map.rpt').decode()
    fitted = raw(core / 'output_files/Dorado.fit.rpt').decode('latin-1')
    memories = dict(synthesis=memory_comparison(raw(parent_reports / 'Dorado.map.rpt').decode(), mapping, allow_host_video_auto_ids=has_observer),
                    physical=memory_comparison(raw(parent_reports / 'Dorado.fit.rpt').decode('latin-1'), fitted, True, has_observer))
    manifest = json_at(root / 'src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD']
    memories['cache'] = readers.native_cache_physical(fitted, manifest, 'independent40')
    assert memories['cache']['unique_m10ks'] == 40
    sources = mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n', 1)[0]
    used = set()
    for line in sources.splitlines():
        fields = [v.strip() for v in line.split(';')[1:-1]]
        if len(fields) == 5 and fields[1] == 'yes':
            used.add(str(Path(fields[3]).relative_to(record['project'])) if fields[3].startswith(record['project'] + '/') else fields[3])
    assert {WRAPPER, BACKEND} <= used, 'Backend/wrapper absent from synthesized netlist'
    if has_observer:
        assert {observer.OBSERVER, observer.CONTB, observer.BACKPLANE, observer.WRAPPER, observer.TOP} <= used, 'Passive diagnostic patched an unused source'
    if has_selftest:
        assert selftest.SELFTEST in used, 'Raw-memory diagnostic absent from synthesized netlist'
    paths = []
    for corner in sorted(readers.CORNERS):
        for mode in readers.MODES:
            name = f'native59-{mode}-{corner}.rpt'
            paths.append(readers.timed(raw(core / name).decode(), name))
        assert ucp(raw(core / f'native59-ucp-{corner}.rpt').decode()) == ucp(raw(parent_reports.parent / f'native59-ucp-{corner}.rpt').decode()), ('Unconstrained coverage differs', corner)
    assert len(paths) == 196
    checks = re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)', raw(core / 'output_files/Dorado.sta.summary').decode())
    assert len(checks) == 31 and min(float(v) for _, v in checks) >= 0
    log = raw(root / 'burst-review-v2.log').decode()
    counts = re.findall(r'^BURST_REGISTER_COUNT (\S+) (\S+) (\d+)$', log, re.M)
    assert len(counts) == 20 and {(c, n) for c, n, _ in counts} == {(c, n) for c in readers.CORNERS for n in ['words', 'tags', 'valid', 'capture', 'response']}
    # Every original MSA group consumes at most twelve of the raw halfword's
    # bits. Quartus prunes the four unused high data bits; 23 raw address bits
    # leave twenty effective tag bits per line. Require the exact fit here.
    msa = raw(root / 'src/build/rtl-executive/native16-tx-cable/msa_ddram.v').decode()
    widths = list(map(int, re.findall(r'dorado_storage_group #\(\.ADDR_BITS\(16\), \.WIDTH\((\d+)\)\)', msa)))
    assert len(widths) == 16 and max(widths) == 12
    registers = re.findall(r'^BURST_REGISTER (.+)$', log, re.M)
    assert len(registers) == len(set(registers)) and len(registers) >= 492
    expected_counts = dict(words=384, tags=80, valid=4, capture=12, response=12)
    response_registers = [n for n in registers if '|rsp_data' in n]
    response_bits = {int(m[1]) for n in response_registers if (m := re.search(r'\|rsp_data\[(\d+)\]$', n))}
    assert response_bits == set(range(12)), 'Missing logical raw response bit'
    response_copies = [n for n in response_registers if n.endswith('~DUPLICATE')]
    if has_observer:
        # Physical optimization makes one extra copy of the unchanged raw
        # response bit1 in this placement. Preserve all twelve originals and
        # accept exactly that named fitter copy, with all its paths timed.
        assert len(response_copies) == 1 and response_copies[0].endswith('|rsp_data[' + ('6' if has_correct_capture else '1' if has_burst_observer else '5' if has_selftest else '1') + ']~DUPLICATE')
        expected_counts['response'] = 13
    else:
        assert not response_copies
    assert len(response_registers) == expected_counts['response']
    if has_correct_capture:
        valid_registers=[n.rsplit('|',1)[-1] for n in registers if '|line_valid' in n]
        assert set(valid_registers)=={f'line_valid[{i}]'for i in range(4)}|{'line_valid[3]~DUPLICATE'} and len(valid_registers)==5
        expected_counts['valid']=5
    assert all(int(count) == expected_counts[n] for c, n, count in counts), ('Buffer physical register coverage differs', counts)
    new_paths = []
    for corner in sorted(readers.CORNERS):
        for label in ['internal', 'fill', 'cached-read', 'tag-select', 'input-capture', 'request', 'completion']:
            for mode in ['setup', 'hold']:
                name = f'burst-{label}-{mode}-{corner}.rpt'
                new_paths.append(readers.timed(raw(core / name).decode(), name))
    assert len(new_paths) == 56
    observer_paths = []
    if has_observer:
        inventory = re.findall(r'^BOOT_FAULT_REGISTERS (\S+) (\d+)$', log, re.M)
        assert len(inventory) == 4 and {c for c, _ in inventory} == readers.CORNERS
        assert all(int(count) == (82 if has_burst_observer else 83 if has_selftest else 82) for _, count in inventory)
        for corner in sorted(readers.CORNERS):
            for mode in ['setup', 'hold']:
                name = f'burst-first-fault-{mode}-{corner}.rpt'
                observer_paths.append(readers.timed(raw(core / name).decode(), name))
        assert len(observer_paths) == 8
    selftest_paths = []
    selftest_registers = []
    trace_alias_filter_warning = None
    if has_selftest:
        inventory = re.findall(r'^STORAGE_SELFTEST_REGISTERS (\S+) (\d+)$', log, re.M)
        assert len(inventory) == 4 and {c for c, _ in inventory} == readers.CORNERS
        selftest_registers = re.findall(r'^STORAGE_SELFTEST_REGISTER (.+)$', log, re.M)
        assert len(selftest_registers) == len(set(selftest_registers))
        assert all(int(n) == len(selftest_registers) for _, n in inventory)
        expected_selftest = {f'{field}[{i}]' for field, width in [('address',23),('first_address',23),('first_expected',12),('first_actual',12),('phase',3),('first_phase',3)] for i in range(width)} | {'observation[0]', 'pending', 'failed'}
        expected_copies = ({f'address[{i}]~DUPLICATE' for i in [3,15,17,18,21]} | {'phase[1]~DUPLICATE','phase[2]~DUPLICATE','failed~DUPLICATE'}) if has_correct_capture else ({f'address[{i}]~DUPLICATE' for i in [4,5,6,7,18,21]} | {'phase[1]~DUPLICATE'}) if has_burst_observer else {f'address[{i}]~DUPLICATE' for i in [0,1,11,14,17,18,19,20,21]} | {'phase[0]~DUPLICATE','phase[1]~DUPLICATE','pending~DUPLICATE'}
        assert len(selftest_registers) == (87 if has_correct_capture else 86 if has_burst_observer else 91) and {n.rsplit('|',1)[-1] for n in selftest_registers} == expected_selftest | expected_copies, 'Self-test logical bits/copies differ'
        fitted_state = json_at(root / 'register-inventory-v1/state.json')
        assert fitted_state['status'] == 'TERMINAL_FITTED_REGISTER_INVENTORY' and fitted_state['returncode'] == 0
        absent = json_at(root / 'register-inventory-v1/absence.json')
        assert set(absent['absent_pids']) == {fitted_state['driver_pid'],fitted_state['child_pid']}
        inventory_tcl = 'inspect_storage_burst_observer_fit.tcl' if has_burst_observer else 'inspect_storage_selftest_fit.tcl'
        assert fitted_state['tcl_sha256'] == sha(raw(root / inventory_tcl)) == sha((ROOT / 'tools' / inventory_tcl).read_bytes())
        fitted_log = raw(root / 'register-inventory-v1/inventory.log').decode()
        if has_burst_observer:
            # The union also probes an output-name alias, but this placement
            # names all97 registers first_burst_*. Require that complete bit
            # set below; classify ONLY this unused alternate-name filter.
            inventory_warning = ('Warning (332174): Ignored filter at inspect_storage_burst_observer_fit.tcl(12): '
                '*|dorado_storage_sdram_burst:u_sdram|burst_observation* could not be matched with a register File: '
                + record['project'] + '/inspect_storage_burst_observer_fit.tcl Line: 12')
            report_warning = ('Warning (332174): Ignored filter at audit_storage_burst_observer_native.tcl(74): '
                '*|dorado_storage_sdram_burst:u_sdram|burst_observation* could not be matched with a register File: '
                + record['project'] + '/audit_storage_burst_observer_native.tcl Line: 74')
            assert [row for row in fitted_log.splitlines() if row.startswith('Warning ')] == [inventory_warning]
            assert 'successful. 0 errors, 1 warning' in fitted_log
            assert [row for row in log.splitlines() if row.startswith('Warning ')] == [report_warning]*4
            assert 'successful. 0 errors, 4 warnings' in log
            trace_alias_filter_warning = dict(inventory=inventory_warning,report=report_warning,complete_trace_logical_bits=97)
        else:
            assert '0 errors, 0 warnings' in fitted_log
        inventories = {}
        for group, count in re.findall(r'^SELFTEST_FIT_COUNT (\S+) (\d+)$', fitted_log, re.M):
            names = re.findall(r'^SELFTEST_FIT_REGISTER ' + group + r' (.+)$', fitted_log, re.M)
            assert len(names) == len(set(names)) == int(count)
            inventories[group] = names
        expected_observer = {f'{field}[{i}]' for field, width in [('first_processor',32),('first_sources',16),('preceding_sources',16),('previous_sources',16)] for i in range(width)} | {'armed','valid'} | (set() if has_burst_observer else {'valid~DUPLICATE'})
        assert set(inventories) == {'observer','selftest','response'} | ({'trace'} if has_burst_observer else set())
        assert {n.rsplit('|',1)[-1] for n in inventories['observer']} == expected_observer and len(inventories['observer']) == (82 if has_burst_observer else 83)
        assert set(inventories['selftest']) == set(selftest_registers)
        assert set(inventories['response']) == set(response_registers)

        for corner in sorted(readers.CORNERS):
            for label in ['selftest-internal', 'selftest-input', 'selftest-output']:
                for mode in ['setup', 'hold']:
                    name = f'burst-{label}-{mode}-{corner}.rpt'
                    selftest_paths.append(readers.timed(raw(core / name).decode(), name))
        assert len(selftest_paths) == 24
    burst_observer_paths = []
    burst_observer_registers = []
    if has_burst_observer:
        inventory_trace = re.findall(r'^BURST_TRACE_COUNT (\S+) (\d+)$', log, re.M)
        assert len(inventory_trace) == 4 and {c for c,_ in inventory_trace} == readers.CORNERS
        burst_observer_registers = re.findall(r'^BURST_TRACE_REGISTER (.+)$', log, re.M)
        assert len(burst_observer_registers) == len(set(burst_observer_registers))
        assert all(int(n) == len(burst_observer_registers) for _,n in inventory_trace)
        expected_trace = {f'first_burst_words[{i}]' for i in range(96)} | {'first_burst_valid'} | ({'first_burst_valid~DUPLICATE'} if has_correct_capture else set())
        assert len(burst_observer_registers) == (98 if has_correct_capture else 97) and {n.rsplit('|',1)[-1] for n in burst_observer_registers} == expected_trace
        assert set(inventories['trace']) == set(burst_observer_registers)
        for corner in sorted(readers.CORNERS):
            for mode in ['setup','hold']:
                name = f'burst-trace-capture-{mode}-{corner}.rpt'
                burst_observer_paths.append(readers.timed(raw(core / name).decode(), name))
        assert len(burst_observer_paths) == 8
    warnings = {}
    classified_truncation = None
    for phase in ['map', 'fit']:
        text = raw(root / (phase + '.log')).decode()
        assert 'Critical Warning' not in text
        before = raw(parent_reports.parent.parent / (phase + '.log')).decode()
        if phase == 'map':
            # The FIFO wrap ternary uses an unsized zero, so Quartus reports
            # 32 -> 2 truncation. Its nonzero branch increments a two-bit
            # index only below three; its other branch assigns zero. This
            # exact warning is harmless for the bound four-line variant.
            source_lines = raw(root / BACKEND).decode().splitlines()
            fifo_lines = [i+1 for i,row in enumerate(source_lines) if row.strip() == "next_line<=fill_line==CACHE_LINES-1 ? 0 : fill_line+1'b1;"]
            assert len(fifo_lines) == 1
            fifo_line = fifo_lines[0]
            assert has_burst_observer or fifo_line == 213
            line = ('Warning (10230): Verilog HDL assignment warning at '
                    f'dorado_storage_sdram_burst.sv({fifo_line}): truncated value with size 32 '
                    'to match size of target (2) File: ' + record['project'] + '/' + BACKEND + f' Line: {fifo_line}')
            assert text.splitlines().count(line) == 1
            classified_truncation = line
            text = text.replace(line, '')
        increase = Counter(re.findall(r'Warning \((\d+)\)', text)) - Counter(re.findall(r'Warning \((\d+)\)', before))
        assert not set(increase) - {'10036'}, ('New warning category/count', phase, increase)
        warnings[phase] = dict(increase)
    summary = raw(core / 'output_files/Dorado.fit.summary').decode()
    assert 'Fitter Status : Successful' in summary
    resources = {}
    for key, label in [('alms', 'Logic utilization (in ALMs)'), ('registers', 'Total registers'), ('ram_blocks', 'Total RAM Blocks'), ('dsp', 'Total DSP Blocks')]:
        match = re.search(re.escape(label) + r'\s*:\s*([\d,]+)', summary)
        assert match
        resources[key] = int(match[1].replace(',', ''))
    assert resources['ram_blocks'] == 332 and resources['dsp'] == 35
    evidence = {}
    for path in root.rglob('*'):
        if not path.is_file():
            continue
        name = str(path.relative_to(root))
        if name.endswith('.gz'):
            name = name[:-3]
        if ((name.startswith('core/') and Path(name).suffix in ('.rpt', '.summary', '.log')) or
                name.startswith('register-inventory-v1/') or name in ['inspect_storage_burst_observer_fit.tcl', 'inspect_storage_selftest_fit.tcl', 'run-state.json', 'burst-review-v2-state.json', 'burst-review-v2.log',
                         'collected-input-check.json', tcl_name, 'map.log', 'fit.log']):
            evidence[name] = sha(raw(root / name))
    return dict(status='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW', scope=__doc__,
                source_hashes=actual, memory_evidence=memories, timing_checks=checks,
                detailed_paths=paths, burst_paths=new_paths, burst_register_counts=expected_counts,
                backend_registers=registers, response_register_copies=response_copies,
                resources=resources, warnings=warnings,
                classified_FIFO_index_truncation=classified_truncation,
                reviewed_evidence_hashes=evidence,
                minimum_slack_ns=min(x['slack_ns'] for x in paths + new_paths + observer_paths + selftest_paths + burst_observer_paths),
                passive_boot_fault_observer=has_observer, observer_component=observer_proof,
                observer_paths=observer_paths, raw_storage_selftest=has_selftest,
                selftest_component=selftest_proof, selftest_full_component=selftest_full_proof, selftest_paths=selftest_paths, selftest_registers=selftest_registers,
                corrected_sdram_read_capture=has_correct_capture, read_capture_qualification=read_capture_qualification,
                raw_burst_observer=has_burst_observer, burst_observer_component=burst_observer_proof,
                burst_observer_full_component=burst_observer_full_proof, burst_observer_paths=burst_observer_paths,
                burst_observer_registers=burst_observer_registers,
                trace_alias_filter_warning=trace_alias_filter_warning,
                frequency_mhz=50, sysper=4, state_admin='INACTIVE',
                firmware_qualified=False, hardware_speedup_qualified=False)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'parent', 'parent-qualification', 'parent-reports', 'output']:
        p.add_argument('--' + name, type=Path, required=True)
    p.add_argument('--observer-component', type=Path)
    p.add_argument('--selftest-component', type=Path)
    p.add_argument('--selftest-full-component', type=Path)
    p.add_argument('--burst-observer-component', type=Path)
    p.add_argument('--burst-observer-full-component', type=Path)
    p.add_argument('--cas-reference',type=Path)
    p.add_argument('--read-capture-matrix',type=Path)
    p.add_argument('--read-capture-board',type=Path)
    a = p.parse_args()
    result = review(a.root.resolve(), a.parent.resolve(), a.parent_qualification.resolve(), a.parent_reports.resolve(), a.observer_component.resolve() if a.observer_component else None, a.selftest_component.resolve() if a.selftest_component else None, a.selftest_full_component.resolve() if a.selftest_full_component else None, a.burst_observer_component.resolve() if a.burst_observer_component else None, a.burst_observer_full_component.resolve() if a.burst_observer_full_component else None, a.cas_reference.resolve() if a.cas_reference else None, a.read_capture_matrix.resolve() if a.read_capture_matrix else None, a.read_capture_board.resolve() if a.read_capture_board else None)
    a.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], result['minimum_slack_ns'], result['resources'])
