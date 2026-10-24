#!/usr/bin/env python3
"""Run a bounded cold-prefix diagnostic of the privately staged 844-Q slice.

Reuse exact original-ROM/native SDRAM/disk/network harness inputs. The disk
file is read-only and guest writes remain in a simulation-side vector. Add
only read-only slice monitors and a separate short host limit. Never alter
the two long-running storage comparisons or their inputs/limits.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from run_native_cedar_memory import run as native_run_test


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(stage,baseline,out,limit,monitor_baseboard=False,firmware_run=False):
    assert not out.exists()and 200000000<=limit<=(5000000000 if firmware_run else 1000000000)
    r=json.loads((stage/'r-front-control-native.json').read_text())
    assert r['status']=='STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT'and not r['assembly_permitted']
    for n,h in r['source_hashes'].items():assert sha(stage/n)==h,n
    original=json.loads(gzip.decompress((baseline/'provenance.json.gz').read_bytes()))
    for n,h in original['inputs'].items():assert sha(baseline/'inputs'/n)==h,n
    out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(baseline/'inputs',inputs)
    for n in r['source_hashes']:
        p=inputs/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((stage/n).read_bytes())
    host=(inputs/'harness/host.cpp').read_text()
    assert 'fopen(medium,"rb")'in host and 'pack[word]=uint16_t(value);'in host
    assert 'fwrite('not in host and 'ofstream'not in host
    monitor='''
  // Passive cold-prefix checks; never drives a guest pin or state bit.
  longint unsigned processor_samples=0,processor_root_edges=0,processor_cp_edges=0;
  reg processor_last_root=0,processor_last_cp=1;
  always @(negedge host_clk)if(!reset && step)begin
    processor_samples++;
    if(dut.m.CLK_ca_p_ && !processor_last_root)processor_root_edges++;
    if(dut.m.CPStrb_p_ != processor_last_cp)processor_cp_edges++;
    processor_last_root=dut.m.CLK_ca_p_;processor_last_cp=dut.m.CPStrb_p_;
    if(processor_samples>16)begin
      if(!dut.m.r_front_alias_ok)$fatal(1,"NATIVE844 live producer aliases disagree host=%0d",host_ticks);
      if(dut.m.r_front_unsettled || (|dut.m.r_front_repeated))
        $fatal(1,"NATIVE844 resolver unsettled/repeated host=%0d q=%h pins=%h repeat=%h",host_ticks,dut.m.r_front_q,dut.m.r_front_live,dut.m.r_front_repeated);
    end
    if(host_ticks+1>=limit)begin
      if(processor_samples<1000)$fatal(1,"Native844 prefix never advanced after physical initialization");
      $display("NATIVE844_PREFIX samples=%0d root_edges=%0d CP_edges=%0d no_guest_state_import=1",processor_samples,processor_root_edges,processor_cp_edges);
    end
  end
'''
    if monitor_baseboard:
        monitor+='''
  // Passive actual CPU PC/phase observation; bb_debug's bus address cannot
  // identify a 6502 instruction. Counter units are enabled machine samples.
  longint unsigned bb_phi_rises=0,bb_sync_rises=0;
  reg bb_last_phi=0,bb_last_sync=0;
  always @(negedge host_clk)if(!reset)begin
    if(step)begin
      if(dut.m.b_BaseBd.MCPreClk && !bb_last_phi)bb_phi_rises++;
      if(dut.m.b_BaseBd.u_f61.sync && !bb_last_sync)bb_sync_rises++;
      bb_last_phi=dut.m.b_BaseBd.MCPreClk;bb_last_sync=dut.m.b_BaseBd.u_f61.sync;
    end
    if(host_ticks%10000000==0)
      $display("NATIVE844_BASEBOARD host=%0d guest=%0d phi_rises=%0d sync_rises=%0d PC=%04h bus=%04h reset=%b phi=%b sync=%b",host_ticks,guest_ticks,bb_phi_rises,bb_sync_rises,
        dut.m.b_BaseBd.u_f61.u_cpu.logic_00.pc,dut.m.b_BaseBd.u_f61.ab,dut.m.b_BaseBd.MCReset_p_,dut.m.b_BaseBd.MCPreClk,dut.m.b_BaseBd.u_f61.sync);
  end
'''
    tb=inputs/'harness/tb_native_cedar.sv';s=tb.read_text();assert s.count('endmodule')==1
    tb.write_text(s.replace('endmodule',monitor+'\nendmodule'))
    used=list(original['used_hdl'])
    for n in r['extra_RTL_sources']:
        if n not in used:used.append(n)
    (out/'host-cycle-limit.txt').write_text(str(limit)+'\n')
    (out/'window-control.txt').write_text('0 2\n')
    record={**original,'status':'PREPARED_PRIVATE_NATIVE844_COLD_PREFIX','stage':str(stage),
            'stage_manifest_name':'r-front-control-native.json','stage_manifest_sha256':sha(stage/'r-front-control-native.json'),
            'inputs':{str(p.relative_to(inputs)):sha(p)for p in inputs.rglob('*')if p.is_file()},'used_hdl':used,
            'window_control_path':str(out/'window-control.txt'),'window_initial_control':'0 2\n',
            'scope':__doc__,'host_limit':limit,'firmware_qualified':False,'hardware_changed':False,
            'bounded_original_firmware_run':firmware_run,
            'passive_actual_BaseBoard_PC_monitor':monitor_baseboard,
            'baseline_run':str(baseline),'baseline_inputs_or_limits_changed':False}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print(record['status'],len(used),'HDL sources;limit',limit,flush=True)


def reuse_run(out,previous):
    """Reuse a terminal build only when every compiled input is identical."""
    r=json.loads(gzip.decompress((out/'provenance.json.gz').read_bytes()))
    p=json.loads(gzip.decompress((previous/'provenance.json.gz').read_bytes()))
    old=json.loads((previous/'run-state.json').read_text())
    assert old['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC'
    assert all(a['returncode']==0 for a in old['actions'])
    assert r['inputs']==p['inputs']and r['used_hdl']==p['used_hdl']and r['defines']==p['defines']and r['artifacts']==p['artifacts']
    for n,h in r['inputs'].items():assert sha(out/'inputs'/n)==sha(previous/'inputs'/n)==h,n
    source=previous/'obj/native-cedar';binary=out/'obj/native-cedar';binary.parent.mkdir(exist_ok=False);shutil.copy2(source,binary)
    assert sha(binary)==sha(source)
    command=[str(binary),'+limit-file='+str(out/'host-cycle-limit.txt'),'+window-control='+str(out/'window-control.txt'),
             '+boot-presses='+str(r['normal_baseboard_boot_presses'])]
    command+=['+'+n+'='+a['path']for n,a in r['artifacts'].items()]
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],reused_terminal_build=str(previous),
               reused_binary_sha256=sha(binary),compiled_input_identity_verified=True,firmware_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    with(out/'test.log').open('x')as log:
        process=subprocess.Popen(command,cwd=out/'inputs/src',stdout=log,stderr=subprocess.STDOUT)
        action=dict(stage='test',command=command,pid=process.pid,started=time.time());state['actions'].append(action);save();rc=process.wait()
        action.update(returncode=rc,finished=time.time())
    log=(out/'test.log').read_text()
    if not rc and 'NATIVE844_PREFIX samples='in log:status='TERMINAL_NATIVE844_COLD_PREFIX_REVIEW_REQUIRED'
    elif not rc:status='TERMINAL_NATIVE844_PREFIX_INSUFFICIENT'
    else:status='TERMINAL_NATIVE844_PREFIX_FAILED'
    state.update(status=status,finished=time.time(),returncode=rc);save();print(status,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);mode=p.add_mutually_exclusive_group(required=True)
    mode.add_argument('--prepare',type=Path);mode.add_argument('--run',type=Path)
    p.add_argument('--stage',type=Path);p.add_argument('--baseline',type=Path);p.add_argument('--limit',type=int,default=250000000)
    p.add_argument('--reuse-from',type=Path)
    p.add_argument('--monitor-baseboard',action='store_true')
    p.add_argument('--firmware-run',action='store_true',help='Prepare a separate original-ROM run up to5B clocks; never extend an existing job')
    a=p.parse_args()
    if a.prepare:
        assert a.stage and a.baseline;prepare(a.stage.resolve(),a.baseline.resolve(),a.prepare.resolve(),a.limit,a.monitor_baseboard,a.firmware_run)
    elif a.reuse_from:reuse_run(a.run.resolve(),a.reuse_from.resolve())
    else:native_run_test(a.run.resolve())
