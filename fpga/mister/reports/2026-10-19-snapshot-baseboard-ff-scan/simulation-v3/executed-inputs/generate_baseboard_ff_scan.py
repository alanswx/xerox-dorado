#!/usr/bin/env python3
"""Compact BaseBoard physical FF/6502-node rotation; other state maps retained.

The original 279 clock/reset/FF bits and all 1725 netlist CPU nodes form one
2004-bit chain. No shadow state or CPU behavior replacement. All consumers
still require a separate global boundary before machine integration.
"""
import copy
import hashlib
from pathlib import Path
import re
import generate_baseboard_full as qualified
import generate_riot_shared_port as shared
from generate_conta_ff_scan import once

ROOT, SOURCE, MODULE = qualified.ROOT, qualified.SOURCE, qualified.MODULE


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def raw_bytes(value):
    return value if isinstance(value, bytes) else value.encode()


def reference(root):
    text, manifest, clones = qualified.generate(root)
    # Use the already-qualified shared RIOT writer in BOTH fit variants.
    helper, schema = shared.generate((root/'verilog/vendor/riot/M6532.sv').read_bytes())
    assert hashlib.sha256(clones['M6532_snapshot.sv'].encode()).hexdigest() == schema['qualified_predecessor_sha256']
    clones['M6532_snapshot.sv'] = helper
    manifest = copy.deepcopy(manifest)
    manifest['shared_riot_manifest'] = schema
    manifest['cloned_sources'] = {n:hashlib.sha256(raw_bytes(s)).hexdigest() for n,s in clones.items()}
    return text, manifest, clones


def ff_helper(text, fields):
    """Replace only the qualified administrative load, preserve normal body."""
    width = sum(f['width'] for f in fields)
    packed = '{'+','.join(f['name'] for f in reversed(fields))+'}'
    text = once(text, '    input  wire sys_clk,',
                f'    input wire state_scan_in,output wire [{width-1}:0] state_scan_out,\n    input  wire sys_clk,')
    matches = list(re.finditer(r'(if\s*\(state_hold\)\s*begin\s*)(.*?)(\s*end\s+else\s+if\s*\(state_advance\)\s*begin)', text, re.S))
    assert len(matches) == 1 and re.match(r'if\s*\(state_write\)', matches[0][2]), 'qualified FF admin load differs'
    m = matches[0]
    serial = f'if(state_write){packed} <= {{state_scan_in,state_scan_out[{width-1}:1]}};'
    assert width > 1
    text = text[:m.start(2)]+serial+text[m.end(2):]
    return once(text, 'endmodule', f' assign state_scan_out={packed};\nendmodule')


def cpu_helper(original):
    raw, schema = qualified.cpu.generate(original)
    text = raw.decode()
    a = text.index('    input state_host_clk,')
    b = text.index('    input           clk,', a)
    # Retain the original vendor CRLF, netlist and relaxation equations.
    text = text[:a]+('    input state_host_clk,state_advance,state_hold,state_scan_step,state_scan_in,\r\n'
                    '    output wire [1724:0] state_scan_out,\r\n')+text[b:]
    a = text.index('    wire state_hold=')
    end = '        if(!state_hold && state_advance)q<=no;\r\n    end'
    b = text.index(end,a)+len(end)
    assert text[a:b].count('q<=no;') == 1
    process = '''    assign state_scan_out=q;
    always @(posedge state_host_clk) begin
        if(state_hold)begin
            if(state_scan_step)q<={state_scan_in,q[1724:1]};
        end else if(state_advance)q<=no;
    end
'''.replace('\n','\r\n')
    generated = (text[:a]+process.rstrip('\r\n')+text[b:]).encode()
    # Preserve the entire original input-node drive and output wiring suffix.
    suffix = original[original.index(b'    always @* begin\r\n        ni = q;'):]
    assert generated.endswith(suffix), 'original CPU combinational suffix differs'
    return generated, schema


