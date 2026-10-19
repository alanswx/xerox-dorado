#!/usr/bin/env python3
"""Matched enabled parallel/serial processor fits with the same RAM placement.

Keep the qualified positive-edge 50MHz/20ns shell and identical constraints on
both sides. No core programming, global pause or speed qualification here.
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
    import fit_processor_full as reference
    from generate_conta_ff_scan import once
    text = reference.top(ports,manifest)
    ni = sum(p['width'] for p in ports if p['direction']=='input')
    text = once(text,'input wire host_clk,advance,input wire','input wire host_clk,advance,system_reset,input wire')
    text = once(text,'output reg frozen,done,error,output reg [17:0] result);',
                'output reg frozen,done,error,scan_hold_required,scan_fault,output reg [17:0] result);')
    text = once(text,f'reg [{ni+38}:0] launch;',f'reg [{ni+39}:0] launch;')
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


def prepare(kind,root,output):
    import fit_processor_full as reference_fit
    import generate_processor_full as reference
    import generate_processor_ff_scan as gen
    from test_processor_full import fixture
    source = root/'verilog/generated'/{'ProcH':'ProcH-Rev-Ce.v','ProcL':'ProcL-Rev-Ci.v'}[kind]
    reference_fit.prepare(kind,source,output)
    parallel,reference_manifest,_ = reference.generate(kind,source)
    serial,manifest,clones = gen.generate(kind,source)
    ports = fixture(source,reference_manifest)[2]['ports']
    originals = {k:root/'verilog/cells'/(k+'.v') for k in manifest['original_sources']}
    import re
    deltas = []
    for kind_cell,p in originals.items():
        if sha(p) != manifest['original_sources'][kind_cell]:
            assert kind_cell == 'cell_MC10170'
            current = gen.ROOT/'verilog/cells'/p.name
            assert sha(current) == manifest['original_sources'][kind_cell]
            tokens = lambda p:re.sub(r'\s+','',re.sub(r'/\*.*?\*/|//[^\n]*','',p.read_text(),flags=re.S))
            assert '"' not in tokens(p) and tokens(p) == tokens(current)
            deltas.append(dict(path=str(p),sha256=sha(p),workspace_sha256=sha(current)))
    for variant,board in [('baseline',parallel),('snapshot',serial)]:
        case = output/variant
        (case/'Processor_snapshot.sv').write_text(board)
        for name,text in clones.items():
            if variant == 'baseline' and name == 'ff_scan_controller.sv':
                continue
            (case/name).write_text(text)
        for p in originals.values():
            shutil.copyfile(p,case/p.name)
        for prom in manifest['immutable_proms']:
            assert sha(root/prom['file']) == sha(gen.ROOT/prom['file']) == prom['sha256']
        (case/'top.sv').write_text(shell(ports,reference_manifest,variant=='snapshot'))
        p = case/'snapshot_fit.qsf'
        text = p.read_text()
        if variant == 'baseline':
            text += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        else:
            text += 'set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
        for port in ('system_reset','scan_hold_required','scan_fault'):
            text += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        p.write_text(text)
    inputs = output/'executed-sources'
    inputs.mkdir()
    names = ['fit_processor_ff_scan.py','generate_processor_ff_scan.py','generate_conta_ff_scan.py',
             'ff_scan_controller.sv','fit_processor_full.py','generate_processor_full.py','test_processor_full.py',
             'generate_processor_ff.py','generate_snapshot_ff.py','generate_contb_full.py','generate_contb.py',
             'generate_f10145.py','generate_f10145_shared_write.py','generate_f10145_zero_write.py',
             'generate_async256.py','audit_contb_im.py','fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py']
    paths = [HERE/n for n in names]
    for p in paths:
        shutil.copyfile(p,inputs/p.name)
    record = json.loads((output/'provenance.json').read_text())
    record.update(scope=__doc__,scan_driver_sha256=sha(Path(__file__)),manifest=manifest,
                  reference_manifest=reference_manifest,comment_only_frozen_deltas=deltas,
                  source_hashes=record['source_hashes'] | {str(p.relative_to(gen.ROOT)):sha(p) for p in paths},
                  expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text() for v in ('baseline','snapshot')},
                  compiled_files={str(p.relative_to(output)):sha(p) for p in output.rglob('*') if p.is_file() and p.name!='provenance.json'})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output,quartus):
    record = json.loads((output/'provenance.json').read_text())
    assert sha(Path(__file__)) == record['scan_driver_sha256'], 'prepared driver differs'
    for name,digest in record['compiled_files'].items():
        assert sha(output/name) == digest, 'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case = output/variant
        assert not (case/'map.log').exists(), 'inspect the existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(record['kind'],variant,label,flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Matched serial processor fit terminal; independent audit still required.',flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--kind',choices=['ProcH','ProcL'])
    p.add_argument('--frozen-root',type=Path)
    p.add_argument('--prepare',type=Path)
    p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare and a.kind and a.frozen_root:
        prepare(a.kind,a.frozen_root.resolve(),a.prepare.resolve())
    elif a.run:
        run(a.run.resolve(),a.quartus)
    else:
        p.error('Specify preparation arguments or --run')
