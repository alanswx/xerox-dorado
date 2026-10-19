#!/usr/bin/env python3
"""Matched enabled IFU serial/parallel fits with unchanged 20 ns shell/SDC.

Both sides retain all eleven independent old-data M10K IFUM groups. This
runner never assembles or deploys a complete core.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def original_shell(ports,manifest):
    from fit_conta_full import top
    text = top(ports,manifest['module'])
    ni = sum(p['width'] for p in ports if p['direction']=='input')
    assert text.count(f'reg [{ni+34}:0] launch;')==1
    assert text.count('always @(negedge host_clk)')==2
    text = text.replace(f'reg [{ni+34}:0] launch;',f'reg [{ni+40}:0] launch;').replace('[11:0]','[17:0]')
    text = text.replace('ContA_m_Rev_m_Cd_monitor',manifest['original_module'])
    text = text.replace('always @(negedge host_clk)','always @(posedge host_clk)')
    return text.replace('Same opposite-edge launch/capture shell and budgets for both variants.',
                        'Matched positive-edge launch/capture:20ns embedded paths at50MHz, both variants.')


def shell(ports,manifest,scan):
    from generate_conta_ff_scan import once
    text = original_shell(ports,manifest)
    ni = sum(p['width'] for p in ports if p['direction']=='input')
    text = once(text,'input wire host_clk,advance,input wire','input wire host_clk,advance,system_reset,input wire')
    text = once(text,'output reg frozen,done,error,output reg [17:0] result);',
                'output reg frozen,done,error,scan_hold_required,scan_fault,output reg [17:0] result);')
    text = once(text,f'reg [{ni+40}:0] launch;',f'reg [{ni+41}:0] launch;')
    text = once(text,'launch<={advance,stimulus,freeze,held,request,write,index,state_data};',
                'launch<={system_reset,advance,stimulus,freeze,held,request,write,index,state_data};')
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
    import fit_ifu_full as reference_fit
    import generate_ifu_full as reference
    import generate_ifu_ff_scan as gen
    from test_ifu_full import fixture
    reference_fit.prepare(root,output)
    parallel,old_manifest,_ = reference.generate(root)
    serial,manifest,clones = gen.generate(root)
    ports = fixture(root,old_manifest)[2]['ports']
    for variant,board in [('baseline',parallel),('snapshot',serial)]:
        case = output/variant
        assert (case/'top.sv').read_text()==original_shell(ports,old_manifest)
        (case/'IFU_snapshot.sv').write_text(board)
        for name,text in clones.items():
            if name!='ff_scan_controller.sv' or variant=='snapshot':
                (case/name).write_text(text)
        (case/'top.sv').write_text(shell(ports,old_manifest,variant=='snapshot'))
        qsf = case/'snapshot_fit.qsf';text = qsf.read_text()
        if variant=='baseline':
            text += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        else:
            text += 'set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
        for port in ('system_reset','scan_hold_required','scan_fault'):
            text += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        qsf.write_text(text)
    paths = [HERE/'ff_scan_controller.sv',HERE/'test_ifu_full.cpp',gen.qualified.im.SIL]
    for module in list(sys.modules.values()):
        filename = getattr(module,'__file__',None)
        if filename:
            path = Path(filename).resolve()
            if path.suffix=='.py' and path.is_relative_to(gen.ROOT):
                paths.append(path)
    paths = list(dict.fromkeys(paths))
    for p in paths:
        target=output/'executed-sources'/p.relative_to(gen.ROOT)
        target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,target)
    record = json.loads((output/'provenance.json').read_text())
    record.update(scope=__doc__,scan_driver_sha256=sha(Path(__file__)),manifest=manifest,reference_manifest=old_manifest,
                  source_hashes=record['source_hashes'] | {str(p):sha(p) for p in paths},
                  expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text() for v in ('baseline','snapshot')},
                  compiled_files={str(p.relative_to(output)):sha(p) for p in output.rglob('*') if p.is_file() and p.name!='provenance.json'})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output,quartus):
    record = json.loads((output/'provenance.json').read_text())
    assert sha(Path(__file__))==record['scan_driver_sha256'], 'prepared runner differs'
    for name,digest in record['compiled_files'].items():
        assert sha(output/name)==digest, 'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=output/variant
        assert not (case/'map.log').exists(), 'inspect the existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print('IFU',variant,label,flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('IFU serial matched fit terminal; independent audit still required.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path)
    p.add_argument('--prepare',type=Path)
    p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args()
    if a.prepare and a.source_root:
        prepare(a.source_root.resolve(),a.prepare.resolve())
    elif a.run:
        run(a.run.resolve(),a.quartus)
    else:
        p.error('Specify preparation arguments or --run')
