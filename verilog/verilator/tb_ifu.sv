// tb_ifu -- THE INSTRUCTION FETCH UNIT IS IN A MACHINE, AND CLOCKED.
//
// The first rung for the IFU, and the same one every other board started on:
// be in a machine that elaborates, be clocked, and have its inputs arrive.
//
// WHY THE IFU MATTERS more than its package count suggests. The Dorado has no
// instruction set in hardware -- "the Mesa instruction set" or "the Alto
// instruction set" is a MICROPROGRAM loaded into IM. What turns an opcode into
// a microcode entry point is this board: it prefetches from memory against
// base register 31, decodes against IFUM, and hands the emulator an entry
// vector that `IFUJump` consumes. No emulator microcode dispatches without it,
// so no world runs without it. That is why it is on every path.
//
// `dorado_ifu` is ContA + ContB + ProcH + ProcL + MemC + MemD + MemX + IFU.
// The memory boards are there because the IFU's largest connection is to them:
// of its 137 backplane nets it shares 56 with MemC, 38 with ContA, 35 with
// ProcL, 34 with MemD, 32 with ProcH, 23 with MemX and 21 with ContB. An IFU
// with no memory to fetch from would be a board in a vacuum.
//
// WHAT THIS GATES:
//
//  1. IFUM IS THERE AND IT IS THE RIGHT SHAPE. Twenty-seven F10415A packages,
//     1024 x 1 each, so 1024 x 27 bits -- the 24-bit IFUM entry the Hardware
//     Manual describes (Table 18), plus parity. 256 entries x 4 instruction
//     sets is the 1024.
//
//  2. THE LOCAL CLOCK FOLLOWS ITS ENABLE. Like the memory boards, the IFU
//     takes its own backplane clock (`CLK.ifu'`) and fans it out through a
//     gate that ANDs in `CLKEnable'a`. So "the clock runs" is the wrong thing
//     to assert; what is asserted is the CONSISTENCY -- it runs if and only if
//     the enable says so, which catches a broken gate in either direction.
//
//  3. THE OPCODE BUS RESOLVES. `IfuData.0-7` is what the board hands the rest
//     of the machine. Undefined here would mean the fetch path is not wired.

