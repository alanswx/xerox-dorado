#!/usr/bin/env python3
"""Check explicit-enable RAM snapshots against original gated-clock cells."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
FIXTURE = '''module snapshot_enable_fixture(
 input wire guest_advance,host_clk,ref_clk,input wire [9:0] address,
 input wire [17:0] data,input wire ce_n,we_n,
 output wire [17:0] q,original_q,
 input wire freeze,guest_held,request,write,
 input wire [10:0] word_index,input wire [17:0] state_data,
 output wire frozen,done,error,output wire [17:0] result);
 dorado_snapshot_f10415_enable_bank candidate(host_clk,guest_advance,address,data,
   ce_n,we_n,q,freeze,guest_held,frozen,request,write,word_index,state_data,
   done,error,result);
 genvar i;
 generate for(i=0;i<18;i=i+1)begin: original
   cell_F10415A bit_cell(ref_clk,address[9],address[8],address[7],address[6],
     address[5],address[4],address[3],address[2],address[1],address[0],
     data[i],ce_n,we_n,original_q[i]);
 end endgenerate
endmodule
'''


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=True)
    source = (HERE/'f10415_enable_bank.sv').read_text()
    cases = [('candidate', source, None)]
    if mutations:
        for name, before, after, diagnostic in (
            ('ungated_read', 'wire read_enable = scan_active || (!freeze_active && guest_advance);',
             "wire read_enable = 1'b1;", 'original cell output differs during backend hold'),
            ('premature_resume', 'if (!freeze_active && guest_advance)',
             'if (!freeze_active)', 'restored pipeline differs before first guest edge'),
            ('lost_pipeline', 'saved_q <= state_data;', "saved_q <= 18'b0;", 'restored pipeline differs'),
            ('running_access', 'wire accepted = scan_active && request;',
             'wire accepted = request;', 'state access accepted while running'),
            ('invalid_alias', "wire valid_index = word_index <= 11'd1024;",
             "wire valid_index = 1'b1;", 'invalid state access accepted'),
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
                   '--top-module', 'snapshot_enable_fixture', '-Wno-fatal',
                   '-DDORADO_F10415_M10K', '--Mdir', str(case/'obj'),
                   str(case/'fixture.sv'), str(case/'bank.sv'),
                   str(ROOT/'verilog/cells/cell_F10415A.v'), str(HERE/'test_f10415_enable.cpp')]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vsnapshot_enable_fixture')], capture_output=True,
                                text=True, timeout=60)
        text = result.stdout+result.stderr
        (case/'test.txt').write_text(text)
        if diagnostic:
            assert result.returncode and diagnostic in text, (name, text)
        else:
            assert result.returncode == 0 and 'ENABLED SNAPSHOT RAM PASS' in text, text
        rows.append(dict(case=name, returncode=result.returncode, expected_failure=diagnostic))
        print(name+': '+text.strip(), flush=True)
    paths = (HERE/'f10415_enable_bank.sv', HERE/'test_f10415_enable.cpp',
             Path(__file__).resolve(), ROOT/'verilog/cells/cell_F10415A.v')
    (out/'results.json').write_text(json.dumps(dict(
        scope='18-bit half-bank clock-enable RTL only; no actual-board/global barrier/fit/physical save-load qualification',
        hashes={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},
        cases=rows), indent=2)+'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    if args.output:
        run(args.output.resolve(), args.mutations)
    else:
        with tempfile.TemporaryDirectory(prefix='dorado-snapshot-enable-', dir='/tmp') as tmp:
            run(Path(tmp), args.mutations)
