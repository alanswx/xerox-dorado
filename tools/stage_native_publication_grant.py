#!/usr/bin/env python3
"""Factor a redundant publication check only behind the native readiness grant.

The standalone caller defaults to its independent readiness barrier. Only the
converted native backplane opts in: its caller advance is the shell's reset,
pack, storage and control-qualified grant. All data and RAM equations remain
unchanged. This is a diagnostic stage, never permission to assemble or load.
"""
import argparse
import json
from pathlib import Path
import shutil

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT = Path(__file__).resolve().parents[1]
CALLER = 'src/verilog/rtl/caller.sv'
BACKPLANE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
MACHINE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
PARAMETER = 'parameter integer EXTERNAL_READY_GRANT=0'


def caller(raw):
    changes = [
        ('module dorado_r_front_owned_packet(',
         'module dorado_r_front_owned_packet #('+PARAMETER+')('),
        ('if(contb_advance && control_ready)begin', 'if(owner_commit)begin'),
        ('assign owner_commit=contb_advance && control_ready;',
         'assign owner_commit=EXTERNAL_READY_GRANT ? contb_advance : contb_advance && control_ready;'),
    ]
    original = raw
    for before, after in changes:
        assert raw.count(before) == 1, before
        raw = raw.replace(before, after, 1)
    restored = raw
    for before, after in reversed(changes):
        restored = restored.replace(after, before, 1)
    assert restored == original
    return raw


def backplane(raw):
    old = module(raw, 'dorado_native_backplane_ce')
    before = 'dorado_r_front_owned_packet r_front_dispatch('
    after = 'dorado_r_front_owned_packet #(.EXTERNAL_READY_GRANT(1)) r_front_dispatch('
    assert old.count(before) == 1
    new = old.replace(before, after, 1)
    assert new.replace(after, before, 1) == old
    return raw.replace(old, new, 1)


def binding(stage):
    """Bind the contract to actual shell, wrapper, backplane and caller text."""
    shell = (stage/'core/Dorado.sv').read_text()
    machine = module((stage/MACHINE).read_text(), 'dorado_native_machine_ce')
    bp = module((stage/BACKPLANE).read_text(), 'dorado_native_backplane_ce')
    cp = (stage/CALLER).read_text()
    checks = [
        (shell, 'wire machine_enable = ~reset && pack_ready && storage_ready && control_ready;'),
        (shell, 'wire network_step = machine_enable;'),
        (shell, 'assign machine_clk = clk_sys;'),
        (shell, 'dorado_native_machine_ce #(.SYSPER(MACHINE_SYSPER)) u_native (\n    .contb_advance(network_step),.control_ready(control_ready),'),
        (machine, 'dorado_native_backplane_ce #(.SYSPER(SYSPER)) m (.guest_advance(contb_advance),\n    .boot_error_sources(boot_error_sources),\n    .contb_advance(contb_advance),'),
        (machine, '.control_ready(control_ready)'),
        (bp, 'dorado_r_front_owned_packet #(.EXTERNAL_READY_GRANT(1)) r_front_dispatch(\n   .storage_clk(storage_clk),.contb_advance(contb_advance),.use_pair(1\'b0),'),
        (bp, '.first_result(r_first_result),.last_result(r_last_result),.control_ready(control_ready),'),
        (cp, PARAMETER),
        (cp, 'assign owner_commit=EXTERNAL_READY_GRANT ? contb_advance : contb_advance && control_ready;'),
        (cp, 'always @(posedge storage_clk)if(owner_commit)begin'),
    ]
    for raw, text in checks:
        assert raw.count(text) == 1, text
    # The real QSF must select the native SDRAM branch containing this grant.
    qsf = (stage/'core/Dorado.qsf').read_text()
    for flag in ['DORADO_NATIVE', 'DORADO_STORAGE_SDRAM']:
        assert qsf.splitlines().count('set_global_assignment -name VERILOG_MACRO '+flag) == 1, flag
    return dict(chain=['emu.network_step', 'u_native.contb_advance',
                       'u_native.m.contb_advance', 'u_native.m.r_front_dispatch.contb_advance'],
                native_parameter=1, standalone_parameter_default=0,
                guard='~reset && pack_ready && storage_ready && control_ready',
                source_hashes={n:sha(stage/n) for n in [CALLER, BACKPLANE, MACHINE, 'core/Dorado.sv', 'core/Dorado.qsf']})


def prepare(parent, out, project):
    assert not out.exists() and project.parent == Path('/home/alans/mister')
    m = json.loads((parent/'r-front-control-native.json').read_text())
    verify(parent, m['source_hashes'])
    assert m['clock_enable_conversion_staged'] and m['compact_operand_tag_qualified']
    assert m['operand_view_tag_support_bits'] == 465 and not m['assembly_permitted']
    output = {n:(parent/n).read_bytes() for n in m['source_hashes']}
    output[CALLER] = caller(output[CALLER].decode()).encode()
    output[BACKPLANE] = backplane(output[BACKPLANE].decode()).encode()
    for n, raw in output.items():
        if Path(n).suffix in ['.qsf', '.qip', '.qpf', '.sdc', '.tcl']:
            output[n] = raw.replace(m['project'].encode(), str(project).encode())
    out.mkdir(parents=True)
    for n, raw in output.items():
        p = out/n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    for name in [Path(__file__).name, 'build_r_front_ready_native.py']:
        shutil.copy2(ROOT/'tools'/name, out/name)
    bound = binding(out)
    result = {**m, 'scope':__doc__, 'project':str(project), 'parent_project':m['project'],
              'parent_stage':str(parent), 'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
              'source_hashes':{n:sha(out/n) for n in output},
              'shared_publication_grant_staged':True, 'publication_grant_binding':bound,
              'incremental_functional_HDL_changes':[CALLER, BACKPLANE],
              'publication_grant_proof_required':True, 'assembly_permitted':False,
              'physical_timing_qualified':False, 'firmware_qualified':False, 'hardware_speedup_qualified':False}
    differences = {n for n,h in m['source_hashes'].items()
                   if Path(n).suffix in ['.sv','.v'] and result['source_hashes'][n] != h}
    assert differences == {CALLER, BACKPLANE} and len(output) == 578
    (out/'r-front-control-native.json').write_text(json.dumps(result, indent=2)+'\n')
    print('STAGED_NATIVE_SHARED_PUBLICATION_GRANT', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent', 'output', 'project']:
        p.add_argument('--'+n, type=Path, required=True)
    a = p.parse_args()
    prepare(a.parent.resolve(), a.output.resolve(), a.project)
