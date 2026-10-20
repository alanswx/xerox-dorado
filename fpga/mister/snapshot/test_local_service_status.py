#!/usr/bin/env python3
"""Actual UIO/owner local ACK status; default-off matches every original bit.

Check coherent raw device ACKs while ARM permission is missing. Device and
endpoint signals are controlled test models; no native/Main admission claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,raw_receipt,once
from classify_serial_boards import read_inventory
import serial_state_format_msa_chipwidth as fmt
from serial_state_format_service_status import parse_service_status

HERE=Path(__file__).resolve().parent


def inverse(text):
    text=once(text,''' // Optional local ACKs are distinct from host permission. Status flag11
 // advertises words18=coherent local ACKs,19=mask,20=extension version ac01.
 // Keep ABI2 board-only scope; default-off status remains byte-identical.
 parameter [SERVICE_DOMAINS-1:0] LOCAL_SERVICES_MASK=0,
''','')
    for before in(' input wire [SERVICE_DOMAINS-1:0] local_services_held,\n',
        ' reg [SERVICE_DOMAINS-1:0] local_services_snapshot=0;\n',
        '       local_services_snapshot<=local_services_held & LOCAL_SERVICES_MASK;\n',
        '''      18:dout<={{(16-SERVICE_DOMAINS){1'b0}},local_services_snapshot};
      19:dout<={{(16-SERVICE_DOMAINS){1'b0}},LOCAL_SERVICES_MASK};
      20:dout<=(LOCAL_SERVICES_MASK!=0) ? 16'hac01 : 16'b0;
'''):text=once(text,before,'')
    return once(text,"{4'b0,(LOCAL_SERVICES_MASK!=0),command_hold,", "{5'b0,command_hold,")


def run(out):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    parent=ROOT/'fpga/mister/reports/2026-10-20-snapshot-native-network-pause-rejected'
    old=raw_receipt(parent,'terminal-fit/src/fpga/mister/snapshot_serial/board_state_uio_msa.sv')
    source=(HERE/'board_state_uio_msa.sv').read_bytes();assert inverse(source.decode()).encode()==old
    fabric=raw_receipt(parent,'terminal-fit/src/fpga/mister/snapshot_serial/board_state_fabric_msa.sv')
    for n,raw in(('original.sv',old),('candidate.sv',source),('fabric.sv',fabric)):(inputs/n).write_bytes(raw)
    fields={}
    for label in('original','candidate'):
        cmd=['verilator','--xml-only','--top-module','dorado_board_state_uio_msa','-Wno-fatal',"-GSCHEMA_ID=64'haf0b647cb43a3a62",'--xml-output',str(out/(label+'.xml')),str(inputs/(label+'.sv'))]
        (out/(label+'-command.json')).write_text(json.dumps(cmd,indent=2)+'\n')
        with(out/(label+'-xml.log')).open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        data,_=read_inventory((out/(label+'.xml')).read_bytes());rows={}
        for row in data['candidates']:
            if row['category']!='sequential':continue
            assert not row['unpacked_dimensions'];rows[row['name']]=row['bits']
        fields[label]=rows
    expected=dict(fields['original']);expected['local_services_snapshot']=5;assert fields['candidate']==expected
    bits=sum(fields['original'].values());top=(HERE/'test_local_service_status.sv').read_text().replace('ORIGINAL_BITS',str(bits))
    shared='''.clk(clk),.system_reset(system_reset),.transport_reset(transport_reset),
 .hold(hold),.command_hold(command_hold),.local_held(hold),.services_held(services),
 .control_error(control_error),.board_frozen(board_frozen),
 .ready(ready),.done(done),.error(error),.result(result),.fault(fault)'''
    instances=[]
    for label,module,params,bus in(('original','dorado_board_state_uio_reference','', 'original_bus'),
        ('legacy','dorado_board_state_uio_msa','', 'legacy_bus'),('enabled','dorado_board_state_uio_msa',',.LOCAL_SERVICES_MASK(7)','service_bus')):
        body=shared+',.bus('+bus+')'
        if label!='original':body+=',.local_services_held(local_ack)'
        if label=='enabled':body+=',.hold_update(hold_update),.hold_value(hold_value),.request(request),.write(write),.board(board),.index(index),.data(data)'
        instances.append(module+' #(.SCHEMA_ID(SCHEMA)'+params+') '+label+'_uio('+body+');\n')
    top=once(top,'    // INSTANCE_BLOCK','\n'.join(instances))
    compare='\n'.join('check(original_uio.'+n+'===legacy_uio.'+n+',"default-off original field differs: '+n+'");'for n in fields['original'])
    top=once(top,'        // COMPARE_BLOCK',compare)
    (inputs/'reference.sv').write_text(once(old.decode(),'module dorado_board_state_uio_msa #','module dorado_board_state_uio_reference #'))
    for p in(Path(__file__),HERE/'test_local_service_status.sv',HERE/'serial_state_format_service_status.py'):(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None),
        ('torn_local','local_services_snapshot};','local_services_held & LOCAL_SERVICES_MASK};','local ACK snapshot tore'),
        ('foreign_as_local','local_services_snapshot<=local_services_held & LOCAL_SERVICES_MASK;','local_services_snapshot<=services_held & LOCAL_SERVICES_MASK;','local ACK snapshot tore'),
        ('capability_missing','(LOCAL_SERVICES_MASK!=0),command_hold',"1'b0,command_hold",'local status capability'),
        ('wrong_mask','19:dout<={{(16-SERVICE_DOMAINS){1\'b0}},LOCAL_SERVICES_MASK};',"19:dout<=16'h1f;",'default-off UIO output differs'),
        ('wrong_version',"16'hac01 : 16'b0","16'hac02 : 16'b0",'local status mask/version differs')]
    results=[];statuses=[]
    for label,before,after,diagnostic in cases:
        case=out/label;case.mkdir();body=source.decode()
        if before:body=once(body,before,after)
        (case/'candidate.sv').write_text(body);(case/'fixture.sv').write_text(top)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),str(case/'fixture.sv'),str(case/'candidate.sv'),str(inputs/'reference.sv'),str(inputs/'fabric.sv')]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=40)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if diagnostic:assert result.returncode and diagnostic in text,text
        else:
            assert not result.returncode and'LOCAL SERVICE STATUS PASS'in text,text
            statuses=[[int(w,16)for w in line.split()[1:]]for line in text.splitlines()if line.startswith('LOCAL_SERVICE_STATUS ')]
            assert len(statuses)==4
        print(label+': '+text.strip().splitlines()[-1],flush=True)
        results.append(dict(case=label,returncode=result.returncode,expected_failure=diagnostic,
            log_sha256=hashlib.sha256((case/'test.log').read_bytes()).hexdigest(),rtl_sha256=hashlib.sha256(body.encode()).hexdigest()))
    catalog=fmt.build_catalog(ROOT/'fpga/mister/reports');client=fmt.SerialStateFormat(catalog)
    parsed=[parse_service_status(client,words)for words in statuses]
    assert parsed[0]['local_services_held']&2 and not parsed[0]['services_held']&2 and not parsed[0]['ready']
    assert parsed[1]['local_services_held']==7 and parsed[1]['ready']and parsed[2]['local_services_held']==0 and not parsed[2]['ready']
    bad=[]
    for name in('short','long','missing_flag','version','schema','unsupported_mask','outside_mask','reserved_flag','noninteger'):
        words=list(statuses[0])
        if name=='short':words.pop()
        elif name=='long':words.append(0)
        elif name=='missing_flag':words[2]&=~(1<<11)
        elif name=='version':words[20]^=1
        elif name=='schema':words[12]^=1
        elif name=='unsupported_mask':words[19]|=32
        elif name=='outside_mask':words[18]|=8
        elif name=='reserved_flag':words[2]|=1<<12
        else:words[18]=True
        try:parse_service_status(client,words)
        except ValueError:bad.append(dict(case=name,rejected=True))
        else:raise AssertionError('invalid local status accepted: '+name)
    (out/'qualification.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,original_fields=len(fields['original']),original_bits=bits,
        new_required_top_bits=5,sequential_exclusions=[],cases=results,status_words=statuses,parsed_status=parsed,client_failure_controls=bad,
        inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Real UIO and owner only; endpoints/device ACKs controlled. No whole-machine permission, Main coordinator, native fit or restore.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.output.resolve())
