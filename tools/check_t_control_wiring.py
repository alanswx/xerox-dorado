#!/usr/bin/env python3
"""Bind the experimental T-control contract to the original Xerox wire lists.

HM3.8 and ProcH20/ProcL20 define the function; this check verifies package
identity and the capture/data pins used in the independent cone regression.
The clock-path check records the original fanout and enable flip-flop.
It does not implement the two capture enables or qualify the whole processor.
"""
from pathlib import Path
from sil_netlist import load_board


def check(board_name, latch, clock, select_gate, write_gate, low):
    root = Path(__file__).resolve().parents[1]
    directory = root / "chm/sil" / (board_name + ".dm!1_")
    board = load_board(str(directory / (board_name + ".wl")))
    actual = {}
    for name, net in board.nets.items():
        for pin in net["pins"]:
            actual[(pin["pkg"], pin["pin"])] = name
    expected = {
        (latch, 9): clock,
        (latch, 4): "TbSelMd",
        (latch, 13): "RbWriteEn'",
        (latch, 14): "TbWriteEn'",
        (latch, 15): "Last=Curr'",
        (latch, 12): "Curr=Next'",
        (latch, 5): "LC.1a" if low else "LC.1'a",
        (latch, 6): "LC.1'a" if low else "LC.1a",
        (latch, 2): "RbSelMd'" if low else "RbSelMd",
        (latch, 3): "RbSelMd" if low else "RbSelMd'",
        ("h24", 13): "LC.1",
        ("h24", 9): "LC.1a",
        ("h24", 15): "LC.1'a",
        ("f20", 9): "FA=0'b",
        ("f20", 12): "FC=5'",
        ("f20", 13): "FB=7'",
        (select_gate, 6): "LC.0",
        (select_gate, 7): "LC.1'a",
        (write_gate, 4): "LC.2",
        ("h23", 4): "LC.1'a",
        ("h23", 5): "LC.2",
        ("h23", 6): "LC.0",
        ("h23", 7): "LC.1'a",
        ("h23", 10): "LC.0",
    }
    for key, value in expected.items():
        if actual.get(key) != value:
            raise SystemExit(f"{board_name} {key}: {actual.get(key)!r} != {value!r}")
    for pkg, part in [(latch, "MC10176"), ("h24", "MC10101"),
                      ("f20", "MC10100"), ("h23", "MC10102"),
                      (select_gate, "MC10102"), (write_gate, "MC10102")]:
        if board.packages[pkg]["type"] != part:
            raise SystemExit(f"{board_name} {pkg}: wrong package type")
    # TbSelMd's D really is the tied outputs of the LC and FF075 gates.
    d_net = board.nets[actual[(latch, 7)]]
    drivers = {(p["pkg"], p["pin"]) for p in d_net["pins"] if p["dir"] == "out"}
    if drivers != {("f20", 15), (select_gate, 3)}:
        raise SystemExit(f"{board_name}: unexpected T-select drivers {drivers}")
    # Active-low write enables must enter the corresponding capture pins.
    for latch_pin, source_pin, pkg in [(10, 14, "h23"), (11, 2, write_gate)]:
        if actual[(latch, latch_pin)] != actual[(pkg, source_pin)]:
            raise SystemExit(f"{board_name}: write-enable connection changed")
    print(f"T-CONTROL WIRING PASS {board_name} {latch}.9={clock}, {len(expected)} pin checks")


def check_clock_path(board_name, low):
    root = Path(__file__).resolve().parents[1]
    directory = root / "chm/sil" / (board_name + ".dm!1_")
    board = load_board(str(directory / (board_name + ".wl")))
    pins = {(pin['pkg'], pin['pin']): name
            for name, net in board.nets.items() for pin in net['pins']}
    clock = "CLK.pl'" if low else "CLK.ph'"
    enable = "CLKEnable'b" if low else "CLKEnable'c"
    start = "Clk0En'" if low else "Clock0En'"
    phase1 = "pClk1En'" if low else "pClock1En'"
    expected = {
        ('l01', 4): start, ('l01', 6): "StartCycle'a",
        ('l01', 9): enable, ('l01', 11): clock,
        ('l01', 12): "ppClk'b", ('l01', 13): "ppClk'a", ('l01', 14): "ppClk'c",
        ('l02', 2): phase1, ('l02', 6): enable,
        ('l02', 7): start, ('l02', 9): clock,
    }
    types = {'l01': 'SE10210', 'l02': 'SE10231'}
    absent = [('l01', 5), ('l01', 7), ('l01', 10), ('l02', 13)]
    if low:
        expected.update({('g12', 4): "PreSHCP'C", ('g12', 6): phase1,
                         ('c18', 9): "PreSHCP'C", ('c18', 13): "SHCP'C",
                         ('d17', 9): "SHCP'C"})
        types.update(g12='SE10211', c18='SE10210', d17='MC10176')
        absent += [('g12', 5), ('g12', 7), ('c18', 10), ('c18', 11)]
        equation = "SHCP'C = ~l02.Q; l02 samples StartCycle'a on rising(CLK.pl' | CLKEnable'b)"
    else:
        expected.update({('g13', 3): "PreClock0'D", ('g13', 5): "ppClk'c",
                         ('g13', 6): start, ('i18', 3): "Clock0'Da",
                         ('i18', 5): "PreClock0'D", ('h22', 9): "Clock0'Da"})
        types.update(g13='SE10210', i18='SE10210', h22='MC10176')
        absent += [('g13', 7), ('i18', 6), ('i18', 7)]
        equation = "Clock0'Da = CLK.ph' | CLKEnable'c | StartCycle'a"
    for key, value in expected.items():
        if pins.get(key) != value:
            raise SystemExit(f"{board_name} clock path {key}: {pins.get(key)!r} != {value!r}")
    for key in absent:
        if key in pins:
            raise SystemExit(f"{board_name} clock path {key}: expected unwired input, got {pins[key]!r}")
    for package, part in types.items():
        if board.packages[package]['type'] != part:
            raise SystemExit(f"{board_name} clock path {package}: expected {part}")
    outputs = [('l01', 4), ('l01', 14), ('l02', 2)]
    outputs += [('g12', 4), ('c18', 13)] if low else [('g13', 3), ('i18', 3)]
    for output in outputs:
        drivers = {(pin['pkg'], pin['pin']) for pin in board.nets[pins[output]]['pins']
                   if pin['dir'] == 'out'}
        if drivers != {output}:
            raise SystemExit(f"{board_name} clock path {pins[output]}: unexpected drivers {drivers}")
    print(f"T-CLOCK WIRING PASS {board_name}: {equation}; {len(expected)} pins, {len(absent)} unwired inputs")


if __name__ == "__main__":
    check("ProcH-Rev-Ce", "h22", "Clock0'Da", "f18", "f18", False)
    check("ProcL-Rev-Ci", "d17", "SHCP'C", "g23", "g23", True)
    check_clock_path("ProcH-Rev-Ce", False)
    check_clock_path("ProcL-Rev-Ci", True)
