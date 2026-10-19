#!/usr/bin/env python3
"""Complete private transport/shell XML inventory; no guest-state exclusions."""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import xml.etree.ElementTree as ET
import fit_board_state_serial as fit

ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'tools'))
from sil_state_inventory import inventory,constant


def report(xml):
    data=inventory(xml);bindings={}
    for mod in ET.fromstring(xml).findall('.//module'):
        if mod.get('origName') not in ('dorado_board_state_fabric_serial','dorado_board_state_uio_serial'):continue
        bindings[mod.get('origName')]={v.get('name'):constant(v.find('const').get('name')) for v in mod.findall('var')
            if (v.get('param')=='true' or v.get('localparam')=='true') and v.find('const') is not None}
    data['bindings']=bindings
    return data


def classify(data):
    assert data['bindings']==dict(
        dorado_board_state_fabric_serial=dict(RESPONSE_LIMIT=2048,SERVICE_DOMAINS=5,AGE_BITS=11),
        dorado_board_state_uio_serial=dict(SCHEMA_ID=0x79c5a16e42d38b07,SERVICE_DOMAINS=5,RESPONSE_LIMIT=2048)), 'transport parameter/schema binding differs'
    wanted={}
    def add(scope,values):
        for n,w in values.items():wanted['snapshot_fit.'+scope+n]=w
    add('',dict(input_stage=280,launch=280,outputs=168))
    add('fabric.',dict(command_hold=1,control_error=1,pending=1,was_write=1,owner=4,age=11,done=1,error=3,result=18,fault=1))
    add('uio.',dict(hold_update=1,hold_value=1,request=1,write=1,board=4,index=24,data=18,
        busy=1,complete=1,txn_sequence=32,control_errors=32,command=16,dout=16,count=12,
        valid_packet=1,mode=1,operation=1,pending_board=4,pending_index=24,pending_data=18,
        last_result=18,last_error=3,flags_snapshot=16,frozen_snapshot=11,sequence_snapshot=32,
        errors_snapshot=32,result_snapshot=18,error_snapshot=3,services_snapshot=5))
    decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        width=wanted.pop(row['path'],None)
        assert width is not None,'extra/duplicate transport state: '+row['path']
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==(width,width,[]),'transport field geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=width,scope='PRIVATE SHELL' if row['path'].count('.')==1 else 'PRIVATE TRANSPORT'))
    assert not wanted,'missing transport state: '+str(list(wanted)[:3])
    bits=sum(r['bits'] for r in decisions);assert bits==1117,bits
    return dict(status='PASS',scope=__doc__,private_transport_bits=389,private_shell_bits=728,
        guest_state_bits=0,sequential_exclusions=[],decisions=decisions,bindings=data['bindings'],xml_sha256=data['xml_sha256'])


def controls(data):
    results=[]
    targets=dict(lost_hold='.fabric.command_hold',lost_fault='.fabric.fault',lost_owner='.fabric.owner',
                 lost_busy='.uio.busy',lost_sequence='.uio.txn_sequence',lost_status='.uio.result_snapshot')
    for case in [*targets,'short_age','short_index','short_services','extra_state','duplicate','bad_schema','short_timeout']:
        bad=copy.deepcopy(data)
        if case in targets:
            before=len(bad['candidates']);bad['candidates']=[r for r in bad['candidates'] if not r['path'].endswith(targets[case])]
            assert len(bad['candidates'])<before
        elif case in ('short_age','short_index','short_services'):
            suffix=dict(short_age='.fabric.age',short_index='.uio.index',short_services='.uio.services_snapshot')[case]
            next(r for r in bad['candidates'] if r['path'].endswith(suffix))['width']-=1
        elif case in ('extra_state','duplicate'):
            row=copy.deepcopy(next(r for r in bad['candidates'] if r['category']=='sequential'))
            if case=='extra_state':row['path']+='_hidden'
            bad['candidates'].append(row)
        elif case=='bad_schema':bad['bindings']['dorado_board_state_uio_serial']['SCHEMA_ID']^=1
        else:bad['bindings']['dorado_board_state_fabric_serial']['RESPONSE_LIMIT']=16
        try:classify(bad)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged serial transport inventory accepted: '+case)
    return results


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    for n in ('board_state_fabric_serial.sv','board_state_uio_serial.sv'):(out/n).write_bytes((fit.HERE/n).read_bytes())
    (out/'top.sv').write_text(fit.shell(True))
    command=['verilator','--xml-only','--top-module','snapshot_fit','-Wno-fatal','--xml-output',str(out/'transport.xml'),
             str(out/'top.sv'),str(out/'board_state_fabric_serial.sv'),str(out/'board_state_uio_serial.sv')]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    data=report((out/'transport.xml').read_bytes());result=classify(data);result['failure_controls']=controls(data)
    for name,value in [('inventory',data),('classification',result)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print('SERIAL TRANSPORT CLASSIFICATION PASS:389 private transport+728 shell bits;',len(result['failure_controls']),'damaged controls reject')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
