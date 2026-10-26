#!/usr/bin/env python3
"""Qualify reset-masked RIOT RAM before native integration.

Compare complete logical RAM and registered read behavior with the existing
bulk-clear semantics. Preserve four-state testing in Icarus, binary coverage
in Verilator and two deliberately wrong implementations with real mismatches.
The candidate remains unconnected; no native clock or firmware claim follows.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def run(out):
    assert not out.exists();out.mkdir(parents=True)
    inputs=out/'inputs';inputs.mkdir()
    for name,src in [('ram.sv','verilog/rtl/dorado_riot_ram.sv'),('tb.sv','verilog/verilator/tb_riot_ram.sv')]:
        shutil.copy2(ROOT/src,inputs/name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    original=(inputs/'ram.sv').read_text()
    variants=[('positive',None),('missing-reset-mask',('read_written ? read_payload : 8\'b0','read_payload')),
              ('write-before-read',('if(read_enable) read_payload<=backing[address];',
                                    'if(read_enable) read_payload<=write_enable ? write_data : backing[address];'))]
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,cases=[],
               production_helper_sha256=sha(ROOT/'verilog/rtl/dorado_riot_ram.sv'),
               bench_sha256=sha(ROOT/'verilog/verilator/tb_riot_ram.sv'),
               files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for engine,variant,mutation in [('icarus','positive',None),*[('verilator',n,m)for n,m in variants]]:
        d=out/(engine+'-'+variant);d.mkdir();raw=original
        if mutation:
            before,after=mutation;assert raw.count(before)==1;raw=raw.replace(before,after,1)
        (d/'ram.sv').write_text(raw)
        commands=([['iverilog','-g2012','-s','tb_riot_ram','-o',str(d/'test.vvp'),str(d/'ram.sv'),str(inputs/'tb.sv')],['vvp',str(d/'test.vvp')]]if engine=='icarus'else
                  [['verilator','--binary','--timing','--top-module','tb_riot_ram','-Wno-fatal','-j','2','--Mdir',str(d/'obj'),'-o','test',str(d/'ram.sv'),str(inputs/'tb.sv')],[str(d/'obj/test')]])
        case=dict(engine=engine,variant=variant,expected_negative=mutation is not None,helper_sha256=sha(d/'ram.sv'),actions=[])
        state['cases'].append(case);save()
        for label,cmd in zip(['compile','test'],commands):
            with(d/(label+'.log')).open('x')as log:
                child=subprocess.Popen(cmd,stdout=log,stderr=log)
                action=dict(label=label,pid=child.pid,command=cmd,started=time.time());case['actions'].append(action);save();code=child.wait()
            action.update(returncode=code,finished=time.time(),log_sha256=sha(d/(label+'.log')));save()
            if code:break
        log=(d/'test.log').read_text()if(d/'test.log').exists()else''
        assert case['actions'][0]['returncode']==0
        if mutation:
            passed=code!=0 and 'RIOT_RAM read mismatch'in log
        else:
            metric=re.findall(r'^RIOT_RAM_PASS .*$',log,re.M)
            passed=code==0 and len(metric)==1 and ('checks=41285'if engine=='icarus'else'checks=40261')in metric[0]
            case['metrics']=metric
        case.update(passed=passed,returncode=code);save()
        assert passed,(engine,variant,log[-1200:])
    state.update(status='PASS_RESET_MASKED_RIOT_BLOCK_RAM_COMPONENT',finished=time.time(),
                 four_state_checked=True,positive_engines=2,genuine_negative_implementations=2,
                 native_integration_or_physical_RAM_inference_or_firmware_qualified=False)
    save();print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.output.resolve())
