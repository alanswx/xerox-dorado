#!/usr/bin/env python3
"""Remove unused packed-bus dependencies at control and R-address boundaries.

Only the two consumer interfaces change. Physical RAM data/parity and their
processor consumers stay live. No state, capture edge or functional pin changes.
The frozen consumer source establishes which input bits can reach each block.
"""
import hashlib
import re


def sha(raw):return hashlib.sha256(raw.encode()).hexdigest()


def rewrite_control(path):
    original=path.read_text()
    assert sha(original)=='a20b9e52134e671444e3e826ca907f3525cfad192944d04811171c099f67c32d'
    # raw_ca_pins forwards these slices; every other control input comes from
    # the CB/MC slice [397:326]. Include overwritten/unused forwarded bits too.
    mapping=re.search(r' function automatic \[130:0\] raw_ca_pins\(.*?endfunction',original,re.S)[0]
    assert mapping==''' function automatic [130:0] raw_ca_pins(input [325:0] p);
  reg [130:0] r;
  begin
   r=0;r[75:0]=p[75:0];r[111:76]=p[281:246];r[122:112]=p[292:282];
   r[123]=p[294];r[124]=p[205];r[125]=p[293];r[127:126]=p[244:243];r[128]=p[310];r[129]=p[297];r[130]=p[317];raw_ca_pins=r;
  end
 endfunction'''
    live=set(range(76))|set(range(243,245))|set(range(246,295))|{205,297,310,317}|set(range(326,398))
    # Other direct p references in joint functions are within this union.
    joint=original[original.index('module dorado_control_hold_ordered_step'):]
    assert 'b=p[397:326]'in joint and 'a=raw_ca_pins(p[325:0])'in joint
    mask=sum(1<<i for i in live)
    old='dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],'
    new=f'''// Consumer-specific inputs: unused processor data must not enter the
 // joint clock resolver merely because it shares a packed boundary bus.
 wire [397:0] control_before=pins_before[397:0] & 398'h{mask:x};
 wire [397:0] control_after=pins_after[397:0] & 398'h{mask:x};
 dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,control_before,control_after,'''
    assert original.count(old)==1
    raw=original.replace(old,new,1);assert raw.replace(new,old,1)==original
    path.write_text(raw)
    return dict(parent_sha256=sha(original),output_sha256=sha(raw),live_input_bits=sorted(live),inverse_byte_identical=True)


def rewrite_memory(path,preview):
    original=path.read_text();source=preview.read_text()
    # The address preview's only dynamic pin indexing is the bounded task
    # priority loop, which accesses bits20..64. All literal references and
    # slices are outside the RAM data/parity and adjacent processor-data slots.
    assert 'for(integer task_no=1;task_no<16;task_no=task_no+1)'in source
    assert 'p[19+task_no] || (p[34+task_no] && p[49+task_no])'in source
    refs=re.findall(r'\b(?:p|pins_before|pins_after)\[([^]]+)\]',source)
    for ref in refs:
        if ref in ['19+task_no','34+task_no','49+task_no']:continue
        assert re.fullmatch(r'\d+(?::\d+)?',ref),ref
        ends=list(map(int,ref.split(':')));assert max(ends)<127 or min(ends)>151,ref
    old='dorado_local_r_port_preview preview(stop_tasks_capture,state_q,clock_before,clock_after,pins_before,pins_after,'
    new='''// Data/parity slots127..151 are not address-preview inputs. Keep the
 // real file outputs connected to the full processor evaluator below.
 wire [201:0] preview_before={pins_before[201:152],25'b0,pins_before[126:0]};
 wire [201:0] preview_after={pins_after[201:152],25'b0,pins_after[126:0]};
 dorado_local_r_port_preview preview(stop_tasks_capture,state_q,clock_before,clock_after,preview_before,preview_after,'''
    assert original.count(old)==1
    raw=original.replace(old,new,1);assert raw.replace(new,old,1)==original
    path.write_text(raw)
    return dict(parent_sha256=sha(original),output_sha256=sha(raw),preview_sha256=sha(source),unused_input_bits=list(range(127,152)),inverse_byte_identical=True)
