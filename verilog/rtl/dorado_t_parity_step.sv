// Coupled T data/parity plus R parity checking at physical operand boundaries.
// Experiment only; no native integration or original-clock-rate claim.
// State[403:0]: unchanged T path. [467:404]/[531:468]: H/L 16x4 parity
// SRAMs (only D0/Q0 wired). [535:532]/[539:536]: full H/L g16 banks,
// {Rparity,Tparity,aux-D9,aux-D7}, in original F10000 output order.
// Pins[126:0]: T path; [142:127]: R.15..00; [143]/[144]: H/L R-RAM
// parity outputs; [145]: H D9; [146]/[147]: L D9/D7; [148]/[149]:
// H RbBypassDly/RisIFdata; [150]/[151] are the independent L masks.
// R producer/storage and other inherited cuts remain.
`default_nettype none
module dorado_t_parity_step(
 input wire [539:0] state_q,input wire clock_before,clock_after,
 input wire [151:0] pins_before,pins_after,
 output reg [539:0] state_next,output wire [24:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults);
 function automatic [3:0] parity_checks(input [539:0] q,input [151:0] p);
   parity_checks={(^p[142:135])^q[539],(^q[403:396])^q[538],
                  (^p[134:127])^q[535],(^q[395:388])^q[534]};
 endfunction
 function automatic [3:0] fault_requests(input [539:0] q,input [151:0] p);
   reg [3:0] ok;
   reg bh,bl;
   begin
     ok=parity_checks(q,p);
     bh=!(q[93]|q[39]|q[38]|p[117]);bl=!(q[99]|q[55]|q[54]|p[117]);
     fault_requests={!(p[150]|p[151]|ok[3]),!(bl|p[117]|ok[2]),
                     !(p[148]|p[149]|ok[1]),!(bh|p[117]|ok[0])};
   end
 endfunction
 wire [3:0] source_faults_before=fault_requests(state_q,pins_before);
 wire [3:0] source_faults_after=fault_requests(state_q,pins_after);
 wire [126:0] child_before={pins_before[126:115],source_faults_before[2],source_faults_before[3],pins_before[112:0]};
 wire [126:0] child_after={pins_after[126:115],source_faults_after[2],source_faults_after[3],pins_after[112:0]};
 wire [403:0] t_next;
 dorado_t_file_step t_path(state_q[403:0],clock_before,clock_after,child_before,child_after,
   t_next,clock_events,repeated_events,unsettled,write_n,latch_n,t_latch_n,bypass);
 wire [3:0] old_h_address={state_q[80],state_q[81],state_q[82],state_q[83]};
 wire [3:0] old_l_address={state_q[84],state_q[85],state_q[86],state_q[87]};
 wire [3:0] h_address={t_next[80],t_next[81],t_next[82],t_next[83]};
 wire [3:0] l_address={t_next[84],t_next[85],t_next[86],t_next[87]};
 always @* begin
   state_next=state_q;state_next[403:0]=t_next;
   // Clock1 captures the SRAM output before DblClock's address-register Q
   // exposes the other task's entry. Preserve that source epoch explicitly.
   if(clock_events[13]) state_next[535:532]={pins_after[143],state_q[404+4*old_h_address],pins_after[145],1'b0};
   if(clock_events[18]) state_next[539:536]={pins_after[144],state_q[468+4*old_l_address],pins_after[146],pins_after[147]};
   if(!write_n[0]) state_next[404+4*h_address+:4]={3'b0,~^t_next[107:100]};
   if(!write_n[1]) state_next[468+4*l_address+:4]={3'b0,~^t_next[115:108]};
 end
 assign parity_ok=parity_checks(state_next,pins_after);
 assign faults=fault_requests(state_next,pins_after);
endmodule
`default_nettype wire
