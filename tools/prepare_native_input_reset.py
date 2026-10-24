#!/usr/bin/env python3
"""Freeze a fresh native boot test with reset-correct terminal input adapters.

The original bounded jobs remain untouched. Replace only the keyboard and
terminal utility modules, after both simulators pass the stopped-clock reset
test and independent negative controls. Retain original firmware, clocks,
normal C/button stimulus, memory hardware model and read-only host medium.
"""
import argparse
import gzip
import json
from pathlib import Path
import time

from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
TB='harness/tb_native_cedar.sv'
OBSERVER='''
// Passive input-reset checks; no original board or guest state is driven.
initial begin
 wait(!reset);repeat(2)@(negedge host_clk);
 if ({dut.kw0,dut.kw1,dut.kw2,dut.kw3} !== 64'hffffffffffffffff)
  $fatal(1,"Native input reset matrix %h %h %h %h",dut.kw0,dut.kw1,dut.kw2,dut.kw3);
 if(dut.u_terminal.msg !== 32'h81ffff80 || dut.u_terminal.msg_bit !== 0)
  $fatal(1,"Native input reset terminal %h",dut.u_terminal.msg);
 $display("NATIVE_INPUT_RESET_PASS host=%0d guest=%0d matrix=ffff/ffff/ffff/ffff terminal=%h",
  host_ticks,guest_ticks,dut.u_terminal.msg);
end
always @(posedge host_clk)if((host_ticks%10000000)==0)
 $display("NATIVE_INPUT_PROGRESS host=%0d guest=%0d raw=%h queued=%0d event=%h matrix=%h/%h/%h/%h",
  host_ticks,guest_ticks,ps2_key,key_queued,machine_key,dut.kw0,dut.kw1,dut.kw2,dut.kw3);
'''


def prepare(source,proof,synthesis,out):
    assert not out.exists()
    p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_STOPPED_INPUT_RESET_WITH_NEGATIVE_CONTROLS'
    assert len(p['cases'])==16
    synth=json.loads((synthesis/'run-state.json').read_text())
    assert synth['status']=='PASS_INPUT_ADAPTER_SYNTHESIS'and synth['returncode']==0
    synth_inputs=json.loads((synthesis/'inputs.json').read_text())
    for name,h in synth_inputs.items():assert sha(synthesis/name)==h,name
    for engine in ['icarus','verilator']:
        cases=[v for v in p['cases']if v['engine']==engine]
        assert len(cases)==8
        assert sum(v['expected_rejection']for v in cases)==4
        for case in cases:
            assert (case['test_returncode']!=0)==case['expected_rejection']
    r=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    assert r['skip_disk_wait_jumper_fitted'] and r['original_ROM_bytes_unchanged']
    assert int((source/'host-cycle-limit.txt').read_text())==5000000000
    assert r['passive_storage_traffic_observer']and r['passive_storage_window_observer']
    replacements={}
    for name in ['dorado_keyboard.v','dorado_terminal.v']:
        target=ROOT/'verilog/cells'/name
        assert sha(target)==p['inputs'][name]==sha(proof/'inputs'/name)
        assert sha(target)==synth_inputs[name]
        assert 'always @(posedge sys_clk or posedge reset)'in target.read_text()
        replacements['src/verilog/cells/'+name]=target
    out.mkdir(parents=True);inputs=out/'inputs';hashes={};changes=[]
    for name,h in r['inputs'].items():
        parent=source/'inputs'/name;assert sha(parent)==h,name
        raw=parent.read_bytes()
        if name in replacements:
            assert b'always @(posedge sys_clk) begin'in raw
            raw=replacements[name].read_bytes()
        if name==TB:
            assert raw.count(b'endmodule')==1
            raw=raw.replace(b'endmodule',OBSERVER.encode()+b'\nendmodule')
        target=inputs/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
        hashes[name]=sha(target)
        if hashes[name]!=h:changes.append(dict(path=name,parent_sha256=h,output_sha256=hashes[name]))
    assert {v['path']for v in changes}==set(replacements)|{TB}
    host=(inputs/'harness/host.cpp').read_text()
    assert 'fopen(medium,"rb")'in host and 'fwrite'not in host
    for name in ['host-cycle-limit.txt','window-control.txt']:
        (out/name).write_bytes((source/name).read_bytes())
    (inputs/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    hashes[Path(__file__).name]=sha(inputs/Path(__file__).name)
    r.update(status='PREPARED_NATIVE_STOPPED_INPUT_RESET_DIAGNOSTIC',scope=__doc__,prepared=time.time(),
             inputs=hashes,source_run=str(source),source_provenance_sha256=sha(source/'provenance.json.gz'),
             input_reset_proof=str(proof),input_reset_proof_sha256=sha(proof/'run-state.json'),
             input_reset_synthesis=str(synthesis),input_reset_synthesis_sha256=sha(synthesis/'run-state.json'),
             input_adapter_changes=changes,original_firmware_clocks_memory_and_boot_stimulus_unchanged=True,
             original_bounded_jobs_changed=False,medium_runtime_writes_persisted=False,
             host_limit=5000000000,hardware_changed=False,firmware_qualified=False,
             physical_timing_qualified=False,speedup_qualified=False,
             window_control_path=str(out/'window-control.txt'))
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    print(r['status'],len(hashes),'frozen inputs',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','proof','synthesis','output']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();prepare(args.source.resolve(),args.proof.resolve(),args.synthesis.resolve(),args.output.resolve())
