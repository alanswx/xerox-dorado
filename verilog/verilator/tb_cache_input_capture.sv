// CD advances at the same modeled edge as the clk1 fanout. The D1 cache
// input registers must keep the word present before that transition, as
// MemD01's CD -> mux -> b10 path requires. Otherwise both cache banks take
// even words and every odd word is lost during a refill.
`timescale 1ns/1ps
`default_nettype none
module tb_cache_input_capture;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg capture_clk=0,next_clock=0;
  reg [17:0] bus_word=0,next_bus=0;
  always @(posedge sys_clk) begin
    capture_clk<=next_clock;
    bus_word<=next_bus;
  end
  MemD_m_Rev_m_Ca d (.sys_clk(sys_clk), .CLK_md_p_(1'b0));
wire [17:0] captured={d.D1in_00,d.D1in_01,d.D1in_02,d.D1in_03,d.D1in_04,d.D1in_05,d.D1in_06,d.D1in_07,d.D1in_08,d.D1in_09,d.D1in_10,d.D1in_11,d.D1in_12,d.D1in_13,d.D1in_14,d.D1in_15,d.D1in_16,d.D1in_17};
task sample(input [17:0] setup_word,input [17:0] following_word);
    begin
      @(negedge sys_clk); next_clock=0; next_bus=setup_word;
      repeat(5) @(negedge sys_clk);
      next_clock=1; next_bus=following_word;
      repeat(3) @(negedge sys_clk);
      if(captured!==setup_word)
        $fatal(1,"D1 capture=%h expected setup=%h (following=%h)",captured,setup_word,following_word);
      next_clock=0;
      repeat(3) @(negedge sys_clk);
    end
  endtask
  initial begin
    force d.clk1_p_Ab=capture_clk;
    force d.clk1_p_Ac=capture_clk;
    force d.preDin_00=bus_word[17];
    force d.preDin_01=bus_word[16];
    force d.preDin_02=bus_word[15];
    force d.preDin_03=bus_word[14];
    force d.preDin_04=bus_word[13];
    force d.preDin_05=bus_word[12];
    force d.preDin_06=bus_word[11];
    force d.preDin_07=bus_word[10];
    force d.preDin_08=bus_word[9];
    force d.preDin_09=bus_word[8];
    force d.preDin_10=bus_word[7];
    force d.preDin_11=bus_word[6];
    force d.preDin_12=bus_word[5];
    force d.preDin_13=bus_word[4];
    force d.preDin_14=bus_word[3];
    force d.preDin_15=bus_word[2];
    force d.preDin_16=bus_word[1];
    force d.preDin_17=bus_word[0];
    sample(18'h14802,18'h14c0b);
    sample(18'h12345,18'h2abcd);
    sample(18'h00000,18'h3ffff);
    sample(18'h3ffff,18'h00000);
    $display("tb_cache_input_capture: PASS -- D1 keeps all 16 setup data bits and both parity bits");
    $finish;
  end
endmodule
`default_nettype wire
