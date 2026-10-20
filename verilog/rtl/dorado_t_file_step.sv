// Task-selection/capture pipeline plus the original T-data/storage path.
// Component experiment; not instantiated in the native core. The Pdata/dMD
// buses and unused g18 D pins remain explicit physical boundary inputs.
// Q[87:0] = task_capture_step; [93:88]/[99:94] = H/L g18 (six Q each);
// [107:100]/[115:108] = H/L k01+k02 mux latches, raw bit-number order;
// [116+16*slot+:16] = four T-file SRAM slices at physical address slot.
// [387:372] = j03+j04 read/IFU latches; [403:388] = i03+i04 T latches.
// pins[75:0] = task_capture interface; [91:76] = {Pdata.15..Pdata.00};
// [107:92] = {dMD.15..dMD.00}; [111:109] = H g18 D6,D10,D11;
// [108] reserved zero; H g18 D5 is FF076 OR Freeze, D7 is unconnected.
// [116:112] = L g18 D5,D6,D7,D10,D11 (low bit is first pin).
// [117] = TisIFdata; [118] = SignIfuData; [126:119] = IfuData.7..0.
// The inherited arrival/setup contract applies. No analog or native-speed
// qualification is implied; ALU/IFU and memory Hold producers remain outside.
`default_nettype none
module dorado_t_file_step(
 input wire [403:0] state_q,input wire clock_before,clock_after,
 input wire [126:0] pins_before,pins_after,
 output reg [403:0] state_next,output wire [24:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass);
 wire [87:0] core_next;
 dorado_task_capture_step core(state_q[87:0],clock_before,clock_after,
   pins_before[75:0],pins_after[75:0],core_next,clock_events,repeated_events,unsettled);
 wire pp_h=clock_after | !core_next[13] | core_next[12];
 wire pp_l=clock_after | !core_next[8] | core_next[10];
 assign latch_n={pp_l | core_next[48],pp_h | core_next[32]};
 // HoldDly g18 and the T write-enable are captured on the original clocks.
 wire hold_h=clock_events[12] ? pins_after[68] : state_q[93];
 wire hold_l=clock_events[17] ? pins_after[68] : state_q[99];
 assign write_n={pp_l | !core_next[23] | hold_l | core_next[54],
                 pp_h | !core_next[23] | hold_h | core_next[38]};
 assign t_latch_n={~core_next[48],~core_next[32]};
 assign bypass={!(hold_l | core_next[55] | core_next[54] | pins_after[117]),
                !(hold_h | core_next[39] | core_next[38] | pins_after[117])};
 wire [3:0] address_h={core_next[80],core_next[81],core_next[82],core_next[83]};
 wire [3:0] address_l={core_next[84],core_next[85],core_next[86],core_next[87]};
 always @* begin
   state_next=state_q;state_next[87:0]=core_next;
   if(clock_events[12]) state_next[93:88]={pins_after[68],pins_after[111:110],1'b0,pins_after[109],
     ((pins_after[19:12]==8'o076) | pins_after[0])};
   if(clock_events[17]) state_next[99:94]={pins_after[68],pins_after[116:112]};
   if(!latch_n[0]) state_next[107:100]=core_next[36] ? pins_after[99:92] : pins_after[83:76];
   if(!latch_n[1]) state_next[115:108]=core_next[52] ? pins_after[107:100] : pins_after[91:84];
   if(!write_n[0]) state_next[116+16*address_h+:8]=state_next[107:100];
   if(!write_n[1]) state_next[124+16*address_l+:8]=state_next[115:108];
   if(!latch_n[0]) state_next[379:372]=pins_after[117] ? {8{pins_after[118]}} : state_next[116+16*address_h+:8];
   if(!latch_n[1]) state_next[387:380]=pins_after[117] ? pins_after[126:119] : state_next[124+16*address_l+:8];
   if(!t_latch_n[0]) state_next[395:388]=bypass[0] ? state_next[107:100] : state_next[379:372];
   if(!t_latch_n[1]) state_next[403:396]=bypass[1] ? state_next[115:108] : state_next[387:380];
 end
endmodule
`default_nettype wire
