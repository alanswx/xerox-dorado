#!/usr/bin/env python3
"""Check the direct phase contract against Xerox's original ContA wire list.

Seven instruction-phase Qs, their combinational D/initialization cone and the
clock fanout's pin connections are covered. Run/Hold/Freeze state, physical
fanout delays, SC&NoFreeze and CRamClock are not translated by this check.
"""
from pathlib import Path
from sil_netlist import load_board

ROOT = Path(__file__).resolve().parents[1]


def check():
    path = ROOT / 'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'
    board = load_board(str(path))
    pins = {(p['pkg'], p['pin']): name
            for name, net in board.nets.items() for p in net['pins']}
    expected = {
        'k03': ('MC10231', {10: 'dStartCycle', 11: "preclk2'Bb", 13: 'sPhase0',
                            14: "StartCycle'a", 15: 'preStartCyclea'}),
        'k04': ('MC10231', {9: "clk2'Be", 10: 'dStartCycle', 13: 'sPhase0',
                            14: "StartCycle'", 15: 'StartCycle'}),
        'l07': ('MC10231', {2: 'Phase1', 4: 'sPhase0', 7: 'ContA27.sil+2',
                            9: "clk2'Bf", 10: 'StartCycle', 12: 'sPhase0',
                            14: "Phase0'", 15: 'Phase0'}),
        'k05': ('MC10231', {2: 'Phase3', 4: 'sPhase0', 7: 'Phase2',
                            9: "clk2'Be", 10: 'Phase1', 13: 'sPhase0',
                            14: "Phase2'", 15: 'Phase2'}),
        'j01': ('MC10231', {10: 'Phase3', 11: "clk2'Bc", 13: 'sPhase0',
                            14: "Phase4'", 15: 'Phase4'}),
        'j05': ('MC10117', {2: 'ContA27.sil+1', 3: 'dStartCycle',
                            4: 'RWTPCorRWIM', 5: "Phase0'", 7: "Phase4'"}),
        'i04': ('MC10104', {3: 'ContA27.sil+2', 4: 'Phase0',
                            6: 'ContA27.sil+1', 7: 'Phase0',
                            10: "StopAtT1'", 11: "bCLKEnable'e", 14: 'sPhase0'}),
        # All these outputs have the Boolean level CLK.ca' | Stop | Run'.
        # Different physical buffer chains do not prove coincident arrivals.
        'i05': ('MC10210', {2: "bCLKEnable'b", 3: "bCLKEnable'a",
                            4: "bCLKEnable'c", 5: 'Stop', 6: "Run'",
                            9: "Run'", 11: 'Stop', 12: "bCLKEnable'e",
                            13: "bCLKEnable'd", 14: "bCLKEnable'f"}),
        'l01': ('SE10210', {2: "prepreclk'e", 3: "prepreclk'd",
                            7: "CLK.ca'", 11: "CLK.ca'", 14: "prepreclk'c"}),
        'f12': ('SE10210', {5: "prepreclk'c", 7: "bCLKEnable'c",
                            9: "bCLKEnable'c", 11: "prepreclk'c",
                            12: "preclk2'Bb", 13: "preclk2'Ba"}),
        'g12': ('SE10212', {5: "bCLKEnable'f", 7: "prepreclk'e",
                            9: "prepreclk'e", 11: "bCLKEnable'f", 14: "preclk2'Bd"}),
        'j06': ('SE10210', {4: "clk2'Bc", 6: "preclk2'Ba"}),
        'i07': ('SE10210', {2: "clk2'Be", 4: "clk2'Bf", 7: "preclk2'Bd"}),
    }
    checked = 0
    for package, (kind, mapping) in expected.items():
        if board.packages[package]['type'] != kind:
            raise ValueError('Original package type changed: ' + package)
        for pin, net in mapping.items():
            if pins.get((package, pin)) != net:
                raise ValueError(f'{package}.{pin}: {pins.get((package,pin))!r} != {net!r}')
            checked += 1
    # These absent inputs are the on-chip pulldowns used by the equations;
    # the unused shared clock must not silently gain another edge source.
    absent = [('j05', 6), ('j05', 9), ('k03', 9), ('k03', 12),
              ('k04', 11), ('k04', 12), ('l07', 5), ('l07', 6),
              ('l07', 11), ('l07', 13), ('k05', 5), ('k05', 6),
              ('k05', 11), ('k05', 12), ('j01', 9), ('j01', 12),
              ('i05', 7), ('i05', 10), ('l01', 5), ('l01', 6),
              ('l01', 9), ('l01', 10), ('f12', 6), ('f12', 10),
              ('g12', 6), ('g12', 10), ('j06', 5), ('j06', 7),
              ('i07', 5), ('i07', 6)]
    for key in absent:
        if key in pins:
            raise ValueError('Expected original unwired input: ' + str(key))
    outputs = [(pkg,pin,net) for pkg,(_,mapping) in expected.items()
               for pin,net in mapping.items()
               if any(p['pkg']==pkg and p['pin']==pin and p['dir']=='out'
                      for p in board.nets[net]['pins'])]
    for package,pin,net in outputs:
        drivers={(p['pkg'],p['pin']) for p in board.nets[net]['pins'] if p['dir']=='out'}
        if drivers!={(package,pin)}:
            raise ValueError('Unexpected additional original driver: '+net)
    print(f'CONTROL_PHASE_WIRING PASS packages={len(expected)} pins={checked} pulldowns={len(absent)}')


if __name__ == '__main__':
    check()
