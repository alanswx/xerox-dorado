#!/usr/bin/env python3
"""Check the frozen native BL8 source at66.7MHz against physical SDRAM models.

Retain every original pin-bench assertion, including reset/drain, backpressure,
both ranks' refresh and tOH. Select the actual native overlap/capture/early-read
options. The pin bench uses26 address bits for both physical ranks; native has
23, so this is broader physical-port coverage, not a full native boot test.
No source, clock, limits or running job in an existing stage is changed.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BACKEND='src/fpga/mister/dorado_storage_sdram_burst.sv'


def run(stage,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['frequency_hz']==66666667 and m['sysper']==4
    native=module((stage/'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv').read_text(),'dorado_native_machine_ce')
    actual='dorado_storage_sdram_burst #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ),.CACHE_LINES(4),.RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(1)) u_sdram('
    assert native.count(actual)==1 and 'localparam integer GROUPS=128,RAW_BITS=23;'in native
    assert 'localparam integer STORAGE_CLOCK_HZ=`DORADO_HOST_CLOCK_HZ;'in native
    assert 'VERILOG_MACRO "DORADO_HOST_CLOCK_HZ=66666667"'in(stage/'core/Dorado.qsf').read_text()
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    shutil.copy2(stage/BACKEND,inputs/'burst.sv')
    for name,source in [('baseline.sv','fpga/mister/dorado_storage_sdram.sv'),
                        ('model.sv','verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv')]:
        shutil.copy2(ROOT/source,inputs/name)
    original=(ROOT/'verilog/verilator/tb_storage_sdram_burst.sv').read_text()
    before='.CACHE_LINES(CACHE_LINES)) dut('
    after='.CACHE_LINES(CACHE_LINES),.RESPONSE_OVERLAP(1),.CAPTURE_READ_WORD(1),.EARLY_READ_RESPONSE(1)) dut('
    assert original.count(before)==1
    bench=original.replace(before,after,1)
    assert bench.replace(after,before,1)==original and bench.count('$fatal')==original.count('$fatal')
    (inputs/'tb.sv').write_text(bench)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
               stage_manifest_sha256=sha(stage/'r-front-control-native.json'),native_backend_sha256=sha(stage/BACKEND),
               native_address_bits=23,pin_bench_address_bits=26,
               native_options={'RESPONSE_OVERLAP':1,'CAPTURE_READ_WORD':1,'EARLY_READ_RESPONSE':1,'CACHE_LINES':4},
               original_assertions_retained=original.count('$fatal'),cases=[],
               files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for name,hz,delay,mutation in [('clock50',50000000,0,None),('clock66',66666667,0,None),('clock66-skew6',66666667,6,None),
                                  ('clock66-early-capture',66666667,6,'early'),('clock66-wrong-frequency-binding',66666667,0,'frequency')]:
        d=out/name;d.mkdir();backend=(inputs/'burst.sv').read_text();tb=bench
        if mutation=='early':
            old='delay_count<=2;state<=READ_WAIT;';new='delay_count<=1;state<=READ_WAIT;'
            assert backend.count(old)==1;backend=backend.replace(old,new,1)
        elif mutation=='frequency':
            old='.CLOCK_HZ(CLOCK_HZ),.CACHE_LINES(CACHE_LINES)'
            assert tb.count(old)==1;tb=tb.replace(old,'.CLOCK_HZ(50000000),.CACHE_LINES(CACHE_LINES)',1)
        (d/'burst.sv').write_text(backend);(d/'tb.sv').write_text(tb)
        command=['verilator','--binary','--timing','--autoflush','-j','2','--top-module','tb_storage_sdram_burst',
                 '-Wno-fatal','-Wno-TIMESCALEMOD','-Wno-WIDTHTRUNC','-Wno-WIDTHEXPAND','-Wno-REALCVT','-Wno-PINMISSING','-Wno-DEFPARAM',
                 '+define+DORADO_CORRECT_READ_CAPTURE','-GCLOCK_HZ='+str(hz),'-GCLOCK_DELAY_NS='+str(delay),
                 '--Mdir',str(d/'obj'),'-o','test',str(inputs/'baseline.sv'),str(d/'burst.sv'),str(d/'tb.sv'),str(inputs/'model.sv')]
        case=dict(name=name,clock_hz=hz,clock_delay_ns=delay,negative=mutation,actions=[]);state['cases'].append(case);save()
        for label,cmd in [('compile',command),('test',[str(d/'obj/test')])]:
            with(d/(label+'.log')).open('x')as log:
                child=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=child.pid,command=cmd,started=time.time())
                case['actions'].append(a);save();code=child.wait()
            a.update(returncode=code,finished=time.time(),log_sha256=sha(d/(label+'.log')));save()
            if code:break
        raw=(d/'test.log').read_text()if(d/'test.log').exists()else''
        assert case['actions'][0]['returncode']==0,(name,(d/'compile.log').read_text()[-1200:])
        metrics=re.findall(r'^STORAGE-SDRAM-BURST PASS .*$',raw,re.M)
        if mutation=='early':passed=code!=0 and 'read 'in raw
        elif mutation=='frequency':passed=code!=0 and any(k in raw.lower()for k in ['power','initial','200','independent physical chip-model error'])
        else:passed=code==0 and len(metrics)==1
        case.update(passed=passed,metrics=metrics,returncode=code);save()
        if not passed:
            state.update(status='TERMINAL_NATIVE_SDRAM_PIN_CHECK_NOT_ESTABLISHED',finished=time.time());save()
            raise AssertionError((name,raw[-1800:]))
    state.update(status='PASS_NATIVE_SOURCE_SDRAM_66MHZ_PIN_MODEL',finished=time.time(),
                 full_native_CDC_physical_STA_or_hardware_qualified=False)
    save();print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['stage','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();run(a.stage.resolve(),a.output.resolve())
