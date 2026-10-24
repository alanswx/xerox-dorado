// Complete active native clock sources within one nominal15ns interval.
// Reuse the proved CPU/pixel producer; merge BaseBoard10MHz and the existing
// Ethernet23530kHz model without rounding Ethernet onto the1.25ns CPU grid.
// One unit is1/9412ns: CPU1.25ns=11765units, Ether half-period=200000units.
// Six records bound four CPU/pixel records plus one edge of each slow source.
// Cold zero-phase crystals actually need at most five because BaseBoard
// transitions coincide with pixel transitions. Keep simultaneous changes in
// ONE record, before consumers evaluate the original derived clocks/windows.
//
// UNCONNECTED source implementation. Lanes0=SCP',1=ECP,2=debug-inactive j02
// OR observation,3=raw pixel,4=BaseBoard crystal,5=Ethernet EClk0. Lane2 is
// not the actual gated CPU clock: consumers must recompute j02 debug inputs.
// reset is cold source initialization, not the guest boot/reset button.
// advance freezes transport virtual time, never architectural processor Hold.
// Physical SDRAM/DDR/ARM clocks remain outside this virtual event schedule.
`default_nettype none
module dorado_primary_clock_events #(
 parameter integer INITIAL_CPU_PHASE=0
)(
 input wire clk,reset,advance,
 output wire [5:0] levels_before,
 output reg [5:0] levels_after,
 output reg [2:0] event_count,
 output reg [107:0] offsets,
 output reg [35:0] masks,levels
);
 localparam [17:0] INTERVAL=18'd141180;
 localparam [17:0] UNUSED=18'h3ffff;
 wire [3:0] inner_before,inner_after;
 wire [2:0] inner_count;
 wire [15:0] inner_offsets,inner_masks,inner_levels;
 dorado_clock_event_batch #(.INITIAL_CPU_PHASE(INITIAL_CPU_PHASE)) inner(
  .clk(clk),.reset(reset),.advance(advance),
  .levels_before(inner_before),.levels_after(inner_after),.event_count(inner_count),
  .offsets(inner_offsets),.masks(inner_masks),.levels(inner_levels));
 reg [3:0] bb_phase_tenths=0;
 function automatic [18:0] encode_bb_phase(input [3:0] tenths);
  case(tenths)
   4'd0:encode_bb_phase=19'd0;
   4'd1:encode_bb_phase=19'd47060;
   4'd2:encode_bb_phase=19'd94120;
   4'd3:encode_bb_phase=19'd141180;
   4'd4:encode_bb_phase=19'd188240;
   4'd5:encode_bb_phase=19'd235300;
   4'd6:encode_bb_phase=19'd282360;
   4'd7:encode_bb_phase=19'd329420;
   4'd8:encode_bb_phase=19'd376480;
   4'd9:encode_bb_phase=19'd423540;
   4'd10:encode_bb_phase=19'd470600;
   default:encode_bb_phase=19'd0;
  endcase
 endfunction
 wire [18:0] bb_phase=encode_bb_phase(bb_phase_tenths);
 reg [17:0] ether_phase=0;
 reg bb_level=0,ether_level=0;
 wire [4:0] bb_tenths_sum={1'b0,bb_phase_tenths}+5'd3;
 wire [18:0] ether_sum={1'b0,ether_phase}+19'd141180;
 wire bb_due=bb_phase_tenths>=4'd7;
 wire ether_due=ether_sum>=19'd200000;
 wire [18:0] bb_remaining=encode_bb_phase(4'd10-bb_phase_tenths);
 wire [17:0] ether_remaining=18'd200000-ether_phase;
 assign levels_before={ether_level,bb_level,inner_before};
 reg [17:0] candidate_time[0:5];
 reg [5:0] candidate_mask[0:5];
 reg present[0:5];
 reg [2:0] rank[0:5];
 reg bb_matches,ether_matches,bb_insert,ether_insert;
 reg [5:0] joined,cursor;
 reg [17:0] timestamp;
 function automatic [17:0] scale_quantum(input [3:0] quantum);
  case(quantum)
   4'd1:scale_quantum=18'd11765;
   4'd2:scale_quantum=18'd23530;
   4'd3:scale_quantum=18'd35295;
   4'd4:scale_quantum=18'd47060;
   4'd5:scale_quantum=18'd58825;
   4'd6:scale_quantum=18'd70590;
   4'd7:scale_quantum=18'd82355;
   4'd8:scale_quantum=18'd94120;
   4'd9:scale_quantum=18'd105885;
   4'd10:scale_quantum=18'd117650;
   4'd11:scale_quantum=18'd129415;
   4'd12:scale_quantum=18'd141180;
   default:scale_quantum=18'd0;
  endcase
 endfunction
 always @*begin
  event_count=0;offsets=0;masks=0;levels=0;
  cursor=levels_before;joined=0;timestamp=0;
  bb_matches=0;ether_matches=0;bb_insert=0;ether_insert=0;
  for(integer i=0;i<6;i++)begin
   candidate_time[i]=UNUSED;candidate_mask[i]=0;present[i]=0;rank[i]=0;
  end
  if(advance && !reset)begin
   for(integer i=0;i<4;i++)if(i<int'(inner_count))begin
    candidate_time[i]=scale_quantum(inner_offsets[4*i+:4]);
    candidate_mask[i]={2'b0,inner_masks[4*i+:4]};present[i]=1;
   end
   if(bb_due)begin candidate_time[4]=18'(bb_remaining);candidate_mask[4]=6'b010000;end
   if(ether_due)begin candidate_time[5]=ether_remaining;candidate_mask[5]=6'b100000;end
   for(integer i=0;i<4;i++)if(present[i])begin
    if(bb_due && candidate_time[i]==candidate_time[4])begin
     bb_matches=1;candidate_mask[i]=candidate_mask[i]|candidate_mask[4];
    end
    if(ether_due && candidate_time[i]==candidate_time[5])begin
     ether_matches=1;candidate_mask[i]=candidate_mask[i]|candidate_mask[5];
    end
   end
   bb_insert=bb_due && !bb_matches;
   ether_insert=ether_due && !ether_matches && !(bb_due && candidate_time[4]==candidate_time[5]);
   if(bb_insert && ether_due && candidate_time[4]==candidate_time[5])
    candidate_mask[4]=candidate_mask[4]|candidate_mask[5];
   present[4]=bb_insert;present[5]=ether_insert;
   for(integer i=0;i<4;i++)begin
    rank[i]=3'(i)+3'(bb_insert && candidate_time[4]<candidate_time[i])+
                       3'(ether_insert && candidate_time[5]<candidate_time[i]);
    if(present[i] && candidate_time[i]<candidate_time[4])rank[4]=rank[4]+3'd1;
    if(present[i] && candidate_time[i]<candidate_time[5])rank[5]=rank[5]+3'd1;
   end
   rank[4]=rank[4]+3'(ether_insert && candidate_time[5]<candidate_time[4]);
   rank[5]=rank[5]+3'(bb_insert && candidate_time[4]<candidate_time[5]);
   event_count=inner_count+3'(bb_insert)+3'(ether_insert);
   for(integer record=0;record<6;record++)begin
    joined=0;timestamp=0;
    for(integer i=0;i<6;i++)if(present[i] && rank[i]==3'(record))begin
     joined=joined|candidate_mask[i];timestamp=timestamp|candidate_time[i];
    end
    cursor=cursor^joined;
    if(record<int'(event_count))begin
     offsets[18*record+:18]=timestamp;
     masks[6*record+:6]=joined;
     levels[6*record+:6]=cursor;
    end
   end
  end
  // BB/Ether endpoints are independent of where their records were inserted.
  levels_after={levels_before[5]^(ether_due && advance && !reset),
                levels_before[4]^(bb_due && advance && !reset),cursor[3:0]};
 end
 always @(posedge clk)begin
  if(reset)begin bb_phase_tenths<=0;ether_phase<=0;bb_level<=0;ether_level<=0;end
  else if(advance)begin
   bb_phase_tenths<=4'(bb_due ? bb_tenths_sum-5'd10 : bb_tenths_sum);
   ether_phase<=18'(ether_due ? ether_sum-19'd200000 : ether_sum);
   bb_level<=bb_level^bb_due;ether_level<=ether_level^ether_due;
  end
 end
endmodule
`default_nettype wire
