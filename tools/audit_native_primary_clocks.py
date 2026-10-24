#!/usr/bin/env python3
"""Bind every active native crystal and audit combined15ns event capacity.

Compare an integer phase-accumulator schedule with an independent absolute
Fraction/deadline schedule and original four-flop CPU divider, across the
complete300us joint period and all eight CPU rotations. Retain counterexamples
to four-record capacity and the1.25ns-only grid. This is source/schedule
evidence, not an integrated board consumer, FPGA timing or firmware pass.
Ethernet retains the current23530kHz model value; no silent frequency change.
"""
import argparse
from collections import Counter, defaultdict
from fractions import Fraction
import json
from pathlib import Path
import re
import shutil

from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
UNITS_PER_NS = 9412  # lcm(4,2353); Ether half-period=50000/2353ns.
INTERVAL = 15*UNITS_PER_NS
CPU_SUBSTEP = 15*UNITS_PER_NS//4
HALF_PERIODS = [10*UNITS_PER_NS, 50*UNITS_PER_NS, 200000]
TICKS = 20000  #300us: all source phases and levels return to their starts.
FILES = {
    'machine.sv': 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv',
    'backplane.v': 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v',
    'baseboard.v': 'src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v',
    'baseboard-crystal.v': 'src/verilog/cells/cell_MC12061.v',
    'dsketh.sv': 'src/fpga/mister/snapshot_native_dsketh/DskEth_full_snapshot.sv',
    'dispy.sv': 'src/fpga/mister/snapshot_native_dispy/DispY_full_snapshot.sv',
}


def divider_step(s):
    # Independent original four-flop/reference VCO step, not scp/ecp truth maps.
    n = list(s)
    n[4] = 1-s[4]
    if s[4]:
        n[1], n[0] = s[3], s[1]
    else:
        n[3], n[2] = 1-s[2], s[3]
    return n


def divider_levels(s):
    scp, ecp = 1-s[3], s[0]
    return scp | (ecp << 1) | ((scp | ecp) << 2)


def mapped_levels(phase):
    scp = int(phase < 2 or phase >= 6)
    ecp = int(phase == 0 or phase >= 5)
    return scp | (ecp << 1) | ((scp | ecp) << 2)


def absolute_reference(rotation):
    by_time = defaultdict(int)
    s = [1, 0, 0, 0, 1]
    for _ in range(rotation):
        s = divider_step(s)
    initial = previous = divider_levels(s)
    for step in range(1, TICKS*4+1):
        s = divider_step(s)
        level = divider_levels(s)
        if level != previous:
            by_time[Fraction(15*step, 4)] |= level ^ previous
        previous = level
    for lane, half in [(3, Fraction(10)), (4, Fraction(50)), (5, Fraction(50000, 2353))]:
        for edge in range(1, int(Fraction(TICKS*15)/half)+1):
            by_time[edge*half] |= 1 << lane
    buckets = [[] for _ in range(TICKS)]
    for t, mask in sorted(by_time.items()):
        tick = (t.numerator-1)//(15*t.denominator)
        offset = (t-15*tick)*UNITS_PER_NS
        assert offset.denominator == 1 and 0 < offset <= INTERVAL
        buckets[tick].append((int(offset), mask))
    return initial, buckets


