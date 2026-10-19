#!/usr/bin/env python3
"""Matched enabled parallel/serial ContA fits with identical async RAM helpers.

Both use the same complete physical board and50MHz/opposite-edge shell, with
identical constraints. The compact FF schema and hold/fault interface differ.
No full-core integration, global snapshot, speed or FPGA programming.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import generate_conta_ff_scan as gen
import generate_conta_full as previous
import fit_conta_full as qualified

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def shell(ports, module, scan):
    text = qualified.top(ports, module)
    text = gen.once(text, 'input wire host_clk,advance,input wire',
                    'input wire host_clk,advance,system_reset,input wire')
    text = gen.once(text, 'output reg frozen,done,error,output reg [17:0] result);',
                    'output reg frozen,done,error,scan_hold_required,scan_fault,output reg [17:0] result);')
    ni = sum(p['width'] for p in ports if p['direction'] == 'input')
    text = gen.once(text, f'reg [{ni+34}:0] launch;', f'reg [{ni+35}:0] launch;')
    text = gen.once(text, 'launch<={advance,stimulus,freeze,held,request,write,index,state_data};',
                    'launch<={system_reset,advance,stimulus,freeze,held,request,write,index,state_data};')
    text = gen.once(text, 'wire step_r,freeze_r,held_r,request_r,write_r;',
                    'wire system_reset_r,step_r,freeze_r,held_r,request_r,write_r;')
    text = gen.once(text, 'assign {step_r,stimulus_r,freeze_r,held_r,request_r,write_r,index_r,state_r}=launch;',
                    'assign {system_reset_r,step_r,stimulus_r,freeze_r,held_r,request_r,write_r,index_r,state_r}=launch;')
    text = gen.once(text, 'wire bank_frozen,bank_done,bank_error;',
                    'wire bank_scan_hold,bank_scan_fault,bank_frozen,bank_done,bank_error;')
    text = gen.once(text, 'always @(negedge host_clk){q,frozen,done,error,result}<={bank_q,bank_frozen,bank_done,bank_error,bank_result};',
                    'always @(negedge host_clk){q,frozen,done,error,result,scan_hold_required,scan_fault}<={bank_q,bank_frozen,bank_done,bank_error,bank_result,bank_scan_hold,bank_scan_fault};')
    if scan:
        text = gen.once(text, '.state_host_clk(host_clk),.state_guest_advance(step_r),',
                        '.state_system_reset(system_reset_r),.scan_hold_required(bank_scan_hold),.scan_fault(bank_scan_fault),\n .state_host_clk(host_clk),.state_guest_advance(step_r),')
    else:
        text = gen.once(text, '`ifdef SNAPSHOT_BANK_EXPERIMENT',
                        "assign {bank_scan_hold,bank_scan_fault}=2'b0;\n`ifdef SNAPSHOT_BANK_EXPERIMENT")
    return text


def prepare(root, output):
    source = root/'build/rtl-executive/native16-tx-cable/ContA_monitor.v'
    qualified.prepare(source, output)
    parallel, parallel_manifest, old_clones = previous.generate(source)
    serial, manifest, clones = gen.generate(source, async_files=True)
    assert all(old_clones[n] == s for n,s in clones.items()
               if n not in ('ff_scan_controller.sv','cell_F10145A_snapshot.sv'))
    record = json.loads((output/'provenance.json').read_text())
    from test_conta_full import fixture
    ports = fixture(source, parallel_manifest)[2]['ports']
    originals = {kind:root/'verilog/cells'/(kind+'.v') for kind in parallel_manifest['original_sources']}
    import re
    comment_deltas = []
    for kind,path in originals.items():
        if sha(path) != parallel_manifest['original_sources'][kind]:
            assert kind == 'cell_MC10170'
            current = gen.ROOT/'verilog/cells'/path.name
            tokens = lambda p: re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', p.read_text(), flags=re.S))
            assert '"' not in tokens(path) and tokens(path) == tokens(current)
            comment_deltas.append(dict(path=str(path),sha256=sha(path),workspace_sha256=sha(current)))
    for variant,board in [('baseline',parallel),('snapshot',serial)]:
        case = output/variant
        (case/'ContA_snapshot.sv').write_text(board)
        for name,text in clones.items():
            if variant == 'baseline' and name == 'ff_scan_controller.sv':
                continue
            (case/name).write_text(text)
        for path in originals.values():
            shutil.copyfile(path, case/path.name)
        (case/'top.sv').write_text(shell(ports, manifest['module'], variant == 'snapshot'))
        qsf = case/'snapshot_fit.qsf'
        text = qsf.read_text()
        if variant == 'baseline':
            text += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        else:
            text += 'set_global_assignment -name SYSTEMVERILOG_FILE ff_scan_controller.sv\n'
        for port in ('system_reset','scan_hold_required','scan_fault'):
            text += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        qsf.write_text(text)
    inputs = output/'executed-sources'
    inputs.mkdir()
    paths = [HERE/n for n in ('fit_conta_ff_scan.py','generate_conta_ff_scan.py','ff_scan_controller.sv',
                              'fit_conta_full.py','generate_conta_full.py','generate_snapshot_ff.py',
                              'generate_contb_full.py','generate_contb.py','audit_contb_im.py',
                              'generate_f10145.py','generate_f10145_shared_write.py','generate_f10145_zero_write.py',
                              'test_conta_full.py','fit_f10415.py','fit_f10415_enable.py')]
    for path in paths:
        shutil.copyfile(path, inputs/path.name)
    record.update(scope=__doc__,scan_driver_sha256=sha(Path(__file__)),manifest=manifest,
                  reference_manifest=parallel_manifest,comment_only_frozen_deltas=comment_deltas,
                  shared_ram_sha256=manifest['cloned_sources']['cell_F10145A_snapshot.sv'],
                  source_hashes=record['source_hashes'] | {str(p.relative_to(gen.ROOT)):sha(p) for p in paths},
                  expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text() for v in ('baseline','snapshot')},
                  compiled_files={str(p.relative_to(output)):sha(p) for p in output.glob('*/*') if p.is_file()})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--prepare',type=Path)
    p.add_argument('--frozen-root',type=Path)
    p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    if a.prepare and a.frozen_root:
        prepare(a.frozen_root.resolve(),a.prepare.resolve())
    elif a.run:
        record = json.loads((a.run/'provenance.json').read_text())
        assert sha(Path(__file__)) == record['scan_driver_sha256'], 'prepared driver differs'
        qualified.run(a.run.resolve(),a.quartus)
    else:
        p.error('Specify --prepare/--frozen-root or --run')
