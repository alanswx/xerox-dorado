#!/usr/bin/env python3
"""Check falling-edge old-word preservation at the actual native group boundary.

On an accepted write, waiting is false, so cache_data cannot be changed by a
response on that edge. Capture it at the following falling edge. The old word
is consumed only after a later write acknowledgement; no extra request cycle
or original machine/DRAM capture is added. Native STA and MSA proof remain.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]
GROUP='src/fpga/mister/dorado_storage_group.sv'
GROUP_SHA='027bbe93b206a26b116397a1a8d3f24d85ac3af2d67cc6786cc48e8e975308d1'


def sha(b):return hashlib.sha256(b).hexdigest()


def transform(raw):
    assert sha(raw)==GROUP_SHA
    text=raw.decode();old='                if(req_write)old_data<=cache_data;'
    assert text.count(old)==1
    text=text.replace(old,'                // Old-word preservation is captured on the next falling edge.')
    old='    // These are cell_MK4096P_6\'s cycles, on the paused machine clock.'
    assert text.count(old)==1
    text=text.replace(old,'''    // The accepted write excludes a simultaneous group response (waiting
    // must be false). cache_data therefore retains its accepted old value
    // through the following falling edge, before any write ACK can arrive.
    // Move this transport-only capture away from machine rising captures.
    reg preserve_old_word=0;
    always @(posedge host_clk)
        preserve_old_word<=req_valid && req_ready && req_write;
    always @(negedge host_clk)
        if(preserve_old_word)old_data<=cache_data;

''' +old)
    return text.encode()


def run(parent,out):
    assert not out.exists()
    recipe=json.loads((parent/'timing-paths-native.json').read_text())
    raw=(parent/GROUP).read_bytes();assert sha(raw)==recipe['source_hashes'][GROUP]==GROUP_SHA
    out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
    (inputs/'reference.sv').write_bytes(raw.replace(b'module dorado_storage_group #(',b'module reference_storage_group #('))
    (inputs/'dorado_storage_group.sv').write_bytes(transform(raw))
    for target,source in [('addr.v','src/verilog/cells/cell_MK4096P_addr.v'),
                          ('preview.sv','src/fpga/mister/dorado_storage_addr_preview.sv')]:
        b=(parent/source).read_bytes();assert sha(b)==recipe['source_hashes'][source]
        (inputs/target).write_bytes(b)
    tb=(ROOT/'verilog/verilator/tb_storage_ready_prediction.sv').read_text()
    tb=tb.replace('module tb_storage_ready_prediction #(parameter integer WIDTH=12);',
                  'module tb_storage_ready_prediction #(parameter integer WIDTH=12, FAST_RESPONSE=0);')
    tb=tb.replace('reference_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH)) gold(',
                  'reference_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH),.HOST_ADDR_SAMPLE(1)) gold(')
    tb=tb.replace('dorado_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH)) dut(',
                  'dorado_storage_group #(.ADDR_BITS(16),.WIDTH(WIDTH),.HOST_ADDR_SAMPLE(1)) dut(')
    old='.host_clk(clk),.machine_clk(machine_clk),.reset(reset),.ras_n(ras_n)'
    assert tb.count(old)==1
    tb=tb.replace(old,'.host_clk(clk),.machine_clk(machine_clk),.reset(reset),.machine_step(run),.addr_after_edge(after_addr),.ras_n(ras_n)')
    tb=tb.replace(',gold.old_data}', '}').replace(',dut.old_data}', '}')
    old='pending<=1;latency<=1+int\'(rng[13:10]);saved<=gw ? gd : memory_[ga];'
    assert tb.count(old)==1
    tb=tb.replace(old,old+'''\n    if(FAST_RESPONSE)begin rsp_valid<=1;rsp_data<=gw ? gd : memory_[ga];end''')
    tb=tb.replace('  // Explicit FPGA-zero power-on fixture', '''  // An invalid internal old-word value may differ until the falling edge.
  // Its first original machine/DRAM consumer must see the same saved word.
  always @(posedge machine_clk)if(gold.write_pending && gold.write_done)
    if(gold.old_data!==dut.old_data)$fatal(1,"OLD_WORD_FALLING consumed old word differs");
  // Explicit FPGA-zero power-on fixture''') if '  // Explicit FPGA-zero power-on fixture' in tb else tb.replace(
        ' // Explicit FPGA-zero power-on fixture', ''' always @(posedge machine_clk)if(gold.write_pending && gold.write_done)
  if(gold.old_data!==dut.old_data)$fatal(1,"OLD_WORD_FALLING consumed old word differs");
 // Explicit FPGA-zero power-on fixture''')
    assert 'OLD_WORD_FALLING consumed' in tb
    (inputs/'tb.sv').write_text(tb)
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    frozen={p.name:sha(p.read_bytes()) for p in inputs.iterdir()}
    (out/'provenance.json').write_text(json.dumps(dict(scope=__doc__,inputs=frozen,
        parent_manifest_sha256=sha((parent/'timing-paths-native.json').read_bytes()),
        original_group_sha256=GROUP_SHA,old_word_moves_half_cycle=True,
        architectural_capture_cycles_changed=False,new_transport_control_bits_per_group=1,
        native_qualified=False),indent=2)+'\n')
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),actions=[],results={})

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    def execute(label,cmd):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(list(map(str,cmd)),cwd=inputs,stdout=log,stderr=subprocess.STDOUT)
            a=dict(label=label,pid=p.pid,command=list(map(str,cmd)),started=time.time());state['actions'].append(a);save()
            a.update(returncode=p.wait(),finished=time.time());save()
        return a['returncode'],(out/(label+'.log')).read_text()

    def case(engine,width,fast,source=None,negative=False):
        name=engine+'-w'+str(width)+'-fast'+str(fast)+('-bad' if negative else '')
        sources=[inputs/'reference.sv',source or inputs/'dorado_storage_group.sv',inputs/'addr.v',inputs/'preview.sv',inputs/'tb.sv']
        if engine=='icarus':
            exe=out/(name+'.vvp');cmd=['iverilog','-g2012','-s','tb_storage_ready_prediction',
                '-Ptb_storage_ready_prediction.WIDTH='+str(width),'-Ptb_storage_ready_prediction.FAST_RESPONSE='+str(fast),'-o',exe,*sources];launch=['vvp',exe]
        else:
            obj=out/('obj-'+name);cmd=['verilator','--binary','--timing','-j','2','-Wno-fatal','-Werror-UNOPTFLAT',
                '--top-module','tb_storage_ready_prediction','-GWIDTH='+str(width),'-GFAST_RESPONSE='+str(fast),
                '--Mdir',obj,'-o','test-old-word',*sources];launch=[obj/'test-old-word']
        rc,log=execute(name+'-compile',cmd);assert rc==0 and 'UNOPTFLAT'not in log,(name,log[-2000:])
        rc,log=execute(name+'-test',launch)
        if negative:
            assert rc!=0 and ('STORAGE_READY_PREDICTION' in log or 'OLD_WORD_FALLING' in log),(name,log[-2000:])
            state['results'][name]=dict(rejected=True,diagnostic=log)
        else:
            assert rc==0 and 'STORAGE_READY_GROUP PASS' in log,(name,log[-2000:])
            state['results'][name]=dict(passed=True,output=log)
        save();print(name,'rejected'if negative else log.splitlines()[0],flush=True)

    save()
    try:
        for engine in ['icarus','verilator']:
            for width in [2,8,12,16]:
                for fast in [0,1]:case(engine,width,fast)
        bad=out/'new-word-instead-of-old.sv';bad.write_bytes(transform(raw).replace(b'if(preserve_old_word)old_data<=cache_data;',b'if(preserve_old_word)old_data<=saved_data;'))
        for engine in ['icarus','verilator']:case(engine,16,1,bad,True)
        assert all(sha((inputs/n).read_bytes())==h for n,h in frozen.items())
        state.update(status='PASS_OLD_WORD_FALLING_GROUP_TEMPORAL_TWO_SIMULATORS',finished=time.time(),inputs_unchanged=True,physical_MSA_native_qualified=False);save()
    except BaseException as e:
        state.update(status='TERMINAL_FAILED',finished=time.time(),error=repr(e));save();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();run(a.parent.resolve(),a.output.resolve())
