#!/usr/bin/env python3
"""Freeze/check passive first-error capture in both local simulators."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]


def run(out):
    assert not out.exists()
    inputs=out/'inputs';inputs.mkdir(parents=True)
    for name,source in [('monitor.sv','fpga/mister/dorado_boot_fault_monitor.sv'),
                        ('tb.sv','verilog/verilator/tb_boot_fault_monitor.sv'),
                        (Path(__file__).name,'tools/'+Path(__file__).name)]:
        (inputs/name).write_bytes((ROOT/source).read_bytes())
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(command,label,negative=False):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            row=dict(stage=label,pid=child.pid,command=command,started=time.time())
            state['actions'].append(row);save();code=child.wait()
            row.update(returncode=code,finished=time.time());save()
        text=(out/(label+'.log')).read_text()
        assert (code!=0 and 'BOOT-FAULT:'in text)if negative else code==0,(label,code,text[-1000:])
    save()
    try:
        execute(['iverilog','-g2012','-s','tb_boot_fault_monitor','-o',str(out/'positive.vvp'),str(inputs/'monitor.sv'),str(inputs/'tb.sv')],'icarus-compile')
        execute(['vvp',str(out/'positive.vvp')],'icarus-positive')
        source=(inputs/'monitor.sv').read_text()
        for label,old,new in [
            ('premature-arm',"if(error_n && sources[15:10]==6'b111111)","if(sources[15:10]==6'b111111)"),
            ('late-source','first_sources<=sources;','first_sources<=previous_sources;'),
            ('overwrite-first','if(!valid)begin',"if(1'b1)begin")]:
            assert source.count(old)==1
            path=inputs/(label+'.sv');path.write_text(source.replace(old,new))
            execute(['iverilog','-g2012','-s','tb_boot_fault_monitor','-o',str(out/(label+'.vvp')),str(path),str(inputs/'tb.sv')],label+'-compile')
            execute(['vvp',str(out/(label+'.vvp'))],label+'-test',True)
        execute(['verilator','--binary','--timing','-Wno-TIMESCALEMOD','--top-module','tb_boot_fault_monitor','--Mdir',str(out/'obj'),'-o','boot-fault',str(inputs/'monitor.sv'),str(inputs/'tb.sv')],'verilator-compile')
        execute([str(out/'obj/boot-fault')],'verilator-positive')
        state.update(status='PASS_PASSIVE_BOOT_FAULT_MONITOR',finished=time.time())
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
    (out/'provenance.json').write_text(json.dumps(dict(inputs={str(p.relative_to(inputs)):hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},scope=__doc__),indent=2)+'\n')
    print(state['status'],flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    run(parser.parse_args().output.resolve())
