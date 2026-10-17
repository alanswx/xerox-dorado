// HM Figure 11: a nonzero syndrome with an even number of ones is a
// double error. Such an error must not be mistaken for a correctable bit.
`timescale 1ns/1ps
`default_nettype none
module tb_memory_ecc_classify;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg [7:0] syndrome=0;
  MemD_m_Rev_m_Ca d (.sys_clk(sys_clk),.CLK_md_p_(1'b0));
  initial begin
    force d.Syn0_p_=~syndrome[7]; force d.Syn1_p_=~syndrome[6];
    force d.Syn2_p_=~syndrome[5]; force d.Syn3=syndrome[4];
    force d.tSyn4=syndrome[3]; force d.tSyn5=syndrome[2];
    force d.tSyn6=syndrome[1]; force d.Syn7_p_=~syndrome[0];
    force d.Error_p_=(syndrome==0);
    for(integer i=0;i<256;i=i+1) begin
      syndrome=i[7:0]; repeat(2) @(negedge sys_clk);
      if(d.DoubleError_p_ !== !(syndrome!=0 && (($countones(syndrome)%2)==0)))
        $fatal(1,"syndrome=%h DoubleError'=%b, expected nonzero-even classification",syndrome,d.DoubleError_p_);
    end
    $display("tb_memory_ecc_classify: PASS -- all 256 syndromes classify nonzero even parity as double error");
    $finish;
  end
endmodule
`default_nettype wire
