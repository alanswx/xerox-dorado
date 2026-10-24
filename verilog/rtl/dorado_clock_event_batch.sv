// Natural BaseBoard/50MHz pixel events within one nominal15ns interval.
// Four intervals form an ordinary60ns microinstruction BEFORE genuine holds.
// Original BaseBd11 divider: eight3.75ns positions per30ns CPU clock;
// SCP'=g04.Q', ECP=h03.Q. Initial phase selects a steady-state rotation.
// Reuse the qualified rational pixel producer; merge at exact1.25ns quanta.
// Each record applies ALL simultaneous source changes to ONE pre-event view.
//
// NOT connected to native boards. Consumers must evaluate input arrivals at
// the interval boundary, then each record in order; re-evaluate derived clocks,
// transparent latches and SRAM windows after each event. j02's actual debug
// pulse input must be combined by the BaseBoard evaluator at EACH event.
// The ROOT bit below is only SCP'|ECP with that debug pulse inactive.
// advance freezes transport virtual time, never architectural processor Hold.
`default_nettype none
module dorado_clock_event_batch #(
 parameter integer INITIAL_CPU_PHASE=0
)(
 input wire clk,reset,advance,
 // Lane bits:0=SCP',1=ECP,2=debug-inactive j02 root,3=raw pixel.
 output wire [3:0] levels_before,
 output reg [3:0] levels_after,
 output reg [2:0] event_count,
 // Four chronological records; record0 is in the low four bits.
 // Offsets are1.25ns units in(0,12], including interval-end events.
 // Unused records are zero. Masks identify transitions, levels give the
 // complete source state AFTER the event, not just the changed bits.
 output reg [15:0] offsets,masks,levels
);
 reg [2:0] cpu_phase=3'(INITIAL_CPU_PHASE);
 wire [1:0] pixel_count;
 wire pixel_before,pixel_after;
 wire [31:0] pixel_first,pixel_second,pixel_interval;
 dorado_oscillator_events #(.SAMPLES_PER_INSTRUCTION(4),.FREQUENCY_KHZ(50000)) pixel(
  .clk(clk),.reset(reset),.advance(advance),.edge_count(pixel_count),
  .rising(),.falling(),.rising_first(),.last_at_tick(),
  .level_before(pixel_before),.level_after(pixel_after),
  .first_offset_units(pixel_first),.second_offset_units(pixel_second),.interval_units(pixel_interval));
 function automatic scp_at(input [2:0] p);
  scp_at=p<3'd2 || p>=3'd6;
 endfunction
 function automatic ecp_at(input [2:0] p);
  ecp_at=p==3'd0 || p>=3'd5;
 endfunction
 assign levels_before={pixel_before,scp_at(cpu_phase)|ecp_at(cpu_phase),ecp_at(cpu_phase),scp_at(cpu_phase)};
 reg [2:0] phase_cursor;
 reg [3:0] cursor,next_levels,changed;
 always @*begin
  event_count=0;offsets=0;masks=0;levels=0;
  phase_cursor=cpu_phase;cursor=levels_before;next_levels=cursor;changed=0;
  if(advance && !reset)for(integer quantum=1;quantum<=12;quantum++)begin
   next_levels=cursor;
   if(quantum%3==0)begin
    phase_cursor=phase_cursor+3'd1;
    next_levels[0]=scp_at(phase_cursor);next_levels[1]=ecp_at(phase_cursor);
    next_levels[2]=next_levels[0]|next_levels[1];
   end
   // The existing producer uses6,000,000 rational units per15ns interval.
   // One1.25ns quantum is500,000 such units; no rounded time division.
   if((pixel_count>=1 && pixel_first==32'(quantum*500000)) ||
      (pixel_count==2 && pixel_second==32'(quantum*500000)))next_levels[3]=!cursor[3];
   changed=cursor^next_levels;
   if(changed!=0)begin
    offsets[4*event_count+:4]=4'(quantum);
    masks[4*event_count+:4]=changed;
    levels[4*event_count+:4]=next_levels;
    event_count=event_count+3'd1;
   end
   cursor=next_levels;
  end
  levels_after=cursor;
 end
 always @(posedge clk)begin
  if(reset)cpu_phase<=3'(INITIAL_CPU_PHASE);
  else if(advance)cpu_phase<=cpu_phase+3'd4;
 end
 initial if(INITIAL_CPU_PHASE<0 || INITIAL_CPU_PHASE>7)
  $fatal(1,"CLOCK_BATCH invalid initial CPU phase");
endmodule
`default_nettype wire
