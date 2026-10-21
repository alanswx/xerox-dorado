// ContA a01 CP decoder and j02/i02 maintenance-control registers.
// Original .wl pin order; MC10161 active-low outputs clock MC10176 on release.
// Component only: a caller supplies actual CP bus before/after each event.
// No initial state, extra pipeline cycle or host completion is introduced.
`default_nettype none
module dorado_cp_control_step(
 input wire [11:0] state_q,
 // [6:0]=CPOut.7..1; [9:7]={CPAddr.0',CPAddr.1',CPAddr.2'};
 // [10]=CPStrb'. Clock/data changes must obey original setup/hold.
 input wire [10:0] pins_before,pins_after,
 output reg [11:0] state_next,
 output wire [1:0] clock_events,
 output wire [1:0] clocks_after
);
 function automatic [1:0] clocks(input [10:0] pins);
   clocks={pins[10] | (pins[9:7]!=3'b110),
           pins[10] | (pins[9:7]!=3'b111)};
 endfunction
 assign clocks_after=clocks(pins_after);
 assign clock_events=clocks_after & ~clocks(pins_before);
 always @* begin
   state_next=state_q;
   // j02 {NoDispatch,rMIRa,rCT,Jam,StopAtT1,rStop}.
   if(clock_events[0]) state_next[5:0]={pins_after[3],pins_after[4],pins_after[5],
                                      pins_after[2],pins_after[1],pins_after[0]};
   // i02 {unused,unused,ClrReady,unused,CP=UseCPReg,GetTLink}.
   // The two unwired D pins have the original ECL input pulldown value.
   if(clock_events[1]) state_next[11:6]={2'b00,pins_after[6],pins_after[1],
                                       pins_after[3],pins_after[2]};
 end
endmodule
`default_nettype wire
