#!/usr/bin/env python3
"""Expose joint control capture frames and share them with processor consumers.

Each binary atomic transfer has four proved settling waves. A frame carries
48 original controller/task Qs before and after capture, and12 physical capture
events. Two framesets describe CP and root arrival. These are combinational
wires, with no owner, reset, RAM or architectural cycle added. Unknown inputs
retain the original local evaluator. Native source/arrival and timing remain
separate qualification gates.
"""
import re

from prove_cp_prefix_cache import module

CONTROL=['dorado_control_hold_ordered_step','dorado_control_hold_atomic_step','dorado_control_terminal_body',
         'dorado_control_certified_fall_step']
ADAPTERS=['dorado_local_r_front_ready_step','dorado_local_r_front_step','dorado_local_r_memory_step',
          'dorado_local_r_address_step','dorado_local_r_flags_step','dorado_local_r_operand_step',
          'dorado_local_t_parity_step','dorado_local_t_file_step']

KNOWN=''' function automatic shared_inputs_known(input [1940:0] context_bits);
  begin
   case(^context_bits)
    1'b0,1'b1:shared_inputs_known=1'b1;
    default:shared_inputs_known=1'b0;
   endcase
  end
 endfunction
'''


def change(text,old,new):
    assert text.count(old)==1,old
    result=text.replace(old,new,1);assert result.replace(new,old,1)==text
    return result


def calls(text,name,extra):
    """Append an explicit port only to positional instances of this module."""
    pattern=r'(?m)^ '+re.escape(name)+r'(?:\s+#\([^\n]*\))?\s+\w+\('
    matches=list(re.finditer(pattern,text));assert matches,name
    for m in reversed(matches):
        start=m.end();depth=1;i=start
        while depth:
            if text[i]=='(':depth+=1
            if text[i]==')':depth-=1
            i+=1
        old=text[start:i-1];assert not old.lstrip().startswith('.'),name
        text=text[:i-1]+','+extra+text[i-1:]
    return text,len(matches)


def control(raw,mutation=None):
    assert mutation in [None,'wrong-start']
    terminal=module(raw,CONTROL[2]);atomic=module(raw,CONTROL[1]);ordered=module(raw,CONTROL[0])
    t=change(terminal,'output wire [WAVES-1:0] observed_mask,observed_changed,observed_clear);',
             'output wire [WAVES-1:0] observed_mask,observed_changed,observed_clear,\n output reg [108*WAVES-1:0] capture_frames);')
    t=change(t,'  old_ap=ca_pins(state_q,pins_after);','  capture_frames=0;\n  old_ap=ca_pins(state_q,pins_after);')
    marker='   old_view=mir_switch_view;old_tasking_view=tasking_view;'
    t=change(t,marker,'   capture_frames[108*wave+:108]={n[47:0],q[47:0],capture[12],capture[10:0]};\n'+marker)
    if mutation=='wrong-start':
        t=change(t,'{n[47:0],q[47:0],capture[12],capture[10:0]}',
                 "{n[47:0],q[47:0]^(48'b1<<23),capture[12],capture[10:0]}")
    a=change(atomic,'input wire refresh_before_run_capture,output reg [93:0] flags);',
             'input wire refresh_before_run_capture,output reg [93:0] flags,output wire [431:0] capture_frames);')
    a=change(a,' wire [155:0] short_q,original_q;',
             ' wire [108*WAVES-1:0] unused_frames;\n wire [155:0] short_q,original_q;')
    a=change(a,'mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear);',
             'mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear,unused_frames);')
    a=change(a,'short_flags,unused_mask,unused_changed,unused_clear);',
             'short_flags,unused_mask,unused_changed,unused_clear,capture_frames);')
    a=change(a,' end else begin: unchanged_parameter',
             " end else begin: unchanged_parameter\n  assign capture_frames=432'b0;")
    o=change(ordered,'output wire [268:0] prefix_live);',
             'output wire [268:0] prefix_live,output wire [863:0] capture_frames);')
    o=change(o,' wire split_event=',
             ' wire [431:0] prefix_frames,staged_frames,direct_frames;\n assign capture_frames={fast_path ? direct_frames:staged_frames,prefix_frames};\n wire split_event=')
    o=change(o,'  refresh_before_run_capture,);',
             '  refresh_before_run_capture,,prefix_frames);')
    o=change(o,'  refresh_before_run_capture,staged_flags);',
             '  refresh_before_run_capture,staged_flags,staged_frames);')
    o=change(o,'  refresh_before_run_capture,direct_flags,fast_path,);',
             '  refresh_before_run_capture,direct_flags,fast_path,,direct_frames);')
    c=change(module(raw,CONTROL[3]),'input wire entry_canonical,output reg using_sparse);',
             'input wire entry_canonical,output reg using_sparse,output wire [431:0] capture_frames);')
    c=change(c,'  refresh_before_run_capture,full_result[93:0]);',
             '  refresh_before_run_capture,full_result[93:0],capture_frames);')
    # A fast entry bypasses the retained PREFIX payload, whose older key may
    # be unrelated. Use the actual direct evaluator's frames in that branch.
    # Its sparse fall remains governed by the original certificate. Paired
    # sparse-first consumers still retain their prior evaluator.
    code='\n'.join([o,a,t,c])
    for name in CONTROL:code=re.sub(r'\b'+name+r'\b',name.replace('dorado_control_','dorado_capture_'),code)
    return code


