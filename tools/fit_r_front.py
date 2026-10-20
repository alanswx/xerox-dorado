#!/usr/bin/env python3
"""Measure the qualified R-front/R-memory component in the isolated 15 ns shell.

No assembly or deployment. This checks an implementation budget, not native
Dorado timing or speed. Prepare only from a completed frozen simulation gate;
run Quartus only on misterubuntu.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import fit_control_clock as fixture

ROOT = Path(__file__).resolve().parents[1]
RTL = ['dorado_r_front_step.sv', 'dorado_r_memory_step.sv',
       'dorado_r_address_step.sv', 'dorado_r_port_preview.sv',
       'dorado_processor_capture_step.sv', 'dorado_r_file_ports.sv',
       'dorado_r_flags_step.sv', 'dorado_r_operand_step.sv',
       'dorado_t_parity_step.sv', 'dorado_t_file_step.sv',
       'dorado_task_capture_step.sv']


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def quartus_cone(raw):
    """Lower reference observations to ordinary synthesizable connections.

    The qualified cone observes original package ports hierarchically. Export
    the front data from ProcL and substitute each local observation with the
    exact expression already connected to that package port. Clock aliases are
    the sampled adapters' existing OR expressions; no storage is introduced.
    This deliberately handles only the fixed, checked generator output shape.
    """
    front = re.findall(r'^ assign front_data\[[^\n]+$', raw, re.M)
    assert len(front) == 11
    for line in front:
        raw = raw.replace(line + '\n', '')
    header = ' output wire [91:0] flags);'
    assert raw.count(header) == 1
    raw = raw.replace(header, header[:-2] + ',output wire [43:0] front_data);')
    call = 'rb_forward_n,sl,cl,fl);'
    assert raw.count(call) == 1
    raw = raw.replace(call, 'rb_forward_n,sl,cl,fl,front_data);')
    for module in ('r_front_conta_sampled', 'r_front_proch_sampled', 'r_front_procl_sampled'):
        match = re.search(r'module ' + module + r'\(.*?\nendmodule', raw, re.S)
        assert match, module
        body = match[0]
        if module == 'r_front_procl_sampled':
            body = body.replace('\nendmodule', '\n' + '\n'.join(
                line.replace('l.', '') for line in front) + '\nendmodule')
        instances = {}
        for kind, name, ports in re.findall(r'^ ((?:sampled|comb)_\w+) (\w+)\((.*)\);$', body, re.M):
            connections = dict(re.findall(r'\.(\w+)\((.*?)\)(?=,\.\w+\(|$)', ports))
            assert connections and name not in instances, name
            instances[name] = (kind, connections)

        def connected(observation):
            instance, port, index = observation[1], observation[2], observation[3]
            kind, pins = instances[instance]
            if port in ('ca', 'cb'):
                assert kind in ('sampled_MC10231', 'sampled_SE10231'), kind
                return '(' + pins['p9'] + '|' + pins['p6' if port == 'ca' else 'p11'] + ')'
            if port == 'clk':
                assert kind in ('sampled_F10000', 'sampled_F10000_gated'), kind
                return '(' + pins['p4'] + '|' + pins['p13'] + ')'
            expression = pins[port]
            if index is not None:
                assert port == 'state_q'
                part = re.fullmatch(r'state_q\[(\d+)\+:(\d+)\]', expression)
                assert part and int(index) < int(part[2]), expression
                return 'state_q[' + str(int(part[1]) + int(index)) + ']'
            return '(' + expression + ')'

        body = re.sub(r'^ *assign[^\n]*$', lambda line: re.sub(
            r'\b(\w+)\.(p\d+|ca|cb|clk|state_q)(?:\[(\d+)\])?', connected, line[0]), body, flags=re.M)
        assert not re.search(r'^ *assign[^\n]*\b\w+\.\w+', body, re.M), module
        raw = raw[:match.start()] + body + raw[match.end():]
    start = ' for(genvar i=0;i<16;i=i+1) begin: map_t_file\n'
    end = ' end\n r_front_conta_sampled c('
    assert raw.count(start) == raw.count(end) == 1
    return raw.replace(start, ' genvar i;\n generate\n for(i=0;i<16;i=i+1) begin: map_t_file\n').replace(
        end, ' end\n endgenerate\n r_front_conta_sampled c(')


def prepare(qualified, out, capture_observers=False):
    gate = json.loads((qualified / 'results.json').read_text())
    assert gate['status'] == 'PASS_R_FRONT_ARRIVAL_CONTRACT'
    assert gate['physical_bits'] == 736 and gate['ram_bits'] == 9216
    assert len(gate['arrivals']) == 6 and len(gate['cases']) == 13
    assert len(gate['equivalence_controls']) == 0
    for name, digest in gate['inputs'].items():
        assert sha(qualified / 'executed-inputs' / name) == digest, name
        assert sha(ROOT / name) == digest, ('Source changed since qualification', name)
    cone_record = json.loads((qualified / 'dorado_r_front_cone.json').read_text())
    assert sha(qualified / 'dorado_r_front_cone.sv') == cone_record['output_sha256']
    original_cone = (qualified / 'dorado_r_front_cone.sv').read_text()
    synthesis_cone = quartus_cone(original_cone)
    out.mkdir(parents=True, exist_ok=False)
    top = fixture.TOP
    result_bits = 1181 if capture_observers else 971
    edits = [('[51:0]', '[1210:0]', 3), ('[50:0]', '[' + str(result_bits-1) + ':0]', 1),
             ('[31:0]', '[735:0]', 1),
             ('wire [8:0] before_controls,after_controls,events,repeated;',
              'wire [235:0] before_controls,after_controls;\n'
              ' wire [56:0] events,repeated;\n'
              ' wire [1:0] write_n,latch_n,t_latch_n,bypass,r_read_n,r_visible_n,rm_flags_n;\n'
              ' wire [3:0] parity_ok,faults,r_flags,select_n,port_select_n;\n'
              ' wire [31:0] address,port_address;\n'
              ' wire [1:0] forwarded_bypass,r_write_n,file_read_parity;\n'
              ' wire [15:0] file_read_data;wire [201:0] source_pins;\n'
              ' wire [7:0] fa_n;wire advance;', 1),
             ('dorado_control_clock_step', 'dorado_r_front_step', 1),
             ('after_controls}=launch;', 'after_controls,advance}=launch;', 1),
             ('evaluator(q,before_clock,after_clock,',
              'evaluator(host_clk,advance,q,before_clock,after_clock,', 1)]
    outputs = ('next_q,events,repeated,unsettled,write_n,latch_n,t_latch_n,bypass,'
               'parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n,address,'
               'select_n,forwarded_bypass,r_write_n,port_address,port_select_n,'
               'file_read_data,file_read_parity,source_pins,fa_n')
    # source_pins/fa_n observe combinational wires after state_next has
    # already been computed. They feed neither state nor a functional port.
    # Capturing them beside state_next artificially serializes task
    # arbitration and the following epoch's RBase read into one host period.
    # Keep that historical observation experiment available explicitly.
    stored_outputs = outputs if capture_observers else outputs.removesuffix(',source_pins,fa_n')
    edits += [('next_q,events,repeated,unsettled);', outputs + ');', 1),
              ('result<={next_q,events,repeated,unsettled};',
               'result<={' + stored_outputs + '};', 1)]
    for before, after, count in edits:
        assert top.count(before) == count, before
        top = top.replace(before, after)
    qsf = fixture.QSF.replace('dorado_control_clock_step.sv', RTL[0])
    for name in [*RTL[1:], 'dorado_r_front_cone.sv']:
        qsf += 'set_global_assignment -name SYSTEMVERILOG_FILE ' + name + '\n'
    files = {'top.sv': top, 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
             'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n',
             'audit.tcl': fixture.AUDIT}
    for name, raw in files.items():
        (out / name).write_text(raw)
    for name in RTL:
        (out / name).write_bytes((qualified / 'executed-inputs/verilog/rtl' / name).read_bytes())
    (out / 'qualified-cone.sv.txt').write_text(original_cone)
    (out / 'dorado_r_front_cone.sv').write_text(synthesis_cone)
    for name in ('fit_control_clock.py', 'fit_r_front.py'):
        (out / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    (out / 'functional-gate.json').write_bytes((qualified / 'results.json').read_bytes())
    record = dict(scope=__doc__, component='r_front' if capture_observers else 'r_front_functional', clock_period_ns=15,
                  ram_bits=9216, input_staging_bits=1211, launch_bits=1211,
                  result_bits=result_bits, functional_gate_sha256=sha(qualified / 'results.json'),
                  observation_outputs_registered=capture_observers,
                  observation_only_outputs=['source_pins','fa_n'],
                  cone_syntax='Ordinary ports for package observations; separate genvar and explicit generate; no wiring/function/storage change',
                  files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
                  assembly_permitted=False, native_qualified=False)
    (out / 'provenance.json').write_text(json.dumps(record, indent=2) + '\n')
    print('Prepared 736-bit R-front evaluator: 1211 launch/' + str(result_bits) + ' result bits; no assembly/deploy')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument('--prepare', type=Path)
    action.add_argument('--run', type=Path)
    parser.add_argument('--qualified', type=Path)
    parser.add_argument('--capture-observers', action='store_true',
                        help='Also register post-state observation buses (historical rejected experiment)')
    parser.add_argument('--quartus', type=Path,
                        default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args = parser.parse_args()
    if args.prepare:
        assert args.qualified, '--qualified is required to prepare'
        prepare(args.qualified.resolve(), args.prepare.resolve(), args.capture_observers)
    else:
        fixture.run(args.run.resolve(), args.quartus.resolve())
