#!/usr/bin/env python3
"""Budget the complete isolated event BaseBoard at15ns on Cyclone V.

Keep every external input/output, all1725 MOS6502 nodes, the six original
clock Qs and every live bit/128-byte RAM of all FIVE RIOTs. Registered input
shell, unchanged15ns/2ns-I/O constraints, no timing exceptions or retiming.
Original SRAM/EPROM and remaining BaseBoard packages stay in the actual
board. This does not qualify native processor/CP subevents or firmware speed.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import hashlib

import fit_control_clock as fixture
from diagnose_native_baseboard_reset import BOARD
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
FIELDS = ['out_a', 'out_b', 'dir_a', 'dir_b', 'd_out', 'interrupt', 'timer',
          'prescaler', 'incr', 'rollover', 'irq_en', 'edge_detect', 'riot_stuff.old_pa7']


def observation_ports(out):
    """Lift diagnostic wires through explicit ports, with exact inverse audit.

    Quartus17 rejects simulator-style cross-module hierarchical references.
    These edits add ports and wire aliases only; never change a state/update.
    """
    root = out/'inputs'
    changes = []

    def edit(name, module, width, expression=None, connections=(), extra=''):
        p = root/name
        original = p.read_bytes()
        raw = original.decode()
        replacements = []

        def replace_at(index, old, new):
            nonlocal raw
            assert raw[index:index+len(old)] == old
            raw = raw[:index]+new+raw[index+len(old):]
            replacements.append((index, old, new))

        start = raw.index('module '+module)
        close = raw.index(');', start)
        replace_at(close, ');', f',\n output wire [{width-1}:0] event_observe\n);')
        for instance, signal in connections:
            m = re.search(r'\b'+instance+r'\s*\(', raw[start:])
            assert m
            index = start+m.end()
            replace_at(index, '', '.event_observe('+signal+'),\n')
        if module == 'M6532':
            # The static PA7 history bit retains its update/reset semantics;
            # only its declaration scope changes so Quartus can observe it.
            old='\t\treg old_pa7;'
            assert raw.count(old)==1
            replace_at(raw.index(old),old,'')
            old='reg edge_detect;'
            assert raw.count(old)==1
            replace_at(raw.index(old),old,old+'\nreg old_pa7;')
        if expression or extra:
            end = raw.index('endmodule', start)
            replace_at(end, '', extra+('assign event_observe={'+expression+'};\n' if expression else ''))
        inverse = raw
        for index, old, new in reversed(replacements):
            assert inverse[index:index+len(new)] == new
            inverse = inverse[:index]+old+inverse[index+len(new):]
        assert inverse.encode() == original
        p.write_bytes(raw.encode())
        changes.append(dict(path=name, original_sha256=hashlib.sha256(original).hexdigest(),
            observed_sha256=sha(p), inverse_byte_identical=True,
            replacements=[dict(offset=i,before=a,after=b) for i,a,b in replacements]))

    edit('src/verilog/vendor/6502/chip_6502.v', 'chip_6502', 1725, 'q')
    edit('src/verilog/cells/cell_MCS6502.v', 'cell_MCS6502', 1725,
         connections=[('u_cpu', 'event_observe')])
    riot_fields=[n.replace('riot_stuff.old_pa7','old_pa7') for n in FIELDS]+[f'riot_ram[{i}]' for i in range(128)]
    edit('src/verilog/vendor/riot/M6532.sv', 'M6532', 1097, ','.join(riot_fields))
    edit('experiment/dorado_riot_clock_event.sv', 'dorado_riot_clock_event', 1097,
         connections=[('u_riot', 'event_observe')])
    edit('experiment/cell_MCS6532_event.v', 'cell_MCS6532_event', 1097,
         connections=[('u_event_riot', 'event_observe')])
    packages = ['c62', 'f62', 'f63', 'i62', 'l62']
    extra = 'wire [1724:0] cpu_observe;\n'+''.join(f'wire [1096:0] riot_{p}_observe;\n' for p in packages)
    expression = 'event_clock_q,cpu_observe,'+','.join('riot_'+p+'_observe' for p in packages)
    edit(BOARD, 'BaseBd_m_Rev_m_Am_nominal', 7216, expression,
         [('u_f61', 'cpu_observe')]+[('u_'+p, 'riot_'+p+'_observe') for p in packages], extra)
    return changes


def prepare(boot, paused, out):
    assert not out.exists()
    states = []
    for d in [boot, paused]:
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] == 'PASS_ISOLATED_EVENT_BASEBOARD_ROM_MIDAS'
        assert s['require_6502_settled_before_phase'] and not s['diagnostic_only']
        assert all(a['returncode'] == 0 for a in s['actions'])
        for name, digest in s['inputs'].items():
            assert sha(d/'inputs'/name) == digest
        assert sha(d/'tb_baseboard_event_boot.sv') == s['fixture_sha256']
        log = (d/'test.log').read_text()
        assert 'BB_RESET_MIDAS_RETURN ticks=176750 phases=2651 fetches=865 stores=289 x=ff' in log
        assert 'BB_EVENT_COLD_REFERENCE intervals=176750 crystal_rises=26513 Q_bits=6' in log
        assert 'BB_EVENT_CONVERGENCE checks=5178 unsettled=0' in log
        states.append(s)
    assert states[0]['inputs'] == states[1]['inputs']
    assert not states[0]['pseudo_random_transport_clock_pauses'] and states[1]['pseudo_random_transport_clock_pauses']
    raw = (boot/'inputs'/BOARD).read_text()
    ports = re.findall(r'^\s*(input|output)\s+(?:wire\s+)?(\[[^]]+\]\s*)?(\w+)\s*[,)]?', raw[:raw.index(');')], re.M)
    assert len(ports) == 134
    inputs = [(w, n) for d, w, n in ports if d == 'input' and n != 'sys_clk']
    outputs = [(w, n) for d, w, n in ports if d == 'output']
    assert len(inputs) == 64 and not any(w for w, n in inputs)
    external_output_bits = sum(1 if not w else abs(int(re.findall(r'\d+', w)[0])-int(re.findall(r'\d+', w)[1]))+1 for w, n in outputs)
    observed = [n for w, n in outputs]+['state_observe']
    result_bits = external_output_bits+6+1725+5*(73+128*8)
    assert result_bits == 7316
    connections = ['.sys_clk(host_clk)', '.event_observe(state_observe)']+['.'+n+f'(launch[{i}])' for i, (w, n) in enumerate(inputs)]+['.'+n+'('+n+')' for w, n in outputs]
    top = f'''module clock_fit(input wire host_clk,input wire [63:0] pins,output reg [{result_bits-1}:0] result);
 reg [63:0] input_stage,launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 wire [7215:0] state_observe;
'''+''.join(' wire '+(w or '')+n+';\n' for w, n in outputs)+'''
 BaseBd_m_Rev_m_Am_nominal #(.SYSPER(4)) bb(
'''+',\n'.join(connections)+');\n always @(posedge host_clk)result<={'+',\n'.join(observed)+'};\nendmodule\n'
    out.mkdir(parents=True)
    for name in states[0]['inputs']:
        source = boot/'inputs'/name
        target = out/'inputs'/name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
    observation_changes = observation_ports(out)
    # Resolve the ORIGINAL board/cell literal ROM paths from this project.
    for source in (out/'inputs/src/verilog/proms').rglob('*'):
        if source.is_file():
            target = out/source.relative_to(out/'inputs/src')
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target)
    source_recipe = json.loads(__import__('gzip').decompress((Path(states[0]['source'])/'provenance.json.gz').read_bytes()))
    HDL = ['inputs/'+n for n in source_recipe['used_hdl']]
    HDL += ['inputs/'+n for n in states[0]['inputs'] if n.startswith('experiment/') and Path(n).suffix in ['.v', '.sv']]
    qsf = fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in HDL))
    qsf += ''.join('set_global_assignment -name SEARCH_PATH inputs/src/'+n+'\n' for n in ['verilog/cells', 'verilog/generated', 'verilog/vendor/6502', 'verilog/vendor/riot'])
    qsf += ''.join('set_global_assignment -name VERILOG_MACRO '+n+'\n' for n in source_recipe['defines'])
    audit = fixture.AUDIT.replace('CONTROL_CLOCK_', 'BB_EVENT_').replace(' set launches [get_registers {*launch*}]', f''' set ports [get_ports {{result[*]}}]
 if {{[get_collection_size $ports]!={result_bits}}} {{error "Incomplete BaseBoard/state/RAM output coverage"}}
 puts "BB_EVENT_OUTPUT_BITS [get_collection_size $ports]"
 set launches [get_registers {{*launch*}}]''')
    for name, text in {'top.sv': top, 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
                       'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n', 'audit.tcl': audit}.items():
        (out/name).write_text(text)
    for name in ['fit_control_clock.py', Path(__file__).name]:
        shutil.copy2(ROOT/'tools'/name, out/name)
    provenance = dict(scope=__doc__, clock_period_ns=15, result_bits=result_bits,
        external_input_bits=64, external_output_bits=external_output_bits,
        original_MOS6502_node_bits=1725, RIOT_scalar_bits_per_package=73,
        observation_port_changes=observation_changes, observation_adds_no_state_or_transfer_changes=True,
        RIOT_RAM_bytes_per_package=128, RIOT_packages=5, original_clock_Q_bits=6,
        boot=str(boot), boot_state_sha256=sha(boot/'run-state.json'),
        paused_boot=str(paused), paused_boot_state_sha256=sha(paused/'run-state.json'),
        assembly_permitted=False, native_qualified=False, native_CPU_root_CP_subevents_integrated=False,
        firmware_qualified=False, speedup_qualified=False, hardware_requires_user_release=True,
        files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(provenance, indent=2)+'\n')
    print('PREPARED_COMPLETE_EVENT_BASEBOARD_15NS_COMPONENT', result_bits, flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['boot', 'paused-boot', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    prepare(a.boot.resolve(), a.paused_boot.resolve(), a.output.resolve())
