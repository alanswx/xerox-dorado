#!/usr/bin/env python3
"""Add passive first-error visibility to the exact reviewed raw-BL8 stage.

Only diagnostic ports/wires, a one-shot observer, command70's unused selector
pages and the helper source assignment differ. No guest state/control changes.
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OBSERVER = 'src/fpga/mister/dorado_boot_fault_monitor.sv'
CONTB = 'src/fpga/mister/snapshot_native/ContB_full_snapshot.sv'
BACKPLANE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
WRAPPER = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
TOP = 'core/Dorado.sv'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def substitute(raw, old, new, name):
    assert raw.count(old.encode()) == 1, (name, old, raw.count(old.encode()))
    return raw.replace(old.encode(), new.encode())


def expose(name, raw):
    if name == CONTB:
        raw = substitute(raw, '    input  wire sys_clk,', '    output wire [15:0] boot_error_sources,\n    input  wire sys_clk,', name)
        assignment = '''
  // Read-only ContB09 pins; no changes to any error enable or driver.
  assign boot_error_sources={IMLHPEenable,IMRHPEenable,IOPEenable,
    RamPEenable,MemPEenable,MDPEenable,~IMLHPE_p_,~IMRHPE_p_,
    IOPE,RamPE,MemPE,MdPE,IMLHPEenable_p_,IMRHPEenable_p_,
    ContB09_sil_pl_4,ContB09_sil_pl_2};
'''
        raw = substitute(raw, '  cell_MC10105 u_j03 (', assignment + '  cell_MC10105 u_j03 (', name)
    elif name == BACKPLANE:
        # Only the primary backplane is instantiated by the native wrapper.
        # Leave the unused convenience machine module unchanged.
        head, tail = raw.split(b'module dorado_native_backplane_machine', 1)
        head = substitute(head, '    output wire [31:0] processor_debug,', '    output wire [15:0] boot_error_sources,\n    output wire [31:0] processor_debug,', name)
        head = substitute(head, '  ContB_full_snapshot_experiment b_ContB (', '  ContB_full_snapshot_experiment b_ContB (\n    .boot_error_sources(boot_error_sources),', name)
        raw = head + b'module dorado_native_backplane_machine' + tail
    elif name == WRAPPER:
        raw = substitute(raw, '    output wire [31:0] baseboard_debug,processor_debug,', '    output wire [15:0] boot_error_sources,\n    output wire [31:0] baseboard_debug,processor_debug,', name)
        raw = substitute(raw, '  dorado_native_backplane #(.SYSPER(SYSPER)) m (', '  dorado_native_backplane #(.SYSPER(SYSPER)) m (\n    .boot_error_sources(boot_error_sources),', name)
    elif name == TOP:
        raw = substitute(raw, 'wire [31:0] baseboard_debug,processor_debug;', '''wire [31:0] baseboard_debug,processor_debug;
wire [15:0] boot_error_sources;
wire [127:0] boot_fault_observation;
dorado_boot_fault_monitor u_boot_fault_monitor(.clk(clk_sys),
    .error_n(control_debug[8]),.sources(boot_error_sources),
    .processor(processor_debug),.observation(boot_fault_observation));
wire boot_fault_view=status[15] && status[14:11]<4;
wire [31:0] boot_fault_page=boot_fault_observation[32*status[12:11]+:32];''', name)
        raw = substitute(raw, '    .baseboard_debug(baseboard_debug),', '    .boot_error_sources(boot_error_sources),\n    .baseboard_debug(baseboard_debug),', name)
        raw = substitute(raw, '.machine_debug(pack_fault ? pack_fault_debug : speed_monitor_active ? count_debug : native_key_overflow ?', '.machine_debug(pack_fault ? pack_fault_debug : boot_fault_view ? boot_fault_page : speed_monitor_active ? count_debug : native_key_overflow ?', name)
    return raw


def stage(parent, qualification, out, project):
    assert not out.exists() and project.is_absolute()
    manifest = json.loads((parent / 'sdram-burst-native.json').read_text())
    q = json.loads(qualification.read_text())
    assert q['status'] == 'PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW'
    assert q['source_hashes'] == manifest['source_hashes'] and len(q['source_hashes']) == 499
    output = {}
    for name, digest in manifest['source_hashes'].items():
        raw = (parent / name).read_bytes()
        assert sha(raw) == digest, name
        if Path(name).suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            raw = raw.replace(manifest['project'].encode(), str(project).encode())
        output[name] = expose(name, raw)
    output[OBSERVER] = (ROOT / 'fpga/mister/dorado_boot_fault_monitor.sv').read_bytes()
    output['core/Dorado.qsf'] += ('\n# Passive original boot first-error diagnostic.\n'
        'set_global_assignment -name SYSTEMVERILOG_FILE ' + str(project / OBSERVER) + '\n').encode()
    out.mkdir(parents=True)
    for name, raw in output.items():
        dest = out / name
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(raw)
    record = dict(manifest, project=str(project), passive_boot_fault_observer=True,
        observer_parent_manifest_sha256=sha((parent / 'sdram-burst-native.json').read_bytes()),
        observer_parent_qualification_sha256=sha(qualification.read_bytes()),
        observer_deltas=[CONTB,BACKPLANE,WRAPPER,TOP,OBSERVER,'core/Dorado.qsf'],
        source_hashes={n:sha(v) for n,v in output.items()},
        status='STAGED_PASSIVE_BOOT_FAULT_OBSERVER_REVIEW_REQUIRED',assembly_permitted=False)
    (out / 'sdram-burst-native.json').write_text(json.dumps(record,indent=2)+'\n')
    for name in [Path(__file__).name,'build_r_front_ready_native.py']:
        (out / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    print(record['status'],len(output),'exact staged inputs')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent','qualification','output','project']:
        parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    stage(args.parent.resolve(),args.qualification.resolve(),args.output.resolve(),args.project)
