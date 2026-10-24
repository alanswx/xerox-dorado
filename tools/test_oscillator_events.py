#!/usr/bin/env python3
"""Qualify ordered crystal events and expose the actual native pixel-rate limit.

Bind to the exact native wrapper/cell/DispY wiring. Compare against independent
absolute-time transition counts, ordered polarity, endpoint coincidence and
pause/reset state in two simulators. These events are not yet consumed by the
native display graph: no machine clocks, timer bindings or hardware change.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]
RTL='verilog/rtl/dorado_oscillator_events.sv'
TB='verilog/verilator/tb_oscillator_events.sv'
BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
DISPLAY='src/fpga/mister/snapshot_native_dispy/DispY_full_snapshot.sv'
CELL='src/verilog/cells/cell_K1115A.v'


def sha(path):
    import hashlib
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(stage,out):
    assert not out.exists()
    p=json.loads((stage/'r-front-control-native.json').read_text())
    for name,digest in p['source_hashes'].items():assert sha(stage/name)==digest,name
    wrapper=(stage/WRAPPER).read_text();bp=(stage/BP).read_text();display=(stage/DISPLAY).read_text()
    assert 'cell_K1115A #(.FREQ_KHZ(50000),.SYSPER(2*SYSPER)) u_pixclk'in wrapper
    assert '.RawPixelClk(pixel_clk)'in wrapper and '.Crystal('not in wrapper
    assert 'DispY_full_snapshot_experiment b_DispY ('in bp
    assert 'assign Crystal = Crystal__DispY;'in bp
    assert '.Crystal__drv(Crystal__DispY)'in bp
    assert 'cell_K1115A_snapshot #(.FREQ_KHZ(50000)) u_a05 ('in display
    assert '.p8(DispY25_sil_pl_1)'in display
    assert '.p4(Crystal__drv)'in display
    b01=re.search(r'cell_MC10102 u_b01 \(.*?\n  \);',display,re.S)[0]
    assert '.p7(RawPixelClk)'in b01
    assert len(re.findall(r'\bPixelClkVCO\b',display))==1
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    paths={Path(RTL).name:ROOT/RTL,Path(TB).name:ROOT/TB,'cell_K1115A.v':stage/CELL,
           'native-wrapper.sv':stage/WRAPPER,'native-backplane.v':stage/BP,'native-display.sv':stage/DISPLAY,
           'native-manifest.json':stage/'r-front-control-native.json',Path(__file__).name:Path(__file__)}
    for name,path in paths.items():(inputs/name).write_bytes(path.read_bytes())
    rtl=(inputs/Path(RTL).name).read_text()
    variants={
        'positive':rtl,
        'drop-second':rtl.replace("assign edge_count=active ? due : 2'd0;", "assign edge_count=active ? (due==2 ? 2'd1 : due) : 2'd0;",1),
        'reverse-order':rtl.replace("assign rising_first=(edge_count!=0) && !level_before;", "assign rising_first=(edge_count!=0) && level_before;",1),
        'advance-while-held':rtl.replace('else if(advance)begin remainder<=', "else if(1'b1)begin remainder<=",1),
        'late-offset':rtl.replace("assign first_offset_units=(edge_count!=0) ? 32'(MODULUS)-32'(remainder) : 32'd0;",
                                  "assign first_offset_units=(edge_count!=0) ? 32'(STEP) : 32'd0;",1),
    }
    assert len(set(variants.values()))==5
    for name,text in variants.items():(inputs/(name+'.sv')).write_text(text)
    (inputs/'invalid.sv').write_text('''module tb_invalid;
wire [1:0] n;
dorado_oscillator_events #(.SAMPLES_PER_INSTRUCTION(2),.FREQUENCY_KHZ(50000)) invalid(
.clk(1'b0),.reset(1'b0),.advance(1'b1),.edge_count(n));
initial begin #1;$fatal(1,"Missing excessive-rate rejection");end
endmodule
''')
    state=dict(status='RUNNING',scope=__doc__,driver_pid=os.getpid(),started=time.time(),actions=[],cases=[],
               native_manifest_sha256=sha(stage/'r-front-control-native.json'),
               source_bindings=dict(active_pixel='Wrapper50MHz K1115A, samples=2*boardSYSPER, RawPixelClk to DispY b01.p7',
                   legacy_crystal='DispY a05/default16 drives Crystal connector; wrapper does not connect it',
                   PixelClkVCO='Declared unused on this DispY revision'),
               inputs={name:sha(path)for name,path in ((q.name,q)for q in inputs.iterdir())},
               events_connected_to_native_consumers=False,physical_timing_qualified=False,
               firmware_qualified=False,speedup_qualified=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def execute(label,command):
        with(out/(label+'.log')).open('x')as log:
            process=subprocess.Popen(list(map(str,command)),stdout=log,stderr=subprocess.STDOUT)
            action=dict(label=label,pid=process.pid,command=list(map(str,command)),started=time.time())
            state['actions'].append(action);save();code=process.wait()
            action.update(returncode=code,finished=time.time());save()
        return code
    save()
    try:
        for engine in ['icarus','verilator']:
            for name in [*variants,'native-square','invalid-rate']:
                label=engine+'-'+name
                top='tb_native_square_wave'if name=='native-square'else'tb_invalid'if name=='invalid-rate'else'tb_oscillator_events'
                sources=[inputs/'cell_K1115A.v',inputs/Path(TB).name]if name=='native-square'else \
                    [inputs/'positive.sv',inputs/'invalid.sv']if name=='invalid-rate'else [inputs/(name+'.sv'),inputs/Path(TB).name]
                if engine=='icarus':
                    binary=out/(label+'.vvp')
                    command=['iverilog','-g2012','-s',top,'-o',binary,*sources]
                    test=['vvp',binary]
                else:
                    obj=out/('obj-'+name)
                    command=['verilator','--binary','--timing','-j','2','-Wno-fatal','-Wno-PINMISSING',
                             '-Wno-TIMESCALEMOD','--top-module',top,'--Mdir',obj,'-o','test',*sources]
                    test=[obj/'test']
                assert execute(label+'-compile',command)==0,label+' compile'
                code=execute(label+'-test',test);text=(out/(label+'-test.log')).read_text()
                if name=='positive':
                    assert code==0 and 'OSC_EVENTS_ALL_PASS cases=12'in text
                    rows=re.findall(r'OSC_EVENTS_PASS samples=(\d+) frequency=(\d+) single=(\d+) double=(\d+) paused=(\d+) endpoint=(\d+) epochs=(\d+)',text)
                    assert len(rows)==12
                    row=[r for r in rows if r[0:2]==('4','50000')][0]
                    assert int(row[3])>0 and int(row[5])>0
                elif name=='native-square':
                    assert code==0 and 'OSC_NATIVE_CAPABILITY_PASS'in text
                    assert 'samples=4 expected_edges=6000 actual_edges=4000'in text
                    assert re.search(r'samples=8 expected_edges=6000 actual_edges=600[01] remainder_escapes=0',text)
                elif name=='invalid-rate':
                    assert code!=0 and 'OSC_EVENTS unsupported interval'in text
                else:
                    assert code!=0 and re.search(r'OSC_EVENTS (count/state|edge order|committed phase|endpoint coincidence|edge timestamp)',text)
                state['cases'].append(dict(engine=engine,case=name,returncode=code,expected_rejection=name not in ['positive','native-square'],
                                           log_sha256=sha(out/(label+'-test.log'))));save()
        state.update(status='PASS_ORDERED_OSCILLATOR_EVENTS_AND_NATIVE_RATE_COUNTEREXAMPLE',finished=time.time())
    except BaseException:
        state.update(status='TERMINAL_OSCILLATOR_EVENT_TEST_FAILED',finished=time.time());save();raise
    save();print(state['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--stage',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.stage.resolve(),a.output.resolve())
