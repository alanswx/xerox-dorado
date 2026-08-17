// tb_mir -- the MICROINSTRUCTION LOAD PATH, all 36 bits, RTL against the C
// emulator.
//
// This is the deepest cross-check in the project so far, and the two sides
// share no ancestry at all:
//
//   * the RTL comes from PARC's 1979 wire list. Six MC10172 dual decoders, at
//     ContA g01/h01/i01 and ContB h04/i03/i04, are wired as 1-to-4
//     DEMULTIPLEXERS -- the data bit enters on each half's ENABLE pin and the
//     two select lines route it to one of four outputs -- so nine CPOut lines
//     times four function codes fan out to the thirty-six field lines
//     `sRSTK.0-3`, `sALUF.0-3`, `sBSEL.0-2`, `sLC.0-2`, `sASEL.0-2`,
//     `sBLOCK`, `sFF.0-7`, `sJCN.0-7`, `sIMLH` and `sIMRH`.
//   * `dorado_decode_mir()` in dorado/src/disasm.c was written from the
//     Hardware Manual's microinstruction format, byte by byte, years later and
//     from a document rather than a netlist.
//
// Every one of the 36 bits has to land in the same place. dorado/tests/
// cpreg_vectors.c strobes ONE microinstruction byte at a time from a cleared
// MIR, so the fields the C decoder reports are exactly the bits that strobe
// sets, and this drives the same strobes through the gates and reads the field
// lines back.
//
// WHY THE FIELD LINES ARE SAMPLED DURING THE STROBE. They are the SET inputs
// of the microinstruction register, not its outputs: a decoder output is a
// level that stands while the strobe is asserted and the selects hold. So the
// strobe is taken low, the lines are read, and only then is it released.
//
// The one convention this assumes is PARC's, and it is the same one that holds
// everywhere else in the archive: `.0` is the MOST significant bit of a field.
// `sRSTK.0` is the microinstruction's RSTK bit 3, the one the Hardware Manual
// puts in iw2 rather than iw0.
//
// Both Control boards are instantiated because the fields are split between
// them -- ContB carries RSTK, ALUF, BSEL, LC and ASEL, ContA carries BLOCK, FF
// and JCN -- and they are driven from the one bus, exactly as the backplane
// does it.
//
// Run with +vectors=<file>; the make target does that.

`default_nettype none

module tb_mir;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg [2:0] ckdiv = 3'd0;
  always @(posedge sys_clk) ckdiv <= ckdiv + 3'd1;
  wire clk_c_n = ckdiv[2];

  reg [2:0] addr_n;
  reg [8:0] cpout;
  reg       strb_n;

  ContA_m_Rev_m_Cd ua (
      .sys_clk(sys_clk), .CLK_ca_p_(clk_c_n),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n)
  );

  ContB_m_Rev_m_Cd ub (
      .sys_clk(sys_clk), .CLK_cb_p_(clk_c_n),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n)
  );

  // The fields, most significant bit first as PARC numbers them.
  wire [3:0] rstk  = {ub.sRSTK_0, ub.sRSTK_1, ub.sRSTK_2, ub.sRSTK_3};
  wire [3:0] aluf  = {ub.sALUF_0, ub.sALUF_1, ub.sALUF_2, ub.sALUF_3};
  wire [2:0] bsel  = {ub.sBSEL_0, ub.sBSEL_1, ub.sBSEL_2};
  wire [2:0] lc    = {ub.sLC_0,   ub.sLC_1,   ub.sLC_2};
  wire [2:0] asel  = {ub.sASEL_0, ub.sASEL_1, ub.sASEL_2};
  wire       block = ua.sBLOCK;
  wire [7:0] ff    = {ua.sFF_0, ua.sFF_1, ua.sFF_2, ua.sFF_3,
                      ua.sFF_4, ua.sFF_5, ua.sFF_6, ua.sFF_7};
  wire [7:0] jcn   = {ua.sJCN_0, ua.sJCN_1, ua.sJCN_2, ua.sJCN_3,
                      ua.sJCN_4, ua.sJCN_5, ua.sJCN_6, ua.sJCN_7};

  integer fd, code, cases, errors, shown;
  integer fn, extra;
  reg [7:0] data;
  reg [3:0] w_rstk, w_aluf;
  reg [2:0] w_bsel, w_lc, w_asel;
  integer   w_block;
  reg [7:0] w_ff, w_jcn;
  string    path, line, tag;

  task fail(input string what, input [7:0] got, input [7:0] want);
    begin
      errors = errors + 1;
      if (shown < 12) begin
        shown = shown + 1;
        $display("  FAIL fn=%0d data=%02h extra=%0d : %s RTL %02h, emulator %02h",
                 fn, data, extra, what, got, want);
      end
    end
  endtask

  initial begin
    addr_n = 3'b111; cpout = 9'd0; strb_n = 1'b1;
    cases = 0; errors = 0; shown = 0;
    repeat (40) @(posedge sys_clk);

    if (!$value$plusargs("vectors=%s", path)) path = "cpreg.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $fatal(1, "tb_mir: cannot open %s", path);

    while (!$feof(fd)) begin
      void'($fgets(line, fd));
      code = $sscanf(line, "%s %d %h %d %h %h %h %h %h %d %h %h",
                     tag, fn, data, extra, w_rstk, w_aluf, w_bsel, w_lc,
                     w_asel, w_block, w_ff, w_jcn);
      if (code != 12 || tag != "MIR") continue;

      cases = cases + 1;
      addr_n = ~fn[2:0];
      cpout  = {data, extra[0]};        // CPOut.0 is data[7]; CPOut.8 last
      strb_n = 1'b1;
      repeat (6) @(posedge sys_clk);
      strb_n = 1'b0;                    // assert -- the field lines stand now
      repeat (6) @(posedge sys_clk);

      if (rstk  !== w_rstk)  fail("RSTK",  {4'd0, rstk},  {4'd0, w_rstk});
      if (aluf  !== w_aluf)  fail("ALUF",  {4'd0, aluf},  {4'd0, w_aluf});
      if (bsel  !== w_bsel)  fail("BSEL",  {5'd0, bsel},  {5'd0, w_bsel});
      if (lc    !== w_lc)    fail("LC",    {5'd0, lc},    {5'd0, w_lc});
      if (asel  !== w_asel)  fail("ASEL",  {5'd0, asel},  {5'd0, w_asel});
      if (block !== w_block[0]) fail("BLOCK", {7'd0, block}, {7'd0, w_block[0]});
      if (ff    !== w_ff)    fail("FF",    ff,            w_ff);
      if (jcn   !== w_jcn)   fail("JCN",   jcn,           w_jcn);

      strb_n = 1'b1;
      repeat (6) @(posedge sys_clk);
    end
    $fclose(fd);

    $display("tb_mir: %0d microinstruction strobes from the C emulator, %0d field mismatches",
             cases, errors);
    if (cases == 0) $fatal(1, "no MIR vectors were read");
    if (errors != 0)
      $fatal(1, "the RTL microinstruction demultiplexers and the C emulator disagree");
    $finish;
  end

endmodule

`default_nettype wire
