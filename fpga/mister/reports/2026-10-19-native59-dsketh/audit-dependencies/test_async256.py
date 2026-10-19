#!/usr/bin/env python3
"""Compare both async RAM adapters to untouched originals and array oracles."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

import generate_async256 as gen

HERE = Path(__file__).resolve().parent
FIXTURE = '''module async256_fixture(
 input wire host_clk,ref_clk,advance,freeze,held,request,write,
 input wire [8:0] index,input wire [3:0] state_data,state_data_f,
 input wire [7:0] address,input wire [3:0] data,select_mb,
 input wire ce_f,we_mb_n,we_f_n,
 output wire [1:0] frozen,done,error,
 output wire [3:0] result_mb,result_f,q_mb,ref_q_mb,
 output wire q_f,ref_q_f,
 output wire [1023:0] memory_mb,ref_memory_mb,
 output wire [255:0] memory_f,ref_memory_f);
 cell_MB7071H_snapshot candidate_mb(
 .state_host_clk(host_clk),.state_advance(advance),.state_freeze(freeze),
 .state_guest_held(held),.state_request(request),.state_write(write),
 .state_index(index),.state_data(state_data),.state_frozen(frozen[0]),
 .state_done(done[0]),.state_error(error[0]),.state_result(result_mb),
 .sys_clk(ref_clk),
 MBPORTS
 cell_MB7071H original_mb(.sys_clk(ref_clk),
 MBREFPORTS
 cell_F10414_snapshot candidate_f(
 .state_host_clk(host_clk),.state_advance(advance),.state_freeze(freeze),
 .state_guest_held(held),.state_request(request),.state_write(write),
 .state_index(index),.state_data(state_data_f),.state_frozen(frozen[1]),
 .state_done(done[1]),.state_error(error[1]),.state_result(result_f),
 .sys_clk(ref_clk),
 FPORTS
 cell_F10414 original_f(.sys_clk(ref_clk),
 FREFPORTS
 genvar i;
 generate for(i=0;i<256;i=i+1)begin:observe
  assign memory_mb[i*4+:4]=candidate_mb.mem[i];
  assign ref_memory_mb[i*4+:4]=original_mb.mem[i];
  assign memory_f[i]=candidate_f.mem[i];
  assign ref_memory_f[i]=original_f.mem[i];
 end endgenerate
endmodule
'''
MBPORTS = '''.p2(we_mb_n),.p3(select_mb[3]),.p4(select_mb[2]),
 .p5(select_mb[1]),.p6(select_mb[0]),
 .p7(address[7]),.p8(address[6]),.p9(address[5]),.p10(address[4]),
 .p11(address[3]),.p12(address[2]),.p13(address[1]),.p14(address[0]),
 .p15(1'b0),.p16(data[3]),.p17(data[2]),.p18(data[1]),.p19(data[0]),
 .p20(q_mb[3]),.p21(q_mb[2]),.p22(q_mb[1]),.p23(q_mb[0]));'''
FPORTS = '''.p1(address[7]),.p2(address[6]),.p3(address[5]),.p4(address[4]),
 .p7(ce_f),.p9(address[3]),.p10(address[2]),.p11(address[1]),.p12(address[0]),
 .p13(data[0]),.p14(we_f_n),.p15(q_f));'''
FIXTURE = FIXTURE.replace('MBREFPORTS', MBPORTS.replace('q_mb', 'ref_q_mb'))
FIXTURE = FIXTURE.replace('FREFPORTS', FPORTS.replace('q_f', 'ref_q_f'))
FIXTURE = FIXTURE.replace('MBPORTS', MBPORTS).replace('FPORTS', FPORTS)


def run(output, mutations):
    output.mkdir(parents=True, exist_ok=False)
    sources = {kind: gen.generate(kind)[0] for kind in gen.SPECS}
    cases = [('candidate', None, None, None, None)]
    if mutations:
        for name, before, after, diagnostic in [
            ('ungated_guest', '!state_hold && state_advance &&', '!state_hold &&', 'held guest state changed'),
            ('freeze_writes', '!state_hold && state_advance &&', 'state_advance &&', 'held state differs'),
            ('running_access', 'state_hold && state_frozen && state_request', 'state_request', 'running request accepted'),
            ('entry_access', 'state_hold && state_frozen && state_request', 'state_hold && state_request', 'entry request accepted'),
            ('invalid_alias', 'wire state_valid = !state_index[8];', "wire state_valid = 1'b1;", 'invalid request accepted'),
            ('wrong_resume_address', 'state_read = mem[state_address];', "state_read = mem[8'b0];", 'asynchronous output differs'),
            ('lost_restore', 'mem[state_address] <= state_write_data;', "mem[state_address] <= '0;", 'ordinary oracle differs'),
        ]:
            for kind in gen.SPECS:
                old,new,why=before,after,diagnostic
                if kind=='F10414':
                    old=old.replace('<= state_data;', '<= state_data[0];')
                    new=new.replace("<= 4'b0;", "<= 1'b0;")
                    why=why.replace('original MB7071H', 'original F10414')
                cases.append((name+'_'+kind, kind, old, new, why))
    rows = []
    for name, target, before, after, diagnostic in cases:
        case = output/name
        case.mkdir()
        (case/'fixture.sv').write_text(FIXTURE)
        for kind, source in sources.items():
            if before is not None and kind == target:
                assert source.count(before) == 1, (name, before)
                source = source.replace(before, after)
            (case/(kind+'.sv')).write_text(source)
        command = ['verilator', '--cc', '--exe', '--build', '-j', '4',
            '--top-module', 'async256_fixture', '-Wno-fatal', '--Mdir', str(case/'obj'),
            str(case/'fixture.sv'), *[str(case/(k+'.sv')) for k in gen.SPECS],
            *[str(gen.ROOT/'verilog/cells'/('cell_'+k+'.v')) for k in gen.SPECS],
            str(HERE/'test_async256.cpp')]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vasync256_fixture')], capture_output=True, text=True, timeout=60)
        text = result.stdout+result.stderr
        (case/'test.txt').write_text(text)
        if diagnostic:
            assert result.returncode and diagnostic in text, (name, text)
        else:
            assert result.returncode == 0 and 'ASYNC256 PASS' in text, text
        rows.append(dict(case=name, returncode=result.returncode, expected_failure=diagnostic))
        print(name+': '+text.strip(), flush=True)
    controls = []
    for kind in gen.SPECS:
        original = (gen.ROOT/'verilog/cells'/('cell_'+kind+'.v')).read_text()
        for name, changed in [('extra_state', original.replace('endmodule', 'reg lost;\nendmodule')),
                              ('write_change', original.replace('mem[a] <=', 'mem[0] <='))]:
            assert changed != original
            try:
                gen.generate(kind, changed)
            except ValueError:
                controls.append(dict(kind=kind, case=name, rejected=True))
            else:
                raise AssertionError('unreviewed original accepted')
    paths = [Path(__file__).resolve(), HERE/'generate_async256.py', HERE/'test_async256.cpp']
    paths += [gen.ROOT/'verilog/cells'/('cell_'+k+'.v') for k in gen.SPECS]
    (output/'results.json').write_text(json.dumps(dict(cases=rows, source_controls=controls,
        manifests={k: gen.generate(k)[1] for k in gen.SPECS},
        hashes={str(p.relative_to(gen.ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}), indent=2)+'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    run(args.output.resolve(), args.mutations)
