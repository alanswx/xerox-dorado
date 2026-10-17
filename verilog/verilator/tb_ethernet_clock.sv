// The board's oscillator must keep absolute time at every CPU sample ratio.
`timescale 1ns/1ps
module tb_ethernet_clock;
  reg clk=0;always #1 clk=~clk;
  DskEth_m_Rev_m_Cf #(.SYSPER(4)) a(.sys_clk(clk));
  DskEth_m_Rev_m_Cf #(.SYSPER(8)) b(.sys_clk(clk));
  DskEth_m_Rev_m_Cf #(.SYSPER(16)) c(.sys_clk(clk));
  integer na=0,nb=0,nc=0;
  reg pa,pb,pc;
  task check_rate(input integer edges,input integer ratio);
    real expected;
    expected=20000.0*30.0*47060.0/(ratio*1000000.0);
    if(edges<expected-2||edges>expected+2)
      $fatal(1,"Ethernet crystal wrong at SYSPER=%d: edges=%d expected=%f",ratio,edges,expected);
  endtask
  initial begin
    repeat(2000)@(negedge clk);
    pa=a.EClk0;pb=b.EClk0;pc=c.EClk0;
    repeat(20000)begin
      @(negedge clk);
      if(a.EClk0!=pa)na++;if(b.EClk0!=pb)nb++;if(c.EClk0!=pc)nc++;
      pa=a.EClk0;pb=b.EClk0;pc=c.EClk0;
    end
    $display("ETHERNET-CLOCK edges: SYSPER4=%d SYSPER8=%d SYSPER16=%d",na,nb,nc);
    check_rate(na,4);check_rate(nb,8);check_rate(nc,16);
    $display("ETHERNET-CLOCK PASS: 23.530 MHz at all three CPU sample ratios");
    $finish;
  end
endmodule
