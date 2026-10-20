#!/usr/bin/env python3
"""Clocked host-service endpoint and actual Main SPI client qualifications.

Real RTL/client transactions, with modeled native boundary/local ACK inputs.
No Main disk/daemon coordinator, physical DDR fence, native Core integration,
complete snapshot, Quartus or hardware claim is made by these component tests.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import subprocess
import sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from sil_state_inventory import inventory

FIELDS={'hold_update':1,'hold_value':1,'services_permitted':5,'epoch':64,'phase':2,'errors':32,
        'command':16,'dout':16,'count':6,'valid_packet':1,'pending_epoch':64,'pending_services':5,
        'phase_snapshot':2,'epoch_snapshot':64,'services_snapshot':5,'available_snapshot':5,
        'local_snapshot':5,'boundary_snapshot':6,'errors_snapshot':32}


def sha(raw):return hashlib.sha256(raw).hexdigest()


def run(out,mutations):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    main=HERE.parent/'main'
    files={'host_service_control.sv':HERE/'host_service_control.sv',
        'board_state_uio_msa.sv':HERE/'board_state_uio_msa.sv',
        'test_host_service_control.sv':HERE/'test_host_service_control.sv',
        'dorado_snapshot_client.cpp':main/'dorado_snapshot_client.cpp',
        'dorado_snapshot_client.h':main/'dorado_snapshot_client.h',
        'snapshot_host_top.sv':main/'tests/snapshot_host_top.sv',
        'test_snapshot_client.cpp':main/'tests/test_snapshot_client.cpp',
        'spi.h':main/'tests/spi.h','user_io.h':main/'tests/user_io.h'}
    sources={n:p.read_text()for n,p in files.items()}
    for n,text in sources.items():(inputs/n).write_text(text)
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    rtl_cases=[('rtl',None,None,None,None)]
    if mutations:rtl_cases +=[
        ('grant_without_ack',' && qualified_services','',1,'unacknowledged local service granted'),
        ('stale_nonce','pending_epoch==epoch',"1'b1",4,'stale lease changed grants'),
        ('wrong_build','if(din!==BUILD_ID[15:0])valid_packet<=0;',"if(1'b0)valid_packet<=0;",1,'damaged identity/nonce/padding changed lease'),
        ('partial_commit','count==16','count>=15',1,'partial/overlong grant committed'),
        ('parser_thaw','if(phase!=IDLE)begin\n    phase<=FAULT;services_permitted<=0;',
         'if(phase!=IDLE)begin\n    phase<=IDLE;services_permitted<=0;',1,'parser reset released quarantine'),
        ('revoked_ack','(services_permitted & local_held)!==services_permitted',"1'b0",1,'ACK revocation did not quarantine'),
        ('fault_blocks_status','end else if(!enable)begin','end else if(board_fault)begin phase<=FAULT;services_permitted<=0;\n  end else if(!enable)begin',1,'device fault blocked status parser')]
    results=[]
    for label,before,after,count,expected in rtl_cases:
        case=out/label;case.mkdir();rtl=inputs/'host_service_control.sv'
        if before:
            assert sources[rtl.name].count(before)==count,(label,sources[rtl.name].count(before))
            rtl=case/rtl.name;rtl.write_text(sources[rtl.name].replace(before,after))
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),str(rtl),str(inputs/'test_host_service_control.sv')]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:900],flush=True)
        if expected:assert result.returncode and expected in text,text
        else:assert not result.returncode and'HOST SERVICE CONTROL PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=expected))
    cpp_cases=[('main-client',None,None,None,None)]
    compatibility=['c++','-std=c++14','-Wall','-Wextra','-Werror','-I'+str(inputs),
        '-c',str(inputs/'dorado_snapshot_client.cpp'),'-o',str(out/'main-client-cxx14.o')]
    (out/'main-client-cxx14-command.json').write_text(json.dumps(compatibility,indent=2)+'\n')
    with(out/'main-client-cxx14.log').open('w')as log:subprocess.run(compatibility,stdout=log,stderr=subprocess.STDOUT,check=True)
    if mutations:cpp_cases +=[
        ('mgl_identity','user_io_get_core_name(1)','user_io_get_core_name()',1,'Main failed to decode actual RTL status'),
        ('wrong_nonce_word','uint16_t(nonce>>(16*i))','uint16_t(nonce>>(16*((i+1)%4)))',1,'Main action framing differs'),
        ('unvalidated_build','get64(words+4)!=build_','false',1,'mismatched build accepted'),
        ('malformed_authority','words[16]>31','false',1,'malformed status published authority')]
    for label,before,after,count,expected in cpp_cases:
        case=out/label;case.mkdir();cpp=inputs/'dorado_snapshot_client.cpp'
        if before:
            assert sources[cpp.name].count(before)==count
            cpp=case/cpp.name;cpp.write_text(sources[cpp.name].replace(before,after))
        cmd=['verilator','--cc','--exe','--build','--assert','-j','4','--top-module','snapshot_host_top','-Wno-fatal',
            '--Mdir',str(case/'obj'),str(inputs/'host_service_control.sv'),str(inputs/'board_state_uio_msa.sv'),str(inputs/'snapshot_host_top.sv'),
            str(cpp),str(inputs/'test_snapshot_client.cpp'),'-CFLAGS','-I'+str(inputs)]
        (case/'commands.json').write_text(json.dumps([cmd,[str(case/'obj/Vsnapshot_host_top')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vsnapshot_host_top')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text[:900],flush=True)
        if expected:assert result.returncode and expected in text,text
        else:assert not result.returncode and'MAIN SNAPSHOT CLIENT RTL PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,expected_failure=expected))
    case=out/'inventory';case.mkdir()
    cmd=['verilator','--xml-only','--timing','--assert','--top-module','snapshot_host_top','-Wno-fatal',
        '--xml-output',str(case/'host.xml'),str(inputs/'host_service_control.sv'),str(inputs/'board_state_uio_msa.sv'),str(inputs/'snapshot_host_top.sv')]
    (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
    data=inventory((case/'host.xml').read_bytes());actual={};legacy_bits=0
    for row in data['candidates']:
        if row['category']=='combinational_review':continue
        if row['path']=='snapshot_host_top.command_hold':continue
        if row['path'].startswith('snapshot_host_top.legacy.'):
            assert row['category']=='sequential';legacy_bits+=row['bits'];continue
        assert row['path'].startswith('snapshot_host_top.controller.')and row['category']=='sequential',row
        assert row['unpacked_dimensions']==[]and row['bits']==row['width']
        actual[row['path'].removeprefix('snapshot_host_top.controller.')]=row['bits']
    assert actual==FIELDS and sum(actual.values())==332 and legacy_bits==353
    (case/'inventory.json').write_text(json.dumps(data,indent=2)+'\n')
    result=dict(status='PASS_HOST_SERVICES_AND_MAIN_CLIENT_COMPONENT',scope=__doc__,cases=results,
        new_required_service_bits=332,required_field_geometry=FIELDS,access_complete=False,
        inputs={p.name:sha(p.read_bytes())for p in inputs.iterdir()},
        limits='New endpoint/Main client components only. Actual native wiring, Main disk/daemon lease coordinator, input/video ACKs, physical DDR fence, state access, full snapshots and FPGA qualification remain open.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
