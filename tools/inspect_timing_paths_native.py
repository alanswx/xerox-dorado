#!/usr/bin/env python3
"""Inspect the combined timing-path fit without granting assembly permission.

Read all inherited and added categories, source/compiled-parameter bindings,
physical RAM behavior and clock coverage. Negative timing rejects the image.
Positive timing still requires the complete preassembly review and guards;
this diagnostic is never an assembly, firmware or speed qualification.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

from review_sdram_burst_native import memory_comparison, readers
from audit_native_conta import ucp
from review_pack_window_native import sample_paths
from review_storage_response_native import HOST_CLOCK
from review_storage_cache_native import canonical


def inspect(root, parent, qualification):
    hashes = {}

    def read(name):
        data = (root/name).read_bytes()
        hashes[name] = hashlib.sha256(data).hexdigest()
        return data.decode('latin1')

    def js(name):
        return json.loads(read(name))

    recipe = js('timing-paths-native.json')
    assert recipe['frequency_hz'] == 57142857 and recipe['sysper'] == 4
    assert recipe['state_admin'] == 'INACTIVE' and not recipe['assembly_permitted']
    assert recipe['request_lookahead'] == recipe['response_overlap'] == 1
    assert recipe['parallel_lookup'] and recipe['parallel_ready_compare'] and recipe['ALU_carry_select']
    assert recipe['raw_buffer_lines'] == 4 and len(recipe['modified_RTL_sources']) == 6
    actual = {}
    for n, h in recipe['source_hashes'].items():
        read(n); actual[n] = hashes[n]
        assert actual[n] == h, n
    assert len(actual) == 503
    build = js('run-state.json'); reports = js('storage-ready-reports-v1/run-state.json')
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert reports['status'] == 'TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED'
    assert [a['stage'] for a in build['stages']] == ['map','fit','sta','clocks','corners','control-crossings','contb-crossings','adapter-crossings','storage-ready']
    assert [a['stage'] for a in reports['actions']] == ['ready','backend','inventory','cache-counter','response-overlap','baseboard-crystal','request-lookahead']
    assert all(a['returncode'] == 0 for a in build['stages'] + reports['actions'])
    collected = js('collected-ready-input-check.json')
    pids = {build['builder_pid'], reports['driver_pid']} | {a['pid'] for a in build['stages']+reports['actions']}
    assert set(collected['absent_pids']) == pids and collected['actual'] == actual
    read('build_r_front_ready_native.py'); assert hashes['build_r_front_ready_native.py'] == build['builder_sha256']
    for n, digest in reports['report_inputs'].items():
        read('storage-ready-reports-v1/'+n); assert hashes['storage-ready-reports-v1/'+n] == digest

    previous = json.loads(qualification.read_text())
    assert previous['status'] == 'PASS_NATIVE_STORAGE_RESPONSE_PREASSEMBLY_REVIEW'
    assert previous['source_hashes'] == recipe['parent_source_hashes']
    assert len(previous['detailed_paths']) == 380
    names = [p['report'].removesuffix('.gz') for p in previous['detailed_paths']]
    for corner in sorted(readers.CORNERS):
        for prefix, labels in [('crystal',['oscillator','downstream','baseboard']),
                               ('look',['selection','cached-response','admission','group-ready','outgoing'])]:
            for label in labels:
                for mode in ['setup','hold']:
                    names.append(f'storage-ready-reports-v1/{prefix}-{label}-{mode}-{corner}.rpt')
    assert len(names) == len(set(names)) == 444
    paths = []
    for n in names:
        body = read(n)
        m = re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)', body)
        assert m and int(m[1]) > 0, ('Missing timing paths', n)
        paths.append(dict(report=n, paths=int(m[1]), violations=int(m[2]), slack_ns=float(m[3]),
                          worst_from_to=[s.strip() for s in body.splitlines() if re.match(r'^;\s*(?:From Node|To Node)\s*;',s)][:2]))
    summary = re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)', read('core/output_files/Dorado.sta.summary'))
    assert len(summary) == 31
    negative = [p for p in paths if p['violations'] or p['slack_ns'] <= 0]
    summary_failed = [(n,v) for n,v in summary if float(v) <= 0]
    coverage = {}
    for corner in sorted(readers.CORNERS):
        n = 'core/native59-ucp-'+corner+'.rpt'
        coverage[corner] = ucp(read(n)); assert coverage[corner] == ucp((parent/n).read_text())
    if not negative and not summary_failed:
        for n in names:
            if n.startswith('core/storage-sample-'):
                fields = n.split('-'); sample_paths(read(n), fields[2], fields[3], HOST_CLOCK)
    clocks = read('core/native-clocks.rpt')
    for n in [HOST_CLOCK, 'dorado_sdram_clk']:
        rows = [s for s in clocks.splitlines() if s.startswith('; '+n+' ')]
        assert len(rows) == 1 and re.search(r'; Generated\s*; 17\.500\s*;',rows[0])

    mapping = read('core/output_files/Dorado.map.rpt'); fitting = read('core/output_files/Dorado.fit.rpt')
    memory = {}
    for label, n, fitted in [('mapping','Dorado.map.rpt',False),('fitting','Dorado.fit.rpt',True)]:
        memory[label] = memory_comparison((parent/'core/output_files'/n).read_text(encoding='latin1'), read('core/output_files/'+n), fitted, True)
    memory['cache'] = readers.native_cache_physical(fitting, js('src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD'], 'independent40')
    table = mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n',1)[0]
    used = {f[3].replace(recipe['project']+'/','') for line in table.splitlines() if len(f:=[s.strip() for s in line.split(';')[1:-1]]) == 5 and f[1] == 'yes'}
    assert set(recipe['modified_RTL_sources']) <= used
    for key, value, count in [('HOST_CLOCK_HZ',57142857,1),('CACHE_LINES',4,1),('RESPONSE_OVERLAP',1,2),('REQUEST_LOOKAHEAD',1,1),('HOST_ADDR_SAMPLE',1,128),('DIRTY_BLOCK_WINDOW',1,1),('SNAPSHOT_PAUSE',0,1)]:
        values = re.findall(r';\s*'+key+r'\s*;\s*(\d+)\s*;',mapping)
        assert len(values) == count and all(int(v) == value for v in values), key
    headers = list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',mapping))
    sections = [mapping[h.start():headers[i+1].start() if i+1 < len(headers) else len(mapping)] for i,h in enumerate(headers) if 'cell_MC12061:u_d08' in h[1]]
    assert len(sections) == 1 and re.search(r'; SYSPER\s*; 8\s*;',sections[0]) and re.search(r'; FREQ_KHZ\s*; 10000\s*;',sections[0])

    families = {}
    log = read('storage-ready-reports-v1/request-lookahead.log')
    for corner in sorted(readers.CORNERS):
        families[corner] = {}
        for label in ['lookahead','arbiter','backend','groups']:
            pattern = re.escape(corner)+' '+label+' '
            rows = re.findall(r'^LOOK_FAMILY_REGISTER '+pattern+r'(.+)$',log,re.M)
            counts = re.findall(r'^LOOK_FAMILY_COUNT '+pattern+r'(\d+)$',log,re.M)
            assert len(counts) == 1 and int(counts[0]) == len(rows) == len(set(rows)) > 0
            families[corner][label] = len(rows)
            if label == 'groups':
                prefix = 'emu:emu|dorado_native_machine:u_native|dorado_native_backplane:m|'
                assert {n.rsplit('|',1)[0] for n in rows} == {prefix+f'msa_ddram:b_{board}|dorado_storage_group:u_storage_{i}' for board in ['msa','msaOdd','msa1','msa1Odd','msa2','msa2Odd','msa3','msa3Odd'] for i in range(16)}
    warning_counts = {}
    for n in ['map.log','fit.log','sta.log']:
        body = read(n)
        assert not re.search(r'^Error',body,re.M)
        warning_counts[n] = Counter(re.findall(r'^(?:Critical )?Warning \((\d+)\):',body,re.M))
    return dict(status='REJECT_NATIVE_TIMING_PATHS' if negative or summary_failed else 'TIMING_PATHS_POSITIVE_PREASSEMBLY_REVIEW_STILL_REQUIRED',
                scope=__doc__,source_hashes=actual,reviewed_evidence_hashes=hashes,summary_checks=summary,failed_summary_checks=summary_failed,
                detailed_paths=paths,negative_categories=negative,coverage=coverage,memory=memory,lookahead_families=families,warning_counts=warning_counts,
                minimum_setup_ns=min(p['slack_ns'] for p in paths if '-setup-' in p['report']),minimum_hold_ns=min(p['slack_ns'] for p in paths if '-hold-' in p['report']),
                assembly_permitted=False,firmware_qualified=False,hardware_speedup_qualified=False,limits='Terminal diagnostic only; full exact recipe regeneration, warning classification, capture/register checks and rejection controls remain necessary even for positive timing.')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['root','parent','qualification','output']:
        p.add_argument('--'+n,type=Path,required=True)
    a = p.parse_args(); assert not a.output.exists()
    r = inspect(a.root.resolve(),a.parent.resolve(),a.qualification.resolve())
    a.output.write_text(json.dumps(r,indent=2)+'\n')
    print(r['status'],len(r['detailed_paths']),'categories; setup',r['minimum_setup_ns'],'hold',r['minimum_hold_ns'])
