// F's byte/parity and nibble slices must capture the same setup word before
// clk1 advances the cache output. First Alto word: 00e5 (jump to boot code).
`timescale 1ns/1ps
module tb_ifu_f_capture;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg capture_clk=0, next_clock=0;
  reg [15:0] bus_word=0, next_bus=0;
  always @(posedge sys_clk) begin capture_clk<=next_clock; bus_word<=next_bus; end
  MemD_m_Rev_m_Ca d (.sys_clk(sys_clk),.CLK_md_p_(1'b0));
  wire [17:0] captured={d.F_00,d.F_01,d.F_02,d.F_03,d.F_04,d.F_05,d.F_06,d.F_07,
    d.F_08,d.F_09,d.F_10,d.F_11,d.F_12,d.F_13,d.F_14,d.F_15,d.F_16,d.F_17};
  task sample(input [15:0] before_word,input [15:0] next_word);
    reg [17:0] expected;
    begin
      @(negedge sys_clk); next_clock=0; next_bus=before_word;
      repeat(5) @(negedge sys_clk);
      expected={before_word,~^before_word[15:8],~^before_word[7:0]};
      @(negedge sys_clk); next_clock=1; next_bus=next_word;
      repeat(3) @(negedge sys_clk);
      if(captured!==expected)
        $fatal(1,"F captured %h instead of setup word/parity %h (next cache word %h)",captured,expected,next_word);
      next_clock=0; repeat(3) @(negedge sys_clk);
    end
  endtask
  initial begin
    force d.Fclk_p_a=capture_clk; force d.Fclk_p_b=capture_clk;
    force d.clk1_p_Ca=capture_clk; force d.clk1_p_Cb=capture_clk;
    force d.F_u_D_p_=0;
    force d.D_00=bus_word[15]; force d.D_01=bus_word[14];
    force d.D_02=bus_word[13]; force d.D_03=bus_word[12];
    force d.D_04=bus_word[11]; force d.D_05=bus_word[10];
    force d.D_06=bus_word[9];  force d.D_07=bus_word[8];
    force d.D_08=bus_word[7];  force d.D_09=bus_word[6];
    force d.D_10=bus_word[5];  force d.D_11=bus_word[4];
    force d.D_12=bus_word[3];  force d.D_13=bus_word[2];
    force d.D_14=bus_word[1];  force d.D_15=bus_word[0];
    force d.D_16=~^bus_word[15:8]; force d.D_17=~^bus_word[7:0];
    sample(16'h00e5,16'h0000);
    sample(16'h8400,16'h2848);
    sample(16'h1234,16'ha55a);
    sample(16'h8001,16'h7ffe);
    $display("tb_ifu_f_capture: PASS -- all 18 F bits retain the cache word's setup value");
    $finish;
  end
endmodule
