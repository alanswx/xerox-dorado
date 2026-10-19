#!/usr/bin/env python3
"""Serial transport/ownership contract, real longest scan, stub foreign domains.

No native wrapper/ARM quiesce, FPGA integration or complete snapshot claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

HERE=Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(out,ethernet,mutations):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    paths=[Path(__file__).resolve(),HERE/'test_board_state_serial.sv',HERE/'board_state_fabric_serial.sv',
           HERE/'board_state_uio_serial.sv',HERE/'ff_scan_controller.sv',ethernet]
    hashes={str(p):sha(p) for p in paths}
    for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
    cases=[('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('short_timeout','board_state_fabric_serial.sv','RESPONSE_LIMIT=2048','RESPONSE_LIMIT=16','result differs'),
            ('service_bypass','board_state_fabric_serial.sv',"(services_held==={SERVICE_DOMAINS{1'b1}})","1'b1",'missing service acknowledged'),
            ('board_bypass','board_state_fabric_serial.sv',"(board_frozen===11'h7ff)","1'b1",'request before complete boundary'),
            ('width_alias','board_state_fabric_serial.sv','0:index_valid=!(|index[23:12]);',"0:index_valid=1'b1;",'result differs'),
            ('owner_alias','board_state_fabric_serial.sv','owner<=board;',"owner<=0;",'result differs'),
            ('write_leak','board_state_fabric_serial.sv','board_error[owner] || was_write','board_error[owner]','result differs'),
            ('child_hold','board_state_fabric_serial.sv',' || children_holding || children_faulted',' || children_faulted','child hold not propagated'),
            ('abort','board_state_fabric_serial.sv','assign scan_abort=fault || children_faulted ||',"assign scan_abort=1'b0 && (fault || children_faulted ||",'lost boundary not quarantined'),
            ('transport_reset_owner','board_state_fabric_serial.sv','// A parser reset is never a thaw or recovery operation.',
             'command_hold<=0;fault<=0; // damaged parser reset releases owner',('transport reset released quarantine','owner allowed fault release','active transport reset lost ownership')),
            ('release_busy_owner','board_state_fabric_serial.sv','fault || children_faulted || pending || children_holding',
             'fault || children_faulted','owner released busy session'),
            ('busy_routing','board_state_fabric_serial.sv','boundary && !pending && !fault','boundary && !fault','busy transaction selected another child'),
            ('request_schema','board_state_uio_serial.sv',"6:if(din!=SCHEMA_ID[63:48])valid_packet<=0;",'6:begin end','bad schema accepted'),
            ('abi','board_state_uio_serial.sv',"2:if(din!=16'd2)valid_packet<=0;",'2:begin end','bad ABI accepted'),
            ('control_early','board_state_uio_serial.sv','7:begin\n       if(is_control)mode<=din[0];',
             '7:begin\n       if(is_control)begin mode<=din[0];hold_update<=1;hold_value<=din[0];end ',('long control accepted','bad key accepted')),
            ('request_short','board_state_uio_serial.sv','count==13','count>=12','short request accepted'),
            ('request_padding','board_state_uio_serial.sv',"if(is_control || |din[15:2])valid_packet<=0;",'if(is_control)valid_packet<=0;','request padding ignored'),
            ('status_schema','board_state_uio_serial.sv','15:dout<=SCHEMA_ID[63:48];',"15:dout<=16'b0;",'serial capability/schema differs'),
            ('torn_status','board_state_uio_serial.sv','7:dout<=result_snapshot[15:0];','7:dout<=last_result[15:0];','torn status result'),
            ('command_collision','board_state_uio_serial.sv',"command>=16'h76", "command>=16'h70",'EXT_BUS ownership collision'),
        ]
    records=[]
    for name,target,before,after,why in cases:
        case=out/name;case.mkdir();sources=[]
        for p in paths[1:]:
            raw=(inputs/p.name).read_bytes()
            if p.name==target:
                assert raw.count(before.encode())==1,(name,before)
                raw=raw.replace(before.encode(),after.encode(),1)
                if name=='abort':
                    before_end=b'(transport_reset && hold);';assert raw.count(before_end)==1
                    raw=raw.replace(before_end,b'(transport_reset && hold));',1)
                if name=='busy_routing':
                    needle=b'!children_holding && !children_faulted';assert raw.count(needle)==1
                    raw=raw.replace(needle,b'!children_faulted',1)
            q=case/p.name;q.write_bytes(raw);sources.append(q)
        command=['iverilog','-g2012','-s','board_state_serial_fixture','-o',str(case/'test.vvp'),*map(str,sources)]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
        print(name+': '+text.strip().splitlines()[0],flush=True)
        if why:assert result.returncode and any(x in text for x in ((why,) if isinstance(why,str) else why)),(name,text)
        else:assert result.returncode==0 and 'SERIAL BOARD STATE PASS' in text,text
        records.append(dict(case=name,returncode=result.returncode,expected_failure=why))
    assert all(sha(Path(p))==h for p,h in hashes.items()),'inputs changed during test'
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,source_hashes=hashes,cases=records),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve(),a.mutations)
