// A held Input can become enabled on clk1 as memory releases IOHold. The
// OutRegFull JK must use its setup inputs, like the underflow-check D flop.
// Post-edge JK capture consumes the word a clock before the error checker,
// which then reports an underflow despite every payload word being present.
`timescale 1ns/1ps
module tb_disk_input_hold;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg clock1=0, input_n=1, next_clock=0, next_input_n=1;
  always @(posedge sys_clk) begin
    clock1<=next_clock;
    input_n<=next_input_n;
  end
  DskEth_m_Rev_m_Cf d (.sys_clk(sys_clk), .CLK_disk_p_(1'b0));
  task settle;
    repeat(4) @(negedge sys_clk);
  endtask
  initial begin
    force d.Clock1_p_Ca=clock1;
    force d.bIOin_p_=input_n;
    force d.TIOA_eq_Data_p_=0;
    force d.TIOA_eq_Us_p_=0;
    force d.FifoEmpty=1;
    force d.OutRegWrite_p_=1;
    force d.ClearErrors=0;
    settle;
    // One available word, with no refill. The device's output register is
    // test stimulus; no processor or FIFO service loop is substituted here.
    @(negedge sys_clk); d.u_c16.qb=1; d.u_c15.qb=0;
    settle;
    next_clock=1; next_input_n=0; settle;
    if(!d.OutRegFull)
      $fatal(1,"Input becoming enabled on clk1 consumed OutReg before its setup window");
    next_clock=0; settle;
    next_clock=1; settle;
    if(d.OutRegFull || d.FifoUnderflow)
      $fatal(1,"valid Input must consume one word without underflow: full=%b underflow=%b",d.OutRegFull,d.FifoUnderflow);
    next_input_n=1; settle;
    $display("tb_disk_input_hold: PASS -- Input resumed at clk1 consumes exactly one word without false underflow");
    $finish;
  end
endmodule
