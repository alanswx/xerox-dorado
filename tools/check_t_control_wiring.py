#!/usr/bin/env python3
"""Bind the experimental T-control contract to the original Xerox wire lists.

HM3.8 and ProcH20/ProcL20 define the function; this check verifies package
identity and the capture/data pins used in the independent cone regression.
It does not infer the two capture enables or qualify the whole processor.
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


if __name__ == "__main__":
    check("ProcH-Rev-Ce", "h22", "Clock0'Da", "f18", "f18", False)
    check("ProcL-Rev-Ci", "d17", "SHCP'C", "g23", "g23", True)
