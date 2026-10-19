#!/usr/bin/env python3
"""UIO and bounded board fabric qualification, no FPGA or guest state imports."""
import argparse,hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]


def run(out,ethernet):
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
 paths=[Path(__file__).resolve(),HERE/'test_board_state.sv',HERE/'board_state_fabric.sv',HERE/'board_state_uio.sv',ethernet]
 for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
 cmd=['iverilog','-g2012','-s','board_state_fixture','-o',str(out/'test.vvp'),*[str(inputs/n)for n in('test_board_state.sv','board_state_fabric.sv','board_state_uio.sv',ethernet.name)]]
 (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
 with(out/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
 r=subprocess.run(['vvp',str(out/'test.vvp')],capture_output=True,text=True,timeout=60);text=r.stdout+r.stderr;(out/'test.txt').write_text(text);print(text,end='');assert not r.returncode and'BOARD STATE PASS'in text,text
 (out/'results.json').write_text(json.dumps(dict(status='PASS',scope='Board-only diagnostic transport and stub-board routing, not machine/global snapshot; unchanged native59Ethernet coexistence',executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()}),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True);a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve())
