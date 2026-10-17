// The cache bank/address advances on the same modeled clk1 edge that captures
// D for the Md file. A register-to-register transfer keeps the setup value;
// sampling the next bank's output caused the disk boot's RTC read to return
// label word 007e instead of 0004. Exercise the generated board's four latches.
`timescale 1ns/1ps
module tb_md_capture;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg capture_clk=0;
  reg [15:0] bus_word=0;
  reg next_clock=0;
  reg [15:0] next_bus=0;
  always @(posedge sys_clk) begin
    capture_clk<=next_clock;
    bus_word<=next_bus;
  end
  MemD_m_Rev_m_Ca d (.sys_clk(sys_clk), .CLK_md_p_(1'b0));
  wire [17:0] captured={
    d.u_h04.p5,d.u_h04.p4,d.u_h04.p11,d.u_h04.p12,
    d.u_h15.p5,d.u_h15.p4,d.u_h15.p11,d.u_h15.p12,
    d.u_h11.p5,d.u_h11.p4,d.u_h11.p11,d.u_h11.p12,
    d.u_h17.p5,d.u_h17.p4,d.u_h17.p11,d.u_h17.p12,
    d.u_h06.p5,d.u_h06.p4};
  task sample(input [15:0] before_word,input [15:0] next_word);
    reg [17:0] expected;
    begin
      @(negedge sys_clk); next_clock=0; next_bus=before_word;
      repeat(5) @(negedge sys_clk);
      expected={before_word,~^before_word[15:8],~^before_word[7:0]};
      // Clock fanout and the bank's clock-to-Q transition share a fabric
      // sample. The receiving flip-flops must retain the pre-edge D value.
      @(negedge sys_clk); next_clock=1; next_bus=next_word;
      repeat(3) @(negedge sys_clk);
      if(captured!==expected)
        $fatal(1,"Md capture got %h, expected setup word/parity %h (next bank %h)",
          captured,expected,next_word);
      next_clock=0;
      repeat(3) @(negedge sys_clk);
    end
  endtask
  initial begin
    force d.clk1_p_Aa=capture_clk;
    force d.clk1_p_Ca=capture_clk;
    force d.clk1_p_Cb=capture_clk;
    force d.D_00=bus_word[15]; force d.D_01=bus_word[14];
    force d.D_02=bus_word[13]; force d.D_03=bus_word[12];
    force d.D_04=bus_word[11]; force d.D_05=bus_word[10];
    force d.D_06=bus_word[9];  force d.D_07=bus_word[8];
    force d.D_08=bus_word[7];  force d.D_09=bus_word[6];
    force d.D_10=bus_word[5];  force d.D_11=bus_word[4];
    force d.D_12=bus_word[3];  force d.D_13=bus_word[2];
    force d.D_14=bus_word[1];  force d.D_15=bus_word[0];
    force d.D_16=~^bus_word[15:8];
    force d.D_17=~^bus_word[7:0];
    sample(16'h0004,16'h007e);
    sample(16'h1234,16'ha55a);
    sample(16'h8001,16'h7ffe);
    sample(16'hffff,16'h0000);
    sample(16'h0000,16'hffff);
    $display("tb_md_capture: PASS -- all 16 Md bits and both parity bits retain setup data across a cache-bank change");
    $finish;
  end
endmodule
