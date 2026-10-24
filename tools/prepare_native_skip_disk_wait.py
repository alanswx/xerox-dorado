#!/usr/bin/env python3
"""Freeze a native simulation with the period SkipDiskWait' jumper fitted.

Ground only MCS6532 PB1, the documented active-low backpanel input. Keep every
EPROM byte, guest/storage clock, CPU/memory model, pack and normal boot-button
sequence. Add passive CP timing records. This changes the physical environment
of a new experiment; never extend or restart the original bounded runs.
"""
import argparse
import gzip
import json
from pathlib import Path
import time

from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BASEBOARD='src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
TB='harness/tb_native_cedar.sv'
PROGRAMS=['doradoreset.masm','doradoio.mdefs','doradocpint.masm']
OBSERVER='''
// Passive CP observer. It does not drive guest, device or transport state.
wire cp_observe_clock=dut.m.b_ContA.u_j02.p9;
wire [5:0] cp_observe_data={dut.m.b_ContA.u_j02.p12,dut.m.b_ContA.u_j02.p11,
 dut.m.b_ContA.u_j02.p10,dut.m.b_ContA.u_j02.p7,dut.m.b_ContA.u_j02.p6,dut.m.b_ContA.u_j02.p5};
wire cp_observe_mask=dut.m.b_ContA.Stop|dut.m.b_ContA.Run_p_;
reg cp_observe_previous=0,cp_observe_root=0;
longint unsigned cp_observe_writes=0,cp_observe_changes=0,cp_observe_running=0,cp_observe_collisions=0;
longint unsigned cp_observe_words[0:63];
initial begin
 for(integer i=0;i<64;i++)cp_observe_words[i]=0;
 $display("NATIVE_SKIP_DISK_WAIT input=PB1 level=0 original_ROM=unchanged");
end
always @(posedge machine_clk) begin
 if(cp_observe_clock && !cp_observe_previous)begin
  cp_observe_writes=cp_observe_writes+1;
  cp_observe_words[cp_observe_data]=cp_observe_words[cp_observe_data]+1;
  if(cp_observe_data[5]!=dut.m.b_ContA.NoDispatch)begin
   cp_observe_changes=cp_observe_changes+1;
   if(!cp_observe_mask)cp_observe_running=cp_observe_running+1;
   if(dut.m.CLK_InBase!=cp_observe_root)cp_observe_collisions=cp_observe_collisions+1;
  end
  if(cp_observe_writes<=1024 || (cp_observe_data[5]!=dut.m.b_ContA.NoDispatch && !cp_observe_mask))
   $display("NATIVE_CP_WRITE host=%0d guest=%0d pc6502=%h data=%h NoDispatch=%b next_NoDispatch=%b instr_mask=%b root=%b old_root=%b cpu=%h",
    host_ticks,guest_ticks,dut.m.b_BaseBd.u_f61.u_cpu.logic_00.pc,cp_observe_data,
    dut.m.b_ContA.NoDispatch,cp_observe_data[5],cp_observe_mask,dut.m.CLK_InBase,cp_observe_root,cpu_debug);
 end
 cp_observe_previous<=cp_observe_clock;cp_observe_root<=dut.m.CLK_InBase;
end
always @(posedge host_clk)if((host_ticks%10000000)==0)
 $display("NATIVE_CP_PROGRESS host=%0d pc6502=%h writes=%0d NoDispatch_changes=%0d unmasked_changes=%0d root_collisions=%0d",
  host_ticks,dut.m.b_BaseBd.u_f61.u_cpu.logic_00.pc,cp_observe_writes,cp_observe_changes,cp_observe_running,cp_observe_collisions);
'''


def prepare(source,out):
    assert not out.exists()
    state=json.loads((source/'run-state.json').read_text())
    assert state['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC' and state['actions'][-1]['returncode']==0
    assert int((source/'host-cycle-limit.txt').read_text())==5000000000
    r=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
    assert r['passive_storage_traffic_observer'] and r['passive_storage_window_observer']
    out.mkdir(parents=True);inputs=out/'inputs';records=[];hashes={}
    for n,h in r['inputs'].items():
        p=source/'inputs'/n;assert sha(p)==h,n
        raw=p.read_bytes()
        if n==BASEBOARD:
            old=b'1\'b0, TSetRun, SkipWait_p___drv, MCPStrb})'
            new=b'1\'b0, TSetRun, 1\'b0, MCPStrb})'
            assert raw.count(old)==1;raw=raw.replace(old,new)
        if n==TB:
            assert raw.count(b'endmodule')==1;raw=raw.replace(b'endmodule',OBSERVER.encode()+b'\nendmodule')
        q=inputs/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(raw);hashes[n]=sha(q)
        if h!=hashes[n]:records.append(dict(path=n,parent_sha256=h,output_sha256=hashes[n]))
    assert {x['path'] for x in records}=={BASEBOARD,TB}
    programs={}
    for name in PROGRAMS:
        p=ROOT/'chm/dorado/expanded/doradobaserom.dm!12_'/name
        q=inputs/'skip-wait-evidence'/name;q.parent.mkdir(exist_ok=True);q.write_bytes(p.read_bytes())
        n=str(q.relative_to(inputs));hashes[n]=sha(q);programs[str(p.relative_to(ROOT))]=sha(p)
    reset=(inputs/'skip-wait-evidence/doradoreset.masm').read_text()
    assert ";\tDelay X sec only if SkipDiskWait' is high." in reset
    assert "ANDI\tSkipDiskWait'" in reset
    driver=inputs/Path(__file__).name;driver.write_bytes(Path(__file__).read_bytes());hashes[driver.name]=sha(driver)
    # The existing host medium implementation opens the pack read-only and
    # keeps writes in its in-process vector; no original medium is modified.
    host=(inputs/'harness/host.cpp').read_text()
    assert 'fopen(medium,"rb")' in host and 'pack[word]=uint16_t(value)' in host
    assert 'fwrite' not in host
    (out/'host-cycle-limit.txt').write_bytes((source/'host-cycle-limit.txt').read_bytes())
    (out/'window-control.txt').write_bytes((source/'window-control.txt').read_bytes())
    r.update(status='PREPARED_NATIVE_SKIP_WAIT_INPUT_DIAGNOSTIC',scope=__doc__,prepared=time.time(),
             inputs=hashes,source_run=str(source),source_provenance_sha256=sha(source/'provenance.json.gz'),
             source_terminal_state_sha256=sha(source/'run-state.json'),physical_environment_changes=records,
             original_firmware_source_evidence=programs,skip_disk_wait_jumper_fitted=True,original_ROM_bytes_unchanged=True,
             original_clock_and_guest_models_unchanged=True,host_limit=5000000000,original_run_limits_changed=False,
             passive_CP_observer=True,medium_runtime_writes_persisted=False,firmware_qualified=False,
             physical_timing_qualified=False,hardware_changed=False,speedup_qualified=False,
             window_control_path=str(out/'window-control.txt'))
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    print(r['status'],len(hashes),'inputs; original5B bound retained',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.source.resolve(),a.output.resolve())
