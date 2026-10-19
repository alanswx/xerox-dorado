#!/usr/bin/env python3
"""Matched enabled MemC serial/parallel fits with unchanged staged20ns timing.

Both sides retain identical asynchronous MLAB RAM/file helpers, latch behavior
and timing constraints. No complete-core assembly or deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def shell(ports,manifest,scan):
    from fit_memc_full import top
    from generate_conta_ff_scan import once
    text = top(ports,manifest)
    ni = sum(p['width'] for p in ports if p['direction']=='input')
    text = once(text,'input wire host_clk,advance,input wire','input wire host_clk,advance,system_reset,input wire')
    text = once(text,'output reg frozen,done,error,output reg [17:0] result);',
                'output reg frozen,done,error,scan_hold_required,scan_fault,output reg [17:0] result);')
    assert text.count(f'reg [{ni+38}:0]') == 2
    text = text.replace(f'reg [{ni+38}:0]',f'reg [{ni+39}:0]')
    text = once(text,'input_stage<={advance,stimulus,freeze,held,request,write,index,state_data};',
                'input_stage<={system_reset,advance,stimulus,freeze,held,request,write,index,state_data};')
    text = once(text,'wire step_r,freeze_r,held_r,request_r,write_r;',
                'wire system_reset_r,step_r,freeze_r,held_r,request_r,write_r;')
    text = once(text,'assign {step_r,stimulus_r,freeze_r,held_r,request_r,write_r,index_r,state_r}=launch;',
                'assign {system_reset_r,step_r,stimulus_r,freeze_r,held_r,request_r,write_r,index_r,state_r}=launch;')
    text = once(text,'wire bank_frozen,bank_done,bank_error;',
                'wire bank_scan_hold,bank_scan_fault,bank_frozen,bank_done,bank_error;')
    text = once(text,'always @(posedge host_clk){q,frozen,done,error,result}<={bank_q,bank_frozen,bank_done,bank_error,bank_result};',
                'always @(posedge host_clk){q,frozen,done,error,result,scan_hold_required,scan_fault}<={bank_q,bank_frozen,bank_done,bank_error,bank_result,bank_scan_hold,bank_scan_fault};')
    if scan:
        text = once(text,'.state_host_clk(host_clk),.state_guest_advance(step_r),',
                    '.state_system_reset(system_reset_r),.state_scan_hold_required(bank_scan_hold),.state_scan_fault(bank_scan_fault),\n .state_host_clk(host_clk),.state_guest_advance(step_r),')
    else:
        text = once(text,'`ifdef SNAPSHOT_BANK_EXPERIMENT',"assign {bank_scan_hold,bank_scan_fault}=2'b0;\n`ifdef SNAPSHOT_BANK_EXPERIMENT")
    return text


def prepare(root,output):
    import fit_memc_full as reference_fit
    import generate_memc_full as reference
    import generate_memc_ff_scan as gen
    from test_memc_full import fixture
    reference_fit.prepare('MemC',root/gen.SOURCE,output)
    parallel,old_manifest,_ = reference.generate('MemC',root/gen.SOURCE)
    serial,manifest,clones = gen.generate(root)
    ports = fixture(root/gen.SOURCE,old_manifest)[2]['ports']
    for variant,board in [('baseline',parallel),('snapshot',serial)]:
        case = output/variant
        (case/'MemC_snapshot.sv').write_text(board)
        for name,text in clones.items():
            if name != 'ff_scan_controller.sv' or variant=='snapshot':
                (case/name).write_text(text)
        (case/'top.sv').write_text(shell(ports,old_manifest,variant=='snapshot'))
        qsf = case/'snapshot_fit.qsf'
        text = qsf.read_text()
        if variant=='baseline':
            text += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        else:
            text += 'set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
        for port in ('system_reset','scan_hold_required','scan_fault'):
            text += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        qsf.write_text(text)
    # The remote runner imports no generators. Preserve project-local Python
    # dependencies and original wire-list inputs with their source paths.
    import sys
    paths=[HERE/'ff_scan_controller.sv',HERE/'test_memc_full.cpp']
    for module in list(sys.modules.values()):
        filename=getattr(module,'__file__',None)
        if filename:
            path=Path(filename).resolve()
            if path.suffix=='.py' and path.is_relative_to(gen.ROOT):
                paths.append(path)
    paths=list(dict.fromkeys(paths))
    for path in paths:
        target=output/'executed-sources'/path.relative_to(gen.ROOT)
        target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
    record = json.loads((output/'provenance.json').read_text())
    record.update(scope=__doc__,scan_driver_sha256=sha(Path(__file__)),manifest=manifest,reference_manifest=old_manifest,
                  source_hashes=record['source_hashes'] | {str(p.relative_to(gen.ROOT)):sha(p) for p in paths},
                  expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text() for v in ('baseline','snapshot')},
                  compiled_files={str(p.relative_to(output)):sha(p) for p in output.rglob('*') if p.is_file() and p.name!='provenance.json'})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output,quartus):
    record = json.loads((output/'provenance.json').read_text())
    assert sha(Path(__file__))==record['scan_driver_sha256'], 'prepared runner differs'
    for name,digest in record['compiled_files'].items():
        assert sha(output/name)==digest, 'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case = output/variant
        assert not (case/'map.log').exists(), 'inspect the existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print('MemC',variant,label,flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('MemC scan matched fit terminal; independent audit still required.',flush=True)


if __name__=='__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path)
    p.add_argument('--prepare',type=Path)
    p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare and a.source_root:
        prepare(a.source_root.resolve(),a.prepare.resolve())
    elif a.run:
        run(a.run.resolve(),a.quartus)
    else:
        p.error('Specify preparation arguments or --run')
