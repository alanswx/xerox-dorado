#!/usr/bin/env python3
"""Check snapshot-review coverage against real elaboration and an original cell."""
from pathlib import Path
import subprocess
import tempfile
import xml.etree.ElementTree as ET
from sil_state_inventory import inventory

ROOT = Path(__file__).resolve().parent.parent
SOURCE = '''
module leaf(input logic clk, en, input logic [2:0] addr,
            input logic [3:0] data, output logic [3:0] q,
            output logic [1:0] a,b, output logic [3:0] latch_q,
            output logic [3:0] comb);
  logic [3:0] ram[0:7];
  logic [3:0] rom[0:7];
  logic [3:0] task_only[0:7];
  logic [2:0][3:0] multidim[-2:2][1:0];
  logic [2:0] captured;
  initial $readmemh("fixture-rom.mem",rom);
  task update_row; task_only[addr] <= data; endtask
  always @(posedge clk) begin
    captured = addr;
    if(en) begin ram[addr] <= data; update_row(); end
    q <= ram[addr] ^ rom[addr] ^ task_only[addr];
    {a,b} <= data;
    multidim[-1][0] <= {data,data,data};
  end
  always_latch if(en) latch_q <= data;
  always_comb comb = data ^ {1'b0,captured};
endmodule
module top(input logic clk,en, input logic [9:0] addr,
           input logic [3:0] data, output wire [32:0] out);
  for(genvar i=0;i<2;i++)begin: units
    leaf u(clk,en,addr[2:0],data,out[i*16+:4],out[i*16+4+:2],
           out[i*16+6+:2],out[i*16+8+:4],out[i*16+12+:4]);
  end
  cell_F10415A im(clk,addr[9],addr[8],addr[7],addr[6],addr[5],
                 addr[4],addr[3],addr[2],addr[1],addr[0],data[0],
                 1'b0,en,out[32]);
endmodule
'''


def main():
    with tempfile.TemporaryDirectory(prefix='dorado-inventory-', dir='/tmp') as tmp:
        tmp = Path(tmp)
        source, xml = tmp/'fixture.sv', tmp/'fixture.xml'
        source.write_text(SOURCE)
        command = ['verilator', '--xml-only', '--top-module', 'top', '-Wno-fatal',
                   '--xml-output', str(xml), str(source), str(ROOT/'verilog/cells/cell_F10415A.v')]
        result = subprocess.run(command, capture_output=True, text=True)
        assert result.returncode == 0, result.stderr
        data = xml.read_bytes()
        report = inventory(data)
        rows = {row['path']: row for row in report['candidates']}
        leaf = [row for row in rows.values() if row['name'] == 'q']
        assert len(leaf) == 2 and all(row['category'] == 'sequential' for row in leaf)
        for instance in ('top.units[0].u', 'top.units[1].u'):
            # Some Verilator versions encode generate indices in hierarchy.
            group = {row['name']: row for row in rows.values()
                     if row['path'].rsplit('.', 1)[0].replace('__BRA__', '[').replace('__KET__', ']') == instance}
            assert group['ram']['bits'] == 32
            assert group['rom']['category'] == 'initial_only_candidate'
            assert group['task_only']['category'] == 'subroutine_review'
            assert group['captured']['bits'] == 3 and group['captured']['category'] == 'sequential'
            assert all(group[name]['category'] == 'sequential' for name in ('a', 'b', 'latch_q'))
            assert group['comb']['category'] == 'combinational_review'
            assert group['multidim']['width'] == 12 and group['multidim']['bits'] == 120
            assert group['multidim']['unpacked_dimensions'] == [[-2, 2], [1, 0]]
        # Original chip: saving just its1024 RAM bits loses the read pipeline.
        assert rows['top.im.mem']['bits'] == 1024
        assert rows['top.im.dout_r']['bits'] == 1
        assert rows['top.im.dout_r']['category'] == 'sequential'
        # Selected array index is a source, not a destination/state writer.
        assert not any(row['name'] == 'addr' for row in rows.values())
        assert inventory(data) == report, 'inventory is not deterministic'

        # Removing a real read pipeline must break the coverage expectation.
        root = ET.fromstring(data)
        cell = next(m for m in root.findall('.//module') if m.get('name') == 'cell_F10415A')
        process = next(p for p in cell.findall('./always')
                       if any(a[-1].get('name') == 'dout_r' for a in p.findall('./assigndly')))
        cell.remove(process)
        mutated = inventory(ET.tostring(root))
        assert 'top.im.dout_r' not in {r['path'] for r in mutated['candidates']}
        # Unknown destinations fail rather than silently shrinking coverage.
        node = next(a[-1] for a in cell.findall('.//assigndly') if a[-1].tag == 'arraysel')
        node[0].set('name', 'missing_state')
        try:
            inventory(ET.tostring(root))
        except ValueError as error:
            assert 'Unresolved module destination' in str(error)
        else:
            raise AssertionError('unknown state destination accepted')
    print('STATE INVENTORY PASS: original RAM/read pipeline, task writes, ROM loads, '
          'latch/edge/blocking captures, concat/index/array geometry, hierarchy and failure controls')


if __name__ == '__main__':
    main()
