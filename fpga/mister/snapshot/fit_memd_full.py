#!/usr/bin/env python3
"""Matched isolated MemD-board fits; no FPGA programming or clock change."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


HERE=Path(__file__).resolve().parent


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def top(ports,manifest):
    from fit_conta_full import top as control_top
    text=control_top(ports,manifest['module'])
    ni=sum(p['width'] for p in ports if p['direction']=='input')
    before=f'reg [{ni+34}:0] launch;'
    assert text.count(before)==1 and text.count('ContA_m_Rev_m_Cd_monitor')==1
    assert text.count('always @(negedge host_clk)')==2
    text=text.replace('always @(negedge host_clk)','always @(posedge host_clk)')
    text=text.replace('Same opposite-edge launch/capture shell and budgets for both variants.',
                      'Both MemD neighbors sample on positive host edges; full20ns budgets.')
    text=text.replace(before,f'reg [{ni+42}:0] launch;').replace('[11:0]','[19:0]').replace('ContA_m_Rev_m_Cd_monitor',manifest['original_module'])
    # Isolate virtual-pin hold from real positive-edge embedded board paths.
    # Both variants get the same input staging; cell RTL/SDC remain unchanged.
    fragment='always @(posedge host_clk)\n  launch<={advance,stimulus,freeze,held,request,write,index,state_data};'
    assert text.count(fragment)==1
    staged=f'reg [{ni+42}:0] input_stage;\n'+\
        ' always @(negedge host_clk)\n  input_stage<={advance,stimulus,freeze,held,request,write,index,state_data};\n'+\
        ' always @(posedge host_clk) launch<=input_stage;'
    text=text.replace(fragment,staged)
    assert text.count('#(.SYSPER(4))')==2
    return text.replace('#(.SYSPER(4))','#(.SYSPER(4),.STORAGE_INPUT_DELAY(0))')



def prepare(source_root,output):
    source=source_root/'verilog/generated/MemD-Rev-Ca.v'
    kind='MemD'
    import generate_memd_full as gen
    from test_memd_full import fixture
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(source_root)
    ports=fixture(source_root,manifest)[2]['ports'];wrapper=top(ports,manifest)
    originals=[source_root/name for name in manifest['original_sources']]
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        shutil.copyfile(source,case/'MemD_original.v')
        (case/'MemD_snapshot.sv').write_text(board)
        for name,text in clones.items():(case/name).write_text(text)
        for path in originals:shutil.copyfile(path,case/path.name)
        for prom in manifest.get('immutable_proms',[]):
            target=case/prom['file'];target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(gen.ROOT/prom['file'],target)
        (case/'top.sv').write_text(wrapper);(case/'bank.sdc').write_text(SDC)
        (case/'audit.tcl').write_text(AUDIT)
        (case/'snapshot_fit.qpf').write_text('PROJECT_REVISION = "snapshot_fit"\n')
        qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name VERILOG_MACRO "DORADO_F10145_LOGIC=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        # The frozen board contains a local loop-variable declaration in
        # its optional delay branch. Parse the unchanged original as SV,
        # just as the native full-core project does, even with delay zero.
        for name in ['top.sv','MemD_snapshot.sv','MemD_original.v',*clones]:
            qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+name+'\n'
        for name in [p.name for p in originals]:
            qsf+='set_global_assignment -name VERILOG_FILE '+name+'\n'
        for port in ['advance','stimulus[*]','freeze','held','request','write','index[*]',
                     'state_data[*]','q[*]','frozen','done','error','result[*]']:
            qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        if variant=='snapshot':
            qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(qsf)
    names=['fit_memd_full.py','generate_memd_full.py','audit_memd_im.py','audit_ifu_im.py','generate_processor_ff.py','test_memd_full.py',
           'generate_snapshot_ff.py','generate_contb_full.py','generate_contb.py','generate_f10145.py',
           'f10470_word_bank.sv','audit_contb_im.py','fit_conta_full.py','fit_f10415.py','fit_f10415_enable.py']
    paths=[HERE/name for name in names]+originals+[gen.ROOT/p['file'] for p in manifest.get('immutable_proms',[])]
    # audit_contb_im imports the original Sil helpers even though this frozen
    # experiment does not regenerate a board. Preserve those dependencies too.
    for module in list(sys.modules.values()):
        filename=getattr(module,'__file__',None)
        if filename:
            path=Path(filename).resolve()
            if path.suffix=='.py' and path.is_relative_to(gen.ROOT/'tools'):
                paths.append(path)
    paths=list(dict.fromkeys(paths))
    record=dict(kind=kind,scope='Isolated complete modeled '+kind+'; original continuous baseline vs exact-advance adapter. Matched50MHz/negative-edge external input stage then positive-edge launch/capture shell/20ns embedded paths/2ns max0ns min virtual I/O; counts include shell/pruning. Small-file scan outputs require every consumer held externally; no global barrier, physical machine snapshot or speed claim.',
        manifest=manifest,driver_sha256=digest(Path(__file__)),source_board_sha256=digest(source),
        source_hashes={str(p.relative_to(gen.ROOT)):digest(p) for p in paths},
        compiled_files={str(p.relative_to(output)):digest(p) for p in output.rglob('*') if p.is_file()})
    record['expected_qsf']={v:(output/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')}
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output,quartus):
    record=json.loads((output/'provenance.json').read_text())
    assert digest(Path(__file__))==record['driver_sha256'],'prepared driver differs'
    for name,expected in record['compiled_files'].items():
        assert digest(output/name)==expected,'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=output/variant
        assert not (case/'map.log').exists(),'Inspect existing job/evidence; do not blindly rerun'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                               ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(record['kind'],variant,label,flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print(record['kind']+' fits terminal; audit timing/M10K/provenance before qualification.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root',type=Path)
    parser.add_argument('--prepare',type=Path)
    parser.add_argument('--run',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare and args.source_root:prepare(args.source_root.resolve(),args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    else:parser.error('Specify --source-root/--prepare or --run')
