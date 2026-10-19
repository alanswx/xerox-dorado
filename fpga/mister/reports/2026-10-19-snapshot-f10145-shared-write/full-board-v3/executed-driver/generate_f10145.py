#!/usr/bin/env python3
"""Clone the original 16x4 file with bounded physical-state access.

Preserves its exact write-window process under the actual host advance.
Only the synthesis-translate-off diagnostic block is removed. This is an
isolated experiment, not a machine barrier or an installed FPGA snapshot.
"""
import argparse
import hashlib
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
ORIGINAL=ROOT/'verilog/cells/cell_F10145A.v'
NORMAL='''  always @(posedge sys_clk) begin
    we_d <= p13;
    if (!p13) begin                          // while WE' is asserted (low)
      a_lat  <= a;
      d_lat  <= {p12, p11, p4, p5};
      ce_lat <= ~p3;
    end
    if (p13 && !we_d && ce_lat) mem[a_lat] <= d_lat;   // WE' rising: commit
  end'''


def once(text,before,after):
    if text.count(before)!=1:raise ValueError('unreviewed original source: '+before[:80])
    return text.replace(before,after,1)


def generate(original=None):
    if original is None:original=ORIGINAL.read_text()
    start=original.index('  // synthesis translate_off\n')
    end=original.index('  // synthesis translate_on\n',start)+len('  // synthesis translate_on\n')
    if original.count('synthesis translate_off')!=1 or original.count('synthesis translate_on')!=1:
        raise ValueError('unreviewed simulation-only region')
    diagnostic=original[start:end]
    expected=['a_first','d_first','in_we','n_amoved','n_dmoved','n_writes','ramwe_dbg']
    # No removed name may feed ordinary RTL. The exact process below is
    # separately guarded; diagnostic assignments/prints do not synthesize.
    import re
    text=original[:start]+original[end:]
    for name in expected:
        if re.search(r'\b'+name+r'\b',text):raise ValueError('diagnostic escapes synthesis exclusion: '+name)
    # Guard every physical declaration and process. An added history bit or
    # changed RAM width must require review rather than escape the manifest.
    clean=re.sub(r'/\*.*?\*/|//[^\n]*','',text,flags=re.S)
    clean=once(clean,re.sub(r'//[^\n]*','',NORMAL),'')
    for declaration,count in [('reg [3:0] mem [0:15];',2),('reg we_d;',1),
                              ('reg [3:0] a_lat, d_lat;',1),('reg       ce_lat;',1)]:
        if clean.count(declaration)!=count:raise ValueError('unreviewed physical declaration: '+declaration)
        clean=clean.replace(declaration,'')
    clean=re.sub(r'"(?:\\.|[^"\\])*"','""',clean)
    if re.search(r'\b(reg|logic|always\w*|initial|final)\b',clean):
        raise ValueError('unreviewed extra physical state/process')
    text=once(text,'module cell_F10145A (','module cell_F10145A_snapshot (')
    text=once(text,'    input  wire sys_clk,','''    input wire state_host_clk, state_advance, state_freeze, state_guest_held,
    input wire state_request, state_write,
    input wire [4:0] state_index,
    input wire [9:0] state_data,
    output reg state_frozen = 1'b0,
    output reg state_done = 1'b0, state_error = 1'b0,
    output reg [9:0] state_result,
    input  wire sys_clk,''')
    body=NORMAL.removeprefix('  always @(posedge sys_clk) begin\n').removesuffix('\n  end')
    process='''  // Group0 words0..15: raw4-bit RAM. Word16: all10 write-window bits.
  // The caller holds ALL guest state before asserting state_guest_held.
  // Entry/release/running requests are ignored; invalid rows cannot mutate.
  wire state_hold = state_freeze && state_guest_held;
  wire state_accept = state_hold && state_frozen && state_request;
  wire state_valid = state_index <= 5'd16;
  wire [9:0] state_window = {ce_lat,d_lat,a_lat,we_d};
  always @(posedge state_host_clk) begin
    state_frozen <= state_hold;
    state_done <= state_accept;
    state_error <= state_accept && !state_valid;
    if (state_accept) begin
      state_result <= state_index == 5'd16 ? state_window :
                      (state_index < 5'd16 ? {6'b0,mem[state_index[3:0]]} : 10'b0);
      if (state_write && state_valid) begin
        if (state_index == 5'd16) {ce_lat,d_lat,a_lat,we_d} <= state_data;
        else mem[state_index[3:0]] <= state_data[3:0];
      end
    end
    if (!state_hold && state_advance) begin
'''+body+'''
    end
  end'''
    text=once(text,NORMAL,process)
    manifest=dict(module='cell_F10145A_snapshot',logical_state_bits=74,
        ram=dict(words=16,width=4,indices=[0,15]),
        write_window=dict(index=16,width=10,fields=[
            dict(name='we_d',width=1,lsb=0),dict(name='a_lat',width=4,lsb=1),
            dict(name='d_lat',width=4,lsb=5),dict(name='ce_lat',width=1,lsb=9)]),
        excluded_simulation_only=expected,
        exclusion_reason='Original synthesis translate_off diagnostics; no reference outside excluded region',
        excluded_source_sha256=hashlib.sha256(diagnostic.encode()).hexdigest(),
        original_sha256=hashlib.sha256(original.encode()).hexdigest(),
        generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        scope='Physical register-file/window state only; no global barrier, board integration, fit or machine restore')
    return text,manifest


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=False)
    source,manifest=generate()
    (args.output/'cell_F10145A_snapshot.sv').write_text(source)
    (args.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