def audit(stage, out):
    assert not out.exists()
    m = json.loads((stage/'r-front-control-native.json').read_text())
    for name, digest in m['source_hashes'].items():
        assert sha(stage/name) == digest
    sources = {name: (stage/path).read_text() for name, path in FILES.items()}
    machine = re.sub(r'\s+', '', sources['machine.sv'])
    assert 'cell_K1115A#(.FREQ_KHZ(50000),.SYSPER(2*SYSPER))u_pixclk' in machine
    assert '.RawPixelClk(pixel_clk)' in machine and '.Crystal(' not in machine
    assert 'cell_MC12061 #(.SYSPER(2*SYSPER)) u_d08' in sources['baseboard.v']
    assert 'parameter integer FREQ_KHZ = 10000' in sources['baseboard-crystal.v']
    assert '.p10(Basebd01_sil_pl_4)' in sources['baseboard.v']
    assert 'cell_K1115A_snapshot #(.FREQ_KHZ(23530), .SYSPER(2*SYSPER)) u_j20' in sources['dsketh.sv']
    assert '.p8(EClk0)' in sources['dsketh.sv'] and '.p7(EClk0)' in sources['dsketh.sv']
    assert 'cell_K1115A_snapshot #(.FREQ_KHZ(50000)) u_a05' in sources['dispy.sv']
    assert 'assign Crystal = Crystal__DispY;' in sources['backplane.v']
    out.mkdir(parents=True)
    source_hashes = {}
    for name, path in FILES.items():
        shutil.copy2(stage/path, out/name)
        source_hashes[name] = sha(out/name)
    for name, path in [('cpu-pixel-source.sv', ROOT/'verilog/rtl/dorado_clock_event_batch.sv'),
                       ('original-divider-reference.py', ROOT/'tools/prove_clock_event_batch.py')]:
        shutil.copy2(path, out/name)
        source_hashes[name] = sha(out/name)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    rotations = []
    for rotation in range(8):
        initial, expected = absolute_reference(rotation)
        assert initial == mapped_levels(rotation)
        phase, phases, level = rotation, [0, 0, 0], initial
        histogram = Counter()
        first_overflow = first_maximum = None
        for tick in range(TICKS):
            records = defaultdict(int)
            cpu = mapped_levels(phase)
            for substep in range(1, 5):
                phase = (phase+1) % 8
                after = mapped_levels(phase)
                if cpu != after:
                    records[substep*CPU_SUBSTEP] |= cpu ^ after
                cpu = after
            for lane, half in enumerate(HALF_PERIODS, 3):
                remaining = INTERVAL
                offset = 0
                while phases[lane-3]+remaining >= half:
                    delta = half-phases[lane-3]
                    offset += delta
                    records[offset] |= 1 << lane
                    remaining -= delta
                    phases[lane-3] = 0
                phases[lane-3] += remaining
            actual = sorted(records.items())
            assert actual == expected[tick], (rotation, tick, actual, expected[tick])
            assert len(actual) <= 6
            histogram[len(actual)] += 1
            witness = dict(tick=tick, start_ns=tick*15, records=[dict(offset_units=t, changed_lanes=mask) for t, mask in actual])
            if len(actual) > 4 and first_overflow is None:
                first_overflow = witness
            if len(actual) == 6 and first_maximum is None:
                first_maximum = witness
            for _, mask in actual:
                level ^= mask
        assert phase == rotation and phases == [0, 0, 0] and level == initial
        rotations.append(dict(rotation=rotation, intervals=TICKS, histogram=dict(sorted(histogram.items())),
            first_four_record_overflow=first_overflow, first_six_record_interval=first_maximum,
            all_offsets_masks_and_returned_phase_match=True))
    assert any(r['first_four_record_overflow'] is not None for r in rotations)
    maximum = max(int(count) for r in rotations for count in r['histogram'])
    assert maximum == 5
    first_ether = Fraction(50000, 2353)
    assert (first_ether/Fraction(5, 4)).denominator != 1
    result = dict(status='PASS_SOURCE_BOUND_ALL_PRIMARY_CLOCK_SCHEDULE_AUDIT',
        stage=str(stage), stage_manifest_sha256=sha(stage/'r-front-control-native.json'), source_hashes=source_hashes,
        actual_active_sources={'CPU': 'Original BaseBoard SCP/ECP divider, 3.75ns substeps',
            'pixel': 'Native machine u_pixclk, 50000kHz; RawPixelClk',
            'BaseBoard': 'MC12061 d08, 10000kHz; p10 feeds the original e07/g08 divider/6502',
            'Ethernet': 'DskEth j20, existing23530kHz model; EClk0 -> i20 -> EClk1/EClk2'},
        excluded_source='DispY a05 feeds the Crystal connector; actual native wrapper leaves it unused.',
        rotations=rotations, compared_intervals=TICKS*8, joint_period_ns=TICKS*15,
        exact_units_per_ns=UNITS_PER_NS, interval_units=INTERVAL,
        required_record_capacity=maximum, source_level_lanes=6,
        cold_BaseBoard_edges_coincide_with_pixel=True,
        six_records_conservative_bound_for_independent_crystal_phases=True,
        narrow_CPU_pixel_four_record_capacity_insufficient_for_complete_machine=True,
        first_Ethernet_edge_ns=str(first_ether), first_Ethernet_edge_units=200000,
        one_point_25ns_grid_inexact_for_current_Ethernet_model=True,
        reference_methods=['Absolute Fraction deadlines and original four-flop divider', 'Integer phase accumulators and CPU truth maps'],
        Ethernet_frequency_changed=False, native_RTL_or_clocks_changed=False,
        source_schedule_only=True, native_consumers_integrated=False,
        FPGA_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k: result[k] for k in ['status', 'compared_intervals', 'required_record_capacity', 'exact_units_per_ns']}), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    audit(a.stage.resolve(), a.output.resolve())
