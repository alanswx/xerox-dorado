// MemD c14 regenerates low-byte odd parity when filling cache from storage.
// Check the real board connections with bytes from the Alto boot sector.
`timescale 1ns/1ps
module tb_memory_byte_parity;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg [7:0] storage_byte=0;
  MemD_m_Rev_m_Ca d (.sys_clk(sys_clk),.CLK_md_p_(1'b0));
  task check(input [7:0] b,input expected);
    storage_byte=b;
    repeat(2) @(negedge sys_clk);
    if(d.SinD_17!==expected || d.SinD_17_p_!==!expected)
      $fatal(1,"storage byte %h needs odd parity %b, got %b (complement %b)",
        b,expected,d.SinD_17,d.SinD_17_p_);
  endtask
  initial begin
    force d.SinD_08=storage_byte[7]; force d.SinD_09=storage_byte[6];
    force d.SinD_10=storage_byte[5]; force d.SinD_11=storage_byte[4];
    force d.SinD_12=storage_byte[3]; force d.SinD_13=storage_byte[2];
    force d.SinD_14=storage_byte[1]; force d.SinD_15=storage_byte[0];
    check(8'h30,1); // first Alto instruction 8530: two low-byte bits
    check(8'h85,0); // three bits
    check(8'h00,1);
    check(8'hff,1);
    check(8'h01,0);
    $display("tb_memory_byte_parity: PASS -- storage refill regenerates odd byte parity");
    $finish;
  end
endmodule
