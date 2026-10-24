#!/usr/bin/env python3
"""Check projected cache keys in unchanged full references and four-state RTL.

Complete mode replaces only the cache guard comparison in the established
844-Q/9,216-RAM/100-clock graph test. Architectural reference counts must stay
identical; cache stalls may decrease. Four-state mode isolates the actual
guard key and compares all ready/fast/hit outputs with the original for every
stored/live key X/Z position and known-mismatch background. A deliberately
zeroed unknown fallback must fail. No native timing/boot/speed claim follows.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_cache_key_projection import projected
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
ANSI = ''' output reg [268:0] prefix_saved=0,
 output reg [10:0] prefix_cp_saved=0,after_cp_saved=0);'''
BODY = ''' output reg [268:0] prefix_saved,
 output reg [10:0] prefix_cp_saved,after_cp_saved);
 initial begin prefix_saved=0;prefix_cp_saved=0;after_cp_saved=0;end'''

TB = '''`timescale 1ns/1ps
module tb_key_four_state;
 reg [566:0] stored,live;
 localparam [566:0] CARE=567'hMASK;
 wire old_ready,old_fast,old_hit,new_ready,new_fast,new_hit;
 original_cache old_guard(1'b0,1'b0,1'b0,156'b0,156'b0,1'b0,1'b0,398'b0,398'b0,
  1'b0,1'b0,269'b0,old_ready,old_fast,old_hit,,,);
 projected_cache new_guard(1'b0,1'b0,1'b0,156'b0,156'b0,1'b0,1'b0,398'b0,398'b0,
  1'b0,1'b0,269'b0,new_ready,new_fast,new_hit,,,);
 integer checks=0,unknown_checks=0,binary_checks=0;
 integer pattern,side,kind,background,bit_index,other;
 reg [566:0] base;
 task unknown_case;
  begin
   #1;
   if({new_ready,new_fast,new_hit} !== {old_ready,old_fast,old_hit})
    $fatal(1,"CP_KEY_FOUR_STATE mismatch pattern=%0d side=%0d kind=%0d background=%0d bit=%0d old=%b%b%b new=%b%b%b",
     pattern,side,kind,background,bit_index,old_ready,old_fast,old_hit,new_ready,new_fast,new_hit);
   if(background==0 && old_hit !== 1'bx)$fatal(1,"Original unknown control was not exercised");
   if(background==1 && old_hit !== 1'b0)$fatal(1,"Known mismatch did not dominate unknown");
   checks=checks+1;unknown_checks=unknown_checks+1;
  end
 endtask
 initial begin
  // Isolate this actual combinational interface; no host edge occurs.
  force old_guard.cache_key=stored;force new_guard.cache_key=stored;
  force old_guard.current_key=live;force new_guard.current_key=live;
  force old_guard.cache_valid=1'b1;force new_guard.cache_valid=1'b1;
  force old_guard.canonical_valid=1'b0;force new_guard.canonical_valid=1'b0;
  for(pattern=0;pattern<2;pattern=pattern+1)begin
   base=pattern==0 ? 567'b0 : {81{7'b1010011}};
   stored=base;live=base;#1;
   if({old_ready,old_fast,old_hit,new_ready,new_fast,new_hit} !== 6'b101101)
    $fatal(1,"Equal known key must hit in both guards");
   checks=checks+1;binary_checks=binary_checks+1;
   for(bit_index=0;bit_index<567;bit_index=bit_index+1)begin
    stored=base;live=base;live[bit_index]=~live[bit_index];#1;
    if(old_hit !== 1'b0 || old_ready !== 1'b0 || new_hit !== !CARE[bit_index] || new_ready !== !CARE[bit_index])
     $fatal(1,"CP_KEY_BINARY cared/unused behavior mismatch bit=%0d",bit_index);
    checks=checks+1;binary_checks=binary_checks+1;
   end
   for(side=0;side<2;side=side+1)
    for(kind=0;kind<2;kind=kind+1)
     for(background=0;background<2;background=background+1)
      for(bit_index=0;bit_index<567;bit_index=bit_index+1)begin
       stored=base;live=base;other=(bit_index+1)%567;
       if(background)live[other]=~live[other];
       if(side==0)live[bit_index]=kind ? 1'bz : 1'bx;
       else stored[bit_index]=kind ? 1'bz : 1'bx;
       unknown_case;
      end
  end
  $display("CP_KEY_FOUR_STATE_PASS checks=%0d binary_checks=%0d unknown_checks=%0d key_bits=567 outputs=3",checks,binary_checks,unknown_checks);
  $finish;
 end
endmodule
'''


def lower_initializers(cache):
    if ANSI not in cache:
        assert BODY in cache
        return cache
    assert cache.count(ANSI) == 1
    result = cache.replace(ANSI, BODY, 1)
    assert result.replace(BODY, ANSI, 1) == cache
    return result


def run(source, proof, out, four_state, bad_unknown):
    assert not out.exists() and (not bad_unknown or four_state)
    p = json.loads((proof/'run-state.json').read_text())
    assert p['status'] == 'PASS_CP_CACHE_KEY_INDUCTIVE_SAFETY' and p['unbounded_safety_proven']
    assert sha(proof/'proof.log') == p['log_sha256']
    for name, digest in p['source_hashes'].items():
        assert sha(proof/name) == digest
    care = set(p['cared_key_bits'])
    original_cache = module((proof/'actual-source.sv').read_text(), 'dorado_cp_prefix_cache')
    candidate_cache = (proof/'candidate-cache.sv').read_text()
    assert candidate_cache == projected(original_cache, care)
    paths, records = [], []
    out.mkdir(parents=True)
    if four_state:
        if bad_unknown:
            before = 'default: prefix_key_match=(cache_key==current_key);'
            assert candidate_cache.count(before) == 1
            candidate_cache = candidate_cache.replace(before, "default: prefix_key_match=1'b0;", 1)
        (out/'original-cache.sv').write_text(original_cache.replace('module dorado_cp_prefix_cache(', 'module original_cache(', 1))
        (out/'candidate-cache.sv').write_text(candidate_cache.replace('module dorado_cp_prefix_cache(', 'module projected_cache(', 1))
        (out/'tb.sv').write_text(TB.replace('MASK', format(sum(1 << i for i in care), 'x')))
        paths = [out/'original-cache.sv', out/'candidate-cache.sv', out/'tb.sv']
        engine, top, defines = 'icarus', 'tb_key_four_state', []
    else:
        parent = json.loads((source/'run-state.json').read_text())
        assert parent['status'] == 'PASS_SHORT_CONTROL_COMPLETE_ORIGINAL_COMPARISON'
        raw = (source/'candidate.sv').read_text()
        old_cache = module(raw, 'dorado_cp_prefix_cache')
        assert lower_initializers(old_cache) == original_cache
        new_cache = projected(old_cache, care)
        assert lower_initializers(new_cache) == candidate_cache
        actual = raw.replace(old_cache, new_cache, 1)
        assert actual.replace(new_cache, old_cache, 1) == raw
        for row in parent['source_files']:
            path = source/row['path']
            assert sha(path) == row['output_sha256']
            target = out/row['path']
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(actual.encode() if path.name == 'candidate.sv' else path.read_bytes())
            paths.append(target)
            records.append(dict(path=row['path'], parent_sha256=sha(path), output_sha256=sha(target), changed=sha(path) != sha(target)))
        assert sum(row['changed'] for row in records) == 1
        assert (out/'inputs/tb_r_front_local.sv').read_bytes() == (source/'inputs/tb_r_front_local.sv').read_bytes()
        engine, top = parent['engine'], 'tb_r_front_local'
        defines = [s for s in parent['actions'][0]['command'] if s.startswith('-D')]
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    if engine == 'icarus':
        commands = [['iverilog', '-g2012', '-s', top, *defines, '-o', str(out/'test.vvp'), *map(str, paths)],
                    ['vvp', '-i', str(out/'test.vvp')]]
    else:
        commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', top,
                     *defines, '--Mdir', str(out/'obj'), '-o', 'test', *map(str, paths)], [str(out/'obj/test')]]
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        source=str(source), proof=str(proof), proof_state_sha256=sha(proof/'run-state.json'),
        engine=engine, four_state=four_state, bad_unknown=bad_unknown, source_files=records,
        actions=[], original_testbench_and_references_unchanged=not four_state,
        native_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        hardware_changed=False, assembly_permitted=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for label, command in zip(['compile', 'test'], commands):
        with (out/(label+'.log')).open('x') as log:
            proc = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, command=command, pid=proc.pid, started=time.time())
            state['actions'].append(row)
            save()
            row.update(returncode=proc.wait(), finished=time.time())
            save()
        if row['returncode'] != 0:
            break
    log = (out/'test.log').read_text() if (out/'test.log').exists() else ''
    if bad_unknown:
        passed = state['actions'][0]['returncode'] == 0 and row['returncode'] != 0 and 'CP_KEY_FOUR_STATE mismatch' in log
        status = 'PASS_CP_KEY_UNKNOWN_FALLBACK_COUNTEREXAMPLE'
    elif four_state:
        expected = 'CP_KEY_FOUR_STATE_PASS checks=10208 binary_checks=1136 unknown_checks=9072 key_bits=567 outputs=3'
        passed = row['returncode'] == 0 and expected in log
        status = 'PASS_CP_KEY_FOUR_STATE_COMPARISON'
    else:
        expected = 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392'
        passed = row['returncode'] == 0 and expected in log
        status = 'PASS_CP_KEY_COMPLETE_ORIGINAL_COMPARISON'
        if passed:
            old = (source/'test.log').read_text()
            clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
            assert len(clocks) == 100 and clocks == re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', old, re.M)
            state.update(all100_clock_counts_match=True, cache_coverage=re.search(r'^CP_PREFIX_CACHE (.+)$', log, re.M)[1],
                parent_cache_coverage=parent['cache_coverage'])
    state.update(status=status if passed else 'TERMINAL_CP_KEY_TEST_NOT_ESTABLISHED', finished=time.time(),
        files={str(p.relative_to(out)): sha(p) for p in out.rglob('*') if p.is_file()
               and 'obj' not in p.relative_to(out).parts and p.suffix != '.vvp' and p.name != 'run-state.json'})
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'proof', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--four-state', action='store_true')
    p.add_argument('--bad-unknown', action='store_true')
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.proof.resolve(), a.output.resolve(), a.four_state, a.bad_unknown))
