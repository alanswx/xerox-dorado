// tb_boot0 -- PARC's block loader: a 17-byte HUNK walked into IM, and the
// words read back out of the array.
//
// `SendAHunk` in doradoboot.masm, on top of the `SendViaMIR` that `sendmir`
// proves. A HUNK IS 17 BYTES AND THAT NUMBER IS EXACT: one `ExtraBits` byte
// followed by eight half-microinstructions of two bytes each, and eight halves
// of seventeen bits is one hundred and thirty-six bits -- seventeen bytes.
// Four microinstructions per hunk.
//
//     SendAHunk:  ExtraBits <- Hunk[0], Y <- 1
//       repeat:   MicroHalf <- 40 (left)   ; SendAHalfMicroInstruction
//                 MicroHalf |= 1 (right)   ; SendAHalfMicroInstruction
//                 IMAddress++
//                 until Y = 17
//
// Each `SendViaMIR` takes its two data bytes HIGH FIRST, shifts one bit out of
// `ExtraBits` (MSB first), and indexes `IMStoreTable` with
// `(MicroHalf & 1) << 1 | extraBit`. Both halves of an instruction go to the
// SAME IMAddress; the address steps once per instruction, not per half.
//
// THE READBACK IS THE POINT. Counting set bits, which is all `writeim-test`
// could do, cannot tell a right answer from a permuted one. IM is 144
// F10415A packages = 36 distinct (half, data-line) slots x 4 banks, and this
// file is GENERATED from ContB's wire list so each slot is read by name:
// pin 15 is the data line (`RBMux.00`..`.15`, `RBMuxP`, and the secondary bit
// -- `MidasOrRSTK.2` on the left, `ContB04.sil+1` on the right), pin 13 is
// `WEL'`/`WER'` and so the half, pin 14 is the chip select and so the bank.
//
// ADDRESS 0x100..0x103 LAND AT INDICES 0x80, 0x80, 0x81, 0x81. The low address
// bit picks the bank, not the index -- IM is interleaved -- so consecutive
// microinstructions sit in different banks at the same offset. That is why the
// four addresses here are consecutive: it exercises the interleave rather than
// stepping past it.

