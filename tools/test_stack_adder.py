#!/usr/bin/env python3
"""Exhaustively compare the inferred stack adder with original MC10182 cells.

All 4096 six-bit operand pairs and both connected internal carries compare.
The physical wiring fixes all M/S pins and initial carry LOW. Two broken
carry-boundary variants must fail in both simulators after compilation.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

from rewrite_stack_adder import ROOT, rewrite


def bench(record, mutation):
    fragment = record['inserted']
    for field, operand in [('operand_a', 'a'), ('operand_b', 'b')]:
        marker = 'wire [5:0] stack_add_'+operand+'={'+','.join(record[field])+'};'
        assert fragment.count(marker) == 1
        fragment = fragment.replace(marker, 'wire [5:0] stack_add_'+operand+'='+operand+';')
    marker = 'assign {'+','.join(record['sum_outputs'])+'}=stack_add_sum[5:0];'
    assert fragment.count(marker) == 1
    fragment = fragment.replace(marker, 'assign sum=stack_add_sum[5:0];')
    for name, width in re.findall(r'^ assign (\w+)=stack_add_low(2|4)\[\d\];', fragment, re.M):
        fragment = fragment.replace('assign '+name+'=', 'assign c'+width+'=')
    if mutation:
        width = int(mutation[-1])
        original = " wire [6:0] stack_add_sum={1'b0,stack_add_a}+{1'b0,stack_add_b};"
        assert fragment.count(original) == 1
        replacement = f''' wire [{width-1}:0] bad_low=stack_add_a[{width-1}:0]+stack_add_b[{width-1}:0];
 wire [{5-width}:0] bad_high=stack_add_a[5:{width}]+stack_add_b[5:{width}];
 wire [6:0] stack_add_sum={{1'b0,bad_high,bad_low}};'''
        fragment = fragment.replace(original, replacement)
    raw = '''`timescale 1ns/1ps
`default_nettype none
module native_stack(input wire [5:0] a,b,output wire [5:0] sum,output wire c2,c4);
'''+fragment+'''endmodule
module tb_stack_adder;
 reg [5:0] a=0,b=0;
 wire [5:0] original_sum,native_sum;
 wire original_c2,original_c4,native_c2,native_c4;
 native_stack candidate(a,b,native_sum,native_c2,native_c4);
'''
    for i in range(3):
        raw += f''' cell_MC10182 original_{i}(.p12(a[{2*i+1}]),.p5(a[{2*i}]),
  .p11(b[{2*i+1}]),.p6(b[{2*i}]),.p13({"1'b0" if i==0 else 'original_c'+str(2*i)}),
  .p7(1'b0),.p9(1'b0),.p10(1'b0),.p1(1'b0),.p8(1'b0),.p16(1'b0),
  .p14(original_sum[{2*i+1}]),.p4(original_sum[{2*i}])'''
        if i < 2:
            raw += ',.p2(original_c'+str(2*(i+1))+')'
        raw += ');\n'
    # A second comparison preserves the actual operand wiring verbatim.
    # Enumerate every independent physical input net, rather than deriving
    # arithmetic operands through the replacement's mapping a second time.
    inputs = set()
    outputs = set()
    signals = set()
    aliases = []
    for pkg, line in zip(['j17', 'k17', 'l17'], record['original_instances']):
        ports = {int(pin): net for pin, net in re.findall(r'\.p(\d+)\(([^()]+)\)', line)}
        inputs.update(ports[n] for n in [5, 6, 11, 12])
        outputs.update(ports[n] for n in [2, 4, 14] if n in ports)
        signals.update(n for n in ports.values() if n != "1'b0")
        if 2 in ports:
            aliases.append((ports[2].removesuffix('_'+pkg+'_2'), ports[2]))
    assert len(inputs) == 9 and len(outputs) == 8
    body = ' wire '+','.join(sorted(signals))+';\n'
    body += ''.join(f' assign {n}=physical[{i}];\n' for i, n in enumerate(sorted(inputs)))
    body += ''.join(f' assign {n}={driver};\n' for n, driver in aliases)
    body += ' assign observed={'+','.join(sorted(outputs))+'};\n'
    wired = ''
    for label, logic in [('original', ''.join(record['original_instances']).replace('comb_MC10182', 'cell_MC10182')),
                         ('native', record['inserted'])]:
        wired += 'module '+label+'_wired(input wire [8:0] physical,output wire [7:0] observed);\n'+body+logic+'endmodule\n'
    raw += ''' reg [8:0] physical=0;
 wire [7:0] original_wired_outputs,native_wired_outputs;
 original_wired physical_reference(physical,original_wired_outputs);
 native_wired physical_candidate(physical,native_wired_outputs);
 integer checks=0;
 initial begin
  for(integer x=0;x<64;x++)for(integer y=0;y<64;y++)begin
   a=6'(x);b=6'(y);#10;
   if({native_sum,native_c4,native_c2} !== {original_sum,original_c4,original_c2})begin
    $display("STACK_ADDER_DIFF check=%0d A=%h B=%h original=%h/%b%b native=%h/%b%b",checks,a,b,original_sum,original_c4,original_c2,native_sum,native_c4,native_c2);
    $fatal(1,"Stack adder differs from original connected outputs");
   end
   checks++;
  end
  $display("STACK_ADDER_EQUIVALENCE PASS checks=%0d",checks);
  for(integer x=0;x<512;x++)begin
   physical=9'(x);#10;
   if(original_wired_outputs !== native_wired_outputs)
     $fatal(1,"Stack adder actual wiring mismatch check=%0d physical=%h original=%h native=%h",x,physical,original_wired_outputs,native_wired_outputs);
  end
  $display("STACK_ADDER_WIRING PASS checks=512");$finish;
 end
endmodule
`default_nettype wire
'''
    return raw+wired


def run(out):
    assert not out.exists()
    out.mkdir(parents=True)
    inputs = out/'inputs'
    inputs.mkdir()
    source = ROOT/'verilog/cells/cell_MC10182.v'
    parent = ROOT/'build/rtl-executive/r-front-ready-qualified-v1-20261020'
    cone_meta = json.loads((parent/'dorado_r_front_cone.json').read_text())
    assert hashlib.sha256(source.read_bytes()).hexdigest() == cone_meta['source_cells']['verilog/cells/cell_MC10182.v']
    (inputs/source.name).write_bytes(source.read_bytes())
    cone = out/'rewritten-cone.sv'
    cone.write_bytes((parent/'dorado_r_front_cone.sv').read_bytes())
    transform = rewrite(cone)
    (out/'transform.json').write_text(json.dumps(transform, indent=2)+'\n')
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), actions=[], cases=[])
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    def execute(command, log):
        with log.open('w') as f:
            p = subprocess.Popen(list(map(str, command)), stdout=f, stderr=subprocess.STDOUT)
            row = dict(command=list(map(str, command)), pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            rc = p.wait()
            row.update(returncode=rc, finished=time.time())
            save()
            return rc
    save()
    for engine in ['icarus', 'verilator']:
        for mutation in [None, 'drop2', 'drop4']:
            d = out/(engine+'-'+(mutation or 'original'))
            d.mkdir()
            tb = d/'tb.sv'
            tb.write_text(bench(transform, mutation))
            command = ['iverilog', '-g2012', '-s', 'tb_stack_adder', '-o', d/'test.vvp', tb, inputs/source.name] if engine == 'icarus' else [
                'verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_stack_adder', '--Mdir', d/'obj', '-o', 'test', tb, inputs/source.name]
            assert execute(command, d/'compile.log') == 0, (engine, mutation, 'compile failure')
            rc = execute(['vvp', d/'test.vvp'] if engine == 'icarus' else [d/'obj/test'], d/'test.log')
            log = (d/'test.log').read_text()
            if mutation:
                check = 67 if mutation == 'drop2' else 79
                assert rc != 0 and f'STACK_ADDER_DIFF check={check} ' in log, (engine, mutation, log)
            else:
                assert rc == 0 and 'STACK_ADDER_EQUIVALENCE PASS checks=4096' in log
                assert 'STACK_ADDER_WIRING PASS checks=512' in log
            state['cases'].append(dict(engine=engine, mutation=mutation, checks=4096 if not mutation else check,
                                       physical_wiring_checks=512 if not mutation else 0,
                                       result='PASS' if not mutation else 'REJECTED_CARRY_ERROR'))
            save()
    state.update(status='PASS_STACK_ADDER_BINARY_EQUIVALENCE', finished=time.time(),
                 source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                 cone_sha256=transform['output_sha256'], native_integrated=False, physical_timing_qualified=False)
    save()
    (out/'results.json').write_text(json.dumps(state, indent=2)+'\n')
    print(state['status'], '4096 vectors in BOTH engines; four broken-carry cases reject')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    run(p.parse_args().output.resolve())
