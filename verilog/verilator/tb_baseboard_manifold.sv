// Exercise the real BaseBoard's wired open-collector serial-data selector.
// Stimulus replaces only the three signals entering j17, not its outputs.
`default_nettype none
module tb_baseboard_manifold;
  reg sys_clk=0;
  always #1 sys_clk=~sys_clk;
  BaseBd_m_Rev_m_Am bb(.sys_clk(sys_clk));
  reg shift_clock=0,shift_data=0,read_data=0;
  integer checked=0;
  reg [11:0] serial_address=12'ha35;
  wire [11:0] dmd={bb.DMD_00,bb.DMD_01,bb.DMD_02,bb.DMD_03,
                   bb.DMD_04,bb.DMD_05,bb.DMD_06,bb.DMD_07,
                   bb.DMD_08,bb.DMD_09,bb.DMD_10,bb.DMD_11};
  initial begin
    force bb.TDMuxClk=shift_clock;
    force bb.Basebd05_sil_pl_8=shift_data;
    force bb.CPDMuxData=read_data;
    // All eight combinations include both simultaneously idle NAND outputs
    // and either one actively pulling down. The old OR fails on c=0,r=1.
    for(integer c=0;c<2;c=c+1)
      for(integer w=0;w<2;w=w+1)
        for(integer r=0;r<2;r=r+1)begin
          shift_clock=c[0];shift_data=w[0];read_data=r[0];
          #8;
          if(bb.CPDMuxData_p_ !== ~(c[0] ? w[0] : r[0]))
            $fatal(1,"BaseBoard j17 wired selector failed clock=%0d write=%0d read=%0d got=%b",c,w,r,bb.CPDMuxData_p_);
          if(bb.DMuxData__drv !== (c[0] ? w[0] : r[0]))
            $fatal(1,"BaseBoard l24 translated serial data has wrong polarity");
          checked=checked+1;
        end
    force bb.EclUp=1'b0;
    read_data=0;shift_clock=0;
    #8;
    for(integer b=11;b>=0;b=b-1)begin
      shift_data=serial_address[b];
      #8;shift_clock=1;
      #8;shift_clock=0;
      #8;
    end
    if(dmd !== serial_address)
      $fatal(1,"BaseBoard manifold shift got %03h expected %03h",dmd,serial_address);
    $display("BASEBOARD-MANIFOLD PASS: all %0d shift/read data combinations through j17/l24; 12-bit address %03h captured by l21/l22",checked,dmd);
    $finish;
  end
endmodule
`default_nettype wire