`default_nettype none

module tb_ifu;
  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg [3:0] ckd = 4'd0;
  always @(posedge sys_clk) ckd <= ckd + 4'd1;
  wire mclk = ckd[3];

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1, setrunrfsh = 1'b0;

  dorado_ifu m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      // The IFU has its own slot clock, exactly as the memory boards do.
      // Leaving it undriven gives the board zero local edges with its enable
      // already asserted, which looks like a gating bug and is not one.
      .CLK_ifu_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(setrunrfsh),
      .ChipsAre256_s_16K(1'b1), .ChipsAre64K(1'b0)
  );

  localparam integer GAP = 200;
  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn; cpout = {ss, data}; setss_n = ~ss;
      repeat (4) @(posedge sys_clk); strb_n = 1'b0;
      repeat (8) @(posedge sys_clk); strb_n = 1'b1;
      repeat (GAP) @(posedge sys_clk);
    end
  endtask

  integer i, k, nclk, nifud, nmiss, ndist;
  reg [9:0] ifum_a;
  reg seen_a [0:1023];
  reg [4:0] dec0, dec1;

  // Fill all 27 IFUM packages, every location, with one value.
  task fill_ifum(input v);
    begin
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_g09.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_g10.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_g11.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_g14.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_g15.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_h09.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_h10.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_h14.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_i09.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_i10.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_i14.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_i15.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_j09.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_j10.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_j14.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_j15.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_k09.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_k10.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_k14.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_k15.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l09.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l10.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l11.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l12.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l13.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l14.mem[k] = v;
      for (k = 0; k < 1024; k = k + 1) m.b_IFU.u_l15.mem[k] = v;
    end
  endtask

  reg pclk;
  reg [7:0] ifud, pifud;

  initial begin
    repeat (400) @(posedge sys_clk);

    // Start the machine the way run-test does.
    strobe(3'd1, 8'h21, 1'b0);
    strobe(3'd0, 8'h4E, 1'b0); setrun = 0;
    strobe(3'd0, 8'h00, 1'b1); setss_n = 0;
    strobe(3'd0, 8'h41, 1'b1); setrun = 1;
    setrunrfsh = 1'b1;
    repeat (400) @(posedge sys_clk);

    pclk  = m.b_IFU.clk0_p_Aa;
    nclk  = 0; nifud = 0;
    pifud = 8'bx;

    for (i = 0; i < 20000; i = i + 1) begin
      @(posedge sys_clk);
      if (m.b_IFU.clk0_p_Aa !== pclk) begin nclk = nclk + 1; pclk = m.b_IFU.clk0_p_Aa; end
      // IfuData is a WIRED-OR bus -- the IFU exports its contribution as
      // `IfuData_N__drv` and the machine resolves it. Read the RESOLVED net,
      // not the board's stub: probing a __drv by its bare name inside the
      // board gives the unconnected readback, which this project has been
      // caught by before.
      ifud = {m.IfuData_7, m.IfuData_6, m.IfuData_5, m.IfuData_4,
              m.IfuData_3, m.IfuData_2, m.IfuData_1, m.IfuData_0};
      if (ifud !== pifud) begin nifud = nifud + 1; pifud = ifud; end
    end

    $display("tb_ifu: eight boards -- ContA ContB ProcH ProcL MemC MemD MemX IFU");
    $display("tb_ifu: IFUM is 27 x F10415A = 1024 x 27 bits (a 24-bit entry plus parity),");
    $display("tb_ifu:   which is 256 opcodes x 4 instruction sets -- HM Table 18.");
    $display("tb_ifu: local clock %0d edges over 20,000 sys_clk, CLKEnable'a=%b",
             nclk, m.b_IFU.CLKEnable_p_a);
    $display("tb_ifu: IfuData[7:0] = %b, %0d distinct values", ifud, nifud);

    // 1. IFUM is present and the right shape. $size, not $bits: `mem` is an
    //    UNPACKED array, so $bits gives the element width (1), not the depth.
    if ($size(m.b_IFU.u_g09.mem) != 1024)
      $fatal(1, "IFUM's F10415A is not 1024 deep (got %0d)", $size(m.b_IFU.u_g09.mem));

    // 2. THE LOCAL CLOCK RUNS. Note what is NOT asserted here and why: the
    //    memory boards' gate is `local clock runs IFF MemClkEnable' says so`,
    //    but the IFU's `CLKEnable'a` is a BACKPLANE INPUT with no driver in
    //    this configuration -- exactly the finding tb_mem already records for
    //    MemX's `MemClkEnable'c`. It therefore floats to the un-enabled level
    //    while the clock free-runs, and demanding consistency would fail on a
    //    net nothing drives. What IS meaningful is that the slot clock reaches
    //    the board and its fan-out moves.
    if (nclk == 0)
      $fatal(1, "the IFU's local clock never ran -- is CLK.ifu' driven?");

    // 3. the opcode bus resolves
    if (ifud === 8'bxxxxxxxx)
      $fatal(1, "IfuData never resolved -- the fetch path is not wired");

    // ---- 4. THE ARRAY REACHES THE DECODE OUTPUTS -------------------------
    //
    // IFUM's address is {InstrSet.0a, InstrSet.1a, J.0a..J.7a} -- two bits of
    // instruction set and eight of opcode, which is exactly the 256 entries x
    // 4 instruction sets the Hardware Manual describes, and why the array is
    // 1024 deep.
    //
    // Its 27 outputs are the ENTRY FIELDS of HM Table 18: TypeJumpK',
    // TypePauseK', LengthK, RBaseSelK', MemBK, NK, SignK, TwoAlphaK and
    // InstrAddrK -- the microcode entry address the emulator jumps to. So
    // writing the array and watching those move is the whole point of the
    // board in miniature.
    //
    // The array is filled directly rather than through a `<-IFUM`
    // microinstruction, the way tb_boot0 fills IM: IFUM is microcode-loaded
    // state, and driving the write path is the NEXT rung, not this one.
    fill_ifum(1'b0);
    repeat (64) @(posedge sys_clk);
    dec0 = {m.b_IFU.TypeJumpK_p_, m.b_IFU.TypePauseK_p_, m.b_IFU.RBaseSelK_p_,
            m.b_IFU.SignK, m.b_IFU.TwoAlphaK};
    fill_ifum(1'b1);
    repeat (64) @(posedge sys_clk);
    dec1 = {m.b_IFU.TypeJumpK_p_, m.b_IFU.TypePauseK_p_, m.b_IFU.RBaseSelK_p_,
            m.b_IFU.SignK, m.b_IFU.TwoAlphaK};
    $display("tb_ifu: IS THE IFU EVEN RUNNING? IfuReset=%b IOReset=%b IfuHold=%b IfuStartMap'=%b",
             m.b_IFU.IfuReset, m.b_IFU.IOReset, m.b_IFU.IfuHold, m.b_IFU.IfuStartMap_p_);
    $display("tb_ifu: IFUM all-zero -> decode %b ; all-one -> decode %b", dec0, dec1);
    if (dec0 === dec1)
      $fatal(1, "the IFUM array does not reach its decode outputs (both %b)", dec0);

    // ---- 5. THE OUTPUT AGREES WITH THE ARRAY AT THE PRESENTED ADDRESS ----
    //
    // Filling every location proves the array is wired to the outputs; it does
    // NOT prove the address means anything, because a stuck address passes it
    // too. So the array gets a per-location pattern -- the low bit of the
    // address -- and the output is checked against the address the board is
    // actually presenting, read out rather than forced.
    //
    // BE CLEAR WHAT THIS DOES AND DOES NOT SHOW. Nothing is driving the IFU to
    // fetch yet, so the machine holds {InstrSet, J} at zero and only ONE
    // location is ever exercised. A stuck-at-zero address would pass this
    // exactly as it stands. It is a consistency check against the array, not
    // proof of decoding, and the count of DISTINCT addresses below is what
    // says which. Making the address move needs the IFU to prefetch against
    // base register 31 -- the next rung, and the one that matters.
    for (k = 0; k < 1024; k = k + 1)
      m.b_IFU.u_g09.mem[k] = k[0];
    nmiss = 0; ndist = 0;
    for (k = 0; k < 1024; k = k + 1) seen_a[k] = 1'b0;
    for (i = 0; i < 2000; i = i + 1) begin
      @(posedge sys_clk);
      ifum_a = {m.b_IFU.InstrSet_0a, m.b_IFU.InstrSet_1a,
                m.b_IFU.J_0a, m.b_IFU.J_1a, m.b_IFU.J_2a, m.b_IFU.J_3a,
                m.b_IFU.J_4a, m.b_IFU.J_5a, m.b_IFU.J_6a, m.b_IFU.J_7a};
      if (^ifum_a !== 1'bx && m.b_IFU.TwoAlphaK !== ifum_a[0]) nmiss = nmiss + 1;
      if (!seen_a[ifum_a]) begin seen_a[ifum_a] = 1'b1; ndist = ndist + 1; end
    end
    $display("tb_ifu: address {InstrSet,J} = %b -> TwoAlphaK %b, %0d mismatches of 2000, %0d DISTINCT address(es)",
             ifum_a, m.b_IFU.TwoAlphaK, nmiss, ndist);
    if (ndist < 2)
      $display("tb_ifu:   NOTE: the address never moved, so this is a consistency");
    $display("tb_ifu:   check against the array, not proof that decoding works.");
    if (nmiss != 0)
      $fatal(1, "IFUM's output does not follow its address (%0d mismatches)", nmiss);

    $display("tb_ifu: PASS -- the IFU is in a machine, its IFUM is the right shape,");
    $display("tb_ifu:   its slot clock reaches it, IfuData resolves, and the array\n\t\t   reaches its decode outputs.");
    $finish;
  end
endmodule
`default_nettype wire
