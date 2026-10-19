#!/usr/bin/env python3
"""Experimental complete ContA with a compact serial FF access bank.

All 681 FF bits remain in the unchanged qualified helpers. RAM/window and phase
access retain their previous addresses; FF addresses are compact words0..37.
This requires a new schema/ABI and a global hold owner before core integration.
"""
import copy
import hashlib
from pathlib import Path
import re
import generate_conta_full as qualified

ROOT, MODULE, PHASES = qualified.ROOT, qualified.MODULE, qualified.PHASES


def once(text, before, after):
    assert text.count(before) == 1, 'unexpected qualified ContA source: '+before[:100]
    return text.replace(before, after, 1)


def generate(source, async_files=False):
    text, manifest, clones = qualified.generate(source)
    manifest = copy.deepcopy(manifest)
    predecessor = manifest['generated_sha256']
    text = once(text, '    output reg state_frozen=1\'b0,state_done=1\'b0,state_error=1\'b0,',
                '''    input wire state_system_reset,
    output wire scan_hold_required,scan_fault,
    output reg state_frozen=1'b0,
    output wire state_done,state_error,''')
    offset = 0
    rows = []
    for row in manifest['ff']:
        width = row['width']
        rows.append(dict(package=row['package'], kind=row['kind'], lsb=offset, width=width,
                         fields=copy.deepcopy(row['fields'])))
        start = re.search(r'\b'+row['kind']+r'_snapshot\b[^;]*?\b'+row['package']+r'\s*\(', text)
        assert start, 'missing FF instance: '+row['package']
        end = text.index(');', start.end())
        body = text[start.start():end]
        old_write = f'.state_write(state_accept && state_write && state_package==7\'d{row["index"]} && state_row==0)'
        body = once(body, old_write, '.state_write(scan_step)')
        body = once(body, '.state_data(state_data)',
                    f".state_data({{{18-width}'b0,scan_next[{offset}+:{width}]}})")
        text = text[:start.start()]+body+text[end:]
        offset += width
    assert offset == manifest['ff_bits'] == 681
    packed = '{'+','.join(f"state_ff_result[{r['index']}][{r['width']-1}:0]" for r in reversed(manifest['ff']))+'}'
    controller = f'''
  // Existing physical FFs form the chain; no mirror state bank is allocated.
  wire [680:0] scan_bits={packed};
  wire [680:0] scan_next={{scan_serial_in,scan_bits[680:1]}};
  wire scan_serial_in,scan_step,scan_ready,scan_busy,scan_done,scan_error;
  wire [17:0] scan_result;
  reg state_response_ff;
  reg regular_done=1'b0,regular_error=1'b0;
  dorado_ff_scan_controller #(.STATE_BITS(681)) u_ff_scan(
    .clk(state_host_clk),.reset(state_system_reset),
    .freeze(state_freeze),.guest_held(state_guest_held),
    .request(state_accept && state_is_ff),.write(state_write),
    .index({{12'b0,state_index}}),.data(state_data),
    .serial_out(scan_bits[0]),.serial_in(scan_serial_in),.scan_step(scan_step),
    .ready(scan_ready),.hold_required(scan_hold_required),.busy(scan_busy),
    .done(scan_done),.error(scan_error),.fault(scan_fault),.result(scan_result));
  assign state_done=regular_done || scan_done;
  assign state_error=regular_error || scan_error;
'''
    text = once(text, '  // Package0..74: FF word0.75..83:16x4 RAM words0..15/window16.\n',
                '  // Compact FF words0..37. RAM packages75..83 and phase84 retain their addresses.\n'+controller)
    text = once(text, 'wire state_hold=state_freeze && state_guest_held;',
                'wire state_hold=(state_freeze && state_guest_held) || scan_hold_required;')
    text = once(text, 'wire state_accept=state_hold && state_frozen && state_request;',
                'wire state_accept=state_freeze && state_guest_held && state_frozen && state_request && !scan_busy && !scan_fault && !state_system_reset;')
    text = once(text, '    state_frozen<=state_hold;',
                '    state_frozen<=state_hold && !state_system_reset;')
    text = once(text, 'wire state_is_ff=state_package<75 && state_row==0;',
                "wire state_is_ff=state_index<12'd38;")
    text = once(text, "assign state_result=state_response_ram ? {8'b0,state_ram_result[state_response_ram_index]} : state_aux_result;",
                "assign state_result=state_response_ff ? scan_result : (state_response_ram ? {8'b0,state_ram_result[state_response_ram_index]} : state_aux_result);")
    text = once(text, '    state_done<=state_accept;\n    state_error<=state_accept && !state_valid;',
                '''    regular_done<=state_accept && !state_is_ff;
    regular_error<=state_accept && !state_valid;''')
    text = once(text, '      state_response_ram<=state_is_ram;',
                '      state_response_ff<=state_is_ff;\n      state_response_ram<=state_is_ram;')
    text = once(text, "      state_aux_result<=state_is_ff ? state_ff_result[state_package] :\n                        (state_is_phase ? {12'b0,{clk1_p_Da,clk1_p_Cb,clk1_p_Ca,clk1_p_Ac,clk1_p_Ab,clk1_p_Aa}} : 18'b0);",
                "      state_aux_result<=state_is_phase ? {12'b0,{clk1_p_Da,clk1_p_Cb,clk1_p_Ca,clk1_p_Ac,clk1_p_Ab,clk1_p_Aa}} : 18'b0;")
    # During a partial rotation even a lost external boundary must not release
    # local FFs, phase state or RAM/window helpers. Foreign consumers still
    # require the separate global barrier, which is not installed here.
    assert text.count('.state_freeze(state_freeze),.state_guest_held(state_guest_held)') == 9
    text = text.replace('.state_freeze(state_freeze),.state_guest_held(state_guest_held)',
                        '.state_freeze(state_hold),.state_guest_held(state_hold)')
    clones['ff_scan_controller.sv'] = (Path(__file__).parent/'ff_scan_controller.sv').read_text()
    if async_files:
        import generate_f10145_zero_write as files
        helper, schema = files.generate(mlab=True, async_mlab=True)
        assert hashlib.sha256(clones['cell_F10145A_snapshot.sv'].encode()).hexdigest() == schema['qualified_predecessor_sha256']
        clones['cell_F10145A_snapshot.sv'] = helper
        manifest['shared_file_manifest'] = schema
    manifest.update(generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                    qualified_parallel_predecessor_sha256=predecessor,
                    async_files=async_files,
                    cloned_sources={n:hashlib.sha256(s.encode()).hexdigest() for n,s in clones.items()},
                    scan=dict(state_bits=681,words=38,word_bits=18,layout=rows,
                              last_word_bits=15,completion_edges=681,
                              schema='Experimental compact FF bank, RAM/phase addresses unchanged; incompatible with per-package FF layout'),
                    scope=__doc__)
    assert manifest['logical_state_bits'] == 1353
    return text, manifest, clones
