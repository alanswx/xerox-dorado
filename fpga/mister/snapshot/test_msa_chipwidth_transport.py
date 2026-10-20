#!/usr/bin/env python3
"""Chip-width schema over SPI through all eight actual complete MSA adapters.

The inherited eleven endpoint/service stubs and stopped guest are explicit;
this qualifies routing, bounds and ownership, not native machine pause.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_chipwidth as msa
import serial_state_format_msa_chipwidth as fmt
import test_msa_transport as base

HERE=Path(__file__).resolve().parent


def group_source():
    old=(HERE/'msa_state_group.sv').read_text()
    source=base.once(old,'module dorado_msa_state_group(','module dorado_msa_state_group_chipwidth(')
    source=base.once(source,"index[7:0]<8'd150","index[7:0]<8'd86")
    source=base.once(source,'word0..149.','word0..85.')
    assert source.replace('dorado_msa_state_group_chipwidth','dorado_msa_state_group',1).replace("index[7:0]<8'd86","index[7:0]<8'd150",1).replace('word0..85.','word0..149.',1)==old
    return source


def fabric_source():
    return base.once((HERE/'board_state_fabric_msa.sv').read_text(),"index[7:0]<8'd150","index[7:0]<8'd86")


def fixture(catalog):
    old_catalog=fmt.prior.build_catalog(msa.base.ROOT/'fpga/mister/reports');s,oldvectors=base.fixture(old_catalog)
    s=base.once(s,f"SCHEMA=64'h{old_catalog['schema_id']}",f"SCHEMA=64'h{catalog['schema_id']}")
    s=base.once(s,'dorado_msa_state_group msa_group','dorado_msa_state_group_chipwidth msa_group')
    s=base.once(s,'wire [2689:0] msa_physical','wire [1537:0] msa_physical')
    s=base.once(s,'reg [21519:0] msa_saved','reg [12303:0] msa_saved')
    s=s.replace("24'h000795","24'h000755")
    s=base.once(s,'(i<<8)|150','(i<<8)|86')
    anchor='   if(total_calls()!=before_calls)$fatal(1,"MSA hole reached child");'
    s=base.once(s,anchor,'   send_request(0,11,(i<<8)|91,0);expect_result(2,0);\n   send_request(0,11,(i<<8)|92,0);expect_result(2,0);\n   send_request(0,11,(i<<8)|149,0);expect_result(2,0);\n   send_request(0,11,(i<<8)|150,0);expect_result(2,0);\n'+anchor)
    anchor='  // All members, including an unselected one, participate in the boundary.'
    oldtag=old_catalog['schema_id']
    s=base.once(s,anchor,f'''  before_errors=control_errors;
  prefix(16'h77,2,64'h{oldtag});word(0);finish_packet();idle(3);
  if(!hold||control_errors!=before_errors+1)$fatal(1,"old MSA catalog changed hold");
'''+f'''  before_errors=control_errors;
  prefix(16'h77,2,64'h56e997e5c1fb54d0);word(0);finish_packet();idle(3);
  if(!hold||control_errors!=before_errors+1)$fatal(1,"old compact catalog changed hold");
'''+anchor)
    client=fmt.SerialStateFormat(catalog);vectors=[]
    def packet(words):
        vectors.extend(f"word(16'h{w:04x});"for w in words);vectors.append('finish_packet();')
    packet(client.control_words(True));vectors.append('idle(4);if(!port_ready)$fatal(1,"formatted MSA hold unavailable");')
    for write in (True,False):
        for u in range(8):
            for row,width in ((0,18),(85,8)):
                value=(0x259a1+u*0x137b+row)&((1<<width)-1)
                packet(client.request_words(11,(u<<8)|row,write,value if write else 0))
                vectors.append('expect_result(0,0);'if write else f"expect_result(0,18'h{value:05x});")
    packet(client.control_words(False));vectors.append('idle(4);if(hold)$fatal(1,"formatted MSA release failed");')
    vectors='\n'.join(vectors)+'\n';s=base.once(s,oldvectors,vectors)
    s=base.once(s,'MSA TRANSPORT PASS','CHIPWIDTH MSA TRANSPORT PASS')
    return s,vectors


def format_tests(catalog):
    client=fmt.SerialStateFormat(catalog);board=catalog['boards'][11];values=[];expected=0
    for index,width,offset in fmt.old.iter_words(board):
        value=(index*0x19e377+0x514a9)&((1<<width)-1);values.append((index,value));expected|=value<<offset
    image=fmt.old.encode_board(board,values)
    assert len(image)==1538 and image==expected.to_bytes(1538,'little')
    assert list(fmt.old.decode_board(board,image))==values
    for unit in range(8):
        for index in (86,91,92,149,150,255):base.reject(lambda:client.request_words(11,(unit<<8)|index))
        base.reject(lambda:client.request_words(11,(unit<<8)|85,True,256))
    base.reject(lambda:client.request_words(11,2048));base.reject(lambda:client.request_words(12,0))
    controls=[]
    for case in ('alias','stride','member_order','missing_member','total','budget','missing_slot','padding','old_geometry','abi'):
        bad=copy.deepcopy(catalog);b=bad['boards'][11]
        if case=='alias':b['runs'][2]['word']=0
        elif case=='stride':b['member_stride']=86
        elif case=='member_order':b['members'][0],b['members'][1]=b['members'][1],b['members'][0]
        elif case=='missing_member':b['members'].pop()
        elif case=='total':bad['board_bits']+=1
        elif case=='budget':bad['response_limit']=2048
        elif case=='missing_slot':bad['boards'].pop()
        elif case=='padding':b['runs'][1]['width']=18
        elif case=='old_geometry':b['member_bits']=2690
        else:bad['abi']=1
        payload={k:v for k,v in bad.items()if k not in ('catalog_sha256','schema_id')};digest=hashlib.sha256(fmt.old.canonical(payload)).hexdigest()
        bad.update(catalog_sha256=digest,schema_id=digest[:16]);base.reject(lambda:fmt.SerialStateFormat(bad));controls.append(dict(case=case,rejected=True))
    changed=copy.deepcopy(catalog);isolated=fmt.SerialStateFormat(changed);before=isolated.request_words(11,0x755)
    changed['boards'][11]['runs'][0]['word']=99;assert isolated.request_words(11,0x755)==before
    tag=int(catalog['schema_id'],16)
    status=[0xd0b0,2,0x797,12,0xfff,0xabcd,0x1234,0x3456,3,0,0x55aa,0xffff,*[(tag>>(16*i))&65535 for i in range(4)],0x1f,4096]
    assert client.parse_status(status)['frozen']==0xfff
    for field in (0,1,3,12,13,14,15,17):
        bad=list(status);bad[field]^=1;base.reject(lambda:client.parse_status(bad))
    for field,value in ((2,0x8000),(4,0x1000),(8,4),(9,7),(16,0x20)):
        bad=list(status);bad[field]=value;base.reject(lambda:client.parse_status(bad))
    base.reject(lambda:client.parse_status(status[:-1]))
    return dict(status='PASS',msa_bits=12304,msa_bytes=1538,words=688,independent_dense_integer_oracle=True,
                status_caps_and_padding=True,failure_controls=controls)


def run(out,ethernet,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    catalog=fmt.build_catalog(msa.base.ROOT/'fpga/mister/reports');top,vectors=fixture(catalog)
    (out/'catalog.json').write_text(json.dumps(catalog,indent=2)+'\n');(out/'formatted-vectors.svh').write_text(vectors)
    (out/'format-results.json').write_text(json.dumps(format_tests(catalog),indent=2)+'\n')
    board,manifest,helpers=msa.generate();assets={'msa_board.sv':board,**helpers,'msa_state_group_chipwidth.sv':group_source(),
        'board_state_fabric_msa.sv':fabric_source(),'board_state_uio_msa.sv':(HERE/'board_state_uio_msa.sv').read_text()}
    originals=msa.base.original_cells();originals.update({'msa_ddram.v':msa.base.original(msa.base.SOURCE),
        'dorado_storage_group.sv':msa.compact.original.original_source(),ethernet.name:ethernet.read_bytes()})
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    for p in (Path(__file__),Path(fmt.__file__),Path(base.__file__),HERE/'test_board_state_serial.sv',HERE/'msa_state_group.sv'):
        (inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None)]
    if mutations:cases += [
        ('short_timeout','board_state_fabric_msa.sv','RESPONSE_LIMIT=4096','RESPONSE_LIMIT=1000','result differs'),
        ('wrong_member','msa_state_group_chipwidth.sv','unit_request=(admit && valid_index)?route',"unit_request=(admit && valid_index)?8'b1",'result differs'),
        ('unstripped_index','msa_state_group_chipwidth.sv',"unit_index={16'b0,index[7:0]}",'unit_index=index','result differs'),
        ('live_owner','msa_state_group_chipwidth.sv','unit_done[owner]','unit_done[index[10:8]]','result differs'),
        ('old_bounds','board_state_fabric_msa.sv',"index[7:0]<8'd86","index[7:0]<8'd150",'result differs'),
        ('group_boundary','msa_state_group_chipwidth.sv',"assign frozen=(unit_frozen===8'hff);","assign frozen=1'b1;",'missing MSA boundary accepted'),
        ('hold_unknown','msa_state_group_chipwidth.sv',"children_holding=(unit_hold_required!==8'b0)",'children_holding=|unit_hold_required','unknown MSA hold did not fail closed'),
        ('write_payload','msa_state_group_chipwidth.sv','assign unit_data=data;',"assign unit_data=18'b0;",'result differs'),
        ('old_schema','board_state_uio_msa.sv',"6:if(din!=SCHEMA_ID[63:48])valid_packet<=0;",'6:begin end','bad schema accepted'),
        ('status_count','board_state_uio_msa.sv',"3:dout<=16'd12;","3:dout<=16'd11;",'serial capability/schema differs'),
    ]
    results=[]
    for label,target,before,after,why in cases:
        case=out/label;case.mkdir();compiled=[]
        for name,body in assets.items():
            if name==target:assert body.count(before)==1,(label,before);body=body.replace(before,after,1)
            if label=='wrong_member'and name==target:body=base.once(body,'owner<=index[10:8];','owner<=0;')
            p=case/name;p.write_text(body);compiled.append(p)
        (case/'fixture.sv').write_text(top)
        command=['iverilog','-g2012','-s','msa_transport_fixture','-o',str(case/'test.vvp'),str(case/'fixture.sv'),*map(str,compiled),*[str(inputs/n)for n in originals]]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip().splitlines()[0],flush=True)
        if why:assert result.returncode and why in text,(label,text)
        else:assert result.returncode==0 and 'CHIPWIDTH MSA TRANSPORT PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,physical_msa_bits=12304,
        schema_id=catalog['schema_id'],catalog_sha256=catalog['catalog_sha256'],formatted_packets=34,
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Actual eight compact adapters, eleven endpoint and five service stubs; guest stopped. No native pause/FPGA snapshot qualification.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve(),a.mutations)
