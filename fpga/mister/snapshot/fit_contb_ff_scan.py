#!/usr/bin/env python3
"""Matched enabled ContB serial/parallel fits with original gated-clock shell.

Keep all eight IM half-banks, old-data behavior, original SDC and 50 MHz
budgets. No complete-core assembly, deployment or clock migration.
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


def shell(ports,scan):
    from fit_contb_full import top
    from generate_conta_ff_scan import once
    text=top(ports)
    text=once(text,'input wire host_clk,run,input wire','input wire host_clk,run,system_reset,input wire')
    text=once(text,'output reg frozen,done,error,output reg [17:0] result);',
              'output reg frozen,done,error,scan_hold_required,scan_fault,output reg [17:0] result);')
    text=once(text,'reg [36:0] admin;','reg [37:0] admin;')
    text=once(text,'admin <= {freeze,held,request,write,state_index,state_data};',
              'admin <= {system_reset,freeze,held,request,write,state_index,state_data};')
    text=once(text,'wire freeze_r,held_r,request_r,write_r;',
              'wire system_reset_r,freeze_r,held_r,request_r,write_r;')
    text=once(text,'assign {freeze_r,held_r,request_r,write_r,index_r,data_r}=admin;',
              'assign {system_reset_r,freeze_r,held_r,request_r,write_r,index_r,data_r}=admin;')
    text=once(text,'wire bank_frozen,bank_done,bank_error;',
              'wire bank_scan_hold,bank_scan_fault,bank_frozen,bank_done,bank_error;')
    text=once(text,'{frozen,done,error,result} <= {bank_frozen,bank_done,bank_error,bank_result};',
              '{frozen,done,error,result,scan_hold_required,scan_fault} <= {bank_frozen,bank_done,bank_error,bank_result,bank_scan_hold,bank_scan_fault};')
    if scan:
        text=once(text,'.state_host_clk(host_clk),.state_guest_advance(enabled),',
                  '.state_system_reset(system_reset_r),.state_scan_hold_required(bank_scan_hold),.state_scan_fault(bank_scan_fault),\n .state_host_clk(host_clk),.state_guest_advance(enabled),')
    else:
        text=once(text,'`ifdef SNAPSHOT_BANK_EXPERIMENT',"assign {bank_scan_hold,bank_scan_fault}=2'b0;\n`ifdef SNAPSHOT_BANK_EXPERIMENT")
    return text


def prepare(root,out):
    import generate_contb_ff_scan as gen
    import fit_contb_full as reference_fit
    import test_contb_full as reference_test
    with gen.source_context(root):
        reference_fit.prepare(out)
        old,old_manifest,_=gen.qualified.generate()
        ports=reference_test.fixture(old_manifest)[2]
    new,manifest,clones=gen.generate(root)
    for variant,board in [('baseline',old),('snapshot',new)]:
        case=out/variant
        (case/'ContB_snapshot.sv').write_text(board)
        for name,text in clones.items():
            if name!='ff_scan_controller.sv' or variant=='snapshot':
                (case/name).write_text(text)
        # The old preparer copies its repository's ordinary cells. Compile
        # the actual frozen cells instead, retaining the reviewed MC10170
        # comment delta in provenance and all immutable source bytes.
        for name in manifest['grouping_sources']:
            shutil.copyfile(root/name,case/Path(name).name)
        (case/'top.sv').write_text(shell(ports,variant=='snapshot'))
        qsf=case/'snapshot_fit.qsf';text=qsf.read_text()
        if variant=='baseline':text+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        else:text+='set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
        for port in ('system_reset','scan_hold_required','scan_fault'):
            text+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        qsf.write_text(text)
    wirelist=root/gen.WIRELIST if (root/gen.WIRELIST).exists() else gen.ROOT/gen.WIRELIST
    paths=[root/gen.SOURCE,wirelist,wirelist.with_suffix('.lc'),
           *[root/n for n in manifest['grouping_sources']],HERE/'f10415_enable_bank.sv',
           HERE/'ff_scan_controller.sv',HERE/'test_contb_full.cpp']
    for module in list(sys.modules.values()):
        filename=getattr(module,'__file__',None)
        if filename:
            path=Path(filename).resolve()
            if path.suffix=='.py' and path.is_relative_to(gen.ROOT):paths.append(path)
    paths=list(dict.fromkeys(paths))
    for path in paths:
        target=out/'executed-sources'/path.relative_to(gen.ROOT)
        target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
    record=dict(scope=__doc__,manifest=manifest,reference_manifest=old_manifest,
        scan_driver_sha256=sha(Path(__file__)),source_hashes={str(p):sha(p) for p in paths},
        compiled_files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file() and p.name!='provenance.json'},
        expected_qsf={v:(out/v/'snapshot_fit.qsf').read_text() for v in ('baseline','snapshot')})
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text())
    assert sha(Path(__file__))==record['scan_driver_sha256'], 'prepared runner differs'
    for name,digest in record['compiled_files'].items():assert sha(out/name)==digest, 'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=out/variant
        assert not (case/'map.log').exists(), 'inspect the existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print('ContB',variant,label,flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('ContB serial matched fit terminal; independent audit still required.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args()
    if a.prepare and a.source_root:prepare(a.source_root.resolve(),a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify preparation arguments or --run')
