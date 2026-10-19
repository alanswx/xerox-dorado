#!/usr/bin/env python3
"""Full processor-board original/restore test, including async RAM scan effects."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

import generate_processor_full as gen

HERE=Path(__file__).resolve().parent


def fixture(source,manifest):
    ports=[]
    for match in re.finditer(r'^\s+(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?(\w+)(?=[,\n])',source.read_text(),re.M):
        direction,msb,lsb,name=match.groups()
        if name=='sys_clk':continue
        ports.append(dict(direction=direction,name=name,width=1 if msb is None else abs(int(msb)-int(lsb))+1))
    inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output']
    ni=sum(p['width'] for p in inputs);no=sum(p['width'] for p in outputs)
    bits=manifest['logical_state_bits'];nw=len(manifest['files'])+len(manifest['rams'])
    code=[f'''module processor_full_fixture(
 input wire host_clk,run,run_ref,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,
 input wire [15:0] state_index,input wire [17:0] state_data,
 output wire frozen,done,error,output wire [17:0] result,
 output wire [{no-1}:0] original_outputs,candidate_outputs,
 output wire [{bits-1}:0] original_state,candidate_state,
 output wire advance,ref_advance,output wire [{nw-1}:0] ref_write);
 reg enabled=0,ref_enabled=0;
 always @(negedge host_clk)begin enabled<=run;ref_enabled<=run_ref;end
 wire guest_clk=host_clk & enabled;
 wire ref_clk=host_clk & ref_enabled;
 assign advance=enabled;assign ref_advance=ref_enabled;''']
    output_names={p['name'] for p in outputs}
    for copy,module,clock in [('original',manifest['original_module'],'ref_clk'),('candidate',manifest['module'],'guest_clk')]:
        for p in outputs:
            shape='' if p['width']==1 else f"[{p['width']-1}:0] "
            code.append(f"wire {shape}{copy}_{p['name']};")
        connections=['.sys_clk('+clock+')'];offset=0
        for p in inputs:
            name,width=p['name'],p['width']
            value=f'stimulus[{offset}]' if width==1 else f'stimulus[{offset}+:{width}]'
            if name+'__drv' in output_names:value+=' | '+copy+'_'+name+'__drv'
            connections.append(f'.{name}({value})');offset+=width
        connections += [f".{p['name']}({copy}_{p['name']})" for p in outputs]
        if copy=='candidate':
            connections += ['.state_host_clk(host_clk)','.state_guest_advance(enabled)',
                '.state_freeze(freeze)','.state_guest_held(held)','.state_request(request)',
                '.state_write(write)','.state_index(state_index)','.state_data(state_data)',
                '.state_frozen(frozen)','.state_done(done)','.state_error(error)','.state_result(result)']
        code += [f' {module} #(.SYSPER(4)) {copy}(',','.join(connections),');']
        code.append(f'assign {copy}_outputs={{'+','.join(copy+'_'+p['name'] for p in reversed(outputs))+'};')
    words=[];offset=0
    def state(word,width,expression):
        nonlocal offset
        words.append(dict(word=word,width=width,offset=offset))
        for copy in ('original','candidate'):
            code.append(f'assign {copy}_state[{offset}+:{width}]='+expression(copy)+';')
        offset+=width
    for row in manifest['ff']:
        state(row['word'],row['width'],lambda c,r=row:'{'+','.join(c+'.'+r['package']+'.'+f['name'] for f in reversed(r['fields']))+'}')
    for row in manifest['files']:
        for a in range(16):
            state(row['word']|a,4,lambda c,r=row,a=a:f"{c}.{r['package']}.mem[{a}]")
        state(row['word']|16,10,lambda c,r=row:'{'+','.join(c+'.'+r['package']+'.'+f['name'] for f in reversed(r['write_window']['fields']))+'}')
    for row in manifest['rams']:
        for a in range(256):
            state(row['word']|a,row['width'],lambda c,r=row,a=a:f"{c}.{r['package']}.mem[{a}]")
    state(manifest['phase']['word'],10,lambda c:'{'+','.join(c+'.'+name for name in reversed(manifest['phase']['fields']))+'}')
    assert offset==bits and len({w['word'] for w in words})==len(words)
    code.append('integer row; initial begin')
    for i,r in enumerate(manifest['files']+manifest['rams']):
        count=16 if r['kind']=='cell_F10145A' else 256;width=4 if count==16 else r['width']
        for copy in ('original','candidate'):
            code.append(f"for(row=0;row<{count};row=row+1){copy}.{r['package']}.mem[row]=(row*7+{i*3+1}) & {(1<<width)-1};")
    code.append('end')
    for i,r in enumerate(manifest['files']+manifest['rams']):
        base='original.'+r['package']+'.'
        if r['kind']=='cell_F10145A':predicate=base+'p13 && !'+base+'we_d && '+base+'ce_lat'
        elif r['kind']=='cell_MB7071H':predicate='!'+base+'p2 && !('+ ' | '.join(base+'p'+str(p) for p in (3,4,5,6))+')'
        else:predicate='!'+base+'p14 && !'+base+'p7'
        code.append(f'assign ref_write[{i}]={predicate};')
    code.append('endmodule')
    header='#pragma once\n'+f'constexpr unsigned INPUTS={ni},OUTPUTS={no},WORDS={len(words)},STATE_BITS={bits},INDEX_COUNT=65536,WRITERS={nw};\n'
    header+='struct StateWord { unsigned word,width,offset; };\nconstexpr StateWord state_words[]={\n'
    header+=',\n'.join('{'+f"{w['word']},{w['width']},{w['offset']}"+'}' for w in words)+'};\n'
    return '\n'.join(code)+'\n',header,dict(ports=ports,state_words=words)


def run(kind,source,output,mutations):
    output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(kind,source)
    top,header,metadata=fixture(source,manifest)
    cells=[gen.ROOT/'verilog/cells'/f'{cell}.v' for cell in manifest['original_sources']]
    cases=[('candidate',board,clones,None)]
    if mutations:
        for name,target,before,after,diagnostic in [
            ('ungated_ff','board','.state_advance(state_guest_advance)',".state_advance(1'b1)",'board physical state differs'),
            ('ungated_phase','board','else if (state_guest_advance) begin','else begin','board physical state differs'),
            ('running_access','board','state_hold && state_frozen && state_request','state_request','running request accepted'),
            ('lost_1668_history','cell_MC1668_snapshot.v','{ckb_d,cka_d,qb,qa}<=state_data[3:0];','{qb,qa}<=state_data[1:0];','state bit write/readback differs'),
            ('lost_quint_latch','cell_MC10175_snapshot.v','{q}<=state_data[4:0];',"q<=5'b0;",('state bit write/readback differs','board physical state differs')),
            ('lost_large_ram','cell_MB7071H_snapshot.sv','state_hold ? state_data :',"state_hold ? 4'b0 :",'state bit write/readback differs'),
            ('lost_small_window','cell_F10145A_snapshot.sv','{ce_lat,d_lat,a_lat,we_d} <= state_data;',"{ce_lat,d_lat,a_lat,we_d} <= 10'b0;",('state bit write/readback differs','board physical state differs')),
            ('invalid_alias','board','&& state_row==0;\n  wire state_is_small=', '&& state_row<2;\n  wire state_is_small=', 'invalid state access accepted'),
        ]:
            original=board if target=='board' else clones[target]
            assert before in original,(name,before)
            changed=original.replace(before,after,1);copies=clones.copy()
            if target!='board':copies[target]=changed
            cases.append((name,changed if target=='board' else board,copies,diagnostic))
    results=[]
    for name,text,copies,diagnostic in cases:
        case=output/name;case.mkdir()
        (case/'board.sv').write_text(text);(case/'fixture.sv').write_text(top);(case/'ports.h').write_text(header)
        for filename,contents in copies.items():(case/filename).write_text(contents)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','processor_full_fixture',
            '-Wno-fatal','-DDORADO_F10145_LOGIC','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
            str(case/'fixture.sv'),str(case/'board.sv'),*[str(case/n) for n in copies],
            str(source),*map(str,cells),str(HERE/'test_processor_full.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vprocessor_full_fixture')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        print(kind+'/'+name+': '+text.strip(),flush=True)
        if diagnostic:
            reasons=(diagnostic,)if isinstance(diagnostic,str)else diagnostic
            assert result.returncode and any(reason in text for reason in reasons),(name,text)
        else:assert result.returncode==0 and 'FULL PROCESSOR RESTORE PASS'in text,text
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (output/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    paths=[Path(__file__).resolve(),HERE/'test_processor_full.cpp',HERE/'generate_processor_full.py',
           HERE/'generate_processor_ff.py',HERE/'generate_snapshot_ff.py',HERE/'generate_contb_full.py',
           HERE/'generate_contb.py',HERE/'audit_contb_im.py',HERE/'generate_f10145.py',HERE/'generate_async256.py',source,*cells,*[gen.ROOT/p['file']for p in manifest['immutable_proms']]]
    (output/'results.json').write_text(json.dumps(dict(scope=manifest['scope'],
        hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},cases=results),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--kind',choices=gen.SPECS,required=True)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--mutations',action='store_true')
    args=parser.parse_args();run(args.kind,args.source.resolve(),args.output.resolve(),args.mutations)