`default_nettype none


module tb_boot0;

  localparam integer GAP = 200;   // sys_clk between Control strobes

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;
  reg [3:0] ckd = 4'd0;
  always @(posedge sys_clk) ckd <= ckd + 4'd1;
  wire mclk = ckd[3];

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       dmd = 1'b0, dmc = 1'b0, udmd = 1'b0;

  dorado_proc m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n)
  );

  // THE BASEBOARD'S CONTROL REGISTER, modelled here because this machine has no
  // BaseBoard. `SetRun` and `SetSS'` are BACKPLANE wires, not CP-bus lines --
  // BaseBd f02 (an MC10124 TTL-to-ECL translator) drives them, and what it
  // translates is latched by g07, an SN74LS175 clocked by `TControlStrb'`:
  //
  //     g07 .p9  = TControlStrb'   (clock -- so it latches on strobe RELEASE)
  //     g07 .p4  = TCPBus.07  -> .p2 TSetRun          -> f02 -> SetRun
  //     g07 .p5  = TCPBus.08  -> .p7 (Q')             -> f02 -> SetSS'
  //
  // PARC numbers MSB first, so over the nine CP-bus bits .07 is the low bit of
  // the eight-bit Control byte -- `SetRun = 1` in doradoio.mdefs -- and .08 is
  // the ninth bit, where DoControl's CARRY lands ("LDAI Control^1 / RORA"),
  // i.e. SetSS. So a Control strobe ALREADY carries both, and setting the
  // ports by hand as well was a second, unsynchronised copy of the same state.
  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn; cpout = {data, ss};
      repeat (4) @(posedge sys_clk);
      strb_n = 1'b0; repeat (6) @(posedge sys_clk);
      strb_n = 1'b1;
      if (fn == 3'd0) begin           // Control: g07 clocks on release
        setrun  =  data[0];
        setss_n = ~ss;
      end
      repeat (4) @(posedge sys_clk);
    end
  endtask

  // SetMufflerAddress: twelve bits, MSB first, then the ManClk pulse that
  // DoClock(UseDMD)/DoClock(0) makes. The first bit shifted in ends up at
  // DMD.00, the last at DMD.11.
  integer k;
  task manifold(input [11:0] word);
    begin
      for (k = 11; k >= 0; k = k - 1) begin
        dmd = word[k];
        repeat (4) @(posedge sys_clk); dmc = 1'b1;
        repeat (4) @(posedge sys_clk); dmc = 1'b0;   // the chain shifts on the fall
        repeat (4) @(posedge sys_clk);
      end
      udmd = 1'b1; repeat (12) @(posedge sys_clk);
      udmd = 1'b0; repeat (12) @(posedge sys_clk);
    end
  endtask

  // BMux and RBMux, most significant bit first as PARC numbers them.
  wire [15:0] bmux = {m.BMux_00, m.BMux_01, m.BMux_02, m.BMux_03,
                      m.BMux_04, m.BMux_05, m.BMux_06, m.BMux_07,
                      m.BMux_08, m.BMux_09, m.BMux_10, m.BMux_11,
                      m.BMux_12, m.BMux_13, m.BMux_14, m.BMux_15};
  wire [15:0] rbmux = {m.b_ContB.RBMux_00, m.b_ContB.RBMux_01, m.b_ContB.RBMux_02,
                       m.b_ContB.RBMux_03, m.b_ContB.RBMux_04, m.b_ContB.RBMux_05,
                       m.b_ContB.RBMux_06, m.b_ContB.RBMux_07, m.b_ContB.RBMux_08,
                       m.b_ContB.RBMux_09, m.b_ContB.RBMux_10, m.b_ContB.RBMux_11,
                       m.b_ContB.RBMux_12, m.b_ContB.RBMux_13, m.b_ContB.RBMux_14,
                       m.b_ContB.RBMux_15};

  integer i, hits, hits_a, hits_b;
  reg [15:0] pat_a, pat_b;

  // Link[4:15] -- twelve bits, the whole IM address space, and exactly what
  // cpu.c takes the Write-IM address from (`link_at_issue & 0xFFF`). PARC
  // numbers the register MSB-first, so .04 is the top bit.
  wire [11:0] link_hi = ~{m.b_ContA.Link_04_p_, m.b_ContA.Link_05_p_,
                          m.b_ContA.Link_06_p_, m.b_ContA.Link_07_p_,
                          m.b_ContA.Link_08_p_, m.b_ContA.Link_09_p_,
                          m.b_ContA.Link_10_p_, m.b_ContA.Link_11_p_,
                          m.b_ContA.Link_12_p_, m.b_ContA.Link_13_p_,
                          m.b_ContA.Link_14_p_, m.b_ContA.Link_15_p_};
  wire [11:0] tnia = {m.TNIA_04, m.TNIA_05, m.TNIA_06, m.TNIA_07,
                      m.TNIA_08, m.TNIA_09, m.TNIA_10, m.TNIA_11,
                      m.TNIA_12, m.TNIA_13, m.TNIA_14, m.TNIA_15};
  // dRA' is the address as ContB's multiplexer leaves it; the MC10211 buffers
  // invert it again into the array's RA lines.
  wire [11:0] dra = ~{m.b_ContB.dRA_00_p_, m.b_ContB.dRA_01_p_,
                      m.b_ContB.dRA_02_p_, m.b_ContB.dRA_03_p_,
                      m.b_ContB.dRA_04_p_, m.b_ContB.dRA_05_p_,
                      m.b_ContB.dRA_06_p_, m.b_ContB.dRA_07_p_,
                      m.b_ContB.dRA_08_p_, m.b_ContB.dRA_09_p_,
                      m.b_ContB.dRA_10_p_, m.b_ContB.dRA_11_p_};

  // Jam CPRegToLink# -- PARC's IRTable, `30 13 EF 04 40`.
  task jam_link(input [15:0] v);
    begin
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h04, 1'b0); strobe(3'd7, 8'h40, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (600) @(posedge sys_clk);
    end
  endtask

  task wipe_im;
    begin
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l19.mem[i] = 1'b0;
    end
  endtask

  task count_im(output integer n);
    begin
      n = 0;
      for (i=0;i<1024;i=i+1) begin
        if (m.b_ContB.u_a10.mem[i]) n = n + 1;
        if (m.b_ContB.u_a11.mem[i]) n = n + 1;
        if (m.b_ContB.u_a14.mem[i]) n = n + 1;
        if (m.b_ContB.u_a15.mem[i]) n = n + 1;
        if (m.b_ContB.u_a16.mem[i]) n = n + 1;
        if (m.b_ContB.u_a17.mem[i]) n = n + 1;
        if (m.b_ContB.u_a18.mem[i]) n = n + 1;
        if (m.b_ContB.u_a19.mem[i]) n = n + 1;
        if (m.b_ContB.u_b10.mem[i]) n = n + 1;
        if (m.b_ContB.u_b11.mem[i]) n = n + 1;
        if (m.b_ContB.u_b14.mem[i]) n = n + 1;
        if (m.b_ContB.u_b15.mem[i]) n = n + 1;
        if (m.b_ContB.u_b16.mem[i]) n = n + 1;
        if (m.b_ContB.u_b17.mem[i]) n = n + 1;
        if (m.b_ContB.u_b18.mem[i]) n = n + 1;
        if (m.b_ContB.u_b19.mem[i]) n = n + 1;
        if (m.b_ContB.u_c10.mem[i]) n = n + 1;
        if (m.b_ContB.u_c11.mem[i]) n = n + 1;
        if (m.b_ContB.u_c14.mem[i]) n = n + 1;
        if (m.b_ContB.u_c15.mem[i]) n = n + 1;
        if (m.b_ContB.u_c16.mem[i]) n = n + 1;
        if (m.b_ContB.u_c17.mem[i]) n = n + 1;
        if (m.b_ContB.u_c18.mem[i]) n = n + 1;
        if (m.b_ContB.u_c19.mem[i]) n = n + 1;
        if (m.b_ContB.u_d10.mem[i]) n = n + 1;
        if (m.b_ContB.u_d11.mem[i]) n = n + 1;
        if (m.b_ContB.u_d14.mem[i]) n = n + 1;
        if (m.b_ContB.u_d15.mem[i]) n = n + 1;
        if (m.b_ContB.u_d16.mem[i]) n = n + 1;
        if (m.b_ContB.u_d17.mem[i]) n = n + 1;
        if (m.b_ContB.u_d18.mem[i]) n = n + 1;
        if (m.b_ContB.u_d19.mem[i]) n = n + 1;
        if (m.b_ContB.u_e06.mem[i]) n = n + 1;
        if (m.b_ContB.u_e07.mem[i]) n = n + 1;
        if (m.b_ContB.u_e08.mem[i]) n = n + 1;
        if (m.b_ContB.u_e09.mem[i]) n = n + 1;
        if (m.b_ContB.u_e10.mem[i]) n = n + 1;
        if (m.b_ContB.u_e11.mem[i]) n = n + 1;
        if (m.b_ContB.u_e14.mem[i]) n = n + 1;
        if (m.b_ContB.u_e15.mem[i]) n = n + 1;
        if (m.b_ContB.u_e16.mem[i]) n = n + 1;
        if (m.b_ContB.u_e17.mem[i]) n = n + 1;
        if (m.b_ContB.u_e18.mem[i]) n = n + 1;
        if (m.b_ContB.u_e19.mem[i]) n = n + 1;
        if (m.b_ContB.u_f10.mem[i]) n = n + 1;
        if (m.b_ContB.u_f11.mem[i]) n = n + 1;
        if (m.b_ContB.u_f14.mem[i]) n = n + 1;
        if (m.b_ContB.u_f15.mem[i]) n = n + 1;
        if (m.b_ContB.u_f16.mem[i]) n = n + 1;
        if (m.b_ContB.u_f17.mem[i]) n = n + 1;
        if (m.b_ContB.u_f18.mem[i]) n = n + 1;
        if (m.b_ContB.u_f19.mem[i]) n = n + 1;
        if (m.b_ContB.u_g10.mem[i]) n = n + 1;
        if (m.b_ContB.u_g11.mem[i]) n = n + 1;
        if (m.b_ContB.u_g14.mem[i]) n = n + 1;
        if (m.b_ContB.u_g15.mem[i]) n = n + 1;
        if (m.b_ContB.u_g16.mem[i]) n = n + 1;
        if (m.b_ContB.u_g17.mem[i]) n = n + 1;
        if (m.b_ContB.u_g18.mem[i]) n = n + 1;
        if (m.b_ContB.u_g19.mem[i]) n = n + 1;
        if (m.b_ContB.u_h10.mem[i]) n = n + 1;
        if (m.b_ContB.u_h11.mem[i]) n = n + 1;
        if (m.b_ContB.u_h14.mem[i]) n = n + 1;
        if (m.b_ContB.u_h15.mem[i]) n = n + 1;
        if (m.b_ContB.u_h16.mem[i]) n = n + 1;
        if (m.b_ContB.u_h17.mem[i]) n = n + 1;
        if (m.b_ContB.u_h18.mem[i]) n = n + 1;
        if (m.b_ContB.u_h19.mem[i]) n = n + 1;
        if (m.b_ContB.u_i16.mem[i]) n = n + 1;
        if (m.b_ContB.u_i17.mem[i]) n = n + 1;
        if (m.b_ContB.u_i18.mem[i]) n = n + 1;
        if (m.b_ContB.u_i19.mem[i]) n = n + 1;
      end
    end
  endtask

  // Jam CPRegToIM# (PARC's IRTable, `20 13 EF 03 4F`) with `v` in CPReg.
  task jam_write(input [15:0] v);
    begin
      // Stop first. With the MIR clock held, a running machine repeats the
      // jammed instruction forever, and the next jam has to start from rest.
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);                       // Clock: InhibitCAHolds+ClrReady
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h03, 1'b1); strobe(3'd7, 8'h4F, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (600) @(posedge sys_clk);
    end
  endtask

  integer n0, n1, n2, j, s;
  reg p0, p1, p2;
  always @(posedge sys_clk) begin
    if (m.b_ContA.clk0_p_Ca !== p0) begin n0 = n0 + 1; p0 = m.b_ContA.clk0_p_Ca; end
    if (m.b_ContA.clk1_p_Ca !== p1) begin n1 = n1 + 1; p1 = m.b_ContA.clk1_p_Ca; end
    if (m.b_ContA.clk2_p_Bc !== p2) begin n2 = n2 + 1; p2 = m.b_ContA.clk2_p_Bc; end
  end
  task zero; begin n0 = 0; n1 = 0; n2 = 0; end endtask
  task settle; begin repeat (200) @(posedge sys_clk); end endtask

  // Jam a microinstruction and take the first step: ClrStop+ClrMIR+ClrCT+
  // Freeze, then the four MIR bytes, then SetRun with SS and no ClrStop.
  task jam_step(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      strobe(3'd0, 8'h01, 1'b1);
    end
  endtask

  // One more microinstruction, jam untouched. PARC's three strobes; what the
  // machine needs is the FIRST plus at least one more without ClrStop.
  task step_again;
    begin
      strobe(3'd0, 8'h41, 1'b1);
      strobe(3'd0, 8'h01, 1'b1);
      strobe(3'd0, 8'h00, 1'b1);
    end
  endtask


  reg [11:0] link_seen;
  integer total_written;

  // Bits set at ONE within-bank index, across the right half's packages.
  task scan_one(input integer idx);
    begin
      i = idx;
      hits = 0;
        if (m.b_ContB.u_a10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e06.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e07.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e08.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e09.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i19.mem[i]) hits = hits + 1;
    end
  endtask

  task scan_all;
    begin
      total_written = 0;
      for (i = 0; i < 1024; i = i + 1) begin
        scan_one(i);
        if (hits != 0) begin
          $display("   IM[%0d] (0x%h): %0d right-half bits", i, i[11:0], hits);
          total_written = total_written + hits;
        end
      end
    end
  endtask

  task count_at(input integer addr, output integer n);
    begin scan_one(addr); n = hits; end
  endtask

  // ---- PARC's own routines, transcribed from doradoboot.masm/doradocpint.masm

  // SetCPReg~ : "LDA ToCPRegH / EORI 0ff / ... / LDA ToCPRegL / EORI 0ff".
  // The TILDE IS THE POINT -- the BaseBoard writes the COMPLEMENT, because
  // ContA b02's MC10159 inverts on the way to BMux. Write ~v and the datapath
  // delivers v.
  task set_cpreg_tilde(input [15:0] v);
    begin
      strobe(3'd2, ~v[15:8], 1'b0);
      strobe(3'd3, ~v[7:0],  1'b0);
    end
  endtask

  // DoDoradoMicroInst, single-step variant: DoClock(InhibitCAHolds+ClrReady),
  // Control(ClrStop+ClrMIR+ClrCT+Freeze) SS=0, Control(0) SS=1, the four MIR
  // bytes, Control(SetRun) SS=1, then BasicStopDorado's Control(SetRun) SS=1
  // and Control(0) SS=1.
  task parc_micro(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                  input [7:0] b3, input [7:0] b4);
    begin
      strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      zero;
      // The real BaseBoard is a 1 MHz 6502 running JSR DoControl between
      // these; the gap is MICROSECONDS, not the fourteen sys_clk a testbench
      // takes. GAP models that.
      strobe(3'd0, 8'h01, 1'b1);   // Control(SetRun), SS
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h01, 1'b1);               // BasicStopDorado
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (800) @(posedge sys_clk);
      $display("      micro %02h: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%h FF=%b",
        b1, n0, n1, n2, m.b_ContA.Stop, link_hi,
        ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,m.b_ContA.FF_3_p_,
          m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_});
    end
  endtask

  task nop_micro;
    begin parc_micro(8'h70, 8'h01, 8'h0F, 8'h4C, 8'h40); end
  endtask

  // SendViaMIR: the address into Link (with a Nop after it, which "holds CPReg
  // constant through T3 of the previous instruction"), then the data and the
  // IMStoreTable entry for this half.
  //   IMStoreTable[0] IMLHRSTK.0Is0#   60 33 EF 03 4F
  //   IMStoreTable[1] IMLHRSTK.0Is1#   20 73 EF 03 4F
  //   IMStoreTable[2] IMRHBLOCKIs0#    20 13 EF 03 4F
  //   IMStoreTable[3] IMRHBLOCKIs1#    60 53 EF 03 4F
  task send_via_mir(input [15:0] imaddr, input [15:0] word,
                    input right_half, input extra);
    begin
      set_cpreg_tilde(imaddr);
      parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
      nop_micro;                                        // ...AndNop
      // The address is in Link HERE. The store below carries FF=176 too and
      // reloads Link with its own DATA -- the link_at_issue hazard -- so this
      // is the only moment it can be checked.
      link_seen = link_hi;
      if (link_seen !== imaddr[11:0])
        $fatal(1, "CPRegToLink# did not put the address in Link");
      set_cpreg_tilde(word);
      if (!right_half && !extra) parc_micro(8'h60, 8'h33, 8'hEF, 8'h03, 8'h4F);
      if (!right_half &&  extra) parc_micro(8'h20, 8'h73, 8'hEF, 8'h03, 8'h4F);
      if ( right_half && !extra) parc_micro(8'h20, 8'h13, 8'hEF, 8'h03, 8'h4F);
      if ( right_half &&  extra) parc_micro(8'h60, 8'h53, 8'hEF, 8'h03, 8'h4F);
    end
  endtask


  function [15:0] rd_L0(input integer idx);
    rd_L0 = {m.b_ContB.u_j06.mem[idx], m.b_ContB.u_k06.mem[idx], m.b_ContB.u_l06.mem[idx], m.b_ContB.u_i10.mem[idx], m.b_ContB.u_j10.mem[idx], m.b_ContB.u_k10.mem[idx], m.b_ContB.u_l10.mem[idx], m.b_ContB.u_f06.mem[idx], m.b_ContB.u_g06.mem[idx], m.b_ContB.u_h06.mem[idx], m.b_ContB.u_j16.mem[idx], m.b_ContB.u_k16.mem[idx], m.b_ContB.u_l16.mem[idx], m.b_ContB.u_a06.mem[idx], m.b_ContB.u_b06.mem[idx], m.b_ContB.u_c06.mem[idx]};
  endfunction
  function [15:0] rd_R0(input integer idx);
    rd_R0 = {m.b_ContB.u_a10.mem[idx], m.b_ContB.u_b10.mem[idx], m.b_ContB.u_c10.mem[idx], m.b_ContB.u_d10.mem[idx], m.b_ContB.u_a16.mem[idx], m.b_ContB.u_b16.mem[idx], m.b_ContB.u_c16.mem[idx], m.b_ContB.u_d16.mem[idx], m.b_ContB.u_e06.mem[idx], m.b_ContB.u_e10.mem[idx], m.b_ContB.u_f10.mem[idx], m.b_ContB.u_g10.mem[idx], m.b_ContB.u_h10.mem[idx], m.b_ContB.u_g16.mem[idx], m.b_ContB.u_h16.mem[idx], m.b_ContB.u_i16.mem[idx]};
  endfunction
  function [15:0] rd_L1(input integer idx);
    rd_L1 = {m.b_ContB.u_j07.mem[idx], m.b_ContB.u_k07.mem[idx], m.b_ContB.u_l07.mem[idx], m.b_ContB.u_i11.mem[idx], m.b_ContB.u_j11.mem[idx], m.b_ContB.u_k11.mem[idx], m.b_ContB.u_l11.mem[idx], m.b_ContB.u_f07.mem[idx], m.b_ContB.u_g07.mem[idx], m.b_ContB.u_h07.mem[idx], m.b_ContB.u_j17.mem[idx], m.b_ContB.u_k17.mem[idx], m.b_ContB.u_l17.mem[idx], m.b_ContB.u_a07.mem[idx], m.b_ContB.u_b07.mem[idx], m.b_ContB.u_c07.mem[idx]};
  endfunction
  function [15:0] rd_R1(input integer idx);
    rd_R1 = {m.b_ContB.u_a11.mem[idx], m.b_ContB.u_b11.mem[idx], m.b_ContB.u_c11.mem[idx], m.b_ContB.u_d11.mem[idx], m.b_ContB.u_a17.mem[idx], m.b_ContB.u_b17.mem[idx], m.b_ContB.u_c17.mem[idx], m.b_ContB.u_d17.mem[idx], m.b_ContB.u_e07.mem[idx], m.b_ContB.u_e11.mem[idx], m.b_ContB.u_f11.mem[idx], m.b_ContB.u_g11.mem[idx], m.b_ContB.u_h11.mem[idx], m.b_ContB.u_g17.mem[idx], m.b_ContB.u_h17.mem[idx], m.b_ContB.u_i17.mem[idx]};
  endfunction
  function [15:0] rd_L2(input integer idx);
    rd_L2 = {m.b_ContB.u_j08.mem[idx], m.b_ContB.u_k08.mem[idx], m.b_ContB.u_l08.mem[idx], m.b_ContB.u_i14.mem[idx], m.b_ContB.u_j14.mem[idx], m.b_ContB.u_k14.mem[idx], m.b_ContB.u_l14.mem[idx], m.b_ContB.u_f08.mem[idx], m.b_ContB.u_g08.mem[idx], m.b_ContB.u_h08.mem[idx], m.b_ContB.u_j18.mem[idx], m.b_ContB.u_k18.mem[idx], m.b_ContB.u_l18.mem[idx], m.b_ContB.u_a08.mem[idx], m.b_ContB.u_b08.mem[idx], m.b_ContB.u_c08.mem[idx]};
  endfunction
  function [15:0] rd_R2(input integer idx);
    rd_R2 = {m.b_ContB.u_a14.mem[idx], m.b_ContB.u_b14.mem[idx], m.b_ContB.u_c14.mem[idx], m.b_ContB.u_d14.mem[idx], m.b_ContB.u_a18.mem[idx], m.b_ContB.u_b18.mem[idx], m.b_ContB.u_c18.mem[idx], m.b_ContB.u_d18.mem[idx], m.b_ContB.u_e08.mem[idx], m.b_ContB.u_e14.mem[idx], m.b_ContB.u_f14.mem[idx], m.b_ContB.u_g14.mem[idx], m.b_ContB.u_h14.mem[idx], m.b_ContB.u_g18.mem[idx], m.b_ContB.u_h18.mem[idx], m.b_ContB.u_i18.mem[idx]};
  endfunction
  function [15:0] rd_L3(input integer idx);
    rd_L3 = {m.b_ContB.u_j09.mem[idx], m.b_ContB.u_k09.mem[idx], m.b_ContB.u_l09.mem[idx], m.b_ContB.u_i15.mem[idx], m.b_ContB.u_j15.mem[idx], m.b_ContB.u_k15.mem[idx], m.b_ContB.u_l15.mem[idx], m.b_ContB.u_f09.mem[idx], m.b_ContB.u_g09.mem[idx], m.b_ContB.u_h09.mem[idx], m.b_ContB.u_j19.mem[idx], m.b_ContB.u_k19.mem[idx], m.b_ContB.u_l19.mem[idx], m.b_ContB.u_a09.mem[idx], m.b_ContB.u_b09.mem[idx], m.b_ContB.u_c09.mem[idx]};
  endfunction
  function [15:0] rd_R3(input integer idx);
    rd_R3 = {m.b_ContB.u_a15.mem[idx], m.b_ContB.u_b15.mem[idx], m.b_ContB.u_c15.mem[idx], m.b_ContB.u_d15.mem[idx], m.b_ContB.u_a19.mem[idx], m.b_ContB.u_b19.mem[idx], m.b_ContB.u_c19.mem[idx], m.b_ContB.u_d19.mem[idx], m.b_ContB.u_e09.mem[idx], m.b_ContB.u_e15.mem[idx], m.b_ContB.u_f15.mem[idx], m.b_ContB.u_g15.mem[idx], m.b_ContB.u_h15.mem[idx], m.b_ContB.u_g19.mem[idx], m.b_ContB.u_h19.mem[idx], m.b_ContB.u_i19.mem[idx]};
  endfunction

  function sec_L0(input integer idx);
    sec_L0 = m.b_ContB.u_i06.mem[idx];
  endfunction
  function sec_R0(input integer idx);
    sec_R0 = m.b_ContB.u_f16.mem[idx];
  endfunction
  function sec_L1(input integer idx);
    sec_L1 = m.b_ContB.u_i07.mem[idx];
  endfunction
  function sec_R1(input integer idx);
    sec_R1 = m.b_ContB.u_f17.mem[idx];
  endfunction
  function sec_L2(input integer idx);
    sec_L2 = m.b_ContB.u_i08.mem[idx];
  endfunction
  function sec_R2(input integer idx);
    sec_R2 = m.b_ContB.u_f18.mem[idx];
  endfunction
  function sec_L3(input integer idx);
    sec_L3 = m.b_ContB.u_i09.mem[idx];
  endfunction
  function sec_R3(input integer idx);
    sec_R3 = m.b_ContB.u_f19.mem[idx];
  endfunction

  // The SECONDARY bit of a half -- RSTK[0] on the left, BLOCK on the right --
  // which the array stores beside the sixteen data bits. Checking only the
  // data leaves it unverified: a mutation that reversed the ExtraBits shift
  // passed until this was added.
  function sec_at(input integer bank, input right, input integer idx);
    begin
      sec_at = 1'bx;
      if (!right) begin
        case (bank)
          0: sec_at = sec_L0(idx); 1: sec_at = sec_L1(idx);
          2: sec_at = sec_L2(idx); 3: sec_at = sec_L3(idx);
        endcase
      end else begin
        case (bank)
          0: sec_at = sec_R0(idx); 1: sec_at = sec_R1(idx);
          2: sec_at = sec_R2(idx); 3: sec_at = sec_R3(idx);
        endcase
      end
    end
  endfunction

  // Search every bank for a word at one index. Returns the bank, or -1.
  function integer find_word(input integer idx, input right, input [15:0] want);
    begin
      find_word = -1;
      if (!right) begin
        if (rd_L0(idx) === want) find_word = 0;
        else if (rd_L1(idx) === want) find_word = 1;
        else if (rd_L2(idx) === want) find_word = 2;
        else if (rd_L3(idx) === want) find_word = 3;
      end else begin
        if (rd_R0(idx) === want) find_word = 0;
        else if (rd_R1(idx) === want) find_word = 1;
        else if (rd_R2(idx) === want) find_word = 2;
        else if (rd_R3(idx) === want) find_word = 3;
      end
    end
  endfunction

  // ---- SendAHunk ----------------------------------------------------------
  reg [7:0]  hunk [0:16];
  reg [7:0]  extrabits;
  integer    yy, kk2;
  reg [15:0] imaddr;

  task send_a_hunk(input [15:0] start_addr);
    begin
      extrabits = hunk[0];
      yy = 1;
      imaddr = start_addr;
      for (kk2 = 0; kk2 < 4; kk2 = kk2 + 1) begin
        // left half, then right, both at the SAME IMAddress
        send_via_mir(imaddr, {hunk[yy], hunk[yy+1]}, 1'b0, extrabits[7]);
        extrabits = {extrabits[6:0], 1'b0}; yy = yy + 2;
        send_via_mir(imaddr, {hunk[yy], hunk[yy+1]}, 1'b1, extrabits[7]);
        extrabits = {extrabits[6:0], 1'b0}; yy = yy + 2;
        imaddr = imaddr + 16'd1;
      end
    end
  endtask

  integer bnk, nfound, w;
  reg [15:0] expect_l [0:3];
  reg [15:0] expect_r [0:3];

  // ---- REAL MICROCODE, from the C emulator's own .MB loader ---------------
  integer fd, nf, hunks, halves, bad;
  string  path, tag, line;
  integer ha;
  reg [7:0]  b [0:16];
  reg [15:0] hw [0:7];

  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (2000) @(posedge sys_clk);
    manifold(12'h030);
    manifold(12'h1E0);
    if (m.StopMIRClk !== 1'b1) $fatal(1, "the MIR clock is not held");
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca; p2 = m.b_ContA.clk2_p_Bc;
    zero;
    wipe_im;
    nop_micro; nop_micro;      // warm the pipeline; see tb_sendmir

    // One hunk: ExtraBits then four instructions, left half first.
    // EVERY WORD IS BYTE-ASYMMETRIC on purpose. The first version of this test
    // used 1111/2222/..., and the mutation that swaps the two data bytes
    // PASSED against it: with equal halves there is nothing to swap. PARC
    // sends the HIGH byte first ("LDA@Y Hunk / STA ToCPRegH"), and that is
    // only tested if the two bytes differ.
    hunk[0]  = 8'h00;
    hunk[1]  = 8'h12; hunk[2]  = 8'h34;   // instr 0 left  = 1234
    hunk[3]  = 8'h56; hunk[4]  = 8'h78;   // instr 0 right = 5678
    hunk[5]  = 8'h9A; hunk[6]  = 8'hBC;   // instr 1 left  = 9abc
    hunk[7]  = 8'hDE; hunk[8]  = 8'hF0;   // instr 1 right = def0
    hunk[9]  = 8'h0F; hunk[10] = 8'h1E;   // instr 2 left  = 0f1e
    hunk[11] = 8'h2D; hunk[12] = 8'h3C;   // instr 2 right = 2d3c
    hunk[13] = 8'h4B; hunk[14] = 8'h5A;   // instr 3 left  = 4b5a
    hunk[15] = 8'h69; hunk[16] = 8'h78;   // instr 3 right = 6978
    expect_l[0] = 16'h1234; expect_r[0] = 16'h5678;
    expect_l[1] = 16'h9ABC; expect_r[1] = 16'hDEF0;
    expect_l[2] = 16'h0F1E; expect_r[2] = 16'h2D3C;
    expect_l[3] = 16'h4B5A; expect_r[3] = 16'h6978;

    send_a_hunk(16'h0100);
    nop_micro; nop_micro;      // let the last store's T3 complete

    nfound = 0;
    for (w = 0; w < 4; w = w + 1) begin
      bnk = find_word((16'h0100 + w) >> 1, 1'b0, expect_l[w]);
      $display("tb_boot0: IM[0x%h] left  = %h -> %s",
               16'h0100 + w, expect_l[w],
               (bnk < 0) ? "NOT FOUND" : "found");
      if (bnk >= 0) nfound = nfound + 1;
      bnk = find_word((16'h0100 + w) >> 1, 1'b1, expect_r[w]);
      $display("tb_boot0: IM[0x%h] right = %h -> %s",
               16'h0100 + w, expect_r[w],
               (bnk < 0) ? "NOT FOUND" : "found");
      if (bnk >= 0) nfound = nfound + 1;
    end

    $display("tb_boot0: %0d of 8 half-words read back correctly", nfound);
    if (nfound !== 8)
      $fatal(1, "the synthetic hunk did not land in IM word for word");

    // ---- and now REAL MICROCODE -----------------------------------------
    //
    // dorado/tests/boot0_hunks.c packs Xerox's own AEmu.mb into PARC's hunk
    // format through the C emulator's .MB loader, and prints the half-words
    // the array should end up holding. The two sides share no code: one is
    // mb.c and microcode.c, the other is 4,096 words of modelled ECL RAM
    // reached through the BaseBoard's control-processor bus.
    if (!$value$plusargs("vectors=%s", path)) path = "boot0.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $fatal(1, "tb_boot0: cannot open %s", path);

    hunks = 0; halves = 0; bad = 0;
    while (!$feof(fd)) begin
      void'($fgets(line, fd));
      nf = $sscanf(line,
        "%s %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h",
        tag, ha, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8],
        b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16],
        hw[0], hw[1], hw[2], hw[3], hw[4], hw[5], hw[6], hw[7]);
      if (nf != 27 || tag != "HUNK") continue;

      wipe_im;
      nop_micro; nop_micro;              // warm the pipeline
      for (w = 0; w < 17; w = w + 1) hunk[w] = b[w];
      send_a_hunk(ha[15:0]);
      nop_micro; nop_micro;              // let the last store's T3 complete

      hunks = hunks + 1;
      for (w = 0; w < 4; w = w + 1) begin
        halves = halves + 2;
        // b[0] IS the eight secondary bits, MSB first, one per half -- so the
        // ExtraBits byte checks itself against what the array stored.
        bnk = find_word((ha + w) >> 1, 1'b0, hw[2*w]);
        if (bnk < 0) begin
          bad = bad + 1;
          $display("tb_boot0: IM[0x%h] left  want %h -- NOT FOUND", ha + w, hw[2*w]);
        end else if (sec_at(bnk, 1'b0, (ha + w) >> 1) !== b[0][7 - 2*w]) begin
          bad = bad + 1;
          $display("tb_boot0: IM[0x%h] left  secondary bit wrong", ha + w);
        end
        bnk = find_word((ha + w) >> 1, 1'b1, hw[2*w+1]);
        if (bnk < 0) begin
          bad = bad + 1;
          $display("tb_boot0: IM[0x%h] right want %h -- NOT FOUND", ha + w, hw[2*w+1]);
        // COMPLEMENTED, and only on the right. The right half's secondary is
        // BLOCK, and BLOCK is the one field bit the MIR wires inverted --
        // `sBLOCK` to R, `rMIRa` to S, `Block` off Q' -- so the array holds
        // `dBlock'`. Measured: all 64 left secondary bits matched and all 64
        // right ones were the complement, which is a systematic inversion and
        // not noise. See docs/verilog-handoff.md, "BLOCK is wired INVERTED"
        // and "dBlock' comes out of IM COMPLEMENTED".
        end else if (sec_at(bnk, 1'b1, (ha + w) >> 1) === b[0][6 - 2*w]) begin
          bad = bad + 1;
          $display("tb_boot0: IM[0x%h] right secondary bit wrong", ha + w);
        end
      end
    end
    $fclose(fd);

    $display("tb_boot0: REAL MICROCODE -- %0d hunks, %0d half-words, %0d wrong",
             hunks, halves, bad);
    if (hunks == 0) $fatal(1, "no hunks in the vector file");
    if (bad != 0)   $fatal(1, "IM does not match the C emulator's .MB loader");
    $display("tb_boot0: PARC's block loader walks real microcode into IM, and it matches the C emulator.");
    $finish;
  end

endmodule

`default_nettype wire
