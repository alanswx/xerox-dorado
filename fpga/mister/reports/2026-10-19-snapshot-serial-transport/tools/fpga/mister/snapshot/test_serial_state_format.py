#!/usr/bin/env python3
"""Qualify pinned board maps, dense encoding and real ABI2 formatted packets."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import serial_state_format as fmt

HERE=Path(__file__).resolve().parent


def reject(call,why):
    try:call()
    except ValueError:return
    raise AssertionError('accepted '+why)


def value_at(board,index,width):
    return (index*0x19e377+board*0x4321+0x514a9)&((1<<width)-1)


def run(out,ethernet):
    out.mkdir(parents=True,exist_ok=False)
    catalog=fmt.build_catalog(fmt.ROOT/'fpga/mister/reports');client=fmt.SerialStateFormat(catalog)
    (out/'catalog.json').write_text(json.dumps(catalog,indent=2)+'\n')
    # Independent integer oracle spans byte and18bit word boundaries.
    synthetic=dict(bits=44,runs=[dict(word=0,count=2,width=18,offset=0),
        dict(word=9,count=1,width=1,offset=36),dict(word=16,count=1,width=7,offset=37)])
    words=[(0,0x23456),(1,0x15555),(9,1),(16,0x65)]
    expected=(0x23456|(0x15555<<18)|(1<<36)|(0x65<<37)).to_bytes(6,'little')
    assert fmt.encode_board(synthetic,words)==expected
    assert list(fmt.decode_board(synthetic,expected))==words
    reject(lambda:fmt.encode_board(synthetic,words[:-1]),'short image')
    reject(lambda:fmt.encode_board(synthetic,words+[words[-1]]),'extra image word')
    reject(lambda:fmt.encode_board(synthetic,[words[1],words[0],*words[2:]]),'reordered words')
    reject(lambda:fmt.encode_board(synthetic,[*words[:-1],(16,128)]),'word padding')
    reject(lambda:list(fmt.decode_board(synthetic,expected[:-1])),'short dense image')
    reject(lambda:list(fmt.decode_board(synthetic,expected[:-1]+bytes([expected[-1]|0xf0]))),'dense padding')
    outcomes=[]
    for board in catalog['boards']:
        values=((index,value_at(board['id'],index,width)) for index,width,_ in fmt.iter_words(board))
        image=fmt.encode_board(board,values);count=0
        for (actual_index,value),(index,width,_) in zip(fmt.decode_board(board,image),fmt.iter_words(board)):
            assert actual_index==index and value==value_at(board['id'],index,width)
            count+=1
        assert count==board['words'];(out/(board['name']+'.bin')).write_bytes(image)
        outcomes.append(dict(board=board['name'],bits=board['bits'],words=count,bytes=len(image),sha256=hashlib.sha256(image).hexdigest()))
    assert sum(r['bits'] for r in outcomes)==1944744
    controls=[]
    for case in ('fingerprint','alias','gap','oversized','wrong_width','missing_board','wrong_abi','missing_map_run','wrong_total'):
        bad=copy.deepcopy(catalog)
        if case=='fingerprint':bad['catalog_sha256']='0'*64
        else:
            if case=='alias':bad['boards'][0]['runs'][1]['word']=bad['boards'][0]['runs'][0]['word']
            elif case=='gap':bad['boards'][0]['runs'][1]['offset']+=1
            elif case=='oversized':bad['boards'][0]['runs'][0]['word']=1<<12
            elif case=='wrong_width':bad['boards'][0]['runs'][0]['width']=19
            elif case=='missing_board':bad['boards'].pop()
            elif case=='wrong_abi':bad['abi']=1
            elif case=='missing_map_run':bad['boards'][7]['runs'].pop()
            else:bad['board_bits']+=1
            payload={k:v for k,v in bad.items() if k not in ('catalog_sha256','schema_id')}
            digest=hashlib.sha256(fmt.canonical(payload)).hexdigest();bad.update(catalog_sha256=digest,schema_id=digest[:16])
        reject(lambda:fmt.SerialStateFormat(bad),case);controls.append(dict(case=case,rejected=True))
    tag=int(catalog['schema_id'],16)
    expected_prefix=[0xb04d,2,*[(tag>>(16*i))&0xffff for i in range(4)]]
    assert client.control_words(True)==[0x77,*expected_prefix,1]
    p=client.request_words(7,0xd10001,True,1)
    assert p==[0x78,*expected_prefix,1,7,1,0xd1,1,0]
    for args in [(11,0,False,0),(0,1<<12,False,0),(10,47<<11,False,0),(10,111,True,64),(0,0,False,1),(True,0,False,0)]:
        reject(lambda:client.request_words(*args),'invalid request')
    status=[0xd0b0,2,0x797,11,0x7ff,0xabcd,0x1234,0x3456,3,0,0x55aa,0xffff,*expected_prefix[2:],0x1f,2048]
    decoded=client.parse_status(status)
    assert decoded==dict(hold=True,busy=False,complete=True,fault=False,ready=True,frozen=0x7ff,
        sequence=0x1234abcd,result=0x33456,error=0,control_errors=0xffff55aa,services_held=0x1f)
    for field in (0,1,3,12,13,14,15,17):
        bad=list(status);bad[field]^=1;reject(lambda:client.parse_status(bad),'status capability/schema')
    for field,value in ((2,0x8000),(4,0x800),(8,4),(9,7),(16,0x20)):
        bad=list(status);bad[field]=value;reject(lambda:client.parse_status(bad),'status padding')
    reject(lambda:client.parse_status(status[:-1]),'short status')
    # Cached client data cannot be retargeted by mutating the source JSON.
    changed=copy.deepcopy(catalog);isolated=fmt.SerialStateFormat(changed);before=isolated.request_words(0,0)
    changed['boards'][0]['runs'][0]['word']=99
    assert isolated.request_words(0,0)==before
    # Run packets produced by the Python formatter through actual UIO+owner+
    # longest scan RTL, alongside unchanged native Ethernet. Other endpoints
    # and quiesce services remain the explicitly documented test stubs.
    vectors=[]
    def packet(words):
        vectors.extend(f"word(16'h{w:04x});" for w in words);vectors.append('finish_packet();')
    packet(client.control_words(True));vectors.append('idle(4);if(!port_ready)$fatal(1,"formatted control failed");')
    for board in catalog['boards']:
        identity=board['id'];run=board['runs'][-1];index=0 if identity==10 else run['word']+run['count']-1
        width=18 if identity==10 else run['width'];value=value_at(identity,index,width)
        packet(client.request_words(identity,index,True,value));vectors.append('expect_result(0,0);')
        packet(client.request_words(identity,index));expected=value if identity==10 else value^(index&0x3ffff)^(index>>18)^(identity*0x1111)
        vectors.append(f"expect_result(0,18'h{expected:05x});")
    packet(client.control_words(False));vectors.append('idle(4);if(hold)$fatal(1,"formatted release failed");')
    vectors='\n'.join(vectors)+'\n';(out/'formatted-vectors.svh').write_text(vectors)
    source=(HERE/'test_board_state_serial.sv').read_text().replace("SCHEMA=64'h79c5a16e42d38b07",f"SCHEMA=64'h{tag:016x}")
    marker='  cold_reset();\n  word(16\'h70);';assert source.count(marker)==1
    source=source.replace(marker,'  cold_reset();\n'+vectors+'  cold_reset();\n  word(16\'h70);',1)
    (out/'fixture.sv').write_text(source)
    names=['board_state_fabric_serial.sv','board_state_uio_serial.sv','ff_scan_controller.sv']
    paths=[]
    for p in [*[HERE/n for n in names],ethernet]:
        q=out/p.name;q.write_bytes(p.read_bytes());paths.append(q)
    command=['iverilog','-g2012','-s','board_state_serial_fixture','-o',str(out/'test.vvp'),str(out/'fixture.sv'),*map(str,paths)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    result=subprocess.run(['vvp',str(out/'test.vvp')],capture_output=True,text=True,timeout=60)
    (out/'test.txt').write_text(result.stdout+result.stderr)
    assert result.returncode==0 and 'SERIAL BOARD STATE PASS' in result.stdout,result.stdout+result.stderr
    report=dict(status='PASS',scope=__doc__,catalog_sha256=catalog['catalog_sha256'],schema_id=catalog['schema_id'],
        boards=outcomes,total_bits=sum(r['bits'] for r in outcomes),runs=sum(len(b['runs']) for b in catalog['boards']),
        failure_controls=controls,formatted_rtl_packets=24,rtl_result=result.stdout.strip(),
        source_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [Path(__file__),Path(fmt.__file__),HERE/'test_board_state_serial.sv',*[HERE/n for n in names],ethernet]})
    (out/'results.json').write_text(json.dumps(report,indent=2)+'\n');print('SERIAL FORMAT PASS',report['schema_id'],report['total_bits'],'bits;',report['runs'],'runs;24 formatted packets through real RTL')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve())