SHARED='''module dorado_local_task_capture_atomic_step #(parameter integer WAVES=16)(
 input wire stop_tasks_capture,input wire [1:0] canonical_task_clock,
 input wire [87:0] state_q,input wire clock_before,clock_after,
 input wire [75:0] pins_before,pins_after,output reg [87:0] state_next,
 output reg [24:0] clock_events,repeated_events,output reg unsettled,
 input wire [432:0] shared_capture);
 FUNCTIONS
 wire [87:0] original_q;wire [24:0] original_events,original_repeated;wire original_unsettled;
 dorado_local_task_capture_atomic_original_step #(.WAVES(WAVES)) original(
  stop_tasks_capture,canonical_task_clock,state_q,clock_before,clock_after,pins_before,pins_after,
  original_q,original_events,original_repeated,original_unsettled);
 reg [87:0] q,n;reg [24:0] previous,levels,captures,events,repeated;
 reg [27:0] p,source_p;reg [107:0] frame;reg [11:0] control_captures;
 reg settled,repeat_curr;
 always @*begin
  q=state_q;n=q;source_p=capture_pins(state_q,pins_after);previous=clocks(q,clock_before,pins_before);
  levels=previous;captures=0;events=0;repeated=0;settled=0;p=0;frame=0;control_captures=0;
  repeat_curr=pins_after[68] && state_q[75];
  for(integer wave=0;wave<WAVES;wave++)begin
   if(wave<4)begin
    frame=shared_capture[108*wave+:108];control_captures=frame[11:0];
   end else begin
    frame={shared_capture[431:384],shared_capture[431:384],12'b0};control_captures=0;
   end
   q[31:0]=frame[43:12];q[79:64]=frame[59:44];
   p=capture_pins(q,pins_after);levels=clocks(q,clock_after,pins_after);captures=levels & ~previous;
   captures[8:0]=control_captures[8:0];captures[19]=control_captures[9];
   captures[20]=control_captures[10];captures[21]=control_captures[10];captures[22]=control_captures[11];
   repeated|=captures&events;events|=captures;
   n=q;n[31:0]=frame[91:60];n[79:64]=frame[107:92];
   n[47:32]=processor_transfer(q[47:32],captures[13:9],source_p,!q[23],1'b0);
   n[63:48]=processor_transfer(q[63:48],captures[18:14],source_p,!q[23],1'b1);
   if(captures[23])n[83:80]=repeat_curr ? state_q[43:40] : (!state_q[32] ? state_q[47:44] : ~source_p[23:20]);
   if(captures[24])n[87:84]=repeat_curr ? state_q[59:56] : (!state_q[48] ? state_q[63:60] : ~source_p[23:20]);
   settled=(n==q && levels==previous);previous=levels;q=n;
  end
  repeated[22]|=canonical_task_clock[1];
  {state_next,clock_events,repeated_events,unsettled}={original_q,original_events,original_repeated,original_unsettled};
  case(^{state_q,clock_before,clock_after,pins_before,pins_after,stop_tasks_capture,canonical_task_clock,shared_capture})
   1'b0,1'b1:if(shared_capture[432]){state_next,clock_events,repeated_events,unsettled}={q,events,repeated,!settled};
   default:begin end
  endcase
 end
endmodule
'''


