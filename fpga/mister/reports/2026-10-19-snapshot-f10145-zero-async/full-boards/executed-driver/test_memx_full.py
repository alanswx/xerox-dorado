#!/usr/bin/env python3
"""Complete native MemX original/oracle/physical-probe/state-restore tests."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

import generate_memx_full as gen

HERE=Path(__file__).resolve().parent


def fixture(root,m):
    source=root/'verilog/generated/MemX-Rev-Ch.v';ports=[]
    for match in re.finditer(r'^\s+(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?(\w+)(?=[,\n])',source.read_text(),re.M):
        direction,msb,lsb,name=match.groups()
        if name!='sys_clk':ports.append(dict(direction=direction,name=name,width=1 if msb is None else abs(int(msb)-int(lsb))+1))
    inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output']
    ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs);nr=len(m['rams']);ns=len(m['files'])
    aux_bits=m['logical_state_bits']-nr*65536
    code=[f'''module memx_full_fixture(
 input wire host_clk,run,run_ref,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [23:0] state_index,input wire [17:0] state_data,
 input wire [15:0] probe_row,output wire frozen,done,error,output wire [17:0] result,
 output wire [{no-1}:0] original_outputs,candidate_outputs,
 output wire [{aux_bits-1}:0] original_ff,candidate_ff,
 output wire [{nr-1}:0] original_probe,candidate_probe,map_data,map_ras_n,map_cas_n,map_we_n,
 output wire [{nr*8-1}:0] map_addresses,output wire [{ns-1}:0] small_write,
 output wire advance,ref_advance);
 reg enabled=0,ref_enabled=0;
 always @(negedge host_clk)begin enabled<=run;ref_enabled<=run_ref;end
 wire guest_clk=host_clk & enabled,ref_clk=host_clk & ref_enabled;
 assign advance=enabled;assign ref_advance=ref_enabled;''']
    output_names={p['name']for p in outputs}
    for copy,module,clock in [('original',m['original_module'],'ref_clk'),('candidate',m['module'],'guest_clk')]:
        for p in outputs:code.append('wire '+(''if p['width']==1 else f"[{p['width']-1}:0] ")+copy+'_'+p['name']+';')
        connections=['.sys_clk('+clock+')'];offset=0
        for p in inputs:
            value=f'stimulus[{offset}]'if p['width']==1 else f"stimulus[{offset}+:{p['width']}]"
            if p['name']+'__drv'in output_names:value+=' | '+copy+'_'+p['name']+'__drv'
            connections.append(f".{p['name']}({value})");offset+=p['width']
        connections +=[f".{p['name']}({copy}_{p['name']})"for p in outputs]
        if copy=='candidate':connections +=['.state_host_clk(host_clk)','.state_guest_advance(enabled)',
            '.state_freeze(freeze)','.state_guest_held(held)','.state_request(request)','.state_write(write)',
            '.state_index(state_index)','.state_data(state_data)','.state_frozen(frozen)',
            '.state_done(done)','.state_error(error)','.state_result(result)']
        code +=[f' {module} #(.SYSPER(4)) {copy}(',','.join(connections),');']
        code.append('assign '+copy+'_outputs={'+','.join(copy+'_'+p['name']for p in reversed(outputs))+'};')
    words=[];offset=0
    def aux(word,width,expression):
        nonlocal offset
        words.append(dict(word=word,width=width,offset=offset))
        for copy in ('original','candidate'):code.append(f'assign {copy}_ff[{offset}+:{width}]='+expression(copy)+';')
        offset+=width
    for row in m['ff']:
        aux(row['word'],row['width'],lambda c,r=row:'{'+','.join(c+'.'+r['package']+'.'+f['name']for f in reversed(r['fields']))+'}')
    for row in m['files']:
        for a in range(16):aux(row['word']|a,4,lambda c,r=row,a=a:f"{c}.{r['package']}.mem[{a}]")
        aux(row['word']|16,10,lambda c,r=row:'{'+','.join(c+'.'+r['package']+'.'+f['name']for f in reversed(r['write_window']['fields']))+'}')
        base='original.'+row['package']+'.';code.append(f"assign small_write[{row['memory_index']}]={base}p13 && !{base}we_d && {base}ce_lat;")
    aux(m['phase']['word'],14,lambda c:'{'+','.join(c+'.'+f['name']for f in reversed(m['phase']['fields']))+'}')
    map_aux_start=len(words)
    for i,row in enumerate(m['rams']):
        package=row['package'];fields=row['history']['fields']
        for copy in ('original','candidate'):
            value=copy+'.'+package+'.state_history'if copy=='candidate'else'{'+','.join(copy+'.'+package+'.'+f['name']for f in reversed(fields))+'}'
            code.append(f'wire [22:0] {copy}_map_history_{i}={value};')
        aux(row['word']|65536,18,lambda c,i=i:f'{c}_map_history_{i}[17:0]')
        aux(row['word']|65537,5,lambda c,i=i:f'{c}_map_history_{i}[22:18]')
        for copy in ('original','candidate'):code.append(f'assign {copy}_probe[{i}]={copy}.{package}.mem[probe_row];')
        for port,pin in [('map_data','p2'),('map_ras_n','p4'),('map_cas_n','p15'),('map_we_n','p3')]:code.append(f'assign {port}[{i}]=original.{package}.{pin};')
        code.append(f'assign map_addresses[{i*8}+:8]=original.{package}.a;')
    assert offset==aux_bits
    code.append('integer row; initial begin')
    for i,r in enumerate(m['files']):
        for copy in ('original','candidate'):code.append(f"for(row=0;row<16;row=row+1){copy}.{r['package']}.mem[row]=(row*7+{i*3+1})&15;")
    for i,r in enumerate(m['rams']):
        value=f"((row*32'd1103515245+32'd12345+32'd{i*7919}) ^ (row>>3) ^ (row>>7) ^ (row>>11) ^ (row>>15)) & 1"
        for copy in ('original','candidate'):code.append(f"for(row=0;row<65536;row=row+1){copy}.{r['package']}.mem[row]={value};")
    code+=['end','endmodule']
    header='#pragma once\n'+f'constexpr unsigned INPUTS={ni},OUTPUTS={no},AUX_BITS={aux_bits},WORDS={len(words)},MAPS={nr},SMALLS={ns},MAP_AUX_START={map_aux_start},INDEX_COUNT=16777216,STATE_BITS={m["logical_state_bits"]};\n'
    header+='struct StateWord{unsigned word,width,offset;};\nconstexpr StateWord state_words[]={\n'
    header+=',\n'.join('{'+f"{w['word']},{w['width']},{w['offset']}"+'}'for w in words)+'};\n'
    header+='constexpr unsigned MAP_PACKAGES[]={'+','.join(str(r['index'])for r in m['rams'])+'};\n'
    offset=0
    for p in inputs:header+=f"constexpr unsigned PIN_{p['name']}={offset};\n";offset+=p['width']
    return '\n'.join(code)+'\n',header,dict(ports=ports,state_words=words,map_aux_start=map_aux_start)


def run(root,out,mutations):
    out.mkdir(parents=True,exist_ok=False);board,m,clones=gen.generate(root);top,header,metadata=fixture(root,m)
    cells=[root/n for n in m['original_sources']];cases=[('candidate',board,clones,None)]
    (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');(out/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    for p in [Path(__file__).resolve(),HERE/'generate_memx_full.py',HERE/'test_memx_full.cpp']:(out/(p.stem+'-executed'+p.suffix)).write_bytes(p.read_bytes())
    if mutations:
        for name,target,before,after,diagnostic in [
            ('ungated_ff','board','.state_advance(state_guest_advance)',".state_advance(1'b1)",'board auxiliary state differs'),
            ('ungated_phase','board','else if(state_guest_advance)begin','else begin','board auxiliary state differs'),
            ('running_access','board','state_hold && state_frozen && state_request','state_request','running request accepted'),
            ('freeze_without_held','board','state_hold=state_freeze && state_guest_held','state_hold=state_freeze','unheld freeze accepted'),
            ('response_truncation','board','reg [4:0] state_response_memory_index;','reg [3:0] state_response_memory_index;','aux state export differs'),
            ('invalid_alias','board',"state_package<7'd74 && state_row==0;","state_package<7'd74 && state_row<2;",('invalid state access accepted','board auxiliary state differs')),
            ('lost_map_history','cell_MosRam_snapshot.sv','we_d,saved_dout_r,dout','we_d,dout_r,dout','aux bit write/readback differs'),
            ('ungated_map_read','cell_MosRam_snapshot.sv','state_scan || (!state_hold && state_advance)',"1'b1",'board auxiliary state differs'),
            ('lost_small_window','cell_F10145A_snapshot.sv','{ce_lat,d_lat,a_lat,we_d} <= state_data;',"{ce_lat,d_lat,a_lat,we_d} <= 10'b0;",('aux bit write/readback differs','board auxiliary state differs')),
        ]:
            s=board if target=='board'else clones[target];assert before in s,(name,before);changed=s.replace(before,after,1);copies=clones.copy()
            if target!='board':copies[target]=changed
            cases.append((name,changed if target=='board'else board,copies,diagnostic))
    results=[]
    for name,text,copies,diagnostic in cases:
        case=out/name;case.mkdir()
        for n,s in {'board.sv':text,'fixture.sv':top,'ports.h':header,**copies}.items():(case/n).write_text(s)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','memx_full_fixture','-Wno-fatal','-DDORADO_F10145_LOGIC',
            '--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),str(case/'board.sv'),*[str(case/n)for n in copies],
            str(root/'verilog/generated/MemX-Rev-Ch.v'),*map(str,cells),str(HERE/'test_memx_full.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        q=subprocess.run([str(case/'obj/Vmemx_full_fixture')],capture_output=True,text=True,timeout=1200);message=q.stdout+q.stderr
        (case/'test.txt').write_text(message);print(name+': '+message.strip(),flush=True)
        if diagnostic:
            wanted=(diagnostic,)if isinstance(diagnostic,str)else diagnostic;assert q.returncode and any(x in message for x in wanted),(name,message)
        else:assert q.returncode==0 and 'FULL MemX RESTORE PASS'in message,message
        results.append(dict(case=name,returncode=q.returncode,expected_failure=diagnostic))
    paths=[Path(__file__).resolve(),HERE/'test_memx_full.cpp',HERE/'generate_memx_full.py',HERE/'generate_mosram.py',
        HERE/'generate_contb_full.py',HERE/'generate_contb.py',HERE/'generate_snapshot_ff.py',HERE/'generate_processor_ff.py',HERE/'generate_jk.py',HERE/'generate_f10145.py',
        root/'verilog/generated/MemX-Rev-Ch.v',*cells,*[root/p['file']for p in m['immutable_proms']]]
    (out/'results.json').write_text(json.dumps(dict(scope=m['scope'],cases=results,
        source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.source_root.resolve(),a.output.resolve(),a.mutations)
