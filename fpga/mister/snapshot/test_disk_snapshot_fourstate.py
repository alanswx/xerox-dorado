#!/usr/bin/env python3
"""Four-state admission, held RAM, release and lifecycle revocation of disk pause."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,once
from test_disk_cache_phase import normalized


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    raw=(ROOT/'fpga/mister/dorado_pack_hps.sv').read_text()
    bench=Path(__file__).with_name('test_disk_snapshot_barrier.sv').read_text()
    cases=[('candidate',None,None),
        ('guest_not_held',"(snapshot_guest_held===1'b1)","1'b1"),
        ('ignore_host_ack',"(sd_ack===1'b0)","1'b1"),
        ('unheld_ram','    if (!snapshot_block) begin\n    sd_buff_din',"    if (1'b1) begin\n    sd_buff_din"),
        ('early_release',"if ((snapshot_hold===1'b0) && (sd_ack===1'b0) && (sd_buff_wr===1'b0)) snapshot_paused <= 0;","if (snapshot_hold===1'b0) snapshot_paused <= 0;"),
        ('ignore_media_identity','(attached===snapshot_attached)',"1'b1"),
        ('nonsticky_ram_hold','(snapshot_paused || snapshot_enter)','snapshot_enter')]
    results=[]
    for label,before,after in cases:
        case=out/label;case.mkdir();text=raw
        if before:
            assert before in text;text=text.replace(before,after)
        files={'medium.sv':normalized(text),'fixture.sv':bench}
        for n,s in files.items():(case/n).write_text(s)
        cmd=['iverilog','-g2012','-s','tb','-o',str(case/'test.vvp'),*[str(case/n)for n in files]]
        (case/'commands.json').write_text(json.dumps([cmd,['vvp',str(case/'test.vvp')]],indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run(['vvp',str(case/'test.vvp')],capture_output=True,text=True,timeout=40)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if before:assert result.returncode and'FATAL'in text,text
        else:assert not result.returncode and'DISK PAUSE FOURSTATE PASS'in text,text
        print(label+': '+text.strip(),flush=True)
        results.append(dict(case=label,returncode=result.returncode,inputs={n:hashlib.sha256(s.encode()).hexdigest()for n,s in files.items()}))
    inputs=out/'executed-inputs';inputs.mkdir();(inputs/'medium.sv').write_text(raw)
    for p in(Path(__file__),Path(__file__).with_name('test_disk_snapshot_barrier.sv')):(inputs/p.name).write_bytes(p.read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PASS',cases=results,new_required_wrapper_bits=2,
        inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Disk lease only; real global quarantine and Main/HPS lifecycle ordering still require coordinator integration.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
