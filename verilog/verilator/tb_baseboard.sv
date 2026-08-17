// tb_baseboard -- does the BaseBoard's 6502 fetch from its own EPROMs?
//
// The BaseBoard is the one board where every piece is now real: Andrew
// Holme's netlist-derived 6502, MiSTer's 6532 RIOTs, the SN74LS138 that
// decodes `Rom0'`..`Rom7'`, and eight 2716 sockets holding the base ROM the
// C emulator boots. So it can be asked the only question that matters of a
// processor board: does the address bus walk, and does it walk somewhere
// sensible?
//
// The sharpest check is the RESET VECTOR. A 6502 comes out of reset by
// reading 0xFFFC and 0xFFFD, and in this ROM those hold 0xF3A7. Seeing the
// address bus present 0xFFFC would mean the core, the decoder, the socket
// mapping and the image are all right at once -- four things this project
// derived separately.
//
// Everything except the fabric clock is left unconnected, which the harness
// treats as low; see dorado_machine for why that is the honest default.

`default_nettype none

module tb_baseboard;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  BaseBd_m_Rev_m_Am u (.sys_clk(sys_clk));

  // The 6502's address and data buses, by hierarchical reference.
  wire [15:0] mca = {u.MCA_15, u.MCA_14, u.MCA_13, u.MCA_12,
                     u.MCA_11, u.MCA_10, u.MCA_09, u.MCA_08,
                     u.MCA_07, u.MCA_06, u.MCA_05, u.MCA_04,
                     u.MCA_03, u.MCA_02, u.MCA_01, u.MCA_00};

  integer i, changes, seen_rom, seen_vector;
  reg [15:0] prev;

  initial begin
    changes = 0; seen_rom = 0; seen_vector = 0;
    prev = 16'hXXXX;
    for (i = 0; i < 200000; i = i + 1) begin
      @(posedge sys_clk);
      if (mca !== prev) begin
        changes = changes + 1;
        prev = mca;
        if (mca >= 16'hC000) seen_rom = seen_rom + 1;
        if (mca == 16'hFFFC || mca == 16'hFFFD) seen_vector = seen_vector + 1;
      end
    end

    $display("tb_baseboard: address bus changed %0d times, %0d into ROM (0xC000+)", changes, seen_rom);
    if (seen_vector)
      $display("tb_baseboard: the 6502 addressed its RESET VECTOR 0xFFFC/D %0d times", seen_vector);
    else
      $display("tb_baseboard: the reset vector 0xFFFC/D was never addressed");
    $finish;
  end

endmodule

`default_nettype wire
