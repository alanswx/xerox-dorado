#!/usr/bin/env python3
"""Run original BaseBoard firmware with explicit BB/RIOT clock consumers.

Copy one immutable native simulation source set. Replace only the d08/e07/g08
clock outputs with the qualified event source/six-Q cone and route its exact
PHI2 rise into all five RIOTs. The real1725-node6502, original EPROM/RAM and
remaining packages stay unchanged. Use15ns host periods and require the real
256-store/256-DEX reset loop and first MidasSetup return. This isolates slow
BaseBoard timing; it does NOT integrate CPU-root/CP subevents or prove Cedar,
full native timing, physical timing,60ns microinstructions or hardware speed.
"""
import argparse
import gzip
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from diagnose_native_baseboard_reset import fixture, BOARD
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
EVENT_BLOCK = '''
  // Explicit original BB crystal/divider/RIOT event integration experiment.
  // No guest reset initializes these six physical Qs, matching the originals.
  reg [5:0] event_clock_q;
  wire [5:0] event_clock_next,event_before,event_after;
  wire [2:0] event_count;
  wire [107:0] event_offsets;
  wire [35:0] event_masks,event_levels;
  wire [41:0] event_states;
  wire [48:0] event_edges;
  wire [6:0] event_roots,event_max,event_pre,event_aux;
  wire event_invalid;
  dorado_primary_clock_events event_source(sys_clk,1'b0,1'b1,
    event_before,event_after,event_count,event_offsets,event_masks,event_levels);
  dorado_baseboard_clock_step event_cone(event_clock_q,event_before,event_after,
    event_count,event_levels,1'b0,1'b0,6'b0,1'b1,event_clock_next,
    event_states,event_edges,event_roots,event_max,event_pre,event_aux,event_invalid);
  always @(posedge sys_clk)event_clock_q<=event_clock_next;
  wire event_phi_rise=event_edges[6]|event_edges[13]|event_edges[20]|
    event_edges[27]|event_edges[34]|event_edges[41]|event_edges[48];
  wire event_phi_fall=event_edges[5]|event_edges[12]|event_edges[19]|
    event_edges[26]|event_edges[33]|event_edges[40]|event_edges[47];
  assign Basebd01_sil_pl_4=event_before[4];
  assign Max=(event_clock_q[3:0]==4'hf);
  assign MCPreClk=~event_clock_q[4];
  assign BaseBd10_sil_pl_3=~event_clock_q[5];
'''


def disconnect(raw, package, bindings):
    pattern = r'(  cell_\w+(?: #\([^\n]*\))? u_'+package+r' \(\n)(.*?)(^  \);[^\n]*)'
    match = re.search(pattern, raw, re.M | re.S)
    assert match, package
    body = match[2]
    for pin, net in bindings.items():
        old = f'.p{pin}({net})'
        assert body.count(old) == 1, (package, old)
        body = body.replace(old, f'.p{pin}()', 1)
    return raw[:match.start(2)]+body+raw[match.end(2):]


def riot_package(raw):
    start = raw.index('  // Keep the modeled 6502 phase')
    end = raw.index('  // `oe` is the core', start)
    body = '''  // Caller supplies the actual pre-edge bus and PHI2 event.
  dorado_riot_clock_event u_event_riot(
    .clk(sys_clk),.consume(1'b1),.phi_rise(clock_capture),
    .reset_n_at_edge(p34),.address_at_edge({p40,p2,p3,p4,p5,p6,p7}),
    .rw_at_edge(p35),.ram_n_at_edge(p36),.cs1_at_edge(p38),.cs2_n_at_edge(p37),
    .data_at_edge(d_in),.pa_at_edge(pa_in),.pb_at_edge(pb_in),
    .data_out(d_out),.pa_out(pa_out),.pb_out(pb_out),
    .irq_n(irq_n),.output_enable(oe));

'''
    result = raw[:start]+body+raw[end:]
    result = result.replace('module cell_MCS6532 (', 'module cell_MCS6532_event (', 1)
    result = result.replace('input  wire sys_clk,', 'input  wire sys_clk,\n    input wire clock_capture,', 1)
    return result


