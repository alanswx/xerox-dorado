#!/usr/bin/env python3
"""Compact ContB FF rotation, retaining all original IM groups and pipelines.

No shadow bank or memory regrouping. Local quarantine and reset-entry gates
do not replace the required global machine/service hold.
"""
from contextlib import contextmanager
import copy
import hashlib
from pathlib import Path
import re
import audit_contb_im as im
import generate_contb_full as qualified
import generate_snapshot_ff as ff
from generate_conta_ff_scan import once

ROOT, MODULE = im.ROOT, qualified.MODULE
SOURCE='verilog/generated/ContB-Rev-Cd.v'
WIRELIST='chm/sil/ContB-Rev-Cd.dm!1_/ContB-Rev-Cd.wl'
BOARD_SHA='172f68891c2ad75dd8f69a9d8a8a3d369bc83d4f6db1d54b77a7b0be595add58'
WIRELIST_SHA='25e82daa80f25dce0986b269ce0a145981fbb7d2be24b80e3d6c649d2e02ba68'
LOCATIONS_SHA='8ce7eb1bb436c11583a3bb95fe4e04bb96f7f898656a3e9751a0e0f9f7e4c80e'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


@contextmanager
def source_context(root):
    """Bind the older qualified generators to the actual frozen inputs."""
    root=root.resolve()
    assert sha(root/SOURCE)==BOARD_SHA, 'unreviewed frozen ContB'
    wirelist=root/WIRELIST if (root/WIRELIST).exists() else ROOT/WIRELIST
    assert sha(wirelist)==WIRELIST_SHA and sha(wirelist.with_suffix('.lc'))==LOCATIONS_SHA
    for kind in {k for k,_,_ in im.instances((root/SOURCE).read_text())}:
        old=root/'verilog/cells'/(kind+'.v');current=ROOT/'verilog/cells'/(kind+'.v')
        if old.read_bytes()!=current.read_bytes():
            assert kind=='cell_MC10170' and [t[0] for t in ff.tokens(old.read_text())]==[
                t[0] for t in ff.tokens(current.read_text())], 'qualified cell differs: '+kind
    previous=im.ROOT,im.GENERATED,im.SIL
    im.ROOT,im.GENERATED,im.SIL=root,root/SOURCE,wirelist
    try:
        yield
    finally:
        im.ROOT,im.GENERATED,im.SIL=previous


def reference(root):
    with source_context(root):
        return qualified.generate()


