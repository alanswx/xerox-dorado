#!/usr/bin/env python3
"""Matched complete-MSA compact versus chip-width cache, same 50 MHz area fixture.

Use the pinned reviewed clock experiment's shell/policy with ALTCLKCTRL and its
two administrative ports removed from BOTH variants. Original host/guest clock
bindings are retained. No assembly, native integration or speed claim.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(out):
    import generate_msa_chipwidth as gen
    from generate_serial_boards import raw_receipt
    reference=gen.base.ROOT/'fpga/mister/reports/2026-10-19-snapshot-msa-clocked-rejected'
    receipt=lambda n:raw_receipt(reference,'fit-terminal/matched/'+n)
    previous=json.loads(receipt('provenance.json'))
    top=receipt('baseline/top.sv').decode()
    gate=''' wire ff_clock,ff_clock_request;
 altclkctrl #(.clock_type("Global Clock"),.ena_register_mode("falling edge"),.intended_device_family("Cyclone V")) ff_clock_control(
 .inclk({3'b000,host_clk}),.clkselect(2'b00),.ena(advance_r || ff_clock_request),.outclk(ff_clock));
'''
    assert top.count(gate)==1;top=top.replace(gate,'',1)
    ports='.state_ff_clk(ff_clock),\n.state_ff_clock_request(ff_clock_request),\n'
    assert top.count(ports)==1;top=top.replace(ports,'',1)
    assert 'altclkctrl'not in top and 'ff_clock'not in top
    baseline,baseline_manifest,baseline_helpers=gen.parent.generate()
    candidate,candidate_manifest,candidate_helpers=gen.generate()
    combinational={name:raw for name,raw in gen.base.original_cells().items() if name[:-2]not in gen.base.native.FIELDS}
    out.mkdir(parents=True,exist_ok=False)
    for label,board,helpers in [('baseline',baseline,baseline_helpers),('chipwidth',candidate,candidate_helpers)]:
        case=out/label;case.mkdir()
        for name,text in {'msa_board.sv':board,**helpers,'top.sv':top,'bank.sdc':previous['expected_sdc'],
                          'audit.tcl':previous['expected_audit'],'snapshot_fit.qsf':previous['expected_qsf'],
                          'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n'}.items():(case/name).write_text(text)
        for name,raw in combinational.items():(case/('original_'+name)).write_bytes(raw)
    executed=out/'executed-sources';executed.mkdir()
    for p in [Path(__file__),Path(gen.__file__),Path(gen.compact.__file__),Path(gen.base.__file__)]:
        (executed/p.name).write_bytes(p.read_bytes())
    record=dict(scope=__doc__,builder_sha256=sha(Path(__file__)),reference_manifest=baseline_manifest,candidate_manifest=candidate_manifest,
        expected_top=top,expected_qsf=previous['expected_qsf'],expected_sdc=previous['expected_sdc'],expected_audit=previous['expected_audit'],
        fixture_parent_receipt='2026-10-19-snapshot-msa-clocked-rejected/fit-terminal/matched',
        compiled_files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},
        limits='Matched complete-MSA only; no native schema/barrier, complete-core fit or speed qualification.')
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Prepared matched compact/chipwidth complete-MSA area experiment',flush=True)


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text());assert sha(Path(__file__))==record['builder_sha256']
    for name,digest in record['compiled_files'].items():assert sha(out/name)==digest,'prepared input changed: '+name
    for label in ('baseline','chipwidth'):
        case=out/label;assert not(case/'map.log').exists(),'observe existing build; do not restart'
        for tool,args,stage in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(label,stage,flush=True)
            with(case/(stage+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Matched chipwidth MSA fit terminal; independent source/timing/area review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --prepare or --run')
