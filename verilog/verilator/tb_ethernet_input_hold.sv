// Receiver Input side effects must use setup at the board clock edge.
// Available word and no refill are component stimuli; no guest completion.
`timescale 1ns/1ps
module tb_ethernet_input_hold;
  reg clk=0; always #1 clk=~clk;
  reg clock1=0,input_n=0,next_clock=0,next_input_n=0;
  always @(posedge clk)begin clock1<=next_clock;input_n<=next_input_n;end
  DskEth_m_Rev_m_Cf d(.sys_clk(clk));
  task settle;repeat(4)@(negedge clk);endtask
  initial begin
    force d.Clock1_p_Bb=clock1;
    force d.bIOin_p_=input_n;
    force d.TIOA_eq_EthData_p_=0;
    force d.RxReadFifo_p_=1;
    force d.RxBOP=1;
    settle();d.u_k04.qb=1;settle();
    // Input was asserted before clk1 and becomes disabled on that edge.
    // Its bus-register word has been read; full must clear exactly once.
    next_clock=1;next_input_n=1;settle();
    if(d.RxBusRegFull)$fatal(1,"completed Input missed receive full clear at clk1");
    next_clock=0;settle();next_clock=1;settle();
    if(d.RxBusRegFull)$fatal(1,"receive register became full with no refill");
    // Conversely, an Input enabled on the edge has not read this setup
    // word yet. Retain it until the following clock, then clear it.
    next_clock=0;settle();d.u_k04.qb=1;settle();
    next_clock=1;next_input_n=0;settle();
    if(!d.RxBusRegFull)$fatal(1,"Input becoming enabled consumed a word before its setup window");
    next_clock=0;settle();next_clock=1;settle();
    if(d.RxBusRegFull)$fatal(1,"stable Input failed to consume the setup word");
    $display("RECEIVE INPUT HOLD PASS: retiring and newly enabled Input use the setup word");
    $finish;
  end
endmodule
