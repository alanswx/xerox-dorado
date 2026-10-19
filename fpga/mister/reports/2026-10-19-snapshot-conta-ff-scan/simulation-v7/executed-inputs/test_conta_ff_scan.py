#!/usr/bin/env python3
"""Full actual ContA oracle with compact FF scans and unchanged RAM/phase words.

Derive a copy of the qualified fixture/oracle, changing only the administrative
word map, completion wait and added quarantine checks. Normal output/state,
all-bit writes, restore/first-edge and continued-execution checks remain intact.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_conta_ff_scan as gen
import test_conta_full as qualified

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixture(source, manifest):
    top, old_header, metadata = qualified.fixture(source, manifest)
    top = gen.once(top, 'input wire host_clk,run,run_ref,input wire',
                   'input wire host_clk,run,run_ref,system_reset,input wire')
    top = gen.once(top, ' output wire advance,ref_advance,output wire [8:0] ref_write);',
                   ' output wire advance,ref_advance,scan_hold_required,scan_fault,output wire [8:0] ref_write);')
    top = gen.once(top, '.state_host_clk(host_clk)',
                   '.state_system_reset(system_reset),.scan_hold_required(scan_hold_required),.scan_fault(scan_fault),.state_host_clk(host_clk)')
    words = [dict(word=i, width=min(18,681-18*i), offset=18*i) for i in range(38)]
    assert len(metadata['state_words']) == 229
    words += metadata['state_words'][75:]
    assert len(words) == 192 and sum(r['width'] for r in words) == 1353
    header = old_header[:old_header.index('struct StateWord')].replace('WORDS=229', 'WORDS=192')
    header += 'struct StateWord { unsigned word,width,offset; };\nconstexpr StateWord state_words[]={\n'
    header += ',\n'.join('{'+f"{r['word']},{r['width']},{r['offset']}"+'}' for r in words)+'};\n'
    metadata = dict(metadata, state_words=words, compact_ff_bits=681,
                    raw_state_export='Unchanged original/candidate all1353 physical-bit hierarchical probes')
    return top, header, metadata


def driver():
    original = (HERE/'test_conta_full.cpp').read_text()
    old = '''    cycle(false,false,false,compare);++requests;
    check(d.done,"missing state response");
    check(bool(d.error)==error,"invalid state access accepted");
    return d.result;'''
    new = '''    cycle(false,false,false,false);++requests;
    unsigned waited=0;
    while (!d.done) {
        // Spoof RAM writes while a scan owns the board. These must be ignored.
        d.request=1;d.state_index=(75u<<5)|16;d.write=1;d.state_data=0x3ffff;
        cycle(false,false,false,false);
        check(++waited<=682,"scan completion timeout");
    }
    d.request=0;
    if (word<38 && !error) check(waited==681,"scan rotation length differs");
    check(!d.scan_hold_required && !d.scan_fault,"completed access still held/faulted");
    check(bool(d.error)==error,"invalid state access accepted");
    if (compare) equal();
    return d.result;'''
    text = gen.once(original, old, new)
    text = gen.once(text, '    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;',
                    '    d.system_reset=0;d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;')
    first = '    for (unsigned i=0;i<20000;++i) {\n        pins();bool advance=(random_word()&3)!=0;\n        cycle(advance,advance,i%3==0);'
    reset_entry = '''    d.freeze=1;d.held=1;d.request=0;cycle(false,false);
    d.system_reset=1;d.request=1;d.write=1;
    d.state_index=(75u<<5)|16;d.state_data=0x3ffff;
    cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset admitted access");
    equal();
    d.system_reset=0;cycle(false,false);
    check(d.frozen&&!d.done,"reset reentry request accepted");
    d.request=0;d.freeze=0;d.held=0;cycle(false,false);
    d.request=1;d.write=0;d.state_index=0;d.state_data=0;
'''
    text = gen.once(text, first, reset_entry+first)
    text = gen.once(text, 'all1353 physical bits,229 state words,', 'all1353 physical bits,192 state words,')
    anchor = '    check(walked_bits==1353&&late_changes>10000&&holds>10000,"insufficient coverage");'
    quarantine = '''    d.request=0;cycle(false,false);
    d.freeze=1;d.held=1;cycle(false,false);
    d.request=1;d.write=1;d.state_index=0;d.state_data=0x12345;
    cycle(false,false,false,false);
    d.request=0;cycle(false,false,false,false);cycle(false,false,false,false);
    check(d.scan_hold_required,"partial scan lost board hold");
    d.freeze=0;d.held=0;cycle(false,false,false,false);
    check(d.scan_fault&&d.scan_hold_required&&d.error&&d.done,"lost boundary not quarantined");
    auto quarantined=d.candidate_state;
    d.request=1;d.write=1;d.state_index=(75u<<5)|16;d.state_data=0x3ffff;
    for (unsigned i=0;i<20;++i) {
        pins();cycle(true,false,false,false);
        check(d.scan_hold_required&&d.scan_fault&&d.frozen,"quarantine released");
        for (unsigned word=0;word<(STATE_BITS+31)/32;++word)
            check(d.candidate_state[word]==quarantined[word],"quarantined board state changed");
    }
    d.system_reset=1;d.freeze=1;d.held=1;cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset failed to close access");
    d.system_reset=0;cycle(false,false,false,false);
    check(d.frozen&&!d.done,"fault-reset reentry request accepted");
    d.request=0;cycle(false,false,false,false);
'''
    text = gen.once(text, anchor, quarantine+anchor)
    return original, text


def run(root, output, mutations=False, async_files=False):
    output.mkdir(parents=True, exist_ok=False)
    source = root/'build/rtl-executive/native16-tx-cable/ContA_monitor.v'
    board, manifest, clones = gen.generate(source, async_files=async_files)
    top, header, metadata = fixture(source, manifest)
    original_cpp, cpp = driver()
    inputs = output/'executed-inputs'
    inputs.mkdir()
    (inputs/'original-test.cpp').write_text(original_cpp)
    (inputs/'test_conta_ff_scan.cpp').write_text(cpp)
    originals = [root/'verilog/cells'/(kind+'.v') for kind in manifest['original_sources']]
    comment_deltas = []
    for path in originals:
        expected = manifest['original_sources'][path.stem]
        if sha(path) != expected:
            assert path.name == 'cell_MC10170.v', 'unreviewed frozen original differs: '+path.name
            current = gen.ROOT/'verilog/cells'/path.name
            assert sha(current) == expected
            tokens = lambda p: re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', p.read_text(), flags=re.S))
            assert '"' not in tokens(path) and tokens(path) == tokens(current)
            comment_deltas.append(dict(path=path.name, actual_sha256=sha(path), workspace_sha256=expected,
                                       identical_tokens_sha256=hashlib.sha256(tokens(path).encode()).hexdigest()))
    dependencies = [Path(__file__), HERE/'generate_conta_ff_scan.py', HERE/'ff_scan_controller.sv',
                    HERE/'test_conta_full.py', HERE/'test_conta_full.cpp', HERE/'generate_conta_full.py',
                    HERE/'generate_snapshot_ff.py', HERE/'generate_contb_full.py', HERE/'generate_f10145.py']
    if async_files:
        dependencies += [HERE/'generate_f10145_zero_write.py',HERE/'generate_f10145_shared_write.py']
    for path in dependencies+originals+[source]:
        (inputs/path.name).write_bytes(path.read_bytes())
    input_hashes = {str(p):sha(p) for p in dependencies+originals+[source]}
    (output/'driver-provenance.json').write_text(json.dumps(dict(
        scope=__doc__,source_hashes=input_hashes,
        derived_cpp_sha256=sha(inputs/'test_conta_ff_scan.cpp'),
        comment_only_frozen_deltas=comment_deltas),indent=2)+'\n')
    cases = [('candidate', None, None, None, None)]
    if mutations:
        cases += [
            ('lost_chain_bit', 'board', 'scan_bits[680:1]', "{scan_bits[680:2],1'b0}", 'board physical state differs'),
            ('lost_ff_shift', 'board', '.state_write(scan_step)', ".state_write(1'b0)", 'board physical state differs'),
            ('early_completion', 'ff_scan_controller.sv', 'position == STATE_BITS-1', 'position == STATE_BITS-2', 'scan rotation length differs'),
            ('busy_owner', 'board', ' && !scan_busy', '', 'scan rotation length differs'),
            ('lost_local_hold', 'board', '(state_freeze && state_guest_held) || scan_hold_required', '(state_freeze && state_guest_held)', 'quarantine released'),
            ('reset_access', 'board', ' && !state_system_reset;', ';', 'system reset admitted access'),
        ]
    results = []
    for name, target, before, after, diagnostic in cases:
        case = output/name
        case.mkdir()
        text, copies = board, dict(clones)
        if target:
            if target == 'board':
                assert before in text
                text = text.replace(before, after, 1)
            else:
                copies[target] = gen.once(copies[target], before, after)
        (case/'ContA_snapshot.sv').write_text(text)
        (case/'fixture.sv').write_text(top)
        (case/'ports.h').write_text(header)
        for filename, contents in copies.items():
            (case/filename).write_text(contents)
        command = ['verilator','--cc','--exe','--build','-j','4','--top-module','conta_full_fixture',
                   '-Wno-fatal','-DDORADO_F10145_LOGIC','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
                   str(case/'fixture.sv'),str(case/'ContA_snapshot.sv'),*[str(case/n) for n in copies],
                   str(source),*map(str,originals),str(inputs/'test_conta_ff_scan.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result = subprocess.run([str(case/'obj/Vconta_full_fixture')],capture_output=True,text=True,timeout=180)
        log = result.stdout+result.stderr
        (case/'test.txt').write_text(log)
        print(name+': '+log.strip(),flush=True)
        if diagnostic:
            assert result.returncode and diagnostic in log, (name,log)
        else:
            assert result.returncode == 0 and 'FULL CONTA RESTORE PASS' in log, log
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    assert all(sha(Path(p)) == h for p,h in input_hashes.items()), 'input source changed during test'
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (output/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    (output/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,
        source_hashes=input_hashes,
        comment_only_frozen_deltas=comment_deltas),indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--frozen-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true')
    p.add_argument('--async-files',action='store_true')
    a = p.parse_args()
    run(a.frozen_root.resolve(),a.output.resolve(),a.mutations,a.async_files)
