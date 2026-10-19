#!/usr/bin/env python3
"""Matched isolated full IFU fits; no production source or FPGA changes."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(source_root,output):
    import generate_ifu_full as gen
    from test_ifu_full import fixture
    from fit_conta_full import top
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    output.mkdir(parents=True,exist_ok=False);board,manifest,clones=gen.generate(source_root)
    ports=fixture(source_root,manifest)[2]['ports'];ni=sum(p['width']for p in ports if p['direction']=='input')
    wrapper=top(ports,manifest['module'])
    assert wrapper.count(f'reg [{ni+34}:0] launch;')==1 and wrapper.count('always @(negedge host_clk)')==2
    wrapper=wrapper.replace(f'reg [{ni+34}:0] launch;',f'reg [{ni+40}:0] launch;').replace('[11:0]','[17:0]')
    wrapper=wrapper.replace('ContA_m_Rev_m_Cd_monitor',manifest['original_module']).replace('always @(negedge host_clk)','always @(posedge host_clk)')
    wrapper=wrapper.replace('Same opposite-edge launch/capture shell and budgets for both variants.',
        'Matched positive-edge launch/capture:20ns embedded paths at50MHz, both variants.')
    originals=[source_root/name for name in manifest['original_sources']]
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name VERILOG_MACRO "DORADO_F10415_M10K=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for name in ['top.sv','IFU_snapshot.sv',*clones]:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+name+'\n'
    for name in ['IFU_original.v',*[p.name for p in originals]]:qsf+='set_global_assignment -name VERILOG_FILE '+name+'\n'
    for port in ['advance','stimulus[*]','freeze','held','request','write','index[*]','state_data[*]','q[*]','frozen','done','error','result[*]']:
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        shutil.copyfile(source_root/'verilog/generated/IFU-Rev-Ch.v',case/'IFU_original.v')
        (case/'IFU_snapshot.sv').write_text(board)
        for name,text in clones.items():(case/name).write_text(text)
        for p in originals:shutil.copyfile(p,case/p.name)
        target=case/gen.PROM;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source_root/gen.PROM,target)
        for name,text in {'top.sv':wrapper,'bank.sdc':SDC,'audit.tcl':AUDIT,'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
            'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():
            (case/name).write_text(text)
    here=Path(__file__).resolve().parent
    paths=[here/name for name in ['fit_ifu_full.py','generate_ifu_full.py','test_ifu_full.py','audit_ifu_im.py',
        'audit_contb_im.py','generate_contb_full.py','generate_snapshot_ff.py','generate_jk.py','f10415_word_bank.sv',
        'fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py']]
    paths +=originals+[source_root/'verilog/generated/IFU-Rev-Ch.v',source_root/gen.PROM]
    record=dict(scope='Isolated complete modeled IFU; original continuous baseline vs exact-advance/state adapter, matched50MHz positive-edge launch/capture shell/20ns embedded paths and2ns max0ns min virtual I/O. Original omitted CE ports retained. Counts include shell/admin/pruning; no full-core barrier/physical snapshot/original60ns or speed qualification.',
        manifest=manifest,driver_sha256=digest(Path(__file__)),source_hashes={str(p):digest(p)for p in paths},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.rglob('*')if p.is_file()},
        expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(directory,quartus):
    record=json.loads((directory/'provenance.json').read_text())
    assert digest(Path(__file__))==record['driver_sha256'],'prepared driver changed'
    for name,expected in record['compiled_files'].items():assert digest(directory/name)==expected,'prepared input changed: '+name
    for variant in ('baseline','snapshot'):
        case=directory/variant;assert not(case/'map.log').exists(),'inspect existing job/evidence; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Full IFU fits terminal; qualify inference/sources/all-corner timing before integration.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source-root',type=Path)
    parser.add_argument('--prepare',type=Path);parser.add_argument('--run',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare and args.source_root:prepare(args.source_root.resolve(),args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    else:parser.error('Specify --source-root/--prepare or --run')