def task_adapter(raw):
    original=module(raw,'dorado_local_task_capture_atomic_step')
    functions=original[original.index('    function automatic'):original.index('    wire [75:0] task_data_pins=')]
    # Do not copy task/controller TRANSFER functions into the new consumer.
    # They remain solely in its exact unknown/unselected fallback.
    for name in ['controller_transfer','task_transfer']:
        pattern=r'    function automatic[^\n]*\b'+name+r'\([^;]*;.*?    endfunction\n'
        functions,n=re.subn(pattern,'',functions,count=1,flags=re.S);assert n==1,name
    old=original.replace('module dorado_local_task_capture_atomic_step #','module dorado_local_task_capture_atomic_original_step #',1)
    wrapper=module(raw,'dorado_local_task_capture_step')
    wrapper=change(wrapper,'output wire [24:0] clock_events,repeated_events,output wire unsettled);',
                   'output wire [24:0] clock_events,repeated_events,output wire unsettled,input wire [864:0] shared_capture);')
    wrapper=change(wrapper,'  prefix_q,prefix_events,prefix_repeated,prefix_unsettled);',
                   '  prefix_q,prefix_events,prefix_repeated,prefix_unsettled,{shared_capture[864],shared_capture[431:0]});')
    wrapper=change(wrapper,'  state_next,root_events,root_repeated,root_unsettled);',
                   '  state_next,root_events,root_repeated,root_unsettled,{shared_capture[864],shared_capture[863:432]});')
    assert 'controller_transfer(' not in SHARED and 'task_transfer(' not in SHARED
    return old+'\n'+SHARED.replace('FUNCTIONS',functions)+'\n'+wrapper


def adapter(raw):
    name=re.search(r'^module (\w+)\(',raw,re.M)[1];assert name in ADAPTERS,name
    i=raw.index(');',raw.index('module '+name+'('))
    raw=raw[:i]+',input wire [864:0] shared_capture'+raw[i:]
    count=0
    for child in ADAPTERS+['dorado_local_task_capture_step']:
        if re.search(r'(?m)^ '+child+r' ',raw):raw,n=calls(raw,child,'shared_capture');count+=n
    assert count>=1,name
    return raw


def kernel(raw,enabled=True):
    # Pure/packet processor modules have the same child signature. Leave the
    # original native module unused but compilable with the fallback selected.
    count=0
    for name in ['dorado_r_front_control_step','dorado_r_front_pure_step','dorado_r_front_prefix_step','dorado_r_front_parallel_fault_step']:
        if not re.search(r'^module '+name+r'\b',raw,re.M):continue
        before=module(raw,name);after=before
        if name in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
            after,n=calls(after,'dorado_local_r_front_ready_step',"865'b0");assert n==1
        else:
            old=' dorado_control_hold_ordered_step control('
            after=change(after,old,' dorado_capture_hold_ordered_step control(')
            after=change(after,' wire cp_commit=advance && cp_ready;',
                         ' wire cp_commit=advance && cp_ready;\n'+
                         ' wire [863:0] shared_frames;\n'+KNOWN+' wire [864:0] shared_capture={'+"1'b"+str(int(enabled))+''' && !pair_fall &&
  shared_inputs_known({held_q,clock_before,clock_after,pins_before,pins_after,
    refresh_before_run_capture,cp_fast,cp_prefix_saved,cp_prefix_cp_saved,cp_after_cp_saved}),shared_frames};
''')
            after,n=calls(after,'dorado_capture_hold_ordered_step','shared_frames');assert n==1
            after,n=calls(after,'dorado_local_r_front_ready_step','shared_capture');assert n==1
            if name=='dorado_r_front_parallel_fault_step':
                after=change(after,'.original_fault(child_faults[1]),','.shared_capture(shared_capture),\n  .original_fault(child_faults[1]),')
        raw=change(raw,before,after);count+=1
    # Its two cofactor consumers receive the SAME descriptor, not another
    # controller calculation. Data-only FF cofactors retain their own inputs.
    if 'module dorado_parallel_r_fault_step(' in raw:
        before=module(raw,'dorado_parallel_r_fault_step');after=change(before,
            'input wire original_fault,output reg fault);',
            'input wire original_fault,output reg fault,input wire [864:0] shared_capture);')
        for instance in ['early_zero','early_one']:
            start=after.index('dorado_local_r_front_ready_step '+instance+'(');end=after.index(');',start)
            after=after[:end]+',\n  .shared_capture(shared_capture)'+after[end:]
        raw=change(raw,before,after)
    assert count>=1
    return raw
