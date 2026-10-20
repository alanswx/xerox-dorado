#!/usr/bin/env python3
"""Four-state ABI2 transport, original contract and eight actual indexed MSAs.

Guest advances are stopped in this transport fixture. Foreign service barriers
and eleven other endpoints remain stubs; this is not native machine pause or
an FPGA snapshot. MSA normal behavior/full physical restore is qualified by
the prerequisite complete-board receipt, not by these SPI routing exercises.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import generate_msa_board as msa
import serial_state_format_msa as fmt

HERE=Path(__file__).resolve().parent


def once(s,a,b):
    assert s.count(a)==1,(a,s.count(a));return s.replace(a,b,1)


def hoist_msa_declarations(source):
    # Icarus with default_nettype none requires declarations before use.
    # Move only the already-qualified administrative fabric; leave all board
    # pins, helper bodies, equations and state addresses unchanged.
    marker='\n  wire [17:0] state_ff_result[0:77];'
    assert source.count(marker)==1
    start=source.index(marker);end=source.index('endmodule',start);old_fabric=source[start:end]
    vector='  wire [15:0] state_storage_serial,state_storage_frozen,state_storage_drained;\n'
    fabric=once(old_fabric,vector,'')
    fabric=once(fabric,'  wire [17:0] state_ff_result[0:77];\n',vector+'  wire [17:0] state_ff_result[0:77];\n')
    bare=source[:start]+source[end:];at=bare.index('\n);')+3
    normalized=bare[:at]+fabric+bare[at:]
    inverse=normalized[:at]+normalized[at+len(fabric):]
    pos=inverse.index('endmodule');inverse=inverse[:pos]+old_fabric+inverse[pos:]
    assert inverse==source
    return normalized,dict(scope='Exact inverse of declaration-order-only fabric relocation; no normal logic/pin/state-map edits',
                           original_sha256=hashlib.sha256(source.encode()).hexdigest(),
                           normalized_sha256=hashlib.sha256(normalized.encode()).hexdigest(),byte_identity_after_inverse=True)


def fixture(catalog):
    s=(HERE/'test_board_state_serial.sv').read_text();tag=int(catalog['schema_id'],16)
    s=once(s,'module board_state_serial_fixture;','module msa_transport_fixture;')
    s=once(s,"SCHEMA=64'h79c5a16e42d38b07",f"SCHEMA=64'h{tag:016x}")
    s=s.replace('dorado_board_state_uio_serial','dorado_board_state_uio_msa').replace('dorado_board_state_fabric_serial','dorado_board_state_fabric_msa')
    s=once(s,'reg [10:0] suppress_frozen','reg [11:0] suppress_frozen')
    s=once(s,'reg [9:0] stub_done=0,stub_error=0;reg [10:0] board_frozen=0;',
             'reg [9:0] stub_done=0,stub_error=0;reg [10:0] main_frozen=0;wire [11:0] board_frozen={msa_frozen,main_frozen};')
    s=once(s,'wire [10:0] board_done={long_done,stub_done},board_error={long_error,stub_error};',
             'wire [11:0] board_done={msa_done,long_done,stub_done},board_error={msa_error,long_error,stub_error};')
    s=once(s,"wire [10:0] board_holds=inject_hold|{long_hold,10'b0};","wire [11:0] board_holds=inject_hold|{msa_hold,long_hold,10'b0};")
    s=once(s,"wire [10:0] board_faults=inject_fault|{long_fault,10'b0};","wire [11:0] board_faults=inject_fault|{msa_fault,long_fault,10'b0};")
    s=once(s,'wire [197:0] board_result={long_result,stub_result};','wire [215:0] board_result={msa_result,long_result,stub_result};')
    s=once(s,'wire [10:0] board_request;','wire [11:0] board_request;')
    s=once(s,"board_frozen<={11{hold&&local_held}}&~suppress_frozen;","main_frozen<={11{hold&&local_held}}&~suppress_frozen[10:0];")
    s=once(s,"board_frozen!==11'h7ff","board_frozen!==12'hfff")
    s=once(s,'delayed<=0;board_frozen<=0;','delayed<=0;main_frozen<=0;')
    s=once(s,'if(waited>2055)','if(waited>4105)')
    s=once(s,'status_words[3]!==11','status_words[3]!==12')
    s=once(s,'status_words[17]!==2048','status_words[17]!==4096')
    s=once(s,'for(i=11;i<16;i=i+1)','for(i=12;i<16;i=i+1)')
    s=once(s,'ticks-before_ticks<2048','ticks-before_ticks<4096')
    s=once(s,'total_calls=0;for(t=0;t<11;t=t+1)total_calls=total_calls+calls[t];',
             'total_calls=0;for(t=0;t<11;t=t+1)total_calls=total_calls+calls[t];for(t=0;t<8;t=t+1)total_calls=total_calls+msa_calls[t];')
    s=once(s,'   system_reset=1;idle(2);','   msa_suppress=0;msa_inject_hold=0;msa_inject_fault=0;system_reset=1;idle(2);')
    # These are complete, already-qualified MSA boards, not scan-chain models.
    code='''
 wire msa_frozen,msa_done,msa_error,msa_hold,msa_fault;wire [17:0] msa_result;
 wire [7:0] msa_request,msa_unit_frozen,msa_unit_done,msa_unit_error,msa_unit_hold,msa_unit_fault;
 wire [143:0] msa_unit_result;wire [23:0] msa_unit_index;wire msa_unit_write;wire [17:0] msa_unit_data;
 reg [7:0] msa_suppress=0,msa_inject_hold=0,msa_inject_fault=0;
 integer msa_calls[0:7];wire [2689:0] msa_physical[0:7];
 dorado_msa_state_group msa_group(.clk(clk),.system_reset(system_reset),.transport_reset(transport_reset),.scan_abort(scan_abort),
 .request(board_request[11]),.write(wr),.index(index),.data(data),
 .unit_frozen(msa_unit_frozen&~msa_suppress),.unit_done(msa_unit_done),.unit_error(msa_unit_error),
 .unit_hold_required(msa_unit_hold|msa_inject_hold),.unit_fault(msa_unit_fault|msa_inject_fault),.unit_result(msa_unit_result),
 .frozen(msa_frozen),.hold_required(msa_hold),.fault(msa_fault),.done(msa_done),.error(msa_error),.result(msa_result),
 .unit_request(msa_request),.unit_index(msa_unit_index),.unit_write(msa_unit_write),.unit_data(msa_unit_data));
 genvar u;
 generate for(u=0;u<8;u=u+1)begin:members
 always @(posedge clk)begin
  if(system_reset)msa_calls[u]<=0;
  else if(msa_request[u])msa_calls[u]<=msa_calls[u]+1;
 end
'''
    from test_msa_board import ports
    con=['.sys_clk(1\'b0)','.storage_clk(clk)',".storage_reset(1'b0)",'.storage_ready()',
         ".storage_req_ready(16'b0)",".storage_rsp_valid(16'b0)",".storage_rsp_data(256'b0)"]
    con+=['.'+p['name']+'('+(''if p['direction']=='output'else str(p['width'])+"'b0")+')'
          for p in ports()if p['name']!='sys_clk'and not p['name'].startswith('storage_')]
    con+=['.'+name+'()'for name in ('storage_req_valid','storage_req_write','storage_req_addr','storage_req_data','storage_rsp_ready')]
    con+=['.state_guest_advance(1\'b0)','.state_freeze(hold)','.state_guest_held(local_held)',
          '.state_global_hold(hold)','.state_global_abort(scan_abort)','.state_system_reset(system_reset)',
          '.state_request(msa_request[u])','.state_write(msa_unit_write)','.state_index(msa_unit_index)',
          '.state_data(msa_unit_data)','.state_ready()','.state_frozen(msa_unit_frozen[u])','.state_done(msa_unit_done[u])',
          '.state_error(msa_unit_error[u])','.state_scan_hold_required(msa_unit_hold[u])','.state_scan_fault(msa_unit_fault[u])',
          '.state_memory_drained()','.state_result(msa_unit_result[u*18+:18])']
    code+=msa.MODULE+' #(.SYSPER(4)) member(\n'+',\n'.join(con)+'\n);\n'
    code+='assign msa_physical[u]={'+','.join('member.u_storage_'+str(i)+'.state_bits'for i in reversed(range(16)))+',member.ff_scan_bits};\nend endgenerate\n'
    code+='''always @(posedge clk)begin
 if((msa_request&(msa_request-1))!=0)$fatal(1,"nononehot MSA request");
 if((|msa_request)&&msa_group.pending)$fatal(1,"busy MSA rerouted");
 if((|msa_request)&&(!hold||!local_held||msa_unit_frozen!==8'hff))$fatal(1,"MSA request before complete boundary");
 end
'''
    declarations=' wire msa_frozen,msa_done,msa_error,msa_hold,msa_fault;wire [17:0] msa_result;\n'
    code=once(code,declarations,'')
    s=once(s,' reg [9:0] stub_done=0,stub_error=0;',declarations+' reg [9:0] stub_done=0,stub_error=0;')
    s=once(s,' integer k;reg [17:0] content[0:9];',code+' integer k;reg [17:0] content[0:9];')
    client=fmt.SerialStateFormat(catalog);vectors=[]
    def packet(words):
        vectors.extend(f"word(16'h{w:04x});"for w in words);vectors.append('finish_packet();')
    packet(client.control_words(True));vectors.append('idle(4);if(!port_ready)$fatal(1,"formatted MSA hold unavailable");')
    for u in range(8):
        for row,width in ((0,18),(149,8)):
            value=(0x259a1+u*0x137b+row)&((1<<width)-1)
            packet(client.request_words(11,(u<<8)|row,True,value));vectors.append('expect_result(0,0);')
    for u in range(8):
        for row,width in ((0,18),(149,8)):
            value=(0x259a1+u*0x137b+row)&((1<<width)-1)
            packet(client.request_words(11,(u<<8)|row));vectors.append(f"expect_result(0,18'h{value:05x});")
    packet(client.control_words(False));vectors.append('idle(4);if(hold)$fatal(1,"formatted MSA release failed");')
    vectors='\n'.join(vectors)+'\n'
    s=once(s,'  cold_reset();\n  word(16\'h70);','  cold_reset();\n'+vectors+'  cold_reset();\n  word(16\'h70);')
    tail='''
  cold_reset();control(1);
  // Indexed holes/high bits never reach any member.
  for(i=0;i<8;i=i+1)begin
   before_calls=total_calls();send_request(1,11,(i<<8)|150,18'h3ffff);expect_result(2,0);
   send_request(0,11,(i<<8)|255,0);expect_result(2,0);
   if(total_calls()!=before_calls)$fatal(1,"MSA hole reached child");
  end
  send_request(0,11,24'h000800,0);expect_result(2,0);
  before_errors=control_errors;
  prefix(16'h77,2,64'hc876f8b45b768814);word(0);finish_packet();idle(3);
  if(!hold||control_errors!=before_errors+1)$fatal(1,"old catalog changed hold");
  // All members, including an unselected one, participate in the boundary.
  msa_suppress=8'h80;idle(3);before_calls=total_calls();send_request(0,11,0,0);idle(3);
  if(port_ready!==0||total_calls()!=before_calls)$fatal(1,"missing MSA boundary accepted");
  msa_suppress=8'bx0000000;idle(3);send_request(0,11,0,0);idle(3);
  if(port_ready!==0||total_calls()!=before_calls)$fatal(1,"unknown MSA boundary accepted");
  msa_suppress=0;idle(3);
  send_request(1,11,24'h000300,18'h236ad);idle(12);
  if(!busy||!msa_group.pending||msa_group.owner!==3)$fatal(1,"real MSA scan not active");
  before_calls=total_calls();direct=1;d_request=1;d_write=1;d_board=11;d_index=24'h000795;d_data=18'h3ffff;
  idle(20);d_request=0;before_ticks=ticks;
  while(busy)begin tick();if(ticks-before_ticks>4105)$fatal(1,"unbounded MSA completion");end
  direct=0;expect_result(0,0);
  if(total_calls()!=before_calls)$fatal(1,"busy MSA request accepted");
  send_request(0,11,24'h000300,0);expect_result(0,18'h236ad);
  send_request(1,11,24'h000795,18'h3ffff);expect_result(0,0);
  send_request(0,11,24'h000795,0);expect_result(0,18'hff);
  // Loss of a different member's boundary aborts the selected real scan.
  send_request(1,11,24'h000600,18'h2345);idle(12);msa_suppress=8'h01;idle(4);expect_result(5,0);
  if(!port_fault||!msa_fault||!hold||!msa_unit_fault[6])$fatal(1,"MSA boundary loss not quarantined");
  msa_saved={msa_physical[7],msa_physical[6],msa_physical[5],msa_physical[4],msa_physical[3],msa_physical[2],msa_physical[1],msa_physical[0]};
  transport_reset=1;idle(2);transport_reset=0;control(0);idle(8);
  if(!hold||!port_fault||msa_saved!=={msa_physical[7],msa_physical[6],msa_physical[5],msa_physical[4],msa_physical[3],msa_physical[2],msa_physical[1],msa_physical[0]})$fatal(1,"MSA parser reset thawed physical state");
  cold_reset();control(1);send_request(1,11,24'h000700,18'h1b247);idle(12);
  msa_saved={msa_physical[7],msa_physical[6],msa_physical[5],msa_physical[4],msa_physical[3],msa_physical[2],msa_physical[1],msa_physical[0]};
  transport_reset=1;tick();transport_reset=0;idle(4);
  if(!hold||!port_fault||!msa_fault||!msa_unit_fault[7]||msa_saved!=={msa_physical[7],msa_physical[6],msa_physical[5],msa_physical[4],msa_physical[3],msa_physical[2],msa_physical[1],msa_physical[0]})$fatal(1,"active MSA parser reset lost ownership");
  cold_reset();control(1);msa_inject_hold=8'bx0000000;idle(3);before_errors=control_errors;
  control(0);if(msa_hold!==1||hold!==1||control_errors!=before_errors+1)$fatal(1,"unknown MSA hold did not fail closed");
  msa_inject_hold=0;msa_inject_fault=8'b0000x000;idle(3);msa_inject_fault=0;idle(3);
  if(hold!==1||port_fault!==1)$fatal(1,"unknown MSA fault not quarantined");
  cold_reset();control(1);control(0);idle(4);if(hold)$fatal(1,"clean MSA session failed release");
'''
    s=once(s,' reg [2003:0] saved,expected_chain,quarantined;',' reg [21519:0] msa_saved;\n reg [2003:0] saved,expected_chain,quarantined;')
    s=once(s,'  $display("SERIAL BOARD STATE PASS',tail+'  $display("MSA TRANSPORT PASS')
    s=once(s,'real2004scan/ABI2/schema/all11boards/5services/timeout/sticky-global-hold/transport-vs-SYSTEM-reset/Ethernet',
             'actual8MSAs/12slots/ABI2/schema/5stubservices/4096budget/sticky-global-hold/transport-vs-SYSTEM-reset/Ethernet')
    return s,vectors


def reject(call):
    try:call()
    except ValueError:return
    raise AssertionError('invalid MSA format accepted')


def format_tests(catalog):
    client=fmt.SerialStateFormat(catalog);board=catalog['boards'][11];values=[];expected=0
    for index,width,offset in fmt.old.iter_words(board):
        value=(index*0x19e377+0x514a9)&((1<<width)-1);values.append((index,value));expected|=value<<offset
    image=fmt.old.encode_board(board,values)
    assert len(image)==2690 and image==expected.to_bytes(2690,'little')
    assert list(fmt.old.decode_board(board,image))==values
    for unit in range(8):
        for index in(150,255):reject(lambda:client.request_words(11,(unit<<8)|index))
        reject(lambda:client.request_words(11,(unit<<8)|149,True,256))
    reject(lambda:client.request_words(11,2048));reject(lambda:client.request_words(12,0))
    controls=[]
    for case in('alias','stride','member_order','missing_member','total','budget','missing_slot','padding','abi'):
        bad=copy.deepcopy(catalog);b=bad['boards'][11]
        if case=='alias':b['runs'][2]['word']=0
        elif case=='stride':b['member_stride']=150
        elif case=='member_order':b['members'][0],b['members'][1]=b['members'][1],b['members'][0]
        elif case=='missing_member':b['members'].pop()
        elif case=='total':bad['board_bits']+=1
        elif case=='budget':bad['response_limit']=2048
        elif case=='missing_slot':bad['boards'].pop()
        elif case=='padding':b['runs'][1]['width']=18
        else:bad['abi']=1
        payload={k:v for k,v in bad.items()if k not in('catalog_sha256','schema_id')};digest=hashlib.sha256(fmt.old.canonical(payload)).hexdigest()
        bad.update(catalog_sha256=digest,schema_id=digest[:16]);reject(lambda:fmt.SerialStateFormat(bad));controls.append(dict(case=case,rejected=True))
    changed=copy.deepcopy(catalog);isolated=fmt.SerialStateFormat(changed);before=isolated.request_words(11,0x795)
    changed['boards'][11]['runs'][0]['word']=99;assert isolated.request_words(11,0x795)==before
    return dict(status='PASS',msa_bits=21520,msa_bytes=2690,words=1200,independent_dense_integer_oracle=True,failure_controls=controls)


def run(out,ethernet,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir();catalog=fmt.build_catalog(msa.ROOT/'fpga/mister/reports')
    top,vectors=fixture(catalog);(out/'catalog.json').write_text(json.dumps(catalog,indent=2)+'\n');(out/'formatted-vectors.svh').write_text(vectors)
    (out/'format-results.json').write_text(json.dumps(format_tests(catalog),indent=2)+'\n')
    board,manifest,clones=msa.generate();board,normalization=hoist_msa_declarations(board)
    (out/'msa-declaration-normalization.json').write_text(json.dumps(normalization,indent=2)+'\n')
    assets={'msa_board.sv':board,**clones}
    for name in('board_state_fabric_msa.sv','board_state_uio_msa.sv','msa_state_group.sv'):assets[name]=(HERE/name).read_text()
    originals=msa.original_cells()
    originals.update({'msa_ddram.v':msa.original(msa.SOURCE),'dorado_storage_group.sv':msa.storage.original_source(),ethernet.name:ethernet.read_bytes()})
    for name,raw in originals.items():(inputs/name).write_bytes(raw)
    for p in(Path(__file__).resolve(),HERE/'serial_state_format_msa.py',HERE/'test_board_state_serial.sv'):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None)]
    if mutations:cases +=[
        ('short_timeout','board_state_fabric_msa.sv','RESPONSE_LIMIT=4096','RESPONSE_LIMIT=2048','result differs'),
        ('wrong_member','msa_state_group.sv','unit_request=(admit && valid_index)?route','unit_request=(admit && valid_index)?8\'b1','result differs'),
        ('unstripped_index','msa_state_group.sv',"unit_index={16'b0,index[7:0]}",'unit_index=index','result differs'),
        ('live_owner','msa_state_group.sv','unit_done[owner]','unit_done[index[10:8]]','result differs'),
        ('group_boundary','msa_state_group.sv',"assign frozen=(unit_frozen===8'hff);","assign frozen=1'b1;",'missing MSA boundary accepted'),
        ('hold_unknown','msa_state_group.sv',"children_holding=(unit_hold_required!==8'b0)",'children_holding=|unit_hold_required','unknown MSA hold did not fail closed'),
        ('write_payload','msa_state_group.sv','assign unit_data=data;',"assign unit_data=18'b0;",'result differs'),
        ('old_schema','board_state_uio_msa.sv',"6:if(din!=SCHEMA_ID[63:48])valid_packet<=0;",'6:begin end','bad schema accepted'),
        ('status_count','board_state_uio_msa.sv',"3:dout<=16'd12;","3:dout<=16'd11;",'serial capability/schema differs'),
    ]
    results=[]
    for label,target,before,after,why in cases:
        case=out/label;case.mkdir();compiled=[]
        for name,body in assets.items():
            if name==target:assert body.count(before)==1,(label,before);body=body.replace(before,after,1)
            p=case/name;p.write_text(body);compiled.append(p)
        (case/'fixture.sv').write_text(top)
        command=['iverilog','-g2012','-s','msa_transport_fixture','-o',str(case/'test.vvp'),str(case/'fixture.sv'),*map(str,compiled),*[str(inputs/n)for n in originals]]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip().splitlines()[0],flush=True)
        if why:assert result.returncode and why in text,(label,text)
        else:assert result.returncode==0 and 'MSA TRANSPORT PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=why))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,physical_msa_bits=21520,
        schema_id=catalog['schema_id'],catalog_sha256=catalog['catalog_sha256'],formatted_packets=34,
        limits='Four-state transport with eight actual MSA adapters; original eleven endpoints/foreign services are stubs, guest advances stopped. Not native machine pause, external RAM capture, core fit/timing or FPGA snapshot.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True);p.add_argument('--mutations',action='store_true')
    a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve(),a.mutations)