def generate(root):
    text, manifest, clones = reference(root)
    predecessor = manifest['generated_sha256']
    old = copy.deepcopy(manifest)
    text = once(text, "    output reg state_frozen=1'b0,state_done=1'b0,state_error=1'b0,output wire [17:0] state_result,",
                '''    input wire state_system_reset,
    output wire state_scan_hold_required,state_scan_fault,
    output wire state_frozen,state_done,state_error,output wire [17:0] state_result,''')
    layout, offset, helper_kinds = [], 0, set()
    for row in manifest['packages']:
        if row['type'] not in ('FF','CPU'):
            continue
        width = row['logical_state_bits']
        fields = row['fields'] if row['type']=='FF' else row['schema']['fields']
        layout.append(dict(package=row['package'],kind=row['kind'],lsb=offset,width=width,fields=copy.deepcopy(fields)))
        name = row['package']
        match = next(x for x in qualified.INSTANCES.finditer(text) if x[3] == name)
        kind, gap, _, body = match.groups()
        if row['type']=='FF':
            assert kind == row['kind']+'_snapshot'
            stem = kind
            filename = next(n for n in clones if n.startswith(stem+'.'))
            if kind not in helper_kinds:
                clones[filename] = ff_helper(clones[filename], fields)
                helper_kinds.add(kind)
            body = once(body, f".state_write(state_accept && state_write && state_valid && state_package==6'd{row['index']})",
                        '.state_write(scan_step)')
            body = once(body, '.state_data(state_data)', ".state_data(18'b0)")
            body = once(body, f".state_result(state_child_result[{row['index']}])", '.state_result()')
            if row['kind'] in qualified.newff.FIELDS:
                body = once(body, '.state_index(state_row[1:0])', ".state_index(2'b0)")
            body = f'\n    .state_scan_in(scan_next[{offset+width-1}]),.state_scan_out(scan_bits[{offset}+:{width}]),'+body
        else:
            # The raw CPU node vector is serialized by the SAME owner as the
            # discrete FFs; its old 96-way parallel administrative mux is gone.
            start = body.index('    .state_host_clk(')
            boundary = body.index('.state_result(state_child_result[47]),')+len('.state_result(state_child_result[47]),')
            body = body[:start]+f'''    .state_host_clk(state_host_clk),.state_advance(state_guest_advance),.state_hold(state_hold),
    .state_scan_step(scan_step),.state_scan_in(scan_next[{offset+width-1}]),.state_scan_out(scan_bits[{offset}+:{width}]),'''+body[boundary:]
        text = text[:match.start()]+kind+gap+name+' ('+body+');'+text[match.end():]
        row['legacy_parallel_word'] = row.pop('word')
        offset += width
    assert offset == 2004 and len(layout) == 31
    cpu, _ = cpu_helper((root/'verilog/vendor/6502/chip_6502.v').read_bytes())
    clones['chip_6502_snapshot.sv'] = cpu
    wrapper = clones['cell_MCS6502_snapshot.sv']
    a = wrapper.index('    input wire state_host_clk,')
    b = wrapper.index('    input  wire sys_clk,',a)
    wrapper = wrapper[:a]+'''    input wire state_host_clk,state_advance,state_hold,state_scan_step,state_scan_in,
    output wire [1724:0] state_scan_out,
'''+wrapper[b:]
    a = wrapper.index('      .state_host_clk(')
    b = wrapper.index('.state_result(state_result),',a)+len('.state_result(state_result),')
    wrapper = wrapper[:a]+'''      .state_host_clk(state_host_clk),.state_advance(state_advance),.state_hold(state_hold),
      .state_scan_step(state_scan_step),.state_scan_in(state_scan_in),.state_scan_out(state_scan_out),'''+wrapper[b:]
    clones['cell_MCS6502_snapshot.sv'] = wrapper
    a = text.index(' wire state_hold=')
    # Replace the complete qualified board fabric; all original ordinary
    # instances/pins and grouped SRAM wiring below are byte-preserved.
    b = text.index(' assign state_child_frozen[0]=',a)
    regular = [r for r in old['packages'] if r['type'] not in ('FF','CPU')]
    valid = " (state_package==0 && state_row<11'd112) ||\n"+' ||\n'.join(
        f" (state_package==6'd{r['index']} && state_row<11'd{r['words_count']})" for r in regular)
    fabric = ''' wire [2003:0] scan_bits;
 wire [2003:0] scan_next={scan_serial_in,scan_bits[2003:1]};
 wire scan_serial_in,scan_step,scan_ready,scan_busy,scan_done,scan_error;
 wire [17:0] scan_result;
 wire state_hold=(state_freeze && state_guest_held) || state_scan_hold_required;
 reg state_boundary_ready=1'b0;
 wire state_accept=state_freeze && state_guest_held && state_frozen && state_request && !scan_busy && !state_scan_fault && !state_system_reset;
 wire [5:0] state_package=state_index[16:11];wire [10:0] state_row=state_index[10:0];
 wire [17:0] state_child_result[0:47];wire [47:0] state_child_frozen;
 wire state_is_scan=state_package==0 && state_row<11'd112;
 wire state_valid=
'''+valid+''';
 reg [5:0] state_response_package;
 reg state_response_child,state_response_scan;
 reg regular_done=1'b0,regular_error=1'b0;
 assign state_frozen=!state_system_reset && state_boundary_ready && (&state_child_frozen);
 assign state_done=regular_done || scan_done;
 assign state_error=regular_error || scan_error;
 assign state_result=state_response_scan?scan_result:(state_response_child?state_child_result[state_response_package]:18'b0);
 always @(posedge state_host_clk)begin
  state_boundary_ready<=state_hold && !state_system_reset;
  regular_done<=state_accept && !state_is_scan;
  regular_error<=state_accept && !state_valid;
  if(state_accept)begin
   state_response_scan<=state_is_scan;
   state_response_child<=state_valid && !state_is_scan && !state_write;
   state_response_package<=state_package<6'd48 ? state_package :6'd0;
  end
 end
 dorado_ff_scan_controller #(.STATE_BITS(2004)) u_ff_scan(
  .clk(state_host_clk),.reset(state_system_reset),.freeze(state_freeze),.guest_held(state_guest_held),
  .request(state_accept && state_is_scan),.write(state_write),.index({13'b0,state_row}),.data(state_data),
  .serial_out(scan_bits[0]),.serial_in(scan_serial_in),.scan_step(scan_step),.ready(scan_ready),
  .hold_required(state_scan_hold_required),.busy(scan_busy),.done(scan_done),.error(scan_error),
  .fault(state_scan_fault),.result(scan_result));
'''
    text = text[:a]+fabric+text[b:]
    for row in old['packages']:
        if row['type'] in ('FF','CPU'):
            if row['type']=='CPU':text = once(text, 'endmodule', ' assign state_child_frozen[47]=state_hold;\nendmodule')
            text = once(text, 'endmodule', f" assign state_child_result[{row['index']}]=18'b0;\nendmodule")
    # Quarantine must hold EVERY local consumer of the rotated clock/CPU pins.
    assert text.count('.state_freeze(state_freeze),.state_guest_held(state_guest_held)') == 13
    text = text.replace('.state_freeze(state_freeze),.state_guest_held(state_guest_held)',
                        '.state_freeze(state_hold),.state_guest_held(state_hold)')
    assert text.count('.freeze(state_freeze),.guest_held(state_guest_held)') == 5  # four SRAMs + scan controller
    # Keep the controller observing the EXTERNAL boundary so it detects loss.
    needle='.freeze(state_freeze),.guest_held(state_guest_held),\n .frozen('
    assert text.count(needle)==4
    text=text.replace(needle,'.freeze(state_hold),.guest_held(state_hold),\n .frozen(')
    clones['ff_scan_controller.sv']=(Path(__file__).parent/'ff_scan_controller.sv').read_text()
    manifest.update(generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        qualified_parallel_predecessor_sha256=predecessor,state_access_schema='compact-ff-cpu-v1',
        scan=dict(state_bits=2004,words=112,word_bits=18,base_word=0,layout=layout,
                  word_map=[dict(index=i,lsb=18*i,width=min(18,2004-18*i)) for i in range(112)],
                  last_word_bits=6,completion_edges=2004),
        cloned_sources={n:hashlib.sha256(raw_bytes(s)).hexdigest() for n,s in clones.items()},scope=__doc__)
    assert manifest['logical_state_bits']==40366
    return text,manifest,clones


if __name__=='__main__':
    import argparse,json
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=generate(a.source_root.resolve());(a.output/'BaseBd_snapshot.sv').write_text(board)
    for n,s in clones.items():(a.output/n).write_bytes(raw_bytes(s))
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
