#!/usr/bin/env python3
"""Isolated zero-reply asynchronous-RAM audit; no whole-core qualification."""
import argparse
import json
from pathlib import Path
import re
import shutil
import tempfile
import fit_riot
import generate_f10145_zero_write as gen
from audit_baseboard_full import table


def audit(root):
    result = fit_riot.audit(root)
    record = json.loads((root/'provenance.json').read_text())
    assert record['private_zero_write_reply'] and record['original_async_mlab_style']
    result['physical_state'] = {}
    for variant, mlab in [('baseline', False), ('snapshot', True)]:
        text, manifest = gen.generate(mlab=mlab, async_mlab=mlab)
        case = root/variant
        assert (case/'cell_F10145A_snapshot.sv').read_bytes() == text.encode(), 'candidate source differs'
        assert manifest['logical_state_bits'] == 74
        mapping = (case/'snapshot_fit.map.rpt').read_text(encoding='latin1')
        rows = table(mapping, 'Analysis & Synthesis Resource Utilization by Entity', 'Compilation Hierarchy Node')
        cell = [r for r in rows if r['Entity Name'] == 'cell_F10145A_snapshot']
        assert len(cell) == 1
        expected = 23 if mlab else 87
        total = 31 if mlab else 87
        assert cell[0]['Dedicated Logic Registers'] == f'{total} ({expected})', 'retained control/state bits differ'
        result['physical_state'][variant] = dict(logical_ram_bits=64, write_window_bits=10,
                                                  private_interface_bits=13, owned_registers=expected,
                                                  total_synthesis_registers=total,
                                                  inferred_ram_input_registers=total-expected)
        if mlab:
            fit = (case/'snapshot_fit.fit.rpt').read_text(encoding='latin1')
            memories = table(fit, 'Fitter RAM Summary', 'Name')
            assert len(memories) == 2, 'two synchronized read replicas required'
            locations = set()
            for row in memories:
                for key, value in {'Type':'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
                    'Port A Depth':'16','Port B Depth':'16','Port A Width':'4','Port B Width':'4',
                    'Port A Input Registers':'yes','Port A Output Registers':'no',
                    'Port B Input Registers':'no','Port B Output Registers':'no',
                    'Size':'64','Implementation Bits':'64','M10K blocks':'0','MLAB cells':'4'}.items():
                    assert row[key] == value, 'RAM geometry/latency differs: '+key
                assert re.fullmatch(r'LAB_X\d+_Y\d+_N0', row['Location']), 'RAM has no legal location'
                locations.add(row['Location'])
            assert len(locations) == 2 and re.search(r'; Total MLAB memory bits\s*; 128\s*;', fit), 'MLAB storage missing'
            assert 'is uninferred' not in (case/'map.log').read_text(), 'requested RAM not inferred'
            result['physical_state'][variant].update(memory=memories, physical_replica_bits=128)
    result['scope'] = (__doc__+' All74logical bits remain:64RAM bits in two same-write replicas and10window '
        'bits, plus13private interface registers. Registered administrative reads exclude writes; '
        'private writes/public fabric return zero. Ordinary output is asynchronous. Original no_rw_check '
        'placement hint is used; no SDC cuts/clock/latency changes. Actual full-core timing/boot still required.')
    return result


def controls(root):
    result = []
    for name, path, before, after in [
        ('source','snapshot/cell_F10145A_snapshot.sv','state_accept && !state_write && state_index','state_accept && state_index'),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('state','snapshot/snapshot_fit.map.rpt','31 (23)','30 (22)'),
        ('memory','snapshot/snapshot_fit.fit.rpt','128','127'),
        ('latency','snapshot/snapshot_fit.fit.rpt','; no                     ; no                      ; 64', '; yes                    ; no                      ; 64'),
    ]:
        with tempfile.TemporaryDirectory(prefix='f10145-zero-async-', dir='/tmp') as temp:
            out = Path(temp)/'fit'
            shutil.copytree(root, out, ignore=shutil.ignore_patterns('db','incremental_db'))
            p = out/path
            text = p.read_text(encoding='latin1')
            assert before in text, 'missing damaged-fit target: '+name
            p.write_text(text.replace(before,after), encoding='latin1')
            try:
                audit(out)
            except AssertionError as e:
                result.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:
                raise AssertionError('damaged asynchronous RAM evidence accepted: '+name)
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory',type=Path)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    r = audit(a.directory.resolve())
    r['failure_controls'] = controls(a.directory.resolve())
    a.output.write_text(json.dumps(r,indent=2)+'\n')
    print('ZERO-REPLY ASYNC RAM AUDIT PASS:74logical bits/128replica RAM bits/23regs; '
          f"ALM delta {r['delta']['alms']};minimum {r['minimum_snapshot_slack_ns']}ns;6bad controls reject")
