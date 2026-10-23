// Control-A MIR storage: ContA a23..e24 plus the separate g16 PrBlock copy.
// Physical Q order is bound by inventory_mir_clock_boundaries.py. Do not merge
// d23's two JCN.2or3 drivers or g16's clock with the e23 Block capture.
// MC10231: common and individual clocks OR; active-high S dominates R.
// Component setup contract: data/maintenance inputs settle before clock edges.
// No reset, initialization or extra architectural pipeline stage is added.
// HM section 2.3's complete 60 ns cadence is NOT established by this slice.
`default_nettype none
module dorado_mir_capture_step(
 input wire [21:0] state_q,
 // [7:0] dFF.i, [15:8] dJCN.i, [16] dIMRH, [17] dBlock';
 // [25:18] sFF.i, [33:26] sJCN.i, [34] sIMRH, [35] sBLOCK;
 // [38:36] brMIRc..a; [44:39] h*clk0'Cf..Ca.
 input wire [44:0] pins_before,pins_after,
 output reg [21:0] state_next,
 output wire [5:0] clock_events
);
 assign clock_events=pins_after[44:39] & ~pins_before[44:39];
 reg [21:0] data,set_q,reset_q,capture;
 always @* begin
  data[7:0]={pins_after[4],pins_after[5],pins_after[6],pins_after[7],
             pins_after[0],pins_after[1],pins_after[2],pins_after[3]};
  data[15:8]={pins_after[10],pins_after[11],pins_after[10],pins_after[11],
              pins_after[12],pins_after[13],pins_after[8],pins_after[9]};
  data[19:16]={pins_after[14],pins_after[15],pins_after[17],pins_after[16]};
  data[20]=1'b0;data[21]=pins_after[17];
  set_q[7:0]={pins_after[22],pins_after[23],pins_after[24],pins_after[25],
              pins_after[18],pins_after[19],pins_after[20],pins_after[21]};
  set_q[15:8]={pins_after[28],pins_after[29],pins_after[28],pins_after[29],
               pins_after[30],pins_after[31],pins_after[26],pins_after[27]};
  set_q[19:16]={pins_after[32],pins_after[33],pins_after[36],pins_after[34]};
  set_q[20]=1'b0;set_q[21]=pins_after[38];
  reset_q[7:0]={8{pins_after[36]}};
  reset_q[15:8]={8{pins_after[37]}};
  reset_q[19:16]={pins_after[37],pins_after[37],pins_after[35],pins_after[37]};
  reset_q[20]=1'b0;reset_q[21]=pins_after[35];
  capture={{1{clock_events[5]}},1'b0,{4{clock_events[4]}},{4{clock_events[3]}},
           {4{clock_events[2]}},{4{clock_events[1]}},{4{clock_events[0]}}};
  state_next=state_q;
  for(integer i=0;i<22;i=i+1)begin
   if(set_q[i])state_next[i]=1'b1;
   else if(reset_q[i])state_next[i]=1'b0;
   else if(capture[i])state_next[i]=data[i];
  end
 end
endmodule
`default_nettype wire
