// Replay the measured first Cedar MapBuf write/hold boundary on MemX.
`default_nettype none
module tb_map_buffer_hold;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg [2:0] phase=0;
  always @(posedge sys_clk)phase<=phase+1;
  wire board_clock=phase[1];
  wire mem_sh=phase[2];
  reg hold_request=0;
  reg [17:0] b={16'h0004,2'b10};
  MemX_m_Rev_m_Ch #(.SYSPER(4)) m(.sys_clk(sys_clk),.CLK_mx_p_(board_clock),.MemSH(mem_sh),
    .MemClkEnable_p_c(1'b0),.ClkEnable_p_a(1'b0),.HoldMapBuf(hold_request),
    .BMux_00(b[17]),
    .BMux_01(b[16]),
    .BMux_02(b[15]),
    .BMux_03(b[14]),
    .BMux_04(b[13]),
    .BMux_05(b[12]),
    .BMux_06(b[11]),
    .BMux_07(b[10]),
    .BMux_08(b[9]),
    .BMux_09(b[8]),
    .BMux_10(b[7]),
    .BMux_11(b[6]),
    .BMux_12(b[5]),
    .BMux_13(b[4]),
    .BMux_14(b[3]),
    .BMux_15(b[2]),
    .BMux_16(b[1]),
    .BMux_17(b[0]));
  wire [17:0] captured={m.Mapbuf_00,m.Mapbuf_01,m.Mapbuf_02,m.Mapbuf_03,m.Mapbuf_04,m.Mapbuf_05,m.Mapbuf_06,m.Mapbuf_07,m.Mapbuf_08,m.Mapbuf_09,m.Mapbuf_10,m.Mapbuf_11,m.Mapbuf_12,m.Mapbuf_13,m.Mapbuf_14,m.Mapbuf_15,m.Mapbuf_16,m.Mapbuf_17};
  initial begin #10000; $fatal(1,"Replay phase timeout"); end
  task map_write(input [15:0] page,input [15:0] saved_link);
    begin
      @(negedge m.preFH_p_A);
      @(negedge sys_clk); // phase0 of the measured Map instruction
      repeat(5)@(negedge sys_clk);b={page,2'b11};
      repeat(3)@(negedge sys_clk);hold_request=1;
      repeat(5)@(negedge sys_clk);b={saved_link,2'b01};
      repeat(20)@(negedge sys_clk);
      $display("MAP HOLD captured=%h expected=%h",captured,{page,2'b11});
      if(captured!=={page,2'b11})$fatal(1,"Saved Link overwrote the map page after hold request");
      hold_request=0;
      repeat(24)@(negedge sys_clk);
    end
  endtask
  initial begin
    repeat(100)@(negedge sys_clk);
    map_write(16'h0123,16'h0e35);
    map_write(16'hfedc,16'h0e54);
    $display("MAP HOLD PASS: both pages survive following saved Link, buffer reopens after release");
    $finish;
  end
endmodule
`default_nettype wire