def run(source, riot_tests, out, require_settled=True, gated=False):
    assert not out.exists()
    r = json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    t = json.loads((riot_tests/'run-state.json').read_text())
    assert t['status'] == 'PASS_RIOT_EXPLICIT_EVENT_ORIGINAL_STATE_AND_RAM'
    assert len(t['cases']) == 10 and all(row['passed'] for row in t['cases'])
    for name, digest in t['inputs'].items():
        assert sha(riot_tests/'inputs'/name) == digest
    selection = ROOT/'build/rtl-executive/single-bb-canonical-selection-v1-20261024/results.json'
    selected = json.loads(selection.read_text())
    assert selected['status'] == 'SELECTED_QUALIFIED_SINGLE_BB_TRANSFER_SOURCES'
    for name, digest in selected['canonical_files'].items():
        assert sha(ROOT/name) == digest
    out.mkdir(parents=True)
    inputs = out/'inputs'
    hashes = {}
    for name, digest in r['inputs'].items():
        original = source/'inputs'/name
        assert sha(original) == digest, name
        target = inputs/name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(original, target)
        hashes[name] = digest
    board = inputs/BOARD
    old = board.read_text()
    raw = old
    for package, bindings in [('d08', {10:'Basebd01_sil_pl_4'}), ('e07', {15:'Max'}),
                              ('g08', {6:'MCPreClk', 8:'BaseBd10_sil_pl_3'})]:
        raw = disconnect(raw, package, bindings)
    matches = list(re.finditer(r'  cell_MCS6532 u_(\w+) \(\n    \.sys_clk\(sys_clk\),', raw))
    assert sorted(m[1] for m in matches) == ['c62', 'f62', 'f63', 'i62', 'l62']
    raw = re.sub(r'  cell_MCS6532 u_(\w+) \(\n    \.sys_clk\(sys_clk\),',
        r'  cell_MCS6532_event u_\1 (\n    .sys_clk(sys_clk),\n    .clock_capture(event_phi_rise),', raw)
    assert raw.count('\nendmodule') == 1
    raw = raw.replace('\nendmodule', EVENT_BLOCK+'\nendmodule', 1)
    board.write_text(raw)
    hashes[BOARD] = sha(board)
    experiment = inputs/'experiment'
    experiment.mkdir()
    extras = []
    for name in ['dorado_primary_clock_events.sv', 'dorado_clock_event_batch.sv',
                 'dorado_oscillator_events.sv', 'dorado_baseboard_clock_step.sv', 'dorado_riot_clock_event.sv']:
        target = experiment/name
        shutil.copy2(ROOT/'verilog/rtl'/name, target)
        hashes[str(target.relative_to(inputs))] = sha(target)
        extras.append(target)
    target = experiment/'cell_MCS6532_event.v'
    target.write_text(riot_package((inputs/'src/verilog/cells/cell_MCS6532.v').read_text()))
    hashes[str(target.relative_to(inputs))] = sha(target)
    extras.append(target)
    assert sha(inputs/'src/verilog/vendor/riot/M6532.sv') == t['inputs']['M6532.sv']
    assert sha(experiment/'dorado_riot_clock_event.sv') == t['inputs']['dorado_riot_clock_event.sv']
    bench, environment = fixture(raw, 4, gated, True, False, False, False)
    clock = 'host_clk' if gated else 'sys_clk'
    assert bench.count('always #8.75 '+clock+'=~'+clock+';') == 1
    bench = bench.replace('always #8.75 '+clock+'=~'+clock+';', 'always #7.5 '+clock+'=~'+clock+';', 1)
    # Retain every original firmware check. Add exact crystal-derived periods.
    old_marker = '    midas_returned=1;'
    assert bench.count(old_marker) == 1
    bench = bench.replace(old_marker, '''    if(shortest_period!=66 || longest_period!=129)
     $fatal(1,"BB_EVENT expected cold129-tick initial observation then66/67-tick periods");
    $display("BB_EVENT_PERIOD min=%0d max=%0d nominal_ns=1000 cold_initial_interval=129",shortest_period,longest_period);
'''+old_marker, 1)
    extra_checks = '''
integer convergence_checks=0,unsettled_edges=0;
integer phase_age=0,last_unsettled_age=0,max_last_unsettled_age=0;
integer shortest_phase=1000000,longest_phase=0,measured_phases=0;
reg have_phase=0;
// Independent absolute50ns crystal deadlines and original package transfers.
// Check EVERY six-Q endpoint and crystal level from the actual cold zero
// state, including the initial count0->15 transient. No merger/event masks,
// DUT phase counters or copied step function are used by this reference.
longint unsigned reference_ns=0,next_crystal_ns=50;
integer reference_intervals=0,reference_crystal_rises=0;
reg reference_crystal=0;
reg [5:0] reference_q=0;
reg [3:0] previous_counter;
reg previous_pre;
always @(negedge sys_clk)begin
 reference_ns=reference_ns+15;
 while(next_crystal_ns<=reference_ns)begin
  reference_crystal=~reference_crystal;next_crystal_ns=next_crystal_ns+50;
  if(reference_crystal)begin
   reference_crystal_rises=reference_crystal_rises+1;
   previous_counter=reference_q[3:0];previous_pre=~reference_q[4];
   if(previous_counter==15)reference_q[3:0]=11;
   else reference_q[3:0]=previous_counter+1'b1;
   if(previous_counter!=15 && reference_q[3:0]==15)reference_q[4]=~reference_q[4];
   if(!previous_pre && !reference_q[4])reference_q[5]=~reference_q[5];
  end
 end
 if(bb.event_clock_q !== reference_q || bb.event_before[4] !== reference_crystal)
  $fatal(1,"BB_EVENT cold/steady source reference ns=%0d Q=%h expected=%h",reference_ns,bb.event_clock_q,reference_q);
 reference_intervals=reference_intervals+1;
end
always @(posedge sys_clk)begin
 if(bb.MCReset_p_ && (bb.event_phi_rise || bb.event_phi_fall))begin
  convergence_checks=convergence_checks+1;
  if(have_phase)begin
   if(last_unsettled_age>max_last_unsettled_age)max_last_unsettled_age=last_unsettled_age;
   if(phase_age<shortest_phase)shortest_phase=phase_age;
   if(phase_age>longest_phase)longest_phase=phase_age;
   measured_phases=measured_phases+1;
  end
  have_phase=1;phase_age=0;last_unsettled_age=0;
  if(bb.u_f61.u_cpu.q !== bb.u_f61.u_cpu.no)begin
   unsettled_edges=unsettled_edges+1;
   $display("BB_EVENT_UNSETTLED tick=%0d phase=%b rise=%b pc=%h nodes=%h",ticks,bb.MCPreClk,bb.event_phi_rise,bb.u_f61.u_cpu.logic_00.pc,bb.u_f61.u_cpu.q^bb.u_f61.u_cpu.no);
   REQUIRE_SETTLED
  end
 end
 else if(have_phase)begin
  phase_age=phase_age+1;
  if(bb.u_f61.u_cpu.q !== bb.u_f61.u_cpu.no)last_unsettled_age=phase_age;
 end
 if(bb.event_invalid)$fatal(1,"BB_EVENT invalid source packet");
end
final begin
 $display("BB_EVENT_CONVERGENCE checks=%0d unsettled=%0d host_ns=15 CPU_root_subevents_integrated=0",convergence_checks,unsettled_edges);
 $display("BB_EVENT_COLD_REFERENCE intervals=%0d crystal_rises=%0d Q_bits=6",reference_intervals,reference_crystal_rises);
 $display("BB_EVENT_SETTLING phases=%0d max_last_unsettled_age=%0d shortest_phase_counter=%0d longest_phase_counter=%0d",measured_phases,max_last_unsettled_age,shortest_phase,longest_phase);
end
'''.replace('REQUIRE_SETTLED', '$fatal(1,"BB_EVENT 6502 not settled before its next phase");' if require_settled else '')
    bench = bench.replace('\nendmodule', extra_checks+'\nendmodule', 1)
    tb = out/'tb_baseboard_event_boot.sv'
    tb.write_text(bench)
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    shutil.copy2(ROOT/'tools/diagnose_native_baseboard_reset.py', out/'diagnose_native_baseboard_reset.py')
    command = ['verilator', '--binary', '--timing', '--autoflush', '--top-module', 'tb_native_baseboard_reset',
        '-j', '2', '-Wno-fatal', '-Wno-PINMISSING', '-Wno-TIMESCALEMOD',
        '-Iverilog/cells', '-Iverilog/generated', '-Iverilog/vendor/6502', '-Iverilog/vendor/riot',
        *['-D'+d for d in r['defines']], '--Mdir', str(out/'obj'), '-o', 'test',
        *[str(inputs/name) for name in r['used_hdl']], *map(str, extras), str(tb)]
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        source=str(source), source_provenance_sha256=sha(source/'provenance.json.gz'),
        source_selection_sha256=sha(selection), RIOT_tests_state_sha256=sha(riot_tests/'run-state.json'),
        host_period_ns=15, exact_original_ROM_and_6502_and_RAM=True, original_RIOT_core_unchanged=True,
        pseudo_random_transport_clock_pauses=gated,
        five_RIOTs_use_explicit_preedge_capture=True, extra_guest_cycles=0, new_phase_history_bits=0,
        require_6502_settled_before_phase=require_settled, diagnostic_only=not require_settled,
        CPU_root_CP_subevents_integrated=False, native_firmware_qualified=False,
        native_timing_qualified=False, hardware_changed=False, speedup_qualified=False,
        inputs=hashes, fixture_sha256=sha(tb), external_inputs=environment, actions=[])

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for label, command in [('compile', command), ('test', [str(out/'obj/test')])]:
        with (out/(label+'.log')).open('x') as log:
            p = subprocess.Popen(command, cwd=inputs/'src', stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, command=command, pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            code = p.wait()
            row.update(returncode=code, finished=time.time())
            save()
        if code:
            break
    log = (out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed = code == 0 and 'BB_RESET_CLEAR_EXIT' in log and 'BB_RESET_MIDAS_RETURN' in log
    state.update(status=('PASS_ISOLATED_EVENT_BASEBOARD_ROM_MIDAS' if passed and require_settled else
        'TERMINAL_EVENT_BASEBOARD_DIAGNOSTIC_REVIEW_REQUIRED' if passed else 'TERMINAL_EVENT_BASEBOARD_NOT_ESTABLISHED'),
        finished=time.time())
    save()
    print(state['status'], flush=True)
    return code


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'riot-tests', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--diagnose-unsettled', action='store_true', help='Retain all original ROM checks; report unsettled nodes without accepting qualification')
    p.add_argument('--gated', action='store_true', help='Pause the virtual BaseBoard with the existing deterministic transport barrier pattern')
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.riot_tests.resolve(), a.output.resolve(), not a.diagnose_unsettled, a.gated))
