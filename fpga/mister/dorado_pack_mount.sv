// hps_io updates img_size in several SPI words BEFORE pulsing img_mounted.
// A nonzero partial size must not start a medium read before that pulse.
module dorado_pack_mount (
    input wire clk, reset, size_nonzero, mounted,
    output wire attached
);
  reg seen = 0;
  always @(posedge clk)
    if(reset || !size_nonzero)seen<=0;
    else if(mounted)seen<=1;
  assign attached=seen && size_nonzero;
endmodule
