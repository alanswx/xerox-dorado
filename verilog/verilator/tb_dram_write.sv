`default_nettype none

// Distinct columns must survive consecutive early-write DRAM cycles. The
// address latch updates on CAS falling; the synchronous array must not also
// write through the previous column on that same fabric clock.
module tb_dram_write;
  reg sys_clk = 0;
  always #1 sys_clk = ~sys_clk;
  reg ras = 1, cas = 1, we = 1, data_in = 0;
  reg [6:0] a = 0;
  wire [13:0] addr;
  wire data_out, map_data_out;
  cell_MK4096P_addr address_latch (
    .sys_clk(sys_clk), .p4(ras), .p15(cas), .a(a), .addr(addr)
  );
  cell_MK4096P_6 dram (
    .sys_clk(sys_clk), .p2(data_in), .p3(we), .p4(ras), .p15(cas),
    .p5(a[1]), .p6(a[3]), .p7(a[2]), .p10(a[6]), .p11(a[5]),
    .p12(a[4]), .p13(a[0]), .addr_i(addr), .p14(data_out)
  );
  cell_MosRam map_dram (
    .sys_clk(sys_clk), .p2(data_in), .p3(we), .p4(ras), .p15(cas),
    .p5(a[1]), .p6(a[3]), .p7(a[2]), .p10(a[6]), .p11(a[5]),
    .p12(a[4]), .p13(a[0]), .p14(map_data_out),
    .p1(1'b0), .p8(1'b0), .p9(1'b0), .p16(1'b0)
  );
  task step;
    repeat (4) @(negedge sys_clk);
  endtask
  task write_bit(input [6:0] row, input [6:0] col, input value);
    begin
      we = 0; data_in = value; a = row; ras = 0; step();
      a = col; cas = 0; step();
      cas = 1; step(); ras = 1; we = 1; step();
    end
  endtask
  task read_bit(input [6:0] row, input [6:0] col, input expected);
    begin
      a = row; ras = 0; step(); a = col; cas = 0; step();
      if (data_out !== expected)
        $fatal(1, "DRAM row=%h col=%h read=%b expected=%b", row, col, data_out, expected);
      if (map_data_out !== expected)
        $fatal(1, "map DRAM row=%h col=%h read=%b expected=%b", row, col, map_data_out, expected);
      cas = 1; step(); ras = 1; step();
    end
  endtask
  // Mostek's data-in latch samples on the later falling edge of CAS' or
  // WRITE'. Changes after the hold interval must not rewrite the cell.
  task write_latched(input [6:0] col, input late_write);
    begin
      data_in = 1; we = late_write; a = 7'h25; ras = 0; step();
      a = col; cas = 0; step();
      if (late_write) begin we = 0; step(); end
      data_in = 0; step();
      cas = 1; step(); ras = 1; we = 1; step();
    end
  endtask
  initial begin
    step();
    write_bit(7'h23, 7'h15, 1);
    write_bit(7'h23, 7'h16, 0);
    write_bit(7'h23, 7'h17, 1);
    write_bit(7'h24, 7'h15, 0);
    read_bit(7'h23, 7'h15, 1);
    read_bit(7'h23, 7'h16, 0);
    read_bit(7'h23, 7'h17, 1);
    read_bit(7'h24, 7'h15, 0);
    write_latched(7'h31, 0);
    write_latched(7'h32, 1);
    read_bit(7'h25, 7'h31, 1);
    read_bit(7'h25, 7'h32, 1);
    // Page-mode writes keep the row open while CAS' selects new columns.
    a = 7'h26; ras = 0; we = 0; step();
    a = 7'h41; data_in = 1; cas = 0; step(); cas = 1; step();
    a = 7'h42; data_in = 0; cas = 0; step(); cas = 1; step();
    a = 7'h43; data_in = 1; cas = 0; step(); cas = 1; step();
    ras = 1; we = 1; step();
    read_bit(7'h26, 7'h41, 1);
    read_bit(7'h26, 7'h42, 0);
    read_bit(7'h26, 7'h43, 1);
    $display("tb_dram_write: PASS -- distinct addresses, early/late data latching, page mode");
    $finish;
  end
endmodule
`default_nettype wire
