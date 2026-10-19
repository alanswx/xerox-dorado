#!/usr/bin/env python3
"""Matched enabled BaseBoard parallel/serial fits with identical shared RIOT RAM.

Retain original staged positive-edge 50 MHz shell, SDC, four SRAM banks,
firmware, N4/SYSPER4 and vendor CPU/RIOT logic. No core assembly/deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys

HERE=Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def shell(original,scan):
    from generate_conta_ff_scan import once
    text=once(original,'input wire host_clk,advance,freeze,held,request,write,',
              'input wire host_clk,advance,freeze,held,request,write,system_reset,')
    text=once(text,'output reg frozen,done,error);','output reg frozen,done,error,scan_hold_required,scan_fault);')
    text=once(text,'reg [103:0] input_stage,launch;','reg [104:0] input_stage,launch;')
    text=once(text,'input_stage<={advance,freeze,held,request,write,pins,index,data};',
              'input_stage<={system_reset,advance,freeze,held,request,write,pins,index,data};')
    text=once(text,'wire advance_r,freeze_r,held_r,request_r,write_r;',
              'wire system_reset_r,advance_r,freeze_r,held_r,request_r,write_r;')
    text=once(text,'assign {advance_r,freeze_r,held_r,request_r,write_r,pins_r,index_r,data_r}=launch;',
              'assign {system_reset_r,advance_r,freeze_r,held_r,request_r,write_r,pins_r,index_r,data_r}=launch;')
    text=once(text,'wire frozen_w,done_w,error_w;',
              'wire frozen_w,done_w,error_w,scan_hold_w,scan_fault_w;')
    text=once(text,'{outputs,result,frozen,done,error}<={outputs_w,result_w,frozen_w,done_w,error_w};',
              '{outputs,result,frozen,done,error,scan_hold_required,scan_fault}<={outputs_w,result_w,frozen_w,done_w,error_w,scan_hold_w,scan_fault_w};')
    if scan:
        text=once(text,'.state_host_clk(host_clk),.state_guest_advance(advance_r),',
                  '.state_system_reset(system_reset_r),.state_scan_hold_required(scan_hold_w),.state_scan_fault(scan_fault_w),\n .state_host_clk(host_clk),.state_guest_advance(advance_r),')
    else:
        text=once(text,'`ifdef SNAPSHOT_BANK_EXPERIMENT',"assign {scan_hold_w,scan_fault_w}=2'b0;\n`ifdef SNAPSHOT_BANK_EXPERIMENT")
    return text


def prepare(root,out):
    import generate_baseboard_ff_scan as gen
    import fit_baseboard_full as reference_fit
    reference_fit.prepare(root,out)
    old,old_manifest,old_clones=gen.reference(root)
    new,manifest,clones=gen.generate(root)
    old_shell=(out/'baseline/top.sv').read_text()
    for variant,board,helpers in [('baseline',old,old_clones),('snapshot',new,clones)]:
        case=out/variant
        (case/'BaseBd_snapshot.sv').write_text(board)
        for name,s in helpers.items():(case/name).write_bytes(gen.raw_bytes(s))
        (case/'top.sv').write_text(shell(old_shell,variant=='snapshot'))
        qsf=case/'snapshot_fit.qsf';text=qsf.read_text()
        if variant=='baseline':text+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        else:text+='set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
        for port in ('system_reset','scan_hold_required','scan_fault'):
            text+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        qsf.write_text(text)
    paths=[Path(__file__).resolve(),root/gen.SOURCE,*[root/n for n in manifest['original_sources']],
           *gen.qualified.eprom.images(root).values()]
    for module in list(sys.modules.values()):
        filename=getattr(module,'__file__',None)
        if filename:
            p=Path(filename).resolve()
            if p.suffix=='.py' and p.is_relative_to(gen.ROOT):paths.append(p)
    paths=list(dict.fromkeys(paths))
    for p in paths:
        q=out/'executed-sources'/p.relative_to(gen.ROOT)
        q.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,q)
    record=dict(scope=__doc__,manifest=manifest,reference_manifest=old_manifest,
        scan_driver_sha256=sha(Path(__file__)),source_hashes={str(p):sha(p) for p in paths},
        compiled_files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file() and p.name!='provenance.json'},
        expected_qsf={v:(out/v/'snapshot_fit.qsf').read_text() for v in ('baseline','snapshot')})
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text())
    assert sha(Path(__file__))==record['scan_driver_sha256'],'prepared runner differs'
    for name,h in record['compiled_files'].items():assert sha(out/name)==h,'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=out/variant
        assert not(case/'map.log').exists(),'inspect existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print('BaseBoard',variant,label,flush=True)
            with(case/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('BaseBoard serial matched fit terminal; independent audit required.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args()
    if a.prepare and a.source_root:prepare(a.source_root.resolve(),a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --source-root/--prepare or --run')
