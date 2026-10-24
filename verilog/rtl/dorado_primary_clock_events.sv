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
 reg [18:0] bb_phase=0;
 reg [17:0] ether_phase=0;
 reg bb_level=0,ether_level=0;
 wire [19:0] bb_sum={1'b0,bb_phase}+20'd141180;
 wire [18:0] ether_sum={1'b0,ether_phase}+19'd141180;
 wire bb_due=bb_sum>=20'd470600;
 wire ether_due=ether_sum>=19'd200000;
 wire [18:0] bb_remaining=19'd470600-bb_phase;
 wire [17:0] ether_remaining=18'd200000-ether_phase;
 assign levels_before={ether_level,bb_level,inner_before};
 reg [17:0] candidate_time[0:5];
 reg [5:0] candidate_mask[0:5];
 reg [17:0] first;
 reg [5:0] cursor,combined;
 always @*begin
  event_count=0;offsets=0;masks=0;levels=0;
  cursor=levels_before;first=UNUSED;combined=0;
  for(integer i=0;i<6;i++)begin candidate_time[i]=UNUSED;candidate_mask[i]=0;end
  if(advance && !reset)begin
   for(integer i=0;i<4;i++)if(i<int'(inner_count))begin
    candidate_time[i]=18'(inner_offsets[4*i+:4])*18'd11765;
    candidate_mask[i]={2'b0,inner_masks[4*i+:4]};
   end
   if(bb_due)begin candidate_time[4]=18'(bb_remaining);candidate_mask[4]=6'b010000;end
   if(ether_due)begin candidate_time[5]=ether_remaining;candidate_mask[5]=6'b100000;end
   for(integer record=0;record<6;record++)begin
    first=UNUSED;
    for(integer i=0;i<6;i++)if(candidate_time[i]<first)first=candidate_time[i];
    combined=0;
    if(first<=INTERVAL)begin
     for(integer i=0;i<6;i++)if(candidate_time[i]==first)begin
      combined=combined|candidate_mask[i];candidate_time[i]=UNUSED;
     end
     cursor=cursor^combined;
     offsets[18*event_count+:18]=first;
     masks[6*event_count+:6]=combined;
     levels[6*event_count+:6]=cursor;
     event_count=event_count+3'd1;
    end
   end
  end
  levels_after=cursor;
 end
 always @(posedge clk)begin
  if(reset)begin bb_phase<=0;ether_phase<=0;bb_level<=0;ether_level<=0;end
  else if(advance)begin
   bb_phase<=19'(bb_due ? bb_sum-20'd470600 : bb_sum);
   ether_phase<=18'(ether_due ? ether_sum-19'd200000 : ether_sum);
   bb_level<=bb_level^bb_due;ether_level<=ether_level^ether_due;
  end
 end
endmodule
`default_nettype wire
