#!/usr/bin/env python3
"""Native52: add qualified ContA host-advance adapters to frozen Native51.

Normal-operation integration only: ALL admin ports stay inactive until the
complete-machine barrier and state transport exist. No FPGA programming.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def once(text,before,after):
    if text.count(before)!=1:raise ValueError('unexpected frozen input shape: '+before[:90])
    return text.replace(before,after,1)


def prepare_assets(source,frozen_root,output):
    import generate_conta_full as gen
    from build_native_contb import comment_lines_removed
    output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(source)
    if len(re.findall(r'\bSYSPER\b',gen.ff.masked_comments(source.read_text())))!=1:
        raise ValueError('ContA SYSPER is no longer an unused declaration; review actual native parameter binding')
    files={'ContA_full_snapshot.sv':board,**clones}
    shared=['cell_MC10176_snapshot.v','cell_MC10231_snapshot.v']
    for name,text in files.items():(output/name).write_text(text)
    originals={f'verilog/cells/{kind}.v':sha for kind,sha in manifest['original_sources'].items()}
    originals['build/rtl-executive/native16-tx-cable/ContA_monitor.v']=manifest['original_sha256']
    equivalence={}
    for name,qualified in list(originals.items()):
        path=frozen_root/name;frozen=digest(path)
        if frozen==qualified:continue
        current=gen.ROOT/name
        if not current.is_file()or comment_lines_removed(current.read_text())!=comment_lines_removed(path.read_text()):
            raise ValueError('qualified source differs from actual frozen core: '+name)
        equivalence[name]=dict(qualified_sha256=qualified,frozen_sha256=frozen,
            code_sha256=hashlib.sha256(comment_lines_removed(path.read_text()).encode()).hexdigest(),
            reason='Only full-line comments/blank lines differ; complete code lines match, no block comments/continued lines accepted')
        originals[name]=frozen
    metadata=dict(manifest=manifest,expected_original_sources=originals,
        assets={name:digest(output/name)for name in files},shared_assets=shared,
        comment_only_equivalence=equivalence,builder_sha256=digest(Path(__file__)),
        scope='Frozen51 plus ContA exact-advance adapters; shared qualified ContB FF clones reused byte-identically; all admin inactive, no machine save/load')
    (output/'assets.json').write_text(json.dumps(metadata,indent=2)+'\n')


CROSSINGS='''# Post-fit reports only. No timing exceptions or SDC mutations.
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
 } {
  set board [get_registers $pattern]
  set others [remove_from_collection [get_registers *] $board]
  set board_count [get_collection_size $board]
  set other_count [get_collection_size $others]
  if {$board_count==0 || $other_count==0} {error "Missing $label/core register coverage"}
  puts "CONTROL_CROSSING_CORNER $name $label board=$board_count others=$other_count"
  foreach mode {setup hold} {
   report_timing -$mode -from $board -to $others -npaths 12 -detail full_path -file native52-$label-out-$mode-$name.rpt
   report_timing -$mode -from $others -to $board -npaths 12 -detail full_path -file native52-$label-in-$mode-$name.rpt
  }
 }
 delete_timing_netlist
}
project_close
'''


def build(baseline,output,assets,quartus):
    metadata=json.loads((assets/'assets.json').read_text())
    if digest(Path(__file__))!=metadata['builder_sha256']:raise ValueError('prepared builder differs')
    for name,sha in metadata['assets'].items():
        if digest(assets/name)!=sha:raise ValueError('prepared asset differs: '+name)
    for name,sha in metadata['expected_original_sources'].items():
        if digest(baseline/'src'/name)!=sha:raise ValueError('qualified original differs: '+name)
    for name in metadata['shared_assets']:
        if digest(baseline/'src/fpga/mister/snapshot_native'/name)!=metadata['assets'][name]:
            raise ValueError('existing shared ContB adapter differs: '+name)
    coretext=(baseline/'core/Dorado.sv').read_text()
    if coretext.count('always @(negedge clk_sys) network_step <= machine_enable;')!=1 or coretext.count('.contb_advance(network_step)')!=1:
        raise ValueError('original exact guest-advance routing differs')
    output.mkdir(parents=True,exist_ok=False)
    shutil.copytree(baseline/'src',output/'src')
    shutil.copytree(baseline/'core',output/'core',ignore=shutil.ignore_patterns(
        'db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
    for path in (output/'core').rglob('*'):
        if path.is_file()and path.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
            path.write_text(path.read_text().replace(str(baseline),str(output)))
    bp=output/'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    text=bp.read_text()
    text=once(text,'ContA_m_Rev_m_Cd_monitor b_ContA (','''ContA_full_snapshot_experiment b_ContA (
    // Same falling-edge mirror already used by ContB; no new clock/gate.
    .state_host_clk(storage_clk),.state_guest_advance(contb_advance),
    // No global machine/service barrier yet: keep ALL admin inactive.
    .state_freeze(1'b0),.state_guest_held(1'b0),.state_request(1'b0),
    .state_write(1'b0),.state_index(12'b0),.state_data(18'b0),
    .state_frozen(),.state_done(),.state_error(),.state_result(),''')
    bp.write_text(text)
    target=output/'src/fpga/mister/snapshot_native_conta';target.mkdir(exist_ok=False)
    new=[name for name in metadata['assets']if name not in metadata['shared_assets']]
    for name in new:shutil.copyfile(assets/name,target/name)
    shutil.copyfile(assets/'assets.json',target/'assets.json')
    qsf=output/'core/Dorado.qsf';text=qsf.read_text()
    for name in new:text+='\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(target/name)+'\n'
    qsf.write_text(text)
    changed=[];added=[]
    for path in (output/'src').rglob('*'):
        if not path.is_file():continue
        relative=path.relative_to(output/'src');old=baseline/'src'/relative
        if not old.exists():added.append(str(relative))
        elif path.read_bytes()!=old.read_bytes():changed.append(str(relative))
    if changed!=['build/rtl-executive/native16-tx-cable/dorado_native_backplane.v']:
        raise ValueError('unintended source mutation: '+str(changed))
    expected={str(Path('fpga/mister/snapshot_native_conta')/name)for name in new+['assets.json']}
    if set(added)!=expected:raise ValueError('unintended added source')
    if (output/'core/Dorado.sv').read_bytes()!=(baseline/'core/Dorado.sv').read_bytes():raise ValueError('outer clock/wrapper changed')
    for name in ('Dorado.sdc','native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc'):
        if (output/'core'/name).read_text().replace(str(output),str(baseline))!=(baseline/'core'/name).read_text():raise ValueError('timing constraints changed: '+name)
    corners=output/'core/device-corners.tcl';corners.write_text(corners.read_text().replace('native51-','native52-'))
    (output/'core/audit-control-crossings.tcl').write_text(CROSSINGS)
    record=dict(baseline=str(baseline),candidate=str(output),change=metadata['scope'],
        snapshot_admin='INACTIVE; no physical save/load',extra_timing_exceptions='None',
        source_changes=changed,added_sources=added,shared_sources=metadata['shared_assets'],
        speed='Same50MHz/SYSPER4; no speed claim. Native51 observed loader slowdown remains open.',
        source_hashes={str(path.relative_to(output)):digest(path)for path in [
            output/'core/Dorado.sv',qsf,output/'core/device-corners.tcl',output/'core/audit-control-crossings.tcl',
            *[output/'src'/name for name in changed+added]]})
    (output/'conta-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    for tool,args,label in [('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),
        ('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),
        ('quartus_sta',['-t','audit_native_clocks.tcl'],'audit'),
        ('quartus_sta',['-t','device-corners.tcl'],'device-corners'),
        ('quartus_sta',['-t','audit-control-crossings.tcl'],'control-crossings')]:
        print('Native52',label,flush=True)
        with (output/(label+'.log')).open('w')as log:
            subprocess.run([str(quartus/tool),*args],cwd=output/'core',stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Native52 full-core build terminal; inspect sources/all timing before deployment.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare-assets',type=Path);parser.add_argument('--source',type=Path);parser.add_argument('--frozen-source-root',type=Path)
    parser.add_argument('--build',action='store_true');parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native51_v3_20261019'))
    parser.add_argument('--output',type=Path);parser.add_argument('--assets',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare_assets and args.source and args.frozen_source_root:prepare_assets(args.source.resolve(),args.frozen_source_root.resolve(),args.prepare_assets.resolve())
    elif args.build and args.output and args.assets:build(args.baseline.resolve(),args.output.resolve(),args.assets.resolve(),args.quartus)
    else:parser.error('Specify --prepare-assets/--source/--frozen-source-root or --build/--output/--assets')
