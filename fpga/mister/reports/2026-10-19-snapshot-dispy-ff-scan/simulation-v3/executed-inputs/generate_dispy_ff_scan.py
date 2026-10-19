#!/usr/bin/env python3
"""Complete display board with compact serial access to its existing FFs.

RAM, raw read pipelines, oscillator and phase addresses/semantics are retained.
No shadow bank or rate change; foreign consumers still need a global barrier.
"""
import copy
import hashlib
from pathlib import Path
import re
import generate_dispy_full as qualified
from generate_conta_ff_scan import once

ROOT, SOURCE, MODULE = qualified.ROOT, qualified.SOURCE, qualified.MODULE


def generate(root):
    text,manifest,clones = qualified.generate(root)
    manifest = copy.deepcopy(manifest)
    predecessor = manifest['generated_sha256']
    bits = manifest['ff_bits']
    assert bits == 1439 and len(manifest['ff']) == 140
    words = (bits+17)//18
    text = once(text,"    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,",'''    input wire state_system_reset,
    output wire state_scan_hold_required,state_scan_fault,
    output reg state_frozen=1'b0,
    output wire state_done,state_error,''')
    offset,layout = 0,[]
    for row in manifest['ff']:
        width = row['width']
        layout.append(dict(package=row['package'],kind=row['kind'],lsb=offset,width=width,fields=copy.deepcopy(row['fields'])))
        start = re.search(r'\b'+row['kind']+r'_snapshot\b[^;]*?\b'+row['package']+r'\s*\(',text)
        assert start
        end = text.index(');',start.end())
        body = text[start.start():end]
        body = once(body,f".state_write(state_accept && state_write && state_is_ff && state_package==8'd{row['index']})",'.state_write(scan_step)')
        body = once(body,'.state_data(state_data)',f".state_data({{{18-width}'b0,scan_next[{offset}+:{width}]}})")
        text = text[:start.start()]+body+text[end:]
        offset += width
    assert offset == bits
    packed = '{'+','.join(f"state_ff_result[{r['index']}][{r['width']-1}:0]" for r in reversed(manifest['ff']))+'}'
    controller = f'''
  // Scan the physical FF bank; no independent shadow state is allocated.
  wire [1438:0] scan_bits={packed};
  wire [1438:0] scan_next={{scan_serial_in,scan_bits[1438:1]}};
  wire scan_serial_in,scan_step,scan_ready,scan_busy,scan_done,scan_error;
  wire [17:0] scan_result;
  reg state_response_ff;
  reg regular_done=1'b0,regular_error=1'b0;
  dorado_ff_scan_controller #(.STATE_BITS(1439)) u_ff_scan(
    .clk(state_host_clk),.reset(state_system_reset),.freeze(state_freeze),.guest_held(state_guest_held),
    .request(state_accept && state_is_ff),.write(state_write),.index({{5'b0,state_index}}),.data(state_data),
    .serial_out(scan_bits[0]),.serial_in(scan_serial_in),.scan_step(scan_step),.ready(scan_ready),
    .hold_required(state_scan_hold_required),.busy(scan_busy),.done(scan_done),.error(scan_error),
    .fault(state_scan_fault),.result(scan_result));
  assign state_done=regular_done || scan_done;
  assign state_error=regular_error || scan_error;
'''
    text = once(text,'  // 19-bit index:8package/11row. All valid state is explicit; no row alias.',
                controller+'\n  // Compact FF bank; RAM/pipeline/oscillator/phase keep their original access map.')
    text = once(text,'wire state_hold=state_freeze && state_guest_held;',
                'wire state_hold=(state_freeze && state_guest_held) || state_scan_hold_required;')
    text = once(text,'wire state_accept=state_hold && state_frozen && state_request;',
                'wire state_accept=state_freeze && state_guest_held && state_frozen && state_request && !scan_busy && !state_scan_fault && !state_system_reset;')
    text = once(text,"wire state_is_ff=state_package<8'd140 && state_row==0;",f"wire state_is_ff=state_index<19'd{words};")
    text = once(text,'assign state_result=state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result;',
                'assign state_result=state_response_ff?scan_result:(state_response_memory?state_memory_result[state_response_memory_index]:state_aux_result);')
    text = once(text,'    state_frozen<=state_hold;state_done<=state_accept;state_error<=state_accept && !state_valid;',
                '''    state_frozen<=state_hold && !state_system_reset;
    regular_done<=state_accept && !state_is_ff;regular_error<=state_accept && !state_valid;''')
    text = once(text,'      state_response_memory<=state_valid && state_is_memory && !state_write;',
                '      state_response_ff<=state_is_ff;\n      state_response_memory<=state_valid && state_is_memory && !state_write;')
    phase = '{'+','.join(f['name'] for f in reversed(manifest['phase']['fields']))+'}'
    text = once(text,"      state_aux_result<=state_write?18'b0:state_is_ff?state_ff_result[state_package]:(state_is_phase?{10'b0,"+phase+"}:18'b0);",
                "      state_aux_result<=!state_write && state_is_phase?{10'b0,"+phase+"}:18'b0;")
    needle = '.state_freeze(state_freeze),.state_guest_held(state_guest_held)'
    assert text.count(needle) == 13
    text = text.replace(needle,'.state_freeze(state_hold),.state_guest_held(state_hold)')
    needle = '.freeze(state_freeze),.guest_held(state_guest_held)'
    assert text.count(needle) == 3+1  # three word banks plus controller, which must see the external boundary
    # Replace only the three word-memory instances; the controller monitors
    # the unmodified external boundary to detect loss rather than hiding it.
    for row in manifest['memories']:
        if row['kind'] != 'cell_F10415A':
            continue
        start = text.index(' '+row['instance']+'(')
        end = text.index(');',start)
        body = once(text[start:end],needle,'.freeze(state_hold),.guest_held(state_hold)')
        text = text[:start]+body+text[end:]
    import generate_f10145_zero_write as files
    helper,schema = files.generate(mlab=True,async_mlab=True)
    assert hashlib.sha256(clones['cell_F10145A_snapshot.sv'].encode()).hexdigest() == schema['qualified_predecessor_sha256']
    clones['cell_F10145A_snapshot.sv'] = helper
    clones['ff_scan_controller.sv'] = (Path(__file__).parent/'ff_scan_controller.sv').read_text()
    for row in manifest['ff']:
        row['legacy_parallel_word'] = row.pop('word')
    manifest.update(generated_sha256=hashlib.sha256(text.encode()).hexdigest(),qualified_parallel_predecessor_sha256=predecessor,
                    cloned_sources={n:hashlib.sha256(s.encode()).hexdigest() for n,s in clones.items()},
                    shared_file_manifest=schema,state_access_schema='compact-ff-v1',
                    scan=dict(state_bits=bits,words=words,word_bits=18,layout=layout,
                              word_map=[dict(index=i,lsb=18*i,width=min(18,bits-18*i)) for i in range(words)],
                              last_word_bits=17,completion_edges=bits),scope=__doc__)
    return text,manifest,clones
