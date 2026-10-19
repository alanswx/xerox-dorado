#!/usr/bin/env python3
"""Qualify bounded RAM snapshot access against unchanged original cells."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
FIXTURE = '''module snapshot_fixture(
 input wire clk, ref_clk, input wire [9:0] address,
 input wire [33:0] data, input wire ce_n,we_n,
 output wire [33:0] q,original_q,
 input wire freeze,request,write,
 input wire [10:0] word_index,input wire [33:0] state_data,
 output wire frozen,done,error,output wire [33:0] result);
 dorado_snapshot_f10415_bank candidate(clk,address,data,ce_n,we_n,q,
   freeze,frozen,request,write,word_index,state_data,done,error,result);
 genvar i;
 generate for(i=0;i<34;i=i+1)begin: original
   cell_F10415A bit_cell(ref_clk,address[9],address[8],address[7],address[6],
     address[5],address[4],address[3],address[2],address[1],address[0],
     data[i],ce_n,we_n,original_q[i]);
 end endgenerate
endmodule
'''


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=True)
    source = (HERE/'f10415_bank.sv').read_text()
    cases = [('production', source, None)]
    if mutations:
        for name, before, after, diagnostic in (
            ('lost_pipeline', 'saved_q <= state_data[bit_index];', 'saved_q <= 1\'b0;',
             'original cell output differs'),
            ('scan_visible', '(frozen ? saved_q : read_q)', 'read_q',
             'original cell output differs'),
            ('running_access', 'wire accepted = freeze && frozen && request;',
             'wire accepted = request;', 'state access accepted while running'),
            ('invalid_alias', 'wire valid_index = word_index <= 11\'d1024;',
             'wire valid_index = 1\'b1;', 'invalid state access accepted'),
        ):
            assert source.count(before) == 1, name
            cases.append((name, source.replace(before, after), diagnostic))
    rows = []
    for name, text, diagnostic in cases:
        case = out/name
        case.mkdir(exist_ok=True)
        (case/'fixture.sv').write_text(FIXTURE)
        (case/'bank.sv').write_text(text)
        command = ['verilator', '--cc', '--exe', '--build', '-j', '4',
                   '--top-module', 'snapshot_fixture', '-Wno-fatal',
                   '-DDORADO_F10415_M10K', '--Mdir', str(case/'obj'),
                   str(case/'fixture.sv'), str(case/'bank.sv'),
                   str(ROOT/'verilog/cells/cell_F10415A.v'), str(HERE/'test_f10415.cpp')]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vsnapshot_fixture')],
                                capture_output=True, text=True, timeout=60)
        output = result.stdout+result.stderr
        (case/'test.txt').write_text(output)
        if diagnostic:
            assert result.returncode and diagnostic in output, (name, result.returncode, output)
        else:
            assert result.returncode == 0 and 'SNAPSHOT RAM PASS' in output, output
        rows.append(dict(case=name, returncode=result.returncode, expected_failure=diagnostic))
        print(name+': '+output.strip(), flush=True)
    hashes = {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
              for p in (HERE/'f10415_bank.sv', HERE/'test_f10415.cpp',
                        ROOT/'verilog/cells/cell_F10415A.v')}
    (out/'results.json').write_text(json.dumps(dict(
        scope='Isolated 34-slice IM-like RAM bank; no full board, clock barrier, external RAM or complete-machine restore',
        hashes=hashes, cases=rows), indent=2)+'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    if args.output:
        run(args.output.resolve(), args.mutations)
    else:
        with tempfile.TemporaryDirectory(prefix='dorado-snapshot-ram-', dir='/tmp') as tmp:
            run(Path(tmp), args.mutations)
