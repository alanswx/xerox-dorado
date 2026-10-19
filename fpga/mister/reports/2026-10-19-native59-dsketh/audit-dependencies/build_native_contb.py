#!/usr/bin/env python3
"""Native51: integrate qualified ContB clock-enable adapters into frozen49.

Prepare assets on Mac, then run --build on misterubuntu. Snapshot admin inputs
remain INACTIVE: this tests the board in the full CPU hierarchy before any
global barrier/state transport is implemented. It cannot save/restore a live
machine and must not be presented as that feature. No FPGA is programmed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def once(text,before,after):
    if text.count(before)!=1:
        raise ValueError('unexpected frozen source shape: '+before[:100])
    return text.replace(before,after,1)


def comment_lines_removed(text):
    # Deliberately narrow: ignore only complete // comment lines and blank
    # lines. Inline comments, code whitespace, strings and every code line
    # remain byte-exact. The reviewed parity source has no block comments or
    # multi-line strings; do not generalize this to arbitrary Verilog.
    if '/*' in text or '\\\n' in text:
        raise ValueError('unreviewed comment-equivalence source shape')
    return '\n'.join(line for line in text.splitlines()
                     if line.strip() and not line.lstrip().startswith('//'))


def prepare_assets(directory,comment_reference=None):
    import audit_contb_im as im
    import generate_contb_full as gen
    directory.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate()
    (directory/'ContB_full_snapshot.sv').write_text(board)
    for name,source in clones.items():
        (directory/name).write_text(source)
    shutil.copyfile(Path(__file__).parent/'f10415_enable_bank.sv',directory/'f10415_enable_bank.sv')
    metadata=dict(board_manifest=manifest,
                  expected_original_sources={**manifest['grouping_sources'],
                      'verilog/generated/ContB-Rev-Cd.v':manifest['original_sha256']},
                  assets={p.name:digest(p) for p in directory.iterdir()},
                  builder_sha256=digest(Path(__file__)),
                  scope='Full-core normal-operation clock-enable integration only; admin inactive, no global barrier/physical state access')
    if comment_reference:
        name='verilog/cells/cell_MC10170.v'
        qualified=(im.ROOT/name).read_text()
        frozen=comment_reference.read_text()
        if comment_lines_removed(qualified)!=comment_lines_removed(frozen):
            raise ValueError('frozen parity cell has a code difference')
        metadata['comment_only_equivalence']={name:dict(
            qualified_sha256=metadata['expected_original_sources'][name],
            frozen_sha256=digest(comment_reference),
            comparison='Identical complete code lines after removing only full-line // comments and blank lines; no block comments or continued lines accepted',
            code_sha256=hashlib.sha256(comment_lines_removed(frozen).encode()).hexdigest())}
        metadata['expected_original_sources'][name]=digest(comment_reference)
    (directory/'assets.json').write_text(json.dumps(metadata,indent=2)+'\n')


def build(baseline,out,assets,quartus):
    meta=json.loads((assets/'assets.json').read_text())
    if digest(Path(__file__))!=meta['builder_sha256']:
        raise ValueError('builder differs from prepared assets')
    for name,sha in meta['assets'].items():
        if digest(assets/name)!=sha:
            raise ValueError('asset differs: '+name)
    for name,sha in meta['expected_original_sources'].items():
        if digest(baseline/'src'/name)!=sha:
            raise ValueError('qualified original differs from frozen49: '+name)
    out.mkdir(parents=True,exist_ok=False)
    shutil.copytree(baseline/'src',out/'src')
    shutil.copytree(baseline/'core',out/'core',ignore=shutil.ignore_patterns(
        'db','incremental_db','output_files','*.rpt','*.summary','*.log','c5_pin_model_dump.txt'))
    for p in (out/'core').rglob('*'):
        if p.is_file() and p.suffix in ('.qsf','.qip','.sdc','.tcl','.qpf'):
            p.write_text(p.read_text().replace(str(baseline),str(out)))
    generated=out/'src/build/rtl-executive/native16-tx-cable'
    p=generated/'dorado_native_backplane.v'
    s=p.read_text()
    # The file also contains an unused probe/test wrapper with a similar
    # storage port. Restrict this edit to the active backplane's unique header.
    s=once(s,'    output wire [31:0] receive_debug,\n    input wire storage_clk, storage_reset,',
           '    output wire [31:0] receive_debug,\n    input wire contb_advance,\n    input wire storage_clk, storage_reset,')
    s=once(s,'ContB_m_Rev_m_Cd b_ContB (', '''ContB_full_snapshot_experiment b_ContB (
    .state_host_clk(storage_clk), .state_guest_advance(contb_advance),
    // No whole-machine barrier yet: keep ALL state access inactive.
    .state_freeze(1'b0), .state_guest_held(1'b0), .state_request(1'b0),
    .state_write(1'b0), .state_index(15'b0), .state_data(18'b0),
    .state_frozen(), .state_done(), .state_error(), .state_result(),''')
    p.write_text(s)
    p=generated/'dorado_native_machine.sv'
    s=p.read_text()
    s=once(s,'    input wire sys_clk,clk_host,reset,baseboard_boot_n,large_format,',
           '    input wire contb_advance,\n    input wire sys_clk,clk_host,reset,baseboard_boot_n,large_format,')
    s=once(s,'dorado_native_backplane #(.SYSPER(SYSPER)) m (',
           'dorado_native_backplane #(.SYSPER(SYSPER)) m (\n    .contb_advance(contb_advance),')
    p.write_text(s)
    p=out/'core/Dorado.sv'
    s=p.read_text()
    if s.count('always @(negedge clk_sys) network_step <= machine_enable;')!=1 or s.count('.ena(machine_enable)')!=1:
        raise ValueError('original clock gate/actual advance contract differs')
    s=once(s,'dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (',
           'dorado_native_machine #(.SYSPER(MACHINE_SYSPER)) u_native (\n    .contb_advance(network_step),')
    p.write_text(s)
    target=out/'src/fpga/mister/snapshot_native'
    target.mkdir(exist_ok=False)
    for name in meta['assets']:
        shutil.copyfile(assets/name,target/name)
    (target/'assets.json').write_text(json.dumps(meta,indent=2)+'\n')
    p=out/'core/Dorado.qsf'
    s=p.read_text()
    for name in meta['assets']:
        s+=f'\nset_global_assignment -name SYSTEMVERILOG_FILE {target/name}\n'
    p.write_text(s)
    changed=[]
    added=[]
    for p in (out/'src').rglob('*'):
        if not p.is_file():
            continue
        relative=p.relative_to(out/'src')
        old=baseline/'src'/relative
        if not old.exists():
            added.append(str(relative))
        elif p.read_bytes()!=old.read_bytes():
            changed.append(str(relative))
    expected=['build/rtl-executive/native16-tx-cable/dorado_native_backplane.v',
              'build/rtl-executive/native16-tx-cable/dorado_native_machine.sv']
    if sorted(changed)!=sorted(expected):
        raise ValueError('unintended source changes: '+str(changed))
    if set(added)!={str(Path('fpga/mister/snapshot_native')/n)for n in (*meta['assets'],'assets.json')}:
        raise ValueError('unintended added sources: '+str(added))
    for name in ('native_timing.sdc','sdram_timing.sdc','checkpoint_timing.sdc','Dorado.sdc'):
        if (out/'core'/name).read_text().replace(str(out),str(baseline))!=(baseline/'core'/name).read_text():
            raise ValueError('timing constraints changed: '+name)
    p=out/'core/device-corners.tcl'
    p.write_text(p.read_text().replace('native49-','native51-'))
    record=dict(baseline=str(baseline),candidate=str(out),change=meta['scope'],
                source_changes=changed,added_sources=added,
                extra_timing_exceptions='None',snapshot_admin='INACTIVE; no physical save/load',
                speed='UNCHANGED50MHz/SYSPER4; no measured speed claim',
                source_hashes={str(p.relative_to(out)):digest(p) for p in [
                    out/'core/Dorado.sv',out/'core/Dorado.qsf',*(out/'src'/n for n in changed+added)]})
    (out/'contb-integration.json').write_text(json.dumps(record,indent=2)+'\n')
    for exe,args,label in (
        ('quartus_map',['Dorado'],'map'),('quartus_fit',['Dorado'],'fit'),
        ('quartus_asm',['Dorado'],'asm'),('quartus_sta',['Dorado'],'sta'),
        ('quartus_sta',['-t','audit_native_clocks.tcl'],'audit'),
        ('quartus_sta',['-t','device-corners.tcl'],'device-corners'),
    ):
        print('Native51',label,flush=True)
        with (out/(label+'.log')).open('w')as log:
            subprocess.run([str(quartus/exe),*args],cwd=out/'core',
                           stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Native51 build terminal; inspect whole-core timing before deployment.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare-assets',type=Path)
    parser.add_argument('--comment-reference',type=Path,
                        help='Reviewed frozen49 MC10170 source; permit full-line-comment differences only')
    parser.add_argument('--build',action='store_true')
    parser.add_argument('--baseline',type=Path,default=Path('/home/alans/mister/Dorado_Native49_20261019'))
    parser.add_argument('--output',type=Path)
    parser.add_argument('--assets',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare_assets:
        prepare_assets(args.prepare_assets.resolve(),args.comment_reference)
    elif args.build and args.output and args.assets:
        build(args.baseline.resolve(),args.output.resolve(),args.assets.resolve(),args.quartus.resolve())
    else:
        parser.error('use --prepare-assets DIRECTORY or --build --output NEW_DIRECTORY --assets DIRECTORY')
