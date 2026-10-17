// Exercise the original memory-board diagnostic address shift chains at
// the shared-bus turnaround used by the BaseBoard's muffler protocol.
`default_nettype none
module tb_memory_dmux;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  reg dclk=0,serial=0;
  wire rc,rd,rx;
  wire bus_data=(dclk & serial)|rc|rd|rx;
  MemC_m_Rev_m_Be #(.SYSPER(4)) mc(.sys_clk(sys_clk),.DMuxClk(dclk),
    .DMuxData(bus_data),.DMuxData__drv(rc));
  MemD_m_Rev_m_Ca #(.SYSPER(4)) md(.sys_clk(sys_clk),.DMuxClk(dclk),
    .DMuxData(bus_data),.DMuxData__drv(rd));
  MemX_m_Rev_m_Ch #(.SYSPER(4)) mx(.sys_clk(sys_clk),.DMuxClk(dclk),
    .DMuxData(bus_data),.DMuxData__drv(rx));
  // DMadr.01 on MemC/MemX is ORed with address-decode inhibits;
  // inspect the shift-register output itself, before that decode.
  wire [10:0] ac={mc.u_k24.q[1],mc.DMadr_02,mc.DMadr_03,mc.DMadr_04,mc.DMadr_05,
    mc.DMadr_06,mc.DMadr_07,mc.DMadr_08,mc.DMadr_09,mc.DMadr_10,mc.DMadr_11};
  wire [10:0] ad={md.u_k21.q[1],md.DMadr_02,md.DMadr_03,md.DMadr_04,md.DMadr_05,
    md.DMadr_06,md.DMadr_07,md.DMadr_08,md.DMadr_09,md.DMadr_10,md.DMadr_11};
  wire [10:0] ax={mx.u_l21.q[1],mx.DMadr_02,mx.DMadr_03,mx.DMadr_04,mx.DMadr_05,
    mx.DMadr_06,mx.DMadr_07,mx.DMadr_08,mx.DMadr_09,mx.DMadr_10,mx.DMadr_11};
  reg [10:0] expected=0;
  integer strobes=0;
  task shift(input [10:0] word);
    begin
      // Twelve strobes, MSB first, as SetMufflerAddress sends them.
      for(integer b=11;b>=0;b--)begin
        @(negedge sys_clk);serial=(b==11)?0:word[b];dclk=1;
        repeat(3+word%3)@(negedge sys_clk);
        dclk=0;
        repeat(3+(word/3)%3)@(negedge sys_clk);
        expected={expected[9:0],serial};strobes++;
        if(ac!==expected || ad!==expected || ax!==expected)
          $fatal(1,"DMux strobe%0d expected=%h MemC=%h MemD=%h MemX=%h",strobes,expected,ac,ad,ax);
      end
    end
  endtask
  initial begin
    repeat(20)@(negedge sys_clk);
    for(integer a=0;a<2048;a++)shift(a[10:0]);
    $display("MEMORY DMUX PASS: all 2048 addresses, %0d shared-bus turnaround strobes on MemC/MemD/MemX",strobes);
    $finish;
  end
endmodule
`default_nettype wire
