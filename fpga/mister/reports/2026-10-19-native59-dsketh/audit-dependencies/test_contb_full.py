#!/usr/bin/env python3
"""Check full modeled ContB continue/restore against an untouched original."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

import audit_contb_im as im
import generate_contb_full as gen
from test_contb_snapshot import fixture as ram_fixture

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def fixture(manifest):
    text, header, ports = ram_fixture(manifest)
    text = gen.replace_once(text, 'module contb_snapshot_fixture(', 'module contb_full_fixture(')
    text = gen.replace_once(text, 'input wire host_clk,run,', 'input wire host_clk,run,run_ref,')
    text = gen.replace_once(text, 'input wire [13:0] state_index,', 'input wire [14:0] state_index,')
    text = gen.replace_once(text, 'output wire [7:0] ce_n,we_n);',
                           'output wire [7:0] ce_n,we_n,\n output wire ref_advance,\n output wire [208:0] original_ff,candidate_ff);')
    text = gen.replace_once(text, 'always @(negedge host_clk) enabled <= run;',
                           'reg ref_enabled=0;\n always @(negedge host_clk) begin enabled <= run; ref_enabled <= run_ref; end\n wire ref_clk=host_clk & ref_enabled;\n assign ref_advance=ref_enabled;')
    text = text.replace('.sys_clk(guest_clk)', '.sys_clk(ref_clk)', 1)
    text = gen.replace_once(text, 'ContB_snapshot_experiment #', gen.MODULE+' #')
    code, offset = [], 0
    for cell in manifest['ff']:
        width, pkg = cell['width'], cell['package']
        for copy in ('original', 'candidate'):
            fields = [f'{copy}.u_{pkg}.{f["name"]}' for f in reversed(cell['fields'])]
            code.append(f' assign {copy}_ff[{offset}+:{width}] = {{'+', '.join(fields)+'};')
        offset += width
    assert offset == 209
    text = gen.replace_once(text, 'endmodule', '\n'.join(code)+'\nendmodule')
    header += 'constexpr unsigned FF_COUNT=22;\n'
    header += 'constexpr unsigned FF_WIDTHS[]={'+','.join(str(c['width']) for c in manifest['ff'])+'};\n'
    offsets, offset = [], 0
    for c in manifest['ff']:
        offsets.append(offset)
        offset += c['width']
    header += 'constexpr unsigned FF_OFFSETS[]={'+','.join(map(str,offsets))+'};\n'
    return text, header, ports


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=False)
    board, manifest, clones = gen.generate()
    top, header, ports = fixture(manifest)
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'ports.json').write_text(json.dumps(ports,indent=2)+'\n')
    cells = [ROOT/'verilog/cells'/f'{k}.v' for k in sorted({k for k,_,_ in im.instances(im.GENERATED.read_text())})]
    cases = [('candidate',board,clones,None)]
    if mutations:
        for name, target, before, after, diagnostic in (
            ('lost_history','cell_MC10176_snapshot.v',
             '{ck_d, d_pre, q} <= state_data[12:0];',
             '{d_pre, q} <= state_data[11:0];','FF bit write/readback differs'),
            ('lost_precapture','cell_MC10176_snapshot.v',
             '{ck_d, d_pre, q} <= state_data[12:0];',
             '{ck_d, q} <= {state_data[12],state_data[5:0]};','FF bit write/readback differs'),
            ('running_write','board',
             'wire state_ff_write = state_accept && state_write && state_valid_ff;',
             'wire state_ff_write = state_write && state_valid_ff;','board FF state differs'),
            ('ungated_ff','board', '.state_advance(state_guest_advance)',
             ".state_advance(1'b1)",'board FF state differs'),
            ('ram_alias','board',
             ".request(state_accept && !state_index[14] && state_index[13:11] == 3'd0)",
             ".request(state_accept && state_index[13:11] == 3'd0)",'RAM changed by FF write'),
            ('wrong_ff_response','board',
             'state_valid_ff ? state_ff_result[state_index[10:0]]',
             "state_valid_ff ? state_ff_result[state_index[10:0] ^ 11'd1]",'FF export differs'),
        ):
            source = board if target=='board' else clones[target]
            assert before in source,name
            changed = source.replace(before,after,1)
            altered = clones.copy()
            if target!='board':
                altered[target]=changed
            cases.append((name,changed if target=='board' else board,altered,diagnostic))
    rows=[]
    for name, altered, copies, diagnostic in cases:
        case=out/name
        case.mkdir()
        (case/'ContB_full_snapshot.sv').write_text(altered)
        (case/'fixture.sv').write_text(top)
        (case/'ports.h').write_text(header)
        for n,source in copies.items():
            (case/n).write_text(source)
        command=['verilator','--cc','--exe','--build','-j','4',
                 '--top-module','contb_full_fixture','-Wno-fatal','-DDORADO_F10415_M10K',
                 '--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
                 str(case/'fixture.sv'),str(case/'ContB_full_snapshot.sv'),
                 *[str(case/n) for n in copies],str(im.GENERATED),
                 str(HERE/'f10415_enable_bank.sv'),*map(str,cells),str(HERE/'test_contb_full.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vcontb_full_fixture')],capture_output=True,text=True,timeout=90)
        text=result.stdout+result.stderr
        (case/'test.txt').write_text(text)
        print(name+': '+text.strip(),flush=True)
        if diagnostic:
            assert result.returncode and diagnostic in text,(name,text)
        else:
            assert result.returncode==0 and 'FULL CONTB RESTORE PASS' in text,text
        rows.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    paths=[Path(__file__),HERE/'test_contb_full.cpp',HERE/'generate_contb_full.py',
           HERE/'generate_contb.py',HERE/'audit_contb_im.py',HERE/'test_contb_snapshot.py',
           HERE/'f10415_enable_bank.sv',im.GENERATED,*cells]
    (out/'results.json').write_text(json.dumps(dict(
        scope='Full modeled ContB same-build state access/continue/restore in isolated RTL; fixture-only RAM seed and captured external stimulus; no global barrier/whole-machine/physical restore or timing claim',
        hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
        cases=rows),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--mutations',action='store_true')
    args=parser.parse_args()
    if args.output:
        run(args.output.resolve(),args.mutations)
    else:
        with tempfile.TemporaryDirectory(prefix='dorado-contb-full-',dir='/tmp') as tmp:
            run(Path(tmp)/'test',args.mutations)
