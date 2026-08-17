// tb_cpreg -- the machine's BOOT INTERFACE, RTL against the C emulator.
//
// This is how a Dorado starts. The BaseBoard's 6502 has no access to IM or to
// the processor's registers; it drives a nine-bit bus (`CPOut.0-8`), a
// three-bit function code (`CPAddr.0'-2'`) and a strobe (`CPStrb'`) across the
// backplane to the Control boards, and everything else -- CPReg, the
// microinstruction register, run and single-step -- is built out of that.
//
// Two independent derivations meet here, which is the whole point of
// generating the RTL from PARC's netlists:
//
//   * the RTL: an MC10161 on ContA decodes the function code into
//     `CPStrb0'`..`CPStrb3'`, and MC10176 hex flip-flops at a03, f02, g02 and
//     h02 latch `CPReg.00`-`.15` from `CPOut.0-7`;
//   * the C emulator: `apply_mcp_strobe()` in src/baseboard.c, written from
//     PARC's `doradoio.mdefs` and `doradocpint.masm`, which says function 2 is
//     the high byte and function 3 the low one.
//
// dorado/tests/cpreg_vectors.c dumps what the C side computes -- including
// baseboard.c directly, so it runs the emulator's real decoder -- and this
// feeds the same strobes through the gates.
//
// THREE THINGS THE NETLIST SETTLES, and all three had to be right for a single
// vector to pass:
//
//   * The function code arrives COMPLEMENTED and the strobes are active low.
//     ContA's decoder takes `CPAddr.0'` on its most significant select, so
//     function 0 presents 7 and comes out on Q7', which the board calls
//     `CPStrb0'`. The naming is self-consistent once you see it.
//   * PARC numbers MSB-FIRST, here as everywhere. `CPOut.0` is the data byte's
//     bit 7: it traces back through `TCPBus.00` to `MCPBus.00`, which is a
//     RIOT's PA7. And `CPReg.00` is the high byte's most significant bit.
//   * The flip-flops latch on the TRAILING edge of the strobe, because
//     `CPStrb0'` and its siblings are active low and an MC10176 clocks on a
//     rising edge.
//
// Only functions 2 and 3 are checked, because only those are decoded on ContA;
// functions 4-7, the four microinstruction bytes, are strobed elsewhere and
// belong to a later test.
//
// Run with +vectors=<file>; the make target does that.

`default_nettype none

module tb_cpreg;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  // The board's own ECL clock, divided down from the fabric clock the way the
  // BaseBoard's divider chain does. Nothing on the CPReg path uses it -- those
  // flip-flops are clocked by the strobe -- but a board with a dead clock is
  // not a fair test of anything.
  reg [2:0] ckdiv = 3'd0;
  always @(posedge sys_clk) ckdiv <= ckdiv + 3'd1;
  wire clk_ca_n = ckdiv[2];

  reg [2:0] addr_n;      // CPAddr.0'..2', the complemented function code
  reg [8:0] cpout;       // CPOut.0..8, MSB-first
  reg       strb_n;      // CPStrb', active low

  ContA_m_Rev_m_Cd u (
      .sys_clk    (sys_clk),
      .CLK_ca_p_  (clk_ca_n),
      .CPAddr_0_p_(addr_n[2]),
      .CPAddr_1_p_(addr_n[1]),
      .CPAddr_2_p_(addr_n[0]),
      .CPOut_0    (cpout[8]),
      .CPOut_1    (cpout[7]),
      .CPOut_2    (cpout[6]),
      .CPOut_3    (cpout[5]),
      .CPOut_4    (cpout[4]),
      .CPOut_5    (cpout[3]),
      .CPOut_6    (cpout[2]),
      .CPOut_7    (cpout[1]),
      .CPOut_8    (cpout[0]),
      .CPStrb_p_  (strb_n)
  );

  // CPReg, most significant bit first, as PARC numbers it.
  wire [15:0] cpreg = {u.CPReg_00, u.CPReg_01, u.CPReg_02, u.CPReg_03,
                       u.CPReg_04, u.CPReg_05, u.CPReg_06, u.CPReg_07,
                       u.CPReg_08, u.CPReg_09, u.CPReg_10, u.CPReg_11,
                       u.CPReg_12, u.CPReg_13, u.CPReg_14, u.CPReg_15};

  // One strobe, the way the BaseBoard issues it: present the function code and
  // the data, take the strobe low, then release it.
  task do_strobe(input [2:0] fn, input [7:0] data, input extra);
    begin
      addr_n = ~fn;
      cpout  = {data, extra};          // CPOut.0 is data[7]; CPOut.8 is last
      strb_n = 1'b1;
      repeat (8) @(posedge sys_clk);
      strb_n = 1'b0;                   // assert
      repeat (8) @(posedge sys_clk);
      strb_n = 1'b1;                   // release -- the flip-flops latch here
      repeat (8) @(posedge sys_clk);
    end
  endtask

  integer fd, n, cases, errors, shown, checked;
  string     line, tag;
  reg [7:0]  mcpbusl, data;
  reg [15:0] want;
  reg [7:0]  m0, m1, m2, m3, m4;
  integer    running, ss, mirloaded;
  string     path;

  initial begin
    addr_n = 3'b111; cpout = 9'd0; strb_n = 1'b1;
    cases = 0; errors = 0; shown = 0; checked = 0;
    repeat (40) @(posedge sys_clk);

    if (!$value$plusargs("vectors=%s", path)) path = "cpreg.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $fatal(1, "tb_cpreg: cannot open %s", path);

    // The vector file carries two sections; `CP` lines are this test's.
    while (!$feof(fd)) begin
      void'($fgets(line, fd));
      n = $sscanf(line, "%s %h %h %h %h %h %h %h %h %d %d %d",
                  tag, mcpbusl, data, want, m0, m1, m2, m3, m4,
                  running, ss, mirloaded);
      if (n != 12 || tag != "CP") continue;

      cases = cases + 1;
      do_strobe(mcpbusl[6:4], data, mcpbusl[7]);

      // Only functions 2 and 3 reach CPReg on this board; the others leave it
      // alone, which the C emulator's running total also does.
      if (mcpbusl[6:4] == 3'd2 || mcpbusl[6:4] == 3'd3) begin
        checked = checked + 1;
        if (cpreg !== want) begin
          errors = errors + 1;
          if (shown < 10) begin
            shown = shown + 1;
            $display("  FAIL fn=%0d data=%02h : RTL CPReg %04h, emulator %04h",
                     mcpbusl[6:4], data, cpreg, want);
          end
        end
      end

    end
    $fclose(fd);

    $display("tb_cpreg: %0d strobes from the C emulator, %0d of them loading CPReg, %0d mismatches",
             cases, checked, errors);
    if (cases == 0)   $fatal(1, "no vectors were read");
    if (checked == 0) $fatal(1, "no vector exercised CPReg");
    if (errors != 0)  $fatal(1, "the RTL Control board and the C emulator disagree");
    $finish;
  end

endmodule

`default_nettype wire