def generate(root):
    text,manifest,clones=reference(root)
    manifest=copy.deepcopy(manifest)
    predecessor=manifest['generated_sha256']
    assert len(manifest['ff'])==22 and manifest['sequential_bits']==209
    text=once(text,'    input wire state_host_clk, state_guest_advance,',
              '''    input wire state_system_reset,
    output wire state_scan_hold_required,state_scan_fault,
    input wire state_host_clk, state_guest_advance,''')
    layout,offset=[],0
    for row in manifest['ff']:
        width=row['width'];package='u_'+row['package']
        layout.append(dict(package=package,kind=row['kind'],lsb=offset,width=width,fields=copy.deepcopy(row['fields'])))
        start=re.search(r'\b'+row['kind']+r'_snapshot\s+'+package+r'\s*\(',text)
        assert start
        end=text.index(');',start.end())
        body=once(text[start.start():end],'.state_hold(state_freeze && state_guest_held)', '.state_hold(state_hold)')
        body=once(body,f".state_write(state_ff_write && state_index[10:0] == 11'd{row['index']})",'.state_write(scan_step)')
        body=once(body,'.state_data(state_data)',f".state_data({{{18-width}'b0,scan_next[{offset}+:{width}]}})")
        text=text[:start.start()]+body+text[end:];offset+=width
    assert offset==209
    packed='{'+','.join(f"state_ff_result[{r['index']}][{r['width']-1}:0]" for r in reversed(manifest['ff']))+'}'
    controller=f'''
  // Scan only the existing physical FFs. IM groups retain their old map.
  wire [208:0] scan_bits={packed};
  wire [208:0] scan_next={{scan_serial_in,scan_bits[208:1]}};
  wire scan_serial_in,scan_step,scan_ready,scan_busy,scan_done,scan_error;
  wire [17:0] scan_result;
  wire state_hold=(state_freeze && state_guest_held) || state_scan_hold_required;
  reg state_boundary_ready=1'b0;
  reg state_response_ff;
  always @(posedge state_host_clk) state_boundary_ready<=state_hold && !state_system_reset;
  dorado_ff_scan_controller #(.STATE_BITS(209)) u_ff_scan(
    .clk(state_host_clk),.reset(state_system_reset),.freeze(state_freeze),.guest_held(state_guest_held),
    .request(state_accept && state_valid_ff),.write(state_write),.index({{13'b0,state_index[10:0]}}),.data(state_data),
    .serial_out(scan_bits[0]),.serial_in(scan_serial_in),.scan_step(scan_step),.ready(scan_ready),
    .hold_required(state_scan_hold_required),.busy(scan_busy),.done(scan_done),.error(scan_error),
    .fault(state_scan_fault),.result(scan_result));
'''
    text=once(text,'  // Full modeled ContB state: group0..7 IM, group8 sequential packages.',
              controller+'\n  // Groups0..7 IM, compact FF words in group8; other groups remain invalid.')
    text=once(text,'  reg [17:0] state_aux_result;\n','')
    text=once(text,'  assign state_frozen = &state_bank_frozen;',
              '  assign state_frozen = !state_system_reset && state_boundary_ready && (&state_bank_frozen);')
    text=once(text,'wire state_accept = state_freeze && state_guest_held && state_frozen && state_request;',
              'wire state_accept = state_freeze && state_guest_held && state_frozen && state_request && !scan_busy && !state_scan_fault && !state_system_reset;')
    text=once(text,"state_is_ff && state_index[10:0] < 11'd22", "state_is_ff && state_index[10:0] < 11'd12")
    text=once(text,'  wire state_ff_write = state_accept && state_write && state_valid_ff;\n','')
    text=once(text,'state_aux_done <= state_accept && state_index[14];',
              'state_aux_done <= state_accept && state_index[14] && !state_valid_ff;')
    text=once(text,'      state_response_aux <= state_index[14];',
              '      state_response_aux <= state_index[14];\n      state_response_ff <= state_valid_ff;')
    text=once(text,"      state_aux_result <= state_valid_ff ? state_ff_result[state_index[10:0]] : 18'b0;\n",'')
    text=once(text,'assign state_done = (|state_bank_done) | state_aux_done;',
              'assign state_done = (|state_bank_done) | state_aux_done | scan_done;')
    text=once(text,'assign state_error = (|state_bank_error) | state_aux_error;',
              'assign state_error = (|state_bank_error) | state_aux_error | scan_error;')
    text=once(text,'state_response_aux ? state_aux_result : state_bank_result[state_response_bank]',
              "state_response_aux ? (state_response_ff ? scan_result : 18'b0) : state_bank_result[state_response_bank]")
    needle='.freeze(state_freeze), .guest_held(state_guest_held)'
    assert text.count(needle)==8
    text=text.replace(needle,'.freeze(state_hold), .guest_held(state_hold)')
    for row in manifest['ff']:
        row['legacy_parallel_word']=row.pop('word')
    clones['f10415_enable_bank.sv']=(ROOT/'fpga/mister/snapshot/f10415_enable_bank.sv').read_text()
    clones['ff_scan_controller.sv']=(ROOT/'fpga/mister/snapshot/ff_scan_controller.sv').read_text()
    deltas=[]
    for name in manifest['grouping_sources']:
        if (root/name).read_bytes()!=(ROOT/name).read_bytes():
            deltas.append(dict(file=name,compiled_sha256=sha(root/name),current_sha256=sha(ROOT/name),
                               review='MC10170 comment only; exact Verilog token stream is unchanged.'))
    manifest.update(generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        qualified_parallel_predecessor_sha256=predecessor,ff_bits=209,index_bits=15,
        state_access_schema='compact-ff-v1',
        scan=dict(state_bits=209,words=12,base_word=16384,word_bits=18,layout=layout,
                  word_map=[dict(index=16384+i,lsb=18*i,width=min(18,209-18*i)) for i in range(12)],
                  last_word_bits=11,completion_edges=209),comment_only_source_deltas=deltas,
        cloned_sources={n:hashlib.sha256(s.encode()).hexdigest() for n,s in clones.items()},scope=__doc__)
    return text,manifest,clones
