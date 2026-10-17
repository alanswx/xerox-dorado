// Connectivity gate for the generated native wrapper, including BaseBd's
// observer of the transmit cable. This is not a CPU or Ethernet protocol test.
`default_nettype none
module tb_native_cable;
  reg host_clk=0;
  always #1 host_clk=~host_clk;
  reg cable=0;
  wire external_tx_n;
  // The paused machine remains in reset; only the physical cable boundary
  // is stimulated. No guest memory, instructions or completion state changes.
  dorado_native_machine dut(.sys_clk(1'b0),.clk_host(host_clk),.reset(1'b1),
    .baseboard_boot_n(1'b1),.large_format(1'b1),.ethernet_rx(1'b0),
    .ethernet_tx_n(external_tx_n));
  reg [15:0] pattern=16'ha65c;
  initial begin
    force dut.m.b_DskEth.XmtData_p___drv=cable;
    for(integer i=0;i<16;i++)begin
      cable=pattern[i];
      repeat(4) @(negedge host_clk);
      if(external_tx_n !== cable || dut.m.b_BaseBd.XmtData_p_ !== cable)
        $fatal(1,"native TX cable disconnected: drive=%b external=%b BaseBd=%b",
          cable,external_tx_n,dut.m.b_BaseBd.XmtData_p_);
    end
    $display("NATIVE CABLE PASS: DskEth transmit reaches external cable and BaseBoard observer for all 16 samples");
    $finish;
  end
endmodule
`default_nettype wire
