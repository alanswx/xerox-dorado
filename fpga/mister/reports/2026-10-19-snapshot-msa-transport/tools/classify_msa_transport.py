#!/usr/bin/env python3
"""Fresh twelve-slot transport/router/eight-MSA state and exact parameter bindings.

The eleven other endpoints and service barriers are external input contracts.
All eight actual MSA physical/private states are included. No native wrapper,
external raw RAM, ARM state, full-core fit or machine snapshot is qualified.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
import generate_msa_board as gen
import test_msa_transport as tested
import test_msa_board as board_test
from classify_msa_board import PRIVATE as MEMBER_PRIVATE
from classify_serial_boards import read_inventory,alias_controls

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import constant

FABRIC=dict(command_hold=1,control_error=1,pending=1,was_write=1,owner=4,age=12,done=1,error=3,result=18,fault=1)
UIO=dict(hold_update=1,hold_value=1,request=1,write=1,board=4,index=24,data=18,
 busy=1,complete=1,txn_sequence=32,control_errors=32,command=16,dout=16,count=12,
 valid_packet=1,mode=1,operation=1,pending_board=4,pending_index=24,pending_data=18,
 last_result=18,last_error=3,flags_snapshot=16,frozen_snapshot=12,sequence_snapshot=32,
 errors_snapshot=32,result_snapshot=18,error_snapshot=3,services_snapshot=5)
GROUP=dict(pending=1,was_write=1,local_fault=1,owner=3,done=1,error=1,result=18)


def fixture(source):
    begin=source.index(' dorado_board_state_uio_msa #(');end=source.index(' dorado_ethernet_uio',begin)
    transport=source[begin:end]
    begin=source.index(' wire [7:0] msa_request,');end=source.index(' integer k;reg [17:0] content[0:9];',begin)
    members=source[begin:end]
    members=tested.once(members,' reg [7:0] msa_suppress=0,msa_inject_hold=0,msa_inject_fault=0;\n','')
    members=tested.once(members,'integer msa_calls[0:7];','')
    members=tested.once(members,''' always @(posedge clk)begin
  if(system_reset)msa_calls[u]<=0;
  else if(msa_request[u])msa_calls[u]<=msa_calls[u]+1;
 end
''','')
    for name in('frozen','hold','fault'):
        before={'frozen':'msa_unit_frozen&~msa_suppress','hold':'msa_unit_hold|msa_inject_hold','fault':'msa_unit_fault|msa_inject_fault'}[name]
        members=tested.once(members,before,{'frozen':'msa_unit_frozen','hold':'msa_unit_hold','fault':'msa_unit_fault'}[name])
    header='''module msa_stack_state_fixture(
 input wire clk,system_reset,transport_reset,local_held,input wire [4:0] services_held,
 input wire [10:0] main_frozen,main_done,main_error,main_hold,main_fault,input wire [197:0] main_result,
 inout wire [35:0] state_bus,output wire hold,scan_abort,output wire [10:0] main_request);
 wire command_hold,hold_update,hold_value,control_error;
 wire u_request,u_write,port_ready,port_done,port_fault,busy,complete;
 wire [3:0] u_board;wire [23:0] u_index;wire [17:0] u_data,port_result;
 wire [2:0] port_error;wire [31:0] txn_sequence,control_errors;
 wire msa_frozen,msa_done,msa_error,msa_hold,msa_fault;wire [17:0] msa_result;
 wire [11:0] board_frozen={msa_frozen,main_frozen},board_done={msa_done,main_done},board_error={msa_error,main_error};
 wire [11:0] board_holds={msa_hold,main_hold},board_faults={msa_fault,main_fault},board_request;
 wire [215:0] board_result={msa_result,main_result};
 wire request=u_request,wr=u_write,update_hold=hold_update,value_hold=hold_value;
 wire [3:0] selected=u_board;wire [23:0] index=u_index;wire [17:0] data=u_data;
 assign main_request=board_request[10:0];
'''
    schema=re.search(r" localparam \[63:0\] SCHEMA=64'h[0-9a-f]+;",source)[0]
    return header+schema+'\n'+transport+members+'endmodule\n'


def bindings(raw):
    values={}
    for module in ET.fromstring(raw).findall('.//module'):
        name=module.get('origName')
        if name not in('dorado_board_state_fabric_msa','dorado_board_state_uio_msa'):continue
        assert name not in values,'duplicate transport specialization'
        values[name]={v.get('name'):constant(v.find('const').get('name'))for v in module.findall('./var')
                      if(v.get('param')=='true'or v.get('localparam')=='true')and v.find('const')is not None}
    return values


def classify(data,catalog):
    tag=int(catalog['schema_id'],16)
    assert data['bindings']==dict(dorado_board_state_fabric_msa=dict(RESPONSE_LIMIT=4096,SERVICE_DOMAINS=5,AGE_BITS=12),
        dorado_board_state_uio_msa=dict(SCHEMA_ID=tag,SERVICE_DOMAINS=5,RESPONSE_LIMIT=4096)),'transport schema/deadline binding differs'
    expected={};prefix='msa_stack_state_fixture.'
    def add(scope,fields,kind):
        for name,width in fields.items():expected[prefix+scope+name]=(width,kind)
    add('fabric.',FABRIC,'PRIVATE TRANSPORT');add('uio.',UIO,'PRIVATE TRANSPORT');add('msa_group.',GROUP,'PRIVATE TRANSPORT')
    physical={r['path']:r['width']for r in board_test.physical_fields()}
    for unit in range(8):
        add(f'members__BRA__{unit}__KET__.member.',physical,'PHYSICAL MSA')
        add(f'members__BRA__{unit}__KET__.member.',MEMBER_PRIVATE,'PRIVATE MSA')
    decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        wanted=expected.pop(row['path'],None)
        assert wanted is not None,'extra/duplicate stack state: '+row['path']
        width,scope=wanted
        assert(row['width'],row['bits'],row['unpacked_dimensions'])==(width,width,[]),'stack state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=width,scope=scope))
    assert not expected,'missing stack state: '+str(list(expected)[:3])
    totals={scope:sum(r['bits']for r in decisions if r['scope']==scope)for scope in('PHYSICAL MSA','PRIVATE MSA','PRIVATE TRANSPORT')}
    assert totals=={'PHYSICAL MSA':21520,'PRIVATE MSA':664,'PRIVATE TRANSPORT':417},totals
    return dict(status='PASS',scope=__doc__,physical_msa_bits=21520,private_msa_bits=664,private_transport_bits=417,
                sequential_exclusions=[],decisions=decisions,bindings=data['bindings'],xml_sha256=data['xml_sha256'],
                normalized_xml_sha256=data['normalized_xml_sha256'],resolved_module_aliases=data['resolved_module_aliases'])


def controls(data,catalog):
    results=[]
    for case in('missing_physical','missing_member','missing_owner','extra','duplicate','width','dimensions','short_bitmap','short_deadline','bad_schema'):
        broken=copy.deepcopy(data);rows=broken['candidates']
        target=next(r for r in rows if r['path']=='msa_stack_state_fixture.members__BRA__7__KET__.member.u_storage_0.old_data')
        if case=='missing_physical':rows.remove(target)
        elif case=='missing_member':broken['candidates']=[r for r in rows if not r['path'].startswith('msa_stack_state_fixture.members__BRA__7__KET__.member.')]
        elif case=='missing_owner':rows.remove(next(r for r in rows if r['path']=='msa_stack_state_fixture.msa_group.owner'))
        elif case in('extra','duplicate'):
            added=copy.deepcopy(target)
            if case=='extra':added['path']+='_hidden'
            rows.append(added)
        elif case=='width':target['width']+=1
        elif case=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif case=='short_bitmap':next(r for r in rows if r['path']=='msa_stack_state_fixture.uio.frozen_snapshot')['width']=11
        elif case=='short_deadline':broken['bindings']['dorado_board_state_fabric_msa']['RESPONSE_LIMIT']=2048
        else:broken['bindings']['dorado_board_state_uio_msa']['SCHEMA_ID']^=1
        try:classify(broken,catalog)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged MSA stack inventory accepted: '+case)
    return results


def run(tests,out):
    result=json.loads((tests/'results.json').read_text());assert result['status']=='PASS'
    catalog=json.loads((tests/'catalog.json').read_text());tested.fmt.validate_catalog(catalog)
    original,vectors=tested.fixture(catalog);case=tests/'candidate'
    assert(case/'fixture.sv').read_text()==original
    out.mkdir(parents=True,exist_ok=False);(out/'top.sv').write_text(fixture(original))
    command=json.loads((case/'command.json').read_text());sources=[p for p in command if p.endswith(('.v','.sv'))and p!=str(case/'fixture.sv')]
    cmd=['verilator','--xml-only','--assert','--top-module','msa_stack_state_fixture','-Wno-fatal',
         '--xml-output',str(out/'state.xml'),str(out/'top.sv'),*sources]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'state.xml').read_bytes();data,normalized=read_inventory(raw);data['bindings']=bindings(raw)
    (out/'state-normalized.xml').write_bytes(normalized);r=classify(data,catalog);r['failure_controls']=controls(data,catalog)+alias_controls(raw)
    for name,value in [('inventory',data),('classification',r)]:(out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status='PASS',physical_msa_bits=21520,private_msa_bits=664,private_transport_bits=417,failure_controls=len(r['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('tests',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.tests.resolve(),a.output.resolve())
