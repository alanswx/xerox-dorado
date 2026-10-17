`default_nettype none

// MemC23 specifies i24's B inputs wired to SB for 256-word pages. When
// that input is selected it is high, so MemC must release the shared row-address bus, so MemX can put
// the mapped real page on it. The other selection supplies the column.
module tb_memory_address_bus;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg [3:0] va=0, column=0;
  MemC_m_Rev_m_Be m(.sys_clk(sys_clk), .MemRfsh(1'b0));
  wire [3:0] address={m.MemAd_5__drv,m.MemAd_6__drv,
                      m.MemAd_7__drv,m.MemAd_8__drv};
  initial begin
    force m.VA_20=va[3]; force m.VA_21=va[2];
    force m.VA_22=va[1]; force m.VA_23=va[0];
    force m.MemC01_sil_pl_17=1;
    for(integer i=0;i<16;i=i+1) begin
      va=i[3:0]; repeat(4) @(negedge sys_clk);
      if(address!==0)
        $fatal(1,"256-word-page row bus driven by MemC: VA20..23=%h contribution=%h",va,address);
    end
    force m.MemC01_sil_pl_9=column[0];
    force m.MemC10_sil_pl_2=column[1];
    force m.MemC09_sil_pl_14=column[2];
    force m.MemC09_sil_pl_13=column[3];
    force m.MemC01_sil_pl_17=0;
    for(integer i=0;i<16;i=i+1) begin
      column=i[3:0]; repeat(4) @(negedge sys_clk);
      if(address!==~column)
        $fatal(1,"column address=%h expected=%h",address,~column);
    end
    $display("tb_memory_address_bus: PASS -- MemC releases row bus and supplies all column patterns");
    $finish;
  end
endmodule
`default_nettype wire
