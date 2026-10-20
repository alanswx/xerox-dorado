#!/usr/bin/env python3
"""Compare new MSA helpers to original cells and independent pin/state oracles."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_msa_ff as gen

HERE=Path(__file__).resolve().parent
CASES=[('cell_SN74166',None),('cell_SN74S174',None),('cell_MK4096P_addr',7),('cell_MK4096P_addr',8)]


def fixture():
    text='''module msa_ff_fixture(input wire host_clk,ref_clk,advance,hold,write,
 input wire [2:0] selected,input wire [17:0] data,
 input wire [31:0] pins[4],output wire [17:0] physical[4],original_state[4],result[4],
 output wire [15:0] outputs[4],original_outputs[4]);
'''
    for i,(kind,row_bits) in enumerate(CASES):
        param=f' #(.ROW_BITS({row_bits}))' if row_bits else ''
        if row_bits:
            pairs=f'.p4(pins[{i}][3]),.p15(pins[{i}][14]),.a(pins[{i}][{16+row_bits-1}:16]),.addr(outputs[{i}][{2*row_bits-1}:0])'
            output_width=2*row_bits
        else:
            source=(gen.ROOT/'verilog/cells'/(kind+'.v')).read_text()
            inputs=re.findall(r'\binput\s+wire\s+(p\d+)\b',source)
            outputs=re.findall(r'\boutput\s+wire\s+(p\d+)\b',source)
            output_width=1 if i==0 else 6
            output_bits={'p13':0} if i==0 else dict(p2=0,p5=1,p7=2,p10=3,p12=4,p15=5)
            pairs=','.join(f'.{p}(pins[{i}][{int(p[1:])-1}])'for p in inputs)+','
            pairs+=','.join(f'.{p}(outputs[{i}][{output_bits[p]}])'for p in outputs)
        text+=f'begin:case{i}\n{kind}_msa_snapshot{param} cell_i(.state_host_clk(host_clk),.state_advance(advance),.state_hold(hold),.state_write(write && selected==3\'d{i}),.state_data(data),.state_result(result[{i}]),.sys_clk(ref_clk),{pairs});\n'
        text+=f'{kind}{param} ref_i(.sys_clk(ref_clk),{pairs.replace("outputs[","original_outputs[")});\n'
        width=sum(w for _,w in gen.fields(kind,row_bits or 8))
        for output,prefix in [('physical','cell_i'),('original_state','ref_i')]:
            packed='{'+','.join(prefix+'.'+name for name,_ in reversed(gen.FIELDS[kind]))+'}'
            text+=f"assign {output}[{i}]={{{{{18-width}{{1'b0}}}},{packed}}};\n"
        if output_width<16:
            text+=f'assign outputs[{i}][15:{output_width}]=0;assign original_outputs[{i}][15:{output_width}]=0;\n'
        text+='end\n'
    return text+'endmodule\n'


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False)
    inputs=out/'executed-inputs';inputs.mkdir()
    for p in [Path(__file__).resolve(),HERE/'generate_msa_ff.py',HERE/'generate_snapshot_ff.py',HERE/'test_msa_ff.cpp']:
        (inputs/p.name).write_bytes(p.read_bytes())
    for kind in gen.FIELDS:
        raw=(gen.ROOT/'verilog/cells'/(kind+'.v')).read_bytes()
        assert hashlib.sha256(raw).hexdigest()==gen.SHA[kind]
        (inputs/(kind+'.v')).write_bytes(raw)
    generated=gen.generate()
    cases=[('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('lost_hold','cell_SN74166','if(state_hold)begin',"if(1'b0)begin",'physical state oracle differs'),
            ('lost_advance','cell_SN74S174','else if(state_advance)begin',"else if(1'b1)begin",'physical state oracle differs'),
            ('reversed_parallel_load','cell_SN74166','{p14,p12,p11,p10,p5,p4,p3,p2}','{p2,p3,p4,p5,p10,p11,p12,p14}','physical state oracle differs'),
            ('clock_gated_clear','cell_SN74S174',"if (!p1)","if (!p1 && ck_en)",'physical state oracle differs'),
            ('lost_history_restore','cell_SN74166','{ck_d,q} <= state_data[(9)-1:0];',"{ck_d,q} <= {1'b0,state_data[7:0]};",'physical state oracle differs'),
            ('swapped_row_column','cell_MK4096P_addr','assign addr = {row, col};','assign addr = {col, row};','output oracle differs'),
            ('lost_strobe_restore','cell_MK4096P_addr','{cas_d,ras_d,col,row} <= state_data[(2*ROW_BITS+2)-1:0];',"{cas_d,ras_d,col,row} <= {2'b0,state_data[2*ROW_BITS-1:0]};",'physical state oracle differs'),
        ]
    results=[]
    for label,kind,before,after,why in cases:
        case=out/label;case.mkdir();paths=[]
        for name,source in generated.items():
            if kind and name==kind+'_msa_snapshot.sv':
                assert source.count(before)==1,(label,before)
                source=source.replace(before,after,1)
            p=case/name;p.write_text(source);paths.append(p)
        (case/'fixture.sv').write_text(fixture())
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_ff_fixture','-Wno-fatal','--Mdir',str(case/'obj'),str(case/'fixture.sv'),*map(str,paths),*[str(inputs/(k+'.v'))for k in gen.FIELDS],str(inputs/'test_msa_ff.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w')as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_ff_fixture')],capture_output=True,text=True,timeout=60)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log)
        print(label+': '+log.strip(),flush=True)
        if why:assert result.returncode and why in log,(label,log)
        else:assert result.returncode==0 and 'MSA_FF PASS' in log,log
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    controls=[]
    for kind in gen.FIELDS:
        source=(inputs/(kind+'.v')).read_text().replace('endmodule','reg hidden;\nendmodule')
        try:gen.generate_one(kind,source)
        except ValueError as error:controls.append(dict(case=kind+'_source',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('unreviewed source accepted')
    result=dict(status='PASS',scope=__doc__,representative_bits=50,source_controls=controls,cases=results,
        actual_native_msa_bits_per_board=636,actual_native_msa_instances=8,
        limits='Three primitive families only; not MSA board/frontend access, native hold barriers, complete snapshots or timing.',
        source_sha256=gen.SHA,generated_sha256={n:hashlib.sha256(s.encode()).hexdigest()for n,s in generated.items()},
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()})
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.mutations)
