#!/usr/bin/env python3
"""Stage tested HIGH-transparent l06/l07 windows in the private native core.

Keep all physical state/RAM/visible-R owners and 57.142857 MHz/SYSPER=4.
The same eight flag bits now store closed values; board ports see physical Q.
No new state, reset, guest cycle, constraint, assembly or firmware permission.
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(parent, runs, out, project):
    assert not out.exists() and project.is_absolute() and len(runs) == 2
    original = json.loads((parent / 'r-front-control-native.json').read_text())
    assert original['physical_Q_export_separate_from_closed_storage']
    assert original['canonical_visible_R_owners'] == 16 and not original['assembly_permitted']
    assert original['frequency_hz'] == 57142857 and original['sysper'] == 4
    for n, h in original['source_hashes'].items():
        assert sha(parent / n) == h, n
    gates, changed = [], {}
    for engine, d in zip(['icarus', 'verilator'], runs):
        r = json.loads((d / 'run-state.json').read_text())
        assert r['status'] == 'PASS_R_FRONT_FLAG_WINDOWS' and r['engine'] == engine and not r.get('mutation')
        assert r['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
        assert r['actions'][-1]['returncode'] == 0 and r['all_original_checks_preserved'] and r['pin_checks_added']
        assert r['closed_encoding_bits'] == 88 and r['actual_after_clock_levels_exported']
        for row in r['source_files']:
            p = d / row['path']
            assert sha(p) == row['output_sha256']
            if row['changed'] and p.name != 'tb_r_front_local.sv':
                if p.name in changed:
                    assert sha(changed[p.name]) == sha(p)
                changed[p.name] = p
        helper = d / 'dorado_r_flag_window_step.sv'
        assert sha(helper) == r['helper_sha256']
        if helper.name in changed:
            assert sha(changed[helper.name]) == sha(helper)
        changed[helper.name] = helper
        unit = ROOT / f'build/rtl-executive/r-flag-window-{engine}-v2-20261023'
        u = json.loads((unit / 'run-state.json').read_text())
        assert u['status'] == 'PASS_R_FLAG_WINDOW' and not u['mutation']
        assert sha(unit / helper.name) == sha(helper)
        for mutation in ['omit_close', 'ignore_set', 'set_over_clock', 'omit_new_set']:
            bad = ROOT / f'build/rtl-executive/r-flag-window-{mutation}-{engine}-v2-20261023'
            b = json.loads((bad / 'run-state.json').read_text())
            assert b['status'] == 'R_FLAG_WINDOW_FAILED' and b['actions'][0]['returncode'] == 0 and b['actions'][-1]['returncode'] != 0
            assert 'R flag ' in (bad / 'test.log').read_text()
        for mutation, check in [('omit_flag_close', 15), ('hide_open_flag', 14)]:
            bad = ROOT / f'build/rtl-executive/r-front-flag-windows-{mutation}-{engine}-v3-20261023'
            b = json.loads((bad / 'run-state.json').read_text())
            assert b['status'] == 'R_FRONT_FLAG_WINDOWS_FAILED' and b['actions'][-1]['returncode'] != 0
            assert f'R front state mismatch check={check}' in (bad / 'test.log').read_text()
        gates.append(dict(directory=str(d), engine=engine, run_state_sha256=sha(d / 'run-state.json'),
                          unit_run_state_sha256=sha(unit / 'run-state.json')))
    assert len(changed) == 8
    data = {}
    for n in original['source_hashes']:
        raw = (parent / n).read_bytes()
        if Path(n).suffix in {'.qsf', '.qip', '.qpf', '.sdc', '.tcl'}:
            raw = raw.replace(original['project'].encode(), str(project).encode())
        data[n] = raw
    for name, p in changed.items():
        target = 'src/verilog/rtl/' + ('dorado_r_front_control_step.sv' if name == 'candidate.sv' else name)
        assert target in data or name == 'dorado_r_flag_window_step.sv'
        data[target] = p.read_bytes()
    extra = list(original['extra_RTL_sources']) + ['src/verilog/rtl/dorado_r_flag_window_step.sv']
    assert len(extra) == len(set(extra)) == 30
    data['core/Dorado.qsf'] += f'\nset_global_assignment -name SYSTEMVERILOG_FILE {project}/src/verilog/rtl/dorado_r_flag_window_step.sv\n'.encode()
    out.mkdir(parents=True)
    for n, raw in data.items():
        p = out / n;p.parent.mkdir(parents=True, exist_ok=True);p.write_bytes(raw)
    r = dict(original)
    r.update(scope=__doc__, project=str(project), source_hashes={n: sha(out / n) for n in data},
             parent_stage=str(parent), parent_manifest_sha256=sha(parent / 'r-front-control-native.json'),
             extra_RTL_sources=extra, physical_flag_windows=gates, closed_latch_encoding_bits=88,
             original_flag_clock_and_set_levels_exported=True, all_eight_flag_pin_views_checked=True,
             source_pause_contract_qualified=False, native_consumer_setup_hold_qualified=False,
             physical_timing_qualified=False, firmware_qualified=False, assembly_permitted=False,
             future60ns_partition_qualified=False, speedup_qualified=False)
    (out / 'r-front-control-native.json').write_text(json.dumps(r, indent=2) + '\n')
    print('STAGED_PHYSICAL_FLAG_NATIVE_EXPERIMENT')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent', 'icarus', 'verilator', 'output', 'project']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    prepare(a.parent.resolve(), [a.icarus.resolve(), a.verilator.resolve()], a.output.resolve(), a.project)
