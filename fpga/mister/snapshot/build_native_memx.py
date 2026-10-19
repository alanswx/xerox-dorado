#!/usr/bin/env python3
"""Native57: integrate qualified MemX adapter into frozen Native56b.

Normal-operation qualification only. All state admin inputs remain inactive;
no machine-wide barrier/save/load, speed change or FPGA programming here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def once(text,before,after):
    if text.count(before)!=1:
        raise ValueError('unexpected frozen source shape: '+before[:90])
    return text.replace(before,after,1)


def prepare_assets(frozen_root,output):
    import generate_memx_full as gen
    output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(frozen_root)
    files={'MemX_full_snapshot.sv':board,**clones}
    originals={'verilog/generated/MemX-Rev-Ch.v':manifest['original_sha256'],
               **manifest['original_sources'],**{p['file']:p['sha256']for p in manifest['immutable_proms']}}
    shared={}
    for name in clones:
        choices=[Path('fpga/mister')/d/name for d in ('snapshot_native','snapshot_native_conta','snapshot_native_processor','snapshot_native_ifu','snapshot_native_memc','snapshot_native_memd')
                 if (frozen_root/'fpga/mister'/d/name).is_file()]
        if not choices:continue
        if len(choices)!=1 or digest(frozen_root/choices[0])!=manifest['cloned_sources'][name]:
            raise ValueError('shared qualified adapter differs/ambiguous: '+name)
        shared[name]=str(choices[0])
    if set(files)-set(shared)!={'MemX_full_snapshot.sv','cell_MosRam_snapshot.sv'}:
        raise ValueError('unexpected new MemX sources')
    for name,text in files.items():(output/name).write_text(text)
    for name,sha in originals.items():
        if digest(frozen_root/name)!=sha:raise ValueError('frozen original differs: '+name)
    record=dict(manifests={'MemX':manifest},assets={name:digest(output/name) for name in files},
        expected_original_sources=originals,shared_assets=shared,
        builder_sha256=digest(Path(__file__)),
        scope='Frozen56b plus complete exact-advance MemX adapter including all independent64K-map socket histories; shared qualified cell adapters reused byte-identically. All state admin inactive; no global save/load or speed claim.')
    (output/'assets.json').write_text(json.dumps(record,indent=2)+'\n')


CROSSINGS='''# Reporting only: unchanged SDC, no new timing exceptions.
project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 foreach {pattern label} {
  {*|ContA_full_snapshot_experiment:b_ContA|*} conta
  {*|ContB_full_snapshot_experiment:b_ContB|*} contb
  {*|ProcH_full_snapshot_experiment:b_ProcH|*} proch
  {*|ProcL_full_snapshot_experiment:b_ProcL|*} procl
  {*|IFU_full_snapshot_experiment:b_IFU|*} ifu
  {*|MemC_full_snapshot_experiment:b_MemC|*} memc
  {*|MemD_full_snapshot_experiment:b_MemD|*} memd
  {*|MemX_full_snapshot_experiment:b_MemX|*} memx
 } {
  set board [get_registers $pattern]
  set others [remove_from_collection [get_registers *] $board]
  set board_count [get_collection_size $board]
  set other_count [get_collection_size $others]
  if {$board_count==0 || $other_count==0} {error "Missing $label/core register coverage"}
  puts "ADAPTER_CROSSING_CORNER $name $label board=$board_count others=$other_count"
  foreach mode {setup hold} {
   report_timing -$mode -from $board -to $others -npaths 12 -detail full_path -file native57-$label-out-$mode-$name.rpt
   report_timing -$mode -from $others -to $board -npaths 12 -detail full_path -file native57-$label-in-$mode-$name.rpt
  }
 }
 delete_timing_netlist
}
project_close
'''


def build(baseline,output,assets,quartus):
    metadata=json.loads((assets/'assets.json').read_text())
    if digest(Path(__file__))!=metadata['builder_sha256']:
        raise ValueError('prepared builder differs')
    for name,sha in metadata['assets'].items():
        if digest(assets/name)!=sha:raise ValueError('prepared asset differs: '+name)
    for name,sha in metadata['expected_original_sources'].items():
        if digest(baseline/'src'/name)!=sha:raise ValueError('original source/PROM differs: '+name)
    for name,path in metadata['shared_assets'].items():
        if digest(baseline/'src'/path)!=metadata['assets'][name]:
            raise ValueError('shared adapter differs: '+name)
    if (baseline/'core/Dorado.qsf').read_text().count('set_global_assignment -name VERILOG_MACRO \"DORADO_STORAGE_INPUT_DELAY=2\"')!=1:
        raise ValueError('qualified native storage input delay changed')
    coretext=(baseline/'core/Dorado.sv').read_text()
    if coretext.count('always @(negedge clk_sys) network_step <= machine_enable;')!=1 or \
            coretext.count('.contb_advance(network_step)')!=1:
        raise ValueError('exact actual-advance routing changed')
    output.mkdir(parents=True,exist_ok=False)
    shutil.copytree(baseline/'src',output/'src')
    shutil.copytree(baseline/'core',output/'core',ignore=shutil.ignore_patterns(
        'db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
    for p in (output/'core').rglob('*'):
        if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
            p.write_text(p.read_text().replace(str(baseline),str(output)))
    bp=output/'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    text=bp.read_text()
    for kind,manifest in metadata['manifests'].items():
        original=manifest['original_module']+' b_'+kind+' ('
        replacement=manifest['module']+' b_'+kind+''' (
    // Same actual falling-edge-latched enable as the global gate/ContA/ContB.
    .state_host_clk(storage_clk),.state_guest_advance(contb_advance),
    // Complete-machine barriers/transport are not installed: ALL admin OFF.
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index(24'b0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),'''
        text=once(text,original,replacement)
    bp.write_text(text)
    target=output/'src/fpga/mister/snapshot_native_memx';target.mkdir(exist_ok=False)
    new=[name for name in metadata['assets'] if name not in metadata['shared_assets']]
    for name in new:shutil.copyfile(assets/name,target/name)
    shutil.copyfile(assets/'assets.json',target/'assets.json')
    qsf=output/'core/Dorado.qsf';text=qsf.read_text()
    for name in new:text+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(target/name)+'\n'
    qsf.write_text(text)
    changed=[];added=[]
    for p in (output/'src').rglob('*'):
        if not p.is_file():continue
        name=p.relative_to(output/'src');old=baseline/'src'/name
        if not old.exists():added.append(str(name))
        elif p.read_bytes()!=old.read_bytes():changed.append(str(name))
    if changed!=['build/rtl-executive/native16-tx-cable/dorado_native_backplane.v']:
        raise ValueError('unintended source mutation: '+str(changed))
    expected={str(Path('fpga/mister/snapshot_native_memx')/name) for name in new+['assets.json']}
    if set(added)!=expected:raise ValueError('unintended added source')
    if (output/'core/Dorado.sv').read_bytes()!=(baseline/'core/Dorado.sv').read_bytes():
        raise ValueError('outer clock/wrapper changed')
    for name in ('Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc'):
        if (output/'core'/name).read_text().replace(str(output),str(baseline))!=(baseline/'core'/name).read_text():
            raise ValueError('timing constraints changed: '+name)
    corners=output/'core/device-corners.tcl';corners.write_text(corners.read_text().replace('native56-','native57-'))
    (output/'core/audit-adapter-crossings.tcl').write_text(CROSSINGS)
    record=dict(baseline=str(baseline),candidate=str(output),change=metadata['scope'],
        snapshot_admin='INACTIVE; no physical save/load',extra_timing_exceptions='None',
        source_changes=changed,added_sources=added,shared_sources=metadata['shared_assets'],
        speed='Same50MHz/SYSPER4; no speed claim. Protocol replay variability remains open.',
        source_hashes={str(p.relative_to(output)):digest(p) for p in [output/'core/Dorado.sv',qsf,
            output/'core/device-corners.tcl',output/'core/audit-adapter-crossings.tcl',
            *[output/'src'/name for name in changed+added]]})
    (output/'memx-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),
        ('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),
        ('quartus_sta',['-t','audit_native_clocks.tcl'],'audit'),
        ('quartus_sta',['-t','device-corners.tcl'],'device-corners'),
        ('quartus_sta',['-t','audit-adapter-crossings.tcl'],'adapter-crossings')]:
        print('Native57',label,flush=True)
        with (output/(label+'.log')).open('w') as log:
            subprocess.run([str(quartus/tool),*args],cwd=output/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Native57 build terminal; audit sources/timing/inference before deployment.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare-assets',type=Path)
    parser.add_argument('--frozen-source-root',type=Path)
    parser.add_argument('--build',action='store_true')
    parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native56b_20261019'))
    parser.add_argument('--output',type=Path);parser.add_argument('--assets',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare_assets and args.frozen_source_root:
        prepare_assets(args.frozen_source_root.resolve(),args.prepare_assets.resolve())
    elif args.build and args.output and args.assets:
        build(args.baseline.resolve(),args.output.resolve(),args.assets.resolve(),args.quartus)
    else:parser.error('Specify asset preparation inputs or --build/--output/--assets')
