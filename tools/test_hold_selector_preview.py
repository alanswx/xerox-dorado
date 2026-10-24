#!/usr/bin/env python3
"""Check only the selector preview in complete references and four-state RTL.

Complete mode retains every original testbench/reference/check and compares
all100 clock counts and the existing cache coverage. Four-state mode compares
the full326-pin producer at both5/24 waves with X/Z in every1253 input bit,
plus an independent all-values unknown-selector fallback contract. Negative
fallback corruption must fail after successful compilation. No timing claim.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from preview_hold_selectors import sources, transform, NAME
from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

TB = '''`timescale 1ns/1ps
module tb_selector_unknowns;
 reg [1252:0] stimulus=0,base;
 wire [155:0] q;
 wire before_root,after_root,refresh,fast;
 wire [400:0] before_pins,after_pins;
 wire [268:0] prefix;
 wire [10:0] prefix_cp,after_cp;
 assign {q,before_root,after_root,before_pins,after_pins,refresh,fast,prefix,prefix_cp,after_cp}=stimulus;
 wire [325:0] original[0:1],actual[0:1];
 reg [397:0] fallback_pins=0;
 reg [2:0] fallback_value=0;
 wire [2:0] fallback_selected[0:1];
 for(genvar w=0;w<2;w++)begin: modes
  selector_equivalence #(.WAVES(w==0 ? 5:24)) complete(
   .state_q(q),.clock_before(before_root),.clock_after(after_root),.pins_before(before_pins),.pins_after(after_pins),
   .refresh(refresh),.fast_path(fast),.prefix_saved(prefix),.prefix_cp_saved(prefix_cp),.after_cp_saved(after_cp),
   .equal(),.different(),.original_producer(original[w]),.actual_producer(actual[w]),.original_selector(),.selected());
  dorado_hold_selector_preview #(.WAVES(w==0 ? 5:24)) fallback(
   .state_q(156'b0),.clock_before(1'b0),.clock_after(1'b0),.pins_before(398'b0),.pins_after(fallback_pins),
   .refresh_before_run_capture(1'b0),.fast_path(1'b0),.prefix_saved(269'b0),.prefix_cp_saved(11'b0),.after_cp_saved(11'b0),
   .original_selector(fallback_value),.selected(fallback_selected[w]));
 end
 integer checks=0,unknown_checks=0,fallback_checks=0;
 integer bit_index,kind;
 reg [31:0] random_word=32'h7e3a91b5;
 task fill;
  begin for(integer k=0;k<1253;k++)begin
   random_word=random_word^(random_word<<13);random_word=random_word^(random_word>>17);random_word=random_word^(random_word<<5);
   stimulus[k]=random_word[0];
  end end
 endtask
 task compare;
  begin #1;for(integer w=0;w<2;w++)begin
   if(actual[w] !== original[w])$fatal(1,"SELECTOR_PREVIEW four-state mismatch wave=%0d bit=%0d kind=%0d got=%h expected=%h",w,bit_index,kind,actual[w],original[w]);
   checks=checks+1;
  end end
 endtask
 initial begin
  for(integer n=0;n<64;n++)begin fill;compare;end
  fill;base=stimulus;
  for(bit_index=0;bit_index<1253;bit_index=bit_index+1)for(kind=0;kind<2;kind=kind+1)begin
   stimulus=base;stimulus[bit_index]=kind ? 1'bz : 1'bx;compare;unknown_checks=unknown_checks+2;
  end
  for(kind=0;kind<2;kind=kind+1)for(integer n=0;n<8;n++)begin
   fallback_pins=0;fallback_pins[386]=kind ? 1'bz : 1'bx;fallback_value=3'(n);#1;
   for(integer w=0;w<2;w++)begin
    if(fallback_selected[w] !== fallback_value)$fatal(1,"SELECTOR_PREVIEW unknown fallback mismatch wave=%0d kind=%0d value=%b got=%b",w,kind,fallback_value,fallback_selected[w]);
    fallback_checks=fallback_checks+1;
   end
  end
  $display("SELECTOR_PREVIEW_FOUR_STATE_PASS producer_checks=%0d unknown_checks=%0d fallback_checks=%0d inputs=1253 outputs=326 waves=5/24",checks,unknown_checks,fallback_checks);
  $finish;
 end
endmodule
'''


def run(source, proof, out, four_state, bad_unknown):
    assert not out.exists() and (not bad_unknown or four_state)
    p = json.loads((proof/'run-state.json').read_text())
    assert p['status'] == 'PASS_SELECTOR_PREVIEW_ALL_PRODUCER_BITS'
    assert p['compared_producer_output_bits'] == 326 and p['arbitrary_binary_input_bits'] == 1253
    assert p['compared_output_bits'] == 482 and p['compared_next_state_bits'] == 156
    for name, digest in p['files'].items():
        assert sha(proof/name) == digest
    original = (source/'candidate.sv').read_text()
    parent = json.loads((source/'run-state.json').read_text())
    assert parent['status'] == 'PASS_CP_KEY_COMPLETE_ORIGINAL_COMPARISON'
    actual, parts = sources(original, bad_unknown=bad_unknown)
    proven = (proof/'original.sv').read_text()
    for name in ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body', 'dorado_control_hold_ordered_step']:
        assert module(original, name) == module(proven, name)
    expected = module((proof/'candidate.sv').read_text(), NAME)
    if bad_unknown:
        expected = expected.replace('default:selected=original_selector;', "default:selected=3'b0;", 1)
    assert module(actual, NAME) == expected
    out.mkdir(parents=True)
    paths, records = [], []
    if four_state:
        for name, text in parts.items():
            (out/(name+'.sv')).write_text(text)
            paths.append(out/(name+'.sv'))
        (out/'tb.sv').write_text(TB)
        paths.append(out/'tb.sv')
        engine, top, defines = 'icarus', 'tb_selector_unknowns', []
    else:
        for row in parent['source_files']:
            path = source/row['path']
            assert sha(path) == row['output_sha256']
            target = out/row['path']
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(actual.encode() if path.name == 'candidate.sv' else path.read_bytes())
            paths.append(target)
            records.append(dict(path=row['path'], parent_sha256=sha(path), output_sha256=sha(target), changed=sha(path) != sha(target)))
        assert sum(r['changed'] for r in records) == 1
        assert (out/'inputs/tb_r_front_local.sv').read_bytes() == (source/'inputs/tb_r_front_local.sv').read_bytes()
        engine, top = parent['engine'], 'tb_r_front_local'
        defines = [s for s in parent['actions'][0]['command'] if s.startswith('-D')]
    (out/'candidate.sv').write_text(actual)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    if engine == 'icarus':
        commands = [['iverilog', '-g2012', '-s', top, *defines, '-o', str(out/'test.vvp'), *map(str, paths)], ['vvp', '-i', str(out/'test.vvp')]]
    else:
        commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', top,
            *defines, '--Mdir', str(out/'obj'), '-o', 'test', *map(str, paths)], [str(out/'obj/test')]]
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__, source=str(source),
        proof=str(proof), proof_state_sha256=sha(proof/'run-state.json'), engine=engine, four_state=four_state,
        bad_unknown=bad_unknown, source_files=records, original_testbench_and_references_unchanged=not four_state,
        actions=[], native_timing_qualified=False, firmware_qualified=False, speedup_qualified=False, hardware_changed=False)

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
        passed = state['actions'][0]['returncode'] == 0 and row['returncode'] != 0 and 'SELECTOR_PREVIEW' in log
        status = 'PASS_SELECTOR_UNKNOWN_FALLBACK_COUNTEREXAMPLE'
    elif four_state:
        expected = 'SELECTOR_PREVIEW_FOUR_STATE_PASS producer_checks=5140 unknown_checks=5012 fallback_checks=32 inputs=1253 outputs=326 waves=5/24'
        passed = row['returncode'] == 0 and expected in log
        status = 'PASS_SELECTOR_PREVIEW_FOUR_STATE'
    else:
        passed = row['returncode'] == 0 and 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392' in log
        status = 'PASS_SELECTOR_COMPLETE_ORIGINAL_COMPARISON'
        if passed:
            old = (source/'test.log').read_text()
            clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
            assert len(clocks) == 100 and clocks == re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', old, re.M)
            coverage = re.search(r'^CP_PREFIX_CACHE (.+)$', log, re.M)[1]
            assert coverage == parent['cache_coverage']
            state.update(all100_clock_counts_match=True, cache_coverage=coverage)
    state.update(status=status if passed else 'TERMINAL_SELECTOR_TEST_NOT_ESTABLISHED', finished=time.time(),
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
