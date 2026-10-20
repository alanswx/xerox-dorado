#!/usr/bin/env python3
"""Actual frozen MSA outputs/state/raw-memory traffic and complete serial restore."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_msa_board as gen
from generate_serial_boards import raw_receipt

HERE=Path(__file__).resolve().parent


def physical_fields():
    root=gen.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-ff'
    data=json.loads(raw_receipt(root,'native-classification-v2/inventory.json'))
    prefix='dorado_native_machine.m.b_msa.'
    rows=[dict(path=r['path'][len(prefix):],width=r['width'])for r in data['candidates']
          if r['category']=='sequential'and r['path'].startswith(prefix)]
    assert len(rows)==458 and sum(r['width']for r in rows)==2690
    return rows


def ports():
    source=gen.ff.masked_comments(gen.original(gen.SOURCE).decode())
    header=source[source.index('    input  wire sys_clk,'):source.index('\n);')]
    rows=[]
    for line in header.splitlines():
        line=line.strip().rstrip(',');m=re.fullmatch(r'(input|output)\s+wire\s*(?:\[(\d+):(\d+)\]\s*)?([\w, ]+)',line)
        if not m:
            if line:raise ValueError('unreviewed MSA port declaration: '+line)
            continue
        direction,hi,lo,names=m.groups();width=1 if hi is None else abs(int(hi)-int(lo))+1
        for name in names.split(','):rows.append(dict(name=name.strip(),direction=direction,width=width))
    assert len(rows)==len({r['name']for r in rows})
    return rows


def fixture(manifest):
    regular=[p for p in ports()if p['name']!='sys_clk'and not p['name'].startswith('storage_')]
    inputs=[p for p in regular if p['direction']=='input'];outputs=[p for p in regular if p['direction']=='output']
    ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs);physical=physical_fields()
    canonical={};offset=0
    for row in physical:canonical[row['path']]=(offset,row['width']);offset+=row['width']
    mapping=[None]*2690
    for row in manifest['layout']:
        start,width=canonical[row['path']];assert width==row['width']
        for b in range(width):assert mapping[row['lsb']+b]is None;mapping[row['lsb']+b]=start+b
    assert set(mapping)==set(range(2690))
    code=f'''module msa_board_fixture(
 input wire clk,ref_clk,advance,freeze,held,global_hold,global_abort,system_reset,request,write,reset,
 input wire [23:0] state_index,input wire [17:0] state_data,input wire [{ni-1}:0] stimulus,
 input wire [15:0] req_ready,rsp_valid,input wire [15:0] rsp_data[16],
 output wire [15:0] req_valid,req_write,rsp_ready,ref_req_valid,ref_req_write,ref_rsp_ready,
 output wire [15:0] req_addr[16],req_data[16],ref_req_addr[16],ref_req_data[16],
 output wire storage_ready,ref_storage_ready,ready,frozen,done,error,hold_required,fault,drained,
 output wire [15:0] helper_frozen,output wire busy,scan_step,
 output wire [17:0] result,output wire [{no-1}:0] dut_outputs,reference_outputs,
 output wire [2689:0] dut_state,reference_state);
 wire reference_machine_clk=ref_clk && advance;
 wire candidate_machine_clk=clk && advance;
'''
    for side,module,host,machine in [('reference','msa_ddram','ref_clk','reference_machine_clk'),('dut',gen.MODULE,'clk','candidate_machine_clk')]:
        con=[f'.sys_clk({machine})',f'.storage_clk({host})','.storage_reset(reset)',f'.storage_ready({"ref_storage_ready"if side=="reference"else"storage_ready"})']
        for p in outputs:code+=f'wire {side}_{p["name"]};\n'
        offset=0;output_names={p['name']for p in outputs}
        for p in inputs:
            value=f'stimulus[{offset}]'if p['width']==1 else f'stimulus[{offset}+:{p["width"]}]'
            if p['name']+'__drv'in output_names:value+=' | '+side+'_'+p['name']+'__drv'
            con.append('.'+p['name']+'('+value+')');offset+=p['width']
        con+=['.'+p['name']+'('+side+'_'+p['name']+')'for p in outputs]
        for name in ('req_addr','req_data','rsp_data'):
            code+=f'wire [255:0] {side}_{name}_bus;\n'
            for i in range(16):
                target=(('ref_'+name)if side=='reference'else name)if name!='rsp_data'else'rsp_data'
                code+=f'assign '+(f'{side}_{name}_bus[{16*i}+:16]={target}[{i}];\n'if name=='rsp_data'else f'{target}[{i}]={side}_{name}_bus[{16*i}+:16];\n')
            con.append(f'.storage_{name}({side}_{name}_bus)')
        con+=['.storage_req_ready(req_ready)','.storage_rsp_valid(rsp_valid)']
        con +=[f'.storage_{n}({"ref_"if side=="reference"else""}{n})'for n in('req_valid','req_write','rsp_ready')]
        if side=='dut':
            con+=['.state_guest_advance(advance)','.state_freeze(freeze)','.state_guest_held(held)',
                  '.state_global_hold(global_hold)','.state_global_abort(global_abort)','.state_system_reset(system_reset)',
                  '.state_request(request)','.state_write(write)','.state_index(state_index)','.state_data(state_data)',
                  '.state_scan_hold_required(hold_required)','.state_scan_fault(fault)','.state_memory_drained(drained)']
            con+=['.state_'+n+'('+n+')'for n in('ready','frozen','done','error','result')]
        code+=module+' #(.SYSPER(4)) '+side+'(\n'+',\n'.join(con)+'\n);\n'
        code+=f'assign {side}_outputs={{'+','.join(side+'_'+p['name']for p in reversed(outputs))+'};\n'
        offset=0
        for row in physical:
            code+=f'assign {side}_state[{offset}+:{row["width"]}]={side}.{row["path"]};\n';offset+=row['width']
    code+='assign helper_frozen=dut.state_storage_frozen;assign busy=dut.scan_busy;assign scan_step=dut.scan_step;\nendmodule\n'
    header=f'constexpr unsigned STIM_BITS={ni},STATE_BITS=2690,STATE_WORDS=85,SCAN_WORDS=150;\n'
    header+='constexpr unsigned canonical_bit[2690]={'+','.join(map(str,mapping))+'};\n'
    return code,header,dict(ports=regular,inputs=ni,outputs=no,physical=physical,canonical_bit=mapping)


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    board,manifest,clones=gen.generate();top,header,metadata=fixture(manifest)
    for p in(Path(__file__).resolve(),HERE/'generate_msa_board.py',HERE/'test_msa_board.cpp',HERE/'generate_storage_group.py',HERE/'generate_msa_ff.py'):
        (inputs/p.name).write_bytes(p.read_bytes())
    (inputs/'msa_ddram.v').write_bytes(gen.original(gen.SOURCE))
    (inputs/'dorado_storage_group.sv').write_bytes(gen.storage.original_source())
    originals=gen.original_cells()
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    cases=[('candidate',None,None,None,None)]
    if mutations:cases +=[
        ('lost_global_hold','msa_board.sv','wire state_hold=state_global_hold ||','wire state_hold=', 'held raw request/response leak'),
        ('lost_scan_abort','msa_board.sv','.freeze(state_freeze && !state_global_abort)','.freeze(state_freeze)','abort did not quarantine'),
        ('broken_ff_frontend_link','msa_board.sv','ff_scan_next={state_storage_serial[0],ff_scan_bits[865:1]}',"ff_scan_next={1'b0,ff_scan_bits[865:1]}",'read rotation changed physical state'),
        ('broken_frontend_link','msa_board.sv','.state_serial_in(state_storage_serial[1])',".state_serial_in(1'b0)",'read rotation changed physical state'),
        ('lost_ff_hold','cell_SN74166.sv','if(state_hold)begin',"if(1'b0)begin",'held physical state differs'),
        ('lost_frontend_hold','storage_group.sv','if(state_hold)begin',"if(1'b0)begin",'held physical state differs'),
        ('lost_last_word','ff_scan_controller.sv','offset < 18','offset < 7','read word differs'),
    ]
    results=[]
    for label,target,before,after,why in cases:
        case=out/label;case.mkdir();assets={'msa_board.sv':board,**clones}
        if target:assert assets[target].count(before)==1,(label,before);assets[target]=assets[target].replace(before,after,1)
        for name,source in assets.items():(case/name).write_text(source)
        (case/'fixture.sv').write_text(top);(case/'parameters.h').write_text(header)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','msa_board_fixture','-Wno-fatal','--assert',
                 '--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),*[str(case/n)for n in assets],
                 str(inputs/'msa_ddram.v'),str(inputs/'dorado_storage_group.sv'),*[str(inputs/n)for n in originals],str(inputs/'test_msa_board.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vmsa_board_fixture')],capture_output=True,text=True,timeout=180)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log);print(label+': '+log.strip(),flush=True)
        if why:assert result.returncode and why in log,(label,log)
        else:assert result.returncode==0 and 'MSA BOARD PASS'in log,log
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    for name,value in [('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,
        physical_bits=2690,executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='One complete MSA family at native SYSPER4; native integration/global raw/ARM barriers, external RAM save, transport budget, core area/timing and hardware snapshot still pending.'))]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.mutations)
