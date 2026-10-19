#!/usr/bin/env python3
"""Complete native MemD original/oracle/hold/scan/destructive-restore tests."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

import generate_memd_full as gen

HERE=Path(__file__).resolve().parent


def fixture(root,manifest):
    source=root/'verilog/generated/MemD-Rev-Ca.v';ports=[]
    for m in re.finditer(r'^\s+(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?(\w+)(?=[,\n])',source.read_text(),re.M):
        direction,msb,lsb,name=m.groups()
        if name!='sys_clk':ports.append(dict(direction=direction,name=name,width=1 if msb is None else abs(int(msb)-int(lsb))+1))
    inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output']
    ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs);nb=len(manifest['banks']);ns=len(manifest['files'])
    aux_bits=manifest['ff_bits']+ns*74+manifest['phase_bits']+manifest['storage_input_bits']
    code=[f'''module memd_full_fixture(
 input wire host_clk,run,run_ref,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [19:0] state_index,input wire [17:0] state_data,
 input wire [11:0] probe_row,
 output wire frozen,done,error,output wire [17:0] result,
 output wire [{no-1}:0] original_outputs,candidate_outputs,
 output wire [{aux_bits-1}:0] original_ff,candidate_ff,
 output wire [71:0] original_raw,candidate_raw,data_bits,original_probe,candidate_probe,
 output wire [{nb*12-1}:0] addresses,output wire [{nb-1}:0] we_n,ce_n,
 output wire [{ns-1}:0] small_write,output wire advance,ref_advance,
 output wire [17:0] storage_data,output wire storage_clock);
 reg enabled=0,ref_enabled=0;
 always @(negedge host_clk)begin enabled<=run;ref_enabled<=run_ref;end
 wire guest_clk=host_clk & enabled,ref_clk=host_clk & ref_enabled;
 assign advance=enabled;assign ref_advance=ref_enabled;''']
    output_names={p['name']for p in outputs}
    for copy,module,clock in [('original',manifest['original_module'],'ref_clk'),('candidate',manifest['module'],'guest_clk')]:
        for p in outputs:
            shape=''if p['width']==1 else f"[{p['width']-1}:0] "
            code.append(f"wire {shape}{copy}_{p['name']};")
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
        code +=[f' {module} #(.SYSPER(4),.STORAGE_INPUT_DELAY(2)) {copy}(',','.join(connections),');']
        code.append('assign '+copy+'_outputs={'+','.join(copy+'_'+p['name']for p in reversed(outputs))+'};')
    words=[];offset=0
    def aux(word,width,expression):
        nonlocal offset
        words.append(dict(word=word,width=width,offset=offset))
        for copy in ('original','candidate'):code.append(f'assign {copy}_ff[{offset}+:{width}]='+expression(copy)+';')
        offset+=width
    for row in manifest['ff']:
        aux(row['word'],row['width'],lambda c,r=row:'{'+','.join(c+'.'+r['package']+'.'+f['name']for f in reversed(r['fields']))+'}')
    for row in manifest['files']:
        for a in range(16):aux(row['word']|a,4,lambda c,r=row,a=a:f"{c}.{r['package']}.mem[{a}]")
        aux(row['word']|16,10,lambda c,r=row:'{'+','.join(c+'.'+r['package']+'.'+f['name']for f in reversed(r['write_window']['fields']))+'}')
        base='original.'+row['package']+'.'
        code.append(f"assign small_write[{row['memory_index']}]={base}p13 && !{base}we_d && {base}ce_lat;")
    aux(manifest['phase']['word'],10,lambda c:'{'+','.join(c+'.'+n for n in reversed(manifest['phase']['fields']))+'}')
    for a in range(2):aux(manifest['storage_input']['word']|a,18,lambda c,a=a:f'{c}.storage_rx_pipeline.words[{a}]')
    aux(manifest['storage_input']['word']|2,1,lambda c:f'{c}.storage_rx_pipeline.clock_d')
    assert offset==aux_bits
    code.append('assign storage_data=original.storage_rx_raw;assign storage_clock=original.clk2_p_Aa;')
    bank_offsets=[];offset=0
    for i,bank in enumerate(manifest['banks']):
        width=bank['width'];bank_offsets.append(offset);first=bank['members'][0]['instance']
        code.append(f'assign original_raw[{offset}+:{width}]={{'+','.join('original.'+m['instance']+'.dout_r'for m in reversed(bank['members']))+'};')
        code.append(f'assign candidate_raw[{offset}+:{width}]=candidate.snapshot_bank_{i}.logical_q;')
        code.append(f'assign data_bits[{offset}+:{width}]={{'+','.join('original.'+m['instance']+'.p17'for m in reversed(bank['members']))+'};')
        code.append(f'assign addresses[{i*12}+:12]={{'+','.join(f'original.{first}.p{p}'for p in gen.im.ADDRESS)+'};')
        code.append(f'assign we_n[{i}]=original.{first}.p15;assign ce_n[{i}]=original.{first}.p16;')
        # Fixture-only probes bypass the state decoder independently. A
        # decoder with matching read/write aliases must still fail here.
        code.append(f'assign original_probe[{offset}+:{width}]={{'+','.join('original.'+m['instance']+'.mem[probe_row]'for m in reversed(bank['members']))+'};')
        code.append(f'assign candidate_probe[{offset}+:{width}]=candidate.snapshot_bank_{i}.mem[probe_row];')
        offset+=width
    assert offset==72
    code.append('integer row; initial begin')
    for i,r in enumerate(manifest['files']):
        for copy in ('original','candidate'):code.append(f"for(row=0;row<16;row=row+1){copy}.{r['package']}.mem[row]=(row*7+{i*3+1}) & 15;")
    for i,bank in enumerate(manifest['banks']):
        value=f"((row*32'd1103515245+32'd12345+32'd{i*7919}) ^ (row>>3) ^ (row>>7) ^ (row>>11))"
        for bit,m in enumerate(bank['members']):code.append(f"for(row=0;row<4096;row=row+1)original.{m['instance']}.mem[row]=({value}>>{bit}) & 1;")
        code.append(f"for(row=0;row<4096;row=row+1)candidate.snapshot_bank_{i}.mem[row]={value} & {(1<<bank['width'])-1};")
    code+=['end','endmodule']
    header='#pragma once\n'+f'constexpr unsigned INPUTS={ni},OUTPUTS={no},WORDS={len(words)},BANKS={nb},SMALLS={ns},INDEX_COUNT=1048576,STATE_BITS={manifest["logical_state_bits"]};\n'
    header+='struct StateWord {unsigned word,width,offset;};\nconstexpr StateWord state_words[]={\n'
    header+=',\n'.join('{'+f"{w['word']},{w['width']},{w['offset']}"+'}'for w in words)+'};\n'
    for name,values in [('BANK_WIDTHS',[r['width']for r in manifest['banks']]),('BANK_PACKAGES',[r['index']for r in manifest['banks']]),('BANK_OFFSETS',bank_offsets)]:
        header+='constexpr unsigned '+name+'[]={'+','.join(map(str,values))+'};\n'
    offset=0
    for p in inputs:header+=f"constexpr unsigned PIN_{p['name']}={offset};\n";offset+=p['width']
    return '\n'.join(code)+'\n',header,dict(ports=ports,state_words=words,bank_offsets=bank_offsets)


def run(root,output,mutations):
    output.mkdir(parents=True,exist_ok=False);board,manifest,clones=gen.generate(root)
    top,header,metadata=fixture(root,manifest);cells=[root/n for n in manifest['original_sources']]
    cases=[('candidate',board,clones,None)]
    if mutations:
        for name,target,before,after,diagnostic in [
            ('ungated_ff','board','.state_advance(state_guest_advance)',".state_advance(1'b1)",'board auxiliary state differs'),
            ('ungated_phase','board','else if(state_guest_advance)begin','else begin','board auxiliary state differs'),
            ('running_access','board','state_hold && state_frozen && state_request','state_request','running request accepted'),
            ('freeze_without_held','board','state_hold=state_freeze && state_guest_held;','state_hold=state_freeze;','unheld freeze accepted'),
            ('lost_read_pipeline','f10470_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'RAM bit write/readback differs'),
            ('ungated_ram','f10470_word_bank.sv','scan_active || (!freeze_active && guest_advance)',"1'b1",'board raw pipeline differs'),
            ('invalid_alias','board',"state_package<7'd92 && state_row==0;","state_package<7'd92 && state_row<2;",'board auxiliary state differs'),
            ('response_truncation','board','reg [4:0] state_response_memory_index;','reg [3:0] state_response_memory_index;','RAM/pipeline state export differs'),
            ('row_alias','f10470_word_bank.sv','word_index[11:0]:address',"{1'b0,word_index[10:0]}:address",'RAM/pipeline state export differs'),
            ('lost_small_window','cell_F10145A_snapshot.sv','{ce_lat,d_lat,a_lat,we_d} <= state_data;',"{ce_lat,d_lat,a_lat,we_d} <= 10'b0;",'aux bit write/readback differs'),
            ('lost_storage_word','board','words[state_row[0]]<=state_data;',"words[state_row[0]]<=18'b0;",'aux bit write/readback differs'),
            ('lost_storage_history','board','clock_d<=state_data[0];',"clock_d<=1'b0;",('aux bit write/readback differs','board auxiliary state differs')),
        ]:
            original=board if target=='board'else clones[target];assert before in original,(name,before)
            changed=original.replace(before,after,1);copies=clones.copy()
            if target!='board':copies[target]=changed
            cases.append((name,changed if target=='board'else board,copies,diagnostic))
        cases.append(('unsupported_storage_delay',board,clones,'MemD snapshot requires STORAGE_INPUT_DELAY=2'))
    results=[]
    for name,text,copies,diagnostic in cases:
        case=output/name;case.mkdir();(case/'board.sv').write_text(text)
        case_top=top
        if name=='unsupported_storage_delay':
            before='#(.SYSPER(4),.STORAGE_INPUT_DELAY(2)) candidate'
            assert top.count(before)==1
            case_top=top.replace(before,'#(.SYSPER(4),.STORAGE_INPUT_DELAY(1)) candidate')
        (case/'fixture.sv').write_text(case_top);(case/'ports.h').write_text(header)
        for n,s in copies.items():(case/n).write_text(s)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','memd_full_fixture','-Wno-fatal',
            '-DDORADO_F10145_LOGIC','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),
            str(case/'board.sv'),*[str(case/n)for n in copies],str(root/'verilog/generated/MemD-Rev-Ca.v'),*map(str,cells),str(HERE/'test_memd_full.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmemd_full_fixture')],capture_output=True,text=True,timeout=360)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        if diagnostic:
            expected=(diagnostic,)if isinstance(diagnostic,str)else diagnostic
            assert result.returncode and any(s in text for s in expected),(name,text)
        else:assert result.returncode==0 and 'FULL MemD RESTORE PASS'in text,text
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (output/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    paths=[Path(__file__).resolve(),HERE/'test_memd_full.cpp',HERE/'generate_memd_full.py',HERE/'audit_memd_im.py',
        HERE/'audit_ifu_im.py',HERE/'audit_contb_im.py',HERE/'generate_contb_full.py',HERE/'generate_snapshot_ff.py',
        HERE/'generate_processor_ff.py',HERE/'generate_f10145.py',HERE/'f10470_word_bank.sv',root/'verilog/generated/MemD-Rev-Ca.v',*cells]
    (output/'results.json').write_text(json.dumps(dict(scope=manifest['scope'],cases=results,
        source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args()
    run(a.source_root.resolve(),a.output.resolve(),a.mutations)
