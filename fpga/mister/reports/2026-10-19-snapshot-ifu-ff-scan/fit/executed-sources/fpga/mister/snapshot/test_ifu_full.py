#!/usr/bin/env python3
"""Complete IFU ordinary/hold/scan/restore tests against an untouched board."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

import generate_ifu_full as gen

HERE=Path(__file__).resolve().parent


def fixture(source_root,manifest):
    source=source_root/'verilog/generated/IFU-Rev-Ch.v';ports=[]
    for m in re.finditer(r'^\s+(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?(\w+)(?=[,\n])',source.read_text(),re.M):
        direction,msb,lsb,name=m.groups()
        if name!='sys_clk':ports.append(dict(direction=direction,name=name,width=1 if msb is None else abs(int(msb)-int(lsb))+1))
    inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output']
    ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs);nb=len(manifest['banks'])
    ff_rows=manifest['ff']+[dict(word=manifest['phase']['word'],width=4,phase=manifest['phase']['fields'])]
    ff_bits=sum(r['width']for r in ff_rows)
    code=[f'''module ifu_full_fixture(
 input wire host_clk,run,run_ref,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [17:0] state_index,state_data,
 output wire frozen,done,error,output wire [17:0] result,
 output wire [{no-1}:0] original_outputs,candidate_outputs,
 output wire [{ff_bits-1}:0] original_ff,candidate_ff,
 output wire [26:0] original_raw,candidate_raw,data_bits,
 output wire [{nb*10-1}:0] addresses,output wire [{nb-1}:0] we_n,
 output wire advance,ref_advance);
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
        code +=[f' {module} #(.SYSPER(4)) {copy}(',','.join(connections),');']
        code.append('assign '+copy+'_outputs={'+','.join(copy+'_'+p['name']for p in reversed(outputs))+'};')
    offsets=[];offset=0
    for row in ff_rows:
        offsets.append(offset)
        for copy in ('original','candidate'):
            fields=[copy+'.'+n for n in reversed(row['phase'])]if'phase'in row else [copy+'.'+row['package']+'.'+f['name']for f in reversed(row['fields'])]
            code.append(f"assign {copy}_ff[{offset}+:{row['width']}]={{"+','.join(fields)+'};')
        offset+=row['width']
    bank_offsets=[];offset=0
    for i,bank in enumerate(manifest['banks']):
        width=bank['width'];bank_offsets.append(offset)
        code.append(f'assign original_raw[{offset}+:{width}]={{'+','.join('original.u_'+m['package']+'.dout_r'for m in reversed(bank['members']))+'};')
        code.append(f'assign candidate_raw[{offset}+:{width}]=candidate.snapshot_bank_{i}.logical_q;')
        code.append(f'assign data_bits[{offset}+:{width}]={{'+','.join('original.u_'+m['package']+'.p15'for m in reversed(bank['members']))+'};')
        first=bank['members'][0]['package']
        code.append(f'assign addresses[{i*10}+:10]={{'+','.join(f'original.u_{first}.p{p}'for p in gen.im.logic.ADDRESS)+'};')
        code.append(f'assign we_n[{i}]=original.u_{first}.p13;');offset+=width
    assert offset==27
    code.append('integer row; initial begin')
    for i,bank in enumerate(manifest['banks']):
        value=f"(row*32'd1103515245+32'd12345+32'd{i*7919})"
        for bit,m in enumerate(bank['members']):
            code.append(f"for(row=0;row<1024;row=row+1)original.u_{m['package']}.mem[row]=({value}>>{bit}) & 1;")
        code.append(f"for(row=0;row<1024;row=row+1)candidate.snapshot_bank_{i}.mem[row]={value} & {(1<<bank['width'])-1};")
    code+=['end','endmodule']
    header='#pragma once\n'+f'constexpr unsigned INPUTS={ni},OUTPUTS={no},FF_COUNT={len(ff_rows)},BANKS={nb},INDEX_COUNT=262144,STATE_BITS={manifest["logical_state_bits"]};\n'
    for name,values in [('FF_WIDTHS',[r['width']for r in ff_rows]),('FF_WORDS',[r['word']for r in ff_rows]),('FF_OFFSETS',offsets),
                        ('BANK_WIDTHS',[r['width']for r in manifest['banks']]),('BANK_PACKAGES',[r['index']for r in manifest['banks']]),('BANK_OFFSETS',bank_offsets)]:
        header+='constexpr unsigned '+name+'[]={'+','.join(map(str,values))+'};\n'
    return '\n'.join(code)+'\n',header,dict(ports=ports,ff_rows=ff_rows,ff_offsets=offsets,bank_offsets=bank_offsets)


def run(source_root,output,mutations):
    output.mkdir(parents=True,exist_ok=False);board,manifest,clones=gen.generate(source_root)
    top,header,metadata=fixture(source_root,manifest)
    cells=[source_root/name for name in manifest['original_sources']]
    cases=[('candidate',board,clones,None)]
    if mutations:
        for name,target,before,after,diagnostic in [
            ('ungated_ff','board','.state_advance(state_guest_advance)',".state_advance(1'b1)",'board FF state differs'),
            ('ungated_phase','board','else if(state_guest_advance)begin','else begin','board FF state differs'),
            ('running_access','board','state_hold && state_frozen && state_request','state_request','running request accepted'),
            ('lost_jk_history','cell_MC10135_snapshot.v','{ck_d,jk_pre,qb,qa}<=state_data[6:0];','{jk_pre,qb,qa}<=state_data[5:0];','FF bit write/readback differs'),
            ('lost_read_pipeline','f10415_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'RAM bit write/readback differs'),
            ('ungated_ram','f10415_word_bank.sv','scan_active || (!freeze_active && guest_advance)',"1'b1",'board raw pipeline differs'),
            ('invalid_alias','board',"state_package<7'd94 && state_row==0;","state_package<7'd94 && state_row<2;",'board FF state differs'),
        ]:
            text=board if target=='board'else clones[target];assert before in text,(name,before)
            changed=text.replace(before,after,1);copies=clones.copy()
            if target!='board':copies[target]=changed
            cases.append((name,changed if target=='board'else board,copies,diagnostic))
    results=[]
    for name,text,copies,diagnostic in cases:
        case=output/name;case.mkdir();(case/'board.sv').write_text(text);(case/'fixture.sv').write_text(top);(case/'ports.h').write_text(header)
        for n,s in copies.items():(case/n).write_text(s)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','ifu_full_fixture','-Wno-fatal',
            '-DDORADO_F10415_M10K','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),
            str(case/'board.sv'),*[str(case/n)for n in copies],str(source_root/'verilog/generated/IFU-Rev-Ch.v'),
            *map(str,cells),str(HERE/'test_ifu_full.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vifu_full_fixture')],capture_output=True,text=True,timeout=240)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        if diagnostic:assert result.returncode and diagnostic in text,(name,text)
        else:assert result.returncode==0 and 'FULL IFU RESTORE PASS'in text,text
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (output/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    paths=[Path(__file__).resolve(),HERE/'test_ifu_full.cpp',HERE/'generate_ifu_full.py',HERE/'audit_ifu_im.py',
           HERE/'audit_contb_im.py',HERE/'generate_contb_full.py',HERE/'generate_snapshot_ff.py',HERE/'generate_jk.py',
           HERE/'f10415_word_bank.sv',source_root/'verilog/generated/IFU-Rev-Ch.v',*cells,gen.ROOT/gen.PROM]
    (output/'results.json').write_text(json.dumps(dict(scope=manifest['scope'],cases=results,
        source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);parser.add_argument('--mutations',action='store_true')
    args=parser.parse_args();run(args.source_root.resolve(),args.output.resolve(),args.mutations)
